/*
 * Portability
 *
 * Copyright (C) 2009, Broadcom Corporation
 * All Rights Reserved.
 * 
 * This is UNPUBLISHED PROPRIETARY SOURCE CODE of Broadcom Corporation;
 * the contents of this file may not be disclosed to third parties, copied
 * or duplicated in any form, in whole or in part, without the prior
 * written permission of Broadcom Corporation.
 *
 * $Id: portability.h,v 1.7 2009/06/09 06:19:04 Exp $
 */

#ifndef _WPS_PORTAB_
#define _WPS_PORTAB_

#include <wpstypes.h>

#ifndef __cplusplus
#include "stdio.h"
#include "stdlib.h"
#include "typedefs.h"

#ifdef true
#undef true
#endif
#define true 1

#ifdef false
#undef false
#endif
#define false 0

char * alloc_init(int size);
#endif /* __cplusplus */

#ifdef __cplusplus
extern "C" {
#endif

#include "string.h"

/* Synchronization functions */
#define WpsSyncCreate(handle)
#define WpsSyncDestroy(handle)
#define WpsLock(h)
#define WpsUnlock(handle)


/* Byte swapping functions. To be implemented by application. */
uint32 WpsHtonl(uint32 intlong);
uint16 WpsHtons(uint16 intshort);
uint32 WpsHtonlPtr(uint8 * in, uint8 * out);
uint16 WpsHtonsPtr(uint8 * in, uint8 * out);
uint32 WpsNtohl(uint8* intlong);
uint16 WpsNtohs(uint8 * intshort);
void WpsSleep(uint32 seconds);
void WpsSleepMs(uint32 ms);


typedef struct {
	char ssid[SIZE_SSID_LENGTH];
	uint32 ssidLen;
	char keyMgmt[SIZE_20_BYTES];
	char nwKey[SIZE_64_BYTES];
	uint32 nwKeyLen;
	uint32 encrType;
	uint16 wepIndex;
} WpsEnrCred;

bool wps_isSRPBC(IN uint8 *p_data, IN uint32 len);
bool wps_isWPSS(IN uint8 *p_data, IN uint32 len);
bool wl_is_wps_ie(uint8 **wpaie, uint8 **tlvs, uint *tlvs_len);
bool is_wps_ies(uint8* cp, uint len);
bool is_SelectReg_PBC(uint8* cp, uint len);
bool is_ConfiguredState(uint8* cp, uint len);
uint8 *wps_parse_tlvs(uint8 *tlv_buf, int buflen, uint key);
int set_mac_address(char *mac_string, char *mac_bin);
void wps_set_reg_result(uint8 val);
bool wps_isSELR(IN uint8 *p_data, IN uint32 len);

#ifdef __cplusplus
}
#endif

#endif /* _WPS_PORTAB_ */
