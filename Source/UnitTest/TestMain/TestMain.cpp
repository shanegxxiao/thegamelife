#include "UnitTestPCH.h"
#define BOOST_TEST_MODULE MainTest
#include <boost/test/included/unit_test.hpp>

#include "TestMain.h"
#ifdef WIN32
#include <vld/vld.h>
#endif // WIN32
#include "Utility/MemoryLeakChecker.h"
bool gbKeepRunningFlag = true;

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

TestMain::TestMain()
{
	mpkMemoryLeakChecker = new Utility::MemoryLeakChecker;
	SetConsoleCtrlHandler(ConsoleHandler, TRUE);
}
TestMain::~TestMain()
{
	delete mpkMemoryLeakChecker;
	system("pause");
}

BOOST_GLOBAL_FIXTURE(TestMain);