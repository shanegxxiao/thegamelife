#ifndef __DownloadMgr_h__
#define __DownloadMgr_h__

#include <list>
namespace Network
{
	class DownloadTask;

	class DownloadMgr
	{
	public:
		DownloadMgr();
		~DownloadMgr();

		bool init();
		bool shut();

	DownloadTask* addTask(std::string strURL, const std::string& strLocal, unsigned int size = 0); // size表示预测的尺寸
	bool removeTask(DownloadTask* pkTask);
	void pauseAllTasks();
	void resumeAllTasks();

		void stat(); // 统计下载相关数据，建议每秒统计一次

		unsigned int getSpeed() { return muiDownloadSpeed; }
		unsigned int getDownloadedBytes() { return muiDownloadedBytes; }

	private:
		std::list<DownloadTask*> mDownloadTasksList;	// 下载任务列表，完成的任务应及时清除

		unsigned int muiLastStatTick;				// 上次统计的时刻
		unsigned int muiDownloadSpeed;			// 平均下载速度
		unsigned int muiDownloadedBytes;			// 累计总下载量
		unsigned int muiLastDownloadedBytes;	// 上次统计时刻的累计总下载量
		unsigned int muiFinishedBytes;			// 已完成的任务的尺寸
	};
}
#endif//__DownloadMgr_h__
