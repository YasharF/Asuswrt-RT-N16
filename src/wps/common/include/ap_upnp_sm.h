/*
 * Broadcom WPS inband UPnP interface
 *
 * Copyright (C) 2009, Broadcom Corporation
 * All Rights Reserved.
 * 
 * This is UNPUBLISHED PROPRIETARY SOURCE CODE of Broadcom Corporation;
 * the contents of this file may not be disclosed to third parties, copied
 * or duplicated in any form, in whole or in part, without the prior
 * written permission of Broadcom Corporation.
 *
 * $Id: ap_upnp_sm.h,v 1.12 2009/09/02 04:18:54 Exp $
 */

#ifndef __AP_UPNP_SM_H__
#define __AP_UPNP_SM_H__

#include <eap_defs.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef BCMUPNP
#include <security_ipc.h>
#else
/*
 * UPnP command between WPS and WFA device
 */
#define WPS_UPNPDEV_ADDR		"127.0.0.1"
#define WPS_UPNPDEV_PORT		40000

#define UPNP_WPS_TYPE_SSR            1           /* Set Selected Registrar */
#define UPNP_WPS_TYPE_PMR            2           /* Wait For Put Message Resp */
#define UPNP_WPS_TYPE_GDIR           3           /* Wait For Get DevInfo Resp */
#define UPNP_WPS_TYPE_PWR            4           /* Put WLAN Response */
#define UPNP_WPS_TYPE_WE             5           /* WLAN Event */
#define UPNP_WPS_TYPE_QWFAS          6           /* Query WFAWLANConfig Subscribers */
#define UPNP_WPS_TYPE_MAX            7

typedef struct {
	unsigned int type;
	unsigned int dst_addr;
	unsigned int length;
	unsigned char data[1];
} UPNP_WPS_CMD;
#define UPNP_WPS_CMD_SIZE 12
#endif	/* BCMUPNP */

typedef struct {
	void *mc_dev;
	char ifname[16];
	int if_instance;
	bool m_waitForGetDevInfoResp;
	bool m_waitForPutMessageResp;
	char msg_to_send[WPS_EAP_DATA_MAX_LENGTH]; /* message to be sent */
	int msg_len;
	void (*init_wlan_event)(int, char *, int);
	void (*update_wlan_event)(int, unsigned char *, char *, int, int);
	uint32 (*send_data)(int, char *, uint32, int);
	char* (*parse_msg)(char *, int, int *, int *, char *);
} UPNP_WPS_AP_STATE;

/*
 * UPnP command between WPS and WFA device
 */

/* Functions */
uint32 ap_upnp_sm_init(void *mc_dev, int pmr_trigger, void *init_wlan_event,
	void *update_wlan_event, void *send_data, void *parse_msg, int instance);
uint32 ap_upnp_sm_deinit();
uint32 ap_upnp_sm_sendMsg(char *dataBuffer, uint32 dataLen);
uint32 ap_upnp_sm_process_msg(char *msg, int msg_len);
int ap_upnp_sm_get_msg_to_send(char **data);
void ap_upnp_sm_set_ssr_ipaddr(char *ipaddr);
char *ap_upnp_sm_get_ssr_ipaddr();


#ifdef __cplusplus
}
#endif

#endif	/* __AP_UPNP_SM_H__ */
