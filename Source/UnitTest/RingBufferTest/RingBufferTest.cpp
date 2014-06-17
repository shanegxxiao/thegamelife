#include "UnitTestPCH.h"
#include <boost/test/unit_test.hpp>
#include "Network/RingBuffer.h"
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/random.hpp>

const unsigned int gcuiBufferReadWiteCnt = 1000;
const unsigned int gcuiBuffSize = 1024 * 8;
const unsigned int gcuiMaxPacketDataSize = 1024 - 4;
const unsigned int gcuiMaxPacketSize = gcuiMaxPacketDataSize + sizeof(unsigned int);
Network::RingBuffer<char> gkRingBuff(gcuiBuffSize, gcuiMaxPacketSize);
unsigned int guiTotalSend;
unsigned int guiTotalRecv;
bool gbWriting = true;

struct RingBufferTestSuiteFixture 
{
	RingBufferTestSuiteFixture()
	{
		gkRingBuff.clear();
	}
	~RingBufferTestSuiteFixture()
	{

	}
};

struct RingBufferTestCaseFixture
{
	RingBufferTestCaseFixture()
	{
		gkRingBuff.clear();
		guiTotalSend = 0;
		guiTotalRecv = 0;
		gbWriting = true;
	}
	~RingBufferTestCaseFixture()
	{

	}
};

void checkRecvDataA2Z(char *acBuff, unsigned int uiSize)
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

void syncSendMsgThreadFunc(Network::RingBuffer<char>* pkRingBuff, unsigned int uiPeriodMS)
{
	char cChar = 'a';
	boost::uniform_int<> kDistribution(1, gcuiMaxPacketDataSize) ;
	boost::mt19937 kEngine ;
	boost::variate_generator<boost::mt19937, boost::uniform_int<> > kRandom(kEngine, kDistribution);

	pkRingBuff->clear();
	for (unsigned int ui = 0; ui < gcuiBufferReadWiteCnt; ++ui)
	{
		unsigned int uiSize = kRandom();
		char* pBuff = new char[uiSize];
		for (unsigned int uj = 0; uj < uiSize; ++uj)
		{
			pBuff[uj] = cChar;
			if (cChar == 'z')
			{
				cChar = 'a';
			}
			else
			{
				++cChar;
			}
		}

		while (!pkRingBuff->writeImmediately((char*)pBuff, uiSize));
		boost::this_thread::sleep(boost::posix_time::milliseconds(uiPeriodMS));
		guiTotalSend += uiSize;
		delete[] pBuff;
	}
	gbWriting = false;
}

void asyncSendMsgThreadFunc(Network::RingBuffer<char>* pkRingBuff, unsigned int uiPeriodMS)
{
	char cChar = 'a';
	pkRingBuff->clear();
	for (unsigned int ui = 0; ui < gcuiBufferReadWiteCnt; ++ui)
	{
		char *pDestBuff;
		unsigned int uiDestWritableBuffSize;
		if (pkRingBuff->writeBegin(pDestBuff, uiDestWritableBuffSize))
		{
			if (uiDestWritableBuffSize == 0)
			{
				pkRingBuff->writeEnd(0);
				continue;
			}
			boost::uniform_int<> kDistribution(1, uiDestWritableBuffSize) ;
			boost::mt19937 kEngine ;
			boost::variate_generator<boost::mt19937, boost::uniform_int<> > kRandom(kEngine, kDistribution);
			unsigned int uiSize = kRandom();
			char* pBuff = new char[uiSize];
			for (unsigned int uj = 0; uj < uiSize; ++uj)
			{
				pBuff[uj] = cChar;
				if (cChar == 'z')
				{
					cChar = 'a';
				}
				else
				{
					++cChar;
				}
			}
			boost::this_thread::sleep(boost::posix_time::milliseconds(uiPeriodMS));
			memcpy_s(pDestBuff, uiDestWritableBuffSize, pBuff, uiSize);
			pkRingBuff->writeEnd(uiSize);
			guiTotalSend += uiSize;
			delete[] pBuff;
		}
	}
	gbWriting = false;
}

