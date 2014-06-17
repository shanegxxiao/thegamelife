#ifndef __HttpNetwork_h__
#define __HttpNetwork_h__
#include "HttpRequest.h"
#include "HttpRespone.h"
#include <boost/function.hpp>
#include <boost/thread.hpp>
#include <queue>
#include <vector>
#include "HttpConnection.h"

namespace Network
{
	class HttpNetwork
	{
	public:
		struct RequestCmp  
		{
			bool operator()(const HttpRequestPtr &na, const HttpRequestPtr &nb)  
			{
				return na->getPriority() > nb->getPriority();
			}
		};
		struct ResponeCmp
		{
			bool operator()(const HttpResponePtr &na, const HttpResponePtr &nb)  
			{
				return na->getPriority() > nb->getPriority();
			}  
		};
		typedef boost::function<void(HttpRequestPtr)> RequestHandler;
		typedef boost::function<void(HttpResponePtr)> ResponeHandler;
		typedef boost::function<void(HttpConnectionPtr)> ClientErrorHandler;
		typedef boost::function<void(HttpConnectionPtr)> ServerErrorHandler;
		typedef boost::function<void(HttpConnectionPtr)> ResponeSentHandler;
		typedef std::priority_queue<HttpRequestPtr, std::vector<HttpRequestPtr>, RequestCmp> HttpRequestQueue;
		typedef std::priority_queue<HttpResponePtr, std::vector<HttpResponePtr>, ResponeCmp> HttpResponeQueue;
		typedef std::vector<HttpConnectionPtr> HttpClientConnectionVector;
		typedef std::vector<HttpConnectionPtr> HttpServerConnectionVector;

	public:
		HttpNetwork();
		~HttpNetwork();
		bool startup(const Network::HttpNetworkParams& kParams = Network::HttpNetworkParams::Default());
		bool listen(const char* pcListenIP, unsigned int uiListenPort);
		void running();
		bool shutdown();

		/// 用户通过回调函数获得的request智能指针只能在主线程释放
		void bindRequestHandler(RequestHandler kHandler);
		/// 用户通过回调函数获得的response智能指针只能在主线程释放
		void bindResponeHandler(ResponeHandler kHandler);
		void bindClientErrorHandler(ClientErrorHandler kErrorHandler);
		void bindServerErrorHandler(ServerErrorHandler kErrorHandler);
		void bindResponeSentHandler(ResponeSentHandler kResponeSentHandler);

		void sendRequest(HttpRequestPtr spRequest);
		void sendRespone(HttpResponePtr spRespone);

	private:
		HttpConnectionPtr getIdealClientConnection(bool bSSL);
		void putIdealClientConnection(HttpConnectionPtr spConnection);
		HttpConnectionPtr getIdealServerConnection(bool bSSL);
		void putIdealServerConnection(HttpConnectionPtr spConnection);

		/// client
		/// @{
		HttpRequestPtr getRequestFromSendingQueue();
		void handleRequestSendingQueue();
		void addRecvingResponeConnection(HttpConnectionPtr spConnection);
		void handleResponeRecvingQueue();
		/// @{

		/// server
		/// @{
		bool startAccept();
		void onAccept(HttpConnectionPtr& pkConnection,
			const boost::system::error_code& kError);
		void addRecvingRequestConnection(HttpConnectionPtr spConnection);
		void eraseRequestRecvingConnection(HttpConnectionPtr spConnection);
		void handleRequestRecvingQueue();
		HttpConnectionPtr getRequestRecvingConnection();
		HttpResponePtr getResponeFromSendingQueue();
		void handleResponeSendingQueue();
		void addSendingResponeConnection(HttpConnectionPtr spConnection);
		void handleSendingResponeConnection();
		/// @}

