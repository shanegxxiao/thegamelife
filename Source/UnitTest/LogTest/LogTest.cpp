#include "UnitTestPCH.h"
#include <boost/test/unit_test.hpp>
#include <boost/filesystem.hpp>
#include <boost/operators.hpp>
#include <boost/bind.hpp>
#include <fstream>
//#include <boost/log/common.hpp>
//#include <boost/log/filters.hpp>
//#include <boost/log/formatters.hpp>
//#include <boost/log/trivial.hpp>
//#include <boost/log/sinks.hpp>
//#include <boost/log/core.hpp>
//#include <boost/log/exceptions.hpp>
//#include <boost/log/attributes.hpp>
//#include <boost/log/utility/init/to_file.hpp>
//#include <boost/log/utility/empty_deleter.hpp>
//#include <boost/log/sources/record_ostream.hpp>
#include "Runtime/LogSystem.h"
#include <atlconv.h>

//namespace logging = boost::log;
//namespace sinks = boost::log::sinks;
//namespace src = boost::log::sources;
//namespace fmt = boost::log::formatters;
//namespace flt = boost::log::filters;
//namespace attrs = boost::log::attributes;
//namespace keywords = boost::log::keywords;

//void init()
//{
//	logging::init_log_to_file
//		(
//		keywords::file_name = "e:\\sample_%N.log",                  // file name pattern
//		keywords::rotation_size = 10 * 1024 * 1024,             // rotate files every 10 MiB...
//		// ...or at midnight
//		keywords::time_based_rotation = sinks::file::rotation_at_time_point(0, 0, 0),
//		keywords::format = "[%TimeStamp%]: %_%"                 // log record format
//		);
//
//	// Construct the sink
//	typedef sinks::synchronous_sink< sinks::text_ostream_backend > text_sink;
//	boost::shared_ptr< text_sink > pSink = boost::make_shared< text_sink >();
//
//	// Add a stream to write log to
//	pSink->locked_backend()->add_stream(boost::make_shared< std::ofstream >("e:\\sample.log"));
//	pSink->locked_backend()->set_formatter
//		(
//		fmt::format("%1%: <%2%> %3%")
//		% fmt::attr< unsigned int >("LineID", keywords::format = "%08x")
//		% fmt::attr< logging::trivial::severity_level >("Severity")
//		% fmt::message()
//		);
//
//	// We have to provide an empty deleter to avoid destroying the global stream object
//	boost::shared_ptr< std::ostream > pStream(&std::clog, logging::empty_deleter());
//	pSink->locked_backend()->add_stream(pStream);
//
//	// Register the sink in the logging core
//	logging::core::get()->add_sink(pSink);
//
//	logging::core::get()->set_filter
//		(
//		flt::attr< logging::trivial::severity_level >("Severity") >= logging::trivial::info
//		);
//}

Runtime::LogSystem gkLogSystem;

namespace Runtime
{
	void InputCallback(Runtime::stdstring strCmd)
	{
		USES_CONVERSION;
		gkLogSystem.WriteLog(Runtime::L_INFO, T2A(strCmd.c_str()));
	}

	bool CtrlHandler(unsigned long msgType)
	{
		return true;
	}
}

BOOST_AUTO_TEST_SUITE(LogTestSuite)

//BOOST_AUTO_TEST_CASE(boostLogTest)
//{
//	init();
//
//	src::logger lg;
//
//	//BOOST_LOG(lg) << "Hello, World test";
//
//	BOOST_LOG_TRIVIAL(trace) << "A trace severity message";
//	BOOST_LOG_TRIVIAL(debug) << "A debug severity message";
//	BOOST_LOG_TRIVIAL(info) << "An informational severity message";
//	BOOST_LOG_TRIVIAL(warning) << "A warning severity message";
//	BOOST_LOG_TRIVIAL(error) << "An error severity message";
//	BOOST_LOG_TRIVIAL(fatal) << "A fatal severity message";
//}
//
//BOOST_AUTO_TEST_CASE(selfDefineLogTest)
//{
//	logging::init_log_to_file("e:\\sample.log");
//	logging::core::get()->set_filter
//		(
//		flt::attr< logging::trivial::severity_level >("Severity") >= logging::trivial::info
//		);
//
//	src::logger lg;
//	logging::record rec = lg.open_record();
//	if (rec)
//	{
//		rec.message() = "Hello world 1";
//		lg.push_record(rec);
//	}
//
//	BOOST_LOG(lg) << "Hello, World 2";
//}

BOOST_AUTO_TEST_CASE(utilityLogTest)
{
	Runtime::LogFile kLogFile;
	Runtime::ConsoleSystem kConsoleSystem(L"UtilityLogTest", 
		boost::bind(&Runtime::InputCallback, _1),
		boost::bind(&Runtime::CtrlHandler, _1),
		GetConsoleWindow());
	Runtime::LogToVSOutput kLogToVSOutput;

	gkLogSystem.AddLogTarget(&kLogFile, false);
	gkLogSystem.AddLogTarget(&kConsoleSystem, true);
	gkLogSystem.AddLogTarget(&kLogToVSOutput, false);

	BOOST_CHECK(gkLogSystem.Initialize());
	while (gbKeepRunningFlag)
	{
		gkLogSystem.Running();
	}
	BOOST_CHECK(gkLogSystem.Shutdown());
}

BOOST_AUTO_TEST_SUITE_END()