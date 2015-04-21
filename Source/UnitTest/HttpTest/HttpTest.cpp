#include "UnitTestPCH.h"
#include <boost/test/unit_test.hpp>
#include <boost/random.hpp>
#include <boost/random/variate_generator.hpp>
#include "BoostHttpClient.h"
#include "Network/HttpNetwork.h"

Network::HttpNetwork mkHttpNetwork;
int g_totalRequestCnt = 0;

BOOST_AUTO_TEST_SUITE(HttpTestSuite)

BOOST_AUTO_TEST_CASE(BoostExample)
{
	try
	{
		boost::asio::io_service io_service;
		client c(io_service, "www.boost.org", "/LICENSE_1_0.txt");
		io_service.run();
	}
	catch (std::exception& e)
	{
		std::cout << "Exception: " << e.what() << "\n";
	}
}

class HttpNetworkCallback
{
public:
	unsigned int muiRequestCnt;
	unsigned int muiResponeCnt;
	unsigned int muiErrorCnt;

public:
	HttpNetworkCallback()
	{
		muiRequestCnt = 0;
		muiResponeCnt = 0;
	}
	void HttpRequestHandler(Network::HttpRequestPtr spRequest)
	{
		++muiRequestCnt;
		std::cout << spRequest->method << std::endl;
		std::cout << spRequest->http_version << std::endl;
		for (Network::Headers::iterator kIter = spRequest->headers.begin();
			kIter != spRequest->headers.end(); ++kIter)
		{
			std::cout << kIter->first << ":" << kIter->second << std::endl;
		}
		std::cout << spRequest->content << std::endl;
		std::cout << "HttpResponeHandler count " << muiRequestCnt <<std::endl;

		Network::HttpResponePtr spRespone = spRequest->GenerateRespone();
		spRespone->setPriority(1.f);
		spRespone->status_code = 200;
		spRespone->putHeaderField("Content-Type", "text/x-json");
		spRespone->putHeaderField("Content-Length", "11");
		spRespone->content = "hello world";
		mkHttpNetwork.sendRespone(spRespone);
	}
	void HttpResponeHandler(Network::HttpResponePtr spRespone)
	{
		++muiResponeCnt;
		std::cout << spRespone->content << std::endl;
		std::cout << "HttpResponeHandler count " << muiResponeCnt <<std::endl;
	}
	void HttpClientErrorHandler(Network::HttpConnectionPtr spConnection)
	{
		++muiErrorCnt;
		std::cout << "Http Client Error:" << std::endl;
		std::cout << spConnection->getLastError().value() << ":" << spConnection->getLastError().message() << std::endl;
		std::cout << muiErrorCnt << ":" << spConnection->getRespondString() << std::endl;
	}
	void HttpServerErrorHandler(Network::HttpConnectionPtr spConnection)
	{
		++muiErrorCnt;
		std::cout << "Http Server Error:" << std::endl;
		std::cout << spConnection->getLastError().value() << ":" << spConnection->getLastError().message() << std::endl;
		std::cout << muiErrorCnt << ":" << spConnection->getRespondString() << std::endl;
	}
};

HttpNetworkCallback kCallback;

void addRequest()
{
	//Network::HttpRequestPtr spRequestLBW;
	//spRequestLBW.reset(new Network::HttpRequest);
	//spRequestLBW->setPriority(Network::HttpRequestPriority::highest);
	//spRequestLBW->method = "GET";
	//spRequestLBW->uri = "/api/login/Check_Cssh_login_Api.html";
	//spRequestLBW->uri += "?uname=test";
	//spRequestLBW->uri += "&upwd=098f6bcd4621d373cade4e832627b4f6";
	//spRequestLBW->uri += "&sn=2305866279502761984";
	//spRequestLBW->uri += "&ucheckstr=836c44f65d7cb50e73dde56b7490f06b";
	//spRequestLBW->putHeaderField("Host", "www.lbwgame.com");
	//spRequestLBW->putHeaderField("Accept", "text/x-json,*/*");
	//spRequestLBW->putHeaderField("Connection", "close");
	//mkHttpNetwork.sendRequest(spRequestLBW);

	Network::HttpRequestPtr spRequestBoost;
	spRequestBoost.reset(new Network::HttpRequest);
	spRequestBoost->setPriority(Network::HttpRequestPriority::lowest);
	spRequestBoost->method = "GET";
	spRequestBoost->uri = "/LICENSE_1_0.txt";
	spRequestBoost->putHeaderField("Host", "www.boost.org");
	spRequestBoost->putHeaderField("Accept", "text/x-json,*/*");
	spRequestBoost->putHeaderField("Connection", "close");
	mkHttpNetwork.sendRequest(spRequestBoost);

	//Network::HttpRequestPtr spRequestLocal;
	//spRequestLocal.reset(new Network::HttpRequest);
	//spRequestLocal->setPriority(Network::HttpRequestPriority::lowest);
	//spRequestLocal->method = "GET";
	//spRequestLocal->uri = "/test.html";
	//spRequestLocal->putHeaderField("Host", "127.0.0.1");
	//spRequestLocal->putHeaderField("Accept", "text/x-json,*/*");
	//spRequestLocal->putHeaderField("Connection", "close");
	//mkHttpNetwork.sendRequest(spRequestLocal);
	

	++g_totalRequestCnt;
}

