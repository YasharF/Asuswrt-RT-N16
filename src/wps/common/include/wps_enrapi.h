/*
 * WPS Enrollee API
 *
 * Copyright (C) 2009, Broadcom Corporation
 * All Rights Reserved.
 * 
 * This is UNPUBLISHED PROPRIETARY SOURCE CODE of Broadcom Corporation;
 * the contents of this file may not be disclosed to third parties, copied
 * or duplicated in any form, in whole or in part, without the prior
 * written permission of Broadcom Corporation.
 *
 * $Id: wps_enrapi.h,v 1.15 2009/11/18 02:44:41 Exp $
 */

#ifndef __WPS_ENROLLEE_API_H__
#define __WPS_ENROLLEE_API_H__

#include <wps_devinfo.h>

typedef enum {
	WPS_WL_AKM_NONE = 0,
	WPS_WL_AKM_PSK,
	WPS_WL_AKM_PSK2,
	WPS_WL_AKM_BOTH
} WPS_WL_AKM_E;

/* eap state machine states */
enum {
	INIT = 0,
	EAPOL_START_SENT,
	EAP_IDENTITY_SENT,
	PROCESSING_PROTOCOL,
	REG_SUCCESSFUL,
	REG_FAILED,
	EAP_TIMEOUT,
	EAP_FAILURE
};

/* returned to callers of process_registrar_message or process_timers */
enum {
	INTERNAL_ERROR = 0, /* internal processing erro (eap not initialized ..)  */
	REG_SUCCESS, /* registration completed */
	REG_FAILURE /* registration failed  */
};

enum {
	PBC_NOT_FOUND = 0,
	PBC_FOUND_OK,
	PBC_OVERLAP,
	PBC_TIMEOUT
};

#define ENCRYPT_NONE 1
#define ENCRYPT_WEP 2
#define ENCRYPT_TKIP 4
#define ENCRYPT_AES 8
typedef struct wps_ap_list_info {
	bool        used;
	uint8       ssid[SIZE_SSID_LENGTH];
	uint8       ssidLen;
	uint8       BSSID[6];
	uint8       *ie_buf;
	uint32      ie_buflen;
	uint8       channel;
	uint16      band;
	uint8       wep;
	uint8       scstate;
} wps_ap_list_info_t;


uint32 wps_enr_config_init(DevInfo *dev_info);
uint32 wps_enr_reg_config_init(DevInfo *dev_info, char *nwKey, int nwKeyLen);
void wps_cleanup();
uint32 wps_start_enrollment(char *pin, unsigned long time);
uint32 wps_start_registration(char *pin, unsigned long time);
void wps_get_credentials(WpsEnrCred* credential, const char *ssid, int len);
void wps_get_reg_M7credentials(WpsEnrCred* credential);
void wps_get_reg_M8credentials(WpsEnrCred* credential);
int wps_process_ap_msg(char *eapol_msg, int len);
int wps_get_msg_to_send(char **data, uint32 time);
int wps_eap_check_timer(uint32 time);
int wps_get_aplist(wps_ap_list_info_t *list_in, wps_ap_list_info_t *list_out);
int wps_get_pbc_ap(wps_ap_list_info_t *list_in, char *bssid, char *ssid, uint8 *,
	unsigned long time, char start);
uint32 wps_build_pbc_proberq(uint8 *buff, int len);
uint32 wps_eap_send_msg(char * dataBuffer, uint32 dataLen);
void wps_eap_failure();
int wps_get_sent_msg_id();
bool wps_get_select_reg(const wps_ap_list_info_t *ap_in);
int wps_get_retrans_msg_to_send(char **data, uint32 time, char *mtype);
char* wps_get_msg_string(int mtype);
int wps_get_recv_msg_id();
int wps_get_eap_state();
char wps_get_ap_msg_type(char *eapol_msg, int len);
bool wps_validateChecksum(IN unsigned long int PIN);
uint32 wps_update_RFBand(uint8 rfBand);

#endif /* __WPS_ENROLLEE_API_H__ */
