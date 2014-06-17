#ifndef __HttpsClientConnection_h__
#define __HttpsClientConnection_h__
#include "HttpConnection.h"

namespace Network
{
	class HttpsClientConnection : public HttpConnection
	{
	public:
		HttpsClientConnection(boost::asio::io_service& io_service,
			boost::asio::ssl::context& kSSLContext,
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
		virtual void handleHandshake(const boost::system::error_code& err);
		virtual void handleWriteRequest(const boost::system::error_code& err,
			std::size_t bytes_transferred);
		virtual void handleReadStatusLineAndHeaders(const boost::system::error_code& err,
			std::size_t bytes_transferred);
		virtual void handleReadMessageBody(const boost::system::error_code& err,
			std::size_t bytes_transferred);
		void checkTimeout();


	private:
		typedef boost::asio::ssl::stream<boost::asio::ip::tcp::socket> SSLSocket;
		unsigned int muiRequestStringSize;
		SSLSocket mkSSLSocket;
	};
	typedef boost::shared_ptr<HttpsClientConnection> HttpsClientConnectionPtr;

	inline bool HttpsClientConnection::canBeClose()
	{
		return ((getStatus() & Status::error_occur) > 0 ||
			(getStatus() & Status::network_timeout) > 0);
	}
	inline HttpResponePtr HttpsClientConnection::getResponed()
	{
		if ((getStatus() & Status::responed) > 0)
		{
			return mspRespone;
		}
		HttpResponePtr spRespone;
		return spRespone;
	}
	inline std::string HttpsClientConnection::getRemoteIP()
	{
		boost::system::error_code kErrorCode;
		return mkSSLSocket.next_layer().remote_endpoint(kErrorCode).address().to_string();
	}
	inline boost::asio::ip::tcp::socket& HttpsClientConnection::getSocket()
	{
		return mkSSLSocket.next_layer();
	}
}
#endif//__HttpsClientConnection_h__