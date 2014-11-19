#include "UtilityPCH.h"
#include "FileSystemHelper.h"
#include "UtilityDefine.h"

#ifdef WIN32
#include <Windows.h>
#include <AtlConv.h>
#include <ShellAPI.h>
#include <mmsystem.h>
#include <io.h>
#include <Shlwapi.h>
#endif // WIN32

namespace Utility
{
	namespace FileSystemHelper
	{
		const std::string FileSystemHelper::GetModulePath()
		{
#ifdef WIN32
			USES_CONVERSION;
			TCHAR acBuff[UTILITY_MAX_PATH];
			GetModuleFileName(NULL, acBuff, UTILITY_MAX_PATH);

			std::string strTmp = W2A(acBuff);
			unsigned int uiPos = strTmp.find_last_of("\\");
			std::string strResult = strTmp.substr(0, uiPos);
			return strResult;
#else
			boost::filesystem::path full_path(boost::filesystem::current_path());
			return full_path.string();
#endif // WIN32

		}
		bool FileSystemHelper::EnsurePathValid(const std::string& strPath)
		{
			std::string strPathTmp = strPath;
			unsigned int uiPos = strPathTmp.find('\\', 0);
			while (uiPos != std::string::npos)
			{
				std::string strPath = strPathTmp.substr(0, uiPos);
				if (!boost::filesystem::is_directory(strPath.c_str()))
				{
					if (!boost::filesystem::create_directory(strPath.c_str()))
					{
						return false;
					}
				}
				uiPos = strPathTmp.find_first_of('\\', uiPos + 1);
			}
			return true;
		}
		bool FileSystemHelper::CopyFileWrap(const std::string& strSrcPath,
			const std::string& strDestPath,
			bool bFailedIfExist)
		{
			boost::system::error_code ec;
			boost::filesystem::copy_file(strSrcPath, strDestPath, bFailedIfExist ?
				boost::filesystem::copy_option::fail_if_exists :
				boost::filesystem::copy_option::overwrite_if_exists, ec);
			if (ec)
			{
				return false;
			}
			return true;
		}
		bool FileSystemHelper::DeleteFileWrap(const std::string& strPath)
		{
			if (!boost::filesystem::exists(strPath))
			{
				return false;
			}
			return boost::filesystem::remove(strPath.c_str());
		}
		bool FileSystemHelper::DeleteDir(const std::string& strDirectory)
		{
			if (!boost::filesystem::is_directory(strDirectory))
			{
				return false;
			}
			boost::system::error_code ec;
			boost::filesystem::remove_all(strDirectory, ec);
			if (ec)
			{
				return false;
			}
			return true;
		}

		const std::vector<boost::filesystem::path>& FileSystemHelper::ScanDirectory(const char *path,
			const char *pattern, int recurseDepth, std::vector < boost::filesystem::path >
			&fileVector/* = *(new vector<boost::filesystem::path>())*/)
		{
			boost::filesystem::path fullpath(path);
			std::vector<boost::filesystem::path> &ret = fileVector;

			if (!boost::filesystem::exists(fullpath))
			{
				return ret;
			}
			/// 无参构造函数是最后那个iterator的value 摘抄如下
			/// If the end of the directory elements is reached, the iterator becomes equal 
			/// to the end iterator value. The constructor directory_iterator() with no arguments 
			/// always constructs an end iterator object, which is the only legitimate iterator to 
			/// be used for the end condition. The result of operator* on an end iterator is not defined. 
			/// For any other iterator value a const directory_entry& is returned. The result ofoperator-> 
			/// on an end iterator is not defined. For any other iterator value a const directory_entry* is returned.
			boost::filesystem::directory_iterator end_iter;
			for (boost::filesystem::directory_iterator iter(fullpath); iter != end_iter; ++iter)
			{
				try
				{
					if (boost::filesystem::is_directory(*iter))
					{
						if (recurseDepth > 0)
						{
							ScanDirectory(iter->path().string().c_str(), pattern, recurseDepth - 1, ret);
						}
						else if (recurseDepth == -1)
						{
							ScanDirectory(iter->path().string().c_str(), pattern, -1, ret);
						}
					}
					else
					{
						boost::regex reg(pattern);
						if (boost::regex_match(iter->path().filename().string(), reg))
						{
							ret.push_back(iter->path());
						}
					}
				}
				catch (const std::exception&)
				{
					continue;
				}
			}
			return ret;
		}
	}
}