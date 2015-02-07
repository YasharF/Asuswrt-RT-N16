/*
 * WPS OSL file
 *
 * Copyright (C) 2009, Broadcom Corporation
 * All Rights Reserved.
 * 
 * This is UNPUBLISHED PROPRIETARY SOURCE CODE of Broadcom Corporation;
 * the contents of this file may not be disclosed to third parties, copied
 * or duplicated in any form, in whole or in part, without the prior
 * written permission of Broadcom Corporation.
 *
 * $Id: wps_linux_osl.c,v 1.13 2009/11/19 10:18:47 Exp $
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <ctype.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <wlutils.h>
#include <tutrace.h>
#include <wpserror.h>
#include <wpscommon.h>
#include <wps_ap.h>
#include <ap_upnp_sm.h>

#include <shutils.h>
#include <wlif_utils.h>
#include <security_ipc.h>
#include <wps.h>

static char if_name[20] = "";

/*
 * AP mode only stub functions
 */
#if defined(BCMWPSAP) && !defined(BCMWPSAPSTA)
int
wpsenr_open_session(void *wps_app, char* ifname)
{
	return 0;
}
#endif /* defined(BCMWPSAP) && !defined(BCMWPSAPSTA) */


uint32
WpsHtonl(uint32 intlong)
{
	return htonl(intlong);
}

uint16
WpsHtons(uint16 intshort)
{
	return htons(intshort);
}

uint16
WpsHtonsPtr(uint8 * in, uint8 * out)
{
	uint16 v;
	uint8 *c;
	c = (uint8 *)&v;
	c[0] = in[0]; c[1] = in[1];
	v = htons(v);
	out[0] = c[0]; out[1] = c[1];
	return v;
}

uint32
WpsHtonlPtr(uint8 * in, uint8 * out)
{
	uint32 v;
	uint8 *c;
	c = (uint8 *)&v;
	c[0] = in[0]; c[1] = in[1]; c[2] = in[2]; c[3] = in[3];
	v = htonl(v);
	out[0] = c[0]; out[1] = c[1]; out[2] = c[2]; out[3] = c[3];
	return v;
}

uint32
WpsNtohl(uint8 *a)
{
	uint32 v;
	v = (a[0] << 24) + (a[1] << 16) + (a[2] << 8) + a[3];
	return v;
}

uint16
WpsNtohs(uint8 *a)
{
	uint16 v;
	v = (a[0]*256) + a[1];
	return v;
}

void
WpsSleep(uint32 seconds)
{
	sleep(seconds);
}

void
WpsSleepMs(uint32 ms)
{
	usleep(1000*ms);
}

/* OSL arp function */
int
wps_osl_arp_get(char *client_ip, unsigned char mac[6])
{
	FILE *arpfile;

	char proc_net_arp[] = "/proc/net/arp";
	char titlestr[100], ipaddrstr[20], hwtypestr[10], flagstr[6],
		hwaddrstr[20], maskstr[6], devicestr[20];

	/* Get from arp table for ip to mac */
	if (!(arpfile = fopen(proc_net_arp, "r")) || !fgets(titlestr, 100, arpfile)) {
		if (arpfile)
			fclose(arpfile);

		TUTRACE((TUTRACE_ERR, "can't open /proc/net/arp!!\n"));
		return -1;
	}

	/* parsing /proc/net/arp contents to get desired client mac */
	while (fscanf(arpfile, "%s\t%s\t%s\t%s\t%s\t%s\n",
		ipaddrstr, hwtypestr, flagstr, hwaddrstr, maskstr, devicestr) != EOF) {

		if (!strcasecmp(client_ip, ipaddrstr)) {
			ether_atoe(hwaddrstr, mac);
			TUTRACE((TUTRACE_INFO,
				"SSR client mac = %02x:%02x:%02x:%02x:%02x:%02x!!\n",
				mac[0], mac[1], mac[2],
				mac[3], mac[4], mac[5]));
			break;
		}
	}

	fclose(arpfile);

	return 0;
}

