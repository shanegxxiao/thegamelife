#ifndef __ConCallback_h__
#define __ConCallback_h__

#include "Network/TcpNetwork.h"
#include <boost/date_time/posix_time/posix_time.hpp>

class ConCallback
{
public:
	ConCallback(Network::TcpNetwork* pkTcpNetwork);
	void onConFunc(Network::ConnectionID uConID);
	void onDisConFunc(Network::ConnectionID uConID);
	void onRecvFunc(Network::ConnectionID uConID, const unsigned char* acData, unsigned int uSize);
	bool onPacketFunc(Network::ConnectionID uConID, const unsigned char* acData, unsigned int uSize);
	void onSentFunc(Network::ConnectionID uConID, unsigned int uSize);
	void errorHandleFunc(Network::ConnectionID uConID, const boost::system::error_code& kErrorCode);
	std::string getPassword();
	void outputStats();

private:
	void checkRecvDataA2Z(char *acBuff, unsigned int uiSize);

private:
	volatile long muiOnConCnt;
	volatile long muiOnDisConCnt;
	volatile long muiOnRecvDataSize;
	volatile long muiOnSendDataSize;
	volatile long muiOnErrorCnt;

	boost::posix_time::ptime muiStartTick;

	Network::TcpNetwork* mpkTcpNetwork;
};

#endif//__ConCallback_h__
