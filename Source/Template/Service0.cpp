#include "TemplatePCH.h"
#include "Service0.h"

RUNTIME_CREATE_SERVICE(Service0, Runtime::Object, 0)
SERVICE_INIT_AFTER(Service0, Service2)
SERVICE_INIT_BEFORE(Service0, Service1)

SERVICE_SHUT_BEFORE(Service0, Service1)
Service0::Service0()
{

}
Service0::~Service0()
{

}
bool Service0::Initialize(Runtime::ParamList& kParamList)
{
	std::vector<std::string> *pkInitVector = 0;
	void* pInitVector = 0;
	kParamList.GetParam("InitVector", pInitVector);
	pkInitVector = (std::vector<std::string> *)pInitVector;
	pkInitVector->push_back("Service0::Initialize");

	void* pShutVector = 0;
	kParamList.GetParam("ShutVector", pShutVector);
	mpkShutVector = (std::vector<std::string>*)pShutVector;

	return true;
}
void Service0::Running()
{

}
bool Service0::Shutdown()
{
	mpkShutVector->push_back("Service0::Shutdown");
	return true;
}