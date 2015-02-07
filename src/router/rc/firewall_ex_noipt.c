/*
 * Copyright 2004, Broadcom Corporation
 * All Rights Reserved.
 * 
 * THIS SOFTWARE IS OFFERED "AS IS", AND BROADCOM GRANTS NO WARRANTIES OF ANY
 * KIND, EXPRESS OR IMPLIED, BY STATUTE, COMMUNICATION OR OTHERWISE. BROADCOM
 * SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A SPECIFIC PURPOSE OR NONINFRINGEMENT CONCERNING THIS SOFTWARE.
 *
 * $Id: firewall_ex_noipt.c,v 1.2 2008/10/24 03:32:48 james26_jang Exp $
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

#define foreach_x(x)	for(i=0; i<atoi(nvram_safe_get(x)); i++)

char *g_buf;
char g_buf_pool[1024];

void g_buf_init()
{
	g_buf = g_buf_pool;
}

char *g_buf_alloc(char *g_buf_now)
{
	g_buf += strlen(g_buf_now)+1;

	return(g_buf_now);
}

void nvram_unsets(char *name, int count)
{
	char itemname_arr[32];
	int i;

	for(i=0; i<count; i++)
	{
		sprintf(itemname_arr, "%s%d", name, i);
		nvram_unset(itemname_arr);
	}
}

char *proto_conv(char *proto_name, int idx)
{	
	char *proto;
	char itemname_arr[32];
	
	sprintf(itemname_arr,"%s%d", proto_name, idx);
	proto=nvram_get(itemname_arr);
		
	if(!strncasecmp(proto, "Both", 3)) strcpy(g_buf, "both");
	else if(!strncasecmp(proto, "TCP", 3)) strcpy(g_buf, "tcp");
	else if(!strncasecmp(proto, "UDP", 3)) strcpy(g_buf, "udp");
	else if(!strncasecmp(proto, "OTHER", 5)) strcpy(g_buf, "other");
	else strcpy(g_buf,"tcp");
	return (g_buf_alloc(g_buf));
}

char *protoflag_conv(char *proto_name, int idx, int isFlag)
{	
	char *proto;
	char itemname_arr[32];
	
	sprintf(itemname_arr,"%s%d", proto_name, idx);
	proto=nvram_get(itemname_arr);

	strcpy(g_buf, "");
	
	if (!isFlag)
	{		
		if(strncasecmp(proto, "UDP", 3)==0) strcpy(g_buf, "udp");
		else strcpy(g_buf, "tcp");
	}
	else
	{	
		if(strlen(proto)>3)
		{
			strcpy(g_buf, proto+4);
		}			
		else strcpy(g_buf,"");
	}	
	return (g_buf_alloc(g_buf));
}

char *portrange_ex_conv(char *port_name, int idx)
{
	char *port, *strptr;
	char itemname_arr[32];
	
	sprintf(itemname_arr,"%s%d", port_name, idx);
	port=nvram_get(itemname_arr);

	strcpy(g_buf, "");
	
	if(!strncmp(port, ">", 1)) {
		sprintf(g_buf, "%d-65535", atoi(port+1) + 1);
	}
	else if(!strncmp(port, "=", 1)) {
		sprintf(g_buf, "%d-%d", atoi(port+1), atoi(port+1));
	}
	else if(!strncmp(port, "<", 1)) {
		sprintf(g_buf, "1-%d", atoi(port+1) - 1);
	}
	else if(strptr=strchr(port, ':'))
	{		
		strcpy(itemname_arr, port);
		strptr = strchr(itemname_arr, ':');
		sprintf(g_buf, "%d-%d", atoi(itemname_arr), atoi(strptr+1));		
	}
	else if(*port)
	{
		sprintf(g_buf, "%d-%d", atoi(port), atoi(port));
	}
	else
	{
		sprintf(g_buf, "");
	}
	
	return(g_buf_alloc(g_buf));
}

char *portrange_ex2_conv(char *port_name, int idx, int *start, int *end)
{
	char *port, *strptr;
	char itemname_arr[32];
	
	sprintf(itemname_arr,"%s%d", port_name, idx);
	port=nvram_get(itemname_arr);

	strcpy(g_buf, "");
	
	if(!strncmp(port, ">", 1)) 
	{
		sprintf(g_buf, "%d-65535", atoi(port+1) + 1);
		*start=atoi(port+1);
		*end=65535;
	}
	else if(!strncmp(port, "=", 1)) 
	{
		sprintf(g_buf, "%d-%d", atoi(port+1), atoi(port+1));
		*start=*end=atoi(port+1);
	}
	else if(!strncmp(port, "<", 1)) 
	{
		sprintf(g_buf, "1-%d", atoi(port+1) - 1);
		*start=1;
		*end=atoi(port+1);
	}
	else if(strptr=strchr(port, ':'))
	{		
		strcpy(itemname_arr, port);
		strptr = strchr(itemname_arr, ':');
		sprintf(g_buf, "%d-%d", atoi(itemname_arr), atoi(strptr+1));	
		*start=atoi(itemname_arr);
		*end=atoi(strptr+1);
	}
	else if(*port && atoi(port)!=0)
	{
		sprintf(g_buf, "%d-%d", atoi(port), atoi(port));
		*start=atoi(port);
		*end=atoi(port);
	}
	else
	{
		sprintf(g_buf, "");
		*start=1;
		*end=65535;
	}
	
	return(g_buf_alloc(g_buf));
}

char *portrange_conv(char *port_name, int idx)
{
	char itemname_arr[32];
	
	sprintf(itemname_arr,"%s%d", port_name, idx);
	strcpy(g_buf, nvram_safe_get(itemname_arr));	
	
	return(g_buf_alloc(g_buf));
}

char *iprange_conv(char *ip_name, int idx)
{
	char *ip;
	char itemname_arr[32];
	char startip[16], endip[16];
	int i, j, k;
	
	sprintf(itemname_arr,"%s%d", ip_name, idx);
	ip=nvram_safe_get(itemname_arr);
	strcpy(g_buf, "");
	
	// scan all ip string
	i=j=k=0;
	
	while(*(ip+i))
	{
		if (*(ip+i)=='*') 
		{
			startip[j++] = '1';
			endip[k++] = '2';
			endip[k++] = '5';
			endip[k++] = '4';
			// 255 is for broadcast
		}
		else 
		{
			startip[j++] = *(ip+i);
			endip[k++] = *(ip+i);
		}
		i++;
	}	
	
	startip[j++] = 0;
	endip[k++] = 0;

	if (strcmp(startip, endip)==0)
		sprintf(g_buf, "%s", startip);
	else
		sprintf(g_buf, "%s-%s", startip, endip);
	return(g_buf_alloc(g_buf));
}


char *iprange_ex2_conv(char *ip_name, int idx, unsigned int *srcip, unsigned int *srcmask)
{
	char ip[32];
	char itemname_arr[32];
	char startip[16], netmask[16];
	int i, j, k;
	
	sprintf(itemname_arr,"%s%d", ip_name, idx);

	if (nvram_invmatch(itemname_arr, ""))
		strcpy(ip, nvram_safe_get(itemname_arr));
	else strcpy(ip, "*.*.*.*");


	strcpy(g_buf, "");

	

	if (strstr(ip, "*.*.*.*")) *srcmask=0x0;
	else if(strstr(ip, "*.*.*")) *srcmask=0xff000000;
	else if(strstr(ip, "*.*")) *srcmask=0xffff0000;
	else if(strstr(ip, "*")) *srcmask=0xffffff00;
	else *srcmask=0xffffffff;
	
	// scan all ip string
	i=j=k=0;
	
	while(*(ip+i))
	{
		if (*(ip+i)=='*') 
		{
			startip[j++] = '0';
			// 255 is for broadcast
		}
		else 
		{
			startip[j++] = *(ip+i);
		}
		i++;
	}	
	
	startip[j++] = 0;

	*srcip = ntohl(inet_addr(startip));
	sprintf(g_buf, "%s", startip);

	return(g_buf_alloc(g_buf));
}

char *ip_conv(char *ip_name, int idx)
{	
	char itemname_arr[32];

	sprintf(itemname_arr,"%s%d", ip_name, idx);
	sprintf(g_buf, "%s", nvram_safe_get(itemname_arr));
	return(g_buf_alloc(g_buf));
}


char *general_conv(char *ip_name, int idx)
{	
	char itemname_arr[32];

	sprintf(itemname_arr,"%s%d", ip_name, idx);
	sprintf(g_buf, "%s", nvram_safe_get(itemname_arr));
	return(g_buf_alloc(g_buf));
}

char *filter_conv(char *proto, char *flag, char *srcip, char *srcport, char *dstip, char *dstport)
{
	char newstr[64];

	//printf("filter : %s,%s,%s,%s,%s,%s\n", proto, flag, srcip, srcport, dstip, dstport);
	
	strcpy(g_buf, "");		
				                        			
        if (strcmp(proto, "")!=0)
	{
		sprintf(newstr, " -p %s", proto);
        	strcat(g_buf, newstr);
        }				
            			
        if (strcmp(flag, "")!=0)
        {
        	sprintf(newstr, " --tcp-flags %s RST", flag);
		strcat(g_buf, newstr);
	}			
                 
	if (strcmp(srcip, "")!=0)
	{
		if (strchr(srcip , '-'))
			sprintf(newstr, " --src-range %s", srcip);
		else	
			sprintf(newstr, " -s %s", srcip);
		strcat(g_buf, newstr);
	}				
            			
	if (strcmp(srcport, "")!=0)
	{
		sprintf(newstr, " --sport %s", srcport);
		strcat(g_buf, newstr);
        }			
            			
        if (strcmp(dstip, "")!=0)
        {
		if (strchr(dstip, '-'))
			sprintf(newstr, " --dst-range %s", dstip);
		else	
        		sprintf(newstr, " -d %s", dstip);
		strcat(g_buf, newstr);
        }
			
        if (strcmp(dstport, "")!=0)
        {
        	sprintf(newstr, " --dport %s", dstport);
		strcat(g_buf, newstr);
	}
	return(g_buf_alloc(g_buf));
	//printf("str: %s\n", g_buf);
}

void timematch_conv(char *mstr, char *nv_date, char *nv_time)
{	
	char *datestr[] = { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
	char timestart[6], timestop[6];
	char *time, *date;
	int i, head;

	date = nvram_safe_get(nv_date);
	time = nvram_safe_get(nv_time);

	if (strlen(date)!=7||strlen(time)!=8) goto no_match;

	if (strncmp(date, "1111111", 7)==0 &&
	    strncmp(time, "00002359", 8)==0) goto no_match;
	
	i=0;
	strncpy(timestart, time, 2);
	i+=2;
	timestart[i++] = ':';
	strncpy(timestart+i, time+2, 2);
	i+=2;
	timestart[i]=0;
	i=0;
	strncpy(timestop, time+4, 2);
	i+=2;
	timestop[i++] = ':';
	strncpy(timestop+i, time+6, 2);
	i+=2;
	timestop[i]=0;

	sprintf(mstr, "-m time --timestart %s:00 --timestop %s:00 --days ",
			timestart, timestop);

	head=1;

	for(i=0;i<7;i++)
	{
		if (*(date+i)=='1')
		{
			if (head)
			{
			    sprintf(mstr, "%s %s", mstr, datestr[i]);
			    head=0;
			}
			else
			{	
			    sprintf(mstr, "%s,%s", mstr, datestr[i]);
			}
		}
	}
	return;
	
no_match:
	sprintf(mstr, "");
	return;
}


void timematch_ex_conv(char *nv_date, char *nv_time, int *days, int *sec_start, int *sec_end)
{	
	char *time, *date;
	char start_hour[3], end_hour[3], start_min[3], end_min[3];
	int i, tmp;

	date = nvram_safe_get(nv_date);
	time = nvram_safe_get(nv_time);

	if (strlen(date)!=7||strlen(time)!=8) goto no_match;

	if (strncmp(date, "1111111", 7)==0 &&
	    strncmp(time, "00002359", 8)==0) goto no_match;

	start_hour[2]=end_hour[2]=start_min[2]=end_min[2]=0;
	strncpy(start_hour, time, 2);
	strncpy(start_min, time+2, 2);
	strncpy(end_hour, time+4, 2);
	strncpy(end_min, time+6, 2);

	*sec_start = atoi(start_hour)*60*60 + atoi(start_min)*60;
	*sec_end = atoi(end_hour)*60*60 + atoi(end_min)*60;

	//if (*sec_end<*sec_start)
	//{
	//	tmp=*sec_start;
	//	*sec_start=*sec_end;
	//	*sec_end=tmp;
	//}

	*days=0;

	for(i=0;i<7;i++)
	{
		if (*(date+i)=='1')
		{	
			*days|=(1<<i);
		}
	}
	return;
	
no_match:
	*sec_start = 0;
	*sec_end = 86399;
	*days=0x7f;
	return;
}

int dayrange(int days, int *offset, int *start, int *end)
{
	int s=-1, e=-1;	

	while(*offset<7)
	{
		if (days&(0x01<<(*offset)))
		{
			if (s==-1) s=*offset;
			e = *offset;	
		}
		else if(s!=-1) break;

		(*offset)++;
	}
	if (s!=-1)
	{
		*start = s;
		*end = e;

		printf("day range: %d %d\n", *start, *end);
		return 1;
	}
	else return 0;	
}

void
p(int step)
{
	printf("P: %d %s\n", step, g_buf);
}


void 
ip2class(char *lan_ip, char *netmask, char *buf)
{
	unsigned int val, ipval;
	int i=0;

	// only handle class A,B,C	
	val = (unsigned int)inet_addr(netmask);
	ipval = (unsigned int)inet_addr(lan_ip);
	if (val==0x00000ff) sprintf(buf, "%d.0.0.0/8", (ipval&0x000000ff));
	else if (val==0x0000ffff) sprintf(buf, "%d.%d.0.0/16", (ipval&0x000000ff), (ipval&0x0000ff00)>>8);
	else sprintf(buf, "%d.%d.%d.0/24", (ipval&0x000000ff), (ipval&0x0000ff00)>>8, (ipval&0x00ff0000)>>16);
	//printf(buf);	
}

#ifndef NOIPTABLES
nat_setting(char *wan_if, char *wan_ip, char *lan_if, char *lan_ip, char *logaccept, char *logdrop)
{
	FILE *fp, *fp1;
        char *proto, *flag, *srcip, *srcport, *dstip, *dstport;
        char *setting, tmpbuf[32], lan_class[32];
	int i;
	int wan_port;
	
	if ((fp=fopen("/tmp/nat_rules", "w"))==NULL) return -1;
	if ((fp1=fopen("/tmp/nat_forward_rules", "w"))==NULL) return -1;

	fprintf(fp, "*nat\n"
	       	":PREROUTING ACCEPT [0:0]\n"
	       	":POSTROUTING ACCEPT [0:0]\n"
	  	":OUTPUT ACCEPT [0:0]\n");

	//Log	
   	//if(nvram_match("misc_natlog_x", "1"))
   	// 	fprintf(fp, "-A PREROUTING -i %s -j LOG --log-prefix ALERT --log-level 4\n", wan_if);

   	if (nvram_match("misc_http_x", "1"))
	{
		wan_port=8080;
   		if (nvram_invmatch("misc_httpport_x", ""))
      			wan_port=atoi(nvram_safe_get("misc_httpport_x")); 	
   		fprintf(fp, "-A PREROUTING -p tcp -m tcp -d %s --dport %d -j DNAT --to-destination %s:80\n", wan_ip, wan_port, lan_ip);
	}

	if (nvram_match("wan_nat_x", "1"))
	{
   		fprintf(fp, "-A POSTROUTING -o %s -j MASQUERADE\n", wan_if);

		// masquerade lan to lan
		ip2class(lan_ip, nvram_safe_get("lan_netmask"), lan_class);
		fprintf(fp, "-A POSTROUTING -o %s -s %s -d %s -j MASQUERADE\n", lan_if, lan_class, lan_class);
	}

	// Port forwarding or Virtual Server
   	if (nvram_match("wan_nat_x", "1") && nvram_match("vts_enable_x", "1"))
   	{     		
   		g_buf_init();
   				     		
     		foreach_x("vts_num_x")
     		{
     			char *proto;
			char *protono;
			char *port;
			char *dstip;
     				
     			proto = proto_conv("vts_proto_x", i);
			protono = portrange_conv("vts_protono_x", i);
			port = portrange_conv("vts_port_x", i);
			dstip = ip_conv("vts_ipaddr_x", i);
     				
			if (strcmp(proto, "tcp")==0 || strcmp(proto, "both")==0)
			{
				fprintf(fp, "-A PREROUTING -p tcp -m tcp -d %s --dport %s -j DNAT --to %s\n", 
					wan_ip, port, dstip);

				fprintf(fp1, "-A FORWARD -p tcp -m tcp -d %s --dport %s -j %s\n", 
					dstip, port, logaccept);
			}     				
				
			if (strcmp(proto, "udp")==0 || strcmp(proto, "both")==0)
			{

				fprintf(fp, "-A PREROUTING -p udp -m udp -d %s --dport %s -j DNAT --to %s\n", 
					wan_ip, port, dstip);

				fprintf(fp1, "-A FORWARD -p udp -m udp -d %s --dport %s -j %s\n", 
					dstip, port, logaccept);
			}     				        			
			if (strcmp(proto, "other")==0)
			{
				fprintf(fp, "-A PREROUTING -p %s -d %s -j DNAT --to %s\n",
					protono, wan_ip, dstip);

				fprintf(fp1, "-A FORWARD -p %s -d %s -j %s\n",
					protono, dstip, logaccept);
			}     				        				}	
	}	

   	// Exposed station	
   	if (nvram_match("wan_nat_x", "1") && nvram_invmatch("dmz_ip", ""))
   	{   		
      		fprintf(fp, "-A PREROUTING -d %s -j DNAT --to %s\n", 
			wan_ip, nvram_safe_get("dmz_ip"));      

      		fprintf(fp1, "-A FORWARD -d %s -j %s\n", 
			nvram_safe_get("dmz_ip"), logaccept);      
      	}


	fprintf(fp, "COMMIT\n");
	
	fclose(fp);
	fclose(fp1);

	eval("iptables-restore", "/tmp/nat_rules");
}

int
filter_setting(char *wan_if, char *wan_ip, char *lan_if, char *lan_ip, char *logaccept, char *logdrop)
{

	FILE *fp, *fp1;
        char *proto, *flag, *srcip, *srcport, *dstip, *dstport;
        char *setting, line[256];
	int i;
	int wan_port;
	
	if ((fp=fopen("/tmp/filter_rules", "w"))==NULL) return -1;

	fprintf(fp, "*filter\n:INPUT ACCEPT [0:0]\n:FORWARD ACCEPT [0:0]\n:OUTPUT ACCEPT [0:0]\n:logaccept - [0:0]\n:logdrop - [0:0]\n");


	if (nvram_match("fw_enable_x", "1"))
	{
		/* Filter known SPI state */
		fprintf(fp, "-A INPUT -m state --state INVALID -j %s\n"
	          "-A INPUT -m state --state RELATED,ESTABLISHED -j %s\n"
	          "-A INPUT -i lo -m state --state NEW -j %s\n"
	          "-A INPUT -i %s -m state --state NEW -j %s\n"
		,logdrop, logaccept, "ACCEPT", lan_if, "ACCEPT");
	
	// Firewall between WAN and Local
	if (nvram_match("misc_http_x", "1"))
	{
		fprintf(fp, "-A INPUT -p tcp -m tcp -d %s --dport 80 -j %s\n", nvram_safe_get("lan_ipaddr"), logaccept);
		//fprintf(fp, "-A INPUT -p tcp -m tcp -d %s --dport %s -j %s\n", wan_ip, nvram_safe_get("misc_httpport_x"), logaccept);
	}

	if (nvram_match("usb_webenable_x", "2"))
	{
		fprintf(fp, "-A INPUT -p tcp -m tcp -d %s --dport %s -j %s\n", wan_ip, nvram_safe_get("usb_webhttpport_x"), logaccept);

		fprintf(fp, "-A INPUT -p tcp -m udp -d %s --dport %s -j %s\n", wan_ip, nvram_safe_get("usb_webactivex_x"), logaccept);
	}

	if (nvram_invmatch("usb_ftpenable_x", "0"))
	{	
		fprintf(fp, "-A INPUT -p tcp -m tcp -d %s --dport %s -j %s\n", wan_ip, nvram_safe_get("usb_ftpport_x"), logaccept);
	}

	if (nvram_invmatch("misc_ping_x", "0"))
	{
		fprintf(fp, "-A INPUT -p icmp -d %s -j %s\n", wan_ip, logaccept);
	}

	if (nvram_invmatch("misc_lpr_x", "0"))
	{
		fprintf(fp, "-A INPUT -p tcp -m tcp -d %s --dport %d -j %s\n", wan_ip, 515, logaccept);

		fprintf(fp, "-A INPUT -p tcp -m tcp -d %s --dport %d -j %s\n", wan_ip, 3838, logaccept);
	}

	fprintf(fp, "-A INPUT -j %s\n", logdrop);
	}

	/* Drop the wrong state, INVALID, packets */
	fprintf(fp, "-A FORWARD -m state --state INVALID -j %s\n", logdrop);

	/* Accept the redirect, might be seen as INVALID, packets */
	fprintf(fp, "-A FORWARD -i %s -o %s -j %s\n", lan_if, lan_if, logaccept);	
	/* Clamp TCP MSS to PMTU of WAN interface */
	if( nvram_match("wan_proto", "pppoe"))
		fprintf(fp, "-A FORWARD -p tcp --tcp-flags SYN,RST SYN -m tcpmss --mss %d: -j TCPMSS "
			  "--set-mss %d\n", atoi(nvram_safe_get("wan_pppoe_mtu"))-39, atoi(nvram_safe_get("wan_pppoe_mtu"))-40);
	
	if (nvram_match("fw_enable_x", "1"))
	{
   		// DoS attacks   
   		// sync-flood protection	
   		fprintf(fp, "-A FORWARD -p tcp --syn -m limit --limit 1/s -j %s\n", logaccept);
   		// furtive port scanner
   		fprintf(fp, "-A FORWARD -p tcp --tcp-flags SYN,ACK,FIN,RST RST -m limit --limit 1/s -j %s\n", logaccept);
   		// ping of death
   		fprintf(fp, "-A FORWARD -p icmp --icmp-type echo-request -m limit --limit 1/s -j %s\n", logaccept);
	}
	
	// FILTER from LAN to WAN
	if(nvram_match("fw_lw_enable_x", "1"))
	{   		
		char lanwan_timematch[128];
		char ptr[32], *icmplist;
		char *ftype, *dtype;

		timematch_conv(lanwan_timematch, "filter_lw_date_x", "filter_lw_time_x");	    
		// LAN/WAN filter		
		g_buf_init();

		if (nvram_match("filter_lw_default_x", "DROP"))
		{
			dtype = logdrop;
			ftype = logaccept;
		}
		else
		{
			dtype = logaccept;
			ftype = logdrop;
		}
			
       		foreach_x("filter_lw_num_x")
       		{	               			
            		proto = protoflag_conv("filter_lw_proto_x", i, 0);
            		flag = protoflag_conv("filter_lw_proto_x", i, 1);
			srcip = ip_conv("filter_lw_srcip_x", i);
			srcport = portrange_conv("filter_lw_srcport_x", i);
			dstip = ip_conv("filter_lw_dstip_x", i);
			dstport = portrange_conv("filter_lw_dstport_x", i);	
			setting=filter_conv(proto, flag, srcip, srcport, dstip, dstport); 
         		fprintf(fp, "-A FORWARD %s -i %s -o %s %s -j %s\n", lanwan_timematch, lan_if, wan_if, setting, ftype);
		}            

       		// ICMP       	       			       			
       		foreach(ptr, nvram_safe_get("filter_lw_icmp_x"), icmplist)
		{	   	 		
	        	fprintf(fp, "-A FORWARD %s -i %s -o %s -p icmp --icmp-type %s -j %s\n", lanwan_timematch, lan_if, wan_if, ptr, ftype);
	        }	

		// Default
         	fprintf(fp, "-A FORWARD -i %s -o %s -j %s\n", lan_if, wan_if, dtype);
	} 

	// Filter from WAN to LAN
	if(nvram_match("fw_wl_enable_x", "1"))
	{   		
		char wanlan_timematch[128];
		char ptr[32], *icmplist;
		char *dtype, *ftype;

		timematch_conv(wanlan_timematch, "filter_wl_date_x", "filter_wl_time_x");
		g_buf_init();
	
		if (nvram_match("filter_wl_default_x", "DROP"))
		{
			dtype = logdrop;
			ftype = logaccept;
		}
		else
		{
			dtype = logaccept;
			ftype = logdrop;
		}
			
       		foreach_x("filter_wl_num_x")
       		{	               			
            		proto = protoflag_conv("filter_wl_proto_x", i, 0);
            		flag = protoflag_conv("filter_wl_proto_x", i, 1);
			srcip = ip_conv("filter_wl_srcip_x", i);
			srcport = portrange_conv("filter_wl_srcport_x", i);
			dstip = ip_conv("filter_wl_dstip_x", i);
			dstport = portrange_conv("filter_wl_dstport_x", i);	
			setting=filter_conv(proto, flag, srcip, srcport, dstip, dstport);

         		fprintf(fp, "-A FORWARD %s -i %s -o %s %s -j %s\n", wanlan_timematch, wan_if, lan_if, setting, ftype);
		}            
       			 
       		// ICMP       	       			       			
       		foreach(ptr, nvram_safe_get("filter_wl_icmp_x"), icmplist)
		{	   	 		
	        	fprintf(fp, "-A FORWARD %s -i %s -o %s -p icmp --icmp-type %s -j %s\n", wanlan_timematch, wan_if, lan_if, ptr, ftype);
	        }	
         	
		// Default
		fprintf(fp, "-A FORWARD -i %s -o %s -j %s\n", wan_if, lan_if, dtype);
       	}

	/* Write forward chain rules of NAT */
	if ((fp1 = fopen("/tmp/nat_forward_rules", "r"))!=NULL)
	{
		while(fgets(line, sizeof(line), fp1))
		{
			fprintf(fp, "%s", line);
		}
		fclose(fp1);
	}


	/* logaccept chain */
	fprintf(fp, "-A logaccept -m state --state NEW -j LOG --log-prefix \"ACCEPT \" "
		  "--log-tcp-sequence --log-tcp-options --log-ip-options\n"
		  "-A logaccept -j ACCEPT\n");

	/* logdrop chain */
	fprintf(fp,"-A logdrop -m state --state NEW -j LOG --log-prefix \"DROP\" "
		  "--log-tcp-sequence --log-tcp-options --log-ip-options\n"
		  "-A logdrop -j DROP\n");
	
	fprintf(fp, "COMMIT\n");	
       	fclose(fp);
	eval("iptables-restore", "/tmp/filter_rules");	
}

