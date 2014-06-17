#include "NetworkPCH.h"
#include "HttpNetworkParams.h"

namespace Network
{
	const HttpNetworkParams& HttpNetworkParams::Default()
	{
		static HttpNetworkParams skHttpNetworkParams;
		skHttpNetworkParams.strListenAddress = "0.0.0.0";
		skHttpNetworkParams.uiListenPort = 80;
		skHttpNetworkParams.uiListenConnectionNum = 5;
		skHttpNetworkParams.uiPreCreateConCnt = 100;
		skHttpNetworkParams.uiMaxPoolConCnt = 100;
		skHttpNetworkParams.uiIOThreadCount = 0;
		skHttpNetworkParams.uiClientTimeoutSeconds = 600;
		skHttpNetworkParams.uiServerTimeoutSeconds = 1200;
		skHttpNetworkParams.uiRequestSendNumPerFrame = 1000;
		skHttpNetworkParams.uiResponeProcessNumPerFrame = 1000;
		skHttpNetworkParams.uiRequestProcessNumPerFrame = 1000;
		skHttpNetworkParams.uiResponeSendNumPerFrame = 1000;
		skHttpNetworkParams.bUseSSL = false;
		skHttpNetworkParams.eSSLMethod = boost::asio::ssl::context_base::sslv23;
		skHttpNetworkParams.bUseDefaultVerifyPaths = true;
		skHttpNetworkParams.uiSSLOption = 0;
		skHttpNetworkParams.uiVerifyMode = 0;
		return skHttpNetworkParams;
	}

}