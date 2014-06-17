#include "NetworkPCH.h"
#include "IdealConnectionContainer.h"
#include "TcpConnection.h"
#include "SSLConnection.h"

namespace Network
{
	IdealConnectionContainer::IdealConnectionContainer(
		boost::asio::io_service &kIOService, TcpNetworkParams* pkParams,
		boost::asio::ssl::context* pkSSLContext)
		: mkIOService(kIOService),
		  mpkTcpNetworkParams(pkParams),
		  mpkSSLContext(pkSSLContext)
	{
		for (unsigned int ui = 0; ui < mpkTcpNetworkParams->uiPreCreateConCnt; ++ui)
		{
			put(newConnection(CT_DEFAULT));
		}
	}
	IdealConnectionContainer::~IdealConnectionContainer()
	{
		ConnectionPtr spConnection;
		spConnection = mpkTCPConListHead;
		while (spConnection)
		{
			mpkTCPConListHead = spConnection->mpkNext;
			spConnection->mpkPrev.reset();
			spConnection->mpkNext.reset();
			spConnection = mpkTCPConListHead;
		}
		spConnection = mpkSSLConListHead;
		while (spConnection)
		{
			mpkSSLConListHead = spConnection->mpkNext;
			spConnection->mpkPrev.reset();
			spConnection->mpkNext.reset();
			spConnection = mpkSSLConListHead;
		}
		boost::lock_guard<boost::mutex> kLockGuard(mkConnectionsMainListMutex);
		for (ConnectionList::iterator kIter = mkConnectionsMainList.begin();
			kIter != mkConnectionsMainList.end(); ++kIter)
		{
			kIter->second.reset();
		}
		mkConnectionsMainList.clear();
	}
	ConnectionPtr IdealConnectionContainer::newConnection(const ConnectionType& eType)
	{
		ConnectionPtr pkConnection;
		switch (eType)
		{
		case CT_SSL:
			pkConnection.reset(new SSLConnection(
				mkIOService,
				mpkTcpNetworkParams,
				mpkSSLContext));
			break;
		case CT_TCP:
			pkConnection.reset(new TcpConnection(
				mkIOService,
				mpkTcpNetworkParams));
			break;
		case CT_DEFAULT:
		default:
			if (mpkTcpNetworkParams->bUseSSL)
			{
				pkConnection.reset(new SSLConnection(
					mkIOService,
					mpkTcpNetworkParams,
					mpkSSLContext));
			}
			else
			{
				pkConnection.reset(new TcpConnection(
					mkIOService,
					mpkTcpNetworkParams));
			}
			break;
		}
		boost::lock_guard<boost::mutex> kLockGuard(mkConnectionsMainListMutex);
		mkConnectionsMainList.insert(ConnectionList::value_type(pkConnection.get(), pkConnection));

		return pkConnection;
	}
	bool IdealConnectionContainer::deleteConnection(ConnectionPtr& pkConnection)
	{
		boost::lock_guard<boost::mutex> kLockGuard(mkConnectionsMainListMutex);
		if (mkConnectionsMainList.size() <= mpkTcpNetworkParams->uiMaxPoolConCnt)
		{
			return false;
		}
		ConnectionList::iterator kIter = mkConnectionsMainList.find(pkConnection.get());
		BOOST_ASSERT(kIter != mkConnectionsMainList.end());
		if (kIter != mkConnectionsMainList.end())
		{
			kIter->second.reset();
			mkConnectionsMainList.erase(kIter);
		}
		return true;
	}
	ConnectionPtr IdealConnectionContainer::get(const ConnectionType& eType)
	{
		ConnectionPtr pkConnection;
		switch (eType)
		{
		case CT_SSL:
			pkConnection = getSSLConnection();
			break;
		case CT_TCP:
			pkConnection = getTCPConnection();
			break;
		case CT_DEFAULT:
		default:
			if (mpkTcpNetworkParams->bUseSSL)
			{
				pkConnection = getSSLConnection();
			}
			else
			{
				pkConnection = getTCPConnection();
			}
		}
		return pkConnection;
	}
	ConnectionPtr IdealConnectionContainer::getTCPConnection()
	{
		boost::lock_guard<boost::mutex> kLockGuard(mkTCPConListMutex);
		if (mpkTCPConListHead)
		{
			ConnectionPtr pkTmp = mpkTCPConListHead;
			mpkTCPConListHead = pkTmp->mpkNext;
			if (mpkTCPConListHead)
			{
				mpkTCPConListHead->mpkPrev.reset();
			}
			pkTmp->mpkNext.reset();
			pkTmp->assignID();
			return pkTmp;
		}
		ConnectionPtr pkConnection = newConnection(CT_TCP);
		pkConnection->assignID();
		return pkConnection;
	}
	ConnectionPtr IdealConnectionContainer::getSSLConnection()
	{
		boost::lock_guard<boost::mutex> kLockGuard(mkSSLConListMutex);
		if (mpkSSLConListHead)
		{
			ConnectionPtr pkTmp = mpkSSLConListHead;
			mpkSSLConListHead = pkTmp->mpkNext;
			if (mpkSSLConListHead)
			{
				mpkSSLConListHead->mpkPrev.reset();
			}
			pkTmp->mpkNext.reset();
			pkTmp->assignID();
			return pkTmp;
		}
		ConnectionPtr pkConnection = newConnection(CT_SSL);
		pkConnection->assignID();
		return pkConnection;
	}
	void IdealConnectionContainer::put(ConnectionPtr& pkConnection)
	{
		if (deleteConnection(pkConnection))
		{
			return;
		}
		pkConnection->reset();
		if (pkConnection->isSSLConnection())
		{
			putSSLConnection(pkConnection);
		}
		else
		{
			putTCPConnection(pkConnection);
		}
	}
	void IdealConnectionContainer::putTCPConnection(ConnectionPtr& pkConnection)
	{
		boost::lock_guard<boost::mutex> kLockGuard(mkTCPConListMutex);
		if (!mpkTCPConListHead)
		{
			mpkTCPConListHead = pkConnection;
			mpkTCPConListHead->mpkPrev.reset();
			mpkTCPConListHead->mpkNext.reset();
		}
		else
		{
			pkConnection->mpkPrev.reset();
			pkConnection->mpkNext = mpkTCPConListHead;
			mpkTCPConListHead->mpkPrev = pkConnection;
			mpkTCPConListHead = pkConnection;
		}
	}
	void IdealConnectionContainer::putSSLConnection(ConnectionPtr& pkConnection)
	{
		boost::lock_guard<boost::mutex> kLockGuard(mkSSLConListMutex);
		if (!mpkSSLConListHead)
		{
			mpkSSLConListHead = pkConnection;
			mpkSSLConListHead->mpkPrev.reset();
			mpkSSLConListHead->mpkNext.reset();
		}
		else
		{
			pkConnection->mpkPrev.reset();
			pkConnection->mpkNext = mpkSSLConListHead;
			mpkSSLConListHead->mpkPrev = pkConnection;
			mpkSSLConListHead = pkConnection;
		}
	}
}