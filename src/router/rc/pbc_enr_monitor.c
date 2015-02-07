#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <linux/reboot.h>

#include <wlutils.h>
#include <bcmutils.h>
#include <bcmnvram.h>
#include <shutils.h>

#include <wlscan.h>

extern int restart_wps(void);
void wet_to_ap();

int nvram_commit_needed = 0;
struct timeval tv_ref, tv_now;

char backup_wps_config_state[32];
char backup_wps_cli[32];
char backup_wps_client_role[32];
char backup_wps_pbc_force[32];
char backup_wps_timer_start[32];
char backup_wps_waiting[32];
unsigned char BSSID[6];

void check_reftime()
{
	gettimeofday(&tv_now, NULL);
	if ((tv_now.tv_usec - tv_ref.tv_usec) >= 0)
	{
		fprintf(stderr, "%ld, %ld\n", tv_now.tv_sec-tv_ref.tv_sec, (tv_now.tv_usec-tv_ref.tv_usec)/1000);

		if ((tv_now.tv_sec-tv_ref.tv_sec) >= 120)
		{
			fprintf(stderr, "Failed (timeout)...\n");
			wet_to_ap();
		}
	}
	else
	{
		fprintf(stderr, "%ld, %ld\n", tv_now.tv_sec-tv_ref.tv_sec-1, (1000000+tv_now.tv_usec-tv_ref.tv_usec)/1000);

		if ((tv_now.tv_sec-tv_ref.tv_sec-1) >= 120)
		{
			fprintf(stderr, "Failed (timeout)...\n");
			wet_to_ap();
		}
	}
}

typedef struct wps_ap_list_info
{
	bool        used;
	uint8       ssid[33];
	uint8       ssidLen; 
	uint8       BSSID[6];
	uint8       *ie_buf;
	uint32      ie_buflen;
	uint8       channel;
	uint8       wep;
} wps_ap_list_info_t;

wps_ap_list_info_t ap_list[WPS_ENR_MAX_AP_SCAN_LIST_LEN];
char scan_result[WPS_ENR_DUMP_BUF_LEN]; 

char *
wps_enr_get_scan_results(char *ifname)
{
	int ret, retry_times = 0;
	wl_scan_params_t *params;
	wl_scan_results_t *list = (wl_scan_results_t*)scan_result;
	int params_size = WL_SCAN_PARAMS_FIXED_SIZE + NUMCHANS * sizeof(uint16);

	params = (wl_scan_params_t*)malloc(params_size);
	if (params == NULL) {
		return NULL;
	}

	memset(params, 0, params_size);
	params->bss_type = DOT11_BSSTYPE_INFRASTRUCTURE;
	memcpy(&params->bssid, &ether_bcast, ETHER_ADDR_LEN);
	params->scan_type = -1;
	params->nprobes = -1;
	params->active_time = -1;
	params->passive_time = -1;
	params->home_time = -1;
	params->channel_num = 0;

retry:
	ret = wl_ioctl(ifname, WLC_SCAN, params, params_size);
	if (ret < 0) {
		if (retry_times++ < WPS_ENR_SCAN_RETRY_TIMES) {
			fprintf(stderr, "set scan command failed, retry %d\n", retry_times);
			sleep(1);
			goto retry;
		}
	}

	sleep(2);

	list->buflen = WPS_ENR_DUMP_BUF_LEN;
	ret = wl_ioctl(ifname, WLC_SCAN_RESULTS, scan_result, WPS_ENR_DUMP_BUF_LEN);
	if (ret < 0 && retry_times++ < WPS_ENR_SCAN_RETRY_TIMES) {
		fprintf(stderr, "get scan result failed, retry %d\n", retry_times);
		sleep(1);
		goto retry;
	}

	free(params);
	if (ret < 0)
		return NULL;

	return scan_result;
}

extern int wpa_parse_wpa_ie(const unsigned char *wpa_ie, size_t wpa_ie_len, struct wpa_ie_data *data);

