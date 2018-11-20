/****************************************************************************
 *                                                                          *
 *                        Copyright 1996-2001                               *
 *                     Checco Computer Services                             *
 *                        All Rights Reserved.                              *
 *                                                                          *
 *     This software contains proprietary and confidential information      *
 *     belonging to Checco Computer Services, and may not be decompiled,    *
 *     disassembled, disclosed, reproduced or copied without the prior      *
 *     written consent of Checco Computer Services.                         *
 *                                                                          *
 ****************************************************************************
 * ConcurrentLock PUBLIC header                                             *
 ****************************************************************************

    These SyncObjects allow true multi-threaded 
        CONCURRENT READ and EXCLUSIVE WRITE access to any class.

 ****************************************************************************

    HISTORY:
    -------

    Until now, you were told that all class methods should access/update their members
    in a thread safe manner, so like the rest of us sheep, you created a member variable:

        CMutex m_mutex
        
    and wrapped every class method with:

        CSingleLock mSingleLock(&m_mutex);
        if(!mSingleLock.Lock(timeout))
        {
            // log some error
            return E_FAIL;
        }

    In a really complex multithreaded application, this causes EXCLUSIVE access to that
    class, even if the method does not update any member variables! This results in a
    class ementation that only allows SERIAL access, not really what was meant by
    the multithreading capabilities of OO design.
    
    Indeed, locking is needed for read-only methods that use items such as STL iterators, 
    whereas a call to a destructive class method from another thread may render that 
    iterator invalid.

    What was really needed was a way for you to allow multiple threads CONCURRENT READ
    access to the class, and EXCLUSIVE WRITE... 

    Hence, "ConcurrentSemaphore" and "ConcurrentLock" !

 ****************************************************************************

    MACRO DEFINITIONS:
    -----------------

    *** Use of these macros is highly recommended because if you decide to revert back
        to using critical sections, then "#define USE_CRITSECT" before including this
        header, and the macros translate to use CCriticalSection / CSingleLock !!!

    MACRO USAGE:
    -----------

    In your class, insert this line prefereably at the top of bottom of the class:
        (because it declares a scope of public)

        DECLARE_LOCKABLE;
     
    to obtain a resource for ANY LOCKABLE class object:

        DECLARE_RESOURCE_LOCK(object,"any debug string");

    to access a resource in a NON-DESTRUCTIVE manner

        if( !RESOURCE_READ_LOCK(object,timeout) )
        {
            // log some error
        }

    to access a resource in a DESTRUCTIVE manner

        if( !RESOURCE_WRITE_LOCK(object,timeout) )
        {
            // log some error
        }

    to relinqish access to the resource

        RESOURCE_UNLOCK(object);

 ****************************************************************************

    NATIVE USAGE:
    ------------

    In your class, insert these lines:

        public: // use "private" or "protected" if no other classes lock this resource
            ConcurrentSemaphore mCritSec;
        
    to obtain a resource for the CURRENT class object:

        ConcurrentLock mSingleLock(&mCritSec);

    to obtain a resource for ANOTHER class object:

        ConcurrentLock mSingleLock(&(object->mCritSec)); // only if variable is declared public

    to access a resource in a NON-DESTRUCTIVE manner

        if(mSingleLock.Lock(access_shared,timeout) != WAIT_OBJECT_0)
        {
            // log some error
        }

    to access a resource in a DESTRUCTIVE manner

        if(mSingleLock.Lock(access_thread_exclusive,timeout) != WAIT_OBJECT_0)
        {
            // log some error
        }

    to relinqish access to the resource

        mSingleLock.UnLock();


 ****************************************************************************

    TIPS:   to be most efficient,
    ----

        1) start with read access immediately before referencing a member, 
        2) ask for write access immediately before updating member,
        3) relinquish control when resource is no longer referenced

 ****************************************************************************

    EXAMPLE:
    -------

        //
        // 1) start with read access immediately before referencing a member
        //
        DECLARE_RESOURCE_LOCK(this,"example function");
        if( !RESOURCE_READ_LOCK(this,timeout) )
        {
            // log some error
            // don't forget to cleanup
            return E_FAIL;
        }
        some_iterator iter;
        for ( iter = someList.begin(); iter != someList.end(); iter ++ )
        {
            if ( (*iter) == someElement )
            {
                //
                // 2) ask for write access immediately before updating member,
                //
                if( !RESOURCE_WRITE_LOCK(this,timeout) )
                {
                    // log some error
                    // don't forget to cleanup
                    return E_FAIL;
                }
                someList.erase(iter);
                //
                // !!! since list is modified, leave the loop !!!
                //
                break;
            }
        }
        //
        // 3) relinquish control when resource is no longer referenced
        //
        RESOURCE_UNLOCK(this);
        return S_OK;
  
 ****************************************************************************

    HOW does it work?
    ----------------

    Imagine, if you will, that the ladies' room in your office is broken, and everyone
    must wait in line for using the men's room (which has many urinals, but only one
    stall with a toilet).

    Many men can use the urinals at the same time, but if a woman wants to use the
    toilet, she must wait for all the men to finish their business and leave the room.

    Any remaining men in line must wait for any woman occupying the bathroom to finish 
    her business and leave the room.

    Since there is only one toilet, each woman must wait for the room to be empty before
    entering.

    There are some special conditions though:

      If a man wants to use the urinal, and the only people in there are family members
      (resources from the same thread), he can enter.

      If a man wants to use the toilet, he must go back in line (although he is allowed to
      be placed in front) and wait for everyone else to leave.
    
    (Draw it on paper, it works.)

    Every use of urinal represents READONLY access, and every use of the toilet represents
    WRITEABLE access to the resource.

 *****************************************************************************/


