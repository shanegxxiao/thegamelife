#include "DatabasePCH.h"
#include "OtlTask.h"
#include "OtlConnection.h"

namespace Database
{
	OtlTask::OtlTask()
	: miPriority(1000),
	  mbExecuteOnWorkThread(true),
	  mbFinishOnWorkThread(true),
	  mbAutoDelete(true),
	  mbDeleteOnWorkThread(false),
	  meStatus(TS_PENDING)
	{
	}
	OtlTask::~OtlTask()
	{
	}
	void OtlTask::execute(OtlConnection* pkConnection)
	{
		if (!setStatus(TS_EXECUTING))
		{
			return;
		}
		onExecute(pkConnection);
		setStatus(TS_EXECUTTED);
	}
	void OtlTask::finish()
	{
		if (!setStatus(TS_FINISHING))
		{
			return;
		}
		onFinish();
		setStatus(TS_FINISHED);
	}
	bool OtlTask::reset()
	{
		if (getStatus() != TS_CANCELLED &&
			getStatus() != TS_FINISHED)
		{
			return false;
		}
		if (!onReset())
		{
			meStatus = TS_CANCELLED;
			return false;
		}
		meStatus = TS_PENDING;
		return true;
	}
}
