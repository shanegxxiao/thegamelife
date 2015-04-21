#ifndef __SPC_PLATFORM_WIN32_ENCODE__
#define __SPC_PLATFORM_WIN32_ENCODE__

#include <string>
#include <stdarg.h>

namespace Utility
{
    // multi-byte encode
    enum _emEncode
    {
        E_LOCAL,				// local default, ANSI
        E_ANSI,
        E_GBK,
        E_UTF8,
        E_BIG5,
        E_Max
    };


    /// get code page
    const char* GetCodePage(unsigned int e);

    // ANSI to utf-16
    void a2w(std::string const& src, std::wstring & dst);
    std::wstring a2w(std::string const& src);

    /// utf-16 to ANSI
    void w2a(std::wstring const& src, std::string & dst);
    std::string w2a(std::wstring const& src);

    /// ANSI to utf-8
    void a2u8( std::string const& src, std::string & dst );
    std::string a2u8( std::string const& src );

    /// utf-8 to ANSI
    void u82a( std::string const& src, std::string & dst );
    std::string u82a( std::string const& src );

    /// utf-8 to utf-16
    void u82w( std::string const& src, std::wstring & dst );
    std::wstring u82w( std::string const& src );

    /// utf-16 to utf-8
    void w2u8( std::wstring const& src, std::string & dst );
    std::string w2u8( std::wstring const& src );

	/// local to utf-8
	void acp2u8( std::string const& src, std::string & dst );
	std::string acp2u8( std::string const& src );

	/// local to ansi
	void acp2a( std::string const& src, std::string & dst );
	std::string acp2a( std::string const& src );

	/// gbk to ansi
	void gbk2a( std::string const& src, std::string & dst );
	std::string gbk2a( std::string const& src );

	/// gbk to utf-8
	void gbk2u8( std::string const& src, std::string & dst );
	std::string gbk2u8( std::string const& src );

    std::wstring to_w(std::string const& str);
    std::string w_to(std::wstring const& str);

    /// common
    void encode_convert( std::string const& src, std::wstring & dst, const char* srcCodePage );
    void encode_convert(std::wstring const& src, std::string & dst, const char* dstCodePage);
    void encode_convert(std::string const& src, std::string & dst, const char* srcCodePage, const char* dstCodePage);

    /// get local code page
    class Local
    {
    public:
        operator const char* () const;
    };

    /// helper class
    template <unsigned int uiEncode = E_LOCAL>
    class AutoEncode
    {
    public:
        AutoEncode() {}
        ~AutoEncode() {}
        AutoEncode( std::string const& str )
            : m_str( str )
        {
            encode_convert( m_str, m_wstr, GetCodePage(uiEncode) );
        }

        AutoEncode( std::wstring const& wstr )
            : m_wstr( wstr )
        {
            encode_convert( m_wstr, m_str, GetCodePage(uiEncode) );
        }

        template<unsigned int _cp>
        AutoEncode(AutoEncode<_cp> const& other)
            : m_wstr(other)
        {
            encode_convert( m_wstr, m_str, GetCodePage(uiEncode) );
        }

        unsigned int cp() const
        {
            return uiEncode;
        }

        operator std::string const& () const
        {
            return m_str;
        }

        operator std::wstring const& () const
        {
            return m_wstr;
        }

        operator const char* () const
        {
            return m_str.c_str();
        }

        operator const wchar_t* () const
        {
            return m_wstr.c_str();
        }

    private:
        std::wstring m_wstr;
        std::string m_str;
    };

    typedef AutoEncode<> LocalAutoEncode;
}

#endif