int porttrigger_setting()
{
	netconf_app_t apptarget, *app;
	int i;
	char *out_proto, *in_proto, *out_port, *in_port, *desc;
	int  out_start, out_end, in_start, in_end;

	if (nvram_invmatch("wan_nat_x", "1") ||
	    nvram_invmatch("autofw_enable_x", "1")) 
		return -1;
	
	g_buf_init();
   				     		
     	foreach_x("autofw_num_x")
     	{	
     		out_proto = proto_conv("autofw_outproto_x", i);
     		out_port = portrange_ex2_conv("autofw_outport_x", i, &out_start, &out_end);
     		in_proto = proto_conv("autofw_inproto_x", i);
		in_port = portrange_ex2_conv("autofw_inport_x", i, &in_start, &in_end);
		desc = general_conv("autofw_desc_x", i);

		app = &apptarget;
		memset(app, 0, sizeof(netconf_app_t));

		/* Parse outbound protocol */
		if (!strncasecmp(out_proto, "tcp", 3))
			app->match.ipproto = IPPROTO_TCP;
		else if (!strncasecmp(out_proto, "udp", 3))
			app->match.ipproto = IPPROTO_UDP;
		else continue;

		/* Parse outbound port range */
		app->match.dst.ports[0] = htons(out_start);
		app->match.dst.ports[1] = htons(out_end);

		/* Parse related protocol */
		if (!strncasecmp(in_proto, "tcp", 3))
			app->proto = IPPROTO_TCP;
		else if (!strncasecmp(in_proto, "udp", 3))
			app->proto = IPPROTO_UDP;
		else continue;

		/* Parse related destination port range */
		app->dport[0] = htons(in_start);
		app->dport[1] = htons(in_end);

		/* Parse mapped destination port range */
		app->to[0] = htons(in_start);
		app->to[1] = htons(in_end);

		/* Parse description */
		if (desc)
			strncpy(app->desc, desc, sizeof(app->desc));

		/* Set interface name (match packets entering LAN interface) */
		strncpy(app->match.in.name, nvram_safe_get("lan_ifname"), IFNAMSIZ);

		/* Set LAN source port range (match packets from any source port) */
		app->match.src.ports[1] = htons(0xffff);

		/* Set target (application specific port forward) */
		app->target = NETCONF_APP;

		if (valid_autofw_port(app))
		{	
			netconf_add_fw((netconf_fw_t *)app);
		}
	}
}

