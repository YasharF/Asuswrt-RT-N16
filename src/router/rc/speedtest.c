/*****************************************************************************************************************
 * Filr Name	: speedtest.c
 * Description  : modify from ping.c
 * related file : qosutils.h qosutils.c
 * Auther       : Wendel Huang
 * History	: 2006.05.22 	post to WL500gP
		  2006.07.18 	Post to WL700g, and remove execution message form release version
		  2006.08.01    Debug the error in MER mode
 *****************************************************************************************************************/
#include <stdio.h>
#include <sys/param.h>
#include <sys/socket.h>
#include <sys/file.h>
#include <sys/time.h>
#include <sys/times.h>
#include <sys/signal.h>
#include <string.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <shutils.h>

#include <asm/unistd.h>
#include <linux/unistd.h>

#include "qosutils.h"

#include <sys/ioctl.h>
#include <net/if.h>
#include <ethtool-util.h>
#include <shutils.h>
#include <bcmnvram.h>

#ifdef WL600g
extern ADSL_CONNECTION_INFO glbAdslInfo;// global ADSL info variable is declared in syscall.c
#endif

#define STRUCT_LEN(name)    sizeof(name)/sizeof(name[0])
#define MAX_BANDWIDTH	15359	// S = (1500-64)*8/1024 (kbits)
				// S *1000 / (thelta t) * 1.369 (weight) = B
#define DETECT_FILE "/tmp/detect_ip"
#define INIT_INTERVAL 200000

static const int DEFDATALEN = 56;
static const int MAXIPLEN = 60;
static const int MAXICMPLEN = 76;
static const int MAXWAIT = 10;
static const int ADSLMXLEN =1400;
static const int HEADOVERHEAD = 42;
unsigned long int triptime[18];		//14(frame header)+20(ip header)+8(icmp header)
static int request_time=1;
static int interval=INIT_INTERVAL;

#define	A(bit)		rcvd_tbl[(bit)>>3]	/* identify byte in array */
#define	B(bit)		(1 << ((bit) & 0x07))	/* identify bit in byte */
#define	SET(bit)	(A(bit) |= B(bit))
#define	CLR(bit)	(A(bit) &= (~B(bit)))
#define	TST(bit)	(A(bit) & B(bit))

enum { WAN_RATE_SUCCESS, WAN_LINK_FAIL };

/*struct icmp_map_t{
	unsigned difference;
	unsigned rate;
} icmp_map[] = {
	{3000, MAX_BANDWIDTH},
	{1638, 2048},
	{819, 1024},
	{600, 640},
	{410, 512},
	{204, 256},
	{102, 128},
	{51, 64},
	{27, 33},
};//difference = rate * 0.8
*/

//2008.12 SZ-magic{
#define csprintf(fmt, args...) do{\
	FILE *cp = fopen("/dev/console", "w");\
	if(cp) {\
		fprintf(cp, fmt, ## args);\
		fclose(cp);\
	}\
}while(0)
//2008.12 SZ-magic}

struct wan_link_rate {
	unsigned ds;
	unsigned us;
};

/* common routines */
static int in_cksum(unsigned short *buf, int sz)
{
	int nleft = sz;
	int sum = 0;
	unsigned short *w = buf;
	unsigned short ans = 0;

	while (nleft > 1) {
		sum += *w++;
		nleft -= 2;
	}

	if (nleft == 1) {
		*(unsigned char *) (&ans) = *(unsigned char *) w;
		sum += ans;
	}

	sum = (sum >> 16) + (sum & 0xFFFF);
	sum += (sum >> 16);
	ans = ~sum;
	return (ans);
}

static struct sockaddr_in pingaddr;

int datalen; /* intentionally uninitialized to work around gcc bug */

// brcm: changed default value of pingcount from 0 to 4.
static long ntransmitted=0, nreceived=0;
int pingcount=10;
static int myid;
static struct timeval tstart;	// tend;

struct hostent *hostent;

static char gateway_ip[10][30], get_sp[6][10];
static void sendping();
static int unpack(char *, int, struct sockaddr_in *);
static int detect();
static int pre_ping(const char *host);
//static void sendtrack();

/**************************************************************************/

