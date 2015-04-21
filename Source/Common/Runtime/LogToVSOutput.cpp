#include "RuntimePCH.h"
#include "LogToVSOutput.h"
#include "Utility/Encode.h"
#include <boost/date_time/posix_time/posix_time.hpp>
#include <iostream>
#ifdef WIN32
#include <atlconv.h>
#include <tchar.h>
#endif // WIN32

namespace Runtime
{
    void LogToVSOutput::WriteLog(LogLevel eLogLevel, std::string msg)
	{
        WriteLog(eLogLevel, Utility::a2w(msg));
	}
    void LogToVSOutput::WriteLog(LogLevel eLogLevel, std::wstring msg)
    {
        if (eLogLevel >= m_eLogLevel)
        {
#ifdef WIN32
            OutputDebugString(Utility::a2w(boost::posix_time::to_iso_string(boost::posix_time::second_clock::local_time())).c_str());
            OutputDebugString(L" > ");
            OutputDebugString(msg.c_str());
            OutputDebugString(L"\n");
#else
            std::cout << boost::posix_time::to_iso_string(boost::posix_time::second_clock::local_time());
            std::cout << " > ";
            std::cout << Utility::w2a(msg);
            std::cout << std::endl;
#endif //WIN32
        }
    }
}
