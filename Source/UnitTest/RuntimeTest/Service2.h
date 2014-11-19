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

private:
	std::vector<std::string>* mpkShutVector;
};

#endif//__Service2_h__