void syncRecvMsgThreadFunc(Network::RingBuffer<char>* pkRingBuff, unsigned int uiPeriodMS)
{
	char acBuff[gcuiBuffSize + 1];
	while (gbWriting || pkRingBuff->readableSize() > 0)
	{
		unsigned int uiSizeRead = pkRingBuff->readImmediately(acBuff, gcuiBuffSize);
		if (uiSizeRead)
		{
			guiTotalRecv += uiSizeRead;
			acBuff[uiSizeRead] = '\0';
			std::cout << "recvMsgThreadFunc::" << acBuff << "::" << guiTotalRecv << std::endl;
			checkRecvDataA2Z(acBuff, uiSizeRead);
			boost::this_thread::sleep(boost::posix_time::milliseconds(uiPeriodMS));
		}
	}
}
void asyncRecvMsgThreadFunc(Network::RingBuffer<char>* pkRingBuff, unsigned int uiPeriodMS)
{
	char acBuff[gcuiBuffSize + 1];
	while (gbWriting || pkRingBuff->readableSize() > 0)
	{
		char* pBuff;
		unsigned int uiReadableSize;
		if (pkRingBuff->readBegin(pBuff, uiReadableSize))
		{
			if (uiReadableSize == 0)
			{
				pkRingBuff->readEnd(0);
				continue;
			}
			boost::this_thread::sleep(boost::posix_time::milliseconds(uiPeriodMS));
			memcpy_s(acBuff, gcuiBuffSize + 1, pBuff, uiReadableSize);
			pkRingBuff->readEnd(uiReadableSize);
			guiTotalRecv += uiReadableSize;
			acBuff[uiReadableSize] = '\0';
			std::cout << "recvMsgThreadFunc::" << acBuff << "::" << guiTotalRecv << std::endl;
			checkRecvDataA2Z(acBuff, uiReadableSize);
		}
	}
}

#pragma pack(push, 1)
struct Packet 
{
	unsigned int uiPacketSize;
	char acBuff[gcuiMaxPacketDataSize + 1];
};
#pragma pack(pop)

void syncSendPacketThreadFunc(Network::RingBuffer<char>* pkRingBuff, unsigned int uiPeriodMS)
{
	char cChar = 'a';
	boost::uniform_int<> kDistribution(1, gcuiMaxPacketDataSize) ;
	boost::mt19937 kEngine ;
	boost::variate_generator<boost::mt19937, boost::uniform_int<> > kRandom(kEngine, kDistribution);

	pkRingBuff->clear();
	Packet kPacket;

	for (unsigned int ui = 0; ui < gcuiBufferReadWiteCnt; ++ui)
	{
		unsigned int uiSize = kRandom();
		char* pBuff = new char[uiSize];
		for (unsigned int uj = 0; uj < uiSize; ++uj)
		{
			pBuff[uj] = cChar;
			if (cChar == 'z')
			{
				cChar = 'a';
			}
			else
			{
				++cChar;
			}
		}
		memcpy_s(kPacket.acBuff, gcuiMaxPacketDataSize, pBuff, uiSize);
		kPacket.uiPacketSize = uiSize + 4;
		while (!pkRingBuff->writeImmediately((char*)(&kPacket), kPacket.uiPacketSize));
		boost::this_thread::sleep(boost::posix_time::milliseconds(uiPeriodMS));
		guiTotalSend += kPacket.uiPacketSize;
		delete[] pBuff;
	}
	gbWriting = false;
}

