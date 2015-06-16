#include "AuthPCH.h"
#include "Runtime/Runtime.h"
#include "Utility/Encode.h"

#if defined(WIN32)&&defined(_DEBUG)
#include <vld/vld.h>
#endif // WIN32

bool gbKeepRunning = true;

#ifdef WIN32
bool CtrlHandler(DWORD msgType)
{
    gbKeepRunning = false;
    if (msgType == CTRL_C_EVENT)
    {
        return true;
    }
    else if (msgType == CTRL_CLOSE_EVENT)
    {
        /// Note: The system gives you very limited time to exit in this condition
        return true;
    }
    return false;
}
void InputHandler(Runtime::stdstring strCmd)
{
    LOG_TRACE(Utility::w2a(strCmd).c_str());
}
#endif//WIN32

int main()
{
    boost::property_tree::ptree kParamList;

    /// object system parameters
    /// @{
    std::string strWorkDir = Utility::FileSystemHelper::GetModulePath();
    kParamList.add("ObjectSystem.WorkDir", strWorkDir);
    kParamList.add("ObjectSystem.DllDir", strWorkDir);
    kParamList.add("ObjectSystem.ResourceDir", strWorkDir);
    kParamList.add("ObjectSystem.ConfigDir", strWorkDir);
    kParamList.add("ObjectSystem.ObjectCreator", (void*)RUNTIME_EXPORTED_MEMBER);
    /// @}

    /// log service parameters
    /// @{
    kParamList.add("LogService.UseConsoleSystem", true);
    kParamList.add("LogService.UseHtmlFileLogTarget", true);
    kParamList.add("LogService.UseVSOutputLogTarget", true);
    kParamList.add("LogService.UseBoostLogTarget", true);
#ifdef WIN32
    Runtime::ConsoleCtrlHandle ctrlHandler = boost::bind(&CtrlHandler, _1);
    Runtime::ConsoleInputHandler inputHandler = boost::bind(&InputHandler, _1);
    kParamList.add("LogService.ConsoleCtrlHandle", (void*)&ctrlHandler);
    kParamList.add("LogService.ConsoleInputHandle", (void*)&inputHandler);
    kParamList.add("LogService.ConsoleName", "Game Console");
#endif//WIN32
    /// @}

    gpkObjectSystem->Initialize(kParamList);

    LOG_INFO("all services initialized, run!");

    while (gbKeepRunning)
    {
        gpkObjectSystem->Running();
    }

    LOG_INFO("shutting down...");

    gpkObjectSystem->Shutdown();
}