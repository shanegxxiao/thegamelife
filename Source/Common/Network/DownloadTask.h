#ifndef __DownloadTask_h__
#define __DownloadTask_h__
namespace Network
{

#define THREAD_COUNT	2		// 每个任务线程数
#define BUFFER_SIZE	1024	// 缓冲区尺寸

	class DownloadTask
	{
	public:
		// 下载任务状态
		enum TaskStatus
		{
			TS_STOPED,			// 停止
			TS_DOWNLOADING,	// 下载中
			TS_PAUSE,			// 暂停
			TS_COMPLETED,		// 完成
		};

	public:
		virtual bool start() = 0;
		virtual bool pause() = 0;
		virtual bool resume() = 0;
		virtual bool stop() = 0;

	unsigned int getDownloadedBytes() { return muiDownloadedBytes; }
	unsigned int getTotalBytes() { return muiTotalBytes; }
	TaskStatus getStatus() { return meStatus; }

	protected:
		DownloadTask(const std::string& strURL, const std::string& strLocal);
		virtual ~DownloadTask();

		/*
		解析URL地址，输出到各个分量，【注意：mstrURL必须是完整形式】
		示例：
		mstrURL：		"http://127.0.0.1:80/123/abc/abc.txt"
		mstrHost:		"127.0.0.1"
		muiPort:			80
		mstrFilePath:	"http://127.0.0.1:80/123/abc/"
		mstrFileName:	"abc.txt"
		*/
		bool parseURL();

		friend class DownloadMgr;

	protected:
		std::string mstrURL;					// 下载地址
		unsigned int muiTotalBytes;				// 总的尺寸
		TaskStatus meStatus;					// 下载状态
		unsigned int muiDownloadedBytes;		// 累计下载量

		std::string mstrHost;					// 主机地址
		std::string mstrPort;					// 主机端口
		std::string mstrFilePath;				// 文件的远程URL目录
		std::string mstrFileName;				// 文件名
		std::string mstrLocal;					// 本地保存位置
	};
}
#endif//__DownloadTask_h__