// benchmarkDlg.cpp : implementation file
//

#include "benchmark.h"
#include "benchmarkDlg.h"

Weakness* Kryptonite = NULL;
/////////////////////////////////////////////////////////////////////////////
// CBenchmarkDlg dialog

CBenchmarkDlg::CBenchmarkDlg()
{
	m_nLockType = SELECT_CONCUR;
	m_nRead = 4;
	m_msecsR = 10;
	m_timeoutR = 250;
	m_nWrite = 2;
	m_msecsW = 25;
	m_timeoutW = 250;

	initStats();
}

void CBenchmarkDlg::START()
{
	initStats();

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
}

void CBenchmarkDlg::initStats()
{

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

CBenchmarkDlg::~CBenchmarkDlg()
{
	STOP();
}
 
void CBenchmarkDlg::STOP() 
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

	if ( Kryptonite )
	{
		delete Kryptonite;
		Kryptonite = NULL;
	}
}

void CBenchmarkDlg::updateStats(BOOL isWrite, long accessTime) 
{
	if ( isWrite )
	{
		m_nTriesW ++;
		if ( accessTime >= 0 )
		{
			m_nSuccessW ++;

			m_TotalAccessTimeW += accessTime;
			if ( m_MinAccessTimeW == 0 || accessTime < m_MinAccessTimeW )
			{
				m_MinAccessTimeW = accessTime;
			}
			if ( m_MaxAccessTimeW == 0 || accessTime > m_MaxAccessTimeW )
			{
				m_MaxAccessTimeW = accessTime;
			}
			m_AvgAccessTimeW = m_TotalAccessTimeW / m_nSuccessW;
		}
		else
		{
			m_nFailuresW ++;
		}
	}
	else
	{
		m_nTriesR ++;
		if ( accessTime >= 0 )
		{
			m_nSuccessR ++;

			m_TotalAccessTimeR += accessTime;
			if ( m_MinAccessTimeR == 0 || accessTime < m_MinAccessTimeR )
			{
				m_MinAccessTimeR = accessTime;
			}
			if ( m_MaxAccessTimeR == 0 || accessTime > m_MaxAccessTimeR )
			{
				m_MaxAccessTimeR = accessTime;
			}
			m_AvgAccessTimeR = m_TotalAccessTimeR / m_nSuccessR;
		}
		else
		{
			m_nFailuresR ++;
		}
	}
}

void CBenchmarkDlg::dumpStats(BOOL reset)
{
   _ftprintf(stderr,_T("\n------------------------------------------\n"));
   // read threads
   if (m_nRead)
   {
	_ftprintf(stderr,_T("\nREAD Thread Status: %0.1f %%"),
		m_nTriesR ? 
			100.0 * (double) m_nSuccessR / (double) m_nTriesR 
			: 0.0 );
	_ftprintf(stderr,_T("\n\t Tries:[%6lu], Success:[%6lu], Failed:[%6lu]"),
		m_nTriesR,m_nSuccessR,m_nFailuresR);
	_ftprintf(stderr,_T("\n\t   Min:[%6lu],     Max:[%6lu],    Avg:[%6lu]"),
		m_MinAccessTimeR, m_MaxAccessTimeR, m_AvgAccessTimeR );
   }
   if (m_nWrite)
   {
	// write threads
	_ftprintf(stderr,_T("\nWRITE Thread Status: %0.1f %%"),
		m_nTriesW ? 
			100.0 * (double) m_nSuccessW / (double) m_nTriesW
			: 0.0 );
	_ftprintf(stderr,_T("\n\t Tries:[%6lu], Success:[%6lu], Failed:[%6lu]"),
		m_nTriesW,m_nSuccessW,m_nFailuresW);
	_ftprintf(stderr,_T("\n\t   Min:[%6lu],     Max:[%6lu],    Avg:[%6lu]"),
		m_MinAccessTimeW, m_MaxAccessTimeW, m_AvgAccessTimeW );
   }
   if ( reset ) initStats();
   _ftprintf(stderr,_T("\n------------------------------------------\n"));
}

