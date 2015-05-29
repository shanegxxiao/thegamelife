#ifndef __LogFile_h__
#define __LogFile_h__

#include "LogTarget.h"
#include <fstream>
#include <string>

namespace Runtime
{
	class LogHtmlFile : public LogTarget
	{
	public:
		struct LogStyle
		{
			LogStyle() : iFontName(""), iFontSize(2), iColor(""), bBold(false), 
				bItalic(false), bUnderline(false) {}
			char* iFontName;
			short iFontSize;
			char* iColor;
			bool bBold;
			bool bItalic;
			bool bUnderline;
		};

	public:
		LogHtmlFile();
		LogHtmlFile(char* file);
		virtual ~LogHtmlFile();
		virtual bool Initialize();
		virtual bool Shutdown();
        virtual void WriteLog(LogLevel eLogLevel, std::string msg);
        virtual void WriteLog(LogLevel eLogLevel, std::wstring msg);

        /// TODO: optimization and unicode support
        /// @{
		void Open(const char* file, const char* colorbg="#eeeeee");
		void Append(const char* file);

		void WriteLogEx(LogLevel typ, LogStyle *style, char* msg);

		void WriteLink(LogLevel typ, char* msg, char* file);
		void WriteLinkEx(LogLevel typ, LogStyle *style, char* msg, char* file);

		void Breakline();

		bool IsOpen();
		const char* GetFileName();
        /// @}

	private:
		std::ofstream *m_pFile;
		std::string m_strFilename;
		bool m_bIsOpen;
	};

	inline bool LogHtmlFile::IsOpen()
	{
		return m_bIsOpen;
	}

	inline const char* LogHtmlFile::GetFileName()
	{
		return m_strFilename.c_str();
	}
}
#endif