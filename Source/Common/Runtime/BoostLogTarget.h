#ifndef __BoostLogTarget_h__
#define __BoostLogTarget_h__

#include "LogSystemDefine.h"
#include "LogTarget.h"
#include <string>

namespace Runtime
{
	class BoostLogTarget : public LogTarget
	{
	public:
		BoostLogTarget(bool bFile = true, bool bConsole = false);
		virtual ~BoostLogTarget();

		virtual bool Initialize();
		virtual bool Shutdown();
		virtual void WriteLog(LogLevel eLogLevel, std::string msg);
        virtual void WriteLog(LogLevel eLogLevel, std::wstring msg);

		void SetLogFileName(std::wstring filename);

	private:
		bool mbLogToFile;
		bool mbLogToConsole;
		std::wstring mLogFile;
	};
}

#endif//__BoostLogTarget_h__