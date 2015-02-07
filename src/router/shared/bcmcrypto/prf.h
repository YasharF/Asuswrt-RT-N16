/*
 * prf.h
 * PRF function used in WPA and TGi
 *
 * Copyright 2003, Broadcom Corporation
 * All Rights Reserved.
 *     
 * This is UNPUBLISHED PROPRIETARY SOURCE CODE of Broadcom Corporation; the
 * contents of this file may not be disclosed to third parties, copied or
 * duplicated in any form, in whole or in part, without the prior written
 * permission of Broadcom Corporation.
 *
 * $Id: prf.h,v 1.1.1.1 2008/07/21 09:20:24 james26_jang Exp $
 */

#ifndef _PRF_H_
#define _PRF_H_

#include <typedefs.h>

/* lengths in Bytes */
#define PRF_MAX_I_D_LEN	1024
#define PRF_MAX_KEY_LEN	64
#define PRF_OUTBUF_LEN	80

int PRF(unsigned char *key, int key_len, unsigned char *prefix, 
	int prefix_len, unsigned char *data, int data_len, 
	unsigned char *output, int len);

int fPRF(unsigned char *key, int key_len, unsigned char *prefix, 
	int prefix_len, unsigned char *data, int data_len, 
	unsigned char *output, int len);

void hmac_sha1(unsigned char *text, int text_len, unsigned char *key,
	       int key_len, unsigned char *digest);

void hmac_md5(unsigned char *text, int text_len, unsigned char *key,
	      int key_len, unsigned char *digest);

#endif /* _PRF_H_ */

