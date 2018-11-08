/****************************************************************************/
/*                                                                          */
/*                          Copyright 2000                                  */
/*                     Checco Computer Services                             */
/*                        All Rights Reserved.                              */
/*                                                                          */
/*     This software contains proprietary and confidential information      */
/*     belonging to Checco Computer Services, and may not be decompiled,    */
/*     disassembled, disclosed, reproduced or copied without the prior      */
/*     written consent of Checco Computer Services.                         */
/*                                                                          */
/****************************************************************************/

/*
 *  PROJECT:		
 *  SUBSYSTEM:		
 *
 *  DESCRIPTION:	ConcurrentSemaphore base class.
 *
 *  SEE ALSO:
 *
 *  OTHER:
 *
 *    Miscellaneous:
 */

#include "ConcurrentLock_Private.h"

//////////////////////////////////////////////////////////////////////////////
#include <map>
#pragma warning(disable:4786)
using namespace std;

typedef vector < ConcurrentSemaphore* > _t_ConcurrentSemList;
typedef _t_ConcurrentSemList::iterator _t_ConcurrentSemIter;

typedef map < DWORD, _t_ConcurrentSemList > _t_ThreadLockList;
typedef _t_ThreadLockList::iterator _t_ThreadLockIter;

static _t_ThreadLockList g_ThreadLockGranted;
static _t_ThreadLockList g_ThreadLockPending;

///////////////////////////////////////////////////////////////////////////
//  Operating System Dependent Settings
const BOOL MANUAL_WAIT       = TRUE;
const DWORD INTERNAL_TIMEOUT = INFINITE;
const DWORD TRACE_WAITS      = 0;
const DWORD TRACE_ERRORS     = 0;
const BOOL DEADLOCK_CHECK    = FALSE;
const DWORD DEADLOCK_NEST    = 5;

//////////////////////////////////////////////////////////////////////////////
bool ConcurrentSemaphore::isLockPendingForThread(DWORD dwThread)
{
	_t_ConcurrentSemList* gThreadList = &g_ThreadLockPending[dwThread];
	_t_ConcurrentSemIter iterP;
	// STEP 1:
	//	loop through all pending locks in dwThread
	for ( iterP = gThreadList->begin(); iterP != gThreadList->end(); iterP ++ )
	{
		// ok, dwThread has a pending reference to this
		if ( (*iterP) == this )
		{
			return true;
		}
	}
	return false;
}

bool ConcurrentSemaphore::isLockGrantedForThread(DWORD dwThread)
{
	_t_ConcurrentSemList* gThreadList = &g_ThreadLockGranted[dwThread];
	 _t_ConcurrentSemIter iterG;
	// STEP 1:
	//	loop through all granted locks in dwThread
	for ( iterG = gThreadList->begin(); iterG != gThreadList->end(); iterG ++ )
	{
		// ok, dwThread has a granted reference to this
		if ( (*iterG) == this )
		{
			return true;
		}
	}
	return false;
}

bool ConcurrentSemaphore::simpleDeadLock(DWORD dwPendingThread)
{
   if ( DEADLOCK_CHECK )
   {
	// STEP 1: 
	//	loop through all threads that have granted locks
	for ( _t_ThreadLockIter iterG = g_ThreadLockGranted.begin(); 
			iterG != g_ThreadLockGranted.end(); 
			iterG ++ )
	{
		DWORD dwGrantedThread = (*iterG).first;
		// skip ourselves, of course
		if ( dwGrantedThread == dwPendingThread ) 
		{
			continue;
		}
		// STEP 2:
		//	loop through all granted locks in dwGrantedThread
		//	to find any cross references to this pending lock
		if ( isLockGrantedForThread(dwGrantedThread) )
		{
			// ok, dwGrantedThread has a cross reference 
			//	loop through all pending locks in dwGrantThread
			_t_ConcurrentSemList* gThreadList = &g_ThreadLockPending[dwGrantedThread];
			_t_ConcurrentSemIter iterP ;
			for ( iterP = gThreadList->begin(); iterP != gThreadList->end(); iterP ++ )
			{
				//	check for reverse reference of any pending locks
				ConcurrentSemaphore* grantedSem = (*iterP);
				if ( grantedSem->isLockGrantedForThread(dwPendingThread) )
				{
					// yes, we have a simple deadlock
					TRACE(_T("\n:!!! SimpleDeadLock between \n")
						  _T("\t: Request(TID=%08lx, SEM=%p, \"%s\")\n")
						  _T("\t: Suspect(TID=%08lx, SEM=%p, \"%s\")\n"),
							dwPendingThread, this,	m_szDebugInfo,
							dwGrantedThread, grantedSem, grantedSem->m_szDebugInfo
						 );
					return true;
				}
			}
		}
	}
   }
   return false;
}

