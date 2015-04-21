#include "DatabasePCH.h"
#include "OtlConnection.h"
#include "OtlDirectExecuteTask.h"

namespace Database
{
	OtlDirectExecuteTask::OtlDirectExecuteTask()
		: mulAffectRows(0)
	{

	}
	OtlDirectExecuteTask::~OtlDirectExecuteTask()
	{

	}
	void OtlDirectExecuteTask::onExecute(OtlConnection* pkConnection)
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
	void OtlDirectExecuteTask::onFinish()
	{

	}
}