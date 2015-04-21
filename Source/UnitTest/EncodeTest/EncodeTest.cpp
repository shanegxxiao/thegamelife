#include "UnitTestPCH.h"
#include <boost/test/unit_test.hpp>
#include "Utility/Encode.h"

BOOST_AUTO_TEST_SUITE(EncodeTestSuite)

BOOST_AUTO_TEST_CASE(EncodeTest)
{
    std::string localcp = std::string(Utility::Local());
    std::cout << localcp << std::endl;

    std::string local = "中文";
    std::string utf8 = Utility::gbk2a(local);
    std::wstring wstr = Utility::a2w(utf8);
}

BOOST_AUTO_TEST_SUITE_END()
