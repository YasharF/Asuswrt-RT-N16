/*
 * udhcpc scripts
 *
 * Copyright 2004, Broadcom Corporation
 * All Rights Reserved.
 * 
 * THIS SOFTWARE IS OFFERED "AS IS", AND BROADCOM GRANTS NO WARRANTIES OF ANY
 * KIND, EXPRESS OR IMPLIED, BY STATUTE, COMMUNICATION OR OTHERWISE. BROADCOM
 * SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A SPECIFIC PURPOSE OR NONINFRINGEMENT CONCERNING THIS SOFTWARE.
 *
 * $Id: udhcpc_ex.c,v 1.3 2008/09/05 12:00:11 james26_jang Exp $
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <net/route.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <time.h>
#include <unistd.h>

#include <bcmnvram.h>
#include <netconf.h>
#include <shutils.h>
#include <rc.h>

#include <signal.h>

static char udhcpstate[8];

static int
expires(char *lan_ifname, unsigned int in)
{
	time_t now;
	FILE *fp;
	char tmp[100];

	time(&now);
	snprintf(tmp, sizeof(tmp), "/tmp/udhcpc%d.expires", 0); 
	if (!(fp = fopen(tmp, "w"))) {
		perror(tmp);
		return errno;
	}
	fprintf(fp, "%d", (unsigned int) now + in);
	fclose(fp);
	return 0;
}	

/* 
 * deconfig: This argument is used when udhcpc starts, and when a
 * leases is lost. The script should put the interface in an up, but
 * deconfigured state.
*/
static int
deconfig(void)
{
	char *lan_ifname = safe_getenv("interface");

#ifdef PBCENR
	if (	nvram_match("sw_mode_ex", "2") &&
		nvram_invmatch("lan_ipaddr_t", "") && nvram_invmatch("lan_netmask_t", ""))
		ifconfig(lan_ifname, IFUP,
			 nvram_safe_get("lan_ipaddr_t"),
			 nvram_safe_get("lan_netmask_t"));
	else
#endif
		ifconfig(lan_ifname, IFUP,
			 nvram_safe_get("lan_ipaddr"),
			 nvram_safe_get("lan_netmask"));

	expires(lan_ifname, 0);

	lan_down_ex(lan_ifname);

	logmessage("dhcp client", "%s: lease is lost", udhcpstate);
	//wanmessage("lost IP from server");

	dprintf("done\n");
	return 0;
}

