#ifndef _NETWORK_H_
#define _NETWORK_H_

#define	TCPIP_MaxIPLen				16
#define	TCPIP_MaxDomainLen			63
#define	TCPIP_MaxMacAddrLen			17
#define	TCPIP_MaxHostnameLen			14
#define	TCPIP_MaxBootprotoLen			7
#define	TCPIP_MaxEnableLen			7
#define	TCPIP_INVALID_CHARACTOR_HOSTNAME		"/\\[];:|=.,<>+?* '`\"#$"
#define		TCPIP_MaxInterfaceLen    16
#define TIME_LEN      19             /* YYYY/MM/DD HH:MM:SS */  

#define		TCPIP_YES		"yes"
#define		TCPIP_NO		"no"
#define		TCPIP_NW1		"ixp0"


typedef struct sys_ip {
	char interface[TCPIP_MaxInterfaceLen+1];			// "eth0", "eth1", or "ppp0"
	char bootproto[TCPIP_MaxBootprotoLen+1]; 		// type of boot:  "dhcp" or "static"
	char ip_addr[TCPIP_MaxIPLen+1];				// ip address
	char netmask[TCPIP_MaxIPLen+1];				// netmask
	char gateway[TCPIP_MaxIPLen+1];				// gateway
	char dns_server1[TCPIP_MaxIPLen+1];			// first dns server ip address
	char dns_server2[TCPIP_MaxIPLen+1];			// second dns server ip address
	char dns_server3[TCPIP_MaxIPLen+1];			// third dns server ip address
	char domain_name[TCPIP_MaxDomainLen+1];		// domain name
	char mac_address[TCPIP_MaxMacAddrLen+1]; 		// MAC address: readonly
	char hostname[TCPIP_MaxHostnameLen+1];		// hostname
	char ip_default[TCPIP_MaxIPLen+1];				// default ip address
	char netmask_default[TCPIP_MaxIPLen+1];			// default netmask address
	int interval;								// interval to request ip address from DHCP Server
} sys_ip;


//--------------------------------------------------------------------
// purpose: Read all TCP/IP info from file /etc/CGI_ds.conf
// output: buffer with TCP/IP info
// return: TCP_OK - succeed
//                    othes - fail
//--------------------------------------------------------------------
int ReadTCPIPConf(sys_ip *conf);

//--------------------------------------------------------------------
// purpose: Save all TCP/IP info to file /etc/CGI_ds.conf
// input: buffer with TCP/IP info
// return: TCP_OK - succeed
//                    othes - fail
//--------------------------------------------------------------------
int SaveTCPIPConf(sys_ip *conf);

//--------------------------------------------------------------------
// purpose: 1.Reset (restart) Linux TCP/IP configuration files according to the content in /etc/CGI_ds.conf file.
//                 2. Restart Linux TCP/IP programs.
// return: TCP_OK - succeed
//                    othes - fail
//--------------------------------------------------------------------
int ResetTCPIP(void);

//--------------------------------------------------------------------
// purpose: 1.Restore all TCPIP settings (files) to factory default
//                 2.Restart Linux TCPIP programs.
// input : interface - LAN interface
// return: TCP_OK - succeed
//                    othes - fail
//--------------------------------------------------------------------
int RestoreDefaultTCPIP(char *interface);

#define	TCPIP_OK						0			//Succeed
#define	TCPIP_SaveTCPIPConfErr			-5			//Fail to save tcp/ip configuration info.
#define	TCPIP_SaveDHCPSConfErr			-4			//Fail to save DHCP Server configuration info.
#define	TCPIP_RestoreDefaultTCPIPErr		-3			//Fail to restore default tcp/ip settings.
#define	TCPIP_RestoreDefaultDHCPSErr		-2			//Fail to restore default  DHCP Server settings.
#define	TCPIP_InputParamErr				-1			//Input parameter is invalid.
#define	TCPIP_BootprotoErr					1			//The value of bootproto is invalid.
#define	TCPIP_IPAddrBlankErr		 		2			//The bootproto is "static" but the IP Address is blank.
#define	TCPIP_IPAddrErr					3			//The IP Address is invalid.
#define	TCPIP_DefaultIPErr					4			//The default IP Address is invalid.
#define	TCPIP_NetmaskAddrBlankErr			5			//The bootproto is "static" but the Netmask Address is blank.
#define	TCPIP_NetmaskAddrErr				6			//The Netmask Address is invalid.
#define	TCPIP_DefaultNetmaskErr			7			//The default Netmask Address is invalid.
#define	TCPIP_GatewayAddrErr				8			//The Gateway Address is invalid.
#define	TCPIP_TimeDelayErr				9			// Interval to request ip address from DHCP Server is invalid.
#define	TCPIP_DNS1AddrErr		 		10			//The first DNS Server Address is invalid.
#define	TCPIP_DNS2AddrErr				11			//The second DNS Server Address is invalid.
#define	TCPIP_DNS3AddrErr				12			//The first DNS Server Address is invalid.
#define	TCPIP_DHCPSErr					13			//The value of DHCP is invalid. 
#define	TCPIP_DHCPRangeBlankErr			14			//The DHCP Server is enabled but the rang of the IP Address leased is blank.
#define	TCPIP_DHCPSStartAddrErr			15			//The Start Address leased by DHCP Server is invalid.
#define	TCPIP_DHCPSEndAddrErr			16			//The End  Address leased by DHCP Server is invalid.
#define	TCPIP_DHCPRangeErr				17			//The rang of the IP Address leased by DHCP Server is invalid.
#define	TCPIP_DHCPSLeaseTimeErr			18			// Lease time of DHCP Server is invalid.
#define	TCPIP_DomainNameErr				19			//Illegal character or characters in domain name.
#define	TCPIP_WINSAddrErr				20			//The WINS Server Address is invalid.
#define	TCPIP_HostnameErr					21			//Illegal character or characters in hostname.
#define	TCPIP_DBCSInHostnameErr			22			//DBCS in hostname.
#define	TCPIP_HostnameBlankErr			23			//Hostname cannot be blank.
#define	TCPIP_ReadTCPIPConfErr			24			//Fail to read tcp/ip configuration info
#define	TCPIP_ReadDHCPSConfErr			25			//Fail to read DHCP Server configuration info
#define	TCPIP_ResetTCPIPErr				26			//Fail to reset TCP/IP
#define	TCPIP_ResetDHCPSErr				27			//Fail to reset DHCP Server
#define	TCPIP_ReadLeaseInfoErr				28			//Fail to lease info of  DHCP Server
#endif
