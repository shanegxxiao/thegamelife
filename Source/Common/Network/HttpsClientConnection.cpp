#include "NetworkPCH.h"
#include <iostream>
#include <istream>
#include <ostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/algorithm/string.hpp>
#include "HttpsClientConnection.h"

namespace Network
{
	HttpsClientConnection::HttpsClientConnection(boost::asio::io_service& io_service,
		boost::asio::ssl::context& kSSLContext,
		HttpNetworkParams& kParam)
		: HttpConnection(io_service, kParam)
		, mkSSLSocket(io_service, kSSLContext)
	{
	}
	void HttpsClientConnection::close()
	{
		mkResolver.cancel();
		if (mkSSLSocket.next_layer().is_open())
		{
			boost::system::error_code kErrorCode;
			mkSSLSocket.next_layer().cancel(kErrorCode);
			mkSSLSocket.next_layer().shutdown(boost::asio::socket_base::shutdown_both, kErrorCode);
			mkSSLSocket.next_layer().close(kErrorCode);
		}
		setStatus(Status::closed);
	}
	void HttpsClientConnection::running()
	{
		checkTimeout();
	}
	void HttpsClientConnection::open(HttpRequestPtr spRequest)
	{
		mspRequest = spRequest;
		mkOpenTick = boost::posix_time::microsec_clock::universal_time();
		// Start an asynchronous resolve to translate the server and service names
		// into a list of endpoints.
		std::ostream request_stream(&mstrRequest);
		request_stream << spRequest->toString();

		std::string strAddress = spRequest->headers["Host"];
		std::string::size_type colonPos = strAddress.find(":");
		std::string strHost = strAddress;
		std::string strPort = "http";
		if (colonPos != std::string::npos)
		{
			strHost = strAddress.substr(0, colonPos);
			strPort = strAddress.substr(colonPos + 1, strAddress.length() - colonPos - 1);
		}
		
		setStatus(Status::resolving);
		boost::asio::ip::tcp::resolver::query query(strHost, strPort);
		mkResolver.async_resolve(query,
			boost::bind(&HttpConnection::handleResolve, shared_from_this(),
			boost::asio::placeholders::error,
			boost::asio::placeholders::iterator));
	}
	void HttpsClientConnection::handleResolve(const boost::system::error_code& err,
		boost::asio::ip::tcp::resolver::iterator endpoint_iterator)
	{
		if (err)
		{
			setStatus(Status::error_occur);
			setLastError(err);
			return;
		}
		// Attempt a connection to each endpoint in the list until we
		// successfully establish a connection.
		setStatus(Status::connecting);
		boost::asio::async_connect(mkSSLSocket.next_layer(), endpoint_iterator,
			boost::bind(&HttpConnection::handleConnect, shared_from_this(),
			boost::asio::placeholders::error));
	}