#ifndef _ConcurrentLock_H
#define _ConcurrentLock_H

///////////////////////////////////////////////////////////////////////////
// Forward Declarations
class ConcurrentSemaphore;
class ConcurrentLock;

///////////////////////////////////////////////////////////////////////////
//  Operating System Dependent Settings
#define ccl_DLL_BASE	"ConcurrentLock"
#if defined(_UNICODE)
  #define ccl_DLL_UNI	"U"
#else
  #define ccl_DLL_UNI	""
#endif
#if defined(_DEBUG)
   #define ccl_DLL_DBG	"_D"
#else
   #define ccl_DLL_DBG	""
#endif
#define	ccl_DLL_NAME	ccl_DLL_BASE ccl_DLL_UNI ccl_DLL_DBG

#ifdef _WIN32
	// Class Linking Definitions
    #if !defined(_CCL_API)
       #define _CCL_API _declspec(dllimport)
       #pragma comment(lib,ccl_DLL_NAME)
 	   #pragma message("...Importing " ccl_DLL_NAME)
    #else
 	   #pragma message("...Building for Win32 ... " ccl_DLL_NAME)
    #endif
#else // !_WIN32
	#pragma message("...Building for UNIX..." ccl_DLL_NAME)
    #define _CCL_API    // ensure this is defined properly
	#include <stl.h>
	#include <pthread.h>
	#include <time.h>
	#include <sys/time.h>
	#include <unistd.h>
	typedef unsigned long ULONG;
	typedef ULONG DWORD;
	typedef unsigned int UINT;
	typedef UINT BOOL;
	typedef char TCHAR;
	typedef char *LPTSTR;
	typedef const char *LPCTSTR;
	#define _T(x)      x
   #include <glib.h>
   #include <unistd.h>
   #include <time.h>
   #include <sys/time.h>
   #define ASSERT(x)	g_assert(x)
   #define _ASSERTE(x)	g_assert(x)
   #define GetCurrentThreadId	pthread_self
   #define _tcscpy(s1,s2)	strcpy(s1,s2)
   #define _tcsncpy(s1,s2,n)	strncpy(s1,s2,n)
   #define Sleep(ms)		usleep(ms*1000)
   #define INFINITE	(0xFFFFFFFF)

#endif // !_WIN32

class _CCL_API cclSyncObject
{
	friend class cclSingleLock;
public:
	cclSyncObject() { };
	virtual ~cclSyncObject() {};

private:
	virtual BOOL Lock(DWORD dwTimeout=INFINITE) = 0;
	virtual BOOL Unlock() = 0;
};

