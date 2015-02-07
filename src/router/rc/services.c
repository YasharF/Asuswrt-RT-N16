/*
 * Miscellaneous services
 *
 * Copyright 2004, Broadcom Corporation
 * All Rights Reserved.
 * 
 * THIS SOFTWARE IS OFFERED "AS IS", AND BROADCOM GRANTS NO WARRANTIES OF ANY
 * KIND, EXPRESS OR IMPLIED, BY STATUTE, COMMUNICATION OR OTHERWISE. BROADCOM
 * SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A SPECIFIC PURPOSE OR NONINFRINGEMENT CONCERNING THIS SOFTWARE.
 *
 * $Id: services.c,v 1.7 2008/09/24 07:42:54 james26_jang Exp $
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <errno.h>

#include <bcmnvram.h>
#include <netconf.h>
#include <shutils.h>
#include <rc.h>

#ifndef ASUS_EXT
int
start_dhcpd(void)
{
	FILE *fp;
	char name[100];

	if (nvram_match("router_disable", "1") || nvram_invmatch("lan_proto", "dhcp"))
		return 0;

	dprintf("%s %s %s %s\n",
		nvram_safe_get("lan_ifname"),
		nvram_safe_get("dhcp_start"),
		nvram_safe_get("dhcp_end"),
		nvram_safe_get("lan_lease"));

	/* Touch leases file */
	if (!(fp = fopen("/tmp/udhcpd.leases", "a"))) {
		perror("/tmp/udhcpd.leases");
		return errno;
	}
	fclose(fp);

	/* Write configuration file based on current information */
	if (!(fp = fopen("/tmp/udhcpd.conf", "w"))) {
		perror("/tmp/udhcpd.conf");
		return errno;
	}
	fprintf(fp, "pidfile /var/run/udhcpd.pid\n");
	fprintf(fp, "start %s\n", nvram_safe_get("dhcp_start"));
	fprintf(fp, "end %s\n", nvram_safe_get("dhcp_end"));
	fprintf(fp, "interface %s\n", nvram_safe_get("lan_ifname"));
	fprintf(fp, "remaining yes\n");
	fprintf(fp, "lease_file /tmp/udhcpd.leases\n");
	fprintf(fp, "option subnet %s\n", nvram_safe_get("lan_netmask"));
	fprintf(fp, "option router %s\n", nvram_safe_get("lan_ipaddr"));
	fprintf(fp, "option dns %s\n", nvram_safe_get("lan_ipaddr"));
	fprintf(fp, "option lease %s\n", nvram_safe_get("lan_lease"));
	snprintf(name, sizeof(name), "%s_wins", nvram_safe_get("dhcp_wins"));
	if (nvram_invmatch(name, ""))
		fprintf(fp, "option wins %s\n", nvram_get(name));
	snprintf(name, sizeof(name), "%s_domain", nvram_safe_get("dhcp_domain"));
	if (nvram_invmatch(name, ""))
		fprintf(fp, "option domain %s\n", nvram_get(name));
	fclose(fp);

	eval("udhcpd", "-i", "br0", "/tmp/udhcpd.conf");

	dprintf("done\n");
	return 0;
}

int
stop_dhcpd(void)
{
	char sigusr1[] = "-XX";
	int ret;

/*
* Process udhcpd handles two signals - SIGTERM and SIGUSR1
*
*  - SIGUSR1 saves all leases in /tmp/udhcpd.leases
*  - SIGTERM causes the process to be killed
*
* The SIGUSR1+SIGTERM behavior is what we like so that all current client
* leases will be honorred when the dhcpd restarts and all clients can extend
* their leases and continue their current IP addresses. Otherwise clients
* would get NAK'd when they try to extend/rebind their leases and they 
* would have to release current IP and to request a new one which causes 
* a no-IP gap in between.
*/
	sprintf(sigusr1, "-%d", SIGUSR1);
	eval("killall", sigusr1, "udhcpd");
	ret = eval("killall", "udhcpd");

	dprintf("done\n");
	return ret;
}

int
start_dns(void)
{
	int ret;
	FILE *fp;
	
	if (nvram_match("router_disable", "1"))
		return 0;

	/* Create resolv.conf with empty nameserver list */
	if (!(fp = fopen("/tmp/resolv.conf", "w"))) {
		perror("/tmp/resolv.conf");
		return errno;
	}
	fclose(fp);

	/* launch dns relay */
	ret = eval("dnsmasq", "-h", "-n",
		   "-i", nvram_safe_get("lan_ifname"),
		   "-r", "/tmp/resolv.conf");

	dprintf("done\n");
	return ret;
}	

