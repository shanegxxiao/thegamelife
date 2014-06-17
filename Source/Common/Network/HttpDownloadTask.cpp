#include "NetworkPCH.h"
#include "HttpDownloadTask.h"
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/array.hpp>
#include <iostream>


namespace Network
{
	void ThreadHTTPDownloadFunc(HttpSec* task)
	{
		try
		{
			if (!task)
				return;

			// 0.打开未下载完的文件
			//////////////////////////////////////////////////////////////////////////
			FILE* f = NULL;
			int startpos = 0; // 已下载的尺寸
			if (boost::filesystem::exists(task->strLocal))
			{
				startpos = (int)boost::filesystem::file_size(task->strLocal);
				f = fopen(task->strLocal.c_str(), "r+b");
				if (!f)
					return;
				fseek(f, startpos, SEEK_SET);
			}
			else
			{
				f = fopen(task->strLocal.c_str(), "wb");
				if (!f)
					return;
			}

			// 1.连接
			//////////////////////////////////////////////////////////////////////////
			// 取得与服务器名相关的端点列表
			boost::asio::io_service io_service;
			boost::asio::ip::tcp::resolver resolver(io_service);
			boost::asio::ip::tcp::resolver::query query(task->strHost.c_str(), task->strPort.c_str());
			boost::asio::ip::tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);

			// 尝试连接每个服务器端点，直到成功建立连接
			boost::asio::ip::tcp::socket socket(io_service);
			boost::asio::connect(socket, endpoint_iterator);

			// 2.发送请求
			//////////////////////////////////////////////////////////////////////////
			// 填写并发送请求
			boost::asio::streambuf request;
			std::ostream request_stream(&request);
			request_stream << "GET " << task->strFilePath << task->strFileName << " HTTP/1.1\r\n";
			request_stream << "Host: " << task->strHost << "\r\n";
			request_stream << "Accept: */*\r\n";
			request_stream << "Range: bytes=" << task->nStart+startpos << "-" << task->nEnd << "\r\n";
			request_stream << "Connection: close\r\n\r\n";
			boost::asio::write(socket, request);

			// 3.获取文件尺寸
			//////////////////////////////////////////////////////////////////////////
			// 读取响应头
			boost::array<char, BUFFER_SIZE> buf; // 每次最多接收1024字节（1K）
			boost::system::error_code ec;
			std::size_t nLen = socket.read_some(boost::asio::buffer(buf), ec);

			if (strstr(buf.data(), "416 Requested Range Not Satisfiable")) // 范围不对，可能是下载完了
			{
				fclose(f);
				return;
			}

			// 寻找"Content-Length:"，它的后面跟着就是文件尺寸信息了
			char* sz_content_len = strstr(buf.data(), "Content-Length:");
			if (!sz_content_len)
			{
				fclose(f);
				return;
			}
			sz_content_len += strlen("Content-Length:");
			int content_len = atoi(sz_content_len);

			// 4.下载具体的文件数据
			//////////////////////////////////////////////////////////////////////////
			// 读取（接收）并写入实际数据到文件
			// 先写入第一次接收的正文数据
			int recved = 0;
			char* datastart = strstr(buf.data(), "\r\n\r\n");
			if (!datastart)
			{
				fclose(f);
				return;
			}
			datastart += 4;
			int firstlen = nLen-(datastart-buf.data());
			fwrite(datastart, 1, firstlen, f);
			recved += firstlen;
			// 再接收写入其余数据
			while(recved < content_len)
			{
				if (task->task->getStatus() == DownloadTask::TS_PAUSE)
				{
					boost::this_thread::sleep(boost::posix_time::milliseconds(10)); // 避免暂停时CPU占用过高
					continue;
				}
				else if (task->task->getStatus() == DownloadTask::TS_STOPED)
					break;

				nLen = socket.read_some(boost::asio::buffer(buf), ec);
				if (nLen == 0)
					break;
				fwrite(buf.data(), 1, nLen, f);
				recved += nLen;
			}
			fclose(f);
		}
		catch (std::exception& e)
		{
			//std::cout << "Exception: " << e.what() << "\n";
			throw e;
		}
	}

	HttpDownloadTask::HttpDownloadTask(const std::string& strURL, const std::string& strLocal)
	: DownloadTask(strURL, strLocal)
	{
	}

	HttpDownloadTask::~HttpDownloadTask()
	{
	}

	bool HttpDownloadTask::start()
	{
		// 检查状态
		if (meStatus != TS_STOPED)
			return false;

		//// 检查是否已经下载过了
		//if (boost::filesystem::exists(mstrLocal))
		//{
		//	meStatus = DownloadTask::TS_COMPLETED;
		//	return true;
		//}

		// 确保本地目录已生成
		int pos = mstrLocal.find_last_of("/");
		if (pos == -1)
			return false;
		std::string strLocalPath = mstrLocal.substr(0, pos+1);
		if (!boost::filesystem::exists(strLocalPath))
			boost::filesystem::create_directories(strLocalPath);

		// 解析下载地址
		if (!parseURL())
			return false;

		// 判断预测的远程文件尺寸
		if (muiTotalBytes == 0)
		{
			muiTotalBytes = getContentLength(); // 未预测，则手动去获取
			if (muiTotalBytes == 0)
				return false;
		}

		// 计算各个分段尺寸
		unsigned int secSize = muiTotalBytes / THREAD_COUNT;

		// 设置状态
		meStatus = TS_DOWNLOADING;

		// 填充分段信息
		HttpSec secs[THREAD_COUNT];
		for (int i = 0; i < THREAD_COUNT; i++)
		{
			secs[i].task = this;
			secs[i].strHost = mstrHost;
			secs[i].strPort = mstrPort;
			secs[i].strFilePath = mstrFilePath;
			secs[i].strFileName = mstrFileName;
			char num[4] = {};
			sprintf(num, "%d", i);
			secs[i].strLocal = mstrLocal + num;
			secs[i].nStart = i*secSize;
			if (i == THREAD_COUNT-1)
				secs[i].nEnd = muiTotalBytes;
			else
				secs[i].nEnd = (i+1)*secSize - 1;
		}

#ifdef WIN32
		DWORD starttick = GetTickCount();
#else
		unsigned int starttick = clock();
#endif // WIN32
		//std::cout << "Start download...\n";

		// 开启各个分段的下载线程
		boost::thread_group kThreadGroup;
		for (int i = 0; i < THREAD_COUNT; i++)
			kThreadGroup.create_thread(boost::bind(&ThreadHTTPDownloadFunc, &secs[i]));

		// 等待各个分段下载完成
		kThreadGroup.join_all();
		//std::cout << "Download completed. Cost tick: " << GetTickCount()-starttick << "\n";

		if (THREAD_COUNT > 1)
		{
			// 合并分段为单一文件
			boost::filesystem::ofstream to_file(mstrLocal, std::ios_base::out | std::ios_base::binary);
			if (!to_file)
				return false;
			char buf[BUFFER_SIZE] = {};
			for (int i = 0; i < THREAD_COUNT; i++)
			{
				boost::filesystem::ifstream from_file(secs[i].strLocal, std::ios_base::in | std::ios_base::binary);
				if (!from_file)
					return false;

				while (1)
				{
					int s = (int)from_file.read(buf, 1024).gcount();
					if (s <= 0)
						break;
					to_file.write(buf, s);
					if (to_file.fail())
						return false;
				}

				if (!from_file.eof())
					return false;
			}

			// 删除分段文件
			for (int i = 0; i < THREAD_COUNT; i++)
				boost::filesystem::remove(secs[i].strLocal);
		}
		else // 针对单线程的优化
		{
			boost::filesystem::rename(secs[0].strLocal.c_str(), mstrLocal.c_str());
		}

		//// 解压文件
		//unsigned int uiPos = mstrLocal.find_last_of('.');
		//std::string destFileName = mstrLocal.substr(0, uiPos);
		//if (dec.DecodeFile(mstrLocal.c_str(), destFileName.c_str()) != SZ_OK)
		//	return false;

		//// 删除压缩文件
		//boost::filesystem::remove(mstrLocal);

		meStatus = DownloadTask::TS_COMPLETED;

		//std::cout << "Combine completed. Total cost tick: " << GetTickCount()-starttick << "\n";

		return true;
	}

	bool HttpDownloadTask::pause()
	{
		meStatus = TS_PAUSE;
		return true;
	}

	bool HttpDownloadTask::resume()
	{
		meStatus = TS_DOWNLOADING;
		return true;
	}

	bool HttpDownloadTask::stop()
	{
		meStatus = TS_STOPED;
		return true;
	}

	unsigned int HttpDownloadTask::getContentLength()
	{
		try
		{
			// 1.连接
			//////////////////////////////////////////////////////////////////////////
			// 取得与服务器名相关的端点列表
			boost::asio::io_service io_service;
			boost::asio::ip::tcp::resolver resolver(io_service);
			boost::asio::ip::tcp::resolver::query query(mstrHost.c_str(), mstrPort.c_str());
			boost::asio::ip::tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);

			// 尝试连接每个服务器端点，直到成功建立连接
			boost::asio::ip::tcp::socket socket(io_service);
			boost::asio::connect(socket, endpoint_iterator);

			// 2.发送请求
			//////////////////////////////////////////////////////////////////////////
			// 填写并发送请求
			boost::asio::streambuf request;
			std::ostream request_stream(&request);
			request_stream << "GET " << mstrFilePath << mstrFileName << " HTTP/1.1\r\n";
			request_stream << "Host: " << mstrHost << "\r\n";
			request_stream << "Accept: */*\r\n";
			request_stream << "Range: bytes=0-\r\n";
			request_stream << "Connection: close\r\n\r\n";
			boost::asio::write(socket, request);

			// 3.获取文件尺寸
			//////////////////////////////////////////////////////////////////////////
			// 读取响应头
			boost::array<char, BUFFER_SIZE> buf;
			boost::system::error_code ec;
			socket.read_some(boost::asio::buffer(buf), ec);
			// URL文件是否存在
			if (strstr(buf.data(), "404 Not Found"))
				return 0;

			// 寻找"Content-Length:"，它的后面跟着就是文件尺寸信息了
			char* sz_content_len = strstr(buf.data(), "Content-Length:");
			if (!sz_content_len)
				return 0;
			sz_content_len += strlen("Content-Length:");
			return atoi(sz_content_len);
		}
		catch (std::exception& e)
		{
			//std::cout << "Exception: " << e.what() << "\n";
			throw e;
		}
		return 0;
	}
}