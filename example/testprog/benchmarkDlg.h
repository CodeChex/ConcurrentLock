// benchmarkDlg.h : header file
//

#include "benchmarkThreadDlg.h"
#include <list> 
typedef list<BenchmarkThread*> _t_vec_thread;

/////////////////////////////////////////////////////////////////////////////

class CBenchmarkDlg
{
public:
	CBenchmarkDlg();
	~CBenchmarkDlg();

	void START();
	void STOP();

	void initStats();
	void dumpStats(BOOL reset=true);
	void updateStats(BOOL isWrite, long accessTime);

	_t_vec_thread m_vec_thread;

	UINT	m_nRead;
	UINT	m_msecsR;
	UINT	m_timeoutR;

	UINT	m_nWrite;
	UINT	m_msecsW;
	UINT	m_timeoutW;

	int	m_nLockType;

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
};

extern CBenchmarkDlg CentralBooking;
