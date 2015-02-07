#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <netconf.h>
#include <rc.h>
#include <shutils.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <wlutils.h>
#include <bcmnvram.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define WIF "eth1"
#define URE_PERIOD 4

struct itimerval itv;
unsigned char bssid[6];
char ure_mac[18];
int link_down_then_up_threshold = 1;
int wl_psk = 0;
int wl_connected = 0, wl_connected_old = 0;
int wl_connected_count = 0;
unsigned char SEND_NULLDATA[]={0x73, 0x65, 0x6e, 0x64, 0x5f, 0x6e, 0x75, 0x6c, 0x6c, 0x64, 0x61, 0x74, 0x61, 0x0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};

/*
int
match_subnet(const char *ip1, const char *sb1, const char *ip2, const char *sb2)
{
	struct in_addr in_addr_ip1, in_addr_sb1, in_addr_ip2, in_addr_sb2;
	struct in_addr in_addr_sb;

	if (sb1 && sb2 && ip1 && ip2)
	{
		fprintf(stderr, "ip1: %s\n", ip1);
		fprintf(stderr, "sb1: %s\n", sb1);
		fprintf(stderr, "ip2: %s\n", ip2);
		fprintf(stderr, "sb2: %s\n", sb2);
	}

	if (sb1 && sb2)
	{
		inet_aton(sb1, &in_addr_sb1);
		inet_aton(sb2, &in_addr_sb2);
	
		if (in_addr_sb1.s_addr < in_addr_sb2.s_addr)
			in_addr_sb.s_addr = in_addr_sb1.s_addr;
		else
			in_addr_sb.s_addr = in_addr_sb2.s_addr;
	}
	else
		return 0;

	if (ip1 && ip2)
	{
		inet_aton(ip1, &in_addr_ip1);
		inet_aton(ip2, &in_addr_ip2);

		if ((in_addr_ip1.s_addr & in_addr_sb1.s_addr) == (in_addr_ip2.s_addr & in_addr_sb2.s_addr))
			return 1;
		else
			return 0;
	}
	else
		return 0;
}

struct timeval tv_ref, tv_now;
void print_reftime()
{
	gettimeofday(&tv_now, NULL);
	if ((tv_now.tv_usec - tv_ref.tv_usec) >= 0)
		fprintf(stderr, "sec: %ld, msec: %ld\n", tv_now.tv_sec-tv_ref.tv_sec, (tv_now.tv_usec-tv_ref.tv_usec)/1000);
	else
		fprintf(stderr, "sec: %ld, msec: %ld\n", tv_now.tv_sec-tv_ref.tv_sec-1, (1000000+tv_now.tv_usec-tv_ref.tv_usec)/1000);
}
*/

void
alarmtimer_ure(unsigned long sec, unsigned long usec)
{
	itv.it_value.tv_sec  = sec;
	itv.it_value.tv_usec = usec;
	itv.it_interval = itv.it_value;
	setitimer(ITIMER_REAL, &itv, NULL);
}

int
is_URE()
{
	int wl_ure = 0;

	if (!wl_ioctl(WIF, WLC_GET_WET, &wl_ure, sizeof(wl_ure)))
	{	
		if (wl_ure)
			return 1;
	}

	return 0;
}

int
get_ure_status()
{
	struct maclist *authorized;
	int maclist_size;
	int max_sta_count = 128;
	int wl_authorized = 0;
	int wl_associated = 0;
	wlc_ssid_t wst = {0, ""};

	if (wl_ioctl(WIF, WLC_GET_SSID, &wst, sizeof(wst)))
		goto wl_ioctl_error;

	memset(ure_mac, 0x0, 18);
	if (!wl_ioctl(WIF, WLC_GET_BSSID, bssid, sizeof(bssid)))
	{
		if ( !(!bssid[0] && !bssid[1] && !bssid[2] && !bssid[3] && !bssid[4] && !bssid[5]) )
		{
			wl_associated = 1;
			sprintf(ure_mac, "%02X:%02X:%02X:%02X:%02X:%02X", (unsigned char)bssid[0],
									  (unsigned char)bssid[1],
									  (unsigned char)bssid[2],
									  (unsigned char)bssid[3],
									  (unsigned char)bssid[4],
									  (unsigned char)bssid[5]);
			memcpy(SEND_NULLDATA + 14, bssid, 6);
		}
	}
	else
		goto wl_ioctl_error;

	if (wl_psk)
	{
		maclist_size = sizeof(authorized->count) + max_sta_count * sizeof(struct ether_addr);
		authorized = malloc(maclist_size);

		if (authorized)
		{
			// query wl for authorized sta list
			strcpy((char*)authorized, "autho_sta_list");

			if (!wl_ioctl(WIF, WLC_GET_VAR, authorized, maclist_size))
			{
				if (authorized->count > 0) wl_authorized = 1;
				free(authorized);
			}
			else
			{
				free(authorized);
				goto wl_ioctl_error;
			}
		}
	}

	if (wl_associated && !strncmp(wst.SSID, nvram_safe_get("wl0_ssid"), wst.SSID_len))
	{
		if (wl_psk)
		{
			if (wl_authorized)
			{
				fprintf(stderr, "Connected with: \"%s\" (%s)\n", nvram_safe_get("wl0_ssid"), ure_mac);
				return 2;
			}
			else
			{
				fprintf(stderr, "Connecting to \"%s\" (%s)\n", nvram_safe_get("wl0_ssid"), ure_mac);
				return 1;
			}
		}
		else
		{
			fprintf(stderr, "Connected with: \"%s\" (%s)\n", nvram_safe_get("wl0_ssid"), ure_mac);
			return 2;
		}
	}
	else
	{
		fprintf(stderr, "Disconnected...\n");
		return 0;
	}

wl_ioctl_error:
	fprintf(stderr, "wl_ioctl() error\n");
	return 0;
}

