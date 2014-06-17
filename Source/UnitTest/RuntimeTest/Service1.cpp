#include "UnitTestPCH.h"
#include "Service1.h"

RUNTIME_CREATE_SERVICE(Service1, Runtime::Object, 0)
SERVICE_INIT_AFTER(Service1, Service0)

SERVICE_SHUT_AFTER(Service1, Service0)
SERVICE_SHUT_BEFORE(Service1, Service2)
Service1::Service1()
{

}
Service1::~Service1()
{

}
bool Service1::Initialize(Runtime::ParamList& kParamList)
{
	std::vector<std::string> *pkInitVector = 0;
	void* pInitVector = 0;
	kParamList.GetParam("InitVector", pInitVector);
	pkInitVector = (std::vector<std::string> *)pInitVector;
	pkInitVector->push_back("Service1::Initialize");

	void* pShutVector = 0;
	kParamList.GetParam("ShutVector", pShutVector);
	mpkShutVector = (std::vector<std::string>*)pShutVector;

	return true;
}
void Service1::Running()
{

}
bool Service1::Shutdown()
{
	mpkShutVector->push_back("Service1::Shutdown");
	return true;
}