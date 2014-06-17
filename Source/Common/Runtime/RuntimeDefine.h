#ifndef __FrameworkDLLType_h__
#define __FrameworkDLLType_h__

namespace Runtime
{
	typedef unsigned int WPARAM;
	typedef long LPARAM;
	typedef void* HWND;
	
	//Message structure
	typedef struct tagRTMsg
	{
		unsigned int uiMessage;
		WPARAM wParam;
		LPARAM lParam;
		bool bStop;               ///< 是否消息停止传递下去

		tagRTMsg()
		{
			uiMessage = 0;
			wParam = 0;
			lParam = 0;
			bStop = false;
		}
	} RTMsg;

	//Object runtime priority
	typedef enum tagPriority
	{
		RTP_LOWEST = 0x10000000,
		RTP_BELLOW_LOW = 0x20000000,
		RTP_LOW = 0x30000000,
		RTP_BELLOW_NORMAL = 0x40000000,
		RTP_NORMAL = 0x50000000,
		RTP_BELLOW_HIGH = 0x60000000,
		RTP_HIGH = 0x70000000,
		RTP_HIGHEST = 0x80000000
	}RTPriority;

	typedef HWND WndowRef;

	class Object;
	class ObjectCreator;

	typedef void (Object::*MsgFunc)(RTMsg &kMsg);
	typedef struct _MsgHandler 
	{
		Object* mpkObject;
		MsgFunc mpFunc;
		_MsgHandler(Object* pkObject, MsgFunc pFunc)
		{
			mpkObject = pkObject;
			mpFunc = pFunc;
		}
	}MsgHandler;
	typedef std::map<unsigned int, MsgHandler> Msg2HandlerMap;

	enum DependencyMode
	{
		DM_Init,
		DM_Shut,
		DM_Cnt
	};
	enum DependencyType
	{
		DT_Before,
		DT_After,
		DT_Cnt
	};
	struct Dependency
	{
		DependencyType meType;
		std::string mstrObjectName;
		Dependency* mpNext;
		ObjectCreator* mpCreator;

		Dependency(DependencyType type, const char* objectName)
		{
			meType = type;
			mstrObjectName = objectName;
			mpNext = 0;
			mpCreator = 0;
		}
	};
}

#define MIN_MSG_VALUE 0
#define MAX_MSG_VALUE 10000

#pragma warning(disable : 4251)

#endif//#ifndef __FrameworkDLLType_h__
//END OF FILE