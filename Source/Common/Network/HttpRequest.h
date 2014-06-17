#ifndef __HttpRequest_h__
#define __HttpRequest_h__
#include "HttpNetworkParams.h"
#include "HttpNetworkErrorCode.h"
#include <boost/smart_ptr.hpp>
#include <boost/asio/streambuf.hpp>

namespace Network
{
	class HttpNetwork;
	class HttpRespone;
	class HttpConnection;
	class HttpClientConnection;
	class HttpServerConnection;
	class HttpsClientConnection;
	class HttpsServerConnection;
	typedef boost::shared_ptr<HttpRespone> HttpResponePtr;
	typedef boost::shared_ptr<HttpConnection> HttpConnectionPtr;

	class HttpRequest : public boost::enable_shared_from_this<HttpRequest>
	{
	public:
		HttpRequest();
		~HttpRequest();
		std::string toString();
		void putHeaderField(std::string strName, std::string strValue);
		unsigned int getContentLength();
		void setUserData(void* pUserData);
		void* getUserData();
		void setPriority(float fPriority);
		float getPriority();
		void setNeedWaittingRespone();
		HttpResponePtr GenerateRespone();
		HttpConnectionPtr getServerConnection();
		void useSSL(bool bUseSSL);
		bool isUseSSL();

	public:
		std::string method;
		std::string uri;
		std::string http_version;
		Headers headers;
		std::string content;

	private:
		friend Network::HttpNetwork;
		friend Network::HttpClientConnection;
		friend Network::HttpServerConnection;
		friend Network::HttpsClientConnection;
		friend Network::HttpsServerConnection;
		float priority;
		void* userData;
		boost::asio::streambuf mstrHttpRequest;
		HttpConnectionPtr mspServerConnection;
		bool mbUseSSL;
	};
	typedef boost::shared_ptr<HttpRequest> HttpRequestPtr;

	inline void HttpRequest::setUserData(void* pUserData)
	{
		this->userData = pUserData;
	}
	inline void* HttpRequest::getUserData()
	{
		return userData;
	}
	inline void HttpRequest::setPriority(float fPriority)
	{
		priority = fPriority;
	}
	inline float HttpRequest::getPriority()
	{
		return priority;
	}
	inline HttpConnectionPtr HttpRequest::getServerConnection()
	{
		return mspServerConnection;
	}
	inline void HttpRequest::useSSL(bool bUseSSL)
	{
		mbUseSSL = bUseSSL;
	}
	inline bool HttpRequest::isUseSSL()
	{
		return mbUseSSL;
	}
}

#endif//__HttpRequest_h__