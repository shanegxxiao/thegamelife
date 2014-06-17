#include "RuntimePCH.h"
#include "LogTarget.h"

namespace Runtime
{
	LogTarget::LogTarget()
		: m_bUseSelfLogLevel(false)
	{

	}
	LogTarget::~LogTarget()
	{

	}
	bool LogTarget::Initialize()
	{
		return true;
	}
	void LogTarget::Running()
	{

	}
	bool LogTarget::Shutdown()
	{
		return true;
	}
	void LogTarget::UseSelfLogLevelSetting()
	{
		m_bUseSelfLogLevel = true;
	}
	void LogTarget::SetLogLevel(LogLevel eLogLevel)
	{
		m_eLogLevel = eLogLevel;
	}
	LogLevel LogTarget::GetLogLevel()
	{
		return m_eLogLevel;
	}
	bool LogTarget::IsUseSelfLogLevelSetting()
	{
		return m_bUseSelfLogLevel;
	}
}