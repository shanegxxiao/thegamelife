#ifndef __RefObject_h__
#define __RefObject_h__

#include "RuntimeDefine.h"

namespace Runtime
{
	class RefObject
	{
	public:
		RefObject(void);
		virtual ~RefObject(void);
		void IncRefCnt();
		void DecRefCnt();
		unsigned int GetRefCnt() const;
		static unsigned int GetTotalObjectCnt();
	protected:
		virtual void DeleteThis();
	private:
		unsigned int m_uiRefCount;
		static unsigned int ms_uiObjects;
	};

	inline RefObject::RefObject(void)
	{
		m_uiRefCount = 0;
		BOOST_INTERLOCKED_INCREMENT((long*)&ms_uiObjects);
	}

	inline RefObject::~RefObject(void)
	{
		BOOST_INTERLOCKED_DECREMENT((long*)&ms_uiObjects);
	}

	inline void RefObject::IncRefCnt()
	{
		BOOST_INTERLOCKED_INCREMENT((long*)&m_uiRefCount);
	}

	inline void RefObject::DecRefCnt()
	{
		BOOST_ASSERT(m_uiRefCount > 0);
		if (BOOST_INTERLOCKED_DECREMENT((long*)&m_uiRefCount) == 0)
		{
			DeleteThis();
		}
	}

	inline unsigned int RefObject::GetRefCnt() const
	{
		return m_uiRefCount;
	}

	inline unsigned int RefObject::GetTotalObjectCnt()
	{
		return ms_uiObjects;
	}
}

#endif//#ifndef __RefObject_h__
//END OF FILE