class _CCL_API cclSingleLock
{
// Constructors
public:
	cclSingleLock(cclSyncObject* pObject, BOOL bInitialLock = FALSE)
	{
		m_pObject = pObject;
		m_bAcquired = false;
		if ( bInitialLock )	Lock();
		//fprintf(stderr,"+++ cclSingleLock:cclSingleLock(0x%p) = %lu\n",m_pObject,m_bAcquired);
	};

// Operations
public:
	BOOL Lock(DWORD dwTimeout = INFINITE)
	{
		if ( m_pObject )
		{
			if ( !m_bAcquired )
			{
	//fprintf(stderr,"  > cclSingleLock:Lock(0x%p,%ld) = %lu\n",m_pObject,dwTimeout,m_bAcquired);
				m_bAcquired = m_pObject->Lock(dwTimeout);
	//fprintf(stderr,"  < cclSingleLock:Lock(0x%p,%ld) = %lu\n",m_pObject,dwTimeout,m_bAcquired);
			}
		}
		return m_bAcquired;
	};

	BOOL Unlock()
	{
		if ( m_bAcquired )
		{
	//fprintf(stderr,"  > cclSingleLock:Unlock(0x%p) = %lu\n",m_pObject,!m_bAcquired);
			if ( m_pObject->Unlock() ) m_bAcquired = false;
	//fprintf(stderr,"  < cclSingleLock:Unlock(0x%p) = %lu\n",m_pObject,!m_bAcquired);
		}
		return ( !m_bAcquired );
	};

	BOOL IsLocked()
	{
		return m_bAcquired;
	};

// Implementation
public:
	~cclSingleLock()
	{
		Unlock();
		//fprintf(stderr,"--- cclSingleLock:~cclSingleLock(0x%p) = %lu\n",m_pObject,m_bAcquired);
	};

protected:
	cclSyncObject* m_pObject;
	BOOL    m_bAcquired;
};

#ifdef _WIN32
#include "Locks_Win32.inl"
#else // !_WIN32
#include "Locks_Unix.inl"
#endif // !_WIN32

///////////////////////////////////////////////////////////////////////////
// Default constants
#ifdef _DEBUG
    #define DEFAULT_LOCK_TIMEOUT     1000
#else
    #define DEFAULT_LOCK_TIMEOUT     500 // INFINITE
#endif

///////////////////////////////////////////////////////////////////////////
// cclCriticalSection / cclSingleLock MACROS

#define CRITSECT_DECLARE_LOCKABLE                 public: cclCriticalSection m_ResourceSem;
#define CRITSECT_DECLARE_RESOURCE_LOCK(o,desc)      cclSingleLock myLock_##o(&(o->m_ResourceSem)); \
		//fprintf(stderr,"CRITSECT_DECLARE_RESOURCE_LOCK(%s)\n",desc);
#define CRITSECT_RESOURCE_READ_LOCK(o,timeout)      (myLock_##o.IsLocked() || myLock_##o.Lock(INFINITE) == TRUE) 
#define CRITSECT_RESOURCE_WRITE_LOCK(o,timeout)     (myLock_##o.IsLocked() || myLock_##o.Lock(INFINITE) == TRUE)
#define CRITSECT_RESOURCE_DELETE_LOCK(o,timeout)    (myLock_##o.IsLocked() || myLock_##o.Lock(INFINITE) == TRUE)
#define CRITSECT_RESOURCE_UNLOCK(o)                 myLock_##o.Unlock();

#define CRITSECT_DECLARE_ELEMENT_LOCKABLE(e)        public: cclCriticalSection m_ElementSem_##e;
#define CRITSECT_DECLARE_ELEMENT_LOCK(o,e,desc)     cclSingleLock myLock_##o_##e(&(o->m_ElementSem_##e)); \
		//fprintf(stderr,"CRITSECT_DECLARE_ELEMENT_LOCK(%s)\n",desc);
#define CRITSECT_ELEMENT_READ_LOCK(o,e,timeout)     (myLock_##o_##e.IsLocked() || myLock_##o_##e.Lock(INFINITE) == TRUE)
#define CRITSECT_ELEMENT_WRITE_LOCK(o,e,timeout)    (myLock_##o_##e.IsLocked() || myLock_##o_##e.Lock(INFINITE) == TRUE)
#define CRITSECT_ELEMENT_DELETE_LOCK(o,e,timeout)   (myLock_##o_##e.IsLocked() || myLock_##o_##e.Lock(INFINITE) == TRUE)
#define CRITSECT_ELEMENT_UNLOCK(o,e)                myLock_##o_##e.Unlock();

