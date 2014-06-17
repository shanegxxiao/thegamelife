#include "RuntimePCH.h"
#include "LogToVSOutput.h"

#ifdef WIN32
#include <atlconv.h>
#include <tchar.h>
#endif // WIN32

namespace Runtime
{
	void LogToVSOutput::WriteLog(LogLevel eLogLevel, char *pcLogInfo)
	{
#ifdef WIN32
		SYSTEMTIME kSystemTime;
		GetLocalTime(&kSystemTime);

		USES_CONVERSION;
		TCHAR acBuff[MAX_OUTPUT_LENGTH];
		memset(acBuff, 0, MAX_OUTPUT_LENGTH);
		wsprintf(acBuff, _T("%02d:%02d:%02d:%03d > %s\n"), kSystemTime.wHour,
			kSystemTime.wMinute, kSystemTime.wSecond, kSystemTime.wMilliseconds, A2T(pcLogInfo));
		OutputDebugString(acBuff);
#else
		std::cout << boost::posix_time::to_iso_string(boost::posix_time::second_clock::local_time()) <<
				pcLogInfo << std::endl;
#endif //WIN32
	}
}