int
dhcpc_apply_delayed()
{
	FILE *fp;

	/* write pid */
	if ((fp = fopen("/var/run/dhcpc_apply_delayed.pid", "w")) != NULL)
	{
		fprintf(fp, "%d", getpid());
		fclose(fp);
	}

	sleep(5);
	eval("/tmp/landhcpc", "apply");

	return 0;
}

void catch_sig_ure(int sig)
{
	if (sig == SIGUSR1)
	{
		alarmtimer_ure(0, 0);

		sleep(1);
		eval("/tmp/landhcpc", "apply");

		alarm(URE_PERIOD * 2);
	}
	else if (sig == SIGTSTP)
	{
		alarmtimer_ure(0, 0);

		kill_pidfile_s("/var/run/dhcpc_apply_delayed.pid", SIGTERM);	// kill pending process
		unlink("/var/run/dhcpc_apply_delayed.pid");

//		alarm(URE_PERIOD);
	}
	else if (sig == SIGALRM)
	{
		wl_connected_old = wl_connected;
		wl_connected = get_ure_status();

		if (wl_connected)
		{
			if (wl_connected_count++ >= 10)
			{
				wl_connected_count = 0;
				wl_ioctl(WIF, WLC_SET_VAR, SEND_NULLDATA, sizeof(SEND_NULLDATA));	// wl send_nulldata xx:xx:xx:xx:xx:xx
			}	

			if (wl_connected != wl_connected_old)
			{
				fprintf(stderr, "Wireless link down & up!\n");

				if (link_down_then_up_threshold)
				{
					link_down_then_up_threshold--;
					if (!kill_pidfile_s("/var/run/udhcpd-br0.pid", SIGUSR1))
						kill_pidfile_s("/var/run/udhcpd-br0.pid", SIGTERM);
				}
				else
				{
					fprintf(stderr, "perform dhcp lease release\n");
					kill_pidfile_s("/var/run/udhcpc_lan.pid", SIGUSR2);	// perform lease release
					fprintf(stderr, "perform dhcp lease renew\n");
					kill_pidfile_s("/var/run/udhcpc_lan.pid", SIGUSR1);	// perform lease renew
				}
			}
		}
		else
			wl_connected_count = 0;

		alarm(URE_PERIOD);
	}
}

int
ure_monitor()
{
	if (nvram_match("wl0_ssid", "") || !is_URE())
		return 0;

	FILE *fp;

	/* write pid */
	if ((fp = fopen("/var/run/ure_monitor.pid", "w")) != NULL)
	{
		fprintf(fp, "%d", getpid());
		fclose(fp);
	}

	wl_psk = strstr(nvram_safe_get("wl0_akm"), "psk") ? 1 : 0;

	if (nvram_match("wl0_akm", "psk") || nvram_match("wl0_crypto", "tkip+aes"))
		link_down_then_up_threshold = 2;
	fprintf(stderr, "link_down_then_up_threshold: %d\n", link_down_then_up_threshold);

	signal(SIGUSR1, catch_sig_ure);
	signal(SIGALRM, catch_sig_ure);
	signal(SIGTSTP, catch_sig_ure);

	alarm(1);

	while (1)
	{
		pause();
	}

	return 0;
}

int
start_ure_monitor()
{
	char *ure_monitor_argv[] = {"ure_monitor", NULL};
	int umpid;

	return _eval(ure_monitor_argv, NULL, 0, &umpid);
}
