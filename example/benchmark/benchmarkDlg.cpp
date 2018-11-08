// benchmarkDlg.cpp : implementation file
//

#include "stdafx.h"
#include "benchmark.h"
#include "benchmarkDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

Weakness* Kryptonite = NULL;
/////////////////////////////////////////////////////////////////////////////
// CBenchmarkDlg dialog

CBenchmarkDlg::CBenchmarkDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBenchmarkDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBenchmarkDlg)
	m_nRead = 10;
	m_msecsR = 50;
	m_timeoutR = 125;
	m_nWrite = 1;
	m_msecsW = 10;
	m_timeoutW = 250;
	m_szAvgR = _T("");
	m_szAvgW = _T("");
	m_szFailedR = _T("");
	m_szFailedW = _T("");
	m_szMaxR = _T("");
	m_szMaxW = _T("");
	m_szMinR = _T("");
	m_szMinW = _T("");
	m_szSuccessR = _T("");
	m_szSuccessW = _T("");
	m_szTriesR = _T("");
	m_szTriesW = _T("");
	m_nLockType = SELECT_CONCUR;
	m_szSuccessRateR = _T("");
	m_szSuccessRateW = _T("");
	m_duration = 10;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	// read threads
	m_nTriesR = 0;
	m_nSuccessR = 0;
	m_nFailuresR = 0;
	m_AvgAccessTimeR = 0;
	m_TotalAccessTimeR = 0;
	m_MinAccessTimeR = 0;
	m_MaxAccessTimeR = 0;

	// write threads
	m_nTriesW = 0;
	m_nSuccessW = 0;
	m_nFailuresW = 0;
	m_AvgAccessTimeW = 0;
	m_TotalAccessTimeW = 0;
	m_MinAccessTimeW = 0;
	m_MaxAccessTimeW = 0;

}

void CBenchmarkDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBenchmarkDlg)
	DDX_Control(pDX, IDC_DURATION, m_ctl_duration);
	DDX_Control(pDX, IDC_LOCK_CONCUR, m_ctl_lockConcur);
	DDX_Control(pDX, IDC_LOCK_MUTEX, m_ctl_lockMutex);
	DDX_Control(pDX, IDC_LOCK_CRITSECT, m_ctl_lockCritSect);
	DDX_Control(pDX, IDC_TIMEOUT_W, m_ctl_timeoutW);
	DDX_Control(pDX, IDC_TIMEOUT_R, m_ctl_timeoutR);
	DDX_Control(pDX, IDC_NUM_WRITE_THREADS, m_ctl_nThreadW);
	DDX_Control(pDX, IDC_NUM_READ_THREADS, m_ctl_nThreadR);
	DDX_Control(pDX, IDC_MSECS_W, m_ctl_msecsW);
	DDX_Control(pDX, IDC_MSECS_R, m_ctl_msecsR);
	DDX_Control(pDX, IDC_SHOW_THREADS, m_btnThreadShow);
	DDX_Control(pDX, IDC_STOP, m_btnStop);
	DDX_Control(pDX, IDC_START, m_btnStart);
	DDX_Text(pDX, IDC_MSECS_R, m_msecsR);
	DDV_MinMaxUInt(pDX, m_msecsR, 10, 600000);
	DDX_Text(pDX, IDC_TIMEOUT_R, m_timeoutR);
	DDV_MinMaxUInt(pDX, m_timeoutR, 100, 600000);
	DDX_Text(pDX, IDC_MSECS_W, m_msecsW);
	DDV_MinMaxUInt(pDX, m_msecsW, 10, 600000);
	DDX_Text(pDX, IDC_TIMEOUT_W, m_timeoutW);
	DDV_MinMaxUInt(pDX, m_timeoutW, 100, 600000);
	DDX_Text(pDX, IDC_NUM_READ_THREADS, m_nRead);
	DDV_MinMaxUInt(pDX, m_nRead, 0, 50);
	DDX_Text(pDX, IDC_NUM_WRITE_THREADS, m_nWrite);
	DDV_MinMaxUInt(pDX, m_nWrite, 0, 50);
	DDX_Text(pDX, IDC_S_AVG_R, m_szAvgR);
	DDX_Text(pDX, IDC_S_AVG_W, m_szAvgW);
	DDX_Text(pDX, IDC_S_FAILED_R, m_szFailedR);
	DDX_Text(pDX, IDC_S_FAILED_W, m_szFailedW);
	DDX_Text(pDX, IDC_S_MAX_R, m_szMaxR);
	DDX_Text(pDX, IDC_S_MAX_W, m_szMaxW);
	DDX_Text(pDX, IDC_S_MIN_R, m_szMinR);
	DDX_Text(pDX, IDC_S_MIN_W, m_szMinW);
	DDX_Text(pDX, IDC_S_SUCCESS_R, m_szSuccessR);
	DDX_Text(pDX, IDC_S_SUCCESS_W, m_szSuccessW);
	DDX_Text(pDX, IDC_S_TRIES_R, m_szTriesR);
	DDX_Text(pDX, IDC_S_TRIES_W, m_szTriesW);
	DDX_Radio(pDX, IDC_LOCK_CONCUR, m_nLockType);
	DDX_Text(pDX, IDC_SUCCESS_RATE_R, m_szSuccessRateR);
	DDX_Text(pDX, IDC_SUCCESS_RATE_W, m_szSuccessRateW);
	DDX_Text(pDX, IDC_DURATION, m_duration);
	DDV_MinMaxUInt(pDX, m_duration, 0, 60);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CBenchmarkDlg, CDialog)
	//{{AFX_MSG_MAP(CBenchmarkDlg)
	ON_MESSAGE( WM_UPDATE_TOTALS, OnUpdateStats )
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_START, OnStart)
	ON_BN_CLICKED(IDC_STOP, OnStop)
	ON_BN_CLICKED(IDC_SHOW_THREADS, OnShowThreads)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBenchmarkDlg message handlers