/*
 * bound: This argument is used when udhcpc moves from an unbound, to
 * a bound state. All of the paramaters are set in enviromental
 * variables, The script should configure the interface, and set any
 * other relavent parameters (default gateway, dns server, etc).
*/
static int
bound(void)
{
	char *lan_ifname = safe_getenv("interface");
	char *value;
	char tmp[100], prefix[] = "lanXXXXXXXXXX_";
	struct in_addr fixed_addr;

	snprintf(prefix, sizeof(prefix), "lan_");
#ifdef PBCENR
	if (/*nvram_invmatch("ui_triggered", "1") || */nvram_invmatch("sw_mode_ex", "2"))
	{
		if ((value = getenv("ip")))
		{
			inet_aton(value, &fixed_addr);
			value = inet_ntoa(fixed_addr);
			nvram_set(strcat_r(prefix, "ipaddr_t", tmp), value);
		}
		if ((value = getenv("subnet")))
			nvram_set(strcat_r(prefix, "netmask_t", tmp), value);
		if ((value = getenv("router")))
			nvram_set(strcat_r(prefix, "gateway_t", tmp), value);
		if ((value = getenv("dns")))
			nvram_set(strcat_r(prefix, "dns_t", tmp), value);
		if ((value = getenv("wins")))
			nvram_set(strcat_r(prefix, "wins_t", tmp), value);
//		if ((value = getenv("hostname")))
//			sethostname(value, strlen(value) + 1);
		if ((value = getenv("domain")))
			nvram_set(strcat_r(prefix, "domain_t", tmp), value);
		if ((value = getenv("lease"))) {
			nvram_set(strcat_r(prefix, "lease_t", tmp), value);
			expires(lan_ifname, atoi(value));
		}
	}
	else	// step 1 of 2-step dhcp lease application for RT-N12 repeater mode UI
	{
		if ((value = getenv("ip")))
		{
			inet_aton(value, &fixed_addr);
			value = inet_ntoa(fixed_addr);
			nvram_set(strcat_r(prefix, "ipaddr_new", tmp), value);
		}
		if ((value = getenv("subnet")))
			nvram_set(strcat_r(prefix, "netmask_new", tmp), trim_r(value));
	        if ((value = getenv("router")))
			nvram_set(strcat_r(prefix, "gateway_new", tmp), trim_r(value));
		if ((value = getenv("dns")))
			nvram_set(strcat_r(prefix, "dns_new", tmp), trim_r(value));
		else
			nvram_set(strcat_r(prefix, "dns_new", tmp), "");
		if ((value = getenv("wins")))
			nvram_set(strcat_r(prefix, "wins_new", tmp), trim_r(value));
		else
			nvram_set(strcat_r(prefix, "wins_new", tmp), "");
		if ((value = getenv("domain")))
			nvram_set(strcat_r(prefix, "domain_new", tmp), trim_r(value));
		else
			nvram_set(strcat_r(prefix, "domain_new", tmp), "");
		if ((value = getenv("lease")))
		{
			nvram_set(strcat_r(prefix, "lease_new", tmp), trim_r(value));
			expires(lan_ifname, atoi(value));
		}
		else
			nvram_set(strcat_r(prefix, "lease_new", tmp), "");
		nvram_set("lan_ifname_new", lan_ifname);
		nvram_set("lan_udhcpstate_new", udhcpstate);

		fprintf(stderr, "Get lease %s/%s from %s\n",
			nvram_safe_get("lan_ipaddr_new"),
			nvram_safe_get("lan_netmask_new"),
			nvram_safe_get("lan_gateway_new"));
	}

	if (nvram_match("sw_mode_ex", "2"))
	{
		if (!kill_pidfile_s("/var/run/udhcpd-br0.pid", SIGUSR1))
			kill_pidfile_s("/var/run/udhcpd-br0.pid", SIGTERM);
	}

	if (nvram_match("sw_mode_ex", "2") && nvram_match("ui_triggered", "1"))
	{	// apply dhcp lease in case RT-N12 repeater mode UI does not
                char *dhcpc_argv[] = {"dhcpc_apply_delayed", NULL};
                pid_t pid;

                _eval(dhcpc_argv, NULL, 0, &pid);
	}
	else
	{
		if (nvram_match("sw_mode_ex", "2"))
		{
			nvram_set("lan_ipaddr_now", nvram_safe_get("lan_ipaddr_new"));
			nvram_set("lan_netmask_now", nvram_safe_get("lan_netmask_new"));
			nvram_set("lan_gateway_now", nvram_safe_get("lan_gateway_new"));
			nvram_set("lan_dns_now", nvram_safe_get("lan_dns_new"));
			nvram_set("lan_wins_now", nvram_safe_get("lan_wins_new"));
			nvram_set("lan_domain_now", nvram_safe_get("lan_domain_new"));
			nvram_set("lan_lease_now", nvram_safe_get("lan_lease_new"));

			nvram_unset("lan_ipaddr_new");
			nvram_unset("lan_netmask_new");
			nvram_unset("lan_gateway_new");
			nvram_unset("lan_dns_new");
			nvram_unset("lan_wins_new");
			nvram_unset("lan_domain_new");
			nvram_unset("lan_lease_new");

			nvram_set("lan_ipaddr_t", nvram_safe_get("lan_ipaddr_now"));
			nvram_set("lan_netmask_t", nvram_safe_get("lan_netmask_now"));
			nvram_set("lan_gateway_t", nvram_safe_get("lan_gateway_now"));
			nvram_set("lan_dns_t", nvram_safe_get("lan_dns_now"));
			nvram_set("lan_wins_t", nvram_safe_get("lan_wins_now"));
			nvram_set("lan_domain_t", nvram_safe_get("lan_domain_now"));
			nvram_set("lan_lease_t", nvram_safe_get("lan_lease_now"));
		}

		ifconfig(lan_ifname, IFUP,
			 nvram_safe_get(strcat_r(prefix, "ipaddr_t", tmp)),
			 nvram_safe_get(strcat_r(prefix, "netmask_t", tmp)));

		lan_up_ex(lan_ifname);

		logmessage("dhcp client", "%s IP : %s from %s", 
			udhcpstate, 
			nvram_safe_get(strcat_r(prefix, "ipaddr_t", tmp)), 
			nvram_safe_get(strcat_r(prefix, "gateway_t", tmp)));

//		wanmessage("");
	}
#else
        if ((value = getenv("ip")))
        {
                inet_aton(value, &fixed_addr);
                value = inet_ntoa(fixed_addr);
                nvram_set(strcat_r(prefix, "ipaddr_t", tmp), value);
        }
        if ((value = getenv("subnet")))
                nvram_set(strcat_r(prefix, "netmask_t", tmp), value);
        if ((value = getenv("router")))
                nvram_set(strcat_r(prefix, "gateway_t", tmp), value);
        if ((value = getenv("dns")))
                nvram_set(strcat_r(prefix, "dns_t", tmp), value);
        if ((value = getenv("wins")))
                nvram_set(strcat_r(prefix, "wins_t", tmp), value);
//      if ((value = getenv("hostname")))
//              sethostname(value, strlen(value) + 1);
        if ((value = getenv("domain")))
                nvram_set(strcat_r(prefix, "domain_t", tmp), value);
        if ((value = getenv("lease"))) {
                nvram_set(strcat_r(prefix, "lease_t", tmp), value);
                expires(lan_ifname, atoi(value));
        }

        ifconfig(lan_ifname, IFUP,
                 nvram_safe_get(strcat_r(prefix, "ipaddr_t", tmp)),
                 nvram_safe_get(strcat_r(prefix, "netmask_t", tmp)));

        lan_up_ex(lan_ifname);

        logmessage("dhcp client", "%s IP : %s from %s",
                udhcpstate,
                nvram_safe_get(strcat_r(prefix, "ipaddr_t", tmp)),
                nvram_safe_get(strcat_r(prefix, "gateway_t", tmp)));

        //wanmessage("");
#endif
	dprintf("done\n");
	return 0;
}

