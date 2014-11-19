#include "RuntimePCH.h"
#include "Object.h"
#include "ObjectCreator.h"
#include "ObjectSystem.h"
#include "Utility/StringHelper.h"
#include "Utility/FileSystemHelper.h"
#include "Utility/UtilityDefine.h"

#ifdef WIN32
#include <Windows.h>
#include <atlconv.h>
#else
#include <dlfcn.h>
#endif // WIN32

namespace Runtime
{
	ObjectSystem* ObjectSystem::GetInstancePtr()
	{
		static ObjectSystem skObjectSystem;
		return &skObjectSystem;
	}

	ObjectSystem::ObjectSystem(void)
	{
	}

	ObjectSystem::~ObjectSystem(void)
	{
		m_kServicesIndexByName.clear();
		m_kServicesIndexByID.clear();
		m_kEntitiesIndexByName.clear();
		m_kEntitiesIndexByID.clear();
	}

	void ObjectSystem::GetSortedServiceList(DependencyMode eMode, 
		ObjectCreatorVector& kServiceList)
	{
		for(CreatorContainerIndexByNameIter kIter = m_kServicesIndexByName.begin();
			kIter != m_kServicesIndexByName.end(); ++kIter)
		{
			InsertIntoSortedServiceList(eMode, kServiceList, kIter->second);
		}
	}

	void ObjectSystem::InsertIntoSortedServiceList(DependencyMode eMode, 
		ObjectCreatorVector& kServiceList, ObjectCreator* pkCreator)
	{
		/// If we are already on the list, return.
		if(GetIndexOfServiceInSortedList(kServiceList, pkCreator) != -1)
			return;

		/// If we don't have dependencies, just push the module
		/// to the back of the list.

		if(!pkCreator->GetDependency(eMode))
		{
			kServiceList.push_back(pkCreator);         
			return;
		}

		/// First make sure that all 'after' dependencies are in the list.
		for(Dependency* dependency = pkCreator->GetDependency(eMode);
			dependency != 0; dependency = dependency->mpNext)
		{
			if(dependency->meType != DT_After)
				continue;

			dependency->mpCreator = GetServiceCreator(dependency->mstrObjectName);
			if(!dependency->mpCreator)
				continue; ///< Allow modules to not exist.

			if(GetIndexOfServiceInSortedList(kServiceList, dependency->mpCreator) == -1)
				InsertIntoSortedServiceList(eMode, kServiceList, dependency->mpCreator);
		}

		printf("ModuleManager::_insertModuleIntoList - Cycle in 'after' %s dependency chain of '%s'\n",
			eMode == DM_Init ? "init" : "shutdown",
			pkCreator->GetClassNameString().c_str());
		assert((GetIndexOfServiceInSortedList(kServiceList, pkCreator) == -1));

		/// Now add the module itself.
		const unsigned int numModules = kServiceList.size();
		for (unsigned int i = 0; i < numModules; ++ i)
		{
			const bool thisBeforeCurrent  = pkCreator->ConstrainedToComeBefore(kServiceList[i], eMode);
			const bool currentAfterThis   = kServiceList[i]->ConstrainedToComeAfter(pkCreator, eMode);

			printf("ModuleManager::_insertModuleIntoList - Ambiguous %s placement of module '%s' relative to '%s'\n",
				eMode == DM_Init ? "init" : "shutdown",
				pkCreator->GetClassNameString().c_str(),
				kServiceList[i]->GetClassNameString().c_str());
			assert(!(thisBeforeCurrent && currentAfterThis));

			/// If no contraints relate us to this module,
			/// push us one more position back in the line.
			if(!thisBeforeCurrent && !currentAfterThis)
				continue;

			// If this module is contrained to come before the
			// module at our current position but that module does
			// not actually have dependencies of its own, make sure
			// that module is at the back of the module list so that
			// if we have more dependencies, it will not prevent us
			// from correctly positioning us in relation to them.

			if( thisBeforeCurrent && !kServiceList[i]->GetDependency(eMode) && i != numModules - 1 )
			{
				ObjectCreator* depCreator = kServiceList[i];
				kServiceList.erase(kServiceList.begin() + i);
				-- i;
				kServiceList.push_back(depCreator);
				continue;
			}

			/// Try the reverse constraint with all remaining modules in the list.
			/// If there is one for which we have one, then the placement of this
			/// module is ambiguous.
			for(unsigned int n = i + 1; n < numModules; ++ n)
			{
				printf("ModuleManager::_insertModuleIntoList - Ambiguous %s constraint on module '%s' to come before '%s' yet after '%s'\n",
					eMode == DM_Init ? "init" : "shutdown",
					pkCreator->GetClassNameString().c_str(),
					kServiceList[i]->GetClassNameString().c_str(),
					kServiceList[n]->GetClassNameString().c_str());
				assert((!(kServiceList[n]->ConstrainedToComeBefore(pkCreator, eMode)
				|| pkCreator->ConstrainedToComeAfter(kServiceList[n], eMode))));
			}

			// Add the module at this position.
			kServiceList.insert(kServiceList.begin() + i, pkCreator);
			return;
		}

		// No constraint-based position.  Just append.
		kServiceList.push_back(pkCreator);
	}

