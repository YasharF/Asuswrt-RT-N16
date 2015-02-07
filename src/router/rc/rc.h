/*
 * Router rc control script
 *
 * Copyright 2004, Broadcom Corporation
 * All Rights Reserved.
 * 
 * THIS SOFTWARE IS OFFERED "AS IS", AND BROADCOM GRANTS NO WARRANTIES OF ANY
 * KIND, EXPRESS OR IMPLIED, BY STATUTE, COMMUNICATION OR OTHERWISE. BROADCOM
 * SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A SPECIFIC PURPOSE OR NONINFRINGEMENT CONCERNING THIS SOFTWARE.
 *
 * $Id: rc.h,v 1.1.1.1 2008/07/21 09:17:42 james26_jang Exp $
 */

#ifndef _rc_h_
#define _rc_h_

#include <bcmconfig.h>
#include <netinet/in.h>

#define IFUP (IFF_UP | IFF_RUNNING | IFF_BROADCAST | IFF_MULTICAST)

#define sin_addr(s) (((struct sockaddr_in *)(s))->sin_addr)

#define MAX_NO_BRIDGE 2

#define csprintf(fmt, args...) do{\
        FILE *cp = fopen("/dev/console", "w");\
        if(cp) {\
                fprintf(cp, fmt, ## args);\
                fclose(cp);\
        }\
}while(0)

extern in_addr_t inet_addr_(const char *cp);

/* udhcpc scripts */
extern int udhcpc_main(int argc, char **argv);

/* ppp scripts */
extern int ipup_main(int argc, char **argv);
extern int ipdown_main(int argc, char **argv);
extern int ppp_ifunit(char *ifname);

/* http functions */
extern int http_get(const char *server, char *buf, size_t count, off_t offset);
extern int http_post(const char *server, char *buf, size_t count);
extern int http_stats(const char *url);

/* init */
extern int console_init(void);
extern pid_t run_shell(int timeout, int nowait);
extern void signal_init(void);
extern void fatal_signal(int sig);

/* interface */
extern int ifconfig(char *ifname, int flags, char *addr, char *netmask);
extern int route_add(char *name, int metric, char *dst, char *gateway, char *genmask);
extern int route_del(char *name, int metric, char *dst, char *gateway, char *genmask);
extern void config_loopback(void);
extern int config_vlan(void);

/* network */
extern void start_lan(void);
extern void stop_lan(void);
extern void start_wan(void);
extern void stop_wan(void);
extern void start_wl(void);
extern void wan_up(char *ifname);
extern void wan_down(char *ifname);
extern int hotplug_net(void);
extern int wan_ifunit(char *ifname);
extern int wan_primary_ifunit(void);

/* services */
extern int start_dhcpd(void);
extern int stop_dhcpd(void);
extern int start_dns(void);
extern int stop_dns(void);
extern int start_ntpc(void);
extern int stop_ntpc(void);
extern int start_eapd(void);
extern int stop_eapd(void);
//extern int start_nas(char *type);
extern int start_nas(void);
extern int stop_nas(void);
extern int start_services(void);
extern int stop_services(void);
extern int start_wps(void);
extern int start_telnetd(void);
extern int stop_telnetd(void);

extern void start_wps_proc (int mode);
extern int restart_wps_proc (void);
extern void stop_wps_proc (void);

/* firewall */
#ifdef __CONFIG_NETCONF__
extern int start_firewall(void);
extern int stop_firewall(void);
extern int start_firewall2(char *ifname);
extern int stop_firewall2(char *ifname);
#else
#define start_firewall() do {} while (0)
#define stop_firewall() do {} while (0)
#define start_firewall2(ifname) do {} while (0)
#define stop_firewall2(ifname) do {} while (0)
#endif

/* routes */
extern int preset_wan_routes(char *ifname);

#ifdef BTN_SETUP
enum BTNSETUP_STATE
{
	BTNSETUP_NONE=0,
	BTNSETUP_DETECT,
	BTNSETUP_START,
	BTNSETUP_DATAEXCHANGE,
	BTNSETUP_DATAEXCHANGE_FINISH,
	BTNSETUP_DATAEXCHANGE_EXTEND,
	BTNSETUP_FINISH
};
#endif
#endif /* _rc_h_ */
