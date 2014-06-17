#ifndef __DynCreate_h__
#define __DynCreate_h__

#include "Object.h"
#include "ObjectCreator.h"
#include "ObjectSystem.h"

#define RUNTIME_DECLARE_OBJECT_CREATOR(this_class, parent_class, class_id, class_type) \
class this_class##_ObjectCreator : public Runtime::ObjectCreator \
{ \
public: \
	this_class##_ObjectCreator(Runtime::ObjectCreator*& pNextObjectCreator) : \
	ObjectCreator(#this_class, #parent_class, class_id, class_type, pNextObjectCreator) \
	{ \
		pNextObjectCreator = this; \
	} \
	virtual Runtime::Object* CreateObject() \
	{ \
		return new this_class; \
	} \
};

#define RUNTIME_CREATE_SERVICE(this_class, parent_class, class_id) \
RUNTIME_DECLARE_OBJECT_CREATOR(this_class, parent_class, class_id, Runtime::Object::OT_SERVICE) \
this_class##_ObjectCreator* gp_##this_class##_ObjectCreator;\
this_class##_ObjectCreator& Get_ObjectCreator_##this_class(Runtime::ObjectCreator*& pObjectCreator)\
{\
	static this_class##_ObjectCreator s_creator(pObjectCreator); \
	return s_creator;\
}\
struct this_class##_ObjectCreator_Init \
{ \
	this_class##_ObjectCreator_Init(Runtime::ObjectCreator*& pObjectCreator) \
	{ \
		this_class##_ObjectCreator& k_##this_class##_ObjectCreator = Get_ObjectCreator_##this_class(pObjectCreator); \
		gp_##this_class##_ObjectCreator = &k_##this_class##_ObjectCreator;\
	} \
} this_class##_ObjectCreator_Auto_Init(__ObjectCreator__);

#define SERVICE_INIT_BEFORE(this_class, dep_class) \
struct this_class##_ObjectCreator_Init_Before \
{ \
	this_class##_ObjectCreator_Init_Before() \
	{ \
		gp_##this_class##_ObjectCreator->AddInitDependency(Runtime::DT_Before, #dep_class);\
	} \
} this_class##_ObjectCreator_Auto_Init_Before;

#define SERVICE_INIT_AFTER(this_class, dep_class) \
struct this_class##_ObjectCreator_Init_After \
{ \
	this_class##_ObjectCreator_Init_After() \
	{ \
	gp_##this_class##_ObjectCreator->AddInitDependency(Runtime::DT_After, #dep_class);\
	} \
} this_class##_ObjectCreator_Auto_Init_After;

#define SERVICE_SHUT_BEFORE(this_class, dep_class) \
struct this_class##_ObjectCreator_Shut_Before \
{ \
	this_class##_ObjectCreator_Shut_Before() \
	{ \
	gp_##this_class##_ObjectCreator->AddShutDependency(Runtime::DT_Before, #dep_class);\
	} \
} this_class##_ObjectCreator_Auto_Shut_Before;

#define SERVICE_SHUT_AFTER(this_class, dep_class) \
struct this_class##_ObjectCreator_Shut_After \
{ \
	this_class##_ObjectCreator_Shut_After() \
	{ \
	gp_##this_class##_ObjectCreator->AddShutDependency(Runtime::DT_After, #dep_class);\
	} \
} this_class##_ObjectCreator_Auto_Shut_After;

#define RUNTIME_CREATE_ENTITY(this_class, parent_class, class_id) \
RUNTIME_DECLARE_OBJECT_CREATOR(this_class, parent_class, class_id, Runtime::Object::OT_ENTITY) \
	this_class##_ObjectCreator& Get_ObjectCreator_##this_class(Runtime::ObjectCreator*& pObjectCreator)\
{\
	static this_class##_ObjectCreator s_creator(pObjectCreator); \
	return s_creator;\
}\
struct this_class##_ObjectCreator_Init \
{ \
	this_class##_ObjectCreator_Init(Runtime::ObjectCreator*& pClassCreator) \
	{ \
		Get_ObjectCreator_##this_class(pClassCreator); \
	} \
} this_class##_ObjectCreator_Auto_Init(__ObjectCreator__);

#define RUNTIME_DLL_EXPORTED_FUNCTION \
extern "C" \
{ \
	__declspec(dllexport) void GetDllExportedObjectCreator(Runtime::ObjectCreator*& pObjectCreator) \
	{ \
		pObjectCreator = __ObjectCreator__; \
	} \
} \
Runtime::ObjectCreator *__ObjectCreator__ = NULL;

#define DECLARE_RUNTIME_EXPORTED_MEMBER Runtime::ObjectCreator *__ObjectCreator__ = NULL;
#define EXTERN_RUNTIME_EXPORTED_MEMBER extern Runtime::ObjectCreator *__ObjectCreator__;
#define RUNTIME_EXPORTED_MEMBER __ObjectCreator__

#endif//#define __DynCreate_h__
//END OF FILE