// BenchmarkThreadDlg.cpp : implementation file
//

#include "stdafx.h"

#include "mmsystem.h"
#pragma comment(lib,"winmm")

#include "benchmark.h"
#include "BenchmarkThreadDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// BenchmarkThreadDlg dialog

BenchmarkThreadDlg::BenchmarkThreadDlg(CWnd* pParent /*=NULL*/)
	: CDialog(BenchmarkThreadDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(BenchmarkThreadDlg)
	m_szAvg = _T("");
	m_szFailed = _T("");
	m_szMax = _T("");
	m_szMin = _T("");
	m_szSuccess = _T("");
	m_szTries = _T("");
	//}}AFX_DATA_INIT
	m_nTries = 0;
	m_nSuccess = 0;
	m_nFailures = 0;
	m_AvgAccessTime = 0;
	m_TotalAccessTime = 0;
	m_MinAccessTime = 0;
	m_MaxAccessTime = 0;
}


void BenchmarkThreadDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(BenchmarkThreadDlg)
	DDX_Control(pDX, IDC_DETAILS, m_btnDetails);
	DDX_Control(pDX, IDC_LIST1, m_list);
	DDX_Text(pDX, IDC_S_AVG, m_szAvg);
	DDX_Text(pDX, IDC_S_FAILED, m_szFailed);
	DDX_Text(pDX, IDC_S_MAX, m_szMax);
	DDX_Text(pDX, IDC_S_MIN, m_szMin);
	DDX_Text(pDX, IDC_S_SUCCESS, m_szSuccess);
	DDX_Text(pDX, IDC_S_TRIES, m_szTries);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(BenchmarkThreadDlg, CDialog)
	//{{AFX_MSG_MAP(BenchmarkThreadDlg)
	ON_MESSAGE( WM_UPDATE_WINDOW, OnUpdateWindow )
	ON_MESSAGE( WM_ADD_TIMESTAMP, OnAddTimestamp )
	ON_MESSAGE( WM_UPDATE_STATS, OnUpdateStats )
	ON_BN_CLICKED(IDC_DETAILS, OnDetails)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// BenchmarkThreadDlg message handlers

BOOL BenchmarkThreadDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	SetWindowText(m_szTitle);
	
	// TODO: Add extra initialization here
	m_list.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	m_list.ModifyStyle(0,LVS_NOSORTHEADER);
	UINT nIdx = 0;

	nIdx = m_list.InsertColumn(nIdx,_T("TimeStamp"),LVCFMT_LEFT,100);
	m_list.SetColumnWidth(nIdx,LVSCW_AUTOSIZE_USEHEADER);

	nIdx = m_list.InsertColumn(nIdx,_T("Access (msecs)"),LVCFMT_LEFT,100);
	m_list.SetColumnWidth(nIdx,LVSCW_AUTOSIZE_USEHEADER);

	m_list.DeleteAllItems();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

LRESULT BenchmarkThreadDlg::OnUpdateWindow(WPARAM wParam, LPARAM lParam) 
{
	SetWindowText(m_szTitle);
	if ( wParam ) 
		ShowWindow(wParam);
	UpdateData(FALSE);
	return 0;
}

LRESULT BenchmarkThreadDlg::OnAddTimestamp(WPARAM wParam, LPARAM lParam) 
{
	CString szTmp;

	szTmp.Format(_T("%lu"),lParam);
	int rIdx = m_list.InsertItem(0,szTmp);
	m_nTries ++;

	if ( (long) wParam >= 0 )
	{
		m_nSuccess ++;
		m_TotalAccessTime += wParam;

		if ( m_MinAccessTime == 0 || wParam < m_MinAccessTime )
			m_MinAccessTime = wParam;

		if ( m_MaxAccessTime == 0 || wParam > m_MaxAccessTime )
			m_MaxAccessTime = wParam;

		m_AvgAccessTime = m_TotalAccessTime / m_nSuccess;

		szTmp.Format(_T("%lu"),wParam);
		SendMessage(WM_UPDATE_STATS,TRUE);
	}
	else
	{
		m_nFailures ++;
		szTmp = _T("* FAILED *");
		SendMessage(WM_UPDATE_STATS,FALSE);
	}
	m_list.SetItemText(rIdx,1,szTmp);

	UpdateData(FALSE);
	return 0;
}

LRESULT BenchmarkThreadDlg::OnUpdateStats(WPARAM wParam, LPARAM lParam) 
{
	m_szTries.Format(_T("%lu"),m_nTries);
	if ( wParam )
	{
		m_szSuccess.Format(_T("%lu"),m_nSuccess);
		m_szAvg.Format(_T("%lu"),m_AvgAccessTime);
	}
	else
	{
		m_szFailed.Format(_T("%lu"),m_nFailures);
	}
	UpdateData(FALSE);
	return 0;
}

