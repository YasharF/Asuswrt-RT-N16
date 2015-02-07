/*
 * ----------------------------------------------------------------------
 * ChangeList:
 * 2001/01/15			Vincent 001
 *   Implementation Change: We merge all configuration file into one.
 * Read /etc/CGI_ds.conf with different section. [NAT], [WANPORT]
 * ----------------------------------------------------------------------
 */
#ifndef _NET_NETWORK_
#define _NET_NETWORK_

/* ---------- Global Parameter ----------*/
#include "NET_network.h"


/* ---------- Linux File Declaration ---------- */

				/* files: configuration */
#define DS_CONF "/etc/CGI_ds.conf"         /* Disk Server's configure file */
#define IF_CONF "/etc/sysconfig/network-scripts/ifcfg-"	    /* + NET_DEVICE */
#define NETWORK_CONF "/etc/sysconfig/network"               /* Gateway */
#define DNS_CONF "/etc/resolv.conf"                         /* DNS */
#define INETD_CONF "/usr/local/xinetd.d/telnet"  	    /* Telnet */
#define DEF_HOST_CONF "/var/log/DEFHOSTNAME"         /* default server name */
#define ETC_HOSTS_CONF "/etc/hosts"        /*  linux use this for IP lookp */
#define ETC_HOST_CONF "/etc/HOSTNAME"
#define DHCPD_LOCK_CONF "/var/lock/subsys/dhcpd" /* dhcpd start lock flag */
#define DHCPD_PID_CONF "/var/run/udhcpd.pid"  /* dhcpd really active flag */
#define DHCPD_DATA_CONF "/etc/udhcpd.conf"     /* main configure file */
#define DHCPD_LEASE_CONF "/etc/udhcpd.leases"   /* lease file, must exist */
#ifdef _TCPIP_OUTPUT_
#define IF_OUTFILE_1 "/harddisk/hd~2/data/IPData"   /* current tcp/ip info */
#define IF_OUTFILE_2 "/harddisk/hd~3/data/IPData"
#endif 

				/* files: executive */
#ifndef _OFF_STDOUT_
#define IFUP_EXE "/sbin/ifup"                         /* network */
#define IFDOWN_EXE "/sbin/ifdown"                     /* network */
#define GATEWAY_EXE "/sbin/route"                     /* gateway */
#define HOSTNAME_EXE "/bin/hostname"                  /* server name */
#define INETSERV_EXE "/etc/rc.d/rc.xinetd"            /* inet daemon */
#define DHCPSERV_EXE "udhcpd"                         /* uDHCP server */

#else
#define IFUP_EXE "/sbin/ifup &> /dev/null"                       /* network */
#define IFDOWN_EXE "/sbin/ifdown &> /dev/null"                   /* network */
#define GATEWAY_EXE "/sbin/route &>/dev/null"                   /* gateway */
#define HOSTNAME_EXE "/bin/hostname &>/dev/null"             /* server name */
#define INETSERV_EXE "/etc/rc.d/rc.xinetd &>/dev/null"        /* xinet daemon */
#define  DHCPSERV_EXE "/usr/sbin/udhcpd &>/dev/null"         /* uDHCP server */

#endif 
				/* NAT Feature */
#ifdef _NAT_FUNCTION_
#define NET_SYSCTL_CONF    "/proc/sys/net/ipv4/ip_forward"      /* ip_forward */

#ifndef _OFF_STDOUT_
#define NAT_STOP_DIAL_EXE  "/etc/ppp/dial stop"  /* disable dialup daemon */
#define NAT_SET_IPMASQ_EXE "/etc/ppp/masq_reset" /* enable ip_masq */
#define NAT_STOP_PPPOE_EXE "/usr/sbin/adsl-stop"  /* disable pppoe */
#else
#define NAT_STOP_DIAL_EXE  "/etc/ppp/dial stop &>/dev/null"  /* disable dialup daemon */
#define NAT_SET_IPMASQ_EXE "/etc/ppp/masq_reset &>/dev/null" /* enable ip_masq */
#define NAT_STOP_PPPOE_EXE "/usr/sbin/adsl-stop &>/dev/null"  /* disable pppoe */
#endif 
			/* Vincent 001 - Use CGI_ds.conf - Start BLOCK */
#define NAT_NAT_CONF       DS_CONF                 /* NAT: nat_enable=yes/no */
#define NAT_PPP_SECTION    "[NAT]"                 /* [NAT] */
#define NAT_WAN_SECTION    "[WANPORT]"             /* [WANPORT] */
			/* Vincent 001 - Use CGI_ds.conf - Stop  BOLCK */
