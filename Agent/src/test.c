#include <stdio.h>
#include "API.h"

WINDOWS Win32 = { 0 };

int main(void) {
    DWORD dwRead      = 0;
    DWORD dwBlockSize = 0;
    DWORD httpFlags   = 0;
    char* data        = (char*)LocalAlloc(LMEM_FIXED | LMEM_ZEROINIT, sizeof(char));
    
    BindAPI();

    HINTERNET hSession = Win32.API.rWinHttpOpen(
        L"TEST",
        WINHTTP_ACCESS_TYPE_NO_PROXY,
        WINHTTP_NO_PROXY_NAME,
        WINHTTP_NO_PROXY_BYPASS,
        0
    );

    if (!hSession) {
        perror("hSession error");
        return -1;
    }

    HINTERNET hConnect = Win32.API.rWinHttpConnect(
        hSession,
        L"10.233.183.87",
        8000,
        0
    );

    if (!hConnect) {
        perror("hConnect error");
        Win32.API.rWinHttpCloseHandle(hSession);
        return -1;
    }

    httpFlags = WINHTTP_FLAG_BYPASS_PROXY_CACHE | WINHTTP_FLAG_SECURE;
    HINTERNET hRequest = Win32.API.rWinHttpOpenRequest(
        hConnect,
        L"GET",
        L"/", 
        NULL,
        NULL,
        NULL,
        httpFlags
    );

    if (!hRequest) {
        perror("hRequest error");
        Win32.API.rWinHttpCloseHandle(hConnect);
        Win32.API.rWinHttpCloseHandle(hSession);
        return -1;
    }

    httpFlags = SECURITY_FLAG_IGNORE_UNKNOWN_CA        |
                SECURITY_FLAG_IGNORE_CERT_DATE_INVALID |
                SECURITY_FLAG_IGNORE_CERT_CN_INVALID   |
                SECURITY_FLAG_IGNORE_CERT_WRONG_USAGE;
    if (!Win32.API.rWinHttpSetOption(
        hRequest,
        WINHTTP_OPTION_SECURITY_FLAGS,
        &httpFlags,
        sizeof(httpFlags)
    )) {
        perror("Set option error");
        Win32.API.rWinHttpCloseHandle(hRequest);
        Win32.API.rWinHttpCloseHandle(hConnect);
        Win32.API.rWinHttpCloseHandle(hSession);
        return -1;
    }

    if (!Win32.API.rWinHttpSendRequest(
        hRequest,
        NULL,
        0,
        NULL,
        0, 0, 0
    )) {
        perror("Send msg error");
        Win32.API.rWinHttpCloseHandle(hRequest);
        Win32.API.rWinHttpCloseHandle(hConnect);
        Win32.API.rWinHttpCloseHandle(hSession);
        return -1;
    }

    if (!Win32.API.rWinHttpReceiveResponse(hRequest, NULL)) {
        perror("Receive error");
        Win32.API.rWinHttpCloseHandle(hRequest);
        Win32.API.rWinHttpCloseHandle(hConnect);
        Win32.API.rWinHttpCloseHandle(hSession);
        return -1;
    }

    if (!Win32.API.rWinHttpQueryDataAvailable(hRequest, &dwBlockSize)) {
        perror("Get data length error");
        Win32.API.rWinHttpCloseHandle(hRequest);
        Win32.API.rWinHttpCloseHandle(hConnect);
        Win32.API.rWinHttpCloseHandle(hSession);
        return -1;
    }

    data = (char*)LocalReAlloc((HLOCAL)data, (SIZE_T)dwBlockSize, LMEM_MOVEABLE | LMEM_ZEROINIT);
    if (!Win32.API.rWinHttpReadData(
        hRequest,
        data,
        dwBlockSize,
        &dwRead
    )) {
        perror("Get data error");
        Win32.API.rWinHttpCloseHandle(hRequest);
        Win32.API.rWinHttpCloseHandle(hConnect);
        Win32.API.rWinHttpCloseHandle(hSession);
        return -1;
    }

    printf("%s\n", data);
    LocalFree((HLOCAL)data);

    Win32.API.rWinHttpCloseHandle(hRequest);
    Win32.API.rWinHttpCloseHandle(hConnect);
    Win32.API.rWinHttpCloseHandle(hSession);

    return 0;
}