/*
 * WPS main (platform dependent portion)
 *
 * Copyright (C) 2009, Broadcom Corporation
 * All Rights Reserved.
 * 
 * This is UNPUBLISHED PROPRIETARY SOURCE CODE of Broadcom Corporation;
 * the contents of this file may not be disclosed to third parties, copied
 * or duplicated in any form, in whole or in part, without the prior
 * written permission of Broadcom Corporation.
 *
 * $Id: wps_linux_main.c,v 1.22 2009/11/12 08:21:18 Exp $
 */
#include <stdio.h>
#include <linux/types.h>
#include <unistd.h>
#include <errno.h>

#include <ctype.h>
#include <typedefs.h>
#include <linux/sockios.h>
typedef u_int64_t u64;
typedef u_int32_t u32;
typedef u_int16_t u16;
typedef u_int8_t u8;
#include <linux/ethtool.h>

#include <bcmcrypto/bn.h>
#include <bcmcrypto/dh.h>
#include <bcmcrypto/md5.h>

#include <wpsheaders.h>
#include <wpscommon.h>
#include <wpserror.h>
#include <bcmnvram.h>
#include <wpstypes.h>
#include <sminfo.h>

#include <netinet/in.h>
#include <net/if.h>
#include <arpa/inet.h>

#include <wlutils.h>
#include <tutrace.h>
#include <sys/ioctl.h>
#include <signal.h>

#include <shutils.h>
#include <ap_upnp_sm.h>
#include <wlif_utils.h>
#include <bcmparams.h>
#include <security_ipc.h>
#include <wps_ui.h>
#include <wps.h>

#define WPSM_PID_FILE_PATH	"/tmp/wps_monitor.pid"

extern void RAND_linux_init();

char wps_conf_buf[1024];
char *wps_conf_buf_ptr;
static int wps_conf_num;
static char *wps_conf_list[256];


void
wps_osl_restart_wl()
{
	/* restart all process */
	system("kill -1 1");
}

int
wps_getProcessStates()
{
	return atoi(nvram_safe_get("wps_proc_status"));
}

void
wps_setProcessStates(int state)
{
	char buf[32];

	sprintf(buf, "%d", state);
	nvram_set("wps_proc_status", buf);
	return;
}

static int
findstr(char *s, char *t)
{
	int len_t = strlen(t);
	int len_s = strlen(s);
	int i;

	if (len_t > len_s)
		return 0;
	for (i = 0; i <= (len_s - len_t); i++) {
		if (*(s+i) == *t)
			if (strstr(s+i, t))
				return 1;
	}

	return 0;
}

