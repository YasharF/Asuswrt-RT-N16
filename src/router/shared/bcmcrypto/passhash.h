/*
 * passhash.h
 * Perform password->key hash algorithm as defined in WPA and 802.11i
 * specifications
 *
 * Copyright 2002, Broadcom Corporation
 * All Rights Reserved.
 *
 * This is UNPUBLISHED PROPRIETARY SOURCE CODE of Broadcom Corporation; the
 * contents of this file may not be disclosed to third parties, copied or
 * duplicated in any form, in whole or in part, without the prior written
 * permission of Broadcom Corporation.
 *
 * $Id: passhash.h,v 1.1.1.1 2008/07/21 09:20:23 james26_jang Exp $
 */

#ifndef _PASSHASH_H_
#define _PASSHASH_H_

/* passhash: perform passwork->key hash algorithm as defined in WPA and 802.11i
	specifications
	password is an ascii string of 8 to 63 characters in length
	ssid is up to 32 bytes
	ssidlen is the length of ssid in bytes
	output must be at lest 40 bytes long, and returns a 256 bit key
	returns 0 on success, non-zero on failure */
int passhash(char *password, int passlen, unsigned char *ssid, int ssidlen, unsigned char *output);

#endif /* _PASSHASH_H_ */

