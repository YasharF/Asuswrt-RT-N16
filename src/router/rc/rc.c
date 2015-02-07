/*
 * Copyright 2004, ASUSTek Inc.
 * All Rights Reserved.
 * 
 * THIS SOFTWARE IS OFFERED "AS IS", AND ASUS GRANTS NO WARRANTIES OF ANY
 * KIND, EXPRESS OR IMPLIED, BY STATUTE, COMMUNICATION OR OTHERWISE. BROADCOM
 * SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A SPECIFIC PURPOSE OR NONINFRINGEMENT CONCERNING THIS SOFTWARE.
 *
 * $Id: rc.c,v 1.23 2008/11/10 02:13:56 james26_jang Exp $
 */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>
#include <syslog.h>
#include <signal.h>
#include <string.h>
#include <sys/klog.h>
#include <sys/types.h>
#include <sys/mount.h>
#include <sys/reboot.h>
#include <sys/stat.h>
#include <sys/sysmacros.h>
#include <sys/time.h>
#include <sys/utsname.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <net/if_arp.h>
#include <dirent.h>

#include <epivers.h>
#include <bcmnvram.h>
#include <mtd.h>
#include <shutils.h>
#include <rc.h>
#include <netconf.h>
#include <nvparse.h>
#include <bcmdevs.h>
#include <wlutils.h>
#include <bcmparams.h>
#include <etioctl.h>
#include <linux/sockios.h>
//#include <linux/ethtool.h>
#include "ethtool-util.h"
#include <epivers.h>
#include <router_version.h>
#include <ezc.h>

#ifdef USB_SUPPORT
#define PRINTER_SUPPORT 1
#define MASSSTORAGE_SUPPORT 1
#ifdef DLM
#define AUDIO_SUPPORT 1
#define WEBCAM_SUPPORT 1
#endif
#endif

#ifdef __CONFIG_EMF__
extern void load_emf(void);
#endif /* __CONFIG_EMF__ */

static void restore_defaults(void);
static void sysinit(void);
static void rc_signal(int sig);

extern struct nvram_tuple router_defaults[];

static int restore_defaults_g=0;

#define RESTORE_DEFAULTS() (!nvram_match("restore_defaults", "0") || nvram_invmatch("os_name", "linux"))

static int
build_ifnames(char *type, char *names, int *size)
{
	char name[32], *next;
	int len = 0;
	int s;

	/* open a raw scoket for ioctl */
	if ((s = socket(AF_INET, SOCK_RAW, IPPROTO_RAW)) < 0)
       		return -1;

	/*
	 * go thru all device names (wl<N> il<N> et<N> vlan<N>) and interfaces to 
	 * build an interface name list in which each i/f name coresponds to a device
	 * name in device name list. Interface/device name matching rule is device
	 * type dependant:
	 *
	 *	wl:	by unit # provided by the driver, for example, if eth1 is wireless
	 *		i/f and its unit # is 0, then it will be in the i/f name list if
	 *		wl0 is in the device name list.
	 *	il/et:	by mac address, for example, if et0's mac address is identical to
	 *		that of eth2's, then eth2 will be in the i/f name list if et0 is 
	 *		in the device name list.
	 *	vlan:	by name, for example, vlan0 will be in the i/f name list if vlan0
	 *		is in the device name list.
	 */
	foreach (name, type, next) {
		struct ifreq ifr;
		int i, unit;
		char var[32], *mac; 
		unsigned char ea[ETHER_ADDR_LEN];

		/* vlan: add it to interface name list */
		if (!strncmp(name, "vlan", 4)) {
			/* append interface name to list */
			len += snprintf(&names[len], *size - len, "%s ", name);
			continue;
		}

		/* others: proceed only when rules are met */
		for (i = 1; i <= DEV_NUMIFS; i ++) {
			/* ignore i/f that is not ethernet */
			ifr.ifr_ifindex = i;
			if (ioctl(s, SIOCGIFNAME, &ifr))
				continue;
			if (ioctl(s, SIOCGIFHWADDR, &ifr))
				continue;
			if (ifr.ifr_hwaddr.sa_family != ARPHRD_ETHER)
				continue;
			if (!strncmp(ifr.ifr_name, "vlan", 4))
				continue;

			/* wl: use unit # to identify wl */
			if (!strncmp(name, "wl", 2)) {
				if (wl_probe(ifr.ifr_name) ||
				    wl_ioctl(ifr.ifr_name, WLC_GET_INSTANCE, &unit, sizeof(unit)) ||
				    unit != atoi(&name[2]))					
					continue;
			}
			/* et/il: use mac addr to identify et/il */
			else if (!strncmp(name, "et", 2) || !strncmp(name, "il", 2)) {
				snprintf(var, sizeof(var), "%smacaddr", name);
				if (!(mac = nvram_get(var)) || !ether_atoe(mac, ea) ||
				    bcmp(ea, ifr.ifr_hwaddr.sa_data, ETHER_ADDR_LEN))
					continue;

				// add by Chen-I to filter out wl interface here
				if (!wl_probe(ifr.ifr_name))
					continue;

			}
			/* mac address: compare value */
			else if (ether_atoe(name, ea) && !bcmp(ea, ifr.ifr_hwaddr.sa_data, ETHER_ADDR_LEN))
				;
			/* others: ignore */
			else
				continue;

			/* append interface name to list */
			len += snprintf(&names[len], *size - len, "%s ", ifr.ifr_name);
		}
	}
	
	close(s);

	*size = len;
	return 0;
}

#ifdef __CONFIG_WSCCMD__
static void
wps_restore_defaults(void)
{
        /* cleanly up nvram for WPS */
        nvram_unset("wps_seed");
        nvram_unset("wps_config_state");
        nvram_unset("wps_addER");
        nvram_unset("wps_device_pin");
        nvram_unset("wps_pbc_force");
        nvram_unset("wps_config_command");
        nvram_unset("wps_proc_status");
        nvram_unset("wps_status");
        nvram_unset("wps_method");
        nvram_unset("wps_proc_mac");
        nvram_unset("wps_sta_pin");
        nvram_unset("wps_currentband");
        nvram_unset("wps_restart");
        nvram_unset("wps_event");
        nvram_unset("wps_restart");

        nvram_unset("wps_enr_mode");
        nvram_unset("wps_enr_ifname");
        nvram_unset("wps_enr_ssid");
        nvram_unset("wps_enr_bssid");
        nvram_unset("wps_enr_wsec");

        nvram_unset("wps_unit");
}
#endif /* __CONFIG_WSCCMD__ */


