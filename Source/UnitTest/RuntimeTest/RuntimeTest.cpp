#include "UnitTestPCH.h"
#include <boost/test/unit_test.hpp>
#include "Utility/FileSystemHelper.h"
#include "Runtime/Runtime.h"

struct RuntimeTestSuiteFixture 
{
	RuntimeTestSuiteFixture()
	{
		
	}
	~RuntimeTestSuiteFixture()
	{
		
	}
};

struct RuntimeTestCaseFixture
{
	std::vector<std::string> kInitVector;
	std::vector<std::string> kShutVector;
    boost::property_tree::ptree kParamList;
	RuntimeTestCaseFixture()
	{
		void* pInitVector = (void*)&kInitVector;
		kParamList.add("InitVector", pInitVector);
		void* pShutVector = (void*)&kShutVector;
        kParamList.add("ShutVector", pShutVector);

		std::string strWorkDir = Utility::FileSystemHelper::GetModulePath();
        kParamList.add("acWorkDir", strWorkDir);
        kParamList.add("acDllDir", strWorkDir);
        kParamList.add("acResourceDir", strWorkDir);
        kParamList.add("acConfigDir", strWorkDir);
        kParamList.add("gpObjectCreator", (void*)RUNTIME_EXPORTED_MEMBER);

		gpkObjectSystem->Initialize(kParamList);
	}
	~RuntimeTestCaseFixture()
	{
		gpkObjectSystem->Shutdown();

		for (unsigned int ui = 0; ui < kShutVector.size(); ++ui)
		{
			std::cout << kShutVector[ui] << std::endl;
		}
	}
};

BOOST_FIXTURE_TEST_SUITE(RuntimeTestSuite, RuntimeTestSuiteFixture)

BOOST_FIXTURE_TEST_CASE(RuntimeTest0, RuntimeTestCaseFixture)
{
	for (unsigned int ui = 0; ui < kInitVector.size(); ++ui)
	{
		std::cout << kInitVector[ui] << std::endl;
	}
}

BOOST_AUTO_TEST_SUITE_END()