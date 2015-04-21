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
bool Service2::Initialize(boost::property_tree::ptree& kParamList)
{
    std::vector<std::string> *pkInitVector = 0;
    pkInitVector = (std::vector<std::string>*)kParamList.get<void*>("InitVector");
    pkInitVector->push_back("Service0::Initialize");

    mpkShutVector = (std::vector<std::string>*)kParamList.get<void*>("ShutVector");
	
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