#ifdef GUEST_ACCOUNT
static void
virtual_radio_restore_defaults(void)
{
	char tmp[100], prefix[] = "wlXXXXXXXXXX_mssid_";
	int i,j;
	
	nvram_unset("unbridged_ifnames");
	nvram_unset("ure_disable");
	
	/* Delete dynamically generated variables */
	for (i = 0; i < MAX_NVPARSE; i++) {
		sprintf(prefix, "wl%d_", i);
		nvram_unset(strcat_r(prefix, "vifs", tmp));
		nvram_unset(strcat_r(prefix, "ssid", tmp));
		nvram_unset(strcat_r(prefix, "guest", tmp));
		nvram_unset(strcat_r(prefix, "ure", tmp));
		nvram_unset(strcat_r(prefix, "ipconfig_index", tmp));
		nvram_unset(strcat_r(prefix, "nas_dbg", tmp));
		sprintf(prefix, "lan%d_", i);
		nvram_unset(strcat_r(prefix, "ifname", tmp));
		nvram_unset(strcat_r(prefix, "ifnames", tmp));
		nvram_unset(strcat_r(prefix, "gateway", tmp));
		nvram_unset(strcat_r(prefix, "proto", tmp));
		nvram_unset(strcat_r(prefix, "ipaddr", tmp));
		nvram_unset(strcat_r(prefix, "netmask", tmp));
		nvram_unset(strcat_r(prefix, "lease", tmp));
		nvram_unset(strcat_r(prefix, "stp", tmp));
		nvram_unset(strcat_r(prefix, "hwaddr", tmp));
		sprintf(prefix, "dhcp%d_", i);
		nvram_unset(strcat_r(prefix, "start", tmp));
		nvram_unset(strcat_r(prefix, "end", tmp));
		
		/* clear virtual versions */
		for (j=0; j< 16;j++){
			sprintf(prefix, "wl%d.%d_", i,j);
			nvram_unset(strcat_r(prefix, "ssid", tmp));
			nvram_unset(strcat_r(prefix, "ipconfig_index", tmp));
			nvram_unset(strcat_r(prefix, "guest", tmp));		
			nvram_unset(strcat_r(prefix, "closed", tmp));
			nvram_unset(strcat_r(prefix, "wpa_psk", tmp));
			nvram_unset(strcat_r(prefix, "auth", tmp));
			nvram_unset(strcat_r(prefix, "wep", tmp));
			nvram_unset(strcat_r(prefix, "auth_mode", tmp));
			nvram_unset(strcat_r(prefix, "crypto", tmp));
			nvram_unset(strcat_r(prefix, "akm", tmp));
                        nvram_unset(strcat_r(prefix, "hwaddr", tmp));
                        nvram_unset(strcat_r(prefix, "bss_enabled", tmp));
                        nvram_unset(strcat_r(prefix, "bss_maxassoc", tmp));
                        nvram_unset(strcat_r(prefix, "wme_bss_disable", tmp));
                        nvram_unset(strcat_r(prefix, "ifname", tmp));
                        nvram_unset(strcat_r(prefix, "unit", tmp));
                        nvram_unset(strcat_r(prefix, "ap_isolate", tmp));
                        nvram_unset(strcat_r(prefix, "macmode", tmp));
                        nvram_unset(strcat_r(prefix, "maclist", tmp));
                        nvram_unset(strcat_r(prefix, "maxassoc", tmp));
                        nvram_unset(strcat_r(prefix, "mode", tmp));
                        nvram_unset(strcat_r(prefix, "radio", tmp));
                        nvram_unset(strcat_r(prefix, "radius_ipaddr", tmp));
                        nvram_unset(strcat_r(prefix, "radius_port", tmp));
                        nvram_unset(strcat_r(prefix, "radius_key", tmp));
                        nvram_unset(strcat_r(prefix, "key", tmp));
                        nvram_unset(strcat_r(prefix, "key1", tmp));
                        nvram_unset(strcat_r(prefix, "key2", tmp));
                        nvram_unset(strcat_r(prefix, "key3", tmp));
                        nvram_unset(strcat_r(prefix, "key4", tmp));
                        nvram_unset(strcat_r(prefix, "wpa_gtk_rekey", tmp));
                        nvram_unset(strcat_r(prefix, "nas_dbg", tmp));
		}
	}
}

#endif	
static void
upgrade_defaults(void)
{
        char temp[100];
        int i;
        bool bss_enabled = TRUE;
        char *val;
                                                                                                                  
        /* Check whether upgrade is required or not
         * If lan1_ifnames is not found in NVRAM , upgrade is required.
         */
        if (!nvram_get("lan1_ifnames") && !RESTORE_DEFAULTS()) {
                cprintf("NVRAM upgrade required.  Starting.\n");
                                                                                                                  
                if (nvram_match("ure_disable", "1")) {
                        nvram_set("lan1_ifname", "br1");
                        nvram_set("lan1_ifnames", "wl0.1 wl0.2 wl0.3 wl1.1 wl1.2 wl1.3");
                }
                else {
                        nvram_set("lan1_ifname", "");
                        nvram_set("lan1_ifnames", "");
                        for (i = 0; i < 2; i++) {
                                snprintf(temp, sizeof(temp), "wl%d_ure", i);
                                if (nvram_match(temp, "1")) {
                                        snprintf(temp, sizeof(temp), "wl%d.1_bss_enabled", i);
                                        nvram_set(temp, "1");
                                }
                                else {
                                        bss_enabled = FALSE;
                                        snprintf(temp, sizeof(temp), "wl%d.1_bss_enabled", i);
                                        nvram_set(temp, "0");
                                }
                        }
                }
                if (nvram_get("lan1_ipaddr")) {
                        nvram_set("lan1_gateway", nvram_get("lan1_ipaddr"));
                }
                                                                                                                  
                for (i = 0; i < 2; i++) {
                        snprintf(temp, sizeof(temp), "wl%d_bss_enabled", i);
                        nvram_set(temp, "1");
                        snprintf(temp, sizeof(temp), "wl%d.1_guest", i);
                        if (nvram_match(temp, "1")) {
                                nvram_unset(temp);
                                if (bss_enabled) {
                                        snprintf(temp, sizeof(temp), "wl%d.1_bss_enabled", i);
                                        nvram_set(temp, "1");
                                }
                        }
                                                                                                                  
                        snprintf(temp, sizeof(temp), "wl%d.1_net_reauth", i);
                        val = nvram_get(temp);
                        if(!val || (*val == 0))
                                nvram_set(temp, nvram_default_get(temp));
                                                                                                                  
                        snprintf(temp, sizeof(temp), "wl%d.1_wpa_gtk_rekey", i);
                        val = nvram_get(temp);
                        if(!val || (*val == 0))
                                nvram_set(temp, nvram_default_get(temp));
                }

                nvram_commit();

                cprintf("NVRAM upgrade complete.\n");
        }
}


