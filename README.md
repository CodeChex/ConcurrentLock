# ConcurrentLock
Simple C++ library for CONCURRENT READ and EXCLUSIVE WRITE access to a class or its members

 ****************************************************************************

    These SyncObjects allow true multi-threaded 
        CONCURRENT READ and EXCLUSIVE WRITE access to any class.

 ****************************************************************************

    HISTORY:
    -------

    Until now, you were told that all class methods should access/update their members
    in a thread safe manner, so like the rest of us sheep, you created a member variable:

        CCriticalSection mCritSec;
        
    and wrapped every class method with:

        CSingleLock mSingleLock(&mCritSec);
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

 ******************************************************************************
