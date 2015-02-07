
/*
 * Copyright 2004, Broadcom Corporation
 * All Rights Reserved.
 * 
 * THIS SOFTWARE IS OFFERED "AS IS", AND BROADCOM GRANTS NO WARRANTIES OF ANY
 * KIND, EXPRESS OR IMPLIED, BY STATUTE, COMMUNICATION OR OTHERWISE. BROADCOM
 * SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A SPECIFIC PURPOSE OR NONINFRINGEMENT CONCERNING THIS SOFTWARE.
 *
 * $Id: firewall_noipt.c,v 1.1.1.1 2008/07/21 09:17:41 james26_jang Exp $
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <dirent.h>

#include <bcmnvram.h>
#include <shutils.h>
#include <rc.h>
#include <netconf.h>
#include <nvparse.h>

/* Add filter to specified table */
static void
add_filter(netconf_filter_t *filter, int dir)
{
	filter->dir = dir;
	netconf_add_filter(filter);
}

/* Add port forward and a matching ACCEPT rule to the FORWARD table */
void
add_forward(netconf_nat_t *nat, int dir, int target)
{
	netconf_filter_t filter;

	/* Set up LAN side match */
	memset(&filter, 0, sizeof(filter));
	filter.match.ipproto = nat->match.ipproto;
	filter.match.src.ports[1] = nat->match.src.ports[1];
	filter.match.dst.ipaddr.s_addr = nat->ipaddr.s_addr;
	filter.match.dst.netmask.s_addr = htonl(0xffffffff);
	filter.match.dst.ports[0] = nat->ports[0];
	filter.match.dst.ports[1] = nat->ports[1];
	strncpy(filter.match.in.name, nat->match.in.name, IFNAMSIZ);

	/* Accept connection */
	filter.target = target;
	filter.dir = dir;

	/* Do it */
	netconf_add_nat(nat);
	netconf_add_filter(&filter);
}

#ifdef NOIPTABLES
/* Add port forward and a matching ACCEPT rule to the FORWARD table */
static void
add_forward_ex(char *chain, char *proto, int port)
{
#ifdef REMOVE
	netconf_filter_t filter;

	/* Set up LAN side match */
	memset(&filter, 0, sizeof(filter));
	filter.match.ipproto = nat->match.ipproto;
	filter.match.src.ports[1] = nat->match.src.ports[1];
	filter.match.dst.ipaddr.s_addr = nat->ipaddr.s_addr;
	filter.match.dst.netmask.s_addr = htonl(0xffffffff);
	filter.match.dst.ports[0] = nat->ports[0];
	filter.match.dst.ports[1] = nat->ports[1];
	strncpy(filter.match.in.name, nat->match.in.name, IFNAMSIZ);

	/* Accept connection */
	filter.target = target;
	filter.dir = dir;

	/* Do it */
	netconf_add_filter(&filter);
#endif
}

static void
add_netmap_ex(char *chain, char *proto, char *dstip, int port, char *toip)
{
#ifdef REMOVE
	netconf_filter_t filter;

	/* Set up LAN side match */
	memset(&filter, 0, sizeof(filter));
	filter.match.ipproto = nat->match.ipproto;
	filter.match.src.ports[1] = nat->match.src.ports[1];
	filter.match.dst.ipaddr.s_addr = nat->ipaddr.s_addr;
	filter.match.dst.netmask.s_addr = htonl(0xffffffff);
	filter.match.dst.ports[0] = nat->ports[0];
	filter.match.dst.ports[1] = nat->ports[1];
	strncpy(filter.match.in.name, nat->match.in.name, IFNAMSIZ);

	/* Accept connection */
	filter.target = target;
	filter.dir = dir;

	/* Do it */
	netconf_add_nat(nat);
#endif
}
#endif