//static void sendping(int junk)
static void sendping()
{
#ifdef QOS_DEBUG
	printf("	Start of sendping()\n");
#endif
	struct icmp *pkt;
	int i;
	char packet[datalen + 8];

	pkt = (struct icmp *) packet;

	int repeat = 0;
	ntransmitted=0;
	nreceived=0;
	switch(request_time)
	{
		case 1:
		case 2: 
		case 3:	
			pingcount=9;
			printf("pingcount:%d\n",pingcount);
			break;
		case 4:
		case 5:
		case 6:
			pingcount=15;
			printf("pingcount:%d\n",pingcount);
			break;
		case 7:
		case 8:
		case 9:
			pingcount=29;
			printf("pingcount:%d\n",pingcount);
			break;
		case 10:
		case 11:
		case 12:
			pingcount=54;
			printf("pingcount:%d\n",pingcount);
			break;
		case 13:
		case 14:
		case 15:
			pingcount=138;
			printf("pingcount:%d\n",pingcount);
			break;
		case 16:
		case 17:
		case 18:
			pingcount=238;
			printf("pingcount:%d\n",pingcount);
			break;
		default:
			printf("pingcount error!\n");
	}
	for ( ; repeat<pingcount ; repeat++ ) {
		int pingsock1;
		if( (pingsock1=socket(AF_INET, SOCK_RAW, IPPROTO_ICMP)) < 0 ) { 
#ifdef QOS_DEBUG
			printf("	deltatime() ** socket() Error!!\n");
#endif
			exit(2);
		}
		pkt->icmp_type = ICMP_ECHO;
		pkt->icmp_code = 0;
		if(repeat==pingcount-1)
		{
			pkt->icmp_cksum = 0;
		}
		else
		{		
			pkt->icmp_cksum = 1;
		}
		pkt->icmp_seq = htons(pingcount);
		pkt->icmp_id = myid;
		gettimeofday((struct timeval *)pkt->icmp_data, NULL);
		if(repeat==0)
		{
			gettimeofday(&tstart, NULL);
		}
		pkt->icmp_cksum = in_cksum((unsigned short *) pkt, sizeof(packet));

		struct timeval tmshow;
		memcpy(&tmshow, pkt->icmp_data, sizeof(struct timeval));

#ifdef QOS_DEBUG
		printf("	sendping() ** sequence = %d 	sendtime= %ld\n", pkt->icmp_seq, tmshow.tv_usec);
#endif
		int j=1;
		while(j<interval){
			j+=1;
		}

		i = sendto(pingsock1, packet, sizeof(packet), 0,
				(struct sockaddr *) &pingaddr, sizeof(struct sockaddr_in));
		//usleep(1);
		//2006.08.01	This is for sending echo-request error in MER mode
		//printf("	sendping() ** ntransmitted=%ld : sendto = %d\n", ntransmitted, i);
		//printf("QOS Detecting#%ld : sendto = %d\n", ntransmitted, i);
		//printf("QOS \n");

#ifdef QOS_DEBUG
		if(i<0) 
			switch(errno) {
				case EBADF:
					printf("EBADF\n");
				case EFAULT:
					printf("EFAULT\n");
				case ENOTSOCK:
					printf("ENOTSOCK\n");
				case EINTR:
					printf("EINTR\n");
				case EAGAIN:
					printf("EAGAIN\n");
				case ENOBUFS: 
					printf("ENOBUFS\n");
					//case ENOMEN:
					//printf("ENOMEN\n");
				case EINVAL:
					printf("EINVAL\n");
				default:
					printf("sendto DNS error\n");
			}
#endif

		if ( i<0 || (size_t)i != sizeof(packet) )
			printf("	sendping() ** sendto Error !!\n");
		close(pingsock1);
	}
}

static int unpack(char *buf, int sz, struct sockaddr_in *from)
{
	struct icmp *icmppkt;
	struct iphdr *iphdr;
	//struct timeval tv, *tp;
	struct timeval tv;
	int hlen;
	//unsigned long int triptime;

	gettimeofday(&tv, NULL);

	/* check IP header */
	iphdr = (struct iphdr *) buf;
	hlen = iphdr->ihl << 2;
	/* discard if too short */
	if (sz < (ICMP_MINLEN))
		return -1;

	sz -= hlen;
	icmppkt = (struct icmp *) (buf + hlen);

	if (icmppkt->icmp_type == ICMP_ECHOREPLY) {

		if (icmppkt->icmp_id != myid) {
#ifdef QOS_DEBUG
	    	    printf("	unpack() ** not my ping !! icmp->icmpid = %d : myid =%d \n", icmppkt->icmp_id, myid);	
#endif
	    	    return -1;				/* not our ping */
		}

	        ++nreceived;
		if ((tv.tv_usec -= tstart.tv_usec) < 0) {
			--tv.tv_sec;
			tv.tv_usec += 1000000;
		}
		tv.tv_sec -= tstart.tv_sec;

		triptime[request_time-1] = tv.tv_sec * 1000000 + tv.tv_usec;
		//nvram_set("qos_ubw_status", "initialing");
                /*brcm: changed display message to show actually receive data bytes length
                  rather than ICMP packet length which is ICMP_MINLEN+dataLen */
#ifdef QOS_DEBUG
		printf("	unpack() ** new sec = %ld	new usec = %ld\n", tv.tv_sec, tv.tv_usec);
#endif
	}
	else if (icmppkt->icmp_type == ICMP_UNREACH) {
		    nvram_set("qos_ubw_reason", "Host Unreachable");	

#ifdef QOS_DEBUG
		    printf("	unpack() ** DNS Host Unreachable !!\n");
#endif
		    nvram_set("qos_ubw_status", "fail");
		    return -1;
	}
	else {
		    nvram_set("qos_ubw_status", "fail");
#ifdef QOS_DEBUG
		    printf("	unpack() ** icmp->type : %d\n", icmppkt->icmp_type);
#endif
		    return -1;
	}

	return 1;
	

}

