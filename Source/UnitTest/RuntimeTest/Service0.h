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

private:
	std::vector<std::string>* mpkShutVector;
};

#endif//__Service0_h__