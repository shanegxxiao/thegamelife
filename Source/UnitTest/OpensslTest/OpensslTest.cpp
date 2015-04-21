#include "UnitTestPCH.h"
#include <boost/test/unit_test.hpp>
#include <iostream>
#include <stdio.h>
#include "openssl/rc4.h"

BOOST_AUTO_TEST_SUITE(OpensslTestSuite)

BOOST_AUTO_TEST_CASE(RC4Test)
{
	char pwd[] = "passworddfsafdsafdsafdsafdsewqrtewq325673#%W#$";
	RC4_KEY key0;
	RC4_KEY key1;
	RC4_set_key(&key0, sizeof(pwd), (const unsigned char*)pwd);
	RC4_set_key(&key1, sizeof(pwd), (const unsigned char*)pwd);

	unsigned char buff0[13] = "hello world!";
	unsigned char buff1[13];
	unsigned char buff2[13];

	RC4(&key0, sizeof(buff0), buff0, buff1);
	RC4(&key1, sizeof(buff1), buff1, buff2);

	std::cout << buff0 << std::endl << std::endl;
	std::cout << buff1 << std::endl << std::endl;
	std::cout << buff2 << std::endl << std::endl;
}

BOOST_AUTO_TEST_SUITE_END()
