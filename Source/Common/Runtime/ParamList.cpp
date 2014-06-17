#include "RuntimePCH.h"
#include "ParamList.h"

namespace Runtime
{
	ParamList::ParamList(void)
	{

	}

	ParamList::~ParamList(void)
	{
		ParamListIter kIter = m_kParamList.begin();
		while (kIter != m_kParamList.end())
		{
			delete kIter->second;
			++kIter;
		}
		m_kParamList.clear();
	}
}