static void
restore_defaults(void)
{
	struct nvram_tuple generic[] = {
		{ "lan_ifname", "br0", 0 },
		{ "lan_ifnames", "eth0 eth2 eth3 eth4", 0 },
		{ "wan_ifname", "eth1", 0 },
		{ "wan_ifnames", "eth1", 0 },
		{ 0, 0, 0 }
	};
	struct nvram_tuple vlan[] = {
		{ "lan_ifname", "br0", 0 },
		{ "lan_ifnames", "vlan0 eth1 eth2 eth3", 0 },
		{ "wan_ifname", "vlan1", 0 },
		{ "wan_ifnames", "vlan1", 0 },
		{ 0, 0, 0 }
	};
	struct nvram_tuple dyna[] = {
		{ "lan_ifname", "br0", 0 },
		{ "lan_ifnames", "", 0 },
		{ "wan_ifname", "", 0 },
		{ "wan_ifnames", "", 0 },
		{ 0, 0, 0 }
	};



#define RC1_START() 
#define RC1_STOP()  
#define RC7_START()
#define RC7_STOP()
#define LINUX_OVERRIDES() 
#define EXTRA_RESTORE_DEFAULTS() 

	struct nvram_tuple *linux_overrides;
	struct nvram_tuple *t, *u;
	int restore_defaults, i;
	uint boardflags;
	char *landevs, *wandevs;
	char lan_ifnames[128], wan_ifnames[128];
	char wan_ifname[32], *next;
	int len;
	int ap = 0;

	/* Restore defaults if told to or OS has changed */
	restore_defaults = !nvram_match("restore_defaults", "0") || nvram_invmatch("os_name", "linux");

	if (restore_defaults)
		cprintf("Restoring defaults...");

	/* Delete dynamically generated variables */
	if (restore_defaults) {
		char tmp[100], prefix[] = "wlXXXXXXXXXX_";
		for (i = 0; i < MAX_NVPARSE; i++) {
			del_filter_client(i);
			del_forward_port(i);
			del_autofw_port(i);
			snprintf(prefix, sizeof(prefix), "wl%d_", i);
			for (t = router_defaults; t->name; t ++) {
				if (!strncmp(t->name, "wl_", 3))
					nvram_unset(strcat_r(prefix, &t->name[3], tmp));
			}
			snprintf(prefix, sizeof(prefix), "wan%d_", i);
			for (t = router_defaults; t->name; t ++) {
				if (!strncmp(t->name, "wan_", 4))
					nvram_unset(strcat_r(prefix, &t->name[4], tmp));
			}
		}
#ifdef __CONFIG_WSCCMD__
                wps_restore_defaults();
#endif /* __CONFIG_WSCCMD__ */

#ifdef GUEST_ACCOUNT
		virtual_radio_restore_defaults();				
#endif
	}

	/* 
	 * Build bridged i/f name list and wan i/f name list from lan device name list
	 * and wan device name list. Both lan device list "landevs" and wan device list
	 * "wandevs" must exist in order to preceed.
	 */
	if ((landevs = nvram_get("landevs")) && (wandevs = nvram_get("wandevs"))) {
		/* build bridged i/f list based on nvram variable "landevs" */
		len = sizeof(lan_ifnames);
		if (!build_ifnames(landevs, lan_ifnames, &len) && len)
			dyna[1].value = lan_ifnames;
		else
			goto canned_config;
		/* build wan i/f list based on nvram variable "wandevs" */
		len = sizeof(wan_ifnames);
		if (!build_ifnames(wandevs, wan_ifnames, &len) && len) {
			dyna[3].value = wan_ifnames;
			foreach (wan_ifname, wan_ifnames, next) {
				dyna[2].value = wan_ifname;
				break;
			}
		}
		else ap = 1;
		linux_overrides = dyna;
	}
	/* override lan i/f name list and wan i/f name list with default values */
	else {
canned_config:
		boardflags = strtoul(nvram_safe_get("boardflags"), NULL, 0);
		if (boardflags & BFL_ENETVLAN)
			linux_overrides = vlan;
		else
			linux_overrides = generic;

		/* override the above linux_overrides with a different table */
		LINUX_OVERRIDES();
	}


	/* Restore defaults */
	for (t = router_defaults; t->name; t++) {
		if (restore_defaults || !nvram_get(t->name)) {
			for (u = linux_overrides; u && u->name; u++) {
				if (!strcmp(t->name, u->name)) {
					nvram_set(u->name, u->value);
					break;
				}
			}
			if (!u || !u->name)
				nvram_set(t->name, t->value);
		}
	}

	/* Force to AP */
	if (ap)
		nvram_set("router_disable", "1");

        /* Always set OS defaults */
        nvram_set("os_name", "linux");
        nvram_set("os_version", ROUTER_VERSION_STR);
        nvram_set("os_date", __DATE__);
        /* Always set WL driver version! */
        nvram_set("wl_version", EPI_VERSION_STR);
                                                                                                                  
        nvram_set("is_modified", "0");
        nvram_set("ezc_version", EZC_VERSION_STR);

	/* Commit values */
	if (restore_defaults) {
		EXTRA_RESTORE_DEFAULTS();
		/* default value of vlan */
		nvram_commit();		
		cprintf("done\n");
	}
}

static void
set_wan0_vars(void)
{
	int unit;
	char tmp[100], prefix[] = "wanXXXXXXXXXX_";
	
	/* check if there are any connections configured */
	for (unit = 0; unit < MAX_NVPARSE; unit ++) {
		snprintf(prefix, sizeof(prefix), "wan%d_", unit);
		if (nvram_get(strcat_r(prefix, "unit", tmp)))
			break;
	}
	/* automatically configure wan0_ if no connections found */
	if (unit >= MAX_NVPARSE) {
		struct nvram_tuple *t;
		char *v;

		/* Write through to wan0_ variable set */
		snprintf(prefix, sizeof(prefix), "wan%d_", 0);
		for (t = router_defaults; t->name; t ++) {
			if (!strncmp(t->name, "wan_", 4)) {
				if (nvram_get(strcat_r(prefix, &t->name[4], tmp)))
					continue;
				v = nvram_get(t->name);
				nvram_set(tmp, v ? v : t->value);
			}
		}
		nvram_set(strcat_r(prefix, "unit", tmp), "0");
		nvram_set(strcat_r(prefix, "desc", tmp), "Default Connection");
		nvram_set(strcat_r(prefix, "primary", tmp), "1");
	}
}

/*
 * create /etc/passwd and /etc/group files
 */
 
static void 
make_etc(void)
{
       FILE *f;
       char *name, *pass;
       
       /* crypt using md5, no salt */
       name = nvram_get("http_username") ? : "admin";
       pass = crypt(nvram_get("http_passwd") ? : "admin", "$1$");
       
       if ((f = fopen("/etc/passwd", "w"))) {
               fprintf(f, "%s:%s:0:0:Administrator:/:/bin/sh\n"
                       "nobody:x:99:99:nobody:/:/sbin/nologin\n", name, pass);
               fclose(f);
       }
       
       if ((f = fopen("/etc/group", "w"))) {
               fprintf(f, "root:x:0:%s\nnobody:x:99:\n", name);
               fclose(f);
       }

       /* uClibc TZ */
       if ((f = fopen("/etc/TZ", "w"))) {
               fprintf(f, "%s\n", nvram_safe_get("time_zone"));
               fclose(f);
       }

       /* set lan_hostname */
       if (!nvram_invmatch("lan_hostname", ""))
       {
               /* derive from et0 mac addr */
               char *mac = nvram_get("et0macaddr");
               if (mac && strlen(mac) == 17)
               {
                       char hostname[16];
                       sprintf(hostname, "RT-%c%c%c%c%c%c%c%c%c%c%c%c",
                               mac[0], mac[1], mac[3], mac[4], mac[6], mac[7],
                               mac[9], mac[10], mac[12], mac[13], mac[15], mac[16]);
                       nvram_set("lan_hostname", hostname);
               }
       }

       /* hostname */
       if ((f = fopen("/proc/sys/kernel/hostname", "w"))) {
               fputs(nvram_safe_get("lan_hostname"), f);
               fclose(f);
       }
}




static int noconsole = 0;

