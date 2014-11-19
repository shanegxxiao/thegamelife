#include "TemplatePCH.h"
#include "Runtime/Runtime.h"

int main(int argc, char* argv[])
{
	Runtime::ParamList kParamList;

	std::vector<std::string> kInitVector;
	std::vector<std::string> kShutVector;
	void* pInitVector = (void*)&kInitVector;
	kParamList.AddParam("InitVector", pInitVector);
	void* pShutVector = (void*)&kShutVector;
	kParamList.AddParam("ShutVector", pShutVector);
	
	std::string strWorkDir = Utility::FileSystemHelper::GetModulePath();
	void* pVoidTmp = (void*)strWorkDir.c_str();
	kParamList.AddParam("acWorkDir", pVoidTmp);
	kParamList.AddParam("acDllDir", pVoidTmp);
	kParamList.AddParam("acResourceDir", pVoidTmp);
	kParamList.AddParam("acConfigDir", pVoidTmp);
	kParamList.AddParam("gpObjectCreator", (void*)RUNTIME_EXPORTED_MEMBER);

	gpkObjectSystem->Initialize(kParamList);
	gpkObjectSystem->Running();
	gpkObjectSystem->Shutdown();

	std::cout << "initialized services list:" << std::endl;
	for (unsigned int ui = 0; ui < kInitVector.size(); ++ui)
	{
		std::cout << kInitVector[ui] << std::endl;
	}

	std::cout << "shutdown services list:" << std::endl;
	for (unsigned int ui = 0; ui < kShutVector.size(); ++ui)
	{
		std::cout << kShutVector[ui] << std::endl;
	}

	system("pause");
}