int
start_firewall(void)
{
	DIR *dir;
	struct dirent *file;
	FILE *fp;
	char name[NAME_MAX];
	netconf_filter_t filter;
	netconf_app_t app;
	int i, j;
	char var[256], *next;
	int log_level, log_drop, log_accept;

	/* Reset firewall */
	netconf_reset_fw();

#ifndef REMOVE_QTR
	/* Block obviously spoofed IP addresses */
	if (!(dir = opendir("/proc/sys/net/ipv4/conf")))
		perror("/proc/sys/net/ipv4/conf");
	while (dir && (file = readdir(dir))) {
		if (strncmp(file->d_name, ".", NAME_MAX) != 0 &&
		    strncmp(file->d_name, "..", NAME_MAX) != 0) {
			sprintf(name, "/proc/sys/net/ipv4/conf/%s/rp_filter", file->d_name);
			if (!(fp = fopen(name, "r+"))) {
				perror(name);
				break;
			}
			fputc('1', fp);
			fclose(fp);
		}
	}
	if (dir) closedir(dir);
#endif

	/* Add max conntrack as 4096, thanks for Oleg */
	if ((fp=fopen("/proc/sys/net/ipv4/ip_conntrack_max", "r+")))
	{
		if (nvram_get("misc_conntrack_x")==NULL) fputs("4096", fp);
		else fputs(nvram_safe_get("misc_conntrack_x"), fp);
		fclose(fp);
	}

	/* Optionally log connections */
	log_level = atoi(nvram_safe_get("log_level"));
	log_drop = (log_level & 1) ? NETCONF_LOG_DROP : NETCONF_DROP;
	log_accept = (log_level & 2) ? NETCONF_LOG_ACCEPT : NETCONF_ACCEPT;

	/*
	 * Inbound drops
	 */
#ifndef REMOVE_QTR
	/* Drop invalid packets */
	memset(&filter, 0, sizeof(filter));
	filter.match.state = NETCONF_INVALID;
	filter.target = NETCONF_DROP;
	add_filter(&filter, NETCONF_IN);
	add_filter(&filter, NETCONF_FORWARD);
#endif

	/*
	 * Forward drops
 	 */
	if (nvram_match("fw_lw_enable_x","1")) {
	/* Client filters */
	for (i = 0; i < MAX_NVPARSE; i++) {
		netconf_filter_t start, end;

#ifdef ASUS_EXT
		int days, daystart, dayend;

		if (get_filter_client_ex(i, &start, &end, &days) && !(start.match.flags & NETCONF_DISABLED)) 
		{
			
			while (ntohl(start.match.src.ipaddr.s_addr) <= ntohl(end.match.src.ipaddr.s_addr)) 
			{

				/* Override target */
				start.target = log_drop;
				j=0;
				while(dayrange(days, &j, &daystart, &dayend))
				{
					start.match.days[0] = daystart;
					start.match.days[1] = dayend;
					add_filter(&start, NETCONF_FORWARD);
					if (j>=7) break;

				}
				start.match.src.ipaddr.s_addr = htonl(ntohl(start.match.src.ipaddr.s_addr) + 1);
			}
		}
#else
		if (get_filter_client(i, &start, &end) && !(start.match.flags & NETCONF_DISABLED)) {
			while (ntohl(start.match.src.ipaddr.s_addr) <= ntohl(end.match.src.ipaddr.s_addr)) {
				/* Override target */
				start.target = log_drop;
				add_filter(&start, NETCONF_FORWARD);
				start.match.src.ipaddr.s_addr = htonl(ntohl(start.match.src.ipaddr.s_addr) + 1);
			}
		}
#endif
	}
	}

#ifndef ASUS_EXT
	/* Filter by MAC address */
	if (!nvram_match("filter_macmode", "disabled")) {
		memset(&filter, 0, sizeof(filter));
		strcpy(filter.match.in.name, nvram_safe_get("lan_ifname"));
		if (nvram_match("filter_macmode", "allow")) {
			/* Allow new connections from the LAN side only from the specified addresses */
			filter.target = log_accept;
			filter.match.state = NETCONF_NEW;
		} else {
			/* Drop connections from the specified addresses */
			filter.target = log_drop;
		}
		foreach(var, nvram_safe_get("filter_maclist"), next) {
			if (ether_atoe(var, filter.match.mac.octet))
				add_filter(&filter, NETCONF_FORWARD);
		}
	}
#endif

#ifndef REMOVE_QTR
	/*
	 * Inbound accepts
	 */
	/* Allow new connections from the loopback interface */
	memset(&filter, 0, sizeof(filter));
	filter.match.state = NETCONF_NEW;
	strcpy(filter.match.in.name, "lo");
	filter.target = log_accept;
	add_filter(&filter, NETCONF_IN);
	add_filter(&filter, NETCONF_FORWARD);

	/* Allow new connections from the LAN side */
	memset(&filter, 0, sizeof(filter));
	filter.match.state = NETCONF_NEW;
	strcpy(filter.match.in.name, nvram_safe_get("lan_ifname"));
	filter.target = log_accept;
	add_filter(&filter, NETCONF_IN);
	if (!nvram_match("filter_macmode", "allow"))
		add_filter(&filter, NETCONF_FORWARD);

	/* Allow established and related outbound connections back in */
	memset(&filter, 0, sizeof(filter));
	filter.match.state = NETCONF_ESTABLISHED | NETCONF_RELATED;
	filter.target = log_accept;
	add_filter(&filter, NETCONF_IN);
	add_filter(&filter, NETCONF_FORWARD);
#endif
	/*
	 * NAT
	 */

	/* Enable IP masquerading */
	if ((fp = fopen("/proc/sys/net/ipv4/ip_forward", "r+"))) {
		fputc('1', fp);
		fclose(fp);
	} else
		perror("/proc/sys/net/ipv4/ip_forward");


	if (nvram_match("autofw_enable_x","1")) {
	/* Application specific port forwards */
	for (i = 0; i < MAX_NVPARSE; i++) {
		memset(&app, 0, sizeof(app));
		
		if (
#ifdef ASUS_EXT
		get_autofw_port_ex(i, &app) 
#else		
		get_autofw_port(i, &app) 
#endif
		&& !(app.match.flags & NETCONF_DISABLED))
			netconf_add_fw((netconf_fw_t *) &app);
	}
	}

#ifndef REMOVE_QTR
    /*
     * Inbound defaults
     */

    /* Log refused connections */
    memset(&filter, 0, sizeof(filter));
    filter.target = log_drop;
    add_filter(&filter, NETCONF_IN);
    add_filter(&filter, NETCONF_FORWARD);
#endif

	dprintf("done\n");
	return 0;
}


