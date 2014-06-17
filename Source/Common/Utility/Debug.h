#ifndef __Debug_h__
#define __Debug_h__
#ifdef WIN32

#include <stdio.h>
#include <windows.h>
#include <dbghelp.h>
#include <tchar.h>
#include <string>
#pragma comment(lib, "Dbghelp.lib")

LONG WINAPI UnhandledFilter(struct _EXCEPTION_POINTERS *lpExceptionInfo);
#define EXCEPTION_FILTER ::SetUnhandledExceptionFilter(UnhandledFilter)

#endif // WIN32

#endif//#ifndef __Debug_h__
//END OF FILE