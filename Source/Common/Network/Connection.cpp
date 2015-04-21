#include "NetworkPCH.h"
#include "Connection.h"

#if defined(__GNUC__)

#include <netinet/tcp.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>

#elif defined(__OSX__)

#include <netinet/tcp.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>

#else

#include <winsock2.h>
#include <Windows.h>
#include <MSTcpIP.h>

#endif

namespace Network
{
#ifdef _DEBUG
	long Connection::msSendCnt = 0;
	long Connection::msRecvCnt = 0;
#endif // _DEBUG

	Connection::Connection(boost::asio::io_service& io_service,
		TcpNetworkParams* pkParams)
		: mpkTcpNetworkParams(pkParams),
		mbClosed(true),
		mbClosing(true),
		mbShouldBeClosed(false),
		mbSendIOReqRelay(false),
		mbRecvIOReqRelay(false),
		mpkExtraSendBuffPool(0),
		mbServerSide(true),
		mbNeedCallCloseCallback(true)
	{
		mpSendBuffer = new RingBuffer<unsigned char>(
			mpkTcpNetworkParams->uiSendBuffSize, 
			mpkTcpNetworkParams->uiMaxConnectionWriteSize);
		mpRecvBuffer = new RingBuffer<unsigned char>(
			mpkTcpNetworkParams->uiRecvBuffSize, 
			mpkTcpNetworkParams->uiMaxConnectionWriteSize);

		if (!mpkTcpNetworkParams->bCloseOnBufferOverflow)
		{
			mpkExtraSendBuffPool = new UCExSendBufferPool(
				mpkTcpNetworkParams->uiExtraBufferCount, 
				mpkTcpNetworkParams->uiMaxConnectionWriteSize);
		}
	}