int
start_firewall2(char *wan_ifname)
{
	netconf_nat_t nat;
	netconf_filter_t filter;
	int log_level, log_accept;
	char tmp[100], prefix[] = "wanXXXXXXXXXX_";
	char wan_ip[32], lan_ip[32], lan_class[32];
	int i;

	/* Optionally log connections */
	log_level = atoi(nvram_safe_get("log_level"));
	log_accept = (log_level & 2) ? NETCONF_LOG_ACCEPT : NETCONF_ACCEPT;
	
	/* Allow new connections from the WAN side */
	if (nvram_match("fw_disable", "1")) {
		memset(&filter, 0, sizeof(filter));
		filter.match.state = NETCONF_NEW;
		strcpy(filter.match.in.name, wan_ifname);
		filter.target = log_accept;
		add_filter(&filter, NETCONF_IN);
		add_filter(&filter, NETCONF_FORWARD);
	}

	/* Enable IP masquerading */
	memset(&nat, 0, sizeof(nat));
	inet_aton(nvram_safe_get("lan_ipaddr"), &nat.match.src.ipaddr);
	inet_aton(nvram_safe_get("lan_netmask"), &nat.match.src.netmask);
	nat.match.src.ipaddr.s_addr &= nat.match.src.netmask.s_addr;
	strcpy(nat.match.out.name, wan_ifname);
	nat.target = NETCONF_MASQ;
	netconf_add_nat(&nat);

	/* Enable IP masquerading, loopback */
#ifndef REMOVE_QTR
	memset(&nat, 0, sizeof(nat));
	inet_aton(nvram_safe_get("lan_ipaddr"), &nat.match.src.ipaddr);
	inet_aton(nvram_safe_get("lan_netmask"), &nat.match.src.netmask);
	nat.match.src.ipaddr.s_addr &= nat.match.src.netmask.s_addr;
	inet_aton(nvram_safe_get("lan_ipaddr"), &nat.match.dst.ipaddr);
	inet_aton(nvram_safe_get("lan_netmask"), &nat.match.dst.netmask);
	nat.match.dst.ipaddr.s_addr &= nat.match.dst.netmask.s_addr;
	strcpy(nat.match.out.name, nvram_safe_get("lan_ifname"));
	nat.target = NETCONF_MASQ;
	netconf_add_nat(&nat);
#endif
	/* Enable remote management */
	if (nvram_invmatch("http_wanport", "")) {
		/* Set up WAN side match */
		memset(&nat, 0, sizeof(nat));
		nat.match.ipproto = IPPROTO_TCP;
		nat.match.src.ports[1] = htons(0xffff);
		nat.match.dst.ports[0] = htons(atoi(nvram_safe_get("http_wanport")));
		nat.match.dst.ports[1] = htons(atoi(nvram_safe_get("http_wanport")));
		//strncpy(nat.match.in.name, wan_ifname, IFNAMSIZ);


    		if (nvram_invmatch("wan_ipaddr_t", ""))
    		{	
			inet_aton("255.255.255.255", &nat.match.dst.netmask);
			inet_aton(nvram_safe_get("wan_ipaddr_t") , &nat.match.dst.ipaddr);
    		}

		/* Set up DNAT to LAN */
		nat.target = NETCONF_DNAT;
		(void) inet_aton(nvram_safe_get("lan_ipaddr"), &nat.ipaddr);
		nat.ports[0] = htons(atoi(nvram_safe_get("http_lanport")));
		nat.ports[1] = htons(atoi(nvram_safe_get("http_lanport")));

		add_forward(&nat, NETCONF_IN, log_accept);
	}

	/* Persistent (static) port forwards */
	if (nvram_match("vts_enable_x","1")) {
	for (i = 0; i < MAX_NVPARSE; i++) {
		memset(&nat, 0, sizeof(nat));
		if (
#ifdef ASUS_EXT
	get_forward_port_ex(i, &nat) 
#else
	get_forward_port(i, &nat) 
#endif

	&& !(nat.match.flags & NETCONF_DISABLED)) {
			/* Set interface name (match packets entering WAN interface) */

    			if (nvram_invmatch("wan_ipaddr_t", ""))
    			{	
				inet_aton("255.255.255.255", &nat.match.dst.netmask);
				inet_aton(nvram_safe_get("wan_ipaddr_t") , &nat.match.dst.ipaddr);
    			}
			//strncpy(nat.match.in.name, wan_ifname, IFNAMSIZ);
			add_forward(&nat, NETCONF_FORWARD, log_accept);
		}
	}
	}

	/* Forward all WAN ports to DMZ IP address */
	if (nvram_invmatch("dmz_ip", "")) {
		/* Set up WAN side match */
		memset(&nat, 0, sizeof(nat));
		nat.match.src.ports[1] = htons(0xffff);
		nat.match.dst.ports[1] = htons(0xffff);
		strncpy(nat.match.in.name, wan_ifname, IFNAMSIZ);

    		if (nvram_invmatch("wan_ipaddr_t", ""))
    		{	
			inet_aton("255.255.255.255", &nat.match.dst.netmask);
			inet_aton(nvram_safe_get("wan_ipaddr_t") , &nat.match.dst.ipaddr);
    		}

		/* Set up DNAT to LAN */
		nat.target = NETCONF_DNAT;
		(void) inet_aton(nvram_safe_get("dmz_ip"), &nat.ipaddr);
		nat.ports[1] = htons(0xffff);

		nat.match.ipproto = IPPROTO_TCP;
		add_forward(&nat, NETCONF_FORWARD, log_accept);
		nat.match.ipproto = IPPROTO_UDP;
		add_forward(&nat, NETCONF_FORWARD, log_accept);
	}

	/* Clamp TCP MSS to PMTU of WAN interface */
	sprintf(prefix, "wan%d_", wan_ifunit(wan_ifname));
	if (nvram_match(strcat_r(prefix, "proto", tmp), "pppoe"))
		netconf_clamp_mss_to_pmtu();

   	if (nvram_match("router_disable", "0") && nvram_invmatch("sp_battle_ips", "0"))
	{
		#define BASEPORT "6112"
		#define BASEPORT_NEW 10000

		strcpy(lan_ip, nvram_safe_get("lan_ipaddr"));
		strcpy(wan_ip, nvram_safe_get("wan_ipaddr_t"));
		ip2class(lan_ip, nvram_safe_get("lan_netmask"), lan_class);

#ifdef NOIPTABLES
		add_netmap_ex("PREROUTING", "udp", wan_ip, BASEPORT, lan_class);
		add_netmap_ex("POSTROUTING", "udp", lan_class, BASEPORT, wan_ip);
		add_forward_ex("FORWARD", "udp", BASEPORT);
#else
		/* run starcraft patch anyway */
		//fprintf(fp, "-A PREROUTING -p udp -d %s --sport %d -j NETMAP --to %s\n", wan_ip, BASEPORT, lan_class);
		eval("iptables", "-t", "nat", "-I", "PREROUTING", "-p", "udp", "-d", wan_ip, "--sport", BASEPORT, "-j", "NETMAP", "--to", lan_class);

		//fprintf(fp, "-A POSTROUTING -p udp -s %s --dport %d -j NETMAP --to %s\n", lan_class, BASEPORT, wan_ip);
		eval("iptables", "-t", "nat", "-I", "POSTROUTING", "-p", "udp", "-s", lan_class, "--dport", BASEPORT, "-j", "NETMAP", "--to", wan_ip);

		//fprintf(fp1, "-A FORWARD -p udp --dport %d -j %s\n", BASEPORT, logaccept);
		eval("iptables", "-A", "FORWARD", "-p", "udp", "--dport", BASEPORT, "-j", "ACCEPT");
#endif
	}
	dprintf("done\n");
	return 0;
}


