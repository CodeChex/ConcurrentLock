// BenchmarkThreadDlg.cpp : implementation file
//

#include "benchmark.h"
#include "benchmarkDlg.h"

#ifdef _WIN32
#include "mmsystem.h"
#pragma comment(lib,"winmm")
#endif

/////////////////////////////////////////////////////////////////////////////
// BenchmarkThreadDlg dialog

BenchmarkThreadDlg::BenchmarkThreadDlg()
{
	m_nTries = 0;
	m_nSuccess = 0;
	m_nFailures = 0;
	m_AvgAccessTime = 0;
	m_TotalAccessTime = 0;
	m_MinAccessTime = 0;
	m_MaxAccessTime = 0;
}

void BenchmarkThreadDlg::OnAddTimestamp(long accessTime, unsigned long timestamp) 
{
	m_nTries ++;

	if ( accessTime >= 0 )
	{
		m_nSuccess ++;
		m_TotalAccessTime += accessTime;

		if ( m_MinAccessTime == 0 || accessTime < m_MinAccessTime )
			m_MinAccessTime = accessTime;

		if ( m_MaxAccessTime == 0 || accessTime > m_MaxAccessTime )
			m_MaxAccessTime = accessTime;

		m_AvgAccessTime = m_TotalAccessTime / m_nSuccess;
	}
	else
	{
		m_nFailures ++;
	}
}

/////////////////////////////////////////////////////////////////////////////
// BenchmarkThread

BenchmarkThread::BenchmarkThread(UINT msecs, UINT timeout, bool isWrite)
{
    m_threadHandle = 0;
    m_threadID = 0;

	m_pDlg = new BenchmarkThreadDlg;

	m_delay_msecs = msecs;
	m_timeout = timeout;
	m_writelock = isWrite;

	m_pendingAbort = false;
}

BenchmarkThread::~BenchmarkThread()
{
    stopThread();

#ifdef _WIN32
	if ( WaitForSingleObject(m_threadHandle,5000) == WAIT_TIMEOUT )
	{
		TerminateThread(m_threadHandle,0xff);
	}
#else
	// zzz
#endif

	if ( m_pDlg )
	{
		delete m_pDlg;
		m_pDlg = NULL;
	}
}

/////////////////////////////////////////////////////////////////////////////
// BenchmarkThread message handlers

THREADRET WINAPI BenchmarkThreadEntryPoint(LPVOID pParam)
{
    BenchmarkThread* pThread = static_cast<BenchmarkThread*>(pParam);
    UINT lockRC = 0;
    if ( pThread )
    {
		pThread->m_threadID = GetCurrentThreadId();
	_ftprintf(stderr,_T("\nStarted THREAD [%08lx]..."), GetCurrentThreadId());
		while ( Kryptonite && !pThread->m_pendingAbort )
		{
			Weakness_ConcurrentLock* K_CONCUR = NULL;
			Weakness_Mutex*	K_MUTEX	= NULL;
			Weakness_CritSect* K_CRITSECT = NULL;
			switch ( CentralBooking.m_nLockType )
			{
			   case SELECT_CONCUR:
			      K_CONCUR = static_cast<Weakness_ConcurrentLock*>(Kryptonite);
			      break;
			   case SELECT_MUTEX:
			      K_MUTEX = static_cast<Weakness_Mutex*>(Kryptonite);
			      break;
			   case SELECT_CRITSECT:
			      K_CRITSECT = static_cast<Weakness_CritSect*>(Kryptonite);
			      break;
			}
			if ( K_CONCUR )
				pThread->test_CONCUR(K_CONCUR);
			else if ( K_MUTEX )
				pThread->test_MUTEX(K_MUTEX);
			else if ( K_CRITSECT )
				pThread->test_CRITSECT(K_CRITSECT);
		}
	_ftprintf(stderr,_T("\nStopping THREAD [%08lx]..."), GetCurrentThreadId());
    }
#ifdef _WIN32
	CloseHandle(pThread->m_threadHandle);
#endif
    pThread->m_threadHandle = 0;
    pThread->m_threadID = 0;

    return NULL;
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
				CentralBooking.updateStats(m_writelock,-1);
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
				CentralBooking.updateStats(m_writelock,-1);
				return;
			}
			else
			{
				kNode->countRead ++;
			}
		}
		DWORD dwEndtime = timeGetTime();
		DWORD dwDeltaTime = dwEndtime - dwTimeBegin;
		CentralBooking.updateStats(m_writelock,dwDeltaTime);
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
				CentralBooking.updateStats(m_writelock,-1);
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
				CentralBooking.updateStats(m_writelock,-1);
				return;
			}
			else
			{
				kNode->countRead ++;
			}
		}
		DWORD dwDeltaTime = timeGetTime() - dwTimeBegin;
		CentralBooking.updateStats(m_writelock,dwDeltaTime);
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
				CentralBooking.updateStats(m_writelock,-1);
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
				CentralBooking.updateStats(m_writelock,-1);
				return;
			}
			else
			{
				kNode->countRead ++;
			}
		}
		DWORD dwDeltaTime = timeGetTime() - dwTimeBegin;
		CentralBooking.updateStats(m_writelock,dwDeltaTime);
		Sleep(m_delay_msecs);
		CRITSECT_RESOURCE_UNLOCK(kNode);
	}
}

DWORD BenchmarkThread::startThread(void)
{
    DWORD rc = 0;
    if ( !m_threadHandle )
    {
		m_pendingAbort = false;
#ifdef _WIN32
        m_threadHandle = CreateThread(NULL,
                                    0,
                                    BenchmarkThreadEntryPoint,
                                    this,
                                    0,
                                    &m_threadID);
#else
	pthread_create ( &m_threadHandle, NULL, BenchmarkThreadEntryPoint, this );
#endif
    }
    return rc;
}

DWORD BenchmarkThread::stopThread(void)
{
    DWORD rc = 0;
  	// terminate thread
	if ( m_threadHandle )
	{
		m_pendingAbort = true;
	}
    return rc;
}




