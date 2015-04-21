#include "UnitTestPCH.h"
#include <boost/test/unit_test.hpp>
#include <boost/filesystem.hpp>
#include <boost/operators.hpp>
#include <boost/bind.hpp>
#include "Utility/Format.hpp"

BOOST_AUTO_TEST_SUITE(FormatTestSuite)
BOOST_AUTO_TEST_CASE(FormatTest)
{
	std::string result = "hello world 中文, 12345, 67, 8, 9.000000";
	std::string str = Utility::format("hello world %s, %ld, %d, %u, %f", "中文", 12345l, 67, 8u, 9.0f);
	BOOST_CHECK_EQUAL(result, str);

	std::wstring wresult = L"hello world 中文, 12345, 67, 8, 9.000000";
	std::wstring wstr = Utility::format(L"hello world %s, %ld, %d, %u, %f", L"中文", 12345l, 67, 8u, 9.0f);
	BOOST_CHECK(wresult == wstr);
}
BOOST_AUTO_TEST_SUITE_END()
