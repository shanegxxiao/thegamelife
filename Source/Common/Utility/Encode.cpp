#include "UtilityPCH.h"
#include "Encode.h"
#include <boost/locale.hpp>

namespace Utility
{
    const char* GetCodePage(unsigned int e)
    {
        switch (e)
        {
        case E_ANSI:
            return "gbk";

        case E_GBK:
            return "gbk";

        case E_UTF8:
            return "utf-8";

        case E_BIG5:
            return "big5";

        case E_LOCAL:
        default:
            return Local();
        }
    }

    void a2w(std::string const& src, std::wstring & dst)
    {
        encode_convert(src, dst, GetCodePage(E_ANSI));
    }

    std::wstring a2w(std::string const& src)
    {
        std::wstring dst;
        a2w( src, dst );
        return dst;
    }

    void w2a( std::wstring const& src, std::string & dst )
    {
        encode_convert(src, dst, GetCodePage(E_ANSI));
    }

    std::string w2a( std::wstring const& src )
    {
        std::string dst;
        w2a( src, dst );
        return dst;
    }

    void u82w( std::string const& src, std::wstring & dst )
    {
        encode_convert( src, dst, GetCodePage(E_UTF8) );
    }

    std::wstring u82w( std::string const& src )
    {
        std::wstring dst;
        u82w( src, dst );
        return dst;
    }

    void w2u8( std::wstring const& src, std::string & dst )
    {
        encode_convert( src, dst, GetCodePage(E_UTF8) );
    }

    std::string w2u8( std::wstring const& src )
    {
        std::string dst;
        w2u8( src, dst );
        return dst;
    }

	void acp2u8( std::string const& src, std::string & dst )
	{
        encode_convert(src, dst, Local(), GetCodePage(E_UTF8));
	}

	std::string acp2u8( std::string const& src )
	{
		std::string dst;
		acp2u8(src, dst);
		return dst;
	}

    void acp2a(std::string const& src, std::string & dst)
    {
        encode_convert(src, dst, Local(), GetCodePage(E_ANSI));
    }
    std::string acp2a(std::string const& src)
    {
        std::string dst;
        acp2a(src, dst);
        return dst;
    }

	void gbk2a( std::string const& src, std::string & dst )
	{
        encode_convert(src, dst, GetCodePage(E_GBK), GetCodePage(E_ANSI));
	}
	std::string gbk2a( std::string const& src )
	{
		std::string dst;
		gbk2a(src, dst);
		return dst;
	}

	void gbk2u8(std::string const& src, std::string & dst)
	{
		encode_convert(src, dst, GetCodePage(E_GBK), GetCodePage(E_UTF8));
	}
	std::string gbk2u8(std::string const& src)
	{
		std::string dst;
		gbk2u8(src, dst);
		return dst;
	}

    std::wstring to_w(std::string const& str)
    {
        std::wstring result;
        encode_convert(str, result, GetCodePage(E_ANSI));
        return result;
    }


    std::string w_to( std::wstring const& str )
    {
        std::string result;
        encode_convert(str, result, GetCodePage(E_ANSI));
        return result;
    }


    void a2u8( std::string const& src, std::string & dst )
    {
        encode_convert(src, dst, GetCodePage(E_ANSI), GetCodePage(E_UTF8));
    }

    std::string a2u8( std::string const& src )
    {
        std::string dst;
        a2u8( src, dst );
        return dst;
    }

    void u82a( std::string const& src, std::string & dst )
    {
        encode_convert(src, dst, GetCodePage(E_UTF8), GetCodePage(E_ANSI));
    }

    std::string u82a( std::string const& src )
    {
        std::string dst;
        u82a( src, dst );
        return dst;
    }

    void encode_convert(std::string const& src, std::wstring & dst, const char* srcCodePage)
    {
        dst = boost::locale::conv::to_utf<wchar_t>(src, srcCodePage);
    }

    void encode_convert(std::wstring const& src, std::string & dst, const char* dstCodePage)
    {
        dst = boost::locale::conv::from_utf(src, dstCodePage);
    }

    void encode_convert(std::string const& src, std::string & dst, const char* srcCodePage, const char* dstCodePage)
    {
        std::wstring tmp;
        encode_convert(src, tmp, srcCodePage);
        encode_convert(tmp, dst, dstCodePage);
    }

    Local::operator const char* () const
    {
        static boost::locale::generator gen;
        static std::string local = std::use_facet<boost::locale::info>(gen("")).encoding();
        return local.c_str();
    }

}