	Connection::~Connection()
	{
		delete mpSendBuffer;
		delete mpRecvBuffer;
		if (mpkExtraSendBuffPool)
		{
			delete mpkExtraSendBuffPool;
			mpkExtraSendBuffPool = 0;
		}
	}
	bool Connection::write(unsigned char* acBuff, unsigned int uiSize)
	{
		///// Test
		///// @{
		//checkDataA2Z((char*)acBuff, uiSize);
		///// @}

		if (shouldBeClosed() || isClosed() || uiSize > mpkTcpNetworkParams->uiMaxConnectionWriteSize)
		{
			return false;
		}
		if (isDataInExtraSendBuff())
		{
			pushToExtraSendBuff(acBuff, uiSize);
			writeExtraSendBuff();
		}
		else
		{
			if (!mpSendBuffer->writeImmediately(acBuff, uiSize))
			{
				if (mpkTcpNetworkParams->bCloseOnBufferOverflow)
				{
					/// ���ͻ����������ֱ�ӶϿ�
					asyncClose();
					boost::system::error_code kErrorCode = Network::send_buff_overflow;
					pushErrorCode(kErrorCode);
					return false;
				}
				pushToExtraSendBuff(acBuff, uiSize);
			}
		}
		/// ���IO������������Ͽ���������û�йر��������IO��������
		if (!mbSendIOReqRelay && !isClosed())
		{
			send(true);
		}
		return true;
	}
	unsigned int Connection::read(unsigned char* acBuff, unsigned int uiBuffSize)
	{
		/// ���IO������������Ͽ���������û�йر��������IO��������
		if (!mbRecvIOReqRelay && !isClosed())
		{
			recv(true);
		}
		return mpRecvBuffer->readImmediately(acBuff, uiBuffSize);
	}
	bool Connection::readPacketsBegin(unsigned char*& acBuff, unsigned int& uiReadableSize)
	{
		/// ���IO������������Ͽ���������û�йر��������IO��������
		if (!mbRecvIOReqRelay && !isClosed())
		{
			recv(true);
		}
		return mpRecvBuffer->readPacketsBegin(acBuff, uiReadableSize);
	}
	void Connection::readPacketsEnd(unsigned int uiProcessedSize)
	{
		mpRecvBuffer->readPacketEnd(uiProcessedSize);
	}
	bool Connection::isDataInExtraSendBuff()
	{
		if (mpkExtraSendBuffPool)
		{
			return mpkExtraSendBuffPool->hasData();
		}
		return false;
	}
	void Connection::pushToExtraSendBuff(unsigned char* pcBuff, unsigned int uiSize)
	{
		mpkExtraSendBuffPool->pushData(pcBuff, uiSize);
		///// Test
		///// @{
		//checkDataA2Z((char*)pkBuff->pBuffer, uiSize);
		///// @}
	}
	void Connection::writeExtraSendBuff()
	{
		unsigned char* pcBuff = 0;
		unsigned int uiSize = 0;
		mpkExtraSendBuffPool->getData(pcBuff, uiSize);
		while (pcBuff && uiSize > 0 && 
			mpSendBuffer->writeImmediately(pcBuff, uiSize))
		{
			///// Test
			///// @{
			//checkDataA2Z((char*)pcBuff, uiSize);
			///// @}

			mpkExtraSendBuffPool->popData();
			mpkExtraSendBuffPool->getData(pcBuff, uiSize);
		}
	}
	void Connection::trySendExtraSendBuff()
	{
		/// ֻ�е�������������رղ��п����в��ܼ�ʱ���͵����
		if (mpkTcpNetworkParams->bCloseOnBufferOverflow)
		{
			return;
		}
		/// ���IO������������Ͽ���������û�йر��������IO��������
		if (!mbSendIOReqRelay && !isClosed())
		{
			writeExtraSendBuff();
			send(true);
		}
	}
	bool Connection::setKeepAlive()
	{
		if (!mpkTcpNetworkParams || !mpkTcpNetworkParams->bKeepAlive)
		{
			return true;
		}
#ifdef __GNUC__

		// For *n*x systems
		int native_fd = getSocket().native();
		int timeout = mpkTcpNetworkParams->ulKeepAliveTime;
		int intvl = mpkTcpNetworkParams->ulKeepAliveInterval;
		int probes = 5;///< ���?ʧ��keep alive������
		int on = mpkTcpNetworkParams->bKeepAlive ? 1 : 0;

		int ret_keepalive = setsockopt(native_fd, SOL_SOCKET, SO_KEEPALIVE, (void*) &on, sizeof(int));
		int ret_keepidle = setsockopt(native_fd, SOL_TCP, TCP_KEEPIDLE, (void*) &timeout, sizeof(int));
		int ret_keepintvl = setsockopt(native_fd, SOL_TCP, TCP_KEEPINTVL, (void*) &intvl, sizeof(int));
		int ret_keepinit = setsockopt(native_fd, SOL_TCP, TCP_KEEPCNT, (void*) &probes, sizeof(int));

		if(ret_keepalive || ret_keepidle || ret_keepintvl || ret_keepinit)
		{
			/// Failed to enable keep alive on TCP client socket!
			boost::system::error_code kErrorCode = Network::set_keep_alive_option_failed;
			pushErrorCode(kErrorCode);
			return false;
		}

#elif defined(__OSX__)

		int native_fd = getSocket().native();
		int timeout = mpkTcpNetworkParams->ulKeepAliveTime;
		int intvl = mpkTcpNetworkParams->ulKeepAliveInterval;
		int on = mpkTcpNetworkParams->bKeepAlive ? 1 : 0;

		/// Set the timeout before the first keep alive message
		int ret_sokeepalive = setsockopt(native_fd, SOL_SOCKET, SO_KEEPALIVE, (void*) &on, sizeof(int));
		int ret_tcpkeepalive = setsockopt(native_fd, IPPROTO_TCP, TCP_KEEPALIVE, (void*) &timeout, sizeof(int));
		int ret_tcpkeepintvl = setsockopt(native_fd, IPPROTO_TCP, TCP_CONNECTIONTIMEOUT, (void*) &intvl, sizeof(int));

		if(ret_sokeepalive || ret_tcpkeepalive || ret_tcpkeepintvl)
		{
			/// Failed to enable keep alive on TCP client socket!
			boost::system::error_code kErrorCode = Network::set_keep_alive_option_failed;
			pushErrorCode(kErrorCode);
			return false;
		}

#else
		/// http://msdn.microsoft.com/en-us/library/dd877220(VS.85).aspx
		/// Partially supported on windows
		/// On Windows Vista and later, the number of keep-alive probes (data retransmissions) is set to 10 and cannot be changed.
		/// On Windows Server 2003, Windows XP, and Windows 2000, the default setting for number of keep-alive probes is 5.
		/// The number of keep-alive probes is controllable through the TcpMaxDataRetransmissions and PPTPTcpMaxDataRetransmissions registry settings. 
		/// The number of keep-alive probes is set to the larger of the two registry key values. 
		/// If this number is 0, then keep-alive probes will not be sent. If this number is above 255, then it is adjusted to 255.
		struct tcp_keepalive keepalive_options;
		keepalive_options.onoff = mpkTcpNetworkParams->bKeepAlive ? 1 : 0;
		keepalive_options.keepalivetime = mpkTcpNetworkParams->ulKeepAliveTime;
		keepalive_options.keepaliveinterval = mpkTcpNetworkParams->ulKeepAliveInterval;

		BOOL keepalive_val = mpkTcpNetworkParams->bKeepAlive ? 1 : 0;
		SOCKET native = getSocket().native();
		DWORD bytes_returned;

		int ret_keepalive = setsockopt(native, SOL_SOCKET, SO_KEEPALIVE, (const char *)&keepalive_val, sizeof(keepalive_val));
		int ret_iotcl = WSAIoctl(native, SIO_KEEPALIVE_VALS, (LPVOID)&keepalive_options, (DWORD) sizeof(keepalive_options), NULL, 0,
			(LPDWORD)&bytes_returned, NULL, NULL);

		if (ret_keepalive || ret_iotcl)
		{
			/// Failed to set keep alive timeout on TCP client socket!
			boost::system::error_code kErrorCode = Network::set_keep_alive_option_failed;
			pushErrorCode(kErrorCode);
			return false;
		}
#endif
		return true;
	}
}