static void
sysinit(void)
{
	time_t tm = 0;
	time_t now;
        struct tm gm, local;
        struct timezone tz;
	char *loglevel;

	/* /proc */
	mount("proc", "/proc", "proc", MS_MGC_VAL, NULL);

#ifdef LINUX26
        mount("sysfs", "/sys", "sysfs", MS_MGC_VAL, NULL);
#endif /* LINUX26 */

	/* /tmp */
	mount("ramfs", "/tmp", "ramfs", MS_MGC_VAL, NULL);
	mkdir("/tmp/rc_notification", 0777);	// 2007.10 James
	mkdir("/tmp/rc_action_incomplete", 0777);	// 2007.10 James

	/* /var */
	mkdir("/tmp/var", 0777);
	mkdir("/var/lock", 0777);
	mkdir("/var/log", 0777);
	mkdir("/var/run", 0777);
	mkdir("/var/tmp", 0777);
	mkdir("/tmp/samba", 0777);
	mkdir("/tmp/samba/private", 0777);
	mkdir("/tmp/samba/var", 0777);
	mkdir("/tmp/samba/var/locks", 0777);
	mkdir("/tmp/samba/lib", 0777);

	/* Setup console */
	if (console_init())
		noconsole = 1;

#ifdef LINUX26
	mkdir("/dev/pts", 0755);
	mount("devpts", "/dev/pts", "devpts", MS_MGC_VAL, NULL);
        mkdir("/dev/shm", 0777);
        eval("/sbin/hotplug2", "--coldplug");
#endif /* LINUX26 */

	 /* create /etc/{passwd,group,TZ} */
	make_etc();

        if ((loglevel = nvram_get("console_loglevel")))
                klogctl(8, NULL, atoi(loglevel));
        else
                klogctl(8, NULL, 1);

        /* Update kernel timezone and time */
        setenv("TZ", nvram_safe_get("time_zone"), 1);
        time(&now);
        gmtime_r(&now, &gm);
        localtime_r(&now, &local);
        tz.tz_minuteswest = (mktime(&gm) - mktime(&local)) / 60;
        settimeofday(NULL, &tz);
}


static void
insmod(void)
{
	char buf[PATH_MAX];
	struct utsname name;
	struct stat tmp_stat;

	/* Modules */
	uname(&name);
	snprintf(buf, sizeof(buf), "/lib/modules/%s", name.release);
	if (stat("/proc/modules", &tmp_stat) == 0 &&
	    stat(buf, &tmp_stat) == 0) {

		char module[80], *modules, *next;

	        /* Load ctf */
                eval("insmod", "ctf");

#ifdef __CONFIG_EMF__
                /* Load the EMF & IGMP Snooper modules */
                load_emf();
#endif

		modules = nvram_get("kernel_mods") ? : "et wl";
		foreach(module, modules, next)
			eval("insmod", module);
		
#ifdef USB_SUPPORT
#ifdef LINUX26
		modules = "nls_cp932 nls_cp936 nls_cp949 nls_cp950 nls_cp1250 nls_cp1251 nls_euc-jp";
#else
		modules = "nls_cp950 nls_big5 nls_cp936 nls_gb2312 nls_utf8";
#endif
		foreach(module, modules, next)
			eval("insmod", module);
#endif

	}

	/* set the packet size */
//	if (nvram_match("jumbo_frame_enable", "1"))
//		eval("et", "robowr", "0x40", "0x05", nvram_get("jumbo_frame_size"));

	dprintf("done\n");
}


/* States */
enum {
	RESTART,
	STOP,
	START,
	TIMER,
	IDLE,
	SERVICE,
};
static int state = START;
static int signalled = -1;


/* Signal handling */
static void
rc_signal(int sig)
{
	if (state == IDLE) {	
		if (sig == SIGHUP) {
			dprintf("signalling RESTART\n");
			signalled = RESTART;
		}
		else if (sig == SIGUSR2) {
			dprintf("signalling START\n");
			signalled = START;
		}
		else if (sig == SIGINT) {
			dprintf("signalling STOP\n");
			signalled = STOP;
		}
		else if (sig == SIGALRM) {
			dprintf("signalling TIMER\n");
			signalled = TIMER;
		}
		else if (sig == SIGUSR1) {
			dprintf("signalling USR1\n");
			signalled = SERVICE;
		}
	}
}

/* Timer procedure */
int
do_timer(void)
{
	int interval = atoi(nvram_safe_get("timer_interval"));
	time_t now;
	struct tm gm, local;
	struct timezone tz;

	dprintf("%d\n", interval);

#ifdef ASUS_EXT
	/* Update kernel timezone */
	setenv("TZ", nvram_safe_get("time_zone_x"), 1);
	time(&now);
	gmtime_r(&now, &gm);
	localtime_r(&now, &local);
	tz.tz_minuteswest = (mktime(&gm) - mktime(&local)) / 60;
	settimeofday(NULL, &tz);
	return 0;
#endif
	if (interval == 0)
		return 0;

	/* Report stats */
	if (nvram_invmatch("stats_server", "")) {
		char *stats_argv[] = { "stats", nvram_get("stats_server"), NULL };
		_eval(stats_argv, NULL, 5, NULL);
	}

	/* Sync time */
	start_ntpc();

	/* Update kernel timezone */
	setenv("TZ", nvram_safe_get("time_zone_x"), 1);
	time(&now);
	gmtime_r(&now, &gm);
	localtime_r(&now, &local);
	tz.tz_minuteswest = (mktime(&gm) - mktime(&local)) / 60;
	settimeofday(NULL, &tz);
	alarm(interval);
	return 0;
}

static void
early_defaults(void)
{
	int stbport;
//	char hw_name[32];
	
//	printf("dbg: early def 3\n");

	//snprintf(hw_name, 9, "%s", nvram_get("hardware_version"));
//	snprintf(hw_name, 6, "%s", nvram_safe_get("hardware_version"));	// 2008.03 James.
	
//	printf("dbg: hw_name = %s!\n", hw_name);

	if(nvram_match("wan_route_x", "IP_Bridged")){
//		if(nvram_match("boardtype", "0x48E") && nvram_match("boardnum", "45")){
//			printf("dbg: IP_Bridged\n");
		if(nvram_match("boardtype", "0x04cf")){
                        nvram_set("vlan1ports", "0 1 2 3 4 8*");
                        nvram_set("vlan2ports", "8u");
                }
		else {
			nvram_set("vlan0ports", "0 1 2 3 4 5*");
			nvram_set("vlan1ports", "5u");
		}
//		}
	}
	else{ /* router mode, use vlans */
		/* Adjust switch config to bridge STB LAN port with WAN port */
		stbport = atoi(nvram_safe_get("wan_stb_x"));
		
		/* Check existing config for validity */
		if(stbport < 0 || stbport > 5)
			stbport = 0;
		/* N12 ports */
		if(nvram_match("boardtype", "0x04CD") ){
				static char *vlan0ports[] = {"0 1 2 3 5*",
								 "0 1 2 5*",
								 "0 1 3 5*",
								 "0 2 3 5*",
								 "1 2 3 5*",
								 "2 3 5*"};
				static char *vlan1ports[] = {"4 5u",
								 "3 4 5u",
								 "2 4 5u",
								 "1 4 5u",
								 "4 0 5u",
								 "0 1 4 5u"};
				
				nvram_set("vlan0ports", vlan0ports[stbport]);
				nvram_set("vlan1ports", vlan1ports[stbport]);
		} 
		/* N16 ports */
		else if(nvram_match("boardtype", "0x04cf")){
                                /* Set LAN ports */
                                static char *vlan0ports[] = {"1 2 3 4 8*",
                                                                 "1 2 3 8*",
                                                                 "1 2 4 8*",
                                                                 "1 3 4 8*",
                                                                 "2 3 4 8*",
                                                                 "3 4 8*"};
                                /* Set WAN ports */
                                static char *vlan1ports[] = {"0 8u",            // Defaults
								"0 4 8u",      	// LAN 1
								"0 3 8u",      	// LAN 2
								"0 2 8u",      	// LAN 3
								"0 1 8u",      	// LAN 4
								"0 1 2 8u"};    // LAN 3&4
								
				nvram_set("vlan1ports", vlan0ports[stbport]);   // Set LAN ports
				nvram_set("vlan2ports", vlan1ports[stbport]);   // Set WAN ports
//				nvram_set("networkmap_down","1");
		}
	}
}