void wpa_cipher_parse(int cipher)
{
	if (cipher ==( WPA_CIPHER_TKIP_|WPA_CIPHER_CCMP_))
	{
		fprintf(stderr, "\nRemote AP uses TKIP+AES encryption!\n");
		if (nvram_match("sw_mode_ex", "2"))
			nvram_set("sta_crypto", "tkip+aes");
		else
			nvram_set("wl_crypto", "tkip+aes");
	}
}

void
wps_enr_get_scan_results_sp(int wpa_mode)
{
	int ret, retry_times = 0;
	wl_scan_params_t *params;
	wl_scan_results_t *list = (wl_scan_results_t*)scan_result;
	int params_size = WL_SCAN_PARAMS_FIXED_SIZE + NUMCHANS * sizeof(uint16);

	params = (wl_scan_params_t*)malloc(params_size);
	if (params == NULL) {
		return;
	}

	memset(params, 0, params_size);
	params->bss_type = DOT11_BSSTYPE_INFRASTRUCTURE;
	memcpy(&params->bssid, BSSID, ETHER_ADDR_LEN);
	params->scan_type = -1;
	params->nprobes = -1;
	params->active_time = -1;
	params->passive_time = -1;
	params->home_time = -1;
	params->channel_num = 0;

retry:
	ret = wl_ioctl(WIF, WLC_SCAN, params, params_size);
	if (ret < 0) {
		if (retry_times++ < WPS_ENR_SCAN_RETRY_TIMES) {
			fprintf(stderr, "set scan command failed, retry %d\n", retry_times);
			sleep(1);
			goto retry;
		}
	}

	sleep(2);

	list->buflen = WPS_ENR_DUMP_BUF_LEN;
	ret = wl_ioctl(WIF, WLC_SCAN_RESULTS, scan_result, WPS_ENR_DUMP_BUF_LEN);
	if (ret < 0 && retry_times++ < WPS_ENR_SCAN_RETRY_TIMES) {
		fprintf(stderr, "get scan result failed, retry %d\n", retry_times);
		sleep(1);
		goto retry;
	}

	free(params);
	if (ret < 0)
		return;

	int i, left;
	char *info_b;
	wl_bss_info_t *info;
	struct wpa_ie_data wid;
	struct bss_ie_hdr *ie;

	info = &(list->bss_info[0]);
	info_b = (unsigned char *)info;
	
	for(i = 0; i < list->count; i++)
	{
		ie = (struct bss_ie_hdr *) ((unsigned char *) info + sizeof(*info));

		if (wpa_mode == 2)
		for (left = info->ie_length; left > 0; // RSN IE
			left -= (ie->len + 2), ie = (struct bss_ie_hdr *) ((unsigned char *) ie + 2 + ie->len)) 
		{
			if (ie->elem_id != DOT11_MNG_RSN_ID)
				continue;

			if (wpa_parse_wpa_ie(&ie->elem_id, ie->len+2, &wid) == 0)
			{
				wpa_cipher_parse(wid.pairwise_cipher);
				break;
			}
		}
		else if (wpa_mode == 1)
		for (left = info->ie_length; left > 0; // WPA IE
			left -= (ie->len + 2), ie = (struct bss_ie_hdr *) ((unsigned char *) ie + 2 + ie->len)) 
		{
			if (ie->elem_id != DOT11_MNG_WPA_ID)
				continue;

			if (wpa_parse_wpa_ie(&ie->elem_id, ie->len+2, &wid) == 0)
			{
				wpa_cipher_parse(wid.pairwise_cipher);
				break;
			}
		}

		info = (wl_bss_info_t *) ((unsigned char *) info + info->length);
	}

}

