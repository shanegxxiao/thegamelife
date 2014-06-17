#ifndef __Service2_h__
#define __Service2_h__

#include "Runtime/Object.h"

class Service2 : public Runtime::Object
{
public:
	Service2();
	virtual ~Service2();

	virtual bool Initialize(Runtime::ParamList& kParamList);
	virtual void Running();
	virtual bool Shutdown();
	virtual Runtime::Object::ObjectType GetType();

private:
	std::vector<std::string>* mpkShutVector;
};

inline Runtime::Object::ObjectType Service2::GetType()
{
	return Runtime::Object::OT_SERVICE;
}

#endif//__Service2_h__