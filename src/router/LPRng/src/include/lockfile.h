/***************************************************************************
 * LPRng - An Extended Print Spooler System
 *
 * Copyright 1988-2003, Patrick Powell, San Diego, CA
 *     papowell@lprng.com
 * See LICENSE for conditions of use.
 * $Id: lockfile.h,v 1.1.1.1 2007/02/15 12:13:01 jiahao Exp $
 ***************************************************************************/



#ifndef _LOCKFILE_H_
#define _LOCKFILE_H_ 1

/* PROTOTYPES */
int Do_lock( int fd, int block );
int Do_unlock( int fd );
int LockDevice(int fd, int block );

#endif
