#if !defined(AFX_BENCHMARKTHREADDLG_H__39B61862_18A7_4D9B_BE4A_358B284862CA__INCLUDED_)
#define AFX_BENCHMARKTHREADDLG_H__39B61862_18A7_4D9B_BE4A_358B284862CA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BenchmarkThreadDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// BenchmarkThreadDlg dialog

class BenchmarkThreadDlg : public CDialog
{
// Construction
public:
	BenchmarkThreadDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(BenchmarkThreadDlg)
	enum { IDD = IDD_THREAD };
	CButton	m_btnDetails;
	CListCtrl	m_list;
	CString	m_szAvg;
	CString	m_szFailed;
	CString	m_szMax;
	CString	m_szMin;
	CString	m_szSuccess;
	CString	m_szTries;
	//}}AFX_DATA
	CString m_szTitle;
	DWORD m_nTries;
	DWORD m_nSuccess;
	DWORD m_nFailures;
	DWORD m_AvgAccessTime;
	DWORD m_TotalAccessTime;
	DWORD m_MinAccessTime;
	DWORD m_MaxAccessTime;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(BenchmarkThreadDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(BenchmarkThreadDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDetails();
	afx_msg LRESULT OnUpdateWindow(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnAddTimestamp(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUpdateStats(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// BenchmarkThread thread

class BenchmarkThread 
{
    // things we should not allow
    private:
        BenchmarkThread(const BenchmarkThread& rCopy) { throw; };
        BenchmarkThread& operator=(const BenchmarkThread& rCopy) { throw; };
        BenchmarkThread& operator==(const BenchmarkThread& rCopy) { throw; };
        BenchmarkThread& operator!=(const BenchmarkThread& rCopy) { throw; };

    public:
		BenchmarkThread(UINT msecs, UINT timeout, bool isWrite);           // protected constructor used by dynamic creation
		virtual ~BenchmarkThread();

	// Methods
    public:
        HRESULT startThread(void);
        HRESULT stopThread(void);

		void test_CONCUR(Weakness_ConcurrentLock* kNode);
		void test_MUTEX(Weakness_Mutex* kNode);
		void test_CRITSECT(Weakness_CritSect* kNode);

    protected:
        static DWORD WINAPI threadEntryPoint(LPVOID pParam);

	// Attributes
	public:
        HANDLE m_threadHandle;
        DWORD  m_threadID;
		UINT m_delay_msecs;
		UINT m_timeout;
		bool m_writelock;
		BenchmarkThreadDlg* m_pDlg;
		bool m_pendingAbort;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BENCHMARKTHREADDLG_H__39B61862_18A7_4D9B_BE4A_358B284862CA__INCLUDED_)
