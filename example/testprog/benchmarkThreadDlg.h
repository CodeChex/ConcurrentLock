// BenchmarkThreadDlg.h : header file
//

class BenchmarkThreadDlg 
{
// Construction
public:
	BenchmarkThreadDlg();   // standard constructor

	DWORD m_nTries;
	DWORD m_nSuccess;
	DWORD m_nFailures;
	DWORD m_AvgAccessTime;
	DWORD m_TotalAccessTime;
	DWORD m_MinAccessTime;
	DWORD m_MaxAccessTime;

	void OnAddTimestamp(long accessTime, unsigned long timestamp);
};

/////////////////////////////////////////////////////////////////////////////
// BenchmarkThread thread

class BenchmarkThread 
{
    public:
		BenchmarkThread(UINT msecs, UINT timeout, bool isWrite);           // protected constructor used by dynamic creation
		virtual ~BenchmarkThread();

	// Methods
    public:
        DWORD startThread(void);
        DWORD stopThread(void);

		void test_CONCUR(Weakness_ConcurrentLock* kNode);
		void test_MUTEX(Weakness_Mutex* kNode);
		void test_CRITSECT(Weakness_CritSect* kNode);

	// Attributes
	public:
#ifdef _WIN32
        HANDLE m_threadHandle;
#else
        pthread_t m_threadHandle;
#endif
        DWORD  m_threadID;
		UINT m_delay_msecs;
		UINT m_timeout;
		bool m_writelock;
		BenchmarkThreadDlg* m_pDlg;
		bool m_pendingAbort;
};

#ifdef _WIN32
	#define THREADRET DWORD
#else
	#define THREADRET LPVOID
#endif

extern THREADRET WINAPI BenchmarkThreadEntryPoint(LPVOID pParam);