void BenchmarkThreadDlg::OnDetails() 
{
	if ( m_list.IsWindowVisible() )
	{
		m_list.ShowWindow(SW_HIDE);
		m_btnDetails.SetWindowText(_T("Show Details..."));
	}
	else
	{
		m_list.ShowWindow(SW_SHOW);
		m_btnDetails.SetWindowText(_T("Hide Details..."));
	}
}

/////////////////////////////////////////////////////////////////////////////
// BenchmarkThread

BenchmarkThread::BenchmarkThread(UINT msecs, UINT timeout, bool isWrite)
{
    m_threadHandle = NULL;
    m_threadID = NULL;

	m_pDlg = new BenchmarkThreadDlg;
	m_pDlg->Create(BenchmarkThreadDlg::IDD,CWnd::GetDesktopWindow());

	m_delay_msecs = msecs;
	m_timeout = timeout;
	m_writelock = isWrite;

	m_pendingAbort = false;
}

BenchmarkThread::~BenchmarkThread()
{
    stopThread();
	if ( WaitForSingleObject(m_threadHandle,5000) == WAIT_TIMEOUT )
	{
		TerminateThread(m_threadHandle,0xff);
	}

	// TODO:  perform any per-thread cleanup here
	if ( m_pDlg )
	{
		if ( IsWindow(m_pDlg->m_hWnd) )	
		{
			m_pDlg->DestroyWindow();
		}
		delete m_pDlg;
		m_pDlg = NULL;
	}
}

/////////////////////////////////////////////////////////////////////////////
// BenchmarkThread message handlers

DWORD WINAPI BenchmarkThread::threadEntryPoint(LPVOID pParam)
{
    BenchmarkThread* pThread = static_cast<BenchmarkThread*>(pParam);
    UINT rc = 0;
	UINT lockRC = 0;
    if ( pThread )
    {
		pThread->m_pDlg->m_szTitle.Format(_T("Benchmark Thread: %lu (%s)"),
			GetCurrentThreadId(), 
			pThread->m_writelock ? _T("Write") : _T("Read") );

		pThread->m_pDlg->PostMessage(WM_UPDATE_WINDOW,0,0);

		while ( Kryptonite && !pThread->m_pendingAbort )
		{
			Weakness_ConcurrentLock* K_CONCUR	= dynamic_cast<Weakness_ConcurrentLock*>(Kryptonite);
			Weakness_Mutex*			 K_MUTEX	= dynamic_cast<Weakness_Mutex*>(Kryptonite);
			Weakness_CritSect*		 K_CRITSECT = dynamic_cast<Weakness_CritSect*>(Kryptonite);
			if ( K_CONCUR )
				pThread->test_CONCUR(K_CONCUR);
			else if ( K_MUTEX )
				pThread->test_MUTEX(K_MUTEX);
			else if ( K_CRITSECT )
				pThread->test_CRITSECT(K_CRITSECT);
		}
    }
	CloseHandle(pThread->m_threadHandle);
    pThread->m_threadHandle = NULL;
    pThread->m_threadID = NULL;

    return rc;
}

void BenchmarkThread::test_CONCUR(Weakness_ConcurrentLock* kNode)
{
	if ( kNode )
	{
		DWORD dwTimeBegin = timeGetTime();
		CONCUR_DECLARE_RESOURCE_LOCK(kNode,_T("Kryptonite CONCURRENT_LOCK"));
		if ( m_writelock )
		{
			// access a resource in a DESTRUCTIVE manner
			if ( !CONCUR_RESOURCE_WRITE_LOCK(kNode,m_timeout) )
			{
				TRACE(_T("Thread:[%u] CONCURRENT_LOCK WRITE-LOCK timeout\n"),m_threadID);
				m_pDlg->PostMessage(WM_ADD_TIMESTAMP,-1,dwTimeBegin);
				AfxGetApp()->GetMainWnd()->PostMessage(WM_UPDATE_TOTALS,m_writelock,-1);
				return;
			}
			else
			{
				kNode->countWrite ++;
			}
		}
		else
		{
			// access a resource in a NON-DESTRUCTIVE manner
			if ( !CONCUR_RESOURCE_READ_LOCK(kNode,m_timeout) )
			{
				TRACE(_T("Thread:[%u] CONCURRENT_LOCK READ-LOCK timeout\n"),m_threadID);
				m_pDlg->PostMessage(WM_ADD_TIMESTAMP,-1,dwTimeBegin);
				AfxGetApp()->GetMainWnd()->PostMessage(WM_UPDATE_TOTALS,m_writelock,-1);
				return;
			}
			else
			{
				kNode->countRead ++;
			}
		}
		DWORD dwDeltaTime = timeGetTime() - dwTimeBegin;
		m_pDlg->PostMessage(WM_ADD_TIMESTAMP,dwDeltaTime,dwTimeBegin);
		AfxGetApp()->GetMainWnd()->PostMessage(WM_UPDATE_TOTALS,m_writelock,dwDeltaTime);
		Sleep(m_delay_msecs);
		CONCUR_RESOURCE_UNLOCK(kNode);
	}
}

