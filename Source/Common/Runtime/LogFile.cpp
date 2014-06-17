#include "RuntimePCH.h"
#include "LogFile.h"
#include "Utility/FileSystemHelper.h"
#include "Utility/UtilityDefine.h"
#include <time.h>
#include <string>

namespace Runtime
{
	std::string GetDateAndTime(void)
	{
		return boost::posix_time::to_iso_string(boost::posix_time::second_clock::local_time());
	}

	LogFile::LogFile()
	: m_pFile(0),
	  m_bIsOpen(false)
	{
		char acBuff[MAX_OUTPUT_LENGTH];
		memset(acBuff, 0, MAX_OUTPUT_LENGTH);

		boost::posix_time::ptime now = boost::posix_time::second_clock::local_time();

		sprintf(acBuff, "%s\\Log_%04d%02d%02d.html",
			Utility::FileSystemHelper::GetModulePath().c_str(),
			now.date().year(), now.date().month(), now.date().day());
		m_strFilename = acBuff;
	}

	LogFile::LogFile(char* file)
	: m_pFile(0),
	  m_bIsOpen(false)
	{
		m_strFilename = file;
	}

	LogFile::~LogFile()
	{
	}

	bool LogFile::Initialize()
	{
		if (m_strFilename.length() == 0)
		{
			return false;
		}
		m_eLogLevel = L_INFO;
		m_bIsOpen = false;
		m_pFile = new std::ofstream;
		Open(m_strFilename.c_str());
		return true;		
	}
	bool LogFile::Shutdown()
	{
		if(m_bIsOpen)
		{
			m_pFile->close();
			m_bIsOpen=false;
		}
		if (m_pFile)
			delete m_pFile;
		m_pFile = NULL;
		return true;
	}
	
	void LogFile::Open(const char* file, const char* colorbg)
	{
		m_strFilename = file;
		m_pFile->open(m_strFilename.c_str(),std::ios::app);
		if(m_pFile->is_open())
		{
			m_bIsOpen=true;
			*m_pFile << "<html>\n"
				<< "<body link=\"#FFFFFF\" vlink=\"#FFFF97\" alink=\"#FFFFFF\" bgcolor=\"" << colorbg << "\">\n"
				<< "<table width=100% bgcolor=#aeaeff><tr> <td> <font face=arial size=+1>\n"
				<< GetDateAndTime()
				<< "\n"
				<< "</font></table>\n"
				<< "<hr>\n"
				<< "</body></html>\n";
		}
	}

	void LogFile::Append(const char* file)
	{
		// append to an existing logfile
		m_strFilename = file;
		m_pFile->open(m_strFilename.c_str(),std::ios::app);
		if(m_pFile->is_open())
			m_bIsOpen=true;
	}

	void LogFile::WriteLog(LogLevel typ, char* pcLogInfo)
	{
		std::string strmsg(pcLogInfo);
		if (m_bIsOpen && typ >= m_eLogLevel)
		{
			*m_pFile << "<font face=\"courier new\" size=\"2\" color=\""<< gs_cLogColor[typ]
			<< "\">" << GetDateAndTime() << ": " << strmsg << "<BR>\n</font>\n" << std::endl; 
		}
	}

	void LogFile::WriteLog(LogLevel typ, const char* format, ...)
	{
		if (m_bIsOpen && typ >= m_eLogLevel)	
		{
			va_list args;
			va_start(args, format);
			char buffer[UTILITY_MAX_PATH];
#ifdef WIN32
			vsprintf_s(buffer, UTILITY_MAX_PATH, format, args);
#else
			vsnprintf(buffer, UTILITY_MAX_PATH, format, args);
#endif // WIN32
			WriteLog(typ, buffer);
		}
	}

	void LogFile::WriteLogEx(LogLevel typ, XGLogStyle *style, const char* format, ... )
	{
		if (m_bIsOpen && typ >= m_eLogLevel)	
		{
			va_list args;
			va_start(args, format);
			char buffer[UTILITY_MAX_PATH];
#ifdef WIN32
			vsprintf_s(buffer, UTILITY_MAX_PATH, format, args);
#else
			vsnprintf(buffer, UTILITY_MAX_PATH, format, args);
#endif // WIN32
			WriteLogEx(typ, style, buffer);
		}
	}

	void LogFile::WriteLogEx(LogLevel typ, XGLogStyle *style, char* msg )
	{
		if (m_bIsOpen && typ >= m_eLogLevel)
		{
			if ( strlen(style->iFontName) )
				*m_pFile	<< "<font face=\"" << style->iFontName << "\"";
			else
				*m_pFile	<< "<font face=\"courier new\"";

			if ( style->iFontSize > 0 )
				*m_pFile	<< "size=\"" << style->iFontSize << "\" color=\"";
			else
				*m_pFile	<< "size=\"2\" color=\"";

			if ( strlen(style->iColor) ) 
				*m_pFile << style->iColor << "\">" << GetDateAndTime()<< ": "; 
			else
				*m_pFile << gs_cLogColor[typ] << "\">" << GetDateAndTime() << ": "; 

			if ( style->bBold )
				*m_pFile << "<b>";
			if ( style->bItalic )
				*m_pFile << "<i>";
			if ( style->bUnderline )
				*m_pFile << "<u>";

			*m_pFile << msg;

			if ( style->bBold )
				*m_pFile << "</b>";
			if ( style->bItalic )
				*m_pFile << "</i>";
			if ( style->bUnderline )
				*m_pFile << "</u>";

			*m_pFile<< "<BR>\n</font>\n" << std::endl; 
		}
	}

	void LogFile::WriteLink(LogLevel typ, char* msg, char* file)
	{
		if (m_bIsOpen && typ >= m_eLogLevel)
		{
			*m_pFile << "<a href=\"" << file << "\">"
				<< "<font face=\"courier new\" size=\"2\" color=\""<< gs_cLogColor[typ] 
			<< "\">" << GetDateAndTime() << ": " << msg << "<BR>\n</font>\n" << "</a>" << std::endl;
		}
	}

	void LogFile::WriteLinkEx(LogLevel typ, XGLogStyle *style, char* msg, char* file )
	{
		if (m_bIsOpen && typ >= m_eLogLevel)
		{
			if ( strlen(style->iFontName) )
				*m_pFile	<< "<font face=\"" << style->iFontName << "\"";
			else
				*m_pFile	<< "<font face=\"courier new\"";

			if ( style->iFontSize > 0 )
				*m_pFile	<< "size=\"" << style->iFontSize << "\" color=\"";
			else
				*m_pFile	<< "size=\"2\" color=\"";

			if ( strlen(style->iColor) ) 
				*m_pFile << style->iColor << "\">" << GetDateAndTime() << ": "; 
			else
				*m_pFile << gs_cLogColor[typ] << "\">" << GetDateAndTime() << ": "; 

			if ( style->bBold )
				*m_pFile << "<b>";
			if ( style->bItalic )
				*m_pFile << "<i>";
			if ( style->bUnderline )
				*m_pFile << "<u>";

			*m_pFile << msg;

			if ( style->bBold )
				*m_pFile << "</b>";
			if ( style->bItalic )
				*m_pFile << "</i>";
			if ( style->bUnderline )


				*m_pFile<< "<BR>\n</font>\n </a>" << std::endl; 
		}
	}

	void LogFile::Breakline()
	{
		if (m_bIsOpen)
			*m_pFile << "<hr>" << std::endl;
	}
}
