#ifndef __ParamList_h__
#define __ParamList_h__

#include "Param.h"
#include "RuntimeDefine.h"

namespace Runtime
{
	class ParamList
	{
	public:
		ParamList(void);
		~ParamList(void);

		bool AddParam(std::string name, char value);
		bool AddParam(std::string name, unsigned char value);
		bool AddParam(std::string name, short value);
		bool AddParam(std::string name, unsigned short value);
		bool AddParam(std::string name, int value);
		bool AddParam(std::string name, unsigned int value);
		bool AddParam(std::string name, long value);
		bool AddParam(std::string name, unsigned long value);
		bool AddParam(std::string name, float value);
		bool AddParam(std::string name, double value);
		bool AddParam(std::string name, bool value);
		bool AddParam(std::string name, void *value);

		bool GetParam(std::string name, char& value);
		bool GetParam(std::string name, unsigned char& value);
		bool GetParam(std::string name, short& value);
		bool GetParam(std::string name, unsigned short& value);
		bool GetParam(std::string name, int& value);
		bool GetParam(std::string name, unsigned int& value);
		bool GetParam(std::string name, long& value);
		bool GetParam(std::string name, unsigned long& value);
		bool GetParam(std::string name, float& value);
		bool GetParam(std::string name, double& value);
		bool GetParam(std::string name, bool& value);
		bool GetParam(std::string name, void *&value);

	private:
		typedef std::map<std::string, Param*> ParamMap;
		typedef ParamMap::iterator	ParamListIter;
		typedef ParamMap::value_type Parameter;

		ParamMap m_kParamList;
	};

	inline bool ParamList::AddParam(std::string name, char value)
	{
		if ((m_kParamList.insert(Parameter(name, new Param(value)))).second)
		{
			return true;
		}
		return false;
	}

	inline bool ParamList::AddParam(std::string name, unsigned char value)
	{
		if ((m_kParamList.insert(Parameter(name, new Param(value)))).second)
		{
			return true;
		}
		return false;
	}

	inline bool ParamList::AddParam(std::string name, short value)
	{
		if ((m_kParamList.insert(Parameter(name, new Param(value)))).second)
		{
			return true;
		}
		return false;
	}

	inline bool ParamList::AddParam(std::string name, unsigned short value)
	{
		if ((m_kParamList.insert(Parameter(name, new Param(value)))).second)
		{
			return true;
		}
		return false;
	}

	inline bool ParamList::AddParam(std::string name, int value)
	{
		if ((m_kParamList.insert(Parameter(name, new Param(value)))).second)
		{
			return true;
		}
		return false;
	}

	inline bool ParamList::AddParam(std::string name, unsigned int value)
	{
		if ((m_kParamList.insert(Parameter(name, new Param(value)))).second)
		{
			return true;
		}
		return false;
	}

	inline bool ParamList::AddParam(std::string name, long value)
	{
		if ((m_kParamList.insert(Parameter(name, new Param(value)))).second)
		{
			return true;
		}
		return false;
	}

	inline bool ParamList::AddParam(std::string name, unsigned long value)
	{
		if ((m_kParamList.insert(Parameter(name, new Param(value)))).second)
		{
			return true;
		}
		return false;
	}

	inline bool ParamList::AddParam(std::string name, float value)
	{
		if ((m_kParamList.insert(Parameter(name, new Param(value)))).second)
		{
			return true;
		}
		return false;
	}

	inline bool ParamList::AddParam(std::string name, double value)
	{
		if ((m_kParamList.insert(Parameter(name, new Param(value)))).second)
		{
			return true;
		}
		return false;
	}

	inline bool ParamList::AddParam(std::string name, bool value)
	{
		if ((m_kParamList.insert(Parameter(name, new Param(value)))).second)
		{
			return true;
		}
		return false;
	}

	inline bool ParamList::AddParam(std::string name, void *value)
	{
		if ((m_kParamList.insert(Parameter(name, new Param(value)))).second)
		{
			return true;
		}
		return false;
	}

