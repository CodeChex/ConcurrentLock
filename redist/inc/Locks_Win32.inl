#pragma once

#ifdef IMPL_USING_MFC

typedef class CSyncObject		cclSyncObject;
typedef class CSingleLock		cclSingleLock;
typedef class CMutex			cclMutex;
typedef class CCriticalSection	cclCriticalSection;
typedef class CEvent			cclEvent;

#else

class _CCL_API cclMutex : public cclSyncObject
{
public:
	cclMutex() : cclSyncObject()
	{
		m_mtx = ::CreateMutex(NULL,FALSE,NULL);
	};
	virtual ~cclMutex()
	{
		Unlock();
		if ( m_mtx ) ::CloseHandle(m_mtx);
		m_mtx = NULL;
	};

protected:
	virtual BOOL Lock(DWORD dwTimeout=INFINITE)
	{
		return ( ::WaitForSingleObjectEx(m_mtx,dwTimeout,TRUE) == WAIT_OBJECT_0 );
	}
	virtual BOOL Unlock()
	{
		return ::ReleaseMutex(m_mtx);
	}

protected:
	HANDLE m_mtx;
};

class _CCL_API cclCriticalSection : public cclSyncObject
{
public:
	cclCriticalSection() : cclSyncObject()
	{
		memset(&m_critsec,0,sizeof(m_critsec));
		::InitializeCriticalSection(&m_critsec);
	};
	virtual ~cclCriticalSection()	
	{
		Unlock();
		::DeleteCriticalSection(&m_critsec);
		memset(&m_critsec,0,sizeof(m_critsec));
	};

protected:
	virtual BOOL Lock(DWORD dwTimeout=INFINITE)
	{
		::EnterCriticalSection(&m_critsec);
		return TRUE;
	};
	virtual BOOL Unlock()
	{
		::LeaveCriticalSection(&m_critsec);
		return TRUE;
	}

protected:
	CRITICAL_SECTION m_critsec;
};

class _CCL_API cclEvent
{
public:
	cclEvent()
	{
		m_evt = ::CreateEvent(NULL,FALSE,FALSE,NULL);
	};
	virtual ~cclEvent()
	{
		PulseEvent();
		if ( m_evt ) ::CloseHandle(m_evt);
	};

	BOOL SetEvent()
	{
		return ::SetEvent(m_evt);
	};

	BOOL ResetEvent()
	{
		return ::ResetEvent(m_evt);
	};

	BOOL PulseEvent()
	{
		return ::PulseEvent(m_evt);
	};

	DWORD Wait(DWORD dwTimeout=INFINITE)
	{
		//ResetEvent();
		return ( ::WaitForSingleObjectEx(m_evt,dwTimeout,TRUE) == WAIT_OBJECT_0 );
	};

protected:
	HANDLE m_evt;
};

#endif // _AFXDLL