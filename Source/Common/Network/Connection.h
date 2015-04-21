#ifndef __Connection_h__
#define __Connection_h__

#include "RingBuffer.h"
#include "ConnectionIDGenerator.h"
#include "ExtraSendBufferPool.h"
#include "TcpNetworkParams.h"
#include "TcpNetworkErrorCode.h"

namespace Network
{
	class Connection;
	typedef boost::shared_ptr<Connection> ConnectionPtr;

	/// ���ڹ���TCP����������պͷ��ͻ�������������ֱ�ӶϿ�����
	class Connection : public boost::enable_shared_from_this<Connection>
	{
	public:
		friend class IdealConnectionContainer;
		friend class TcpNetwork;
		typedef boost::asio::ip::tcp::socket Socket;
		typedef std::vector<boost::system::error_code> ErrorCodeVector;
		typedef ExtraSendBufferPool<unsigned char> UCExSendBufferPool;
		typedef boost::function<void(const boost::system::error_code)> OnConnectFunc;
		typedef boost::function<void(const boost::system::error_code)> OnHandShakeFunc;

	public:
		virtual Socket& getSocket() = 0;
		virtual void connect(boost::asio::ip::tcp::endpoint& kEndpoint, 
			boost::system::error_code& kErrorCode) = 0;
		virtual void asyncConnect(boost::asio::ip::tcp::endpoint& kEndpoint,
			OnConnectFunc pfnOnConnect) = 0;

		virtual void handShake(boost::system::error_code& kErrorCode){}
		virtual void asyncHandShake(OnHandShakeFunc pfnOnHandShakeFunc){}

		/// ֻ�������̵߳��ã���ֻ����Ψһһ���̵߳���
		virtual bool open() = 0;
		/// ǿ�ƹرգ��п��ܵ��½��ջ���ͷ��ͻ����е����δ������
		virtual void close() = 0;
		/// �첽�رգ����getErrorCodes����Ϊ����Ὣ��ݷ�����ɲ��ҽ��յ�������ݴ�����Ż�ر�
		virtual void asyncClose() = 0;
		/// SSLConnection���ӹرպ�������ʱ�ᷢ��handshakeʧ�ܣ�����д������ӿ���������
		virtual void reset() = 0;

		/// ��ȡԶ��IP��std::string��ʽ
		virtual std::string getRemoteIP() = 0;
		/// ��ȡԶ��IP, unsigned long��ʽ
		virtual unsigned long getRemoteAddress() = 0;
		/// ��ȡ����IP��std::string��ʽ
		virtual std::string getLocalIP() = 0;
		/// ��ȡ����IP, unsigned long��ʽ
		virtual unsigned long getLocalAddress() = 0;

		virtual bool isSSLConnection();

		void setAsServerSide();
		void setAsClientSide();
		bool isServerSide();
		bool isClientSide();

		/// ���Թرգ�ֻ���Ѿ���markShouldBeClosed()���ҽ��պͷ��ͻ��涼Ϊ��
		/// ��ʱ��Źرպͷ���true�����򷵻�false
		bool tryClose();
		/// �Ƿ������Ѿ��ر�
		virtual bool isClosed();
		/// ֻ�������̵߳��ã���ֻ����Ψһһ���̵߳���
		bool write(unsigned char* acBuff, unsigned int uiSize);
		/// ֻ�������̵߳��ã���ֻ����Ψһһ���̵߳���
		unsigned int read(unsigned char* acBuff, unsigned int uiBuffSize);
		/// ֻ�������̵߳��ã���ֻ����Ψһһ���̵߳���
		/// @param acBuff ����ʼλ��
		/// @param uiReadableSize �ɶ��ڴ��С
		/// @return ���readPacketsBegin�Ѿ�������readPacketsEndδ��Ե��÷���false
		bool readPacketsBegin(unsigned char*& acBuff, unsigned int& uiReadableSize);
		/// ֻ�������̵߳��ã���ֻ����Ψһһ���̵߳���
		/// �ύ�Ѿ�����İ��С
		void readPacketsEnd(unsigned int uiProcessedSize);
		/// ��ȡ����ID
		ConnectionID getID();
		/// ��IO�̷߳������ʱ��ֱ��ǿ�ƹر����ӣ����Ѵ��������һ��mkErrorCodeVector
		/// ����ͨ���������ȡ��������IO�̷߳���Ĵ���
		/// ��Ҫ��������������������Ϊ�п��ܽ��պͷ�������IO�¼��������߳�ͬʱ���ش���
		/// @{
		unsigned int getErrorCodesCount();
		ErrorCodeVector getErrorCodes();
		void pushErrorCode(const boost::system::error_code &kErrorCode);
		void clearErrorCodes();
		/// @}
		/// ���Խ�û�з�����Ķ��ⷢ�ͻ����е���ݷ��ͳ�ȥ
		void trySendExtraSendBuff();
	    /// ��ȡ��ǰ���ջ������е�һ����Ĵ�С������ֵΪfalse��ʱ�򣬱�ʾ���С������
	    /// �����ջ����е���ݲ����ʾ���С(sizeof(unsinged int))ʱ������false
	    bool currentReceivedPacketSize(unsigned int& uiPacketSize);

