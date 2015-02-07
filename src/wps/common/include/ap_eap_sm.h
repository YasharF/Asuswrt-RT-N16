/*
 * Inband EAP
 *
 * Copyright (C) 2009, Broadcom Corporation
 * All Rights Reserved.
 * 
 * This is UNPUBLISHED PROPRIETARY SOURCE CODE of Broadcom Corporation;
 * the contents of this file may not be disclosed to third parties, copied
 * or duplicated in any form, in whole or in part, without the prior
 * written permission of Broadcom Corporation.
 *
 * $Id: ap_eap_sm.h,v 1.11 2009/07/29 02:32:06 Exp $
 */

#ifndef _AP_EAP_SM_H_
#define _AP_EAP_SM_H_

#include <eap_defs.h>
#include <proto/ethernet.h>

typedef struct {
	char state; /* current state. */
	char sent_msg_id; /* out wps message ID */
	char recv_msg_id; /* in wps message ID */
	unsigned char sta_mac[ETHER_ADDR_LEN]; /* STA's ethernet address */
	unsigned char bssid[ETHER_ADDR_LEN]; /* incoming bssid */
	unsigned char eap_id; /* identifier of the last request  */
	char msg_to_send[WPS_EAP_DATA_MAX_LENGTH]; /* message to be sent */
	int msg_len;
	int last_sent_len;
	int resent_count;
	unsigned long last_check_time;
	int send_count;
	void *mc_dev;
	char * (*parse_msg)(char *, int, int *);
	unsigned int (*send_data)(char *, uint32);
} EAP_WPS_AP_STATE;

int ap_eap_sm_startWPSReg(unsigned char *sta_mac, unsigned char *ap_mac);
uint32 ap_eap_sm_process_sta_msg(char *msg, int msg_len);
uint32 ap_eap_sm_init(void *mc_dev, char *mac_sta, char * (*parse_msg)(char *, int, int *),
	unsigned int (*send_data)(char *, uint32));
uint32 ap_eap_sm_deinit();
uint32 ap_eap_sm_sendMsg(char * dataBuffer, uint32 dataLen);
uint32 ap_eap_sm_sendWPSStart();
unsigned char *ap_eap_sm_getMac(int mode);
int ap_eap_sm_get_msg_to_send(char **data);
void ap_eap_sm_Failure(int deauthFlag);
int ap_eap_sm_check_timer(uint32 time);
int ap_eap_sm_get_eap_state();

#endif /* _AP_EAP_SM_H_ */
