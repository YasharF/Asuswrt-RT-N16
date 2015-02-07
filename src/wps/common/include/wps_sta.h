/*
 * WPS station
 *
 * Copyright (C) 2009, Broadcom Corporation
 * All Rights Reserved.
 * 
 * This is UNPUBLISHED PROPRIETARY SOURCE CODE of Broadcom Corporation;
 * the contents of this file may not be disclosed to third parties, copied
 * or duplicated in any form, in whole or in part, without the prior
 * written permission of Broadcom Corporation.
 *
 * $Id: wps_sta.h,v 1.6 2008/10/20 05:55:44 Exp $
 */
#ifndef __WPS_STA_H__
#define __WPS_STA_H__

#define WPS_MAX_AP_SCAN_LIST_LEN 50
#define PBC_WALK_TIME 120

wps_ap_list_info_t *create_aplist();

int add_wps_ie(unsigned char *p_data, int length);

int rem_wps_ie(unsigned char *p_data, int length);

int join_network(char* ssid, uint8 wsec);

int leave_network();

int wps_get_bssid(char *bssid);

int wps_get_ssid(char *ssid, int *len);

int wps_get_bands(uint *band_num, uint *active_band);

int do_wpa_psk(WpsEnrCred* credential);

int join_network_with_bssid(char* ssid, uint8 wsec, char *bssid);


#endif /* __WPS_STA_H__ */
