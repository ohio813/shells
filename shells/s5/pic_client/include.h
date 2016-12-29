/**
  Copyright © 2016 Odzhan. All Rights Reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are
  met:

  1. Redistributions of source code must retain the above copyright
  notice, this list of conditions and the following disclaimer.

  2. Redistributions in binary form must reproduce the above copyright
  notice, this list of conditions and the following disclaimer in the
  documentation and/or other materials provided with the distribution.

  3. The name of the author may not be used to endorse or promote products
  derived from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY AUTHORS "AS IS" AND ANY EXPRESS OR
  IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT,
  INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
  HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
  POSSIBILITY OF SUCH DAMAGE. */

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <windows.h>
#include <winnt.h>
#include <winternl.h>
#include <intrin.h>

#include <stddef.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

//#define DEBUG 1 // only for compiling as executable

#if defined(DEBUG) && DEBUG > 0
 #define DEBUG_PRINT(...) { \
   fprintf(stderr, "\nDEBUG: %s:%d:%s(): ", __FILE__, __LINE__, __FUNCTION__); \
   fprintf(stderr, __VA_ARGS__); \
 }
#else
 #define DEBUG_PRINT(...) // Don't do anything in release builds
#endif

#pragma intrinsic(memcmp, memcpy, memset)

#define memcpy(x,y,z) __movsb(x,y,z)
#define memmove(x,y,z) __movsb(x,y,z)
#define memset(x,y,z) __stosb(x,y,z)

#include "aes.h"
#include "sha3.h"
#include "modexp.h"

typedef struct _MY_PEB_LDR_DATA {
  ULONG      Length;
  BOOL       Initialized;
  PVOID      SsHandle;
  LIST_ENTRY InLoadOrderModuleList;
  LIST_ENTRY InMemoryOrderModuleList;
  LIST_ENTRY InInitializationOrderModuleList;
} MY_PEB_LDR_DATA, *PMY_PEB_LDR_DATA;

typedef struct _MY_LDR_DATA_TABLE_ENTRY
{
  LIST_ENTRY     InLoadOrderLinks;
  LIST_ENTRY     InMemoryOrderLinks;
  LIST_ENTRY     InInitializationOrderLinks;
  PVOID          DllBase;
  PVOID          EntryPoint;
  ULONG          SizeOfImage;
  UNICODE_STRING FullDllName;
  UNICODE_STRING BaseDllName;
} MY_LDR_DATA_TABLE_ENTRY, *PMY_LDR_DATA_TABLE_ENTRY;

typedef BOOL (WINAPI* GetFileSizeEx_t)(
  _In_  HANDLE         hFile,
  _Out_ PLARGE_INTEGER lpFileSize);

typedef BOOL (WINAPI* Wow64DisableWow64FsRedirection_t)(
    __out       PVOID *OldValue);
    
typedef BOOL (WINAPI* CryptAcquireContextA_t) (
    __out       HCRYPTPROV  *phProv,
    __in_opt    LPCSTR    szContainer,
    __in_opt    LPCSTR    szProvider,
    __in        DWORD       dwProvType,
    __in        DWORD       dwFlags);
    
typedef BOOL (WINAPI* CryptReleaseContext_t) (
    __in    HCRYPTPROV  hProv,
    __in    DWORD       dwFlags);
    
typedef BOOL (WINAPI* CryptGenRandom_t) (
    __in                    HCRYPTPROV  hProv,
    __in                    DWORD   dwLen,
    __inout_bcount(dwLen)   BYTE    *pbBuffer);
    
typedef HMODULE (WINAPI* LoadLibrary_t)(
  _In_ LPCTSTR lpFileName
);

typedef int (WINAPI* WSACleanup_t)(void);

typedef DWORD (WINAPI* GetTickCount_t)(void);
typedef DWORD (WINAPI* GetLastError_t)(void);
typedef BOOL (WINAPI* CloseHandle_t)(_In_ HANDLE hFile);

typedef BOOL (WINAPI* CreateProcess_t)(
  _In_opt_    LPCTSTR               lpApplicationName,
  _Inout_opt_ LPTSTR                lpCommandLine,
  _In_opt_    LPSECURITY_ATTRIBUTES lpProcessAttributes,
  _In_opt_    LPSECURITY_ATTRIBUTES lpThreadAttributes,
  _In_        BOOL                  bInheritHandles,
  _In_        DWORD                 dwCreationFlags,
  _In_opt_    LPVOID                lpEnvironment,
  _In_opt_    LPCTSTR               lpCurrentDirectory,
  _In_        LPSTARTUPINFO         lpStartupInfo,
  _Out_       LPPROCESS_INFORMATION lpProcessInformation
);

