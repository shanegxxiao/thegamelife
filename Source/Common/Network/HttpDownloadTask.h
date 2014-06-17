#ifndef __HttpDownloadTask_h__
#define __HttpDownloadTask_h__
#include "DownloadTask.h"
namespace Network
{
	// HTTP下载分段
	struct HttpSec
	{
		HttpSec():task(0),nStart(0),nEnd(0)
		{
		}
		~HttpSec()
		{
		}
		DownloadTask* task;			// 任务指针
		std::string strHost;			// 主机地址
		std::string strPort;			// 主机端口
		std::string strFilePath;	// 文件的远程URL目录
		std::string strFileName;	// 文件名
		std::string strLocal;		// 本地保存位置
		unsigned int nStart;					// 分段的起始位置
		unsigned int nEnd;						// 分段的起始位置
	};

	// HTTP下载任务
	class HttpDownloadTask : public DownloadTask
	{
	public:
		virtual bool start();
		virtual bool pause();
		virtual bool resume();
		virtual bool stop();

	protected:
		unsigned int getContentLength();

	private:
		HttpDownloadTask(const std::string& strURL, const std::string& strLocal);
		virtual ~HttpDownloadTask();

		friend class DownloadMgr;
	};
}
#endif//__HttpDownloadTask_h__