bool ConcurrentSemaphore::complexDeadLock(DWORD dwPendingThread,ULONG nestLevel)
{
   if ( DEADLOCK_CHECK )
   {
	if ( nestLevel > DEADLOCK_NEST ) return false;
	// STEP 1: 
	//	loop through all threads that have granted locks
	for ( _t_ThreadLockIter iterG = g_ThreadLockGranted.begin(); 
			iterG != g_ThreadLockGranted.end(); 
			iterG ++ )
	{
		DWORD dwGrantedThread = (*iterG).first;
		// skip ourselves, of course
		if ( dwGrantedThread == dwPendingThread ) 
		{
			continue;
		}
		// STEP 2:
		//	loop through all granted locks in dwGrantedThread
		//	to find any cross references to this pending lock
		if ( isLockGrantedForThread(dwGrantedThread) )
		{
			// ok, dwGrantedThread has a cross reference 
			//	loop through all pending locks in dwGrantThread
			_t_ConcurrentSemList* gThreadList = &g_ThreadLockPending[dwGrantedThread];
			_t_ConcurrentSemIter iterP ;
			for ( iterP = gThreadList->begin(); iterP != gThreadList->end(); iterP ++ )
			{
				//	check for reverse reference of any pending locks
				ConcurrentSemaphore* grantedSem = (*iterP);
				if ( grantedSem->isLockGrantedForThread(dwPendingThread) )
				{
					// yes, we have a simple deadlock
					TRACE(_T("\n:!!! SimpleDeadLock between \n")
						  _T("\t: Request(TID=%08lx, SEM=%p, \"%s\")\n")
						  _T("\t: Suspect(TID=%08lx, SEM=%p, \"%s\")\n"),
							dwPendingThread, this,	m_szDebugInfo,
							dwGrantedThread, grantedSem, grantedSem->m_szDebugInfo
						 );
					return true;
				}
				else if ( grantedSem->complexDeadLock(dwPendingThread,nestLevel+1) )
				{
					return true;
				}
			}
		}
	}
   }
   return false;
}

//////////////////////////////////////////////////////////////////////////////
ConcurrentSemaphore::ConcurrentSemaphore() 
{
    mValid = false;
    memset(m_szDebugInfo,0,LOCK_DEBUGINFO_SZ);
    m_threadId = GetCurrentThreadId();
    m_access = access_none;

	if ( MANUAL_WAIT )
		pUpdateEvent = NULL;
	else
		pUpdateEvent = new cclEvent;

    mValid = true;
    DebugResetStats();
}

ConcurrentSemaphore::~ConcurrentSemaphore() 
{
	//fprintf(stderr,"  > ConcurrentSemaphore::~ConcurrentSemaphore()\n");
    if ( !isValid() )
    {
        TRACE(_T("!!! OBJECT_NOT_VALID: ConcurrentSemaphore::~ConcurrentSemaphore\n"));
        return;
    }
	INTERNAL_DECLARE_RESOURCE_LOCK(this,"ConcurrentSemaphore::~ConcurrentSemaphore")
    if ( !INTERNAL_RESOURCE_DELETE_LOCK(this,INFINITE) )
    {
        TRACE(_T("!!! INTERNAL_ERROR: ConcurrentSemaphore::~ConcurrentSemaphore\n"));
		_ASSERTE ( simpleDeadLock(GetCurrentThreadId()) == false );
        return;
    }
    if ( TRACE_WAITS )
        DebugPrintStats(TRACE_WAITS);

    mValid = false;

    m_access = access_none;
    ConcurrentLock* pLock;
    _t_ConcurrentLockIter iter;

    while( !m_requestList.empty() )
    {
        iter = m_requestList.begin();
        pLock = (*iter);
        pLock->m_timeout = 0;
        pLock->m_access = access_none;
        pLock->m_pSem = NULL;
        TRACE(_T("~ConcurrentSemaphore: Clearing REQUEST (info=%s) from thread (%lu)\n"), 
            pLock->m_szDebugInfo, pLock->m_threadId );
        m_requestList.erase(iter);
    }

    while( !m_ownerList.empty() )
    {
        iter = m_ownerList.begin();
        pLock = (*iter);
        pLock->m_timeout = 0;
        pLock->m_access = access_none;
        pLock->m_pSem = NULL;
        TRACE(_T("~ConcurrentSemaphore: Clearing OWNER (info=%s) from thread (%lu)\n"), 
            pLock->m_szDebugInfo, pLock->m_threadId );
        m_ownerList.erase(iter);
    }

    if (pUpdateEvent)
	{
        delete(pUpdateEvent);
		pUpdateEvent = NULL;
	}
};

//////////////////////////////////////////////////////////////////////////////
void ConcurrentSemaphore::SetDebugInfo(LPCTSTR pszDebugInfo)
{
	//fprintf(stderr,"  > ConcurrentSemaphore::SetDebugInfo()\n");
    if ( !isValid() )
    {
        //TRACE(_T("!!! OBJECT_NOT_VALID: ConcurrentSemaphore::SetDebugInfo\n"));
        return;
    }
	INTERNAL_DECLARE_RESOURCE_LOCK(this,"ConcurrentSemaphore::SetDebugInfo")
    if ( !INTERNAL_RESOURCE_WRITE_LOCK(this,INTERNAL_TIMEOUT) )
    {
        //TRACE(_T("!!! INTERNAL_ERROR: ConcurrentSemaphore::SetDebugInfo\n"));
		_ASSERTE ( simpleDeadLock(GetCurrentThreadId()) == false );
        return;
    }
    memset(m_szDebugInfo,0,LOCK_DEBUGINFO_SZ);
    if ( pszDebugInfo ) _tcsncpy(m_szDebugInfo,pszDebugInfo,LOCK_DEBUGINFO_SZ-1);
}