	void HttpsClientConnection::handleConnect(const boost::system::error_code& err)
	{
		if (err)
		{
			setStatus(Status::error_occur);
			setLastError(err);
			return;
		}
		muiRequestStringSize = mstrRequest.size();
		setStatus(Status::ssl_hand_shake);
		mkSSLSocket.async_handshake(boost::asio::ssl::stream_base::client,
			boost::bind(&HttpConnection::handleHandshake, shared_from_this(),
			boost::asio::placeholders::error));
	}
	void HttpsClientConnection::handleHandshake(const boost::system::error_code& err)
	{
		if (err)
		{
			setStatus(Status::error_occur);
			setLastError(err);
			return;
		}
		// The connection was successful. Send the request.
		setStatus(Status::write_request);
		boost::asio::async_write(mkSSLSocket, mstrRequest,
			boost::bind(&HttpConnection::handleWriteRequest,
			shared_from_this(),
			boost::asio::placeholders::error, 
			boost::asio::placeholders::bytes_transferred));
	}
	void HttpsClientConnection::handleWriteRequest(const boost::system::error_code& err,
		std::size_t bytes_transferred)
	{
		if (err)
		{
			setStatus(Status::error_occur);
			setLastError(err);
			return;
		}
		if (muiRequestStringSize != bytes_transferred)
		{
			setStatus(Status::error_occur);
			setLastError(Network::async_transfer_error_bytes);
			return;
		}

		// Read the response status line. The response_ streambuf will
		// automatically grow to accommodate the entire line. The growth may be
		// limited by passing a maximum size to the streambuf constructor.
		mstrResponse.consume(mstrResponse.size());
		setStatus(Status::read_status_line_and_headers);
		boost::asio::async_read_until(mkSSLSocket, mstrResponse, "\r\n\r\n",
			boost::bind(&HttpConnection::handleReadStatusLineAndHeaders,
			shared_from_this(),
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred));
		
	}
	void HttpsClientConnection::handleReadStatusLineAndHeaders(const boost::system::error_code& err,
		std::size_t bytes_transferred)
	{
		if (bytes_transferred <= 0)
		{
			setStatus(Status::error_occur);
			setLastError(Network::async_transfer_zero_bytes);
			return;
		}

		mspRespone.reset(new HttpRespone);
		mspRespone->setRequest(mspRequest);
		mspRespone->setRemoteIP(getRemoteIP());
		
		std::istream response_stream(&mstrResponse);
		/// read status line
		/// @{
		response_stream >> mspRespone->http_version;
		response_stream >> mspRespone->status_code;
		std::getline(response_stream, mspRespone->status_message);
		/// Check that response is OK.
		if (!response_stream || mspRespone->http_version.substr(0, 
			Network::HttpMiscStrings::http_length) != Network::HttpMiscStrings::http)
		{
			setStatus(Status::error_occur);
			setLastError(Network::invalid_response);
			return;
		}
		/// @}

		/// read headers
		/// @{
		std::string header;
		while (std::getline(response_stream, header) && header != "\r")
		{
			std::string::size_type nPos = header.find_first_of(Network::HttpMiscStrings::colon);
			std::string name = header.substr(0, nPos);
			std::string value = header.substr(nPos + 1, header.length() - nPos - 1);
			boost::trim(name);
			boost::trim(value);
			mspRespone->putHeaderField(name, value);
		}
		/// @}

		/// read content
		/// @{
		if (mstrResponse.size() > 0)
		{
			mspRespone->content += std::string(
				boost::asio::buffers_begin(mstrResponse.data()),
				boost::asio::buffers_begin(mstrResponse.data()) + mstrResponse.size());
			mstrResponse.consume(mstrResponse.size());
		}
		if (err)
		{
			setStatus(Status::error_occur);
			setLastError(err);
			return;
		}
		if (mspRespone->getContentLength() <= mspRespone->content.length())
		{
			setStatus(Status::responed);
			clearLastError();
			return;
		}
		/// Start reading remaining data until EOF or error occur.
		setStatus(Status::read_content);
		boost::asio::async_read(mkSSLSocket, mstrResponse,
			boost::asio::transfer_at_least(1),
			boost::bind(&HttpConnection::handleReadMessageBody,
			shared_from_this(),
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred));
		/// @}
	}
	void HttpsClientConnection::handleReadMessageBody(const boost::system::error_code& err,
		std::size_t bytes_transferred)
	{
		if (bytes_transferred <= 0)
		{
			setStatus(Status::error_occur);
			setLastError(Network::async_transfer_zero_bytes);
			return;
		}

		// Write all of the data that has been read so far.
		mspRespone->content += std::string(
			boost::asio::buffers_begin(mstrResponse.data()),
			boost::asio::buffers_begin(mstrResponse.data()) + mstrResponse.size());
		mstrResponse.consume(mstrResponse.size());

		if (mspRespone->getContentLength() <= mspRespone->content.length())
		{
			setStatus(Status::responed);
			clearLastError();
			return;
		}

		if (err)
		{
			setStatus(Status::error_occur);
			setLastError(err);
			return;
		}

		// Continue reading remaining data until EOF.
		setStatus(Status::read_content);
		boost::asio::async_read(mkSSLSocket, mstrResponse,
		boost::asio::transfer_at_least(1),
		boost::bind(&HttpConnection::handleReadMessageBody,
		shared_from_this(),
		boost::asio::placeholders::error,
		boost::asio::placeholders::bytes_transferred));
	}
	void HttpsClientConnection::checkTimeout()
	{
		if ((getStatus() & Status::responed) > 0)
		{
			return;
		}
		boost::posix_time::millisec_posix_time_system_config::time_duration_type time_elapse = 
			boost::posix_time::microsec_clock::universal_time() - mkOpenTick;
		if (time_elapse.total_seconds() >= (int)mkNetworkParams.uiClientTimeoutSeconds)
		{
			setStatus(Status::network_timeout);
			setLastError(Network::network_timeout);
		}
	}
}