#include "UnitTestPCH.h"
#include <boost/test/unit_test.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/asio.hpp>

#include <Utility/SerializableStream.hpp>

BOOST_AUTO_TEST_SUITE(SerializationTestSuite)

BOOST_AUTO_TEST_CASE(BoostSerializationTestCase)
{
    boost::asio::streambuf buff;
    boost::archive::binary_oarchive bo(buff);
    boost::archive::binary_iarchive bi(buff);

    int i = 8;
    bo & i;
    int j = 0;
    bi & j;
    BOOST_CHECK_EQUAL(i, j);

    std::string strin("hello world");
    bo & strin;
    std::string strout;
    bi & strout;
    BOOST_CHECK_EQUAL(strin, strout);
}

BOOST_AUTO_TEST_CASE(SerializationTestCase)
{
    Utility::SerializeOutStream bo(256);
    int i = 8, j = 9;
    std::string strin("hello world");
    bo & i & j & strin;

    Utility::SerializeInStream bi(bo.data(), bo.size());
    int m, n;
    std::string strout;
    bi & m & n & strout;
    BOOST_CHECK_EQUAL(i, m);
    BOOST_CHECK_EQUAL(j, n);
    BOOST_CHECK_EQUAL(strin, strout);

    bool btrue = false;
    try
    {
        bi & n;
    }
    catch (...)
    {
        btrue = true;
    }
    BOOST_CHECK(btrue);
    
    try
    {
        Utility::SerializeOutStream bo(44);
        int i = 8;
        bo & i;

        Utility::SerializeInStream bi(bo.data(), bo.size());
        int j;
        bi & j;
        BOOST_CHECK_EQUAL(i, j);
    }
    catch (...)
    {
        BOOST_CHECK(false);
    }

    try
    {
        Utility::SerializeOutStream bo(43);
        int i = 8;
        bo & i;
    }
    catch (...)
    {
        return;
    }
    BOOST_CHECK(false);
}

BOOST_AUTO_TEST_SUITE_END()