void ConcurrentSemaphore::DebugPrintStats(DWORD onlyWaitsGreaterOrEqualThan)
{
	//fprintf(stderr,"  > ConcurrentSemaphore::DebugPrintStats()\n");
    if ( !isValid() )
    {
        TRACE(_T("!!! OBJECT_NOT_VALID: ConcurrentSemaphore::DebugPrintStats\n"));
        return;
    }
	INTERNAL_DECLARE_RESOURCE_LOCK(this,"ConcurrentSemaphore::DebugPrintStats")
    if ( !INTERNAL_RESOURCE_READ_LOCK(this,INTERNAL_TIMEOUT) )
    {
        TRACE(_T("!!! INTERNAL_ERROR: ConcurrentSemaphore::DebugPrintStats\n"));
		_ASSERTE ( simpleDeadLock(GetCurrentThreadId()) == false );
        return;
    }
    TRACE(_T("??? ConcurrentSem(%p,\"%s\")\n"),
            this, m_szDebugInfo );

    TRACE(_T("\t:Efficiency     :  Overlaps(%lu), maxDepth(%lu)\n"),
            m_overlappedLocks, m_overlappedDepthMax );

    if ( m_sharedWaitMax >= onlyWaitsGreaterOrEqualThan )
    {
        TRACE(_T("\t:Shared   Access:  Granted(%lu) Denied(%lu) Aborted(%lu) MaxWait(%lu,\"%s\")\n"),
                m_sharedGrants, m_sharedDenies, m_sharedAborts, 
                m_sharedWaitMax, m_szSharedWaitMax );
    }
    if ( m_threadWaitMax >= onlyWaitsGreaterOrEqualThan )
    {
        TRACE(_T("\t:Thread   Access:  Granted(%lu) Denied(%lu) Aborted(%lu) MaxWait(%lu,\"%s\")\n"),
                m_threadGrants, m_threadDenies, m_threadAborts,
                m_threadWaitMax, m_szThreadWaitMax );
    }
    if ( m_resourceWaitMax >= onlyWaitsGreaterOrEqualThan )
    {
        TRACE(_T("\t:Resource Access:  Granted(%lu) Denied(%lu) Aborted(%lu) MaxWait(%lu,\"%s\")\n"),
                m_resourceGrants, m_resourceDenies, m_resourceAborts, 
                m_resourceWaitMax, m_szResourceWaitMax );
    }
    DebugResetStats();
}

void ConcurrentSemaphore::DebugResetStats(void)
{
	//fprintf(stderr,"  > ConcurrentSemaphore::DebugResetStats()\n");
    if ( !isValid() )
    {
        TRACE(_T("!!! OBJECT_NOT_VALID: ConcurrentSemaphore::DebugResetStats\n"));
        return;
    }
	INTERNAL_DECLARE_RESOURCE_LOCK(this,"ConcurrentSemaphore::DebugResetStats")
    if ( !INTERNAL_RESOURCE_WRITE_LOCK(this,INTERNAL_TIMEOUT) )
    {
        TRACE(_T("!!! INTERNAL_ERROR: ConcurrentSemaphore::DebugResetStats\n"));
		_ASSERTE ( simpleDeadLock(GetCurrentThreadId()) == false );
        return;
    }
    m_overlappedLocks  = m_overlappedDepthMax = 0;
    m_sharedGrants     = m_threadGrants     = m_resourceGrants     = 0;
    m_sharedDenies     = m_threadDenies     = m_resourceDenies     = 0;
    m_sharedAborts     = m_threadAborts     = m_resourceAborts     = 0;
    m_sharedWaitMax    = m_threadWaitMax    = m_resourceWaitMax    = 0;
    memset(m_szSharedWaitMax,0,LOCK_DEBUGINFO_SZ);
    memset(m_szThreadWaitMax,0,LOCK_DEBUGINFO_SZ);
    memset(m_szResourceWaitMax,0,LOCK_DEBUGINFO_SZ);
}

//////////////////////////////////////////////////////////////////////////////
bool ConcurrentSemaphore::addLock(_t_ConcurrentLockList& lockList,ConcurrentLock* pLock)
{
	//fprintf(stderr,"  > ConcurrentSemaphore::addLock()\n");
    if ( !isValid() )
    {
        TRACE(_T("!!! OBJECT_NOT_VALID: ConcurrentSemaphore::addLock\n"));
        return false;
    }
	INTERNAL_DECLARE_RESOURCE_LOCK(this,"ConcurrentSemaphore::addLock")
    if ( !INTERNAL_RESOURCE_WRITE_LOCK(this,INTERNAL_TIMEOUT) )
    {
        TRACE(_T("!!! INTERNAL_ERROR: ConcurrentSemaphore::addLock\n"));
		_ASSERTE ( simpleDeadLock(GetCurrentThreadId()) == false );
        return false;
    }
    if ( !pLock->isValid() )
    {
        TRACE(_T("!!! REQUESTER_NOT_VALID: ConcurrentSemaphore::addLock\n"));
        return false;
    }
    lockList.push_back(pLock);

    if (pUpdateEvent) pUpdateEvent->SetEvent();

	if ( DEADLOCK_CHECK )
	{
		// added to check for deadlocks
		_t_ConcurrentSemList* gThreadList = ( &lockList == &m_ownerList )
												? &g_ThreadLockGranted[pLock->m_threadId]
												: &g_ThreadLockPending[pLock->m_threadId];
		gThreadList->push_back(this);
	}
    return true;
}

