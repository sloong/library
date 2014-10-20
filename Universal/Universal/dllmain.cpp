#include "stdafx.h"
#include <iostream>
#include <windows.h>
#include <winerror.h>

#include "factory.h"
#include "IUniversal.h"

using namespace std;

HMODULE g_hModule;
ULONG g_num;

HRESULT RegisterCOMLibrary(LPCWSTR lpPath);
HRESULT UnregisterCOMLibrary();

HRESULT RegisterCOMLibrary(LPCWSTR lpPath)
{
	HKEY thk, tclsidk;

	HRESULT hRes;

	hRes = RegOpenKey(HKEY_CLASSES_ROOT, L"CLSID", &thk);
	if (ERROR_SUCCESS == hRes)
	{
		hRes = RegCreateKey(thk, g_GUID, &tclsidk);
		if (ERROR_SUCCESS == hRes)
		{
			HKEY tinps32k;
			if (ERROR_SUCCESS == RegCreateKey(tclsidk, L"InprocServer32", &tinps32k))
			{

				if (ERROR_SUCCESS == RegSetValue(tinps32k, NULL, REG_SZ, lpPath, wcslen(lpPath) * 2))
				{

				}
				RegCloseKey(tinps32k);
			}
			RegCloseKey(tclsidk);
		}

		RegCloseKey(thk);
	}

	if (ERROR_SUCCESS == RegCreateKey(HKEY_CLASSES_ROOT, g_CLSID, &thk))
	{
		if (ERROR_SUCCESS == RegCreateKey(thk, L"CLSID", &tclsidk))
		{
			if (ERROR_SUCCESS == RegSetValue(tclsidk,
				NULL,
				REG_SZ,
				g_GUID,
				wcslen(g_GUID)*sizeof(TCHAR)))
			{
				MessageBox(NULL, L"Register Succeeded.", L"", MB_OK);
			}
		}
	}

	return 0;
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
	return 0;
}

HRESULT UnregisterCOMLibrary()
{
	HKEY thk;
	TCHAR szPath[MAX_PATH] = { 0 };
	swprintf_s(szPath, MAX_PATH, TEXT("%s\\InprocServer32"), g_GUID);

	if (ERROR_SUCCESS == RegOpenKey(HKEY_CLASSES_ROOT, L"CLSID", &thk))
	{
		RegDeleteKey(thk, szPath);
		RegDeleteKey(thk, g_GUID);

		RegCloseKey(thk);
	}
	if (ERROR_SUCCESS == RegOpenKey(HKEY_CLASSES_ROOT, g_CLSID, &thk))
	{
		RegDeleteKey(thk, L"CLSID");
	}
	RegDeleteKey(HKEY_CLASSES_ROOT, g_CLSID);
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
	if (IID_IUniversal == rclsid)
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
