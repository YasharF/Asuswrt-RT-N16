/*
 * aes.h
 * AES encrypt/decrypt wrapper functions used around Rijndael reference 
 * implementation
 *
 * Copyright 2003, Broadcom Corporation
 * All Rights Reserved.
 *     
 * This is UNPUBLISHED PROPRIETARY SOURCE CODE of Broadcom Corporation; the
 * contents of this file may not be disclosed to third parties, copied or
 * duplicated in any form, in whole or in part, without the prior written
 * permission of Broadcom Corporation.
 *
 * $Id: aes.h,v 1.1.1.1 2008/07/21 09:20:23 james26_jang Exp $
 */

#ifndef _AES_H_
#define _AES_H_

#include <typedefs.h>
/* For size_t */
#include <stddef.h>

#define AES_BLOCK_SZ    	16
#define AES_BLOCK_BITLEN	(AES_BLOCK_SZ * 8)
#define AES_KEY_BITLEN(kl)  	(kl * 8)
#define AES_ROUNDS(kl)		((AES_KEY_BITLEN(kl) / 32) + 6)
#define AES_MAXROUNDS		14

void aes_encrypt(const size_t KL, const uint8 *K, const uint8 *ptxt, uint8 *ctxt);
void aes_decrypt(const size_t KL, const uint8 *K, const uint8 *ctxt, uint8 *ptxt);

#define aes_block_encrypt(nr, rk, ptxt, ctxt) rijndaelEncrypt(rk, nr, ptxt, ctxt)
#define aes_block_decrypt(nr, rk, ctxt, ptxt) rijndaelDecrypt(rk, nr, ctxt, ptxt)

#endif /* _AES_H_ */