int
stop_dns(void)
{
	int ret = eval("killall", "dnsmasq");
	
	/* Remove resolv.conf */
	unlink("/tmp/resolv.conf");

	dprintf("done\n");
	return ret;
}	
#endif

int
start_telnetd(void)
{
        int ret;
       
        /* telnet is enabled by default */
        if (nvram_match("telnet_enable", "0"))
                return 0;

        char *argv[] = {"telnetd", NULL};
        pid_t pid;

        ret = _eval(argv, NULL, 0, &pid);

        dprintf("done\n");
        return ret;
}

int
stop_telnetd(void)
{
        int ret = eval("killall", "telnetd");

        dprintf("done\n");
        return ret;
}


int
start_httpd(void)
{
	int ret;

	chdir("/www");
#ifdef ASUS_EXT
	if (nvram_invmatch("router_disable", "1"))
//#ifdef RTN10
//		ret = eval("httpd", "eth0");
//#else
		ret = eval("httpd", nvram_safe_get("wan0_ifname"));
//#endif
	else
#endif
	ret = eval("httpd");

	chdir("/");

	dprintf("done\n");
	return ret;
}

int
stop_httpd(void)
{
	int ret = eval("killall", "httpd");

	dprintf("done\n");
	return ret;
}

int
start_upnp(void)
{
	char *wan_ifname;
	int ret;
	char var[100], prefix[] = "wanXXXXXXXXXX_";

	if (!nvram_invmatch("upnp_enable", "0") || nvram_match("router_disable", "1"))
		return 0;
	
	ret = eval("killall", "-SIGUSR1", "upnp");
	if (ret != 0) {
	    snprintf(prefix, sizeof(prefix), "wan%d_", wan_primary_ifunit());
//	    wan_ifname = nvram_match(strcat_r(prefix, "proto", var), "pppoe") ? 
//					nvram_safe_get(strcat_r(prefix, "pppoe_ifname", var)) :
//					nvram_safe_get(strcat_r(prefix, "ifname", var));
		if (nvram_match(strcat_r(prefix, "proto", var), "pppoe") 
			 || nvram_match(strcat_r(prefix, "proto", var), "pptp")
			 || nvram_match(strcat_r(prefix, "proto", var), "l2tp"))
			wan_ifname = nvram_safe_get(strcat_r(prefix, "pppoe_ifname", var));
		else
			wan_ifname = nvram_safe_get(strcat_r(prefix, "ifname", var));

	    ret = eval("upnp", "-D",
//		       "-L", nvram_safe_get("lan_ifname"),
		       "-W", wan_ifname);

	}
	dprintf("done\n");
	return ret;
}

int
stop_upnp(void)
{
	int ret = 0;

	if(nvram_match("upnp_enable", "0"))
		ret = eval("killall", "upnp");

	dprintf("done\n");
	return ret;
}

#if 0
int
start_nas(char *type)
{
	char cfgfile[64];
	char pidfile[64];
	if (!type || !*type)
		type = "lan";
	snprintf(cfgfile, sizeof(cfgfile), "/tmp/nas.%s.conf", type);
	snprintf(pidfile, sizeof(pidfile), "/tmp/nas.%s.pid", type);
	{
		char *argv[] = {"nas", cfgfile, pidfile, type, NULL};
		pid_t pid;

		_eval(argv, NULL, 0, &pid);
		dprintf("done\n");
	}
	return 0;
}
#endif

int
start_nas(void)
{
	int ret = eval("nas");
                                                                                                  
	return ret;
//        char *nas_argv[] = {"nas", NULL};
//        pid_t pid;

//        _eval(nas_argv, NULL, 0, &pid);
//	return 0;
}

int
stop_nas(void)
{
	int ret = eval("killall", "nas");

	dprintf("done\n");
	return ret;
}

int
start_ntpc(void)
{
#ifdef ASUS_EXT
	char *ntp_argv[] = {"ntp", NULL};
	pid_t pid;

	_eval(ntp_argv, NULL, 0, &pid);
#else
	char *servers = nvram_safe_get("ntp_server");
	char *ntp_argv[] = {"ntpclient", "-h", servers, "-i", "3", "-l", "-s", NULL};
	
	if (strlen(servers)) {
		_eval(ntp_argv, NULL, 0, NULL);
	}
	
	dprintf("done\n");
#endif
	return 0;
}

int
stop_ntpc(void)
{
#ifdef ASUS_EXT
	int ret = eval("killall", "ntp");
#else
	int ret = eval("killall", "ntpclient");
#endif

	dprintf("done\n");
	return ret;
}

/* +++ Cherry Cho added in 2006/12/14. +++ */
int start_lltd(void)
{
	chdir("/usr/sbin");
	eval("lld2d", "br0");
	chdir("/");
	
	return 0;
}