wps_ap_list_info_t *
wps_enr_create_aplist()
{
	wl_scan_results_t *list = (wl_scan_results_t*)scan_result;
	wl_bss_info_t *bi;
	wl_bss_info_107_t *old_bi;
	uint i, wps_ap_count = 0;

	if (wps_enr_get_scan_results(WIF) == NULL)
		return NULL;

	memset(ap_list, 0, sizeof(ap_list));
	if (list->count == 0)
		return 0;
	else if (list->version != WL_BSS_INFO_VERSION &&
			list->version != LEGACY_WL_BSS_INFO_VERSION) {
		/* fprintf(stderr, "Sorry, your driver has bss_info_version %d "
		    "but this program supports only version %d.\n",
		    list->version, WL_BSS_INFO_VERSION); */
		return NULL;
	}

	bi = list->bss_info;
	for (i = 0; i < list->count; i++) {
	/* Convert version 107 to 108 */
		if (bi->version == LEGACY_WL_BSS_INFO_VERSION) {
			old_bi = (wl_bss_info_107_t *)bi;
			bi->chanspec = CH20MHZ_CHSPEC(old_bi->channel);
			bi->ie_length = old_bi->ie_length;
			bi->ie_offset = sizeof(wl_bss_info_107_t);
		}    
		if (bi->ie_length) {
			if(wps_ap_count < WPS_ENR_MAX_AP_SCAN_LIST_LEN) {
				ap_list[wps_ap_count].used = TRUE;
				memcpy(ap_list[wps_ap_count].BSSID, (uint8 *)&bi->BSSID, 6);
				strncpy((char *)ap_list[wps_ap_count].ssid, (char *)bi->SSID, bi->SSID_len);
				ap_list[wps_ap_count].ssid[bi->SSID_len] = '\0';
				ap_list[wps_ap_count].ssidLen= bi->SSID_len;
				ap_list[wps_ap_count].ie_buf = (uint8 *)(((uint8 *)bi) + bi->ie_offset);
				ap_list[wps_ap_count].ie_buflen = bi->ie_length;
				ap_list[wps_ap_count].channel = (uint8)(bi->chanspec & WL_CHANSPEC_CHAN_MASK);
				ap_list[wps_ap_count].wep = bi->capability & DOT11_CAP_PRIVACY;
				wps_ap_count++;
			}
		}
		bi = (wl_bss_info_t*)((int8*)bi + bi->length);
	}

	return ap_list;
}

uint8 *
wps_enr_parse_tlvs(uint8 *tlv_buf, int buflen, uint key)
{
	uint8 *cp;
	int totlen;

	cp = tlv_buf;
	totlen = buflen;

	/* find tagged parameter */
	while (totlen >= 2) {
		uint tag;
		int len;

		tag = *cp;
		len = *(cp +1);

		/* validate remaining totlen */
		if ((tag == key) && (totlen >= (len + 2)))
			return (cp);

		cp += (len + 2);
		totlen -= (len + 2);
	}

	return NULL;
}

