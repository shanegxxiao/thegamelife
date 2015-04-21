#include "UnitTestPCH.h"
#include <boost/test/unit_test.hpp>
#include <boost/random.hpp>
#include <boost/random/variate_generator.hpp>
#include "BoostHttpClient.h"
#include "Network/HttpNetwork.h"

Network::HttpNetwork mkHttpsNetwork;
int g_totalHttpsRequestCnt = 0;

BOOST_AUTO_TEST_SUITE(HttpsTestSuite)

class HttpsNetworkCallback
{
public:
	unsigned int muiRequestCnt;
	unsigned int muiResponeCnt;
	unsigned int muiErrorCnt;

public:
	HttpsNetworkCallback()
	{
		muiRequestCnt = 0;
		muiResponeCnt = 0;
	}
	void HttpsRequestHandler(Network::HttpRequestPtr spRequest)
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
		std::cout << "HttpsResponeHandler count " << muiRequestCnt <<std::endl;

		Network::HttpResponePtr spRespone = spRequest->GenerateRespone();
		spRespone->setPriority(1.f);
		spRespone->status_code = 200;
		spRespone->putHeaderField("Content-Type", "text/x-json");
		spRespone->putHeaderField("Content-Length", "11");
		spRespone->content = "hello world";
		mkHttpsNetwork.sendRespone(spRespone);
	}
	void HttpsResponeHandler(Network::HttpResponePtr spRespone)
	{
		++muiResponeCnt;
		std::cout << spRespone->content << std::endl;
		std::cout << "HttpsResponeHandler count " << muiResponeCnt <<std::endl;
	}
	void HttpsClientErrorHandler(Network::HttpConnectionPtr spConnection)
	{
		++muiErrorCnt;
		std::cout << "Https Client Error:" << std::endl;
		std::cout << spConnection->getLastError().value() << ":" << spConnection->getLastError().message() << std::endl;
		std::cout << muiErrorCnt << ":" << spConnection->getRespondString() << std::endl;
	}
	void HttpsServerErrorHandler(Network::HttpConnectionPtr spConnection)
	{
		++muiErrorCnt;
		std::cout << "Https Server Error:" << std::endl;
		std::cout << spConnection->getLastError().value() << ":" << spConnection->getLastError().message() << std::endl;
		std::cout << muiErrorCnt << ":" << spConnection->getRespondString() << std::endl;
	}

	std::string getPassword()
	{
		return "test";
	}
};

HttpsNetworkCallback kCallback;

void addRequest()
{
	Network::HttpRequestPtr spRequestBSH;
	spRequestBSH.reset(new Network::HttpRequest);
	spRequestBSH->useSSL(true);
	spRequestBSH->setPriority(Network::HttpRequestPriority::lowest);
	spRequestBSH->method = "GET";
	spRequestBSH->uri = "/games/xxfx/API/Check.ashx";
	spRequestBSH->putHeaderField("Host", "www.1465.tw:443");
	spRequestBSH->putHeaderField("Accept", "text/x-json,*/*");
	spRequestBSH->putHeaderField("Connection", "close");
	mkHttpsNetwork.sendRequest(spRequestBSH);
	
	++g_totalHttpsRequestCnt;
}

