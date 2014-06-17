#ifndef __LogTarget_h__
#define __LogTarget_h__

#include "LogSystemDefine.h"

namespace Runtime
{
	class LogTarget
	{
	public:
		LogTarget();
		virtual ~LogTarget();

		virtual bool Initialize();
		virtual void Running();
		virtual bool Shutdown();
		virtual void SetLogLevel(LogLevel eLogLevel);
		virtual LogLevel GetLogLevel();
		virtual void WriteLog(LogLevel eLogLevel, char *pcLogInfo) = 0;

		void UseSelfLogLevelSetting();
		bool IsUseSelfLogLevelSetting();

	protected:
		bool m_bUseSelfLogLevel;
		LogLevel m_eLogLevel;
	};
}

#endif//__LogTarget_h__