int
start_firewall_ex(char *wan_if, char *wan_ip, char *lan_if, char *lan_ip)
{
	DIR *dir;
	struct dirent *file;
	FILE *fp;
	char name[NAME_MAX];
	char logaccept[32], logdrop[32];
	
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
	closedir(dir);


	/* Determine the log type */
	if (nvram_match("fw_log_x", "accept") || nvram_match("fw_log_x", "both"))
		strcpy(logaccept, "logaccept");
	else strcpy(logaccept, "ACCEPT");

	if (nvram_match("fw_log_x", "drop") || nvram_match("fw_log_x", "both"))
		strcpy(logdrop, "logdrop");
	else strcpy(logdrop, "DROP");
		

	/* nat setting */
	nat_setting(wan_if, wan_ip, lan_if, lan_ip, logaccept, logdrop);

	/* Filter setting */
	filter_setting(wan_if, wan_ip, lan_if, lan_ip, logaccept, logdrop);

	/* Trigger port setting */
	porttrigger_setting();

#ifdef XBOX_SUPPORT
	if( (fp=fopen("/proc/sys/net/ipv4/ip_conntrack_udp_timeouts", "r+")) ){
		fprintf(fp, "%d %d", 65, 180);
		fclose(fp);
	} else
		perror("/proc/sys/net/ipv4/ip_conntrack_udp_timeouts");
#endif

	if( (fp=fopen("/proc/sys/net/ipv4/ip_forward", "r+")) ){
		fputc('1', fp);
		fclose(fp);
	} else
		perror("/proc/sys/net/ipv4/ip_forward");

	return ;
}


