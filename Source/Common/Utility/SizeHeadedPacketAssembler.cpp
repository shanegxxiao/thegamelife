#include "UtilityPCH.h"
#include "SizeHeadedPacketAssembler.h"

namespace Utility
{
    SizeHeadedPacketAssembler::SizeHeadedPacketAssembler(unsigned int uiMAXPacketSize)
        : muiMaxPacketSize(uiMAXPacketSize)
        , mkPacket(muiMaxPacketSize)
        , mkStream(mkPacket.dataPtr(), muiMaxPacketSize)
    {
    }
    SizeHeadedPacketAssembler::~SizeHeadedPacketAssembler()
    {

    }
    char* SizeHeadedPacketAssembler::Data()
    {
        return mkPacket.packetPtr();
    }
    unsigned int SizeHeadedPacketAssembler::Size()
    {
        return mkPacket.packetSize();
    }
    void SizeHeadedPacketAssembler::reset()
    {
        mkStream.reset();
    }
}