bool ConcurrentSemaphore::queryLock(_t_ConcurrentLockList& lockList,ConcurrentLock* pLock,bool erase)
{
	//fprintf(stderr,"  > ConcurrentSemaphore::queryLock()\n");
    if ( !isValid() )
    {
        TRACE(_T("!!! OBJECT_NOT_VALID: ConcurrentSemaphore::queryLock\n"));
        return false;
    }
	INTERNAL_DECLARE_RESOURCE_LOCK(this,"ConcurrentSemaphore::queryLock")
    if ( !INTERNAL_RESOURCE_READ_LOCK(this,INTERNAL_TIMEOUT) )
    {
        TRACE(_T("!!! INTERNAL_ERROR: ConcurrentSemaphore::queryLock\n"));
		_ASSERTE ( simpleDeadLock(GetCurrentThreadId()) == false );
        return false;
    }
    /*
    if ( !pLock->isValid() )
    {
        TRACE(_T("!!! REQUESTER_NOT_VALID: ConcurrentSemaphore::queryLock\n"));
        return false;
    }
    */
    for ( _t_ConcurrentLockIter iter = lockList.begin(); iter != lockList.end(); iter ++ )
    {
        if ( (*iter) == pLock ) 
        {
            if ( erase )
            {
				INTERNAL_RESOURCE_WRITE_LOCK(this,INFINITE);
                lockList.erase(iter);
				if (pUpdateEvent) pUpdateEvent->SetEvent();

				if ( DEADLOCK_CHECK )
				{
					// added to check for deadlocks
					_t_ConcurrentSemList* gThreadList = ( &lockList == &m_ownerList )
															? &g_ThreadLockGranted[pLock->m_threadId]
															: &g_ThreadLockPending[pLock->m_threadId];
					_t_ConcurrentSemIter iterG;
					for ( iterG = gThreadList->begin(); iterG != gThreadList->end(); iterG ++ )
					{
						if ( (*iterG) == this ) 
						{
							gThreadList->erase(iterG);
						}
					}
				}
            }
            return true;
        }
    }
    return false;
};

bool ConcurrentSemaphore::pendingAny(ConcurrentLock* pLock)
{
	//fprintf(stderr,"  > ConcurrentSemaphore::pendingAny()\n");
    if ( !isValid() )
    {
        TRACE(_T("!!! OBJECT_NOT_VALID: ConcurrentSemaphore::pendingAny\n"));
        return false;
    }
	INTERNAL_DECLARE_RESOURCE_LOCK(this,"ConcurrentSemaphore::pendingAny")
    if ( !INTERNAL_RESOURCE_READ_LOCK(this,INTERNAL_TIMEOUT) )
    {
        TRACE(_T("!!! INTERNAL_ERROR: ConcurrentSemaphore::pendingAny\n"));
		_ASSERTE ( simpleDeadLock(GetCurrentThreadId()) == false );
        return false;
    }
    /*
    if ( !pLock->isValid() )
    {
        TRACE(_T("!!! REQUESTER_NOT_VALID: ConcurrentSemaphore::pendingAny\n"));
        return false;
    }
    */
     _t_ConcurrentLockIter iter;
    for ( iter = m_requestList.begin(); iter != m_requestList.end() && (*iter) != pLock; iter ++ )
    {
        return true;
    }
    return false;
};

bool ConcurrentSemaphore::pendingShared(ConcurrentLock* pLock)
{
	//fprintf(stderr,"  > ConcurrentSemaphore::pendingShared()\n");
    if ( !isValid() )
    {
        TRACE(_T("!!! OBJECT_NOT_VALID: ConcurrentSemaphore::pendingShared\n"));
        return false;
    }
	INTERNAL_DECLARE_RESOURCE_LOCK(this,"ConcurrentSemaphore::pendingShared")
    if ( !INTERNAL_RESOURCE_READ_LOCK(this,INTERNAL_TIMEOUT) )
    {
        TRACE(_T("!!! INTERNAL_ERROR: ConcurrentSemaphore::pendingShared\n"));
		_ASSERTE ( simpleDeadLock(GetCurrentThreadId()) == false );
        return false;
    }
    /*
    if ( !pLock->isValid() )
    {
        TRACE(_T("!!! REQUESTER_NOT_VALID: ConcurrentSemaphore::pendingShared\n"));
        return false;
    }
    */
     _t_ConcurrentLockIter iter;
    for ( iter = m_requestList.begin(); iter != m_requestList.end() && (*iter) != pLock; iter ++ )
    {
        if ( (*iter)->getAccess() == access_shared )
		{
            return true;
		}
    }
    return false;
};

bool ConcurrentSemaphore::pendingExclusive(ConcurrentLock* pLock)
{
	//fprintf(stderr,"  > ConcurrentSemaphore::pendingExclusive()\n");
    if ( !isValid() )
    {
        TRACE(_T("!!! OBJECT_NOT_VALID: ConcurrentSemaphore::pendingExclusive\n"));
        return false;
    }
	INTERNAL_DECLARE_RESOURCE_LOCK(this,"ConcurrentSemaphore::pendingExclusive")
    if ( !INTERNAL_RESOURCE_READ_LOCK(this,INTERNAL_TIMEOUT) )
    {
        TRACE(_T("!!! INTERNAL_ERROR: ConcurrentSemaphore::pendingExclusive\n"));
		_ASSERTE ( simpleDeadLock(GetCurrentThreadId()) == false );
        return false;
    }
    /*
    if ( !pLock->isValid() )
    {
        TRACE(_T("!!! REQUESTER_NOT_VALID: ConcurrentSemaphore::pendingExclusive\n"));
        return false;
    }
    */
     _t_ConcurrentLockIter iter;
    for ( iter = m_requestList.begin(); iter != m_requestList.end() && (*iter) != pLock; iter ++ )
    {
        if ( (*iter)->getAccess() != access_shared )
		{
            return true;
		}
    }
    return false;
};