void asyncRecvPacketThreadFunc(Network::RingBuffer<char>* pkRingBuff, unsigned int uiPeriodMS)
{
	Packet kPacket;
	while (gbWriting || pkRingBuff->readableSize() > 0)
	{
		char* pBuff;
		unsigned int uiReadableSize;
		if (pkRingBuff->readPacketsBegin(pBuff, uiReadableSize))
		{
			if (uiReadableSize == 0)
			{
				pkRingBuff->readPacketEnd(0);
				continue;
			}
			boost::this_thread::sleep(boost::posix_time::milliseconds(uiPeriodMS));
			unsigned int uiPacketSize = *((unsigned int*)pBuff);
			memcpy_s(&kPacket, gcuiMaxPacketSize, pBuff, uiPacketSize);
			pkRingBuff->readPacketEnd(uiPacketSize);
			guiTotalRecv += uiPacketSize;
			kPacket.acBuff[uiPacketSize - 4] = '\0';
			std::cout << "recvMsgThreadFunc::" << kPacket.acBuff << "::" << uiPacketSize << "/" << guiTotalRecv << std::endl;
			checkRecvDataA2Z(kPacket.acBuff, uiPacketSize - 4);
		}
	}
}

BOOST_FIXTURE_TEST_SUITE(RingBufferTestSuite, RingBufferTestSuiteFixture)

BOOST_FIXTURE_TEST_CASE(RingBufferTest0, RingBufferTestCaseFixture)
{
	boost::uniform_int<> kDistribution(0, 0) ;
	boost::mt19937 kEngine ;
	boost::variate_generator<boost::mt19937, boost::uniform_int<> > kRandom(kEngine, kDistribution);

	boost::thread_group kThreadGroup;

	/// 同步发送同步接收
	kThreadGroup.create_thread(boost::bind(&syncSendMsgThreadFunc, &gkRingBuff, kRandom()));
	kThreadGroup.create_thread(boost::bind(&syncRecvMsgThreadFunc, &gkRingBuff, kRandom()));

	kThreadGroup.join_all();

	BOOST_CHECK_EQUAL(guiTotalSend, guiTotalRecv);
	BOOST_TEST_MESSAGE("guiTotalSend = " << guiTotalSend << "; guiTotalRecv = " << guiTotalRecv << ";");
	std::cout << guiTotalSend << "::" << guiTotalRecv << std::endl;
}

BOOST_FIXTURE_TEST_CASE(RingBufferTest1, RingBufferTestCaseFixture)
{
	boost::uniform_int<> kDistribution(0, 0) ;
	boost::mt19937 kEngine ;
	boost::variate_generator<boost::mt19937, boost::uniform_int<> > kRandom(kEngine, kDistribution);

	boost::thread_group kThreadGroup;

	/// 异步发送异步接收
	kThreadGroup.create_thread(boost::bind(&asyncSendMsgThreadFunc, &gkRingBuff, kRandom()));
	kThreadGroup.create_thread(boost::bind(&asyncRecvMsgThreadFunc, &gkRingBuff, kRandom()));

	kThreadGroup.join_all();

	BOOST_CHECK_EQUAL(guiTotalSend, guiTotalRecv);
	std::cout << guiTotalSend << "::" << guiTotalRecv << std::endl;
}


