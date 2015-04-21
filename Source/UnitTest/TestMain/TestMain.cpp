#include "UnitTestPCH.h"
#define BOOST_TEST_MODULE MainTest
#include <boost/test/included/unit_test.hpp>
#include "TestMain.h"
#include "Utility/MemoryLeakChecker.h"

#if defined(WIN32)&&defined(_DEBUG)
#include <vld/vld.h>
#endif // WIN32


bool gbKeepRunningFlag = true;

#ifdef WIN32
BOOL WINAPI ConsoleHandler(DWORD msgType)
{
	if (msgType == CTRL_C_EVENT)
	{
		printf("Ctrl-C!\n");
		gbKeepRunningFlag = 0;
		return TRUE;
	}
	else if (msgType == CTRL_CLOSE_EVENT)
	{
		printf("Close console window!\n");
		gbKeepRunningFlag = 0;
		/// Note: The system gives you very limited time to exit in this condition
		return TRUE;
	}

	return FALSE;
}
#endif//WIN32

TestMain::TestMain()
{
	mpkMemoryLeakChecker = new Utility::MemoryLeakChecker;

#ifdef WIN32
	SetConsoleCtrlHandler(ConsoleHandler, TRUE);
#endif//WIN32

}
TestMain::~TestMain()
{
	delete mpkMemoryLeakChecker;
#ifdef WIN32
	system("pause");
#endif//WIN32
}

BOOST_GLOBAL_FIXTURE(TestMain);
