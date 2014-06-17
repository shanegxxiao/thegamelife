#ifndef __targetver_h__
#define __targetver_h__

// 包括 SDKDDKVer.h 将定义可用的最高版本的 Windows 平台。

// 如果要为以前的 Windows 平台生成应用程序，请包括 WinSDKVer.h，并将
// WIN32_WINNT 宏设置为要支持的平台，然后再包括 SDKDDKVer.h。

#ifdef WIN32

#include <SDKDDKVer.h>

#endif // WIN32

#endif//__targetver_h__