portmapping_main(int argc, char *argv[])
{	
	char actionname[32], portname[32], ipname[32];
	// Check wan interface
	// argv[1] = Set or Unset	
	// argv[2] = Port
	// argv[3] = IP
	// argv[4] = Item
	sprintf(actionname, "4_MappedAction_%S", argv[4]);
	sprintf(ipname, "4_MappedIP_%s", argv[4]);
	sprintf(portname, "4_MappedInPort_%s", argv[4]); 
	
	if (strcmp(argv[1], "Set")==0)
	{		
		nvram_set(actionname, argv[1]);
		nvram_set(portname, argv[2]);
		nvram_set(ipname, argv[3]);
	}
	else
	{
		nvram_set(actionname, argv[1]);
	}
	
	if (nvram_match("wan_proto", "pppoe"))
	{
		start_firewall_ex("ppp0", "", "br0", "");
	}	
	else
	{
		start_firewall_ex("eth0", "", "br0", "");
	}		
}
#endif	

void write_static_leases(char *file)
{
	FILE *fp;
	char *ip, *mac;
	int i;

	fp=fopen(file, "w");

	if (fp==NULL) return;
	
	g_buf_init();
			
       	foreach_x("dhcp_staticnum_x")
       	{	               			
            	ip = general_conv("dhcp_staticip_x", i);
		mac = general_conv("dhcp_staticmac_x", i);
		fprintf(fp, "%s,%s\r\n", ip, mac);
	}
	fclose(fp);
}