	inline bool ParamList::GetParam(std::string name, char& value)
	{
		ParamListIter iter = m_kParamList.find(name);
		if (iter != m_kParamList.end() && (iter->second)->GetType() == Param::CHAR)
		{
			value = (iter->second)->GetChar();
			return true;
		}
		return false;
	}

	inline bool ParamList::GetParam(std::string name, unsigned char& value)
	{
		ParamListIter iter = m_kParamList.find(name);
		if (iter != m_kParamList.end() && (iter->second)->GetType() == Param::UCHAR)
		{
			value = (iter->second)->GetUChar();
			return true;
		}
		return false;
	}

	inline bool ParamList::GetParam(std::string name, short& value)
	{
		ParamListIter iter = m_kParamList.find(name);
		if (iter != m_kParamList.end() && (iter->second)->GetType() == Param::SHORT)
		{
			value = (iter->second)->GetShort();
			return true;
		}
		return false;
	}

	inline bool ParamList::GetParam(std::string name, unsigned short& value)
	{
		ParamListIter iter = m_kParamList.find(name);
		if (iter != m_kParamList.end() && (iter->second)->GetType() == Param::USHORT)
		{
			value = (iter->second)->GetUShort();
			return true;
		}
		return false;
	}

	inline bool ParamList::GetParam(std::string name, int& value)
	{
		ParamListIter iter = m_kParamList.find(name);
		if (iter != m_kParamList.end() && (iter->second)->GetType() == Param::INT)
		{
			value = (iter->second)->GetInt();
			return true;
		}
		return false;
	}

	inline bool ParamList::GetParam(std::string name, unsigned int& value)
	{
		ParamListIter iter = m_kParamList.find(name);
		if (iter != m_kParamList.end() && (iter->second)->GetType() == Param::UINT)
		{
			value = (iter->second)->GetUInt();
			return true;
		}
		return false;
	}

	inline bool ParamList::GetParam(std::string name, long& value)
	{
		ParamListIter iter = m_kParamList.find(name);
		if (iter != m_kParamList.end() && (iter->second)->GetType() == Param::LONG)
		{
			value = (iter->second)->GetLong();
			return true;
		}
		return false;
	}

	inline bool ParamList::GetParam(std::string name, unsigned long& value)
	{
		ParamListIter iter = m_kParamList.find(name);
		if (iter != m_kParamList.end() && (iter->second)->GetType() == Param::ULONG)
		{
			value = (iter->second)->GetLong();
			return true;
		}
		return false;
	}

	inline bool ParamList::GetParam(std::string name, float& value)
	{
		ParamListIter iter = m_kParamList.find(name);
		if (iter != m_kParamList.end() && (iter->second)->GetType() == Param::FLOAT)
		{
			value = (iter->second)->GetFloat();
			return true;
		}
		return false;
	}

	inline bool ParamList::GetParam(std::string name, double& value)
	{
		ParamListIter iter = m_kParamList.find(name);
		if (iter != m_kParamList.end() && (iter->second)->GetType() == Param::DOUBLE)
		{
			value = (iter->second)->GetDouble();
			return true;
		}
		return false;
	}

	inline bool ParamList::GetParam(std::string name, bool& value)
	{
		ParamListIter iter = m_kParamList.find(name);
		if (iter != m_kParamList.end() && (iter->second)->GetType() == Param::BOOL)
		{
			value = (iter->second)->GetBool();
			return true;
		}
		return false;
	}

	inline bool ParamList::GetParam(std::string name, void *&value)
	{
		ParamListIter iter = m_kParamList.find(name);
		if (iter != m_kParamList.end() && (iter->second)->GetType() == Param::VOIDPTR)
		{
			value = (iter->second)->GetPVoid();
			return true;
		}
		return false;
	}
}
#endif//#ifndef __ParamList_h__
//END OF FILE