static int
set_wsec(char *ifname, void *credential, int mode)
{
	char tmp[128];
	unsigned char psk_mode = 0;
	WpsEnrCred *cred = (WpsEnrCred *)credential;
	char prefix[] = "wlXXXXXXXXXX_";

	/* empty credential check */
	if (cred->ssidLen == 0) {
		TUTRACE((TUTRACE_INFO, "Ignore apply new credential because ssid is empty\n"));
		return 0;
	}

	TUTRACE((TUTRACE_INFO,
		"nvram set key = %s keyMgmt = %s ssid = %s(b_configured)\n",
		cred->nwKey, cred->keyMgmt, cred->ssid));

	/* convert os name to wl name */
	if (osifname_to_nvifname(ifname, prefix, sizeof(prefix)) != 0) {
		TUTRACE((TUTRACE_INFO, "Convert to nvname failed\n"));
		return 0;
	}
	strcat(prefix, "_");

	/* Check credential */
	if (findstr(cred->keyMgmt, "WPA-PSK"))
		psk_mode |= 1;
	if (findstr(cred->keyMgmt, "WPA2-PSK"))
		psk_mode |= 2;

	switch (psk_mode) {
	case 1:
		wps_osl_set_conf(strcat_r(prefix, "akm", tmp), "psk ");
		break;
	case 2:
		wps_osl_set_conf(strcat_r(prefix, "akm", tmp), "psk2 ");
		break;
	case 3:
		wps_osl_set_conf(strcat_r(prefix, "akm", tmp), "psk psk2 ");
		break;
	default:
		wps_osl_set_conf(strcat_r(prefix, "akm", tmp), "");
		break;
	}

	if (findstr(cred->keyMgmt, "SHARED"))
		wps_osl_set_conf(strcat_r(prefix, "auth", tmp), "1");
	else
		wps_osl_set_conf(strcat_r(prefix, "auth", tmp), "0");

	/* set SSID */
	wps_osl_set_conf(strcat_r(prefix, "ssid", tmp), cred->ssid);
	if (psk_mode)
		wps_osl_set_conf(strcat_r(prefix, "wep", tmp), "disabled");

	/* set Encr type */
	if (cred->encrType == WPS_ENCRTYPE_NONE)
		wps_osl_set_conf(strcat_r(prefix, "wep", tmp), "disabled");
	else if (cred->encrType == WPS_ENCRTYPE_WEP)
		wps_osl_set_conf(strcat_r(prefix, "wep", tmp), "enabled");
	else if (cred->encrType == WPS_ENCRTYPE_TKIP)
		wps_osl_set_conf(strcat_r(prefix, "crypto", tmp), "tkip");
	else if (cred->encrType == WPS_ENCRTYPE_AES)
		wps_osl_set_conf(strcat_r(prefix, "crypto", tmp), "aes");
	else if (cred->encrType == (WPS_ENCRTYPE_TKIP | WPS_ENCRTYPE_AES))
		wps_osl_set_conf(strcat_r(prefix, "crypto", tmp), "tkip+aes");
	else
		wps_osl_set_conf(strcat_r(prefix, "crypto", tmp), "tkip");

	if (cred->encrType == WPS_ENCRTYPE_WEP) {
		char buf[16] = {0};
		sprintf(buf, "%d", cred->wepIndex);
		wps_osl_set_conf(strcat_r(prefix, "key", tmp), buf);
		sprintf(buf, "key%d", cred->wepIndex);
		wps_osl_set_conf(strcat_r(prefix, buf, tmp), cred->nwKey);
	}
	else {
		/* set key */
		if (cred->nwKeyLen < 64) {
			wps_osl_set_conf(strcat_r(prefix, "wpa_psk", tmp), cred->nwKey);
		}
		else {
			char temp_key[65] = {0};
			memcpy(temp_key, cred->nwKey, 64);
			temp_key[64] = 0;
			wps_osl_set_conf(strcat_r(prefix, "wpa_psk", tmp), temp_key);
		}
	}

	/* Disable nmode for WEP and TKIP for TGN spec */
	switch (cred->encrType) {
	case WPS_ENCRTYPE_WEP:
	case WPS_ENCRTYPE_TKIP:
		wps_osl_set_conf(strcat_r(prefix, "nmode", tmp), "0");
		break;
	default:
		wps_osl_set_conf(strcat_r(prefix, "nmode", tmp), "-1");
		break;
	}

	return 1;
}

int
wps_set_wsec(int ess_id, char *wps_ifname, void *credential, int mode)
{
	char lan_name_prefix[] = "lanXXXXXXXXX_";
	int instance;
	char *value;
	char *oob = NULL;

	char tmp[100];
	char *wlnames = wps_ifname;
	char *next = NULL;
	char ifname[IFNAMSIZ];


	if (ess_id != -1) {
		sprintf(tmp, "ess%d_upnp_instance", ess_id);
		value = wps_get_conf(tmp);

		if (value != NULL) {
			instance = atoi(value);

			if (instance == 0)
				strcpy(lan_name_prefix, "lan_");
			else
				sprintf(lan_name_prefix, "lan%d_", instance);

			/* Default OOB set to enabled */
			oob = nvram_get(strcat_r(lan_name_prefix, "wps_oob", tmp));
			if (!oob)
				oob = "enabled";

			/* Check wether OOB is true */
			if (!strcmp(oob, "enabled")) {
				/* OOB mode, apply to all wl if */
				sprintf(tmp, "ess%d_wlnames", ess_id);
				wlnames = wps_safe_get_conf(tmp);

				TUTRACE((TUTRACE_INFO,
					"wps_set_wsec: OOB set config\n"));
			}

			/* Set OOB and built-in reg (Per-ESS) */
			if (mode == EModeApProxyRegistrar) {
				wps_osl_set_conf(strcat_r(lan_name_prefix, "wps_oob", tmp),
					"disabled");
				TUTRACE((TUTRACE_INFO, "OOB state configed\n"));

				wps_osl_set_conf(strcat_r(lan_name_prefix, "wps_reg", tmp),
					"enabled");
				TUTRACE((TUTRACE_INFO, "built-in registrar enabled\n"));
			} else if (mode == EModeUnconfAp) {
				wps_osl_set_conf(strcat_r(lan_name_prefix, "wps_oob", tmp),
					"disabled");
				TUTRACE((TUTRACE_INFO, "OOB state configed\n"));
			}
		}
	}

	/* Apply the wsec */
	foreach(ifname, wlnames, next) {
		/* exclude enr_enable (STA mode) */
		sprintf(tmp, "%s_wps_mode", ifname);
		value = wps_safe_get_conf(tmp);
		if (ess_id != -1 && !strcmp(value, "enr_enabled"))
			continue;

		set_wsec(ifname, credential, mode);
		TUTRACE((TUTRACE_INFO,
			"wps_set_wsec: Set config to %s\n", ifname));
	}

	/*
	 * Do commit
	 */
	nvram_commit();

	TUTRACE((TUTRACE_INFO, "wps: wsec configuraiton set completed\n"));
	return 1;
}

