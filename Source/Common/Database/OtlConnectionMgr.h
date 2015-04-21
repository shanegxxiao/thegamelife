#ifndef __OtlService_h__
#define __OtlService_h__

#include "OtlConnection.h"

namespace Database
{
	class OtlConnectionMgr
	{
	private:
		typedef std::vector<OtlConnection*> ConnectionList;

	public:
		OtlConnectionMgr();
		~OtlConnectionMgr();

        bool Initialize();
        void Running();
        bool Shutdown();

		OtlConnection* Connect(const std::string& strConStr);
		bool CloseConnection(OtlConnection* pkConnection);

	private:
		ConnectionList mkConnectionList;
	};
}

#endif//__DatabaseMgr_h__