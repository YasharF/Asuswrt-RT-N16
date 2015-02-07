/*
 * WPS AP
 *
 * Copyright (C) 2009, Broadcom Corporation
 * All Rights Reserved.
 * 
 * This is UNPUBLISHED PROPRIETARY SOURCE CODE of Broadcom Corporation;
 * the contents of this file may not be disclosed to third parties, copied
 * or duplicated in any form, in whole or in part, without the prior
 * written permission of Broadcom Corporation.
 *
 * $Id: wps_ap.h,v 1.30 2009/10/16 04:00:55 Exp $
 */

#ifndef __WPS_AP_H__
#define __WPS_AP_H__

#define WPS_OVERALL_TIMEOUT	140 /* 120 + 20 for VISTA testing tolerance */
#define WPS_MSG_TIMEOUT	30

#ifndef IFNAMSIZ
#define IFNAMSIZ 16
#endif

typedef struct {
	int wps_mode;
	int ess_id;
	char ifname[IFNAMSIZ];
	unsigned char mac_ap[6];
	unsigned char mac_sta[6];
	unsigned char *pre_nonce;
	unsigned char *pre_privkey;

	bool config_state;
	char default_keyMgmt[20];

	void *mc;			/* state machine context */

	unsigned long pkt_count;
	unsigned long pkt_count_prv;

	int wps_state;			/* state machine operating state */
	unsigned long start_time;	/* workspace init time */
	unsigned long touch_time;	/* workspace latest operating time */

	int return_code;
} wpsap_wksp_t;

/*
 * implemented in wps_ap.c
 */
uint32 wpsap_osl_eapol_send_data(char *dataBuffer, uint32 dataLen);
char* wpsap_osl_eapol_parse_msg(char *msg, int msg_len, int *len);

/* Common interface to ap wksp */
int wpsap_open_session(void *wps_app, int mode, unsigned char *mac, unsigned char *mac_sta,
	char *osifname, char *enr_nonce, char *priv_key);

#endif /* __WPS_AP_H__ */
