#include "DatabasePCH.h"
#include "Connection.h"
#include "Task.h"
namespace Database
{
	Connection::Connection()
		: mbWorkThreadKeepRunning(true),
		  mbWorkThreadExisted(false)
	{
	}
	Connection::~Connection()
	{
	}
	bool Connection::open(const std::string& strConStr)
	{
		mbWorkThreadKeepRunning = true;
		mbWorkThreadExisted = false;
		mstrConStr = strConStr;
		try
		{
			/// connect to the database
			mOtlDBConnection.rlogon(mstrConStr.c_str());
		}
		catch (otl_exception&)
		{
			return false;
		}
		mkThreadGroup.create_thread(boost::bind(
			&Connection::workThreadRun,
			this));
		return true;
	}
	bool Connection::close()
	{
		mbWorkThreadKeepRunning = false;
		while (!mbWorkThreadExisted ||
			getTaskCount(mkMainThreadExecuteTasksQueue, 
			mkMainThreadExecuteTasksQueueMutex) ||
			getTaskCount(mkMainThreadFinishTasksQueue, 
			mkMainThreadFinishTasksQueueMutex) ||
			getTaskCount(mkMainThreadDeleteTasksQueue, 
			mkMainThreadDeleteTasksQueueMutex) ||
			getTaskCount(mkWorkThreadExecuteTasksQueue, 
			mkWorkThreadExecuteTasksQueueMutex) ||
			getTaskCount(mkWorkThreadFinishTasksQueue, 
			mkWorkThreadFinishTasksQueueMutex) ||
			getTaskCount(mkWorkThreadDeleteTasksQueue, 
			mkWorkThreadDeleteTasksQueueMutex))
		{
			mainThreadRun();
		}
		
		mkThreadGroup.join_all();
		mOtlDBConnection.logoff();
		return true;
	}
	void Connection::mainThreadRun()
	{
		Task *pkTask = popMainThreadExecuteTask();
		if (pkTask)
		{
			pkTask->execute(this);
			if (pkTask->getFinishedOnWorkThread())
			{
				pushWorkThreadFinishTask(pkTask);
			}
			else
			{
				pushMainThreadFinishTask(pkTask);
			}
		}

		pkTask = popMainThreadFinishTask();
		if (pkTask)
		{
			pkTask->finish();
			if (pkTask->getAutoDelete())
			{
				if (pkTask->getDeleteOnWorkThread())
				{
					pushWorkThreadDeleteTask(pkTask);
				}
				else
				{
					pushMainThreadDeleteTask(pkTask);
				}
			}
		}

		pkTask = popMainThreadDeleteTask();
		if (pkTask)
		{
			delete pkTask;
		}
	}
	void Connection::workThreadRun()
	{
		while (mbWorkThreadKeepRunning ||
			getTaskCount(mkWorkThreadExecuteTasksQueue, 
			mkWorkThreadExecuteTasksQueueMutex) ||
			getTaskCount(mkWorkThreadFinishTasksQueue, 
			mkWorkThreadFinishTasksQueueMutex) ||
			getTaskCount(mkWorkThreadDeleteTasksQueue, 
			mkWorkThreadDeleteTasksQueueMutex))
		{
			Task *pkTask = popWorkThreadExecuteTask();
			if (pkTask)
			{
				pkTask->execute(this);
				if (pkTask->getFinishedOnWorkThread())
				{
					pushWorkThreadFinishTask(pkTask);
				}
				else
				{
					pushMainThreadFinishTask(pkTask);
				}
			}

			pkTask = popWorkThreadFinishTask();
			if (pkTask)
			{
				pkTask->finish();
				if (pkTask->getAutoDelete())
				{
					if (pkTask->getDeleteOnWorkThread())
					{
						pushWorkThreadDeleteTask(pkTask);
					}
					else
					{
						pushMainThreadDeleteTask(pkTask);
					}
				}
			}

			pkTask = popWorkThreadDeleteTask();
			if (pkTask)
			{
				delete pkTask;
			}
		}
		mbWorkThreadExisted = true;
	}
	void Connection::addTask(Task* pkTask)
	{
		if (!pkTask)
		{
			return;
		}
		if (pkTask->getExecuteOnWorkThread())
		{
			pushWorkThreadExecutTask(pkTask);
		}
		else
		{
			pushMainThreadExecuteTask(pkTask);
		}
	}
	bool Connection::cancelTask(Task *pkTask)
	{
		boost::lock_guard<boost::mutex> kLockGuardWork(mkWorkThreadExecuteTasksQueueMutex);
		for (PriorityTaskQueue::iterator kIter = mkWorkThreadExecuteTasksQueue.begin();
			kIter != mkWorkThreadExecuteTasksQueue.end(); ++kIter)
		{
			if (kIter->second == pkTask)
			{
				mkWorkThreadExecuteTasksQueue.erase(kIter);
				return true;
			}
		}

		boost::lock_guard<boost::mutex> kLockGuardMain(mkMainThreadExecuteTasksQueueMutex);
		for (PriorityTaskQueue::iterator kIter = mkMainThreadExecuteTasksQueue.begin();
			kIter != mkMainThreadExecuteTasksQueue.end(); ++kIter)
		{
			if (kIter->second == pkTask)
			{
				mkMainThreadExecuteTasksQueue.erase(kIter);
				return true;
			}
		}

		return false;
	}
	void Connection::pushWorkThreadExecutTask(Task *pkTask)
	{
		if (!pkTask)
		{
			return;
		}
		boost::lock_guard<boost::mutex> kLockGuard(mkWorkThreadExecuteTasksQueueMutex);
		mkWorkThreadExecuteTasksQueue.insert(PriorityTaskQueue::value_type(pkTask->getPriority(), pkTask));
	}
	void Connection::pushMainThreadExecuteTask(Task *pkTask)
	{
		if (!pkTask)
		{
			return;
		}
		boost::lock_guard<boost::mutex> kLockGuard(mkMainThreadExecuteTasksQueueMutex);
		mkMainThreadExecuteTasksQueue.insert(PriorityTaskQueue::value_type(pkTask->getPriority(), pkTask));
	}
	void Connection::pushWorkThreadFinishTask(Task *pkTask)
	{
		if (!pkTask)
		{
			return;
		}
		boost::lock_guard<boost::mutex> kLockGuard(mkWorkThreadFinishTasksQueueMutex);
		mkWorkThreadFinishTasksQueue.insert(PriorityTaskQueue::value_type(pkTask->getPriority(), pkTask));
	}
	void Connection::pushMainThreadFinishTask(Task *pkTask)
	{
		if (!pkTask)
		{
			return;
		}
		boost::lock_guard<boost::mutex> kLockGuard(mkMainThreadFinishTasksQueueMutex);
		mkMainThreadFinishTasksQueue.insert(PriorityTaskQueue::value_type(pkTask->getPriority(), pkTask));
	}
	void Connection::pushWorkThreadDeleteTask(Task *pkTask)
	{
		if (!pkTask)
		{
			return;
		}
		boost::lock_guard<boost::mutex> kLockGuard(mkWorkThreadDeleteTasksQueueMutex);
		mkWorkThreadDeleteTasksQueue.insert(PriorityTaskQueue::value_type(pkTask->getPriority(), pkTask));
	}
	void Connection::pushMainThreadDeleteTask(Task *pkTask)
	{
		if (!pkTask)
		{
			return;
		}
		boost::lock_guard<boost::mutex> kLockGuard(mkMainThreadDeleteTasksQueueMutex);
		mkMainThreadDeleteTasksQueue.insert(PriorityTaskQueue::value_type(pkTask->getPriority(), pkTask));
	}
	Task* Connection::popWorkThreadExecuteTask()
	{
		boost::lock_guard<boost::mutex> kLockGuard(mkWorkThreadExecuteTasksQueueMutex);
		PriorityTaskQueue::iterator kIter = mkWorkThreadExecuteTasksQueue.begin();
		if (kIter == mkWorkThreadExecuteTasksQueue.end())
		{
			return 0;
		}
		Task *pkTask = kIter->second;
		mkWorkThreadExecuteTasksQueue.erase(kIter);
		return pkTask;
	}
	Task* Connection::popMainThreadExecuteTask()
	{
		boost::lock_guard<boost::mutex> kLockGuard(mkMainThreadExecuteTasksQueueMutex);
		PriorityTaskQueue::iterator kIter = mkMainThreadExecuteTasksQueue.begin();
		if (kIter == mkMainThreadExecuteTasksQueue.end())
		{
			return 0;
		}
		Task *pkTask = kIter->second;
		mkMainThreadExecuteTasksQueue.erase(kIter);
		return pkTask;
	}
	Task* Connection::popWorkThreadFinishTask()
	{
		boost::lock_guard<boost::mutex> kLockGuard(mkWorkThreadFinishTasksQueueMutex);
		PriorityTaskQueue::iterator kIter = mkWorkThreadFinishTasksQueue.begin();
		if (kIter == mkWorkThreadFinishTasksQueue.end())
		{
			return 0;
		}
		Task *pkTask = kIter->second;
		mkWorkThreadFinishTasksQueue.erase(kIter);
		return pkTask;
	}
	Task* Connection::popMainThreadFinishTask()
	{
		boost::lock_guard<boost::mutex> kLockGuard(mkMainThreadFinishTasksQueueMutex);
		PriorityTaskQueue::iterator kIter = mkMainThreadFinishTasksQueue.begin();
		if (kIter == mkMainThreadFinishTasksQueue.end())
		{
			return 0;
		}
		Task *pkTask = kIter->second;
		mkMainThreadFinishTasksQueue.erase(kIter);
		return pkTask;
	}
	Task* Connection::popWorkThreadDeleteTask()
	{
		boost::lock_guard<boost::mutex> kLockGuard(mkWorkThreadDeleteTasksQueueMutex);
		PriorityTaskQueue::iterator kIter = mkWorkThreadDeleteTasksQueue.begin();
		if (kIter == mkWorkThreadDeleteTasksQueue.end())
		{
			return 0;
		}
		Task *pkTask = kIter->second;
		mkWorkThreadDeleteTasksQueue.erase(kIter);
		return pkTask;
	}
	Task* Connection::popMainThreadDeleteTask()
	{
		boost::lock_guard<boost::mutex> kLockGuard(mkMainThreadDeleteTasksQueueMutex);
		PriorityTaskQueue::iterator kIter = mkMainThreadDeleteTasksQueue.begin();
		if (kIter == mkMainThreadDeleteTasksQueue.end())
		{
			return 0;
		}
		Task *pkTask = kIter->second;
		mkMainThreadDeleteTasksQueue.erase(kIter);
		return pkTask;
	}
}