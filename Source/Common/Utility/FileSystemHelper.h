#ifndef __FileSystemHelper_h__
#define __FileSystemHelper_h__

namespace Utility
{
	namespace FileSystemHelper
	{
		const std::string GetModulePath();
		bool EnsurePathValid(const std::string& strPath);
		bool CopyFileWrap(const std::string& strSrcPath, const std::string& strDestPath, bool bFailedIfExist);
		bool DeleteFileWrap(const std::string& strPath);
		bool DeleteDir(const std::string& strDirectory);
		const std::vector<boost::filesystem::path>& ScanDirectory(const char *path,
			const char *pattern, int recurseDepth, std::vector<boost::filesystem::path>
			&fileVector = *(new std::vector<boost::filesystem::path>()));
	};
}

#endif//__FileSystemHelper_h__