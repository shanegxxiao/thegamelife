#ifndef __LogSystem_h__
#define __LogSystem_h__

#include "LogSystemDefine.h"
#include "LogTarget.h"
#include "LogFile.h"
#include "ConsoleSystem.h"
#include "LogToVSOutput.h"
#include <vector>

namespace Runtime
{
	class LogSystem
	{
	public:
		LogSystem();
		~LogSystem();
		bool AddLogTarget(LogTarget* pkLogTarget, bool bLoopRunning);
		bool RemoveLogTarget(LogTarget* pkLogTarget);

		bool Initialize();
		void Running();
		bool Shutdown();

		void WriteLog(LogLevel eLogLevel, char *pcLogInfo);
		void WriteLog(LogLevel typ, const char* format, ...);
		void SetLogLevel(LogLevel eLogLevel);
		LogLevel GetLogLevel();

	private:
		typedef std::vector<LogTarget*> LogTargetVector;
		LogTargetVector m_kLogTargets;
		LogTargetVector m_kLooprRunningLogTargets;
		LogLevel m_eLogLevel;
	};
}

#endif//__LogSystem_h__
//END OF FILE