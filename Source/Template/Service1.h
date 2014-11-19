#ifndef __Service1_h__
#define __Service1_h__

#include "Runtime/Object.h"

class Service1 : public Runtime::Object
{
public:
	Service1();
	virtual ~Service1();

	virtual bool Initialize(Runtime::ParamList& kParamList);
	virtual void Running();
	virtual bool Shutdown();

private:
	std::vector<std::string>* mpkShutVector;
};

#endif//__Service1_h__