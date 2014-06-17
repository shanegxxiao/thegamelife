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

	/// 用于管理TCP数据流，接收和发送缓冲区如果溢出就直接断开链接
	class Connection : public boost::enable_shared_from_this<Connection>
	{
	public:
		friend class IdealConnectionContainer;
		friend class TcpNetwork;
		typedef boost::asio::ip::tcp::socket Socket;
		typedef std::vector<const boost::system::error_code> ErrorCodeVector;
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

		/// 只能在主线程调用，或只能有唯一一个线程调用
		virtual bool open() = 0;
		/// 强制关闭，有可能导致接收缓存和发送缓存中的数据未被处理
		virtual void close() = 0;
		/// 异步关闭，如果getErrorCodes（）为空则会将数据发送完成并且接收的完整数据处理完才会关闭
		virtual void asyncClose() = 0;
		/// SSLConnection链接关闭后再重用时会发生handshake失败，所以写了这个接口重置链接
		virtual void reset() = 0;

		/// 获取远程IP，std::string形式
		virtual std::string getRemoteIP() = 0;
		/// 获取远程IP, unsigned long形式
		virtual unsigned long getRemoteAddress() = 0;
		/// 获取本地IP，std::string形式
		virtual std::string getLocalIP() = 0;
		/// 获取本地IP, unsigned long形式
		virtual unsigned long getLocalAddress() = 0;

		virtual bool isSSLConnection();

		void setAsServerSide();
		void setAsClientSide();
		bool isServerSide();
		bool isClientSide();

		/// 尝试关闭，只有已经被markShouldBeClosed()并且接收和发送缓存都为空
		/// 的时候才关闭和返回true，否则返回false
		bool tryClose();
		/// 是否链接已经关闭
		virtual bool isClosed();
		/// 只能在主线程调用，或只能有唯一一个线程调用
		bool write(unsigned char* acBuff, unsigned int uiSize);
		/// 只能在主线程调用，或只能有唯一一个线程调用
		unsigned int read(unsigned char* acBuff, unsigned int uiBuffSize);
		/// 只能在主线程调用，或只能有唯一一个线程调用
		/// @param acBuff 包起始位置
		/// @param uiReadableSize 可读内存大小
		/// @return 如果readPacketsBegin已经调用且readPacketsEnd未配对调用返回false
		bool readPacketsBegin(unsigned char*& acBuff, unsigned int& uiReadableSize);
		/// 只能在主线程调用，或只能有唯一一个线程调用
		/// 提交已经处理的包大小
		void readPacketsEnd(unsigned int uiProcessedSize);
		/// 获取链接ID
		ConnectionID getID();
		/// 在IO线程发生错误时会直接强制关闭链接，并把错误码放入一个mkErrorCodeVector
		/// 可以通过这个函数取得所有在IO线程发生的错误；
		/// 需要给这个错误码数组加锁，因为有可能接收和发送两个IO事件在两个线程同时返回错误
		/// @{
		unsigned int getErrorCodesCount();
		ErrorCodeVector getErrorCodes();
		void pushErrorCode(const boost::system::error_code &kErrorCode);
		void clearErrorCodes();
		/// @}
		/// 尝试将没有发送完的额外发送缓存中的数据发送出去
		void trySendExtraSendBuff();
	    /// 获取当前接收缓冲区中第一个包的大小，如果返回值为false的时候，表示包大小不可用
	    /// 当接收缓存中的数据不足表示包大小(sizeof(unsinged int))时，返回false
	    bool currentReceivedPacketSize(unsigned int& uiPacketSize);

		/// set public for callback reason, DONOT call other than itself
		/// @{
	public:
		/// 发送完成后IO线程会回调这个函数
		virtual void handleSent(const boost::system::error_code& kErrorCode,
			size_t uBytesTransferred) = 0;
		/// 接收完成后IO线程会回调这个函数
		virtual void handleRecvd(const boost::system::error_code& kErrorCode,
			size_t uBytesTransferred) = 0;
		/// asyncClose的回调
		virtual void handleClose(const boost::system::error_code& kErrorCode) = 0;

		/// for ssl
		/// @{
		virtual void asyncWriteSome(unsigned char *acBuff, unsigned int uiSize){}
		virtual void asyncReadSome(unsigned char *acBuff, unsigned int uiSize){}
		virtual void closeWrapper(){}
		/// @}
		
		/// @}

	protected:
		/// @param io_service boost::asio::io_service引用
		/// @param pkParams 连接参数
		Connection(boost::asio::io_service& io_service,
			TcpNetworkParams* pkParams);
		virtual ~Connection();
		/// 发起一个发送请求，所有IO线程中同时只会有一个IO线程发起此函数调用
		/// 主线程检查mbSendIOReqRelay为false时会发起此函数调用
		/// @param bCallAtMainThread 是否在主线程发起的调用
		virtual void send(bool bCallAtMainThread) = 0;
		/// 发起一个接收请求，所有IO线程中同时只会有一个IO线程发起此函数调用
		/// 主线程检查mbRecvIOReqRelay为false时会发起此函数调用
		/// @param bCallAtMainThread 是否在主线程发起的调用
		virtual void recv(bool bCallAtMainThread) = 0;

		void assignID();
		/// 标识链接需要被关闭，标识后将不接受写数据@see Connection::write
		void markShouldBeClosed();
		/// 判断是否已经markShouldBeClosed()
		bool shouldBeClosed();
		/// 判断是否已经markShouldBeClosed()并且接收和发送缓存都为空
		bool canBeClosed();
		/// 判断是否有已经接收但还没处理的数据
		bool hasRecvedData();
		/// 判断是否有已经接收但还没处理的完整数据包
		bool hasRecvedPacket();

		/// 检查是否有没有及时发送的数据
		bool isDataInExtraSendBuff();
		/// 将不能及时发送的数据放到额外的发送缓冲区
		void pushToExtraSendBuff(unsigned char* pcBuff, unsigned int uiSize);
		/// 将额外发送缓冲区里的数据写到发送缓存中
		void writeExtraSendBuff();
		/// 设置socket的keep alive选项
		bool setKeepAlive();

	protected:
		RingBuffer<unsigned char> *mpSendBuffer;
		RingBuffer<unsigned char> *mpRecvBuffer;
		ErrorCodeVector mkErrorCodeVector;        ///< 缓存所有在IO线程发生的错误码
		boost::recursive_mutex mkErrorCodeVectorMutex;
		
		ConnectionID muID;
		ConnectionPtr mpkNext;
		ConnectionPtr mpkPrev;
		bool volatile mbShouldBeClosed;           ///< 准备关闭，等待未完成的数据收发完成
		bool volatile mbClosing;                  ///< 开始关闭，收发不了数据了
		bool volatile mbClosed;                   ///< 已经关闭
		bool mbNeedCallCloseCallback;             ///< 是否需要调用链接关闭的回调
		/// Connection::mbSendIOReqRelay和Connection::mbRecvIOReqRelay除了初始化时为false，
		/// 其它时候只有当IO线程的IO请求投递接力断开时，才会被置为false；
		/// IO请求投递接力是指IO请求返回时会继续发起相同的IO请求，如果因为各种原因
		/// 没有继续起起相同的IO请求，这里就称之为IO请求投递接力断开。
		/// @{
		bool volatile mbSendIOReqRelay;           ///< 是否IO发送请求投递接力正常
		bool volatile mbRecvIOReqRelay;           ///< 是否IO接收请求投递接力正常
		/// @}
		TcpNetworkParams* mpkTcpNetworkParams;
		UCExSendBufferPool* mpkExtraSendBuffPool; ///< 当发送缓冲区溢出时且mbCloseOnBuffOverflow为false时，发送的数据临时存在此
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
		return mkErrorCodeVector.size();
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