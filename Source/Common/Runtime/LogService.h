#ifndef __LogService_h__
#define __LogService_h__

#include "Runtime.h"
#include "LogSystem.h"

namespace Runtime
{
	class LogService : public Runtime::Object
	{
	public:
		LogService();
		virtual ~LogService();
		virtual bool Initialize(Runtime::ParamList& kParamList);
		virtual void Running();
		virtual bool Shutdown();
		virtual Runtime::Object::ObjectType GetType();

	private:
		Runtime::LogFile* mpkLogFile;
#ifdef WIN32
		Runtime::ConsoleSystem* mpkConsoleSystem;
#endif // WIN32
		Runtime::LogToVSOutput* mpkLogToVSOutput;
		Runtime::LogSystem* mpkLogSystem;
	};

	inline Runtime::Object::ObjectType LogService::GetType()
	{
		return Runtime::Object::OT_SERVICE;
	}

}

extern Runtime::LogSystem* gspkLogSystem;
#define LOG_DEBUG(format, ...) gspkLogSystem->WriteLog(Runtime::L_DEBUG, format, __VA_ARGS__)
#define LOG_INFO(format, ...) gspkLogSystem->WriteLog(Runtime::L_INFO, format, __VA_ARGS__)
#define LOG_WARNING(format, ...) gspkLogSystem->WriteLog(Runtime::L_WARNING, format, __VA_ARGS__)
#define LOG_ERROR(format, ...) gspkLogSystem->WriteLog(Runtime::L_ERROR, format, __VA_ARGS__)
#define LOG_FATALERROR(format, ...) gspkLogSystem->WriteLog(Runtime::L_FATALERROR, format, __VA_ARGS__)

#endif//__LogService_h__
