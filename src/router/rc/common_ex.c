/*
 * Copyright 2004, ASUSTek Inc.
 * All Rights Reserved.
 * 
 * THIS SOFTWARE IS OFFERED "AS IS", AND ASUS GRANTS NO WARRANTIES OF ANY
 * KIND, EXPRESS OR IMPLIED, BY STATUTE, COMMUNICATION OR OTHERWISE. BROADCOM
 * SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A SPECIFIC PURPOSE OR NONINFRINGEMENT CONCERNING THIS SOFTWARE.
 *
 * $Id: common_ex.c,v 1.16 2008/11/10 02:33:53 james26_jang Exp $
 */


#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <signal.h>
#include <string.h>
#include <bcmnvram.h>
#include <shutils.h>
#include <netconf.h>
#include <wlioctl.h>
#include <sys/time.h>
#include <syslog.h>
#include <stdarg.h>
#include <arpa/inet.h>
#include "rc.h"

#include <sys/syscall.h>
#include <linux/unistd.h>

#define XSTR(s) STR(s)
#define STR(s) #s

static char list[2048];

/* remove space in the end of string */
char *trim_r(char *str)
{
	int i;

	i=strlen(str);

	while(i>=1)
	{
		if (*(str+i-1) == ' ' || *(str+i-1) == 0x0a || *(str+i-1) == 0x0d) *(str+i-1)=0x0;
		else break;
		i--;
	}
	return(str);
}

/* convert mac address format from XXXXXXXXXXXX to XX:XX:XX:XX:XX:XX */
char *mac_conv(char *mac_name, int idx, char *buf)
{
	char *mac, name[32];
	int i, j;

	if (idx!=-1)	
		sprintf(name, "%s%d", mac_name, idx);
	else sprintf(name, "%s", mac_name);

	mac = nvram_safe_get(name);

	if (strlen(mac)==0) 
	{
		buf[0] = 0;
	}
	else
	{
		j=0;	
		for(i=0; i<12; i++)
		{		
			if (i!=0&&i%2==0) buf[j++] = ':';
			buf[j++] = mac[i];
		}
		buf[j] = 0;
	}

	dprintf("mac: %s\n", buf);

	return(buf);
}

