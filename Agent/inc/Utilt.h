#pragma once
#ifndef _UTILT_H_
#define _UTILT_H_

#include <cwchar>
#include <string>
#include <locale>
#include <codecvt>

namespace spear
{
    inline std::wstring ToWstring(const std::string& in)
    {
        std::wstring_convert<std::codecvt_utf8<wchar_t> > converter;
        return converter.from_bytes(in);
    }

    std::string GBKToUTF8(const std::string& gbk);
} // namespace spear

#endif