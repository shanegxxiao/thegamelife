#include "NetworkPCH.h"
#include "HttpServerConnection.h"
#include <iostream>
#include <istream>
#include <ostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/algorithm/string.hpp>

namespace Network
{
	HttpServerConnection::HttpServerConnection(boost::asio::io_service& io_service,
		HttpNetworkParams& kParam)
		: HttpConnection(io_service, kParam)
		, mkSocket(io_service)
	{

	}
	void HttpServerConnection::open(HttpRequestPtr spRequest)
	{
		mspRequest = spRequest;
		mspRequest->mspServerConnection = shared_from_this();
		mkOpenTick = boost::posix_time::microsec_clock::universal_time();
		setStatus(Status::read_request_line_and_headers);
		boost::asio::async_read_until(mkSocket, mstrRequest, "\r\n\r\n",
			boost::bind(&HttpConnection::handleReadRequestLineAndHeaders,
			shared_from_this(),
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred));
	}
	void HttpServerConnection::running()
	{
		checkTimeout();
	}
	void HttpServerConnection::close()
	{
		if (mkSocket.is_open())
		{
			boost::system::error_code kErrorCode;
			mkSocket.cancel(kErrorCode);
			mkSocket.shutdown(boost::asio::socket_base::shutdown_both, kErrorCode);
			mkSocket.close(kErrorCode);
		}
		setStatus(Status::closed);
	}
	void HttpServerConnection::handleReadRequestLineAndHeaders(const boost::system::error_code& err,
		std::size_t bytes_transferred)
	{
		if (bytes_transferred <= 0)
		{
			setStatus(Status::error_occur);
			setLastError(Network::async_transfer_zero_bytes);
			return;
		}
		std::istream request_stream(&mstrRequest);
		/// read status line
		/// @{
		request_stream >> mspRequest->method;
		request_stream >> mspRequest->uri;
		std::getline(request_stream, mspRequest->http_version);
		boost::trim(mspRequest->http_version);
		/// Check that request is OK.
		if (!request_stream || mspRequest->http_version.substr(0, 
			Network::HttpMiscStrings::http_length) != Network::HttpMiscStrings::http)
		{
			setStatus(Status::error_occur);
			setLastError(Network::invalid_request);
			return;
		}
		/// @}

		/// read headers
		/// @{
		std::string header;
		while (std::getline(request_stream, header) && header != "\r")
		{
			std::string::size_type nPos = header.find_first_of(Network::HttpMiscStrings::colon);
			std::string name = header.substr(0, nPos);
			std::string value = header.substr(nPos + 1, header.length() - nPos - 1);
			boost::trim(name);
			boost::trim(value);
			mspRequest->putHeaderField(name, value);
		}
		/// @}

		/// read content
		/// @{
		if (mstrRequest.size() > 0)
		{
			mspRequest->content += std::string(
				boost::asio::buffers_begin(mstrRequest.data()),
				boost::asio::buffers_begin(mstrRequest.data()) + mstrRequest.size());
			mstrRequest.consume(mstrRequest.size());
		}
		if (err)
		{
			setStatus(Status::error_occur);
			setLastError(err);
			return;
		}
		if (mspRequest->getContentLength() <= mspRequest->content.length())
		{
			setStatus(Status::requested);
			clearLastError();
			return;
		}
		/// Start reading remaining data until EOF or error occur.
		setStatus(Status::read_content);
		boost::asio::async_read(mkSocket, mstrRequest,
			boost::asio::transfer_at_least(1),
			boost::bind(&HttpConnection::handleReadMessageBody,
			shared_from_this(),
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred));
		/// @}
	}
	void HttpServerConnection::handleReadMessageBody(const boost::system::error_code& err,
		std::size_t bytes_transferred)
	{
		if (bytes_transferred <= 0)
		{
			setStatus(Status::error_occur);
			setLastError(Network::async_transfer_zero_bytes);
			return;
		}

		// Write all of the data that has been read so far.
		mspRequest->content += std::string(
			boost::asio::buffers_begin(mstrRequest.data()),
			boost::asio::buffers_begin(mstrRequest.data()) + mstrRequest.size());
		mstrRequest.consume(mstrRequest.size());

		if (mspRequest->getContentLength() <= mspRequest->content.length())
		{
			setStatus(Status::requested);
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
		boost::asio::async_read(mkSocket, mstrRequest,
			boost::asio::transfer_at_least(1),
			boost::bind(&HttpConnection::handleReadMessageBody,
			shared_from_this(),
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred));
	}
	void HttpServerConnection::checkTimeout()
	{
		if ((getStatus() & Status::sending_response) > 0 ||
			(getStatus() & Status::response_sent) > 0)
		{
			return;
		}
		boost::posix_time::millisec_posix_time_system_config::time_duration_type time_elapse = 
			boost::posix_time::microsec_clock::universal_time() - mkOpenTick;
		if (time_elapse.total_seconds() >= (int)mkNetworkParams.uiServerTimeoutSeconds)
		{
			setStatus(Status::network_timeout);
			setLastError(Network::network_timeout);
		}
	}
	void HttpServerConnection::sendRespone(HttpResponePtr spRespone)
	{
		BOOST_ASSERT(mspRespone == spRespone);
		std::ostream respone_stream(&mstrResponse);
		respone_stream << mspRespone->toString();
		muiResponeStringSize = mstrResponse.size();
		setStatus(Status::sending_response);
		boost::asio::async_write(mkSocket, mstrResponse,
			boost::bind(&HttpConnection::handleResponeSent,
			shared_from_this(),
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred));
	}
	void HttpServerConnection::handleResponeSent(const boost::system::error_code& err,
		std::size_t bytes_transferred)
	{
		if (err || bytes_transferred != muiResponeStringSize)
		{
			setLastError(Network::send_response_error);
			setStatus(Status::error_occur);
			return;
		}
		clearLastError();
		setStatus(Status::response_sent);
	}
}