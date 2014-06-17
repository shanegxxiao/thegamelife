#include "RuntimePCH.h"
#include "LogService.h"
#include "DynCreate.h"

Runtime::LogSystem* gspkLogSystem = 0;

namespace Runtime
{
	RUNTIME_CREATE_SERVICE(LogService, Runtime::Object, 0)
	LogService::LogService()
		: mpkLogFile(0),
#ifdef WIN32
		  mpkConsoleSystem(0),
#endif // WIN32
		  mpkLogToVSOutput(0),
		  mpkLogSystem(0)
	{

	}
	LogService::~LogService()
	{

	}
	bool LogService::Initialize(Runtime::ParamList& kParamList)
	{
		mpkLogSystem = new Runtime::LogSystem;

		mpkLogFile = new Runtime::LogFile;
		mpkLogSystem->AddLogTarget(mpkLogFile, false);

		void* pVoid = 0;
		kParamList.GetParam("ConsoleCallback", pVoid);
#ifdef WIN32
		ConsoleCallback pfnCallback = *((ConsoleCallback*)pVoid);
		kParamList.GetParam("ConsoleCtrlHandle", pVoid);
		ConsoleCtrlHandle pfnCtrlHandle = *((ConsoleCtrlHandle*)pVoid);
		mpkConsoleSystem = new Runtime::ConsoleSystem(L"ServiceLogger", 
			pfnCallback, pfnCtrlHandle, GetConsoleWindow());
		mpkLogSystem->AddLogTarget(mpkConsoleSystem, true);
#endif WIN32

		mpkLogToVSOutput = new Runtime::LogToVSOutput;	
		mpkLogSystem->AddLogTarget(mpkLogToVSOutput, false);

		gspkLogSystem = mpkLogSystem;

		EnableAutoRunning(Runtime::RTP_NORMAL);

		return mpkLogSystem->Initialize();
	}
	void LogService::Running()
	{
		mpkLogSystem->Running();
	}
	bool LogService::Shutdown()
	{
		if (!mpkLogSystem->Shutdown())
		{
			return false;
		}
		delete mpkLogSystem;
		return true;
	}
}
