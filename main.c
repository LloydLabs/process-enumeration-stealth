#include "main.h"

static HANDLE pes_ntfs_handle()
{
    UNICODE_STRING ntPath;
    RtlInitUnicodeString(&ntPath, PES_NT_ROOT);

    IO_STATUS_BLOCK ioStatusBlock;
    RtlSecureZeroMemory(&ioStatusBlock, sizeof(ioStatusBlock));

    OBJECT_ATTRIBUTES oaDev;
    InitializeObjectAttributes(&oaDev, &ntPath, OBJ_CASE_INSENSITIVE, NULL, NULL);

    HANDLE hDev = INVALID_HANDLE_VALUE;
    if (SUCCEEDED(NtCreateFile(
        &hDev,
        GENERIC_READ | SYNCHRONIZE,
        &oaDev,
        &ioStatusBlock,
        NULL,
        FILE_ATTRIBUTE_NORMAL,
        FILE_SHARE_READ,
        FILE_OPEN,
        0,
        NULL,
        0
    )))
    {
        return hDev;
    }

    return INVALID_HANDLE_VALUE;

}

static PVOID pes_resolve_nt(LPCSTR lpszFunc)
{
    HMODULE hNtLib = LoadLibrary(L"ntdll.dll");
    if (hNtLib == NULL)
    {
        return NULL;
    }

    return GetProcAddress(hNtLib, lpszFunc);
}

int main(int argc, char** argv)
{
    // get a handle to the NTFS object
    HANDLE hDev = pes_ntfs_handle();
    if (hDev == INVALID_HANDLE_VALUE)
    {
        return 0;
    }

    // resolve NtQueryInformationFile
    _NtQueryInformationFile NtQueryInformationFile = pes_resolve_nt("NtQueryInformationFile");
    if (NtQueryInformationFile == NULL)
    {
        CloseHandle(hDev);
        return 0;
    }

    // create a PFILE_PROCESS_IDS_USING_FILE_INFORMATION list
    DWORD dwLen = PES_BUFFER_SIZE;
    PFILE_PROCESS_IDS_USING_FILE_INFORMATION pProcessInfo = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, (SIZE_T)dwLen);
    if (pProcessInfo == NULL)
    {
        CloseHandle(hDev);
        return 0;
    }
       
    IO_STATUS_BLOCK ioStatusBlock;
    RtlSecureZeroMemory(&ioStatusBlock, sizeof(ioStatusBlock));

    // query for accesses to the base NTFS object, which is every process
    if (SUCCEEDED(NtQueryInformationFile(hDev, &ioStatusBlock, pProcessInfo, dwLen, FileProcessIdsUsingFileInformation)))
    {
        for (ULONG i = 0; i < pProcessInfo->NumberOfProcessIdsInList; i++)
        {
            printf("Found a PID => %d\n", pProcessInfo->ProcessIdList[i]);
        }
    }

    // free everything
    HeapFree(GetProcessHeap(), pProcessInfo, 0);
    CloseHandle(hDev);
    return 1;
}