#include "RuntimePCH.h"
#include "LogHtmlFile.h"
#include "Utility/FileSystemHelper.h"
#include "Utility/UtilityDefine.h"
#include "Utility/Encode.h"
#include <time.h>
#include <string>

namespace Runtime
{
	std::string GetDateAndTime(void)
	{
		return boost::posix_time::to_iso_string(boost::posix_time::second_clock::local_time());
	}

	LogHtmlFile::LogHtmlFile()
	: m_pFile(0),
	  m_bIsOpen(false)
	{
        boost::gregorian::date today = boost::gregorian::day_clock::local_day();
        std::string strToday(boost::gregorian::to_iso_string(today));

        std::string modulename = Utility::FileSystemHelper::GetModuleNameA();
        m_strFilename = modulename.substr(0, modulename.find_last_of(".")) + 
            strToday + ".htm";
	}

	LogHtmlFile::LogHtmlFile(char* file)
	: m_pFile(0),
	  m_bIsOpen(false)
	{
		m_strFilename = file;
	}

	LogHtmlFile::~LogHtmlFile()
	{
	}

	bool LogHtmlFile::Initialize()
	{
		if (m_strFilename.length() == 0)
		{
			return false;
		}
		m_eLogLevel = L_TRACE;
		m_bIsOpen = false;
		m_pFile = new std::ofstream;
		Open(m_strFilename.c_str());
		return true;
	}
	bool LogHtmlFile::Shutdown()
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
	
	void LogHtmlFile::Open(const char* file, const char* colorbg)
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

	void LogHtmlFile::Append(const char* file)
	{
		// append to an existing logfile
		m_strFilename = file;
		m_pFile->open(m_strFilename.c_str(),std::ios::app);
		if(m_pFile->is_open())
			m_bIsOpen=true;
	}

    void LogHtmlFile::WriteLog(LogLevel typ, std::string msg)
	{
		if (m_bIsOpen && typ >= m_eLogLevel)
		{
			*m_pFile << "<font face=\"courier new\" size=\"2\" color=\""<< gs_cLogColor[typ]
                << "\">" << GetDateAndTime() << ": " << msg << "<BR>\n</font>\n" << std::endl;
		}
	}
    void LogHtmlFile::WriteLog(LogLevel eLogLevel, std::wstring msg)
    {
        WriteLog(eLogLevel, Utility::w2a(msg));
    }
	void LogHtmlFile::WriteLogEx(LogLevel typ, LogStyle *style, char* msg )
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

	void LogHtmlFile::WriteLink(LogLevel typ, char* msg, char* file)
	{
		if (m_bIsOpen && typ >= m_eLogLevel)
		{
			*m_pFile << "<a href=\"" << file << "\">"
				<< "<font face=\"courier new\" size=\"2\" color=\""<< gs_cLogColor[typ] 
			<< "\">" << GetDateAndTime() << ": " << msg << "<BR>\n</font>\n" << "</a>" << std::endl;
		}
	}

	void LogHtmlFile::WriteLinkEx(LogLevel typ, LogStyle *style, char* msg, char* file )
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

	void LogHtmlFile::Breakline()
	{
		if (m_bIsOpen)
			*m_pFile << "<hr>" << std::endl;
	}
}
