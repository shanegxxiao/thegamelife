#include "NetworkPCH.h"
#include "SSLConnection.h"

namespace Network
{
	SSLConnection::SSLConnection(boost::asio::io_service& io_service,
		TcpNetworkParams* pkParams, boost::asio::ssl::context *pkSSLContext)
		: Connection(io_service, pkParams), 
		  mkIOService(io_service),
		  mpkSSLContext(pkSSLContext)
	{
		mpkSSLSocket = new SSLSocket(io_service, *pkSSLContext);
		mpkStrand = new boost::asio::strand(io_service);
	}

	SSLConnection::~SSLConnection()
	{
		if (mpkSSLSocket)
		{
			delete mpkSSLSocket;
			mpkSSLSocket = NULL;
		}

		if (mpkStrand)
		{
			delete mpkStrand;
			mpkStrand = NULL;
		}
	}
	void SSLConnection::connect(boost::asio::ip::tcp::endpoint& kEndpoint, 
		boost::system::error_code& kErrorCode)
	{
		setAsClientSide();
		mpkSSLSocket->lowest_layer().connect(kEndpoint, kErrorCode);
	}
	void SSLConnection::asyncConnect(boost::asio::ip::tcp::endpoint& kEndpoint,
		OnConnectFunc pfnOnConnect)
	{
		setAsClientSide();
		mpkSSLSocket->lowest_layer().async_connect(kEndpoint, pfnOnConnect);
	}
	void SSLConnection::handShake(boost::system::error_code& kErrorCode)
	{
		if (isServerSide())
		{
			mpkSSLSocket->handshake(boost::asio::ssl::stream_base::server, kErrorCode);
		}
		else
		{
			mpkSSLSocket->handshake(boost::asio::ssl::stream_base::client, kErrorCode);
		}
	}
	void SSLConnection::asyncHandShake(OnHandShakeFunc pfnOnHandShakeFunc)
	{
		if (isServerSide())
		{
			mpkSSLSocket->async_handshake(boost::asio::ssl::stream_base::server, pfnOnHandShakeFunc);
		}
		else
		{
			mpkSSLSocket->async_handshake(boost::asio::ssl::stream_base::client, pfnOnHandShakeFunc);
		}
	}
	bool SSLConnection::open()
	{
		clearErrorCodes();
		if (!mpkSSLSocket->lowest_layer().is_open() || !isClosed())
		{
			boost::system::error_code kErrorCode = Network::accepted_but_failed_when_open;
			pushErrorCode(kErrorCode);
			return false;
		}
		if (mpkExtraSendBuffPool)
		{
			mpkExtraSendBuffPool->resetPool();
		}
		mbClosed = false;
		mbClosing = false;
		mbShouldBeClosed = false;
		mbNeedCallCloseCallback = true;
		mpRecvBuffer->clear();
		mpSendBuffer->clear();
		if (!setKeepAlive())
		{
			return false;
		}
		recv(true);
		return true;
	}
	void SSLConnection::close()
	{
		if (mbClosed)
		{
			return;
		}
		markShouldBeClosed();
		if (!mkClosingMutex.try_lock())
		{
			return;
		}
		mbClosing = true;
		boost::system::error_code kErrorCode;
		if (mpkSSLSocket->lowest_layer().is_open())
		{
			boost::system::error_code kErrorCode;
			mpkSSLSocket->next_layer().cancel(kErrorCode);
			kErrorCode ? pushErrorCode(kErrorCode) : kErrorCode.clear();
			mpkSSLSocket->next_layer().shutdown(boost::asio::socket_base::shutdown_both, kErrorCode);
			kErrorCode ? pushErrorCode(kErrorCode) : kErrorCode.clear();
			mpkSSLSocket->next_layer().close(kErrorCode);
			kErrorCode ? pushErrorCode(kErrorCode) : kErrorCode.clear();
			mpkStrand->post(mpkStrand->wrap(
				boost::bind(&Connection::closeWrapper, shared_from_this())));
		}
		else
		{
			handleClose(kErrorCode);
		}
	}
	void SSLConnection::closeWrapper()
	{
		mpkSSLSocket->async_shutdown(mpkStrand->wrap(
			boost::bind(&Connection::handleClose, shared_from_this(), 
			boost::asio::placeholders::error)));
		//boost::system::error_code kErrorCode;
		//mpkSSLSocket->shutdown(kErrorCode);
		//kErrorCode ? pushErrorCode(kErrorCode) : kErrorCode.clear();
		//mpkSSLSocket->lowest_layer().cancel(kErrorCode);
		//kErrorCode ? pushErrorCode(kErrorCode) : kErrorCode.clear();
		//mpkSSLSocket->lowest_layer().shutdown(boost::asio::socket_base::shutdown_both, kErrorCode);
		//kErrorCode ? pushErrorCode(kErrorCode) : kErrorCode.clear();
		//mpkSSLSocket->lowest_layer().close(kErrorCode);
		//kErrorCode ? pushErrorCode(kErrorCode) : kErrorCode.clear();
		//handleClose(kErrorCode);
	}
	void SSLConnection::asyncClose()
	{
		markShouldBeClosed();
	}