///////////////////////////////////////////////////////////////////////////
// cclMutex / cclSingleLock MACROS

#define MUTEX_DECLARE_LOCKABLE                public: cclMutex m_ResourceSem;
#define MUTEX_DECLARE_RESOURCE_LOCK(o,desc)     cclSingleLock myLock_##o(&(o->m_ResourceSem)); \
		//fprintf(stderr,"MUTEX_DECLARE_RESOURCE_LOCK(%s)\n",desc);
#define MUTEX_RESOURCE_READ_LOCK(o,timeout)     (myLock_##o.IsLocked() || myLock_##o.Lock(timeout) == TRUE)
#define MUTEX_RESOURCE_WRITE_LOCK(o,timeout)    (myLock_##o.IsLocked() || myLock_##o.Lock(timeout) == TRUE)
#define MUTEX_RESOURCE_DELETE_LOCK(o,timeout)   (myLock_##o.IsLocked() || myLock_##o.Lock(timeout) == TRUE)
#define MUTEX_RESOURCE_UNLOCK(o)                myLock_##o.Unlock();

#define MUTEX_DECLARE_ELEMENT_LOCKABLE(e)        public: cclMutex m_ElementSem_##e;
#define MUTEX_DECLARE_ELEMENT_LOCK(o,e,desc)     cclSingleLock myLock_##o_##e(&(o->m_ElementSem_##e)); \
		//fprintf(stderr,"MUTEX_DECLARE_ELEMENT_LOCK(%s)\n",desc);
#define MUTEX_ELEMENT_READ_LOCK(o,e,timeout)     (myLock_##o_##e.IsLocked() || myLock_##o_##e.Lock(timeout) == TRUE)
#define MUTEX_ELEMENT_WRITE_LOCK(o,e,timeout)    (myLock_##o_##e.IsLocked() || myLock_##o_##e.Lock(timeout) == TRUE)
#define MUTEX_ELEMENT_DELETE_LOCK(o,e,timeout)   (myLock_##o_##e.IsLocked() || myLock_##o_##e.Lock(timeout) == TRUE)
#define MUTEX_ELEMENT_UNLOCK(o,e)                myLock_##o_##e.Unlock();

///////////////////////////////////////////////////////////////////////////
// ConcurrentSemaphore / ConcurrentLock MACROS

#define CONCUR_DECLARE_LOCKABLE               public: ConcurrentSemaphore m_ResourceSem;
#define CONCUR_DECLARE_RESOURCE_LOCK(o,desc)    ConcurrentLock myLock_##o(&(o->m_ResourceSem),desc); _e_AccessStatus rcLock_##o; \
	//fprintf(stderr,"CONCUR_DECLARE_RESOURCE_LOCK(%s)\n",desc);
#define CONCUR_RESOURCE_READ_LOCK(o,timeout)    ((rcLock_##o = myLock_##o.Lock(access_shared,timeout)) == request_granted)
#define CONCUR_RESOURCE_WRITE_LOCK(o,timeout)   ((rcLock_##o = myLock_##o.Lock(access_thread_exclusive,timeout)) == request_granted)
#define CONCUR_RESOURCE_DELETE_LOCK(o,timeout)  ((rcLock_##o = myLock_##o.Lock(access_resource_exclusive,timeout)) == request_granted)
#define CONCUR_RESOURCE_UNLOCK(o)               myLock_##o.Unlock();

#define CONCUR_DECLARE_ELEMENT_LOCKABLE(e)         public: ConcurrentSemaphore m_ElementSem_##e;
#define CONCUR_DECLARE_ELEMENT_LOCK(o,e,desc)      ConcurrentLock myLock_##o_##e(&(o->m_ElementSem_##e),desc); _e_AccessStatus rcLock_##o_##e; \
		//fprintf(stderr,"CONCUR_DECLARE_ELEMENT_LOCK(%s)\n",desc);
