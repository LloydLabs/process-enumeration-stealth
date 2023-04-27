#include "main.h"

static 
HANDLE 
get_ntfs_handle(
    VOID
)
{
    UNICODE_STRING ntPath;
    RtlInitUnicodeString(&ntPath, NT_ROOT);

    IO_STATUS_BLOCK ioStatusBlock;
    RtlSecureZeroMemory(&ioStatusBlock, sizeof(ioStatusBlock));

    OBJECT_ATTRIBUTES oaDev;
    InitializeObjectAttributes(&oaDev, &ntPath, OBJ_CASE_INSENSITIVE, NULL, NULL);

    return NtCreateFile(&hDev, GENERIC_READ, &oaDev, &ioStatusBlock, NULL, FILE_ATTRIBUTE_NORMAL, FILE_SHARE_READ, FILE_OPEN, 0, NULL, 0);
}

INT 
main(
    INT argc,
    PCHAR* argv
)
{
    HANDLE hNtfs = get_ntfs_handle();
    if (hNtfs == INVALID_HANDLE_VALUE)
    {
        return 0;
    }

    _NtQueryInformationFile NtQueryInformationFile = GetProcAddress(LoadLibrary(L"ntdll.dll"), "NtQueryInformationFile");
    if (NtQueryInformationFile == NULL)
    {
        CloseHandle(hNtfs);
        return 0;
    }

    PFILE_PROCESS_IDS_USING_FILE_INFORMATION pProcessInfo = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, FILE_PROC_BUFFER_SIZE);
    if (pProcessInfo == NULL)
    {
        CloseHandle(hNtfs);
        return 0;
    }
       
    IO_STATUS_BLOCK statusBlock;
    RtlSecureZeroMemory(&statusBlock, sizeof(statusBlock));

    // query for accesses to the base NTFS object, which is every process
    if (SUCCEEDED(NtQueryInformationFile(hNtfs, &statusBlock, pProcessInfo, FILE_PROC_BUFFER_SIZE, FileProcessIdsUsingFileInformation)))
    {
        for (ULONG i = 0; i < pProcessInfo->NumberOfProcessIdsInList; i++)
        {
            printf("Found a PID => %d\n", pProcessInfo->ProcessIdList[i]);
        }
    }

    HeapFree(GetProcessHeap(), pProcessInfo, 0);
    CloseHandle(hNtfs);
    return 1;
}