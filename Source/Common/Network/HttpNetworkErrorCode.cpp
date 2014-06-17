#include "NetworkPCH.h"
#include "HttpNetworkErrorCode.h"

/// @see http://blog.think-async.com/2010/04/system-error-support-in-c0x-part-4.html

namespace Network
{
	const char* HttpErrorCategoryImpl::name() const
	{
		return "NetworkDefine_HttpErrorCategoryImpl";
	}
	std::string HttpErrorCategoryImpl::message(int ev) const
	{
		switch (ev)
		{
		case invalid_response:
			return "Invalid response";
		case async_transfer_zero_bytes:
			return "Asynchronous transfer zero bytes";
		case async_transfer_error_bytes:
			return "Asynchronous transfer error bytes";
		case network_timeout:
			return "Network time out";
		case invalid_request:
			return "Invalid request";
		case send_response_error:
			return "Send response error";
		default:
			return "Unknown http error";
		}
	}
	const boost::system::error_category& HttpErrorCategory()
	{
		static HttpErrorCategoryImpl kInstance;
		return kInstance;
	}
	boost::system::error_code make_error_code(HttpError e)
	{
		return boost::system::error_code(
			static_cast<int>(e),
			HttpErrorCategory());
	}
	boost::system::error_condition make_error_condition(HttpError e)
	{
		return boost::system::error_condition(
			static_cast<int>(e),
			HttpErrorCategory());
	}
}