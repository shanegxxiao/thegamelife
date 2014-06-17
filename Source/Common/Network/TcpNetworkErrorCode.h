#ifndef __TcpNetworkErrorCode_h__
#define __TcpNetworkErrorCode_h__

#include <boost/system/error_code.hpp>
#include <boost/function.hpp>
#include <boost/asio/ssl.hpp>

namespace Network
{
	/// TCP错误码定义
	/// @{
	enum TcpError
	{
		recv_buff_overflow = 1,             ///< 接收缓冲区溢出
		send_buff_overflow = 2,             ///< 发送缓冲区溢出
		accepted_but_failed_when_open = 3,  ///< 已经接受的链接在打开的时候出错，有可能是客户端已经断开
		error_packet_size = 4,              ///< 数据包大小错误
		listen_fail = 5,                    ///< 端口监听失败
		set_keep_alive_option_failed = 6,   ///< 设置keep alive选项参数失败
	};

	class TcpErrorCategoryImpl
		: public boost::system::error_category
	{
	public:
		virtual const char* name() const;
		virtual std::string message(int ev) const;
	};

	const boost::system::error_category& TcpErrorCategory();
	boost::system::error_code make_error_code(TcpError e);
	boost::system::error_condition make_error_condition(TcpError e);
	/// @}
}

namespace boost
{
	namespace system
	{
		template<> struct is_error_code_enum<Network::TcpError>
		{
			static const bool value = true;
		};
	}
}

#endif//__TcpNetworkErrorCode_h__