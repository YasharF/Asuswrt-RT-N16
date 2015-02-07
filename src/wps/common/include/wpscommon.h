/*
 * WPS Common
 *
 * Copyright (C) 2009, Broadcom Corporation
 * All Rights Reserved.
 * 
 * This is UNPUBLISHED PROPRIETARY SOURCE CODE of Broadcom Corporation;
 * the contents of this file may not be disclosed to third parties, copied
 * or duplicated in any form, in whole or in part, without the prior
 * written permission of Broadcom Corporation.
 *
 * $Id: wpscommon.h,v 1.13 2009/03/06 15:53:29 Exp $
 */

#ifndef _WPS_COMMON_
#define _WPS_COMMON_

#include <wpstypes.h>
#include <portability.h>


#define REGISTRAR_ID_STRING        "WFA-SimpleConfig-Registrar-1-0"
#define ENROLLEE_ID_STRING        "WFA-SimpleConfig-Enrollee-1-0"

#define BUF_SIZE_64_BITS    8
#define BUF_SIZE_128_BITS   16
#define BUF_SIZE_160_BITS   20
#define BUF_SIZE_256_BITS   32
#define BUF_SIZE_512_BITS   64
#define BUF_SIZE_1024_BITS  128
#define BUF_SIZE_1536_BITS  192
#define NVRAM_BUFSIZE	100

#define PERSONALIZATION_STRING  "Wi-Fi Easy and Secure Key Derivation"
#define PRF_DIGEST_SIZE         BUF_SIZE_256_BITS
#define KDF_KEY_BITS            640

#include <wps_devinfo.h>

#define WPS_RESULT_SUCCESS_RESTART			100
#define WPS_RESULT_SUCCESS					101
#define WPS_RESULT_PROCESS_TIMEOUT			102
#define WPS_RESULT_USR_BREAK				103
#define WPS_RESULT_FAILURE					104
#define WPS_RESULT_CONFIGAP_PINFAIL			105

#define WPS_IND_SUCCESS_RESTART			(100<<8)
#define WPS_IND_SUCCESS					(101<<8)
#define WPS_IND_PROCESS_TIMEOUT			(102<<8)
#define WPS_IND_USR_BREAK				(103<<8)
#define WPS_IND_FAILURE					(104<<8)
#define WPS_IND_CONFIGAP_PINFAIL			(105<<8)
typedef enum {
	EModeUnknown = 0,
	EModeUnconfAp = 1,
	EModeClient = 2,
	EModeRegistrar = 3,
	EModeApProxy = 4,
	EModeApProxyRegistrar = 5
} EMode;

typedef enum {
	RECV_ID_EAP = 1,
	RECV_ID_UPNP
} recvid;

typedef enum {
	WPS_INIT = 0,
	WPS_ASSOCIATED,
	WPS_OK,
	WPS_MSG_ERR,
	WPS_TIMEOUT,
	WPS_SENDM2,
	WPS_SENDM7,
	WPS_MSGDONE,
	WPS_PBCOVERLAP
} EWPS_STATE;

typedef enum {
	CB_QUIT = 0,
	CB_TRUFD = 1,
	CB_TRNFC = 2,
	CB_TREAP = 3,
	CB_TRWLAN_BEACON = 4,
	CB_TRWLAN_PR_REQ = 5,
	CB_TRWLAN_PR_RESP = 6,
	CB_TRANS = 7,
	CB_MAIN_PUSH_MSG = 8,
	CB_MAIN_START_AP = 9,
	CB_MAIN_STOP_AP = 10,
	CB_MAIN_START_WPASUPP = 11,
	CB_MAIN_RESET_WPASUPP = 12,
	CB_MAIN_STOP_WPASUPP = 13,
	CB_MAIN_PUSH_MODE = 14,
	CB_MAIN_NEW_STA = 15,
	CB_MAIN_NFC_DATA = 16,
	CB_MAIN_PUSH_REG_RESULT = 17,
	CB_MAIN_REQUEST_PWD = 18,
	CB_TRIP = 19,
	CB_SM = 20,
	CB_SM_RESET = 21,
	CB_TRUFD_INSERTED = 22,
	CB_TRUFD_REMOVED = 23,
	CB_TRUPNP_CP = 24,
	CB_TRUPNP_DEV = 25,
	CB_TRUPNP_DEV_SSR = 26,
	CB_SSR_TIMEOUT = 27,
	CB_VND_TIMEOUT =28 /* Brcm */
} ECBType;

typedef enum {
	TRANSPORT_TYPE_UFD = 1,
	TRANSPORT_TYPE_EAP,
	TRANSPORT_TYPE_WLAN,
	TRANSPORT_TYPE_NFC,
	TRANSPORT_TYPE_BT,
	TRANSPORT_TYPE_IP,
	TRANSPORT_TYPE_UPNP_CP,
	TRANSPORT_TYPE_UPNP_DEV,
	TRANSPORT_TYPE_MAX /* insert new transport types before TRANSPORT_TYPE_MAX */
} TRANSPORT_TYPE;

typedef struct {
	ECBType	eType;
	uint32  dataLength;
} S_CB_HEADER;

typedef struct {
	S_CB_HEADER cbHeader;
	uint32 result;
	void *encrSettings;
	DevInfo *peerInfo;
} S_CB_SM;

#define SM_FAILURE    0
#define SM_SUCCESS    1
#define SM_SET_PASSWD 2
#define WPS_WLAN_EVENT_TYPE_EAP_FRAME 2

#define WPSM_WPSA_PORT		40000 + (1 << 6)
#define WPSAP_WPSM_PORT		40000 + (1 << 7)
#define WPS_LOOPBACK_ADDR		"127.0.0.1"

extern void RAND_bytes(unsigned char *buf, int num);
extern unsigned char *SHA256(const unsigned char *key, size_t n, unsigned char *md);
extern void hmac_sha256(const void *key, int key_len,
	const unsigned char *text, size_t text_len, unsigned char *digest,
	unsigned int *digest_len);
bool wps_getUpnpDevGetDeviceInfo(void *mc_dev);
void wps_setUpnpDevGetDeviceInfo(void *mc_dev, bool value);


#endif /* _WPS_COMMON_ */
