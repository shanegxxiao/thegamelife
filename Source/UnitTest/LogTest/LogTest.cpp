#include "UnitTestPCH.h"
#include <boost/test/unit_test.hpp>
#include <boost/filesystem.hpp>
#include <boost/operators.hpp>
#include <boost/bind.hpp>
#include <fstream>
#include "Runtime/LogSystem.h"
#include "Utility/Encode.h"

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

namespace logging = boost::log;
namespace sinks = boost::log::sinks;
namespace src = boost::log::sources;
namespace attrs = boost::log::attributes;
namespace keywords = boost::log::keywords;
namespace expr = boost::log::expressions;

Runtime::LogSystem gkLogSystem;

#ifdef WIN32
namespace Runtime
{
	void InputCallback(Runtime::stdstring strCmd)
	{
		gkLogSystem.WriteLog(Runtime::L_TRACE, Utility::w2a(strCmd));
		gkLogSystem.WriteLog(Runtime::L_TRACE, strCmd);
	}

	bool CtrlHandler(unsigned long msgType)
	{
		return true;
	}
}
#endif//WIN32

BOOST_AUTO_TEST_SUITE(LogTestSuite)

BOOST_AUTO_TEST_CASE(LogSystemTest)
{
    Runtime::LogHtmlFile kLogFile;
	gkLogSystem.AddLogTarget(&kLogFile, false);
	
#ifdef WIN32
	Runtime::LogToVSOutput kLogToVSOutput;
	gkLogSystem.AddLogTarget(&kLogToVSOutput, false);

	Runtime::ConsoleSystem kConsoleSystem(L"UtilityLogTest",
		boost::bind(&Runtime::InputCallback, _1),
		boost::bind(&Runtime::CtrlHandler, _1),
		GetConsoleWindow());
	gkLogSystem.AddLogTarget(&kConsoleSystem, true);
#endif//WIN32

	Runtime::BoostLogTarget kBoostLog;
    gkLogSystem.AddLogTarget(&kBoostLog, false);

    BOOST_CHECK(gkLogSystem.Initialize());

	gkLogSystem.WriteLog(Runtime::L_TRACE, "hello world 中文 L_TRACE");
	gkLogSystem.WriteLog(Runtime::L_DEBUG, "hello world 中文 L_DEBUG");
	gkLogSystem.WriteLog(Runtime::L_INFO, "hello world 中文 L_INFO");
	gkLogSystem.WriteLog(Runtime::L_WARNING, "hello world 中文 L_WARNING");
	gkLogSystem.WriteLog(Runtime::L_ERROR, "hello world 中文 L_ERROR");
	gkLogSystem.WriteLog(Runtime::L_FATAL, "hello world 中文 L_FATAL");

    while (gbKeepRunningFlag)
    {
        gkLogSystem.Running();
    }
    BOOST_CHECK(gkLogSystem.Shutdown());
}

BOOST_AUTO_TEST_CASE(BoostLogTargetTest)
{
    Runtime::BoostLogTarget kBoostLog;

    gkLogSystem.AddLogTarget(&kBoostLog, false);

    BOOST_CHECK(gkLogSystem.Initialize());
    while (gbKeepRunningFlag)
    {
        gkLogSystem.Running();
    }
    BOOST_CHECK(gkLogSystem.Shutdown());
}

/// boost log related
/// @{

void init()
{
	boost::log::register_simple_formatter_factory< boost::log::trivial::severity_level, char >("Severity");
	logging::add_common_attributes();

    logging::add_file_log
        (
        keywords::file_name = "sample_%N.log",
        keywords::rotation_size = 128 * 1024 * 1024,
		keywords::auto_flush = true,
		keywords::open_mode = (std::ios::out | std::ios::app),
        keywords::time_based_rotation = sinks::file::rotation_at_time_point(0, 0, 0),
        keywords::format = "%TimeStamp%[%Severity%]: %Message%"
        );
	logging::add_console_log(
		std::clog,
		keywords::format = "%TimeStamp%[%Severity%]: %Message%"
		);
}

BOOST_AUTO_TEST_CASE(boostLogTest)
{
    init();

    src::logger lg;

    BOOST_LOG(lg) << "Hello, World test";

    BOOST_LOG_TRIVIAL(trace) << "A trace severity message";
    BOOST_LOG_TRIVIAL(debug) << "A debug severity message";
    BOOST_LOG_TRIVIAL(info) << "An informational severity message";
    BOOST_LOG_TRIVIAL(warning) << "A warning severity message";
    BOOST_LOG_TRIVIAL(error) << "An error severity message";
    BOOST_LOG_TRIVIAL(fatal) << "A fatal severity message";
}

//BOOST_AUTO_TEST_CASE(selfDefineLogTest)
//{
//    logging::init_log_to_file("e:\\sample.log");
//    logging::core::get()->set_filter
//        (
//        flt::attr< logging::trivial::severity_level >("Severity") >= logging::trivial::info
//        );
//
//    src::logger lg;
//    logging::record rec = lg.open_record();
//    if (rec)
//    {
//        rec.message() = "Hello world 1";
//        lg.push_record(rec);
//    }
//
//    BOOST_LOG(lg) << "Hello, World 2";
//}


BOOST_AUTO_TEST_SUITE_END()
