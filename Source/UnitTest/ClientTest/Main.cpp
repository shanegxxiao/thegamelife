#include "ClientTestPCH.h"
#include "Utility/MemoryLeakChecker.h"
#include "Utility/FrameRate.h"
#include <iostream>
#ifdef WIN32
//#include <vld/vld.h>
#endif // WIN32
#include "boost/thread.hpp"
#include "boost/bind.hpp"
#include "boost/random.hpp"
#include "ConCallback.h"
#include "../TestMain/TestCommonDefine.h"
#include <tchar.h>
#include <atlconv.h>

#include <MMSystem.h>
#pragma comment(lib, "Winmm.lib")

#ifdef _DEBUG
#pragma comment(lib, "../../../../Library/Network/Lib/Network_Debug.lib")
#else
#pragma comment(lib, "../../../../Library/Network/Lib/Network_Release.lib")
#endif // _DEBUG

BOOL g_bKeepRunningFlag = 1;
Network::TcpNetwork kTcpNetwork;

BOOL WINAPI ConsoleHandler(DWORD msgType)
{
	if (msgType == CTRL_C_EVENT)
	{
		printf("Ctrl-C!\n");
		g_bKeepRunningFlag = 0;
		return TRUE;
	}
	else if (msgType == CTRL_CLOSE_EVENT)
	{
		printf("Close console window!\n");
		g_bKeepRunningFlag = 0;
		/// Note: The system gives you very limited time to exit in this condition
		return TRUE;
	}

	return FALSE;
}

