#include "RuntimePCH.h"
#include "BoostLogTarget.h"
#include "Utility/Encode.h"
#include "Utility/FileSystemHelper.h"
#include <boost/log/trivial.hpp>
#include <boost/log/common.hpp>  
#include <boost/log/expressions.hpp>  
#include <boost/log/utility/setup/file.hpp>  
#include <boost/log/utility/setup/console.hpp>  
#include <boost/log/utility/setup/common_attributes.hpp>  
#include <boost/log/attributes/timer.hpp>  
#include <boost/log/attributes/named_scope.hpp>  
#include <boost/log/sources/logger.hpp>  
#include <boost/log/support/date_time.hpp>
#include <boost/log/attributes.hpp>

namespace Runtime
{
	BoostLogTarget::BoostLogTarget(bool bFile/* = true*/, bool bConsole/* = false*/)
		: mbLogToFile(bFile), mbLogToConsole(bConsole)
	{
		std::wstring modulename = Utility::FileSystemHelper::GetModuleNameW();
		mLogFile = modulename.substr(0, modulename.find_last_of(L".")) + L"%Y%m%d_%N.log";
	}
    BoostLogTarget::~BoostLogTarget()
	{

	}
    bool BoostLogTarget::Initialize()
	{
		boost::log::register_simple_formatter_factory< boost::log::trivial::severity_level, char >("Severity");
		boost::log::add_common_attributes();

		if (mbLogToFile)
		{
			boost::log::add_file_log
				(
				boost::log::keywords::file_name = Utility::w2a(mLogFile).c_str(),
				boost::log::keywords::rotation_size = 128 * 1024 * 1024,
				boost::log::keywords::auto_flush = true,
				boost::log::keywords::open_mode = (std::ios::out | std::ios::app),
				boost::log::keywords::time_based_rotation = boost::log::sinks::file::rotation_at_time_point(0, 0, 0),
				boost::log::keywords::format = "%TimeStamp%[%Severity%]: %Message%"
				);
		}
		if (mbLogToConsole)
		{
			boost::log::add_console_log
				(
				std::clog,
				boost::log::keywords::format = "%TimeStamp%[%Severity%]: %Message%"
				);
		}
		return true;
	}
    bool BoostLogTarget::Shutdown()
	{
		return true;
	}
    void BoostLogTarget::WriteLog(LogLevel eLogLevel, std::string msg)
    {
        if (eLogLevel >= m_eLogLevel)
        {
			switch (eLogLevel)
			{
			case L_TRACE:
				BOOST_LOG_TRIVIAL(trace) << msg;
				break;
			case L_DEBUG:
				BOOST_LOG_TRIVIAL(debug) << msg;
				break;
			case L_INFO:
				BOOST_LOG_TRIVIAL(info) << msg;
				break;
			case L_WARNING:
				BOOST_LOG_TRIVIAL(warning) << msg;
				break;
			case L_ERROR:
				BOOST_LOG_TRIVIAL(error) << msg;
				break;
			case L_FATAL:
				BOOST_LOG_TRIVIAL(fatal) << msg;
				break;
			default:
				break;
			}
        }
    }
	void BoostLogTarget::WriteLog(LogLevel eLogLevel, std::wstring msg)
	{
		WriteLog(eLogLevel, Utility::w2a(msg));
	}

	void BoostLogTarget::SetLogFileName(std::wstring filename)
	{
		mLogFile = filename;
	}
}