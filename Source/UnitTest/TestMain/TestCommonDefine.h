#ifndef __TestCommonDefine_h__
#define __TestCommonDefine_h__

namespace TestCommonDefine
{
#pragma pack(push, 1)
	struct Packet 
	{
		unsigned int uiPacketSize;
		char acBuff[40 * 1024];
	};
#pragma pack(pop)
}

#endif//__TestCommonDefine_h__