ConcurrentLock* ConcurrentSemaphore::nestedOwnership(ConcurrentLock* pLock)
{
	//fprintf(stderr,"  > ConcurrentSemaphore::nestedOwnership()\n");
    if ( !isValid() )
    {
        TRACE(_T("!!! OBJECT_NOT_VALID: ConcurrentSemaphore::nestedOwnership\n"));
        return NULL;
    }
	INTERNAL_DECLARE_RESOURCE_LOCK(this,"ConcurrentSemaphore::nestedOwnership")
    if ( !INTERNAL_RESOURCE_READ_LOCK(this,INTERNAL_TIMEOUT) )
    {
        TRACE(_T("!!! INTERNAL_ERROR: ConcurrentSemaphore::nestedOwnership\n"));
		_ASSERTE ( simpleDeadLock(GetCurrentThreadId()) == false );
        return NULL;
    }
    /*
    if ( !pLock->isValid() )
    {
        TRACE(_T("!!! REQUESTER_NOT_VALID: ConcurrentSemaphore::nestedOwnership\n"));
        return NULL;
    }
    */
     _t_ConcurrentLockIter iter;
    for ( iter = m_ownerList.begin(); iter != m_ownerList.end(); iter ++ )
    {
        // is there an outside lock within this same thread?
        if ( (*iter) != pLock &&
             (*iter)->getThreadId() == pLock->getThreadId()
           )
        {
            return (*iter);
        }
    }
    return NULL;
};

ConcurrentLock* ConcurrentSemaphore::outsideOwnership(ConcurrentLock* pLock)
{
	//fprintf(stderr,"  > ConcurrentSemaphore::outsideOwnership()\n");
    if ( !isValid() )
    {
        TRACE(_T("!!! OBJECT_NOT_VALID: ConcurrentSemaphore::outsideOwnership\n"));
        return NULL;
    }
	INTERNAL_DECLARE_RESOURCE_LOCK(this,"ConcurrentSemaphore::outsideOwnership")
    if ( !INTERNAL_RESOURCE_READ_LOCK(this,INTERNAL_TIMEOUT) )
    {
        TRACE(_T("!!! INTERNAL_ERROR: ConcurrentSemaphore::outsideOwnership\n"));
		_ASSERTE ( simpleDeadLock(GetCurrentThreadId()) == false );
        return NULL;
    }
    /*
    if ( !pLock->isValid() )
    {
        TRACE(_T("!!! REQUESTER_NOT_VALID: ConcurrentSemaphore::outsideOwnership\n"));
        return NULL;
    }
    */
     _t_ConcurrentLockIter iter;
    for ( iter = m_ownerList.begin(); iter != m_ownerList.end(); iter ++ )
    {
        // is there an outside lock from another thread?
        if ( (*iter)->getThreadId() != pLock->getThreadId() )
        {
            return (*iter);
        }
    }
    return NULL;
};

ConcurrentLock* ConcurrentSemaphore::getCurrentLock(void)
{ 
	//fprintf(stderr,"  > ConcurrentSemaphore::getCurrentLock()\n");
	ConcurrentLock* retPtr = NULL;
    if ( !isValid() )
    {
        TRACE(_T("!!! OBJECT_NOT_VALID: ConcurrentSemaphore::getCurrentLock\n"));
        return NULL;
    }
	INTERNAL_DECLARE_RESOURCE_LOCK(this,"ConcurrentSemaphore::getCurrentLock")
    if ( !INTERNAL_RESOURCE_READ_LOCK(this,INTERNAL_TIMEOUT) )
    {
        TRACE(_T("!!! INTERNAL_ERROR: ConcurrentSemaphore::getCurrentLock\n"));
		_ASSERTE ( simpleDeadLock(GetCurrentThreadId()) == false );
        return NULL;
    }
    retPtr = m_ownerList.empty() ? NULL : m_ownerList.front(); 
	return retPtr;
};

void ConcurrentSemaphore::resetAccess(void)
{
	//fprintf(stderr,"  > ConcurrentSemaphore::resetAccess()\n");
    if ( !isValid() )
    {
        TRACE(_T("!!! OBJECT_NOT_VALID: ConcurrentSemaphore::resetAccess\n"));
        return;
    }
	INTERNAL_DECLARE_RESOURCE_LOCK(this,"ConcurrentSemaphore::resetAccess")
    if ( !INTERNAL_RESOURCE_WRITE_LOCK(this,INTERNAL_TIMEOUT) )
    {
        TRACE(_T("!!! INTERNAL_ERROR: ConcurrentSemaphore::resetAccess\n"));
		_ASSERTE ( simpleDeadLock(GetCurrentThreadId()) == false );
        return;
    }
    m_access = access_none;
     _t_ConcurrentLockIter iter;
    for ( iter = m_ownerList.begin(); iter != m_ownerList.end(); iter ++ )
    {
        m_access = (*iter)->m_access;
        if ( m_access == access_thread_exclusive ) break;  // highest level, so break here
    }
}

//////////////////////////////////////////////////////////////////////////////

