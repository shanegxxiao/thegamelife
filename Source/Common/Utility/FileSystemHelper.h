#ifndef __FileSystemHelper_h__
#define __FileSystemHelper_h__

namespace Utility
{
	class FileSystemHelper
	{
	public:
		static const std::string GetModulePath();
		static bool EnsurePathValid(const std::string& strPath);
		static bool CopyFileWrap(const std::string& strSrcPath, const std::string& strDestPath, bool bFailedIfExist);
		static bool DeleteFileWrap(const std::string& strPath);
		static bool DeleteDir(const std::string& strDirectory);
		static const std::vector<boost::filesystem::path>& ScanDirectory(const char *path, 
			const char *pattern, int recurseDepth, std::vector<boost::filesystem::path>
			&fileVector = *(new std::vector<boost::filesystem::path>()));
	};
}

#endif//__FileSystemHelper_h__