#define CONCUR_ELEMENT_READ_LOCK(o,e,timeout)      ((rcLock_##o_##e = myLock_##o_##e.Lock(access_shared,timeout)) == request_granted)
#define CONCUR_ELEMENT_WRITE_LOCK(o,e,timeout)     ((rcLock_##o_##e = myLock_##o_##e.Lock(access_thread_exclusive,timeout)) == request_granted)
#define CONCUR_ELEMENT_DELETE_LOCK(o,e,timeout)    ((rcLock_##o_##e = myLock_##o_##e.Lock(access_resource_exclusive,timeout)) == request_granted)
#define CONCUR_ELEMENT_UNLOCK(o,e)                 myLock_##o_##e.Unlock();

///////////////////////////////////////////////////////////////////////////
// NOOP MACROS

#define NOOP_DECLARE_LOCKABLE                          
#define NOOP_DECLARE_RESOURCE_LOCK(o,desc)   
#define NOOP_RESOURCE_READ_LOCK(o,timeout)		(true)
#define NOOP_RESOURCE_WRITE_LOCK(o,timeout)		(true)
#define NOOP_RESOURCE_DELETE_LOCK(o,timeout)	(true)
#define NOOP_RESOURCE_UNLOCK(o)                       

#define NOOP_DECLARE_ELEMENT_LOCKABLE(e)         
#define NOOP_DECLARE_ELEMENT_LOCK(o,e,desc)     
#define NOOP_ELEMENT_READ_LOCK(o,e,timeout)     (true) 
#define NOOP_ELEMENT_WRITE_LOCK(o,e,timeout)    (true)
#define NOOP_ELEMENT_DELETE_LOCK(o,e,timeout)   (true)
#define NOOP_ELEMENT_UNLOCK(o,e)                    

///////////////////////////////////////////////////////////////////////////
// internal macros
#define INTERNAL_DECLARE_LOCKABLE                CRITSECT_DECLARE_LOCKABLE            
#define INTERNAL_DECLARE_RESOURCE_LOCK(o,desc)   CRITSECT_DECLARE_RESOURCE_LOCK(o,desc) 
#define INTERNAL_RESOURCE_READ_LOCK(o,timeout)   CRITSECT_RESOURCE_READ_LOCK(o,timeout) 
#define INTERNAL_RESOURCE_WRITE_LOCK(o,timeout)  CRITSECT_RESOURCE_WRITE_LOCK(o,timeout)
#define INTERNAL_RESOURCE_DELETE_LOCK(o,timeout) CRITSECT_RESOURCE_DELETE_LOCK(o,timeout)
#define INTERNAL_RESOURCE_UNLOCK(o)              CRITSECT_RESOURCE_UNLOCK(o)            

#define INTERNAL_DECLARE_ELEMENT_LOCKABLE(e)      CRITSECT_DECLARE_ELEMENT_LOCKABLE(e)     
#define INTERNAL_DECLARE_ELEMENT_LOCK(o,e,desc)   CRITSECT_DECLARE_ELEMENT_LOCK(o,e,desc)  
#define INTERNAL_ELEMENT_READ_LOCK(o,e,timeout)   CRITSECT_ELEMENT_READ_LOCK(o,e,timeout)  
#define INTERNAL_ELEMENT_WRITE_LOCK(o,e,timeout)  CRITSECT_ELEMENT_WRITE_LOCK(o,e,timeout) 
#define INTERNAL_ELEMENT_DELETE_LOCK(o,e,timeout) CRITSECT_ELEMENT_DELETE_LOCK(o,e,timeout)
#define INTERNAL_ELEMENT_UNLOCK(o,e)              CRITSECT_ELEMENT_UNLOCK(o,e)             

///////////////////////////////////////////////////////////////////////////
// Type Declarations 
typedef _CCL_API enum
{
    access_none,
    access_shared,
    access_thread_exclusive,
    access_resource_exclusive
} _e_AccessType;

typedef _CCL_API enum
{
    request_aborted,
    request_denied,
    request_granted,
} _e_AccessStatus;

typedef _CCL_API enum
{
    error_none,
    error_sem_invalid,          // semaphore was not valid
    error_request_invalid,      // requesting lock was not on the request list
    error_request_deadlock,     // requesting lock would cause deadlock
    error_pending_any,          // at least one lock is pending
    error_pending_exclusive,    // an exclusive lock is pending
    error_access_any,           // at least one lock already has access
    error_access_exclusive,     // an exclusive lock already has access
    error_access_outside,       // an outside lock already has access
    error_access_nested,        // an nested lock already has access
} _e_AccessError;

