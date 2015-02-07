/*
 * Network services
 *
 * Copyright 2004, Broadcom Corporation
 * All Rights Reserved.
 * 
 * THIS SOFTWARE IS OFFERED "AS IS", AND BROADCOM GRANTS NO WARRANTIES OF ANY
 * KIND, EXPRESS OR IMPLIED, BY STATUTE, COMMUNICATION OR OTHERWISE. BROADCOM
 * SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A SPECIFIC PURPOSE OR NONINFRINGEMENT CONCERNING THIS SOFTWARE.
 *
 * $Id: network.c,v 1.6 2008/10/16 09:55:34 james26_jang Exp $
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <syslog.h>															
#include <ctype.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <net/if.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/if_arp.h>
#include <signal.h>
typedef u_int64_t u64;
typedef u_int32_t u32;
typedef u_int16_t u16;
typedef u_int8_t u8;

#include <linux/types.h>
#include <linux/sockios.h>
#include <linux/ethtool.h>
#include <bcmnvram.h>
#include <netconf.h>
#include <shutils.h>
#include <wlutils.h>
#include <nvparse.h>
#include <rc.h>
#include <bcmutils.h>
#include <etioctl.h>
#include <bcmparams.h>

#ifdef __CONFIG_EMF__
bool emf_enabled = FALSE;
#endif

extern int qos_get_wan_rate(void);

/*
 * Carry out a socket request including openning and closing the socket
 * Return -1 if failed to open socket (and perror); otherwise return
 * result of ioctl
 */
static int
soc_req(char *name, int action, struct ifreq *ifr)
{
	int s;
	int rv = 0;

	if ((s = socket(AF_INET, SOCK_RAW, IPPROTO_RAW)) < 0) {
		perror("socket");
		return -1;
	}
	strncpy(ifr->ifr_name, name, IFNAMSIZ);
	rv = ioctl(s, action, ifr);
	close(s);

	return rv;
}

/* Check NVRam to see if "name" is explicitly enabled */
static inline int
wl_vif_enabled(const char *name, char *tmp)
{
        return (atoi(nvram_safe_get(strcat_r(name,"_bss_enabled",tmp))));
}

/* Set the HW address for interface "name" if present in NVRam */
static void
wl_vif_hwaddr_set(const char *name)
{
	int rc;
	char *ea;
	char hwaddr[20];
	struct ifreq ifr;

	snprintf(hwaddr, sizeof(hwaddr), "%s_hwaddr", name);
	ea = nvram_get(hwaddr);
	if (ea == NULL) {
		fprintf(stderr, "NET: No hw addr found for %s\n", name);
		return;
	}

	fprintf(stderr, "NET: Setting %s hw addr to %s\n", name, ea);
	ifr.ifr_hwaddr.sa_family = ARPHRD_ETHER;
	ether_atoe(ea, (unsigned char *)ifr.ifr_hwaddr.sa_data);
	if ((rc = soc_req((char *)name, SIOCSIFHWADDR, &ifr)) < 0) {
		fprintf(stderr, "NET: Error setting hw for %s; returned %d\n", name, rc);
	}
}

#ifdef __CONFIG_EMF__
void
emf_mfdb_update(char *lan_ifname, char *lan_port_ifname, bool add)
{
        char word[256], *next;
        char *mgrp, *ifname;

        /* Add/Delete MFDB entries corresponding to new interface */
        foreach (word, nvram_safe_get("emf_entry"), next) {
                ifname = word;
                mgrp = strsep(&ifname, ":");

                if ((mgrp == 0) || (ifname == 0))
                        continue;

                /* Add/Delete MFDB entry using the group addr and interface */
                if (strcmp(lan_port_ifname, ifname) == 0) {
                        eval("emf", ((add) ? "add" : "del"),
                             "mfdb", lan_ifname, mgrp, ifname);
                }
        }

        return;
}

void
emf_uffp_update(char *lan_ifname, char *lan_port_ifname, bool add)
{
        char word[256], *next;
        char *ifname;

        /* Add/Delete UFFP entries corresponding to new interface */
        foreach (word, nvram_safe_get("emf_uffp_entry"), next) {
                ifname = word;

                if (ifname == 0)
                        continue;

                /* Add/Delete UFFP entry for the interface */
                if (strcmp(lan_port_ifname, ifname) == 0) {
                        eval("emf", ((add) ? "add" : "del"),
                             "uffp", lan_ifname, ifname);
                }
        }

        return;
}

void
emf_rtport_update(char *lan_ifname, char *lan_port_ifname, bool add)
{
        char word[256], *next;
        char *ifname;

        /* Add/Delete RTPORT entries corresponding to new interface */
        foreach (word, nvram_safe_get("emf_rtport_entry"), next) {
                ifname = word;

                if (ifname == 0)
                        continue;

                /* Add/Delete RTPORT entry for the interface */
                if (strcmp(lan_port_ifname, ifname) == 0) {
                        eval("emf", ((add) ? "add" : "del"),
                             "rtport", lan_ifname, ifname);
                }
        }

        return;
}

void
start_emf(char *lan_ifname)
{
        char word[256], *next;
        char *mgrp, *ifname;

        if (!nvram_match("emf_enable", "1"))
                return;

        /* Start EMF */
        eval("emf", "start", lan_ifname);

        /* Add the static MFDB entries */
        foreach (word, nvram_safe_get("emf_entry"), next) {
                ifname = word;
                mgrp = strsep(&ifname, ":");

                if ((mgrp == 0) || (ifname == 0))
                        continue;

                /* Add MFDB entry using the group addr and interface */
                eval("emf", "add", "mfdb", lan_ifname, mgrp, ifname);
        }

        /* Add the UFFP entries */
        foreach (word, nvram_safe_get("emf_uffp_entry"), next) {
                ifname = word;
                if (ifname == 0)
                        continue;

                /* Add UFFP entry for the interface */
                eval("emf", "add", "uffp", lan_ifname, ifname);
        }

        /* Add the RTPORT entries */
        foreach (word, nvram_safe_get("emf_rtport_entry"), next) {
                ifname = word;
                if (ifname == 0)
                        continue;

                /* Add RTPORT entry for the interface */
                eval("emf", "add", "rtport", lan_ifname, ifname);
        }

        return;
}

void
load_emf(void)
{
        /* Load the EMF & IGMP Snooper modules */
        eval("insmod", "emf");
        eval("insmod", "igs");

        emf_enabled = TRUE;

        return;
}

void
unload_emf(void)
{
        if (!emf_enabled)
                return;

        /* Unload the EMF & IGMP Snooper modules */
        eval("rmmod", "igs");
        eval("rmmod", "emf");

        emf_enabled = FALSE;

        return;
}
#endif /* __CONFIG_EMF__ */


static int
add_routes(char *prefix, char *var, char *ifname)
{
	char word[80], *next;
	char *ipaddr, *netmask, *gateway, *metric;
	char tmp[100];

	foreach(word, nvram_safe_get(strcat_r(prefix, var, tmp)), next) {

		netmask = word;
		ipaddr = strsep(&netmask, ":");
		if (!ipaddr || !netmask)
			continue;
		gateway = netmask;
		netmask = strsep(&gateway, ":");
		if (!netmask || !gateway)
			continue;
		metric = gateway;
		gateway = strsep(&metric, ":");
		if (!gateway || !metric)
			continue;
		if (inet_addr_(gateway) == INADDR_ANY)
	                gateway = nvram_safe_get("wanx_gateway");


		dprintf("\n\n\nadd %s %d %s %s %s\n\n\n", ifname, atoi(metric), ipaddr, gateway, netmask);
		
		route_add(ifname, atoi(metric) + 1, ipaddr, gateway, netmask);
	}

	return 0;
}