		/// set public for callback reason, DONOT call other than itself
		/// @{
	public:
		/// ������ɺ�IO�̻߳�ص��������
		virtual void handleSent(const boost::system::error_code& kErrorCode,
			size_t uBytesTransferred) = 0;
		/// ������ɺ�IO�̻߳�ص��������
		virtual void handleRecvd(const boost::system::error_code& kErrorCode,
			size_t uBytesTransferred) = 0;
		/// asyncClose�Ļص�
		virtual void handleClose(const boost::system::error_code& kErrorCode) = 0;

		/// for ssl
		/// @{
		virtual void asyncWriteSome(unsigned char *acBuff, unsigned int uiSize){}
		virtual void asyncReadSome(unsigned char *acBuff, unsigned int uiSize){}
		virtual void closeWrapper(){}
		/// @}
		
		/// @}

	protected:
		/// @param io_service boost::asio::io_service����
		/// @param pkParams ���Ӳ���
		Connection(boost::asio::io_service& io_service,
			TcpNetworkParams* pkParams);
		virtual ~Connection();
		/// ����һ��������������IO�߳���ͬʱֻ����һ��IO�̷߳���˺������
		/// ���̼߳��mbSendIOReqRelayΪfalseʱ�ᷢ��˺������
		/// @param bCallAtMainThread �Ƿ������̷߳���ĵ���
		virtual void send(bool bCallAtMainThread) = 0;
		/// ����һ��������������IO�߳���ͬʱֻ����һ��IO�̷߳���˺������
		/// ���̼߳��mbRecvIOReqRelayΪfalseʱ�ᷢ��˺������
		/// @param bCallAtMainThread �Ƿ������̷߳���ĵ���
		virtual void recv(bool bCallAtMainThread) = 0;

		void assignID();
		/// ��ʶ������Ҫ���رգ���ʶ�󽫲�����д���@see Connection::write
		void markShouldBeClosed();
		/// �ж��Ƿ��Ѿ�markShouldBeClosed()
		bool shouldBeClosed();
		/// �ж��Ƿ��Ѿ�markShouldBeClosed()���ҽ��պͷ��ͻ��涼Ϊ��
		bool canBeClosed();
		/// �ж��Ƿ����Ѿ����յ���û��������
		bool hasRecvedData();
		/// �ж��Ƿ����Ѿ����յ���û�����������ݰ�
		bool hasRecvedPacket();

		/// ����Ƿ���û�м�ʱ���͵����
		bool isDataInExtraSendBuff();
		/// �����ܼ�ʱ���͵���ݷŵ�����ķ��ͻ�����
		void pushToExtraSendBuff(unsigned char* pcBuff, unsigned int uiSize);
		/// �����ⷢ�ͻ�����������д�����ͻ�����
		void writeExtraSendBuff();
		/// ����socket��keep aliveѡ��
		bool setKeepAlive();

	protected:
		RingBuffer<unsigned char> *mpSendBuffer;
		RingBuffer<unsigned char> *mpRecvBuffer;
		ErrorCodeVector mkErrorCodeVector;        ///< ����������IO�̷߳���Ĵ�����
		boost::recursive_mutex mkErrorCodeVectorMutex;
		