_e_AccessStatus ConcurrentSemaphore::allowShared(ConcurrentLock* pLock)
{
	//fprintf(stderr,"  > ConcurrentSemaphore::allowShared()\n");
    if ( !isValid() )
    {
        TRACE(_T("!!! OBJECT_NOT_VALID: ConcurrentSemaphore::allowShared\n"));
        return request_aborted;
    }
	INTERNAL_DECLARE_RESOURCE_LOCK(this,"ConcurrentSemaphore::allowShared")
    if ( !INTERNAL_RESOURCE_WRITE_LOCK(this,INTERNAL_TIMEOUT) )
    {
        TRACE(_T("!!! INTERNAL_ERROR: ConcurrentSemaphore::allowShared\n"));
		_ASSERTE ( simpleDeadLock(GetCurrentThreadId()) == false );
        return request_aborted;
    }
    if ( !pLock->isValid() )
    {
        TRACE(_T("!!! REQUESTER_NOT_VALID: ConcurrentSemaphore::allowShared\n"));
        return request_aborted;
    }
    pLock->m_lastError = error_none;

	// are we already being served
	ConcurrentLock* nestedLock = nestedOwnership(pLock);
	ConcurrentLock* outsideLock = outsideOwnership(pLock);
    // some THREAD in MY FAMILY already is there (shortcut to SUCCESS)
    //  allow me to butt in line to get that crazy sibling out of there
    if ( nestedLock )
    {
        pLock->m_lastError = error_access_nested;
        return request_granted;
    }

    // somebody in-line before us wants WRITE access
    if ( pendingExclusive(pLock) )
    {
        pLock->m_lastError = error_pending_exclusive;
        return request_denied;
    }

    // quick exit for majority of cases
    if (m_access == access_none || m_access == access_shared)
    {
        return request_granted;
    }

    pLock->m_lastError = error_access_exclusive;
    return request_denied;
}

_e_AccessStatus ConcurrentSemaphore::allowExclusive(ConcurrentLock* pLock,bool restrict)
{
	//fprintf(stderr,"  > ConcurrentSemaphore::allowExclusive()\n");
    if ( !isValid() )
    {
        TRACE(_T("!!! OBJECT_NOT_VALID: ConcurrentSemaphore::allowExclusive\n"));
        return request_aborted;
    }
	INTERNAL_DECLARE_RESOURCE_LOCK(this,"ConcurrentSemaphore::allowExclusive")
    if ( !INTERNAL_RESOURCE_WRITE_LOCK(this,INTERNAL_TIMEOUT) )
    {
        TRACE(_T("!!! INTERNAL_ERROR: ConcurrentSemaphore::allowExclusive\n"));
		_ASSERTE ( simpleDeadLock(GetCurrentThreadId()) == false );
        return request_aborted;
    }
    if ( !pLock->isValid() )
    {
        TRACE(_T("!!! REQUESTER_NOT_VALID: ConcurrentSemaphore::allowExclusive\n"));
		return request_aborted;
    }
    pLock->m_lastError = error_none;

	// are we already being served
	ConcurrentLock* nestedLock = nestedOwnership(pLock);
	ConcurrentLock* outsideLock = outsideOwnership(pLock);
	if ( nestedLock )
	{
        pLock->m_lastError = error_access_outside;
		if ( outsideLock )
		{
			// need to wait for outside locks to go away
	        return request_denied;
		}
        pLock->m_lastError = error_access_nested;
        // if an request is RESTRICTive, they weren't expecting an update!!!
        if ( restrict )
        {
            return request_aborted;
        }
        return request_granted;
	}

    // somebody in-line before us wants ANY access
    if ( pendingAny(pLock) )
    {
        pLock->m_lastError = error_pending_any;
        return request_denied;
    }

    // quick exit for majority of cases
    if (m_access == access_none)
    {
        return request_granted;
    }

    // some OTHER FAMILY already is there (need to WAIT)
    if (outsideLock)
    {
        pLock->m_lastError = error_access_outside;
        return request_denied;
    }

    // some THREAD in MY FAMILY already is there (shortcut to SUCCESS)
    //  allow me to butt in line to get that crazy sibling out of there
    if ( nestedLock )
    {
        pLock->m_lastError = error_access_nested;
        // if an request is RESTRICTive, they weren't expecting an update!!!
        if ( restrict )
        {
            return request_aborted;
        }
        return request_granted;
    }

    return request_granted;
}

//////////////////////////////////////////////////////////////////////////////

bool ConcurrentSemaphore::Unlock(ConcurrentLock* pLock)
{
	//fprintf(stderr,"  > ConcurrentSemaphore::Unlock()\n");
    if ( !isValid() )
    {
        TRACE(_T("!!! OBJECT_NOT_VALID: ConcurrentSemaphore::Unlock\n"));
        return false;
    }
	INTERNAL_DECLARE_RESOURCE_LOCK(this,"ConcurrentSemaphore::Unlock")
    if ( !INTERNAL_RESOURCE_WRITE_LOCK(this,INTERNAL_TIMEOUT) )
    {
        TRACE(_T("!!! INTERNAL_ERROR: ConcurrentSemaphore::Unlock\n"));
		_ASSERTE ( simpleDeadLock(GetCurrentThreadId()) == false );
        return false;
    }
    /*
    if ( !pLock->isValid() )
    {
        TRACE(_T("!!! REQUESTER_NOT_VALID: ConcurrentSemaphore::Unlock\n"));
       return request_aborted;
    }
    */

    queryLock(m_requestList,pLock,true);
    
    if ( queryLock(m_ownerList,pLock,true) )
        resetAccess();

    pLock->m_access = access_none;
    pLock->m_lastError = error_none;
    return true;
};

//////////////////////////////////////////////////////////////////////////////

