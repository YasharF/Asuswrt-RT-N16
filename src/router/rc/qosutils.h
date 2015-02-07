
struct dns_lists {
        int num_servers;
        char dns_server[4][16];
};

int check_wan_link(int num);
struct dns_lists *get_dns_list(int no);
char *get_gw_ip(void);
