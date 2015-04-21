#ifndef __ConnectionIDGenerator_h__
#define __ConnectionIDGenerator_h__

#include "Utility/Typedef.h"

namespace Network
{
	typedef Utility::ConnectionID ConnectionID;
    extern const ConnectionID INVALID_CONNECTION_ID;

	class ConnectionIDGenerator
	{
	public:
		static ConnectionID NewConnectionID();

	private:
		static ConnectionID m_ulIDSeed;
	};

	inline ConnectionID ConnectionIDGenerator::NewConnectionID()
	{
		return m_ulIDSeed++;
	}
}

#endif//#ifndef __ConnectionIDGenerator_h__