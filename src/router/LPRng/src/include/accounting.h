/***************************************************************************
 * LPRng - An Extended Print Spooler System
 *
 * Copyright 1988-2003, Patrick Powell, San Diego, CA
 *     papowell@lprng.com
 * See LICENSE for conditions of use.
 * $Id: accounting.h,v 1.1.1.1 2007/02/15 12:13:01 jiahao Exp $
 ***************************************************************************/



#ifndef _ACCOUNTING_H_
#define _ACCOUNTING_H_ 1

/* PROTOTYPES */
int Do_accounting( int end, char *command, struct job *job, int timeout );

#endif
