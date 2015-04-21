#include "ServerTestPCH.h"
#include "Utility/MemoryLeakChecker.h"
#ifdef WIN32
//#include <vld/vld.h>
#include <tchar.h>
#include <atlconv.h>
#endif // WIN32

#include "ConCallback.h"
#include "Utility/FrameRate.h"
#include <stdlib.h>
#include "Network/TcpNetwork.h"

bool g_bKeepRunningFlag = true;

#ifdef WIN32
BOOL WINAPI ConsoleHandler(DWORD msgType)
{
    if (msgType == CTRL_C_EVENT)
    {
		printf("Ctrl-C!\n");
		g_bKeepRunningFlag = false;
        return TRUE;
    }
    else if (msgType == CTRL_CLOSE_EVENT)
    {
        printf("Close console window!\n");
        g_bKeepRunningFlag = false;
        /// Note: The system gives you very limited time to exit in this condition
        return TRUE;
    }

    return FALSE;
}
#endif//WIN32

int main(int argc, char* argv[])
{
	char* pcSelfAddress = "0.0.0.0";
	char* pcAddress = pcSelfAddress;
	if (argc > 1)
	{
		pcAddress = argv[1];
	}

	Utility::FrameRate kFrameRate;

	Utility::MemoryLeakChecker kMemoryLeakChecker;

#ifdef WIN32
	SetConsoleCtrlHandler(ConsoleHandler, TRUE);
#endif//WIN32

	Network::TcpNetwork kTcpNetwork;
	
	ConCallback kConCallback(&kTcpNetwork);
	kTcpNetwork.bindOnConnected(boost::bind(&ConCallback::onConFunc, &kConCallback, _1));
	kTcpNetwork.bindOnDisconnected(boost::bind(&ConCallback::onDisConFunc, &kConCallback, _1));
	kTcpNetwork.bindOnRecved(boost::bind(&ConCallback::onRecvFunc, &kConCallback, _1, _2, _3));
	//kTcpNetwork.bindOnPacket(boost::bind(&ConCallback::onPacketFunc, &kConCallback, _1, _2, _3));
	kTcpNetwork.bindOnSent(boost::bind(&ConCallback::onSentFunc, &kConCallback, _1, _2));
	kTcpNetwork.bindOnError(boost::bind(&ConCallback::errorHandleFunc, &kConCallback, _1, _2));

	Network::TcpNetworkParams kParams = Network::TcpNetworkParams::Default();
	kParams.uiSendBuffSize = 100 * 1024;
	kParams.uiRecvBuffSize = 100 * 1024;
	kParams.uiIOThreadCount = 0;
	kParams.uiListenConnectionNum = 5;
	kParams.uiMaxConnectionWriteSize = 40 * 1024 + 4;
	kParams.uiExtraBufferCount = 0;
	kParams.uiPreCreateConCnt = 10;
	kParams.uiMaxPoolConCnt = 10;
	kParams.bCloseOnBufferOverflow = false;
	kParams.eDataProcessMethod = Network::DPM_Stream;
	//kParams.eDataProcessMethod = Network::DPM_Packet;

	kParams.bUseSSL = true;
	kParams.uiSSLOption = boost::asio::ssl::context::default_workarounds
		| boost::asio::ssl::context::no_sslv2
		| boost::asio::ssl::context::single_dh_use;
	kParams.pfnPwdCallback = boost::bind(&ConCallback::getPassword, &kConCallback);
	kParams.strCertificateChainFile = "crt.pem";
	kParams.strPrivateKeyFile = "key.pem";
	kParams.strDHParamFile = "dhparam.pem";
	kParams.strCipherList = "RC4-MD5";

	//kParams.uiVerifyMode = 

	if (kTcpNetwork.startup(kParams))
	{
		std::cout << "tcp server start up, listening " << pcAddress << ":7890" << std::endl;
		kTcpNetwork.listen(pcAddress, 7890);
		while (g_bKeepRunningFlag)
		{
			boost::this_thread::sleep(boost::posix_time::millisec(0));
			kFrameRate.stat();
			kFrameRate.showOnWindowTitle(1e3);
			kTcpNetwork.running();
		}
	}
	while (!kTcpNetwork.tryShutdown());

	std::cout << "tcp server shutdown" << std::endl;

	kConCallback.outputStats();

	system("pause");
	return 0;
}
