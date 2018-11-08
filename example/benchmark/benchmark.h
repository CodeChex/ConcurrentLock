// benchmark.h : main header file for the BENCHMARK application
//

#if !defined(AFX_BENCHMARK_H__E73EE220_C199_4189_AD7A_3B7879EAB90A__INCLUDED_)
#define AFX_BENCHMARK_H__E73EE220_C199_4189_AD7A_3B7879EAB90A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include "ConcurrentLock.h"

#define WM_UPDATE_TOTALS	WM_USER + 999
#define WM_UPDATE_WINDOW	WM_USER + 1000
#define WM_ADD_TIMESTAMP	WM_USER + 1001
#define WM_UPDATE_STATS		WM_USER + 1002

#define SELECT_CONCUR	0
#define SELECT_MUTEX	1
#define SELECT_CRITSECT	2

/////////////////////////////////////////////////////////////////////////////
// CBenchmarkApp:
// See benchmark.cpp for the implementation of this class
//

class CBenchmarkApp : public CWinApp
{
public:
	CBenchmarkApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBenchmarkApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CBenchmarkApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// some global resource
class Weakness : public CObject
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

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BENCHMARK_H__E73EE220_C199_4189_AD7A_3B7879EAB90A__INCLUDED_)
