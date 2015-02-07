#include "typedefs.h"
#include "wanduck.h"

#define csprintf(fmt, args...) do{\
	FILE *cp = fopen("/dev/console", "w");\
	if(cp) {\
		fprintf(cp, fmt, ## args);\
		fclose(cp);\
	}\
}while(0)

#define wan_prefix(unit, prefix) snprintf(prefix, sizeof(prefix), "wan%d_", unit)
#define RFC1123FMT "%a, %d %b %Y %H:%M:%S GMT"

#define CHK_PPP         1
#define CHK_DHCPD       2

#ifdef RTN12
int fer = 0;
int sw_mode = 0;
#endif

void track_set(int k_track){
	csprintf("track test:[%d]\n", k_track);       // tmp test
	if(!syscall(__NR_track_flag, &k_track, NULL))
		csprintf("track set ok\n");
	else
		csprintf("track set fail\n");
}

static void safe_leave(int signo){
csprintf("\n## wanduck.safeexit ##\n");
	FD_ZERO(&allset);
	close(http_sock);
	close(dns_sock);
	
	int i;
	for(i = 0; i <  maxfd; ++i)
		close(i);
	
	sleep(1);
	
	if(rule_setup == 1){
csprintf("\n# Disable direct rule(exit wanduck)\n");

#ifdef RTN12		
		if(sw_mode == 2 )
                	system("iptables-restore /tmp/fake_nat_rules");
                else
#endif
			_eval(del_command, NULL, 0, NULL);
		
		change_redirect_rules(3);
	}
	
// 2007.11 James {
	char *rm_pid[] = {"rm", "-f", "/var/run/wanduck.pid", NULL};
	
	_eval(rm_pid, NULL, 0, NULL);
// 2007.11 James }
	
csprintf("\n# return(exit wanduck)\n");
	exit(0);
}

int passivesock(char *service, char *protocol, int qlen){
	//struct servent *pse;
	struct protoent *ppe;
	struct sockaddr_in sin;
	int s, type;
	int protocol_num;
	
	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = INADDR_ANY;
	
	// map service name to port number
	if((sin.sin_port = htons((u_short)atoi(service))) == 0){
		perror("cannot get service entry");
		
		return -1;
	}
	
	// map protocol name to protocol number
	if((ppe = getprotobyname(protocol)) == (struct protoent*)0){
		protocol_num = 0;
		perror("cannot get proto entry");
		//return -1;
	}
	else
		protocol_num = ppe->p_proto;
	
	if(!strcmp(protocol, "udp"))
		type = SOCK_DGRAM;
	else
		type = SOCK_STREAM;
	
	s = socket(PF_INET, type, protocol_num);
	if(s < 0){
		perror("cannot create socket");
		return -1;
	}
	
	if(bind(s, (struct sockaddr *)&sin, sizeof(sin)) < 0){
		perror("cannot bind port");
		close(s);
		
		return -1;
	}
	
	if(type == SOCK_STREAM && listen(s, qlen) < 0){
		perror("cannot listen to port");
		close(s);
		
		return -1;
	}
	
	return s;
}

int chk_gatewayip(char *wanip){
	if(strcmp(wanip, wan_gateway_t) != 0)
		return CONNED;
	else if(strlen(wanip) == 0)
		return CONNED;
	else{
		disconn_case = CASE_MISROUTE;
		
		return DISCONN;
	}
}

int file_to_buf(char *path, char *buf, int len){
	FILE *fp;
	memset(buf, 0 , len);
	
	if((fp = fopen(path, "r")) != NULL){
		fgets(buf, len, fp);
		fclose(fp);
		
		return 1;
	}
	
	return 0;
}

int get_ppp_pid(char *conntype){
	int pid = -1;
	char tmp[80], tmp1[80];
	
	snprintf(tmp, sizeof(tmp), "/var/run/%s.pid", conntype);
	file_to_buf(tmp, tmp1, sizeof(tmp1));
	pid = atoi(tmp1);
	
	return pid;
}

/* Find process name by pid from /proc directory */
#if 0
char *find_name_by_proc(int pid){
	FILE *fp;
	char line[254];
	char filename[80];
	static char name[80];
	
	snprintf(filename, sizeof(filename), "/proc/%d/status", pid);
	
	if((fp = fopen(filename, "r")) != NULL){
		fgets(line, sizeof(line), fp);
        
		/* Buffer should contain a string like "Name:   binary_name" */
		sscanf(line, "%*s %s", name);
		fclose(fp);
		
		return name;
	}
	
	return "";
}
#endif

