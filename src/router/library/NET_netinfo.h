#ifndef _NET_NETINFO_
#define _NET_NETINFO_

#include "NET_network.h"

/* ------------------------------------------------------------------------*/
/* purpose : Get  current net-info:IP address, netmask, default gateway    */
/*           and copy them into the input structure                        */
/* return : 0 -success                                                     */
/*          others - failed                                                */
/*-------------------------------------------------------------------------*/
int NETReadCurrentNetworkData(TCPIPInfo *info);

/* ------------------------------------------------------------------------*/
/* purpose : Get default server name.                                      */
/* return : 0 -success                                                     */
/*          others - failed                                                */
/*-------------------------------------------------------------------------*/
int NETReadDefaultHostname(char *defaultname);

/* ------------------------------------------------------------------------*/
/* purpose : Read lease hosts information form dhcpd.leases then get legal */
/*           IP address,MAC address,hostname,expire time                   */ 
/* return : 0 -success                                                     */
/*          others - failed                                                */
/* NOTE: This function will put host information to user allocated         */
/*       host info list                                                    */
/*-------------------------------------------------------------------------*/
int NETReadDHCPServerTable(Lease *info);

/* ------------------------------------------------------------------------*/
/* purpose : Get  lan interface is eithre eht0 or eth1                     */
/* return : 0 -success                                                     */
/*          others - failed                                                */
/* NOTE: default lan interface is eth0                                     */
/*-------------------------------------------------------------------------*/
//int NETGetLANDevice(char *devie);

#endif 
