#include "UtilityPCH.h"
#include "Typedef.h"

namespace Utility
{
    const unsigned int RECOMMENDED_MAX_PACKET_SIZE = 8 * 1024;
    const unsigned long long INVALID_CONNECTION_ID = 0xFFFFFFFFFFFFFFFF;
    char* PacketDataPtr(const unsigned char* pPacket)
    {
        return (char*)(pPacket + sizeof(TcpNetworkPacketHead));
    }
    unsigned int PacketDataSize(const unsigned int packetSize)
    {
        return packetSize - sizeof(TcpNetworkPacketHead);
    }
}