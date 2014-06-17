#ifndef __MemoryLeakChecker_h__
#define __MemoryLeakChecker_h__

#ifdef WIN32

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

namespace Utility
{
	struct AutoSetBreakAlloc
	{
		AutoSetBreakAlloc(unsigned int uiAlloc)
		{
			_CrtSetBreakAlloc(uiAlloc);
		}
	};

	class MemoryLeakChecker
	{
	public:
		MemoryLeakChecker()
		{
			_CrtMemCheckpoint(&s1);
			_CrtSetDbgFlag (_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
		}
		~MemoryLeakChecker()
		{
			_CrtMemCheckpoint(&s2);
			if (_CrtMemDifference(&s3, &s1, &s2))
			{
				OutputDebugString(L"=============CRT-STATISTICS-BEGIN=============\n");
				_CrtMemDumpStatistics(&s3);
				OutputDebugString(L"==============CRT-STATISTICS-END==============\n");
			}
		}
	private:
		_CrtMemState s1;
		_CrtMemState s2;
		_CrtMemState s3;
	};
}

#else

namespace Utility
{
	struct AutoSetBreakAlloc
	{
		AutoSetBreakAlloc(unsigned int uiAlloc)
		{
		}
	};

	class MemoryLeakChecker
	{
	public:
		MemoryLeakChecker()
		{
		}
		~MemoryLeakChecker()
		{
		}
	};
}


#endif // WIN32


#endif//__MemoryLeakChecker_h__