#ifndef __HttpServerConnection_h__
#define __HttpServerConnection_h__

#include "HttpConnection.h"

namespace Network
{
	class HttpServerConnection : public HttpConnection
	{
	public:
		HttpServerConnection(boost::asio::io_service& io_service,
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
		virtual void handleHandshake(const boost::system::error_code& err){}
		virtual void handleReadRequestLineAndHeaders(const boost::system::error_code& err,
			std::size_t bytes_transferred);
		virtual void handleReadMessageBody(const boost::system::error_code& err,
			std::size_t bytes_transferred);
		virtual void handleResponeSent(const boost::system::error_code& err,
			std::size_t bytes_transferred);
		void checkTimeout();
		virtual void sendRespone(HttpResponePtr spRespone);
		

	private:
		friend class HttpRespone;
		unsigned int muiResponeStringSize;
		boost::asio::ip::tcp::socket mkSocket;
	};
	inline bool HttpServerConnection::canBeClose()
	{
		return ((getStatus() & Status::error_occur) > 0 ||
			(getStatus() & Status::network_timeout) > 0);
	}
	inline bool HttpServerConnection::isRequestProcessed()
	{
		return ((getStatus() & Status::request_processed) > 0);
	}
	inline HttpRequestPtr HttpServerConnection::getRequest()
	{
		if ((getStatus() & Status::requested) > 0)
		{
			return mspRequest;
		}
		HttpRequestPtr mspRequest;
		return mspRequest;
	}
	inline bool HttpServerConnection::isResponseSent()
	{
		return ((getStatus() & Status::response_sent) > 0);
	}
	inline std::string HttpServerConnection::getRemoteIP()
	{
		boost::system::error_code kErrorCode;
		return mkSocket.remote_endpoint(kErrorCode).address().to_string();
	}
	inline boost::asio::ip::tcp::socket& HttpServerConnection::getSocket()
	{
		return mkSocket;
	}
}


#endif//__HttpServerConnection_h__