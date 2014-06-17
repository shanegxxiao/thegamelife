#include "NetworkPCH.h"
#include "TcpNetworkParams.h"

namespace Network
{
	const TcpNetworkParams& TcpNetworkParams::Default()
	{
		static TcpNetworkParams skTcpNetworkParams;
		skTcpNetworkParams.uiListenConnectionNum = 5;
		skTcpNetworkParams.uiSendBuffSize = 32 * 1024;
		skTcpNetworkParams.uiRecvBuffSize = 32 * 1024;
		skTcpNetworkParams.uiPreCreateConCnt = 100;
		skTcpNetworkParams.uiMaxPoolConCnt = 100;
		skTcpNetworkParams.uiIOThreadCount = 0;
		skTcpNetworkParams.bCloseOnBufferOverflow = false;
		skTcpNetworkParams.uiMaxConnectionWriteSize = 8 * 1024;
		skTcpNetworkParams.uiExtraBufferCount = 0;
		skTcpNetworkParams.bUseSSL = false;
		skTcpNetworkParams.eSSLMethod = boost::asio::ssl::context_base::sslv23;
		skTcpNetworkParams.bUseDefaultVerifyPaths = false;
		skTcpNetworkParams.uiSSLOption = 0;
		skTcpNetworkParams.uiVerifyMode = 0;
		skTcpNetworkParams.eDataProcessMethod = DPM_Packet;
		skTcpNetworkParams.iPacketProcessNumLimit = -1;
		skTcpNetworkParams.bKeepAlive = false;
		skTcpNetworkParams.ulKeepAliveTime = 5000;
		skTcpNetworkParams.ulKeepAliveInterval = 5000;
		return skTcpNetworkParams;
    }
}