BOOL CBenchmarkDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}


void CBenchmarkDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CBenchmarkDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CBenchmarkDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CBenchmarkDlg::OnStart() 
{
	UpdateData(TRUE);
	m_durationSave = m_duration;

	m_btnStop.EnableWindow(TRUE);
	m_btnStart.EnableWindow(FALSE);
	m_ctl_duration.EnableWindow(FALSE);
	m_btnStop.SetCheck(0);
	m_btnStart.SetCheck(1);

	m_ctl_lockConcur.EnableWindow(FALSE);
	m_ctl_lockMutex.EnableWindow(FALSE);
	m_ctl_lockCritSect.EnableWindow(FALSE);
	m_ctl_nThreadR.EnableWindow(FALSE);
	m_ctl_msecsR.EnableWindow(FALSE);
	m_ctl_timeoutR.EnableWindow(FALSE);
	m_ctl_nThreadW.EnableWindow(FALSE);
	m_ctl_msecsW.EnableWindow(FALSE);
	m_ctl_timeoutW.EnableWindow(FALSE);

	m_szAvgR = _T("");
	m_szAvgW = _T("");
	m_szFailedR = _T("");
	m_szFailedW = _T("");
	m_szMaxR = _T("");
	m_szMaxW = _T("");
	m_szMinR = _T("");
	m_szMinW = _T("");
	m_szSuccessR = _T("");
	m_szSuccessW = _T("");
	m_szTriesR = _T("");
	m_szTriesW = _T("");
	m_szSuccessRateR = _T("");
	m_szSuccessRateW = _T("");

	// read threads
	m_nTriesR = 0;
	m_nSuccessR = 0;
	m_nFailuresR = 0;
	m_AvgAccessTimeR = 0;
	m_TotalAccessTimeR = 0;
	m_MinAccessTimeR = 0;
	m_MaxAccessTimeR = 0;

	// write threads
	m_nTriesW = 0;
	m_nSuccessW = 0;
	m_nFailuresW = 0;
	m_AvgAccessTimeW = 0;
	m_TotalAccessTimeW = 0;
	m_MinAccessTimeW = 0;
	m_MaxAccessTimeW = 0;

	// setup lock type to test
	switch(m_nLockType)
	{
		case SELECT_CONCUR:
			Kryptonite = new Weakness_ConcurrentLock();
			break;
		case SELECT_MUTEX:
			Kryptonite = new Weakness_Mutex();
			break;
		case SELECT_CRITSECT:
			Kryptonite = new Weakness_CritSect();
			break;
	}

	int idx;
	for (idx = 0; idx < m_nRead; idx ++ )
	{
		BenchmarkThread* pThread = new BenchmarkThread(m_msecsR,m_timeoutR,false);
		m_vec_thread.push_back(pThread);
		pThread->startThread();
	}
	for (idx = 0; idx < m_nWrite; idx ++ )
	{
		BenchmarkThread* pThread = new BenchmarkThread(m_msecsW,m_timeoutW,true);
		m_vec_thread.push_back(pThread);
		pThread->startThread();
	}

	if ( m_duration )
	{
		SetTimer(0,1000,NULL);
		SetTimer(1,m_duration*1000,NULL);
	}
}

