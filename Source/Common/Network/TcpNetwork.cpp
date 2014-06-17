#include "NetworkPCH.h"
#include "TcpNetwork.h"
#include "IdealConnectionContainer.h"
#include "ActiveConnectionContainer.h"

namespace Network
{
	TcpNetwork::TcpNetwork()
		: muiListenPort(0),
		mkAcceptor(mkIOService),
		mpkIdealConContainer(0),
		mpkActiveConContainer(0),
		mpcRecvTmpBuffer(0),
		mbStartup(false),
		mpkSSLContext(0)
	{
	}

	bool TcpNetwork::startup(const TcpNetworkParams& kParams/* = TcpNetworkParams::Default()*/)
	{
		mkNetworkParams = kParams;

		if (mpkSSLContext)
		{
			delete mpkSSLContext;
			mpkSSLContext = 0;
		}
		if (kParams.bUseSSL)
		{
			/// 初始化SSL上下文
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

		mpkIdealConContainer = new IdealConnectionContainer(
			mkIOService, &mkNetworkParams, mpkSSLContext);
		mpkActiveConContainer = new ActiveConnectionContainer();
		mpcRecvTmpBuffer = new unsigned char[mkNetworkParams.uiRecvBuffSize];

		mpkIOWork = new boost::asio::io_service::work(mkIOService);

		unsigned int uiHardwareConcurrency = 1;
		if (mkNetworkParams.uiIOThreadCount == 0)
		{
			uiHardwareConcurrency = boost::thread::hardware_concurrency();
		}
		for (unsigned int ui = 0; ui < uiHardwareConcurrency; ++ui)
		{
			mkThreadGroup.create_thread(boost::bind(&boost::asio::io_service::run, &mkIOService));
		}
		mbStartup = true;
		return true;
	}

	bool TcpNetwork::listen(const char* pcListenIP, unsigned int uiListenPort)
	{
		if (!pcListenIP)
		{
			return false;
		}
		mstrListenIP = pcListenIP;
		muiListenPort = uiListenPort;
		return startAccept();
	}

	void TcpNetwork::running()
	{
		handleWaitForActivingConnections();
		handleAllActivedConnections();
		handleErrorConnections();
	}

	bool TcpNetwork::shutdown()
	{
		boost::system::error_code kErrorCode;
		mkAcceptor.cancel(kErrorCode);
		mkAcceptor.close(kErrorCode);

		mbStartup = false;
	    if (mpkActiveConContainer)
	    {
	        delete mpkActiveConContainer;
	        mpkActiveConContainer = NULL;
	    }
	    if (mpkIOWork)
	    {
	        delete mpkIOWork;
	        mpkIOWork = NULL;
	    }
		mkIOService.stop();
		mkThreadGroup.join_all();
		if (mpkIdealConContainer)
	    {
	        delete mpkIdealConContainer;
	        mpkIdealConContainer = NULL;
	    }
	    if (mpcRecvTmpBuffer)
	    {
	        delete[] mpcRecvTmpBuffer;
	        mpcRecvTmpBuffer = NULL;
	    }
		if (mpkSSLContext)
		{
			delete mpkSSLContext;
			mpkSSLContext = NULL;
		}
		return true;
	}
	bool TcpNetwork::tryShutdown()
	{
		mbStartup = false;
		boost::system::error_code kErrorCode;
		mkAcceptor.cancel(kErrorCode);
		mkAcceptor.close(kErrorCode);
		mpkActiveConContainer->mkRecursiveMutex.lock();
		std::map<ConnectionID, ConnectionPtr>::iterator kIter = 
			mpkActiveConContainer->mkConnectionMap.begin();
		while (kIter != mpkActiveConContainer->mkConnectionMap.end())
		{
			kIter->second->asyncClose();
			++kIter;
		}
		mpkActiveConContainer->mkRecursiveMutex.unlock();

		running();

		mkWaitActiveConListMutex.lock();
		unsigned int uiWaitActiveConListSize = mkWaitActiveConList.size();
		mkWaitActiveConListMutex.unlock();

		mkHandshakingConMapMutex.lock();
		unsigned int uiHandshakingConMapSize = mkHandshakingConMap.size();
		mkHandshakingConMapMutex.unlock();

		if (mpkActiveConContainer->size() == 0 && 
			uiWaitActiveConListSize == 0 &&
			uiHandshakingConMapSize == 0)
		{
			shutdown();
			return true;
		}
		return false;
	}
	ConnectionID TcpNetwork::connect(const char* pcServerIP, 
		unsigned int uiServerPort, ConnectionType eConnectionType/* = CT_DEFAULT*/)
	{
		if (!mbStartup)
		{
			return INVALID_CONNECTION_ID;
		}
		boost::asio::ip::address kAdrs = boost::asio::ip::address::from_string(pcServerIP);
		boost::asio::ip::tcp::endpoint kEndpoint(kAdrs, uiServerPort);

		ConnectionPtr pkConnection = mpkIdealConContainer->get(eConnectionType);
		if (!pkConnection)
		{
			return INVALID_CONNECTION_ID;
		}
		pkConnection->setAsClientSide();
		boost::system::error_code kErrorCode;
		pkConnection->connect(kEndpoint, kErrorCode);

		if (kErrorCode)
		{
			pkConnection->pushErrorCode(kErrorCode);
			boost::lock_guard<boost::mutex> kLockGuard(mkErrorConListMutex);
			mkErrorConList.push_back(pkConnection);
			return INVALID_CONNECTION_ID;
		}
		if (pkConnection->isSSLConnection())
		{
			boost::system::error_code kErrorCode;
			pkConnection->handShake(kErrorCode);
			if (kErrorCode)
			{
				pkConnection->pushErrorCode(kErrorCode);
				boost::lock_guard<boost::mutex> kLockGuard(mkErrorConListMutex);
				mkErrorConList.push_back(pkConnection);
				return INVALID_CONNECTION_ID;
			}
		}
		if (!pkConnection->open())
		{
			boost::lock_guard<boost::mutex> kLockGuard(mkErrorConListMutex);
			mkErrorConList.push_back(pkConnection);
			return INVALID_CONNECTION_ID;
		}

		mpkActiveConContainer->put(pkConnection->getID(), pkConnection);
		if (mpOnConFunc)
		{
			mpOnConFunc(pkConnection->getID());
		}
		return pkConnection->getID();
	}

	ConnectionID TcpNetwork::asyncConnect(const char* pcServerIP, 
		unsigned int uiServerPort, ConnectionType eConnectionType/* = CT_DEFAULT*/)
	{
		if (!mbStartup)
		{
			return INVALID_CONNECTION_ID;
		}
		boost::asio::ip::address kAdrs = boost::asio::ip::address::from_string(pcServerIP);
		boost::asio::ip::tcp::endpoint kEndpoint(kAdrs, uiServerPort);

		ConnectionPtr pkConnection = mpkIdealConContainer->get(eConnectionType);
		if (!pkConnection)
		{
			return INVALID_CONNECTION_ID;
		}
		pkConnection->setAsClientSide();
		pkConnection->asyncConnect(kEndpoint,
			boost::bind(&TcpNetwork::onConnect, this, pkConnection,
			boost::asio::placeholders::error));
		return pkConnection->getID();
	}

	void TcpNetwork::onConnect(ConnectionPtr& pkConnection, boost::system::error_code& kErrorCode)
	{
		if (kErrorCode)
		{
			pkConnection->pushErrorCode(kErrorCode);
			boost::lock_guard<boost::mutex> kLockGuard(mkErrorConListMutex);
			mkErrorConList.push_back(pkConnection);
			return;
		}
		if (pkConnection->isSSLConnection())
		{
			/// 要在投递asyncHandShake前锁住mkHandshakingConMap，并添加pkConnection到其中
			/// 防止onHandshake的回调先于这一步骤执行而在mkHandshakingConMap找不到对应的pkConnection
			{
				boost::lock_guard<boost::mutex> kLockGuard(mkHandshakingConMapMutex);
				BOOST_ASSERT(mkHandshakingConMap.find(pkConnection->getID()) == mkHandshakingConMap.end());
				mkHandshakingConMap[pkConnection->getID()] = pkConnection;
			}
			pkConnection->asyncHandShake(boost::bind(&TcpNetwork::onHandshake, this, pkConnection,
				boost::asio::placeholders::error));
		}
		else
		{
			boost::lock_guard<boost::mutex> kLockGuard(mkWaitActiveConListMutex);
			mkWaitActiveConList.push_back(pkConnection);
		}
	}

	void TcpNetwork::onHandshake(ConnectionPtr& pkConnection,
		boost::system::error_code& kErrorCode)
	{
		/// 从处在握手流程状态的链接列表中移除
		{
			boost::lock_guard<boost::mutex> kLockGuard(mkHandshakingConMapMutex);
			ConnectionMap::iterator kIter = mkHandshakingConMap.find(pkConnection->getID());
			BOOST_ASSERT(kIter != mkHandshakingConMap.end());
			mkHandshakingConMap.erase(kIter);
		}
		if (kErrorCode)
		{
			pkConnection->asyncClose();
			pkConnection->pushErrorCode(kErrorCode);
			boost::lock_guard<boost::mutex> kLockGuard(mkErrorConListMutex);
			mkErrorConList.push_back(pkConnection);
		}
		else
		{
			/// 添加到等待激活链接列表中
			boost::lock_guard<boost::mutex> kLockGuard(mkWaitActiveConListMutex);
			mkWaitActiveConList.push_back(pkConnection);
		}
	}

	bool TcpNetwork::startAccept()
	{
		if (!mbStartup)
		{
			return false;
		}
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

		for (unsigned int ui = 0; ui < mkNetworkParams.uiListenConnectionNum; ui++)
		{
			ConnectionPtr pkConnection = mpkIdealConContainer->get(CT_DEFAULT);
			pkConnection->setAsServerSide();
			mkAcceptor.async_accept(pkConnection->getSocket(),
				boost::bind(&TcpNetwork::onAccept, this, pkConnection,
				boost::asio::placeholders::error));
		}
		return true;
	}

	void TcpNetwork::onAccept(ConnectionPtr& pkConnection,
		const boost::system::error_code& kError)
	{
		if (kError)
		{
			pkConnection->pushErrorCode(kError);
			boost::lock_guard<boost::mutex> kLockGuard(mkErrorConListMutex);
			mkErrorConList.push_back(pkConnection);
		}
		else
		{
			if (pkConnection->isSSLConnection())
			{
				/// 要在投递asyncHandShake前锁住mkHandshakingConMap，并添加pkConnection到其中
				/// 防止onHandshake的回调先于这一步骤执行而在mkHandshakingConMap找不到对应的pkConnection
				{
					boost::lock_guard<boost::mutex> kLockGuard(mkHandshakingConMapMutex);
					BOOST_ASSERT(mkHandshakingConMap.find(pkConnection->getID()) == mkHandshakingConMap.end());
					mkHandshakingConMap[pkConnection->getID()] = pkConnection;
				}
				pkConnection->asyncHandShake(boost::bind(&TcpNetwork::onHandshake, this, pkConnection,
					boost::asio::placeholders::error));
			}
			else
			{
				boost::lock_guard<boost::mutex> kLockGuard(mkWaitActiveConListMutex);
				mkWaitActiveConList.push_back(pkConnection);
			}
		}
		if (mbStartup)
		{
			ConnectionPtr pkNewConnection = mpkIdealConContainer->get(CT_DEFAULT);
			pkNewConnection->setAsServerSide();
			pkNewConnection->clearErrorCodes();
			mkAcceptor.async_accept(pkNewConnection->getSocket(),
				boost::bind(&TcpNetwork::onAccept, this, pkNewConnection,
				boost::asio::placeholders::error));
		}
	}
	/// 为了在主线执行OnConFunc回调，所以在onAccept将所有链接放入mkWaitActiveConList
	void TcpNetwork::handleWaitForActivingConnections()
	{
		boost::lock_guard<boost::mutex> kLockGuard(mkWaitActiveConListMutex);
		for (ConnectionList::iterator kIter = mkWaitActiveConList.begin();
			kIter != mkWaitActiveConList.end(); ++kIter)
		{
			ConnectionPtr pkConnection = *kIter;
			if (pkConnection->open())
			{
				mpkActiveConContainer->put(pkConnection->getID(), pkConnection);
				if (mpOnConFunc)
				{
					mpOnConFunc(pkConnection->getID());
				}
			}
			else
			{
				boost::lock_guard<boost::mutex> kLockGuard(mkErrorConListMutex);
				mkErrorConList.push_back(pkConnection);
			}
		}
		mkWaitActiveConList.clear();
	}

	void TcpNetwork::handleAllActivedConnections()
	{
		boost::recursive_mutex::scoped_lock kScopedLock(mpkActiveConContainer->mkRecursiveMutex);
		std::map<ConnectionID, ConnectionPtr>::iterator kIter = 
			mpkActiveConContainer->mkConnectionMap.begin();
		while (kIter != mpkActiveConContainer->mkConnectionMap.end())
		{
			visitAciveConnection(kIter->first, kIter->second);
			if (kIter->second->isClosed())
			{
				std::map<ConnectionID, ConnectionPtr>::iterator kTmpIter = kIter++;
				if (kTmpIter->second->mbNeedCallCloseCallback)
				{
					checkAndHandleConnectionErrors(kTmpIter->second);
					if (mpOnDisConFunc)
					{
						mpOnDisConFunc(kTmpIter->second->getID());
					}
				}
				mpkIdealConContainer->put(kTmpIter->second);
				mpkActiveConContainer->mkConnectionMap.erase(kTmpIter);
				continue;
			}
			if (kIter->second->canBeClosed())
			{
				kIter->second->close();
				++kIter;
				continue;
			}
			kIter->second->trySendExtraSendBuff();
			++kIter;
		}
	}

	void TcpNetwork::handleErrorConnections()
	{
		boost::lock_guard<boost::mutex> kLockGuard(mkErrorConListMutex);
		ConnectionList::iterator kIter = mkErrorConList.begin();
		while (kIter != mkErrorConList.end())
		{
			ConnectionPtr pkConnection = *kIter;
			checkAndHandleConnectionErrors(pkConnection);
			if (pkConnection->isClosed())
			{
				mpkIdealConContainer->put(pkConnection);
				kIter = mkErrorConList.erase(kIter);
				continue;
			}
			pkConnection->close();
			++kIter;
		}
	}

	void TcpNetwork::checkAndHandleConnectionErrors(ConnectionPtr& pkConnection)
	{
		if (!pkConnection)
		{
			return;
		}

		Connection::ErrorCodeVector kErrorCodeVec = pkConnection->getErrorCodes();
		if (kErrorCodeVec.size() > 0)
		{
			for (unsigned int ui = 0; ui < kErrorCodeVec.size(); ++ui)
			{
				if (mpOnErrorHandleFunc)
				{
					mpOnErrorHandleFunc(pkConnection->getID(), kErrorCodeVec[ui]);
				}
			}
			pkConnection->clearErrorCodes();
		}
	}

	void TcpNetwork::visitAciveConnection(ConnectionID uID, ConnectionPtr& pkConnection)
	{
		if (!pkConnection)
		{
			return;
		}
		if (mpOnPacketFunc)
		{
			unsigned char* pacBuffer = 0;
			unsigned int uiReadableSize = 0;
			unsigned int uiProcessedSize = 0;
			bool bContinueProcess = true;
			if (pkConnection->readPacketsBegin(pacBuffer, uiReadableSize))
			{
				if (uiReadableSize > 0)
				{
					int iPacketProcessNumLimit = mkNetworkParams.iPacketProcessNumLimit;
					while (bContinueProcess &&
                        (uiReadableSize > uiProcessedSize) &&
                        ((uiReadableSize - uiProcessedSize) > sizeof(unsigned int)) &&
						iPacketProcessNumLimit != 0)
					{
						if (iPacketProcessNumLimit > 0)
						{
							--iPacketProcessNumLimit;
						}
						unsigned char* pcPacketAddress = pacBuffer + uiProcessedSize;
						unsigned int uiPacketSize = *((unsigned int*)pcPacketAddress);
						if (uiPacketSize == 0 || uiPacketSize > mkNetworkParams.uiMaxConnectionWriteSize)
						{
							pkConnection->asyncClose();
							boost::system::error_code kErrorCode = Network::error_packet_size;
							pkConnection->pushErrorCode(kErrorCode);
							break;
						}
						else if (uiPacketSize > uiReadableSize - uiProcessedSize)
						{
							break;
						}
						uiProcessedSize += uiPacketSize;
						bContinueProcess = mpOnPacketFunc(uID, pcPacketAddress, uiPacketSize);
					}
				}
				else
				{
					unsigned int uiPacketSize = 0;
					bool bPacketSizeValid = pkConnection->currentReceivedPacketSize(uiPacketSize);
					if (bPacketSizeValid && (uiPacketSize == 0 || uiPacketSize > mkNetworkParams.uiMaxConnectionWriteSize))
					{
						pkConnection->asyncClose();
						boost::system::error_code kErrorCode = Network::error_packet_size;
						pkConnection->pushErrorCode(kErrorCode);
					}
				}
				pkConnection->readPacketsEnd(uiProcessedSize);
			}
		}
		else if (mpOnRecvFunc)
		{
			unsigned int uiReadSize = pkConnection->read(mpcRecvTmpBuffer, mkNetworkParams.uiRecvBuffSize);
			if (uiReadSize > 0)
			{
				mpOnRecvFunc(uID, mpcRecvTmpBuffer, uiReadSize);
			}
		}
		else
		{
			/// 将包读出来，不做任何处理
			pkConnection->read(mpcRecvTmpBuffer, mkNetworkParams.uiRecvBuffSize);
		}
	}
	bool TcpNetwork::closeConnection(ConnectionID uConID)
	{
		if (!mpkActiveConContainer)
		{
			return false;
		}
		ConnectionPtr pkConnection = mpkActiveConContainer->get(uConID);
		if (!pkConnection)
		{
			return false;
		}
		pkConnection->close();
		while (pkConnection)
		{
			visitAciveConnection(pkConnection->getID(), pkConnection);
			if (pkConnection->tryClose())
			{
				/// 关闭的时候检查是否有错误（因为所有错误都会引起连接关闭）
				pkConnection->mbNeedCallCloseCallback = false;
				checkAndHandleConnectionErrors(pkConnection);
				if (mpOnDisConFunc)
				{
					mpOnDisConFunc(pkConnection->getID());
				}
				pkConnection.reset();
			}
		}
		return true;
	}
	bool TcpNetwork::asyncCloseConnection(ConnectionID uConID)
	{
		if (!mpkActiveConContainer)
		{
			return false;
		}
		ConnectionPtr pkConnection = mpkActiveConContainer->get(uConID);
		if (!pkConnection)
		{
			return false;
		}
		pkConnection->asyncClose();
		return true;
	}
	bool TcpNetwork::send(ConnectionID uConID, unsigned char* acData, unsigned int uSize)
	{
		if (!mbStartup)
		{
			return false;
		}
		ConnectionPtr pkConnection = mpkActiveConContainer->get(uConID);
		if (!pkConnection)
		{
			return false;
		}
		if (pkConnection->write(acData, uSize))
		{
			if (mpOnSentFunc)
			{
				mpOnSentFunc(uConID, uSize);
			}
			return true;
		}
		return false;
	}
	bool TcpNetwork::isActived(ConnectionID uID)
	{
		if (mpkActiveConContainer && mpkActiveConContainer->get(uID))
		{
			return true;
		}
		return false;
	}
	std::string TcpNetwork::getConnectionRemoteIP(ConnectionID uID)
	{
		std::string strResult("");
		if (!mbStartup)
		{
			return strResult;
		}
		ConnectionPtr pkConnection = mpkActiveConContainer->get(uID);
		if (!pkConnection || pkConnection->isClosed() || pkConnection->shouldBeClosed())
		{
			return strResult;
		}
		try
		{
			strResult = pkConnection->getRemoteIP();
		}
		catch (...)
		{
			return strResult;
		}
		return strResult;
	}
	unsigned long TcpNetwork::getConnectionRemoteAddress(ConnectionID uID)
	{
		if (!mbStartup)
		{
			return 0;
		}
		unsigned long ulAddress = 0;
		ConnectionPtr pkConnection = mpkActiveConContainer->get(uID);
		if (!pkConnection || pkConnection->isClosed() || pkConnection->shouldBeClosed())
		{
			return ulAddress;
		}
		try
		{
			ulAddress = pkConnection->getRemoteAddress();
		}
		catch (...)
		{
			ulAddress = 0;
		}
		return ulAddress;
	}
	std::string TcpNetwork::getConnectionLocalIP(ConnectionID uID)
	{
		std::string strResult("");
		if (!mbStartup)
		{
			return strResult;
		}
		ConnectionPtr pkConnection = mpkActiveConContainer->get(uID);
		if (!pkConnection || pkConnection->isClosed() || pkConnection->shouldBeClosed())
		{
			return strResult;
		}
		try
		{
			strResult = pkConnection->getLocalIP();
		}
		catch (...)
		{
			return strResult;
		}
		return strResult;
	}
	unsigned long TcpNetwork::getConnectionLocalAddress(ConnectionID uID)
	{
		if (!mbStartup)
		{
			return 0;
		}
		unsigned long ulAddress = 0;
		ConnectionPtr pkConnection = mpkActiveConContainer->get(uID);
		if (!pkConnection || pkConnection->isClosed() || pkConnection->shouldBeClosed())
		{
			return ulAddress;
		}
		try
		{
			ulAddress = pkConnection->getLocalAddress();
		}
		catch (...)
		{
			ulAddress = 0;
		}
		return ulAddress;
	}
}