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
bool Service1::Initialize(boost::property_tree::ptree& kParamList)
{
    std::vector<std::string> *pkInitVector = 0;
    pkInitVector = (std::vector<std::string>*)kParamList.get<void*>("InitVector");
    pkInitVector->push_back("Service0::Initialize");

    mpkShutVector = (std::vector<std::string>*)kParamList.get<void*>("ShutVector");

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