#endif 

				/* the other */
#define _TMP_FILE1 "/tmp/Eda.qXXXXXX"
#define _TMP_FILE2 "/tmp/Edb.qXXXXXX"
#define _TMP_FILE3 "/tmp/Edc.qXXXXXX"
#define _TMP_FILE4 "/tmp/Edd.qXXXXXX"


/* ---------- Global Length/Variable Declaration ----------*/

				/* Length */
#define FILE_BUF_LEN   1024
#define LINE_BUF_LEN   256
#define SYSTEM_BUF_LEN 256
				/* dhcp server */
#define DHCPDEFLEASE   259200	/* default-lease-time 3 days */
#define DHCPMAXLEASE   604800	/* max-lease-time 1 week */

				/* variable */
/* -- for preventing the conflict in caller function, move these declaration to the subroutine. change from "global variable" to "local variable" -- */
/* tmpfile string will set in one function */
/* char nettmpfile1[] = _TMP_FILE1; */
/* char nettmpfile2[] = _TMP_FILE2; */
/* char nettmpfile3[] = _TMP_FILE3; */
/* char nettmpfile4[] = _TMP_FILE4; */

char filebuffer[FILE_BUF_LEN];		/* buffer for file read */
char linebuffer[LINE_BUF_LEN];  	/* buffer for one line data */
char systembuffer[SYSTEM_BUF_LEN];      /* temp buffer for "system(xxx)" */


/* ---------- Defalut Variable Declaration ----------*/

/* ----- TCP/IP information ----- */
// TCPIPInfo TCPIPData = {
// 	NET_DEVICE,		/* eth0 */
// 	"00:C0:02:12:34:AB",	/* HWADDR */
// 	"192.168.0.2",		/* IPADDR */
// 	"255.255.255.0",	/* NETMASK */
// 	"0.0.0.0",		/* GATEWAY */
// 	"static"		/* BOOTPROTO */
// };


/* ----- Hostname information ----- */
// HOSTInfo HOSTData = {
// 	"SCxxxxxx"		/* default server name */
// };


/* ----- DNS information ----- */
// DNSInfo DNSData = {
// 	"linuxtoday.com",
// 	"0.0.0.0",
// 	"0.0.0.0",
// 	"0.0.0.0"
// };


/* ----- Inet Daemon information ----- */
// INETSERVInfo INETSERV_Data = {
// 	"disable"
// };

/* ---- DHCP Server information ----- */
// DHCPSERVInfo DHCPSERVData= {
// 	"disable",
// 	"0.0.0.0",
// 	"0.0.0.0"
// };

/* ---------- Internal Function ---------- */

					     /* general */
int NETSetTCPIPNetwork(TCPIPInfo *info, DNSInfo *dns);
int NETSetFixIPNetwork(TCPIPInfo *info, DNSInfo *dns);
int NETSetDHCPIPNetwork(TCPIPInfo *info, DNSInfo *dns);
int NETWriteNetworkTCPIPData(TCPIPInfo *info);
int NETRestartNetwork(void);
int NETPutNetworkDNSData(DNSInfo *Nowinfo, char *NewDNS, int count);
int NETArrangeNetworkDNSData(DNSInfo *Nowinfo, DNSInfo *Newinfo);
int NETSetNetworkGateway(TCPIPInfo *info);
int NETReadCurrentNetworkData(TCPIPInfo *info);
int NETCheckDHCPServer(TCPIPInfo *info);
int NETUpdateNetworkHostsFile(void);
int NETReadCurrentHostname(char *hostname);
int NETWriteHostnameData(HOSTInfo *info);
int NETRestartInetService(void);
int NETWriteInetServiceData(INETSERVInfo *info);
int NETUpdataInetServiceData(INETSERVInfo *info);
int NETRestartDHCPServer(void);
int NETCreateDHCPServerLeaseData(void);
#ifdef _TCPIP_OUTPUT_
int NETWriteCurrentTCPIPData(void);
#endif 
#ifdef _NAT_FUNCTION_
int NETWriteIPForwardFlag(int type);
int NETWriteWANDisableFlag(void);
int NETReadNATEnable(void);
int NETSetNATDisable(void);
int NETStopSerialPort(void);
int NETStopWANPort(void);

#endif

#endif 
