// benchmark.h : main header file for the BENCHMARK application
//

#if defined(_WIN32)
   #pragma message("Building for WIN32-MFC")
   #include "stdafx.h"
#else
     #include <stdio.h>
     #include <stdarg.h>
     #include <malloc.h>
     #include <unistd.h>
#endif

#if defined(_WIN32)
   #include <time.h>
#elif defined(__IBMC__) || defined(__IBMCPP__)
   #include <time.h>
   #include <utils.h>
#else
//   #include <sys/file.h>
//   #include <sys/types.h>
   typedef void* LPVOID;
     #include <wchar.h>
     #define _tprintf printf
     #define _ftprintf fprintf
   #define WINAPI
#endif

//#include <errno.h>

#if defined(_MFC)
   #include <afxmt.h>
#elif defined(_WIN32)
#else
   #include <pthread.h>
#endif

#ifdef __sparc
   #include <sys/uio.h>
#endif

#if !defined(max)
   #define max(a,b)     ((a) >= (b) ? (a) : (b))
   #define min(a,b)     ((a) <= (b) ? (a) : (b))
#endif

///////////////////////////////////////////////////////////////////////////

#include "ConcurrentLock.h"
#define SELECT_NONE		0
#define SELECT_CONCUR	1
#define SELECT_MUTEX	2
#define SELECT_CRITSECT	3

/////////////////////////////////////////////////////////////////////////////
// some global resource
class Weakness
{
public:
	Weakness() {countRead=0;countWrite=0;};
	~Weakness() {};

	UINT countRead;
	UINT countWrite;
};

class Weakness_ConcurrentLock : public Weakness
{
	CONCUR_DECLARE_LOCKABLE;
	Weakness_ConcurrentLock() : Weakness() {};
	~Weakness_ConcurrentLock() {};
};

class Weakness_Mutex : public Weakness
{
	MUTEX_DECLARE_LOCKABLE;
	Weakness_Mutex() : Weakness() {};
	~Weakness_Mutex() {};
};

class Weakness_CritSect : public Weakness
{
	CRITSECT_DECLARE_LOCKABLE;
	Weakness_CritSect() : Weakness() {};
	~Weakness_CritSect() {};
};

extern Weakness* Kryptonite;