char *TASK_TYPE;

unsigned long task_mask;

int
check_task(char *cmd, int target)
{
        if(target == CHK_PPP)
        {
	      	if(strstr(cmd, "pppoecd")){
			//csprintf("pppoecd is alive!\n");
        	       	TASK_TYPE = "pppoecd";
	                return 0;
		}else if(strstr(cmd, "pppd")){
                	TASK_TYPE= "pppd";
	                return 0;
        	}else if(strstr(cmd, "bpalogin")){
                	TASK_TYPE= "bpalogin";
	                return 0;
        	}
	}
        else if(target == CHK_DHCPD)
        {
                if(strstr(cmd, "udhcpd")){
                        TASK_TYPE= "udhcpd";
                        return 0;
                }
        }
        else
        {
                TASK_TYPE = "";
	        return -1;
	}
}

char *find_name_by_proc(int target){

        DIR  *dir;
        struct dirent *dent;
        char task_file[50], cmdline[64];
        int pid, fd;

        if(!(dir=opendir("/proc")))
        {
                perror("open proc");
                return -1;
        }

        task_mask = 0;
        TASK_TYPE = "";
        while(dent = readdir(dir))
        {
                if((pid=atoi(dent->d_name)) > 1)
                {
                        memset(task_file, 0, sizeof(task_file));
                        sprintf(task_file, "/proc/%d/cmdline", pid);
                        if((fd=open(task_file, O_RDONLY)) > 0)
                        {
                                memset(cmdline, 0, sizeof(cmdline));
                                read(fd, cmdline, sizeof(cmdline));
                                check_task(cmdline, target);
                                close(fd);
                        } else
                                printf("cannot open %s\n", task_file);
                }
        }

       // printf("** task mask is %lu\n", task_mask); // tmp test

        closedir(dir);
        return TASK_TYPE;
}