	int ObjectSystem::GetIndexOfServiceInSortedList(
		const ObjectCreatorVector& kServiceList, ObjectCreator* pkCreator)
	{
		for (unsigned int ui = 0; ui < kServiceList.size(); ++ui)
		{
			if (kServiceList[ui] == pkCreator)
			{
				return ui;
			}
		}
		return -1;
	}

	bool ObjectSystem::Initialize(ParamList &kParamList)
	{
		Object::ms_pkObjectSystem = this;
		
		void* pVoidTmp = 0;
		char* acWorkDir = 0;
		char* acDllDir = 0;
		char* acResourceDir = 0;
		char* acConfigDir = 0;
		ObjectCreator *pObjectCreator = 0;
		kParamList.GetParam("acWorkDir", pVoidTmp);
		acWorkDir = (char*)pVoidTmp;
		kParamList.GetParam("acDllDir", pVoidTmp);
		acDllDir = (char*)pVoidTmp;
		kParamList.GetParam("acResourceDir", pVoidTmp);
		acResourceDir = (char*)pVoidTmp;
		kParamList.GetParam("acConfigDir", pVoidTmp);
		acConfigDir = (char*)pVoidTmp;
		kParamList.GetParam("gpObjectCreator", pVoidTmp);
		pObjectCreator = (ObjectCreator*)pVoidTmp;
		Object::ms_workDir = acWorkDir;
		Object::ms_dllDir = acDllDir;
		Object::ms_resourceDir = acResourceDir;
		Object::ms_cfgDir = acConfigDir;
		if (!LoadServiceAndEntityFromDLL(Object::ms_dllDir.c_str()))
		{
			/// System LoadServiceAndEntityFromDLL return false.
		}
		if (!LoadServiceAndEntityFromLib(pObjectCreator))
		{
			/// System LoadServiceAndEntityFromLib return false.
		}

		ObjectCreatorVector kServiceVector;
		GetSortedServiceList(DM_Init, kServiceVector);
		for (unsigned int ui = 0; ui < kServiceVector.size(); ++ui)
		{
			Object* pkObject = GetServiceObject(kServiceVector[ui]->GetClassNameString());
			if (!pkObject)
			{
				continue;
			}
			pkObject->Initialize(kParamList);
		}

		return true;
	}