void getsyspara(void)
{
	FILE *fp;
//	char buf[1];
	unsigned long *imagelen;
	char dataPtr[4];
	char verPtr[64];
	char productid[13];
	char fwver[12];
//	int i;
	
	strcpy(productid, "WLHDD");
	strcpy(fwver, "0.1.0.1");

#ifdef LINUX26
	if((fp = fopen("/dev/mtd1", "rb")) != NULL){
#else
	if((fp = fopen("/dev/mtd/1", "rb")) != NULL){
#endif
		if(fseek(fp, 4, SEEK_SET) != 0) goto write_ver;
		fread(dataPtr, 1, 4, fp);
		imagelen = (unsigned long *)dataPtr;

		dprintf("image len %x\n", *imagelen);
		if(fseek(fp, *imagelen-64, SEEK_SET) != 0)
			goto write_ver;
		
		dprintf("seek\n");
		if(!fread(verPtr, 1, 64, fp))
			goto write_ver;
		
		dprintf("ver %x %x", verPtr[0], verPtr[1]);
		strncpy(productid, verPtr+4, 12);
		productid[12] = 0;
		sprintf(fwver, "%d.%d.%d.%d", verPtr[0], verPtr[1], verPtr[2], verPtr[3]);
		
		dprintf("get fw ver: %s\n", productid);
		fclose(fp);
	}
	
write_ver:
	// its a ugle solution for Product ID
	if(strstr(productid, "WL500gx"))
		nvram_set("productid", "WL500g.Deluxe");
//	else if (strstr(productid, "WL500gp"))
//		nvram_set("productid", "WL500g.Premium");
	else
		nvram_set("productid", trim_r(productid));
	
	nvram_set("firmver", trim_r(fwver));
}

void wan_netmask_check(void)
{
	unsigned int ip, gw, nm, lip, lnm;

	if (nvram_match("wan0_proto", "static")
			|| nvram_match("wan0_proto", "pptp")
			|| nvram_match("wan0_proto", "l2tp"))
	{
		ip = inet_addr(nvram_safe_get("wan_ipaddr"));
		gw = inet_addr(nvram_safe_get("wan_gateway"));
		nm = inet_addr(nvram_safe_get("wan_netmask"));

		lip = inet_addr(nvram_safe_get("lan_ipaddr"));
		lnm = inet_addr(nvram_safe_get("lan_netmask"));

		dprintf("ip : %x %x %x\n", ip, gw, nm);

		if (ip==0x0 && (nvram_match("wan0_proto", "pptp") || nvram_match("wan0_proto", "l2tp")))
			return;

		if (ip==0x0 || (ip&lnm)==(lip&lnm))
		{
			nvram_set("wan_ipaddr", "1.1.1.1");
			nvram_set("wan_netmask", "255.0.0.0");	
			nvram_set("wan0_ipaddr", nvram_safe_get("wan_ipaddr"));
			nvram_set("wan0_netmask", nvram_safe_get("wan_netmask"));
		}

		// check netmask here
		if (gw==0 || gw==0xffffffff || (ip&nm)==(gw&nm))
		{
			nvram_set("wan0_netmask", nvram_safe_get("wan_netmask"));
		}
		else
		{		
			for(nm=0xffffffff;nm!=0;nm=(nm>>8))
			{
				if ((ip&nm)==(gw&nm)) break;
			}

			dprintf("nm: %x\n", nm);

			if (nm==0xffffffff) nvram_set("wan0_netmask", "255.255.255.255");
			else if (nm==0xffffff) nvram_set("wan0_netmask", "255.255.255.0");
			else if (nm==0xffff) nvram_set("wan0_netmask", "255.255.0.0");
			else if (nm==0xff) nvram_set("wan0_netmask", "255.0.0.0");
			else nvram_set("wan0_netmask", "0.0.0.0");
		}

	}
}

//#ifdef PBCENR
void init_switch_mode()						// Jiahao for RT-N12
{
#ifdef RTN12
	sw_gpio_init();						// init for switch mode retrieval
	sw_mode_check();					// save switch mode into nvram name sw_mode
//#endif
#else
//#ifdef RTN10 | RTN16
	if (!nvram_get("sw_mode")) {
		nvram_set("sw_mode", "1");
		nvram_set("sw_mode_ex", "1");
	}
#endif
	nvram_set("sw_mode_ex", nvram_safe_get("sw_mode"));	// save working switch mode into nvram name sw_mode_ex

#ifdef RT-N12
	if (nvram_match("sw_mode_ex", "1"))			// Gateway mode
	{
		nvram_set("wan_nat_x", "1");
		nvram_set("wan_route_x", "IP_Routed");
		nvram_set("wl_mode_ex", "ap");
	}
	else if (nvram_match("sw_mode_ex", "2"))		// Repeater mode
	{
		nvram_set("wan_nat_x", "0");
		nvram_set("wan_route_x", "IP_Bridged");

		if (nvram_invmatch("sta_ssid", ""))
			nvram_set("wl_mode_ex", "re");
		else
			nvram_set("wl_mode_ex", "ap");
	}
	else							// AP mode
	{
		nvram_set("wan_nat_x", "0");
		nvram_set("wan_route_x", "IP_Bridged");
		nvram_set("wl_mode_ex", "ap");
	}
#endif
	nvram_set("qos_wireless_enable", "0");
}
//#endif

/* This function is used to map nvram value from asus to Broadcom */
void convert_asus_values(int skipflag)
{	
	char tmpstr[32], macbuf[36];
	char servers[64];
	char ifnames[36];
	char sbuf[64];
	int i, num;
	char *ptr;
	FILE *fp;
cprintf("*** Start convert_asus_values(%d). ***\n", skipflag);
	
// 2008.03 James.  {
	// Fixed the wrong value of nvram. {
	if(nvram_match("macfilter_enable_x", "disabled"))
		nvram_set("macfilter_enable_x", "0");
	
	if(nvram_match("wl_frameburst", "0"))
		nvram_set("wl_frameburst", "off");
	
	if(nvram_match("wl_crypto", "0"))
		nvram_set("wl_crypto", "tkip");
	
//	if(nvram_match("wl_preauth", "1"))
//		nvram_set("wl_preauth", "enabled");
//	else if(nvram_match("wl_preauth", "0"))
//		nvram_set("wl_preauth", "disabled");
	if (nvram_match("wl_preauth", "enabled"))
		nvram_set("wl_preauth", "1");
	else if (nvram_match("wl_preauth", "disabled"))
		nvram_set("wl_preauth", "0");
	// Fixed the wrong value of nvram. }
	
	char *ddns_hostname_x = nvram_safe_get("ddns_hostname_x");
	char *follow_info = strchr(ddns_hostname_x, '.');
	char ddns_account[64];
	int len = 0;
	
	if(nvram_match("ddns_enable_x", "1") && nvram_match("ddns_server_x", "WWW.ASUS.COM")
			&& follow_info != NULL && !strcmp(follow_info, ".asuscomm.com")){
		len = strlen(ddns_hostname_x)-strlen(follow_info);
		
		memset(ddns_account, 0, 64);
		strncpy(ddns_account, ddns_hostname_x, len);
		
		nvram_set("computer_name", ddns_account);
		nvram_set("computer_nameb", ddns_account);
	}
// 2008.03 James. }
	
	if(!skipflag)
	{
		getsyspara();
		/* convert country code from regulation_domain */
		convert_country();
	}
	
#ifndef CDMA
	if(nvram_match("wan_proto", "cdma"))
		nvram_set("wan_proto", "dhcp");
#endif	// CDMA

#ifdef DLM
        if (nvram_match("ftp_lang","")){
                if((nvram_match("regulation_domain","0x47TW"))||(nvram_match("regulation_domain","0X47TW")))
                        nvram_set("ftp_lang","TW");
                else if ((nvram_match("regulation_domain","0x44CN"))||(nvram_match("regulation_domain","0X44CN")))
                        nvram_set("ftp_lang", "CN");
                else if ((nvram_match("regulation_domain","0x46KR"))||(nvram_match("regulation_domain","0X46KR")))
                        nvram_set("ftp_lang", "KR");
                else
                        nvram_set("ftp_lang","EN");
        }
#endif	// DLM
#ifdef WL500GP
	if(nvram_invmatch("httpd_die_reboot", ""))
		nvram_set("httpd_die_reboot", "");

	/* Clean MFG test values when boot */
	if (nvram_invmatch("asus_mfg", ""))
		nvram_set("asus_mfg", "");
#ifdef RTN16
	if (nvram_invmatch("asus_mfg_webcam", ""))
		nvram_set("asus_mfg_webcam", "");
	if (nvram_invmatch("asus_mfg_flash", ""))
		nvram_set("asus_mfg_flash", "");
	if (nvram_invmatch("asus_mfg_printer", ""))
		nvram_set("asus_mfg_printer", "");
#endif
	if (nvram_invmatch("btn_rst", ""))
		nvram_set("btn_rst", "");
	if (nvram_invmatch("btn_ez", ""))
		nvram_set("btn_ez", "");
#endif	// WL500GP

	if (nvram_match("productid", "WL500b") || 
	    nvram_match("productid", "WL500bv2"))
	{
		nvram_set("wl_gmode", "0");
	}

	cprintf("read from nvram\n");

	nvram_set("wl_wps_mode", nvram_safe_get("wps_mode"));
	nvram_set("wlx_wps_mode", nvram_safe_get("wl_wps_mode"));
	nvram_set("wlx_wps_reg", nvram_safe_get("wl_wps_reg"));
//	nvram_set("wl_wps_config_state", nvram_safe_get("wps_config_state"));
//	nvram_set("wl0_wps_config_state", nvram_safe_get("wl_wps_config_state"));
	
	/* These are used for WPS*/
	if (nvram_match("x_Setting", "1") || nvram_match("wps_config_state", "1") || nvram_match("lan_wps_oob","disabled")) {
		nvram_set("is_modified", "1");
		nvram_set("is_default", "0");
		nvram_set("x_Setting", "1");
		nvram_set("wl0_wps_config_state", "1");
		nvram_set("lan_wps_oob","disabled");
		nvram_set("lan1_wps_oob","disabled");
	}

	if (nvram_match("wl0_wps_mode", "enabled")) {
		nvram_set("wps_mode", "enabled");
		nvram_set("wps_enr_mode", "disabled");
	}
	else if (nvram_match("wl0_wps_mode", "enr_enabled")) {
		nvram_set("wps_mode", "disabled");
		nvram_set("wps_enr_mode", "enabled");
	}
	else {
		nvram_set("wps_mode", "disabled");
		nvram_set("wps_enr_mode", "disabled");
	}

	nvram_set ("wps_device_pin", nvram_safe_get("secret_code"));

//	if (nvram_match("wps_client_role", "enrollee"))
//		nvram_set("wl0_akm",nvram_safe_get("wl0_akm"));


	/* Wireless Section */
	nvram_set("wl0_bss_enabled", nvram_safe_get("wl_bss_enabled"));
	
	/* Country Code */
	nvram_set("wl0_country_code", nvram_safe_get("wl_country_code"));

	/* GMODE */
	nvram_set("wl0_gmode", nvram_safe_get("wl_gmode"));
	
	/* NMODE */
	nvram_set("wl0_nmode", nvram_safe_get("wl_nmode"));
	
	if (nvram_match("wl_gmode_protection_x", "1"))
	{
		// cprintf("set to auto\n");
		nvram_set("wl_gmode_protection", "auto");
		nvram_set("wl0_gmode_protection", "auto");
		// n-protection options
		nvram_set("wl_nmode_protection", "auto");
		nvram_set("wl0_nmode_protection", "auto");
	}
	else
	{
		// cprintf("set to off");
		nvram_set("wl_gmode_protection", "off");
		nvram_set("wl0_gmode_protection", "off");
		// n-protection options
		nvram_set("wl_nmode_protection", "off");
		nvram_set("wl0_nmode_protection", "off");
	}	

	if (nvram_match("wl_wep_x", "0") || nvram_match("wl_auth_mode", "psk"))
		nvram_set("wlx_wep", "disabled");
	else nvram_set("wlx_wep", "enabled");

	if (nvram_match("wl_auth_mode", "shared"))
		nvram_set("wlx_auth", "1");
	else nvram_set("wlx_auth", "0");


#ifdef WPA2_WMM
// 2008.06 James. {
	/*if(nvram_match("wl_auth_mode", "wpa")){
		if(nvram_match("wl_wpa_mode", "4")){
			if(nvram_match("wl_crypto", "tkip") || nvram_match("wl_crypto", "0")){
				nvram_set("wl_akm", "wpa");
				nvram_set("wl0_akm", "wpa");
			}
			else if(nvram_match("wl_crypto", "aes")){
				nvram_set("wl_akm", "wpa2");
				nvram_set("wl0_akm", "wpa2");
			}
			else{
				nvram_set("wl_akm", "wpa wpa2");
				nvram_set("wl0_akm", "wpa wpa2");
			}
		}
		else if(nvram_match("wl_wpa_mode", "3")){
			nvram_set("wl_akm", "wpa");
			nvram_set("wl0_akm", "wpa");
		}
	}
	else if(nvram_match("wl_auth_mode", "wpa2")){
		nvram_set("wl_akm", "wpa2");
		nvram_set("wl0_akm", "wpa2");
	}
	else if(nvram_match("wl_auth_mode", "psk")){
		if(nvram_match("wl_wpa_mode", "0")){
			if(nvram_match("wl_crypto", "tkip") || nvram_match("wl_crypto", "0")){
				nvram_set("wl_akm", "psk");
				nvram_set("wl0_akm", "psk");
			}
			else if(nvram_match("wl_crypto", "aes")){
				nvram_set("wl_akm", "psk2");
				nvram_set("wl0_akm", "psk2");
			}
			else{
				nvram_set("wl_akm", "psk psk2");
				nvram_set("wl0_akm", "psk psk2");
			}
		}
		else if(nvram_match("wl_wpa_mode", "1")){
			nvram_set("wl_akm", "psk");
			nvram_set("wl0_akm", "psk");
		}
		else if(nvram_match("wl_wpa_mode", "2")){
			nvram_set("wl_akm", "psk2");
			nvram_set("wl0_akm", "psk2");
		}
	}
	else{
		nvram_set("wl_akm", "");
		nvram_set("wl0_akm", "");
	}//*/
	if(nvram_match("wl_auth_mode", "psk")){
		if(nvram_match("wl_wpa_mode", "1")){
			nvram_set("wl_akm", "psk");
			nvram_set("wlx_akm", "psk");
		}
		else if(nvram_match("wl_wpa_mode", "2")){
			nvram_set("wl_akm", "psk2");
			nvram_set("wlx_akm", "psk2");
		}
		else{	// wl_wpa_mode == 0
			nvram_set("wl_akm", "psk psk2"); // according to the official firmware.
			nvram_set("wlx_akm", "psk psk2");
		}
	}
	else if(nvram_match("wl_auth_mode", "wpa") || nvram_match("wl_auth_mode", "wpa2")){
		if(nvram_match("wl_auth_mode", "wpa2")){
			nvram_set("wl_akm", "wpa2");
			nvram_set("wlx_akm", "wpa2");
		}
		else if(nvram_match("wl_wpa_mode", "3")){
			nvram_set("wl_akm", "wpa");
			nvram_set("wlx_akm", "wpa");
		}
		else{	// wl_wpa_mode == 4
			nvram_set("wl_akm", "wpa wpa2");
			nvram_set("wlx_akm", "wpa wpa2");
		}
	}
	else{
		nvram_set("wl_akm", "");
		nvram_set("wlx_akm", "");
	}//*/
	
	/* avoid the Enterprise-TKIP with 40MHz case, Yen */
//	if (nvram_match("wl_auth_mode", "wpa") && (nvram_match("wl_crypto", "tkip")))
//		nvram_set("wl_nbw_cap", "0");

// 2008.06 James. }
	// thanks for Oleg
	nvram_set("wlx_auth_mode", nvram_match("wl_auth_mode", "radius") ? "radius" : "none");
	
	nvram_set("wlx_preauth", nvram_safe_get("wl_preauth"));
	nvram_set("wlx_net_reauth", nvram_safe_get("wl_net_reauth"));
	nvram_set("wl0_wme", nvram_safe_get("wl_wme"));
	nvram_set("wl0_wme_no_ack", nvram_safe_get("wl_wme_no_ack"));
	nvram_set("wl0_wme_sta_bk", nvram_safe_get("wl_wme_sta_bk"));
	nvram_set("wl0_wme_sta_be", nvram_safe_get("wl_wme_sta_be"));
	nvram_set("wl0_wme_sta_vi", nvram_safe_get("wl_wme_sta_vi"));
	nvram_set("wl0_wme_sta_vo", nvram_safe_get("wl_wme_sta_vo"));
	nvram_set("wl0_wme_ap_bk", nvram_safe_get("wl_wme_ap_bk"));
	nvram_set("wl0_wme_ap_be", nvram_safe_get("wl_wme_ap_be"));
	nvram_set("wl0_wme_ap_vi", nvram_safe_get("wl_wme_ap_vi"));
	nvram_set("wl0_wme_ap_vo", nvram_safe_get("wl_wme_ap_vo"));
// 2008.06 James. {
	nvram_set("wl0_wme_txp_bk", nvram_safe_get("wl_wme_txp_bk"));
	nvram_set("wl0_wme_txp_be", nvram_safe_get("wl_wme_txp_be"));
	nvram_set("wl0_wme_txp_vi", nvram_safe_get("wl_wme_txp_vi"));
	nvram_set("wl0_wme_txp_vo", nvram_safe_get("wl_wme_txp_vo"));
// 2008.06 James. }
#else	// WPA2_WMM
	nvram_set("wl0_auth_mode", nvram_safe_get("wl_auth_mode"));
	nvram_set("wl_akm", "");
	nvram_set("wl0_akm", "");
	nvram_set("wl0_wme", "off");
#endif	// WPA2_WMM
//convert n mode nvram
if(nvram_match("wl_gmode", "4"))	// 11B-only
{       nvram_set("wl0_rate", nvram_safe_get("wl_rate"));
        nvram_set("wl0_nmcsidx","-2"); 	// Use Legacy Rate
        nvram_set("wl0_nmode", "0");
        nvram_set("wl0_gmode", "0");
        nvram_set("wl0_nreqd", "0");
        nvram_set("wl_nreqd", "0");
}
else if(nvram_match("wl_gmode", "3"))	// 11G-only
{       nvram_set("wl0_rate", nvram_safe_get("wl_rate"));
        nvram_set("wl0_nmcsidx","-2");	// Use Legacy Rate
        nvram_set("wl0_nmode", "0");
        nvram_set("wl0_gmode", "2");
        nvram_set("wl0_nreqd", "0");
        nvram_set("wl_nreqd", "0");
}
else if(nvram_match("wl_gmode", "2"))	// 11N-only
{       
	nvram_set("wl0_nmcsidx", "32");	// Max rate for 11N
        nvram_set("wl0_rate", "0");
        nvram_set("wl0_nmode", "-1");
        nvram_set("wl0_gmode", "1");
        nvram_set("wl0_nreqd", "1");
        nvram_set("wl_nreqd", "1");
}
else if(nvram_match("wl_gmode","0"))	// Auto
{
	nvram_set("wl0_nmcsidx", "-1");	// Auto rate for 11N
        nvram_set("wl0_rate", "0");
        nvram_set("wl0_nmode", "-1");
        nvram_set("wl0_gmode", "1");
        nvram_set("wl0_nreqd", "0");
        nvram_set("wl_nreqd", "0");
}
else if(nvram_match("wl_gmode","1"))	// B/G Mixed
{
        nvram_set("wl0_rate", nvram_safe_get("wl_rate"));
	nvram_set("wl0_nmcsidx", "-2");	// Use Legacy Rate
        nvram_set("wl0_nmode", "0");
        nvram_set("wl0_gmode", "1");
        nvram_set("wl0_nreqd", "0");
        nvram_set("wl_nreqd", "0");
}
	// Bandwidth for 11N
#ifdef LANGUAGE_KR
        nvram_set("wl_nbw","20");
	nvram_set("wl_nbw_cap", "0");
#else
	if (nvram_match ("wl_nbw_cap", "0")) {	// 20 MHz in Both Bands
		nvram_set ("wl_nbw", "20");
		nvram_set ("wl_nctrlsb", "none");
		nvram_set ("wl_obss_coex", "0");
	}
	else {					// 40 MHz in Both Bands
		nvram_set ("wl_nbw", "40");		
		nvram_set ("wl_nctrlsb", "lower");
//		if (nvram_invmatch("wl_gmode","2"))
		nvram_set ("wl_obss_coex", "1");
//		else
//			nvram_set ("wl_obss_coex", "0");
	}
#endif
                                                                                                    
        nvram_set("wl0_nbw", nvram_safe_get("wl_nbw"));
	nvram_set("wl0_nbw_cap", nvram_safe_get("wl_nbw_cap"));
	nvram_set("wl0_obss_coex", nvram_safe_get("wl_obss_coex"));
        nvram_set("wl0_nctrlsb", nvram_safe_get("wl_nctrlsb"));
        nvram_set("wl0_nband", nvram_safe_get("wl_nband"));
	nvram_set("wl0_mimo_preamble", nvram_safe_get("wl_mimo_preamble"));
	nvram_set("wl0_wme_apsd", nvram_safe_get("wl_wme_apsd"));

	nvram_set("lan_ipv6_mode", "0"); /* for Windows 7 test */

	nvram_set("wlx_ssid", nvram_safe_get("wl_ssid"));
	nvram_set("wl0_channel", nvram_safe_get("wl_channel"));
	nvram_set("wl0_country_code", nvram_safe_get("wl_country_code"));
//	nvram_set("wl0_rate", nvram_safe_get("wl_rate"));
	nvram_set("wl0_mrate", nvram_safe_get("wl_mrate"));
	nvram_set("wl0_rateset", nvram_safe_get("wl_rateset"));
	nvram_set("wl0_frag", nvram_safe_get("wl_frag"));
	nvram_set("wl0_rts", nvram_safe_get("wl_rts"));
	nvram_set("wl0_dtim", nvram_safe_get("wl_dtim"));
	nvram_set("wl0_bcn", nvram_safe_get("wl_bcn"));
	nvram_set("wl0_plcphdr", nvram_safe_get("wl_plcphdr"));
	nvram_set("wlx_crypto", nvram_safe_get("wl_crypto"));
	nvram_set("wlx_wpa_psk", nvram_safe_get("wl_wpa_psk"));
	nvram_set("wlx_key", nvram_safe_get("wl_key"));
	nvram_set("wlx_key1", nvram_safe_get("wl_key1"));
	nvram_set("wlx_key2", nvram_safe_get("wl_key2"));
	nvram_set("wlx_key3", nvram_safe_get("wl_key3"));
	nvram_set("wlx_key4", nvram_safe_get("wl_key4"));
	
	nvram_set("wl0_closed", nvram_safe_get("wl_closed"));
	nvram_set("wl0_frameburst", nvram_safe_get("wl_frameburst"));
	nvram_set("wl0_afterburner", nvram_safe_get("wl_afterburner"));
	nvram_set("wl0_ap_isolate", nvram_safe_get("wl_ap_isolate"));
	nvram_set("wl0_radio", nvram_safe_get("wl_radio_x"));

//	if (nvram_match("wl_wpa_mode", ""))
//		nvram_set("wl_wpa_mode", "0");


	nvram_set("wl0_radius_ipaddr", nvram_safe_get("wl_radius_ipaddr"));
	nvram_set("wl0_radius_port", nvram_safe_get("wl_radius_port"));
	nvram_set("wl0_radius_key", nvram_safe_get("wl_radius_key"));
	nvram_set("wl0_wpa_gtk_rekey", nvram_safe_get("wl_wpa_gtk_rekey"));


	if (nvram_invmatch("wl_mode_ex", "ap"))
	{
#if 0
		int wepidx=atoi(nvram_safe_get("wl0_key"));
		char wepname[16];

		sprintf(sbuf, "wl join \"%s\"", nvram_safe_get("wl0_ssid"));

		// key ??
		if (nvram_match("wl0_auth_mode", "psk"))
		{
			sprintf(sbuf, "%s key %s", sbuf, nvram_safe_get("wl0_wpa_psk"));
		}
		else if (nvram_match("wl0_wep", "enabled") && wepidx>=1 && wepidx<=4)
		{
			sprintf(wepname, "wl0_key%d", wepidx);
			sprintf(sbuf, "%s key %s", sbuf, nvram_safe_get(wepname));
		}
		
		sprintf(sbuf, "%s imode bss", sbuf);
		
		if (nvram_match("wl_auth_mode", "psk"))
			sprintf(sbuf, "%s amode wpapsk", sbuf);
		else if (nvram_match("wl_auth_mode", "shared"))
			sprintf(sbuf, "%s amode shared", sbuf);
		else sprintf(sbuf, "%s amode open", sbuf);

		nvram_set("wl0_join", sbuf);
		/* Station or Ethernet Bridge Mode */
		nvram_set("wl0_mode", nvram_safe_get("wl_mode_ex"));
#endif
	}
	else
	{
		/* WDS control */
		if ( nvram_match("wl_mode_x", "1") 
#ifdef PBCENR
		     && nvram_invmatch("sw_mode_ex", "2")
#endif
		)
		{
			nvram_set("wl_mode", "wds");
			nvram_set("wl0_mode", "wds");
		}
		else
		{
			nvram_set("wl_mode", "ap");
			nvram_set("wl0_mode", "ap");
		}
#ifdef PBCENR
		if (nvram_invmatch("sw_mode_ex", "2"))
#endif			
			nvram_set("wl0_lazywds", nvram_safe_get("wl_lazywds"));
#ifdef PBCENR
		else
			nvram_set("wl0_lazywds", "0");
#endif
	}

	if (nvram_match("wl_wdsapply_x", "1") && nvram_invmatch("sw_mode_ex", "2"))
	{
		num = atoi(nvram_safe_get("wl_wdsnum_x"));
		list[0]=0;


		for(i=0;i<num;i++)
		{
			sprintf(list, "%s %s", list, mac_conv("wl_wdslist_x", i, macbuf));
		}

		nvram_set("wl_wds", list);	// 2008.06 James.
		nvram_set("wl0_wds", list);
	}
	else
	{
		nvram_set("wl_wds", "");	// 2008.06 James.
		nvram_set("wl0_wds", "");
	}

	/* Mac filter */
	nvram_set("wl0_macmode", nvram_safe_get("wl_macmode"));

	if (nvram_invmatch("wl_macmode", "disabled"))
	{
		num = atoi(nvram_safe_get("wl_macnum_x"));
		list[0]=0;

		for(i=0;i<num;i++)
		{
			sprintf(list, "%s %s", list, mac_conv("wl_maclist_x", i, macbuf));
		}

		nvram_set("wl_maclist", list);
		nvram_set("wl0_maclist", list);
	}


#ifdef GUEST_ACCOUNT
	if(!skipflag)
	{
	if (nvram_match("wl_guest_enable", "1"))
	{
		nvram_set("wl0.1_guest","1");
		nvram_set("wl0.1_ifname", "wl0.1");
		nvram_set("wl0.1_mode", "ap");
		nvram_set("wl0.1_radio", "1");
		nvram_set("wl0.1_ipconfig_index","1");
		//nvram_set("unbridged_ifnames", "wl0.1");
		nvram_set("wl0_vifs", "wl0.1");
		nvram_set("wl0.1_ssid", nvram_safe_get("wl_guest_ssid_1"));
		nvram_set("wl0.1_crypto", nvram_safe_get("wl_guest_crypto_1"));
		nvram_set("wl0.1_wpa_psk", nvram_safe_get("wl_guest_wpa_psk_1"));
		nvram_set("wl0.1_key", nvram_safe_get("wl_guest_key_1"));
		nvram_set("wl0.1_key1", nvram_safe_get("wl_guest_key1_1"));
		nvram_set("wl0.1_key2", nvram_safe_get("wl_guest_key2_1"));
		nvram_set("wl0.1_key3", nvram_safe_get("wl_guest_key3_1"));
		nvram_set("wl0.1_key4", nvram_safe_get("wl_guest_key4_1"));

		if (nvram_match("wl_guest_wep_x_1", "0"))
			nvram_set("wl0.1_wep", "disabled");
		else nvram_set("wl0.1_wep", "enabled");

		if (nvram_match("wl_guest_auth_mode_1", "shared"))
			nvram_set("wl0.1_auth", "1");
		else nvram_set("wl0.1_auth", "0");

		if (nvram_match("wl_guest_auth_mode_1", "wpa"))
		{
			nvram_set("wl0.1_akm", "wpa wpa2");
		}
		else if (nvram_match("wl_guest_auth_mode_1", "psk"))
		{
			nvram_set("wl0.1_akm", "psk psk2");
		}
		else
		{
			nvram_set("wl0.1_akm", "");
		}

		nvram_set("wl0.1_auth_mode", nvram_match("wl_guest_auth_mode_1", "radius") ? "radius" : "none");
		nvram_set("wl0.1_gmode", nvram_safe_get("wl0_gmode"));
		nvram_set("wl0.1_gmode_protection", nvram_safe_get("wl0_gmode_protection"));
		nvram_set("wl0.1_rate", nvram_safe_get("wl_rate"));
		nvram_set("wl0.1_rateset", nvram_safe_get("wl_rateset"));
		nvram_set("wl0.1_frag", nvram_safe_get("wl_frag"));
		nvram_set("wl0.1_rts", nvram_safe_get("wl_rts"));
		nvram_set("wl0.1_dtim", nvram_safe_get("wl_dtim"));
		nvram_set("wl0.1_bcn", nvram_safe_get("wl_bcn"));
		nvram_set("wl0.1_plcphdr", nvram_safe_get("wl_plcphdr"));
		nvram_set("wl0.1_closed", nvram_safe_get("wl_closed"));
		nvram_set("wl0.1_frameburst", nvram_safe_get("wl_frameburst"));
		nvram_set("wl0.1_afterburner", nvram_safe_get("wl_afterburner"));
		nvram_set("wl0.1_ap_isolate", nvram_safe_get("wl_ap_isolate"));
		nvram_set("wl0.1_radio", nvram_safe_get("wl_radio_x"));
		nvram_set("wl0.1_preauth", nvram_safe_get("wl_preauth"));
		nvram_set("wl0.1_net_reauth", nvram_safe_get("wl_net_reauth"));
		nvram_set("wl0.1_wme", nvram_safe_get("wl_wme"));
		nvram_set("wl0.1_wme_no_ack", nvram_safe_get("wl_wme_no_ack"));
		nvram_set("wl0.1_wme_sta_bk", nvram_safe_get("wl_wme_sta_bk"));
		nvram_set("wl0.1_wme_sta_be", nvram_safe_get("wl_wme_sta_be"));
		nvram_set("wl0.1_wme_sta_vi", nvram_safe_get("wl_wme_sta_vi"));
		nvram_set("wl0.1_wme_sta_vo", nvram_safe_get("wl_wme_sta_vo"));
		nvram_set("wl0.1_wme_ap_bk", nvram_safe_get("wl_wme_ap_bk"));
		nvram_set("wl0.1_wme_ap_be", nvram_safe_get("wl_wme_ap_be"));
		nvram_set("wl0.1_wme_ap_vi", nvram_safe_get("wl_wme_ap_vi"));
		nvram_set("wl0.1_wme_ap_vo", nvram_safe_get("wl_wme_ap_vo"));

		nvram_set("lan1_ifname", "wl0.1");

		if (nvram_match("dhcp1_enable_x", "1"))
		{
			nvram_set("lan1_proto", "dhcp");
		}
		else
		{
			nvram_set("lan1_proto", "static");
		}
	}
	else
	{
		nvram_unset("wl0.1_ifname");
		nvram_unset("wl0.1_guest");
		nvram_unset("wl0_vifs");
		nvram_unset("lan1_ifname");
	}
}
#endif

	/* Direct copy value */
	/* LAN Section */
	if(nvram_match("dhcp_enable_x", "1"))
		nvram_set("lan_proto", "dhcp");
	else
		nvram_set("lan_proto", "static");
	
	nvram_set("wan0_proto", nvram_safe_get("wan_proto"));
	if(nvram_match("x_DHCPClient", "0")){	// 2008.05 James.
		nvram_set("wan0_ipaddr", nvram_safe_get("wan_ipaddr"));
		nvram_set("wan0_netmask", nvram_safe_get("wan_netmask"));
		nvram_set("wan0_gateway", nvram_safe_get("wan_gateway"));
	}	// 2008.05 James.
// 2008.09 James. {
	else if(nvram_match("wan_proto", "pppoe")
			|| nvram_match("wan_proto", "pptp")
			|| nvram_match("wan_proto", "l2tp")){
		nvram_set("wan_ipaddr", "0.0.0.0");
		nvram_set("wan_netmask", "0.0.0.0");
		nvram_set("wan_gateway", "0.0.0.0");
		
		if(nvram_match("wan_dnsenable_x", "1")){
			nvram_set("wan_dns1_x", "0.0.0.0");
			nvram_set("wan_dns2_x", "0.0.0.0");
		}
	}
// 2008.09 James. }
	
// 2007.10 James. for the procedure of no-reboot rc, and start_wan() will do this. {
	if(!skipflag){
		nvram_unset("ntp_ready");	// for notifying detectWAN.
		nvram_unset("wan_ready");	// for notifying wanduck.
		nvram_unset("manually_disconnect_wan");	// 2008.07 James.
		nvram_set("wan_ipaddr_t", "");
		nvram_set("wan_netmask_t", "");
		nvram_set("wan_gateway_t", "");
		nvram_set("wan_dns_t", "");
		nvram_set("wan_status_t", "Disconnected");
	}
// 2007.10 James }
	
	wan_netmask_check();
	
	if(nvram_match("wan_proto", "pppoe")
			|| nvram_match("wan_proto", "pptp")
			|| nvram_match("wan_proto", "l2tp")
			){
		nvram_set("wan0_pppoe_ifname", "ppp0");
		nvram_set("upnp_wan_proto", "pppoe");
		nvram_set("wan0_pppoe_username", nvram_safe_get("wan_pppoe_username"));
		nvram_set("wan0_pppoe_passwd", nvram_safe_get("wan_pppoe_passwd"));
		nvram_set("wan0_pppoe_idletime", nvram_safe_get("wan_pppoe_idletime"));
		nvram_set("wan0_pppoe_txonly_x", nvram_safe_get("wan_pppoe_txonly_x"));
		nvram_set("wan0_pppoe_mtu", nvram_safe_get("wan_pppoe_mtu"));
		nvram_set("wan0_pppoe_mru", nvram_safe_get("wan_pppoe_mru"));
		nvram_set("wan0_pppoe_service", nvram_safe_get("wan_pppoe_service"));
		nvram_set("wan0_pppoe_ac", nvram_safe_get("wan_pppoe_ac"));
		nvram_set("wan0_pppoe_options_x", nvram_safe_get("wan_pppoe_options_x"));
		nvram_set("wan0_pptp_options_x", nvram_safe_get("wan_pptp_options_x"));
#ifdef REMOVE
		nvram_set("wan0_pppoe_demand", "1");
		nvram_set("wan0_pppoe_keepalive", "1");
#endif	// REMOVE
		nvram_set("wan0_pppoe_ipaddr", nvram_safe_get("wan_ipaddr"));
		nvram_set("wan0_pppoe_netmask",
				inet_addr_(nvram_safe_get("wan_ipaddr")) &&
				inet_addr_(nvram_safe_get("wan_netmask"))?nvram_get("wan_netmask"):NULL);
		nvram_set("wan0_pppoe_gateway", nvram_get("wan_gateway"));
		
		/* current interface address (dhcp + firewall) */
		nvram_set("wanx_ipaddr", nvram_safe_get("wan_ipaddr"));
	}
// 2007.12 James. {
	else
		nvram_unset("upnp_wan_proto");
// 2007.12 James. }
	
	nvram_set("wan0_hostname", nvram_safe_get("wan_hostname"));

	if (nvram_invmatch("wan_hwaddr_x", ""))
	{
		nvram_set("wan_hwaddr", mac_conv("wan_hwaddr_x", -1, macbuf));
		nvram_set("wan0_hwaddr", mac_conv("wan_hwaddr_x", -1, macbuf));
	}

	nvram_set("wan0_dnsenable_x", nvram_safe_get("wan_dnsenable_x"));
	if(!skipflag){
		nvram_unset("wan0_dns");
		nvram_unset("wanx_dns");
	}

	convert_routes();

	memset(servers, 0, sizeof(servers));

	if (nvram_invmatch("ntp_server0", ""))
		sprintf(servers, "%s%s ", servers, nvram_safe_get("ntp_server0"));
	if (nvram_invmatch("ntp_server1", ""))
		sprintf(servers, "%s%s ", servers, nvram_safe_get("ntp_server1"));

	nvram_set("ntp_servers", servers);

#ifdef PBCENR
	/* Start of Multiple SSID & URE configuration */
	char tmp1[100], prefix1[] = "wlXXXXXXXXXX_mssid_";
	char tmp2[100], prefix2[] = "wlXXXXXXXXXX_mssid_";

	if (nvram_match("sw_mode_ex", "2"))
		num = 2;
	else
#ifdef RTN10
		num = 2;
#else
		num = 4;
#endif		

	for (i = 1; i < 4; i++)				// Jiahao for RT-N12
	{
		sprintf(prefix1, "wl0.%d_", i);

		nvram_unset(strcat_r(prefix1, "bss_enabled", tmp1));
		nvram_unset(strcat_r(prefix1, "ap_isolate", tmp1));
		nvram_unset(strcat_r(prefix1, "auth", tmp1));
		nvram_unset(strcat_r(prefix1, "akm", tmp1));
		nvram_unset(strcat_r(prefix1, "auth_mode", tmp1));
		nvram_unset(strcat_r(prefix1, "bridge", tmp1));
		nvram_unset(strcat_r(prefix1, "bss_maxassoc", tmp1));
		nvram_unset(strcat_r(prefix1, "closed", tmp1));
		nvram_unset(strcat_r(prefix1, "crypto", tmp1));
		nvram_unset(strcat_r(prefix1, "ifname", tmp1));
		nvram_unset(strcat_r(prefix1, "infra", tmp1));
		nvram_unset(strcat_r(prefix1, "key1", tmp1));
		nvram_unset(strcat_r(prefix1, "key2", tmp1));
		nvram_unset(strcat_r(prefix1, "key3", tmp1));
		nvram_unset(strcat_r(prefix1, "key4", tmp1));
		nvram_unset(strcat_r(prefix1, "key", tmp1));
		nvram_unset(strcat_r(prefix1, "maclist", tmp1));
		nvram_unset(strcat_r(prefix1, "macmode", tmp1));
		nvram_unset(strcat_r(prefix1, "mode", tmp1));
		nvram_unset(strcat_r(prefix1, "net_reauth", tmp1));
		nvram_unset(strcat_r(prefix1, "preauth", tmp1));
		nvram_unset(strcat_r(prefix1, "radio", tmp1));
		nvram_unset(strcat_r(prefix1, "radius_ipaddr", tmp1));
		nvram_unset(strcat_r(prefix1, "radius_key", tmp1));
		nvram_unset(strcat_r(prefix1, "radius_port", tmp1));
		nvram_unset(strcat_r(prefix1, "ssid", tmp1));
		nvram_unset(strcat_r(prefix1, "ssid2", tmp1));
		nvram_unset(strcat_r(prefix1, "sta_retry_time", tmp1));
		nvram_unset(strcat_r(prefix1, "unit", tmp1));
		nvram_unset(strcat_r(prefix1, "wep", tmp1));
		nvram_unset(strcat_r(prefix1, "wpa_gtk_rekey", tmp1));
		nvram_unset(strcat_r(prefix1, "wpa_psk", tmp1));
		nvram_unset(strcat_r(prefix1, "wps_config_state", tmp1));
		nvram_unset(strcat_r(prefix1, "wps_mode", tmp1));
		nvram_unset(strcat_r(prefix1, "wps_reg", tmp1));
		nvram_unset(strcat_r(prefix1, "wme", tmp1));
		nvram_unset(strcat_r(prefix1, "wme_bss_disable", tmp1));
		nvram_unset(strcat_r(prefix1, "wmf_bss_enable", tmp1));
	}

	for (i = 1; i < num; i++)			// Jiahao for RT-N12
	{
		sprintf(prefix1, "wl0.%d_", i);
		sprintf(prefix2, "mbss%d_", i);

		if (nvram_invmatch(strcat_r(prefix2, "enabled", tmp2), "1") && nvram_invmatch("sw_mode_ex", "2"))
			continue;

		nvram_set(strcat_r(prefix1, "bss_enabled", tmp1), 	"0");
		nvram_set(strcat_r(prefix1, "ap_isolate", tmp1), 	"0");

		if (nvram_match(strcat_r(prefix2, "auth_mode", tmp2), "shared"))
			nvram_set(strcat_r(prefix1, "auth", tmp1), "1");
		else
			nvram_set(strcat_r(prefix1, "auth", tmp1), "0");

		if (nvram_match(strcat_r(prefix2, "auth_mode", tmp2), "psk"))
		{
			if (nvram_match(strcat_r(prefix2, "wpa_mode", tmp2), "1"))
				nvram_set(strcat_r(prefix1, "akm", tmp1), "psk");
			else if (nvram_match(strcat_r(prefix2, "wpa_mode", tmp2), "2"))
				nvram_set(strcat_r(prefix1, "akm", tmp1), "psk2");
			else
				nvram_set(strcat_r(prefix1, "akm", tmp1), "psk psk2");
		}
		else
			nvram_set(strcat_r(prefix1, "akm", tmp1), "");

		nvram_set(strcat_r(prefix1, "auth_mode", tmp1), 	"none");
		nvram_set(strcat_r(prefix1, "bridge", tmp1), 		"");
		nvram_set(strcat_r(prefix1, "bss_maxassoc", tmp1), 	"128");
		nvram_set(strcat_r(prefix1, "closed", tmp1), 		nvram_safe_get(strcat_r(prefix2, "closed", tmp2)));
		nvram_set(strcat_r(prefix1, "crypto", tmp1), 		nvram_safe_get(strcat_r(prefix2, "crypto", tmp2)));
		sprintf(tmp2, "wl0.%d", i);
		nvram_set(strcat_r(prefix1, "ifname", tmp1), 		tmp2);
		nvram_set(strcat_r(prefix1, "infra", tmp1), 		"1");
		nvram_set(strcat_r(prefix1, "key1", tmp1), 		nvram_safe_get(strcat_r(prefix2, "key1", tmp2)));
		nvram_set(strcat_r(prefix1, "key2", tmp1), 		nvram_safe_get(strcat_r(prefix2, "key2", tmp2)));
		nvram_set(strcat_r(prefix1, "key3", tmp1), 		nvram_safe_get(strcat_r(prefix2, "key3", tmp2)));
		nvram_set(strcat_r(prefix1, "key4", tmp1), 		nvram_safe_get(strcat_r(prefix2, "key4", tmp2)));
		nvram_set(strcat_r(prefix1, "key", tmp1), 		nvram_safe_get(strcat_r(prefix2, "key", tmp2)));
		nvram_set(strcat_r(prefix1, "maclist", tmp1), 		"");
		nvram_set(strcat_r(prefix1, "macmode", tmp1), 		"disabled");
		nvram_set(strcat_r(prefix1, "mode", tmp1), 		"ap");
		nvram_set(strcat_r(prefix1, "net_reauth", tmp1), 	"36000");
		nvram_set(strcat_r(prefix1, "preauth", tmp1), 		"");
		nvram_set(strcat_r(prefix1, "radio", tmp1), 		"1");
		nvram_set(strcat_r(prefix1, "radius_ipaddr", tmp1), 	"");
		nvram_set(strcat_r(prefix1, "radius_key", tmp1), 	"");
		nvram_set(strcat_r(prefix1, "radius_port", tmp1), 	"1812");
		nvram_set(strcat_r(prefix1, "ssid", tmp1), 		nvram_safe_get(strcat_r(prefix2, "ssid", tmp2)));
		nvram_set(strcat_r(prefix1, "ssid2", tmp1), 		nvram_safe_get(strcat_r(prefix2, "ssid2", tmp2)));
		nvram_set(strcat_r(prefix1, "sta_retry_time", tmp1), 	"5");
		sprintf(tmp2, "0.%d", i);
		nvram_set(strcat_r(prefix1, "unit", tmp1), 		tmp2);

		if (nvram_match(strcat_r(prefix2, "wep_x", tmp2), "0") || nvram_match(strcat_r(prefix2, "auth_mode", tmp2), "psk"))
			nvram_set(strcat_r(prefix1, "wep", tmp1), "disabled");
		else
			nvram_set(strcat_r(prefix1, "wep", tmp1), "enabled");

		nvram_set(strcat_r(prefix1, "wpa_gtk_rekey", tmp1), 	nvram_safe_get(strcat_r(prefix2, "wpa_gtk_rekey", tmp2)));
		nvram_set(strcat_r(prefix1, "wpa_psk", tmp1), 		nvram_safe_get(strcat_r(prefix2, "wpa_psk", tmp2)));
		nvram_set(strcat_r(prefix1, "wps_config_state", tmp1), 	"0");
		nvram_set(strcat_r(prefix1, "wps_mode", tmp1), 		"disabled");
		nvram_set(strcat_r(prefix1, "wps_reg", tmp1), 		"disabled");
	}

	if (nvram_invmatch("sw_mode_ex", "1"))
	{
		nvram_set("wps_enr_mode", "disabled");
		nvram_set("wl0_wps_mode", "disabled");
		nvram_set("wl0_wps_reg", "disabled");
	}

	if (nvram_match("sw_mode_ex", "2"))		// Jiahao for RT-N12
	{	
		if (nvram_invmatch("sta_ssid", ""))
		{
			nvram_set("lan_ifnames", "vlan0 eth1 wl0.1");
			nvram_set("wl0_vifs", "wl0.1");

			nvram_set("wl0.1_hwaddr", nvram_safe_get("et0macaddr"));
			nvram_set("wl0_nbw_cap", "2");

			nvram_set("wl_mode", "wet");
			nvram_set("wl0_mode", "wet");
			nvram_set("wl_ure", "1");
			nvram_set("wl0_ure", "1");
			nvram_set("ure_disable", "0");

			nvram_set("wl0.1_bss_enabled", "1");

			nvram_set("wl0.1_auth_mode", "none");
			nvram_set("wl0.1_wme", nvram_safe_get("wl_wme"));
			nvram_set("wl0.1_wme_bss_disable", nvram_safe_get("wl_wme_bss_disable"));
			nvram_set("wl0.1_wmf_bss_enable", nvram_safe_get("wl_wmf_bss_enable"));
		}
		else
		{
			nvram_set("lan_ifnames", "vlan0 eth1");
			nvram_unset("wl0_vifs");

			nvram_set("wl_mode", "ap");
			nvram_set("wl0_mode", "ap");
			nvram_set("wl_ure", "0");
			nvram_set("wl0_ure", "0");
			nvram_set("ure_disable", "1");

			nvram_set("wl0.1_bss_enabled", "0");
			nvram_set("wl0.2_bss_enabled", "0");
			nvram_set("wl0.3_bss_enabled", "0");
		}
		
		nvram_set("wl0_auth_mode", "none");

		if (nvram_invmatch("sta_ssid", ""))
		{
			if (nvram_match("sta_auth_mode", "shared"))
			{
				nvram_set("wl0_auth", "1");
				nvram_set("wl0.1_auth", "1");
			}
			else
			{
				nvram_set("wl0_auth", "0");
				nvram_set("wl0.1_auth", "0");
			}
			if (nvram_match("sta_auth_mode", "psk"))
			{
				if (nvram_match("sta_wpa_mode", "1"))
				{
					nvram_set("wl0_akm", "psk");
					nvram_set("wl0.1_akm", "psk");
				}
				else
				{
					nvram_set("wl0_akm", "psk2");
					nvram_set("wl0.1_akm", "psk2");
				}
			}
			else
			{
				nvram_set("wl0_akm", "");
				nvram_set("wl0.1_akm", "");
			}
			if (nvram_match("sta_wep_x", "0") || nvram_match("sta_auth_mode", "psk"))
			{
				nvram_set("wl0_wep", "disabled");
				nvram_set("wl0.1_wep", "disabled");
			}
			else
			{
				nvram_set("wl0_wep", "enabled");
				nvram_set("wl0.1_wep", "enabled");
			}

			nvram_set("wl0_ssid", nvram_safe_get("sta_ssid"));
			nvram_set("wl0_crypto", nvram_safe_get("sta_crypto"));
			nvram_set("wl0_wpa_psk", nvram_safe_get("sta_wpa_psk"));
			nvram_set("wl0_key1", nvram_safe_get("sta_key1"));
			nvram_set("wl0_key2", nvram_safe_get("sta_key2"));
			nvram_set("wl0_key3", nvram_safe_get("sta_key3"));
			nvram_set("wl0_key4", nvram_safe_get("sta_key4"));
			nvram_set("wl0_key", nvram_safe_get("sta_key"));
			nvram_set("wl0.1_ssid", nvram_safe_get("sta_ssid"));
			nvram_set("wl0.1_crypto", nvram_safe_get("sta_crypto"));
			nvram_set("wl0.1_wpa_psk", nvram_safe_get("sta_wpa_psk"));
			nvram_set("wl0.1_key1", nvram_safe_get("sta_key1"));
			nvram_set("wl0.1_key2", nvram_safe_get("sta_key2"));
			nvram_set("wl0.1_key3", nvram_safe_get("sta_key3"));
			nvram_set("wl0.1_key4", nvram_safe_get("sta_key4"));
			nvram_set("wl0.1_key", nvram_safe_get("sta_key"));
		}
		else
		{
			nvram_set("wl0_wep", "disabled");
			nvram_set("wl0_auth", "0");
			nvram_set("wl0_akm", "");

			nvram_set("wl0_ssid", "ure");
			nvram_set("wl0_crypto", "tkip");
			nvram_set("wl0_wpa_psk", "12345678");
			nvram_set("wl0_key1", "");
			nvram_set("wl0_key2", "");
			nvram_set("wl0_key3", "");
			nvram_set("wl0_key4", "");
			nvram_set("wl0_key", "1");
			
			nvram_set("wl0_closed", "1");
			nvram_set("wl0_macmode", "2");
			nvram_set("wl0_maclist", "");
		}
	}
	else
	{
		nvram_set("lan_ifnames", "vlan0 eth1");
		nvram_unset("wl0_vifs");
//		nvram_set("wl_mode", "ap");
//		nvram_set("wl0_mode", "ap");
		nvram_set("wl_ure", "0");
		nvram_set("wl0_ure", "0");
		nvram_set("ure_disable", "1");

		unsigned char *macaddr_str = nvram_get("et0macaddr");
		unsigned char mac_binary[6];
		unsigned long long macvalue;
		unsigned char *macp;
		char lan_ifnames[32];
		char wl_vifnames[32];
		unsigned char mbss_nolan_needed = 0;
		unsigned char mbss_nolan = nvram_match("wl_ap_isolate", "1") ? 0x1 : 0x0;
		unsigned char mbss_nowan_needed = 0;
		unsigned char mbss_nowan = 0;
		unsigned char mbss_prio_needed = 0;
		unsigned char mbss_prio = 0;
		unsigned char mbss_prio_low = 0;

		memset(mac_binary, 0x0, 6);
		memset(macbuf, 0x0, 13);
		memset(lan_ifnames, 0x0, 32);
		memset(wl_vifnames, 0x0, 32);

		if (macaddr_str)
		{
			ether_atoe(macaddr_str, mac_binary);
			sprintf(macbuf, "%02X%02X%02X%02X%02X%02X",
					mac_binary[0],
					mac_binary[1],
					mac_binary[2],
					mac_binary[3],
					mac_binary[4],
					mac_binary[5]);
			macvalue = strtoll(macbuf, (char **) NULL, 16);

			for (i = 1; i < num; i++)
			{
				macvalue++;

				sprintf(tmpstr, "mbss%d_enabled", i);
				if (nvram_match(tmpstr, "1"))
				{
					macp = &macvalue;
					sprintf(macbuf, "%02X:%02X:%02X:%02X:%02X:%02X\n", *(macp+5), *(macp+4), *(macp+3), *(macp+2), *(macp+1), *(macp+0));
					sprintf(tmpstr, "wl0.%d_hwaddr", i);
					nvram_set(tmpstr, macbuf);

					sprintf(wl_vifnames, "%s wl0.%d", wl_vifnames, i);
					sprintf(tmpstr, "wl0.%d_bss_enabled", i);
					nvram_set(tmpstr, "1");

					sprintf(tmpstr, "mbss%d_nolan", i);
					if (nvram_match(tmpstr, "1"))
					{
						sprintf(tmpstr, "wl0.%d_ap_isolate", i);
						nvram_set(tmpstr, "1");

						mbss_nolan_needed = 1;
						mbss_nolan |= 0x1 << i;
					}

					sprintf(tmpstr, "mbss%d_nowan", i);
					if (nvram_match(tmpstr, "1"))
					{
						mbss_nowan_needed = 1;
						mbss_nowan |= 0x1 << i;
					}

					mbss_prio_needed = 1;
					sprintf(tmpstr, "mbss%d_priority", i);
					if (nvram_match(tmpstr, "0"))
					{
						mbss_prio_low = 1;	// an enabled Multi-SSID is assigned low priority
						mbss_prio |= 0x1 << i * 2 ;
					}
					else				// assume "1"
						mbss_prio |= 0x1 << i * 2 + 1;
				}
				else
					continue;

				sprintf(tmpstr, "wl0.%d_wme", i);
				nvram_set(tmpstr, nvram_safe_get("wl_wme"));
				sprintf(tmpstr, "wl0.%d_wme_bss_disable", i);
				nvram_set(tmpstr, nvram_safe_get("wl_wme_bss_disable"));
				sprintf(tmpstr, "wl0.%d_wmf_bss_enable", i);
				nvram_set(tmpstr, nvram_safe_get("wl_wmf_bss_enable"));
			}

			if (strlen(wl_vifnames))
			{
				nvram_set("wl0_vifs", wl_vifnames);
				sprintf(lan_ifnames, "vlan0 eth1 %s", wl_vifnames);
				nvram_set("lan_ifnames", lan_ifnames);
			}
		}

		if (mbss_nolan_needed)
			syscall(__NR_set_mbss_nolan, mbss_nolan);		// inform kernel of mbss nolan setting

		if (nvram_match("sw_mode_ex", "1") && mbss_nowan_needed)
			syscall(__NR_set_mbss_nowan, mbss_nowan);		// inform kernel of mbss nowan setting

		if (nvram_match("sw_mode_ex", "1") && mbss_prio_needed)
			syscall(__NR_set_mbss_prio, mbss_prio);			// inform kernel of mbss priority setting

		if (nvram_match("sw_mode_ex", "1") && mbss_prio_low)
			nvram_set("qos_wireless_enable", "1");
	}
	/* End of Multiple SSID & URE configuration */
#endif

	if (nvram_match("wan_nat_x", "0") && nvram_match("wan_route_x", "IP_Bridged"))
	{
		sprintf(ifnames, "%s", nvram_safe_get("lan_ifnames"));
#ifndef WL500GP
		sprintf(ifnames, "%s %s", ifnames, nvram_safe_get("wan_ifnames"));
#endif	// WL500GP
		nvram_set("lan_ifnames_t", ifnames);
		nvram_set("br0_ifnames", ifnames);	// 2008.06 James.
		nvram_set("router_disable", "1");
		nvram_set("vlan_enable", "0");
	}
	else 
	{
// 2008.06 James. {
		//nvram_set("lan_ifnames_t", nvram_safe_get("lan_ifnames"));
		memset(ifnames, 0, sizeof(ifnames));
		strcpy(ifnames, nvram_safe_get("lan_ifnames"));
		nvram_set("lan_ifnames_t", ifnames);
		nvram_set("br0_ifnames", ifnames);
// 2008.06 James. }
		nvram_set("router_disable", "0");
		nvram_set("vlan_enable", "1");
	}
		eval("start_ddns");


#ifdef USB_SUPPORT
	// clean some temp variables
	if (!skipflag)
	{
		nvram_set("swap_on", "0");
		//nvram_set("usb_device", "");
//	nvram_set("usb_ftp_device", ""); 	// marked by Jiahao for WL500gP
		nvram_set("usb_path1", "");
		nvram_set("usb_path2", "");
		nvram_set("usb_storage_device", "");
		nvram_set("usb_web_device", "");	// following lines are added by Jiahao for WL500gP
		nvram_set("usb_audio_device", "");
		nvram_set("usb_printer_device", "");	// 2008.05 James.
		nvram_set("usb_path", "");
		nvram_set("usb_path_audio", "");
		nvram_set("usb_path_printer", "");
		nvram_set("usb_path_webcam", "");
		nvram_set("usb_webdriver_x", "");
		nvram_set("usb_web_flag", "");
		nvram_set("usb_disc0_path0", "");
		nvram_set("usb_disc0_path1", "");
		nvram_set("usb_disc0_path2", "");
		nvram_set("usb_disc0_path3", "");
		nvram_set("usb_disc0_path4", "");
		nvram_set("usb_disc0_path5", "");
		nvram_set("usb_disc0_path6", "");
		nvram_set("usb_disc1_path0", "");
		nvram_set("usb_disc1_path1", "");
		nvram_set("usb_disc1_path2", "");
		nvram_set("usb_disc1_path3", "");
		nvram_set("usb_disc1_path4", "");
		nvram_set("usb_disc1_path5", "");
		nvram_set("usb_disc1_path6", "");
		nvram_set("usb_disc0_fs_path0", "");
		nvram_set("usb_disc0_fs_path1", "");
		nvram_set("usb_disc0_fs_path2", "");
		nvram_set("usb_disc0_fs_path3", "");
		nvram_set("usb_disc0_fs_path4", "");
		nvram_set("usb_disc0_fs_path5", "");
		nvram_set("usb_disc0_fs_path6", "");
		nvram_set("usb_disc1_fs_path0", "");
		nvram_set("usb_disc1_fs_path1", "");
		nvram_set("usb_disc1_fs_path2", "");
		nvram_set("usb_disc1_fs_path3", "");
		nvram_set("usb_disc1_fs_path4", "");
		nvram_set("usb_disc1_fs_path5", "");
		nvram_set("usb_disc1_fs_path6", "");
		nvram_set("usb_disc0_index", "0");
		nvram_set("usb_disc1_index", "0");
		nvram_set("usb_disc0_port", "0");
		nvram_set("usb_disc1_port", "0");
		nvram_set("usb_disc0_dev", "");
		nvram_set("usb_disc1_dev", "");
// 2008.06 James. {
		nvram_set("usb_disc0_pid", "");
		nvram_set("usb_disc0_safely_removed", "0");
		nvram_set("usb_disc1_pid", "");
		nvram_set("usb_disc1_safely_removed", "0");
// 2008.06 James. }
#ifdef DLM
// 2007.12 James. {
		nvram_unset("ftp_running");
		nvram_unset("samba_running");
		nvram_unset("dms_running");
		nvram_unset("apps_running");
		nvram_unset("apps_installed");//*/
		nvram_set("eject_from_web", "0");
		//nvram_set("st_ftp_modex", nvram_get("st_ftp_mode"));
		//nvram_set("st_samba_modex", nvram_get("st_samba_mode"));
// 2007.12 James. }
		nvram_set("st_samba_mode_x", "-1");
		nvram_set("apps_dl_x", "-1");
		//nvram_set("apps_dmsx", nvram_get("apps_dms"));  // 2008.07 James.
		nvram_set("apps_dms_usb_port_x", "-1");
		nvram_set("apps_dms_usb_port_x2", "-1");
		nvram_set("apps_status_checked", "1");
		nvram_set("apps_version", "");	// 2008.09 James. for apps_version.
		nvram_set("usb_storage_busy", "0");
		nvram_set("usb_storage_busy2", "0");
		nvram_set("swapoff_failed", "0");
		nvram_set("mnt_type", "");
		nvram_set("dm_block", "0");
// For dynamic machine name Yen
		if (nvram_match("computer_name", ""))
			nvram_set("computer_name", nvram_safe_get("productid"));
		if (nvram_match("computerb_name", ""))
			nvram_set("computerb_name", nvram_safe_get("productid"));
		if (nvram_match("machine_name", ""))
			nvram_set("machine_name", nvram_safe_get("productid"));
#endif	// DLM
#ifdef WCN
		nvram_set("wcn_enable_x", nvram_get("wcn_enable"));
#endif	// WCN
	}
//	nvram_set("apps_dlx", nvram_get("apps_dl"));  // 2008.08 James.
	nvram_set("apps_dlx", "0");  // 2008.08 James.
	nvram_set("apps_dmsx", nvram_get("apps_dms"));  // 2008.07 James.
#endif	// USB_SUPPORT
	nvram_set("no_br", "0");


	if(nvram_invmatch("sp_battle_ips", "0") && !skipflag)
	{
		eval("insmod", "ip_nat_starcraft.o");
		eval("insmod", "ipt_NETMAP.o");
	}

	//2005/09/22 insmod FTP module
	/*if (nvram_match("usb_ftpenable_x", "1") && atoi(nvram_get("usb_ftpport_x"))!=21)
	{
		char ports[32];

		sprintf(ports, "ports=21,%d", atoi(nvram_get("usb_ftpport_x")));
		eval("insmod", "ip_conntrack_ftp.o", ports);	
		eval("insmod", "ip_nat_ftp.o", ports);
	}
	else
	{
		eval("insmod", "ip_conntrack_ftp.o");
		eval("insmod", "ip_nat_ftp.o");
	}//*/

	update_lan_status(1);

#ifdef NOIPTABLES
	if (nvram_match("misc_http_x", "1"))
	{
		if (nvram_invmatch("misc_httpport_x", ""))
			nvram_set("http_wanport", nvram_safe_get("misc_httpport_x"));
		else nvram_set("http_wanport", "8080");
	}
	else nvram_set("http_wanport", "");

	if (nvram_invmatch("fw_enable_x", "0"))
	{
		nvram_set("fw_disable", "0");
	}
	else
	{
		nvram_set("fw_disable", "1");
	}

	num = 0;	
	if (nvram_match("fw_log_x", "accept") ||
		nvram_match("fw_log_x", "both"))
		num |= 2;
		
	if (nvram_match("fw_log_x", "drop") ||
		nvram_match("fw_log_x", "both"))
		num |= 1;
	
	sprintf(tmpstr, "%d", num);
	nvram_set("log_level", tmpstr);	
#endif	// NOIPTABLES

	// pre mapping
	if(nvram_match("time_zone", "KST-9KDT"))
		nvram_set("time_zone", "UCT-9_1");
	else if(nvram_match("time_zone", "RFT-9RFTDST"))
		nvram_set("time_zone", "UCT-9_2");
	
	strcpy(tmpstr, nvram_safe_get("time_zone"));
	/* replace . with : */
	if ((ptr=strchr(tmpstr, '.'))!=NULL) *ptr = ':';
	/* remove *_? */
	if ((ptr=strchr(tmpstr, '_'))!=NULL) *ptr = 0x0;

	// special mapping
	if(nvram_match("time_zone", "JST"))
		nvram_set("time_zone_x", "UCT-9");
	else if(nvram_match("time_zone", "TST-10TDT"))
		nvram_set("time_zone_x", "UCT-10");
	else if(nvram_match("time_zone", "CST-9:30CDT"))
		nvram_set("time_zone_x", "UCT-9:30");
	else nvram_set("time_zone_x", tmpstr);

#ifdef CDMA
	nvram_set("support_cdma", "1");
	nvram_set("cdma_down", "99");
#else	// CDMA
	nvram_unset("support_cdma");
#endif	// CDMA
	nvram_set("reboot", "");
#ifdef WCN
	nvram_set("reboot_WCN", "");
#endif	// WCN
	dprintf("end map\n");
cprintf("*** End convert_asus_values(%d). ***\n", skipflag);
}

/* This function is used to map nvram value from asus to Broadcom */
#if 0
void early_convert_asus_values()
{
	if (nvram_match ("wl_radio_power_x", "12"))
	{
		nvram_set("pa0maxpwr", "79");
		//nvram_set("opo","16");
	}
	else if (nvram_invmatch ("wl_radio_power_x", "12"))
        {
                nvram_set("pa0maxpwr", "78");
                //nvram_set("opo","8");
        }
}	
#endif

char *findpattern(char *target, char *pattern)
{
	char *find;
	int len;

	//printf("find : %s %s\n", target, pattern);

	if ((find=strstr(target, pattern)))
	{
		len = strlen(pattern);
		if (find[len]==';' || find[len]==0)
		{
			return find;
		}
	}
	return NULL;
}

void update_lan_status(int isup)
{
	if (isup)
	{	
		nvram_set("lan_ipaddr_t", nvram_safe_get("lan_ipaddr"));
		nvram_set("lan_netmask_t", nvram_safe_get("lan_netmask"));

		if (nvram_match("wan_route_x", "IP_Routed"))
		{
			if (nvram_match("lan_proto", "dhcp"))
			{
				if (nvram_invmatch("dhcp_gateway_x", ""))
					nvram_set("lan_gateway_t", nvram_safe_get("dhcp_gateway_x"));
				else
					nvram_set("lan_gateway_t", nvram_safe_get("lan_ipaddr"));
			}
			else nvram_set("lan_gateway_t", nvram_safe_get("lan_ipaddr"));
		}
		else nvram_set("lan_gateway_t", nvram_safe_get("lan_gateway"));
	}
	else
	{
		nvram_set("lan_ipaddr_t", "");
		nvram_set("lan_netmask_t", "");
		nvram_set("lan_gateway_t", "");
	}
}


void update_wan_status(int isup)
{
	char *proto;
	char dns_str[64];

	proto = nvram_safe_get("wan_proto");

	if (!strcmp(proto, "static")) nvram_set("wan_proto_t", "Static");
	else if (!strcmp(proto, "dhcp")) nvram_set("wan_proto_t", "Automatic IP");
	else if (!strcmp(proto, "pppoe")) nvram_set("wan_proto_t", "PPPoE");	
	else if (!strcmp(proto, "pptp")) nvram_set("wan_proto_t", "PPTP");
	else if (!strcmp(proto, "l2tp")) nvram_set("wan_proto_t", "L2TP");
#ifdef CDMA
	else if (!strcmp(proto, "cdma")) nvram_set("wan_proto_t", "CDMA");
#endif


	if (!isup)
	{
		nvram_set("wan_ipaddr_t", "");
		nvram_set("wan_netmask_t", "");
		nvram_set("wan_gateway_t", "");
		nvram_set("wan_dns_t", "");
		nvram_set("wan_status_t", "Disconnected");
	}	
	else
	{
		nvram_set("wan_ipaddr_t", nvram_safe_get("wan0_ipaddr"));
		nvram_set("wan_netmask_t", nvram_safe_get("wan0_netmask"));
		nvram_set("wan_gateway_t", nvram_safe_get("wan0_gateway"));

		memset(dns_str, 0, 64);
		if(nvram_invmatch("wan_dnsenable_x", "1"))
		{
			if(nvram_invmatch("wan_dns1_x", ""))
				sprintf(dns_str, "%s", nvram_safe_get("wan_dns1_x"));
			
			if(nvram_invmatch("wan_dns2_x", "")){
				if(strlen(dns_str) > 0)
					sprintf(dns_str, "%s %s", dns_str, nvram_safe_get("wan_dns2_x"));
				else
					sprintf(dns_str, "%s", nvram_safe_get("wan_dns2_x"));
			}
			
			nvram_set("wan_dns_t", dns_str);
		}
		else
			nvram_set("wan_dns_t", nvram_safe_get("wan0_dns"));
		
		nvram_set("wan_status_t", "Connected");
	}
}

/*
 * logmessage
 *
 */
void logmessage(char *logheader, char *fmt, ...){
	va_list args;
	char buf[512];
	
	va_start(args, fmt);
	
	vsnprintf(buf, sizeof(buf), fmt, args);
	openlog(logheader, 0, 0);
	syslog(0, buf);
	closelog();
	va_end(args);
}


/*
 * wanmessage
 *
 */
void wanmessage(char *fmt, ...)
{
  va_list args;
  char buf[512];

  va_start(args, fmt);
  vsnprintf(buf, sizeof(buf), fmt, args);
  nvram_set("wan_reason_t", buf);
  va_end(args);
}

/* 
 * pppstatus
 * 
 */
char *pppstatus(char *buf)
{
   FILE *fp;
   char sline[128], *p;

   if ((fp=fopen("/tmp/wanstatus.log", "r")) && fgets(sline, sizeof(sline), fp))
   {
	p = strstr(sline, ",");
	strcpy(buf, p+1);
   }
   else
   {
	strcpy(buf, "unknown reason");
   }	
   return buf;
}


/* 
 * Kills process whose PID is stored in plaintext in pidfile
 * @param	pidfile	PID file, signal
 * @return	0 on success and errno on failure
 */
int
kill_pidfile_s(char *pidfile, int sig)
{
	FILE *fp = fopen(pidfile, "r");
	char buf[256];
	extern int errno;

	if(fp && fgets(buf, sizeof(buf), fp)){
		pid_t pid = strtoul(buf, NULL, 0);
		fclose(fp);
		return kill(pid, sig);
  }
  else
		return errno;
}


