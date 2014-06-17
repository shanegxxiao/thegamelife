#include "RuntimePCH.h"
#include "ConsoleSystem.h"

#ifdef WIN32

#include "LogSystem.h"
#include <tchar.h>
#include <atlconv.h>

namespace Runtime
{
	ConsoleCtrlHandle ConsoleSystem::m_pfnCtrlHandle;

	ConsoleSystem::ConsoleSystem(const TCHAR *pcCaptionName, ConsoleCallback pfnInputCallback,
		ConsoleCtrlHandle pfnCtrlHandle, HWND hWnd/* = 0*/, int ix/* = 0*/, int iy/* = 0*/, 
		int iRowCnt/* = 100*/, int iColCnt/* = 80*/)
	{
		m_hWnd = hWnd;
		m_eLogLevel = L_INFO;
		m_iWndX = ix;
		m_iWndY = iy;
		m_iWndRowCnt = iRowCnt;
		m_iWndColCnt = iColCnt;
		m_pfnInputCallback = pfnInputCallback;
		m_pfnCtrlHandle = pfnCtrlHandle;
		Printf(m_acCaptionName, MAX_OUTPUT_LENGTH, _T("%s"), pcCaptionName);
	}
		
	ConsoleSystem::~ConsoleSystem()
	{

	}

	void ConsoleSystem::WriteLog(LogLevel eLogLevel, char *pcLogInfo)
	{
		USES_CONVERSION;
		Print(eLogLevel, A2T(pcLogInfo));
	}
		
	bool ConsoleSystem::Initialize()
	{
		InitializeCriticalSection(&m_kOutputRectCS);
		InitializeCriticalSection(&m_kSubmittedCmdQueueCS);

		// Set up the handles for reading/writing:
		if (!m_hWnd)
		{
			if (!AllocConsole())
			{
				return false;
			}
		}
		m_hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
		m_hInput = GetStdHandle(STD_INPUT_HANDLE);

		// Change the window caption:
		if (!SetConsoleTitle(m_acCaptionName))
		{
			OutputDebugString(_T("SetConsoleTitle ERROR.\n"));
		}

		::HWND hWnd = GetConsoleWindow();
		SetWindowPos(hWnd, NULL, m_iWndX, m_iWndY, 0, 0, SWP_NOSIZE);

		InitOutputRect();
		InitCutline();
		InitInputRect();

		m_hThread = CreateThread(NULL, 0, ThreadFn, (LPVOID)this, 0, NULL);

		SetConsoleCtrlHandler(ConsoleHandler, TRUE);

		ClearScreen();
		return true;
	}
	bool ConsoleSystem::Shutdown()
	{
		DeleteCriticalSection(&m_kOutputRectCS);
		DeleteCriticalSection(&m_kSubmittedCmdQueueCS);
		m_bRunning = false;
		CloseHandle(m_hOutput);
		CloseHandle(m_hInput);
		FreeConsole();
		WaitForSingleObject(m_hThread, INFINITE);
		return true;
	}
		
	void ConsoleSystem::Running()
	{
		if (m_pfnInputCallback)
		{
			EnterCriticalSection(&m_kSubmittedCmdQueueCS);
			if (m_kSubmittedCmdQueue.empty())
			{
				LeaveCriticalSection(&m_kSubmittedCmdQueueCS);
				return;
			}
			stdstring strCmd = m_kSubmittedCmdQueue.front();
			m_kSubmittedCmdQueue.pop();
			LeaveCriticalSection(&m_kSubmittedCmdQueueCS);
			m_pfnInputCallback(strCmd);
		}
	}