		ConnectionID muID;
		ConnectionPtr mpkNext;
		ConnectionPtr mpkPrev;
		bool volatile mbShouldBeClosed;           ///< ׼���رգ��ȴ�δ��ɵ�����շ����
		bool volatile mbClosing;                  ///< ��ʼ�رգ��շ����������
		bool volatile mbClosed;                   ///< �Ѿ��ر�
		bool mbNeedCallCloseCallback;             ///< �Ƿ���Ҫ�������ӹرյĻص�
		/// Connection::mbSendIOReqRelay��Connection::mbRecvIOReqRelay���˳�ʼ��ʱΪfalse��
		/// ����ʱ��ֻ�е�IO�̵߳�IO����Ͷ�ݽ����Ͽ�ʱ���Żᱻ��Ϊfalse��
		/// IO����Ͷ�ݽ�����ָIO���󷵻�ʱ���������ͬ��IO���������Ϊ����ԭ��
		/// û�м���������ͬ��IO��������ͳ�֮ΪIO����Ͷ�ݽ����Ͽ���
		/// @{
		bool volatile mbSendIOReqRelay;           ///< �Ƿ�IO��������Ͷ�ݽ�����
		bool volatile mbRecvIOReqRelay;           ///< �Ƿ�IO��������Ͷ�ݽ�����
		/// @}
		TcpNetworkParams* mpkTcpNetworkParams;
		UCExSendBufferPool* mpkExtraSendBuffPool; ///< �����ͻ��������ʱ��mbCloseOnBuffOverflowΪfalseʱ�����͵������ʱ���ڴ�
		bool mbServerSide;

#ifdef _DEBUG
	public:
		static long msSendCnt;
		static long msRecvCnt;
#endif // _DEBUG
	};

	inline void Connection::assignID()
	{
		muID = ConnectionIDGenerator::NewConnectionID();
	}

	inline ConnectionID Connection::getID()
	{
		return muID;
	}

	inline bool Connection::isClosed()
	{
		return (mbClosed && !mbRecvIOReqRelay && !mbSendIOReqRelay);
	}

	inline void Connection::markShouldBeClosed()
	{
		mbShouldBeClosed = true;
	}

	inline bool Connection::shouldBeClosed()
	{
		return mbShouldBeClosed;
	}

	inline bool Connection::canBeClosed()
	{
		if (!shouldBeClosed())
		{
			return false;
		}
		/// had received data must be processed
		if ((mpkTcpNetworkParams->eDataProcessMethod == DPM_Packet && hasRecvedPacket()) ||
			(mpkTcpNetworkParams->eDataProcessMethod == DPM_Stream && hasRecvedData()))
		{
			return false;
		}
		/// try best to send data in send buffer
		bool bHasWaitForSendingData = (mpSendBuffer->readableSize() > 0) ||
				(mpkExtraSendBuffPool ? mpkExtraSendBuffPool->hasData() : false);
		if (!mbClosing && getErrorCodesCount() == 0 && bHasWaitForSendingData)
		{
			return false;
		}
		return true;
	}

	inline bool Connection::tryClose()
	{
		if (canBeClosed() && !isClosed())
		{
			close();
			return isClosed();
		}
		return isClosed();
	}

	inline bool Connection::hasRecvedData()
	{
		return mpRecvBuffer->readableSize() > 0;
	}

	inline bool Connection::hasRecvedPacket()
	{
		unsigned int uiPacketSize = 0;
		bool bPacketSizeValid = mpRecvBuffer->currentPacketSize(uiPacketSize);

		return (bPacketSizeValid && 
			uiPacketSize <= mpRecvBuffer->readableSize() &&
			uiPacketSize > 0 &&
			uiPacketSize <= mpkTcpNetworkParams->uiMaxConnectionWriteSize);
	}
	inline unsigned int Connection::getErrorCodesCount()
	{
		boost::recursive_mutex::scoped_lock kScopedLock(mkErrorCodeVectorMutex);
        return (unsigned int)mkErrorCodeVector.size();
	}
	inline Connection::ErrorCodeVector Connection::getErrorCodes()
	{
		boost::recursive_mutex::scoped_lock kScopedLock(mkErrorCodeVectorMutex);
		return mkErrorCodeVector;
	}

	inline void Connection::pushErrorCode(const boost::system::error_code &kErrorCode)
	{
		boost::recursive_mutex::scoped_lock kScopedLock(mkErrorCodeVectorMutex);
		mkErrorCodeVector.push_back(kErrorCode);
	}

	inline void Connection::clearErrorCodes()
	{
		boost::recursive_mutex::scoped_lock kScopedLock(mkErrorCodeVectorMutex);
		mkErrorCodeVector.clear();
	}
	inline bool Connection::isSSLConnection()
	{
		return false;
	}
	inline void Connection::setAsServerSide()
	{
		mbServerSide = true;
	}
	inline void Connection::setAsClientSide()
	{
		mbServerSide = false;
	}
	inline bool Connection::isServerSide()
	{
		return mbServerSide;
	}
	inline bool Connection::isClientSide()
	{
		return !mbServerSide;
	}
	inline bool Connection::currentReceivedPacketSize(unsigned int& uiPacketSize)
	{
		return mpRecvBuffer->currentPacketSize(uiPacketSize);
	}
}
#endif//__Connection_h__