/* Network IPC part code */
static int
open_udp_socket(char *addr, uint16 port)
{
	int reuse = 1;
	int sock_fd;
	struct sockaddr_in sockaddr;

	/* open loopback socket to communicate with EAPD */
	memset(&sockaddr, 0, sizeof(sockaddr));
	sockaddr.sin_family = AF_INET;
	sockaddr.sin_addr.s_addr = inet_addr(addr);
	sockaddr.sin_port = htons(port);

	if ((sock_fd = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
		TUTRACE((TUTRACE_ERR, "Unable to create loopback socket\n"));
		goto exit0;
	}
	if (setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, (char*)&reuse, sizeof(reuse)) < 0) {
		TUTRACE((TUTRACE_ERR, "Unable to setsockopt to loopback socket %d.\n", sock_fd));
		goto exit1;
	}
	if (bind(sock_fd, (struct sockaddr *)&sockaddr, sizeof(sockaddr)) < 0) {
		TUTRACE((TUTRACE_ERR, "Unable to bind to loopback socket %d\n", sock_fd));
		goto exit1;
	}
	TUTRACE((TUTRACE_INFO, "opened loopback socket %d in port %d\n", sock_fd, port));
	return sock_fd;

	/* error handling */
exit1:
	close(sock_fd);

exit0:
	TUTRACE((TUTRACE_ERR, "failed to open loopback socket\n"));
	return -1;
}

static inline void
close_udp_socket(int fd)
{
	if (fd >= 0)
		close(fd);
}

/* Init eap socket */
int
wps_osl_eap_handle_init()
{
	int handle;

	/* open socket */
	handle = open_udp_socket(WPS_EAP_ADDR, EAPD_WKSP_WPS_UDP_MPORT);
	if (handle < 0) {
		TUTRACE((TUTRACE_ERR, "init eap handle error\n"));
		return -1;
	}

	return handle;
}

void
wps_osl_eap_handle_deinit(int handle)
{
	close_udp_socket(handle);
}


/* Init ui socket */
int
wps_osl_ui_handle_init()
{
	int handle;

	/* open socket */
	handle = open_udp_socket(WPS_UI_ADDR, WPS_UI_PORT);
	if (handle < 0) {
		TUTRACE((TUTRACE_ERR, "init ui handle error\n"));
		return -1;
	}

	return handle;
}

void
wps_osl_ui_handle_deinit(int handle)
{
	close_udp_socket(handle);
}

/* Init upnp socket */
int
wps_osl_upnp_handle_init(int instance)
{
	int handle;

	/* open socket */
	handle = open_udp_socket(WPS_UPNPDEV_ADDR, WPS_UPNPDEV_PORT + instance);
	if (handle < 0) {
		TUTRACE((TUTRACE_ERR, "init upnp if instance %d handle error\n", instance));
		return -1;
	}

	return handle;
}

void
wps_osl_upnp_handle_deinit(int handle)
{
	close_udp_socket(handle);
}

/* Receive packet */
static struct sockaddr_in dst;