int
wps_osl_set_conf(char *config_name, char *config_string)
{
	if (config_name && config_string) {
		TUTRACE((TUTRACE_INFO,	"wps_osl_set_conf: Set %s = %s\n",
			config_name, config_string));
		nvram_set(config_name, config_string);
	}

	return 0;
}

static char *
print_conf(char *os_ifname, char *nv_ifname, char *name, char *value)
{
	char os_prefix[] = "wlXXXXXXXXXX_";
	char nv_prefix[] = "wlXXXXXXXXXX_";
	char tmp[100];

	/* Set up os prefix */
	if (os_ifname)
		snprintf(os_prefix, sizeof(os_prefix), "%s_", os_ifname);
	else
		os_prefix[0] = 0;

	/* Set up nv prefix */
	if (nv_ifname)
		snprintf(nv_prefix, sizeof(nv_prefix), "%s_", nv_ifname);
	else
		nv_prefix[0] = 0;

	/* If value is null, get from NVRAM. */
	if (value == NULL) {
		value = nvram_get(strcat_r(nv_prefix, name, tmp));
		if (!value)
			return NULL;
	}

	wps_conf_buf_ptr += sprintf(wps_conf_buf_ptr, "%s%s=%s\n",
		os_prefix, name, value);

	return value;
}

static unsigned char*
create_device_uuid_string(unsigned char *uuid_string)
{
	unsigned char mac[6] = {0};
	char deviceType[] = "urn:schemas-wifialliance-org:device:WFADevice:1";
	unsigned char *lanmac_str;
	MD5_CTX mdContext;
	unsigned char uuid[16];

	lanmac_str = nvram_get("lan_hwaddr");
	if (lanmac_str)
		ether_atoe(lanmac_str, mac);

	/* Generate hash */
	MD5Init(&mdContext);
	MD5Update(&mdContext, mac, sizeof(mac));
	MD5Update(&mdContext, deviceType, strlen(deviceType));
	MD5Final(uuid, &mdContext);

	sprintf(uuid_string, "%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x",
		uuid[0], uuid[1], uuid[2], uuid[3], uuid[4], uuid[5], uuid[6], uuid[7],
		uuid[8], uuid[9], uuid[10], uuid[11], uuid[12], uuid[13], uuid[14], uuid[15]);

	return uuid_string;
}