static int pre_ping(const char *host)
{
	int package=60;
//	char packet[package+MAXIPLEN+MAXICMPLEN];
	unsigned long int ul_hostip;
	struct timeval socktv;
	int pingsock = -1;
	myid = getpid() & 0xFFFF;
	
	if( (pingsock=socket(AF_INET, SOCK_RAW, IPPROTO_ICMP)) < 0 )
	{ 
		return 0;
	}
	
	socktv.tv_sec = 5;
	socktv.tv_usec = 0;
	setsockopt(pingsock, SOL_SOCKET, SO_RCVTIMEO, &socktv, sizeof(socktv));

	memset(&pingaddr, 0, sizeof(struct sockaddr_in));

	pingaddr.sin_family = AF_INET;
	
	if( (ul_hostip=inet_addr(host)) !=INADDR_NONE )
	{
		pingaddr.sin_addr.s_addr = ul_hostip;
	}
	else if((hostent=gethostbyname(host)) ) 
	{	
		memcpy(&pingaddr.sin_addr, hostent->h_addr, sizeof(pingaddr.sin_addr));
	}
	struct icmp *pkt;
	int i;
	char packet1[package + 8];

	pkt = (struct icmp *) packet1;

	int repeat = 0;
	ntransmitted=0;
	nreceived=0;
	for ( ; repeat<pingcount ; repeat++ ) {
	    pkt->icmp_type = ICMP_ECHO;
	    pkt->icmp_code = 0;
	    pkt->icmp_cksum = 0;
	    pkt->icmp_seq = ntransmitted++;
	    pkt->icmp_id = myid;
	    gettimeofday((struct timeval *)pkt->icmp_data, NULL);
	    gettimeofday(&tstart, NULL);
	    pkt->icmp_cksum = in_cksum((unsigned short *) pkt, sizeof(packet1));
	    struct timeval tmshow;
	    memcpy(&tmshow, pkt->icmp_data, sizeof(struct timeval));
	    i = sendto(pingsock, packet1, sizeof(packet1), 0,
	    		       (struct sockaddr *) &pingaddr, sizeof(struct sockaddr_in));
	    if ( i<0 || (size_t)i != sizeof(packet1) )
	    {
		    printf("	sendping() ** sendto Error !!\n");
		    return 0;
	    }
	}
	int runtimes = 1;
	int errotime = 0;
	while (1) {
		runtimes++;

		struct sockaddr_in from;
		socklen_t fromlen = (socklen_t) sizeof(from);
		int c = 0;

		c = recvfrom(pingsock, packet1, sizeof(packet1), 0,
						  (struct sockaddr *) &from, &fromlen);
		if (c < 0) {
			errotime++;		
	
			if (errno == EINTR) continue;

			if( errotime > 3 ) {
				nvram_set("qos_ubw_status", "fail");	
				nvram_set("qos_ubw_reason", "DNS server fail");
				return 0;
			}
			if (pingcount > 0 && runtimes>=pingcount)
			{
				return 0;
				break;
			}
		        else
				continue;
		}
		else if(c == 0) {
			return 0;
			nvram_set("qos_ubw_status", "fail");
		}
		if (pingcount > 0)
		{
			break;
		}
		
	}
	return 1;
		
}

extern int deltatime( )
{
#ifdef QOS_DEBUG
	printf("	Start of deltatime()\n");
#endif

	datalen =ADSLMXLEN;
	char packet[datalen + MAXIPLEN + MAXICMPLEN];
	struct timeval socktv;
	myid = getpid() & 0xFFFF;
	int pingsock2=-1;

	if( (pingsock2=socket(AF_INET, SOCK_RAW, IPPROTO_ICMP)) < 0 ) { 
#ifdef QOS_DEBUG
		printf("	deltatime() ** socket() Error!!\n");
#endif
		//exit(2);
		return -1;
	}

	socktv.tv_sec = 5;
	socktv.tv_usec = 0;
	setsockopt(pingsock2, SOL_SOCKET, SO_RCVTIMEO, &socktv, sizeof(socktv));

#ifdef QOS_DEBUG
	printf("	deltatime() ** host = %s\n", inet_ntoa(pingaddr.sin_addr));
#endif

	/* start the ping's going ... */
	//signal(SIGALRM, sendping);
	//alarm(1);
	sendping();

	/* listen for replies */
	//int runtimes = 1;
//	int errotime = 0;
	//while (1) {
	//runtimes++;
	struct sockaddr_in from;
	socklen_t fromlen = (socklen_t) sizeof(from);
	int c = 0;

	c = recvfrom(pingsock2, packet, sizeof(packet), 0,
			(struct sockaddr *) &from, &fromlen);
	if(c<0){
		return -1;
	}
	if(unpack(packet, c, &from)<0){
	close(pingsock2);
	return -1;
	}
	close(pingsock2);
#ifdef QOS_DEBUG
	printf("	deltatime() ** tv_sec=%ld : tv_usec=%ld\n", socktv.tv_sec, socktv.tv_usec);
#endif
	return 1;
}