add_wanx_routes(char *prefix, char *ifname, int metric)
{
        char *routes, *msroutes, *tmp;
        char buf[30];
                                                                                                                  
        char ipaddr[] = "255.255.255.255";
        char gateway[] = "255.255.255.255";
        char netmask[] = "255.255.255.255";
                                                                                                                  
        if (!nvram_match("dr_enable_x", "1"))
                return;
                                                                                                                  
        /* routes */
        routes = strdup(nvram_safe_get(strcat_r(prefix, "routes", buf)));
        for (tmp = routes; tmp && *tmp; )
        {
                char *ipaddr = strsep(&tmp, " ");
                char *gateway = strsep(&tmp, " ");
                if (gateway) {
                        route_add(ifname, metric + 1, ipaddr, gateway, netmask);
                }
        }
        free(routes);
                                                                                                                  
        /* ms routes */
        for (msroutes = nvram_get(strcat_r(prefix, "msroutes", buf)); msroutes && isdigit(*msroutes); )
        {

		//printf("dbg: msroutes = \"%s\".\n", msroutes);

                /* read net length */
                int bit, bits = strtol(msroutes, &msroutes, 10);
                struct in_addr ip, gw, mask;
                                                                                                                  
                if (bits < 1 || bits > 32 || *msroutes != ' ')
                        break;
                                                                                                                  
                mask.s_addr = htonl(0xffffffff << (32 - bits));
                                                                                                                  
                /* read network address */
                for (ip.s_addr = 0, bit = 24; bit > (24 - bits); bit -= 8)
                {
                        if (*msroutes++ != ' ' || !isdigit(*msroutes))
                                goto bad_data;
                                                                                                                  
                        ip.s_addr |= htonl(strtol(msroutes, &msroutes, 10) << bit);
                }
                                                                                                                  
                /* read gateway */
                for (gw.s_addr = 0, bit = 24; bit >= 0 && *msroutes; bit -= 8)
                {
                        if (*msroutes++ != ' ' || !isdigit(*msroutes))
                                goto bad_data;
                                                                                                                  
                        gw.s_addr |= htonl(strtol(msroutes, &msroutes, 10) << bit);
                }
                                                                                                                  
                /* clear bits per RFC */
                ip.s_addr &= mask.s_addr;
                                                                                                                  
                strcpy(ipaddr, inet_ntoa(ip));
                strcpy(gateway, inet_ntoa(gw));
                strcpy(netmask, inet_ntoa(mask));
                                                                                                                  
                route_add(ifname, metric + 1, ipaddr, gateway, netmask);
                                                                                                                  
                if (*msroutes == ' ')
                        msroutes++;
        }
bad_data:
        return;
}

static int
del_routes(char *prefix, char *var, char *ifname)
{
	char word[80], *next;
	char *ipaddr, *netmask, *gateway, *metric;
	char tmp[100];
	
	foreach(word, nvram_safe_get(strcat_r(prefix, var, tmp)), next) {
		dprintf("add %s\n", word);
		
		netmask = word;
		ipaddr = strsep(&netmask, ":");
		if (!ipaddr || !netmask)
			continue;
		gateway = netmask;
		netmask = strsep(&gateway, ":");
		if (!netmask || !gateway)
			continue;
		metric = gateway;
		gateway = strsep(&metric, ":");
		if (!gateway || !metric)
			continue;

                if (inet_addr_(gateway) == INADDR_ANY)
                        gateway = nvram_safe_get("wanx_gateway");
		
		dprintf("add %s\n", ifname);
		
		route_del(ifname, atoi(metric) + 1, ipaddr, gateway, netmask);
	}

	return 0;
}

static int
add_lan_routes(char *lan_ifname)
{
	return add_routes("lan_", "route", lan_ifname);
}

static int
del_lan_routes(char *lan_ifname)
{
	return del_routes("lan_", "route", lan_ifname);
}

static void
start_igmpproxy(char *wan_ifname)
{
        static char *igmpproxy_conf = "/tmp/igmpproxy.conf";
        struct stat     st_buf;
        FILE            *fp;

	stop_igmp_proxy();

	if (atoi(nvram_safe_get("udpxy_enable_x")))
		eval("/usr/sbin/udpxy", "-a", nvram_get("lan_ifname") ? : "br0",
			"-m", wan_ifname, "-p", nvram_get("udpxy_enable_x"));

        if (!nvram_match("mr_enable_x", "1"))
                return;
                                                                                                                  
        if ((fp = fopen(igmpproxy_conf, "w")) == NULL) {
                perror(igmpproxy_conf);
                return;
        }
                                                                                                                  
        fprintf(fp, "# automagically generated from web settings\n"
                "quickleave\n\n"
                "phyint %s upstream\n"
                "\taltnet %s\n\n"
                "phyint %s downstream\n\n",
                wan_ifname,
                nvram_get("mr_altnet_x") ? : "0.0.0.0/0",
                nvram_get("lan_ifname") ? : "br0");
                                                                                                                  
        fclose(fp);
                                                                                                                  
        eval("/usr/sbin/igmpproxy", "-c", igmpproxy_conf);

	start_igmp_proxy();
}

