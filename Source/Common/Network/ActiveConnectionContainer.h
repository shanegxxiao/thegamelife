#ifndef __ActiveConnectionContainer_h__
#define __ActiveConnectionContainer_h__

#include "TcpConnection.h"
#include "SSLConnection.h"
#include "ConnectionIDGenerator.h"
namespace Network
{
	class ActiveConnectionContainer
	{
	public:
		typedef boost::function<void(ConnectionID, ConnectionPtr)> VisitFunc;

	public:
		ActiveConnectionContainer();
		~ActiveConnectionContainer();

		void put(ConnectionID ullID, ConnectionPtr& pkConnection);
		ConnectionPtr get(ConnectionID ullID);
		ConnectionPtr erase(ConnectionID ullID);
		unsigned int size();

	private:
		friend class TcpNetwork;

	private:
		boost::recursive_mutex mkRecursiveMutex;
		std::map<ConnectionID, ConnectionPtr> mkConnectionMap;
	};
}

#endif//#ifndef __ActiveConnectionContainer_h__