	void ConsoleSystem::ThreadExecute()
	{
		DWORD dwCharCnt;
		INPUT_RECORD kInputRec;
		ReadConsoleInput(m_hInput, &kInputRec, 1, &dwCharCnt);
		if (kInputRec.EventType == KEY_EVENT && kInputRec.Event.KeyEvent.bKeyDown)
		{
			if (kInputRec.Event.KeyEvent.wVirtualKeyCode == VK_SHIFT ||
				kInputRec.Event.KeyEvent.wVirtualKeyCode == VK_MENU ||
				kInputRec.Event.KeyEvent.wVirtualKeyCode == VK_CONTROL ||
				kInputRec.Event.KeyEvent.wVirtualKeyCode == VK_LEFT ||
				kInputRec.Event.KeyEvent.wVirtualKeyCode == VK_RIGHT ||
				kInputRec.Event.KeyEvent.wVirtualKeyCode == VK_TAB ||
				kInputRec.Event.KeyEvent.wVirtualKeyCode == VK_CAPITAL)
			{
				return;
			}
			else if (kInputRec.Event.KeyEvent.wVirtualKeyCode == VK_UP)
			{
				if (m_kCommandList.size() <= 0)
				{
					return;
				}
				InitInputRect();
				m_kCommandList.push_front(m_strCommand);
				m_strCommand = m_kCommandList.back();
				m_kCommandList.pop_back();
				InputString(m_strCommand);
			}
			else if (kInputRec.Event.KeyEvent.wVirtualKeyCode == VK_DOWN)
			{
				if (m_kCommandList.size() <= 0)
				{
					return;
				}
				InitInputRect();
				m_kCommandList.push_back(m_strCommand);
				m_strCommand = m_kCommandList.front();
				m_kCommandList.pop_front();
				InputString(m_strCommand);
			}
			else if (kInputRec.Event.KeyEvent.wVirtualKeyCode == VK_RETURN)
			{
				PrintString(FW, (TCHAR *)m_strCommand.c_str());
				if (m_strCommand.length() > 0)
				{
					SubmitCmd(m_strCommand.c_str());
					m_kCommandList.push_back(m_strCommand);
					m_strCommand = _T("");
				}
				InitInputRect();
			}
			else if (kInputRec.Event.KeyEvent.wVirtualKeyCode == VK_BACK)
			{
				DeleteChar();
			}
			else
			{
	#ifdef UNICODE
				TCHAR cInput = kInputRec.Event.KeyEvent.uChar.UnicodeChar;
	#else
				TCHAR cInput = kInputRec.Event.KeyEvent.uChar.AsciiChar;
	#endif
				InputChar(cInput);
				m_strCommand += cInput;
			}
		}
	}
		
