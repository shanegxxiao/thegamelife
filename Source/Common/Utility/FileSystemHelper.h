#ifndef __FileSystemHelper_h__
#define __FileSystemHelper_h__

#include <boost/filesystem.hpp>

namespace Utility
{
	namespace FileSystemHelper
	{
		const std::string GetModulePath();
		const std::wstring GetModuleNameW();
        const std::string GetModuleNameA();
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