bool
wps_enr_wl_is_wps_ie(uint8 **wpaie, uint8 **tlvs, uint *tlvs_len)
{
	uint8 *ie = *wpaie;

	/* If the contents match the WPA_OUI and type=1 */
	if ((ie[1] >= 6) && !memcmp(&ie[2], WPA_OUI "\x04", 4))
	{
		unsigned int IEFlags = 0;
/*
		unsigned char version = 0x0, wsc_state = 0, ap_setup_lock = 0, selected_registrar = 0, uuid_e[16], rf_band = 0;
*/
		unsigned short device_password_id = 0xffff/*, selected_registrar_config_method = 0, primary_device_type_category = 0, primary_device_type_subcategory = 0*/;

		unsigned char *pos, *data_head;
		unsigned short len_in_hdr;
		unsigned char tot_len = ie[1];

		data_head = pos = (unsigned char*) &ie[6];
	
		/* TLV:  T=2bytes, L=2bytes, V=... */
		while(pos < data_head + tot_len - 4)
		{
			len_in_hdr = ntohs(*(unsigned short *)(pos+2));

			if(tot_len - (pos - data_head) < len_in_hdr) {
				fprintf(stderr, "Error: crafted WSC packet? tot_len = %d, len_in_hdr = %d\n", tot_len, len_in_hdr);
				break;
			}

			switch( ntohs(*(unsigned short *)pos)) {
/*
				case WSC_ID_VERSION:
					if(len_in_hdr != WSC_ID_VERSION_LEN) {
						fprintf(stderr, "Error: crafted WSC packet? WSC_ID_VERSION.\n");
						goto WSC_FAILED;
					}
					IEFlags |= ntohs(*(unsigned short *)pos);
					version = *(unsigned char *)(pos + 2 + 2);
					break;

				case WSC_ID_SC_STATE:
					if(len_in_hdr != WSC_ID_SC_STATE_LEN) {
						fprintf(stderr, "Error: crafted WSC packet? WSC_ID_SC_STATE.\n");
						goto WSC_FAILED;
					}
					IEFlags |= ntohs(*(unsigned short *)pos);
					wsc_state = *(unsigned char *)(pos + 2 + 2);
					break;
			
				case WSC_ID_AP_SETUP_LOCKED:
					if(len_in_hdr != WSC_ID_AP_SETUP_LOCKED_LEN) {
						fprintf(stderr, "Error: crafted WSC packet? WSC_ID_AP_SETUP_LOCKED.\n");
						goto WSC_FAILED;
					}
					IEFlags |= ntohs(*(unsigned short *)pos);
					ap_setup_lock = *(unsigned char *)(pos + 2 + 2);
					break;

				case WSC_ID_SEL_REGISTRAR:
					if(len_in_hdr != WSC_ID_SEL_REGISTRAR_LEN) {
						fprintf(stderr, "Error: crafted WSC packet? WSC_ID_SEL_REGISTRAR.\n");
						goto WSC_FAILED;
					}
					IEFlags |= ntohs(*(unsigned short *)pos);
					selected_registrar = *(unsigned char *)(pos + 2 + 2);
					break;
*/
				case WSC_ID_DEVICE_PWD_ID:
					if(len_in_hdr != WSC_ID_DEVICE_PWD_ID_LEN) {
						fprintf(stderr, "Error: crafted WSC packet? WSC_ID_DEVICE_PWD_ID.\n");
						goto WSC_FAILED;
					}
					IEFlags |= ntohs(*(unsigned short *)pos);
					device_password_id = ntohs(*(unsigned short *)(pos + 2 + 2));
					break;
/*
				case WSC_ID_SEL_REG_CFG_METHODS:
					if(len_in_hdr != WSC_ID_SEL_REG_CFG_METHODS_LEN) {
						fprintf(stderr, "Error: crafted WSC packet? WSC_ID_SEL_REG_CFG_METHODS.\n");
						goto WSC_FAILED;
					}
					IEFlags |= ntohs(*(unsigned short *)pos);
					selected_registrar_config_method = ntohs(*(unsigned short *)(pos + 2 + 2));
					break;

				case WSC_ID_UUID_E:
					if(len_in_hdr != WSC_ID_UUID_E_LEN) {
						fprintf(stderr, "Error: crafted WSC packet? WSC_ID_UUID_E.\n");
						goto WSC_FAILED;
					}
					IEFlags |= ntohs(*(unsigned short *)pos);
					memcpy(uuid_e, (unsigned char *)pos + 2 + 2, 16);
					break;

				case WSC_ID_RF_BAND:
					if(len_in_hdr != WSC_ID_RF_BAND_LEN) {
						fprintf(stderr, "Error: crafted WSC packet? WSC_ID_RF_BAND.\n");
						goto WSC_FAILED;
					}
					IEFlags |= ntohs(*(unsigned short *)pos);
					rf_band = *(unsigned char *)(pos + 2 + 2);
					break;

				case WSC_ID_PRIMARY_DEVICE_TYPE:
					if(len_in_hdr != WSC_ID_PRIMARY_DEVICE_TYPE_LEN) {
						fprintf(stderr, "Error: crafted WSC packet? WSC_ID_PRIMARY_DEVICE_TYPE.\n");
						goto WSC_FAILED;
					}
					IEFlags |= ntohs(*(unsigned short *)pos);
					primary_device_type_category =  *(unsigned short *)(pos + 2 + 2);
					primary_device_type_subcategory =  *(unsigned short *)(pos + 2 + 2 + 2 + 2);
*/					
				default:
//					fprintf(stderr, "unknown tlv:%04x\n", ntohs(*(unsigned short *)pos));
					break;

			}
			pos = pos+2+2+ len_in_hdr;
		}
/*
		if (device_password_id == 0x0)
			fprintf(stderr, "## PIN\n");
		else if (device_password_id == 0x4)
			fprintf(stderr, "## PBC\n");
		else
			fprintf(stderr, "## ???\n");
*/
		if (device_password_id == 0x4)
			return TRUE;
	}
WSC_FAILED:

	/* point to the next ie */
	ie += ie[1] + 2;
	/* calculate the length of the rest of the buffer */
	*tlvs_len -= (int)(ie - *tlvs);
	/* update the pointer to the start of the buffer */
	*tlvs = ie;

	return FALSE;
}