int stop_lltd(void)/* Cherry Cho added in 2006/12/14. */
{
	int ret;
	
	ret = eval("killall", "lld2d");

	return ret;
}

#if 0
int
start_wsc(void)
{
		
	/* Set values of WSC variables */
	
	if(nvram_get("secret_code"))/* Cherry Cho added in 2007/4/13. */
		nvram_set("wsc_device_pin", nvram_safe_get("secret_code"));
                
	if((!nvram_match("wsc_cli","1")) && nvram_match("wsc_mode","enabled"))
	{
		if (nvram_match("wsc_restart", "no")) {
			nvram_set("wsc_restart", "normal");
		}
		else
		{
			char *wsc_argv[] = {"/bin/wsccmd", NULL};
			pid_t pid;
			
			dprintf("Starting wsc\n");
			nvram_set("wsc_restart", "yes");
			_eval(wsc_argv, NULL, 0, &pid);
			sleep(5);
		}
	}
	return 0;
}

int
stop_wsc(void)
{
   	int ret;

	//if((!nvram_match("wsc_cli","1")) && nvram_match("wsc_mode","enabled"))
	if(!nvram_match("wsc_cli","1"))
	{
		if (nvram_match("wsc_restart", "no")) {
			return 0;
		}
		else
		{
			nvram_set("wsc_restart", "yes");
		   	ret = eval("killall","wsccmd");
		}
	}
   	return ret;
}
#endif
/* --- Cherry Cho added in 2006/12/14. --- */

int
start_wps(void)
{
        char *wps_argv[] = {"/bin/wps_monitor", NULL};
        pid_t pid;
                                     
        nvram_set("wps_status", "0");
        nvram_set("wps_method", "1");
        nvram_set("wps_config_command", "0");
        nvram_set("wps_proc_mac", "");
                                                                                                  
        if (nvram_match("wps_restart", "1")) {
                nvram_set("wps_restart", "0");
        }
        else {
                nvram_set("wps_restart", "0");
                nvram_set("wps_proc_status", "0");
        }
                                                                                                  
        nvram_set("wps_sta_pin", "00000000");
        nvram_set("wps_currentband", "");

	// Yen, Also set wsc parameter for web interface
	// Discard wsc_
#if 0
        nvram_set("wsc_status", "0");
        nvram_set("wsc_method", "1");
        nvram_set("wsc_config_command", "0");
        nvram_set("wsc_proc_mac", "");
                                                                                                  
        if (nvram_match("wsc_restart", "1")) {
                nvram_set("wsc_restart", "0");
        }
        else {
                nvram_set("wsc_restart", "0");
                nvram_set("wsc_proc_status", "0");
        }
                                                                                                  
        nvram_set("wsc_sta_pin", "00000000");
        nvram_set("wsc_currentband", "");
                                                                                                  
#endif
        eval("killall","wps_monitor");
        eval("killall","wps_ap");
        eval("killall","wps_enr");
        _eval(wps_argv, NULL, 0, &pid);
                                                                                                  
        return 0;
                                                                                                  
}


int
stop_wps(void)
{
        int ret = 0;
                                                                                                  
        ret = eval("killall","wps_monitor");
        ret = eval("killall","wps_ap");
                                                                                                  
        return ret;
                                                                                                  
}

#ifdef __CONFIG_IGMP_PROXY__
void
start_igmp_proxy(void)
{
        /* Start IGMP Proxy in Router mode only */
	char *wan_ifname;
	char var[100], prefix[] = "wanXXXXXXXXXX_";

	snprintf(prefix, sizeof(prefix), "wan%d_", wan_primary_ifunit());

       if (nvram_match(strcat_r(prefix, "proto", var), "pppoe")
                         || nvram_match(strcat_r(prefix, "proto", var), "pptp")
                         || nvram_match(strcat_r(prefix, "proto", var), "l2tp"))
                        wan_ifname = nvram_safe_get(strcat_r(prefix, "pppoe_ifname", var));
                else
                        wan_ifname = nvram_safe_get(strcat_r(prefix, "ifname", var));

        if ((!nvram_match("router_disable", "1")) && nvram_match("mr_enable_x", "1"))
//                eval("igmp", nvram_get("wan_ifname"));
                eval("igmp", wan_ifname);
        return;
}

void
stop_igmp_proxy(void)
{
        eval("killall", "igmp");
        return;
}
#endif /* __CONFIG_IGMP_PROXY__ */


int
start_eapd(void)
{
	stop_eapd();

        int ret = eval("/bin/eapd");
                                                                                                  
        return ret;
}
                                                                                                  
int
stop_eapd(void)
{
        int ret = eval("killall","eapd");
                                                                                                  
        return ret;
}

