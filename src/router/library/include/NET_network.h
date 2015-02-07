/*
 * ----------------------------------------------------------------------
 * ChangeList:
 * 2000/12/19			Vincent 002
 *   Add _SOHO_SMC_, _SOHO_LINKSYS_ define for default IP address.
 * Linksys: 192.168.1.1, SMC: 192.168.123.254
 * 2000/12/18			Vincent 001
 *   Add _LINKSYS_G2_ define. If LINKSYS G2 server, use LKGxxxxxx as its default
 * server name. (Also default printer name)
 * ----------------------------------------------------------------------
 */
#ifndef _NET_TCPIP_
#define _NET_TCPIP_

/* ---------- Global Parameter ----------*/
				/* ---- Error Number ---- */
#ifndef _NET_ERRNO_
#define _NET_ERRNO_

#define NET_OK            0     	/* action succeed */
#define NET_ERROR        -1		/* action fail */
#define NET_EFILEOPEN    -2		/* file open error */
#define NET_EFILEREAD    -3     	/* file read error */
#define NET_EFILEWRITE   -4     	/* file write error */
#define NET_ESECTION     -5		/* input section error */
#define NET_EFIELD       -6		/* input field error */
#define NET_ELEN         -7		/* input length error */
#define NET_EIPADDR      -8     	/* ip address invalid */
#define NET_ENETMASK     -9     	/* netmask invalid */
#define NET_EBOOTTYPE    -10    	/* boot type invalid */
#define NET_ESOCKET      -11    	/* open socket fail */

#endif


				/* ---- Global Variable ---- */
#include "NET_global.h"		/* our global definition */

/* ---------- Default Interface ---------- */
#ifndef _NAT_FUNCTION_
#define NET_DEVICE_LAN "ixp0"
#else
#define NET_DEVICE_LAN "eth1"	/* SOHO Server */
#define NET_DEVICE_WAN "eth0"	/* SOHO Server */
#define NET_DEVICE_PPP "ppp0"	/* PPPoE WAN */
#define DEF_NET_DNS1   "168.95.192.1" /* Content Filter need DNS */
#endif
#define NET_DEVICE     NET_DEVICE_LAN

/* ---------- Global Length Declaration ----------*/
#define STR_HWADDR_LEN    17    	/* 00:C0:02:12:34:AB */

#define  _LINKSYS_G2_
				/* ---- TCP/IP Varable ---- */
/* ---- Global Declaration ---- */
				/* -- default server name definition -- */
#ifdef _LINKSYS_G2_		/* Tony 001 - add #ifdef _LINKSYS_G2_ */
#define DEF_NAME_LEN 9		/* default server name length: LKGxxxxxx */
#else
#define DEF_NAME_LEN 8		/* default server name length: SCxxxxxx */
#endif

				/* -- hardware id definition -- */
				/* Total = HWID(12) + '-'(1) + FWVERSION(4) */
#define STR_HWID_LEN 17		/* HWID = 303038353030-0100 */

				/* boot type */
#define   STR_BOOT_DHCP   "dhcp"   /* string type: dhcp */
#define    DS_BOOT_FIXIP  "static" /* our configure file use "static" */
#define   INT_BOOT_FIXIP   1   	   /* integer type: fix ip */
#define   INT_BOOT_DHCP    2   	   /* integer type: dhcp client */

				/* default value */
#ifdef _SOHO_SMC_		/* Vincent 002 - add #ifdef _SOHO_SMC_ */
	#define DEF_NET_IPADDR         "192.168.123.254"
#else
	#ifdef _SOHO_LINKSYS_		/* Vincent 002 - add #ifdef _SOHO_LINKSYS_ */
		#define DEF_NET_IPADDR         "192.168.1.1"
	#else
		#define DEF_NET_IPADDR         "192.168.1.77"
	#endif                         /* Vincent 002 - add #ifdef _SOHO_LINKSYS_ */
#endif                         /* Vincent 002 - add #ifdef _SOHO_SMC_ */
#define DEF_NET_NETMASK        "255.255.255.0"
#define DEF_NET_BOOTPROTO      "static"

/* ---------- Structure Declaration ---------- */

/* ----- TCP/IP information ----- */
#define NET_DEVICE_LEN      16		/* linux support up to 16 */
#define NET_BOOTTYPE_LEN    6		/* dhcp or none/static */

#define INET_IPADDR_LEN     4        	/* unsigned long */

typedef struct {
	char INTERFACE[NET_DEVICE_LEN+1];    /* lan_interface */
	char HWADDR[STR_HWADDR_LEN+1];       /* hw_addr */
	char IPADDR[STR_IPADDR_LEN+1];       /* ip_addr */
	char NETMASK[STR_IPADDR_LEN+1];      /* netmask */
	char GATEWAY[STR_IPADDR_LEN+1];      /* gateway */
	char BOOTPROTO[NET_BOOTTYPE_LEN+1];  /* bootproto */
} TCPIPInfo;


/* ----- Hostname information ----- */
#define HOSTNAME_LEN   14

