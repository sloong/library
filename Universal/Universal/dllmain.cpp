#include "stdafx.h"
#include "factory.h"
#include "IUniversal.h"

using namespace std;

HMODULE g_hModule;
ULONG g_num;

HRESULT RegisterCOMLibrary(LPCWSTR lpPath);
HRESULT UnregisterCOMLibrary();

HRESULT RegisterCOMLibrary(LPCWSTR lpPath)
{
	// This function is used in DllRegisterServer,
	// But in the function, return a error code, always pop-up a succeeded message.
	// So in here, all return check is deleted.
	HKEY thk, tclsidk, tinps32k;

	if (ERROR_SUCCESS == RegOpenKey(HKEY_CLASSES_ROOT, L"CLSID", &thk))
	{
		RegCreateKey(thk, szCLSID_Universal, &tclsidk);
		RegCreateKey(tclsidk, L"InProcServer32", &tinps32k);
		RegSetValue(tinps32k, NULL, REG_SZ, lpPath, (DWORD)wcslen(lpPath) * 2);
		RegCloseKey(tinps32k);
		RegCloseKey(tclsidk);
		RegCloseKey(thk);
	}

	RegCreateKey(HKEY_CLASSES_ROOT, szProgID_Universal, &thk);
	RegCreateKey(thk, L"CLSID", &tclsidk);
	RegSetValue(tclsidk, NULL, REG_SZ, szCLSID_Universal, (DWORD)wcslen(szCLSID_Universal)*sizeof(TCHAR));
	
	return S_OK;
}

extern "C" HRESULT _stdcall DllRegisterServer()
{
	WCHAR szModule[1024];
	DWORD dwResult = GetModuleFileName(g_hModule, szModule, 1024);
	if (0 == dwResult)
	{
		return -1;
	}
	RegisterCOMLibrary(szModule);
	return S_OK;
}

HRESULT UnregisterCOMLibrary()
{
	HKEY thk;
	TCHAR szPath[MAX_PATH] = { 0 };
	swprintf_s(szPath, MAX_PATH, TEXT("%s\\InprocServer32"), szCLSID_Universal);

	if (ERROR_SUCCESS == RegOpenKey(HKEY_CLASSES_ROOT, L"CLSID", &thk))
	{
		RegDeleteKey(thk, szPath);
		RegDeleteKey(thk, szCLSID_Universal);

		RegCloseKey(thk);
	}
	if (ERROR_SUCCESS == RegOpenKey(HKEY_CLASSES_ROOT, szProgID_Universal, &thk))
	{
		RegDeleteKey(thk, L"CLSID");
	}
	RegDeleteKey(HKEY_CLASSES_ROOT, szProgID_Universal);
	return S_OK;
}

extern "C" HRESULT _stdcall DllUnregisterServer()
{
	UnregisterCOMLibrary();

	return 0;
}

extern "C" HRESULT _stdcall DllCanUnloadNow()
{
	if (0 == g_num)
	{
		return S_OK;
	}
	else
	{
		return S_FALSE;
	}
}

extern "C" HRESULT _stdcall DllGetClassObject(__in REFCLSID rclsid, __in REFIID riid, LPVOID FAR* ppv)
{
	if (CLSID_SLOONGUniversal == rclsid)
	{
		CFactory* pFactory = new CFactory();
		if (NULL == pFactory)
		{
			return E_OUTOFMEMORY;
		}
		HRESULT result = pFactory->QueryInterface(riid, ppv);
		return result;
	}
	else
	{
		return CLASS_E_CLASSNOTAVAILABLE;
	}
}


BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
	)
{
	g_hModule = hModule;
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}
