#include "NetworkPCH.h"
#include "HttpNetwork.h"
#include "HttpClientConnection.h"
#include "HttpServerConnection.h"
#include "HttpsClientConnection.h"
#include "HttpsServerConnection.h"

namespace Network
{
	HttpNetwork::HttpNetwork()
		: mkAcceptor(mkIOService)
		, mpkSSLContext(0)
	{

	}
	HttpNetwork::~HttpNetwork()
	{

	}
	bool HttpNetwork::startup(const Network::HttpNetworkParams& kParams)
	{
		mkHttpParameters = kParams;
		mpkIOWork = new boost::asio::io_service::work(mkIOService);

		unsigned int uiHardwareConcurrency = 1;
		if (kParams.uiIOThreadCount == 0)
		{
			uiHardwareConcurrency = boost::thread::hardware_concurrency();
		}
		for (unsigned int ui = 0; ui < uiHardwareConcurrency; ++ui)
		{
			mkThreadGroup.create_thread(boost::bind(&boost::asio::io_service::run, &mkIOService));
		}

		if (mpkSSLContext)
		{
			delete mpkSSLContext;
			mpkSSLContext = 0;
		}
		if (kParams.bUseSSL)
		{
			/// 初始化SSL上下文
			try
			{
				mpkSSLContext = new boost::asio::ssl::context(mkIOService, kParams.eSSLMethod);
				mpkSSLContext->set_options(kParams.uiSSLOption);
				mpkSSLContext->set_verify_mode(kParams.uiVerifyMode);
				if (kParams.bUseDefaultVerifyPaths)
				{
					mpkSSLContext->set_default_verify_paths();
				}
				if (kParams.pfnPwdCallback)
				{
					mpkSSLContext->set_password_callback(kParams.pfnPwdCallback);
				}
				if (kParams.pfnVerifyCallback)
				{
					mpkSSLContext->set_verify_callback(kParams.pfnVerifyCallback);
				}
				if (kParams.strCertificateChainFile.length() > 0)
				{
					mpkSSLContext->use_certificate_chain_file(kParams.strCertificateChainFile);
				}
				if (kParams.strPrivateKeyFile.length() > 0)
				{
					mpkSSLContext->use_private_key_file(kParams.strPrivateKeyFile,
						boost::asio::ssl::context::pem);
				}
				if (kParams.strDHParamFile.length() > 0)
				{
					mpkSSLContext->use_tmp_dh_file(kParams.strDHParamFile);
				}
				if (kParams.strVerifyFile.length() > 0)
				{
					mpkSSLContext->load_verify_file(kParams.strVerifyFile);
				}
				if (kParams.strCipherList.length() > 0)
				{
					SSL_CTX_set_cipher_list(mpkSSLContext->native_handle(), 
						kParams.strCipherList.c_str());
				}
			}
			catch (...)
			{
				return false;
			}
		}
		return true;
	}
	bool HttpNetwork::shutdown()
	{
		boost::system::error_code kErrorCode;
		mkAcceptor.cancel(kErrorCode);
		mkAcceptor.close(kErrorCode);
		if (mpkIOWork)
		{
			delete mpkIOWork;
			mpkIOWork = NULL;
		}
		mkIOService.stop();
		mkThreadGroup.join_all();
		if (mpkSSLContext)
		{
			delete mpkSSLContext;
			mpkSSLContext = NULL;
		}
		return true;
	}
	bool HttpNetwork::listen(const char* pcListenIP, unsigned int uiListenPort)
	{
		if (!pcListenIP)
		{
			return false;
		}
		mstrListenIP = pcListenIP;
		muiListenPort = uiListenPort;
		return startAccept();
	}
	void HttpNetwork::running()
	{
		handleRequestSendingQueue();
		handleResponeRecvingQueue();
		handleRequestRecvingQueue();
		handleResponeSendingQueue();
		handleSendingResponeConnection();
	}
	HttpConnectionPtr HttpNetwork::getIdealClientConnection(bool bSSL)
	{
		HttpConnectionPtr spConnection;
		if (bSSL)
		{
			spConnection.reset(new HttpsClientConnection(mkIOService, *mpkSSLContext,
				mkHttpParameters));
		}
		else
		{
			spConnection.reset(new HttpClientConnection(mkIOService, mkHttpParameters));
		}
		return spConnection;
	}
	void HttpNetwork::putIdealClientConnection(HttpConnectionPtr spConnection)
	{
		spConnection->reset();
		spConnection.reset();
	}
	HttpConnectionPtr HttpNetwork::getIdealServerConnection(bool bSSL)
	{
		HttpConnectionPtr spConnection;
		if (bSSL)
		{
			spConnection.reset(new HttpsServerConnection(mkIOService, *mpkSSLContext,
				mkHttpParameters));
		}
		else
		{
			spConnection.reset(new HttpServerConnection(mkIOService, mkHttpParameters));
		}
		return spConnection;
	}
	void HttpNetwork::putIdealServerConnection(HttpConnectionPtr spConnection)
	{
		spConnection->reset();
		spConnection.reset();
	}
	void HttpNetwork::addRecvingResponeConnection(HttpConnectionPtr spConnection)
	{
		mkRecvingResponeConVector.push_back(spConnection);
	}
	void HttpNetwork::handleRequestSendingQueue()
	{
		unsigned int uiRequestSendCntAtThisFrame = 0;
		HttpRequestPtr spRequest = getRequestFromSendingQueue();
		while (spRequest)
		{
			HttpConnectionPtr spConnection = getIdealClientConnection(spRequest->isUseSSL());
			addRecvingResponeConnection(spConnection);
			spConnection->open(spRequest);
			++uiRequestSendCntAtThisFrame;
			if (uiRequestSendCntAtThisFrame < mkHttpParameters.uiRequestSendNumPerFrame)
			{
				spRequest = getRequestFromSendingQueue();
			}
			else
			{
				spRequest.reset();
			}
		}
	}
	void HttpNetwork::handleResponeRecvingQueue()
	{
		unsigned int uiResponeProcessCnt = 0;
		HttpClientConnectionVector::iterator kIter = mkRecvingResponeConVector.begin();
		while (kIter != mkRecvingResponeConVector.end() &&
			uiResponeProcessCnt < mkHttpParameters.uiResponeProcessNumPerFrame)
		{
			++uiResponeProcessCnt;
			HttpConnectionPtr spConnection = (*kIter);
			spConnection->running();
			if (spConnection->canBeClose())
			{
				spConnection->close();
			}
			else
			{
				HttpResponePtr spResponed = spConnection->getResponed();
				if (spResponed)
				{
					if (mkResponeHandler)
					{
						mkResponeHandler(spResponed);
					}
					/// response handler处理之后，链接关闭,
					/// response持有的request，request持有的client connection
					/// 将在后面判断关闭后被重置，client connection将不在持有
					/// respone和request
					/// @{
					spConnection->close();
					/// @}
				}
			}
			if (spConnection->getLastError())
			{
				if (mkClientErrorHandler)
				{
					mkClientErrorHandler(spConnection);
				}
				spConnection->close();
			}
			if (spConnection->isClosed())
			{
				putIdealClientConnection(spConnection);
				spConnection.reset();
				kIter = mkRecvingResponeConVector.erase(kIter);
			}
			else
			{
				++kIter;
			}
		}
	}
	bool HttpNetwork::startAccept()
	{
		if (mstrListenIP.empty())
		{
			return false;
		}
		try
		{
			boost::asio::ip::address kAdrs = boost::asio::ip::address::from_string(mstrListenIP.c_str());
			boost::asio::ip::tcp::endpoint kEndpoint(kAdrs, muiListenPort);
			mkAcceptor.open(kEndpoint.protocol());
			mkAcceptor.set_option(boost::asio::ip::tcp::acceptor::reuse_address(false));
			mkAcceptor.bind(kEndpoint);
			mkAcceptor.listen();
		}
		catch(...)
		{
			return false;
		}

		for (unsigned int ui = 0; ui < mkHttpParameters.uiListenConnectionNum; ui++)
		{
			HttpConnectionPtr spConnection = getIdealServerConnection(mkHttpParameters.bUseSSL);
			mkAcceptor.async_accept(spConnection->getSocket(),
				boost::bind(&HttpNetwork::onAccept, this, spConnection,
				boost::asio::placeholders::error));
		}
		return true;
	}
	void HttpNetwork::onAccept(HttpConnectionPtr& pkConnection,
		const boost::system::error_code& kError)
	{
		if (kError)
		{
			pkConnection->setStatus(HttpConnection::Status::error_occur);
			pkConnection->setLastError(kError);
		}
		else
		{
			HttpRequestPtr spRequest;
			spRequest.reset(new HttpRequest);
			pkConnection->open(spRequest);
		}
		addRecvingRequestConnection(pkConnection);

		HttpConnectionPtr spConnection = getIdealServerConnection(mkHttpParameters.bUseSSL);
		mkAcceptor.async_accept(spConnection->getSocket(),
			boost::bind(&HttpNetwork::onAccept, this, spConnection,
			boost::asio::placeholders::error));
	}
	void HttpNetwork::addRecvingRequestConnection(HttpConnectionPtr spConnection)
	{
		boost::recursive_mutex::scoped_lock kScopedLock(mkRecvingRequestConVectorMutex);
		mkRecvingRequestConVector.push_back(spConnection);
	}
	void HttpNetwork::eraseRequestRecvingConnection(HttpConnectionPtr spConnection)
	{
		boost::recursive_mutex::scoped_lock kScopedLock(mkRecvingRequestConVectorMutex);
		for (HttpServerConnectionVector::iterator kIter = mkRecvingRequestConVector.begin();
			kIter != mkRecvingRequestConVector.end(); ++kIter)
		{
			if (*kIter == spConnection)
			{
				mkRecvingRequestConVector.erase(kIter);
				return;
			}
		}
	}
	void HttpNetwork::handleRequestRecvingQueue()
	{
		unsigned int uiConnectionProcessedCnt = 0;
		HttpConnectionPtr spConnection = getRequestRecvingConnection();
		while (spConnection &&
			uiConnectionProcessedCnt < mkHttpParameters.uiRequestProcessNumPerFrame)
		{
			++uiConnectionProcessedCnt;
			spConnection->running();
			if (spConnection->canBeClose())
			{
				spConnection->close();
			}
			else
			{
				HttpRequestPtr spRequest = spConnection->getRequest();
				if (spRequest)
				{
					if (mkRequestHandler)
					{
						mkRequestHandler(spRequest);
					}
					spConnection->setStatus(HttpConnection::Status::request_processed);
				}
			}
			if (spConnection->getLastError())
			{
				if (mkServerErrorHandler)
				{
					mkServerErrorHandler(spConnection);
				}
				spConnection->close();
			}
			if (spConnection->isClosed())
			{
				eraseRequestRecvingConnection(spConnection);
				putIdealServerConnection(spConnection);
				spConnection.reset();
			}
			else if (spConnection->isRequestProcessed())
			{
				eraseRequestRecvingConnection(spConnection);
				if ((spConnection->getStatus() & HttpConnection::waiting_response) == 0 &&
					(spConnection->getStatus() & HttpConnection::pending_response) == 0)
				{
					spConnection->close();
					putIdealServerConnection(spConnection);
				}
			}
			spConnection = getRequestRecvingConnection();
		}
	}
	void HttpNetwork::handleResponeSendingQueue()
	{
		unsigned int uiResponeSendCnt = 0;
		HttpResponePtr spRespone = getResponeFromSendingQueue();
		while (spRespone)
		{
			HttpConnectionPtr spServerConnection = spRespone->getRequest()->getServerConnection();
			addSendingResponeConnection(spServerConnection);
			spRespone->send();
			++uiResponeSendCnt;
			if (uiResponeSendCnt < mkHttpParameters.uiResponeSendNumPerFrame)
			{
				spRespone = getResponeFromSendingQueue();
			}
			else
			{
				spRespone.reset();
			}
		}
	}
	void HttpNetwork::handleSendingResponeConnection()
	{
		HttpServerConnectionVector::iterator kIter = mkSendingResponeConVector.begin();
		while (kIter != mkSendingResponeConVector.end())
		{
			HttpConnectionPtr spConnection = *kIter;
			if (spConnection->isResponseSent())
			{
				if (mkResponeSentHandler)
				{
					mkResponeSentHandler(*kIter);
				}
				spConnection->close();
			}
			if (spConnection->getLastError())
			{
				if (mkServerErrorHandler)
				{
					mkServerErrorHandler(spConnection);
				}
				spConnection->close();
			}
			if (spConnection->isClosed())
			{
				putIdealServerConnection(spConnection);
				kIter = mkSendingResponeConVector.erase(kIter);
				continue;
			}
			++kIter;
		}
	}
}