void BenchmarkThread::test_MUTEX(Weakness_Mutex* kNode)
{
	if ( kNode )
	{
		DWORD dwTimeBegin = timeGetTime();
		MUTEX_DECLARE_RESOURCE_LOCK(kNode,"Kryptonite MUTEX");
		if ( m_writelock )
		{
			// access a resource in a DESTRUCTIVE manner
			if ( !MUTEX_RESOURCE_WRITE_LOCK(kNode,m_timeout) )
			{
				TRACE(_T("Thread:[%u] MUTEX WRITE-LOCK timeout\n"),m_threadID);
				m_pDlg->PostMessage(WM_ADD_TIMESTAMP,-1,dwTimeBegin);
				AfxGetApp()->GetMainWnd()->PostMessage(WM_UPDATE_TOTALS,m_writelock,-1);
				return;
			}
			else
			{
				kNode->countWrite ++;
			}
		}
		else
		{
			// access a resource in a NON-DESTRUCTIVE manner
			if ( !MUTEX_RESOURCE_READ_LOCK(kNode,m_timeout) )
			{
				TRACE(_T("Thread:[%u] MUTEX READ-LOCK timeout\n"),m_threadID);
				m_pDlg->PostMessage(WM_ADD_TIMESTAMP,-1,dwTimeBegin);
				AfxGetApp()->GetMainWnd()->PostMessage(WM_UPDATE_TOTALS,m_writelock,-1);
				return;
			}
			else
			{
				kNode->countRead ++;
			}
		}
		DWORD dwDeltaTime = timeGetTime() - dwTimeBegin;
		m_pDlg->PostMessage(WM_ADD_TIMESTAMP,dwDeltaTime,dwTimeBegin);
		AfxGetApp()->GetMainWnd()->PostMessage(WM_UPDATE_TOTALS,m_writelock,dwDeltaTime);
		Sleep(m_delay_msecs);
		MUTEX_RESOURCE_UNLOCK(kNode);
	}
}

void BenchmarkThread::test_CRITSECT(Weakness_CritSect* kNode)
{
	if ( kNode )
	{
		DWORD dwTimeBegin = timeGetTime();
		CRITSECT_DECLARE_RESOURCE_LOCK(kNode,"Kryptonite CRITICAL_SECTION");
		if ( m_writelock )
		{
			// access a resource in a DESTRUCTIVE manner
			if ( !CRITSECT_RESOURCE_WRITE_LOCK(kNode,m_timeout) )
			{
				TRACE(_T("Thread:[%u] CRITICAL_SECTION WRITE-LOCK timeout\n"),m_threadID);
				m_pDlg->PostMessage(WM_ADD_TIMESTAMP,-1,dwTimeBegin);
				AfxGetApp()->GetMainWnd()->PostMessage(WM_UPDATE_TOTALS,m_writelock,-1);
				return;
			}
			else
			{
				kNode->countWrite ++;
			}
		}
		else
		{
			// access a resource in a NON-DESTRUCTIVE manner
			if ( !CRITSECT_RESOURCE_READ_LOCK(kNode,m_timeout) )
			{
				TRACE(_T("Thread:[%u] CRITICAL_SECTION READ-LOCK timeout\n"),m_threadID);
				m_pDlg->PostMessage(WM_ADD_TIMESTAMP,-1,dwTimeBegin);
				AfxGetApp()->GetMainWnd()->PostMessage(WM_UPDATE_TOTALS,m_writelock,-1);
				return;
			}
			else
			{
				kNode->countRead ++;
			}
		}
		DWORD dwDeltaTime = timeGetTime() - dwTimeBegin;
		m_pDlg->PostMessage(WM_ADD_TIMESTAMP,dwDeltaTime,dwTimeBegin);
		AfxGetApp()->GetMainWnd()->PostMessage(WM_UPDATE_TOTALS,m_writelock,dwDeltaTime);
		Sleep(m_delay_msecs);
		CRITSECT_RESOURCE_UNLOCK(kNode);
	}
}

HRESULT BenchmarkThread::startThread(void)
{
    HRESULT rc = E_FAIL;
    if ( !m_threadHandle )
    {
		m_pendingAbort = false;
        m_threadHandle = CreateThread(NULL,
                                    0,
                                    BenchmarkThread::threadEntryPoint,
                                    this,
                                    0,
                                    &m_threadID);
    }
    return rc;
}

HRESULT BenchmarkThread::stopThread(void)
{
    HRESULT rc = E_FAIL;
  	// terminate thread
	if ( m_threadHandle )
	{
		m_pendingAbort = true;
	}
    return rc;
}




