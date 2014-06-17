#ifndef __HttpNetworkErrorCode_h__
#define __HttpNetworkErrorCode_h__

#include <boost/system/error_code.hpp>
#include <boost/function.hpp>
#include <boost/asio/ssl.hpp>

namespace Network
{
	/// HTTP¥ÌŒÛ¬Î∂®“Â
	/// @{
	enum HttpError
	{
		invalid_response = 1,
		async_transfer_zero_bytes = 2,
		async_transfer_error_bytes = 3,
		network_timeout = 4,
		invalid_request = 5,
		send_response_error = 6,
	};

	class HttpErrorCategoryImpl
		: public boost::system::error_category
	{
	public:
		virtual const char* name() const;
		virtual std::string message(int ev) const;
	};

	const boost::system::error_category& HttpErrorCategory();
	boost::system::error_code make_error_code(HttpError e);
	boost::system::error_condition make_error_condition(HttpError e);
	/// @}
}

namespace boost
{
	namespace system
	{
		template<> struct is_error_code_enum<Network::HttpError>
		{
			static const bool value = true;
		};
	}
}

#endif// __HttpNetworkErrorCode_h__