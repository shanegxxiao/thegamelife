#include "RuntimePCH.h"
#include "Object.h"
namespace Runtime
{
	ObjectSystem *Object::ms_pkObjectSystem;
	std::string Object::ms_workDir;
	std::string Object::ms_dllDir;
	std::string Object::ms_resourceDir;
	std::string Object::ms_cfgDir;

	Object::Object(void)
	{

	}

	Object::~Object(void)
	{

	}

	bool Object::Initialize(ParamList& kParamList)
	{
		return true;
	}

	bool Object::Shutdown()
	{
		return true;
	}

	void Object::Running()
	{

	}

	unsigned int Object::SubscribeRTMsg(unsigned int uiMessage, 
		unsigned int uiPriority, MsgFunc pFunc)
	{
		return ms_pkObjectSystem->SubscribeRTMsg(uiMessage, uiPriority, this, pFunc);
	}

	unsigned int Object::UnsubscribeRTMsg(unsigned int uiMessage, MsgFunc pFunc)
	{
		return ms_pkObjectSystem->UnsubscribeRTMsg(uiMessage, this, pFunc);
	}
}