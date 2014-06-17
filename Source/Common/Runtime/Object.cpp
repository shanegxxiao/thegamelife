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

	Object::ObjectType Object::GetType()
	{
		return OT_OBJECT;
	}

	void Object::Running()
	{

	}

	unsigned int Object::SubscribeXGRTMsg(unsigned int uiMessage, 
		unsigned int uiPriority, MsgFunc pFunc)
	{
		return ms_pkObjectSystem->SubscribeXGRTMsg(uiMessage, uiPriority, this, pFunc);
	}

	unsigned int Object::UnsubscribeXGRTMsg(unsigned int uiMessage, MsgFunc pFunc)
	{
		return ms_pkObjectSystem->UnsubscribeXGRTMsg(uiMessage, this, pFunc);
	}
}