_e_AccessStatus ConcurrentSemaphore::Lock(ConcurrentLock* pRequester,_e_AccessType reqAccess)
{
	//fprintf(stderr,"  > ConcurrentSemaphore::Lock()\n");
    if ( !pRequester->isValid() )
    {
        TRACE(_T("!!! REQUESTER_NOT_VALID: ConcurrentSemaphore::Lock\n"));
        return request_aborted;
    }
    if ( !isValid() )
    {
        TRACE(_T("!!! OBJECT_NOT_VALID: ConcurrentSemaphore::Lock\n"));
        return request_aborted;
    }
	INTERNAL_DECLARE_RESOURCE_LOCK(this,"ConcurrentSemaphore::Lock")
    if ( !INTERNAL_RESOURCE_WRITE_LOCK(this,INTERNAL_TIMEOUT) )
    {
        TRACE(_T("!!! INTERNAL_ERROR: ConcurrentSemaphore::Lock\n"));
		_ASSERTE ( simpleDeadLock(GetCurrentThreadId()) == false );
        return request_aborted;
    }

    _e_AccessStatus reqRC = request_denied;
    ULONG timeout = pRequester->getTimeout();

    // check status of request in ownership list
    if ( queryLock(m_ownerList,pRequester) )
    {
        // request already exists in the form that we want it
        if ( reqAccess == pRequester->m_access )
        {
            pRequester->m_lastError = error_none;
            return request_granted;
        }
        // access has changed, place it back in the queue, but with VIP treatment
        queryLock(m_ownerList,pRequester,true);
        resetAccess();
    }

    // check status of request in the request queue
    if ( !queryLock(m_requestList,pRequester) )
    {
        // it doesn't exist yet, so add it to the end
        addLock(m_requestList,pRequester);
    }

	if ( DEADLOCK_CHECK )
	{
		// check for deadlock (after adding it to request list)
		if ( timeout && simpleDeadLock(pRequester->getThreadId()) )
		{
			queryLock(m_requestList,pRequester,true);
			pRequester->m_lastError = error_request_deadlock;
			reqRC = request_aborted;
		}
	}

    // we cannot hold the resource while waiting
    INTERNAL_RESOURCE_UNLOCK(this);

    DWORD timeBegin = timeGetTime();
    // now how will we wait for this request?
    switch(reqAccess)
    {
        case access_shared:
            while ( (reqRC = allowShared(pRequester)) == request_denied ) // purposeful assignment
            {
				if ( pUpdateEvent )
				{
		            if ( WaitForSingleObject(pUpdateEvent,timeout) != WAIT_OBJECT_0 )
	                    break;
				}
				else
				{
					Sleep(0);   // give up timeslice
					if ( (timeGetTime()-timeBegin) > pRequester->getTimeout() )  // purposeful assignment
						break;
				}
            }
            break;

        case access_thread_exclusive:
            while ( (reqRC = allowExclusive(pRequester,false)) == request_denied ) // purposeful assignment
            {
				if ( pUpdateEvent )
				{
		            if ( WaitForSingleObject(pUpdateEvent,timeout) != WAIT_OBJECT_0 )
	                    break;
				}
				else
				{
					Sleep(0);   // give up timeslice
					if ( (timeGetTime()-timeBegin) > pRequester->getTimeout() )  // purposeful assignment
						break;
				}
            }
            break;

        case access_resource_exclusive:
            while ( (reqRC = allowExclusive(pRequester,true)) == request_denied ) // purposeful assignment
            {
				if ( pUpdateEvent )
				{
		            if ( WaitForSingleObject(pUpdateEvent,timeout) != WAIT_OBJECT_0 )
	                    break;
				}
				else
				{
					Sleep(0);   // give up timeslice
					if ( (timeGetTime()-timeBegin) > pRequester->getTimeout() )  // purposeful assignment
						break;
				}
            }
            break;
    }
    // perform this section without interrupts
    if ( !isValid() )
    {
        TRACE(_T("!!! OBJECT_NOT_VALID: ConcurrentSemaphore::Lock\n"));
        return request_aborted;
    }
    if ( !INTERNAL_RESOURCE_WRITE_LOCK(this,INTERNAL_TIMEOUT) )
    {
        TRACE(_T("!!! INTERNAL_ERROR: ConcurrentSemaphore::Lock\n"));
		_ASSERTE ( simpleDeadLock(GetCurrentThreadId()) == false );
        return request_aborted;
    }
    // double-check validity of this object
    if ( !pRequester->isValid() )
    {
        reqRC = request_aborted;
    }
    // remove this lock from request list
    if ( !queryLock(m_requestList,pRequester,true) )
    {
        // TRACE(_T("!!! REQUESTER_NOT_VALID: ConcurrentSemaphore::Lock\n"));
        pRequester->m_lastError = error_request_invalid;
        reqRC = request_aborted;
    }
    // now how will we document for this request?
    if ( TRACE_WAITS )
    {
        // calculate wait time
        DWORD waitTime = timeGetTime()-timeBegin;
        switch(reqAccess)
        {
            case access_shared:
                switch(reqRC)
                {
                    case request_granted:
                        m_sharedGrants ++;
                        if ( waitTime > m_sharedWaitMax )
                        {
                            m_sharedWaitMax = waitTime;
                            _tcscpy(m_szSharedWaitMax,pRequester->m_szDebugInfo);
                        }
                        break;
                    case request_denied:
                        m_sharedDenies ++;
                        break;
                    case request_aborted:
                        m_sharedAborts ++;
                        break;
                }
                break;

            case access_thread_exclusive:
                switch(reqRC)
                {
                    case request_granted:
                        m_threadGrants ++;
                        if ( waitTime > m_threadWaitMax )
                        {
                            m_threadWaitMax = waitTime;
                            _tcscpy(m_szThreadWaitMax,pRequester->m_szDebugInfo);
                        }
                        break;
                    case request_denied:
                        m_threadDenies ++;
                        break;
                    case request_aborted:
                        m_threadAborts ++;
                        break;
                }
                break;

            case access_resource_exclusive:
                switch(reqRC)
                {
                    case request_granted:
                        m_resourceGrants ++;
                        if ( waitTime > m_resourceWaitMax )
                        {
                            m_resourceWaitMax = waitTime;
                            _tcscpy(m_szResourceWaitMax,pRequester->m_szDebugInfo);
                        }
                        break;
                    case request_denied:
                        m_resourceDenies ++;
                        break;
                    case request_aborted:
                        m_resourceAborts ++;
                        break;
                }
                break;
        }
    }

    // add it to the ownership list
    if ( reqRC == request_granted )
    {
        pRequester->m_access = reqAccess;
        addLock(m_ownerList,pRequester);
        resetAccess();
        if ( TRACE_WAITS )
        {
            ULONG overlapDepth = m_ownerList.size();
            if ( overlapDepth > 1 ) // purposeful assignment
            {
                m_overlappedLocks ++;
                m_overlappedDepthMax = max(m_overlappedDepthMax,overlapDepth);
            }
        }
    }
    else if ( TRACE_ERRORS && timeout )
    {
        ConcurrentLock* tmpLock = NULL;
        TRACE(_T("!!! C-Sem(%p,\"%s\",req=%lu,own=%lu), ACCESS=%s [%s]\n"),
                this, 
                m_szDebugInfo,
                m_requestList.size(),
                m_ownerList.size(),
                describeAccessType(reqAccess),
                describeAccessStatus(reqRC) 
             );
        if ( pRequester )
        {
            TRACE(_T("\t:Requester(%08lx,\"%s\"):\"%s\"\n"),
                    pRequester->m_threadId,
                    pRequester->m_szDebugInfo,
                    describeAccessError(pRequester->m_lastError)
                 );
        }
        if ( tmpLock = outsideOwnership(pRequester) ) // purposeful assignment
        {
            TRACE(_T("\t:Outside Owner(%08lx,%s,\"%s\")\n"),
                    tmpLock->m_threadId,
                    describeAccessType(tmpLock->m_access),
                    tmpLock->m_szDebugInfo
                 );
        }
        else if ( tmpLock = nestedOwnership(pRequester) )  // purposeful assignment
        {
            TRACE(_T("\t:Nested Owner(%08lx,%s,\"%s\")\n"),
                    tmpLock->m_threadId,
                    describeAccessType(tmpLock->m_access),
                    tmpLock->m_szDebugInfo
                 );
        }
        else 
        {
            TRACE(_T("\t:NO Owner\n"));
        }
    }
    return reqRC;
}

