#ifndef __DatabaseConnection_h__
#define __DatabaseConnection_h__

namespace Database
{
	class Task;

	class Connection
	{
	private:
		friend class DatabaseService;
#ifdef WIN32
		typedef stdext::hash_multimap<int, Task*> PriorityTaskQueue;
#else
		typedef __gnu_cxx::hash_multimap<int, Task*> PriorityTaskQueue;
#endif//WIN32

	public:
		void addTask(Task* pkTask);
		bool cancelTask(Task *pkTask);
		otl_connect* getOtlConnection();

	private:
		Connection();
		virtual ~Connection();
		bool open(const std::string& strConStr);
		void mainThreadRun();
		bool close();

		void pushWorkThreadExecutTask(Task *pkTask);
		void pushMainThreadExecuteTask(Task *pkTask);
		void pushWorkThreadFinishTask(Task *pkTask);
		void pushMainThreadFinishTask(Task *pkTask);
		void pushWorkThreadDeleteTask(Task *pkTask);
		void pushMainThreadDeleteTask(Task *pkTask);
		Task* popWorkThreadExecuteTask();
		Task* popMainThreadExecuteTask();
		Task* popWorkThreadFinishTask();
		Task* popMainThreadFinishTask();
		Task* popWorkThreadDeleteTask();
		Task* popMainThreadDeleteTask();

		unsigned int getTaskCount(const PriorityTaskQueue &kQueue,
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

	inline otl_connect* Connection::getOtlConnection()
	{
		return &mOtlDBConnection;
	}
	inline unsigned int Connection::getTaskCount(
		const PriorityTaskQueue &kQueue,
		boost::mutex &kMutex)
	{
		boost::lock_guard<boost::mutex> kLockGuard(kMutex);
		return kQueue.size();
	}
}

#endif//__DatabaseConnection_h__