#ifdef HAM
//by icecream 
//function detect wan_ip
static int detect()
{	
	FILE *fp=NULL;
	char cmd[80];
	char detect_ip[20]="18.52.86.150";
	char line[254];
	char get_ip[10][30];
	int j=0;
	while(j<=9)
	{
		memset(&get_ip[j], 0, sizeof(get_ip[j]));
		memset(&gateway_ip[j], 0, sizeof(gateway_ip[j]));
		j++;
	}
	remove(DETECT_FILE);
	int i=0;
	int ping=0;
	snprintf(cmd,sizeof(cmd),"traceroute -m 10 %s >%s",detect_ip,DETECT_FILE);
	system(cmd);
	if ((fp = fopen(DETECT_FILE, "r")) != NULL) 
		{
			while(i<10)
			{
				if( fgets(line, sizeof(line), fp) != NULL ) 
				{
					char *p=NULL;
					p=index(line,'(');
					if(!p)
					{
						ping++;
						if(ping>=3)
						{
							return 0;
						}
						continue;
					}
					int j=1;
					while((*p)!=')')
					{
						p++;
						get_ip[i][j-1]=*p;
						j++;
					}
					get_ip[i][j-2]='\0';
					
				}
				i++;

			}
			i=0;
			while(i<10)
			{
				sprintf(gateway_ip[i],"%s",get_ip[i]);
				i++;
			}	
		}
		fclose(fp);
		return 1;

	
}
#else
static int detect()
{
        FILE *fp=NULL;
        char cmd[80];
        char detect_ip[20]="18.52.86.150";
        char line[254];
        char get_ip[6][30];
        char *no_host="!H", *no_net="!N", *tr_str = "traceroute to";
        int j;

        printf("## detect ##\n");       // tmp test

        j = 0;
        while(j < 6)
        {
                memset(&get_ip[j], 0, sizeof(get_ip[j]));
                memset(&gateway_ip[j], 0, sizeof(gateway_ip[j]));
                memset(&get_sp[j], 0, sizeof(get_sp[j]));
                ++j;
        }
        snprintf(cmd,sizeof(cmd),"traceroute -m 6 %s >%s",detect_ip,DETECT_FILE);
        remove(DETECT_FILE);
        int i;
        int ping=0;
        char *p;
        char *ms_p, *line_p, *tmp_p, tmp_buf[10];
        double ms_num, ms_n, ms_avg, sp;

        system(cmd);

        i = 0;
        if ((fp = fopen(DETECT_FILE, "r")) != NULL)
        {
                while(i < 6)
                {
                        /* get ip */
                        if( fgets(line, sizeof(line), fp) != NULL )
                        {
                                if(strstr(line, no_host) || strstr(line, no_net))       // add
                                {
                                        printf("detect: no host or no network\n");      // tmp test
                                        fclose(fp);
                                        return -1;
                                }

                                if(strstr(line, tr_str))
                                        continue;

                                p=index(line,'(');
                                if(!p)
                                        continue;

                                j=1;
                                while((*p)!=')')
                                {
                                        ++p;
                                        get_ip[i][j-1]=*p;
                                        ++j;
                                }
                                get_ip[i][j-2]='\0';

                        }

                        /* get speed */
                        ms_num = 0.0;
                        ms_n = 0;
                        line_p = line;
                        while((ms_p = strstr(line_p, "ms")) != NULL)
                        {
                                ++ms_n;
                                tmp_p = ms_p-2;
                                line_p = ms_p + 1;
                                j = 0;
                                memset(tmp_buf, 0, sizeof(tmp_buf));

                                for(; *tmp_p!= ' '; --tmp_p, ++j)
                                {
                                        tmp_buf[9-j] = *tmp_p;
                                }
                                j=0;
                                while((tmp_buf[j]=='\0') && (j < 10))
                                {
                                        tmp_buf[j] = '0';
                                        ++j;
                                }

                                ms_num += atof(tmp_buf);
                                //printf("get ms from [%s]: %f, (%s)\n", get_ip[i], ms_num, tmp_buf);     // tmp test
                        }
                        if(ms_num > 0.0)
                        {
                                memset(tmp_buf, 0, sizeof(tmp_buf));
                                ms_avg = ms_num / ms_n;
                                sp = (((38.00/1024.00)/(ms_avg/1000000)))/2;
                                sprintf(get_sp[i], "%.2f", sp);
                                //printf("get ms avg : %f(%f), sp=%s\n", ms_avg, ms_num, get_sp[i]);      // tmp test
                        }
                        ++i;
                }

                i=0;
                while(i<6)
                {
                        sprintf(gateway_ip[i],"%s",get_ip[i]);
                        ++i;
                }
        }
        else
                return 0;

        fclose(fp);
        printf("end detect\n"); // tmp test
        return 1;
}
#endif