typedef HANDLE (WINAPI* CreateThread_t)(
  _In_opt_  LPSECURITY_ATTRIBUTES  lpThreadAttributes,
  _In_      SIZE_T                 dwStackSize,
  _In_      LPTHREAD_START_ROUTINE lpStartAddress,
  _In_opt_  LPVOID                 lpParameter,
  _In_      DWORD                  dwCreationFlags,
  _Out_opt_ LPDWORD                lpThreadId
);

typedef HANDLE (WINAPI* CreateEvent_t)(
  _In_opt_ LPSECURITY_ATTRIBUTES lpEventAttributes,
  _In_     BOOL                  bManualReset,
  _In_     BOOL                  bInitialState,
  _In_opt_ LPCTSTR               lpName
);

typedef BOOL (WINAPI* TerminateProcess_t)(
  _In_ HANDLE hProcess,
  _In_ UINT   uExitCode
);

typedef int (WINAPI* WSAStartup_t)(
  _In_  WORD      wVersionRequested,
  _Out_ LPWSADATA lpWSAData
);

typedef SOCKET (WSAAPI* socket_t)(
  _In_ int af,
  _In_ int type,
  _In_ int protocol
);

typedef int (WINAPI* connect_t)(
  _In_ SOCKET                s,
  _In_ const struct sockaddr *name,
  _In_ int                   namelen
);

typedef int (WINAPI* send_t)(
  _In_       SOCKET s,
  _In_ const char   *buf,
  _In_       int    len,
  _In_       int    flags
);

typedef int (WINAPI* recv_t)(
  _In_  SOCKET s,
  _Out_ char   *buf,
  _In_  int    len,
  _In_  int    flags
);

typedef int (WINAPI* ioctlsocket_t)(
  _In_    SOCKET s,
  _In_    long   cmd,
  _Inout_ u_long *argp
);

typedef int (WINAPI* closesocket_t)(
  _In_ SOCKET s
);

typedef WSAEVENT (WINAPI* WSACreateEvent_t)(void);

typedef int (WINAPI* WSAEventSelect_t)(
  _In_ SOCKET   s,
  _In_ WSAEVENT hEventObject,
  _In_ long     lNetworkEvents
);

typedef int (WINAPI* WSAEnumNetworkEvents_t)(
  _In_  SOCKET             s,
  _In_  WSAEVENT           hEventObject,
  _Out_ LPWSANETWORKEVENTS lpNetworkEvents
);

typedef DWORD (WINAPI* WaitForMultipleObjects_t)(
  _In_       DWORD  nCount,
  _In_ const HANDLE *lpHandles,
  _In_       BOOL   bWaitAll,
  _In_       DWORD  dwMilliseconds
);

typedef BOOL (WINAPI* CreatePipe_t)(
  _Out_    PHANDLE               hReadPipe,
  _Out_    PHANDLE               hWritePipe,
  _In_opt_ LPSECURITY_ATTRIBUTES lpPipeAttributes,
  _In_     DWORD                 nSize
);

typedef HANDLE (WINAPI* CreateNamedPipe_t)(
  _In_     LPCTSTR               lpName,
  _In_     DWORD                 dwOpenMode,
  _In_     DWORD                 dwPipeMode,
  _In_     DWORD                 nMaxInstances,
  _In_     DWORD                 nOutBufferSize,
  _In_     DWORD                 nInBufferSize,
  _In_     DWORD                 nDefaultTimeOut,
  _In_opt_ LPSECURITY_ATTRIBUTES lpSecurityAttributes
);

typedef HANDLE (WINAPI* CreateFile_t)(
  _In_     LPCTSTR               lpFileName,
  _In_     DWORD                 dwDesiredAccess,
  _In_     DWORD                 dwShareMode,
  _In_opt_ LPSECURITY_ATTRIBUTES lpSecurityAttributes,
  _In_     DWORD                 dwCreationDisposition,
  _In_     DWORD                 dwFlagsAndAttributes,
  _In_opt_ HANDLE                hTemplateFile
);

typedef BOOL (WINAPI* ReadFile_t)(
  _In_        HANDLE       hFile,
  _Out_       LPVOID       lpBuffer,
  _In_        DWORD        nNumberOfBytesToRead,
  _Out_opt_   LPDWORD      lpNumberOfBytesRead,
  _Inout_opt_ LPOVERLAPPED lpOverlapped
);

typedef BOOL (WINAPI* WriteFile_t)(
  _In_        HANDLE       hFile,
  _In_        LPCVOID      lpBuffer,
  _In_        DWORD        nNumberOfBytesToWrite,
  _Out_opt_   LPDWORD      lpNumberOfBytesWritten,
  _Inout_opt_ LPOVERLAPPED lpOverlapped
);

typedef BOOL (WINAPI* GetOverlappedResult_t)(
  _In_  HANDLE       hFile,
  _In_  LPOVERLAPPED lpOverlapped,
  _Out_ LPDWORD      lpNumberOfBytesTransferred,
  _In_  BOOL         bWait
);
