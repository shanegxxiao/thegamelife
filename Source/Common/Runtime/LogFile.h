#ifndef __LogFile_h__
#define __LogFile_h__

#include "LogTarget.h"
#include <fstream>
#include <string>

namespace Runtime
{
	class LogFile : public LogTarget
	{
	public:
		struct XGLogStyle
		{
			XGLogStyle() : iFontName(""), iFontSize(2), iColor(""), bBold(false), 
				bItalic(false), bUnderline(false) {}
			char* iFontName;
			short iFontSize;
			char* iColor;
			bool bBold;
			bool bItalic;
			bool bUnderline;
		};

	public:
		LogFile();
		LogFile(char* file);
		virtual ~LogFile();
		virtual bool Initialize();
		virtual bool Shutdown();

		void Open(const char* file, const char* colorbg="#eeeeee");
		void Append(const char* file);

		virtual void WriteLog(LogLevel eLogLevel, char *pcLogInfo);

		void WriteLog(LogLevel typ, const char* format, ...);
		void WriteLogEx(LogLevel typ, XGLogStyle *style, char* msg);
		void WriteLogEx(LogLevel typ, XGLogStyle *style, const char* format, ...);

		void WriteLink(LogLevel typ, char* msg, char* file);
		void WriteLinkEx(LogLevel typ, XGLogStyle *style, char* msg, char* file);

		void Breakline();

		bool IsOpen();
		const char* GetFileName();

	private:
		std::ofstream *m_pFile;
		std::string m_strFilename;
		bool m_bIsOpen;
	};

	inline bool LogFile::IsOpen()
	{
		return m_bIsOpen;
	}

	inline const char* LogFile::GetFileName()
	{
		return m_strFilename.c_str();
	}
}
#endif