void convert_routes(void)
{
	int i;
	char *ip, *netmask, *gateway, *matric, *interface;
	char wroutes[400], lroutes[400];

	wroutes[0] = 0;
	lroutes[0] = 0;	

	g_buf_init();
			
	foreach_x("sr_num_x")
	{
		ip = general_conv("sr_ipaddr_x", i);
		netmask = general_conv("sr_netmask_x", i);
		gateway = general_conv("sr_gateway_x", i);
		matric = general_conv("sr_matric_x", i);
		interface = general_conv("sr_if_x", i);


		printf("%x %s %s %s %s %s\n", i, ip, netmask, gateway, matric, interface);

		if (!strcmp(interface, "WAN"))
		{		
			sprintf(wroutes, "%s %s:%s:%s:%d", wroutes, ip, netmask, gateway, atoi(matric)+1);
		}
		else if (!strcmp(interface, "LAN"))
		{
			sprintf(lroutes, "%s %s:%s:%s:%d", lroutes, ip, netmask, gateway, atoi(matric)+1);
		}	
	}

	//printf("route: %s %s\n", lroutes, wroutes);
	nvram_set("lan_route", lroutes);
	nvram_set("wan0_route", wroutes);
}


#ifdef REMOVE	
void write_ftp_banip(FILE *fp)
{
	char *ip;
	int i;

	g_buf_init();
			
       	foreach_x("usb_bannum_x")
       	{	               			
            	ip = general_conv("usb_ftpbanip_x", i);
		fprintf(fp, "ban=%s\n", ip);
	}
}


