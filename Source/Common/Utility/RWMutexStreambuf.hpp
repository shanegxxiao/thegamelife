#ifndef __RWMutexStreamBuff_hpp__
#define __RWMutexStreamBuff_hpp__

#include "SizeFixedStreambuf.hpp"

/// read write mutex stream buffer

#ifdef _MSC_VER
# pragma warning(push)
# pragma warning(disable:4996)
#endif //_MSC_VER

namespace Utility
{
    class RWMutexStreambuf
        : public SizeFixedStreamBuf
    {
    public:
        RWMutexStreambuf(char_type * pBuffer, std::size_t uSize)
            : SizeFixedStreamBuf(pBuffer, uSize)
        {
            reset();
        }

        RWMutexStreambuf(std::size_t uSize)
            : SizeFixedStreamBuf(uSize)
        {
            reset();
        }

#if defined(_MSC_VER) && (_MSC_VER > 1500)
        void swap(SizeFixedStreamBuf & rhs)
        {
            SizeFixedStreamBuf::swap(rhs);
        }
#endif

        std::size_t capacity() const
        {
            return SizeFixedStreamBuf::capacity();
        }

        std::size_t size() const
        {
            return (ptr() - begin());
        }

        void reset()
        {
            reset(begin());
        }

        void reset(char_type * pos)
        {
            setp(pos, end());
            setg(pos, pos, end());
        }

        inline char_type * ptr() const
        {
            BOOST_ASSERT(SizeFixedStreamBuf::gptr() == SizeFixedStreamBuf::gptr());
            return SizeFixedStreamBuf::gptr();
        }

        inline char_type * begin() const
        {
            return SizeFixedStreamBuf::begin();
        }

        inline char_type * end() const
        {
            return SizeFixedStreamBuf::end();
        }

        inline std::size_t pos() const
        {
            return (ptr() - begin());
        }

        inline bool bump(size_t off)
        {
            return (pubseekoff(off, std::ios_base::cur) != __BADOFF);
        }

        virtual pos_type seekpos(off_type off
            , ::std::ios_base::openmode which = ::std::ios_base::in | ::std::ios_base::out)
        {
            return seekoff(off, ::std::ios_base::beg, which);
        }

        virtual pos_type seekoff(off_type off
            , ::std::ios_base::seekdir way, ::std::ios_base::openmode )
        {
            if (way == std::ios_base::cur)
                off += (ptr() - begin());
            else if (way == std::ios_base::end)
                off += (end() - begin());

            if (off >= 0 && off <= (off_type)capacity())
            {
                reset(offset(off));
                return (pos_type)off;
            }

            return __BADOFF;
        }

    protected:
        virtual std::streamsize xsputn(const char_type *pBuffer, std::streamsize iSize)
        {
            if (!pBuffer || iSize <= 0) return (std::streamsize)0;
            std::streamsize spaceSize = (end() - ptr());
            if (iSize > spaceSize) return (std::streamsize)0;
#ifdef WIN32
            memcpy_s(ptr(), spaceSize, pBuffer, (std::size_t)iSize);
#else
            memcpy(ptr(), pBuffer, (std::size_t)iSize);
#endif//WIN32
            bump((std::size_t)iSize);
            return (std::streamsize)iSize;
        }

        virtual std::streamsize xsgetn(char_type * pBuffer, std::streamsize iSize)
        {
            if (!pBuffer || iSize <= 0) return (std::streamsize)0;
            std::streamsize spaceSize = (end() - ptr());
            if (iSize > spaceSize) return (std::streamsize)0;
#ifdef WIN32
            memcpy_s(pBuffer, (std::size_t)iSize, ptr(), (std::size_t)iSize);
#else
            memcpy(pBuffer, ptr(), (std::size_t)iSize);
#endif//WIN32
            bump((std::size_t)iSize);
            return (std::streamsize)iSize;
        }

    private:
        // remove base_type public function
        using SizeFixedStreamBuf::pubsetbuf;
        using SizeFixedStreamBuf::pubimbue;
        using SizeFixedStreamBuf::getloc;
        using SizeFixedStreamBuf::in_avail;
        using SizeFixedStreamBuf::pubsync;
        using SizeFixedStreamBuf::sbumpc;
        using SizeFixedStreamBuf::sgetc;
        using SizeFixedStreamBuf::snextc;
        using SizeFixedStreamBuf::sputbackc;
        using SizeFixedStreamBuf::stossc;
        using SizeFixedStreamBuf::sungetc;
        using SizeFixedStreamBuf::sputc;
#ifdef WIN32
        using SizeFixedStreamBuf::_Lock;
        using SizeFixedStreamBuf::_Unlock;
#endif // WIN32

    };

} //namespace Utility

#ifdef _MSC_VER
# pragma warning(pop)
#endif //_MSC_VER

#endif //__RWMutexStreamBuff_h__