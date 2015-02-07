/*
 * aeskeywrap.h
 * Perform RFC3394 AES-based key wrap and unwrap functions.  
 *
 * Copyright 2003, Broadcom Corporation
 * All Rights Reserved.
 *     
 * This is UNPUBLISHED PROPRIETARY SOURCE CODE of Broadcom Corporation; the
 * contents of this file may not be disclosed to third parties, copied or
 * duplicated in any form, in whole or in part, without the prior written
 * permission of Broadcom Corporation.
 *
 * $Id: aeskeywrap.h,v 1.1.1.1 2008/07/21 09:20:23 james26_jang Exp $
 */

#ifndef _AESWRAP_H_
#define _AESWRAP_H_

#include <typedefs.h>
/* For size_t */
#include <stddef.h>
#include <bcmcrypto/aes.h>

#define AKW_BLOCK_LEN	8
/* Max size of wrapped data, not including overhead
   The key wrap algorithm doesn't impose any upper bound the wrap length,
   but we need something big enought to handle all users.  802.11i and
   probably most other users will be limited by MTU size, so using a 2K
   buffer limit seems relatively safe. */
#define AKW_MAX_WRAP_LEN	2048

/* aes_wrap: perform AES-based keywrap function defined in RFC3394
	return 0 on success, 1 on error
	input is il bytes
	output is (il+8) bytes */
int aes_wrap(size_t kl, uint8 *key, size_t il, uint8 *input, uint8 *output);

/* aes_unwrap: perform AES-based key unwrap function defined in RFC3394,
	return 0 on success, 1 on error
	input is il bytes
	output is (il-8) bytes */
int aes_unwrap(size_t kl, uint8 *key, size_t il, uint8 *input, uint8 *output);

#endif /* _AESWRAP_H_ */

