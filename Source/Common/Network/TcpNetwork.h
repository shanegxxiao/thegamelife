#ifndef __TcpNetwork_h__
#define __TcpNetwork_h__

#include "TcpConnection.h"
#include "SSLConnection.h"
#include "ConnectionIDGenerator.h"
#include "TcpNetworkParams.h"
#include "TcpNetworkErrorCode.h"

namespace Network
{
	class IdealConnectionContainer;
	class ActiveConnectionContainer;

	class TcpNetwork
	{
	public:
		typedef std::list<ConnectionPtr> ConnectionList;
		typedef std::map<ConnectionID, ConnectionPtr> ConnectionMap;
		typedef boost::function<void(ConnectionID uConID)> OnConFunc;
		typedef boost::function<void(ConnectionID uConID)> OnDisConFunc;
		typedef boost::function<void(ConnectionID uConID, const unsigned char* acData, unsigned int uSize)> OnRecvFunc;
		typedef boost::function<bool(ConnectionID uConID, const unsigned char* acPacket, unsigned int uPacketSize)> OnPacketFunc;
		typedef boost::function<void(ConnectionID uConID, unsigned int uSize)> OnSentFunc;
		typedef boost::function<void(ConnectionID uConID, const boost::system::error_code& kErrorCode)> ErrorHandleFunc;

	public:
		TcpNetwork();
		/// 启动TcpNetwork
		bool startup(const TcpNetworkParams& pkParams = TcpNetworkParams::Default());
		/// 如果是服务器端使用需要调用listen去监听指定的IP和端口
		bool listen(const char* pcListenIP, unsigned int uiListenPort);
		/// 阻塞连接指定IP和地址
		ConnectionID connect(const char* pcServerIP, 
			unsigned int uiServerPort, ConnectionType eConnectionType = CT_DEFAULT);
		/// 异步连接指定IP和地址，链接结果回调通过@see bindOnConnected设置
		ConnectionID asyncConnect(const char* pcServerIP, 
			unsigned int uiServerPort, ConnectionType eConnectionType = CT_DEFAULT);
		/// 异步发送数据，发送完成回调通过@see bindOnSent设置
		// @bWait :  true: 如果缓冲区已满, 返回false; false: 如果缓冲区已满, 断开或new新的缓冲区.
		bool send(ConnectionID uConID, unsigned char* acData, unsigned int uSize);
		/// 阻塞式断开指定ID的链接
		bool closeConnection(ConnectionID uConID);
		/// 异步断开指定ID的链接，断开回调通过@see bindOnDisconnected设置
		bool asyncCloseConnection(ConnectionID uConID);
		/// 主循环
		void running();
		/// 强制退出，如果链接的接收缓存中有数据会被丢弃，发送缓存如果有数据了不会被发送完
		bool shutdown();
		/// 只有当所有已经接收的数据处理完，并且所有链接已经关闭，才关闭和返回true
		bool tryShutdown();
		/// 链接被接受后会放入一个链表@see mkWaitActiveConList
		/// handleWaitForActivingConnections()函数处理后会被放入mpkActiveConContainer
		/// 只有在mpkActiveConContainer里的链接才可以接收和发送数据
		bool isActived(ConnectionID uID);
		/// 获取链接的std::string形式的远程IP地址
		std::string getConnectionRemoteIP(ConnectionID uID);
		/// 获取链接的unsigned long形式的远程IP地址
		unsigned long getConnectionRemoteAddress(ConnectionID uID);
		/// 获取链接的std::string形式的本地IP地址
		std::string getConnectionLocalIP(ConnectionID uID);
		/// 获取链接的unsigned long形式的本地IP地址
		unsigned long getConnectionLocalAddress(ConnectionID uID);
		/// 获取网络初始化参数
		const Network::TcpNetworkParams& getNetworkParames();

