#include "UtilityPCH.h"
#include <stdio.h>
#include "FrameRate.h"
#include "UtilityDefine.h"
#ifdef WIN32
#include <Windows.h>
#include <MMSystem.h>
#pragma comment(lib, "Winmm.lib") 
#endif // WIN32

namespace Utility
{
	FrameRate::FrameRate()
		: muLastFrameTick(0)
	{
		#ifdef WIN32
		GetConsoleTitleA(macOldTitleName, UTILITY_MAX_PATH);
		#endif // WIN32
	}
	FrameRate::~FrameRate()
	{
	}
	void FrameRate::stat()
	{
		boost::timer::cpu_times uCurrentTick = mkTimer.elapsed();
		boost::timer::nanosecond_type uFrameTime = uCurrentTick.wall - muLastFrameTick;
		if (uFrameTime == 0)
		{
			uFrameTime = 1;
		}
		muLastFrameTick = uCurrentTick.wall;
		mfFrameRate = 1.f / (uFrameTime * 1e-9f);
	}
	float FrameRate::getRate()
	{
		return mfFrameRate;
	}
	void FrameRate::showOnWindowTitle(float fUpdateDuration)
	{
		boost::timer::cpu_times uCurrentTick = mkTimer.elapsed();
		if ((uCurrentTick.wall - muWinTitleLastUpdateTick) * 1e-9f < fUpdateDuration)
		{
			return;
		}
		muWinTitleLastUpdateTick = uCurrentTick.wall;
		char acNewTitleName[UTILITY_MAX_PATH];

#ifdef WIN32
		sprintf_s(acNewTitleName, UTILITY_MAX_PATH, "%.2f - %s", mfFrameRate, macOldTitleName);
		SetConsoleTitleA(acNewTitleName);
#else
		snprintf(acNewTitleName, UTILITY_MAX_PATH, "%.2f - %s", mfFrameRate, macOldTitleName);
#endif // WIN32
	}
}