	void SSLConnection::handleClose(const boost::system::error_code& kErrorCode)
	{
		if (kErrorCode)
		{
			pushErrorCode(kErrorCode);
		}
		mbClosed = true;
		mkClosingMutex.unlock();
	}
	void SSLConnection::send(bool bCallAtMainThread)
	{
		/// 只有在主线程才需要检查是否有发送正在进行，
		/// mbSendIOReqRelay只有在@see SSLConnection::handleSent返回且没有发起新的发送
		/// 请求、以及@see SSLConnection::send函数执行时没有数据可以发送，这样
		/// 的两种情况下才会将mbSendIOReqRelay改为false，所以只有主线程调用@see SSLConnection::send
		/// 时需要检查mbSendIOReqRelay是否为true，是则直接返回
		if (isClosed() || (bCallAtMainThread && mbSendIOReqRelay))
		{
			return;
		}
		unsigned char *acBuff = 0;
		unsigned int uiReadableSize = 0;
		if (mpSendBuffer->readBegin(acBuff, uiReadableSize))
		{
			if (uiReadableSize > 0)
			{
				if (!mkClosingMutex.try_lock())
				{
					mpSendBuffer->readEnd(0);
					mbSendIOReqRelay = false;
					return;
				}
				mbSendIOReqRelay = true;
				mpkStrand->post(mpkStrand->wrap(
					boost::bind(&Connection::asyncWriteSome,
					shared_from_this(), acBuff, uiReadableSize)));
				mkClosingMutex.unlock();
			}
			else
			{
				mpSendBuffer->readEnd(0);
				mbSendIOReqRelay = false;
			}
		}
	}
	void SSLConnection::recv(bool bCallAtMainThread)
	{
		/// 只有在主线才需要检查是否有接收正在进行，
		/// mbRecvIOReqRelay只有在@see SSLConnection::handleRecvd返回且没有发起新的接收
		/// 请求@see SSLConnection::recv、以及@see SSLConnection::recv函数执行时没有发起接收请求，这样
		/// 的两种情况下才会将mbRecvIOReqRelay改为false，所以只有主线程调用@see SSLConnection::recv
		/// 时需要检查mbRecvIOReqRelay是否为true，是则直接返回
		if (bCallAtMainThread && mbRecvIOReqRelay)
		{
			return;
		}
		if (isClosed() || shouldBeClosed())
		{
			mbRecvIOReqRelay = false;
			return;
		}
		unsigned char *acBuff = 0;
		unsigned int uiWritableSize = 0;
		if (mpRecvBuffer->writeBegin(acBuff, uiWritableSize))
		{
			if (uiWritableSize > 0)
			{
				if (!mkClosingMutex.try_lock())
				{
					mpRecvBuffer->writeEnd(0);
					mbRecvIOReqRelay = false;
					return;
				}
				mbRecvIOReqRelay = true;
				mpkStrand->post(mpkStrand->wrap(
					boost::bind(&Connection::asyncReadSome,
					shared_from_this(), acBuff, uiWritableSize)));
				mkClosingMutex.unlock();
				return;
			}
			else
			{
				mpRecvBuffer->writeEnd(0);
				if (mpkTcpNetworkParams->bCloseOnBufferOverflow)
				{
					/// 接收缓冲区溢出，直接断开
					asyncClose();
					boost::system::error_code kErrorCode = Network::recv_buff_overflow;
					pushErrorCode(kErrorCode);
				}
			}
		}
		mbRecvIOReqRelay = false;
	}
	void SSLConnection::handleSent(const boost::system::error_code& kErrorCode,
								   size_t uBytesTransferred)
	{
	#ifdef _DEBUG
		BOOST_INTERLOCKED_EXCHANGE_ADD(&msSendCnt, uBytesTransferred);
	#endif // _DEBUG

		mpSendBuffer->readEnd(uBytesTransferred);
		if (kErrorCode)
		{
			pushErrorCode(kErrorCode);
			asyncClose();
			mbSendIOReqRelay = false;
			return;
		}
		if (uBytesTransferred == 0)
		{
			asyncClose();
			mbSendIOReqRelay = false;
			return;
		}
		if (isClosed() || mpSendBuffer->readableSize() == 0)
		{
			mbSendIOReqRelay = false;
			return;
		}
		send(false);
	}
	void SSLConnection::handleRecvd(const boost::system::error_code& kErrorCode,
									size_t uBytesTransferred)
	{
	#ifdef _DEBUG
		BOOST_INTERLOCKED_EXCHANGE_ADD(&msRecvCnt, uBytesTransferred);
	#endif // _DEBUG

		mpRecvBuffer->writeEnd(uBytesTransferred);
		if (kErrorCode)
		{
			pushErrorCode(kErrorCode);
			asyncClose();
			mbRecvIOReqRelay = false;
			return;
		}
		if (uBytesTransferred == 0)
		{
			asyncClose();
			mbRecvIOReqRelay = false;
			return;
		}
		if (isClosed())
		{
			mbRecvIOReqRelay = false;
			return;
		}
		recv(false);
	}
	void SSLConnection::asyncWriteSome(unsigned char *acBuff,
		unsigned int uiSize)
	{
		mpkSSLSocket->async_write_some(
			boost::asio::buffer(acBuff, uiSize),
			mpkStrand->wrap(boost::bind(&Connection::handleSent, shared_from_this(),
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred)));
	}
	void SSLConnection::asyncReadSome(unsigned char *acBuff, unsigned int uiSize)
	{
	mpkSSLSocket->async_read_some(
		boost::asio::buffer(acBuff, uiSize),
		mpkStrand->wrap(boost::bind(&Connection::handleRecvd, shared_from_this(),
		boost::asio::placeholders::error,
		boost::asio::placeholders::bytes_transferred)));
	}
	std::string SSLConnection::getRemoteIP()
	{
		std::string strResult("");
		if (isClosed() || shouldBeClosed())
		{
			return strResult;
		}
		try
		{
			strResult = mpkSSLSocket->lowest_layer().remote_endpoint().address().to_string();
		}
		catch (...)
		{
			return strResult;
		}
		return strResult;
	}
	unsigned long SSLConnection::getRemoteAddress()
	{
		unsigned long ulAddress = 0;
		if (isClosed() || shouldBeClosed())
		{
			return ulAddress;
		}
		try
		{
			ulAddress = inet_addr(getRemoteIP().c_str());
		}
		catch (...)
		{
			return ulAddress;
		}
		return ulAddress;
	}
	std::string SSLConnection::getLocalIP()
	{
		std::string strResult("");
		if (isClosed() || shouldBeClosed())
		{
			return strResult;
		}
		try
		{
			strResult = mpkSSLSocket->lowest_layer().local_endpoint().address().to_string();
		}
		catch (...)
		{
			return strResult;
		}
		return strResult;
	}
	unsigned long SSLConnection::getLocalAddress()
	{
		unsigned long ulAddress = 0;
		if (isClosed() || shouldBeClosed())
		{
			return ulAddress;
		}
		try
		{
			ulAddress = inet_addr(getLocalIP().c_str());
		}
		catch (...)
		{
			return ulAddress;
		}
		return ulAddress;
	}
	bool SSLConnection::hasKnownError()
	{
		boost::recursive_mutex::scoped_lock kScopedLock(mkErrorCodeVectorMutex);
		for (unsigned int ui = 0; ui < mkErrorCodeVector.size(); ++ui)
		{
			if (!mkErrorCodeVector[ui] ||
				mkErrorCodeVector[ui] == boost::asio::error::eof ||
				mkErrorCodeVector[ui] == boost::asio::error::connection_aborted || 
				mkErrorCodeVector[ui] == boost::asio::error::connection_reset || 
				mkErrorCodeVector[ui] == boost::asio::error::connection_refused || 
				mkErrorCodeVector[ui] == boost::asio::error::shut_down || 
				mkErrorCodeVector[ui] == boost::system::error_code(
				ERR_PACK(ERR_LIB_SSL, 0, SSL_R_SHORT_READ), 
				boost::asio::error::get_ssl_category()))
			{
				return true;
			}
		}
		return false;
	}
	void SSLConnection::reset()
	{
		if (mpkStrand)
		{
			delete mpkStrand;
			mpkStrand = 0;
		}
		if (mpkSSLSocket)
		{
			delete mpkSSLSocket;
			mpkSSLSocket = 0;
		}
		if (mpkExtraSendBuffPool)
		{
			mpkExtraSendBuffPool->resetPool();
		}

		mpkSSLSocket = new SSLSocket(mkIOService, *mpkSSLContext);
		mpkStrand = new boost::asio::strand(mkIOService);
	}
}