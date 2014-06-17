#ifndef __NetworkCommonDefine_h__
#define __NetworkCommonDefine_h__

namespace Network
{
	typedef boost::function<std::string (
		std::size_t max_length,  // The maximum size for a password.
		boost::asio::ssl::context_base::password_purpose purpose // Whether password is for reading or writing.
		)> SSLPasswordCallback;
	typedef boost::function<bool (bool preverified, 
		boost::asio::ssl::verify_context& ctx)> SSLVerifyCallback;
}

#endif//__NetworkCommonDefine_h__