void write_ftp_userlist(FILE *fp)
{
	char *user, *pass, *max, *rights;
	int i, maxuser;
	char passwd[32];
	char dir[64];

	g_buf_init();

	mkdir_if_none("/tmp/harddisk/part0/ftp_pub");
	mkdir_if_none("/tmp/harddisk/part0/ftp_pvt");
		
       	foreach_x("usb_ftpnum_x")
       	{	               			
            	user = general_conv("usb_ftpusername_x", i);
            	pass = general_conv("usb_ftppasswd_x", i);
            	max = general_conv("usb_ftpmaxuser_x", i);
            	rights = general_conv("usb_ftprights_x", i);

		if (strlen(max)==0) maxuser=0;
		else maxuser=atoi(max);

		if (strlen(pass)==0) strcpy(passwd, "*");
		else strcpy(passwd, pass);

		if (strcmp(rights, "Private")==0)
		{
			fprintf(fp, "user=%s %s /ftp_pvt/%s/ %d A\n", user, passwd, user, maxuser);
			sprintf(dir, "/tmp/harddisk/part0/ftp_pvt/%s", user);
			mkdir_if_none(dir);
		}
		else if (strcmp(rights, "Read/Write/Erase")==0)
			fprintf(fp, "user=%s %s /ftp_pub/ %d A\n", user, passwd, maxuser);
		else if (strcmp(rights, "Read/write")==0)
			fprintf(fp, "user=%s %s /ftp_pub/ %d DUM\n", user, passwd, maxuser);
		else if (strcmp(rights, "Read Only")==0)
			fprintf(fp, "user=%s %s /ftp_pub/ %d DM\n", user, passwd, maxuser);
		else	fprintf(fp, "user=%s %s /ftp_pub/ %d -\n", user, passwd, maxuser);
	}
}
#endif



void write_acl_list(char *file)
{
	FILE *fp;
	char *mac, macbuf[36];
	int i;

	fp=fopen(file, "w");

	if (fp==NULL) return;
	
	g_buf_init();
			
       	foreach_x("wl_macnum_x")
       	{	               			
            	mac = mac_conv("wl_maclist_x", i, macbuf);
		//printf("write: to file: %d %s\n", i, mac);
		fprintf(fp, "%s\n", mac);
		//printf("count only %d\n", i);
	}
	fclose(fp);
	return;
}

