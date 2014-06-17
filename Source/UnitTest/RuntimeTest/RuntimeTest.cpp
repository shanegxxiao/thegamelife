#include "UnitTestPCH.h"
#include <boost/test/unit_test.hpp>
#include "Utility/FileSystemHelper.h"

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
	Runtime::ParamList kParamList;
	RuntimeTestCaseFixture()
	{
		void* pInitVector = (void*)&kInitVector;
		kParamList.AddParam("InitVector", pInitVector);
		void* pShutVector = (void*)&kShutVector;
		kParamList.AddParam("ShutVector", pShutVector);

		std::string strWorkDir = Utility::FileSystemHelper::GetModulePath();
		void* pVoidTmp = (void*)strWorkDir.c_str();
		kParamList.AddParam("acWorkDir", pVoidTmp);
		kParamList.AddParam("acDllDir", pVoidTmp);
		kParamList.AddParam("acResourceDir", pVoidTmp);
		kParamList.AddParam("acConfigDir", pVoidTmp);
		kParamList.AddParam("gpObjectCreator", (void*)RUNTIME_EXPORTED_MEMBER);

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