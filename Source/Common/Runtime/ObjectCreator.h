#ifndef __ObjectCreator_h__
#define __ObjectCreator_h__

#include "Object.h"
#include "RuntimeDefine.h"

namespace Runtime
{
	class ObjectCreator
	{
	public:
		ObjectCreator(std::string strClassName, std::string strParentName, 
			unsigned int uiClassID, Object::ObjectType eObjectType, 
			ObjectCreator *pNextClassCreator);
		virtual ~ObjectCreator();
		virtual Object *CreateObject();
		virtual ObjectCreator *GetNext();
		virtual std::string GetClassNameString();
		virtual Object::ObjectType GetCompntType();
		virtual unsigned int GetClassId();

	public:
		void AddInitDependency(DependencyType type, const char* objectName);
		void AddShutDependency(DependencyType type, const char* objectName);
		Dependency *GetInitDependency();
		Dependency *GetShutDependency();
		Dependency *GetDependency(DependencyMode eMode);
		bool ConstrainedToComeBefore(ObjectCreator* pCreator, DependencyMode eMode);
		bool ConstrainedToComeAfter(ObjectCreator* pCreator, DependencyMode eMode);

	protected:
		std::string m_strClassName;
		std::string m_strParentName;
		Object::ObjectType m_eObjectType;
		unsigned int m_uiClassID;
		/// Point to the ObjectCreator of the next class that be exporting in the same DLL.
		ObjectCreator *m_pkNextObjectCreator;
		/// Init dependency
		Dependency *m_pkInitDependency;
		/// Shut dependency
		Dependency *m_pkShutDependency;
	};
}
#endif // #ifndef __ObjectCreator_h__
// END OF FILE