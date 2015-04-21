#include "NetworkPCH.h"
#include "DownloadMgr.h"
#include "HttpDownloadTask.h"
namespace Network
{
	DownloadMgr::DownloadMgr()
		:muiLastStatTick(0),muiDownloadSpeed(0),muiDownloadedBytes(0),muiLastDownloadedBytes(0),muiFinishedBytes(0)
	{
	}

	DownloadMgr::~DownloadMgr()
	{
		shut();
	}

	bool DownloadMgr::init()
	{
		stat(); // 进行第一次统计，相当于初始化
		return true;
	}

	bool DownloadMgr::shut()
	{
		for (std::list<DownloadTask*>::iterator itr = mDownloadTasksList.begin(); itr != mDownloadTasksList.end(); ++itr)
		{
			DownloadTask* task = *itr;
			task->stop();
			delete task;
			task = 0;
		}
		mDownloadTasksList.clear();

		return true;
	}

	DownloadTask* DownloadMgr::addTask(std::string strURL, const std::string& strLocal, unsigned int size)
	{
		// 解析URL的协议类型，调用对应的协议下载
		int pos = (int)strURL.find_first_of(":");
		if (pos == -1)
			return 0;
		std::string strProtocol = strURL.substr(0, pos);
		if (strProtocol.compare("http") == 0)
		{
			HttpDownloadTask* task = new HttpDownloadTask(strURL, strLocal);
			if (!task)
				return 0;
			task->muiTotalBytes = size;
			mDownloadTasksList.push_back(task);
			task->start();
			return task;
		}
		else if (strProtocol.compare("ftp") == 0)
		{
		}
		else if (strProtocol.compare("https") == 0)
		{
		}

			return 0;
	}

	bool DownloadMgr::removeTask(DownloadTask* pkTask)
	{
		if (!pkTask)
			return false;

		std::list<DownloadTask*>::iterator itr = mDownloadTasksList.begin();
		for (; itr != mDownloadTasksList.end(); ++itr)
		{
			if (pkTask == *itr)
			{
				pkTask->stop();
				delete pkTask;
				pkTask = 0;
				break;
			}
		}
		mDownloadTasksList.erase(itr);

		return true;
	}

	void DownloadMgr::pauseAllTasks()
	{
		for (std::list<DownloadTask*>::iterator itr = mDownloadTasksList.begin(); itr != mDownloadTasksList.end(); ++itr)
		{
			DownloadTask* task = *itr;
			task->pause();
		}
	}

	void DownloadMgr::resumeAllTasks()
	{
		for (std::list<DownloadTask*>::iterator itr = mDownloadTasksList.begin(); itr != mDownloadTasksList.end(); ++itr)
		{
			DownloadTask* task = *itr;
			task->resume();
		}
	}

	void DownloadMgr::stat()
	{
		// 清除已完成的下载任务
		std::list<DownloadTask*>::iterator itr = mDownloadTasksList.begin();
		while (itr != mDownloadTasksList.end())
		{
			DownloadTask* task = *itr;
			if (task->meStatus != DownloadTask::TS_COMPLETED)
			{
				++itr;
				continue;
			}

			// 更新已完成的尺寸
			muiFinishedBytes += task->getTotalBytes();

			std::list<DownloadTask*>::iterator itrtmp = itr;
			itr++;
			delete *itrtmp;
			*itrtmp = 0;
			mDownloadTasksList.erase(itrtmp);
		}

		// 统计下载速度（指定间隔的下载量，的平均）
		// 计算总下载量
		muiDownloadedBytes = muiFinishedBytes;
		for (std::list<DownloadTask*>::iterator itr = mDownloadTasksList.begin(); itr != mDownloadTasksList.end(); ++itr)
		{
			DownloadTask* task = *itr;
			muiDownloadedBytes += task->getDownloadedBytes();
		}
		// 计算下载速度
#ifdef WIN32
		unsigned int nowatick = GetTickCount();
#else
		unsigned int nowatick = clock();
#endif//WIN32
		unsigned int elapsedtick = nowatick-muiLastStatTick;
		if (elapsedtick == 0)
			elapsedtick = 1;
		muiDownloadSpeed = (muiDownloadedBytes-muiLastDownloadedBytes) / elapsedtick;
		// 记下本次数据
		muiLastDownloadedBytes = muiDownloadedBytes;
		muiLastStatTick = nowatick;
	}
}
