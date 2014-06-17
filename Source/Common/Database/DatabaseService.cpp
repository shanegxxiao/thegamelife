#include "DatabasePCH.h"
#include "DatabaseService.h"

namespace Database
{
	RUNTIME_CREATE_SERVICE(DatabaseService, Runtime::Object, 0)
	DatabaseService::DatabaseService()
	{

	}
	DatabaseService::~DatabaseService()
	{

	}

	bool DatabaseService::Initialize(Runtime::ParamList& kParamList)
	{
		/// initialize the database API environment
		otl_connect::otl_initialize(1);
		return true;
	}
	bool DatabaseService::Shutdown()
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
	void DatabaseService::Running()
	{
		for (unsigned int ui = 0; ui < mkConnectionList.size(); ++ui)
		{
			mkConnectionList[ui]->mainThreadRun();
		}
	}

	Connection* DatabaseService::Connect(const std::string& strConStr)
	{
		Connection* pkCon = new Connection;
		if (!pkCon->open(strConStr))
		{
			delete pkCon;
			return 0;
		}
		mkConnectionList.push_back(pkCon);
		return pkCon;
	}
	bool DatabaseService::CloseConnection(Connection* pkConnection)
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