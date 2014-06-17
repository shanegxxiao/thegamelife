#include "DatabasePCH.h"
#include "Connection.h"
#include "DirectExecuteTask.h"

namespace Database
{
	DirectExecuteTask::DirectExecuteTask()
		: mulAffectRows(0)
	{

	}
	DirectExecuteTask::~DirectExecuteTask()
	{

	}
	void DirectExecuteTask::onExecute(Connection* pkConnection)
	{
		try
		{
			if (!pkConnection)
			{
				return;
			}
			otl_connect *pkConnect = pkConnection->getOtlConnection();
			if (!pkConnect)
			{
				return;
			}
			mulAffectRows = pkConnect->direct_exec(mStrSQL.c_str());
		}
		catch (otl_exception&)
		{
			
		}		
	}
	void DirectExecuteTask::onFinish()
	{

	}
}