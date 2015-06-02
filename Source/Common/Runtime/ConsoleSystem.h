#ifndef __ConsoleSystem_h__
#define __ConsoleSystem_h__

#include "LogSystemDefine.h"
#include <list>
#include <queue>
#include "boost/function.hpp"
#include "LogTarget.h"

#ifdef WIN32
namespace Runtime
{
	typedef boost::function<void (stdstring strCmd)> ConsoleInputHandler;
    typedef boost::function<bool (unsigned long msgType)> ConsoleCtrlHandle;
	class ConsoleSystem : public LogTarget
	{
	public:
		ConsoleSystem(const TCHAR *pcCaptionName, ConsoleInputHandler pfnInputCallback,
			ConsoleCtrlHandle pfnCtrlHandle, HWND hWnd = 0, int ix = 0, int iy = 0,
			int iRowCnt = 100, int iColCnt = 80);
		virtual ~ConsoleSystem();

        virtual bool Initialize();
        virtual void Running();
        virtual bool Shutdown();
        virtual void WriteLog(LogLevel eLogLevel, std::string msg);
        virtual void WriteLog(LogLevel eLogLevel, std::wstring msg);

		void ClearScreen();
		void SubmitCmd(stdstring strCmd);
		
	private:
		void InitOutputRect();
		void InitCutline();
		void InitInputRect();

		void ScrollConsoleOutputRect();
		void InputString(stdstring str);
		void InputChar(TCHAR cInput);
		void DeleteChar();
		void PrintString(UINT uiColor, const TCHAR *acBuffer);

		void ThreadExecute();
		static DWORD WINAPI ThreadFn(LPVOID pVoid);
		static BOOL WINAPI ConsoleHandler(DWORD dwEvent);

	private:
		static ConsoleCtrlHandle m_pfnCtrlHandle;
		LogLevel m_eLogLevel;
		HWND m_hWnd;
		int m_iWndX;
		int m_iWndY;
		int m_iWndRowCnt;
		int m_iWndColCnt;
		stdstring m_strCaptionName;
		int m_iOutputCursorRowIdx;

		SMALL_RECT m_kOutputRect;
		SMALL_RECT m_kInputRect;

		HANDLE m_hOutput;// Handle to write to the console.
		HANDLE m_hInput;// Handle to read from the console.
		HANDLE m_hThread;
		bool m_bRunning;

		CRITICAL_SECTION m_kOutputRectCS;

		ConsoleInputHandler m_pfnInputCallback;
		COORD m_kInputCursorPos;
		stdstring m_strCommand;
		std::list<stdstring> m_kCommandList;

		CRITICAL_SECTION m_kSubmittedCmdQueueCS;
		std::queue<stdstring> m_kSubmittedCmdQueue;
	};
}
#endif // WIN32

#endif//#ifndef __ConsoleSystem_h__
//END OF FILE
