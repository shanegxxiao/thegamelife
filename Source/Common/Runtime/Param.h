#ifndef __Param_h__
#define __Param_h__

#include "RuntimeDefine.h"

namespace Runtime
{
	class Param
	{
	public:
		enum Type
		{
			CHAR,
			UCHAR,
			SHORT,
			USHORT,
			INT,
			UINT,
			LONG,
			ULONG,
			FLOAT,
			DOUBLE,
			BOOL,
			VOIDPTR,
			NIL
		};

	public:
		Param();
		Param(char value);
		Param(unsigned char value);
		Param(short value);
		Param(unsigned short value);
		Param(int value);
		Param(unsigned int value);
		Param(long value);
		Param(unsigned long value);
		Param(float value);
		Param(double value);
		Param(bool value);
		Param(void *value);

		~Param();

		Type GetType();

		void SetChar(char value);
		void SetUChar(unsigned char value);
		void SetShort(short value);
		void SetUShort(unsigned short value);
		void SetInt(int value);
		void SetUInt(unsigned int value);
		void SetLong(long value);
		void SetULong(unsigned long value);
		void SetFloat(float value);
		void SetDouble(double value);
		void SetBool(bool value);
		void SetPVoid(void *value);

		char GetChar();
		unsigned char GetUChar();
		short GetShort();
		unsigned short GetUShort();
		int GetInt();
		unsigned int GetUInt();
		long GetLong();
		unsigned long GetULong();
		float GetFloat();
		double GetDouble();
		bool GetBool();
		void *GetPVoid();

	protected:
		union Data
		{
			char m_char;
			unsigned char m_uChar;
			short m_short;
			unsigned short m_uShort;
			int	m_int;
			unsigned int m_uInt;
			long m_long;
			unsigned long m_uLong;
			float m_float;
			double m_double;
			bool m_bool;
			void *m_pvoid;
		};

		Data m_data;
		Type m_type;
	};

	inline Param::Type Param::GetType()
	{
		return m_type;
	}

	inline void Param::SetChar(char value)
	{
		m_type = CHAR;
		m_data.m_char = value;
	}

	inline void Param::SetUChar(unsigned char value)
	{
		m_type = UCHAR;
		m_data.m_uChar = value;
	}

	inline void Param::SetShort(short value)
	{
		m_type = SHORT;
		m_data.m_short = value;
	}

	inline void Param::SetUShort(unsigned short value)
	{
		m_type = USHORT;
		m_data.m_uShort = value;
	}

	inline void Param::SetInt(int value)
	{
		m_type = INT;
		m_data.m_int = value;
	}

	inline void Param::SetUInt(unsigned int value)
	{
		m_type = LONG;
		m_data.m_long = value;
	}

	inline void Param::SetLong(long value)
	{
		m_type = ULONG;
		m_data.m_uLong = value;
	}

	inline void Param::SetULong(unsigned long value)
	{
		m_type = ULONG;
		m_data.m_uLong = value;
	}

	inline void Param::SetFloat(float value)
	{
		m_type = FLOAT;
		m_data.m_float = value;
	}

	inline void Param::SetDouble(double value)
	{
		m_type = DOUBLE;
		m_data.m_double = value;
	}

	inline void Param::SetBool(bool value)
	{
		m_type = BOOL;
		m_data.m_bool = value;
	}

	inline void Param::SetPVoid(void *value)
	{
		m_type = VOIDPTR;
		m_data.m_pvoid = value;
	}

	inline char Param::GetChar()
	{
		if (m_type == CHAR)
		{
			return m_data.m_char;
		}
		assert("This is not a char.");
		return m_data.m_char;
	}

	inline unsigned char Param::GetUChar()
	{
		if (m_type == UCHAR)
		{
			return m_data.m_uChar;
		}
		assert("This is not an unsigned char.");
		return m_data.m_uChar;
	}

	inline short Param::GetShort()
	{
		if (m_type == SHORT)
		{
			return m_data.m_short;
		}
		assert("This is not a short.");
		return m_data.m_short;
	}

	inline unsigned short Param::GetUShort()
	{
		if (m_type == USHORT)
		{
			return m_data.m_uShort;
		}
		assert("This is not an unsigned short.");
		return m_data.m_uShort;
	}

	inline int Param::GetInt()
	{
		if (m_type == INT)
		{
			return m_data.m_int;
		}
		assert("This is not an int.");
		return m_data.m_int;
	}

	inline unsigned int Param::GetUInt()
	{
		if (m_type == UINT)
		{
			return m_data.m_uInt;
		}
		assert("This is not an unsigned int.");
		return m_data.m_uInt;
	}

	inline long Param::GetLong()
	{
		if (m_type == LONG)
		{
			return m_data.m_long;
		}
		assert("This is not a long.");
		return m_data.m_long;
	}

	inline unsigned long Param::GetULong()
	{
		if (m_type == ULONG)
		{
			return m_data.m_uLong;
		}
		assert("This is not an unsigned long.");
		return m_data.m_uLong;
	}

	inline float Param::GetFloat()
	{
		if (m_type == FLOAT)
		{
			return m_data.m_float;
		}
		assert("This is not a float.");
		return m_data.m_float;
	}

	inline double Param::GetDouble()
	{
		if (m_type == DOUBLE)
		{
			return m_data.m_double;
		}
		assert("This is not a double.");
		return m_data.m_double;
	}

	inline bool Param::GetBool()
	{
		if (m_type == BOOL)
		{
			return m_data.m_bool;
		}
		assert("This is not a bool.");
		return m_data.m_bool;
	}

	inline void *Param::GetPVoid()
	{
		if (m_type == VOIDPTR)
		{
			return m_data.m_pvoid;
		}
		assert("This is not a void pointer.");
		return m_data.m_pvoid;
	}
}

#endif // #ifndef __XG_Param_h__
// END OF FILE
