#include <cwchar>
#include <string>
#include <vector>
#include <windows.h>

#include "Utilt.h"

namespace spear
{
    std::string RunPowerShell(const std::string& strCmd)
    {
        /*
        * Since GreateProcessA cannot switch the path, the SetCurrentDirectoryA function is used to change the current process path.
        */
        if (strCmd.find("cd") != std::string::npos)
        {
            ::SetCurrentDirectoryA(std::string(strCmd.begin() + 3, strCmd.end()).c_str());
        }
        std::string strNewCmd = "powershell.exe -Command \"" + strCmd + "\"";
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
        if (!CreatePipe(&hRead, &hWrite, &sa, 0)) {
            printf("CreatePipe failed (%d).\n", (int)GetLastError());
            goto Exit0;
        }
    
        ZeroMemory(&si, sizeof(si));
        si.cb = sizeof(si);
        ZeroMemory(&pi, sizeof(pi));
    
        si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
        si.wShowWindow = SW_HIDE; // 隐藏窗口
        si.hStdOutput = hWrite;
        si.hStdError = hWrite;
    
        if (!CreateProcessA(NULL, (LPSTR)strNewCmd.c_str(), NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi)) {
            printf("CreateProcess failed (%d).\n", (int)GetLastError());
            goto Exit0;
        }
    
        if (hWrite)
        {
            CloseHandle(hWrite);
            hWrite = NULL;
        }
    
        // 读取子进程的输出
        while (true) {
            if (!ReadFile(hRead, &buffer[0], buffer.size(), &bytesRead, NULL) || bytesRead == 0) {
                if (GetLastError() == ERROR_BROKEN_PIPE)
                    break;
                else
                    printf("ReadFile failed (%d).\n", (int)GetLastError());
            }
            output.append(&buffer[0], bytesRead);
        }
    
        // 等待子进程退出
        if (pi.hProcess)
            WaitForSingleObject(pi.hProcess, (20 * 1000)); // * Wait 20 seconds
    
    Exit0:
        // 关闭进程和线程句柄
        if (pi.hThread)
            CloseHandle(pi.hThread);
    
        if (pi.hProcess)
            CloseHandle(pi.hProcess);
    
        if (hRead)
            CloseHandle(hRead);
    
        if (hWrite)
            CloseHandle(hWrite);
    
        return spear::GBKToUTF8(output);
    }
}