#ifndef __Service0_h__
#define __Service0_h__

#include "Runtime/Object.h"

class Service0 : public Runtime::Object
{
public:
	Service0();
	virtual ~Service0();

	virtual bool Initialize(Runtime::ParamList& kParamList);
	virtual void Running();
	virtual bool Shutdown();
	virtual Runtime::Object::ObjectType GetType();

private:
	std::vector<std::string>* mpkShutVector;
};

inline Runtime::Object::ObjectType Service0::GetType()
{
	return Runtime::Object::OT_SERVICE;
}

#endif//__Service0_h__