int chk_proto(){
	char tmp[100];
	char filename[80], conntype[10];
	struct ifreq ifr;
	struct sockaddr_in *our_ip;
	struct in_addr in;
	int s;
	FILE *fp;
	char *pwanip = NULL;
	
	/* current unit */
	memset(tmp, 0, 100);
	if(wan_unit == 1)
		strcpy(tmp, wan1_proto);
	else
		strcpy(tmp, wan0_proto);
	
	if(!strcmp(tmp, "pptp")
			|| !strcmp(tmp, "PPTP")
			|| !strcmp(tmp, "l2tp")
			|| !strcmp(tmp, "pppoe")
			|| !strcmp(tmp, "PPPoE")
			|| !strcmp(tmp, "BigPong")
			|| !strcmp(tmp, "heartbeat")){

		char buf1[256];
		char *name;
		char* pos = NULL;
		char* cpos = NULL;
		
	/*	system("ls /tmp/ppp > /tmp/ppp/filelist");
		
		if((fp = fopen("/tmp/ppp/filelist", "r")) != NULL)
			while(fgets(buf1, 256, fp) != NULL){
				if((pos = strstr(buf1, "link")) != NULL){
					sprintf(filename, "/tmp/ppp/%s", buf1);
					cpos = strchr(filename, '\n');
					if(cpos)
						*cpos = '\0';
					
					strcpy(conntype, pos+5);
					cpos = strchr(conntype, '\n');
					if(cpos)
						*cpos = '\0';
					
					break;
				}
			}
		else{
			disconn_case = CASE_PPPFAIL;
			fclose(fp);
			
			return DISCONN;
		}
		
		fclose(fp);
		unlink("/tmp/ppp/filelist");
		
		if((fp = fopen(filename, "r")) != NULL){
			int pid = -1;
			fclose(fp);
			
			if(!strcmp(tmp, "heartbeat")){
				char buf[20];
				
				file_to_buf(filename, buf, sizeof(buf));
				pid = atoi(buf);
			}
			else
				pid = get_ppp_pid(conntype);
*/			
			name = find_name_by_proc(CHK_PPP);
//			name = find_name_by_proc(pid);
			if(!strncmp(name, "pppoecd", 7) ||		// for PPPoE
					!strncmp(name, "pppd", 4) ||	// for PPTP
					!strncmp(name, "bpalogin", 8)){	// for HeartBeat
				
				return CONNED;
			}
			else{
				// For some reason, the pppoed had been died, by link file still exist.
				unlink(filename);
				disconn_case = CASE_PPPFAIL;
				
				return DISCONN;
			}
/*		}
		else{
			disconn_case = CASE_PPPFAIL;
			
			return DISCONN;
		}
*/
	}
	else{
// 2008.03 James. {
		if(strlen(wan_gateway_t) != 0 && strlen(lan_ipaddr_t) != 0
				&& !strcmp(wan_gateway_t, lan_ipaddr_t)){
			disconn_case = CASE_THESAMESUBNET;
			
			return DISCONN;
		}
// 2008.03 James. }
		
		/* Open socket to kernel */
		if((s = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
			disconn_case = CASE_OTHERS;
			close(s);
			
			return DISCONN;
		}
		else{
			/* Check for valid IP address */
			memset(tmp, 0, 100);
			if(wan_unit == 1)
				strcpy(tmp, wan1_ifname);
			else
				strcpy(tmp, wan0_ifname);
			
			strncpy(ifr.ifr_name, tmp, IFNAMSIZ);
			if(!ioctl(s, SIOCGIFADDR, &ifr)){
				our_ip = (struct sockaddr_in *)&ifr.ifr_addr;
				in.s_addr = our_ip->sin_addr.s_addr;
				pwanip = inet_ntoa(in);
				close(s);
		
				if(!strcmp(pwanip, "") || pwanip == NULL){
					disconn_case = CASE_DISWAN;
					
					return DISCONN;
				}
				else
					return chk_gatewayip(pwanip);
			}
			else{
				disconn_case = CASE_OTHERS;
				close(s);
				
				return DISCONN;
			}
		}
	}
}

void pre_rSetting()
{
        char *name = find_name_by_proc(CHK_DHCPD);

        if(strlen(name)==0 || strcmp(name, "udhcpd") != 0)
        {
                printf("\nwanduck set r_Setting as 1 (udhcpd dead)\n"); // tmp test
                nvram_set("r_Setting", "1");
        }
}

#ifdef RTN12
#define NOT_SET         0
#define CAT_SET         1
#define HAD_SET         2

int first_enter_repeater()
{
        if(strcmp(nvram_safe_get("sw_mode"), "2")==0)
        {
/*
                 char *lan_addr = nvram_safe_get("lan_ipaddr");
                 char *lan_addr_t = nvram_safe_get("lan_ipaddr_t");
                 if(lan_addr_t && lan_addr)
                 {
                   	if((strlen(lan_addr_t)>0) && strcmp(lan_addr_t, lan_addr) != 0) // assume n13u has gotten ipaddr from wire
			{
                   		printf("\nwanduck set r_Setting as 1\n");       // tmp test
		                nvram_set("r_Setting", "1");
			}
		}
 */
                pre_rSetting(); // set r_Setting when udhcpd is dead
                char *r_setting = nvram_safe_get("r_Setting");

                sw_mode = 2;
                if(strcmp(r_setting, "0")==0)
                {
                        if(fer == NOT_SET)
                                fer = CAT_SET;
                        return 1;
                }
                else if(strcmp(r_setting, "1")==0)
                {
                        fer = HAD_SET;
                }
                else
                        return 0;
        }
        else
        {
                sw_mode = 1;
                if(fer != HAD_SET)
                        fer = NOT_SET;
                return 0;
        }
}
#endif

int if_wan_phyconnected(){
	int fd, err, flag;
	struct ifreq ifr;
	struct ethtool_cmd ecmd;

#ifdef RTN12
        if(fer < HAD_SET)
        {
                if(first_enter_repeater())
                {
                        //printf("First using Repeater\n");     // tmp test
                        disconn_case = CASE_FIRST_REPEATER;
                        return DISCONN;
                }
        }

	if(sw_mode == 2)
                return CONNED;
#endif
	
	memset(&ifr, 0, sizeof(ifr));
	strcpy(ifr.ifr_name, "eth0");
//	strcpy(ifr.ifr_name, nvram_get("wan_ifname"));
	
	fd = socket(AF_INET, SOCK_DGRAM, 0);
	if(fd < 0){
		perror("w socket");
		
		disconn_case = CASE_DISWAN;
		
		return DISCONN;
	}
	
	ecmd.cmd = ETHTOOL_GSET;
	ifr.ifr_data = (caddr_t)&ecmd;
	err = ioctl(fd, SIOCETHTOOL, (void*)&ifr);
	
	if(err == 0){
		char tmpstr[60];
		flag = 0;
		
#ifdef CDMA
		if(!strcmp(nvram_get_x("", "wan_proto"), "cdma"))
			flag = 1;
#endif
		
		int vecarg[2];
		int verbuf;
		
		vecarg[0] = strtoul("0x01", NULL, 0) << 16;;
		vecarg[0] |= strtoul("0x00", NULL, 0) & 0xffff;
		
		sprintf(tmpstr, "0x%04x\n", vecarg[1]);
		
		ifr.ifr_data = (caddr_t)vecarg;
		if(ioctl(fd, SIOCGETCPHYRD, (caddr_t)&ifr) < 0)
			csprintf("etcphyrd error\n");
		close(fd);
		
		verbuf = vecarg[1];  
#if 0
#ifdef WL500GPV2
		if((verbuf&0x0010) != 0)
#elif WL520GU
		if((verbuf&0x0001) != 0)
#else
		if((verbuf&0x0000) != 0)
#endif
#endif
#ifndef RTN12
		if((verbuf&0x0001) != 0) //RT-N16, WAN Port=0
#else
		if((verbuf&0x0010) != 0) //RT-N12/10, WAN Port=4
#endif
			flag = 1;
	

		if(!flag){
			disconn_case = CASE_DISWAN;
			
			return DISCONN;
		}
		else
			return chk_proto();
	}
	else{
		close(fd);
		perror("ioctl");
		
		disconn_case = CASE_DISWAN;
		
		return DISCONN;
	}
}

void change_redirect_rules(int num){
	int i;
	char *clean_ip_conntrack[] = {"cat", "/proc/net/nf_conntrack", NULL};
	
	
	//nvram_set("wan_state_changed", "1");
	track_set(101);
	
	// In experience, need to clean the ip_conntrack up in three times for a clean ip_conntrack.
	for(i = 0; i < num; ++i){
csprintf("**** clean ip_conntrack %d time. ****\n", i+1);
		_eval(clean_ip_conntrack, ">/dev/null", 0, NULL);
		
		if(i != num-1)
			sleep(1);
	}
	
	//nvram_set("wan_state_changed", "0");
	track_set(100);
}

void close_socket(int sockfd, char type){
	close(sockfd);
	FD_CLR(sockfd, &allset);
	client[fd_i].sfd = -1;
	client[fd_i].type = 0;
}

int main(int argc, char **argv){
	char *http_servport, *dns_servport;
	socklen_t clilen;
	struct sockaddr_in cliaddr;
	struct timeval  tval;
	int nready, maxi, sockfd, conn_state;
	/*int pid;
	
	// daemonize
	if((pid = fork()) != 0)
		exit(0);//*/
	
	umask(0);
	
	setsid();
	
	chdir("/");
	
	close(STDIN_FILENO);
	//close(STDOUT_FILENO);
	//close(STDERR_FILENO);
	
	struct stat fstatus;
	int fd;
	int max_tbl_sz = getdtablesize();
	for(fd = (STDERR_FILENO+1); fd <= max_tbl_sz; ++fd){
		if(fstat(fd, &fstatus) == 0){
			csprintf("The inherited fd(%d) is closed.\n", fd);
			close(fd);
		}
	}
	
	signal(SIGHUP, SIG_IGN);
	signal(SIGTERM, safe_leave);
	
	if(argc < 3){
		http_servport = DFL_HTTP_SERV_PORT;
		dns_servport = DFL_DNS_SERV_PORT;
	}
	else{
// 2007.10 James {
		if(atoi(argv[1]) <= 0)
			http_servport = DFL_HTTP_SERV_PORT;
		else
			http_servport = argv[1];
		
		if(atoi(argv[2]) <= 0)
			dns_servport = DFL_DNS_SERV_PORT;
		else
			dns_servport = argv[2];
// 2007.10 James }
	}
	
// 2007.10 James {
	if(build_socket(http_servport, dns_servport, &http_sock, &dns_sock) < 0){
		csprintf("\n*** Fail to build socket! ***\n");
		exit(0);
	}
	
	FILE *fp = fopen("/var/run/wanduck.pid", "w");
	
	if(fp != NULL){
		fprintf(fp, "%d", getpid());
		fclose(fp);
	}
// 2007.10 James }
	
	maxfd = (http_sock > dns_sock)?http_sock:dns_sock;
	maxi = -1;
	
	tval.tv_sec = 3;
	tval.tv_usec = 0;
	
	FD_ZERO(&allset);
	FD_SET(http_sock, &allset);
	FD_SET(dns_sock, &allset);
	
	for(fd_i = 0; fd_i < MAX_USER; ++fd_i){
		client[fd_i].sfd = -1;
		client[fd_i].type = 0;
	}
	
	rule_setup = 0;
	disconn_case = 0;
	clilen = sizeof(cliaddr);
	
	sleep(3);
	
// 2008.03 James. {
	if(nvram_match("x_Setting", "1"))
		isFirstUse = FALSE;
	else
		isFirstUse = TRUE;
	
	get_related_nvram();
	
	if(nvram_match("wan_ready", "1")){
		wan_ready = 1;
		nvram_unset("wan_ready");
		get_related_nvram2();
	}
// 2008.03 James. }
	
	err_state = if_wan_phyconnected();

	
	record_conn_status();	// 2008.02 James.
	
	if(err_state == DISCONN){
		if((nat_enable == 1)
#ifdef RTN12
		|| (fer < HAD_SET)
#endif
		){
csprintf("\n# Enable direct rule\n");
			rule_setup = 1;
			
			_eval(add_command, NULL, 0, NULL);
			
			change_redirect_rules(3);
		}
	}
	else if(err_state == CONNED && isFirstUse){
		if((nat_enable == 1)
#ifdef RTN12
		|| (fer < HAD_SET)
#endif
		){
csprintf("\n#CONNED : Enable direct rule\n");
			rule_setup = 1;
			
			_eval(add_command, NULL, 0, NULL);
			
			change_redirect_rules(3);
		}
	}
	
	for(;;){
		rset = allset;
		tval.tv_sec = 1;
		tval.tv_usec = 0;
		
// 2008.03 James. {
		if(nvram_match("x_Setting", "1"))
			isFirstUse = FALSE;
		else
			isFirstUse = TRUE;
		
		if(wan_ready == 0 && nvram_match("wan_ready", "1") && rule_setup == 1){
csprintf("\n# Rebuild rules\n");
			wan_ready = 1;
			nvram_unset("wan_ready");
			get_related_nvram2();
			
			rule_setup = 1;
			
			_eval(add_command, NULL, 0, NULL);
			
			change_redirect_rules(1);
		}
// 2008.03 James. }

		if((nat_enable == 1)
#ifdef RTN12
		|| (fer < HAD_SET)
#endif
		){
			conn_state = if_wan_phyconnected();
			
			if(conn_state == CONNED){
				if(err_state == DISCONN)
					err_state = D2C;
			}
			else if(conn_state == DISCONN){
				if(err_state == CONNED)
					err_state = C2D;
			}
			
			record_conn_status();	// 2008.02 James.
			
			if(err_state == C2D || (err_state == CONNED && isFirstUse)){
				err_state = DISCONN;
				
				if(rule_setup == 0){	// 2007.10 James
csprintf("\n# Enable direct rule(C2D)\n");
					rule_setup = 1;
					
					_eval(add_command, NULL, 0, NULL);
					
					change_redirect_rules(3);
				}
			}
			else if(err_state == D2C || err_state == CONNED){
				err_state = CONNED;
				
				if(rule_setup == 1 && !isFirstUse){
csprintf("\n#w Disable direct rule(D2C)\n");
					rule_setup = 0;
#ifdef RTN12
					if(sw_mode == 2 )
                                                system("iptables-restore /tmp/fake_nat_rules");
                                        else					
#endif
						_eval(del_command, NULL, 0, NULL);
					
					change_redirect_rules(3);
				}
			}
		}
		else{	// ap mode
			nat_enable = 0;
			
			if(rule_setup == 1){
csprintf("\n#AP Disable direct rule(D2C)\n");
				rule_setup = 0;
				
#ifdef RTN12
				if(sw_mode == 2 )
            	        	        system("iptables-restore /tmp/fake_nat_rules");
                                else					
#endif
					_eval(del_command, NULL, 0, NULL);
				
				change_redirect_rules(3);
			}
		}
		
		if((nready = select(maxfd+1, &rset, NULL, NULL, &tval)) <= 0)
			continue;
		
		if(FD_ISSET(dns_sock, &rset)){
			run_dns_serv(dns_sock);
			if(--nready <= 0)
				continue;
		}
		else if(FD_ISSET(http_sock, &rset)){
			if((connfd = accept(http_sock, (struct sockaddr *)&cliaddr, &clilen)) <= 0){
				perror("http accept");
				continue;
			}
			cur_sockfd = connfd;
			
			for(fd_i = 0; fd_i < MAX_USER; ++fd_i){
				if(client[fd_i].sfd < 0){
					client[fd_i].sfd = cur_sockfd;
					client[fd_i].type = T_HTTP;
					break;
				}
			}
			
			if(fd_i == MAX_USER){
csprintf("wanduck servs full\n");
				close(cur_sockfd);
				
				continue;
			}
			
			FD_SET(cur_sockfd, &allset);
			if(cur_sockfd > maxfd)
				maxfd = cur_sockfd;
			if(fd_i > maxi)
				maxi = fd_i;
			
			if(--nready <= 0)
				continue;	// no more readable descriptors
		}
		
		// polling
		for(fd_i = 0; fd_i <= maxi; ++fd_i){
			if((sockfd = client[fd_i].sfd) < 0)
				continue;
			
			if(FD_ISSET(sockfd, &rset)){
// 2007.10 James {
				int nread;
				ioctl(sockfd, FIONREAD, &nread);
				if(nread == 0){
					close_socket(sockfd, T_HTTP);
					continue;
				}
// 2007.10 James }
				
				cur_sockfd = sockfd;
				
				run_http_serv(sockfd);
				
				if(--nready <= 0)
					break;
			}
		}
	}
	
csprintf("wanduck exit error\n");
	exit(1);
}

void run_http_serv(int sockfd){
	ssize_t n;
	char line[MAXLINE];
	
	memset(line, 0, sizeof(line));
	
	if((n = read(sockfd, line, MAXLINE)) == 0){	// client close
		close_socket(sockfd, T_HTTP);
		
		return;
	}
	else if(n < 0){
		perror("readline");
		return;
	}
	else{
		if(client[fd_i].type == T_HTTP)
			handle_http_req(sockfd, line);
		else
			close_socket(sockfd, T_HTTP);
	}
}

void run_dns_serv(int sockfd){
	int n;
	char line[MAXLINE];
	struct sockaddr_in cliaddr;
	int clilen = sizeof(cliaddr);
	
	memset(line, 0, MAXLINE);
	memset(&cliaddr, 0, clilen);
	
	if((n = recvfrom(sockfd, line, MAXLINE, 0, (struct sockaddr *)&cliaddr, &clilen)) == 0)	// client close
		return;
	else if(n < 0){
		perror("readline");
		return;
	}
	else
		handle_dns_req(sockfd, line, n, (struct sockaddr *)&cliaddr, clilen);
}

void parse_dst_url(char *page_src){
	int i, j;
	char dest[STRLEN], host[64];
	char host_strtitle[7], *hp;
	
	j = 0;
	memset(dest, 0, sizeof(dest));
	memset(host, 0, sizeof(host));
	memset(host_strtitle, 0, sizeof(host_strtitle));
	
	for(i = 0; i < strlen(page_src); ++i){
		if(i >= STRLEN)
			break;
		
		if(page_src[i] == ' ' || page_src[i] == '?'){
			dest[j] = '\0';
			break;
		}
		
		dest[j++] = page_src[i];
	}
	
	host_strtitle[0] = '\n';
	host_strtitle[1] = 'H';
	host_strtitle[2] = 'o';
	host_strtitle[3] = 's';
	host_strtitle[4] = 't';
	host_strtitle[5] = ':';
	host_strtitle[6] = ' ';
	
	if((hp = strstr(page_src, host_strtitle)) != NULL){
		hp += 7;
		j = 0;
		for(i = 0; i < strlen(hp); ++i){
			if(i >= 64)
				break;
			
			if(hp[i] == '\r' || hp[i] == '\n'){
				host[j] = '\0';
				break;
			}
			
			host[j++] = hp[i];
		}
	}
	
	memset(dst_url, 0, sizeof(dst_url));
	sprintf(dst_url, "%s/%s", host, dest);
}

void parse_req_queries(char *content, char *lp, int len, int *reply_size){
	int i, rn;
	
	rn = *(reply_size);
	for(i = 0; i < len; ++i){
		content[rn+i] = lp[i];
		if(lp[i] == 0){
			++i;
			break;
		}
	}
	
	if(i >= len)
		return;
	
	content[rn+i] = lp[i];
	content[rn+i+1] = lp[i+1];
	content[rn+i+2] = lp[i+2];
	content[rn+i+3] = lp[i+3];
	i += 4;
	
	*reply_size += i;
}

void handle_http_req(int sfd, char *line){
	int len;
	
	if(!strncmp(line, "GET /", 5)){
		parse_dst_url(line+5);
		
		len = strlen(dst_url);
		if((dst_url[len-4] == '.') &&
				(dst_url[len-3] == 'i') &&
				(dst_url[len-2] == 'c') &&
				(dst_url[len-1] == 'o')){
			close_socket(sfd, T_HTTP);
			
			return;
		}
		send_page(sfd, NULL, dst_url);
	}
	else
		close_socket(sfd, T_HTTP);
}

void handle_dns_req(int sfd, char *line, int maxlen, struct sockaddr *pcliaddr, int clen){
	dns_query_packet d_req;
	dns_response_packet d_reply;
	int reply_size;
	char reply_content[MAXLINE];
	
	reply_size = 0;
	memset(reply_content, 0, MAXLINE);
	memset(&d_req, 0, sizeof(d_req));
	memcpy(&d_req.header, line, sizeof(d_req.header));
	
	// header
	memcpy(&d_reply.header, &d_req.header, sizeof(dns_header));
	d_reply.header.flag_set.flag_num = htons(0x8580);
	d_reply.header.answer_rrs = htons(0x0001);
	memcpy(reply_content, &d_reply.header, sizeof(d_reply.header));
	reply_size += sizeof(d_reply.header);
	
// 2009.02 James. Force to send answer response.{
	reply_content[5] = 1;	// Questions
	reply_content[7] = 1;	// Answer RRS
	reply_content[9] = 0;	// Authority RRS
	reply_content[11] = 0;	// Additional RRS
// 2009.02 James. }
	
	// queries
	parse_req_queries(reply_content, line+sizeof(dns_header), maxlen-sizeof(dns_header), &reply_size);
	
	// answers
	d_reply.answers.name = htons(0xc00c);
	d_reply.answers.type = htons(0x0001);
	d_reply.answers.ip_class = htons(0x0001);
	d_reply.answers.ttl = htonl(0x00000001);
	d_reply.answers.data_len = htons(0x0004);
	d_reply.answers.addr = htonl(0x0a000001);	// 10.0.0.1
	
	memcpy(reply_content+reply_size, &d_reply.answers, sizeof(d_reply.answers));
	reply_size += sizeof(d_reply.answers);
	
	sendto(sfd, reply_content, reply_size, 0, pcliaddr, clen);
}

void send_page(int sfd, char *file_dest, char *url){
	char buf[2*MAXLINE];
	time_t now;
	char timebuf[100];
	
	memset(buf, 0, sizeof(buf));
	now = time((time_t*)0);
	(void)strftime(timebuf, sizeof(timebuf), RFC1123FMT, gmtime(&now));
	
	sprintf(buf, "%s%s%s%s%s%s", buf, "HTTP/1.0 302 Moved Temporarily\r\n", "Server: wanduck\r\n", "Date: ", timebuf, "\r\n");

#ifdef RTN12	
	if(sw_mode == 2 && disconn_case == CASE_FIRST_REPEATER)
                sprintf(buf, "%s%s%s%s%s%s%s" ,buf , "Connection: close\r\n", "Location:http://", lan_ipaddr_t, "/survey.asp", "\r\nContent-Type: text/plain\r\n", "\r\n<html></html>\r\n");
	else 
#endif
	if(isFirstUse)	// 2008.01 James.
		sprintf(buf, "%s%s%s%s%s%s%s" ,buf , "Connection: close\r\n", "Location:http://", lan_ipaddr_t, "/QIS_wizard.htm?flag=detect", "\r\nContent-Type: text/plain\r\n", "\r\n<html></html>\r\n");
	else if(err_state == C2D || err_state == DISCONN)
		sprintf(buf, "%s%s%s%s%s%d%s%s" ,buf , "Connection: close\r\n", "Location:http://", lan_ipaddr_t, "/error_page.htm?flag=", disconn_case, "\r\nContent-Type: text/plain\r\n", "\r\n<html></html>\r\n");
	
	write(sfd, buf, strlen(buf));
	close_socket(sfd, T_HTTP);
}

// 2008.02 James. {
void record_conn_status(){
	if(err_state == DISCONN || err_state == C2D){
		if(disconn_case == CASE_DISWAN){
			if(Dr_Surf_case == 1)
				return;
			Dr_Surf_case = 1;
			
			logmessage("WAN Connection", "The cable for Ethernet was not plugged in.");
		}
		else if(disconn_case == CASE_PPPFAIL){
			if(Dr_Surf_case == 2)
				return;
			Dr_Surf_case = 2;
			
			FILE *fp = fopen("/tmp/wanstatus.log", "r");
			char log_info[64];
			
			if(fp == NULL){
				logmessage("WAN Connection", "WAN was exceptionally disconnected.");
				return;
			}
			
			memset(log_info, 0, 64);
			fgets(log_info, 64, fp);
			fclose(fp);
			
			if(strstr(log_info, "Failed to authenticate ourselves to peer") != NULL)
				logmessage("WAN Connection", "PPPoE or PPTP authentification failed.");
			else
				logmessage("WAN Connection", "No response from the remote server.");
		}
		else if(disconn_case == CASE_DHCPFAIL){
			if(Dr_Surf_case == 3)
				return;
			Dr_Surf_case = 3;
			
			if(!strcmp(wan0_proto, "dhcp"))
				logmessage("WAN Connection", "ISP's DHCP did not function properly.");
			else
				logmessage("WAN Connection", "Detected that the WAN Connection Type was PPPoE. But the PPPoE Setting was not complete.");
		}
		else if(disconn_case == CASE_MISROUTE){
			if(Dr_Surf_case == 4)
				return;
			Dr_Surf_case = 4;
			
			logmessage("WAN Connection", "The router's ip was the same as gateway's ip. It led to your packages couldn't dispatch to internet correctly.");
		}
		else if(disconn_case == CASE_THESAMESUBNET){
			if(Dr_Surf_case == 6)
				return;
			Dr_Surf_case = 6;
			
			logmessage("WAN Connection", "The LAN's subnet may be the same with the WAN's subnet.");
		}
		else{	// disconn_case == CASE_OTHERS
			if(Dr_Surf_case == 5)
				return;
			Dr_Surf_case = 5;
			
			logmessage("WAN Connection", "WAN was exceptionally disconnected.");
		}
	}
	else if(err_state == D2C){
		if(Dr_Surf_case == 10)
			return;
		Dr_Surf_case = 10;
		
		logmessage("WAN Connection", "WAN was restored.");
	}
}

void logmessage(char *logheader, char *fmt, ...){
	va_list args;
	char buf[512];
	
	va_start(args, fmt);
	
	vsnprintf(buf, sizeof(buf), fmt, args);
	openlog(logheader, 0, 0);
	syslog(0, buf);
	closelog();
	va_end(args);
}
// 2008.02 James. }

int readline(int fd,char *ptr,int maxlen){  // read a line(\n, \r\n) each time
	int n,rc;
	char c;
	*ptr = 0;
	
	for(n = 1; n < maxlen; ++n){
		if((rc = read(fd, &c, 1)) == 1){
			*ptr++ = c;
			
			if(c == '\n')
				break;
		}
		else if(rc == 0){
			if(n == 1)
				return(0);
			else
				break;
		}
		else
			return(-1);
	}
	
	return(n);
}

int build_socket(char *http_port, char *dns_port, int *hd, int *dd){
	if((*hd = passivesock(http_port, "tcp", 10)) < 0){
		csprintf("Fail to socket for httpd port: %s.\n", http_port);
		return -1;
	}
	
	if((*dd = passivesock(dns_port, "udp", 10)) < 0){
		csprintf("Fail to socket for DNS port: %s.\n", dns_port);
		return -1;
	}
	
	return 0;
}

void get_related_nvram(){
	memset(wan0_ifname, 0, 16);
	memset(wan1_ifname, 0, 16);
	memset(wan0_proto, 0, 16);
	memset(wan1_proto, 0, 16);
	memset(lan_ipaddr_t, 0, 16);
	memset(lan_netmask_t, 0, 16);
	
	if(nvram_match("wan_nat_x", "1"))
		nat_enable = 1;
	else
		nat_enable = 0;
	
	wan_unit = atoi(nvram_safe_get("wan_unit"));
	strcpy(wan0_ifname, nvram_safe_get("wan0_ifname"));
	strcpy(wan1_ifname, nvram_safe_get("wan1_ifname"));
	strcpy(wan0_proto, nvram_safe_get("wan0_proto"));
	strcpy(wan1_proto, nvram_safe_get("wan1_proto"));
	strcpy(lan_ipaddr_t, nvram_safe_get("lan_ipaddr_t"));
	strcpy(lan_netmask_t, nvram_safe_get("lan_netmask_t"));
}

void get_related_nvram2(){
	memset(wan_gateway_t, 0, 16);
	strcpy(wan_gateway_t, nvram_safe_get("wan_gateway_t"));
}