typedef struct {
	char HOSTNAME[HOSTNAME_LEN+1];
} HOSTInfo;


/* ----- DNS information ----- */
#define MAX_DNS_CNT     3	/* we only support 3 DNS data */
#define STR_DOMAIN_LEN 80	/* current domain name: dhcp client use it */

typedef struct {
	char DOMAIN[STR_DOMAIN_LEN+1];       /* domain_name */
	char DNS1[STR_IPADDR_LEN+1];         /* dns_server1 */
	char DNS2[STR_IPADDR_LEN+1];         /* dns_server2 */
	char DNS3[STR_IPADDR_LEN+1];         /* dns_server3 */
} DNSInfo;


/* ----- Inet Daemon information ----- */
#define INET_TELNET_NAME   "telnet"
#define INET_TELNET_SOCK   "stream"
#define INET_TELNET_PROTO  "tcp"
#define INET_TELNET_FLAGS  "nowait"
#define INET_TELNET_USER   "root"
#define INET_TELNET_PATH   "/usr/sbin/tcpd"
#define INET_TELNET_ARGV   "in.telnetd"


typedef struct {
	char TELNET[STR_BOOLEAN_LEN+1];       /* telnet_enable */
} INETSERVInfo;


/* ---- DHCP Server information ----- */
typedef struct {
	char DHCPSERVER[STR_BOOLEAN_LEN+1];        /* dhcp_server */
	char DHCPSIP[STR_IPADDR_LEN+1];            /* dhcp_start_ip */
	char DHCPEIP[STR_IPADDR_LEN+1];            /* dhcp_end_ip */
	char DHCPWINSSERVER[STR_IPADDR_LEN+1];     /* wins_server */
} DHCPSERVInfo;

/* ---- DHCP Server Lease information ---- */

#define CHOSTNAME_LEN 17

#ifndef	TIME_LEN
#define TIME_LEN      19             /* YYYY/MM/DD HH:MM:SS */
#endif

typedef struct Lease{
	char IP_ADDR[STR_IPADDR_LEN+1];
	char MAC_ADDR[STR_HWADDR_LEN+1];
	char HOSTNAME[CHOSTNAME_LEN+1];
	char ENDS_TIME[TIME_LEN+1];
        struct Lease *next;
}Lease;

/* ---------- Export Function ---------- */

/* ----------------------------------------------------------------------*/
/* purpose:Set TCP/IP network                                            */
/* return : 0 -success                                                   */
/*          others - failed                                              */
/* ----------------------------------------------------------------------*/
int NETSetTCPIPNetwork(TCPIPInfo *info, DNSInfo *dns);

/* ----------------------------------------------------------------------*/
/* purpose: Read /etc/resolv.conf                                        */
/* return : 0 -success                                                   */
/*          others - failed                                              */
/* ----------------------------------------------------------------------*/
int NETReadCurrentDNSData(DNSInfo *info);

/* -----------------------------------------------------------------------*/
/* purpose : Write TCPIP data to Linux related files (ifcfg-eth0, network)*/
/* return : 0 -success                                                    */
/*          others - failed                                               */
/* ---------------------------------------------------------------------- */
int NETWriteNetworkDNSData(DNSInfo *info, int type);

/* -----------------------------------------------------------------------*/
/* purpose : Write Default Server Name to Linux related file              */
/* /var/log/HOSTNAME                                                      */
/* return : 0 -success                                                    */
/*          others - failed                                               */
/* ---------------------------------------------------------------------- */
int NETWriteDefaultHostnameData(void);

/* ------------------------------------------------------------------------*/
/* purpose : Set Disk Server Name                                          */
/* return : 0 -success                                                     */
/*          others - failed                                                */
/*-------------------------------------------------------------------------*/
int NETSetHostname(HOSTInfo *info);

/* ------------------------------------------------------------------------*/
/* purpose : Set Inet Daemon information                                   */
/* return : 0 -success                                                     */
/*          others - failed                                                */
/* Note : Enable/Disable Telnet Server, pass "enable" or "disable"         */
/*        If pass "yes" or "no" ,it will return NET_ERROR(-1)              */
/*-------------------------------------------------------------------------*/
int NETSetInetService(INETSERVInfo *info);

/* ------------------------------------------------------------------------*/
/* purpose : Set DHCP Server                                               */
/* return : 0 -success                                                     */
/*          others - failed                                                */
/* Note : Enable/Disable Netatalk , pass "enable" or "disable"             */
/*        If pass "yes" or "no" ,it will return NET_ERROR(-1)              */
/*-------------------------------------------------------------------------*/
int NETSetDHCPServer(TCPIPInfo *info, DNSInfo *dns, DHCPSERVInfo *dhcp);

/* ------------------------------------------------------------------------*/
/* purpose : Set ip_forward flag to relative Linux file                    */
/* return : 0 -success                                                     */
/*          others - failed                                                */
/* Note :   Enable/Disable NATFunction , pass 1 -enable or  0 -disable     */
/*-------------------------------------------------------------------------*/
int NETSetNATFunction(int type);

#endif