// 2007.10 James {
static void handle_notifications(void){
	DIR *directory = opendir("/tmp/rc_notification");
	
	printf("handle_notifications() start\n");
	
	if(directory == NULL)
		return;
	
	while(TRUE){
		struct dirent *entry;
		char *full_name;
		FILE *test_fp;

		entry = readdir(directory);
		if (entry == NULL)
			break;
		if (strcmp(entry->d_name, ".") == 0)
			continue;
		if (strcmp(entry->d_name, "..") == 0)
			continue;

		/* Remove the marker file. */
		full_name = (char *)(malloc(strlen(entry->d_name) + 100));
		if (full_name == NULL)
		{
			fprintf(stderr,
					"Error: Failed trying to allocate %lu bytes of memory for "
					"the full name of an rc notification marker file.\n",
					(unsigned long)(strlen(entry->d_name) + 100));
			break;
		}
		sprintf(full_name, "/tmp/rc_notification/%s", entry->d_name);
		remove(full_name);
		
		//printf("Flag : %s\n", entry->d_name);

		/* Take the appropriate action. */
		if (strcmp(entry->d_name, "restart_reboot") == 0)
		{
			fprintf(stderr, "rc rebooting the system.\n");
			sleep(1);	// wait httpd sends the page to the browser.
			eval("reboot");
			return;
		}
		else if (strcmp(entry->d_name, "restart_networking") == 0)
		{
			fprintf(stderr, "rc restarting networking.\n");
			
#ifdef WEB_REDIRECT
			printf("--- SERVICE: Wait to kill wanduck ---\n");
			stop_wanduck();
			
			signalled = RESTART;
			//eval("reboot");
#endif
			return;
		}
#ifdef DLM
		else if (strcmp(entry->d_name, "restart_cifs") == 0)
		{
			fprintf(stderr, "rc restarting CIFS and FTP.\n");
			nvram_set("usb_storage_busy", "1");	// 2007.12 James.
			run_ftpsamba();
			nvram_set("usb_storage_busy", "0");	// 2007.12 James.
		}
		/*else if (strcmp(entry->d_name, "restart_ftp") == 0)
		{
			fprintf(stderr, "rc restarting FTP.\n");
			//run_ftp();
		}*/
#endif
		else if (strcmp(entry->d_name, "restart_ddns") == 0)
		{
			fprintf(stderr, "rc restarting DDNS.\n");
			stop_ddns();
			
			if(nvram_match("ddns_enable_x", "1")){
				start_ddns();
				
#ifdef DLM
				if(nvram_match("ddns_server_x", "WWW.ASUS.COM")
						&& strstr(nvram_safe_get("ddns_hostname_x"), ".asuscomm.com") != NULL){
					// because the computer_name is followed by DDNS's hostname.
					if(nvram_match("samba_running", "1")){
						stop_samba();
						run_samba();
					}
					
					if(nvram_match("ftp_running", "1")){
						stop_ftp();
						run_ftp();
					}
					
					if(nvram_match("dms_running", "1")){
						stop_dms();
						run_dms();
					}
				}
#endif
			}
		}
		else if (strcmp(entry->d_name, "restart_httpd") == 0)
		{
			fprintf(stderr, "rc restarting HTTPD.\n");
			stop_httpd();
			nvram_unset("login_ip");
			nvram_unset("login_timestamp");
			start_httpd();
		}
		else if (strcmp(entry->d_name, "restart_dns") == 0)
		{
			fprintf(stderr, "rc restarting DNS.\n");
			stop_dns();
			start_dns();
		}
		else if (strcmp(entry->d_name, "restart_dhcpd") == 0)
		{
			fprintf(stderr, "rc restarting DHCPD.\n");
			stop_dhcpd();
			start_dhcpd();
		}
		else if (strcmp(entry->d_name, "restart_upnp") == 0)
		{
			fprintf(stderr, "rc stoping UPNP.\n");
			stop_upnp();
			if(nvram_match("upnp_enable", "1")){
				fprintf(stderr, "rc restarting UPNP.\n");
				start_upnp();
			}
			
#ifdef DLM
			fprintf(stderr, "rc stoping Media Server.\n");
			stop_dms();
			if(nvram_match("apps_dms", "1")){
				fprintf(stderr, "rc restarting Media Server.\n");
				run_dms();
			}
#endif
		}
#ifdef QOS
		else if (strcmp(entry->d_name, "restart_qos") == 0) //--SZ Angela 09.03 QOS
		{
                        fprintf(stderr, "rc restarting QOS.\n");
                                                                                                                                      
// 2009.03 James. {
                        nvram_set("qos_userspec_app", "0");
                        nvram_set("qos_global_enable", "0");
                        nvram_set("qos_userdef_enable", "0");
                                                                                                                                      
                        if(nvram_invmatch("qos_rulenum_x", "0"))
                                nvram_set("qos_userspec_app", "1");
                                                                                                                                      
                        if(nvram_match("qos_tos_prio", "1")
                                        || nvram_match("qos_pshack_prio", "1")
                                        || nvram_match("qos_service_enable", "1")
                                        || nvram_match("qos_shortpkt_prio", "1"))
                                nvram_set("qos_global_enable", "1");
                                                                                                                                      
                        if(nvram_match("qos_userspec_app", "1")
                                        || nvram_match("qos_dfragment_enable", "1"))
                                nvram_set("qos_userdef_enable", "1");
                                                                                                                                      
                        if(nvram_match("qos_global_enable", "1") || nvram_match("qos_userdef_enable", "1"))
                                nvram_set("qos_enable", "1");
                        else
                                nvram_set("qos_enable", "0");
                        nvram_commit();
                                                                                                                                      
                        if(nvram_match("qos_enable", "1")){
                        //if(nvram_match("wan_status_t", "Connected")){
// 2009.03 James. }
                                nvram_set("qos_ubw", "0");
                                nvram_set("qos_ubw_tmp", "0");
                                qos_get_wan_rate();
                                fprintf(stderr, "Get wan rate = %s.\n", nvram_safe_get("qos_ubw"));
                                nvram_set("qos_ubw_tmp", nvram_get("qos_ubw"));
                                                                                                                                      
                                Speedtest_Init();

			}
//			fprintf(stderr, "rc restarting QOS.\n");
//			Speedtest_Init();
//			fprintf(stderr, "rc:after Speedtest_init().\n");
		}
#endif
		else if (strcmp(entry->d_name, "restart_syslog") == 0)
		{
			fprintf(stderr, "rc restarting syslogd.\n");
#ifdef ASUS_EXT
			stop_logger();
			start_logger();
#endif
		}
		else if (strcmp(entry->d_name, "restart_firewall") == 0)
		{
			char wan_ifname[16];
			char *wan_proto = nvram_safe_get("wan_proto");
			
			fprintf(stderr, "rc restarting firewall.\n");
			/*if(!nvram_match("wan_status_t", "Connected"))
				continue;//*/
			
// 2008.08 James. {
			//wan_ifname = nvram_get("wan0_ifname");
			memset(wan_ifname, 0, 16);
			strncpy(wan_ifname, nvram_safe_get("wan_ifname_t"), 16);
			if(strlen(wan_ifname) == 0){
				if(!strcmp(wan_proto, "pppoe")
						|| !strcmp(wan_proto, "pptp")
						|| !strcmp(wan_proto, "l2tp"))
					strcpy(wan_ifname, "ppp0");
				else
					strcpy(wan_ifname, "eth0");
			}
// 2008.08 James. }
			
			start_firewall();
			
#ifdef NOIPTABLES
			start_firewall2(wan_ifname);
#else
			fprintf(stderr, "rc restarting IPTABLES firewall.\n");
			start_firewall_ex(wan_ifname, nvram_safe_get("wan0_ipaddr"), "br0", nvram_safe_get("lan_ipaddr"));
#endif
			
#ifndef ASUS_EXT
			/* Start connection dependent firewall */
			start_firewall2(wan_ifname);
#endif
		}
		else if (strcmp(entry->d_name, "restart_ntpc") == 0)
		{
			fprintf(stderr, "rc restarting ntpc.\n");
			stop_ntpc();
			start_ntpc();
		}
		else if (strcmp(entry->d_name, "rebuild_cifs_config_and_password") == 0)
		{
			fprintf(stderr, "rc rebuilding CIFS config and password databases.\n");
//			regen_passwd_files(); /* Must be called before regen_cifs_config_file(). */
			//regen_cifs_config_file();
		}
		else if (strcmp(entry->d_name, "ddns_update") == 0)
		{
			fprintf(stderr, "rc updating ez-ipupdate for ddns changes.\n");
			//update_ddns_changes();
		}
// 2008.01 James. {
		else if(strcmp(entry->d_name, "restart_time") == 0)
		{
			fprintf(stderr, "rc restarting time.\n");
			
			do_timer();
			
#ifdef ASUS_EXT  
			stop_logger();
			start_logger();
#endif
			stop_ntpc();
			start_ntpc();
		}
#ifdef WSC
		else if(!strcmp(entry->d_name, "restart_wps"))
		{
			//;	// do nothing
		}
#endif
#ifdef DLM
		// when apps_running isn't set, it means no disk. when apps_running is set 1 or 0, it means there are disks.
		else if(!strcmp(entry->d_name, "restart_apps")){
			char usb_path1[16], usb_path2[16];
			
			memset(usb_path1, 0, 16);
			memset(usb_path2, 0, 16);
			strcpy(usb_path1, nvram_safe_get("usb_path1.1"));
			strcpy(usb_path2, nvram_safe_get("usb_path1.2"));
			if(!strcmp(usb_path1, "storage") || !strcmp(usb_path2, "storage")){
				eval("killall", "snarf");
				eval("killall", "ctorrent");
				eval("killall", "rtorrent");
				eval("killall", "giftd");
				eval("killall", "dmathined");
				
				nvram_set("apps_running", "0");
				nvram_set("apps_installed", "0");
				nvram_set("apps_dl_x", "0");
				nvram_set("apps_status_checked", "0");
				nvram_set("apps_disk_free", "0");
				
				if(!strcmp(nvram_safe_get("apps_dlx"), "1"))
					run_apps();
				else{
					if(!strcmp(nvram_safe_get("swap_on"), "1")){
						char *swap_file = nvram_safe_get("swap_file");
						
						if(strcmp(swap_file, "") != 0){
							printf("try to swapoff %s.\n", swap_file);
							swapoff(swap_file);
							
							sleep(1);
							
							if(swap_check() == 1){
								logmessage("USB storage", "swapoff unsuccessfully");
								nvram_set("swapoff_failed", "1");
								
								return -1;
							}
							else{
								logmessage("USB storage", "swapoff successfully");
								nvram_set("swap_on", "0");
								nvram_set("swap_file", "");	// 2008.06 James.
								nvram_set("swapoff_failed", "0");
								nvram_set("apps_dms_usb_port_x2", "-1");
								unlink(swap_file);
							}
						}
					}
				}
			}
		}
#endif
// 2008.01 James. }
		else
		{
			fprintf(stderr,
					"WARNING: rc notified of unrecognized event `%s'.\n",
					entry->d_name);
		}

		/*
		 * If there hasn't been another request for the same event made since
		 * we started, we can safely remove the ``action incomplete'' marker.
		 * Otherwise, we leave the marker because we'll go through here again
		 * for this even and mark it complete only after we've completed it
		 * without getting another request for the same event while handling
		 * it.
		 */
		test_fp = fopen(full_name, "r");
		if (test_fp != NULL)
		{
			fclose(test_fp);
		}
		else
		{
			/* Remove the marker file. */
			sprintf(full_name, "/tmp/rc_action_incomplete/%s", entry->d_name);
			remove(full_name);
		}

		free(full_name);
	} 
	
	printf("handle_notifications() end, state : %d\n", state);
	closedir(directory);
}
// 2007.10 James }

