#ifndef __FrameRate_h__
#define __FrameRate_h__

#include "UtilityDefine.h"

namespace Utility
{
	class FrameRate
	{
	public:
		FrameRate();
		~FrameRate();

		void stat();
		float getRate();
		void showOnWindowTitle(float fUpdateDuration);

	private:
		char macOldTitleName[UTILITY_MAX_PATH];
		boost::timer::nanosecond_type muWinTitleLastUpdateTick;
		boost::timer::nanosecond_type muLastFrameTick;
		float mfFrameRate;
		boost::timer::cpu_timer mkTimer;
	};

}
#endif//__FrameRate_h__