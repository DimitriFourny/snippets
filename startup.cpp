// Windows Startup

#include <Windows.h>
#include <ShlObj.h>
#include <strsafe.h>

int main() {
    wchar_t currentPath[MAX_PATH] = { 0 };
    unsigned currentPathLen = 0;
    wchar_t installPath[MAX_PATH] = { 0 };
    wchar_t regeditPath[MAX_PATH + 2] = L"\"";
    BOOL success = FALSE;
    LSTATUS status = 0;

    // Source
    currentPathLen = GetModuleFileName(NULL, currentPath, MAX_PATH);
    if (currentPathLen == 0) {
        return -1;
    }

    // Destination
    status = SHGetFolderPath(0, CSIDL_APPDATA, -1, SHGFP_TYPE_DEFAULT, installPath);
    if (status != S_OK) {
        return -1;
    }
    StringCchCat(installPath, MAX_PATH, L"\\Hello.exe");

    // Copy
    if (lstrcmp(currentPath, installPath) == 0) {
        return 1;
    }
    success = CopyFile(currentPath, installPath, FALSE);
    if (!success) {
        return -1;
    }

    // Add a registry entry
    lstrcatW(regeditPath, installPath);
    lstrcatW(regeditPath, L"\"");

    HKEY hKey = NULL;
    status = RegOpenKeyEx(HKEY_CURRENT_USER, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_SET_VALUE, &hKey);
    if (status != ERROR_SUCCESS) {
        return -1;
    }

    status = RegSetValueEx(hKey, L"Hello", 0, REG_SZ, regeditPath, (lstrlenW(regeditPath)+1)*sizeof(wchar_t));
    if (status != ERROR_SUCCESS) {
        RegCloseKey(hKey);
        return -1;
    }

    RegCloseKey(hKey);
    return 0;
}