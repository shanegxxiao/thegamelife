#include "UnitTestPCH.h"
#include "Service2.h"

RUNTIME_CREATE_SERVICE(Service2, Runtime::Object, 0)
SERVICE_INIT_BEFORE(Service2, Service0)
SERVICE_SHUT_AFTER(Service2, Service1)
Service2::Service2()
{

}
Service2::~Service2()
{

}
bool Service2::Initialize(Runtime::ParamList& kParamList)
{
	std::vector<std::string> *pkInitVector = 0;
	void* pInitVector = 0;
	kParamList.GetParam("InitVector", pInitVector);
	pkInitVector = (std::vector<std::string> *)pInitVector;
	pkInitVector->push_back("Service2::Initialize");

	void* pShutVector = 0;
	kParamList.GetParam("ShutVector", pShutVector);
	mpkShutVector = (std::vector<std::string>*)pShutVector;
	
	return true;
}
void Service2::Running()
{

}
bool Service2::Shutdown()
{
	mpkShutVector->push_back("Service2::Shutdown");
	return true;
}