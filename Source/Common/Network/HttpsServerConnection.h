#ifndef __HttpsServerConnection_h__
#define __HttpsServerConnection_h__

#include "HttpConnection.h"

namespace Network
{
	class HttpsServerConnection : public HttpConnection
	{
	public:
		HttpsServerConnection(boost::asio::io_service& io_service,
			boost::asio::ssl::context& kSSLContext,
			HttpNetworkParams& kParam);
		virtual void open(HttpRequestPtr spRequest);
		virtual void running();
		virtual void close();
		virtual bool canBeClose();
		virtual HttpRequestPtr getRequest();
		virtual std::string getRemoteIP();
		virtual boost::asio::ip::tcp::socket& getSocket();
		virtual bool isRequestProcessed();
		virtual bool isResponseSent();

	private:
		virtual void handleHandshake(const boost::system::error_code& err);
		virtual void handleReadRequestLineAndHeaders(const boost::system::error_code& err,
			std::size_t bytes_transferred);
		virtual void handleReadMessageBody(const boost::system::error_code& err,
			std::size_t bytes_transferred);
		virtual void handleResponeSent(const boost::system::error_code& err,
			std::size_t bytes_transferred);
		virtual void sendRespone(HttpResponePtr spRespone);
		void checkTimeout();
		
		

	private:
		typedef boost::asio::ssl::stream<boost::asio::ip::tcp::socket> SSLSocket;
		friend class HttpRespone;
		unsigned int muiResponeStringSize;
		SSLSocket mkSSLSocket;
	};
	typedef boost::shared_ptr<HttpsServerConnection> HttpsServerConnectionPtr;
	inline bool HttpsServerConnection::canBeClose()
	{
		return ((getStatus() & Status::error_occur) > 0 ||
			(getStatus() & Status::network_timeout) > 0);
	}
	inline bool HttpsServerConnection::isRequestProcessed()
	{
		return ((getStatus() & Status::request_processed) > 0);
	}
	inline HttpRequestPtr HttpsServerConnection::getRequest()
	{
		if ((getStatus() & Status::requested) > 0)
		{
			return mspRequest;
		}
		HttpRequestPtr mspRequest;
		return mspRequest;
	}
	inline bool HttpsServerConnection::isResponseSent()
	{
		return ((getStatus() & Status::response_sent) > 0);
	}
	inline std::string HttpsServerConnection::getRemoteIP()
	{
		boost::system::error_code kErrorCode;
		return mkSSLSocket.next_layer().remote_endpoint(kErrorCode).address().to_string();
	}
	inline boost::asio::ip::tcp::socket& HttpsServerConnection::getSocket()
	{
		return mkSSLSocket.next_layer();
	}
}


#endif//__HttpsServerConnection_h__