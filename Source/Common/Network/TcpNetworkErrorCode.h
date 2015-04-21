#ifndef __TcpNetworkErrorCode_h__
#define __TcpNetworkErrorCode_h__

#include <boost/system/error_code.hpp>
#include <boost/function.hpp>
#include <boost/asio/ssl.hpp>

namespace Network
{
	/// TCP�����붨��
	/// @{
	enum TcpError
	{
		recv_buff_overflow = 1,             ///< ���ջ��������
		send_buff_overflow = 2,             ///< ���ͻ��������
		accepted_but_failed_when_open = 3,  ///< �Ѿ����ܵ������ڴ򿪵�ʱ����?�п����ǿͻ����Ѿ��Ͽ�
		error_packet_size = 4,              ///< ��ݰ��С����
		listen_fail = 5,                    ///< �˿ڼ���ʧ��
		set_keep_alive_option_failed = 6,   ///< ����keep aliveѡ�����ʧ��
	};

	class TcpErrorCategoryImpl
		: public boost::system::error_category
	{
	public:
		virtual const char* name() const BOOST_SYSTEM_NOEXCEPT;
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
