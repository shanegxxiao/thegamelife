#ifndef __LogSystem_h__
#define __LogSystem_h__

#include "LogSystemDefine.h"
#include "LogTarget.h"
#include "LogHtmlFile.h"
#include "ConsoleSystem.h"
#include "LogToVSOutput.h"
#include "BoostLogTarget.h"
#include "Utility/Format.hpp"
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

		void WriteLog(LogLevel eLogLevel, std::string msg);
		void WriteLog(LogLevel eLogLevel, std::wstring msg);
#define _PARAMS(z, n, data) BOOST_PP_COMMA_IF(n) Arg##n const& arg##n
#define BOOST_PP_LOCAL_MACRO(n)                                                                     \
        template < BOOST_PP_ENUM_PARAMS(n, typename Arg) >                                          \
        void WriteLog(LogLevel eLogLevel, std::string const& fmt, BOOST_PP_REPEAT(n, _PARAMS, ~ ))  \
		{                                                                                           \
            try                                                                                     \
		    {                                                                                       \
                WriteLog(eLogLevel, Utility::format(fmt, BOOST_PP_ENUM_PARAMS(n, arg)));         \
		    }                                                                                       \
            catch(std::exception&)                                                                  \
		    {                                                                                       \
                WriteLog(eLogLevel, fmt);                                                    \
		    }                                                                                       \
		}                                                                                           \
        template < BOOST_PP_ENUM_PARAMS(n, typename Arg) >                                          \
        void WriteLog(LogLevel eLogLevel, std::wstring const& fmt, BOOST_PP_REPEAT(n, _PARAMS, ~ )) \
        {                                                                                           \
            try                                                                                     \
            {                                                                                       \
                WriteLog(eLogLevel, Utility::format(fmt, BOOST_PP_ENUM_PARAMS(n, arg)));         \
            }                                                                                       \
            catch(std::exception&)                                                                  \
            {                                                                                       \
                WriteLog(eLogLevel, fmt);                                                    \
			}                                                                                       \
		}
#
#define BOOST_PP_LOCAL_LIMITS (1, _FORMAT_MAX_PARAM_COUNT)
#include BOOST_PP_LOCAL_ITERATE()
#undef _PARAMS

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