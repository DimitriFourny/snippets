#pragma comment(lib, "wbemuuid.lib")
#include <Windows.h>
#include <WbemCli.h>
#include <WbemIdl.h>
#include <stdio.h>

bool dumpClassObject(IWbemClassObject* pClassObject, int indentationLevel = 0);

class EventSink : public IWbemEventSink {
private:
    LONG m_lRef;
    bool bDone;
    bool m_processCreated;

public:
    EventSink(bool processCreated) { 
        m_processCreated = processCreated;
    }

    virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, _COM_Outptr_ void __RPC_FAR *__RPC_FAR *ppvObject) {
        if (riid == IID_IUnknown || riid == IID_IWbemObjectSink) {
            *ppvObject = (IWbemEventSink*) this;
            AddRef();
            return WBEM_S_NO_ERROR;
        }
        else return E_NOINTERFACE;
    }

    virtual ULONG STDMETHODCALLTYPE AddRef(void) { 
        return InterlockedIncrement(&m_lRef); 
    }

    virtual ULONG STDMETHODCALLTYPE Release(void) {
        LONG lRef = InterlockedDecrement(&m_lRef);
        if (lRef == 0)
            delete this;
        return lRef;
    }

    virtual HRESULT STDMETHODCALLTYPE Indicate(long lObjectCount, __RPC__in_ecount_full(lObjectCount) IWbemClassObject **apObjArray) {
        for (long i = 0; i < lObjectCount; i++)
        {
            if (m_processCreated) {
                printf("\n\n[+] New Process!\n");
            } else {
                printf("\n\n[-] Process terminated!\n");
            }

            IWbemClassObject* pObj = apObjArray[i];
            dumpClassObject(pObj);
        }
        return WBEM_S_NO_ERROR;
    };

    virtual HRESULT STDMETHODCALLTYPE SetStatus(long lFlags, HRESULT hResult, __RPC__in_opt BSTR strParam, __RPC__in_opt IWbemClassObject *pObjParam) { 
        return WBEM_S_NO_ERROR;
    };

    virtual HRESULT STDMETHODCALLTYPE SetSinkSecurity( long lSDLength, __RPC__in_ecount_full(lSDLength) BYTE *pSD) {
        return WBEM_S_NO_ERROR;
    }

    virtual HRESULT STDMETHODCALLTYPE IsActive(void) { 
        return WBEM_S_NO_ERROR; 
    }

    virtual HRESULT STDMETHODCALLTYPE GetRestrictedSink(long lNumQueries, __RPC__in_ecount_full(lNumQueries) const LPCWSTR *awszQueries,  __RPC__in_opt IUnknown *pCallback, __RPC__deref_out_opt IWbemEventSink **ppSink) {
        return WBEM_S_NO_ERROR;
    }

    virtual HRESULT STDMETHODCALLTYPE SetBatchingParameters(LONG lFlags, DWORD dwMaxBufferSize, DWORD dwMaxSendLatency) {
        return WBEM_S_NO_ERROR;
    }
};

bool dumpClassObject(IWbemClassObject* pClassObject, int indentationLevel) {
    SAFEARRAY* pStrNames;
    HRESULT hError;

    if (indentationLevel == 0) {
        printf("-----------------------------\n");
    }

    hError = pClassObject->GetNames(NULL, WBEM_FLAG_ALWAYS | WBEM_FLAG_NONSYSTEM_ONLY, NULL, &pStrNames);
    if (FAILED(hError)) {
        return false;
    }

    long lowerBound, upperBound;
    SafeArrayGetLBound(pStrNames, 1, &lowerBound);
    SafeArrayGetUBound(pStrNames, 1, &upperBound);
    long nbElements = upperBound - lowerBound + 1;

    BSTR name;
    VARIANT varVal;
    CIMTYPE cymType;
    for (int i = 0; i < nbElements; i++) {
        name = ((BSTR*)pStrNames->pvData)[i];

        hError = pClassObject->Get(name, 0, &varVal, &cymType, NULL);
        if (SUCCEEDED(hError)) {
            if (wcscmp(name, L"TargetInstance") == 0) {
                wprintf(L"%*s%s : 0x%X\n", indentationLevel * 4, L"", name, varVal.uintVal);
                dumpClassObject((IWbemClassObject*)varVal.uintVal, indentationLevel+1);
            } else if (cymType == CIM_STRING) {
                if (varVal.bstrVal != NULL) {
                    wprintf(L"%*s%s : %s\n", indentationLevel*4, L"", name, varVal.bstrVal);
                } else {
                    wprintf(L"%*s%s : NULL_STR\n", indentationLevel * 4, L"", name);
                }
            } else {
                wprintf(L"%*s%s : 0x%X\n", indentationLevel * 4, L"", name, varVal.uintVal);
            }
        }
    }

    return true;
}

#define ASSERT(hError) if(FAILED(hError)){return -1;}
int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    AllocConsole();
    FILE* fp;
    freopen_s(&fp, "CONOUT$", "w", stdout);

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

    EventSink eventCreated(true);
    pWbemServices->ExecNotificationQueryAsync(L"WQL", L"SELECT * FROM __InstanceCreationEvent WITHIN 1 WHERE TargetInstance ISA 'Win32_Process'", 0, NULL, &eventCreated);
    EventSink eventTerminated(false);
    pWbemServices->ExecNotificationQueryAsync(L"WQL", L"SELECT * FROM __InstanceDeletionEvent WITHIN 1 WHERE TargetInstance ISA 'Win32_Process'", 0, NULL, &eventTerminated);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}