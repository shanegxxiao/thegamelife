#ifndef __OtlConnection_h__
#define __OtlConnection_h__

namespace Database
{
	class OtlTask;
    #undef close

	class OtlConnection
	{
	private:
		friend class OtlConnectionMgr;
#ifdef WIN32
		typedef stdext::hash_multimap<int, OtlTask*> PriorityTaskQueue;
#else
		typedef __gnu_cxx::hash_multimap<int, OtlTask*> PriorityTaskQueue;
#endif//WIN32

	public:
		void addTask(OtlTask* pkTask);
		bool cancelTask(OtlTask *pkTask);
		otl_connect* getOtlConnection();

	private:
		OtlConnection();
		virtual ~OtlConnection();
		bool open(const std::string& strConStr);
		void mainThreadRun();
		bool close();

		void pushWorkThreadExecutTask(OtlTask *pkTask);
		void pushMainThreadExecuteTask(OtlTask *pkTask);
		void pushWorkThreadFinishTask(OtlTask *pkTask);
		void pushMainThreadFinishTask(OtlTask *pkTask);
		void pushWorkThreadDeleteTask(OtlTask *pkTask);
		void pushMainThreadDeleteTask(OtlTask *pkTask);
		OtlTask* popWorkThreadExecuteTask();
		OtlTask* popMainThreadExecuteTask();
		OtlTask* popWorkThreadFinishTask();
		OtlTask* popMainThreadFinishTask();
		OtlTask* popWorkThreadDeleteTask();
		OtlTask* popMainThreadDeleteTask();

		size_t getTaskCount(const PriorityTaskQueue &kQueue,
			boost::mutex &kMutex);
		
		void workThreadRun();

	private:
		boost::thread_group mkThreadGroup;

		boost::mutex mkWorkThreadExecuteTasksQueueMutex;
		PriorityTaskQueue mkWorkThreadExecuteTasksQueue;
		boost::mutex mkWorkThreadFinishTasksQueueMutex;
		PriorityTaskQueue mkWorkThreadFinishTasksQueue;
		boost::mutex mkWorkThreadDeleteTasksQueueMutex;
		PriorityTaskQueue mkWorkThreadDeleteTasksQueue;
		
		boost::mutex mkMainThreadExecuteTasksQueueMutex;
		PriorityTaskQueue mkMainThreadExecuteTasksQueue;
		boost::mutex mkMainThreadFinishTasksQueueMutex;
		PriorityTaskQueue mkMainThreadFinishTasksQueue;
		boost::mutex mkMainThreadDeleteTasksQueueMutex;
		PriorityTaskQueue mkMainThreadDeleteTasksQueue;

		otl_connect mOtlDBConnection;
		std::string mstrConStr;
		bool mbWorkThreadKeepRunning;
		bool mbWorkThreadExisted;
	};

	inline otl_connect* OtlConnection::getOtlConnection()
	{
		return &mOtlDBConnection;
	}
	inline size_t OtlConnection::getTaskCount(
		const PriorityTaskQueue &kQueue,
		boost::mutex &kMutex)
	{
		boost::lock_guard<boost::mutex> kLockGuard(kMutex);
		return kQueue.size();
	}
}

#endif//__DatabaseConnection_h__
