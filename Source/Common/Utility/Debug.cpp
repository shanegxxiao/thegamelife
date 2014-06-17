#include "UtilityPCH.h"
#include "Debug.h"
#include "UtilityDefine.h"

#ifdef WIN32

LONG WINAPI UnhandledFilter(struct _EXCEPTION_POINTERS *lpExceptionInfo)
{
	LONG ret = EXCEPTION_EXECUTE_HANDLER;

	WCHAR workdir[255];
	GetModuleFileName(NULL, workdir, 255);
	std::wstring strWorkDir = workdir;
	int pos = (int)strWorkDir.find_last_of(_T("\\"));
	strWorkDir = strWorkDir.substr(0, pos + 1);
	strWorkDir += _T("Log\\");

	TCHAR szFileName[64];
	SYSTEMTIME st;
	::GetLocalTime(&st);
	wsprintf(szFileName, _T("%sCrash@%04d-%02d-%02d-%02d-%02d-%02d.dmp"), 
		strWorkDir.c_str(),
		st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);

	HANDLE hFile = ::CreateFile(szFileName, GENERIC_WRITE, 0, NULL, 
		CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );
	if (hFile != INVALID_HANDLE_VALUE)
	{
		MINIDUMP_EXCEPTION_INFORMATION ExInfo;

		ExInfo.ThreadId = ::GetCurrentThreadId();
		ExInfo.ExceptionPointers = lpExceptionInfo;
		ExInfo.ClientPointers = false;

		// write the dump
		BOOL bOK = MiniDumpWriteDump(GetCurrentProcess(), 
			GetCurrentProcessId(), hFile, MiniDumpNormal, &ExInfo, NULL, NULL );

		if (bOK)
		{
			OutputDebugString(_T("Create Dump File Success!\n"));
		}
		else
		{
			TCHAR acBuff[UTILITY_MAX_PATH];
			wsprintf(acBuff, _T("MiniDumpWriteDump Failed: %d\n"), GetLastError());
			OutputDebugString(acBuff);
		}

		::CloseHandle(hFile);
	}
	else
	{
		TCHAR acBuff[UTILITY_MAX_PATH];
		wsprintf(acBuff, _T("Create File %s Failed %d\n"), szFileName, GetLastError());
		OutputDebugString(acBuff);
	}
	return ret;
}

#endif // WIN32