#include "NetworkPCH.h"
#include "ConnectionIDGenerator.h"

namespace Network
{
	ConnectionID ConnectionIDGenerator::m_ulIDSeed = 0;
    const ConnectionID INVALID_CONNECTION_ID = Utility::INVALID_CONNECTION_ID;
}