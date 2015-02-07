/*
 * Interface definitions for DES.
 * Copied from des-ka9q-1.0-portable. a public domain DES implementation.
 *
 * Copyright 2003, Broadcom Corporation
 * All Rights Reserved.
 *     
 * This is UNPUBLISHED PROPRIETARY SOURCE CODE of Broadcom Corporation;
 * the contents of this file may not be disclosed to third parties, copied or
 * duplicated in any form, in whole or in part, without the prior written
 * permission of Broadcom Corporation.
 *
 * $Id: des.h,v 1.1.1.1 2008/07/21 09:20:23 james26_jang Exp $
 */

#ifndef _DES_H_
#define _DES_H_

typedef unsigned long DES_KS[16][2];	/* Single-key DES key schedule */

void deskey(DES_KS,unsigned char *,int);

void des(DES_KS,unsigned char *);

#endif /* _DES_H_ */