static int ping_interval()
{
	//char buf[80];
	for(;interval >0;interval-=50000){
		for(request_time=12;request_time<=18;request_time++){
			if(deltatime()<0){
				return 1;
			}
		}
	}
	//sprintf(buf,"%d",interval);
	//nvram_set("gary_first_interval",buf);
}

//RT-N16
#ifndef SIOCETHTOOL
#define SIOCETHTOOL 0x8946
#endif

#ifndef SIOCGIFADDR
#define SIOCGIFADDR 0x8915
#endif 

#ifndef SIOCGETCPHYRD
#define SIOCGETCPHYRD 0x89FE
#endif

int if_wan_phyconnected(){
	int fd, err, flag;
	int disconn_case;
	struct ifreq ifr;
	struct ethtool_cmd ecmd;
	
	memset(&ifr, 0, sizeof(ifr));
//	strcpy(ifr.ifr_name, "eth0");
	strcpy(ifr.ifr_name, nvram_get("wan_ifname"));
	
	fd = socket(AF_INET, SOCK_DGRAM, 0);
	if(fd < 0){
		perror("w socket");
		return 0;
	}
	
	ecmd.cmd = ETHTOOL_GSET;
	ifr.ifr_data = (caddr_t)&ecmd;
	err = ioctl(fd, SIOCETHTOOL, (void*)&ifr);
	
	if(err == 0){
		char tmpstr[60];
		int vecarg[2];
		int verbuf;
		
		vecarg[0] = strtoul("0x01", NULL, 0) << 16;;
		vecarg[0] |= strtoul("0x00", NULL, 0) & 0xffff;
		
		sprintf(tmpstr, "0x%04x\n", vecarg[1]);
		
		ifr.ifr_data = (caddr_t)vecarg;
		if(ioctl(fd, SIOCGETCPHYRD, (caddr_t)&ifr) < 0)
			printf("etcphyrd error\n");
		close(fd);
		
		verbuf = vecarg[1];  

		if((verbuf&0x0001) != 0) //RT-N16, WAN Port=0
			return 1;
		else
			return 0;
	}
	else{
		close(fd);
		perror("ioctl");
		return 0;
	}
}
/**************************************************************************
 * Function Name: qos_get_wan_rate
 * Description  : get the ADSL line rate
 * Parameters   : None
 * Returns      : Is successful when get the ADSL data rate? 
			0 : Success
			1 : Link fail
 **************************************************************************/
