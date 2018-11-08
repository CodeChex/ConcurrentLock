// benchmarkDlg.h : header file
//

#if !defined(AFX_BENCHMARKDLG_H__5089CC70_2288_46D8_8AAE_A83816B4B4ED__INCLUDED_)
#define AFX_BENCHMARKDLG_H__5089CC70_2288_46D8_8AAE_A83816B4B4ED__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BenchmarkThreadDlg.h"

#include <list> 

typedef list<BenchmarkThread*> _t_vec_thread;

/////////////////////////////////////////////////////////////////////////////
// CBenchmarkDlg dialog

class CBenchmarkDlg : public CDialog
{
// Construction
public:
	CBenchmarkDlg(CWnd* pParent = NULL);	// standard constructor

	_t_vec_thread m_vec_thread;

// Dialog Data
	//{{AFX_DATA(CBenchmarkDlg)
	enum { IDD = IDD_BENCHMARK_DIALOG };
	CEdit	m_ctl_duration;
	CButton	m_ctl_lockConcur;
	CButton	m_ctl_lockMutex;
	CButton	m_ctl_lockCritSect;
	CEdit	m_ctl_timeoutW;
	CEdit	m_ctl_timeoutR;
	CEdit	m_ctl_nThreadW;
	CEdit	m_ctl_nThreadR;
	CEdit	m_ctl_msecsW;
	CEdit	m_ctl_msecsR;
	CButton	m_btnThreadShow;
	CButton	m_btnStop;
	CButton	m_btnStart;
	UINT	m_msecsR;
	UINT	m_timeoutR;
	UINT	m_msecsW;
	UINT	m_timeoutW;
	UINT	m_nRead;
	UINT	m_nWrite;
	CString	m_szAvgR;
	CString	m_szAvgW;
	CString	m_szFailedR;
	CString	m_szFailedW;
	CString	m_szMaxR;
	CString	m_szMaxW;
	CString	m_szMinR;
	CString	m_szMinW;
	CString	m_szSuccessR;
	CString	m_szSuccessW;
	CString	m_szTriesR;
	CString	m_szTriesW;
	int		m_nLockType;
	CString	m_szSuccessRateR;
	CString	m_szSuccessRateW;
	UINT	m_duration;
	//}}AFX_DATA

	UINT	m_durationSave;

	// read threads
	DWORD m_nTriesR;
	DWORD m_nSuccessR;
	DWORD m_nFailuresR;
	DWORD m_AvgAccessTimeR;
	DWORD m_TotalAccessTimeR;
	DWORD m_MinAccessTimeR;
	DWORD m_MaxAccessTimeR;

	// write threads
	DWORD m_nTriesW;
	DWORD m_nSuccessW;
	DWORD m_nFailuresW;
	DWORD m_AvgAccessTimeW;
	DWORD m_TotalAccessTimeW;
	DWORD m_MinAccessTimeW;
	DWORD m_MaxAccessTimeW;

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBenchmarkDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CBenchmarkDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnStart();
	afx_msg void OnStop();
	virtual void OnCancel();
	afx_msg void OnShowThreads();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg LRESULT OnUpdateStats(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BENCHMARKDLG_H__5089CC70_2288_46D8_8AAE_A83816B4B4ED__INCLUDED_)