#ifdef REMOVE
int portforward_setting()
{
	netconf_nat_t nattarget, *nat;
	int i;
	char *out_proto, *in_proto, *out_port, *in_port, *desc;
	int  out_start, out_end, in_start, in_end;

	if (nvram_invmatch("wan_nat_x", "1") ||
	    nvram_invmatch("vts_enable_x", "1")) 
		return -1;
	
	g_buf_init();


     	foreach_x("vts_num_x")
     	{
     		char *proto;
		char *protono;
		char *port;
		char *dstip;
     				
     		proto = proto_conv("vts_proto_x", i);
		protono = portrange_conv("vts_protono_x", i);
		port = portrange_conv("vts_port_x", i);
		dstip = ip_conv("vts_ipaddr_x", i);
     				
		if (strcmp(proto, "tcp")==0 || strcmp(proto, "both")==0)
		{

			
		}
	}	


   				     		
     	foreach_x("autofw_num_x")
     	{	
     		out_proto = proto_conv("autofw_outproto_x", i);
     		out_port = portrange_ex2_conv("autofw_outport_x", i, &out_start, &out_end);
     		in_proto = proto_conv("autofw_inproto_x", i);
		in_port = portrange_ex2_conv("autofw_inport_x", i, &in_start, &in_end);
		desc = general_conv("autofw_desc_x", i);

		app = &apptarget;
		memset(app, 0, sizeof(netconf_app_t));

		/* Parse outbound protocol */
		if (!strncasecmp(out_proto, "tcp", 3))
			app->match.ipproto = IPPROTO_TCP;
		else if (!strncasecmp(out_proto, "udp", 3))
			app->match.ipproto = IPPROTO_UDP;
		else continue;

		/* Parse outbound port range */
		app->match.dst.ports[0] = htons(out_start);
		app->match.dst.ports[1] = htons(out_end);

		/* Parse related protocol */
		if (!strncasecmp(in_proto, "tcp", 3))
			app->proto = IPPROTO_TCP;
		else if (!strncasecmp(in_proto, "udp", 3))
			app->proto = IPPROTO_UDP;
		else continue;

		/* Parse related destination port range */
		app->dport[0] = htons(in_start);
		app->dport[1] = htons(in_end);

		/* Parse mapped destination port range */
		app->to[0] = htons(in_start);
		app->to[1] = htons(in_end);

		/* Parse description */
		if (desc)
			strncpy(app->desc, desc, sizeof(app->desc));

		/* Set interface name (match packets entering LAN interface) */
		strncpy(app->match.in.name, nvram_safe_get("lan_ifname"), IFNAMSIZ);

		/* Set LAN source port range (match packets from any source port) */
		app->match.src.ports[1] = htons(0xffff);

		/* Set target (application specific port forward) */
		app->target = NETCONF_APP;

		if (valid_forward_port(app))
		{	
			netconf_add_fw((netconf_fw_t *)app);
		}
	}
}


int lanfilter_setting()
{
	netconf_app_t apptarget, *app;
	int i;

	if (nvram_invmatch("filter_lw_enable_x", "1")) 
		return -1;
	
	g_buf_init();


	timematch_conv(lanwan_timematch, "filter_lw_date_x", "filter_lw_time_x");	    
	// LAN/WAN filter		
	g_buf_init();

	if (nvram_match("filter_lw_default_x", "DROP"))
	{
		dtype = logdrop;
		ftype = logaccept;
	}
	else
	{
		dtype = logaccept;
		ftype = logdrop;
	}
			
       	foreach_x("filter_lw_num_x")
       	{	               			
           	proto = protoflag_conv("filter_lw_proto_x", i, 0);
            	flag = protoflag_conv("filter_lw_proto_x", i, 1);
		srcip = ip_conv("filter_lw_srcip_x", i);
		srcport = portrange_conv("filter_lw_srcport_x", i);
		dstip = ip_conv("filter_lw_dstip_x", i);
		dstport = portrange_conv("filter_lw_dstport_x", i);	
		setting=filter_conv(proto, flag, srcip, srcport, dstip, dstport);
	}
		     		
     	foreach_x("autofw_num_x")
     	{	
     		out_proto = proto_conv("autofw_outproto_x", i);
     		out_port = portrange_ex2_conv("autofw_outport_x", i, &out_start, &out_end);
     		in_proto = proto_conv("autofw_inproto_x", i);
		in_port = portrange_ex2_conv("autofw_inport_x", i, &in_start, &in_end);
		desc = general_conv("autofw_desc_x", i);

		app = &apptarget;
		memset(app, 0, sizeof(netconf_app_t));

		/* Parse outbound protocol */
		if (!strncasecmp(out_proto, "tcp", 3))
			app->match.ipproto = IPPROTO_TCP;
		else if (!strncasecmp(out_proto, "udp", 3))
			app->match.ipproto = IPPROTO_UDP;
		else continue;

		/* Parse outbound port range */
		app->match.dst.ports[0] = htons(out_start);
		app->match.dst.ports[1] = htons(out_end);

		/* Parse related protocol */
		if (!strncasecmp(in_proto, "tcp", 3))
			app->proto = IPPROTO_TCP;
		else if (!strncasecmp(in_proto, "udp", 3))
			app->proto = IPPROTO_UDP;
		else continue;

		/* Parse related destination port range */
		app->dport[0] = htons(in_start);
		app->dport[1] = htons(in_end);

		/* Parse mapped destination port range */
		app->to[0] = htons(in_start);
		app->to[1] = htons(in_end);

		/* Parse description */
		if (desc)
			strncpy(app->desc, desc, sizeof(app->desc));

		/* Set interface name (match packets entering LAN interface) */
		strncpy(app->match.in.name, nvram_safe_get("lan_ifname"), IFNAMSIZ);

		/* Set LAN source port range (match packets from any source port) */
		app->match.src.ports[1] = htons(0xffff);

		/* Set target (application specific port forward) */
		app->target = NETCONF_APP;

		if (valid_autofw_port(app))
		{	
			netconf_add_fw((netconf_fw_t *)app);
		}
	}
}
#endif

