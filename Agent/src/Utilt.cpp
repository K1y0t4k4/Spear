#include "Utilt.h"
#include <windows.h>

namespace spear
{
    std::string GBKToUTF8(const std::string& gbk)
    {
        int wideLen = ::MultiByteToWideChar(CP_ACP, 0, gbk.c_str(), -1, NULL, 0);
        wchar_t* wideBuffer = new wchar_t[wideLen + 1];
        ZeroMemory(wideBuffer, wideLen * 2 + 2);
        ::MultiByteToWideChar(CP_ACP, 0, gbk.c_str(), gbk.length(), wideBuffer, wideLen);

        int multLen = ::WideCharToMultiByte(CP_UTF8, 0, wideBuffer, -1, NULL, 0, NULL, NULL);
        char* multBuffer = new char[multLen + 1];
        ZeroMemory(multBuffer, multLen + 1);
        ::WideCharToMultiByte(CP_UTF8, 0, wideBuffer, wideLen, multBuffer, multLen, NULL, NULL);

        std::string retStr(multBuffer);
        delete[]wideBuffer;
        delete[]multBuffer;
        wideBuffer = NULL;
        multBuffer = NULL;
        return retStr;
    }
}