#include "NetworkPCH.h"
#include "ActiveConnectionContainer.h"
namespace Network
{
	ActiveConnectionContainer::ActiveConnectionContainer()
	{
	}
	ActiveConnectionContainer::~ActiveConnectionContainer()
	{
		boost::recursive_mutex::scoped_lock kScopedLock(mkRecursiveMutex);
		for (std::map<ConnectionID, ConnectionPtr>::iterator kIter = mkConnectionMap.begin();
			kIter != mkConnectionMap.end(); ++kIter)
		{
			kIter->second->close();
		}
		mkConnectionMap.clear();
	}
	void ActiveConnectionContainer::put(ConnectionID ullID, ConnectionPtr& pkConnection)
	{
		boost::recursive_mutex::scoped_lock kScopedLock(mkRecursiveMutex);
		mkConnectionMap[ullID] = pkConnection;
	}
	ConnectionPtr ActiveConnectionContainer::get(ConnectionID ullID)
	{
		ConnectionPtr pkConnection;
		boost::recursive_mutex::scoped_lock kScopedLock(mkRecursiveMutex);
		std::map<ConnectionID, ConnectionPtr>::iterator kIter = mkConnectionMap.find(ullID);
		if (kIter == mkConnectionMap.end())
		{
			return pkConnection;
		}
		return kIter->second;
	}
	ConnectionPtr ActiveConnectionContainer::erase(ConnectionID ullID)
	{
		boost::recursive_mutex::scoped_lock kScopedLock(mkRecursiveMutex);
		std::map<ConnectionID, ConnectionPtr>::iterator kIter = mkConnectionMap.find(ullID);
		if (kIter == mkConnectionMap.end())
		{
			ConnectionPtr spConnection;
			return spConnection;
		}
		ConnectionPtr pkConnection = kIter->second;
		mkConnectionMap.erase(kIter);
		return pkConnection;
	}
	unsigned int ActiveConnectionContainer::size()
	{
		boost::recursive_mutex::scoped_lock kScopedLock(mkRecursiveMutex);
		return mkConnectionMap.size();
	}
}