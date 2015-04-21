#ifndef __SizeHeadedPacketAssembler_hpp__
#define __SizeHeadedPacketAssembler_hpp__

#include "SerializableStream.hpp"
#include "Typedef.h"

namespace Utility
{
    class SizeHeadedPacketAssembler
    {
    public:
        SizeHeadedPacketAssembler(unsigned int uiMAXPacketSize);
        ~SizeHeadedPacketAssembler();
        char* Data();
        unsigned int Size();
        void reset();

        template<class T>
        SizeHeadedPacketAssembler& operator&(T& t)
        {
            mkStream & t;
            mkPacket.setPacketSize((unsigned int)mkStream.size());
            return *this;
        }

    private:
#pragma pack(push)
#pragma pack(1)
        template<class Head>
        struct Packet
        {
            Head* pHead;///< packet head is a variable save the size of a packet so far
            char* pBuff;
            char* pData;
            const unsigned int headSize;
            Packet(unsigned int capacity)
                : headSize(sizeof(Head))
            {
                pBuff = new char[headSize + capacity];
                pHead = (Head*)pBuff;
                pData = pBuff + headSize;
                *pHead = headSize;
            }
            ~Packet()
            {
                delete[] pBuff;
            }
            void setPacketSize(unsigned int dataSize)
            {
                *pHead = headSize + dataSize;
            }
            char* headPtr()
            {
                return pBuff;
            }
            char* packetPtr()
            {
                return pBuff;
            }
            char* dataPtr()
            {
                return pData;
            }
            unsigned int packetSize()///< packet size with head (head size + data size)
            {
                return *pHead;
            }
            unsigned int dataSize()
            {
                return *pHead - headSize;
            }
        };
#pragma pack(pop)
        unsigned int muiMaxPacketSize;
        Packet<Utility::TcpNetworkPacketHead> mkPacket;
        SerializeOutStream mkStream;
    };
}

#endif//__SizeHeadedPacketAssembler_hpp__