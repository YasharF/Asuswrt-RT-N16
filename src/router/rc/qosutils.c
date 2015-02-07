/************************************************************************************************************

 * File Name	: qosutile.c
 * Description  : modify from WRT54GS-v4.70.6:voip_qos.c. Called by speedtest.
 * Auther       : Wendel Huang.
 * History	: 2006.05.22 modify to post on WL500gP.
		  2006.07.18 Post to WL700g, and remove execution message from release version.
		  2006.08.08 modify check_wan_link() to ckeck WL700g wan status.
		  2006.08.14 modify get_dns_list() to get WL700g's DNS setting.
 ************************************************************************************************************/

#include <sys/ioctl.h>
#include <net/if.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <linux/if_ether.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <linux/sockios.h>

#include "shutils.h"
#include "bcmnvram.h"
#include <dirent.h>	// 2008.01 James.
#define IFC_TINY_LEN 	16
#include "qosutils.h"

#ifdef WL500GP	//For WL500gP, WL550gE
int file_to_buf(char *path, char *buf, int len)
{
        FILE *fp;
                                                                                                 
        memset(buf, 0 , len);
                                                                                                 
        if ((fp = fopen(path, "r"))) {
                fgets(buf, len, fp);
                fclose(fp);
                return 1;
        }
                                                                                                 
        return 0;
}

int get_ppp_pid(char *conntype)
{
        int pid = -1;
        char tmp[80], tmp1[80];
        snprintf(tmp, sizeof(tmp), "/var/run/%s.pid", conntype);
#ifdef QOS_DEBUG                                                                                                                        
	printf("	get_ppp_pid() ** tmp = %s\n", tmp);
#endif
        file_to_buf(tmp, tmp1, sizeof(tmp1));
        pid = atoi(tmp1);
#ifdef QOS_DEBUG
	printf("	get_ppp_pid() ** pid = %d\n", pid);
#endif
        return pid;
}

/* Find process name by pid from /proc directory */
char *find_name_by_proc(int pid)
{
        FILE *fp;
        char line[254];
        char filename[80];
        static char name[80];
                                                                                                 
        snprintf(filename, sizeof(filename), "/proc/%d/status", pid);
                                                                                                 
        if((fp = fopen(filename, "r"))){
                fgets(line, sizeof(line), fp);
                /* Buffer should contain a string like "Name:   binary_name" */
                sscanf(line, "%*s %s", name);
#ifdef QOS_DEBUG
		printf("	find_name_by_proc() ** name=%s\n", name);
#endif
                fclose(fp);
                return name;
        }
                                                                                                 
        return "";
}

int osl_ifflags(const char *ifname)
{
    int sockfd;
    struct ifreq ifreq;
    int flags = 0;
                                                                                                 
    if ((sockfd = socket( AF_INET, SOCK_DGRAM, 0 )) < 0) {
        perror("socket");
        return flags;
    }
                                                                                                 
    strncpy(ifreq.ifr_name, ifname, IFNAMSIZ);
    if (ioctl(sockfd, SIOCGIFFLAGS, &ifreq) < 0) {
            flags = 0;
    } else {
            flags = ifreq.ifr_flags;
    }
    close(sockfd);
    return flags;
}

// Added by Joey for ethtool
#include <net/if.h>
#include "ethtool-util.h"
#undef SIOCETHTOOL
#define	SIOCETHTOOL	0x8946
#define	SIOCGETCPHYRD	0x89FE
                                                                                                                                               
int is_phyconnected(char *wan_if)
{
        int fd, err;
        struct ifreq ifr;
        struct ethtool_cmd ecmd;
                                                                                                                                               
        memset(&ifr, 0, sizeof(ifr));
#ifdef RTN10
        strcpy(ifr.ifr_name, "eth0");
#else
        strcpy(ifr.ifr_name, wan_if);
#endif
        fd=socket(AF_INET, SOCK_DGRAM, 0);
        if (fd<0)
        {
                return 0;
        }
        ecmd.cmd=ETHTOOL_GSET;
        ifr.ifr_data = (caddr_t)&ecmd;
        err=ioctl(fd, SIOCETHTOOL, &ifr);
//        close(fd);
		printf("cable not attached 1\n");
        if (err==0)
        {
		printf("cable not attached 2\n");

                // 2006_06_16_Roly
                // 2001.01.08 Modified for BCM5354 Platform
                // detect wan port connection

                int vecarg[2];
                int verbuf;
                                                                                                     
                vecarg[0] = strtoul("0x01", NULL, 0) << 16;
                vecarg[0] |= strtoul("0x00", NULL, 0) & 0xffff;
                                                                                                                             
                ifr.ifr_data = (caddr_t) vecarg;
                if (ioctl(fd, SIOCGETCPHYRD, (caddr_t)&ifr) < 0)
                        printf("etcphyrd error\n");
                printf("0x%04x\n", vecarg[1]);
                close(fd);
                                                                                                                             
                verbuf=vecarg[1];
                        
//                if((verbuf & 0x0010) != 0)
#ifdef RTN16
                if((verbuf & 0x0001) != 0)
#else
                if((verbuf & 0x0010) != 0)
#endif
                        return 1;
        }
        return 0;
}

/**************************************************************************
 * Function Name: check_wan_link()
 * Description  : Check Wan interface's connection status on WL500gp 
 * Parameters   : int num -> wan interface index
 * Returns      : wan status
			0 : WAN link fail
			1 : WAN link UP
 **************************************************************************/
