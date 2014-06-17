#ifndef __TcpNetworkParams_h__
#define __TcpNetworkParams_h__

#include <boost/system/error_code.hpp>
#include <boost/function.hpp>
#include <boost/asio/ssl.hpp>
#include "NetworkCommonDefine.h"

namespace Network
{
	/// TcpNetwork初始化参数
	/// @{
	enum DataProcessMethod
	{
		DPM_Stream,
		DPM_Packet,
		DPM_MAX
	};
	struct TcpNetworkParams 
	{
		unsigned int uiListenConnectionNum;///< 监听socket个数
		unsigned int uiSendBuffSize;///< 发送缓冲区大小
		unsigned int uiRecvBuffSize;///< 接收缓冲区大小
		unsigned int uiPreCreateConCnt;///< 预初始化的链接个数
		unsigned int uiMaxPoolConCnt;///< 最大缓存链接个数（链接池大小）
		unsigned int uiIOThreadCount;///< IO线程个数，0为使用CPU个数
		bool bCloseOnBufferOverflow;///< 是否在发送缓冲区或者接收缓冲区溢出时断开链接
		unsigned int uiMaxConnectionWriteSize;///< 通过@see TcpNetwork::send最大发送的数据大小
		/// 为bCloseOnBufferOverflow为false时缓存发送数据包的内存池大小，
		/// 会申请以uiMaxConnectionWriteSize为大小的内存块，共uiExtraBufferCount这么多个，
		/// 超出uiExtraBufferCount这个大小会再new，所以如果bCloseOnBufferOverflow为true时
		/// 只要内存足够就不会因为缓冲区溢出而断开链接
		/// @{
		unsigned int uiExtraBufferCount;
		/// @}
		DataProcessMethod eDataProcessMethod;///< 数据处理方式，是流还是包的方式
		int iPacketProcessNumLimit;///< 每帧处理的数据包数量限制，-1为不限制
		bool bKeepAlive;///< 是否开启socket的keep alive选项
		/// bKeepAlive为true时以下两个参数才有效
		///@{
		unsigned long ulKeepAliveTime;///< 距上一次发包多久后开始发送keep alive数据包，单位毫秒
		unsigned long ulKeepAliveInterval;///< 发送间隔，单位毫秒
		/// @}


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

		static const TcpNetworkParams& Default();
	};
	/// @}

	/// 链接选项枚举
	/// @{
	enum ConnectionType
	{
		CT_DEFAULT,
		CT_TCP,
		CT_SSL
	};
	/// @}
}

#endif//__TcpNetworkParams_h__