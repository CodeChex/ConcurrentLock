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
 *  PROJECT:		---
 *  SUBSYSTEM:		---
 *
 *  DESCRIPTION:	ConcurrentLock PRIVATE header.
 *
 *  SEE ALSO:
 *
 *  OTHER:
 *
 * 
 *    Miscellaneous:
 */
#ifndef _ConcurrentLockPrivate_H
#define _ConcurrentLockPrivate_H

///////////////////////////////////////////////////////////////////////////
// PreProcessor Declarations
#define lockREGISTRY_NAME   _T("ConcurrentLock")
#define _CCL_API  __declspec( dllexport )

///////////////////////////////////////////////////////////////////////////
//  Operating System Dependent Settings
#ifdef _WIN32
   #include "stdafx.h"
#else
   #include <stdio.h>
#endif	// _WIN32

///////////////////////////////////////////////////////////////////////////
#include "ConcurrentLock.h"

#endif  // _ConcurrentLockPrivate_H
