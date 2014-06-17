#ifndef __ConCallback_h__
#define __ConCallback_h__

class ConCallback
{
public:
	ConCallback();
	void onConFunc(Network::ConnectionID uConID);
	void onDisConFunc(Network::ConnectionID uConID);
	void onRecvFunc(Network::ConnectionID uConID, const unsigned char* acData, unsigned int uSize);
	void onSentFunc(Network::ConnectionID uConID, unsigned int uSize);
	void errorHandleFunc(Network::ConnectionID uConID, const boost::system::error_code& kErrorCode);

	void outputStats();

private:
	void checkRecvDataA2Z(char *acBuff, unsigned int uiSize);

private:
	volatile long muiOnConCnt;
	volatile long muiOnDisConCnt;
	volatile long muiOnRecvDataSize;
	volatile long muiOnSendDataSize;
	volatile long muiOnErrorCnt;

	unsigned int muiStartTick;
};

#endif//__ConCallback_h__