///////////////////////////////////////////////////////////////////////////
// Typedef Declarations (for class)
#pragma warning(disable:4786)
#include <vector>
#pragma warning(disable:4786)
#include <iterator>
#pragma warning(disable:4786)
using namespace std;

// our lists are so small that the performance is not affected
typedef vector < ConcurrentLock* > _t_ConcurrentLockList;
typedef _t_ConcurrentLockList::iterator _t_ConcurrentLockIter;
typedef insert_iterator < _t_ConcurrentLockList > _t_ConcurrentLockInsert;

#define LOCK_DEBUGINFO_SZ   80

///////////////////////////////////////////////////////////////////////////
// Class Definitions

/*
 *  CLASS: ConcurrentLock
 *
 *  DESCRIPTION:    
 *      This is a base class meant to provide the interface for the 
 *      ConcurrentLock objects.
 *
 *  PUBLIC VARIABLES: 
 *
 *  CONSTRUCTOR/DESTRUCTOR FUNCTIONS: 
 *
 *  PUBLIC MEMBER FUNCTIONS:
 *
 *  PROTECTED MEMBER FUNCTIONS: 
 *
 *  SEE ALSO:
 *
 *  OTHER:
 *
 *  Miscellaneous:
 *
 */
class _CCL_API ConcurrentLock
{
    friend class ConcurrentSemaphore;

	// constructor/destructor
    public:
        ConcurrentLock(ConcurrentSemaphore* pSem,LPCTSTR pszDebugInfo=NULL);
        virtual ~ConcurrentLock();

	// Methods
    public:
        _e_AccessStatus Lock(_e_AccessType access,DWORD timeout=DEFAULT_LOCK_TIMEOUT);
        void  Unlock(void);

    public:
        inline DWORD getThreadId(void)
            { return m_threadId; };
        inline _e_AccessStatus getStatus(void) 
            { return m_status; };
        inline _e_AccessType getAccess(void)
            { return m_access; };
        inline _e_AccessError getLastError(void)
            { return m_lastError; };
        inline DWORD getTimeout(void)
            { return m_timeout; };
        inline ConcurrentSemaphore* getSem(void)
            { return m_pSem; };
        bool isValid(void);

    // Attributes
    private:
        ConcurrentSemaphore* m_pSem;
#ifdef _WIN32
        DWORD m_threadId;
#else
        pthread_t m_threadId;
#endif	// _WIN32
        _e_AccessType m_access;
        _e_AccessStatus m_status;
        _e_AccessError m_lastError;
        DWORD m_timeout;
        // debug info and statistics
        TCHAR m_szDebugInfo[LOCK_DEBUGINFO_SZ];
};

/*
 *  CLASS: ConcurrentSemaphore
 *
 *  DESCRIPTION:    
 *      This is a base class meant to provide the interface for the 
 *      ConcurrentSemaphore objects.
 *
 *  PUBLIC VARIABLES: 
 *
 *  CONSTRUCTOR/DESTRUCTOR FUNCTIONS: 
 *
 *  PUBLIC MEMBER FUNCTIONS:
 *
 *  PROTECTED MEMBER FUNCTIONS: 
 *
 *  SEE ALSO:
 *
 *  OTHER:
 *
 *  Miscellaneous:
 *
 */
class _CCL_API ConcurrentSemaphore
{
    friend class ConcurrentLock;

	INTERNAL_DECLARE_LOCKABLE	// doctor, heal thyself
	// constructor/destructor
    public:
        ConcurrentSemaphore();
        virtual ~ConcurrentSemaphore();

	// Methods
    public:
        bool isValid(void);

        inline _e_AccessType getCurrentAccess(void) 
            { return m_access; };
        inline unsigned getAccessCount(void) 
            { return m_ownerList.size(); };
        inline unsigned getRequestCount(void) 
            { return m_requestList.size(); };

        // debug info and statistics
        void SetDebugInfo(LPCTSTR pszDebugInfo);
        void DebugResetStats(void);
        void DebugPrintStats(DWORD onlyWaitsGreaterOrEqualThan);

