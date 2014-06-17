#ifndef __TcpConnection_h__
#define __TcpConnection_h__

#include "Connection.h"

namespace Network
{
	class TcpConnection : public Connection
	{
	public:
		friend class IdealConnectionContainer;
		friend class TcpNetwork;
		typedef boost::asio::ip::tcp::socket TCPSocket;

	public:
		virtual ~TcpConnection();
		virtual Socket& getSocket();
		virtual void connect(boost::asio::ip::tcp::endpoint& kEndpoint, 
			boost::system::error_code& kErrorCode);
		virtual void asyncConnect(boost::asio::ip::tcp::endpoint& kEndpoint,
			OnConnectFunc pfnOnConnect);

		/// 只能在主线程调用，或只能有唯一一个线程调用
		virtual bool open();
		/// 强制关闭，有可能导致接收缓存和发送缓存中的数据未被处理
		virtual void close();
		/// 异步关闭，如果getErrorCodes（）为空则会将数据发送完成并且接收的完整数据处理完才会关闭
		virtual void asyncClose();
		/// SSLConnection链接关闭后再重用时会发生handshake失败，所以写了这个接口重置链接
		virtual void reset();

		/// 获取远程IP，std::string形式
		virtual std::string getRemoteIP();
		/// 获取远程IP, unsigned long形式
		virtual unsigned long getRemoteAddress();

		/// 获取本地IP，std::string形式
		virtual std::string getLocalIP();
		/// 获取本地IP, unsigned long形式
		virtual unsigned long getLocalAddress();
		
	protected:
		/// @param io_service boost::asio::io_service引用
		/// @param pkParams 链接参数
		TcpConnection(boost::asio::io_service& io_service,
			TcpNetworkParams* pkParams);
		
		/// 发起一个发送请求，所有IO线程中同时只会有一个IO线程发起此函数调用
		/// 主线程检查mbSendIOReqRelay为false时会发起此函数调用
		/// @param bCallAtMainThread 是否在主线程发起的调用
		virtual void send(bool bCallAtMainThread);
		/// 发起一个接收请求，所有IO线程中同时只会有一个IO线程发起此函数调用
		/// 主线程检查mbRecvIOReqRelay为false时会发起此函数调用
		/// @param bCallAtMainThread 是否在主线程发起的调用
		virtual void recv(bool bCallAtMainThread);
		/// 发送完成后IO线程会回调这个函数
		virtual void handleSent(const boost::system::error_code& kErrorCode,
			size_t uBytesTransferred);
		/// 接收完成后IO线程会回调这个函数
		virtual void handleRecvd(const boost::system::error_code& kErrorCode,
			size_t uBytesTransferred);
		/// asyncClose的回调
		virtual void handleClose(const boost::system::error_code& kErrorCode);
		
	private:
		TCPSocket mkSocket;
	};
	inline void TcpConnection::reset()
	{
		if (mpkExtraSendBuffPool)
		{
			mpkExtraSendBuffPool->resetPool();
		}
	}
	inline Connection::Socket& TcpConnection::getSocket()
	{
		return mkSocket;
	}
}
#endif//__TcpConnection_h__