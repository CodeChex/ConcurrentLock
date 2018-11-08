#pragma once

#define pt_timeout(var,msec) \
struct timespec var; \
{ \
	struct timeval pt_now; \
	gettimeofday(&pt_now,NULL); \
	pt_now.tv_usec += (msec) * 1000; \
        if (pt_now.tv_usec >= 1000000) \
        { \
	   pt_now.tv_sec ++; \
	   pt_now.tv_usec -= 1000000; \
        } \
	TIMEVAL_TO_TIMESPEC(&pt_now,&var); \
}

inline DWORD timeGetTime()	
{
	struct timeval pt_now;
	gettimeofday(&pt_now,NULL);
	DWORD rc = ((pt_now.tv_sec*1000) + (pt_now.tv_usec/1000));
	return rc;
}

class _CCL_API cclMutex : public cclSyncObject
{
public:
	cclMutex(int mtxkind=PTHREAD_MUTEX_RECURSIVE_NP)
	{
		memset(&m_mtx,0,sizeof(m_mtx));
	    pthread_mutexattr_t tmAttr;
		pthread_mutexattr_init(&tmAttr);
		tmAttr.__mutexkind = mtxkind;
		pthread_mutex_init(&m_mtx,&tmAttr);
		pthread_mutexattr_destroy(&tmAttr);
	};

	virtual ~cclMutex()
	{
		Unlock();
		pthread_mutex_destroy(&m_mtx);
	};

protected:
	virtual BOOL Lock(DWORD dwTimeout=INFINITE)
	{
		if ( dwTimeout != INFINITE )
		{
   #ifdef __USE_XOPEN2K
			pt_timeout(v_timeout,dwTimeout);
			return ( pthread_mutex_timedlock(&m_mtx,&v_timeout) == 0 );
   #else
			DWORD dwTimeBegin = timeGetTime();
			do
			{
				if ( pthread_mutex_trylock(&m_mtx) == 0 ) break;
				// Sleep(1);	// give up timeslice
			} while ((timeGetTime()-dwTimeBegin) < dwTimeout);
        	return ( pthread_mutex_trylock(&m_mtx) == 0 );
   #endif
		}
		else 
		{
			return ( pthread_mutex_lock(&m_mtx) == 0 );
		}
		return FALSE;
	}

	virtual BOOL Unlock()
	{
		return ( pthread_mutex_unlock(&m_mtx) == 0 );
	}

protected:
	pthread_mutex_t m_mtx;
};

class _CCL_API cclCriticalSection : public cclMutex
{
public:
	cclCriticalSection() : cclMutex(PTHREAD_MUTEX_RECURSIVE_NP) {};
	virtual ~cclCriticalSection()	{};

protected:
	virtual BOOL Lock(DWORD dwTimeout=INFINITE)
	{
		return cclMutex::Lock(INFINITE);
	};
};

class _CCL_API cclEvent : public cclMutex
{
public:
	cclEvent() : cclMutex(PTHREAD_MUTEX_RECURSIVE_NP)
	{
		memset(&m_cond,0,sizeof(m_cond));
		pthread_cond_init(&m_cond,NULL);
	};

	virtual ~cclEvent() 
	{
		PulseEvent();
	};

	BOOL SetEvent()
	{
		Lock();
		BOOL rc = (pthread_cond_broadcast(&m_cond) == 0);
		Unlock();
		return rc;
	};

	BOOL ResetEvent()
	{
		return true;
	};

	BOOL PulseEvent()
	{
		BOOL rc = SetEvent();
		ResetEvent();
		return rc;
	};

	DWORD Wait(DWORD dwTimeout=INFINITE)
	{
		DWORD rc = 0xFFFFFFFF;
		Lock();
		if ( dwTimeout != INFINITE )
		{
		    pt_timeout(v_timeout,dwTimeout);
            rc = pthread_cond_timedwait(&m_cond,&m_mtx,&v_timeout);
		}
		else
		{
			rc = pthread_cond_wait(&m_cond,&m_mtx);
		}
		Unlock();
	};

protected:
	pthread_cond_t m_cond;
};

#define WAIT_OBJECT_0	0
#define WAIT_TIMEOUT	(!WAIT_OBJECT_0)
#define WaitForSingleObject(evt,timeout)	evt->Wait(timeout)