		/// 回调设置，所有回调都会在主线程调用
		/// @{
		/// 设置连接完成回调
		void bindOnConnected(OnConFunc pOnConFunc);
		/// 设置连接断开回调
		void bindOnDisconnected(OnDisConFunc pOnDisConFunc);
		/// 与@see bindOnPacket互斥，如果已经调用过@see bindOnPacket则返回false
		bool bindOnRecved(OnRecvFunc pOnRecvFunc);
		/// 与@see bindOnRecved互斥，如果已经调用过@see bindOnRecved则返回false
		/// OnPacketFunc返回bool表示是否要继续处理未处理完的数据
		/// 返回false表示当前包处理完后就结束当前链接这一帧的网络数据处理
		bool bindOnPacket(OnPacketFunc pOnPacketFunc);
		/// 设置发送结果回调
		void bindOnSent(OnSentFunc pOnSentFunc);
		/// 设置连接出错回调
		void bindOnError(ErrorHandleFunc pOnSentFunc);
		/// @}

	protected:
		bool startAccept();
		void onAccept(ConnectionPtr& pkConnection,
			const boost::system::error_code& kError);
		void onConnect(ConnectionPtr& pkConnection,
			boost::system::error_code& kErrorCode);
		void onHandshake(ConnectionPtr& pkConnection,
			boost::system::error_code& kErrorCode);

		void handleWaitForActivingConnections();
		void handleAllActivedConnections();
		void handleErrorConnections();
		void checkAndHandleConnectionErrors(ConnectionPtr& pkConnection);
		void visitAciveConnection(ConnectionID uID, ConnectionPtr& pkConnection);

	protected:
		std::string mstrListenIP;
		unsigned int muiListenPort;
		unsigned char* mpcRecvTmpBuffer;
		boost::asio::io_service mkIOService;
		boost::asio::io_service::work *mpkIOWork;
		boost::asio::ip::tcp::acceptor mkAcceptor;
		boost::thread_group mkThreadGroup;
		TcpNetworkParams mkNetworkParams;
		IdealConnectionContainer* mpkIdealConContainer;
		ActiveConnectionContainer* mpkActiveConContainer;
		ConnectionList mkWaitActiveConList;
		boost::mutex mkWaitActiveConListMutex;
		ConnectionMap mkHandshakingConMap;
		boost::mutex mkHandshakingConMapMutex;
		ConnectionList mkErrorConList;
		boost::mutex mkErrorConListMutex;
		OnConFunc mpOnConFunc;
		OnDisConFunc mpOnDisConFunc;
		OnRecvFunc mpOnRecvFunc;        ///< 与mpOnPacketFunc互斥，只能有一个有值
		OnPacketFunc mpOnPacketFunc;    ///< 与mpOnRecvFunc互斥，只能有一个有值
		OnSentFunc mpOnSentFunc;
		ErrorHandleFunc mpOnErrorHandleFunc;
		bool mbStartup;
		boost::asio::ssl::context* mpkSSLContext;///< SSL上下文
	};

	inline void TcpNetwork::bindOnConnected(OnConFunc pOnConFunc)
	{
		mpOnConFunc = pOnConFunc;
	}
	inline void TcpNetwork::bindOnDisconnected(OnDisConFunc pOnDisConFunc)
	{
		mpOnDisConFunc = pOnDisConFunc;
	}
	inline bool TcpNetwork::bindOnRecved(OnRecvFunc pOnRecvFunc)
	{
		if (mpOnPacketFunc)
		{
			return false;
		}
		mpOnRecvFunc = pOnRecvFunc;
		return true;
	}
	inline bool TcpNetwork::bindOnPacket(OnPacketFunc pOnPacketFunc)
	{
		if (mpOnRecvFunc)
		{
			return false;
		}
		mpOnPacketFunc = pOnPacketFunc;
		return true;
	}
	inline void TcpNetwork::bindOnSent(OnSentFunc pOnSentFunc)
	{
		mpOnSentFunc = pOnSentFunc;
	}
	inline void TcpNetwork::bindOnError(ErrorHandleFunc pOnErrorHandleFunc)
	{
		mpOnErrorHandleFunc = pOnErrorHandleFunc;
	}
	inline const Network::TcpNetworkParams& TcpNetwork::getNetworkParames()
	{
		return mkNetworkParams;
	}
}

#endif//__TcpNetwork_h__