int
restart_wps(void)
{
        fprintf(stderr, "... restart wps ...\n");

        stop_wps();
        stop_nas();
        stop_eapd();

        start_eapd();
        start_nas();
        start_wps();

        return 0;
}

/* Main loop */
static void
main_loop(void)
{
	sigset_t sigset;
	pid_t shell_pid = 0;
	uint boardflags;
	
	/* Convert vital config before loading modules */
	
	/* Basic initialization */
	sysinit();

#ifdef ASUS_EXT	
//#ifdef PBCENR
	init_switch_mode();
//#else
//	nvram_set("sw_mode_ex", nvram_safe_get("sw_mode"));
//#endif
	convert_asus_values(0);		// move up by Jiahao for WL500gP
	cprintf("[rc]:convert asus values\n");
#endif
	early_defaults();
	insmod();

	/* Setup signal handlers */
	signal_init();
	signal(SIGHUP, rc_signal);
	signal(SIGUSR1, rc_signal);
	signal(SIGUSR2, rc_signal);
	signal(SIGINT, rc_signal);
	signal(SIGALRM, rc_signal);
	sigemptyset(&sigset);

	/* Give user a chance to run a shell before bringing up the rest of the system */
	if (!noconsole)
		run_shell(1, 0);

	/* Add vlan */
	boardflags = strtoul(nvram_safe_get("boardflags"), NULL, 0);

	/* Add loopback */
	config_loopback();

        /* Upgrade NVRAM variables to MBSS mode */
        upgrade_defaults();

	/* Convert deprecated variables */
	convert_deprecated();

	/* Restore defaults if necessary */
	restore_defaults();


#ifdef ASUS_EXT	
	convert_asus_values(0);
#endif


	/* Setup wan0 variables if necessary */
	set_wan0_vars();


	/* Loop forever */
	for (;;) {
		switch (state) {
		case SERVICE:
			dprintf("SERVICE\n");
			// 2007.10 James remove {}
			// service_handle();
			if(nvram_get("rc_service") != NULL){	// for original process
				printf("\n### start the original process of rc! ###\n");
				service_handle();
			}
			else{	// for new process
				printf("\n### start the new process of rc! ###\n");
				handle_notifications();
			}
			state = IDLE;
			
			break;
		case RESTART:
			dprintf("RESTART\n");
			/* Fall through */
		case STOP:
			dprintf("STOP\n");

#ifdef ASUS_EXT				
			stop_misc();
#endif
			stop_services();
			stop_wan();
#ifdef WSC
//			stop_wsc();/* Cherry Cho added in 2007/4/27. */
			stop_wps();
#endif			
			stop_lan();

			if (boardflags & BFL_ENETVLAN)
			{
				stop_vlan();
			}

			if (state == STOP) {
				state = IDLE;
				break;
			}
			/* Fall through */
		case START:
			cprintf("START %d\n", boardflags);
			{ /* Set log level on restart */
	        		char *loglevel;
				int loglev = 8;

		                if ((loglevel = nvram_get("console_loglevel"))) {
                			loglev = atoi(loglevel);
		                }
                		klogctl(8, NULL, loglev);
		                if (loglev < 7) {
	                	cprintf("WARNING: console log level set to %d\n", loglev);
        			}
			}
		
			if (boardflags & BFL_ENETVLAN)
			{
				start_vlan();
			}

			start_lan();
			// emi test 
			//eval("utelnetd", "-d");
			start_services();
			start_wan();
			//start_nas("wan");
			//start_nas();
			start_wl();
#ifdef PBCENR
                        if (nvram_match("sw_mode_ex", "2"))
                                start_ure_monitor();
#endif

#ifdef WSC
			if(!nvram_match("wps_down", "1")
#ifdef PBCENR
				&& nvram_match("sw_mode_ex", "1")
#endif
			)
				start_wps();

			
			if(nvram_match("wps_down", "1")
				) {
				stop_wps();
				if (nvram_match("wps_mode", "enabled")) {
					nvram_set("wps_mode", "disabled");
					nvram_set("wl0_wps_mode", "disabled");
				}
			}
#endif
		
#ifdef ASUS_EXT
			if(!nvram_match("misc_down", "1"))
				start_misc();
			sleep(3);

			if(nvram_match("wan_proto", "dhcp"))       /* 0712 ASUS */
                        {
                                printf("start detectWan\n");
				system("detectWan &");
                        }
//			diag_PaN();
			
			//2008.06.30 Yau add to start IP monitor
			if(nvram_match("wan_route_x", "IP_Routed")
					&& strcmp(nvram_safe_get("networkmap_down"), "1") != 0){
				printf("* Start Network Map!\n");
				start_networkmap();
			}
#endif
			/* Fall through */
		case TIMER:
			dprintf("TIMER\n");
			do_timer();			
			/* Fall through */
		case IDLE:
			dprintf("IDLE\n");
			state = IDLE;
			/* Wait for user input or state change */
			while (signalled == -1) {
				if (!noconsole && (!shell_pid || kill(shell_pid, 0) != 0))
					shell_pid = run_shell(0, 1);
				else
					sigsuspend(&sigset);
			}
			state = signalled;
			signalled = -1;
			break;
		default:
			dprintf("UNKNOWN\n");
			return;
		}
	}
}

