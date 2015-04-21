#ifndef __Object_h__
#define __Object_h__

#include "RuntimeDefine.h"
#include "ObjectSystem.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ini_parser.hpp>

namespace Runtime
{
	class Object
	{
	public:
		enum ObjectType
		{
			OT_OBJECT,///< base type, not be recognized
			OT_SERVICE,///< singleton, will be auto initialized and shutdown
			OT_ENTITY,///< can be instantiated multi times, will not be auto initialized and shutdown
			OT_COUNT///< type count
		};

	public:
		Object(void);
		virtual ~Object(void);
		void EnableAutoRunning(RTPriority ePriority);
		unsigned int SubscribeRTMsg(unsigned int uiMessage, 
			unsigned int uiPriority, MsgFunc pFunc);
		unsigned int UnsubscribeRTMsg(unsigned int uiMessage, 
			MsgFunc pFunc);

        virtual bool Initialize(boost::property_tree::ptree& kParamList);
		virtual void Running();
		virtual bool Shutdown();
		
	public:
		static std::string GetWorkDir();
		static const char* GetCWorkDir();
		static std::string GetDllDir();
		static const char* GetCDllDir();
		static std::string GetResourceDir();
		static const char* GetCResourceDir();
		static std::string GetConfigDir();
		static const char* GetCConfigDir();

	protected:
		friend class ObjectSystem;
		static ObjectSystem *ms_pkObjectSystem;
		static std::string ms_workDir;
		static std::string ms_dllDir;
		static std::string ms_resourceDir;
		static std::string ms_cfgDir;
	};

	inline std::string Object::GetWorkDir()
	{
		return ms_workDir;
	}

	inline const char* Object::GetCWorkDir()
	{
		return ms_workDir.c_str();
	}

	inline std::string Object::GetDllDir()
	{
		return ms_dllDir;
	}

	inline const char* Object::GetCDllDir()
	{
		return ms_dllDir.c_str();
	}

	inline std::string Object::GetResourceDir()
	{
		return ms_resourceDir;
	}

	inline const char* Object::GetCResourceDir()
	{
		return ms_resourceDir.c_str();
	}

	inline std::string Object::GetConfigDir()
	{
		return ms_cfgDir;
	}

	inline const char* Object::GetCConfigDir()
	{
		return ms_cfgDir.c_str();
	}

	inline void Object::EnableAutoRunning(RTPriority ePriority)
	{
		gpkObjectSystem->EnableAutoRunning(this, ePriority);
	}
}

#endif//#ifndef __Object_h__
//END OF FILE