	bool ObjectSystem::Shutdown()
	{
		ObjectCreatorVector kServiceVector;
		GetSortedServiceList(DM_Shut, kServiceVector);
		for (unsigned int ui = 0; ui < kServiceVector.size(); ++ui)
		{
			Object* pkObject = GetServiceObject(kServiceVector[ui]->GetClassNameString());
			if (!pkObject)
			{
				continue;
			}
			pkObject->Shutdown();
		}

		NameToObjectMap::iterator kIterService = m_kNameToServiceMap.begin();
		while (kIterService != m_kNameToServiceMap.end())
		{
			delete kIterService->second;
			++kIterService;
		}
		m_kNameToServiceMap.clear();
		m_kIDToServiceMap.clear();
		m_kServicesIndexByName.clear();
		m_kServicesIndexByID.clear();
		m_kEntitiesIndexByName.clear();
		m_kEntitiesIndexByID.clear();
		m_kPriorityRunningObjects.clear();
		for (unsigned int ui = 0; ui < MAX_MSG_VALUE; ++ui)
		{
			m_kMsgToHandlersMap[ui].clear();
			m_kMsgToWindowRefsMap[ui].clear();
		}
		return true;
	}

	Object* ObjectSystem::GetServiceObject(std::string strClassName)
	{
		NameToObjectMap::iterator kIterObject = m_kNameToServiceMap.find(strClassName);
		if (kIterObject != m_kNameToServiceMap.end())
		{
			return kIterObject->second;
		}
		CreatorContainerIndexByNameIter iter =
			m_kServicesIndexByName.find(strClassName);
		if (iter == m_kServicesIndexByName.end())
			return NULL;
		Object *pkObject = (Object*)((iter->second)->CreateObject());
		if (!pkObject)
		{
			return NULL;
		}
		m_kNameToServiceMap[strClassName] = pkObject;
		m_kIDToServiceMap[(iter->second)->GetClassId()] = pkObject;
		return pkObject;
	}

	Object* ObjectSystem::GetServiceObject(unsigned int uiClassID)
	{
		IDToObjectMap::iterator kIterObject = m_kIDToServiceMap.find(uiClassID);
		if (kIterObject != m_kIDToServiceMap.end())
		{
			return kIterObject->second;
		}
		CreatorContainerIndexByIDIter iter = m_kServicesIndexByID.find(uiClassID);
		if (iter == m_kServicesIndexByID.end())
			return NULL;
		Object *pkObject = (Object*)((iter->second)->CreateObject());
		if (!pkObject)
		{
			return NULL;
		}
		m_kNameToServiceMap[(iter->second)->GetClassNameString()] = pkObject;
		m_kIDToServiceMap[uiClassID] = pkObject;
		return pkObject;
	}

	Object* ObjectSystem::CreateEntityObject(std::string strClassName)
	{
		CreatorContainerIndexByNameIter iter =
			m_kEntitiesIndexByName.find(strClassName);
		if (iter == m_kEntitiesIndexByName.end())
			return NULL;
		Object *pkObject = (Object*)((iter->second)->CreateObject());
		return pkObject;
	}

	Object* ObjectSystem::CreateEntityObject(unsigned int classID)
	{
		CreatorContainerIndexByIDIter iter = m_kEntitiesIndexByID.find(classID);
		if (iter == m_kEntitiesIndexByID.end())
			return NULL;
		Object *pkObject = (Object*)((iter->second)->CreateObject());
		return pkObject;
	}