int
start_services(void)
{
	start_telnetd();
	start_httpd();
	start_dns();
	start_dhcpd();
	
#ifdef ASUS_EXT
	start_logger();
#endif

	start_upnp();
	start_eapd();

	//start_nas("lan");
	start_nas();

#ifdef __CONFIG_IGMP_PROXY__
        start_igmp_proxy();
#endif /* __CONFIG_IGMP_PROXY__ */


#ifdef ASUS_EXT
	start_usb();
#endif

#ifdef GUEST_ACCOUNT
	if (nvram_match("wl_guest_enable", "1"))
	{
		sleep(5);
		start_dhcpd_guest();
		start_guest_nas(); 
	}
#endif

	start_lltd(); /* Cherry Cho added in 2006/12/14. */
	
// 2007.11 James {
#ifdef WEB_REDIRECT
	if(!nvram_match("wanduck_down", "1")
			&& nvram_match("wan_nat_x", "1")){
		printf("--- START: Wait to start wanduck ---\n");
		redirect_setting();
		start_wanduck();
		sleep(1);
	}
#endif
// 2007.11 James }
#ifdef DLM
// 2007.10 James {
	if(!strcmp(nvram_safe_get("restart_usb_apps"), "1")){
		nvram_unset("restart_usb_apps");
		start_usb_apps();
	}
// 2007.10 James }
#endif


	dprintf("done\n");
	
	return 0;
}

int
stop_services(void)
{
// 2007.10 James {
	if(!strcmp(nvram_safe_get("samba_running"), "1") ||
			!strcmp(nvram_safe_get("ftp_running"), "1") ||
			!strcmp(nvram_safe_get("apps_running"), "1")
			){
printf("\n*** ready to restart_usb_apps ***\n");
		nvram_set("restart_usb_apps", "1");
	}
// 2007.10 James }
	
#ifdef ASUS_EXT
	stop_usb();
#endif
	stop_nas();
	stop_upnp();
#ifdef ASUS_EXT
	stop_logger();
#endif

#ifdef GUEST_ACCOUNT
	stop_dhcpd_guest();
#endif
	stop_dhcpd();
	stop_dns();
	stop_httpd();

#ifdef __CONFIG_IGMP_PROXY__
        stop_igmp_proxy();
#endif /* __CONFIG_IGMP_PROXY__ */


	stop_lltd();/* Cherry Cho added in 2006/12/14. */	
	dprintf("done\n");
	return 0;
}

#ifdef GUEST_ACCOUNT
/* Start NAS for the guest interfaces */
int
start_guest_nas(void)
{
	char *unbridged_interfaces;
	char *next;
	char name[IFNAMSIZ],lan[IFNAMSIZ];
	int index;
		
	unbridged_interfaces = nvram_get("unbridged_ifnames");
	
	if (unbridged_interfaces)
		foreach(name,unbridged_interfaces,next){
			index = get_ipconfig_index(name);
			if (index < 0) 
				continue;
			snprintf(lan,sizeof(lan),"lan%d",index);
			//start_nas(lan);
			start_nas();
		}

	return 0;
}
#endif

// 2007.10 James {
#ifdef WEB_REDIRECT
int start_wanduck(void){
//	kill_pidfile_s("/var/run/wanduckmain.pid", SIGUSR1);
	char *argv[] = {"/usr/sbin/wanduck", NULL};
	int ret = 0;
	pid_t pid;
	FILE *fp = fopen("/var/run/wanduck.pid", "r");
	if(fp != NULL){
		fclose(fp);
		return 0;
	}
	
	ret = _eval(argv, NULL, 0, &pid);
	
	return ret;
}

int stop_wanduck(void){
//	kill_pidfile_s("/var/run/wanduckmain.pid", SIGUSR2);
	kill_pidfile_s("/var/run/wanduck.pid", SIGTERM);
	
	return 0;
}

/*int start_wanduck_main(void){
	char *argv[] = {"/sbin/wanduckmain", NULL};
	int ret = 0;
	pid_t pid;
	FILE *fp = fopen("/var/run/wanduckmain.pid", "r");
	
	if(fp != NULL){
		fclose(fp);
		return 0;
	}
	
	printf("\n**** start_wanduck_main ****\n");
	ret = _eval(argv, NULL, 0, &pid);
	
	return ret;
}

int stop_wanduck_main(void){
	char *argv[] = {"killall", "wanduckmain", NULL};
	int ret;
	pid_t pid;
	FILE *fp = fopen("/var/run/wanduckmain.pid", "r");
	
	if(fp == NULL)
		return 0;
	fclose(fp);
	
	printf("\n**** stop_wanduck_main ****\n");
	ret = _eval(argv, NULL, 0, &pid);
	
	return ret;
}//*/
#endif
// 2007.10 James }