void
wps_osl_build_conf()
{
	int i, unit;
	char *value;
	char *item, *p, *next;
	char *ifnames;
	char wlnames[256];
	char prefix[] = "wlXXXXXXXXXX_";
	char name[IFNAMSIZ], os_name[IFNAMSIZ], wl_name[IFNAMSIZ];
	char ess_name[IFNAMSIZ], lan_name[IFNAMSIZ];
	char tmp[100];
	int bandtype = 0;
	int band = 0;
	char key[8];
	int ess_id = 0;
	int upnp_instance;
	char *wps_mode;
	unsigned char uuid_string[36];

	/* Init print pointer */
	wps_conf_buf_ptr = wps_conf_buf;

	/* Start config print */
	print_conf(NULL, NULL, "wps_aplockdown_cap", NULL);
	print_conf(NULL, NULL, "wps_aplockdown_count", NULL);
	print_conf(NULL, NULL, "wps_aplockdown_duration", NULL);
	print_conf(NULL, NULL, "wps_aplockdown_ageout", NULL);
	print_conf(NULL, NULL, "wps_aplockdown_forceon", NULL);
	print_conf(NULL, NULL, "wps_device_pin", NULL);
	print_conf(NULL, NULL, "wps_enr_wsec", NULL);
	print_conf(NULL, NULL, "wps_enr_ssid", NULL);
	print_conf(NULL, NULL, "wps_enr_bssid", NULL);
	print_conf(NULL, NULL, "wps_mixedmode", NULL);
	print_conf(NULL, NULL, "wps_random_ssid_prefix", NULL);
	print_conf(NULL, NULL, "wps_randomssid", NULL);
	print_conf(NULL, NULL, "wps_randomkey", NULL);
	print_conf(NULL, NULL, "wps_device_name", NULL);
	print_conf(NULL, NULL, "wps_mfstring", NULL);
	print_conf(NULL, NULL, "wps_modelname", NULL);
	print_conf(NULL, NULL, "wps_modelnum", NULL);
	print_conf(NULL, NULL, "boardnum", NULL);
	print_conf(NULL, NULL, "wps_config_method", NULL);

	/* Set device UUID */
	print_conf(NULL, NULL, "wps_uuid", create_device_uuid_string(uuid_string));

	ess_id = 0;
	/* (LAN) for UPnP and push button */
	for (i = 0; i < 256; i ++) {
		/* Taking care of LAN interface names */
		if (i == 0) {
			strcpy(name, "lan_ifnames");
			strcpy(lan_name, "lan");
		}
		else {
			sprintf(name, "lan%d_ifnames", i);
			sprintf(lan_name, "lan%d", i);
		}

		ifnames = nvram_get(name);
		if (!ifnames)
			continue;

		/* Search for wl_name in ess */
		upnp_instance = -1;
		sprintf(ess_name, "ess%d", ess_id);

		memset(wlnames, 0, sizeof(wlnames));
		band = 0;

		foreach(name, ifnames, next) {
			if (nvifname_to_osifname(name, os_name, sizeof(os_name)) < 0)
				continue;
			if (wl_probe(os_name) ||
				wl_ioctl(os_name, WLC_GET_INSTANCE, &unit, sizeof(unit)))
				continue;

			/* Convert eth name to wl name */
			if (osifname_to_nvifname(name, wl_name, sizeof(wl_name)) != 0)
				continue;

			/* Get configured wireless address */
			snprintf(prefix, sizeof(prefix), "%s_", wl_name);

			wps_mode = nvram_get(strcat_r(prefix, "wps_mode", tmp));
			if (!wps_mode ||
				(strcmp(wps_mode, "enabled") != 0 &&
				strcmp(wps_mode, "enr_enabled") != 0)) {
				continue;
			}

			/* If the radio is disabled, skip it */
			if (!print_conf(os_name, wl_name, "hwaddr", NULL))
				continue;

			/* Print wl specific variables */
			print_conf(os_name, wl_name, "wps_mode", wps_mode);
			print_conf(os_name, wl_name, "mode", NULL);

			/* Set only for AP mode */
			if (strcmp(wps_mode, "enabled") == 0) {
				print_conf(os_name, wl_name, "wep", NULL);
				print_conf(os_name, wl_name, "auth", NULL);
				print_conf(os_name, wl_name, "ssid", NULL);
				print_conf(os_name, wl_name, "akm", NULL);
				print_conf(os_name, wl_name, "crypto", NULL);
				print_conf(os_name, wl_name, "wpa_psk", NULL);

				value = print_conf(os_name, wl_name, "key", NULL);
				if (value) {
					sprintf(key, "key%s", value);
					print_conf(os_name, wl_name, key, NULL);
				}

				/* Get per lan RF bands */
				wl_ioctl(os_name, WLC_GET_BAND, &bandtype, sizeof(bandtype));
				if (bandtype == WLC_BAND_AUTO) {
					int list[3];
					int j;

					wl_ioctl(os_name, WLC_GET_BANDLIST, list, sizeof(list));
					if (list[0] > 2)
						list[0] = 2;

					bandtype = 0;
					for (j = 0; j < list[0]; j++) {
						switch (list[j]) {
						case WLC_BAND_5G:
						case WLC_BAND_2G:
							bandtype |= list[j];
							break;
						default:
							break;
						}
					}
				}

				sprintf(tmp, "%d", bandtype);
				print_conf(os_name, wl_name, "band", tmp);

				/* Aggregage band type */
				band |= bandtype;

				TUTRACE((TUTRACE_INFO, "%s(%s) bandtype = %d\n",
					os_name, wl_name, bandtype));

				/* We got an AP interface, need UPnP attached */
				upnp_instance = i;
			}

			/* Save wlnames of this lan */
			if (strlen(wlnames))
				strcat(wlnames, " ");

			strcat(wlnames, os_name);
		}

		if (strlen(wlnames)) {
			print_conf(ess_name, NULL, "wlnames", wlnames);
			print_conf(ess_name, lan_name, "wps_reg", NULL);
			print_conf(ess_name, lan_name, "wps_oob", NULL);

			/* LED id */
			sprintf(tmp, "%d", ess_id);
			print_conf(ess_name, NULL, "led_id", tmp);

			if (band) {
				/* ALL RF bands supported */
				switch (band) {
				case WLC_BAND_ALL:
					bandtype = WPS_RFBAND_24GHZ | WPS_RFBAND_50GHZ;
					break;
				case WLC_BAND_5G:
					bandtype = WPS_RFBAND_50GHZ;
					break;
				case WLC_BAND_2G:
				default:
					bandtype = WPS_RFBAND_24GHZ;
					break;
				}
				sprintf(tmp, "%d", bandtype);
				print_conf(ess_name, NULL, "band", tmp);
			}

			if (upnp_instance != -1) {
				sprintf(tmp, "%d", upnp_instance);
				print_conf(ess_name, NULL, "upnp_instance", tmp);
			}

			ess_id++;
		}
	}

	/* (WAN) for push button */
	for (i = 0; i < 256; i ++) {
		/* Taking care of WAN interface names */
		if (i == 0)
			strcpy(name, "wan_ifnames");
		else
			sprintf(name, "wan%d_ifnames", i);

		ifnames = nvram_get(name);
		if (!ifnames)
			continue;

		/* Set ESS instance prefix */
		sprintf(ess_name, "ess%d", ess_id);

		/* Search for wl_name in it */
		memset(wlnames, 0, sizeof(wlnames));
		foreach(name, ifnames, next) {
			if (nvifname_to_osifname(name, os_name, sizeof(os_name)) < 0)
				continue;
			if (wl_probe(os_name) ||
				wl_ioctl(os_name, WLC_GET_INSTANCE, &unit, sizeof(unit)))
				continue;

			/* Convert eth name to wl name */
			if (osifname_to_nvifname(name, wl_name, sizeof(wl_name)) != 0)
				continue;

			/* Get configured wireless address */
			snprintf(prefix, sizeof(prefix), "%s_", wl_name);

			value = nvram_get(strcat_r(prefix, "wps_mode", tmp));
			if (!value || strcmp(value, "enr_enabled") != 0)
				continue;

			if (!print_conf(os_name, wl_name, "hwaddr", NULL))
				continue;

			/* Print wl specific variables */
			print_conf(os_name, wl_name, "mode", NULL);

			/* Print wl wps specific variables */
			print_conf(os_name, wl_name, "wps_mode", NULL);

			/* Save wlnames of this lan */
			if (strlen(wlnames))
				strcat(wlnames, " ");

			strcat(wlnames, os_name);
		}

		if (strlen(wlnames)) {
			print_conf(ess_name, NULL, "wlnames", wlnames);

			/* LED id */
			sprintf(tmp, "%d", ess_id++);
			print_conf(ess_name, NULL, "led_id", tmp);
		}
	}

	/* Save maximum instance */
	sprintf(tmp, "%d", ess_id);
	print_conf(NULL, NULL, "wps_ess_num", tmp);

	/* Seperate wps_conf_buf into wps_conf_list[] */
	wps_conf_num = 0;
	for (item = wps_conf_buf, p = item; item && item[0]; item = next, p = 0) {
		/* Get next token */
		strtok_r(p, "\n", &next);
		wps_conf_list[wps_conf_num++] = item;
	}
}

