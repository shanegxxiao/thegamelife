#ifndef __LogSystemDefine_h__
#define __LogSystemDefine_h__

#ifdef WIN32
#include <Windows.h>
#endif // WIN32

namespace Runtime
{
	enum LogLevel
	{
		L_TRACE = 0,
		L_DEBUG,
		L_INFO,
		L_WARNING,
		L_ERROR,
		L_FATAL,
		L_LEVELNUM
	};

#ifdef WIN32
	#ifdef UNICODE
		typedef std::wstring stdstring;
	#else
		typedef std::string stdstring;
	#endif
#endif // WIN32

	#define MAX_OUTPUT_LENGTH 1024
}

#ifndef WIN32

#define FOREGROUND_BLUE      0x0001 // text color contains blue.
#define FOREGROUND_GREEN     0x0002 // text color contains green.
#define FOREGROUND_RED       0x0004 // text color contains red.
#define FOREGROUND_INTENSITY 0x0008 // text color is intensified.
#define BACKGROUND_BLUE      0x0010 // background color contains blue.
#define BACKGROUND_GREEN     0x0020 // background color contains green.
#define BACKGROUND_RED       0x0040 // background color contains red.
#define BACKGROUND_INTENSITY 0x0080 // background color is intensified.
#define COMMON_LVB_LEADING_BYTE    0x0100 // Leading Byte of DBCS
#define COMMON_LVB_TRAILING_BYTE   0x0200 // Trailing Byte of DBCS
#define COMMON_LVB_GRID_HORIZONTAL 0x0400 // DBCS: Grid attribute: top horizontal.
#define COMMON_LVB_GRID_LVERTICAL  0x0800 // DBCS: Grid attribute: left vertical.
#define COMMON_LVB_GRID_RVERTICAL  0x1000 // DBCS: Grid attribute: right vertical.
#define COMMON_LVB_REVERSE_VIDEO   0x4000 // DBCS: Reverse fore/back ground attribute.
#define COMMON_LVB_UNDERSCORE      0x8000 // DBCS: Underscore.

#define COMMON_LVB_SBCSDBCS        0x0300 // SBCS or DBCS flag.

#endif //WIN32

#define FW FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE
#define FGR FOREGROUND_RED
#define FG FOREGROUND_GREEN
#define FB FOREGROUND_BLUE
#define FY FOREGROUND_RED | FOREGROUND_GREEN
#define FC FOREGROUND_GREEN | FOREGROUND_BLUE
#define FM FOREGROUND_BLUE | FOREGROUND_RED 
#define FWI FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY
#define FRI FOREGROUND_RED | FOREGROUND_INTENSITY
#define FGI FOREGROUND_GREEN | FOREGROUND_INTENSITY
#define FBI FOREGROUND_BLUE | FOREGROUND_INTENSITY
#define FYI FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY
#define FCI FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY
#define FMI FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY 
#define FNULL 0

#define BW BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE
#define BR BACKGROUND_RED
#define BG BACKGROUND_GREEN
#define BB BACKGROUND_BLUE
#define BY BACKGROUND_RED | BACKGROUND_GREEN
#define BC BACKGROUND_GREEN | BACKGROUND_BLUE
#define BM BACKGROUND_BLUE | BACKGROUND_RED 
#define BWI BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_INTENSITY
#define BRI BACKGROUND_RED | BACKGROUND_INTENSITY
#define BGI BACKGROUND_GREEN | BACKGROUND_INTENSITY
#define BBI BACKGROUND_BLUE | BACKGROUND_INTENSITY
#define BYI BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_INTENSITY
#define BCI BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_INTENSITY
#define BMI BACKGROUND_BLUE | BACKGROUND_RED | BACKGROUND_INTENSITY 
#define BNULL 0

static char *gs_cLogColor[Runtime::L_LEVELNUM] = { "#ffffff", "#ffffff", "#9d9dee", "#00ff00", "#ddaa00", "#ff0000" };
static unsigned int gs_uiLogColor[Runtime::L_LEVELNUM] = { FW, BW, FG, FY, FGR, BR };

#endif//#ifndef __LogSystemDefine_h__
//END OF FILE