void CBenchmarkDlg::OnStop() 
{
	while ( !m_vec_thread.empty() )
	{
		BenchmarkThread* pThread = m_vec_thread.back();
		m_vec_thread.pop_back();
		if ( pThread )
		{
			pThread->stopThread();
			delete pThread;
		}
	}
	KillTimer(0);
	KillTimer(1);

	m_btnStop.EnableWindow(FALSE);
	m_btnStart.EnableWindow(TRUE);
	m_ctl_duration.EnableWindow(TRUE);
	m_btnStop.SetCheck(1);
	m_btnStart.SetCheck(0);

	m_ctl_lockConcur.EnableWindow(TRUE);
	m_ctl_lockMutex.EnableWindow(TRUE);
	m_ctl_lockCritSect.EnableWindow(TRUE);
	m_ctl_nThreadR.EnableWindow(TRUE);
	m_ctl_msecsR.EnableWindow(TRUE);
	m_ctl_timeoutR.EnableWindow(TRUE);
	m_ctl_nThreadW.EnableWindow(TRUE);
	m_ctl_msecsW.EnableWindow(TRUE);
	m_ctl_timeoutW.EnableWindow(TRUE);

	if ( Kryptonite )
	{
		delete Kryptonite;
		Kryptonite = NULL;
	}

	m_duration = m_durationSave;
	UpdateData(FALSE);
}

void CBenchmarkDlg::OnCancel() 
{
	OnStop();	
	CDialog::OnCancel();
}

void CBenchmarkDlg::OnShowThreads() 
{
	// TODO: Add your control notification handler code here
	
}

LRESULT CBenchmarkDlg::OnUpdateStats(WPARAM wParam, LPARAM lParam) 
{
	BOOL isWrite = (BOOL) wParam;
	long accessTime = (long) lParam;

	if ( isWrite )
	{
		m_nTriesW ++;
		m_szTriesW.Format(_T("%lu"),m_nTriesW);
		if ( accessTime >= 0 )
		{
			m_nSuccessW ++;
			m_szSuccessW.Format(_T("%lu"),m_nSuccessW);

			m_TotalAccessTimeW += accessTime;
			if ( m_MinAccessTimeW == 0 || accessTime < m_MinAccessTimeW )
			{
				m_MinAccessTimeW = accessTime;
				m_szMinW.Format(_T("%lu"),m_MinAccessTimeW);
			}
			if ( m_MaxAccessTimeW == 0 || accessTime > m_MaxAccessTimeW )
			{
				m_MaxAccessTimeW = accessTime;
				m_szMaxW.Format(_T("%lu"),m_MaxAccessTimeW);
			}
			m_AvgAccessTimeW = m_TotalAccessTimeW / m_nSuccessW;
			m_szAvgW.Format(_T("%lu"),m_AvgAccessTimeW);
		}
		else
		{
			m_nFailuresW ++;
			m_szFailedW.Format(_T("%lu"),m_nFailuresW);
		}
		m_szSuccessRateW.Format(_T("%0.1f %%"),
			100.0 * (double) m_nSuccessW / (double) m_nTriesW );
	}
	else
	{
		m_nTriesR ++;
		m_szTriesR.Format(_T("%lu"),m_nTriesR);
		if ( accessTime >= 0 )
		{
			m_nSuccessR ++;
			m_szSuccessR.Format(_T("%lu"),m_nSuccessR);

			m_TotalAccessTimeR += accessTime;
			if ( m_MinAccessTimeR == 0 || accessTime < m_MinAccessTimeR )
			{
				m_MinAccessTimeR = accessTime;
				m_szMinR.Format(_T("%lu"),m_MinAccessTimeR);
			}
			if ( m_MaxAccessTimeR == 0 || accessTime > m_MaxAccessTimeR )
			{
				m_MaxAccessTimeR = accessTime;
				m_szMaxR.Format(_T("%lu"),m_MaxAccessTimeR);
			}
			m_AvgAccessTimeR = m_TotalAccessTimeR / m_nSuccessR;
			m_szAvgR.Format(_T("%lu"),m_AvgAccessTimeR);
		}
		else
		{
			m_nFailuresR ++;
			m_szFailedR.Format(_T("%lu"),m_nFailuresR);
		}
		m_szSuccessRateR.Format(_T("%0.1f %%"),
			100.0 * (double) m_nSuccessR / (double) m_nTriesR );
	}
	UpdateData(FALSE);
	return 0;
}


void CBenchmarkDlg::OnTimer(UINT nIDEvent) 
{
	switch ( nIDEvent )
	{
		case 0:
			m_duration --;
			UpdateData(FALSE);
			break;
		case 1:
			OnStop();	
			break;
	}
	CDialog::OnTimer(nIDEvent);
}
