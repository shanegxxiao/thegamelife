#ifndef __DatabaseTask_h__
#define __DatabaseTask_h__

namespace Database
{
	class Connection;

	class Task
	{
	public:
		enum Status
		{
			TS_PENDING,
			TS_EXECUTING,
			TS_EXECUTTED,
			TS_FINISHING,
			TS_FINISHED,
			TS_CANCELLED
		};
	public:
		Task();
		virtual ~Task();
		void setPriority(int iPriority);
		int getPriority();
		void setExecuteOnWorkThread(bool bOnWorkThread);
		bool getExecuteOnWorkThread();
		void setFinishedOnWorkThread(bool bOnWorkThread);
		bool getFinishedOnWorkThread();
		void setAutoDelete(bool bAutoDelete);
		bool getAutoDelete();
		void setDeleteOnWorkThread(bool bOnWorkThread);
		bool getDeleteOnWorkThread();
		void execute(Connection* pkConnection);
		void finish();
		/// 只有TS_CANCELLED和TS_FINISHED才能重置，否则返回false
		/// 具体的Task可能需要实现onReset以重置具体任务相关变量
		bool reset();
		/// 当状态进入TS_EXECUTING或者TS_FINISHING时不能取消任务
		/// cancel返回false，如果取消成功返回true，已取消的任务
		/// 不会执行onExecute或者onFinish
		bool cancel();
		bool isCancelled();
		Status getStatus();

	protected:
		virtual void onExecute(Connection* pkConnection) = 0;
		virtual void onFinish() = 0;
		virtual bool onReset();
		/// 当状态为TS_CANCELLED时返回false
		bool setStatus(Status eStatus);

	protected:
		Status meStatus;
		int miPriority;
		bool mbExecuteOnWorkThread;
		bool mbFinishOnWorkThread;
		bool mbAutoDelete;
		bool mbDeleteOnWorkThread;
	};

	inline void Task::setPriority(int iPriority)
	{
		miPriority = iPriority;
	}
	inline int Task::getPriority()
	{
		return miPriority;
	}
	inline void Task::setExecuteOnWorkThread(bool bOnWorkThread)
	{
		mbExecuteOnWorkThread = bOnWorkThread;
	}
	inline bool Task::getExecuteOnWorkThread()
	{
		return mbExecuteOnWorkThread;
	}
	inline void Task::setFinishedOnWorkThread(bool bOnWorkThread)
	{
		mbFinishOnWorkThread = bOnWorkThread;
	}
	inline bool Task::getFinishedOnWorkThread()
	{
		return mbFinishOnWorkThread;
	}
	inline void Task::setAutoDelete(bool bAutoDelete)
	{
		mbAutoDelete = bAutoDelete;
	}
	inline bool Task::getAutoDelete()
	{
		return mbAutoDelete;
	}
	inline void Task::setDeleteOnWorkThread(bool bOnWorkThread)
	{
		mbDeleteOnWorkThread = bOnWorkThread;
	}
	inline bool Task::getDeleteOnWorkThread()
	{
		return mbDeleteOnWorkThread;
	}
	inline bool Task::cancel()
	{
		if (meStatus == TS_EXECUTING || 
			meStatus == TS_FINISHING)
		{
			return false;
		}
		meStatus = TS_CANCELLED;
		return true;
	}
	inline bool Task::isCancelled()
	{
		return (getStatus() == TS_CANCELLED);
	}
	inline bool Task::setStatus(Status eStatus)
	{
		if (isCancelled() || getStatus() == TS_FINISHED)
		{
			return false;
		}
		meStatus = eStatus;
		return true;
	}
	inline Task::Status Task::getStatus()
	{
		return meStatus;
	}
	inline bool Task::onReset()
	{
		return true;
	}
}

#endif//__DatabaseTask_h__