#ifndef __LogService_h__
#define __LogService_h__

#include "Runtime.h"
#include "LogSystem.h"
#include <stdarg.h>

namespace Runtime
{
	class LogService : public Runtime::Object
	{
	public:
		LogService();
		virtual ~LogService();
        virtual bool Initialize(boost::property_tree::ptree& kParamList);
		virtual void Running();
		virtual bool Shutdown();

	private:
        Runtime::LogSystem* mpkLogSystem;
        /// log target
        /// @{
#ifdef WIN32
		Runtime::ConsoleSystem* mpkConsoleSystem;
#endif // WIN32
        Runtime::LogHtmlFile* mpkHtmlFile;
        Runtime::LogToVSOutput* mpkLogToVSOutput;
        Runtime::BoostLogTarget* mpkBoostLogTarget;
        /// @}
	};
}

extern Runtime::LogSystem* gspkLogSystem;
#ifdef WIN32
#define LOG_TRACE(format, ...) gspkLogSystem->WriteLog(Runtime::L_TRACE, format, __VA_ARGS__)
#define LOG_DEBUG(format, ...) gspkLogSystem->WriteLog(Runtime::L_DEBUG, format, __VA_ARGS__)
#define LOG_INFO(format, ...) gspkLogSystem->WriteLog(Runtime::L_INFO, format, __VA_ARGS__)
#define LOG_WARNING(format, ...) gspkLogSystem->WriteLog(Runtime::L_WARNING, format, __VA_ARGS__)
#define LOG_ERROR(format, ...) gspkLogSystem->WriteLog(Runtime::L_ERROR, format, __VA_ARGS__)
#define LOG_FATAL(format, ...) gspkLogSystem->WriteLog(Runtime::L_FATAL, format, __VA_ARGS__)
#else
#define LOG_TRACE(format, ...) gspkLogSystem->WriteLog(Runtime::L_TRACE, format, ## __VA_ARGS__)
#define LOG_DEBUG(format, ...) gspkLogSystem->WriteLog(Runtime::L_DEBUG, format, ## __VA_ARGS__)
#define LOG_INFO(format, ...) gspkLogSystem->WriteLog(Runtime::L_INFO, format, ## __VA_ARGS__)
#define LOG_WARNING(format, ...) gspkLogSystem->WriteLog(Runtime::L_WARNING, format, ## __VA_ARGS__)
#define LOG_ERROR(format, ...) gspkLogSystem->WriteLog(Runtime::L_ERROR, format, ## __VA_ARGS__)
#define LOG_FATAL(format, ...) gspkLogSystem->WriteLog(Runtime::L_FATAL, format, ## __VA_ARGS__)
#endif//WIN32
#endif//__LogService_h__