int check_wan_link(int num)
{
	int wan_link = 0;
	char conntype[10];
	
	//Check WAN Cable connect
	if(is_phyconnected(nvram_safe_get("wan_ifname")) == 0){
		wan_link = 0;
 		nvram_set("qos_ubw_status", "fail");
		nvram_set("qos_ubw_reason", "Cable is not attached");
		
		return wan_link;
	}
	
	if(nvram_match("wan_proto", "pptp")
			|| nvram_match("wan_proto", "PPTP")
			|| nvram_match("wan_proto", "l2tp")
			|| nvram_match("wan_proto", "pppoe")
			|| nvram_match("wan_proto", "PPPoE")
			|| nvram_match("wan_proto", "BigPong")
			|| nvram_match("wan_proto", "heartbeat")
			){
		FILE *fp;
		char filename[80];
		char buf1[256];
		char *name;
		char* pos;
		char* cpos;
		DIR *ppp_dir;	// 2008.01 James.
		struct dirent *entry;	// 2008.01 James.
		
		if(num == 0){

			memset(filename, 0, 80);
			ppp_dir = opendir("/tmp/ppp");

			if (ppp_dir == NULL) {
				nvram_set("qos_ubw_status", "fail");
				return wan_link;
			}
			
			while((entry = readdir(ppp_dir)) != NULL){
				if(!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, ".."))
					continue;
				
				if((pos = strstr(entry->d_name, "link")) != NULL){
					sprintf(filename, "/tmp/ppp/%s", entry->d_name);
					strcpy(conntype, pos+5);
					
					break;
				}
			}
			closedir(ppp_dir);
		}
		
		if(strlen(filename) > 0 && (fp = fopen(filename, "r")) != NULL){
			int pid = -1;
			fclose(fp);
			
			if(nvram_match("wan_proto", "heartbeat")){
				char buf[20];
				file_to_buf(filename, buf, sizeof(buf));
				pid = atoi(buf);
			}
			else
				pid = get_ppp_pid(conntype);
			
			name = find_name_by_proc(pid);
			
			if(!strncmp(name, "pppoecd", 7) ||		// for PPPoE
					!strncmp(name, "pppd", 4) ||	// for PPTP
					!strncmp(name, "bpalogin", 8)){	// for HeartBeat
				wan_link = 1;	//connect
 				nvram_set("qos_ubw_status", "initialing");
			}
			else{
#ifdef QOS_DEBUG
				printf("The %s had been died, remove %s\n", nvram_safe_get("wan_proto"), filename);
#endif
				wan_link = 0;   
				// For some reason, the pppoed had been died, by link file still exist.
				unlink(filename);
			}
		}
	}
	else{
		if(nvram_invmatch("wan_ipaddr", "0.0.0.0")){
			wan_link = 1;
 			nvram_set("qos_ubw_status", "initialing");
		}
	}
	
#ifdef QOS_DEBUG
	printf("	check_wan_link() ** wan_link = %d\n", wan_link);
#endif
	
	return wan_link;
}

/**************************************************************************
 * Function Name: get_dns_list()
 * Description  : get DNS server ip from user configuration or retrieved from ISP
 * Parameters   : int no -> indicate using static or dynamaic dns preferentially
                        no = 0 : Static DNS
                        no = 1 : dynamic DNS
 * Returns      : struct dns_lists *  -> contain DNS IP infomation
 **************************************************************************/
struct dns_lists *get_dns_list(int no)
{
	char list[254], prelist[254];
	char *next, word[254];
	struct dns_lists *dns_list = NULL;
	int i, match = 0;
	char got_dns[2][15]={"wan_dns1_x", "wan_dns2_x"};
	
	dns_list = (struct dns_lists *)malloc(sizeof(struct dns_lists));
	memset(dns_list, 0, sizeof(struct dns_lists));
	dns_list->num_servers = 0;
	
	if(no == 0){
		snprintf(prelist, sizeof(prelist), "%s %s", nvram_safe_get(got_dns[0]), nvram_safe_get(got_dns[1]));
		snprintf(list, sizeof(list), "%s %s", prelist , nvram_safe_get("wan0_dns"));
	}
	else{
		//snprintf(prelist, sizeof(prelist), "%s", nvram_safe_get("wan0_dns"));
		snprintf(prelist, sizeof(prelist), "%s", nvram_safe_get("wan_dns_t"));	// 2008.05 James.
		snprintf(list, sizeof(list), "%s %s %s", prelist , nvram_safe_get(got_dns[0]), nvram_safe_get(got_dns[1]));
	}
	
	foreach(word, list, next){
		if(strcmp(word, "0.0.0.0") && strcmp(word, "")){
			match = 0;
			for(i = 0; i < dns_list->num_servers; i++){       // Skip same DNS
				if(!strcmp(dns_list->dns_server[i], word))
					match = 1;
			}
			
			if(!match){
				snprintf(dns_list->dns_server[dns_list->num_servers], sizeof(dns_list->dns_server[dns_list->num_servers]), "%s", word);
				dns_list->num_servers ++;
			}
		}
		
		if(dns_list->num_servers == 4)
			break;      // We only need 3 counts DNS entry
	}
	
	return dns_list;
}

/**************************************************************************
 * Function Name: get_gw_ip()
 * Description  : get gateway IP
 * Parameters   : None
 * Returns      : char*  -> Gateway IP String
 **************************************************************************/
char *get_gw_ip(void) {
	char *addr=NULL;

	addr = (char *)malloc(IFC_TINY_LEN);
	
	strcpy(addr, nvram_safe_get("lan_ipaddr"));	

	return addr;
}
#endif // WL500GP


