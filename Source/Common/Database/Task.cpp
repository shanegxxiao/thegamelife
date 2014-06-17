#include "DatabasePCH.h"
#include "Task.h"
#include "Connection.h"

namespace Database
{
	Task::Task()
	: miPriority(1000),
	  mbExecuteOnWorkThread(true),
	  mbFinishOnWorkThread(true),
	  mbAutoDelete(true),
	  mbDeleteOnWorkThread(false),
	  meStatus(TS_PENDING)
	{
	}
	Task::~Task()
	{
	}
	void Task::execute(Connection* pkConnection)
	{
		if (!setStatus(TS_EXECUTING))
		{
			return;
		}
		onExecute(pkConnection);
		setStatus(TS_EXECUTTED);
	}
	void Task::finish()
	{
		if (!setStatus(TS_FINISHING))
		{
			return;
		}
		onFinish();
		setStatus(TS_FINISHED);
	}
	bool Task::reset()
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