bool ConcurrentSemaphore::isValid(void)
{
	return (this   
#ifdef _AFXDLL
		&& AfxIsValidAddress(this,sizeof(ConcurrentSemaphore))
#endif
        && (mValid == 0x01) // can contain 0xEE when freed
        );
}

/****************************************************************************/
/*                                                                          */
/*                          Copyright 2000                                  */
/*                     Checco Computer Services                             */
/*                        All Rights Reserved.                              */
/*                                                                          */
/*     This software contains proprietary and confidential information      */
/*     belonging to Checco Computer Services, and may not be decompiled,    */
/*     disassembled, disclosed, reproduced or copied without the prior      */
/*     written consent of Checco Computer Services.                         */
/*                                                                          */
/****************************************************************************/
/*
 *  PROJECT:		
 *  SUBSYSTEM:		
 *
 *  DESCRIPTION:	ConcurrentLock base class.
 *
 *  SEE ALSO:
 *
 *  OTHER:
 *
 *
 *    Miscellaneous:
 */

/****************************************************************************/

ConcurrentLock::ConcurrentLock(ConcurrentSemaphore* pSem,LPCTSTR pszDebugInfo)
{ 
    m_threadId = GetCurrentThreadId();
    memset(m_szDebugInfo,0,LOCK_DEBUGINFO_SZ);
    if ( pszDebugInfo ) _tcsncpy(m_szDebugInfo,pszDebugInfo,LOCK_DEBUGINFO_SZ-1);
    //ASSERT(pSem);
    m_pSem = pSem;
    m_access = access_none;
    m_status = request_denied;
    m_timeout = 0;
}

ConcurrentLock::~ConcurrentLock() 
{ 
    Unlock();
}

_e_AccessStatus ConcurrentLock::Lock(_e_AccessType access,DWORD timeout)
{
    m_status = request_denied;
    m_timeout = timeout;
    if ( isValid() && m_pSem->isValid() ) 
    {
        m_status = m_pSem->Lock(this,access);
    }
    return m_status;
}

void ConcurrentLock::Unlock(void)
{
    m_status = request_aborted;
    m_timeout = 0;
    if ( m_pSem->isValid() ) 
    {
        m_pSem->Unlock(this);
    }
}

bool ConcurrentLock::isValid(void)
{
	return (this 
#ifdef _AFXDLL
        && AfxIsValidAddress(this,sizeof(ConcurrentLock))
        // && AfxIsMemoryBlock(this,sizeof(ConcurrentLock))
#endif
        && m_pSem 
        //&& m_access != access_none
        && m_status != request_aborted
        );
}