BOOST_AUTO_TEST_CASE(HttpNetworkTest0)
{
	mkHttpNetwork.startup();
	mkHttpNetwork.bindRequestHandler(boost::bind(&HttpNetworkCallback::HttpRequestHandler, 
		&kCallback, _1));
	mkHttpNetwork.bindResponeHandler(boost::bind(&HttpNetworkCallback::HttpResponeHandler, 
		&kCallback, _1));
	mkHttpNetwork.bindClientErrorHandler(boost::bind(&HttpNetworkCallback::HttpClientErrorHandler, 
		&kCallback, _1));

	for (unsigned int ui = 0; ui < 1000; ++ui)
	{
		addRequest();
	}

	while (gbKeepRunningFlag)
	{
		mkHttpNetwork.running();
	}
	mkHttpNetwork.shutdown();
}

BOOST_AUTO_TEST_CASE(HttpNetworkTest1)
{
	mkHttpNetwork.startup();
	mkHttpNetwork.bindRequestHandler(boost::bind(&HttpNetworkCallback::HttpRequestHandler, 
		&kCallback, _1));
	mkHttpNetwork.bindResponeHandler(boost::bind(&HttpNetworkCallback::HttpResponeHandler, 
		&kCallback, _1));
	mkHttpNetwork.bindClientErrorHandler(boost::bind(&HttpNetworkCallback::HttpClientErrorHandler, 
		&kCallback, _1));

	boost::uniform_int<> distributionRequest(100, 200);
	boost::uniform_int<> distributionTime(1000, 1000);
	boost::mt19937 engine;
	boost::variate_generator< boost::mt19937, boost::uniform_int<> > randomRequest(engine, distributionRequest);
	boost::variate_generator< boost::mt19937, boost::uniform_int<> > randomTime(engine, distributionTime);
	int randomRequestCnt = randomRequest();
	int randomTimeCnt = randomTime();
	g_totalRequestCnt = 0;
	boost::posix_time::ptime mkLastTick = boost::posix_time::microsec_clock::universal_time();

	while (gbKeepRunningFlag)
	{
		boost::posix_time::millisec_posix_time_system_config::time_duration_type time_elapse = 
			boost::posix_time::microsec_clock::universal_time() - mkLastTick;
		if (time_elapse.total_milliseconds() >= randomTimeCnt)
		{
			std::cout << "total request num = " << g_totalRequestCnt << std::endl;
			std::cout << "success num = " << kCallback.muiResponeCnt << std::endl;
			std::cout << "error num = " << kCallback.muiErrorCnt << std::endl;
			std::cout << "unfinished num = " << 
				(g_totalRequestCnt - kCallback.muiResponeCnt - kCallback.muiErrorCnt) << std::endl;

			for (int si = 0; si < randomRequestCnt; ++si)
			{
				addRequest();
			}
			randomRequestCnt = randomRequest();
			randomTimeCnt = randomTime();
			mkLastTick = boost::posix_time::microsec_clock::universal_time();
		}
		mkHttpNetwork.running();
	}
	while (g_totalRequestCnt > (int)(kCallback.muiResponeCnt + kCallback.muiErrorCnt))
	{
		boost::this_thread::sleep(boost::posix_time::milliseconds(100));
		mkHttpNetwork.running();
		std::cout << "total request num = " << g_totalRequestCnt << std::endl;
		std::cout << "success num = " << kCallback.muiResponeCnt << std::endl;
		std::cout << "error num = " << kCallback.muiErrorCnt << std::endl;
		std::cout << "unfinished num = " << 
			(g_totalRequestCnt - kCallback.muiResponeCnt - kCallback.muiErrorCnt) << std::endl;
	}
	mkHttpNetwork.shutdown();
}

BOOST_AUTO_TEST_CASE(HttpNetworkTest2)
{
	mkHttpNetwork.startup();
	mkHttpNetwork.bindRequestHandler(boost::bind(&HttpNetworkCallback::HttpRequestHandler, 
		&kCallback, _1));
	mkHttpNetwork.bindResponeHandler(boost::bind(&HttpNetworkCallback::HttpResponeHandler, 
		&kCallback, _1));
	mkHttpNetwork.bindClientErrorHandler(boost::bind(&HttpNetworkCallback::HttpClientErrorHandler, 
		&kCallback, _1));
	mkHttpNetwork.bindServerErrorHandler(boost::bind(&HttpNetworkCallback::HttpServerErrorHandler, 
		&kCallback, _1));

	mkHttpNetwork.listen("0.0.0.0", 80);

	while (gbKeepRunningFlag)
	{
		mkHttpNetwork.running();
	}
	mkHttpNetwork.shutdown();
}

BOOST_AUTO_TEST_SUITE_END()
