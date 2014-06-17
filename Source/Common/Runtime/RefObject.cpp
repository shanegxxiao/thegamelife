#include "RuntimePCH.h"
#include "RefObject.h"

namespace Runtime
{
	unsigned int RefObject::ms_uiObjects = 0;

	void RefObject::DeleteThis()
	{
		delete this;
	}
}