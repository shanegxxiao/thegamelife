#include "ServerTestPCH.h"
#include "ConCallback.h"
#include <iostream>
#include "../TestMain/TestCommonDefine.h"

ConCallback::ConCallback(Network::TcpNetwork* pkTcpNetwork)
	:	muiOnConCnt(0),
		muiOnDisConCnt(0),
		muiOnRecvDataSize(0),
		muiOnSendDataSize(0),
		muiOnErrorCnt(0)
{
	muiStartTick = boost::detail::win32::GetTickCount();
	mpkTcpNetwork = pkTcpNetwork;
}
void ConCallback::onConFunc(Network::ConnectionID uConID)
{
	BOOST_INTERLOCKED_INCREMENT(&muiOnConCnt);
	//std::cout << "on connected, ID = " << uConID << std::endl;
	//std::cout << "total connections count = " << muiOnConCnt << std::endl;
}
void ConCallback::onDisConFunc(Network::ConnectionID uConID)
{
	BOOST_INTERLOCKED_INCREMENT(&muiOnDisConCnt);
	//std::cout << "on disconnected, ID = " << uConID << std::endl;
	//std::cout << "total connections count = " << muiOnConCnt << std::endl;
}
void ConCallback::onRecvFunc(Network::ConnectionID uConID, 
	const unsigned char* acData, unsigned int uSize)
{
	BOOST_INTERLOCKED_EXCHANGE_ADD(&muiOnRecvDataSize, uSize);
	//char *pBuff = new char[uSize + 1];
	//memcpy_s(pBuff, uSize + 1, acData, uSize);
	//pBuff[uSize] = 0;
	//std::cout << "on received, ID = " << uConID << "," << 
	//	uSize << "," << pBuff << std::endl;

	//checkRecvDataA2Z(pBuff, uSize);

#ifdef _DEBUG
	//std::cout << "received bytes = " << Network::TcpConnection::msRecvCnt << std::endl;
#endif // _DEBUG

	//delete[] pBuff;
	//mpkTcpNetwork->asyncCloseConnection(uConID);
	mpkTcpNetwork->closeConnection(uConID);
}
bool ConCallback::onPacketFunc(Network::ConnectionID uConID, const unsigned char* acData, unsigned int uSize)
{
	BOOST_INTERLOCKED_EXCHANGE_ADD(&muiOnRecvDataSize, uSize);
	TestCommonDefine::Packet kPacket;
	memcpy_s(&kPacket, sizeof(TestCommonDefine::Packet) - 4, acData, uSize);
	kPacket.acBuff[uSize - 4] = '\0';
	//std::cout << "on received, ID = " << uConID << "," << 
	//	uSize << "," << kPacket.acBuff << std::endl;

	checkRecvDataA2Z(kPacket.acBuff, uSize - 4);

#ifdef _DEBUG
	//std::cout << "received bytes = " << Network::TcpConnection::msRecvCnt << std::endl;
	std::cout << "received bytes = " << uSize << std::endl;
#endif // _DEBUG

	return true;
}
void ConCallback::checkRecvDataA2Z(char *acBuff, unsigned int uiSize)
{
	for (unsigned int ui = 0; ui < uiSize - 1; ++ui)
	{
		if (acBuff[ui + 1] - acBuff[ui] != 1 && acBuff[ui + 1] - acBuff[ui] != -25)
		{
			__asm
			{
				int 3;
			}
		}
	}
}
void ConCallback::onSentFunc(Network::ConnectionID uConID, 
	unsigned int uSize)
{
	BOOST_INTERLOCKED_EXCHANGE_ADD(&muiOnSendDataSize, uSize);
	std::cout << "on sent, ID = " << uConID << "," << uSize << std::endl;
}
void ConCallback::errorHandleFunc(Network::ConnectionID uConID, 
	const boost::system::error_code& kErrorCode)
{
	BOOST_INTERLOCKED_INCREMENT(&muiOnErrorCnt);
	std::cout << "errorHandleFunc, ID = " << uConID << ", error code=" << kErrorCode <<  " error message=" << kErrorCode.message() << std::endl;
}
std::string ConCallback::getPassword()
{
	return "test";
}
void ConCallback::outputStats()
{
	std::cout << "OnConCnt = " << muiOnConCnt << std::endl;
	std::cout << "OnDisConCnt = " << muiOnDisConCnt << std::endl;
	std::cout << "OnRecvDataSize = " << muiOnRecvDataSize << std::endl;
	std::cout << "OnSendDataSize = " << muiOnSendDataSize << std::endl;
	std::cout << "OnErrorCnt = " << muiOnErrorCnt << std::endl;
#ifdef _DEBUG
	std::cout << "!OnRecvDataSize = " << Network::TcpConnection::msRecvCnt << std::endl;
	std::cout << "!OnSendDataSize = " << Network::TcpConnection::msSendCnt << std::endl;
#endif // _DEBUG

	unsigned int uiRunTick = boost::detail::win32::GetTickCount() - muiStartTick;
	float fRunSecond = uiRunTick / 1000.f;
	std::cout << "Handle connection " << muiOnConCnt / fRunSecond << "per second." << std::endl;
	std::cout << "Handle data " << muiOnRecvDataSize / fRunSecond << "per second." << std::endl;
}