bool
get_forward_port_ex(int which, netconf_nat_t *nat)
{
	int num;
	//char name[] = "forward_portXXXXXXXXXX", value[1000];
	char *enable, *desc;
	char *lan_ipaddr, *lan_port, *proto, *dst_port;
	int  in_start, in_end, out_dst;
	char lanwan_timematch[128];
	char ptr[32], *icmplist;

	num=atoi(nvram_safe_get("vts_num_x"));

	if (which>=num) return FALSE;

	//printf("forward: %x %x\n", num, which);

	g_buf_init();

     	lan_ipaddr = ip_conv("vts_ipaddr_x", which);
     	lan_port = portrange_ex2_conv("vts_port_x", which, &in_start, &in_end);
     	proto = proto_conv("vts_proto_x", which);
	dst_port = portrange_conv("vts_lport_x", which);
	
	/* Parse outbound protocol */
	memset(nat, 0, sizeof(netconf_nat_t));

	// strncpy(value, nvram_get(name), sizeof(value)); marked by Yen

	/* Parse WAN destination port range */
	nat->match.dst.ports[0] = htons(in_start);
	nat->match.dst.ports[1] = htons(in_end);

	/* Parse LAN IP address */
	(void) inet_aton(lan_ipaddr, &nat->ipaddr);

	/* Parse LAN destination port range */
	out_dst = htons(atoi(dst_port));

	if (out_dst!=0)
	{
		nat->ports[0] = out_dst;
		nat->ports[1] = out_dst;
	}
	else
	{
		nat->ports[0] = htons(in_start);
		nat->ports[1] = htons(in_end);
	}

	/* Parse protocol */
	if (!strncasecmp(proto, "tcp", 3))
		nat->match.ipproto = IPPROTO_TCP;
	else if (!strncasecmp(proto, "udp", 3))
		nat->match.ipproto = IPPROTO_UDP;
	else
		return FALSE;

	/* Set WAN source port range (match packets from any source port) */
	nat->match.src.ports[1] = htons(0xffff);

	/* Set target (DNAT) */
	nat->target = NETCONF_DNAT;

	//printf("add forward\n");
	return valid_forward_port(nat);
}

bool
get_autofw_port_ex(int which, netconf_app_t *app)
{
	int num;
	char *out_proto, *in_proto, *out_port, *in_port, *desc;
	int  out_start, out_end, in_start, in_end;
	char lanwan_timematch[128];
	char ptr[32], *icmplist;
	char *ftype, *dtype;

	num=atoi(nvram_safe_get("autofw_num_x"));

	if (which>=num) return FALSE;

	g_buf_init();

     	out_proto = proto_conv("autofw_outproto_x", which);
     	out_port = portrange_ex2_conv("autofw_outport_x", which, &out_start, &out_end);
     	in_proto = proto_conv("autofw_inproto_x", which);

	in_port = portrange_ex2_conv("autofw_inport_x", which, &in_start, &in_end);

	desc = general_conv("autofw_desc_x", which);

	memset(app, 0, sizeof(netconf_app_t));

	/* Parse outbound protocol */
	if (!strncasecmp(out_proto, "tcp", 3))
		app->match.ipproto = IPPROTO_TCP;
	else if (!strncasecmp(out_proto, "udp", 3))
		app->match.ipproto = IPPROTO_UDP;
	else return FALSE;

	/* Parse outbound port range */
	app->match.dst.ports[0] = htons(out_start);
	app->match.dst.ports[1] = htons(out_end);

	/* Parse related protocol */
	if (!strncasecmp(in_proto, "tcp", 3))
		app->proto = IPPROTO_TCP;
	else if (!strncasecmp(in_proto, "udp", 3))
		app->proto = IPPROTO_UDP;
	else return FALSE;

	/* Parse related destination port range */
	app->dport[0] = htons(in_start);
	app->dport[1] = htons(in_end);

	/* Parse mapped destination port range */
	app->to[0] = htons(in_start);
	app->to[1] = htons(in_end);

	/* Set interface name (match packets entering LAN interface) */
	strncpy(app->match.in.name, nvram_safe_get("lan_ifname"), IFNAMSIZ);

	/* Set LAN source port range (match packets from any source port) */
	app->match.src.ports[1] = htons(0xffff);

	/* Set target (application specific port forward) */
	app->target = NETCONF_APP;


	return valid_autofw_port(app);
}


bool
get_filter_client_ex(int which, netconf_filter_t *start, netconf_filter_t *end, int *days)
{
	int num;
	//char name[] = "filter_clientXXXXXXXXXX", value[1000];
	char *lan_ipaddr, *dstip, *dstport, *proto;
	unsigned int srcip, srcmask;
	int day_start, day_end, sec_start, sec_end;
	int dststart, dstend;

	num=atoi(nvram_safe_get("filter_lw_num_x"));

	if (which>=num) return FALSE;

	//printf("filter num: %x %x\n", num, which);

	g_buf_init();

	timematch_ex_conv("filter_lw_date_x", "filter_lw_time_x", days, &sec_start, &sec_end);	    	
        proto = protoflag_conv("filter_lw_proto_x", which, 0);
	lan_ipaddr = iprange_ex2_conv("filter_lw_srcip_x", which, &srcip, &srcmask);
	dstport = portrange_ex2_conv("filter_lw_dstport_x", which, &dststart, &dstend);

	memset(start, 0, sizeof(netconf_filter_t));
	memset(end, 0, sizeof(netconf_filter_t));

	start->match.src.ipaddr.s_addr = end->match.src.ipaddr.s_addr = htonl(srcip);
	start->match.src.netmask.s_addr = end->match.src.netmask.s_addr = htonl(srcmask);

	/* Parse destination port range */
	start->match.dst.ports[0] = end->match.dst.ports[0] = htons(dststart);
	start->match.dst.ports[1] = end->match.dst.ports[1] = htons(dstend);

	/* Parse protocol */
	if (!strncasecmp(proto, "tcp", 3))
		start->match.ipproto = end->match.ipproto = IPPROTO_TCP;
	else if (!strncasecmp(proto, "udp", 3))
		start->match.ipproto = end->match.ipproto = IPPROTO_UDP;
	else
	{
		return FALSE;
	}

	/* Parse day range */
	start->match.days[0] = end->match.days[0] = 0;
	start->match.days[1] = end->match.days[1] = 0;

	/* Parse time range */
	start->match.secs[0] = end->match.secs[0] = sec_start;
	start->match.secs[1] = end->match.secs[1] = sec_end;

	/* Set interface name (match packets entering LAN interface) */
	strncpy(start->match.in.name, nvram_safe_get("lan_ifname"), IFNAMSIZ);

	/* Set source port range (match packets from any source port) */
	start->match.src.ports[1] = end->match.src.ports[1] = htons(0xffff);

	/* Set default target (drop) */
	start->target = NETCONF_DROP;

	return valid_filter_client(start, end);
}
