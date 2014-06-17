#ifndef __HttpNetworkParams_h__
#define __HttpNetworkParams_h__

#include <boost/system/error_code.hpp>
#include <boost/function.hpp>
#include <boost/asio/ssl.hpp>
#include "NetworkCommonDefine.h"

namespace Network
{
	typedef std::map<std::string, std::string> Headers;

	namespace HttpMiscStrings
	{
		static const char colon = ':';
		static const char dot = '.';
		static const char space = ' ';
		static const char* name_value_separator = ": ";
		static const unsigned int name_value_separator_length = 2;
		static const char* http = "HTTP/";
		static const unsigned int http_length = 5;
		static const char* crlf = "\r\n";
		static const unsigned int crlf_length = 2;
		static const char* version = "1.1";
		static const unsigned int version_length = 3;
	}// namespace misc_strings

	namespace HttpRequestPriority
	{
		const float highest = 0.f;
		const float normal = 1.f;
		const float lowest = 2.f;
	}

	struct HttpNetworkParams 
	{
		std::string strListenAddress;///< http服务监听的地址
		unsigned int uiListenPort;///< http服务监听的端口
		unsigned int uiListenConnectionNum;///< 监听socket个数
		unsigned int uiPreCreateConCnt;///< 预初始化的链接个数
		unsigned int uiMaxPoolConCnt;///< 最大缓存链接个数（链接池大小）
		unsigned int uiIOThreadCount;///< IO线程个数，0为使用CPU个数
		unsigned int uiClientTimeoutSeconds;///< 客户端响应超时时长单位秒
		unsigned int uiServerTimeoutSeconds;///< 服务端响应超时时长单位秒
		unsigned int uiRequestSendNumPerFrame;///< 每帧允许发送的请求数量
		unsigned int uiResponeProcessNumPerFrame;///< 每帧允许处理的回应数量
		unsigned int uiRequestProcessNumPerFrame;/// 每帧允许处理的请求数量
		unsigned int uiResponeSendNumPerFrame;///< 每帧允许发送的回应数量

		/// SSL相关参数，bUseSSL为true时后续参数才有意义
		/// @{
		bool bUseSSL;///< 是否使用SSL
		boost::asio::ssl::context_base::method eSSLMethod;
		std::string strCipherList;///< 要使用的加密套件
		unsigned int uiSSLOption;
		unsigned int uiVerifyMode;
		bool bUseDefaultVerifyPaths;
		std::string strVerifyFile;
		SSLVerifyCallback pfnVerifyCallback;
		SSLPasswordCallback pfnPwdCallback;
		std::string strCertificateChainFile;
		std::string strCertificateFile;
		std::string strPrivateKeyFile;
		std::string strRSAPrivateKeyFile;
		std::string strDHParamFile;
		/// @}
		static const HttpNetworkParams& Default();
	};
}

#endif//__HttpNetworkParams_h__