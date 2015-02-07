/*
 * rc4.h
 * RC4 stream cipher
 *
 * Copyright 2003, Broadcom Corporation
 * All Rights Reserved.
 *     
 * This is UNPUBLISHED PROPRIETARY SOURCE CODE of Broadcom Corporation; the
 * contents of this file may not be disclosed to third parties, copied or
 * duplicated in any form, in whole or in part, without the prior written
 * permission of Broadcom Corporation.
 *
 * $Id: rc4.h,v 1.1.1.1 2008/07/21 09:20:24 james26_jang Exp $
 */

#ifndef _RC4_H_
#define _RC4_H_

#include <typedefs.h>

#define RC4_STATE_NBYTES 256

typedef struct rc4_ks
{ 
   uchar state[RC4_STATE_NBYTES];
   uchar x;
   uchar y;
} rc4_ks_t;

void prepare_key(uchar *key_data_ptr, int key_data_len, rc4_ks_t *key);

void rc4(uchar *buffer_ptr, int buffer_len, rc4_ks_t *key);

#endif /* _RC4_H_ */

