#include "UnitTestPCH.h"
#include <boost/test/unit_test.hpp>
#include "Utility/SizeHeadedPacketAssembler.h"
#include "Utility/SerializableStream.hpp"

BOOST_AUTO_TEST_SUITE(PacketAssemblerTestSuite)

BOOST_AUTO_TEST_CASE(AssemblerTestCase)
{
    Utility::SizeHeadedPacketAssembler kAssembler(8 * 1024);
    int a = 1;
    float b = 2.f;
    double c = 3.0;
    std::string s = "hello world";
    kAssembler & a & b & c & s;

    char* pbuff = kAssembler.Data();
    char* pdata = pbuff + sizeof(unsigned int); 
    unsigned int uiPacketWithHeadSize = kAssembler.Size();
    unsigned int uiPacketDataSize = uiPacketWithHeadSize - sizeof(unsigned int);
    Utility::SerializeInStream kStream(pdata, uiPacketDataSize);
    int aa;
    float bb;
    double cc;
    std::string ss;
    kStream & aa & bb & cc & ss;
    BOOST_CHECK_EQUAL(a, aa);
    BOOST_CHECK_EQUAL(b, bb);
    BOOST_CHECK_EQUAL(c, cc);
    BOOST_CHECK_EQUAL(s, ss);

    try
    {
        int dump;
        kStream & dump;
    }
    catch (...)
    {
        return;
    }
    BOOST_CHECK(false);
}

BOOST_AUTO_TEST_SUITE_END()