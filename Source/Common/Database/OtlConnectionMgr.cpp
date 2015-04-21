#include "DatabasePCH.h"
#include "OtlConnectionMgr.h"

namespace Database
{
	OtlConnectionMgr::OtlConnectionMgr()
	{

	}
	OtlConnectionMgr::~OtlConnectionMgr()
	{

	}

	bool OtlConnectionMgr::Initialize()
	{
		/// initialize the database API environment
		otl_connect::otl_initialize(1);
		return true;
	}
	bool OtlConnectionMgr::Shutdown()
	{
	#if (defined OTL_ORA7 || \
		defined OTL_ORA8 || \
		defined OTL_ORA8I || \
		defined OTL_ORA9I || \
		defined OTL_ORA10G || \
		defined OTL_ORA11G_R2)
		otl_connect::otl_terminate();
	#endif
		return true;
	}
	void OtlConnectionMgr::Running()
	{
		for (unsigned int ui = 0; ui < mkConnectionList.size(); ++ui)
		{
			mkConnectionList[ui]->mainThreadRun();
		}
	}

	OtlConnection* OtlConnectionMgr::Connect(const std::string& strConStr)
	{
		OtlConnection* pkCon = new OtlConnection;
		if (!pkCon->open(strConStr))
		{
			delete pkCon;
			return 0;
		}
		mkConnectionList.push_back(pkCon);
		return pkCon;
	}
	bool OtlConnectionMgr::CloseConnection(OtlConnection* pkConnection)
	{
		for (ConnectionList::iterator kIter = mkConnectionList.begin();
			kIter != mkConnectionList.end(); ++kIter)
		{
			if (*kIter == pkConnection)
			{
				pkConnection->close();
				mkConnectionList.erase(kIter);
				delete pkConnection;
				return true;
			}
		}
		return false;
	}
}