void sendMsgOnceFunc()
{
	while (g_bKeepRunningFlag)
	{
		//boost::this_thread::sleep(boost::posix_time::milliseconds(1000));
		Network::ConnectionID uID = kTcpNetwork.connect("192.168.1.201", 7890);
		while (uID == INVALID_CONNECTION_ID)
		{
			uID = kTcpNetwork.connect("192.168.1.201", 7890);
		}
		if (uID != INVALID_CONNECTION_ID)
		{
			std::cout << "sendMsgOnceFunc connected, ID = " << uID << std::endl;
			char acBuff[] = "sendMsgOnceFunc hello world.\n";
			kTcpNetwork.send(uID, (unsigned char*)acBuff, strlen(acBuff));
		}
		kTcpNetwork.closeConnection(uID);

		kTcpNetwork.running();
	}
}
void sendMsgRepeatFunc()
{
	std::vector<Network::ConnectionID> kConnectionIDVec;
	for (unsigned int ui = 0; ui < 100; ++ui)
	{
		if (!g_bKeepRunningFlag)
		{
			break;
		}
		Network::ConnectionID uID = kTcpNetwork.connect("192.168.1.201", 7890);
		while (uID == INVALID_CONNECTION_ID)
		{
			uID = kTcpNetwork.connect("192.168.1.201", 7890);
		}
		kConnectionIDVec.push_back(uID);
	}
	char cChar = 'a';
	boost::uniform_int<> kDistribution(1, 512) ;
	boost::mt19937 kEngine ;
	boost::variate_generator<boost::mt19937, boost::uniform_int<> > kRandom(kEngine, kDistribution);

	//int i = 10;
	while (g_bKeepRunningFlag)
	{
		boost::this_thread::sleep(boost::posix_time::milliseconds(200));

		//char acBuff[] = "sendMsgRepeatFunc hello world.\n";
		//kTcpNetwork.send(uID, (unsigned char*)acBuff, strlen(acBuff));

		unsigned int uiSize = kRandom();
		char* pBuff = new char[uiSize];
		for (unsigned int ui = 0; ui < uiSize; ++ui)
		{
			pBuff[ui] = cChar;
			if (cChar == 'z')
			{
				cChar = 'a';
			}
			else
			{
				++cChar;
			}
		}
		for (unsigned int ui = 0; ui < kConnectionIDVec.size(); ++ui)
		{
			kTcpNetwork.send(kConnectionIDVec[ui], (unsigned char*)pBuff, uiSize);
		}
			
		delete[] pBuff;

		kTcpNetwork.running();
	}
}
void sendPacketOnceFunc()
{
	Utility::FrameRate kFrameRate;
	char cChar = 'a';
	boost::uniform_int<> kDistribution(1, 8192);
	boost::mt19937 kEngine;
	boost::variate_generator<boost::mt19937, boost::uniform_int<> > kRandom(kEngine, kDistribution);

	while (g_bKeepRunningFlag)
	{
		//boost::this_thread::sleep(boost::posix_time::milliseconds(1000));
		Network::ConnectionID uID = kTcpNetwork.connect("192.168.1.201", 7890);
		while (uID == INVALID_CONNECTION_ID)
		{
			uID = kTcpNetwork.connect("192.168.1.201", 7890);
		}
		if (uID != INVALID_CONNECTION_ID)
		{
			std::cout << "sendPacketOnceFunc connected, ID = " << uID << std::endl;
			TestCommonDefine::Packet kPacket;
			unsigned int uiSize = kRandom();
			for (unsigned int ui = 0; ui < uiSize; ++ui)
			{
				kPacket.acBuff[ui] = cChar;
				if (cChar == 'z')
				{
					cChar = 'a';
				}
				else
				{
					++cChar;
				}
			}
			kPacket.uiPacketSize = uiSize + 4;
			if (kTcpNetwork.send(uID, (unsigned char*)(&kPacket), kPacket.uiPacketSize))
			{
				std::cout << uID << " send successful, packet size " << kPacket.uiPacketSize << std::endl;
				kFrameRate.stat();
				kFrameRate.showOnWindowTitle(1e3);
			}
			else
			{
				std::cout << uID << " send failed, packet size " << kPacket.uiPacketSize << std::endl;
			}
		}
		kTcpNetwork.asyncCloseConnection(uID);

		kTcpNetwork.running();
	}
}
void sendPacketRepeatFunc()
{
	Utility::FrameRate kFrameRate;

	std::vector<Network::ConnectionID> kConnectionIDVec;
	for (unsigned int ui = 0; ui < 100; ++ui)
	{
		if (!g_bKeepRunningFlag)
		{
			break;
		}

		//boost::this_thread::sleep(boost::posix_time::milliseconds(50));

		Network::ConnectionID uID = kTcpNetwork.connect("192.168.1.201", 7890);
		while (uID == INVALID_CONNECTION_ID)
		{
			uID = kTcpNetwork.connect("192.168.1.201", 7890);
			if (!g_bKeepRunningFlag)
			{
				return;
			}
		}
		kConnectionIDVec.push_back(uID);
	}
	char cChar = 'a';
	boost::uniform_int<> kDistribution(1, 8192);
	boost::mt19937 kEngine ;
	boost::variate_generator<boost::mt19937, boost::uniform_int<> > kRandom(kEngine, kDistribution);

	TestCommonDefine::Packet kPacket;
	while (g_bKeepRunningFlag)
	{
		boost::this_thread::sleep(boost::posix_time::milliseconds(10));

		unsigned int uiSize = /*120 * 1024 - 4;*/kRandom();
		for (unsigned int ui = 0; ui < uiSize; ++ui)
		{
			kPacket.acBuff[ui] = cChar;
			if (cChar == 'z')
			{
				cChar = 'a';
			}
			else
			{
				++cChar;
			}
		}
		kPacket.uiPacketSize = uiSize + 4;

		for (unsigned int ui = 0; ui < kConnectionIDVec.size(); ++ui)
		{
			unsigned int uiBeginTick = timeGetTime();
			unsigned int uiTotalPacketCnt = 0;
			unsigned int uiDataSizeCnt = 0;
			unsigned int uiFrameRate = 0;

			if (kTcpNetwork.send(kConnectionIDVec[ui], (unsigned char*)(&kPacket), kPacket.uiPacketSize))
			{
				kFrameRate.stat();
				kFrameRate.showOnWindowTitle(1e3);

				//boost::this_thread::sleep(boost::posix_time::milliseconds(1));

				uiDataSizeCnt += kPacket.uiPacketSize;
				++uiFrameRate;

				if (timeGetTime() - uiBeginTick > 1000)
				{
					std::cout <<uiFrameRate << " frames " << kConnectionIDVec[ui] << " sent data size " << uiDataSizeCnt << std::endl;
					uiDataSizeCnt = 0;
					uiFrameRate = 0;
					uiBeginTick = timeGetTime();
				}

				++uiTotalPacketCnt;
			}
			else
			{
				std::cout << kConnectionIDVec[ui] << " send failed " << uiTotalPacketCnt << std::endl;
			}
		}
		kTcpNetwork.running();
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	Utility::MemoryLeakChecker kMemoryLeakChecker;
	SetConsoleCtrlHandler(ConsoleHandler, TRUE);

	char* pcSelfAddress = "127.0.0.1";
	char* pcAddress = pcSelfAddress;
	if (argc > 1)
	{
		USES_CONVERSION;
		pcAddress = T2A(argv[1]);
	}
	boost::timer::cpu_timer kTimer;
	Utility::FrameRate kFrameRate;

	boost::thread_group kThreadGroup;

	ConCallback kConCallback;
	kTcpNetwork.bindOnConnected(boost::bind(&ConCallback::onConFunc, &kConCallback, _1));
	kTcpNetwork.bindOnDisconnected(boost::bind(&ConCallback::onDisConFunc, &kConCallback, _1));
	kTcpNetwork.bindOnRecved(boost::bind(&ConCallback::onRecvFunc, &kConCallback, _1, _2, _3));
	kTcpNetwork.bindOnSent(boost::bind(&ConCallback::onSentFunc, &kConCallback, _1, _2));
	kTcpNetwork.bindOnError(boost::bind(&ConCallback::errorHandleFunc, &kConCallback, _1, _2));

	Network::TcpNetworkParams kParams = Network::TcpNetworkParams::Default();
	kParams.uiSendBuffSize = 50 * 1024;
	kParams.uiRecvBuffSize = 50 * 1024;
	kParams.uiIOThreadCount = 0;
	kParams.uiListenConnectionNum = 0;
	kParams.uiMaxConnectionWriteSize = 40 * 1024 + 4;
	kParams.uiExtraBufferCount = 0;
	kParams.uiPreCreateConCnt = 10;
	kParams.uiMaxPoolConCnt = 10;
	kParams.bCloseOnBufferOverflow = false;
	kParams.eDataProcessMethod = Network::DPM_Stream;
	//kParams.eDataProcessMethod = Network::DPM_Packet;

	kParams.bUseSSL = true;
	kParams.eSSLMethod = boost::asio::ssl::context::sslv23;
	kParams.uiVerifyMode = boost::asio::ssl::context::verify_none;
	//kParams.strVerifyFile = "cacert.pem";
	
	if (kTcpNetwork.startup(kParams))
	{
		/// Test 0
		/// @{
		//sendMsgOnceFunc();
		/// @}

		/// Test 1
		/// @{
		sendMsgRepeatFunc();
		/// @}
			
		/// Test 2
		/// @{
		//sendPacketOnceFunc();
		/// @}

		/// Test3
		/// @{
		//sendPacketRepeatFunc();
		/// @}

		kThreadGroup.join_all();

		while (!kTcpNetwork.tryShutdown());
	}

	kConCallback.outputStats();

	system("pause");
	return 0;
}