	void ConsoleSystem::InputChar(TCHAR cInput)
	{
		DWORD dwCharCnt;
	#ifdef UNICODE
		if (cInput > 0x80)
		{
			FillConsoleOutputCharacter(m_hOutput, cInput, 2, m_kInputCursorPos, &dwCharCnt);
			m_kInputCursorPos.X += 2;
	#else
		if (cInput & 0x80)
		{
			FillConsoleOutputCharacter(m_hOutput, cInput, 2, m_kInputCursorPos, &dwCharCnt);
			m_kInputCursorPos.X += 1;
	#endif
		}
		else
		{
			FillConsoleOutputCharacter(m_hOutput, cInput, 1, m_kInputCursorPos, &dwCharCnt);
			m_kInputCursorPos.X += 1;
		}
		SetConsoleCursorPosition(m_hOutput, m_kInputCursorPos);
	}
		
	void ConsoleSystem::InputString(stdstring str)
	{
		for (unsigned int ui = 0; ui < str.length(); ++ui)
		{
			InputChar(str.at(ui));
		}
	}
		
	void ConsoleSystem::DeleteChar()
	{
		DWORD dwCharCnt;
		if (m_kInputCursorPos.X <= 2)
		{
			return;
		}
	#ifdef UNICODE
		if ((m_strCommand.at(m_strCommand.length() - 1)) > 0x80)
		{
			m_kInputCursorPos.X -= 2;
			FillConsoleOutputCharacter(m_hOutput, _T(' '), 2, m_kInputCursorPos, &dwCharCnt);
	#else
		if ((m_strCommand.at(m_strCommand.length() - 1)) & 0x80)
		{
			m_kInputCursorPos.X -= 2;
			FillConsoleOutputCharacter(m_hOutput, _T(' '), 2, m_kInputCursorPos, &dwCharCnt);
	#endif
		}
		else
		{
			m_kInputCursorPos.X -= 1;
			FillConsoleOutputCharacter(m_hOutput, _T(' '), 1, m_kInputCursorPos, &dwCharCnt);
		}
		SetConsoleCursorPosition(m_hOutput, m_kInputCursorPos);
		m_strCommand = m_strCommand.substr(0, m_strCommand.length() - 1);
	}
		
	void ConsoleSystem::SubmitCmd(stdstring strCmd)
	{
		EnterCriticalSection(&m_kSubmittedCmdQueueCS);
		m_kSubmittedCmdQueue.push(strCmd);
		LeaveCriticalSection(&m_kSubmittedCmdQueueCS);
	}

	DWORD ConsoleSystem::ThreadFn(LPVOID pVoid)
	{
		ConsoleSystem* pkConsoleSystem = (ConsoleSystem*)pVoid;
		while (pkConsoleSystem->m_bRunning)
		{
			Sleep(10);
			pkConsoleSystem->ThreadExecute();
		}
		return 0;
	}
		
	void ConsoleSystem::InitOutputRect()
	{
		m_iOutputCursorRowIdx = -1;
		COORD kLargestWndSize = GetLargestConsoleWindowSize(m_hOutput);
		if (m_iWndColCnt <= 0 || m_iWndColCnt > kLargestWndSize.X)
		{
			m_iWndColCnt = kLargestWndSize.X;
		}
		if (m_iWndRowCnt <= 0 || m_iWndRowCnt > kLargestWndSize.Y)
		{
			m_iWndRowCnt = kLargestWndSize.Y;
		}
	
		m_kOutputRect.Left = 0;
		m_kOutputRect.Top = 0;
		m_kOutputRect.Right = m_iWndColCnt - 1;
		m_kOutputRect.Bottom = m_iWndRowCnt - 1;
	
		COORD kOutputBufferSize = {m_iWndColCnt, m_iWndRowCnt};

		CONSOLE_SCREEN_BUFFER_INFO kSBInfo;
		GetConsoleScreenBufferInfo(m_hOutput, &kSBInfo);

		if (kOutputBufferSize.X * kOutputBufferSize.Y > kSBInfo.dwMaximumWindowSize.X * kSBInfo.dwMaximumWindowSize.Y)
		{
			if (!SetConsoleScreenBufferSize(m_hOutput, kOutputBufferSize))
			{
				OutputDebugString(_T("InitOutputRect SetConsoleScreenBufferSize ERROR.\n"));
			}
			if (!SetConsoleWindowInfo(m_hOutput, TRUE, &m_kOutputRect))
			{
				OutputDebugString(_T("InitOutputRect SetConsoleWindowInfo ERROR.\n"));
			}
		}
		else
		{
			if (!SetConsoleWindowInfo(m_hOutput, TRUE, &m_kOutputRect))
			{
				OutputDebugString(_T("InitOutputRect SetConsoleWindowInfo ERROR.\n"));
			}
			if (!SetConsoleScreenBufferSize(m_hOutput, kOutputBufferSize))
			{
				OutputDebugString(_T("InitOutputRect SetConsoleScreenBufferSize ERROR.\n"));
			}
		}
	}
		
	void ConsoleSystem::InitCutline()
	{
		CONSOLE_SCREEN_BUFFER_INFO kInfo;
		if (!GetConsoleScreenBufferInfo(m_hOutput, &kInfo))
		{
			OutputDebugString(_T("InitCutline GetConsoleScreenBufferInfo ERROR.\n"));
		}
		WORD wTextAtt = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;

		int iWidth = m_kOutputRect.Right - m_kOutputRect.Left + 1;
		COORD kCharCoord;
		DWORD dwNumOfOutput;
		for (int i = 0; i < iWidth; i += 1)
		{
			kCharCoord.X = m_kOutputRect.Left + i;
			kCharCoord.Y = m_kOutputRect.Bottom - 2;
			if (!FillConsoleOutputAttribute(m_hOutput, wTextAtt, 1, kCharCoord, &dwNumOfOutput))
			{
				OutputDebugString(_T("InitCutline FillConsoleOutputAttribute ERROR.\n"));
			}
			if (!FillConsoleOutputCharacter(m_hOutput, _T('-'), 1, kCharCoord, &dwNumOfOutput))
			{
				OutputDebugString(_T("InitCutline FillConsoleOutputCharacter ERROR.\n"));
			}
		}
		if (!SetConsoleTextAttribute(m_hOutput, kInfo.wAttributes))
		{
			OutputDebugString(_T("InitCutline SetConsoleTextAttribute ERROR.\n"));
		}
	}
		
	void ConsoleSystem::InitInputRect()
	{
		// Change the input console window size and the internal buffer size:
		m_kInputRect.Left = m_kOutputRect.Left;
		m_kInputRect.Top = m_kOutputRect.Bottom - 1;
		m_kInputRect.Right = m_kOutputRect.Right + 1;
		m_kInputRect.Bottom =  m_kOutputRect.Bottom;

		DWORD dwNumCharWrote;
		COORD kInputBeginPos = {m_kInputRect.Left, m_kInputRect.Top};
		int iClearInputSize = (m_kInputRect.Right - m_kInputRect.Left) * (m_kInputRect.Bottom - m_kInputRect.Top);
		FillConsoleOutputCharacter(m_hOutput, _T(' '), iClearInputSize, kInputBeginPos, &dwNumCharWrote);
		CONSOLE_CURSOR_INFO kCursorInfo;
		kCursorInfo.bVisible = TRUE;
		kCursorInfo.dwSize = 100;
		SetConsoleCursorInfo(m_hInput, &kCursorInfo);
		COORD kCursorPos;
		kCursorPos.X = m_kOutputRect.Left;
		kCursorPos.Y = m_kOutputRect.Bottom - 1;
		WriteConsoleOutputCharacter(m_hOutput, _T(">>"), 2, kCursorPos, &dwNumCharWrote);
		kCursorPos.X += 2;
		m_kInputCursorPos = kCursorPos;
		SetConsoleCursorPosition(m_hOutput, kCursorPos);
	}
		
	bool ConsoleSystem::Print(LogLevel eLogLevel, const TCHAR *pcFormat, ...)
	{
		if (eLogLevel < m_eLogLevel)
		{
			return false;
		}
		va_list args;
		va_start(args, pcFormat);
		TCHAR acBuffer[MAX_OUTPUT_LENGTH];
		VsPrintf(acBuffer, MAX_OUTPUT_LENGTH, pcFormat, args);
		va_end(args);

		PrintString(gs_uiLogColor[eLogLevel], acBuffer);

		return true;
	}
		
	void ConsoleSystem::PrintString(UINT uiColor, TCHAR *acBuffer)
	{
		SYSTEMTIME kSystemTime;
		GetLocalTime(&kSystemTime);
		TCHAR acOutput[MAX_OUTPUT_LENGTH];
		memset(acOutput, 0, MAX_OUTPUT_LENGTH);
		Printf(acOutput, _T("%02d:%02d:%02d:%03d >%s"), kSystemTime.wHour,
			kSystemTime.wMinute, kSystemTime.wSecond, kSystemTime.wMilliseconds, acBuffer);

		unsigned int uiOutputCharCnt = _tcslen(acOutput);

		EnterCriticalSection(&m_kOutputRectCS);

		if (m_iOutputCursorRowIdx + 1 > m_kOutputRect.Bottom - 3)
		{
			ScrollConsoleOutputRect();
		}
		else
		{
			++m_iOutputCursorRowIdx;
		}

		COORD kCharPos = {0, m_iOutputCursorRowIdx};
		DWORD dwNumCharWrote;
		for (unsigned int ui = 0; ui < uiOutputCharCnt; ++ui)
		{
			if (kCharPos.X + 2 > m_kOutputRect.Right || kCharPos.X + 1 > m_kOutputRect.Right)
			{
				kCharPos.X = 14;
				if (m_iOutputCursorRowIdx + 1 > m_kOutputRect.Bottom - 3)
				{
					ScrollConsoleOutputRect();
				}
				else
				{
					kCharPos.Y += 1;
					++m_iOutputCursorRowIdx;
				}
			}
	#ifdef UNICODE
			if (acOutput[ui] > 0x80)
			{
				FillConsoleOutputAttribute(m_hOutput, uiColor, 2, kCharPos, &dwNumCharWrote);
				FillConsoleOutputCharacter(m_hOutput, acOutput[ui], 2, kCharPos, &dwNumCharWrote);
				kCharPos.X += 2;
	#else
			if (acOutput[ui] & 0x80)
			{
				FillConsoleOutputAttribute(m_hOutput, uiColor, 2, kCharPos, &dwNumCharWrote);
				FillConsoleOutputCharacter(m_hOutput, acOutput[ui], 2, kCharPos, &dwNumCharWrote);
				++kCharPos.X;
	#endif //UNICODE
			}
			else
			{
				FillConsoleOutputAttribute(m_hOutput, uiColor, 1, kCharPos, &dwNumCharWrote);
				FillConsoleOutputCharacter(m_hOutput, acOutput[ui], 1, kCharPos, &dwNumCharWrote);
				++kCharPos.X;
			}
		}

		LeaveCriticalSection(&m_kOutputRectCS);
	}
		
	void ConsoleSystem::ScrollConsoleOutputRect()
	{
		SMALL_RECT kScrollRect, kClipRect;
		COORD kDestPos = {0, -1};
		CHAR_INFO kCharFill;
		CONSOLE_SCREEN_BUFFER_INFO bInfo;
		GetConsoleScreenBufferInfo(m_hOutput, &bInfo);
		kScrollRect.Left = 0;
		kScrollRect.Top = 0;
		kScrollRect.Right = bInfo.dwSize.X - 1;
		kScrollRect.Bottom = bInfo.dwSize.Y - 4;
		kClipRect = kScrollRect;
		kCharFill.Attributes = bInfo.wAttributes;
		kCharFill.Char.UnicodeChar = _T(' ');
		if (!ScrollConsoleScreenBuffer(m_hOutput, &kScrollRect, &kClipRect, kDestPos, &kCharFill))
		{
			OutputDebugString(_T("ScrollConsoleOutputRect ScrollConsoleScreenBuffer ERROR."));
		}
	}
		
	void ConsoleSystem::ClearScreen()
	{
		EnterCriticalSection(&m_kOutputRectCS);
		CONSOLE_SCREEN_BUFFER_INFO kSBInfo;
		GetConsoleScreenBufferInfo(m_hOutput, &kSBInfo);
		COORD kHome = {0, 0};
		DWORD dwNumCharWrote;
		unsigned long ulSize = kSBInfo.dwSize.X * (kSBInfo.dwSize.Y - 3);
		FillConsoleOutputAttribute(m_hOutput, kSBInfo.wAttributes, ulSize, kHome, &dwNumCharWrote);
		FillConsoleOutputCharacter(m_hOutput, _T(' '), ulSize, kHome, &dwNumCharWrote);
		InitOutputRect();
		LeaveCriticalSection(&m_kOutputRectCS);
	}

	BOOL WINAPI ConsoleSystem::ConsoleHandler(DWORD dwEvent)
	{
		if (m_pfnCtrlHandle && m_pfnCtrlHandle(dwEvent))
		{
			return TRUE;
		}
		return FALSE;
	}
}

#endif // WIN32
