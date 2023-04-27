#ifndef PES_H
#define PES_H

#include <Windows.h>
#include <Lmcons.h>
#include <iphlpapi.h>
#include <stdio.h>
#include <slpublic.h>
#include <strsafe.h>
#include <mbstring.h>
#include <winternl.h>
#include <ntstatus.h>

#pragma comment(lib, "ntdll.lib")

#define NT_ROOT L"\\NTFS\\"
#define FILE_PROC_BUFFER_SIZE 16384

#define FileProcessIdsUsingFileInformation 0x2F

typedef struct _FILE_PROCESS_IDS_USING_FILE_INFORMATION
{
    ULONG NumberOfProcessIdsInList;
	ULONG_PTR ProcessIdList[1];
} FILE_PROCESS_IDS_USING_FILE_INFORMATION, *PFILE_PROCESS_IDS_USING_FILE_INFORMATION;

typedef NTSTATUS(NTAPI* _NtQueryInformationFile)(HANDLE, PIO_STATUS_BLOCK, PVOID, DWORD, FILE_INFORMATION_CLASS);

#endif