bool
wps_enr_is_wps_ies(uint8* cp, uint len)
{
	uint8 *parse = cp;
	uint parse_len = len;
	uint8 *wpaie;

	while ((wpaie = wps_enr_parse_tlvs(parse, parse_len, DOT11_MNG_WPA_ID)))
		if (wps_enr_wl_is_wps_ie(&wpaie, &parse, &parse_len))
			break;
	if (wpaie)
		return TRUE;
	else
		return FALSE;
}

int
wps_enr_get_aplist(wps_ap_list_info_t *list_in, wps_ap_list_info_t *list_out )
{
	wps_ap_list_info_t *ap_in = &list_in[0];
	wps_ap_list_info_t *ap_out = &list_out[0];
	int i=0, wps_apcount = 0;

	while( ap_in->used == TRUE && i < WPS_ENR_MAX_AP_SCAN_LIST_LEN) {
		if(TRUE == wps_enr_is_wps_ies(ap_in->ie_buf, ap_in->ie_buflen)) {
			memcpy(ap_out, ap_in, sizeof(wps_ap_list_info_t));
			wps_apcount++;
			ap_out = &list_out[wps_apcount];	
		}
		i++;
		ap_in = &list_in[i];
	}
	/* in case of on-the-spot filtering, make sure we stop the list  */
	if(wps_apcount < WPS_ENR_MAX_AP_SCAN_LIST_LEN)
		ap_out->used = 0;

	return wps_apcount;
}

int
wps_enr_display_aplist(wps_ap_list_info_t *ap)
{
	char eastr[ETHER_ADDR_STR_LEN];
	int i=0;
	
	if(!ap)
		return 0;

	fprintf(stderr, "\n-------------------------------------\n");
	while(ap->used == TRUE ) {
		fprintf(stderr, " %d :  ", i);
		fprintf(stderr, "SSID:%s  ", ap->ssid);
		fprintf(stderr, "BSSID:%s  ", ether_etoa(ap->BSSID, eastr));	
		fprintf(stderr, "Channel:%d  ", ap->channel);
		if(ap->wep)
			fprintf(stderr, "WEP");
		fprintf(stderr, "\n");
		ap++; 
		i++;
	}
	
	fprintf(stderr, "-------------------------------------\n");
	return 0;
}

int
wps_enr_scan_result()
{
	wps_ap_list_info_t *wpsaplist;
	int wps_apcount = 0;

//	if (nvram_invmatch("wl_wps_mode", "enr_enabled"))
//		return 0;

	wps_enr_create_aplist();

	wpsaplist = ap_list;
	wps_apcount = wps_enr_get_aplist(wpsaplist, wpsaplist);

	if (wps_apcount)
		wps_enr_display_aplist(wpsaplist);

	return wps_apcount;
}

