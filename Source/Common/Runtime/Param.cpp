#include "RuntimePCH.h"
#include "Param.h"

namespace Runtime
{
	Param::Param()
	{
		m_type = NIL;
	}

	Param::Param(char value)
	{
		m_type = CHAR;
		m_data.m_char = value;
	}

	Param::Param(unsigned char value)
	{
		m_type = UCHAR;
		m_data.m_uChar = value;
	}

	Param::Param(short value)
	{
		m_type = SHORT;
		m_data.m_short = value;
	}

	Param::Param(unsigned short value)
	{
		m_type = USHORT;
		m_data.m_uShort = value;
	}

	Param::Param(int value)
	{
		m_type = INT;
		m_data.m_int = value;
	}

	Param::Param(unsigned int value)
	{
		m_type = UINT;
		m_data.m_uInt = value;
	}

	Param::Param(long value)
	{
		m_type = LONG;
		m_data.m_long = value;
	}

	Param::Param(unsigned long value)
	{
		m_type = ULONG;
		m_data.m_uLong = value;
	}

	Param::Param(float value)
	{
		m_type = FLOAT;
		m_data.m_float = value;
	}

	Param::Param(double value)
	{
		m_type = DOUBLE;
		m_data.m_double = value;
	}

	Param::Param(bool value)
	{
		m_type = BOOL;
		m_data.m_bool = value;
	}

	Param::Param(void *value)
	{
		m_type = VOIDPTR;
		m_data.m_pvoid = value;
	}

	Param::~Param()
	{

	}
}