BOOST_AUTO_TEST_CASE(HttpsNetworkTest0)
{
	Network::HttpNetworkParams kHttpParams = Network::HttpNetworkParams::Default();
	kHttpParams.bUseSSL = true;
	mkHttpsNetwork.startup(kHttpParams);
	mkHttpsNetwork.bindRequestHandler(boost::bind(&HttpsNetworkCallback::HttpsRequestHandler, 
		&kCallback, _1));
	mkHttpsNetwork.bindResponeHandler(boost::bind(&HttpsNetworkCallback::HttpsResponeHandler, 
		&kCallback, _1));
	mkHttpsNetwork.bindClientErrorHandler(boost::bind(&HttpsNetworkCallback::HttpsClientErrorHandler, 
		&kCallback, _1));

	for (unsigned int ui = 0; ui < 1000; ++ui)
	{
		addRequest();
	}

	while (gbKeepRunningFlag)
	{
		mkHttpsNetwork.running();
	}
	mkHttpsNetwork.shutdown();
}
BOOST_AUTO_TEST_CASE(HttpsNetworkTest1)
{
	Network::HttpNetworkParams kHttpParams = Network::HttpNetworkParams::Default();
	kHttpParams.bUseSSL = true;
	mkHttpsNetwork.startup(kHttpParams);
	mkHttpsNetwork.bindRequestHandler(boost::bind(&HttpsNetworkCallback::HttpsRequestHandler, 
		&kCallback, _1));
	mkHttpsNetwork.bindResponeHandler(boost::bind(&HttpsNetworkCallback::HttpsResponeHandler, 
		&kCallback, _1));
	mkHttpsNetwork.bindClientErrorHandler(boost::bind(&HttpsNetworkCallback::HttpsClientErrorHandler, 
		&kCallback, _1));

	boost::uniform_int<> distributionRequest(100, 200);
	boost::uniform_int<> distributionTime(1000, 1000);
	boost::mt19937 engine;
	boost::variate_generator< boost::mt19937, boost::uniform_int<> > randomRequest(engine, distributionRequest);
	boost::variate_generator< boost::mt19937, boost::uniform_int<> > randomTime(engine, distributionTime);
	int randomRequestCnt = randomRequest();
	int randomTimeCnt = randomTime();
	g_totalHttpsRequestCnt = 0;
	boost::posix_time::ptime mkLastTick = boost::posix_time::microsec_clock::universal_time();

	while (gbKeepRunningFlag)
	{
		boost::posix_time::millisec_posix_time_system_config::time_duration_type time_elapse = 
			boost::posix_time::microsec_clock::universal_time() - mkLastTick;
		if (time_elapse.total_milliseconds() >= randomTimeCnt)
		{
			std::cout << "total request num = " << g_totalHttpsRequestCnt << std::endl;
			std::cout << "success num = " << kCallback.muiResponeCnt << std::endl;
			std::cout << "error num = " << kCallback.muiErrorCnt << std::endl;
			std::cout << "unfinished num = " << 
				(g_totalHttpsRequestCnt - kCallback.muiResponeCnt - kCallback.muiErrorCnt) << std::endl;

			for (int si = 0; si < randomRequestCnt; ++si)
			{
				addRequest();
			}
			randomRequestCnt = randomRequest();
			randomTimeCnt = randomTime();
			mkLastTick = boost::posix_time::microsec_clock::universal_time();
		}
		mkHttpsNetwork.running();
	}
	while (g_totalHttpsRequestCnt > (int)(kCallback.muiResponeCnt + kCallback.muiErrorCnt))
	{
		boost::this_thread::sleep(boost::posix_time::milliseconds(100));
		mkHttpsNetwork.running();
		std::cout << "total request num = " << g_totalHttpsRequestCnt << std::endl;
		std::cout << "success num = " << kCallback.muiResponeCnt << std::endl;
		std::cout << "error num = " << kCallback.muiErrorCnt << std::endl;
		std::cout << "unfinished num = " << 
			(g_totalHttpsRequestCnt - kCallback.muiResponeCnt - kCallback.muiErrorCnt) << std::endl;
	}
	mkHttpsNetwork.shutdown();
}

BOOST_AUTO_TEST_CASE(HttpsServerTest)
{
	Network::HttpNetworkParams kParams = Network::HttpNetworkParams::Default();
	kParams.bUseSSL = true;
	kParams.uiSSLOption = boost::asio::ssl::context::default_workarounds
		| boost::asio::ssl::context::no_sslv2
		| boost::asio::ssl::context::single_dh_use;
	kParams.pfnPwdCallback = boost::bind(&HttpsNetworkCallback::getPassword, &kCallback);
	kParams.strCertificateChainFile = "crt.pem";
	kParams.strPrivateKeyFile = "key.pem";
	kParams.strDHParamFile = "dhparam.pem";
	kParams.strCipherList = "RC4-MD5";
	mkHttpsNetwork.startup(kParams);
	mkHttpsNetwork.bindRequestHandler(boost::bind(&HttpsNetworkCallback::HttpsRequestHandler, 
		&kCallback, _1));
	mkHttpsNetwork.bindResponeHandler(boost::bind(&HttpsNetworkCallback::HttpsResponeHandler, 
		&kCallback, _1));
	mkHttpsNetwork.bindClientErrorHandler(boost::bind(&HttpsNetworkCallback::HttpsClientErrorHandler, 
		&kCallback, _1));
	mkHttpsNetwork.bindServerErrorHandler(boost::bind(&HttpsNetworkCallback::HttpsServerErrorHandler, 
		&kCallback, _1));

	mkHttpsNetwork.listen("0.0.0.0", 443);

	while (gbKeepRunningFlag)
	{
		mkHttpsNetwork.running();
	}
	mkHttpsNetwork.shutdown();
}

BOOST_AUTO_TEST_SUITE_END()