void
pbc_restart_wps()
{
	nvram_set("wps_mode", "disabled");
	nvram_set("wps_enr_mode", "enabled");
	nvram_set("wl_wps_mode", "enr_enabled");
	nvram_set("wl_wps_reg", "disabled");
	nvram_set("wl_wps_config_state", "0");
	nvram_set("wl0_wps_mode", "enr_enabled");
	nvram_set("wl0_wps_reg", "disabled");
	nvram_set("wl0_wps_config_state", "0");

	nvram_unset("wps_enr_ifname");
	nvram_unset("wps_enr_ssid");
	nvram_unset("wps_enr_bssid");
	nvram_unset("wps_enr_wsec");

	nvram_set("wps_restart", "0");
	nvram_set("wps_proc_status", "0");
	nvram_set("wps_timeout_enable", "0");
	nvram_set("wps_config_command", "0");

	strcpy(backup_wps_config_state, nvram_safe_get("wps_config_state"));
	strcpy(backup_wps_cli, nvram_safe_get("wps_cli"));
	strcpy(backup_wps_client_role, nvram_safe_get("wps_client_role"));
	strcpy(backup_wps_pbc_force, nvram_safe_get("wps_pbc_force"));
	strcpy(backup_wps_timer_start, nvram_safe_get("wps_timer_start"));
	strcpy(backup_wps_waiting, nvram_safe_get("wps_waiting"));

	nvram_unset("wps_config_state");
	nvram_unset("wps_cli");
	nvram_unset("wps_client_role");
	nvram_unset("wps_pbc_force");
	nvram_unset("wps_timer_start");
	nvram_unset("wps_waiting");

	restart_wps();
}

void
wps_enr_save_ap_info()
{
	unsigned char macstr[18];
	char wsec[8];

	sprintf(macstr, "%02X:%02X:%02X:%02X:%02X:%02X",
		ap_list->BSSID[0], ap_list->BSSID[1], ap_list->BSSID[2],
		ap_list->BSSID[3], ap_list->BSSID[4], ap_list->BSSID[5]);
	sprintf(wsec, "%d", ap_list->wep);

	memcpy(BSSID, ap_list->BSSID, ETHER_ADDR_LEN);

	nvram_set("wps_enr_ifname", WIF);
	nvram_set("wps_enr_ssid", ap_list->ssid);
	nvram_set("wps_enr_bssid", macstr);
	nvram_set("wps_enr_wsec", wsec);

	nvram_set("wps_method", "2");
	nvram_set("wps_proc_status", "0");
	nvram_set("wps_unit", "0");
	nvram_set("wps_config_command", "1");
}

void ap_to_wet()
{
	eval("wl", "ap", "0");
	eval("wl", "wet", "1");
	nvram_set("wl_mode", "wet");
	nvram_set("wl0_mode", "wet");
}

