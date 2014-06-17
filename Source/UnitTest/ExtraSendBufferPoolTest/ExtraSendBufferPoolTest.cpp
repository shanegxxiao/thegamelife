#include "UnitTestPCH.h"
#include <boost/test/unit_test.hpp>
#include <iostream>
#include <stdio.h>
#include "Network/ExtraSendBufferPool.h"

BOOST_AUTO_TEST_SUITE(ExtraSendBufferPoolTestSuite)

BOOST_AUTO_TEST_CASE(DataOPTest)
{
	char data0[] = {'a', 'b', 'c'};
	char data1[] = {'x', 'y', 'z'};
	unsigned char* pcData = 0;
	unsigned int uiSize = 0;
	Network::ExtraSendBufferPool<unsigned char> kBuffPool(2, 4);

	while (kBuffPool.hasData())
	{
		kBuffPool.getData(pcData, uiSize);
		std::cout << pcData << ":" << uiSize << std::endl;
		kBuffPool.popData();
	}

	for (unsigned int ui = 0; ui < 10; ++ui)
	{
		kBuffPool.pushData((unsigned char*)data0, sizeof(data0));
		while (kBuffPool.hasData())
		{
			kBuffPool.getData(pcData, uiSize);
			std::cout << pcData << ":" << uiSize << std::endl;
			kBuffPool.popData();
		}
	}

	kBuffPool.pushData((unsigned char*)data0, sizeof(data0));
	kBuffPool.pushData((unsigned char*)data1, sizeof(data0));
	kBuffPool.pushData((unsigned char*)data0, sizeof(data0));
	kBuffPool.pushData((unsigned char*)data1, sizeof(data0));
	while (kBuffPool.hasData())
	{
		kBuffPool.getData(pcData, uiSize);
		std::cout << pcData << ":" << uiSize << std::endl;
		kBuffPool.popData();
	}

	kBuffPool.pushData((unsigned char*)data1, sizeof(data1));
	kBuffPool.pushData((unsigned char*)data0, sizeof(data1));
	kBuffPool.pushData((unsigned char*)data1, sizeof(data1));
	kBuffPool.pushData((unsigned char*)data0, sizeof(data1));
	while (kBuffPool.hasData())
	{
		kBuffPool.getData(pcData, uiSize);
		std::cout << pcData << ":" << uiSize << std::endl;
		kBuffPool.popData();
	}

	while (kBuffPool.hasData())
	{
		kBuffPool.getData(pcData, uiSize);
		std::cout << pcData << ":" << uiSize << std::endl;
		kBuffPool.popData();
	}

	while (kBuffPool.hasData())
	{
		kBuffPool.getData(pcData, uiSize);
		std::cout << pcData << ":" << uiSize << std::endl;
		kBuffPool.popData();
	}
}

BOOST_AUTO_TEST_SUITE_END()