#ifdef HAM
extern int qos_get_wan_rate(void)
{
	static struct wan_link_rate wanrate;
	char us[256];	
	wanrate.us = wanrate.ds = 0;
	int step=3;
	char *ip = nvram_safe_get("wan_dns_t");
	int max_request_time;

	if(if_wan_phyconnected()==0){ 
		nvram_set("qos_ubw_status", "fail");
		nvram_set("qos_ubw_reason", "Cable is not attached");
		return WAN_LINK_FAIL;;
	}

	int choose=1;
	choose=detect();

	if(choose==0)
	{			
		return WAN_LINK_FAIL;
	}
	if(strlen(gateway_ip[step])!=0)
	{
		while(pre_ping(gateway_ip[step])==0)
		{
			if(step==9)
			{
				return WAN_LINK_FAIL;	
			}
			step++;
			while(strlen(gateway_ip[step])==0)
			{
				if(step==9)
				{
					return WAN_LINK_FAIL;	
				}
				step++;
			}
		}
	}
	else
	{
		step--;
		while(strlen(gateway_ip[step])==0)
		{
			if(step==0)
			{
				return WAN_LINK_FAIL;
			}
			step--;	
		}
		while(pre_ping(gateway_ip[step])==0)
		{
			if(step==0)
			{
				return WAN_LINK_FAIL;
			}
			step--;
		}
	}

	usleep(10);
	unsigned long int ul_hostip;
	const char * host = gateway_ip[step];
	//nvram_set("gary_gateway_ip",host);

	memset(&pingaddr, 0, sizeof(struct sockaddr_in));
	pingaddr.sin_family = AF_INET;

	if( (ul_hostip=inet_addr(host)) !=INADDR_NONE )
	{
		pingaddr.sin_addr.s_addr = ul_hostip;
	}
	else if((hostent=gethostbyname(host)) ) 
	{	
		memcpy(&pingaddr.sin_addr, hostent->h_addr, sizeof(pingaddr.sin_addr));
	}
#ifdef QOS_DEBUG
	else
	{
		printf("	deltatime() ** bad host ip or name!!\n");
		return ;
	}
#endif

	ping_interval(); 

	if(interval<INIT_INTERVAL){
		max_request_time=18;
	}
	else{
		max_request_time=12;
		interval=0;
	}
	char buf_max[20];
	sprintf(buf_max,"%d",max_request_time);
	nvram_set("max_request_time",buf_max);
	sleep(1);
	int loop_time=1; //2008.10 magic		

loop:request_time=1;
	//2008.10 magic{
     	if(loop_time++>10)
	     return WAN_LINK_FAIL;
	//2008.10 magic}

	int i=0;
	while(i<=18)
     	{
		triptime[i]=0;
     		i++;
 	}

     	//printf("gateway_ip[step]=%s\n",gateway_ip[step]);
     	while(request_time<=max_request_time)
     	{
     		if(deltatime()<0){
	     		if(max_request_time==12 && request_time<=6){
		     		interval+=50000;
		     		goto loop;
	     		}
	     		else if(max_request_time==18){
		     		interval+=20000;
		     		goto loop;
	     		}
	     		break;
     		}
     		usleep(10);
     		request_time++;
    	}

       	i=0;
     	while(triptime[i]!=0)
     	{
	   	i++;
     	}
     	i=i-(i%3);
   
     	if(i==6)
     	{
	   	triptime[0]=(triptime[0]+triptime[1]+triptime[2])/3;
	 	triptime[1]=(triptime[3]+triptime[4]+triptime[5])/3;
	 	wanrate.us = (double)(15-9)*(datalen+46)*8/(triptime[1]-triptime[0])/1024*1000000*1.1 ;	
     	}
     	else if(i==9) 
     	{
	   	triptime[0]=(triptime[3]+triptime[4]+triptime[5])/3;
	   	triptime[1]=(triptime[6]+triptime[7]+triptime[8])/3;
	   	wanrate.us = (double)(29-15)*(datalen+46)*8/(triptime[1]-triptime[0])/1024*1000000*1.1 ;	
     	}
     	else if(i==12)
     	{
	     	triptime[0]=(triptime[3]+triptime[4]+triptime[5])/3;
	     	triptime[1]=(triptime[6]+triptime[7]+triptime[8])/3;
	     	triptime[2]=(triptime[9]+triptime[10]+triptime[11])/3;
	    	wanrate.us=((double)(54-15)*(datalen+46)*8/(triptime[2]-triptime[0])/1024*1000000);

	    	wanrate.ds=((double)(54-29)*(datalen+46)*8/(triptime[2]-triptime[1])/1024*1000000);	     	
     	}
     	else if(i==18)
     	{	     
	     	//triptime[0]=(triptime[6]+triptime[7]+triptime[8])/3;
	     	triptime[0]=(triptime[9]+triptime[10]+triptime[11])/3;
	     	triptime[1]=(triptime[12]+triptime[13]+triptime[14])/3;
	     	triptime[2]=(triptime[15]+triptime[16]+triptime[17])/3;
	     	wanrate.ds=((double)(138-54)*(datalen+46)*8/(triptime[1]-triptime[0])/1024*1000000);

	     	wanrate.us=((double)(238-138)*(datalen+46)*8/(triptime[2]-triptime[1])/1024*1000000);
	     	wanrate.us=(wanrate.ds+wanrate.us)/2;	     	
	}
 
	if( wanrate.us!=0 ) 
		nvram_set("qos_ubw_status", "success");
	else
		nvram_set("qos_ubw_status", "fail");

	sprintf(us, "%u", wanrate.us);
	nvram_set("qos_ubw", us);

	return WAN_RATE_SUCCESS;

}
#else
int qos_get_wan_rate(void)
{
        static struct wan_link_rate wanrate;
        char us[256], ds[256] ;
        char * gw_ip=NULL;
        struct timeval deltaval;
        double delta_us = 0;
        wanrate.us = wanrate.ds = 0;
        int step=3;
        char *ip = nvram_safe_get("wan_dns_t");
        int max_request_time;
        double sp, sp_avg, sp_valid, tmp_add;
        char ubw_buf[10];
        int i;

        printf("\nQos get wan rate\n"); // tmp test

        if( !check_wan_link(0) )
        {
                printf("check wan link fail\n");        // tmp test
                return WAN_LINK_FAIL;
        }

        gw_ip = nvram_safe_get("lan_ipaddr");

        if((strchr(ip, ' ')) ||(!strcmp(ip, "")))
        {
                struct dns_lists *dns_list = get_dns_list(1);

                for(i=0 ; i<dns_list->num_servers ; i++){
                        ip = dns_list->dns_server[i];
                        if(    (!strchr(ip, ' '))
                                        && (strcmp(ip, ""))
                                        && (strcmp(ip, gw_ip))) //WL600g auto assign LAN IP as DNS Server 
                                break;
                }
                free(dns_list);
        }

        if( (strchr(ip, ' ')) || (!strcmp(ip, "")) || (!strcmp(ip, gw_ip)) ) {
                nvram_set("qos_ubw_status", "fail");
                nvram_set("qos_ubw_reason", "DNS server fail");
                nvram_set("qos_ubw", "0kbit");
                return WAN_LINK_FAIL;
        }
        else
        {
                if(detect() == 1)
                {
                        tmp_add = 0.0;
                        sp_valid = 0.0;
                        for(i=1; i<6; ++i)      /* discard first gateway */
                        {
                                if(strlen(gateway_ip[i]) > 0)
                                {
                                        /*printf("test [%s][%s]\n", gateway_ip[i], get_sp[i]);  // tmp test */
                                        if((sp=atof(get_sp[i])) > 0.0)
                                        {
                                                tmp_add += sp;
                                                ++sp_valid;
                                                /*printf("tmp_add is %f, valid is %f\n", tmp_add, sp_valid);    // tmp test */
                                        }
                                }
                        }
                        sp_avg = tmp_add/sp_valid;
                        memset(ubw_buf, 0, sizeof(ubw_buf));
                        sprintf(ubw_buf, "%.2f", sp_avg);

                        nvram_set("qos_ubw_status", "success");
                        nvram_set("qos_ubw", ubw_buf);
                        nvram_set("qos_ubw_tmp", ubw_buf);
                        nvram_set("qos_ubw_reason", "");

                        printf("Sp avg is %s (%f)(%f)\n", ubw_buf, sp_valid, tmp_add);  // tmp test

                        /*return WAN_RATE_SUCCESS; */
                        return 0;
                }
                else
                {
                        nvram_set("qos_ubw_status", "fail");
                        nvram_set("qos_ubw_reason", "detect fail");
                        nvram_set("qos_ubw", "0");
                        return WAN_LINK_FAIL;
                }
        }
}