wps_hndl_t *
wps_osl_wait_for_all_packets(char *dataBuffer, uint32 *dataLen, wps_hndl_t *hndl_list)
{
	wps_hndl_t *hndl;
	int n, max_fd = -1;
	fd_set fdvar;
	struct timeval timeout;

	struct iovec iov[2];
	struct msghdr msg;
	int bytes;
	int num;


	if (dataBuffer && (! dataLen)) {
		TUTRACE((TUTRACE_ERR, "wpsm_readData: arg err!\n"));
		return NULL;
	}

	timeout.tv_sec = 1;
	timeout.tv_usec = 0;

	FD_ZERO(&fdvar);

	/* Set FD */
	hndl = hndl_list;
	while (hndl) {
		FD_SET(hndl->handle, &fdvar);
		if (max_fd < hndl->handle)
			max_fd = hndl->handle;

		hndl = hndl->next;
	}

	if (max_fd == -1) {
		TUTRACE((TUTRACE_ERR, "wpsm_readData: no fd set!\n"));
		return NULL;
	}

	/* Do select */
	n = select(max_fd + 1, &fdvar, NULL, NULL, &timeout);
	if (n <= 0) {
		/*
		 * to avoid the select operation interferenced by led lighting timer.
		 * this will be removed after led lighting timer is replaced by wireless driver
		 */
		if (n < 0 && errno != EINTR) {
			TUTRACE((TUTRACE_ERR, "wpsm_readData: select recv failed\n"));
		}
		return NULL;
	}

	/* Poll which handle received data */
	hndl = hndl_list;
	while (hndl) {
		if (FD_ISSET(hndl->handle, &fdvar)) {
			/*
			 * Specially hacking eap handle to
			 * retrieve eap wl ifname.
			 */
			num = 0;
			if (hndl->type == WPS_RECEIVE_PKT_EAP) {
				iov[num].iov_base = hndl->ifname;
				iov[num].iov_len = sizeof(hndl->ifname);
				num++;
			}
			iov[num].iov_base = (void *)dataBuffer;
			iov[num].iov_len = *dataLen;

			/* Setup message */
			memset(&dst, 0, sizeof(dst));
			memset(&msg, 0, sizeof(msg));
			msg.msg_name = (void *)&dst;
			msg.msg_namelen = sizeof(dst);
			msg.msg_iov = iov;
			msg.msg_iovlen = num+1;

			/* Read this message */
			bytes = recvmsg(hndl->handle, &msg, 0);
			bytes -= num * sizeof(hndl->ifname);
			if (bytes <= 0) {
				TUTRACE((TUTRACE_ERR,
					"recv failed, recvBytes = %d\n", bytes));
				return NULL;
			}

			*dataLen = bytes;
			return hndl;
		}

		hndl = hndl->next;
	}

	return NULL;
}

/*
 * WPS EAP Packet send functions
 */
static uint32
Eapol_SendDataDown(char * dataBuffer, uint32 dataLen)
{
	struct msghdr mh;
	struct iovec iov[2];
	struct sockaddr_in to;
	int eap_fd = wps_eap_get_handle();

	TUTRACE((TUTRACE_INFO, "Send EAPOL... buffer Length = %d\n",
		dataLen));

	if (!dataBuffer || !dataLen) {
		TUTRACE((TUTRACE_ERR, "Invalid Parameters\n"));
		return WPS_ERR_INVALID_PARAMETERS;
	}

	/* send to EAPD */
	to.sin_addr.s_addr = inet_addr(EAPD_WKSP_UDP_ADDR);
	to.sin_family = AF_INET;
	to.sin_port = htons(EAPD_WKSP_WPS_UDP_RPORT);

	/* outgoing interface name */
	iov[0].iov_base = (void *)if_name;
	iov[0].iov_len = IFNAMSIZ;
	iov[1].iov_base = dataBuffer;
	iov[1].iov_len = dataLen;

	memset(&mh, 0, sizeof(mh));
	mh.msg_name = (void *)&to;
	mh.msg_namelen = sizeof(to);
	mh.msg_iov = iov;
	mh.msg_iovlen = 2;

	if (sendmsg(eap_fd, &mh, 0) < 0) {
		TUTRACE((TUTRACE_ERR, "L2 send failed\n"));
		return TREAP_ERR_SENDRECV;
	}

	return WPS_SUCCESS;
}

uint32
wps_eapol_send_data(char *dataBuffer, uint32 dataLen)
{
	return (Eapol_SendDataDown(dataBuffer, dataLen));
}

char *
wps_eapol_parse_msg(char *msg, int msg_len, int *len)
{
	/* just return msg and msg_len, the msg is a eapol raw date */
	*len = msg_len;
	return msg;
}

/*
 * WPS UI Packet send functions
 */
int
wps_osl_send_uimsg(wps_hndl_t *hndl, char *buf, int len)
{
	int n;

	n = sendto(hndl->handle, buf, len, 0, (struct sockaddr *)&dst,
		sizeof(struct sockaddr_in));

	/* Sleep 100 ms to make sure
	 * WPS have received socket
	 */
	usleep(100*1000);
	return n;
}

int
wps_set_ifname(char *ifname)
{
	strcpy(if_name, ifname);
	return 0;
}


