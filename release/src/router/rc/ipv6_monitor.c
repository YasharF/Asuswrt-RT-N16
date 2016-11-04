/*
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 *
 * Copyright 2012, ASUSTeK Inc.
 * All Rights Reserved.
 *
 * THIS SOFTWARE IS OFFERED "AS IS", AND ASUS GRANTS NO WARRANTIES OF ANY
 * KIND, EXPRESS OR IMPLIED, BY STATUTE, COMMUNICATION OR OTHERWISE. BROADCOM
 * SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A SPECIFIC PURPOSE OR NONINFRINGEMENT CONCERNING THIS SOFTWARE.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>
#include <bcmnvram.h>
#include <shutils.h>
#include <shared.h>
#include <signal.h>

#define NORMAL_PERIOD	1
#define THRESHOOLD	12

int count, once;

static void
ipv6_monitor_exit(int sig)
{
	if (sig == SIGTERM)
	{
		remove("/var/run/ipv6_monitor.pid");
		exit(0);
	}
}

static void
ipv6_monitor(int sig)
{
	char buf[134];

	if (sig == SIGALRM)
	{
		if (getifaddr(get_wan6face(), AF_INET6, GIF_PREFIXLEN))
		{
			if (once)
			{
				snprintf(buf, sizeof(buf), "/var/run/odhcp6c.%s.pid", get_wan6face());
				kill_pidfile_s(buf, SIGTSTP);
			}

			alarm(0);
			ipv6_monitor_exit(SIGTERM);
		}
		else
		{
			if (!once && ++count > THRESHOOLD)
			{
				once = 1;
				snprintf(buf, sizeof(buf), "/var/run/odhcp6c.%s.pid", get_wan6face());
				kill_pidfile_s(buf, SIGALRM);
			}

			alarm(NORMAL_PERIOD);
		}
	}
}

int
ipv6_monitor_main(int argc, char *argv[])
{
	FILE *fp;
	sigset_t sigs_to_catch;

	char tmp[100];
	char prefix[] = "wanXXXXXXXXXX_";
	char *wan_proto;

	snprintf(prefix, sizeof(prefix), "wan%d_", wan_primary_ifunit());
	wan_proto = nvram_get(strcat_r(prefix, "proto", tmp));

	if (!(strcmp(wan_proto, "dhcp") && strcmp(wan_proto, "static") &&
		nvram_match(ipv6_nvname("ipv6_ifdev"), "ppp") &&
		(get_ipv6_service() == IPV6_NATIVE_DHCP || get_ipv6_service() == IPV6_MANUAL))) {
		return 0;
	}

	/* write pid */
	if ((fp = fopen("/var/run/ipv6_monitor.pid", "w")) != NULL)
	{
		fprintf(fp, "%d", getpid());
		fclose(fp);
	}

	count = once = 0;

	/* set the signal handler */
	sigemptyset(&sigs_to_catch);
	sigaddset(&sigs_to_catch, SIGALRM);
	sigaddset(&sigs_to_catch, SIGTERM);
	sigprocmask(SIG_UNBLOCK, &sigs_to_catch, NULL);

	signal(SIGALRM, ipv6_monitor);
	signal(SIGTERM, ipv6_monitor_exit);

	alarm(NORMAL_PERIOD);

	/* Most of time it goes to sleep */
	while (1)
	{
		pause();
	}

	return 0;
}
