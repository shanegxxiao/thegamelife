#ifndef __HttpClientConnection_h__
#define __HttpClientConnection_h__
#include "HttpConnection.h"

namespace Network
{
	class HttpClientConnection : public HttpConnection
	{
	public:
		HttpClientConnection(boost::asio::io_service& io_service,
			HttpNetworkParams& kParam);
		virtual void open(HttpRequestPtr spRequest);
		virtual void running();
		virtual void close();
		virtual bool canBeClose();
		virtual HttpResponePtr getResponed();
		virtual std::string getRemoteIP();
		virtual boost::asio::ip::tcp::socket& getSocket();

	private:
		virtual void handleResolve(const boost::system::error_code& err,
		  boost::asio::ip::tcp::resolver::iterator endpoint_iterator);
		virtual void handleConnect(const boost::system::error_code& err);
		virtual void handleWriteRequest(const boost::system::error_code& err,
			std::size_t bytes_transferred);
		virtual void handleReadStatusLineAndHeaders(const boost::system::error_code& err,
			std::size_t bytes_transferred);
		virtual void handleReadMessageBody(const boost::system::error_code& err,
			std::size_t bytes_transferred);
		void checkTimeout();

	private:
		boost::asio::ip::tcp::socket mkSocket;
		unsigned int muiRequestStringSize;
	};

	inline bool HttpClientConnection::canBeClose()
	{
		return ((getStatus() & Status::error_occur) > 0 ||
			(getStatus() & Status::network_timeout) > 0);
	}
	inline HttpResponePtr HttpClientConnection::getResponed()
	{
		if ((getStatus() & Status::responed) > 0)
		{
			return mspRespone;
		}
		HttpResponePtr spRespone;
		return spRespone;
	}
	inline std::string HttpClientConnection::getRemoteIP()
	{
		boost::system::error_code kErrorCode;
		return mkSocket.remote_endpoint(kErrorCode).address().to_string();
	}
	inline boost::asio::ip::tcp::socket& HttpClientConnection::getSocket()
	{
		return mkSocket;
	}
}
#endif//__HttpClientConnection_h__