/* UPnP packet send functions */
/*
 * Debug hex dump
 */
static void
Upnp_HexDumpAscii(const char *func, int type, const unsigned char *buf, unsigned int len)
{
#ifdef _TUDEBUGTRACE
	int i, llen;
	const unsigned char *pos = buf;
	const int line_len = 16;

	const char *title;

	switch (type)
	{
	case UPNP_WPS_TYPE_SSR:
		title = "UPNP_WPS_TYPE_SSR";
		break;

	case UPNP_WPS_TYPE_PMR:
		title = "UPNP_WPS_TYPE_PMR";
		break;

	case UPNP_WPS_TYPE_GDIR:
		title = "UPNP_WPS_TYPE_GDIR";
		break;

	case UPNP_WPS_TYPE_PWR:
		title = "UPNP_WPS_TYPE_PWR";
		break;

	default:
		title = "UNKNOW TYPE";
		break;
	}

	printf("WPS %s-%s - hexdump_ascii(len=%lu):\n", func, title, (unsigned long) len);
	while (len) {
		llen = len > line_len ? line_len : len;
		printf("    ");
		for (i = 0; i < llen; i++)
			printf(" %02x", pos[i]);
		for (i = llen; i < line_len; i++)
			printf("   ");
		printf("   ");
		for (i = 0; i < llen; i++) {
			if (isprint(pos[i]))
				printf("%c", pos[i]);
			else
				printf("*");
		}
		for (i = llen; i < line_len; i++)
			printf(" ");
		printf("\n");
		pos += llen;
		len -= llen;
	}
#else
	return;

#endif /* _TUDEBUGTRACE */
}

int
wps_osl_upnp_send(int if_instance, UPNP_WPS_CMD *cmd, char *data)
{
	struct sockaddr_in to;
	struct iovec iov[2];
	struct msghdr msg;
	int fd;

	TUTRACE((TUTRACE_INFO, "In %s buffer Length = %d\n", __FUNCTION__, cmd->length));

	Upnp_HexDumpAscii(__FUNCTION__, cmd->type, data, cmd->length);

	/* Create socket */
	fd = socket(AF_INET, SOCK_DGRAM, 0);
	if (fd < 0) {
		TUTRACE((TUTRACE_ERR, "Cannot open socket\n"));
		return WPS_ERR_INVALID_PARAMETERS;
	}

	/* Construct msg */
	iov[0].iov_base = (void *)cmd;
	iov[0].iov_len = UPNP_WPS_CMD_SIZE;
	iov[1].iov_base = (void *)data;
	iov[1].iov_len = cmd->length;

	to.sin_family = AF_INET;
	to.sin_port = htons(UPNP_WFA_PORT + if_instance);
	to.sin_addr.s_addr = inet_addr(WPS_LOOPBACK_ADDR);

	memset(&msg, 0, sizeof(msg));
	msg.msg_name = (void *)&to;
	msg.msg_namelen = sizeof(to);
	msg.msg_iov = iov;
	msg.msg_iovlen = 2;

	if (sendmsg(fd, &msg, 0) < 0) {
		close(fd);
		TUTRACE((TUTRACE_ERR, "Sending message to UPnP failed\n"));
		return WPS_ERR_SYSTEM;
	}

	close(fd);
	return WPS_SUCCESS;
}

/* New WPS IPC command */
int
wps_osl_upnp_ipc(int if_instance, char *buf, char *response, int *len)
{
	int s, send_len;
	struct sockaddr_in to;

	/* Create socket */
	s = socket(AF_INET, SOCK_DGRAM, 0);
	if (s < 0)
		return -1;

	/* Set destination to send */
	memset(&to, 0, sizeof(to));
	to.sin_family = AF_INET;
	to.sin_port = htons(UPNP_IPC_PORT + if_instance);
	to.sin_addr.s_addr = inet_addr(UPNP_IPC_ADDR);

	send_len = strlen(buf)+1;
	sendto(s, buf, send_len, 0, (struct sockaddr *)&to, sizeof(to));

	if (response) {
		*len = recv(s, response, *len, 0);
	}

	close(s);
	return 0;
}
