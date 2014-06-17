#ifndef __HttpRespone_h__
#define __HttpRespone_h__
#include <boost/smart_ptr.hpp>
#include <boost/asio/streambuf.hpp>
#include "HttpRequest.h"
#include "HttpNetworkParams.h"
#include "HttpNetworkErrorCode.h"

namespace Network
{
	class HttpNetwork;
	class HttpClientConnection;
	class HttpServerConnection;
	class HttpsClientConnection;
	class HttpsServerConnection;

	class HttpRespone : public boost::enable_shared_from_this<HttpRespone>
	{
	public:
		HttpRespone();
		~HttpRespone();
		std::string toString();
		void putHeaderField(std::string name, std::string value);
		void setPriority(float fPriority);
		float getPriority();
		unsigned int getContentLength();
		HttpRequestPtr getRequest();
		std::string getRemoteIP();

	public:
		std::string http_version;
		unsigned int status_code;
		std::string status_message;
		Headers headers;
		std::string content;

	private:
		void setRemoteIP(std::string& strRemoteIP);
		void setRequest(HttpRequestPtr spRequest);
		void send();

	private:
		friend Network::HttpRequest;
		friend Network::HttpNetwork;
		friend Network::HttpClientConnection;
		friend Network::HttpServerConnection;
		friend Network::HttpsClientConnection;
		friend Network::HttpsServerConnection;
		float priority;
		HttpRequestPtr originalRequestPtr;
		boost::asio::streambuf mstrReply;
		std::string mstrRemoteIP;
	};
	typedef boost::shared_ptr<HttpRespone> HttpResponePtr;

	inline HttpRequestPtr HttpRespone::getRequest()
	{
		return originalRequestPtr;
	}
	inline void HttpRespone::setRequest(HttpRequestPtr spRequest)
	{
		originalRequestPtr = spRequest;
	}
	inline void HttpRespone::setRemoteIP(std::string& strRemoteIP)
	{
		mstrRemoteIP = strRemoteIP;
	}
	inline std::string HttpRespone::getRemoteIP()
	{
		return mstrRemoteIP;
	}
	inline void HttpRespone::putHeaderField(std::string name, std::string value)
	{
		headers[name] = value;
	}
	inline void HttpRespone::setPriority(float fPriority)
	{
		priority = fPriority;
	}
	inline float HttpRespone::getPriority()
	{
		return priority;
	}
}

#endif//__HttpRespone_h__