BOOST_FIXTURE_TEST_CASE(RingBufferTest2, RingBufferTestCaseFixture)
{
	boost::uniform_int<> kDistribution(0, 0) ;
	boost::mt19937 kEngine ;
	boost::variate_generator<boost::mt19937, boost::uniform_int<> > kRandom(kEngine, kDistribution);

	boost::thread_group kThreadGroup;

	/// 模拟接收缓冲区测试
	kThreadGroup.create_thread(boost::bind(&asyncSendMsgThreadFunc, &gkRingBuff, kRandom()));
	kThreadGroup.create_thread(boost::bind(&syncRecvMsgThreadFunc, &gkRingBuff, kRandom()));

	kThreadGroup.join_all();

	BOOST_CHECK_EQUAL(guiTotalSend, guiTotalRecv);
	std::cout << guiTotalSend << "::" << guiTotalRecv << std::endl;
}
BOOST_FIXTURE_TEST_CASE(RingBufferTest3, RingBufferTestCaseFixture)
{
	boost::uniform_int<> kDistribution(0, 0) ;
	boost::mt19937 kEngine ;
	boost::variate_generator<boost::mt19937, boost::uniform_int<> > kRandom(kEngine, kDistribution);

	boost::thread_group kThreadGroup;

	/// 模拟发送缓冲区测试
	kThreadGroup.create_thread(boost::bind(&syncSendMsgThreadFunc, &gkRingBuff, kRandom()));
	kThreadGroup.create_thread(boost::bind(&asyncRecvMsgThreadFunc, &gkRingBuff, kRandom()));

	kThreadGroup.join_all();

	BOOST_CHECK_EQUAL(guiTotalSend, guiTotalRecv);
	std::cout << guiTotalSend << "::" << guiTotalRecv << std::endl;
}
BOOST_FIXTURE_TEST_CASE(RingBufferTest4, RingBufferTestCaseFixture)
{
	boost::uniform_int<> kDistribution(0, 0) ;
	boost::mt19937 kEngine ;
	boost::variate_generator<boost::mt19937, boost::uniform_int<> > kRandom(kEngine, kDistribution);

	boost::thread_group kThreadGroup;

	/// 模拟包处理测试
	kThreadGroup.create_thread(boost::bind(&syncSendPacketThreadFunc, &gkRingBuff, kRandom()));
	kThreadGroup.create_thread(boost::bind(&asyncRecvPacketThreadFunc, &gkRingBuff, kRandom()));

	kThreadGroup.join_all();

	BOOST_CHECK_EQUAL(guiTotalSend, guiTotalRecv);
	std::cout << guiTotalSend << "::" << guiTotalRecv << std::endl;
}
BOOST_FIXTURE_TEST_CASE(RingBufferAsyncOp, RingBufferTestCaseFixture)
{
	char* pcBuff = 0;
	Packet kPacket;
	kPacket.uiPacketSize = gcuiMaxPacketSize;
	for (unsigned int ui = 0; ui < 8; ++ui)
	{
		unsigned int uiWritableSize = 0;
		if (gkRingBuff.writeBegin(pcBuff, uiWritableSize) && uiWritableSize >= gcuiMaxPacketSize)
		{
			memcpy_s(pcBuff, uiWritableSize, &kPacket, gcuiMaxPacketSize);
			gkRingBuff.writeEnd(gcuiMaxPacketSize);
		}
		else
		{
			gkRingBuff.writeEnd(0);
		}
	}
	BOOST_CHECK_EQUAL(gkRingBuff.readableSize(), gcuiBuffSize);
	BOOST_CHECK_EQUAL(gkRingBuff.writableSize(), 0);
	unsigned int uiCurrentPacketSize = 0;
	BOOST_CHECK(gkRingBuff.currentPacketSize(uiCurrentPacketSize));
	BOOST_CHECK_EQUAL(uiCurrentPacketSize, gcuiMaxPacketSize);

	unsigned int uiReadableSize = 0;
	if (gkRingBuff.readBegin(pcBuff, uiReadableSize) && uiReadableSize >= 0)
	{
		gkRingBuff.readEnd(uiReadableSize);
	}
	else
	{
		gkRingBuff.readEnd(0);
	}
	BOOST_CHECK_EQUAL(uiReadableSize, gcuiMaxPacketSize * 8);

	unsigned int uiWritableSize = 0;
	if (gkRingBuff.writeBegin(pcBuff, uiWritableSize) && uiWritableSize >= gcuiMaxPacketSize)
	{
		memcpy_s(pcBuff, uiWritableSize, &kPacket, gcuiMaxPacketSize);
		gkRingBuff.writeEnd(gcuiMaxPacketSize);
	}
	else
	{
		gkRingBuff.writeEnd(0);
	}

	BOOST_CHECK_EQUAL(uiCurrentPacketSize, gcuiMaxPacketSize);
}
BOOST_AUTO_TEST_SUITE_END()