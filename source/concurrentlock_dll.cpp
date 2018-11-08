// ConcurrentLock.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"

extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// Remove this if you use lpReserved
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE(_T("ConcurrentLock.DLL Initializing!\n"));
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE(_T("ConcurrentLock.DLL Terminating!\n"));
	}
	return 1;   // ok
}
