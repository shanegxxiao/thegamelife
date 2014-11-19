#ifndef __ObjectSystem_h__
#define __ObjectSystem_h__

#include "RuntimeDefine.h"

namespace Runtime
{
	class Object;
	class ObjectCreator;

	class ObjectSystem
	{
	public:
		static ObjectSystem* GetInstancePtr();
		virtual ~ObjectSystem(void);

		bool Initialize(ParamList &kParamList);
		bool Shutdown();

		Object *GetServiceObject(std::string strClassName);
		Object *GetServiceObject(unsigned int uiClassID);
	
		Object *CreateEntityObject(std::string strClassName);
		Object *CreateEntityObject(unsigned int uiClassID);

		//Runtime Message System
		unsigned int SubscribeRTMsg(unsigned int uiMessage, 
			unsigned int uiPriority, Object* pkObject, MsgFunc pFunc);
		unsigned int UnsubscribeRTMsg(unsigned int uiMessage, 
			Object* pkObject, MsgFunc pFunc);
		unsigned int SubscribeRTMsg(unsigned int uiMessage, 
			unsigned int uiPriority, WndowRef *pkReceiver);
		unsigned int UnsubscribeRTMsg(unsigned int uiMessage, 
			WndowRef *pkReceiver);
		void SendRTMsg(RTMsg &kMsg);
		void SendRTMsg(unsigned int uiMessage, WPARAM wParam = 0, LPARAM lParam = 0);

		void EnableAutoRunning(Object *pkObject, RTPriority ePriority);
		void Running();

	public:
		ObjectCreator* GetServiceCreator(std::string &strName);

	private:
		ObjectSystem(void);
		bool LoadServiceAndEntityFromDLL(const char *path);
		bool LoadServiceAndEntityFromLib(ObjectCreator *pkCreatorListHead);
		
		typedef std::vector<ObjectCreator *> ObjectCreatorVector;
		void GetSortedServiceList(DependencyMode eMode, 
			ObjectCreatorVector& kServiceList);
		void InsertIntoSortedServiceList(DependencyMode eMode, 
			ObjectCreatorVector& kServiceList, ObjectCreator* pkCreator);
		int GetIndexOfServiceInSortedList(const ObjectCreatorVector& kServiceList,
			ObjectCreator* pkCreator);

	private:
		
		typedef std::map<std::string, ObjectCreator *> 
			CreatorContainerIndexByName;
		typedef std::map<std::string, ObjectCreator *>::iterator
			CreatorContainerIndexByNameIter;
		typedef std::map<std::string, ObjectCreator *>::value_type
			CreatorContainerByNameValueType;
		typedef std::map<unsigned int, ObjectCreator *>
			CreatorContainerIndexByID;
		typedef std::map<unsigned int, ObjectCreator *>::iterator
			CreatorContainerIndexByIDIter;
		typedef std::map<unsigned int, ObjectCreator *>::value_type
			CreatorContainerByIDValueType;

		typedef std::multimap<unsigned int, Object *> PriorityObjectMap;
		typedef std::multimap<unsigned int, MsgHandler> PriorityHandlerMap;
		typedef std::multimap<unsigned int, WndowRef *> PriorityWindowRefMap;

		typedef std::map<std::string, Object *> NameToObjectMap;
		typedef std::map<unsigned int, Object *> IDToObjectMap;

		typedef void (*PFuncGetObjectCreator)(ObjectCreator *&pkObjectCreator);

	private:
		CreatorContainerIndexByName m_kServicesIndexByName;
		CreatorContainerIndexByID m_kServicesIndexByID;
		CreatorContainerIndexByName m_kEntitiesIndexByName;
		CreatorContainerIndexByID m_kEntitiesIndexByID;
		PriorityHandlerMap m_kMsgToHandlersMap[MAX_MSG_VALUE];
		PriorityWindowRefMap m_kMsgToWindowRefsMap[MAX_MSG_VALUE];
		PriorityObjectMap m_kPriorityRunningObjects;
		NameToObjectMap m_kNameToServiceMap;
		IDToObjectMap m_kIDToServiceMap;
	};

	inline void ObjectSystem::EnableAutoRunning(Object *pkObject,
		RTPriority ePriority)
	{
		m_kPriorityRunningObjects.insert(
			PriorityObjectMap::value_type(ePriority, pkObject));
	}

	inline void ObjectSystem::SendRTMsg(unsigned int uiMessage,
		WPARAM wParam, LPARAM lParam)
	{
		RTMsg kMsg;
		kMsg.uiMessage = uiMessage;
		kMsg.wParam = wParam;
		kMsg.lParam = lParam;
		SendRTMsg(kMsg);
	}

	inline ObjectCreator* ObjectSystem::GetServiceCreator(std::string &strName)
	{
		CreatorContainerIndexByName::iterator kIter = m_kServicesIndexByName.find(strName);
		if (kIter != m_kServicesIndexByName.end())
		{
			return kIter->second;
		}
		return 0;
	}
}

#define gpkObjectSystem Runtime::ObjectSystem::GetInstancePtr()

#endif//#ifndef __ObjectSystem_h__
//END OF FILE