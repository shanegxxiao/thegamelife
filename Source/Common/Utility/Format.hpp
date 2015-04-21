#ifndef __Format_h__
#define __Format_h__

#include <boost/format.hpp>
#include <boost/preprocessor.hpp>

#ifndef _FORMAT_MAX_PARAM_COUNT
#define _FORMAT_MAX_PARAM_COUNT 16
#endif//_FORMAT_MAX_PARAM_COUNT

/// safe format
namespace Utility
{
    namespace detail
    {
        template <typename T>
        inline T const& convert_fmt_arg(T const& t)
        {
            return t;
        }

#if !defined(TGL_FORMAT_NOT_EXTEND_UNSIGNED_CHAR)
        inline unsigned int convert_fmt_arg(unsigned char const& t)
        {
            return static_cast<unsigned int>(t);
        }
#endif 
    }

    inline std::string format(std::string const& fmt)
    {
        return std::string(fmt);
    }

    inline std::wstring format(std::wstring const& fmt)
    {
        return std::wstring(fmt);
    }

#define _PARAMS(z, n, data) BOOST_PP_COMMA_IF(n) Arg##n const& arg##n
#define _FORMAT_PARAMS(z, n, data) % detail::convert_fmt_arg(arg##n)
#
#define BOOST_PP_LOCAL_MACRO(n)                                                         \
    template < BOOST_PP_ENUM_PARAMS(n, typename Arg) >                                  \
    std::string format(std::string const& fmt, BOOST_PP_REPEAT(n, _PARAMS, ~ ))         \
    {                                                                                   \
        try                                                                             \
        {                                                                               \
            return (boost::format(fmt) BOOST_PP_REPEAT(n, _FORMAT_PARAMS, ~ )).str();   \
        }                                                                               \
        catch(std::exception&)                                                          \
        {                                                                               \
            return std::string("FORMAT ERROR: " + fmt);                                                       \
        }                                                                               \
    }                                                                                   \
                                                                                        \
    template < BOOST_PP_ENUM_PARAMS(n, typename Arg) >                                  \
    std::wstring format(std::wstring const& fmt, BOOST_PP_REPEAT(n, _PARAMS, ~ ))       \
    {                                                                                   \
        try                                                                             \
        {                                                                               \
            return (boost::wformat(fmt) BOOST_PP_REPEAT(n, _FORMAT_PARAMS, ~ )).str();  \
        }                                                                               \
        catch(std::exception&)                                                          \
        {                                                                               \
            return std::wstring(L"FORMAT ERROR: " + fmt);                                                      \
        }                                                                               \
    }
#
#define BOOST_PP_LOCAL_LIMITS (1, _FORMAT_MAX_PARAM_COUNT)
#include BOOST_PP_LOCAL_ITERATE()
#
#undef _PARAMS
#undef _FORMAT_PARAMS

}

#endif//__Format_h__
