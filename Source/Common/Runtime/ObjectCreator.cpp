#include "RuntimePCH.h"
#include "ObjectCreator.h"
#include "Utility/UtilityDefine.h"
#ifdef WIN32
#include <Windows.h>
#include <atlconv.h>
#endif // WIN32
namespace Runtime
{
	unsigned long LHashChars(const char *c) 
	{ 
		unsigned long ret=0; 
		long n; 
		unsigned long v; 
		int r; 

		if ((c == NULL) || (*c == '\0')) 
			return(ret); 
		n = 0x100; 
		while (*c) 
		{ 
			v = n | (*c); 
			n += 0x100; 
			r = (int)((v >> 2) ^v) & 0x0f; 
			ret = (ret << r) | (ret >> (32 - r)); 
			ret &= 0xFFFFFFFFL; 
			ret ^= v * v; 
			++c; 
		} 
		return((ret>>16)^ret); 
	} 

	ObjectCreator::ObjectCreator(std::string strClassName,
		std::string strParentName, unsigned int uiClassID,
		Object::ObjectType eObjectType, ObjectCreator* pNextClassCreator)
		: m_pkInitDependency(0), m_pkShutDependency(0)
	{
		m_strClassName = strClassName;
		m_strParentName = strParentName;
		m_uiClassID = (uiClassID == 0) ? LHashChars(strClassName.c_str()) : uiClassID;
		m_eObjectType = eObjectType;
		m_pkNextObjectCreator = pNextClassCreator;
	}

	ObjectCreator::~ObjectCreator()
	{
		while (m_pkInitDependency)
		{
			Dependency* pkDep = m_pkInitDependency;
			m_pkInitDependency = pkDep->mpNext;
			delete pkDep;
		}
		while (m_pkShutDependency)
		{
			Dependency* pkDep = m_pkShutDependency;
			m_pkShutDependency = pkDep->mpNext;
			delete pkDep;
		}
	}

	Object *ObjectCreator::CreateObject()
	{
		return NULL;
	}

	ObjectCreator* ObjectCreator::GetNext()
	{
		return m_pkNextObjectCreator;
	}

	std::string ObjectCreator::GetClassNameString()
	{
		return m_strClassName;
	}

	Object::ObjectType ObjectCreator::GetCompntType()
	{
		return m_eObjectType;
	}

	unsigned int ObjectCreator::GetClassId()
	{
		return m_uiClassID;
	}

	void ObjectCreator::AddInitDependency(DependencyType type, const char* objectName)
	{
		if (m_pkInitDependency)
		{
			Dependency *pkDep = new Dependency(type, objectName);
			pkDep->mpNext = m_pkInitDependency;
			m_pkInitDependency = pkDep;
		}
		else
		{
			m_pkInitDependency = new Dependency(type, objectName);
			m_pkInitDependency->mpNext = 0;
		}
	}

	void ObjectCreator::AddShutDependency(DependencyType type, const char* objectName)
	{
		if (m_pkShutDependency)
		{
			Dependency *pkDep = new Dependency(type, objectName);
			pkDep->mpNext = m_pkShutDependency;
			m_pkShutDependency = pkDep;
		}
		else
		{
			m_pkShutDependency = new Dependency(type, objectName);
			m_pkShutDependency->mpNext = 0;
		}
	}

	Dependency *ObjectCreator::GetInitDependency()
	{
		return m_pkInitDependency;
	}

	Dependency *ObjectCreator::GetShutDependency()
	{
		return m_pkShutDependency;
	}

	Dependency *ObjectCreator::GetDependency(DependencyMode eMode)
	{
		switch (eMode)
		{
		case DM_Init:
			return GetInitDependency();
		case DM_Shut:
			return GetShutDependency();
		default:
			return 0;
		}
		return 0;
	}

	bool ObjectCreator::ConstrainedToComeBefore(ObjectCreator* pCreator, DependencyMode eMode)
	{
		if(pCreator == this)
		{
			return false;
		}

		for(Dependency* pDependency = GetDependency(eMode);
			pDependency != NULL; pDependency = pDependency->mpNext )
		{
			ObjectCreator* pDepCreator = pDependency->mpCreator;
			if(!pDepCreator)
			{
				pDepCreator = ObjectSystem::GetInstancePtr()->GetServiceCreator(pDependency->mstrObjectName);
				if(!pDepCreator)
				{
					// Module does not exist.  Only emit a warning here so that modules
					// can be omitted from a link without requiring the module definitions
					// to be adapted.
					printf("[ObjectCreator] Object %s of '%s' depends on object '%s' which does not exist",
						eMode == DM_Init ? "init" : "shutdown",
						pCreator->GetClassNameString().c_str(), pDependency->mstrObjectName.c_str());
					continue;
				}

				pDependency->mpCreator = pDepCreator;
			}

			if(pDependency->meType == DT_Before)
			{
				if(pDepCreator == pCreator
					|| pDepCreator->ConstrainedToComeBefore(pCreator, eMode))
				{
					return true;
				}
			}
		}

		return false;
	}
	bool ObjectCreator::ConstrainedToComeAfter(ObjectCreator* pCreator, DependencyMode eMode)
	{
		if(pCreator == this)
		{
			return false;
		}

		for(Dependency* pDependency = GetDependency(eMode);
			pDependency != NULL; pDependency = pDependency->mpNext )
		{
			ObjectCreator* pDepCreator = pDependency->mpCreator;
			if(!pDepCreator)
			{
				pDepCreator = ObjectSystem::GetInstancePtr()->GetServiceCreator(pDependency->mstrObjectName);
				if(!pDepCreator)
				{
					// Module does not exist.  Only emit a warning here so that modules
					// can be omitted from a link without requiring the module definitions
					// to be adapted.
					printf("[ObjectCreator] Object %s of '%s' depends on object '%s' which does not exist",
						eMode == DM_Init ? "init" : "shutdown",
						pCreator->GetClassNameString().c_str(), pDependency->mstrObjectName.c_str());
					continue;
				}

				pDependency->mpCreator = pDepCreator;
			}

			if(pDependency->meType == DT_After)
			{
				if(pDepCreator == pCreator
					|| pDepCreator->ConstrainedToComeAfter(pCreator, eMode))
				{
					return true;
				}
			}
		}
		return false;
	}

}
