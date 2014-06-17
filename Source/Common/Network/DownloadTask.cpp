#include "NetworkPCH.h"
#include "DownloadTask.h"
namespace Network
{
	DownloadTask::DownloadTask(const std::string& strURL, const std::string& strLocal)
		: mstrURL(strURL), muiTotalBytes(0), meStatus(TS_STOPED), muiDownloadedBytes(0),
		mstrHost(""), mstrPort(""), mstrFilePath(""), mstrFileName(""), mstrLocal(strLocal)
	{
	}

	DownloadTask::~DownloadTask()
	{
	}

	std::string UrlGB2312(const char* str)
	{
		std::string dd;
		size_t len = strlen(str);
		for (size_t i=0;i<len;i++)
		{
			if(isalnum(str[i]))
			{
				char tempbuff[2];
				sprintf(tempbuff,"%c",str[i]);
				dd.append(tempbuff);
			}
			else if (isspace(str[i]))
			{
				dd.append("%20");
			}
			else
			{
				char tempbuff[4];
				sprintf(tempbuff,"%%%X%X",((unsigned char*)str)[i] >>4,((unsigned char*)str)[i] %16);
				dd.append(tempbuff);
			}
		}
		return dd;
	}

	bool DownloadTask::parseURL()
	{
		int pos0 = mstrURL.find_first_of("//");
		int pos1 = mstrURL.find_last_of(":");
		int pos2 = mstrURL.find_last_of("/");
		if (pos0 == -1 || pos1 == -1 || pos2 == -1)
			return false;

		mstrHost = mstrURL.substr(pos0+2, pos1-pos0-2);
		int port = atoi(mstrURL.substr(pos1+1, pos2-pos1-1).c_str());
		char tmp[8] = {};
		sprintf(tmp, "%d", port);
		mstrPort = tmp;
		mstrFilePath = mstrURL.substr(0, pos2+1);
		mstrFileName = mstrURL.substr(pos2+1, mstrURL.length()-pos2-1);

		mstrFileName = UrlGB2312(mstrFileName.c_str());

		return true;
	}
}
