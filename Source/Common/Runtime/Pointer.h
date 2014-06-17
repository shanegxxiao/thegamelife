#ifndef __Pointer_h__
#define __Pointer_h__

template <class T> class Pointer
{
public:
	// construction and destruction
	Pointer(T* pObject = (T*) 0);
	Pointer(const Pointer& ptr);
	~Pointer();

	// implicit conversions
	operator T*() const;
	T& operator*() const;
	T* operator->() const;

	// assignment
	Pointer& operator=(const Pointer& ptr);
	Pointer& operator=(T* pObject);

	// comparisons
	bool operator==(T* pObject) const;
	bool operator!=(T* pObject) const;
	bool operator==(const Pointer& ptr) const;
	bool operator!=(const Pointer& ptr) const;

protected:
	// the managed pointer
	T* m_pObject;
};

#define SmartPointer(classname) \
class classname; \
	typedef Pointer<classname> classname##Ptr

// Use for casting a smart pointer of one type to a pointer or smart pointer
// of another type.
#define SmartPointerCast(type, smartptr) ((type*) (void*) (smartptr))


template <class T>
inline Pointer<T>::Pointer(T* pObject)
{
	m_pObject = pObject;
	if (m_pObject)
		m_pObject->IncRefCnt();
}

template <class T>
inline Pointer<T>::Pointer(const Pointer& ptr)
{
	m_pObject = ptr.m_pObject;
	if (m_pObject)
		m_pObject->IncRefCnt();
}

template <class T>
inline Pointer<T>::~Pointer()
{
	if (m_pObject)
		m_pObject->DecRefCnt();
}

template <class T>
inline Pointer<T>::operator T*() const
{
	return m_pObject;
}

template <class T>
inline T& Pointer<T>::operator*() const
{
	return *m_pObject;
}

template <class T>
inline T* Pointer<T>::operator->() const
{
	return m_pObject;
}

template <class T>
inline Pointer<T>& Pointer<T>::operator=(const Pointer& ptr)
{
	if (m_pObject != ptr.m_pObject)
	{
		if (m_pObject)
			m_pObject->DecRefCnt();
		m_pObject = ptr.m_pObject;
		if (m_pObject)
			m_pObject->IncRefCnt();
	}
	return *this;
}

template <class T>
inline Pointer<T>& Pointer<T>::operator=(T* pObject)
{
	if (m_pObject != pObject)
	{
		if (m_pObject)
			m_pObject->DecRefCnt();
		m_pObject = pObject;
		if (m_pObject)
			m_pObject->IncRefCnt();
	}
	return *this;
}

template <class T>
inline bool Pointer<T>::operator==(T* pObject) const
{
	return (m_pObject == pObject);
}

template <class T>
inline bool Pointer<T>::operator!=(T* pObject) const
{
	return (m_pObject != pObject);
}

template <class T>
inline bool Pointer<T>::operator==(const Pointer& ptr) const
{
	return (m_pObject == ptr.m_pObject);
}

template <class T>
inline bool Pointer<T>::operator!=(const Pointer& ptr) const
{
	return (m_pObject != ptr.m_pObject);
}

#endif//#ifndef __Pointer_h__
//END OF FILE