#endif

#ifdef WL600g
//**************************************************************************
// Function Name: Speedtest_init
// Description  : init function, called by BcmNtwk_init() in ifcntwkapi.cpp
// Returns      : 0
//**************************************************************************
int Speedtest_init(void) {
        FILE *fp;

        /* Run it under background */
        switch (fork()) {
        case -1:
                exit(0);
                break;
        case 0:
                // start in a new session
                (void) setsid();
                break;
        default:
                // parent process should just die
                return 0;
        }

        /* write pid */
        if ((fp=fopen("/var/run/speedtest.pid", "w"))!=NULL)
        {
                fprintf(fp, "%d", getpid());
                fclose(fp);
        }
	
	nvram_set("qos_ubw_status", "initialing");

	sleep(60);
	if( nvram_match("qos_enable", "1") ) {

		qos_get_wan_rate();
		if(nvram_match("qos_ubw_status", "success") )
			start_qos();
	}

	while(1){
		pause();
	}

        return 0;
}
#else

#define isdigit(c) (c >= '0' && c <= '9') 
static ulong atoid(const char *s)
{
         int i=0,j;
	 ulong k=0;
	 for(j=1; j<5; j++) {
		 i=0;
	         while (isdigit(*s)) {
        	         i = i*10 + *(s++) - '0';
		}
		 k = k*256 +i;
		 if(j == 4)
			 return k;
		 s++;
	}
         return k;
}

int flag = 0;
ulong ipaddr = 0;
#if 0
static void sendtrack()
{
	track_flag(&flag, &ipaddr);
	signal(SIGALRM, sendtrack);
	alarm(60);
}
#endif

