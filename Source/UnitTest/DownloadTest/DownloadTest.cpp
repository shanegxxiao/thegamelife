#include "UnitTestPCH.h"
#include <boost/test/unit_test.hpp>
#include "../../Source/Common/Network/DownloadMgr.h"
#include <boost/thread.hpp>
#include <boost/bind.hpp>


#ifdef _DEBUG
#pragma comment(lib, "Network_Debug.lib")
#else
#pragma comment(lib, "Network_Release.lib")
#endif

BOOST_AUTO_TEST_SUITE(DownloadTestSuite)

BOOST_AUTO_TEST_CASE(HttpDownloadTestCase)
{
	try
	{
		std::string strHost = "www.boost.org";
		std::string strFile = "/LICENSE_1_0.txt";
		boost::asio::io_service io_service;

		// Get a list of endpoints corresponding to the server name.
		boost::asio::ip::tcp::resolver resolver(io_service);
		boost::asio::ip::tcp::resolver::query query(strHost.c_str(), "http");
		boost::asio::ip::tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);

		// Try each endpoint until we successfully establish a connection.
		boost::asio::ip::tcp::socket socket(io_service);
		boost::asio::connect(socket, endpoint_iterator);

		// Form the request. We specify the "Connection: close" header so that the
		// server will close the socket after transmitting the response. This will
		// allow us to treat all data up until the EOF as the content.
		boost::asio::streambuf request;
		std::ostream request_stream(&request);
		request_stream << "GET " << strFile << " HTTP/1.0\r\n";
		request_stream << "Host: " << strHost << "\r\n";
		request_stream << "Accept: */*\r\n";
		request_stream << "Connection: close\r\n\r\n";

		// Send the request.
		boost::asio::write(socket, request);

		// Read the response status line. The response streambuf will automatically
		// grow to accommodate the entire line. The growth may be limited by passing
		// a maximum size to the streambuf constructor.
		boost::asio::streambuf response;
		boost::asio::read_until(socket, response, "\r\n");

		// Check that response is OK.
		std::istream response_stream(&response);
		std::string http_version;
		response_stream >> http_version;
		unsigned int status_code;
		response_stream >> status_code;
		std::string status_message;
		std::getline(response_stream, status_message);
		if (!response_stream || http_version.substr(0, 5) != "HTTP/")
		{
			std::cout << "Invalid response\n";
			return;
		}
		if (status_code != 200)
		{
			std::cout << "Response returned with status code " << status_code << "\n";
			return;
		}

		// Read the response headers, which are terminated by a blank line.
		boost::asio::read_until(socket, response, "\r\n\r\n");

		// Process the response headers.
		std::string header;
		while (std::getline(response_stream, header) && header != "\r")
		std::cout << header << "\n";
		std::cout << "\n";

		// Write whatever content we already have to output.
		if (response.size() > 0)
		std::cout << &response;

		// Read until EOF, writing data to output as we go.
		boost::system::error_code error;
		while (boost::asio::read(socket, response,
				boost::asio::transfer_at_least(1), error))
		std::cout << &response;
		if (error != boost::asio::error::eof)
		throw boost::system::system_error(error);
	}
	catch (std::exception& e)
	{
		std::cout << "Exception: " << e.what() << "\n";
	}
}

void statThreadFunc(Network::DownloadMgr* mgr)
{
	if (!mgr)
		return;

	bool paused = false;
	int cnt = 0;
	bool resumed = false;
	while (1)
	{
		mgr->stat();
		std::cout << "\nDownload Speed:" << mgr->getSpeed() << "\n";
		std::cout << "Total Download Bytes:" << mgr->getDownloadedBytes() << "\n";

		//////////////////////////////////////////////////////////////////////////
		// ²âÊÔÏÂÔØ¿ØÖÆ£ºÔÝÍ£¡¢»Ö¸´
		if (paused == false)
		{
			if (mgr->getDownloadedBytes() > 100000000) // ÀÛ¼ÆÏÂÔØ´óÓÚ100M¾ÍÔÝÍ£
			{
				mgr->pauseAllTasks();
				std::cout << "\nAll Tasks Paused!\n";
				paused = true;
			}
		}

		if (paused)
			cnt++;

		if (cnt > 10) // ÔÝÍ£10Ãë£¬Ö®ºó¼ÌÐøÏÂÔØ
		{
			if (resumed == false)
			{
				mgr->resumeAllTasks();
				std::cout << "\nAll Tasks Resumed!\n";
				resumed = true;
			}
		}
		//////////////////////////////////////////////////////////////////////////

		boost::this_thread::sleep(boost::posix_time::milliseconds(1000));
	}
}

BOOST_AUTO_TEST_CASE(HttpDownloadTestCase2)
{
	try
	{
		Network::DownloadMgr* mgr = new Network::DownloadMgr;
		if (!mgr->init())
			return;

		boost::thread_group kThreadGroup;
		kThreadGroup.create_thread(boost::bind(&statThreadFunc, mgr));

		if (!mgr->addTask("http://127.0.0.1:80/patches/test.7z", "test.7z"))
			return;

		kThreadGroup.join_all();

		delete mgr;
		mgr = 0;
	}
	catch (std::exception& e)
	{
		std::cout << "Exception: " << e.what() << "\n";
	}
}

BOOST_AUTO_TEST_SUITE_END()
