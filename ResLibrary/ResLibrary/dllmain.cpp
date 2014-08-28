// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#ifdef _UNITTEST
#include <vld.h>
#pragma comment(lib,"vld.lib")
#endif // _UNITTEST
#include "ResLibrary.h"

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		if( S_FALSE == Initialize() )
		{
			return FALSE;
		}
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		Shutdown();
		break;
	}
	return TRUE;
}