void wet_to_ap()
{
	char tmp[100], tmp2[16], prefix[] = "wlXXXXXXXXXXXXXXXXX";
	int keylen = 0;

	eval("wl", "wet", "0");
	eval("wl", "ap", "1");
	nvram_set("wl_mode", "ap");
	nvram_set("wl0_mode", "ap");

	nvram_set("wps_mode", "disabled");
	nvram_set("wps_enr_mode", "disabled");
	nvram_set("wl_wps_mode", "disabled");
	nvram_set("wl_wps_reg", "disabled");
	nvram_set("wl_wps_config_state", "0");
	nvram_set("wl0_wps_mode", "disabled");
	nvram_set("wl0_wps_reg", "disabled");
	nvram_set("wl0_wps_config_state", "0");

	nvram_set("wps_config_state", backup_wps_config_state);
	nvram_set("wps_cli", backup_wps_cli);
	nvram_set("wps_client_role", backup_wps_client_role);
	nvram_set("wps_pbc_force", backup_wps_pbc_force);
	nvram_set("wps_timer_start", backup_wps_timer_start);
	nvram_set("wps_waiting", backup_wps_waiting);

	if (nvram_commit_needed)
	{
		if (nvram_match("sw_mode_ex", "2"))
			strcpy(prefix, "sta_");
		else
			strcpy(prefix, "wl_");
	
		nvram_set(strcat_r(prefix, "ssid", tmp), nvram_safe_get("wl0_ssid"));

		if (nvram_match("wl0_auth", "1"))
			nvram_set(strcat_r(prefix, "auth_mode", tmp), "shared");
		else if (strstr(nvram_safe_get("wl0_akm"), "psk"))
		{
			nvram_set(strcat_r(prefix, "auth_mode", tmp), "psk");

			if (strstr(nvram_safe_get("wl0_akm"), "psk ") && strstr(nvram_safe_get("wl0_akm"), "psk2"))
				nvram_set(strcat_r(prefix, "wpa_mode", tmp), "0");
			else if (strstr(nvram_safe_get("wl0_akm"), "psk2"))
				nvram_set(strcat_r(prefix, "wpa_mode", tmp), "2");
			else
				nvram_set(strcat_r(prefix, "wpa_mode", tmp), "1");

			nvram_set(strcat_r(prefix, "crypto", tmp), nvram_safe_get("wl0_crypto"));
			nvram_set(strcat_r(prefix, "wpa_psk", tmp), nvram_safe_get("wl0_wpa_psk"));
			nvram_set(strcat_r(prefix, "wep_x", tmp), "0");
			nvram_set(strcat_r(prefix, "key", tmp), "2");
		}
		else
			nvram_set(strcat_r(prefix, "auth_mode", tmp), "open");

		if (nvram_match("wl0_wep", "enabled"))
		{
			nvram_set(strcat_r(prefix, "key", tmp), nvram_safe_get("wl0_key"));
			nvram_set(strcat_r(prefix, "key1", tmp), nvram_safe_get("wl0_key1"));
			nvram_set(strcat_r(prefix, "key2", tmp), nvram_safe_get("wl0_key2"));
			nvram_set(strcat_r(prefix, "key3", tmp), nvram_safe_get("wl0_key3"));
			nvram_set(strcat_r(prefix, "key4", tmp), nvram_safe_get("wl0_key4"));

			sprintf(tmp2, "wl0_key%d", atoi(nvram_safe_get("wl0_key")));
			if ( (keylen = strlen(nvram_safe_get(tmp2))) == 5 || (keylen == 10))
				nvram_set(strcat_r(prefix, "wep_x", tmp), "1");
			else
				nvram_set(strcat_r(prefix, "wep_x", tmp), "2");
		}

		if (strstr(nvram_safe_get("wl0_akm"), "psk2"))
			wps_enr_get_scan_results_sp(2);
		else if (strstr(nvram_safe_get("wl0_akm"), "psk"))
			wps_enr_get_scan_results_sp(1);

		fprintf(stderr, "nvram committing...\n");
		/* set varibles to successful settings */
		nvram_set("wps_config_state", "1");
		nvram_commit();

		if (nvram_match("sw_mode_ex", "2"))
			reboot(LINUX_REBOOT_CMD_RESTART);
	}

	exit(EXIT_SUCCESS);
}

int
pbc_enr_monitor()
{
	FILE *fp;
	int wps_apcount = 0;
	char *status;
	int msg_init = 0, msg_enrolling = 0;

	/* write pid */
	if ((fp=fopen("/var/run/pbc_enr_monitor.pid", "w"))!=NULL)
	{
		fprintf(fp, "%d", getpid());
		fclose(fp);
	}
	else
		return 0;

	gettimeofday(&tv_ref, NULL);

	ap_to_wet();

	while (wps_apcount != 1)	// Overlapped PBC Config sessions detection
		wps_apcount = wps_enr_scan_result();

	pbc_restart_wps();
	wps_enr_save_ap_info();

	while (1)
	{
		status = nvram_safe_get("wps_proc_status");
		switch (status[0]) {
		case '1': /* WPS_ASSOCIATED */
			if (!msg_enrolling)
			{
				msg_enrolling = 1;
				fprintf(stderr, "Start enrolling...\n");
			}
			break;
		case '2': /* WPS_OK */
			fprintf(stderr, "Succeeded...\n");
			nvram_commit_needed = 1;
			wet_to_ap();
		case '3': /* WPS_MSG_ERR */
			fprintf(stderr, "Failed...\n");
			wet_to_ap();
		case '4': /* WPS_TIMEOUT */
			fprintf(stderr, "Failed (timeout)...\n");
			wet_to_ap();
		case '8': /* WPS_PBCOVERLAP */
			fprintf(stderr, "Failed (pbc overlap)...\n");
			wet_to_ap();
		default:
			if (!msg_init)
			{
				msg_init = 1;
				fprintf(stderr, "Init...\n");
			}
			break;
		}

		check_reftime();
		usleep(200 * 1000);
	}

	return 0;
}
