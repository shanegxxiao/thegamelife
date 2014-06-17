#ifndef __TestMain_h__
#define __TestMain_h__

namespace Utility
{
	class MemoryLeakChecker;
}

class TestMain
{
public:
	TestMain();
	~TestMain();

private:
	Utility::MemoryLeakChecker* mpkMemoryLeakChecker;
};

extern bool gbKeepRunningFlag;

#endif//__TestMain_h__