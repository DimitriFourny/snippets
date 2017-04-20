/* NTFS Alternate Data Stream reader / Delete the security warning for the executables downladed from internet
 * File Streams: https://msdn.microsoft.com/en-us/library/windows/desktop/aa364404(v=vs.85).aspx
 * Output:

     - StreamName = ::$DATA
        StreamSize = 0x226418
     - StreamName = :Zone.Identifier:$DATA
        StreamSize = 0x1A
     ------------------
     [ZoneTransfer]
     ZoneId=3

 */
#include <Windows.h>
#include <stdio.h>

#define STREAM_NAME_LENGTH 1024
#define STREAM_SIZE (sizeof(FILE_STREAM_INFO) + (STREAM_NAME_LENGTH-1)*sizeof(WCHAR))

int main() {
    // List all ADS
    HFILE hFile = CreateFile(L"C:\\Users\\Dimitri\\Desktop\\file.exe", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == HFILE_ERROR) {
        return -1;
    }

    PFILE_STREAM_INFO streamInfo = malloc(STREAM_SIZE);
    if (streamInfo == NULL) {
        return -2;
    }

    ZeroMemory(streamInfo, STREAM_SIZE);
    BOOL success = GetFileInformationByHandleEx(hFile, FileStreamInfo, streamInfo, STREAM_SIZE);
    if (!success) {
        CloseHandle(hFile);
        return -3;
    }

    while (TRUE) {
        wprintf(L"- StreamName = %s\n", &streamInfo->StreamName);
        wprintf(L"\tStreamSize = 0x%X\n", streamInfo->StreamSize);

        if (streamInfo->NextEntryOffset == 0) {
            break;
        }
        streamInfo = (PFILE_STREAM_INFO)((DWORD)streamInfo + streamInfo->NextEntryOffset);
    }
    CloseHandle(hFile);

    // Try to read the Zone.Identifier added by the browser
    // ZoneId=0 => My Computer
    // ZoneId=1 => Local Intranet Zone
    // ZoneId=2 => Trusted Sites Zone
    // ZoneId=3 => Internet Zone
    // ZoneId=4 => Restricted Sites Zone
    hFile = CreateFile(L"C:\\Users\\Dimitri\\Desktop\\file.exe:Zone.Identifier:$DATA", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == HFILE_ERROR) {
        wprintf(L"error: %X\n", GetLastError());
        return -4;
    }

    char buffer[255] = { 0 };
    DWORD dwNbBytes = 0;
    success = ReadFile(hFile, buffer, sizeof(buffer), &dwNbBytes, NULL);
    CloseHandle(hFile);
    if (!success) {
        return -5;
    }

    printf("------------------\n");
    printf("%s\n", buffer);

    // Delete the security warning for the executables downladed from internet
    //DeleteFile(L"C:\\Users\\Dimitri\\Desktop\\file.exe:Zone.Identifier:$DATA");

    return 0;
}