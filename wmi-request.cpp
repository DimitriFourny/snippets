// Performs a WMI request in C++

#pragma comment(lib, "wbemuuid.lib")
#include <Windows.h>
#include <WbemCli.h>

#define ASSERT(hError) if(FAILED(hError)){return -1;}

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    HRESULT hError = S_OK;
    hError = CoInitializeEx(NULL, COINIT_MULTITHREADED);
    ASSERT(hError);

    hError = CoInitializeSecurity(NULL, -1, NULL, NULL, RPC_C_AUTHN_LEVEL_DEFAULT, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE, NULL);
    ASSERT(hError);

    IWbemLocator* pLoc = NULL;
    hError = CoCreateInstance(CLSID_WbemLocator, 0, CLSCTX_INPROC_SERVER, IID_IWbemLocator, (LPVOID*)&pLoc);
    ASSERT(hError);

    IWbemServices* pWbemServices = NULL;
    hError = pLoc->ConnectServer(L"root\\cimv2", NULL, NULL, NULL, WBEM_FLAG_CONNECT_USE_MAX_WAIT, NULL, NULL, &pWbemServices);
    ASSERT(hError);

    IEnumWbemClassObject* pEnumClassObject = NULL;
    hError = pWbemServices->ExecQuery(L"WQL", L"SELECT * FROM Win32_Processor", WBEM_FLAG_FORWARD_ONLY, NULL, &pEnumClassObject);
    ASSERT(hError);

    ULONG ret;
    IWbemClassObject* pClassObject = NULL;
    hError = pEnumClassObject->Next(WBEM_INFINITE, 1L, &pClassObject, &ret);
    ASSERT(hError);

    VARIANT varVal;
    pClassObject->Get(L"Name", 0, &varVal, NULL, NULL);

    MessageBoxW(0, varVal.bstrVal, L"Info", MB_OK|MB_ICONINFORMATION);

    return 0;
}