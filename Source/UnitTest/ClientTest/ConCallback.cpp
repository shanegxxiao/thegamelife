#include "ClientTestPCH.h"
#include "ConCallback.h"
#include <iostream>
#include "../TestMain/TestCommonDefine.h"

ConCallback::ConCallback()
 : muiOnConCnt(0),
   muiOnDisConCnt(0),
   muiOnRecvDataSize(0),
   muiOnSendDataSize(0),
   muiOnErrorCnt(0)
{
	muiStartTick = boost::posix_time::microsec_clock::local_time();
}
void ConCallback::onConFunc(Network::ConnectionID uConID)
{
#ifdef WIN32
	BOOST_INTERLOCKED_INCREMENT(&muiOnConCnt);
#endif//WIN32
	std::cout << "on connected, ID = " << uConID << std::endl;
}
void ConCallback::onDisConFunc(Network::ConnectionID uConID)
{
#ifdef WIN32
	BOOST_INTERLOCKED_INCREMENT(&muiOnDisConCnt);
#endif//WIN32
	std::cout << "on disconnected, ID = " << uConID << std::endl;
}
void ConCallback::onRecvFunc(Network::ConnectionID uConID, 
	const unsigned char* acData, unsigned int uSize)
{
#ifdef WIN32
	BOOST_INTERLOCKED_EXCHANGE_ADD(&muiOnRecvDataSize, uSize);
#else
	muiOnRecvDataSize += uSize;
#endif//WIN32
	char *pBuff = new char[uSize + 1];
#ifdef WIN32
	memcpy_s(pBuff, uSize + 1, acData, uSize);
#else
	memcpy(pBuff, acData, uSize);
#endif//WIN32
	pBuff[uSize] = 0;
	std::cout << "on received, ID = " << uConID << "," << 
		uSize << "," << pBuff << std::endl;

#ifdef _DEBUG
	std::cout << "received bytes = " << Network::TcpConnection::msRecvCnt << std::endl;
#endif // _DEBUG

	delete[] pBuff;
}
void ConCallback::checkRecvDataA2Z(char *acBuff, unsigned int uiSize)
{
	for (unsigned int ui = 0; ui < uiSize - 1; ++ui)
	{
		if (acBuff[ui + 1] - acBuff[ui] != 1 && acBuff[ui + 1] - acBuff[ui] != -25)
		{
			BOOST_ASSERT(false);
		}
	}
}
void ConCallback::onSentFunc(Network::ConnectionID uConID, 
	unsigned int uSize)
{
#ifdef WIN32
	BOOST_INTERLOCKED_EXCHANGE_ADD(&muiOnSendDataSize, uSize);
#endif//WIN32
	std::cout << "on sent, ID = " << uConID << "," << uSize << std::endl;
}
void ConCallback::errorHandleFunc(Network::ConnectionID uConID, 
	const boost::system::error_code& kErrorCode)
{
#ifdef WIN32
	BOOST_INTERLOCKED_INCREMENT(&muiOnErrorCnt);
#endif//WIN32
	std::cout << "errorHandleFunc, ID = " << uConID << ", error code " << kErrorCode << " error message=" << kErrorCode.message()<< std::endl;
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

	boost::posix_time::ptime endtick = boost::posix_time::microsec_clock::local_time();
	boost::posix_time::time_duration uiRunTick = endtick - muiStartTick;
	float fRunSecond = uiRunTick.seconds();
	std::cout << "Handle connection " << muiOnConCnt / fRunSecond << "per second." << std::endl;
	std::cout << "Handle data " << muiOnSendDataSize / fRunSecond << "per second." << std::endl;
}