	bool ObjectSystem::LoadServiceAndEntityFromDLL(const char *path)
	{
		std::vector<boost::filesystem::path> kPathVec;
		Utility::FileSystemHelper::ScanDirectory(path, ".+(\.dll)$", 0, kPathVec);
		for (unsigned int ui = 0; ui < kPathVec.size(); ++ui)
		{
#ifdef WIN32
			USES_CONVERSION;
			HINSTANCE hComponent = ::LoadLibrary(A2W(path));
			if (!hComponent)
			{
				char acBuff[UTILITY_MAX_PATH];
				sprintf_s(acBuff, UTILITY_MAX_PATH, "Call LoadLibrary Error: Cannot load %s.\n", path);
				printf_s(acBuff);
				OutputDebugString(A2W(acBuff));
				continue;
			}
			PFuncGetObjectCreator pFuncGetCreator = 
				(PFuncGetObjectCreator)::GetProcAddress(hComponent,
				"GetDllExportedObjectCreator");
			if (!pFuncGetCreator)
			{
				char acBuff[UTILITY_MAX_PATH];
				sprintf_s(acBuff, UTILITY_MAX_PATH, "Load DLL Error: Can not find the DLL(%s) export function GetDllExportedObjectCreator().\n", path);
				printf_s(acBuff);
				OutputDebugString(A2W(acBuff));
				continue;
			}

#else
			void *pdlHandle = dlopen(path, RTLD_LAZY); 
			char *pszErr = dlerror();
			if( !pdlHandle || pszErr )
			{
				printf("Call LoadLibrary Error: Cannot load %s.\n", path);
				continue;
			}
			PFuncGetObjectCreator pFuncGetCreator = 
				(PFuncGetObjectCreator)dlsym(pdlHandle,
				"GetDllExportedObjectCreator");
			if(!pFuncGetCreator)
			{
				printf("Load DLL Error: Can not find the DLL(%s) export function GetDllExportedObjectCreator().\n", path);
				continue;
			}
#endif // WIN32
			ObjectCreator *pCreator = 0;
			pFuncGetCreator(pCreator);
			while (pCreator)
			{
				unsigned int classID = pCreator->GetClassId();
				std::string className = pCreator->GetClassNameString();
				assert(classID != 0);
				assert(className.size() > 0);
				if (pCreator->GetCompntType() == Object::OT_SERVICE)
				{
					assert(m_kServicesIndexByID.find(classID) == m_kServicesIndexByID.end());
					assert(m_kServicesIndexByName.find(className) == m_kServicesIndexByName.end());
					m_kServicesIndexByID.insert(CreatorContainerByIDValueType(classID, pCreator));
					m_kServicesIndexByName.insert(CreatorContainerByNameValueType(className, pCreator));
				}
				else if (pCreator->GetCompntType() == Object::OT_ENTITY)
				{
					assert(m_kEntitiesIndexByID.find(classID) == m_kEntitiesIndexByID.end());
					assert(m_kEntitiesIndexByName.find(className) == m_kEntitiesIndexByName.end());
					m_kEntitiesIndexByID.insert(CreatorContainerByIDValueType(classID, pCreator));
					m_kEntitiesIndexByName.insert(CreatorContainerByNameValueType(className, pCreator));
				}
				else
				{
					assert(false && "Unrecognized Component Type.");
				}
				pCreator = pCreator->GetNext();
			}
		}
		return true;
	}

	bool ObjectSystem::LoadServiceAndEntityFromLib(ObjectCreator *pkCreatorListHead)
	{
		while (pkCreatorListHead)
		{
			unsigned int classID = pkCreatorListHead->GetClassId();
			std::string className = pkCreatorListHead->GetClassNameString();
			assert(classID != 0);
			assert(className.size() > 0);
			if (pkCreatorListHead->GetCompntType() == Object::OT_SERVICE)
			{
				assert(m_kServicesIndexByID.find(classID) == m_kServicesIndexByID.end());
				assert(m_kServicesIndexByName.find(className) == m_kServicesIndexByName.end());
				m_kServicesIndexByID.insert(CreatorContainerByIDValueType(classID, pkCreatorListHead));
				m_kServicesIndexByName.insert(CreatorContainerByNameValueType(className, pkCreatorListHead));
			}
			else if (pkCreatorListHead->GetCompntType() == Object::OT_ENTITY)
			{
				assert(m_kEntitiesIndexByID.find(classID) == m_kEntitiesIndexByID.end());
				assert(m_kEntitiesIndexByName.find(className) == m_kEntitiesIndexByName.end());
				m_kEntitiesIndexByID.insert(CreatorContainerByIDValueType(classID, pkCreatorListHead));
				m_kEntitiesIndexByName.insert(CreatorContainerByNameValueType(className, pkCreatorListHead));
			}
			else
			{
				assert(false && "Unrecognized Component Type.");
			}
			pkCreatorListHead = pkCreatorListHead->GetNext();
		}
		return true;
	}

