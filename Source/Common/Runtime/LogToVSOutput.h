#ifndef __LogToVSOutput_h__
#define __LogToVSOutput_h__

#include "LogSystemDefine.h"
#include "LogTarget.h"

namespace Runtime
{
	class LogToVSOutput : public LogTarget
	{
	public:
		virtual void WriteLog(LogLevel eLogLevel, std::string msg);
        virtual void WriteLog(LogLevel eLogLevel, std::wstring msg);
	};
}

#endif