void
start_lan(void)
{
	char *lan_ifname = nvram_safe_get("lan_ifname");
//	char br0_ifnames[255];
	char name[80], *next;
	char tmp[100];
//	char tmpstr[48];
//	int i, j;
	int s;
	struct ifreq ifr;
	char hwaddr[ETHER_ADDR_LEN];

	dprintf("%s\n", lan_ifname);

#ifdef GUEST_ACCOUNT
	memset(br0_ifnames,0,sizeof(br0_ifnames));
	
	nvram_unset("unbridged_ifnames");
	nvram_unset("br0_ifnames");

	/* If we're a travel router... then we need to make sure we get
		 the primary wireless interface up before trying to attach slave
		 interface(s) to the bridge */
#endif

#ifdef URE
	if(nvram_match("ure_disable", "0"))
	{
		eval("wlconf", nvram_get("wan0_ifname"), "up");
	}
#endif

 	/* Bring up bridged interfaces */
	if (strncmp(lan_ifname, "br", 2) == 0) {
		eval("brctl", "addbr", lan_ifname);
		eval("brctl", "setfd", lan_ifname, "0");
		if (nvram_match("router_disable", "1") || nvram_match("lan_stp", "0"))
			eval("brctl", "stp", lan_ifname, "off");
		else
			eval("brctl", "stp", lan_ifname, "on");

#ifdef __CONFIG_EMF__
                        if (nvram_match("emf_enable", "1")) {
                                eval("emf", "add", "bridge", lan_ifname);
                                eval("igs", "add", "bridge", lan_ifname);
                        }
#endif /* __CONFIG_EMF__ */

		memset(hwaddr, 0, sizeof(hwaddr));
#ifdef ASUS_EXT
		foreach(name, nvram_safe_get("lan_ifnames_t"), next) {
#else
		foreach(name, nvram_safe_get("lan_ifnames"), next) {
#endif
			if (strncmp(name, "wl", 2) == 0) {
				if (!wl_vif_enabled(name, tmp)) {
					continue; /* Ignore disabled WL VIF */
				}
				wl_vif_hwaddr_set(name);
			}

			/* Bring up interface. Ignore any bogus/unknown interfaces on the NVRAM list */
			// 2280
			if (ifconfig(name, IFUP | IFF_ALLMULTI, NULL, NULL)){
				perror("ifconfig");
			} else {
			/* Set the logical bridge address to that of the first interface */
			if ((s = socket(AF_INET, SOCK_RAW, IPPROTO_RAW)) < 0)
				continue;
			strncpy(ifr.ifr_name, lan_ifname, IFNAMSIZ);
			if (ioctl(s, SIOCGIFHWADDR, &ifr) == 0 &&
			    memcmp(ifr.ifr_hwaddr.sa_data, "\0\0\0\0\0\0", ETHER_ADDR_LEN) == 0) {
				strncpy(ifr.ifr_name, name, IFNAMSIZ);
				if (ioctl(s, SIOCGIFHWADDR, &ifr) == 0) {
					strncpy(ifr.ifr_name, lan_ifname, IFNAMSIZ);
					ifr.ifr_hwaddr.sa_family = ARPHRD_ETHER;
					ioctl(s, SIOCSIFHWADDR, &ifr);

					memcpy(hwaddr, ifr.ifr_hwaddr.sa_data, ETHER_ADDR_LEN);
				}
			}
			close(s);

			/* If not a wl i/f then simply add it to the bridge */
			if (eval("wlconf", name, "up"))
			{
#ifdef GUEST_ACCOUNT
				{
					if (eval("brctl", "addif", lan_ifname, name))
						perror("brctl");
					else{
						char buf[255],*ptr;
						ptr = nvram_get("br0_ifnames");
						if (ptr)
							snprintf(buf,sizeof(buf),"%s %s",ptr,name);
						else
							strncpy(buf,name,sizeof(buf));
						nvram_set("br0_ifnames",buf);
					}
				}
#else
				eval("brctl", "addif", lan_ifname, name);
#endif

#ifdef __CONFIG_EMF__
                                                if (nvram_match("emf_enable", "1"))
                                                        eval("emf", "add", "iface", lan_ifname, name);
#endif /* __CONFIG_EMF__ */

			}
			else 
			{
#ifdef GUEST_ACCOUNT
				char wl_guest[] = "wlXXXXXXXXXX_guest";
				char wl_vifs[]= "wlXXXXXXXXXX_vifs";
				char mode[] = "wlXXXXXXXXXX_mode";
				char *vifs;
#endif

				/* get the instance number of the wl i/f */
				char mode[] = "wlXXXXXXXXXX_mode";
				int unit;
#ifdef ASUS_EXT
				sync_mac(name, nvram_safe_get("et0macaddr"));
#endif
				/* get the instance number of the wl i/f */
				wl_ioctl(name, WLC_GET_INSTANCE, &unit, sizeof(unit));
				snprintf(mode, sizeof(mode), "wl%d_mode", unit);

#ifdef GUEST_ACCOUNT
				snprintf(wl_guest, sizeof(wl_guest), "wl%d_guest", unit);
				snprintf(wl_vifs, sizeof(wl_vifs), "wl%d_vifs", unit);
					
				/* Multi-SSID specific configuration */
				/* Virtual interfaces are created with  the master interface 
				   by wlconf() . Only copy those that have wlX.Y_guest set*/
					   
				vifs = nvram_get(wl_vifs);
					
				if (vifs){
					char buf[255];
					char name[32];
					char vif_guest[32];
					char *ptr=NULL,*next=NULL;
						
					memset(buf,0,sizeof(buf));
						
					ptr = nvram_get("unbridged_ifnames");
						
					if (ptr) snprintf(buf,sizeof(buf),"%s",ptr);
						
					/*Loop thru wlX_vifs to get the virtual interfaces
				          wlX.Y_guest must be set for IP to configure it*/
					foreach(name,vifs,next){
						snprintf(vif_guest,sizeof(vif_guest),"%s_guest",name);
						cprintf("vif_guest=%s\n", vif_guest);
						if (nvram_match(vif_guest,"1")){
							if(*buf)
								snprintf(buf,sizeof(buf),"%s %s",buf,name);
							else
								strncpy(buf,name,sizeof(buf));
						} 
					}

					if (*buf) nvram_set("unbridged_ifnames",buf);
				}
#endif
				/* Receive all multicast frames in WET mode */
				if (nvram_match(mode, "wet"))
				{
					/* Receive all multicast frames in WET mode */
					ifconfig(name, IFUP | IFF_ALLMULTI, NULL, NULL);

//#ifdef GUEST_ACCOUNT
					/* Enable host DHCP relay */
//					if (nvram_match("lan_dhcp", "1"))
					if (nvram_match("sw_mode_ex", "2"))
						wl_iovar_set(name, "wet_host_mac", ifr.ifr_hwaddr.sa_data, ETHER_ADDR_LEN);
//#endif
				}

				/* Do not attach the main wl i/f if in wds mode */
#ifdef GUEST_ACCOUNT
				if ( !nvram_match(mode, "wds") && !nvram_match(wl_guest, "1")){
					char buf[255],*ptr;
					
					eval("brctl", "addif", lan_ifname, name);
					
					ptr = nvram_get("br0_ifnames");
					if (ptr)
						snprintf(buf,sizeof(buf),"%s %s",ptr,name);
					else
						strncpy(buf,name,sizeof(buf));
					nvram_set("br0_ifnames",buf);
				}
#else
				if (nvram_invmatch(mode, "wds"))
					eval("brctl", "addif",lan_ifname,name);
#ifdef __CONFIG_EMF__
                                                if (nvram_match("emf_enable", "1"))
                                                        eval("emf", "add", "iface", lan_ifname, name);
#endif /* __CONFIG_EMF__ */


#endif
			} /*if (eval("wlconf", na.....*/
			} /* if (ifconfig(name,...*/
		} /* foreach().... */

		if (memcmp(hwaddr, "\0\0\0\0\0\0", ETHER_ADDR_LEN) &&
		    (s = socket(AF_INET, SOCK_RAW, IPPROTO_RAW)) >= 0) {
			strncpy(ifr.ifr_name, lan_ifname, IFNAMSIZ);
			ifr.ifr_hwaddr.sa_family = ARPHRD_ETHER;
			memcpy(ifr.ifr_hwaddr.sa_data, hwaddr, ETHER_ADDR_LEN);
			ioctl(s, SIOCSIFHWADDR, &ifr);
			close(s);
		}
	}
	/* specific non-bridged lan i/f */
	else if (strcmp(lan_ifname, "")) {
		/* Bring up interface */
		ifconfig(lan_ifname, IFUP, NULL, NULL);
		/* config wireless i/f */
		eval("wlconf", lan_ifname, "up");
	}

#ifdef GUEST_ACCOUNT
	/* build unbridged ifnames NVRAM var from wl_guest list */
	foreach(name, nvram_safe_get("lan_ifnames"), next) {
	
		char wl_guest[] = "wlXXXXXXXXXX_guest";
		int unit;
		
		if (!eval("wlconf", name, "up")) {
			wl_ioctl(name, WLC_GET_INSTANCE, &unit, sizeof(unit));
			snprintf(wl_guest, sizeof(wl_guest), "wl%d_guest", unit);
			
			if (nvram_match(wl_guest,"1")){
				char buf[255],*ptr;
				
				ptr = nvram_get("unbridged_ifnames");
				if (ptr)
					snprintf(buf,sizeof(buf),"%s %s",ptr,name);
				else
					strncpy(buf,name,sizeof(buf));
					
				nvram_set("unbridged_ifnames",buf);
			}
		}
	}
#endif
	/* Get current LAN hardware address */
	if ((s = socket(AF_INET, SOCK_RAW, IPPROTO_RAW)) >= 0) {
		char eabuf[32];
		strncpy(ifr.ifr_name, lan_ifname, IFNAMSIZ);
		if (ioctl(s, SIOCGIFHWADDR, &ifr) == 0)
			nvram_set("lan_hwaddr", ether_etoa(ifr.ifr_hwaddr.sa_data, eabuf));
		close(s);
	}

#ifdef WPA2_WMM
	/* Set QoS mode */
	if ((s = socket(AF_INET, SOCK_RAW, IPPROTO_RAW)) >= 0) {
		int i, qos;
		caddr_t ifrdata;
		struct ethtool_drvinfo info;

		qos = (strcmp(nvram_safe_get("wl_wme"), "on")) ? 0 : 1;
		for (i = 1; i <= DEV_NUMIFS; i ++) {
			ifr.ifr_ifindex = i;
			if (ioctl(s, SIOCGIFNAME, &ifr))
				continue;
			if (ioctl(s, SIOCGIFHWADDR, &ifr))
				continue;
			if (ifr.ifr_hwaddr.sa_family != ARPHRD_ETHER)
				continue;
			/* get flags */
			if (ioctl(s, SIOCGIFFLAGS, &ifr))
				continue;
			/* if up(wan not up yet at this point) */
			if (ifr.ifr_flags & IFF_UP) {
				ifrdata = ifr.ifr_data;
				memset(&info, 0, sizeof(info));
				info.cmd = ETHTOOL_GDRVINFO;
				ifr.ifr_data = (caddr_t)&info;
				if (ioctl(s, SIOCETHTOOL, &ifr) >= 0) {
					/* currently only need to set QoS to et devices */
					if (!strncmp(info.driver, "et", 2)) {
						ifr.ifr_data = (caddr_t)&qos;
						ioctl(s, SIOCSETCQOS, &ifr);
					}
				}
				ifr.ifr_data = ifrdata;
			}
		}
	}
	close(s);
#endif

#ifdef ASUS_EXT
	/* 
	* Configure DHCP connection. The DHCP client will run 
	* 'udhcpc bound'/'udhcpc deconfig' upon finishing IP address 
	* renew and release.
	*/
	if (nvram_match("router_disable", "1"))
	{
                if (nvram_match("sw_mode_ex", "2"))	// Jiahao for RT-N12
                {
                        nvram_unset("lan_ipaddr_new");
                        nvram_unset("lan_netmask_new");
                        nvram_unset("lan_gateway_new");
                        nvram_unset("lan_dns_new");
                        nvram_unset("lan_wins_new");
                        nvram_unset("lan_domain_new");
                        nvram_unset("lan_lease_new");
                        nvram_unset("lan_ifname_new");
                        nvram_unset("lan_udhcpstate_new");

                        nvram_unset("lan_ipaddr_now");
                        nvram_unset("lan_netmask_now");
                        nvram_unset("lan_gateway_now");
                        nvram_unset("lan_dns_now");
                        nvram_unset("lan_wins_now");
                        nvram_unset("lan_domain_now");
                        nvram_unset("lan_lease_now");
                }

		if (nvram_match("lan_proto_x", "1")) 
		{
			char *dhcp_argv[] = { "udhcpc",
					      "-i", lan_ifname,
					      "-p", "/var/run/udhcpc_lan.pid",
					      "-s", "/tmp/landhcpc",
					      NULL
			};
			pid_t pid;


			/* Bring up and configure LAN interface */
			ifconfig(lan_ifname, IFUP,
		 		nvram_safe_get("lan_ipaddr"), nvram_safe_get("lan_netmask"));

			symlink("/sbin/rc", "/tmp/landhcpc");

			/* Start dhcp daemon */
			_eval(dhcp_argv, NULL, 0, &pid);
		}
		else
		{
			/* Bring up and configure LAN interface */
			ifconfig(lan_ifname, IFUP,
		 		nvram_safe_get("lan_ipaddr"), nvram_safe_get("lan_netmask"));
			lan_up(lan_ifname);

			update_lan_status(1);
		}
	}
	else
	{
		/* Bring up and configure LAN interface */
		ifconfig(lan_ifname, IFUP,
		 	nvram_safe_get("lan_ipaddr"), nvram_safe_get("lan_netmask"));
		/* Install lan specific static routes */
		add_lan_routes(lan_ifname);

		update_lan_status(1);
	}
#else
	/* Bring up and configure LAN interface */
	ifconfig(lan_ifname, IFUP,
		 nvram_safe_get("lan_ipaddr"), nvram_safe_get("lan_netmask"));

	/* Install lan specific static routes */
	add_lan_routes(lan_ifname);
#endif

#ifdef __CONFIG_EMF__
                /* Start the EMF for this LAN */
                start_emf(lan_ifname);
#endif /* __CONFIG_EMF__ */


#ifdef GUEST_ACCOUNT
	/* Bring up unbridged LAN interfaces (if they exist)*/
	{
	char *interfaces,*ifname, *ptr;
	char word[64], *next;
	int index =1 ;
	
	interfaces = nvram_get("unbridged_ifnames");
	if (interfaces)
		foreach(word,interfaces,next){
		
		 	char interface[32], mask[32];
		 	int s;
		 
		 	ptr=word;
			ifname = word;
			index =  get_ipconfig_index(ifname);
			
			if ( index < 0) {
				cprintf("Cannot find index for interface:%s\n",ifname);
				continue;
			}

			snprintf(interface,sizeof(interface),"lan%d_ipaddr",index);
		 	snprintf(mask,sizeof(mask),"lan%d_netmask",index);
		 	ifconfig(ifname, IFUP,nvram_safe_get(interface), nvram_safe_get(mask)); 
		 
		 	/* Get Ethernet hardware address. Note this value NOT is committed to NVRAM */
		 	if ((s = socket(AF_INET, SOCK_RAW, IPPROTO_RAW)) >= 0) {
		 		struct ifreq ifr;
				char macaddr[]="00:00:00:00:00:00";
				char mac[32];
				
				memset(&ifr,0,sizeof(ifr));
				strncpy(ifr.ifr_name, ifname, IFNAMSIZ);
				if (ioctl(s, SIOCGIFHWADDR, &ifr) == 0){
					ether_etoa(ifr.ifr_hwaddr.sa_data, macaddr);
					snprintf(mac,sizeof(mac),"lan%d_hwaddr",index);	
					nvram_set(mac,macaddr);
				}
				close(s);
			}
		 
		}
	}
#endif

#ifndef ASUS_EXT
	/* Start syslogd if either log_ipaddr or log_ram_enable is set */
	if (nvram_invmatch("log_ipaddr", "") || nvram_match("log_ram_enable", "1")) {
		char *argv[] = {
			"syslogd",
			NULL, 		/* -C */
			NULL, NULL,	/* -R host */
			NULL
		};
		int pid;
		int argc = 1;
		
		if (nvram_match("log_ram_enable", "1")) {
			argv[argc++] = "-C";
		}
		else if (!nvram_match("log_ram_enable", "0")) {
			nvram_set("log_ram_enable", "0");
		}
				
		if (nvram_invmatch("log_ipaddr", "")) {
			argv[argc++] = "-R";
			argv[argc++] = nvram_get("log_ipaddr");
		}

		_eval(argv, NULL, 0, &pid);
	}
#endif

	dprintf("%s %s\n",
		nvram_safe_get("lan_ipaddr"),
		nvram_safe_get("lan_netmask"));
}

void
stop_lan(void)
{
	char *lan_ifname = nvram_safe_get("lan_ifname");
	char name[80], *next;

	dprintf("%s\n", lan_ifname);

	/* Stop the syslogd daemon */
	eval("killall", "syslogd");

	/* Remove static routes */
	del_lan_routes(lan_ifname);

	/* Bring down LAN interface */
	ifconfig(lan_ifname, 0, NULL, NULL);

	/* Bring down bridged interfaces */
	if (strncmp(lan_ifname, "br", 2) == 0) {
#ifdef ASUS_EXT
		foreach(name, nvram_safe_get("lan_ifnames_t"), next) {
#else
		foreach(name, nvram_safe_get("lan_ifnames"), next) {
#endif
			eval("wlconf", name, "down");
			ifconfig(name, 0, NULL, NULL);
			eval("brctl", "delif", lan_ifname, name);
		}
		eval("brctl", "delbr", lan_ifname);
	}
	/* Bring down specific interface */
	else if (strcmp(lan_ifname, ""))
		eval("wlconf", lan_ifname, "down");
	
	dprintf("done\n");
}

static int
wan_prefix(char *ifname, char *prefix)
{
	int unit;
	
	if ((unit = wan_ifunit(ifname)) < 0)
		return -1;

	sprintf(prefix, "wan%d_", unit);
	return 0;
}

static int
add_wan_routes(char *wan_ifname)
{
	char prefix[] = "wanXXXXXXXXXX_";

	/* Figure out nvram variable name prefix for this i/f */
	if (wan_prefix(wan_ifname, prefix) < 0)
		return -1;

	return add_routes(prefix, "route", wan_ifname);
}

static int
del_wan_routes(char *wan_ifname)
{
	char prefix[] = "wanXXXXXXXXXX_";

	/* Figure out nvram variable name prefix for this i/f */
	if (wan_prefix(wan_ifname, prefix) < 0)
		return -1;

	return del_routes(prefix, "route", wan_ifname);
}



int
wan_valid(char *ifname)
{
	char name[80], *next;
	
	foreach(name, nvram_safe_get("wan_ifnames"), next)
		if (ifname && !strcmp(ifname, name))
			return 1;

	return 0;
}

void
start_wan(void)
{
	char *wan_ifname;
	char *wan_proto;
	int unit;
	char tmp[100], prefix[] = "wanXXXXXXXXXX_";
	char eabuf[32];
	int s;
	struct ifreq ifr;
	pid_t pid;

	/* check if we need to setup WAN */
	if (nvram_match("router_disable", "1")
#ifdef BTN_SETUP
//			|| is_ots()/* Cherry Cho unmarked in 2007/2/13. */
#endif
	)
		return;

#ifdef ASUS_EXT
	update_wan_status(0);
#endif
	
	/* start connection independent firewall */
	start_firewall();

// 2008.08 James. If wan port wasn't plugged, the filter also can be used. {
	char logaccept[16], logdrop[16], wan_if[8];
	
	memset(logaccept, 0, 16);
	memset(logdrop, 0, 16);
	memset(wan_if, 0, 8);
	
	/* Determine the log type */
	if(nvram_match("fw_log_x", "accept") || nvram_match("fw_log_x", "both"))
		strcpy(logaccept, "logaccept");
	else
		strcpy(logaccept, "ACCEPT");

	if(nvram_match("fw_log_x", "drop") || nvram_match("fw_log_x", "both"))
		strcpy(logdrop, "logdrop");
	else
		strcpy(logdrop, "DROP");

	if(nvram_match("wan_proto", "pppoe")
			|| nvram_match("wan_proto", "pptp")
			|| nvram_match("wan_proto", "l2tp"))
		strcpy(wan_if, "ppp0");
	else
		strcpy(wan_if, "eth0");

	/* Filter setting */
	filter_setting(wan_if, NULL, "br0", NULL, logaccept, logdrop);
// 2008.08 James. }

	/* Create links */
	mkdir("/tmp/ppp", 0777);
//	mkdir("/tmp/ppp/peers", 0777);
	symlink("/sbin/rc", "/tmp/ppp/ip-up");
	symlink("/sbin/rc", "/tmp/ppp/ip-down");	
	symlink("/sbin/rc", "/tmp/udhcpc");

	//symlink("/dev/null", "/tmp/ppp/connect-errors");

	/* Start each configured and enabled wan connection and its undelying i/f */
	for (unit = 0; unit < MAX_NVPARSE; unit ++) 
	{
#ifdef ASUS_EXT // Only multiple pppoe is allowed 
		if (unit>0 && nvram_invmatch("wan_proto", "pppoe")) break;
#endif

		snprintf(prefix, sizeof(prefix), "wan%d_", unit);

		/* make sure the connection exists and is enabled */ 
		wan_ifname = nvram_get(strcat_r(prefix, "ifname", tmp));
		if (!wan_ifname)
			continue;
		wan_proto = nvram_get(strcat_r(prefix, "proto", tmp));
		if (!wan_proto || !strcmp(wan_proto, "disabled"))
			continue;

		/* disable the connection if the i/f is not in wan_ifnames */
		if (!wan_valid(wan_ifname)) {
			nvram_set(strcat_r(prefix, "proto", tmp), "disabled");
			continue;
		}

		dprintf("%s %s\n\n\n\n\n", wan_ifname, wan_proto);

		/* Set i/f hardware address before bringing it up */
		if ((s = socket(AF_INET, SOCK_RAW, IPPROTO_RAW)) < 0)
			continue;

		strncpy(ifr.ifr_name, wan_ifname, IFNAMSIZ);

		/* Since WAN interface may be already turned up (by vlan.c),
		   if WAN hardware address is specified (and different than the current one),
		   we need to make it down for synchronizing hwaddr. */
		if (ioctl(s, SIOCGIFHWADDR, &ifr)) {
			close(s);
			continue;
		}

		ether_atoe(nvram_safe_get(strcat_r(prefix, "hwaddr", tmp)), eabuf);
		if (bcmp(eabuf, ifr.ifr_hwaddr.sa_data, ETHER_ADDR_LEN))
		{
			/* current hardware address is different than user specified */
			ifconfig(wan_ifname, 0, NULL, NULL);
		}

		/* Configure i/f only once, specially for wireless i/f shared by multiple connections */
		if (ioctl(s, SIOCGIFFLAGS, &ifr)) {
			close(s);
			continue;
		}
		if (!(ifr.ifr_flags & IFF_UP)) {
			/* Sync connection nvram address and i/f hardware address */
			memset(ifr.ifr_hwaddr.sa_data, 0, ETHER_ADDR_LEN);

			if (!nvram_invmatch(strcat_r(prefix, "hwaddr", tmp), "") ||
			    !ether_atoe(nvram_safe_get(strcat_r(prefix, "hwaddr", tmp)), ifr.ifr_hwaddr.sa_data) ||
			    !memcmp(ifr.ifr_hwaddr.sa_data, "\0\0\0\0\0\0", ETHER_ADDR_LEN)) {
				if (ioctl(s, SIOCGIFHWADDR, &ifr)) {
					close(s);
					continue;
				}
				nvram_set(strcat_r(prefix, "hwaddr", tmp), ether_etoa(ifr.ifr_hwaddr.sa_data, eabuf));
			}
			else {
				ifr.ifr_hwaddr.sa_family = ARPHRD_ETHER;
				ioctl(s, SIOCSIFHWADDR, &ifr);
			}

			/* Bring up i/f */
			ifconfig(wan_ifname, IFUP, NULL, NULL);

#ifdef URE			
			/* do wireless specific config */
			if (nvram_match("ure_disable", "1")) {
				eval("wlconf", wan_ifname, "up");
				eval("wlconf", wan_ifname, "start");
			}
#endif
		}
	
		close(s);

#ifdef ASUS_EXT
		if (unit==0) 
		{		
			FILE *fp;

			setup_ethernet(nvram_safe_get("wan_ifname"));
			start_pppoe_relay(wan_ifname);

			/* Enable Forwarding */
			if ((fp = fopen("/proc/sys/net/ipv4/ip_forward", "r+"))) 			{
				fputc('1', fp);
				fclose(fp);
			} else
			{	
				perror("/proc/sys/net/ipv4/ip_forward");
			}
		}

		/* 
		* Configure PPPoE connection. The PPPoE client will run 
		* ip-up/ip-down scripts upon link's connect/disconnect.
		*/
#ifdef CDMA
		if (strcmp(wan_proto, "cdma")==0)
        	{
			if(nvram_match("cdma_down", "99"))
			{
				nvram_set("cdma_down", "1");
			}
                	else 
			{
				nvram_set("cdma_down", "0");
				start_cdma();
			}
                	update_wan_status(0);
        	}
		else
#endif
		if (strcmp(wan_proto, "pppoe") == 0 || strcmp(wan_proto, "pptp") == 0 ||
				strcmp(wan_proto, "l2tp") == 0)
		{
			int demand = atoi(nvram_safe_get(strcat_r(prefix, "pppoe_idletime", tmp))) &&
					strcmp(wan_proto, "l2tp") /* L2TP does not support idling */ ;

			/* update demand option */
			nvram_set(strcat_r(prefix, "pppoe_demand", tmp), demand ? "1" : "0");

			/* Bring up  WAN interface */
			ifconfig(wan_ifname, IFUP,
					nvram_get(strcat_r(prefix, "pppoe_ipaddr", tmp)),
					nvram_get(strcat_r(prefix, "pppoe_netmask", tmp)));

			/* launch dhcp client and wait for lease forawhile */
			if (nvram_match(strcat_r(prefix, "pppoe_ipaddr", tmp), "0.0.0.0"))
			{
printf("*** when pppoe_ipaddr=0.0.0.0. ***\n");
				char *wan_hostname = nvram_get(strcat_r(prefix, "hostname", tmp));
				char *dhcp_argv[] = { "udhcpc",
									"-i", wan_ifname,
									"-p", (sprintf(tmp, "/var/run/udhcpc%d.pid", unit), tmp),
									"-s", "/tmp/udhcpc",
									"-b",
									wan_hostname && *wan_hostname ? "-H" : NULL,
									wan_hostname && *wan_hostname ? wan_hostname : NULL,
									NULL
									};
				/* Start dhcp daemon */
				_eval(dhcp_argv, NULL, 0, NULL);
			}
			else {
				/* start firewall */
				start_firewall_ex(nvram_safe_get(strcat_r(prefix, "pppoe_ifname", tmp)),
						"0.0.0.0", "br0", nvram_safe_get("lan_ipaddr"));

				/* setup static wan routes via physical device */
				add_routes("wan_", "route", wan_ifname);
				/* and set default route if specified with metric 1 */
				if (inet_addr_(nvram_safe_get(strcat_r(prefix, "pppoe_gateway", tmp))) &&
						!nvram_match("wan_heartbeat_x", ""))
					route_add(wan_ifname, 2, "0.0.0.0",
							nvram_safe_get(strcat_r(prefix, "pppoe_gateway", tmp)), "0.0.0.0");
				/* start multicast router */
				start_igmpproxy(wan_ifname);
			}

			/* launch pppoe client daemon */
			start_pppd(prefix);

			/* ppp interface name is referenced from this point on */
			wan_ifname = nvram_safe_get(strcat_r(prefix, "pppoe_ifname", tmp));

			/* Pretend that the WAN interface is up */
			if (nvram_match(strcat_r(prefix, "pppoe_demand", tmp), "1"))
			{
				int timeout = 5;
				/* Wait for pppx to be created */
				while (ifconfig(wan_ifname, IFUP, NULL, NULL) && timeout--)
					sleep(1);

				/* Retrieve IP info */
				if ((s = socket(AF_INET, SOCK_RAW, IPPROTO_RAW)) < 0)
					continue;
				strncpy(ifr.ifr_name, wan_ifname, IFNAMSIZ);

				/* Set temporary IP address */
				if (ioctl(s, SIOCGIFADDR, &ifr))
					perror(wan_ifname);
				nvram_set(strcat_r(prefix, "ipaddr", tmp), inet_ntoa(sin_addr(&ifr.ifr_addr)));
				nvram_set(strcat_r(prefix, "netmask", tmp), "255.255.255.255");

				/* Set temporary P-t-P address */
				if (ioctl(s, SIOCGIFDSTADDR, &ifr))
					perror(wan_ifname);
				nvram_set(strcat_r(prefix, "gateway", tmp), inet_ntoa(sin_addr(&ifr.ifr_dstaddr)));

				close(s);

				/*
				 * Preset routes so that traffic can be sent to proper pppx even before
				 * the link is brought up.
				 */
				preset_wan_routes(wan_ifname);
			}
#ifdef ASUS_EXT
			nvram_set("wan_ifname_t", wan_ifname);
#endif
		}
#endif
		/*
		 * Configure DHCP connection. The DHCP client will run
		 * 'udhcpc bound'/'udhcpc deconfig' upon finishing IP address
		 * renew and release.
		 */
		else if (!strcmp(wan_proto, "dhcp")
				) {
			char *wan_hostname = nvram_get(strcat_r(prefix, "hostname", tmp));
			char *dhcp_argv[] = { "udhcpc",
						"-i", wan_ifname,
						"-p", (sprintf(tmp, "/var/run/udhcpc%d.pid", unit), tmp),
						"-s", "/tmp/udhcpc",
						wan_hostname && *wan_hostname ? "-H" : NULL,
						wan_hostname && *wan_hostname ? wan_hostname : NULL,
						NULL
						};
			/* Start dhcp daemon */
			_eval(dhcp_argv, NULL, 0, &pid);
			/* Update wan information for null DNS server */
			update_wan_status(1);

#ifdef ASUS_EXT
			wanmessage("Can not get IP from server");
			nvram_set("wan_ifname_t", wan_ifname);
#endif
		}
		/* Configure static IP connection. */
		else if (strcmp(wan_proto, "static") == 0) {
			/* Assign static IP address to i/f */
			ifconfig(wan_ifname, IFUP,
					nvram_safe_get(strcat_r(prefix, "ipaddr", tmp)),
					nvram_safe_get(strcat_r(prefix, "netmask", tmp)));
			/* We are done configuration */
			wan_up(wan_ifname);
#ifdef ASUS_EXT
			nvram_set("wan_ifname_t", wan_ifname);
#endif
		}

#ifndef ASUS_EXT
		/* Start connection dependent firewall */
		start_firewall2(wan_ifname);
#endif

		dprintf("%s %s\n",
				nvram_safe_get(strcat_r(prefix, "ipaddr", tmp)),
				nvram_safe_get(strcat_r(prefix, "netmask", tmp)));
	}

	/* Report stats */
	if (nvram_invmatch("stats_server", "")) {
		char *stats_argv[] = { "stats", nvram_get("stats_server"), NULL };
		_eval(stats_argv, NULL, 5, NULL);
	}
}

void
stop_wan(void)
{
	char name[80], *next, signal[] = "XXXX";
	
	eval("killall", "stats");
	eval("killall", "ntpclient");

	/* Shutdown and kill all possible tasks */
	eval("killall", "ip-up");
	eval("killall", "ip-down");
#if 0
	snprintf(signal, sizeof(signal), "-%d", SIGHUP);
	eval("killall", signal, "pppoecd");
	eval("killall", signal, "pppd");
	eval("killall", "pppoecd");
#endif
	eval("killall", "l2tpd");
	eval("killall", "pppd");
#ifdef CDMA
	eval("killall", "chat");
#endif
	snprintf(signal, sizeof(signal), "-%d", SIGUSR2);
	eval("killall", signal, "udhcpc");
	eval("killall", "udhcpc");
	eval("killall", "igmpproxy");

	/* Bring down WAN interfaces */
	foreach(name, nvram_safe_get("wan_ifnames"), next)
	{
		ifconfig(name, 0, NULL, NULL);
	}

	/* Remove dynamically created links */
	unlink("/tmp/udhcpc");
	
	unlink("/tmp/ppp/ip-up");
	unlink("/tmp/ppp/ip-down");
//	unlink("/tmp/ppp/options");
	rmdir("/tmp/ppp");

#ifdef ASUS_EXT
	update_wan_status(0);
#endif

	dprintf("done\n");
}

void
stop_wan2(void)
{
	char name[80], *next, signal[] = "XXXX";
	
	eval("killall", "stats");
	eval("killall", "ntpclient");

	/* Shutdown and kill all possible tasks */
	eval("killall", "ip-up");
	eval("killall", "ip-down");
	snprintf(signal, sizeof(signal), "-%d", SIGHUP);
	eval("killall", signal, "pppoecd");
	eval("killall", signal, "pppd");
	eval("killall", "pppoecd");
	eval("killall", "l2tpd");
	eval("killall", "pppd");

	snprintf(signal, sizeof(signal), "-%d", SIGUSR2);
	eval("killall", signal, "udhcpc");
	eval("killall", "udhcpc");

	/* Remove dynamically created links */
	unlink("/tmp/udhcpc");
	
	unlink("/tmp/ppp/ip-up");
	unlink("/tmp/ppp/ip-down");
	unlink("/tmp/ppp/options");
	rmdir("/tmp/ppp");

#ifdef ASUS_EXT
	if(nvram_invmatch("wan_ifname_t", "")) wan_down(nvram_safe_get("wan_ifname_t"));
#endif

	dprintf("done\n");
}

void
start_wl(void)
{
        int i;
        char *lan_ifname = nvram_safe_get("lan_ifname");
        char name[80], *next;
        char tmp[100];
        char *lan_ifnames;
                                                                                                  
        /* If we're a travel router... then we need to make sure we get
                 the primary wireless interface up before trying to attach slave
                 interface(s) to the bridge */
        if(nvram_match("ure_disable", "0") && nvram_match("router_disable", "0")) {
                /* start wlireless */
                eval("wlconf", nvram_get("wan0_ifname"), "start");
        }
                                                                                                  
        /* Bring up bridged interfaces */
        for(i=0; i < MAX_NO_BRIDGE; i++) {
                if(!i) {
                        lan_ifname = nvram_safe_get("lan_ifname");
                        lan_ifnames = nvram_safe_get("lan_ifnames");
                }
                else {
                        snprintf(tmp, sizeof(tmp), "lan%x_ifname", i);
                        lan_ifname = nvram_safe_get( tmp);
                        snprintf(tmp, sizeof(tmp), "lan%x_ifnames", i);
                        lan_ifnames = nvram_safe_get( tmp);
                }
                if (strncmp(lan_ifname, "br", 2) == 0) {
                        foreach(name, lan_ifnames, next) {
                                if (strncmp(name, "wl", 2) == 0) {
                                        if (!wl_vif_enabled(name, tmp)) {
                                                continue; /* Ignore disabled WL VIF */
                                        }
                                }
                                /* If a wl i/f, start it */
                                eval("wlconf", name, "start");
                                                                                                  
                        } /* foreach().... */
                } /* if (strncmp(lan_ifname....*/
                /* specific non-bridged lan i/f */
                else if (strcmp(lan_ifname, "")) {
                        /* start wireless i/f */
                        eval("wlconf", lan_ifname, "start");
                }
        } /* For loop */
#ifdef RTN16
	eval("wl","interference", "2"); /* Improve the interference */
#endif
}


static int
update_resolvconf(void)
{
        FILE *fp;
        char word[100], *next;
	char tmp[100];
	int i, dns_count=0;
         
        /* check if auto dns enabled */
        if (!nvram_match("wan_dnsenable_x", "1"))
                return 0;
                                                                                                                  
        if (!(fp = fopen("/tmp/resolv.conf", "w+"))) {
                perror("/tmp/resolv.conf");
                return errno;
        }
        if(nvram_match("wan_dnsenable_x", "1"))
        {
                fprintf(fp, "nameserver 8.8.8.8\n");                    /*/ add for incompatible cable modem user */
                ++dns_count;
        }

        foreach(word, ((strlen(nvram_safe_get("wan0_dns")) > 0) ? nvram_safe_get("wan0_dns"):
                nvram_safe_get("wanx_dns")), next)
        {
                fprintf(fp, "nameserver %s\n", word);
                ++dns_count;
                nvram_set("auto_dhcp_dns", word);
                printf("[rc] auto set dhcp dns option:%s\n", word);     /*/ tmp test */
        }
        if(dns_count < 3)
        {
                if(dns_count < 2)
                        fprintf(fp, "nameserver 156.154.70.1\n");       /*/ add for incompatible cable modem user */
                fprintf(fp, "nameserver 8.8.4.4\n");                    /*/ add for incompatible cable modem user */
        }

        fclose(fp);

	// start dns patch check 0524
        dns_count = 0;
        /* write also /etc/resolv.conf */
        if (!(fp = fopen("/etc/resolv.conf", "w+"))) {
                nvram_set("update_resolv", "free");
                perror("/etc/resolv.conf");
                return errno;
        }

        if(nvram_match("wan_dnsenable_x", "1"))
        {
                fprintf(fp, "nameserver 8.8.8.8\n");                    /*/ add for incompatible cable modem user */
                ++dns_count;
        }

        foreach(word, ((strlen(nvram_safe_get("wan0_dns")) > 0) ? nvram_safe_get("wan0_dns"):
                nvram_safe_get("wanx_dns")), next)
        {
                fprintf(fp, "nameserver %s\n", word);
                ++dns_count;
        }
        foreach(word, ((strlen(nvram_safe_get("wan0_dns")) > 0) ? nvram_safe_get("wan0_dns"):
                nvram_safe_get("wanx_dns")), next)
        {
                fprintf(fp, "nameserver %s\n", word);
                ++dns_count;
        }
        if(dns_count < 3)
        {
                if(dns_count < 2)
                        fprintf(fp, "nameserver 156.154.70.1\n");       /*/ add for incompatible cable modem user */
                fprintf(fp, "nameserver 8.8.4.4\n");                    /*/ add for incompatible cable modem user */
        }

        fclose(fp);

        nvram_set("update_resolv", "free");

        //start_dns();
        // restart_dns();
        // dns patch check 0524 end

                                                                                                                  
//        foreach(word, (nvram_get("wan0_dns") ? :
//                nvram_safe_get("wanx_dns")), next)
//        {
//                fprintf(fp, "nameserver %s\n", word);
//        }
//        fclose(fp);

#ifdef ASUS_EXT
//        stop_dns();
//        start_dns();
//
	snprintf(tmp, sizeof(tmp), "-%d", SIGHUP);
        eval("killall", tmp, "dproxy");

#else
        /* notify dnsmasq */
        snprintf(tmp, sizeof(tmp), "-%d", SIGHUP);
        eval("killall", tmp, "dnsmasq");
#endif

        return 0;
}


void
wan_up(char *wan_ifname)
{
	char tmp[100], prefix[] = "wanXXXXXXXXXX_";
	char *wan_proto, *gateway;

	/* Figure out nvram variable name prefix for this i/f */
	if (wan_prefix(wan_ifname, prefix) < 0)
	{
		/* called for dhcp+ppp */
		if (!(nvram_match("wan0_ifname", wan_ifname)))
			return;

		/* re-start firewall with old ppp0 address or 0.0.0.0 */
		start_firewall_ex("ppp0", nvram_safe_get("wan0_ipaddr"),
				"br0", nvram_safe_get("lan_ipaddr"));

		/* setup static wan routes via physical device */
		add_routes("wan_", "route", wan_ifname);
		/* and one supplied via DHCP */
		add_wanx_routes("wanx_", wan_ifname, 0);

		gateway = inet_addr_(nvram_safe_get("wan_gateway")) != INADDR_ANY ?
				nvram_get("wan_gateway") : nvram_safe_get("wanx_gateway");

		/* and default route with metric 1 */
		if (inet_addr_(gateway) != INADDR_ANY)
		{
			char word[100], *next;

			route_add(wan_ifname, 2, "0.0.0.0", gateway, "0.0.0.0");

			/* ... and to dns servers as well for demand ppp to work */
			if (nvram_match("wan_dnsenable_x", "1"))
				foreach(word, nvram_safe_get("wanx_dns"), next)
				{
					in_addr_t mask = inet_addr(nvram_safe_get("wanx_netmask"));
					if ((inet_addr(word) & mask) != (inet_addr(nvram_safe_get("wanx_ipaddr")) & mask))
						route_add(wan_ifname, 2, word, gateway, "255.255.255.255");
				}
		}

		/* start multicast router */
		start_igmpproxy(wan_ifname);

		update_resolvconf();

		restart_dhcpd();

		return;
	}

	wan_proto = nvram_safe_get(strcat_r(prefix, "proto", tmp));

	dprintf("%s %s\n", wan_ifname, wan_proto);

	/* Set default route to gateway if specified */
	if (nvram_match(strcat_r(prefix, "primary", tmp), "1"))
	{
		if (strcmp(wan_proto, "dhcp") == 0 || strcmp(wan_proto, "static") == 0)
		{
			/* the gateway is in the local network */
			route_add(wan_ifname, 0, nvram_safe_get(strcat_r(prefix, "gateway", tmp)), NULL, "255.255.255.255");
		}
		/* default route via default gateway */
		route_add(wan_ifname, 0, "0.0.0.0", nvram_safe_get(strcat_r(prefix, "gateway", tmp)), "0.0.0.0");
		/* hack: avoid routing cycles, when both peer and server has the same IP */
		if (strcmp(wan_proto, "pptp") == 0 || strcmp(wan_proto, "l2tp") == 0) {
			/* delete gateway route as it's no longer needed */
			route_del(wan_ifname, 0, nvram_safe_get(strcat_r(prefix, "gateway", tmp)), "0.0.0.0", "255.255.255.255");
		}
	}

	/* Install interface dependent static routes */
	add_wan_routes(wan_ifname);

	/* setup static wan routes via physical device */
	if (strcmp(wan_proto, "dhcp") == 0 || strcmp(wan_proto, "static") == 0)
	{
		nvram_set("wanx_gateway", nvram_safe_get(strcat_r(prefix, "gateway", tmp)));
		add_routes("wan_", "route", wan_ifname);
	}

	/* and one supplied via DHCP */
	if (strcmp(wan_proto, "dhcp") == 0)
		add_wanx_routes(prefix, wan_ifname, 0);

	/* Add dns servers to resolv.conf */
	update_resolvconf();

	restart_dhcpd();

	/* Sync time */
	//start_ntpc();
#ifdef ASUS_EXT
	update_wan_status(1);

	start_firewall_ex(wan_ifname, nvram_safe_get(strcat_r(prefix, "ipaddr", tmp)),
			"br0", nvram_safe_get("lan_ipaddr"));

	start_ddns();
//	stop_upnp();
//	start_upnp();
#endif

#ifdef CDMA
	if ((strcmp(wan_proto, "cdma")==0))
	{
		nvram_set("cdma_down", "2");
	}
#endif
#ifdef QOS
        /* --SZ Angela 09.03 QOS {*/
        
        fprintf(stderr, "network: perform qos.\n");
        if(nvram_match("qos_enable", "1")){
                nvram_set("qos_ubw", "0");
                nvram_set("qos_ubw_tmp", "0");
                qos_get_wan_rate();
                nvram_set("qos_ubw_tmp",nvram_get("qos_ubw"));
                fprintf(stderr, "qos_ubw=%s\n", nvram_get("qos_ubw"));
                
                Speedtest_Init();
        }
        fprintf(stderr, "network: after perform qos.\n");
        /* --SZ Angela 09.03 QOS } */
#endif

	/* start multicast router */
	if(!strcmp(wan_proto, "dhcp")
			|| !strcmp(wan_proto, "static"))
	{
		start_igmpproxy(wan_ifname);
	}
	// dbg by Yen
//	if (nvram_invmatch("wan_stb_x","0")) {
//		eval ("killall","networkmap");
//		sleep(5);
//		nvram_unset("networkmap_down");
//		eval ("networkmap");
//		stop_networkmap();
//		start_networkmap();
//	}

	dprintf("done\n");
}

void
wan_down(char *wan_ifname)
{
	char tmp[100], prefix[] = "wanXXXXXXXXXX_";
	char *wan_proto;

	/* Figure out nvram variable name prefix for this i/f */
	if (wan_prefix(wan_ifname, prefix) < 0)
		return;

	wan_proto = nvram_safe_get(strcat_r(prefix, "proto", tmp));
	
	dprintf("%s %s\n", wan_ifname, wan_proto);

	/* Remove default route to gateway if specified */
	if (nvram_match(strcat_r(prefix, "primary", tmp), "1"))
		route_del(wan_ifname, 0, "0.0.0.0", 
			nvram_safe_get(strcat_r(prefix, "gateway", tmp)),
			"0.0.0.0");

        /* Del route while static IP */
        if (strcmp(wan_proto, "static") == 0)
                ifconfig(wan_ifname, IFUP, "0.0.0.0", NULL);

	/* Remove interface dependent static routes */
	del_wan_routes(wan_ifname);

        /* Update resolv.conf -- leave as is if no dns servers left for demand to work */
        if (*nvram_safe_get("wanx_dns"))
                nvram_unset(strcat_r(prefix, "dns", tmp));
        update_resolvconf();

#ifdef ASUS_EXT
	//printf("update wan status\n");
	update_wan_status(0);
#endif


#ifdef CDMA
	if ((strcmp(wan_proto, "cdma")==0))
	{
		stop_cdma();
		nvram_set("cdma_down", "1");
	}
#endif

	dprintf("done\n");
}

/* Enable WET DHCP relay for ethernet clients */
static int
enable_dhcprelay(char *ifname)
{
	char name[80], *next;

	dprintf("%s\n", ifname);
	
	/* WET interface is meaningful only in bridged environment */
	if (strncmp(ifname, "br", 2) == 0) {
		foreach(name, nvram_safe_get("lan_ifnames"), next) {
			char mode[] = "wlXXXXXXXXXX_mode";
			int unit;

			/* make sure the interface is indeed of wl */
			if (wl_probe(name))
				continue;
			
			/* get the instance number of the wl i/f */
			wl_ioctl(name, WLC_GET_INSTANCE, &unit, sizeof(unit));
			snprintf(mode, sizeof(mode), "wl%d_mode", unit);

			/* enable DHCP relay, there should be only one WET i/f */
			if (nvram_match(mode, "wet")) {
				uint32 ip;
//				inet_aton(nvram_safe_get("lan_ipaddr"), (struct in_addr *)&ip);
				if (nvram_match("lan_ipaddr_t", ""))
				{
					fprintf(stderr, "Enable WET DHCP relay for IP %s\n", nvram_safe_get("lan_ipaddr"));
					inet_aton(nvram_safe_get("lan_ipaddr"), (struct in_addr *)&ip);
				}
				else
				{
					fprintf(stderr, "Enable WET DHCP relay for IP %s\n", nvram_safe_get("lan_ipaddr_t"));
					inet_aton(nvram_safe_get("lan_ipaddr_t"), (struct in_addr *)&ip);
				}
				if (wl_iovar_setint(name, "wet_host_ipv4", ip))
					perror("wet_host_ipv4");
				break;
			}
		}
	}
	return 0;
}

#ifdef ASUS_EXT
void
lan_up(char *lan_ifname)
{
	FILE *fp;
	char word[100], *next;
	char line[100];

	/* Set default route to gateway if specified */
	route_add(lan_ifname, 0, "0.0.0.0", 
			nvram_safe_get("lan_gateway"),
			"0.0.0.0");

	/* Enable WET DHCP relay if requested */
//	if (atoi(nvram_safe_get("dhcp_relay")) == 1)
	if (atoi(nvram_safe_get("sw_mode_ex")) == 2)
		enable_dhcprelay(lan_ifname);

	/* Open resolv.conf to read */
	if (!(fp = fopen("/tmp/resolv.conf", "w"))) {
		perror("/tmp/resolv.conf");
		return errno;
	}

	if (nvram_invmatch("lan_gateway", ""))
		fprintf(fp, "nameserver %s\n", nvram_safe_get("lan_gateway"));

	foreach(word, nvram_safe_get("lan_dns"), next)
	{
		fprintf(fp, "nameserver %s\n", word);
	}
	fclose(fp);

	/* Sync time */
	//start_ntpc();
}

void
lan_down(char *lan_ifname)
{
	/* Remove default route to gateway if specified */
	route_del(lan_ifname, 0, "0.0.0.0", 
			nvram_safe_get("lan_gateway"),
			"0.0.0.0");

	/* remove resolv.conf */
	unlink("/tmp/resolv.conf");
}


void
lan_up_ex(char *lan_ifname)
{
	FILE *fp;
	char word[100], *next;
	char line[100];

	/* Set default route to gateway if specified */
	route_add(lan_ifname, 0, "0.0.0.0", 
			nvram_safe_get("lan_gateway_t"),
			"0.0.0.0");

	/* Enable WET DHCP relay if requested */
//	if (atoi(nvram_safe_get("dhcp_relay")) == 1)
	if (atoi(nvram_safe_get("sw_mode_ex")) == 2)
		enable_dhcprelay(lan_ifname);

	/* Open resolv.conf to read */
	if (!(fp = fopen("/tmp/resolv.conf", "w"))) {
		perror("/tmp/resolv.conf");
		return errno;
	}

	if (nvram_invmatch("lan_gateway_t", ""))
		fprintf(fp, "nameserver %s\n", nvram_safe_get("lan_gateway_t"));

	foreach(word, nvram_safe_get("lan_dns_t"), next)
	{
		fprintf(fp, "nameserver %s\n", word);
	}
	fclose(fp);

	/* Sync time */
	//start_ntpc();
	//update_lan_status(1);
}

void
lan_down_ex(char *lan_ifname)
{
	/* Remove default route to gateway if specified */
	route_del(lan_ifname, 0, "0.0.0.0", 
			nvram_safe_get("lan_gateway_t"),
			"0.0.0.0");

	/* remove resolv.conf */
	unlink("/tmp/resolv.conf");

	update_lan_status(0);
}
#endif

static int
notify_nas(char *type, char *ifname, char *action)
{
	char *argv[] = {"nas4not", type, ifname, action, 
			NULL,	/* role */
			NULL,	/* crypto */
			NULL,	/* auth */
			NULL,	/* passphrase */
			NULL,	/* ssid */
			NULL};
	char *str = NULL;
	int retries = 10;
	char tmp[100], prefix[] = "wlXXXXXXXXXX_";
	int unit;
	char remote[ETHER_ADDR_LEN];
	char ssid[48], pass[80], auth[16], crypto[16], role[8];
	int i;

	/* the wireless interface must be configured to run NAS */
	wl_ioctl(ifname, WLC_GET_INSTANCE, &unit, sizeof(unit));
	snprintf(prefix, sizeof(prefix), "wl%d_", unit);
	if (nvram_match(strcat_r(prefix, "akm", tmp), "") &&
	    nvram_match(strcat_r(prefix, "auth_mode", tmp), "none"))
		return 0;

	/* find WDS link configuration */
	wl_ioctl(ifname, WLC_WDS_GET_REMOTE_HWADDR, remote, ETHER_ADDR_LEN);
	for (i = 0; i < MAX_NVPARSE; i ++) {
		char mac[ETHER_ADDR_STR_LEN];
		uint8 ea[ETHER_ADDR_LEN];

		if (get_wds_wsec(unit, i, mac, role, crypto, auth, ssid, pass) &&
		    ether_atoe(mac, ea) && !bcmp(ea, remote, ETHER_ADDR_LEN)) {
			argv[4] = role;
			argv[5] = crypto;
			argv[6] = auth;
			argv[7] = pass;
			argv[8] = ssid;
			break;
		}
	}

	/* did not find WDS link configuration, use wireless' */
	if (i == MAX_NVPARSE) {
		/* role */
		argv[4] = "auto";
		/* crypto */
		argv[5] = nvram_safe_get(strcat_r(prefix, "crypto", tmp));
		/* auth mode */
		argv[6] = nvram_safe_get(strcat_r(prefix, "akm", tmp));
		/* passphrase */
		argv[7] = nvram_safe_get(strcat_r(prefix, "wpa_psk", tmp));
		/* ssid */
		argv[8] = nvram_safe_get(strcat_r(prefix, "ssid", tmp));
	}

	/* wait till nas is started */
	while (retries -- > 0 && !(str = file2str("/tmp/nas.lan.pid")))
		sleep(1);
	if (str) {
		int pid;
		free(str);
		return _eval(argv, ">/dev/console", 0, &pid);
	}
	return -1;
}

int
hotplug_net(void)
{
	char *lan_ifname = nvram_safe_get("lan_ifname");
	char *interface, *action;

	if (!(interface = getenv("INTERFACE")) ||
	    !(action = getenv("ACTION")))
		return EINVAL;

	if (strncmp(interface, "wds", 3))
		return 0;
#ifdef LINUX26 
        if (!strcmp(action, "add")) {
#else
        if (!strcmp(action, "register")) {
#endif
		/* Bring up the interface and add to the bridge */
		ifconfig(interface, IFUP, NULL, NULL);

#ifdef __CONFIG_EMF__
                if (nvram_match("emf_enable", "1")) {
                        eval("emf", "add", "iface", lan_ifname, interface);
                        emf_mfdb_update(lan_ifname, interface, TRUE);
                        emf_uffp_update(lan_ifname, interface, TRUE);
                        emf_rtport_update(lan_ifname, interface, TRUE);
                }
#endif /* __CONFIG_EMF__ */
		
		/* Bridge WDS interfaces */
		if (!strncmp(lan_ifname, "br", 2) && 
		    eval("brctl", "addif", lan_ifname, interface))
		    return 0;

		/* Notify NAS of adding the interface */
		notify_nas("lan", interface, "up");
	}
	return 0;
}


int
wan_ifunit(char *wan_ifname)
{
	int unit;
	char tmp[100], prefix[] = "wanXXXXXXXXXX_";

	if ((unit = ppp_ifunit(wan_ifname)) >= 0)
		return unit;
	else {
		for (unit = 0; unit < MAX_NVPARSE; unit ++) {
			snprintf(prefix, sizeof(prefix), "wan%d_", unit);
			if (nvram_match(strcat_r(prefix, "ifname", tmp), wan_ifname) &&
			    (nvram_match(strcat_r(prefix, "proto", tmp), "dhcp") ||
#ifdef CDMA
			     nvram_match(strcat_r(prefix, "proto", tmp), "cdma") ||
#endif
			     nvram_match(strcat_r(prefix, "proto", tmp), "static")))
				return unit;
		}
	}
	return -1;
}

int
preset_wan_routes(char *wan_ifname)
{
	char tmp[100], prefix[] = "wanXXXXXXXXXX_";


	/* Figure out nvram variable name prefix for this i/f */
	if (wan_prefix(wan_ifname, prefix) < 0)
		return -1;

	/* Set default route to gateway if specified */
	if (nvram_match(strcat_r(prefix, "primary", tmp), "1"))
	{
		route_add(wan_ifname, 0, "0.0.0.0", "0.0.0.0", "0.0.0.0");
	}

	/* Install interface dependent static routes */
	add_wan_routes(wan_ifname);
	return 0;
}

int
wan_primary_ifunit(void)
{
	int unit;
	
	for (unit = 0; unit < MAX_NVPARSE; unit ++) {
		char tmp[100], prefix[] = "wanXXXXXXXXXX_";
		snprintf(prefix, sizeof(prefix), "wan%d_", unit);
		if (nvram_match(strcat_r(prefix, "primary", tmp), "1"))
			return unit;
	}

	return 0;
}

