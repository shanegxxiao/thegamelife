#ifndef __IdealConnectionContainer_h__
#define __IdealConnectionContainer_h__
#include "TcpNetworkParams.h"
#include "Connection.h"

namespace Network
{
	class IdealConnectionContainer
	{
		typedef std::map<Connection*, ConnectionPtr> ConnectionList;

	public:
		IdealConnectionContainer(boost::asio::io_service &kIOService, 
			TcpNetworkParams* pkParams, boost::asio::ssl::context* pkSSLContext);
		~IdealConnectionContainer();

		ConnectionPtr get(const ConnectionType& eType);
		void put(ConnectionPtr& pkConnection);

	private:
		ConnectionPtr newConnection(const ConnectionType& eType);
		bool deleteConnection(ConnectionPtr& pkConnection);
		ConnectionPtr getTCPConnection();
		ConnectionPtr getSSLConnection();
		void putTCPConnection(ConnectionPtr& pkConnection);
		void putSSLConnection(ConnectionPtr& pkConnection);

	private:
		boost::mutex mkConnectionsMainListMutex;
		ConnectionList mkConnectionsMainList;

		boost::mutex mkTCPConListMutex;
		ConnectionPtr mpkTCPConListHead;

		boost::mutex mkSSLConListMutex;
		ConnectionPtr mpkSSLConListHead;

		boost::asio::io_service &mkIOService;
		TcpNetworkParams* mpkTcpNetworkParams;
		boost::asio::ssl::context* mpkSSLContext;
	};
}

#endif//#ifndef __IdealConnectionContainer_h__