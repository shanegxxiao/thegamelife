#ifndef __typedef_h__
#define __typedef_h__

namespace Utility
{
    extern const unsigned int RECOMMENDED_MAX_PACKET_SIZE;
    extern const unsigned long long INVALID_CONNECTION_ID;
    typedef unsigned long long RequestID;
    typedef unsigned long long ConnectionID;
    typedef unsigned int TcpNetworkPacketHead;
    char* PacketDataPtr(const unsigned char* pPacket);
    unsigned int PacketDataSize(const unsigned int packetSize);
}

#endif//__typedef_h__