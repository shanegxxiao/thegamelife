#include "NetworkPCH.h"
#include "TcpNetworkErrorCode.h"

/// @see http://blog.think-async.com/2010/04/system-error-support-in-c0x-part-4.html

namespace Network
{
	const char* TcpErrorCategoryImpl::name() const
	{
		return "NetworkDefine_TcpErrorCategoryImpl";
	}

	std::string TcpErrorCategoryImpl::message(int ev) const
	{
		switch (ev)
		{
		case recv_buff_overflow:
			return "Receive buffer overflow";
		case send_buff_overflow:
			return "Send buffer overflow";
		case accepted_but_failed_when_open:
			return "Accepted but failed when open";
		case error_packet_size:
			return "Error packet size";
        case listen_fail:
            return "Listen port failed";
		case set_keep_alive_option_failed:
			return "Set keep alive option failed";
		default:
			return "Unknown TCP error";
		}
	}

	const boost::system::error_category& TcpErrorCategory()
	{
		static TcpErrorCategoryImpl kInstance;
		return kInstance;
	}

	boost::system::error_code make_error_code(TcpError e)
	{
		return boost::system::error_code(
			static_cast<int>(e),
			TcpErrorCategory());
	}

	boost::system::error_condition make_error_condition(TcpError e)
	{
		return boost::system::error_condition(
			static_cast<int>(e),
			TcpErrorCategory());
	}
}