	unsigned int ObjectSystem::SubscribeRTMsg(unsigned int uiMessage, unsigned int uiPriority,
		Object* pkObject, MsgFunc pFunc)
	{
		m_kMsgToHandlersMap[uiMessage].insert(PriorityHandlerMap::value_type(uiPriority, MsgHandler(pkObject, pFunc)));
		return m_kMsgToHandlersMap[uiMessage].size();
	}

	unsigned int ObjectSystem::UnsubscribeRTMsg(unsigned int uiMessage,
		Object* pkObject, MsgFunc pFunc)
	{
		PriorityHandlerMap::iterator kIter = m_kMsgToHandlersMap[uiMessage].begin();
		while (kIter != m_kMsgToHandlersMap[uiMessage].end())
		{
			if (kIter->second.mpkObject == pkObject &&
				kIter->second.mpFunc == pFunc)
			{
				m_kMsgToHandlersMap[uiMessage].erase(kIter);
				return m_kMsgToHandlersMap[uiMessage].size();
			}
			++kIter;
		}
		return 0;
	}

	unsigned int ObjectSystem::SubscribeRTMsg(unsigned int uiMessage, unsigned int uiPriority,
		WndowRef *pkReceiver)
	{
		m_kMsgToWindowRefsMap[uiMessage].insert(PriorityWindowRefMap::value_type(uiPriority, pkReceiver));
		return m_kMsgToWindowRefsMap[uiMessage].size();
	}

	unsigned int ObjectSystem::UnsubscribeRTMsg(unsigned int uiMessage, WndowRef *pkReceiver)
	{
		PriorityWindowRefMap::iterator kIter = m_kMsgToWindowRefsMap[uiMessage].begin();
		while (kIter != m_kMsgToWindowRefsMap[uiMessage].end())
		{
			if (kIter->second == pkReceiver)
			{
				m_kMsgToWindowRefsMap[uiMessage].erase(kIter);
				return m_kMsgToWindowRefsMap[uiMessage].size();
			}
			++kIter;
		}
		return 0;
	}

	void ObjectSystem::SendRTMsg(RTMsg &kMsg)
	{
		PriorityHandlerMap::iterator kIterHandler = m_kMsgToHandlersMap[kMsg.uiMessage].end();
		while (kIterHandler != m_kMsgToHandlersMap[kMsg.uiMessage].begin())
		{
			--kIterHandler;
			MsgHandler& kHandler = kIterHandler->second;
			(kHandler.mpkObject->*kHandler.mpFunc)(kMsg);
			if (kMsg.bStop)
			{
				break;
			}
		}

		kMsg.bStop = false;
		PriorityWindowRefMap::iterator kIterWindow = m_kMsgToWindowRefsMap[kMsg.uiMessage].end();
		while (kIterWindow != m_kMsgToWindowRefsMap[kMsg.uiMessage].begin())
		{
			--kIterWindow;
			if (kIterWindow->second)
			{
#ifdef WIN32
				::SendMessage(*((::HWND*)(kIterWindow->second)), kMsg.uiMessage, WPARAM(&kMsg), 0);
#endif // WIN32
				if (kMsg.bStop)
				{
					break;
				}
			}
		}
	}

	void ObjectSystem::Running()
	{
		PriorityObjectMap::iterator kIter = m_kPriorityRunningObjects.end();
		while (kIter != m_kPriorityRunningObjects.begin())
		{
			--kIter;
			kIter->second->Running();
		}
	}
}
