#ifndef __ConnectionIDGenerator_h__
#define __ConnectionIDGenerator_h__

#define INVALID_CONNECTION_ID 0xFFFFFFFFFFFFFFFF

namespace Network
{
	typedef unsigned long long ConnectionID;

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