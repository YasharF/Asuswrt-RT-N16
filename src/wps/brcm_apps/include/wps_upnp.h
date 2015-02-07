/*
 * WPS upnp
 *
 * Copyright (C) 2009, Broadcom Corporation
 * All Rights Reserved.
 * 
 * This is UNPUBLISHED PROPRIETARY SOURCE CODE of Broadcom Corporation;
 * the contents of this file may not be disclosed to third parties, copied
 * or duplicated in any form, in whole or in part, without the prior
 * written permission of Broadcom Corporation.
 *
 * $Id: wps_upnp.h,v 1.5 2009/09/01 02:38:02 Exp $
 */

#ifndef __WPS_UPNP_H__
#define __WPS_UPNP_H__

#include <wps.h>

void wps_upnp_init();
void wps_upnp_deinit();
void wps_upnp_clear_ssr();
void wps_upnp_clear_ssr_timer();
int wps_upnp_ssr_expire();
void wps_upnp_device_uuid(unsigned char *uuid);
char *wps_upnp_parse_msg(char *upnpmsg, int upnpmsg_len, int *len, int *type, char *addr);
int wps_upnp_process_msg(char *upnpmsg, int upnpmsg_len, wps_hndl_t *hndl);
int wps_upnp_send_msg(int if_instance, char *buf, int len, int type);

void wps_upnp_update_wlan_event(int if_instance, unsigned char *macaddr,
	char *databuf, int datalen, int init);
void wps_upnp_update_init_wlan_event(int if_instance, char *mac, int init);

#endif	/* __WPS_UPNP_H__ */
