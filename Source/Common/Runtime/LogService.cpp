#include "RuntimePCH.h"
#include "LogService.h"
#include "DynCreate.h"
#include "Utility/Encode.h"

Runtime::LogSystem* gspkLogSystem = 0;

namespace Runtime
{
	RUNTIME_CREATE_SERVICE(LogService, Runtime::Object, 0)
	LogService::LogService()
        : mpkLogSystem(0)
#ifdef WIN32
		, mpkConsoleSystem(0)
#endif // WIN32
		, mpkHtmlFile(0)
		, mpkLogToVSOutput(0)
        , mpkBoostLogTarget(0)
	{

	}
	LogService::~LogService()
	{

	}
    bool LogService::Initialize(boost::property_tree::ptree& kParamList)
	{
		mpkLogSystem = new Runtime::LogSystem;
        gspkLogSystem = mpkLogSystem;

        /// add log target
        /// @{
#ifdef WIN32
        if (kParamList.get<bool>("LogService.UseConsoleSystem", false))
        {
            void* pConsoleCallback = kParamList.get<void*>("LogService.ConsoleInputHandle", 0);
            ConsoleInputHandler pfnCallback;
            if (pConsoleCallback)
            {
                pfnCallback = *((ConsoleInputHandler*)pConsoleCallback);
            }
            void* pConsoleCtrlHandle = kParamList.get<void*>("LogService.ConsoleCtrlHandle", 0);
            ConsoleCtrlHandle pfnCtrlHandle;
            if (pConsoleCtrlHandle)
            {
                pfnCtrlHandle = *((ConsoleCtrlHandle*)pConsoleCtrlHandle);
            }
            std::string strConsoleName = kParamList.get<std::string>("LogService.ConsoleName", "ConsoleSystem");
            mpkConsoleSystem = new Runtime::ConsoleSystem(Utility::a2w(strConsoleName).c_str(),
                pfnCallback, pfnCtrlHandle, GetConsoleWindow());
            mpkLogSystem->AddLogTarget(mpkConsoleSystem, true);
        }
#endif WIN32
        if (kParamList.get<bool>("LogService.UseHtmlFileLogTarget", false))
        {
            mpkHtmlFile = new Runtime::LogHtmlFile;
            mpkLogSystem->AddLogTarget(mpkHtmlFile, false);
        }
        if (kParamList.get<bool>("LogService.UseVSOutputLogTarget", false))
        {
            mpkLogToVSOutput = new Runtime::LogToVSOutput;
            mpkLogSystem->AddLogTarget(mpkLogToVSOutput, false);
        }
        if (kParamList.get<bool>("LogService.UseBoostLogTarget", false))
        {
            mpkBoostLogTarget = new Runtime::BoostLogTarget;
            mpkLogSystem->AddLogTarget(mpkBoostLogTarget, false);
        }
        /// @}

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