	private:
		std::string mstrListenIP;
		unsigned int muiListenPort;
		RequestHandler mkRequestHandler;
		ResponeHandler mkResponeHandler;
		ClientErrorHandler mkClientErrorHandler;
		ServerErrorHandler mkServerErrorHandler;
		ResponeSentHandler mkResponeSentHandler;
		boost::asio::io_service mkIOService;
		boost::asio::io_service::work *mpkIOWork;
		boost::asio::ip::tcp::acceptor mkAcceptor;
		boost::thread_group mkThreadGroup;
		HttpNetworkParams mkHttpParameters;
		HttpClientConnectionVector mkRecvingResponeConVector;
		boost::recursive_mutex mkRecvingRequestConVectorMutex;
		HttpServerConnectionVector mkRecvingRequestConVector;
		HttpServerConnectionVector mkSendingResponeConVector;

		boost::recursive_mutex mkRequestSendingQueueMutex;
		HttpRequestQueue mkRequestSendingQueue;
		boost::recursive_mutex mkResponeSendingQueueMutex;
		HttpResponeQueue mkResponeSendingQueue;
		/// 客户端、服务端使用的SSL上下文
		///@{
		boost::asio::ssl::context* mpkSSLContext;
		///@}
	};

	inline void HttpNetwork::bindRequestHandler(RequestHandler kHandler)
	{
		mkRequestHandler = kHandler;
	}
	inline void HttpNetwork::bindResponeHandler(ResponeHandler kHandler)
	{
		mkResponeHandler = kHandler;
	}
	inline void HttpNetwork::bindClientErrorHandler(ClientErrorHandler kErrorHandler)
	{
		mkClientErrorHandler = kErrorHandler;
	}
	inline void HttpNetwork::bindServerErrorHandler(ServerErrorHandler kErrorHandler)
	{
		mkServerErrorHandler = kErrorHandler;
	}
	inline void HttpNetwork::bindResponeSentHandler(ResponeSentHandler kResponeSentHandler)
	{
		mkResponeSentHandler = kResponeSentHandler;
	}
	inline void HttpNetwork::sendRequest(HttpRequestPtr spRequest)
	{
		boost::recursive_mutex::scoped_lock kScopedLock(mkRequestSendingQueueMutex);
		mkRequestSendingQueue.push(spRequest);
	}
	inline void HttpNetwork::sendRespone(HttpResponePtr spRespone)
	{
		boost::recursive_mutex::scoped_lock kScopedLock(mkResponeSendingQueueMutex);
		spRespone->getRequest()->getServerConnection()->setStatus(HttpConnection::pending_response);
		mkResponeSendingQueue.push(spRespone);
	}
	inline HttpRequestPtr HttpNetwork::getRequestFromSendingQueue()
	{
		boost::recursive_mutex::scoped_lock kScopedLock(mkRequestSendingQueueMutex);
		if (mkRequestSendingQueue.empty())
		{
			HttpRequestPtr kPtr;
			return kPtr;
		}
		HttpRequestPtr kRequest = mkRequestSendingQueue.top();
		mkRequestSendingQueue.pop();
		return kRequest;
	}
	inline HttpResponePtr HttpNetwork::getResponeFromSendingQueue()
	{
		boost::recursive_mutex::scoped_lock kScopedLock(mkResponeSendingQueueMutex);
		if (mkResponeSendingQueue.empty())
		{
			HttpResponePtr kPtr;
			return kPtr;
		}
		HttpResponePtr kRespone = mkResponeSendingQueue.top();
		mkResponeSendingQueue.pop();
		return kRespone;
	}
	inline HttpConnectionPtr HttpNetwork::getRequestRecvingConnection()
	{
		HttpConnectionPtr spConnection;
		boost::recursive_mutex::scoped_lock kScopedLock(mkRecvingRequestConVectorMutex);
		if (mkRecvingRequestConVector.size() > 0)
		{
			spConnection = mkRecvingRequestConVector[0];
		}
		return spConnection;
	}
	inline void HttpNetwork::addSendingResponeConnection(HttpConnectionPtr spConnection)
	{
		mkSendingResponeConVector.push_back(spConnection);
	}
}
#endif//__HttpNetwork_h__