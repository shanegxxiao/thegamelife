#ifndef __SerializableBinary_h__
#define __SerializableBinary_h__

#include "RWMutexStreambuf.hpp"
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>

namespace Utility
{
    template<class Archive, class StreamBuf>
    class SerializableStream
    {
        typedef RWMutexStreambuf::char_type char_type;
        typedef std::size_t size_t;

    public:
        SerializableStream(char_type * pBuffer, std::size_t uSize)
            : buff(pBuffer, uSize)
        {

        }
        SerializableStream(std::size_t uSize)
            : buff(uSize)
        {

        }
        SerializableStream()
        {

        }
        char_type* data()
        {
            return buff.begin();
        }
        size_t capacity()
        {
            return buff.capacity();
        }
        size_t size()
        {
            return buff.size();
        }
        void reset()
        {
            buff.reset();
        }
        template<class T>
        SerializableStream<Archive, StreamBuf>& operator &(T & t)
        {
            Archive a(buff);
            a & t;
            return *this;
        }

    private:
        StreamBuf buff;
    };
    typedef SerializableStream<boost::archive::binary_iarchive, Utility::RWMutexStreambuf> SerializeInStream;
    typedef SerializableStream<boost::archive::binary_oarchive, Utility::RWMutexStreambuf> SerializeOutStream;
}

#endif//__SerializableBinary_h__