/*
 * renew: This argument is used when a DHCP lease is renewed. All of
 * the paramaters are set in enviromental variables. This argument is
 * used when the interface is already configured, so the IP address,
 * will not change, however, the other DHCP paramaters, such as the
 * default gateway, subnet mask, and dns server may change.
 */
static int
renew(void)
{
	bound();

	dprintf("done\n");
	return 0;
}

static int
apply(void)	// step 2 of 2-step dhcp lease application for RT-N12 repeater mode UI
{
	char lan_ifname[5];
	char udhcpstate_local[8];

	if (	nvram_invmatch("sw_mode_ex", "2") ||
		nvram_invmatch("ui_triggered", "1") ||
		!nvram_get("lan_ipaddr_new") ||
		!nvram_get("lan_netmask_new") ||
		!nvram_get("lan_gateway_new"))
		return 0;

	kill_pidfile_s("/var/run/dhcpc_apply_delayed.pid", SIGTERM);	// kill pending process
	unlink("/var/run/dhcpc_apply_delayed.pid");

	nvram_set("ui_triggered", "0");

	nvram_set("lan_ipaddr_now", nvram_safe_get("lan_ipaddr_new"));
	nvram_set("lan_netmask_now", nvram_safe_get("lan_netmask_new"));
	nvram_set("lan_gateway_now", nvram_safe_get("lan_gateway_new"));
	nvram_set("lan_dns_now", nvram_safe_get("lan_dns_new"));
	nvram_set("lan_wins_now", nvram_safe_get("lan_wins_new"));
	nvram_set("lan_domain_now", nvram_safe_get("lan_domain_new"));
	nvram_set("lan_lease_now", nvram_safe_get("lan_lease_new"));
	strcpy(lan_ifname, nvram_safe_get("lan_ifname_new"));
	strcpy(udhcpstate_local, nvram_safe_get("lan_udhcpstate_new"));

	nvram_unset("lan_ipaddr_new");
	nvram_unset("lan_netmask_new");
	nvram_unset("lan_gateway_new");
	nvram_unset("lan_dns_new");
	nvram_unset("lan_wins_new");
	nvram_unset("lan_domain_new");
	nvram_unset("lan_lease_new");
	nvram_unset("lan_ifname_new");
	nvram_unset("lan_udhcpstate_new");

	nvram_set("lan_ipaddr_t", nvram_safe_get("lan_ipaddr_now"));
	nvram_set("lan_netmask_t", nvram_safe_get("lan_netmask_now"));
	nvram_set("lan_gateway_t", nvram_safe_get("lan_gateway_now"));
	nvram_set("lan_dns_t", nvram_safe_get("lan_dns_now"));
	nvram_set("lan_wins_t", nvram_safe_get("lan_wins_now"));
	nvram_set("lan_domain_t", nvram_safe_get("lan_domain_now"));
	nvram_set("lan_lease_t", nvram_safe_get("lan_lease_now"));

	eval("et", "robowr", "0x0", "0xf", "0x1e");	// link down switch ports

	fprintf(stderr, "Apply lease %s/%s from %s to %s\n",
		nvram_safe_get("lan_ipaddr_t"),
		nvram_safe_get("lan_netmask_t"),
		nvram_safe_get("lan_gateway_t"),
		lan_ifname);

	ifconfig(lan_ifname, IFUP,
		nvram_safe_get("lan_ipaddr_t"),
		nvram_safe_get("lan_netmask_t"));

	lan_up_ex(lan_ifname);

	logmessage("dhcp client", "%s IP : %s from %s", 
		udhcpstate_local,
		nvram_safe_get("lan_ipaddr_t"),
		nvram_safe_get("lan_netmask_t"));

	sleep(1);

	eval("et", "robowr", "0x0", "0xf", "0x0");	// link up switch ports

	return 0;
}

int
udhcpc_ex_main(int argc, char **argv)
{
	if (argv[1]) strcpy(udhcpstate, argv[1]);

	if (!argv[1])
		return EINVAL;
	else if (strstr(argv[1], "deconfig"))
		return deconfig();
	else if (strstr(argv[1], "bound"))
		return bound();
	else if (strstr(argv[1], "renew"))
		return renew();
	else if (strstr(argv[1], "leasefail"))
		return 0;
	else if (strstr(argv[1], "apply"))
		return apply();

	else return deconfig();
}
