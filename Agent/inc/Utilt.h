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
} // namespace spear