/*
 * Name        : main
 * Description : Main entry point for the WPS monitor
 * Arguments   : int argc, char *argv[] - command line parameters
 * Return type : int
 */
int
main(int argc, char* argv[])
{
	FILE *pidfile;
	int flag;

	/* Show usage */
	if (argc > 1) {
		fprintf(stderr, "Usage: wps_monitor\n");
		return -1;
	}

	/*
	 * Check whether this process is running
	 */
	if ((pidfile = fopen(WPSM_PID_FILE_PATH, "r"))) {
		fprintf(stderr, "%s: wps_monitor has been started\n", __FILE__);

		fclose(pidfile);
		return -1;
	}

	/* Cread pid file */
	if ((pidfile = fopen(WPSM_PID_FILE_PATH, "w"))) {
		fprintf(pidfile, "%d\n", getpid());
		fclose(pidfile);
	}
	else {
		perror("pidfile");
		exit(errno);
	}

	/* establish a handler to handle SIGTERM. */
	signal(SIGTERM, wps_stophandler);
	signal(SIGUSR1, wps_restarthandler);

	RAND_linux_init();

	wps_osl_build_conf();

	/* Enter main loop */
	flag = wps_mainloop(wps_conf_num, wps_conf_list);

	if (flag & WPSM_WKSP_FLAG_SET_RESTART)
		nvram_set("wps_restart", "1");

	/* need restart wireless */
	if (flag & WPSM_WKSP_FLAG_RESTART_WL)
		wps_osl_restart_wl();

	/* Destroy pid file */
	unlink(WPSM_PID_FILE_PATH);

	return 0;
}