int Speedtest_Init(void) {
	char net_name[32];
	FILE *fp=NULL;
	char cmd[80];

	printf("*** run Speedtest_Init(). ***\n");
	
	nvram_set("qos_userspec_app", "0");
	nvram_set("qos_global_enable", "0");
	nvram_set("qos_userdef_enable", "0");
/*	moved to common_ex.c by Jiahao
	nvram_set("qos_wireless_enable", "0");
*/
	//nvram_set("qos_enable", "0"); //2008.10 magic

        if ( nvram_invmatch("qos_rulenum_x", "0"))
                nvram_set("qos_userspec_app", "1");
	
	printf("judge informatioin.\n");
	//add by Angela 2008.05
	if(nvram_match("qos_tos_prio", "1")||nvram_match("qos_pshack_prio", "1")
	  || nvram_match("qos_service_enable", "1")|| nvram_match("qos_shortpkt_prio", "1"))
		nvram_set("qos_global_enable", "1");
	
	if(nvram_match("qos_userspec_app", "1") || nvram_match("qos_dfragment_enable", "1"))
		nvram_set("qos_userdef_enable", "1");

/*	moved to common_ex.c by Jiahao
	//SZ-Angela Add for Multi-SSID
	if(nvram_match("wl0.1_priority", "0") || nvram_match("wl0.2_priority", "0") || nvram_match("wl0.3_priority", "0")) 
		nvram_set("qos_wireless_enable", "1");
*/

	/* Get interface name */
	if (nvram_match("wan0_proto", "pppoe") || nvram_match("wan0_proto","pptp"))
		strcpy (net_name, nvram_safe_get("wan0_pppoe_ifname"));
	else	
		strcpy (net_name, nvram_safe_get("wan0_ifname"));

	/* Reset all qdisc first */
	eval ("tc","qdisc","del", "dev", net_name, "root");
	eval ("tc","qdisc","del", "dev", "br0", "root"); //2008.12  SZ-magic br0-->eth0
	//snprintf(cmd,sizeof(cmd),"tc qdisc del dev %s root 2>/dev/null", net_name);
	//system(cmd);
	//snprintf(cmd,sizeof(cmd),"tc qdisc del dev br0 root 2>/dev/null");
	//system(cmd);

	/* Clean iptables*/
	/*eval("iptables", "-F", "-t", "mangle");*/
	if ((fp=fopen("/tmp/mangle_rules", "w"))==NULL) 
	{
		nvram_set("qos_file"," /tmp/mangle_rules file doesnot exist.");
		if((fp = fopen("/tmp/mangle_rules", "w+")) == NULL)
			return 1;
	}
	fprintf(fp, "*mangle\n");
	fprintf(fp, "-F\n");
	fprintf(fp, "COMMIT\n\n");
	fclose(fp);
	eval("iptables-restore", "/tmp/mangle_rules");

	if(nvram_invmatch("qos_manual_ubw","0") && nvram_invmatch("qos_manual_ubw",""))
	{
		nvram_set("qos_ubw",nvram_get("qos_manual_ubw"));
	}
	else 
		nvram_set("qos_ubw",nvram_get("qos_ubw_tmp"));

//2008.10 magic{
	if(nvram_match("qos_global_enable", "1") || 
		nvram_match("qos_userdef_enable", "1") || 
		nvram_match("qos_wireless_enable", "1"))	//SZ-Angela Add for Multi-SSID
	{
		nvram_set("qos_enable", "1");
		nvram_commit();
		if(nvram_invmatch("qos_ubw", "0")){
			flag = 1;
			ipaddr = atoid(nvram_get("wan0_ipaddr"));	//--sz angela		
			start_qos();		
		}
	}
	else
	{
		nvram_set("qos_enable", "0");
		nvram_commit();
		flag = 0;
	}
//2008.10 magic}
#if 1
	if(!syscall(__NR_track_flag, &flag, &ipaddr))
		nvram_set("qos_sys", "right set");
	else 
		nvram_set("qos_sys", "error set");
#endif
	return 0;                                                                  
}
#endif 

#ifdef MYGCC
int main(int argc, char** argv)
{
	int i = 0;
	char *thisarg;

	printf("	%-20s	%-20s\n", "index", "value");

	while( i<argc ) {
	    printf("	%-20d	%-20s\n", i, argv[i]);
	    i++;
	}

	argc--;
	argv++;
	while( argc>0 && **argv=='-' ) {
	    	thisarg = *argv;
		thisarg++;
		switch (*thisarg) {
		case 'c':
			if (--argc <= 0)
			        exit(1);
			argv++;
			pingcount = atoi(*argv);
			break;
		case 's':
			if (--argc <= 0)
			        exit(1);
			argv++;
			datalen = atoi(*argv);
			break;
		default:
			exit(1);
		}
		argc--;
		argv++;
	}

	if( argc<0 ) 	exit(1);
	
        struct timeval deltaval;

        //deltatime(*argv, &deltaval);
	
	printf("\n	start time ** tv_sec=%u : tv_usec=%u\n",tstart.tv_sec, tstart.tv_usec);
	printf("	end time   ** tv_sec=%u : tv_usec=%u\n", tend.tv_sec, tend.tv_usec);
        printf("        	      delta_sec=%u : delta_usec=%u\n", deltaval.tv_sec*1000000, deltaval.tv_usec);
        
	double delta_us = 0;
	delta_us = (int)deltaval.tv_sec * 1000000 + deltaval.tv_usec;
        printf("        delta interval = %f us\n", delta_us);
                                                                                                 
        double bw = ( (( (datalen+42)*pingcount*8 )/delta_us) )*1000000/1024;
        printf("\n	bandwidth = %.0f kbps\n", bw);
	
	return 0;
}
#endif 
