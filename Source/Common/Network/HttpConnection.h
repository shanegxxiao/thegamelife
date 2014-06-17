#ifndef __HttpConnection_h__
#define __HttpConnection_h__

#include <iostream>
#include <istream>
#include <ostream>
#include <string>
#include <boost/smart_ptr.hpp>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include "HttpRequest.h"
#include "HttpRespone.h"

#pragma warning(disable:4482)

namespace Network
{
	class HttpConnection : public boost::enable_shared_from_this<HttpConnection>
	{
	public:
		enum Status
		{
			resolving = 0x1,
			connecting = 0x2,
			write_request = 0x4,
			read_status_line_and_headers = 0x8,
			read_content = 0x10,
			error_occur = 0x20,
			network_timeout = 0x40,
			responed = 0x80,
			closed = 0x100,
			read_request_line_and_headers = 0x200,
			ssl_hand_shake = 0x400,
			requested = 0x800,
			request_processed = 0x1000,
			waiting_response = 0x2000,
			pending_response = 0x4000,
			sending_response = 0x8000,
			response_sent = 0x10000
		};

	public:
		HttpConnection(boost::asio::io_service& io_service,
			HttpNetworkParams& kParam);
		virtual ~HttpConnection();
		virtual void open(HttpRequestPtr spRequest) = 0;
		virtual void running() = 0;
		virtual void close() = 0;
		virtual bool canBeClose() = 0;
		virtual std::string getRemoteIP() = 0;
		virtual boost::asio::ip::tcp::socket& getSocket() = 0;
		virtual void reset();
		virtual bool isClosed();
		virtual bool isTimeout();
		virtual unsigned int getStatus();
		virtual void setStatus(unsigned int uiStatus);
		virtual boost::system::error_code getLastError();
		virtual void setLastError(boost::system::error_code kError);
		virtual void clearLastError();
		virtual HttpRequestPtr getRequest();
		virtual std::string getRequestString();
		virtual HttpResponePtr getResponed();
		virtual std::string getRespondString();

	public:
		/// client connection
		/// @{
		virtual void handleResolve(const boost::system::error_code& err,
			boost::asio::ip::tcp::resolver::iterator endpoint_iterator){BOOST_ASSERT(false);}
		virtual void handleConnect(const boost::system::error_code& err){BOOST_ASSERT(false);}
		virtual void handleWriteRequest(const boost::system::error_code& err,
			std::size_t bytes_transferred){BOOST_ASSERT(false);}
		virtual void handleReadStatusLineAndHeaders(const boost::system::error_code& err,
			std::size_t bytes_transferred){BOOST_ASSERT(false);}
		/// @}
		/// server connection
		/// @{
		virtual bool isRequestProcessed(){BOOST_ASSERT(false);return false;}
		virtual bool isResponseSent(){BOOST_ASSERT(false);return false;}
		virtual void handleReadRequestLineAndHeaders(const boost::system::error_code& err,
			std::size_t bytes_transferred){BOOST_ASSERT(false);}
		virtual void handleResponeSent(const boost::system::error_code& err,
			std::size_t bytes_transferred){BOOST_ASSERT(false);}
		virtual void sendRespone(HttpResponePtr spRespone){BOOST_ASSERT(false);}
		/// @}
		/// server-client connection common used
		/// @{
		virtual void handleHandshake(const boost::system::error_code& err){BOOST_ASSERT(false);}
		virtual void handleReadMessageBody(const boost::system::error_code& err,
			std::size_t bytes_transferred){BOOST_ASSERT(false);}
		/// @}

		void clearStatus();

	protected:
		friend Network::HttpRequest;
		friend Network::HttpRespone;
		HttpNetworkParams& mkNetworkParams;
		boost::asio::ip::tcp::resolver mkResolver;
		boost::posix_time::ptime mkOpenTick;
		unsigned int muiStatus;
		boost::system::error_code mkLastErrorCode;
		/// connection close 里不能释放，需要在reset里释放，防止error handler的时候找不到必要信息
		/// @{
		HttpRequestPtr mspRequest;
		HttpResponePtr mspRespone;
		boost::asio::streambuf mstrRequest;
		boost::asio::streambuf mstrResponse;
		/// @}
	};
	typedef boost::shared_ptr<HttpConnection> HttpConnectionPtr;

	inline bool HttpConnection::isClosed()
	{
		return ((getStatus() & Status::closed) > 0);
	}
	inline bool HttpConnection::isTimeout()
	{
		return ((getStatus() & Status::network_timeout) > 0);
	}
	inline unsigned int HttpConnection::getStatus()
	{
		return muiStatus;
	}
	inline boost::system::error_code HttpConnection::getLastError()
	{
		if ((getStatus() & Status::responed) > 0)
		{
			boost::system::error_code kErrorCode;
			return kErrorCode;
		}
		return mkLastErrorCode;
	}
	inline void HttpConnection::setLastError(boost::system::error_code kError)
	{
		mkLastErrorCode = kError;
	}
	inline void HttpConnection::clearLastError()
	{
		mkLastErrorCode.clear();
	}
	inline HttpRequestPtr HttpConnection::getRequest()
	{
		return mspRequest;
	}
	inline std::string HttpConnection::getRequestString()
	{
		if (mspRequest)
		{
			return mspRequest->toString();
		}
		std::string strRes;
		return strRes;
	}
	inline HttpResponePtr HttpConnection::getResponed()
	{
		return mspRespone;
	}
	inline std::string HttpConnection::getRespondString()
	{
		if (mspRespone)
		{
			return mspRespone->toString();
		}
		std::string strRes;
		return strRes;
	}
	inline void HttpConnection::clearStatus()
	{
		muiStatus = 0;
	}
}

#endif // __HttpConnection_h__