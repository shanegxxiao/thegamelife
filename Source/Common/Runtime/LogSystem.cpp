#include "RuntimePCH.h"
#include "LogSystem.h"
#include "Utility/UtilityDefine.h"

namespace Runtime
{
	LogSystem::LogSystem()
		: m_eLogLevel(L_TRACE)
	{

	}
	LogSystem::~LogSystem()
	{

	}
	bool LogSystem::AddLogTarget(LogTarget* pkLogTarget, bool bLoopRunning)
	{
		for (unsigned int ui = 0; ui < m_kLogTargets.size(); ++ui)
		{
			if (m_kLogTargets[ui] == pkLogTarget)
			{
				return false;
			}
		}
		if (bLoopRunning)
		{
			for (unsigned int ui = 0; ui < m_kLooprRunningLogTargets.size(); ++ui)
			{
				if (m_kLooprRunningLogTargets[ui] == pkLogTarget)
				{
					return false;
				}
			}
			m_kLooprRunningLogTargets.push_back(pkLogTarget);
		}
		m_kLogTargets.push_back(pkLogTarget);
		return true;
	}

	bool LogSystem::RemoveLogTarget(LogTarget* pkLogTarget)
	{
		for (LogTargetVector::iterator kIter = m_kLogTargets.begin();
			kIter != m_kLogTargets.end(); ++kIter)
		{
			if (*kIter == pkLogTarget)
			{
				m_kLogTargets.erase(kIter);
				return true;
			}
		}
		return false;
	}

	bool LogSystem::Initialize()
	{
		for (unsigned int ui = 0; ui < m_kLogTargets.size(); ++ui)
		{
			if (m_kLogTargets[ui])
			{
				if (!m_kLogTargets[ui]->Initialize())
					return false;
			}
		}
		return true;
	}
	void LogSystem::Running()
	{
		for (unsigned int ui = 0; ui < m_kLooprRunningLogTargets.size(); ++ui)
		{
			if (m_kLooprRunningLogTargets[ui])
			{
				m_kLooprRunningLogTargets[ui]->Running();
			}
		}
	}
	bool LogSystem::Shutdown()
	{
		for (unsigned int ui = 0; ui < m_kLogTargets.size(); ++ui)
		{
			if (m_kLogTargets[ui])
			{
				if (!m_kLogTargets[ui]->Shutdown())
				{
					return false;
				}
				delete m_kLogTargets[ui];
			}
		}
		m_kLogTargets.clear();
		return true;
	}

    void LogSystem::WriteLog(LogLevel eLogLevel, std::string msg)
	{
		for (unsigned int ui = 0; ui < m_kLogTargets.size(); ++ui)
		{
			if (m_kLogTargets[ui])
			{
                m_kLogTargets[ui]->WriteLog(eLogLevel, msg);
			}
		}
	}
    void LogSystem::WriteLog(LogLevel eLogLevel, std::wstring msg)
    {
        for (unsigned int ui = 0; ui < m_kLogTargets.size(); ++ui)
        {
            if (m_kLogTargets[ui])
            {
                m_kLogTargets[ui]->WriteLog(eLogLevel, msg);
            }
        }
    }
	void LogSystem::SetLogLevel(LogLevel eLogLevel)
	{
		m_eLogLevel = eLogLevel;
		for (unsigned int ui = 0; ui < m_kLogTargets.size(); ++ui)
		{
			if (m_kLogTargets[ui] && !m_kLogTargets[ui]->IsUseSelfLogLevelSetting())
			{
				m_kLogTargets[ui]->SetLogLevel(eLogLevel);
			}
		}
	}

	LogLevel LogSystem::GetLogLevel()
	{
		return m_eLogLevel;
	}
}
