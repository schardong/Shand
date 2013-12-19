#ifndef SHAND_ENCODING_STRING_ENCODER_INCLUDE
#define SHAND_ENCODING_STRING_ENCODER_INCLUDE

// Copyright Akira Takahashi 2013
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <stdexcept>
#include <boost/predef.h>
#include "./utf16_string.hpp"
#include "./utf8_string.hpp"
#include "./system_string.hpp"

#if BOOST_COMP_MSVC
#include <Windows.h>
#endif

namespace shand {

template <class From, class To>
class encoder;

template <>
class encoder<encoding::utf8, encoding::utf16> {
public:
    static encoding_string<encoding::utf16> encode(const encoding_string<encoding::utf8>& utf8)
    {
        if (utf8.empty())
            return {};

        #if BOOST_COMP_MSVC
            const int len = ::MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), -1, NULL, 0);

            std::wstring utf16(len, wchar_t());

            if (::MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), -1, &utf16[0], utf16.size()) > 0)
                return utf16.c_str();
            else
                throw std::runtime_error("conversion error!");
        #else
            static_cast<void>(utf8);
            throw std::runtime_error("not implemented.");
        #endif
    }
};

template <>
class encoder<encoding::utf8, encoding::system> {
public:
    static encoding_string<encoding::system> encode(const encoding_string<encoding::utf8>& utf8)
    {
        if (utf8.empty())
            return {};

        #if BOOST_COMP_MSVC
            encoding_string<encoding::utf16> utf16 = encoder<encoding::utf8, encoding::utf16>::encode(utf8);

            const int len = ::WideCharToMultiByte(CP_ACP, 0, utf16.c_str(), -1, NULL, 0, NULL, NULL);

            std::string cp932(len, char());

            if (::WideCharToMultiByte(CP_ACP, 0, utf16.c_str(), -1, &cp932[0], len, NULL, NULL) > 0)
                return cp932.c_str();
            else
                throw std::runtime_error("conversion error!");
        #else
            return utf8.c_str();
        #endif
    }
};

} // namespace shand

#endif // SHAND_ENCODING_STRING_ENCODER_INCLUDE