// 2007.10 James. replace all "strstr" to "!strcmp"
int
main(int argc, char **argv)
{
	int result;
	char usb_disc[32];
	char *base = strrchr(argv[0], '/');
	
	base = base ? base + 1 : argv[0];
		
//	FILE *fp;	// added by Jiahao for WL500gP

	/* init */
#ifdef LINUX26
        if (strstr(base, "preinit")) {
                mount("devfs", "/dev", "tmpfs", MS_MGC_VAL, NULL);
                mknod("/dev/console", S_IRWXU|S_IFCHR, makedev(5, 1));
#else /* LINUX26 */
        if (strstr(base, "init")) {
#endif /* LINUX26 */
		main_loop();
		return 0;
	}

	/* Set TZ for all rc programs */
	setenv("TZ", nvram_safe_get("time_zone_x"), 1);

	/* ppp */
	if (!strcmp(base, "ip-up"))
		return ipup_main(argc, argv);
	/* write usb disc status */
	else if (!strcmp(base, "write_disc_status"))	// added by Jiahao for WL500gP
	{
		write_disc_status();
		return 0;
	}
	else if (!strcmp(base, "ip-down"))
		return ipdown_main(argc, argv);
	/* udhcpc [ deconfig bound renew ] */
	else if (!strcmp(base, "udhcpc"))
		return udhcpc_main(argc, argv);
	/* restore default */
	else if (!strcmp(base, "restore"))
	{
		if (argc==2)
		{
			int step = atoi(argv[1]);
			if (step>=1)
			{
				nvram_set("vlan_enable", "1");
				restore_defaults();
			}
			/* Setup wan0 variables if necessary */
			if (step>=2)
				set_wan0_vars();
			if (step>=3)
				RC1_START();
			if (step>=4)
				start_lan();
		}
		return 0;
	}
	/* stats [ url ] */
	else if (!strcmp(base, "stats"))
		return http_stats(argv[1] ? : nvram_safe_get("stats_server"));

	/* erase [device] */
	else if (!strcmp(base, "erase")) {
		if (argv[1])
			return mtd_erase(argv[1]);
		else {
			fprintf(stderr, "usage: erase [device]\n");
			return EINVAL;
		}
	}
	/* write [path] [device] */
	else if (!strcmp(base, "write")) {
		if (argc >= 3)
			return mtd_write(argv[1], argv[2]);
		else {
			fprintf(stderr, "usage: write [path] [device]\n");
			return EINVAL;
		}
	}
//#ifdef USB_SUPPORT
	/* hotplug [event] */
	else if (!strcmp(base, "hotplug")) {

//		fp = fopen("/tmp/hotplug_usb", "a");	// added by Jiahao for WL500gP
//		fprintf(fp, "in hotplug\n");
//		fclose(fp);

		if (argc >= 2) {
			if (!strcmp(argv[1], "net"))
				return hotplug_net();
#ifdef ASUS_EXT
#ifdef USB_SUPPORT
			else if(!strcmp(argv[1], "usb"))
			{

//				fp = fopen("/tmp/hotplug_usb", "a");	// added by Jiahao for WL500gP
//				fprintf(fp, "call hotplug_usb()\n");
//				fclose(fp);

				fprintf(stderr, "hotplug USB\n");

				return hotplug_usb();
			}
			else if (!strcmp(argv[1], "block")) {
				fprintf(stderr, "hotplug USB\n");

				return hotplug_usb();
			}
#endif
#endif
		} 
		else {
			fprintf(stderr, "usage: hotplug [event]\n");
			return EINVAL;
		}
	}
//#endif

#ifdef ASUS_EXT
	/* ddns update ok */
	else if (!strcmp(base, "stopservice")) {
		if (argc >= 2)
			return(stop_service_main(atoi(argv[1])));
		else return(stop_service_main(0));
	}
	/* ddns update ok */
	else if (!strcmp(base, "ddns_updated")) 
	{
		return ddns_updated_main();
	}
	/* ddns update ok */
	else if (!strcmp(base, "start_ddns")) 
	{
		return start_ddns();
	}
	else if (!strcmp(base, "start_dns")) 
	{
		stop_dns();
		return start_dns();
	}

	else if (!strcmp(base, "start_wps_proc")) 
	{
		start_wps_proc(atoi(argv[1]));
		return 0;
	}
	else if (!strcmp(base, "stop_wps_proc")) 
	{
		stop_wps_proc();
		sleep(2);
		printf("restart_wps");
		restart_wps();
		return 0;
	}

#ifdef USB_SUPPORT
	/* send alarm */
#ifdef DLM
	else if (!strcmp(base, "sendalarm")) {
		if (argc >= 1)
			return sendalarm_main(argc, argv);
		else {
			fprintf(stderr, "usage: sendalarm\n");
			return EINVAL;
		}
	}
#endif
#endif
	/* invoke watchdog */
	else if (!strcmp(base, "watchdog")) {
		return(watchdog_main());
	}
	else if (!strcmp(base, "gpio")) {
		return(gpio_main(/*atoi(argv[1])*/));
	}
	else if (!strcmp(base, "radioctrl")) {
		if (argc >= 1)
			return(radio_main(atoi(argv[1])));
		else return EINVAL;
	}
#ifdef BTN_SETUP
	/* invoke ots(one touch setup) */
	else if (!strcmp(base, "ots")) {
		return(ots_main());
	}
#endif

#ifdef USB_SUPPORT
#ifdef WEBCAM_SUPPORT
	/* remove webcam module */
	else if (!strcmp(base, "rmwebcam")) {
		if (argc >= 2)
			return (remove_webcam_main(atoi(argv[1])));
		else return EINVAL;
	}
#endif
	/* remove usbstorage module */
	else if (!strcmp(base, "rmstorage")) {
		return (remove_storage_main());
	}
#endif
	/* run ntp client */
	else if (!strcmp(base, "ntp")) {
		return (ntp_main());
	}
#ifdef USB_SUPPORT
#ifdef DLM
	/* run rcamd */
	else if (!strcmp(base, "rcamdmain")) {
		return (rcamd_main());
	}
	/* run waveserver */
	else if (!strcmp(base, "waveservermain")) {
		return (waveserver_main());
	}
#endif
#endif
	/* write srom */
	else if (!strcmp(base, "wsrom")) 
	{
		do_timer();
		if (argc >= 4) 
			return wsrom_main(argv[1], atoi(argv[2]), atoi(argv[3]));
		else {
			fprintf(stderr, "usage: wsrom [dev] [position] [value in 2 bytes]\n");
			return EINVAL;
		}
	}
	/* write srom */
	else if (!strcmp(base, "rsrom")) 
	{
		if (argc >= 3)
		{	 
			rsrom_main(argv[1], atoi(argv[2]), 1);
			return 0;
		}
		else {
			fprintf(stderr, "usage: rsrom [dev] [position]\n");
			return EINVAL;
		}
	}
	/* write mac */
	else if (!strcmp(base, "wmac")) 
	{
		if (argc >= 3) 
			return write_mac(argv[1], argv[2]);
		else {
			fprintf(stderr, "usage: wmac [dev] [mac]\n");
			return EINVAL;
		}
	}
	/* wlan update */
	else if (!strcmp(base, "wlan_update")) 
	{
		wlan_update();
		return 0;
	}
// 2007.10 James {
#ifdef QOS
	else if(!strcmp(base, "speedtest"))
	{
		qos_get_wan_rate();
		fprintf(stderr, "Get wan rate = %s.\n", nvram_safe_get("qos_ubw"));
		
		return 0;
	}
#endif
	else if(!strcmp(base, "convert_asus_values"))
	{
		convert_asus_values(1);
		
		return 0;
	}
#ifdef DLM
	else if(!strcmp(base, "run_dms")){
		nvram_set("usb_storage_busy", "1");
		run_dms();
		nvram_set("usb_storage_busy", "0");
		
		return 0;
	}
	else if(!strcmp(base, "run_samba")){
		nvram_set("usb_storage_busy", "1");
		run_samba();
		nvram_set("usb_storage_busy", "0");
		
		return 0;
	}
	else if(!strcmp(base, "run_ftp")){
		nvram_set("usb_storage_busy", "1");
		run_ftp();
		nvram_set("usb_storage_busy", "0");
		
		return 0;
	}
	else if(!strcmp(base, "stop_dms")){
		nvram_set("usb_storage_busy", "1");
		stop_dms();
		nvram_set("usb_storage_busy", "0");
		
		return 0;
	}
	else if(!strcmp(base, "stop_samba")){
		nvram_set("usb_storage_busy", "1");
		stop_samba();
		nvram_set("usb_storage_busy", "0");
		
		return 0;
	}
	else if(!strcmp(base, "stop_ftp")){
		nvram_set("usb_storage_busy", "1");
		stop_ftp();
		nvram_set("usb_storage_busy", "0");
		
		return 0;
	}
	else if(!strcmp(base, "stop_ftpsamba")){
		nvram_set("usb_storage_busy", "1");
		stop_ftpsamba();
		nvram_set("usb_storage_busy", "0");
		
		return 0;
	}
	else if(!strcmp(base, "refresh_upnp")){
		stop_dms();
		sleep(2);
		start_dms();
		
		return 0;
	}

#endif // DLM
	/* udhcpc_ex [ deconfig bound renew ], for lan only */
	else if (!strcmp(base, "landhcpc"))
		return udhcpc_ex_main(argc, argv);
#endif

	/* rc [stop|start|restart ] */
	else if (!strcmp(base, "rc")) {
		if (argv[1]) {
			if (strncmp(argv[1], "start", 5) == 0)
				return kill(1, SIGUSR2);
			else if (strncmp(argv[1], "stop", 4) == 0)
				return kill(1, SIGINT);
			else if (strncmp(argv[1], "restart", 7) == 0)
				return kill(1, SIGHUP);
		} else {
			fprintf(stderr, "usage: rc [start|stop|restart]\n");
			return EINVAL;
		}
	}
#ifdef DLM
	else if (!strcmp(base, "run_apps"))
		return run_apps();
	else if (!strcmp(base, "run_ftpsamba"))		// added by Jiahao for WL500gP
	{
		nvram_set("usb_storage_busy", "1");	// 2007.12 James.
		run_ftpsamba();
		nvram_set("usb_storage_busy", "0");	// 2007.12 James.
		
		return 0;
	}
#endif
#ifdef USB_SUPPORT
	else if (!strcmp(base, "hotplug_usb_mass"))	// added by Jiahao for WL500gP
	{
		return hotplug_usb_mass("");
	}
#endif
#ifdef DLM
	else if (!strcmp(base, "eject_usb1"))		// added by Jiahao for WL500gP
	{
		memset(usb_disc, 0, sizeof(usb_disc));

		if(nvram_match("usb_disc0_port", "1")){
//			strcpy(usb_disc, "usb_disc0_safely_removed");
			sprintf(usb_disc, "%s", "usb_disc0_safely_removed");
			nvram_set(usb_disc, "1");
		}
		else if(nvram_match("usb_disc1_port", "1")){
//			strcpy(usb_disc, "usb_disc1_safely_removed");
			sprintf(usb_disc, "%s", "usb_disc0_safely_removed");
			nvram_set(usb_disc, "1");
		}
		else{
			printf("No USB storage device in Port 1.\n");
			return 0;
		}
		
		result = umount_disc_parts_rc(1);
		if(result != 0)
			nvram_set(usb_disc, "0");
		
		return 0;
	}
	else if (!strcmp(base, "eject_usb2"))		// added by Jiahao for WL500gP
	{
		memset(usb_disc, 0, 32);
		if(nvram_match("usb_disc0_port", "2")){
			sprintf(usb_disc, "%s", "usb_disc1_safely_removed");
//			strcpy(usb_disc, "usb_disc0_safely_removed");
			nvram_set(usb_disc, "1");
		}
		else if(nvram_match("usb_disc1_port", "2")){
//			strcpy(usb_disc, "usb_disc1_safely_removed");
			sprintf(usb_disc, "%s", "usb_disc1_safely_removed");
			nvram_set(usb_disc, "1");
		}
		else{
			printf("No USB storage device in Port 2.\n");
			return 0;
		}
		
		result = umount_disc_parts_rc(2);
		if(result != 0)
			nvram_set(usb_disc, "0");
		
		return 0;
	}
#endif
#ifdef PBCENR
        else if (!strcmp(base, "wlscan"))
                return wlscan();
#ifdef ROMOVE
        else if (!strcmp(base, "wlevent"))
                return wlevent();
#endif
        else if (!strcmp(base, "ure_monitor")) {
                return ure_monitor();
        }
        else if (!strcmp(base, "dhcpc_apply_delayed")) {
                return dhcpc_apply_delayed();
        }
        else if (!strcmp(base, "pbc_enr_monitor"))
                return pbc_enr_monitor();
#endif

	return EINVAL;
}