    private:
        _e_AccessStatus Lock(ConcurrentLock* pRequester,_e_AccessType reqAccess);
        bool Unlock(ConcurrentLock* pRequester);
		// deadlock checks
		bool isLockGrantedForThread(DWORD dwThread);
		bool isLockPendingForThread(DWORD dwThread);
		bool simpleDeadLock(DWORD dwPendingThread);
		bool complexDeadLock(DWORD dwPendingThread,ULONG nestLevel=0);

	// Methods
    private:
        ConcurrentLock* getCurrentLock(void);
        bool queryLock(_t_ConcurrentLockList& lockList,ConcurrentLock* pLock,bool erase=false);
        bool addLock(_t_ConcurrentLockList& lockList,ConcurrentLock* pLock);
        bool pendingAny(ConcurrentLock* pLock);
        bool pendingShared(ConcurrentLock* pLock);
        bool pendingExclusive(ConcurrentLock* pLock);
        ConcurrentLock* nestedOwnership(ConcurrentLock* pLock);
        ConcurrentLock* outsideOwnership(ConcurrentLock* pLock);
        void resetAccess(void);
        _e_AccessStatus allowShared(ConcurrentLock* pRequester);
        _e_AccessStatus allowExclusive(ConcurrentLock* pRequester,bool restrict=false);

	// Attributes
    private:
        bool mValid;
		// used for member list updates
#ifdef _WIN32
        DWORD m_threadId;
#else
        pthread_t m_threadId;
#endif	// _WIN32
        cclEvent*      pUpdateEvent;            // used when member list is updated

        _t_ConcurrentLockList m_requestList;
        _t_ConcurrentLockList m_ownerList;
        _e_AccessType m_access;

        // debug info and statistics
        TCHAR m_szDebugInfo[LOCK_DEBUGINFO_SZ];

        DWORD m_overlappedLocks;
        DWORD m_overlappedDepthMax;

        DWORD m_sharedGrants, m_sharedDenies, m_sharedAborts;
        DWORD m_sharedWaitMax;
        TCHAR m_szSharedWaitMax[LOCK_DEBUGINFO_SZ];

        DWORD m_threadGrants, m_threadDenies, m_threadAborts;
        DWORD m_threadWaitMax;
        TCHAR m_szThreadWaitMax[LOCK_DEBUGINFO_SZ];

        DWORD m_resourceGrants, m_resourceDenies, m_resourceAborts;
        DWORD m_resourceWaitMax;
        TCHAR m_szResourceWaitMax[LOCK_DEBUGINFO_SZ];
};

///////////////////////////////////////////////////////////////////////////
inline LPTSTR describeAccessType(_e_AccessType val)
{
    switch (val)
    {
        case access_none:
            return _T("none");
            break;
        case access_shared:
            return _T("read");
            break;
        case access_thread_exclusive:
            return _T("write");
            break;
        case access_resource_exclusive:
            return _T("delete");
            break;
    }
    return _T("???");
}

inline LPTSTR describeAccessStatus(_e_AccessStatus val)
{
    switch (val)
    {
        case request_granted:
            return _T("granted");
            break;
        case request_denied:
            return _T("denied");
            break;
        case request_aborted:
            return _T("aborted");
            break;
    }
    return _T("???");
}

inline LPTSTR describeAccessError(_e_AccessError val)
{
    switch (val)
    {
        case error_none:
            return _T("none");
            break;
        case error_request_invalid:
            return _T("requesting lock was not on the request list");
            break;
        case error_request_deadlock:
            return _T("requesting lock would cause deadlock");
            break;
        case error_sem_invalid:
            return _T("semaphore was no valid");
            break;
        case error_pending_any:
            return _T("at least one lock is pending");
            break;
        case error_pending_exclusive:
            return _T("an exclusive lock is pending");
            break;
        case error_access_any:
            return _T("at least one lock already has access");
            break;
        case error_access_exclusive:
            return _T("an exclusive lock already has access");
            break;
        case error_access_outside:
            return _T("an outside lock already has access");
            break;
        case error_access_nested:
            return _T("an nested lock already has access");
            break;
    }
    return _T("???");
}

#endif  // _ConcurrentLock_H
