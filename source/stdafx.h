// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#pragma once

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

// TODO: reference additional headers your program requires here
#include <time.h>
#include <OAIDL.H>
#include <tchar.h>

#include <string>
#include <vector>
#include <set>
#include <map>
#include <iterator>

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
#include <time.h>
#include <OAIDL.H>
#include <tchar.h>

#include <string>
#include <vector>
#include <set>
#include <map>
#include <iterator>

#pragma warning(disable:4251)
#pragma warning(disable:4482)
#pragma warning(disable:4786)

#include <mmsystem.h>
#pragma comment(lib,"winmm")

#ifndef TRACE
#define TRACE	_tprintf
#endif

