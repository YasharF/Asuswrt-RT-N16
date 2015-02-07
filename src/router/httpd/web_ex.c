/*
 * Broadcom Home Gateway Reference Design
 * Web Page Configuration Support Routines
 *
 * Copyright 2001, Broadcom Corporation
 * All Rights Reserved.
 *
 * This is UNPUBLISHED PROPRIETARY SOURCE CODE of Broadcom Corporation;
 * the contents of this file may not be disclosed to third parties, copied or
 * duplicated in any form, in whole or in part, without the prior written
 * permission of Broadcom Corporation.
 *
 * $Id: web_ex.c,v 1.30 2008/11/12 10:00:43 james26_jang Exp $
 */

#ifdef WEBS
#include <webs.h>
#include <uemf.h>
#include <ej.h>
#else /* !WEBS */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <unistd.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <httpd.h>

extern int conn_fd; // 2009.05 James.
#endif /* WEBS */

#include <wlioctl.h>
#include <fcntl.h>
#include <signal.h>
#include <time.h>
#include <sys/klog.h>
#include <stdarg.h>
#include <sys/wait.h>
#include <dirent.h>
#include <wlutils.h>
#ifdef WRITE2FLASH
#include <linux/mtd/mtd.h>
#include <trxhdr.h>
#endif
#include "typedefs.h"
#include "shutils.h"
#include "bcmutils.h"
#include "bcmnvram_f.h"
#include "common.h"

// 2007.10 James {
#include <bcmnvram.h>
#include "dp.h"	//for discover_all()

#ifdef DLM
#include "initial_web_hook.h"
#include <disk_io_tools.h>
#include <disk_initial.h>
#include <disk_share.h>
//#include <disk_swap.h>

extern disk_info_t *DISKS_INFO; // 2009.05 James.
#endif


#define NVRAM_BUFSIZE 100 //11N
#include <assert.h>

#define wan_prefix(unit, prefix)	snprintf(prefix, sizeof(prefix), "wan%d_", unit)

extern unsigned int login_ip;
extern time_t login_timestamp;
extern unsigned int login_ip_tmp;
extern int firsttime; //2009.06 James


#define FIXED_PPP_DETECT
// 2007.10 James }

#include <syslog.h>
#define logs(s) syslog(LOG_NOTICE, s)

#define sys_restart() kill(1, SIGHUP)
#define sys_reboot() nvram_set("reboot", "1")

#ifdef WEBS
#define init_cgi(query)
#define do_file(webs, file)
#endif

#ifdef LINUX26
#define sys_upgrade(image) eval("write", image, "linux")
#define sys_default() eval("erase", "/dev/mtd3")
#else
#define sys_upgrade(image) eval("write", image, "/dev/mtd/1")
#define sys_default() eval("erase", "/dev/mtd/3")
#endif

#define sys_upload(image) eval("nvram", "restore", image)
#define sys_download(file) eval("nvram", "save", file)
#define sys_stats(url) eval("stats", (url))
#define sys_restore(sid) eval("nvram_x", "get", (sid))
#define sys_commit(sid) nvram_commit()

#define UPNP_E_SUCCESS 0
#define UPNP_E_INVALID_ARGUMENT -1

#define GROUP_FLAG_REFRESH 	0
#define GROUP_FLAG_DELETE 	1
#define GROUP_FLAG_ADD 		2
#define GROUP_FLAG_REMOVE 	3

#define IMAGE_HEADER 		"HDR0"
#define PROFILE_HEADER 		"HDR1"
#define PROFILE_HEADER_NEW 	"HDR2"

/*#ifdef ASUS_DDNS//2007.03.21 Yau add for asus ddns
#define nvram_match(name, match) ({ \
	const char *value = nvram_get(name); \
	(value && !strcmp(value, match)); \
})
#define nvram_safe_get(name) (nvram_get(name) ? : "")
#endif//*/

static int apply_cgi_group(webs_t wp, int sid, struct variable *var, char *groupName, int flag);
static int nvram_generate_table(webs_t wp, char *serviceId, char *groupName);

#ifndef NOUSB
static int apply_right_group(webs_t wp, int sid, struct variable *var, char *groupName, int flag);
void replace (char *Input);
#endif

static int ej_select_country(int eid, webs_t wp, int argc, char_t **argv);
#ifdef DLM
void umount_disc_parts(int usb_port);
#endif

static char * make_wl_prefix(char *prefix,int prefix_size, int mode, char *ifname);
static int wl_phytype_get(webs_t wp, int *phytype);
static int modify_right_group(webs_t wp, int sid, struct variable *var, char *groupName);


#ifndef DLM

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;

extern char *read_whole_file(const char *target){
	FILE *fp = fopen(target, "r");
	char *buffer, *new_str;
	int i;
	unsigned int read_bytes = 0;
	unsigned int each_size = 1024;
	
	if(fp == NULL)
		return NULL;
	
	buffer = (char *)malloc(sizeof(char)*each_size+read_bytes);
	if(buffer == NULL){
		printf("No memory \"buffer\".\n");
		fclose(fp);
		return NULL;
	}
	memset(buffer, 0, sizeof(char)*each_size+read_bytes);
	
	while((i = fread(buffer+read_bytes, sizeof(char), each_size, fp)) == each_size){
		read_bytes += each_size;
		new_str = (char *)malloc(sizeof(char)*each_size+read_bytes);
		if(new_str == NULL){
			printf("No memory \"new_str\".\n");
			free(buffer);
			fclose(fp);
			return NULL;
		}
		memset(new_str, 0, sizeof(char)*each_size+read_bytes);
		memcpy(new_str, buffer, read_bytes);
		
		free(buffer);
		buffer = new_str;
	}
	
	fclose(fp);
	return buffer;
}
#endif

static u64 restart_needed_bits = 0;
static int restart_tatal_time = 0;

static u32 last_arp_info_len = 0;
#ifdef DLM
static u32 last_disk_info_len = 0;
#endif
// 2007.10 James }*/

//char ibuf2[8192];
//char ibuf2=malloc

static int ezc_error = 0;

#define ACTION_UPGRADE_OK   0
#define ACTION_UPGRADE_FAIL 1

int action;

char *serviceId;

int correct_image;
int correct_setting;

#define MAX_GROUP_ITEM 10
#define MAX_GROUP_COUNT 300
#define MAX_LINE_SIZE 512
char *groupItem[MAX_GROUP_ITEM];
char urlcache[128];
//char *next_host;
int delMap[MAX_GROUP_COUNT];
char SystemCmd[64];
char UserID[32]="";
char UserPass[32]="";
char ProductID[32]="";
char CurrentDirPath[16]="/tmp/harddisk"; //2005.12.15 Yau add
char NewDirPath[128]="";
char OpenDirPath[128]="";
extern int redirect;
extern int change_passwd;	// 2008.03 James.
extern int reget_passwd;	// 2008.03 James.

/*
 * Country names and abbreviations from ISO 3166
 */
typedef struct {
	char *name;     /* Long name */
	char *abbrev;   /* Abbreviation */
} country_name_t;
country_name_t country_names[];     /* At end of this file */

/*
 * Country names and abbreviations from ISO 3166
 */
country_name_t country_names[] = {

{"AFGHANISTAN",		 "AF"},
{"ALBANIA",		 "AL"},
{"ALGERIA",		 "DZ"},
{"AMERICAN SAMOA", 	 "AS"},
{"ANDORRA",		 "AD"},
{"ANGOLA",		 "AO"},
{"ANGUILLA",		 "AI"},
{"ANTARCTICA",		 "AQ"},
{"ANTIGUA AND BARBUDA",	 "AG"},
{"ARGENTINA",		 "AR"},
{"ARMENIA",		 "AM"},
{"ARUBA",		 "AW"},
{"AUSTRALIA",		 "AU"},
{"AUSTRIA",		 "AT"},
{"AZERBAIJAN",		 "AZ"},
{"BAHAMAS",		 "BS"},
{"BAHRAIN",		 "BH"},
{"BANGLADESH",		 "BD"},
{"BARBADOS",		 "BB"},
{"BELARUS",		 "BY"},
{"BELGIUM",		 "BE"},
{"BELIZE",		 "BZ"},
{"BENIN",		 "BJ"},
{"BERMUDA",		 "BM"},
{"BHUTAN",		 "BT"},
{"BOLIVIA",		 "BO"},
{"BOSNIA AND HERZEGOVINA","BA"},
{"BOTSWANA",		 "BW"},
{"BOUVET ISLAND",	 "BV"},
{"BRAZIL",		 "BR"},
{"BRITISH INDIAN OCEAN TERRITORY", 	"IO"},
{"BRUNEI DARUSSALAM",	 "BN"},
{"BULGARIA",		 "BG"},
{"BURKINA FASO",	 "BF"},
{"BURUNDI",		 "BI"},
{"CAMBODIA",		 "KH"},
{"CAMEROON",		 "CM"},
{"CANADA",		 "CA"},
{"CAPE VERDE",		 "CV"},
{"CAYMAN ISLANDS",	 "KY"},
{"CENTRAL AFRICAN REPUBLIC","CF"},
{"CHAD",		 "TD"},
{"CHILE",		 "CL"},
{"CHINA",		 "CN"},
{"CHRISTMAS ISLAND",	 "CX"},
{"COCOS (KEELING) ISLANDS","CC"},
{"COLOMBIA",		 "CO"},
{"COMOROS",		 "KM"},
{"CONGO",		 "CG"},
{"CONGO, THE DEMOCRATIC REPUBLIC OF THE", "CD"},
{"COOK ISLANDS",	 "CK"},
{"COSTA RICA",		 "CR"},
{"COTE D'IVOIRE",	 "CI"},
{"CROATIA",		 "HR"},
{"CUBA",		 "CU"},
{"CYPRUS",		 "CY"},
{"CZECH REPUBLIC",	 "CZ"},
{"DENMARK",		 "DK"},
{"DJIBOUTI",		 "DJ"},
{"DOMINICA",		 "DM"},
{"DOMINICAN REPUBLIC", 	 "DO"},
{"ECUADOR",		 "EC"},
{"EGYPT",		 "EG"},
{"EL SALVADOR",		 "SV"},
{"EQUATORIAL GUINEA",	 "GQ"},
{"ERITREA",		 "ER"},
{"ESTONIA",		 "EE"},
{"ETHIOPIA",		 "ET"},
{"FALKLAND ISLANDS (MALVINAS)",	"FK"},
{"FAROE ISLANDS",	 "FO"},
{"FIJI",		 "FJ"},
{"FINLAND",		 "FI"},
{"FRANCE",		 "FR"},
{"FRENCH GUIANA",	 "GF"},
{"FRENCH POLYNESIA",	 "PF"},
{"FRENCH SOUTHERN TERRITORIES",	 "TF"},
{"GABON",		 "GA"},
{"GAMBIA",		 "GM"},
{"GEORGIA",		 "GE"},
{"GERMANY",		 "DE"},
{"GHANA",		 "GH"},
{"GIBRALTAR",		 "GI"},
{"GREECE",		 "GR"},
{"GREENLAND",		 "GL"},
{"GRENADA",		 "GD"},
{"GUADELOUPE",		 "GP"},
{"GUAM",		 "GU"},
{"GUATEMALA",		 "GT"},
{"GUINEA",		 "GN"},
{"GUINEA-BISSAU",	 "GW"},
{"GUYANA",		 "GY"},
{"HAITI",		 "HT"},
{"HEARD ISLAND AND MCDONALD ISLANDS",	"HM"},
{"HOLY SEE (VATICAN CITY STATE)", 	"VA"},
{"HONDURAS",		 "HN"},
{"HONG KONG",		 "HK"},
{"HUNGARY",		 "HU"},
{"ICELAND",		 "IS"},
{"INDIA",		 "IN"},
{"INDONESIA",		 "ID"},
{"IRAN, ISLAMIC REPUBLIC OF",		"IR"},
{"IRAQ",		 "IQ"},
{"IRELAND",		 "IE"},
{"ISRAEL",		 "IL"},
{"ITALY",		 "IT"},
{"JAMAICA",		 "JM"},
{"JAPAN",		 "JP"},
{"JORDAN",		 "JO"},
{"KAZAKHSTAN",		 "KZ"},
{"KENYA",		 "KE"},
{"KIRIBATI",		 "KI"},
{"KOREA, DEMOCRATIC PEOPLE'S REPUBLIC OF", "KP"},
{"KOREA, REPUBLIC OF",	 "KR"},
{"KUWAIT",		 "KW"},
{"KYRGYZSTAN",		 "KG"},
{"LAO PEOPLE'S DEMOCRATIC REPUBLIC", 	"LA"},
{"LATVIA",		 "LV"},
{"LEBANON",		 "LB"},
{"LESOTHO",		 "LS"},
{"LIBERIA",		 "LR"},
{"LIBYAN ARAB JAMAHIRIYA","LY"},
{"LIECHTENSTEIN",	 "LI"},
{"LITHUANIA",		 "LT"},
{"LUXEMBOURG",		 "LU"},
{"MACAO",		 "MO"},
{"MACEDONIA",	 "MK"},
{"MADAGASCAR",		 "MG"},
{"MALAWI",		 "MW"},
{"MALAYSIA",		 "MY"},
{"MALDIVES",		 "MV"},
{"MALI",		 "ML"},
{"MALTA",		 "MT"},
{"MARSHALL ISLANDS",	 "MH"},
{"MARTINIQUE",		 "MQ"},
{"MAURITANIA",		 "MR"},
{"MAURITIUS",		 "MU"},
{"MAYOTTE",		 "YT"},
{"MEXICO",		 "MX"},
{"MICRONESIA", 		 "FM"},
{"MOLDOVA, REPUBLIC OF", "MD"},
{"MONACO",		 "MC"},
{"MONGOLIA",		 "MN"},
{"MONTSERRAT",		 "MS"},
{"MOROCCO",		 "MA"},
{"MOZAMBIQUE",		 "MZ"},
{"MYANMAR",		 "MM"},
{"NAMIBIA",		 "NA"},
{"NAURU",		 "NR"},
{"NEPAL",		 "NP"},
{"NETHERLANDS",		 "NL"},
{"NETHERLANDS ANTILLES", "AN"},
{"NEW CALEDONIA",	 "NC"},
{"NEW ZEALAND",		 "NZ"},
{"NICARAGUA",		 "NI"},
{"NIGER",		 "NE"},
{"NIGERIA",		 "NG"},
{"NIUE",		 "NU"},
{"NORFOLK ISLAND",	 "NF"},
{"NORTHERN MARIANA ISLANDS","MP"},
{"NORWAY",		 "NO"},
{"OMAN",		 "OM"},
{"PAKISTAN",		 "PK"},
{"PALAU",		 "PW"},
{"PALESTINIAN TERRITORY, OCCUPIED", 	"PS"},
{"PANAMA",		 "PA"},
{"PAPUA NEW GUINEA",	 "PG"},
{"PARAGUAY",		 "PY"},
{"PERU",		 "PE"},
{"PHILIPPINES",		 "PH"},
{"PITCAIRN",		 "PN"},
{"POLAND",		 "PL"},
{"PORTUGAL",		 "PT"},
{"PUERTO RICO",		 "PR"},
{"QATAR",		 "QA"},
{"REUNION",		 "RE"},
{"ROMANIA",		 "RO"},
{"RUSSIAN FEDERATION",	 "RU"},
{"RWANDA",		 "RW"},
{"SAINT HELENA",	 "SH"},
{"SAINT KITTS AND NEVIS","KN"},
{"SAINT LUCIA",		 "LC"},
{"SAINT PIERRE AND MIQUELON",	 	"PM"},
{"SAINT VINCENT AND THE GRENADINES", 	"VC"},
{"SAMOA",		 "WS"},
{"SAN MARINO",		 "SM"},
{"SAO TOME AND PRINCIPE","ST"},
{"SAUDI ARABIA",	 "SA"},
{"SENEGAL",		 "SN"},
{"SEYCHELLES",		 "SC"},
{"SIERRA LEONE",	 "SL"},
{"SINGAPORE",		 "SG"},
{"SLOVAKIA",		 "SK"},
{"SLOVENIA",		 "SI"},
{"SOLOMON ISLANDS",	 "SB"},
{"SOMALIA",		 "SO"},
{"SOUTH AFRICA",	 "ZA"},
{"SOUTH GEORGIA", "GS"},
{"SPAIN",		 "ES"},
{"SRI LANKA",		 "LK"},
{"SUDAN",		 "SD"},
{"SURINAME",		 "SR"},
{"SVALBARD AND JAN MAYEN","SJ"},
{"SWAZILAND",		 "SZ"},
{"SWEDEN",		 "SE"},
{"SWITZERLAND",		 "CH"},
{"SYRIAN ARAB REPUBLIC", "SY"},
{"TAIWAN, PROVINCE OF CHINA", 		"TW"},
{"TAJIKISTAN",		 "TJ"},
{"TANZANIA",		"TZ"},
{"THAILAND",		 "TH"},
{"TIMOR-LESTE",		 "TL"},
{"TOGO",		 "TG"},
{"TOKELAU",		 "TK"},
{"TONGA",		 "TO"},
{"TRINIDAD AND TOBAGO",	 "TT"},
{"TUNISIA",		 "TN"},
{"TURKEY",		 "TR"},
{"TURKMENISTAN",	 "TM"},
{"TURKS AND CAICOS ISLANDS",		"TC"},
{"TUVALU",		 "TV"},
{"UGANDA",		 "UG"},
{"UKRAINE",		 "UA"},
{"UNITED ARAB EMIRATES", "AE"},
{"UNITED KINGDOM",	 "GB"},
{"UNITED STATES",	 "US"},
{"UNITED STATES MINOR OUTLYING ISLANDS","UM"},
{"URUGUAY",		 "UY"},
{"UZBEKISTAN",		 "UZ"},
{"VANUATU",		 "VU"},
{"VENEZUELA",		 "VE"},
{"VIET NAM",		 "VN"},
{"VIRGIN ISLANDS, BRITISH", "VG"},
{"VIRGIN ISLANDS, U.S.", "VI"},
{"WALLIS AND FUTUNA",	 "WF"},
{"WESTERN SAHARA", 	 "EH"},
{"YEMEN",		 "YE"},
{"YUGOSLAVIA",		 "YU"},
{"ZAMBIA",		 "ZM"},
{"ZIMBABWE",		 "ZW"},
{"ALL",		 	 "ALL"},
{NULL, 			 NULL}
};

#ifdef WRITE2FLASH
char *flashblock=NULL;
int mtd_fd = -1;
int mtd_count;
mtd_info_t mtd_info;
erase_info_t erase_info;

int flog(char *log)
{
	FILE *fp;

	fp=fopen("/tmp/log", "a+");

	if (fp)
	{	
		fprintf(fp, "log : %d %d %s", erase_info.start, erase_info.length, log);
		fclose(fp);
	}	
}

int flashwrite(char *buf, int count, int filelen)
{
	int len;
	int offset;
	int size;

	// first time, the trx header
	if (mtd_fd==-1)
	{

		/* Open MTD device and get sector size */
#ifdef LINUX26
		if ((mtd_fd = open("/dev/mtd1", O_RDWR)) < 0 ||
#else
		if ((mtd_fd = open("/dev/mtd/1", O_RDWR)) < 0 ||
#endif
		    ioctl(mtd_fd, MEMGETINFO, &mtd_info) != 0 ||
		    mtd_info.erasesize < sizeof(struct trx_header)) {
			flog("1\r\n");
			return 0;
		}

		erase_info.start=0;
		erase_info.length=mtd_info.erasesize;
		mtd_count = 0;

		/* Allocate temporary buffer */
		if (!(flashblock = malloc(erase_info.length))) {
			flog("2\r\n");
			goto fail;
		}
	}
	
	offset=0;
	do {
		if ((mtd_count+(count-offset))>=erase_info.length)
		{
			memcpy(flashblock+mtd_count, buf+offset, erase_info.length-mtd_count);
			offset+=(erase_info.length-mtd_count);
			mtd_count=erase_info.length;
		}
		else 
		{
			memcpy(flashblock+mtd_count, buf+offset, count-offset);
			mtd_count+=(count-offset);
			if (filelen!=0) return 1;
		}

		(void) ioctl(mtd_fd, MEMUNLOCK, &erase_info);
		if (ioctl(mtd_fd, MEMERASE, &erase_info) != 0 ||
	    		write(mtd_fd, flashblock, mtd_count) != mtd_count) {
			flog("3\r\n");
			goto fail;
		}
		erase_info.start+=mtd_count;
		mtd_count=0;
		flog("4\r\n");
		if (filelen==0&&(count-offset)==0) break;
	} while(1);

fail:
	/* if end of file */
	if (flashblock) {
		/* Dummy read to ensure chip(s) are out of lock/suspend state */
		(void) read(mtd_fd, flashblock, 2);
		free(flashblock);
	}

	if (mtd_fd >= 0)
		close(mtd_fd);
	return 1;
}
#endif

char *
rfctime(const time_t *timep)
{
	static char s[201];
	struct tm tm;

	setenv("TZ", nvram_safe_get_x("", "time_zone_x"), 1);
	memcpy(&tm, localtime(timep), sizeof(struct tm));
	strftime(s, 200, "%a, %d %b %Y %H:%M:%S %z", &tm);
	return s;
}

char *
reltime(unsigned int seconds)
{
	static char s[] = "XXXXX days, XX hours, XX minutes, XX seconds";
	char *c = s;

#ifdef SHOWALL
	if (seconds > 60*60*24) {
		c += sprintf(c, "%d days, ", seconds / (60*60*24));
		seconds %= 60*60*24;
	}
	if (seconds > 60*60) {
		c += sprintf(c, "%d hours, ", seconds / (60*60));
		seconds %= 60*60;
	}
	if (seconds > 60) {
		c += sprintf(c, "%d minutes, ", seconds / 60);
		seconds %= 60;
	}
	c += sprintf(c, "%d seconds", seconds);
#else
	c += sprintf(c, "%d secs", seconds);
#endif

	return s;
}

#ifndef WEBS
/******************************************************************************/
char *getip(FILE *fp)
{
	//printf("getip!!!"); 
	//getpeername(fd, &addr, &addrlen);
	/*if(next_host == NULL || !strcmp(next_host, ""))
	{
		//return(nvram_get_x("BRIPAddress", "lan_ipaddr"));
		return(nvram_safe_get("lan_ipaddr_t"));
	}
	else
	{
		return(next_host);
	}//*/
	if (nvram_match("lan_ipaddr_t", ""))
		return nvram_safe_get("lan_ipaddr");
	else
		return nvram_safe_get("lan_ipaddr_t");
}

/*
 *	Redirect the user to another webs page
 */
void websRedirect(webs_t wp, char_t *url)
{
	printf("Redirect to: url=http://%s/%s\n", getip((FILE *)wp), url);
	websWrite(wp, T("<html><head>\r\n"));
	websWrite(wp, T("<meta http-equiv=\"refresh\" content=\"0; url=http://%s/%s\">\r\n"), getip((FILE *)wp), url);
	websWrite(wp, T("<meta http-equiv=\"Content-Type\" content=\"text/html\">\r\n"));
	websWrite(wp, T("</head></html>\r\n"));
	
#ifdef REMOVE_WL600
	websWrite(wp, T("HTTP/1.0 302 Redirect\r\n"));
	websWrite(wp, T("Server: Asus Server\r\n"));
	websWrite(wp, T("Pragma: no-cache\r\n"));
	websWrite(wp, T("Cache-Control: no-cache\r\n"));
	websWrite(wp, T("Content-Type: text//html\r\n"));
	websWrite(wp, T("Location: http://%s/%s"), /*websGetVar(wp, T("HTTP_HOST"), "")*/"192.168.123.249", url);
	
	websWrite(wp, T("<html><head></head><body>\r\n\
		This document has moved to a new <a href=\"http://%s/%s\">location</a>.\r\n\
		Please update your documents to reflect the new location.\r\n\
		</body></html>\r\n"), /*websGetVar(wp, T("HTTP_HOST"), "")*/"192.168.123.249", url);
	websWrite(wp, T("HTTP/1.0 200 OK\r\n"));
#endif
	websDone(wp, 200);
}
#endif

void sys_script(char *name)
{
	char scmd[64];
	
	sprintf(scmd, "/tmp/%s", name);
	//printf("run %s %d %s\n", name, strlen(name), scmd);
	
	//handle special scirpt first
	
	if(!strcmp(name,"syscmd.sh")){
		if(strcmp(SystemCmd, "")){
			sprintf(SystemCmd, "%s > /tmp/syscmd.log\n", SystemCmd);
			system(SystemCmd);
		}	
		else
			system("echo None > /tmp/syscmd.log\n");
	}
	else if(!strcmp(name, "syslog.sh")){
		;// to nothing
	}	
	else if(!strcmp(name, "wan.sh")){
		kill_pidfile_s("/var/run/infosvr.pid", SIGUSR1);
	}
	else if(!strcmp(name, "printer.sh")){	
		// update status of printer
		kill_pidfile_s("/var/run/infosvr.pid", SIGUSR1);
	}
	else if(!strcmp(name, "lpr_remove")){
		kill_pidfile_s("/var/run/lpdparent.pid", SIGUSR2);
	}
#ifdef U2EC
	else if(!strcmp(name, "mfp_requeue")){
		//unsigned int login_ip = (unsigned int)atoll(nvram_safe_get("login_ip"));
		
		if(login_ip == 0x100007f || login_ip == 0x0)
			nvram_set("mfp_ip_requeue", "");
		else{
			struct in_addr addr;
			
			addr.s_addr = login_ip;
			nvram_set("mfp_ip_requeue", inet_ntoa(addr));
		}
		
		int u2ec_fifo = open("/var/u2ec_fifo", O_WRONLY|O_NONBLOCK);
		
		write(u2ec_fifo, "q", 1);
		close(u2ec_fifo);
	}
	else if (!strcmp(name, "mfp_monopolize")){
		//unsigned int login_ip = (unsigned int)atoll(nvram_safe_get("login_ip"));

		if (login_ip==0x100007f || login_ip==0x0)
			nvram_set("mfp_ip_monopoly", "");
		else
		{
			struct in_addr addr;
			addr.s_addr=login_ip;
			nvram_set("mfp_ip_monopoly", inet_ntoa(addr));
		}
		int u2ec_fifo = open("/var/u2ec_fifo", O_WRONLY|O_NONBLOCK);
		write(u2ec_fifo, "m", 1);
		close(u2ec_fifo);
	}
#endif
	else if(!strcmp(name, "wlan11a.sh") || !strcmp(name,"wlan11b.sh")){
		;// do nothing
	}
	else if(!strcmp(name,"leases.sh") || !strcmp(name,"dleases.sh")){
		sys_refresh_lease();
	}
	else if(!strcmp(name,"iptable.sh")){
		;// TODO
	}
	else if(!strcmp(name,"route.sh")){
		// TODO
	}
	else if(!strcmp(name,"radio_disable")){
		//eval("wl", "radio", "off");
		eval("radioctrl", "0");
	}
	else if(!strcmp(name,"radio_enable")){
		//eval("wl", "radio", "on");
		eval("radioctrl", "1");
	}
	else if(!strcmp(name,"dhcpc_renew")){
		sys_renew();
		sleep(1);
	}
	else if(!strcmp(name,"dhcpc_release")){
		sys_release();
		sleep(1);
	}
	else if(!strcmp(name,"eject-usb.sh")){
		eval("rmstorage");
	}
	else if(!strcmp(name,"ddnsclient")){
		eval("start_ddns");
	}
#ifdef CDMA // HSDPA {
	else if(!strcmp(name,"hspda_refresh")){
		kill_pidfile_s("/var/run/AT.pid", SIGUSR1);
	}
#endif // HSDPA }
#ifdef ASUS_DDNS //2007.03.22 Yau add
	else if(!strcmp(name, "hostname_check")){
		int i=0;
		char return_buf[32];
		
		//Clear asus ddns nvram variables.
		nvram_set("httpd_check_ddns","1");
		nvram_set("ddns_timeout", "0");
		nvram_set("rc_service", "ddns_hostname_check");
		
		kill(1, SIGUSR1);
		sleep(1);
		
		while(1){
			strcpy(return_buf, nvram_safe_get("ddns_return_code"));
			if(strcmp(return_buf, ""))
				break;
			else if(i == 6){ //asus ddns timeout 
				nvram_set("ddns_timeout", "1");
				break;
			}
			
			sleep(1);
			++i;
		}
	}
#endif
	else if(strstr(scmd, " ") == 0){ // no parameter, run script with eval
		eval(scmd);
	}
	else
		system(scmd);  
}


/* 
 *   Byte 0 : Firmware Major
 *   Byte 1 : Firmware Minor 
 *   Byte 2 : File system Major
 *   Byte 3 : File system Minor
 *   Byte 4~15 : Product ID
 *   Byte 16~31: Hardware compatible list( 4 pairs)
 *      16+x*4: Minimum Hardware Major
 *      17+x*4: Minimum Hardware Minor
 *      18+x*4: Maximum Hardware Major
 *      19+x*4: Maximum Hardware Minor
 *      x= 0~3
 */
char checkVersion(char *version, unsigned char major, unsigned char minor)
{
        #define VINT(ver) (((((ver)[0])&0xff)<<8)|(((ver)[1])&0xff))        
        int i;
        unsigned int ver, min, max;
        char *clist;
        
        clist = version+16;
        ver = major << 8 | minor;
        
        //printf("ver: %d %d %d\n", major, minor, ver);
        i = 0;
        
        while(VINT(clist+i) && i<16)
        {
            min = VINT(clist+i); 	
            max = VINT(clist+i+2);
                        
            //printf("List: %x %x %x %x %x %x %x %x\n", i, ver, min, max, clist[i], clist[i+1], clist[i+2], clist[i+3]);
           
            if (ver>=min && (max==0 || ver<=max)) return 1; 
            i+=4;	
        }         
        if (i==0 || ver==0) return 1;                              	
        else return 0;
}

void websScan(char_t *str)
{
	unsigned int i, flag;
        char_t *v1, *v2, *v3, *sp;
        char_t groupid[64];
        char_t value[MAX_LINE_SIZE];
        char_t name[MAX_LINE_SIZE];
        
        v1 = strchr(str, '?');
                        
        i = 0;
        flag = 0;
                                     		
	while(v1!=NULL)
	{	   	    	
	    v2 = strchr(v1+1, '=');
	    v3 = strchr(v1+1, '&');
	    
// 2007.12 James. {
		if(v2 == NULL)
			break;
// 2007.12 James. }
	    
	    if (v3!=NULL)
	    {
	       strncpy(value, v2+1, v3-v2-1);
	       value[v3-v2-1] = 0;  
	    }  
	    else
	    {
	       strcpy(value, v2+1);
	    }
	    
	    strncpy(name, v1+1, v2-v1-1);
	    name[v2-v1-1] = 0;
	    /*printf("Value: %s %s\n", name, value);*/
	    
	    if (v2 != NULL && ((sp = strchr(v1+1, ' ')) == NULL || (sp > v2))) 
	    {	    	
	       if (flag && strncmp(v1+1, groupid, strlen(groupid))==0)
	       {	    		    	   
	           delMap[i] = atoi(value);
	           /*printf("Del Scan : %x\n", delMap[i]);*/
	           if (delMap[i]==-1)  break;	           	           
	           i++;
	       }	
               else if (strncmp(v1+1,"group_id", 8)==0)
               {            	                               
                   sprintf(groupid, "%s_s", value);
                   flag = 1;
               }   
            }
	    v1 = strchr(v1+1, '&');
	} 
	delMap[i] = -1;
	return;
}


void websApply(webs_t wp, char_t *url)
{
//#ifdef TRANSLATE_ON_FLY
	do_ej (url, wp);
	websDone (wp, 200);
/*#else   // define TRANSLATE_ON_FLY
	FILE *fp;
	char buf[MAX_LINE_SIZE];

	fp = fopen(url, "r");
	if (fp == NULL)
		return;

	while(fgets(buf, sizeof(buf), fp))
	{
		websWrite(wp, buf);
	} 

	websDone(wp, 200);
	fclose(fp);
#endif//*/
}

/*
 * Example: 
 * lan_ipaddr=192.168.1.1
 * <% nvram_get_x("lan_ipaddr"); %> produces "192.168.1.1"
 * <% nvram_get_x("undefined"); %> produces ""
 */
static int
ej_nvram_get_x(int eid, webs_t wp, int argc, char_t **argv)
{
	char *sid, *name, *c;
	int ret = 0;

	if (ejArgs(argc, argv, "%s %s", &sid, &name) < 2) {
		websError(wp, 400, "Insufficient args\n");
		return -1;
	}

	for (c = nvram_safe_get_x(sid, name); *c; c++) {
		if (isprint(*c) &&
		    *c != '"' && *c != '&' && *c != '<' && *c != '>')
			ret += websWrite(wp, "%c", *c);
		else
			ret += websWrite(wp, "&#%d", *c);
	}

	return ret;
}

#ifdef ASUS_DDNS
static int
ej_nvram_get_ddns(int eid, webs_t wp, int argc, char_t **argv)
{
        char *sid, *name, *c;
        int ret = 0;
                                                                                                                                              
        if (ejArgs(argc, argv, "%s %s", &sid, &name) < 2) {
                websError(wp, 400, "Insufficient args\n");
                return -1;
        }
                                                                                                                                              
        for (c = nvram_safe_get_x(sid, name); *c; c++) {
                if (isprint(*c) &&
                    *c != '"' && *c != '&' && *c != '<' && *c != '>')
                        ret += websWrite(wp, "%c", *c);
                else
                        ret += websWrite(wp, "&#%d", *c);
        }

	if(strcmp(name,"ddns_return_code")==0)
		nvram_set("ddns_return_code","");
	if(strcmp(name,"ddns_timeout")==0)
		nvram_set("ddns_timeout","0");

        return ret;
}
#endif
/*
 * Example: 
 * lan_ipaddr=192.168.1.1
 * <% nvram_get_x("lan_ipaddr"); %> produces "192.168.1.1"
 * <% nvram_get_x("undefined"); %> produces ""
 */
static int
ej_nvram_get_f(int eid, webs_t wp, int argc, char_t **argv)
{
	char *file, *field, *c, buf[64];
	int ret = 0;

	if (ejArgs(argc, argv, "%s %s", &file, &field) < 2) {
		websError(wp, 400, "Insufficient args\n");
		return -1;
	}
			
	strcpy(buf, nvram_safe_get_f(file, field));		
        for (c = buf; *c; c++) {
		if (isprint(*c) &&
		    *c != '"' && *c != '&' && *c != '<' && *c != '>')
			ret += websWrite(wp, "%c", *c);
		else
			ret += websWrite(wp, "&#%d", *c);
	}

	return ret;
}

/*
 * Example: 
 * wan_proto=dhcp
 * <% nvram_match("wan_proto", "dhcp", "selected"); %> produces "selected"
 * <% nvram_match("wan_proto", "static", "selected"); %> does not produce
 */
static int
ej_nvram_match_x(int eid, webs_t wp, int argc, char_t **argv)
{
	char *sid, *name, *match, *output;

	if (ejArgs(argc, argv, "%s %s %s %s", &sid, &name, &match, &output) < 4) {
		websError(wp, 400, "Insufficient args\n");
		return -1;
	}
	
	if (nvram_match_x(sid, name, match))
	{
		return websWrite(wp, output);
	}	

	return 0;
}	

static int
ej_nvram_double_match_x(int eid, webs_t wp, int argc, char_t **argv)
{
	char *sid, *name, *match, *output;
	char *sid2, *name2, *match2;

	if (ejArgs(argc, argv, "%s %s %s %s %s %s %s", &sid, &name, &match, &sid2, &name2, &match2, &output) < 7) {
		websError(wp, 400, "Insufficient args\n");
		return -1;
	}
	
	if (nvram_match_x(sid, name, match) && nvram_match_x(sid2, name2, match2))
	{
		return websWrite(wp, output);
	}	

	return 0;
}

/*
 * Example: 
 * wan_proto=dhcp
 * <% nvram_match("wan_proto", "dhcp", "selected"); %> produces "selected"
 * <% nvram_match("wan_proto", "static", "selected"); %> does not produce
 */
static int
ej_nvram_match_both_x(int eid, webs_t wp, int argc, char_t **argv)
{
	char *sid, *name, *match, *output, *output_not;

	if (ejArgs(argc, argv, "%s %s %s %s %s", &sid, &name, &match, &output, &output_not) < 5) 
        {
		websError(wp, 400, "Insufficient args\n");
		return -1;
	}
	
	if (nvram_match_x(sid, name, match))
	{
		return websWrite(wp, output);
	}
        else
        {
                return websWrite(wp, output_not);
        }	
}

/*
 * Example: 
 * wan_proto=dhcp
 * <% nvram_match_both("wan_proto", "dhcp", "selected", "not"); %> produces "selected"
 * <% nvram_match_both("wan_proto", "static", "selected", "not"); %> produces "not"
 */
static int
ej_nvram_match_list_both_x(int eid, webs_t wp, int argc, char_t **argv)
{
	char *sid, *name, *match, *output, *output_ex;
	int index;

	if (ejArgs(argc, argv, "%s %s %s %s %s %d", &sid, &name, &match, &output, &output_ex, &index) < 6) {
		websError(wp, 400, "Insufficient args\n");
		return -1;
	}

	if (nvram_match_list_x(sid, name, match, index))
		return websWrite(wp, output);
	else
	        return websWrite(wp, output_ex);		
}	


/*
 * Example: 
 * lan_ipaddr=192.168.1.1 192.168.39.248
 * <% nvram_get_list("lan_ipaddr", 0); %> produces "192.168.1.1"
 * <% nvram_get_list("lan_ipaddr", 1); %> produces "192.168.39.248"
 */
static int
ej_nvram_get_list_x(int eid, webs_t wp, int argc, char_t **argv)
{
	char *sid, *name;
	int which;	
	int ret = 0;

	if (ejArgs(argc, argv, "%s %s %d", &sid, &name, &which) < 3) {
		websError(wp, 400, "Insufficient args\n");
		return -1;
	}

	ret += websWrite(wp, nvram_get_list_x(sid, name, which));
	return ret;
}

/*
 * Example: 
 * lan_ipaddr=192.168.1.1 192.168.39.248
 * <% nvram_get_list("lan_ipaddr", 0); %> produces "192.168.1.1"
 * <% nvram_get_list("lan_ipaddr", 1); %> produces "192.168.39.248"
 */
static int
ej_nvram_get_buf_x(int eid, webs_t wp, int argc, char_t **argv)
{
	char *sid, *name;
	int which;		

	if (ejArgs(argc, argv, "%s %s %d", &sid, &name, &which) < 3) {
		websError(wp, 400, "Insufficient args\n");
		return -1;
	}

	
	return 0;
}

/*
 * Example: 
 * lan_ipaddr=192.168.1.1 192.168.39.248
 * <% nvram_get_table_x("lan_ipaddr"); %> produces "192.168.1.1"
 * <% nvram_get_table_x("lan_ipaddr"); %> produces "192.168.39.248"
 */
static int
ej_nvram_get_table_x(int eid, webs_t wp, int argc, char_t **argv)
{
	char *sid, *name;
	int ret = 0;
	
	if(ejArgs(argc, argv, "%s %s", &sid, &name) < 2){
		websError(wp, 400, "Insufficient args\n");
		return -1;
	}
	
	ret += nvram_generate_table(wp, sid, name);
	return ret;
}

/*
 * Example: 
 * wan_proto=dhcp;dns
 * <% nvram_match_list("wan_proto", "dhcp", "selected", 0); %> produces "selected"
 * <% nvram_match_list("wan_proto", "static", "selected", 1); %> does not produce
 */
static int
ej_nvram_match_list_x(int eid, webs_t wp, int argc, char_t **argv)
{
	char *sid, *name, *match, *output;
	int which;

	if (ejArgs(argc, argv, "%s %s %s %s %d", &sid, &name, &match, &output, &which) < 5) {
		websError(wp, 400, "Insufficient args\n");
		return -1;
	}
	
	if (nvram_match_list_x(sid, name, match, which))
		return websWrite(wp, output);
	else
	        return 0;		
}	


static int
ej_select_channel(int eid, webs_t wp, int argc, char_t **argv)
{
	char *sid, chstr[32];
	int ret = 0;	
	int i, channel, channels[32];
	
	if (ejArgs(argc, argv, "%s", &sid) < 1) {
		websError(wp, 400, "Insufficient args\n");
		return -1;
	}
         
        channel=atoi(nvram_safe_get_x("", "wl_channel"));

	if (!wl_channels_in_country(nvram_safe_get_x("", "wl_country_code"), channels))
	{
		wl_channels_in_country_asus(nvram_safe_get_x("", "wl_country_code"), channels);
	}

	i = 0;

	while(channels[i]!=-1)
	{
		if (channels[i] == 0) strcpy(chstr, "Auto");
		else sprintf(chstr, "%d", channels[i]);

        	if (channel==channels[i])
            		ret += websWrite(wp, "<option value=\"%d\" selected>%s</option>", channels[i], chstr);
        	else
            		ret += websWrite(wp, "<option value=\"%d\">%s</option>", channels[i], chstr);
		i++;
	} 
	return ret;
}


static int
ej_urlcache(int eid, webs_t wp, int argc, char_t **argv)
{
	int flag;
		
	if (strcmp(urlcache, "Main_Operation.asp")==0)
	   flag = 2;
	else if (strcmp(urlcache, "Main_Content.asp")==0)
	   flag = 1;		  		
	else
	   flag = 0;
	   
	cprintf("Url:%s %d\n", urlcache, flag);

	if (strcmp(nvram_safe_get_x("IPConnection","wan_route_x"), "IP_Routed")==0)
	{	
	   if (strcmp(nvram_safe_get_x("IPConnection", "wan_nat_x"), "1")==0)
	   {	   
	   	/* disable to redirect page */
	   	if (redirect)
	   	{	
			websWrite(wp, "Basic_GOperation_Content.asp");
			redirect=0;
           	}
	   	else if (flag==2)
			websWrite(wp, "Basic_GOperation_Content.asp");
	   	else if (flag==1)
			websWrite(wp, "Basic_HomeGateway_SaveRestart.asp");
	   	else
	   		websWrite(wp, "Main_Index_HomeGateway.asp");
	    }
	    else
	    {	
	   	/* disable to redirect page */
	   	if (redirect)
	   	{	
			websWrite(wp, "Basic_ROperation_Content.asp");
			redirect=0;
           	}
	   	else if (flag==2)
			websWrite(wp, "Basic_ROperation_Content.asp");
	   	else if (flag==1)
			websWrite(wp, "Basic_Router_SaveRestart.asp");
	   	else
	   		websWrite(wp, "Main_Index_Router.asp");
            }	
	}      
	else
	{
	    if (flag==2)
		websWrite(wp, "Basic_AOperation_Content.asp");
	    else if (flag==1)	
		websWrite(wp, "Basic_AccessPoint_SaveRestart.asp");
	    else	
	    	websWrite(wp, "Main_Index_AccessPoint.asp");
	}		
	strcpy(urlcache,"");
	
	return 0;
}


/* Report sys up time */
static int
ej_uptime(int eid, webs_t wp, int argc, char_t **argv)
{

	FILE *fp;
	char buf[MAX_LINE_SIZE];
	unsigned long uptime;
	int ret;
	char *str = file2str("/proc/uptime");
	time_t tm;

	time(&tm);
	sprintf(buf, rfctime(&tm));

	if (str) {
		unsigned int up = atoi(str);
		free(str);
		sprintf(buf, "%s(%s since boot)", buf, reltime(up));
	}

	ret = websWrite(wp, buf);  
	return ret;	    
}

/* Dump firewall log */
static int
ej_dumplog(int eid, webs_t wp, int argc, char_t **argv)
{
	#define MAX_LOG_BUF 2048
	#define MAX_LOG_SIZE 16384
	char buf[MAX_LOG_BUF], *line;
	char *next, *s;
	int len, ret = 0;
	long filelen;
	time_t tm;
	char *verdict, *src, *dst, *proto, *spt, *dpt;
	FILE *fpOrig, *fp;

        if ((fp = fopen("/etc/linuxigd/firewall.log","a+"))==NULL)
	{	
		cprintf("Open file fail\n");
		goto fail;
	}
	
	if ((fpOrig = fopen("/etc/linuxigd/firewallOrig.log","r"))!=NULL)
	{
	   /* Find size of orignal file */
	   fseek(fpOrig, 0, SEEK_END);
	   filelen = ftell(fp);
	
	   if (filelen>MAX_LOG_SIZE)
	      fseek(fpOrig, -(filelen-MAX_LOG_SIZE), SEEK_END);
	   else
	      fseek(fpOrig, 0, SEEK_SET);     
	      
	   while((len=fread(buf, 1, MAX_LOG_BUF, fpOrig)))
	   {	    	
	       fwrite(buf, 1, len, fp);
	   }  	  							
	   fclose(fpOrig);	   
	   unlink("/etc/linuxigd/firewallOrig.log");
	}   
			
	while((len=klogctl(4, buf, MAX_LOG_BUF))>0)
	{
	    //printf("W:%s\n", buf);
	    fwrite(buf, 1, len, fp);
	}
	
	fclose(fp);
	
	if ((fp = fopen("/etc/linuxigd/firewall.log","r"))==NULL)
	{	
		cprintf("Open file fail\n");
		goto fail;
	}
	
	//fseek(fp, 0, SEEK_SET);
		
#ifdef REMOVE_WL600	
	for (next = buf; (line = strsep(&next, "\n"));) 
#else
	while(fgets(buf, MAX_LOG_BUF, fp))
#endif	
	{
#ifndef REMOVE_WL600		
		line = buf;
		if (!strncmp(line, "<4>DROP", 7))
			verdict = "denied";
		else if (!strncmp(line, "<4>ACCEPT", 9))
			verdict = "accepted";
		else
			continue;

		/* Parse into tokens */
		s = line;
		len = strlen(s);
		while (strsep(&s, " "));

		/* Initialize token values */
		time(&tm);
		src = dst = proto = spt = dpt = "n/a";

		/* Set token values */
		for (s = line; s < &line[len] && *s; s += strlen(s) + 1) {
			if (!strncmp(s, "TIME=", 5))
				tm = strtoul(&s[5], NULL, 10);
			else if (!strncmp(s, "SRC=", 4))
				src = &s[4];
			else if (!strncmp(s, "DST=", 4))
				dst = &s[4];
			else if (!strncmp(s, "PROTO=", 6))
				proto = &s[6];
			else if (!strncmp(s, "SPT=", 4))
				spt = &s[4];
			else if (!strncmp(s, "DPT=", 4))
				dpt = &s[4];
		}

		ret += websWrite(wp, "%s %s connection %s to %s:%s from %s:%s\n",
				 rfctime(&tm), proto, verdict, dst, dpt, src, spt);
#else
		ret += websWrite(wp, "%s\n", buf);				 
		cprintf("L:%s\n",buf);
#endif				 
	}	
	fclose(fp);	
	return ret;
fail:
	ret += websWrite(wp, "");
	return ret;	
}

struct lease_t {
	unsigned char chaddr[16];
	u_int32_t yiaddr;
	u_int32_t expires;
	char hostname[64];
};

int
websWriteCh(webs_t wp, char *ch, int count)
{
   int i, ret;
   
   ret = 0;
   for(i=0; i<count; i++)
      ret+=websWrite(wp, "%s", ch);
   return(ret);   
} 

/* Dump leases in <tr><td>MAC</td><td>IP</td><td>expires</td></tr> format */
static int
#ifdef REMOVE_WL600
ej_dumpleases(int eid, webs_t wp, int argc, char_t **argv)
#else
ej_dumpleases(int eid, webs_t wp, char *lease_file)
#endif
{
	FILE *fp;
	struct lease_t lease;
	int i;
	struct in_addr addr;
	unsigned long expires;
#ifdef REMOVE_WL600	
	char *format;
	char f[]="";
	char sigusr1[] = "-XX";
#endif	
	int ret = 0;

#ifdef REMOVE_WL600
	if (ejArgs(argc, argv, "%s", &format) < 1) {
		websError(wp, 400, "Insufficient args\n");
		return -1;
	}

	/* Write out leases file */
	sprintf(sigusr1, "-%d", SIGUSR1);
	eval("killall", sigusr1, "udhcpd");
#endif	

    ret +=websWrite(wp,"Mac Address       IP Address      Lease Time\n");	
        
	/* Parse leases file */
	if (!(fp = fopen(lease_file, "r"))) 
	{
		//websWrite(wp, "No leases\n");
		return -1;
	}
					    	           	
	while (fread(&lease, sizeof(lease), 1, fp)) {
		//ret += websWrite(wp, "%s", format);
		for (i = 0; i < 6; i++) {
			ret += websWrite(wp, "%02x", lease.chaddr[i]);
			if (i != 5) ret += websWrite(wp, ":");
		}
		addr.s_addr = lease.yiaddr;
		ret += websWrite(wp, " %s", inet_ntoa(addr));
		ret += websWriteCh(wp," ", 16-strlen(inet_ntoa(addr)));
		expires = ntohl(lease.expires);
		
		if (expires==0xffffffff)	
			ret += websWrite(wp, "manual");
		else if (!expires)
			ret += websWrite(wp, "expired");
		else {
			if (expires > 60*60*24) {
				ret += websWrite(wp, "%ld days, ", expires / (60*60*24));
				expires %= 60*60*24;
			}
			if (expires > 60*60) {
				ret += websWrite(wp, "%ld hours, ", expires / (60*60));
				expires %= 60*60;
			}
			if (expires > 60) {
				ret += websWrite(wp, "%ld minutes, ", expires / 60);
				expires %= 60;
			}
			ret += websWrite(wp, "%ld seconds", expires);
		}
		ret += websWrite(wp, "\n");
	}
	fclose(fp);

	return ret;
}

/* Dump leases in <tr><td>MAC</td><td>IP</td><td>expires</td></tr> format */
static int
ej_dumpiptable(int eid, webs_t wp, char *iptable_file)
{	
	/* Format 											    */
	/* DNAT       tcp  --  anywhere             0.0.0.0            tcp dpts:6970:32000 to:192.168.122.2 */
	char buf[MAX_LINE_SIZE], *line, *s;
	char *dst, *prot, *port, *rdst;
	int ret, i, len;
	FILE *fp;

        ret +=websWrite(wp,"Destination     Prot. Port Range  Redirect to\n");	
        
	/* Parse leases file */
	if (!(fp = fopen(iptable_file, "r"))) 
	{
		//websWrite(wp, "No leases\n");
		return -1;
	}
					    	           	
	while (fgets(buf, MAX_LINE_SIZE, fp)) 
	{
	     line = buf;	
	     /* Parse into tokens */
	     s = line;
	     len = strlen(s);
	     
	     //printf("Line: %d %s\n", len, line);
	     
	     while (strsep(&s, " "));

	     
	     
	     /* Initialize token values */	     
	     dst = rdst = "n/a";
	     prot = port = "all";

	     i = 0;
	     /* Set token values */
  	     /* destination protocol:...  proto: */
	     for (s = line; s < &line[len]; s += strlen(s) + 1)
	     {
		if (i==1) /* Destination */
	           dst = &s[0];
	     	else if (!strncmp(s, "tcp", 3)) /* Proto */
	     	   prot = &s[0];
	     	else if (!strncmp(s, "udp", 3)) /* Proto */
	     	   prot = &s[0];   
	     	else if (!strncmp(s, "all", 3)) /* Proto */
	     	   prot = &s[0];   
	     	else if (!strncmp(s, "dpts:", 5))
	           port = &s[5];	
	        else if (!strncmp(s, "dpt:", 4))
	           port = &s[4];	   		    
		else if (!strncmp(s, "to:", 3))
		   rdst = &s[3];
		else if (!strncmp(s, "protox:", 7))
		   prot = &s[7];

		if (strlen(s)!=0) i++;
		//printf("Token: %d %s\n", i, s);
	     }
	     
	     ret +=websWrite(wp, "%s", dst);
	     ret +=websWriteCh(wp, " ", 16-strlen(dst));
	     ret +=websWrite(wp, "%s", prot);
	     ret +=websWriteCh(wp, " ", 6-strlen(prot));
	     ret +=websWrite(wp, "%s", port);
	     ret +=websWriteCh(wp, " ", 12-strlen(port));
	     ret +=websWrite(wp, "%s\n", rdst);	    
	}
	fclose(fp);

	return ret;
}

static int dump_file(webs_t wp, char *filename)
{
	FILE *fp;
	char buf[MAX_LINE_SIZE];
	int ret;

	//printf("dump: %s\n", filename);
	
	fp = fopen(filename, "r");
		
	if (fp==NULL) 
	{
		ret+=websWrite(wp, "");
		return(ret);
	}

	ret = 0;
		
	while (fgets(buf, MAX_LINE_SIZE, fp)!=NULL)
	{	 	
	    //printf("Read time: %s\n", buf);	    	    	    	   
	    ret += websWrite(wp, "%s", buf);
	}		    	                             		
	 
	fclose(fp);		
	
	return(ret);
}

#ifndef NOUSB
int 
deliver_signal(char *pidfile, int sig)
{

	FILE *fp;
	pid_t pid;	    
	fp=fopen(pidfile,"r");	    
	if (fp!=NULL)
	{
	    	fscanf(fp, "%d", &pid);
	    	kill(pid, sig);
	    	fclose(fp);	 
		return 0;
	}
	return 1;
}
#endif

static int
ej_dump(int eid, webs_t wp, int argc, char_t **argv)
{
	FILE *fp;
	char buf[MAX_LINE_SIZE];
	char filename[32];
	char *file, *script;
	int ret;
	
	if(ejArgs(argc, argv, "%s %s", &file, &script) < 2){
		websError(wp, 400, "Insufficient args\n");
		return -1;
	}
	
	//cprintf("File : %s, Script: %s\n", file, script);
	
	// run scrip first to update some status
	if(strcmp(script, ""))
		sys_script(script); 
	
	if(!strcmp(file, "wlan11b.log")){
//#ifndef NOUSB
//		if(nvram_match_x("", "nobr", "1"))
//			return(ej_wl_status_nobr(eid, wp, 0, NULL));
//		else
//#endif
			return(ej_wl_status(eid, wp, 0, NULL));
	}
	else if(!strcmp(file, "leases.log")) 
		return(ej_lan_leases(eid, wp, 0, NULL));
	else if(!strcmp(file, "iptable.log")) 
		return(ej_nat_table(eid, wp, 0, NULL));
	else if(!strcmp(file, "route.log"))
		return(ej_route_table(eid, wp, 0, NULL));
        else if(strcmp(file, "apscan")==0)
                return (ej_SiteSurvey(eid, wp, 0, NULL));
        else if(strcmp(file, "urelease")==0)
                return (ej_urelease(eid, wp, 0, NULL));

	
	ret = 0;
	
	if(!strcmp(file, "syslog.log")){
		sprintf(filename, "/tmp/%s-1", file);
		ret += dump_file(wp, filename);
	}
	
	sprintf(filename, "/tmp/%s", file);
	ret += dump_file(wp, filename);
	
	return ret;
}

static int
ej_load(int eid, webs_t wp, int argc, char_t **argv)
{	
	char *script;
	
	if (ejArgs(argc, argv, "%s", &script) < 1) {
		websError(wp, 400, "Insufficient args\n");
		return -1;
	}
	 	  
	sys_script(script);
	return(websWrite(wp,""));						    
}	

/* Return PPPoE link state */
static int
ej_ppplink(int eid, webs_t wp, int argc, char_t **argv)
{
	FILE *fp = fopen("/tmp/ppp/link", "r");

	if (!fp)
		return websWrite(wp, "Down");
	else {
		fclose(fp);
		return websWrite(wp, "Up");
	}
}



static void
validate_cgi(webs_t wp, int sid, int groupFlag)
{
	struct variable *v;
	char *value;
	const char name[64];
	
	/* Validate and set variables in table order */
	for(v = GetVariables(sid); v->name != NULL; ++v)
	{
		//printf("Name: %s %d\n", v->name, sid);
		memset(name, 0, 64);
		sprintf(name, "%s", v->name);
		
		if((value = websGetVar(wp, name, NULL)))
		{
			if(!strcmp(v->longname, "Group"))
			{
				;
			}
			else
			{
printf("set sid: %s, name: %s, value: %s.\n", GetServiceId(sid), v->name, value);
				nvram_set_x(GetServiceId(sid), v->name, value);
			}
		}
		
#ifdef REMOVE_WL600
		if((!*value && v->nullok) || !v->validate)
		{
			nvram_set_x(GetServiceId(sid), v->name, value);
		}
		else
		{
			if(v->validate(value, v) == UPNP_E_SUCCESS)
				nvram_set_x(FindSvcLink(sid), v->name, value);
			else
				websWrite(wp, "Invalid %s = %s\n", v->name, value);
		}
#endif
	}
}

#ifndef NOUSB
static const char * const apply_header =
"<head>"
"<title>ASUS Wireless AP/Router: Apply</title>"
"<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\">"
"<style type=\"text/css\">"
"body { background: white; color: black; font-family: arial, sans-serif; font-size: 9pt }"
".title	{ font-family: arial, sans-serif; font-size: 13pt; font-weight: bold }"
".subtitle { font-family: arial, sans-serif; font-size: 11pt }"
".label { color: #306498; font-family: arial, sans-serif; font-size: 7pt }"
"</style>"
"</head>"
"<body>"
"<p>"
"<span class=\"title\">APPLY</span><br>"
"<span class=\"subtitle\">This screen notifies you of any errors "
"that were detected while changing the router's settings.</span>"
"<form method=\"get\" action=\"apply.cgi\">"
"<p>"
;

static const char * const apply_footer =
"<p>"
"<input type=\"button\" name=\"action\" value=\"Continue\" OnClick=\"history.go(-1)\">"
"</form>"
"<p class=\"label\">&#169;2001 ASUSTeK COMPUTER INC. All rights reserved.</p>"
"</body>"
;
#endif

enum {
	NOTHING,
	REBOOT,
	RESTART,
};

char *svc_pop_list(char *value, char key)
{    
    char *v, *buf;
    int i;
               
    if (value==NULL || *value=='\0')
       return(NULL);      
            
    buf = value;
    v = strchr(buf, key);

    i = 0;
    
    if (v!=NULL)
    {    	
        *v = '\0';  	
        return(buf);    	   
    }    
    return(NULL);
}

#ifndef NOUSB
static int special_handler(webs_t wp, char_t *url)
{
    #define MAX_SERVICES 5	
    char *flag;
    char *svc[MAX_SERVICES] = {"80","5190","554","23","20:21"};
    char *sid="FirewallConfig";
    char name[24];	
    int i, count;
    	  	
    if ((flag = websGetVar(wp, "FirewallConfig_LanWanActiveSVCFlag", NULL)) && strcmp(flag,""))
    {   
    	count  = 0; 	        	    
        for(i=0;i<MAX_SERVICES;i++)
        { 
			//printf("Save SVC:%d %s\n", i, svc[i]);         	 	    				

    	    if (flag[i] == '1')
    	    { 	    	    		
    	        sprintf(name, "LanWanSrcIP%d", count);
    	        nvram_set_x(sid, name, "");
    	        sprintf(name, "LanWanSrcPort%d", count);
    	        nvram_set_x(sid, name, "");
    	        sprintf(name, "LanWanDstIP%d", count);
    	        nvram_set_x(sid, name, "");
    	        sprintf(name, "LanWanDstPort%d", count);
    	        nvram_set_x(sid, name, svc[i]);
    	        sprintf(name, "LanWanProFlag%d", count);
    	        nvram_set_x(sid, name, "TCP");
    	        count++;
    	    }	    		    	    
    	}  
    	sprintf(name, "%d", count);  		
    	nvram_set_x(sid, "LanWanRuleCount", name);
    }
    
    return 0;
}
#endif

// 2007.10 James {
static char post_buf[10000] = { 0 };
static char post_buf_backup[10000] = { 0 };

static void do_html_post_and_get(char *url, FILE *stream, int len, char *boundary){
	char *query = NULL;
	
	init_cgi(NULL);
	
	memset(post_buf, 0, 10000);
	memset(post_buf_backup, 0, 10000);
	
	if(fgets(post_buf, MIN(len+1, sizeof(post_buf)), stream)){
		len -= strlen(post_buf);
		
		while(len--)
			(void)fgetc(stream);
	}
	
	query = url;
	strsep(&query, "?");
	
	if(query && strlen(query) > 0){
		if(strlen(post_buf) > 0)
			sprintf(post_buf_backup, "?%s&%s", post_buf, query);
		else
			sprintf(post_buf_backup, "?%s", query);
		
		sprintf(post_buf, "%s", post_buf_backup+1);
	}
	else if(strlen(post_buf) > 0)
		sprintf(post_buf_backup, "?%s", post_buf);
	
	websScan(post_buf_backup);	// there was '?' in the head.
	init_cgi(post_buf);	// there wasn't '?' in the head.
}

static char *error_msg_console = NULL, *error_msg = NULL;

static char *get_msg_from_dict(char *lang, const char *const msg_name){
#define MAX_FILE_LENGTH 512
	char current_dir[MAX_FILE_LENGTH];
	char dict_file[MAX_FILE_LENGTH], *dict_info;
	char *follow_info, *follow_info_end, *target;
	int len;
	
	if(lang == NULL || strlen(lang) == 0)
		lang = "EN";
	
	memset(dict_file, 0, MAX_FILE_LENGTH);
	memset(current_dir, 0, MAX_FILE_LENGTH);
	getcwd(current_dir, MAX_FILE_LENGTH);
	sprintf(dict_file, "%s/%s.dict", current_dir, lang);
	
	dict_info = read_whole_file(dict_file);
	if(dict_info == NULL){
		printf("No dictionary file, \"%s\".\n", dict_file);
		return NULL;
	}
	
	follow_info = strstr(dict_info, msg_name);
	if(follow_info == NULL){
		printf("No \"%s\" in the dictionary file.\n", msg_name);
		free(dict_info);
		return NULL;
	}
	
	follow_info += strlen(msg_name)+strlen("=");
	follow_info_end = follow_info;
	while(*follow_info_end != 0 && *follow_info_end != '\n')
		++follow_info_end;
	
	len = follow_info_end-follow_info;
	
	target = (char *)malloc(sizeof(char)*(len+1));
	if(target == NULL){
		printf("No memory \"target\".\n");
		free(dict_info);
		return NULL;
	}
	if(len > 0)
		strncpy(target, follow_info, len);
	target[len] = 0;
	
	free(dict_info);
	return target;
}

static void show_error_msg(const char *const msg_num){
	char msg_name[32];
	
	memset(msg_name, 0, 32);
	sprintf(msg_name, "ALERT_OF_ERROR_%s", msg_num);
	
	error_msg_console = get_msg_from_dict("", msg_name);
	error_msg = get_msg_from_dict(nvram_safe_get("preferred_lang"), msg_name);
	
	return;
}

static void clean_error_msg(){
	if(error_msg_console != NULL)
		free(error_msg_console);
	
	if(error_msg != NULL)
		free(error_msg);
	
	return;
}

// Transfer Char to ASCII
void char_to_ascii(char *output, char *input){
	int i;
	char tmp[10];
	char *ptr;
	
	ptr = output;
	
	for(i = 0; i < strlen(input); ++i){
		if((input[i] >= '0' && input[i] <= '9')
				|| (input[i] >= 'A' && input[i] <= 'Z')
				|| (input[i] >= 'a' && input[i] <= 'z')
				|| input[i] == '!' || input[i] == '*'
				|| input[i] == '(' || input[i] == ')'
				|| input[i] == '_' || input[i] == '-'
//				|| input[i] == '\'' || input[i] == '.'){
				|| input[i] == '.'){
			*ptr = input[i];
			++ptr;
		}
		else{
			memset(tmp, 0, 10);
			sprintf(tmp, "%%%.02X", input[i]);
			strcpy(ptr, tmp);
			ptr += 3;
		}
	}
	
	*ptr = '\0';
}

int nvram_modified = 0;

static int validate_asp_apply(webs_t wp, int sid, int unSetDefault){
	struct variable *v;
	char *value;
	char name[64];
	char buff[100];
	
	/* Validate and set variables in table order */
	for(v = GetVariables(sid); v->name != NULL; ++v){
		memset(name, 0, 64);
		sprintf(name, "%s", v->name);

		if((value = websGetVar(wp, name, NULL))){
//printf("get sid: %s, name: %s, value: %s.\n", GetServiceId(sid), name, value);
			if(!strcmp(v->longname, "Group")){
				;
			}
			else if(strcmp(nvram_safe_get(name), value)){
printf("set sid: %s, name: %s, value: %s(%s).\n", GetServiceId(sid), name, value, nvram_safe_get(name));
				nvram_set_x(GetServiceId(sid), v->name, value);
				
				if(strcmp(name, "x_Setting") && unSetDefault == TRUE)
					nvram_set("x_Setting", "1");
					firsttime = -1; //2009.06 James
				
// 2008.03 James. {
				if(!strcmp(GetServiceId(sid), "General") && !strcmp(v->name, "http_passwd")){
					change_passwd = 1;
					nvram_set("changed_http_passwd", "1");	// to judge if QIS needs authentication.
				}
// 2008.03 James. }
				
// 2007.11 James {
				nvram_modified = 1;
				
				/*if(!strcmp(v->name, "wl_ssid")){
					char buff[100];
					
					memset(buff, 0, 100);
					char_to_ascii(buff, value);
					nvram_set("wl_ssid2", buff);
				}//*/
// 2007.11 James }
				
				if(v->event)
					restart_needed_bits |= v->event;
			}
#ifdef PBCENR
                        if(!strcmp(v->name, "sta_ssid"))
                        {
                                fprintf(stderr, "name: %s, value: %s.\n", name, websGetVar(wp, name, NULL));
                                nvram_set("wl_ssid", value);
                                memset(buff, 0, 100);
                                char_to_ascii(buff, value);
                                nvram_set("wl_ssid2", buff);
                                nvram_set("r_Setting", "1");    // cancel redirect rules
                        }
                        else if(!strcmp(v->name, "sta_auth_mode"))
                        {
                                fprintf(stderr, "name: %s, value: %s.\n", name, websGetVar(wp, name, NULL));
                                nvram_set("wl_auth_mode", value);
                        }
                        else if(!strcmp(v->name, "sta_wep_x"))
                        {
                                fprintf(stderr, "name: %s, value: %s.\n", name, websGetVar(wp, name, NULL));
                                nvram_set("wl_wep_x", value);
                        }
                        else if(!strcmp(v->name, "sta_crypto"))
                        {
                                fprintf(stderr, "name: %s, value: %s.\n", name, websGetVar(wp, name, NULL));
                                nvram_set("wl_crypto", value);
                        }
                        else if(!strcmp(v->name, "sta_wpa_mode"))
                        {
                                fprintf(stderr, "name: %s, value: %s.\n", name, websGetVar(wp, name, NULL));
                                nvram_set("wl_wpa_mode", value);
                        }
                        else if(!strcmp(v->name, "sta_wpa_psk"))
                        {
                                fprintf(stderr, "name: %s, value: %s.\n", name, websGetVar(wp, name, NULL));
                                nvram_set("wl_wpa_psk", value);
                        }
                        else if(!strcmp(v->name, "sta_key"))
                        {
                                fprintf(stderr, "name: %s, value: %s.\n", name, websGetVar(wp, name, NULL));
                                nvram_set("wl_key", value);
                        }
                        else if(!strcmp(v->name, "sta_key_type"))
                        {
                                fprintf(stderr, "name: %s, value: %s.\n", name, websGetVar(wp, name, NULL));
                                nvram_set("wl_key_type", value);
                        }
                        else if(!strcmp(v->name, "sta_key1"))
                        {
                                fprintf(stderr, "name: %s, value: %s.\n", name, websGetVar(wp, name, NULL));
                                nvram_set("wl_key1", value);
                        }
                        else if(!strcmp(v->name, "sta_key2"))
                        {
                                fprintf(stderr, "name: %s, value: %s.\n", name, websGetVar(wp, name, NULL));
                                nvram_set("wl_key2", value);
                        }
                        else if(!strcmp(v->name, "sta_key3"))
                        {
                                fprintf(stderr, "name: %s, value: %s.\n", name, websGetVar(wp, name, NULL));
                                nvram_set("wl_key3", value);
                        }
                        else if(!strcmp(v->name, "sta_key4"))
                        {
                                fprintf(stderr, "name: %s, value: %s.\n", name, websGetVar(wp, name, NULL));
                                nvram_set("wl_key4", value);
                        }
#endif
		}
		
#ifdef REMOVE_WL600
		if((!*value && v->nullok) || !v->validate){
			nvram_set_x(GetServiceId(sid), v->name, value);
		}
		else{
			if(v->validate(value, v) == UPNP_E_SUCCESS)
				nvram_set_x(FindSvcLink(sid), v->name, value);
			else
				websWrite(wp, "Invalid %s = %s\n", v->name, value);
		}
#endif
	}
    
	return nvram_modified;
}

static int update_variables_ex(int eid, webs_t wp, int argc, char_t **argv){
	int sid;
	//char *value;
	char *action_mode;
	//char *current_url;
	//char *next_url;
	char *sid_list;
	//char *value1;
	char *script;
	char groupId[64];
	char *groupName;
	//char *removelist;
	int result;
printf("start update_variables_ex().\n");
	restart_needed_bits = (u64)0;
	restart_tatal_time = 0;
	
	// assign control variables
	action_mode = websGetVar(wp, "action_mode", "");
	//next_host = websGetVar(wp, "next_host", "");
	//current_url = websGetVar(wp, "current_page", "");
	//next_url = websGetVar(wp, "next_page", "");
	script = websGetVar(wp, "action_script", "");
	sid_list = websGetVar(wp, "sid_list", "");
	
	//websWrite(wp, " ");	// for strange web write, must stay
	
	while((serviceId = svc_pop_list(sid_list, ';'))){
		sid = 0;
		while(GetServiceId(sid) != NULL){
			if(!strcmp(GetServiceId(sid), serviceId))
				break;
			
			sid++;
		}
		
		if(serviceId != NULL){
			if(!strcmp(action_mode, "  Save  ") || !strcmp(action_mode, " Apply ")){
				if(!validate_asp_apply(wp, sid, TRUE))
					websWrite(wp, "<script>no_changes_and_no_committing();</script>\n");
				else
					websWrite(wp, "<script>done_committing();</script>\n");
			}
			else if(!strcmp(action_mode, "Update")){
				validate_asp_apply(wp, sid, FALSE);
			}
			else{
				strcpy(groupId, websGetVar(wp, "group_id", ""));
				
				if(strlen(action_mode) > 0){
					groupName = groupId;
					
					validate_asp_apply(wp, sid, FALSE);	// for some nvram with this group.
					if(!strcmp(action_mode, " Add ")){
						restart_needed_bits = (u64)0;
						
						result = apply_cgi_group(wp, sid, NULL, groupName, GROUP_FLAG_ADD);
						
						if(result == 1)
							nvram_set("page_modified", "1");
						
						websWrite(wp, "<script>done_validating(\"%s\");</script>\n", action_mode);
					}
					else if(!strcmp(action_mode, " Del ")){
						restart_needed_bits = (u64)0;
						
						result = apply_cgi_group(wp, sid, NULL, groupName, GROUP_FLAG_REMOVE);
						
						if(result == 1)
							nvram_set("page_modified", "1");
						
						websWrite(wp, "<script>done_validating(\"%s\");</script>\n", action_mode);
					}
					else if(!strcmp(action_mode, " Restart ")){
						struct variable *v;
						
						for(v = GetVariables(sid); v->name != NULL; ++v){
							if(!strcmp(v->name, groupName))
								break;
						}
						printf("--- Restart group %s. ---\n", groupName);
						
						restart_needed_bits |= v->event;
						
						nvram_set("page_modified", "0");
						nvram_set("x_Setting", "1");
						firsttime = -1; //2009.06 James
						
						if(!strcmp(script, "goonsetting")){
							websWrite(wp, "<script>done_validating(\"%s\");</script>\n", action_mode);
							script = "";
						}
						else{
							websWrite(wp, "<script>done_committing();</script>\n");
						}
					}
				}
			}
		}
		
		sid_list = sid_list+strlen(serviceId)+1;
	}
	
	if(strlen(script) > 0){
printf("There is a script!\n");
#ifdef DLM
		if(!strcmp(script, "usbfs_check")){
			nvram_set("st_tool_t", "/bin/fscheck");
			nvram_set("st_toolstatus_t", "USBstarting");
			nvram_set("st_time_t", "3");
			
			websRedirect(wp, "Checking.asp");
			deliver_signal("/var/run/infosvr.pid", SIGUSR1);
			return 0;
		}
		else if(!strcmp(script, "usbfs_eject1"))
			umount_disc_parts(1);
		else if(!strcmp(script, "usbfs_eject2"))
			umount_disc_parts(2);
		else
#endif
#ifdef WSC
		/*if(!strcmp(script, "WPS_config_pin")){
			printf("set WPS be configured!\n");
			
			nvram_set("wsc_method", "1");
			nvram_set("wsc_config_command", "1");
			
			websWrite(wp, "<script>restart_needed_time(6);</script>\n");
		}
		else if(!strcmp(script, "WPS_pin_code")){
			printf("WPS wait the client to join with the pin code!\n");
			
			nvram_set("wsc_sta_pin", websGetVar(wp, "wsc_sta_pin", ""));
			
			nvram_set("wsc_method", "1");
			nvram_set("wsc_config_command", "1");
			
			websWrite(wp, "<script>restart_needed_time(6);</script>\n");
		}//*/
		if(!strcmp(script, "WPS_push_button")){
			printf("WPS wait the client to join with the push button!\n");

			/* Move the action to rc/watchdog.c */
			nvram_set("wps_enable_httpd", "1"); 

#if 0			
			if(nvram_match("wps_waiting", "1") && nvram_match("wps_timer_start", "1")	// unconfig status
					&& nvram_match("wps_method", "1") && nvram_match("wps_config_command", "1"))	// default PIN action
				/* When AP is in the unconfigured AP mode and PIN configuration 
				   is running, PBC can interrupt PIN.*/
				nvram_set("wps_config_command", "2");//Timeout PIN process immediately

			/* If the WPS timeout in defaults, set it as defaults*/
                        if (nvram_match("wps_unconfig_addenr", "1")) {
	                        nvram_set("wps_unconfig_addenr", "");
                                nvram_set("wl_wps_reg", "disabled");
                                nvram_set("wl0_wps_reg", "disabled");
                                nvram_set("wl_wps_config_state", "0");
                                nvram_set("wl0_wps_config_state", "0");
                                nvram_set("wps_config_state", "0");
                        }

                        /* Set variables for WPS te generate configation when AP is in un-configured mode */
                        if (nvram_match("wps_config_state", "0")) {
                                nvram_set("wps_unconfig_addenr", "1");
                                nvram_set("wl_wps_reg", "enabled");
                                nvram_set("wl0_wps_reg", "enabled");
                                nvram_set("wl_wps_config_state", "1");
                                nvram_set("wl0_wps_config_state", "1");
                                nvram_set("wps_config_state", "1");
                        }

                        if (nvram_match("wps_unconfig_addenr", "0")) {
                                nvram_set("is_default", "0");
                                nvram_set("is_modified", "1");
                        }

			/* If pass the asus's setup mode, set it to match the original WPS mode */
                        if (nvram_match("wl_wps_reg", "disabled")) {
	                        nvram_set("wl_wps_config_state", "1");
                                nvram_set("wl0_wps_config_state", "1");
                                nvram_set("wl_wps_reg", "enabled");
                                nvram_set("wl0_wps_reg", "enabled");
                        }


                        /* Use push button to connect client*/
                        nvram_set("wps_method", "2");
                        nvram_set("wps_proc_status", "0");
//			nvram_set("wps_device_pin", nvram_get("secret_code"));
                        nvram_set("wps_sta_pin","00000000");
                        nvram_set("wps_config_command","1");
                        nvram_set("wps_unit", nvram_safe_get("wl_unit"));
                        nvram_set("wps_client_role", "enrollee");
			
#endif			
			websWrite(wp, "<script>restart_needed_time(6);</script>\n");
		}
		else

		if(!strcmp(script, "WPS_PIN_RESET")) {
			eval("stop_wps_proc");
//			nvram_set("wps_config_command", "2");
			nvram_set("wps_config_state", "0");
			nvram_set("wl_wps_config_state", "0");
			nvram_set("wl0_wps_config_state", "0");
//			nvram_set("wl_auth_mode", "open");
//			nvram_set("wl_wep_x", "0");
			nvram_set("wps_proc_status", "0");
			nvram_set("wps_lan_oob","enabled");
			/* WiFi Logo must start WPS after reset OOB */
			eval ("start_wps_proc", "1");
		}
                else if(!strcmp(script, "WPS_unconfig_addenr")){
			printf("START WPS PIN\n");
			eval ("wl", "ssid", nvram_get("wl_ssid"));
			eval ("start_wps_proc", "1");
//                        printf("Set the unconfig addenr\n");
//                        nvram_set("wps_unconfig_addenr", "1");
                }
		else if (!strcmp(script, "RESTART_UPNP")) {
			printf("REFRESH UPNP\n");
			eval ("refresh_upnp");
		}
               else
#endif
		//2008.07.24 Yau add
		if(!strcmp(script, "networkmap_refresh")) {
			if(nvram_match("networkmap_fullscan", "0")){
				FILE *fp = fopen("/tmp/static_ip.inf", "r");
				char buf[256];
				int row = 0;
				
				if(fp != NULL)
					while(fgets(buf, 256, fp) != NULL)
						++row;
				
				websWrite(wp, "<script>restart_needed_time(%d);</script>\n", row*3+5);
				
				//signal to networkmap 
				printf("@@@ Signal to networkmap!!!\n");
				//eval("killall", "-SIGUSR1", "networkmap");
				kill_pidfile_s("/var/run/networkmap.pid", SIGUSR1);
			}
			else
				websWrite(wp, "<script>restart_needed_time(1);</script>\n");
		}
		else //End of Yau add
			sys_script(script);
		
		websWrite(wp, "<script>done_committing();</script>\n");
		
		return 0;
	}
	
	if(restart_needed_bits != 0 && (!strcmp(action_mode, " Apply ") || !strcmp(action_mode, " Restart "))){
		if((restart_needed_bits & RESTART_REBOOT) != 0){
printf("*** set ITVL_RESTART_REBOOT(%d).\n", ITVL_RESTART_REBOOT);
			restart_tatal_time = ITVL_RESTART_REBOOT;
		}
		else if((restart_needed_bits & RESTART_NETWORKING) != 0){
printf("*** set ITVL_RESTART_NETWORKING(%d).\n", ITVL_RESTART_NETWORKING);
			restart_tatal_time = ITVL_RESTART_NETWORKING;
		}
#ifdef CDMA // HSDPA
		else if((restart_needed_bits & RESTART_HSDPA) != 0){
				int restart_seconds = 0;
				
				//if(!strcmp(nvram_safe_get("hsdpa_enable"), "1"))
				if(strcmp(nvram_safe_get("hsdpa_product"), "") != 0)
					restart_seconds = ITVL_RESTART_REBOOT;
				else
					restart_seconds = ITVL_RESTART_HSDPA;
				
printf("*** add ITVL_RESTART_HSDPA(%d).\n", restart_seconds);
				restart_tatal_time += restart_seconds;
			}
#endif // HSDPA
		else{
#ifdef DLM
			if((restart_needed_bits & RESTART_FTPSAMBA) != 0){
printf("*** add ITVL_RESTART_FTPSAMBA(%d).\n", ITVL_RESTART_FTPSAMBA);
				restart_tatal_time += ITVL_RESTART_FTPSAMBA;
			}
#endif
			if((restart_needed_bits & RESTART_DDNS) != 0){
printf("*** add ITVL_RESTART_DDNS(%d).\n", ITVL_RESTART_DDNS);
				restart_tatal_time += ITVL_RESTART_DDNS;
			}
			if((restart_needed_bits & RESTART_HTTPD) != 0){
printf("*** add ITVL_RESTART_HTTPD(%d).\n", ITVL_RESTART_HTTPD);
				restart_tatal_time += ITVL_RESTART_HTTPD;
			}
			if((restart_needed_bits & RESTART_DNS) != 0){
printf("*** add ITVL_RESTART_DNS(%d).\n", ITVL_RESTART_DNS);
				restart_tatal_time += ITVL_RESTART_DNS;
			}
			if((restart_needed_bits & RESTART_DHCPD) != 0){
printf("*** add ITVL_RESTART_DHCPD(%d).\n", ITVL_RESTART_DHCPD);
				restart_tatal_time += ITVL_RESTART_DHCPD;
			}
			if((restart_needed_bits & RESTART_UPNP) != 0){
printf("*** add ITVL_RESTART_UPNP(%d).\n", ITVL_RESTART_UPNP);
				restart_tatal_time += ITVL_RESTART_UPNP;
			}
			if((restart_needed_bits & RESTART_QOS) != 0){
printf("*** add ITVL_RESTART_QOS(%d).\n", ITVL_RESTART_QOS);
				restart_tatal_time += ITVL_RESTART_QOS;
			}
			if((restart_needed_bits & RESTART_SYSLOG) != 0){
printf("*** add ITVL_RESTART_SYSLOG(%d).\n", ITVL_RESTART_SYSLOG);
				restart_tatal_time += ITVL_RESTART_SYSLOG;
			}
			if((restart_needed_bits & RESTART_FIREWALL) != 0){
printf("*** add ITVL_RESTART_FIREWALL(%d).\n", ITVL_RESTART_FIREWALL);
				restart_tatal_time += ITVL_RESTART_FIREWALL;
			}
			if((restart_needed_bits & RESTART_NTPC) != 0){
printf("*** add ITVL_RESTART_NTPC(%d).\n", ITVL_RESTART_NTPC);
				restart_tatal_time += ITVL_RESTART_NTPC;
			}
			if((restart_needed_bits & RESTART_NAS) != 0){
printf("*** add ITVL_RESTART_NAS(%d).\n", ITVL_RESTART_NAS);
				restart_tatal_time += ITVL_RESTART_NAS;
			}
			if((restart_needed_bits & RESTART_TIME) != 0){
printf("*** add ITVL_RESTART_TIME(%d).\n", ITVL_RESTART_TIME);
				restart_tatal_time += ITVL_RESTART_TIME;
			}
#ifdef WSC
			if((restart_needed_bits & RESTART_WPS) != 0){
printf("*** add ITVL_RESTART_WPS(%d).\n", ITVL_RESTART_WPS);
				/*char *wsc_mode = nvram_safe_get("wsc_mode");
				char *old_wsc_mode = nvram_safe_get("old_wsc_mode");
				
				if(nvram_match("wsc_config_state", "0") && nvram_match("wsc_proc_status", "0"))
					restart_tatal_time += ITVL_RESTART_WPS*3;
				else if(strcmp(wsc_mode, old_wsc_mode) != 0 && !strcmp(wsc_mode, "enabled"))
					restart_tatal_time += ITVL_RESTART_WPS*3;
				else*/
					restart_tatal_time += ITVL_RESTART_WPS;
				restart_needed_bits = (unsigned long long)0;
			}
#endif
#ifdef DLM
			if((restart_needed_bits & RESTART_APPS) != 0){
printf("*** add ITVL_RESTART_APPS(%d).\n", ITVL_RESTART_APPS);
				restart_tatal_time += ITVL_RESTART_APPS;
			}
#endif
                        if((restart_needed_bits & RESTART_URE) != 0){
printf("*** add ITVL_RESTART_URE(%d).\n", ITVL_RESTART_URE);
                                restart_tatal_time += ITVL_RESTART_URE;
                       }
		}
		
		if(((restart_needed_bits & RESTART_REBOOT) != 0 || (restart_needed_bits & RESTART_NETWORKING) != 0)
				&& (nvram_match("wan_proto", "pptp") || nvram_match("wan_proto", "l2tp")))
			restart_tatal_time += 4;
		
printf("*** restart_time needs %d seconds.\n", restart_tatal_time);
		websWrite(wp, "<script>restart_needed_time(%d);</script>\n", restart_tatal_time);
	}
	
	return 0;
}

static int convert_asus_variables(int eid, webs_t wp, int argc, char_t **argv){
	if(restart_needed_bits != 0){
printf("*** run convert_asus_variables.\n");
			eval("/sbin/convert_asus_values");
	}
	else
printf("*** Don't run convert_asus_variables.\n");
	
	return 0;
}

static int asus_nvram_commit(int eid, webs_t wp, int argc, char_t **argv){
	if(restart_needed_bits != 0 || nvram_modified == 1){
printf("*** run asus_nvram_commit!\n");
		nvram_modified = 0;
		
		sys_commit();
	}
	else
printf("*** Don't run asus_nvram_commit!\n");
	
	return 0;
}

static int ej_notify_services(int eid, webs_t wp, int argc, char_t **argv){
	int no_run_str = 0;
	
	if(restart_needed_bits != 0){
		no_run_str = 1;
printf("*** run ej_notify_services!\n");
// 2007.11 James {
		if((restart_needed_bits & RESTART_REBOOT) != 0){
/*printf("*** kill wanduck!\n");
			kill_pidfile_s("/var/run/wanduckmain.pid", SIGUSR1);//*/
			
			notify_rc("restart_reboot");
		}
		else if((restart_needed_bits & RESTART_NETWORKING) != 0){
/*printf("*** kill wanduck!\n");
			kill_pidfile_s("/var/run/wanduckmain.pid", SIGUSR1);//*/
			
			notify_rc("restart_networking");
		}
		else{
// 2007.11 James }
#ifdef DLM
			if((restart_needed_bits & RESTART_FTPSAMBA) != 0){
				notify_rc("restart_cifs");
				restart_needed_bits &= ~(u32)RESTART_FTPSAMBA;
			}
#endif
			if((restart_needed_bits & RESTART_DDNS) != 0){
				notify_rc("restart_ddns");
				restart_needed_bits &= ~(u32)RESTART_DDNS;
			}
			if((restart_needed_bits & RESTART_HTTPD) != 0){
				notify_rc("restart_httpd");
				restart_needed_bits &= ~(u32)RESTART_HTTPD;
			}
			if((restart_needed_bits & RESTART_DNS) != 0){
				notify_rc("restart_dns");
				restart_needed_bits &= ~(u32)RESTART_DNS;
			}
			if((restart_needed_bits & RESTART_DHCPD) != 0){
				notify_rc("restart_dhcpd");
				restart_needed_bits &= ~(u32)RESTART_DHCPD;
			}
			if((restart_needed_bits & RESTART_UPNP) != 0){
				notify_rc("restart_upnp");
				restart_needed_bits &= ~(u32)RESTART_UPNP;
			}
			if((restart_needed_bits & RESTART_QOS) != 0){
				notify_rc("restart_qos");
				restart_needed_bits &= ~(u32)RESTART_QOS;
			}
			if((restart_needed_bits & RESTART_SYSLOG) != 0){
				notify_rc("restart_syslog");
				restart_needed_bits &= ~(u32)RESTART_SYSLOG;
			}
			if((restart_needed_bits & RESTART_FIREWALL) != 0){
				notify_rc("restart_firewall");
				restart_needed_bits &= ~(u32)RESTART_FIREWALL;
			}
			if((restart_needed_bits & RESTART_NTPC) != 0){
				notify_rc("restart_ntpc");
				restart_needed_bits &= ~(u32)RESTART_NTPC;
			}
			if((restart_needed_bits & RESTART_NAS) != 0){
				notify_rc("restart_nas");
				restart_needed_bits &= ~(u32)RESTART_NAS;
			}
			if((restart_needed_bits & RESTART_HSDPA) != 0){
				notify_rc("restart_hsdpa");
				restart_needed_bits &= ~(u32)RESTART_HSDPA;
			}
// 2008.01 James {
			if((restart_needed_bits & RESTART_TIME) != 0){
				notify_rc("restart_time");
				restart_needed_bits &= ~(u32)RESTART_TIME;
			}
#ifdef WSC
			if((restart_needed_bits & RESTART_WPS) != 0){
				notify_rc("restart_wps");
				restart_needed_bits &= ~(u32)RESTART_WPS;
			}
#endif
#ifdef DLM
			if((restart_needed_bits & RESTART_APPS) != 0){
				notify_rc("restart_apps");
				restart_needed_bits &= ~(u32)RESTART_APPS;
			}
#endif
// 2008.01 James }
		}
	}
	
	if(no_run_str == 0)
printf("*** Don't run ej_notify_services!\n");
	
	return 0;
}

// for error_page.htm's detection
static int detect_if_wan(int eid, webs_t wp, int argc, char_t **argv){
	int if_wan = is_phyconnected();
	
	websWrite(wp, "%d", if_wan);
	
	return 0;
}

static int detect_wan_connection(int eid, webs_t wp, int argc, char_t **argv){
	int MAX_LOOKUP_NUM = 3, lookup_num;
	
	for(lookup_num = 0; lookup_num < MAX_LOOKUP_NUM; ++lookup_num){
		if(nvram_match("ntp_ready", "1")){
			websWrite(wp, "1");	// can go out of LAN.
			
			break;
		}
		
		if(lookup_num == MAX_LOOKUP_NUM-1){
			cprintf("Can't get the host from ntp!\n");
			websWrite(wp, "-1");	// can not go out of LAN.
			
			break;
		}
		
		cprintf("Sleep 3 seconds...\n");
		sleep(3);
	}
	
	return 0;
}

static int detect_dhcp_pppoe(int eid, webs_t wp, int argc, char_t **argv){
	int ret;
	
	if(nvram_match("wan_nat_x", "1"))	// Gateway mode
		;
	else if(nvram_match("wan_route_x", "IP_Routed"))	// Router mode
		;
	else{	// AP mode
		websWrite(wp, "AP mode");
		return 0;
	}
	
	if(nvram_match("wan_status_t", "Connected")){
#ifdef CDMA // HSDPA
		if(!nvram_match("hsdpa_product", ""))
			websWrite(wp, "3.5G HSDPA");
		else
#endif // HSDPA
			websWrite(wp, "%s", nvram_safe_get("wan_proto"));
	}
	else{
		ret = discover_all();
		
		if(ret == 0)
			websWrite(wp, "no-respond");
		else if(ret == 1)
			websWrite(wp, "dhcp");
		else if(ret == 2)
			websWrite(wp, "pppoe");
		else  // -1
			websWrite(wp, "error");
	}
	
	return 0;
}

static int get_wan_status_log(int eid, webs_t wp, int argc, char_t **argv){
	FILE *fp;
	char log_info[64];
	int i;
	
	memset(log_info, 0, 64);
	
	fp = fopen("/tmp/wanstatus.log", "r");
	if(fp != NULL){
		fgets(log_info, 64, fp);
		
		i = 0;
		while(log_info[i] != 0){
			if(log_info[i] == '\n'){
				log_info[i] = 0;
				break;
			}
			
			++i;
		}
		
		websWrite(wp, "%s", log_info);
		fclose(fp);
	}
	
	return 0;
}

#ifndef FIXED_PPP_DETECT
int get_ppp_pid(char *conntype){
	int pid = -1;
	char tmp[80], tmp1[80];
	
	snprintf(tmp, sizeof(tmp), "/var/run/%s.pid", conntype);
	file_to_buf(tmp, tmp1, sizeof(tmp1));
	pid = atoi(tmp1);
	
	return pid;
}

/* Find process name by pid from /proc directory */
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
#else // FIXED_PPP_DETECT
int check_ppp_exist(){
	DIR *dir;
	struct dirent *dent;
	char task_file[64], cmdline[64];
	int pid, fd;
	
	if((dir = opendir("/proc")) == NULL){
		perror("open proc");
		return -1;
	}
	
	while((dent = readdir(dir)) != NULL){
		if((pid = atoi(dent->d_name)) > 1){
			memset(task_file, 0, 64);
			sprintf(task_file, "/proc/%d/cmdline", pid);
			if((fd = open(task_file, O_RDONLY)) > 0){
				memset(cmdline, 0, 64);
				read(fd, cmdline, 64);
				close(fd);
				
				if(strstr(cmdline, "pppoecd")
						|| strstr(cmdline, "pppd")
						){
					closedir(dir);
					return 0;
				}
			}
			else
				printf("cannot open %s\n", task_file);
		}
	}
	closedir(dir);
	
	return -1;
}
#endif // FIXED_PPP_DETECT

int check_subnet(){
	char lan_ipaddr_t[16];
	char lan_netmask_t[16];
	static unsigned int lan_subnet = 0;
	char wan_ipaddr_t[16];
	char wan_netmask_t[16];
	static unsigned int wan_subnet = 0;
	
	if(lan_subnet == 0){
		memset(lan_ipaddr_t, 0, 16);
		strcpy(lan_ipaddr_t, nvram_safe_get("lan_ipaddr_t"));
		memset(lan_netmask_t, 0, 16);
		strcpy(lan_netmask_t, nvram_safe_get("lan_netmask_t"));
		lan_subnet = inet_network(lan_ipaddr_t)&inet_network(lan_netmask_t);
printf("httpd get lan_subnet=%x!\n", lan_subnet);
	}
	
	if(wan_subnet == 0){
		memset(wan_ipaddr_t, 0, 16);
		strcpy(wan_ipaddr_t, nvram_safe_get("wan_ipaddr_t"));
		memset(wan_netmask_t, 0, 16);
		strcpy(wan_netmask_t, nvram_safe_get("wan_netmask_t"));
		wan_subnet = inet_network(wan_ipaddr_t)&inet_network(wan_netmask_t);
printf("httpd get wan_subnet=%x!\n", wan_subnet);
	}
	
	return (lan_subnet == wan_subnet);
}

static int wanlink_hook(int eid, webs_t wp, int argc, char_t **argv){
#ifndef FIXED_PPP_DETECT
	FILE *fp;
#endif
	char type[32], ip[32], netmask[32], gateway[32], dns[128], statusstr[32];
	int status = 0, unit, s;
	char tmp[100], prefix[] = "wanXXXXXXXXXX_";
#ifndef FIXED_PPP_DETECT
	char filename[80], conntype[10];
#endif
	struct ifreq ifr;
	struct sockaddr_in *our_ip;
	struct in_addr in;
	char *pwanip = NULL;
	DIR *ppp_dir;	// 2008.01 James.
	struct dirent *entry;	// 2008.01 James.
	int got_ppp_link;
	
	/* current unit */
	if((unit = atoi(nvram_safe_get("wan_unit"))) < 0)
		unit = 0;
	wan_prefix(unit, prefix);
	
	if(!is_phyconnected()
#ifdef CDMA // HSDPA {
			&& !nvram_match("hsdpa_product", "")
#endif // HSDPA }
			){
		status = 0;
		strcpy(statusstr, "Cable is not attached");
	}
// 2008.07 James. {
	else if(!strcmp(nvram_safe_get("manually_disconnect_wan"), "1")){
		status = 0;
		strcpy(statusstr, "Disconnected");
	}
// 2008.07 James. }
	else if(nvram_match(strcat_r(prefix, "proto", tmp), "pppoe")
			|| nvram_match(strcat_r(prefix, "proto", tmp), "pptp")
			|| nvram_match(strcat_r(prefix, "proto", tmp), "l2tp")
			)
	{
#ifndef FIXED_PPP_DETECT
		//char conntype[10];
		//char buf1[256];	// 2008.01 James.
		char *name;
		char* pos = NULL;
		char* cpos = NULL;
		
// 2008.01 James {
		//eval("ls /tmp/ppp > /tmp/ppp/filelist");
				
		/*if((fp = fopen("/tmp/ppp/filelist", "r") != NULL)){
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
			
			fclose(fp);	// 2007.11 James. fclose() is right here.
		}
		else{
			status = 0;
			strcpy(statusstr, "Disconnected");
		}
		
		//wendebug
		//printf("filename : %s\n", filename);
		//fclose(fp);	// 2007.11 James. fclose() is wrong here.
		
		unlink("/tmp/ppp/filelist");*/
		
		memset(filename, 0, 80);
		
		ppp_dir = opendir("/tmp/ppp");
		if(ppp_dir != NULL){
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
		else{
			status = 0;
			strcpy(statusstr, "Disconnected");
		}
// 2008.01 James }
		
// 2007.10 James {
		//if((fp = fopen(filename, "r"))){
		if(strlen(filename) > 0 && (fp = fopen(filename, "r")) != NULL){
// 2007.10 James }
			int pid = -1;
			fclose(fp);
			
			if(nvram_match(strcat_r(prefix, "proto", tmp), "heartbeat")){
				char buf[20];
				
				file_to_buf(filename, buf, sizeof(buf));
				pid = atoi(buf);
			}
			else
				pid = get_ppp_pid(conntype);
			                                                                         
			name = find_name_by_proc(pid);  
			
			if(!strncmp(name, "pppoecd", 7) ||	// for PPPoE
					!strncmp(name, "pppd", 4) ||	// for PPTP
					!strncmp(name, "bpalogin", 8)){	// for HeartBeat
				status = 1;
				strcpy(statusstr, "Connected");
			}
			else{
				status = 0;
				strcpy(statusstr, "Disconnected");
				// For some reason, the pppoed had been died, by link file still exist.
				unlink(filename);
			}
		}
		else{
			//wendebug
			//printf("filename : open fail\n");	             
			status = 0;
			strcpy(statusstr, "Disconnected"); 
		}
#else // FIXED_PPP_DETECT
		if((ppp_dir = opendir("/tmp/ppp")) == NULL){
			status = 0;
			strcpy(statusstr, "Disconnected");
		}
		else{
			got_ppp_link = 0;
			while((entry = readdir(ppp_dir)) != NULL){
				if(!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, ".."))
					continue;
				
				if(strstr(entry->d_name, "link") != NULL){
					got_ppp_link = 1;
					
					break;
				}
			}
			closedir(ppp_dir);
			
			if(got_ppp_link == 0){
				status = 0;
				strcpy(statusstr, "Disconnected");
			}
			else if(check_ppp_exist() == -1){
				status = 0;
				strcpy(statusstr, "Disconnected");
			}
			else{
				status = 1;
				strcpy(statusstr, "Connected");
			}
		}
#endif // FIXED_PPP_DETECT
	}
#ifdef CDMA // HSDPA
	else if(!nvram_match("hsdpa_product", "")){
		if((ppp_dir = opendir("/tmp/ppp")) == NULL){
			status = 0;
			strcpy(statusstr, "Disconnected");
		}
		else{
			got_ppp_link = 0;
			while((entry = readdir(ppp_dir)) != NULL){
				if(!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, ".."))
					continue;
				
				if(strstr(entry->d_name, "link") != NULL){
					got_ppp_link = 1;
					
					break;
				}
			}
			closedir(ppp_dir);
			
			if(got_ppp_link == 0){
				status = 0;
				strcpy(statusstr, "Disconnected");
			}
			else if(check_ppp_exist() == -1){
				status = 0;
				strcpy(statusstr, "Disconnected");
			}
			else{
				status = 1;
				strcpy(statusstr, "Connected");
			}
		}
	}
#endif // CDMA
	else {
// 2009.05 James. {
		//if(!strcmp(nvram_safe_get("wan_gateway_t"), nvram_safe_get("lan_ipaddr_t"))){
		if(check_subnet()){
			status = 0;
			strcpy(statusstr, "Disconnected");
		}
		else
// 2009.05 James. }
		/* Open socket to kernel */
		if((s = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
			status = 0;
			strcpy(statusstr, "Disconnected");
		}
		else{
			/* Check for valid IP address */
			strncpy(ifr.ifr_name, nvram_safe_get(strcat_r(prefix, "ifname", tmp)), IFNAMSIZ);
			
			if(!ioctl(s, SIOCGIFADDR, &ifr)){
				our_ip = (struct sockaddr_in *)&ifr.ifr_addr;
				in.s_addr = our_ip->sin_addr.s_addr;
				pwanip = inet_ntoa(in);
				
				if(pwanip == NULL || strlen(pwanip) <= 0){
					status = 0;
					strcpy(statusstr, "Disconnected");
				}
				else if(!strcmp(pwanip, nvram_safe_get("wan_gateway_t"))){
					status = 0;
					strcpy(statusstr, "Disconnected");
				}
				else{
					status = 1;
					strcpy(statusstr, "Connected");
				}
			}
			else{
				status = 0;
				strcpy(statusstr, "Disconnected");
			}
			
			close(s);
		}
	}
	
#ifdef CDMA // HSDPA {
	if(!nvram_match("hsdpa_product", ""))
		strcpy(type, "3.5G HSDPA");
	else
#endif // HSDPA }
	if(nvram_match(strcat_r(prefix, "proto", tmp), "pppoe"))
		strcpy(type, "PPPOE");
	else if(nvram_match(strcat_r(prefix, "proto", tmp), "pptp"))
		strcpy(type, "PPTP");
	else if(nvram_match(strcat_r(prefix, "proto", tmp), "l2tp"))
		strcpy(type, "L2TP");
	else if(nvram_match(strcat_r(prefix, "proto", tmp), "static"))
		strcpy(type, "Static IP");
	else // dhcp
		strcpy(type, "Automatic IP");
	
	strcpy(ip, nvram_safe_get(strcat_r(prefix, "ipaddr", tmp)));
	strcpy(netmask, nvram_safe_get(strcat_r(prefix, "netmask", tmp)));
	strcpy(gateway, nvram_safe_get(strcat_r(prefix, "gateway", tmp)));	
	
// 2007.08 James {
	memset(dns, 0, 128);
	if(nvram_match(strcat_r(prefix, "dnsenable_x", tmp), "1"))
	{
		strcpy(dns, nvram_safe_get(strcat_r(prefix, "dns", tmp)));
	}
	else
	{
		strcpy(dns, nvram_safe_get("wan_dns_t"));
	}
// 2007.08 James }
	
	websWrite(wp, "function wanlink_status() { return %d;}\n", status);
	websWrite(wp, "function wanlink_statusstr() { return '%s';}\n", statusstr);
	websWrite(wp, "function wanlink_type() { return '%s';}\n", type);
	websWrite(wp, "function wanlink_ipaddr() { return '%s';}\n", ip);
	websWrite(wp, "function wanlink_netmask() { return '%s';}\n", netmask);
	websWrite(wp, "function wanlink_gateway() { return '%s';}\n", gateway);
	websWrite(wp, "function wanlink_dns() { return '%s';}\n", dns);

	return 0;
}

static int wan_action_hook(int eid, webs_t wp, int argc, char_t **argv){
	char *action;
	int needed_seconds = 0;
	
	// assign control variables
	action = websGetVar(wp, "wanaction", "");
	if(strlen(action) <= 0){
		fprintf(stderr, "No connect action in wan_action_hook!\n");
		return -1;
	}
	
	fprintf(stderr, "wan action: %s\n", action);
	
	if(!strcmp(action, "Connect")){
		nvram_unset("manually_disconnect_wan");	// 2008.01 James.
		nvram_set("rc_service", "wan_connect");
		needed_seconds = 5;	// WL500-series
//printf("kill -USR1 1.\n");
		kill(1, SIGUSR1);
//printf("sleep 1 second.\n");
		sleep(1);
	}
	else if(!strcmp(action, "Disconnect")){
		nvram_set("manually_disconnect_wan", "1");	// 2008.01 James.
		nvram_set("rc_service", "wan_disconnect");
		needed_seconds = 5;	// WL500-series
//printf("kill -USR1 1.\n");
		kill(1, SIGUSR1);
//printf("sleep 1 second.\n");
		sleep(1);
	}
	
	websWrite(wp, "<script>restart_needed_time(%d);</script>\n", needed_seconds);
	
	return 0;
}

static int ej_get_parameter(int eid, webs_t wp, int argc, char_t **argv){
	//char *c;
	bool last_was_escaped = FALSE;
	//int ret = 0;
	
	if(argc < 1){
		websError(wp, 400,
				"get_parameter() used with no arguments, but at least one "
				"argument is required to specify the parameter name\n");
		return -1;
	}
	
	last_was_escaped = FALSE;
	
	char *value = websGetVar(wp, argv[0], "");
	websWrite(wp, "%s", value);
	/*for(c = websGetVar(wp, argv[0], ""); *c; c++){
		if(isprint((int)*c) &&
			*c != '"' && *c != '&' && *c != '<' && *c != '>' && *c != '\\' &&
			((!last_was_escaped) || !isdigit(*c)))
		{
			ret += websWrite(wp, "%c", *c);
			last_was_escaped = FALSE;
		}
		else if((*c & 0x80) != 0)
		{
			ret += websWrite(wp, "%c", *c);
			last_was_escaped = FALSE;
		}
		else
		{
			ret += websWrite(wp, "&#%d", *c);
			last_was_escaped = TRUE;
		}
	}//*/
	
	return 0;
}

unsigned int getpeerip(webs_t wp){
	int fd, ret, len = sizeof(struct sockaddr_in);
	struct sockaddr peer;
	socklen_t peerlen;
	struct sockaddr_in *sa = (struct sockaddr_in *)malloc(len);
	
	if(sa == NULL)
		return 0;
	memset(sa, 0, len);
	
	//fd = fileno((FILE *)wp);
	fd = conn_fd;
	peerlen = sizeof(struct sockaddr);	// 2009.05 James. must initial the peerlen for getpeername().
	ret = getpeername(fd, (struct sockaddr *)&peer, &peerlen);
	memcpy(sa, &peer, peerlen);
	
	if(!ret){
		//printf("peer: %x\n", sa->sin_addr.s_addr);
		return (unsigned int)sa->sin_addr.s_addr;
	}
	else{
		//printf("error: %d %d", ret, errno);
		return 0;
	}
}

static int login_state_hook(int eid, webs_t wp, int argc, char_t **argv){
	//unsigned int ip, login_ip;
	unsigned int ip;
	char ip_str[16], login_ip_str[16];
	//unsigned long login_timestamp;
	struct in_addr now_ip_addr, login_ip_addr;
	time_t now;
	const int MAX = 80;
	const int VALUELEN = 18;
	char buffer[MAX], values[6][VALUELEN];
	
	//ip = getpeerip(wp);
	ip = login_ip_tmp;
	
	now_ip_addr.s_addr = ip;
	memset(ip_str, 0, 16);
	strcpy(ip_str, inet_ntoa(now_ip_addr));
	time(&now);
	
	//login_ip = (unsigned int)atoll(nvram_safe_get("login_ip"));
	login_ip_addr.s_addr = login_ip;
	memset(login_ip_str, 0, 16);
	strcpy(login_ip_str, inet_ntoa(login_ip_addr));
	//login_timestamp = (unsigned long)atol(nvram_safe_get("login_timestamp"));
	
	memset(buffer, 0, MAX);
	memset(values, 0, 6*VALUELEN);
	
	FILE *fp = fopen("/proc/net/arp", "r");
	if(fp){
		while(fgets(buffer, MAX, fp)){
			if(strstr(buffer, "br0") && !strstr(buffer, "00:00:00:00:00:00")){
				if(sscanf(buffer, "%s%s%s%s%s%s", values[0], values[1], values[2], values[3], values[4], values[5]) == 6){
					if(!strcmp(values[0], ip_str)){
						break;
					}
				}
				
				memset(values, 0, 6*VALUELEN);
			}
			
			memset(buffer, 0, MAX);
		}
		
		fclose(fp);
	}
	
	if(ip != 0 && login_ip == ip){
		websWrite(wp, "function is_logined() { return 1; }\n");
		websWrite(wp, "function login_ip_dec() { return '%u'; }\n", login_ip);
		websWrite(wp, "function login_ip_str() { return '%s'; }\n", login_ip_str);
		
		websWrite(wp, "function login_ip_str_now() { return '%s'; }\n", ip_str);
		
		if(values[3] != NULL)
			websWrite(wp, "function login_mac_str() { return '%s'; }\n", values[3]);
		else
			websWrite(wp, "function login_mac_str() { return ''; }\n");
		//time(&login_timestamp);
	}
	else{
		websWrite(wp, "function is_logined() { return 0; }\n");
		websWrite(wp, "function login_ip_dec() { return '%u'; }\n", login_ip);
		
		//if((unsigned long)(now-login_timestamp) > 60)	//one minitues
			websWrite(wp, "function login_ip_str() { return '0.0.0.0'; }\n");
		/*else */
			websWrite(wp, "function login_ip_str() { return '%s'; }\n", login_ip_str);
		
		websWrite(wp, "function login_ip_str_now() { return '%s'; }\n", ip_str);
		
		if(values[3] != NULL)
			websWrite(wp, "function login_mac_str() { return '%s'; }\n", values[3]);
		else
			websWrite(wp, "function login_mac_str() { return ''; }\n");
	}
	
	return 0;
}

static int ej_get_nvram_list(int eid, webs_t wp, int argc, char_t **argv){
	struct variable *v, *gv;
	char buf[MAX_LINE_SIZE+MAX_LINE_SIZE];
	char *serviceId, *groupName;
	int i, groupCount, sid;
	int firstRow, firstItem;
	
	if(argc != 2)
		return 0;
	
	serviceId = argv[0];
	groupName = argv[1];
	
	sid = LookupServiceId(serviceId);
	if(sid == -1)
		return 0;
	
	/* Validate and set vairables in table order */
	for(v = GetVariables(sid); v->name != NULL; ++v)
		if(!strcmp(groupName, v->name))
			break;
	if(v->name == NULL)
		return 0;
	
	groupCount = atoi(nvram_safe_get_x(serviceId, v->argv[3]));
	
	firstRow = 1;
	for(i = 0; i < groupCount; ++i){
		if(firstRow == 1)
			firstRow = 0;
		else
			websWrite(wp, ", ");
		websWrite(wp, "[");
		
		firstItem = 1;
		for(gv = (struct variable *)v->argv[0]; gv->name != NULL; ++gv){
			if(firstItem == 1)
				firstItem = 0;
			else
				websWrite(wp, ", ");
			
			memset(buf, 0, sizeof(buf));
			sprintf(buf, "\"%s\"", nvram_get_list_x(serviceId, gv->name, i));
			websWrite(wp, "%s", buf);
		}
		
		websWrite(wp, "]");
	}
	
	return 0;
}

static int ej_dhcp_leases(int eid, webs_t wp, int argc, char_t **argv){
	FILE *fp = NULL;
	struct lease_t lease;
	struct in_addr addr;
	unsigned long expires = 0;
	int i, firstRow;
	char str[80], buf[80];
	
	sys_script("leases.sh");
	
	/* Write out leases file */
	if(!(fp = fopen("/tmp/udhcpd-br0.leases", "r")))
		return 0;
	
	firstRow = 1;
	while(fread(&lease, sizeof(lease), 1, fp)){
		/* Do not display reserved leases */
		if(ETHER_ISNULLADDR(lease.chaddr))
			continue;
		
		if(firstRow == 1)
			firstRow = 0;
		else
			websWrite(wp, ", ");
		websWrite(wp, "[");
		
		memset(str, 0, 80);
		memset(buf, 0, 80);
		sprintf(buf, "%-16s", lease.hostname);
		sscanf(buf, "%s", str);
		websWrite(wp, "\"%s\", ", str);
		
		websWrite(wp, "\"");
		for(i = 0; i < 6; ++i){
			websWrite(wp, "%02X", lease.chaddr[i]);
			if(i != 5)
				websWrite(wp, ":");
		}
		websWrite(wp, "\", ");
		
		addr.s_addr = lease.yiaddr;
		memset(str, 0, 80);
		memset(buf, 0, 80);
		sprintf(buf, "%-15s", inet_ntoa(addr));
		sscanf(buf, "%s", str);
		websWrite(wp, "\"%s\", ", str);
		expires = ntohl(lease.expires);
		
		if(expires == 0xffffffff)
			websWrite(wp, "\"Manual\"");
		else if(!expires)
			websWrite(wp, "\"Expired\"");
		else{
			memset(str, 0, 80);
			memset(buf, 0, 80);
			sprintf(buf, "%-15s", reltime(expires));
			sscanf(buf, "%s", str);
			websWrite(wp, "\"%s\"", str);
		}
		
		websWrite(wp, "]");
	}
	
	fclose(fp);
	
	return 0;
}

static int ej_get_arp_table(int eid, webs_t wp, int argc, char_t **argv){
	const int MAX = 80;
	const int FIELD_NUM = 6;
	const int VALUELEN = 18;
	char buffer[MAX], values[FIELD_NUM][VALUELEN];
	int num, firstRow;
	
	FILE *fp = fopen("/proc/net/arp", "r");
	if(fp){
		memset(buffer, 0, MAX);
		memset(values, 0, FIELD_NUM*VALUELEN);
		
		firstRow = 1;
		while(fgets(buffer, MAX, fp)){
			if(strstr(buffer, "br0") && !strstr(buffer, "00:00:00:00:00:00")){
				if(firstRow == 1)
					firstRow = 0;
				else
					websWrite(wp, ", ");
				
				if((num = sscanf(buffer, "%s%s%s%s%s%s", values[0], values[1], values[2], values[3], values[4], values[5])) == FIELD_NUM){
					websWrite(wp, "[\"%s\", \"%s\", \"%s\", \"%s\", \"%s\", \"%s\"]", values[0], values[1], values[2], values[3], values[4], values[5]);
				}
				
				memset(values, 0, FIELD_NUM*VALUELEN);
			}
			
			memset(buffer, 0, MAX);
		}
		
		fclose(fp);
	}
	
	return 0;
}

static int ej_get_arl_table(int eid, webs_t wp, int argc, char_t **argv){
	const int MAX = 80;
	const int FIELD_NUM = 4;
	const int VALUELEN = 18;
	char buffer[MAX], values[FIELD_NUM][VALUELEN];
	int num, firstRow;
	
	FILE *fp = fopen("/proc/net/arl", "r");
	if(fp && fgets(buffer, MAX, fp) != NULL){
		memset(buffer, 0, MAX);
		memset(values, 0, FIELD_NUM*VALUELEN);
		
		firstRow = 1;
		while(fgets(buffer, MAX, fp)){
			if((num = sscanf(buffer, "%s%s%s%s", values[0], values[1], values[2], values[3])) == FIELD_NUM){
				if(values[1][0] != '0' || values[1][1]-'0' < 0 || values[1][1]-'0' > 3){
					memset(values, 0, FIELD_NUM*VALUELEN);
					memset(buffer, 0, MAX);
					
					continue;
				}
				
				if(firstRow == 1)
					firstRow = 0;
				else
					websWrite(wp, ", ");
				
				websWrite(wp, "[\"%s\", \"%s\", \"%s\", \"%s\"]", values[0], values[1], values[2], values[3]);
			}
			
			memset(values, 0, FIELD_NUM*VALUELEN);
			memset(buffer, 0, MAX);
		}
		
		fclose(fp);
	}
	
	return 0;
}

// for detect static IP's client.
static int ej_get_static_client(int eid, webs_t wp, int argc, char_t **argv){
	FILE *fp = fopen("/tmp/static_ip.inf", "r");
	char buf[1024], *head, *tail, field[1024];
	int len, i, first_client, first_field;
	
	if(fp == NULL){
		//printf("Don't detect static clients!\n");
		return 0;
	}
	
	memset(buf, 0, 1024);
	
	first_client = 1;
	while(fgets(buf, 1024, fp)){
		if(first_client == 1)
			first_client = 0;
		else
			websWrite(wp, ", ");
		
		len = strlen(buf);
		buf[len-1] = ',';
		head = buf;
		first_field = 1;
		for(i = 0; i < 7; ++i){
			tail = strchr(head, ',');
			if(tail != NULL){
				memset(field, 0, 1024);
				strncpy(field, head, (tail-head));
			}
			
			if(first_field == 1){
				first_field = 0;
				websWrite(wp, "[");
			}
			else
				websWrite(wp, ", ");
			
			if(strlen(field) > 0)
				websWrite(wp, "\"%s\"", field);
			else
				websWrite(wp, "null");
			
			//if(tail+1 != NULL)
				head = tail+1;
			
			if(i == 6)
				websWrite(wp, "]");
		}
		
		memset(buf, 0, 1024);
	}
	
	fclose(fp);
	return 0;
}

static int ej_get_changed_status(int eid, webs_t wp, int argc, char_t **argv){
	char *arp_info = read_whole_file("/proc/net/arp");
	u32 arp_info_len;
	//u32 arp_change;
#ifdef DLM
	char *disk_info = read_whole_file(PARTITION_FILE);
	char *mount_info = read_whole_file("/proc/mounts");
	u32 disk_info_len, mount_info_len;
	//u32 disk_change;
#endif
	
	if(arp_info != NULL){
		arp_info_len = strlen(arp_info);
		free(arp_info);
	}
	else
		arp_info_len = 0;
	
#ifdef DLM
	if(disk_info != NULL){
		disk_info_len = strlen(disk_info);
		free(disk_info);
	}
	else
		disk_info_len = 0;
	
	if(mount_info != NULL){
		mount_info_len = strlen(mount_info);
		free(mount_info);
	}
	else
		mount_info_len = 0;
#endif
	
	websWrite(wp, "function get_client_status_changed(){\n");
	websWrite(wp, "    return %u;\n", arp_info_len);
	websWrite(wp, "}\n\n");
	
	websWrite(wp, "function get_disk_status_changed(){\n");
#ifdef DLM
	websWrite(wp, "    return %u;\n", disk_info_len);
#else
	websWrite(wp, "    return 0;\n");
#endif
	websWrite(wp, "}\n\n");
	
	websWrite(wp, "function get_mount_status_changed(){\n");
#ifdef DLM
	websWrite(wp, "    return %u;\n", mount_info_len);
#else
	websWrite(wp, "    return 0;\n");
#endif
	websWrite(wp, "}\n\n");
	
	return 0;
}

static bool find_ethaddr_in_list(void *ethaddr, struct maclist *list){
	int i;
	
	for(i = 0; i < list->count; ++i)
		if(!bcmp(ethaddr, (void *)&list->ea[i], ETHER_ADDR_LEN))
			return TRUE;
	
	return FALSE;
}

// no WME in WL500gP V2
// MAC/associated/authorized
static int ej_wl_auth_list(int eid, webs_t wp, int argc, char_t **argv){
	int unit;
	char tmp[100], prefix[] = "wlXXXXXXXXXX_";
	char *name;
	struct maclist *auth, *assoc, *authorized;
	struct maclist *wme;
	int max_sta_count, maclist_size;
	int i, firstRow;
	
	if((unit = atoi(nvram_safe_get("wl_unit"))) < 0)
		return -1;
	
	snprintf(prefix, sizeof(prefix), "wl%d_", unit);
	name = nvram_safe_get(strcat_r(prefix, "ifname", tmp));
	
	/* buffers and length */
	max_sta_count = 256;
	maclist_size = sizeof(auth->count)+max_sta_count*sizeof(struct ether_addr);
	
	auth = malloc(maclist_size);
	assoc = malloc(maclist_size);
	authorized = malloc(maclist_size);
	//wme = malloc(maclist_size);
	
	//if(!auth || !assoc || !authorized || !wme)
	if(!auth || !assoc || !authorized)
		goto exit;
	
	/* query wl for authenticated sta list */
	strcpy((char*)auth, "authe_sta_list");
	if (wl_ioctl(name, WLC_GET_VAR, auth, maclist_size))
		goto exit;
	
	/* query wl for associated sta list */
	assoc->count = max_sta_count;
	if (wl_ioctl(name, WLC_GET_ASSOCLIST, assoc, maclist_size))
		goto exit;
	
	/* query wl for authorized sta list */
	strcpy((char*)authorized, "autho_sta_list");
	if (wl_ioctl(name, WLC_GET_VAR, authorized, maclist_size))
		goto exit;
	
	/* query wl for WME sta list */
	/*strcpy((char*)wme, "wme_sta_list");
	if (wl_ioctl(name, WLC_GET_VAR, wme, maclist_size))
		goto exit;*/
	
	/* build authenticated/associated/authorized sta list */
	firstRow = 1;
	for(i = 0; i < auth->count; ++i){
		char ea[ETHER_ADDR_STR_LEN];
		char *value;
		
		if(firstRow == 1)
			firstRow = 0;
		else
			websWrite(wp, ", ");
		websWrite(wp, "[");
		
		websWrite(wp, "\"%s\"", ether_etoa((void *)&auth->ea[i], ea));
		
		value = (find_ethaddr_in_list((void *)&auth->ea[i], assoc))?"Yes":"No";
		websWrite(wp, ", \"%s\"", value);
		
		value = (find_ethaddr_in_list((void *)&auth->ea[i], authorized))?"Yes":"No";
		websWrite(wp, ", \"%s\"", value);
		
		/*value = (find_ethaddr_in_list((void *)&auth->ea[i], wme))?"Yes":"No";
		websWrite(wp, ", \"%s\"", value);*/
		
		websWrite(wp, "]");
	}
	
	/* error/exit */
exit:
	if(auth) free(auth);
	if(assoc) free(assoc);
	if(authorized) free(authorized);
	//if(wme) free(wme);
	
	return 0;
}

static int ej_get_printer_info(int eid, webs_t wp, int argc, char_t **argv){
	FILE *lpfp;
	char manufacturer[100], models[100], serialnos[100], pool[100];
	char buf[500];
	char *lpparam, *value, *v1 = NULL;
	
	if(!(lpfp = fopen("/proc/usblp/usblpid", "r"))){
		strcpy(manufacturer, "");
		strcpy(models, "");
		strcpy(serialnos, "");
		strcpy(pool, "");
	}
	else{
		while(fgets(buf, 500, lpfp)){
			value = &buf[0];
			lpparam = strsep(&value, "=")?:&buf[0];
			
			if(value){
				v1 = strchr(value, '\n');
				*v1 = '\0';
				
				if(!strcmp(lpparam, "Manufacturer"))
					sprintf(manufacturer, "\"%s\"", value);
				else if(!strcmp(lpparam, "Model"))
					sprintf(models, "\"%s\"", value );
			}
		}
		
		//compatible for WL700gE platform
		strcpy(pool, "VirtualPool");
		fclose(lpfp);
	}
	
	websWrite(wp, "function printer_manufacturers() {\n return [%s];\n}\n", manufacturer);
	websWrite(wp, "function printer_models() {\n return [%s];\n}\n", models);
	websWrite(wp, "function printer_serialn() {\n return [%s];\n}\n", "");
	websWrite(wp, "function printer_pool() {\n return [\"%s\"];\n}\n", pool);
	
	return 0;
}

#ifdef DLM
static int ej_disk_pool_mapping_info(int eid, webs_t wp, int argc, char_t **argv){
	disk_info_t *disks_info, *follow_disk;
	partition_info_t *follow_partition;
	int first;
	
	//disks_info = read_disk_data();
	if(DISKS_INFO == NULL)
		DISKS_INFO = read_disk_data();
	disks_info = DISKS_INFO;
	if(disks_info == NULL){
		websWrite(wp, "%s", initial_disk_pool_mapping_info());
		return -1;
	}
	
	websWrite(wp, "function total_disk_sizes(){\n");
	websWrite(wp, "    return [");
	first = 1;
	for(follow_disk = disks_info; follow_disk != NULL; follow_disk = follow_disk->next){
		if(first == 1)
			first = 0;
		else
			websWrite(wp, ", ");
		
		websWrite(wp, "\"%llu\"", follow_disk->size_in_kilobytes);
	}
	websWrite(wp, "];\n");
	websWrite(wp, "}\n\n");
	
	websWrite(wp, "function disk_interface_names(){\n");
	websWrite(wp, "    return [");
	first = 1;
	for(follow_disk = disks_info; follow_disk != NULL; follow_disk = follow_disk->next){
		if(first == 1)
			first = 0;
		else
			websWrite(wp, ", ");
		
		websWrite(wp, "\"usb\"");
	}
	websWrite(wp, "];\n");
	websWrite(wp, "}\n\n");
	
	char *Ptr;
	
	websWrite(wp, "function pool_names(){\n");
	websWrite(wp, "    return [");
	
	first = 1;
	for(follow_disk = disks_info; follow_disk != NULL; follow_disk = follow_disk->next)
		for(follow_partition = follow_disk->partitions; follow_partition != NULL; follow_partition = follow_partition->next){
			if(first == 1)
				first = 0;
			else
				websWrite(wp, ", ");
			
			if(follow_partition->mount_point == NULL){
				websWrite(wp, "\"unknown\"");
				continue;
			}
			
			Ptr = rindex(follow_partition->mount_point, '/');
			if(Ptr == NULL){
				websWrite(wp, "\"unknown\"");
				continue;
			}
			
			++Ptr;
			if(!strncmp(Ptr, "part", 4))
				websWrite(wp, "\"%s\"", Ptr);
			else
				websWrite(wp, "\"unknown\"");
		}
	websWrite(wp, "];\n");
	websWrite(wp, "}\n\n");
	
	websWrite(wp, "function pool_types(){\n");
	websWrite(wp, "    return [");
	first = 1;
	for(follow_disk = disks_info; follow_disk != NULL; follow_disk = follow_disk->next)
		for(follow_partition = follow_disk->partitions; follow_partition != NULL; follow_partition = follow_partition->next){
			if(first == 1)
				first = 0;
			else
				websWrite(wp, ", ");
			
			if(follow_partition->mount_point == NULL){
				websWrite(wp, "\"unknown\"");
				continue;
			}
			
			websWrite(wp, "\"%s\"", follow_partition->file_system);
		}
	websWrite(wp, "];\n");
	websWrite(wp, "}\n\n");
	
	websWrite(wp, "function pool_mirror_counts(){\n");
	websWrite(wp, "    return [");
	first = 1;
	for(follow_disk = disks_info; follow_disk != NULL; follow_disk = follow_disk->next)
		for(follow_partition = follow_disk->partitions; follow_partition != NULL; follow_partition = follow_partition->next){
			if(first == 1)
				first = 0;
			else
				websWrite(wp, ", ");
			
			websWrite(wp, "0");
		}
	websWrite(wp, "];\n");
	websWrite(wp, "}\n\n");
	
	websWrite(wp, "function pool_status(){\n");
	websWrite(wp, "    return [");
	first = 1;
	for(follow_disk = disks_info; follow_disk != NULL; follow_disk = follow_disk->next)
		for(follow_partition = follow_disk->partitions; follow_partition != NULL; follow_partition = follow_partition->next){
			if(first == 1)
				first = 0;
			else
				websWrite(wp, ", ");
			
			if(follow_partition->mount_point == NULL){
				websWrite(wp, "\"unmounted\"");
				continue;
			}
			
			/*if(strcmp(follow_partition->file_system, "ntfs") == 0)
				websWrite(wp, "\"ro\"");
			else
				websWrite(wp, "\"rw\"");//*/
			//printf("permission = %s\n", follow_partition->permission);
			websWrite(wp, "\"%s\"", follow_partition->permission);
		}
	websWrite(wp, "];\n");
	websWrite(wp, "}\n\n");
	
	websWrite(wp, "function pool_kilobytes(){\n");
	websWrite(wp, "    return [");
	first = 1;
	for(follow_disk = disks_info; follow_disk != NULL; follow_disk = follow_disk->next)
		for(follow_partition = follow_disk->partitions; follow_partition != NULL; follow_partition = follow_partition->next){
			if(first == 1)
				first = 0;
			else
				websWrite(wp, ", ");
			
			websWrite(wp, "%llu", follow_partition->size_in_kilobytes);
		}
	websWrite(wp, "];\n");
	websWrite(wp, "}\n\n");
	
	websWrite(wp, "function pool_encryption_password_is_missing(){\n");
	websWrite(wp, "    return [");
	first = 1;
	for(follow_disk = disks_info; follow_disk != NULL; follow_disk = follow_disk->next)
		for(follow_partition = follow_disk->partitions; follow_partition != NULL; follow_partition = follow_partition->next){
			if(first == 1)
				first = 0;
			else
				websWrite(wp, ", ");
			
			websWrite(wp, "\"no\"");
		}
	websWrite(wp, "];\n");
	websWrite(wp, "}\n\n");
	
	websWrite(wp, "function pool_kilobytes_in_use(){\n");
	websWrite(wp, "    return [");
	first = 1;
	for(follow_disk = disks_info; follow_disk != NULL; follow_disk = follow_disk->next)
		for(follow_partition = follow_disk->partitions; follow_partition != NULL; follow_partition = follow_partition->next){
			if(first == 1)
				first = 0;
			else
				websWrite(wp, ", ");
			
			websWrite(wp, "%llu", follow_partition->used_kilobytes);
		}
	websWrite(wp, "];\n");
	websWrite(wp, "}\n\n");
	
	u64 disk_used_kilobytes;
	
	websWrite(wp, "function disk_usage(){\n");
	websWrite(wp, "    return [");
	first = 1;
	for(follow_disk = disks_info; follow_disk != NULL; follow_disk = follow_disk->next){
		if(first == 1)
			first = 0;
		else
			websWrite(wp, ", ");
		
		disk_used_kilobytes = 0;
		for(follow_partition = follow_disk->partitions; follow_partition != NULL; follow_partition = follow_partition->next)
			disk_used_kilobytes += follow_partition->size_in_kilobytes;
		
		websWrite(wp, "%llu", disk_used_kilobytes);
	}
	websWrite(wp, "];\n");
	websWrite(wp, "}\n\n");
	
	disk_info_t *follow_disk2;
	u32 disk_num, pool_num;
	
	websWrite(wp, "function per_pane_pool_usage_kilobytes(pool_num, disk_num){\n");
	for(follow_disk = disks_info, pool_num = 0; follow_disk != NULL; follow_disk = follow_disk->next){
		for(follow_partition = follow_disk->partitions; follow_partition != NULL; follow_partition = follow_partition->next, ++pool_num){
			websWrite(wp, "    if(pool_num == %d){\n", pool_num);
			
			if(follow_partition->mount_point != NULL)
				for(follow_disk2 = disks_info, disk_num = 0; follow_disk2 != NULL; follow_disk2 = follow_disk2->next, ++disk_num){
					websWrite(wp, "        if(disk_num == %d){\n", disk_num);
					
					if(strcmp(follow_disk2->tag, follow_disk->tag) == 0)
						websWrite(wp, "            return [%llu];\n", follow_partition->size_in_kilobytes);
					else
						websWrite(wp, "            return [0];\n");
					
					websWrite(wp, "        }\n");
				}
			else
				websWrite(wp, "        return [0];\n");
			
			websWrite(wp, "    }\n");
		}
	}
	websWrite(wp, "}\n\n");
	
	//free_disk_data(&disks_info);
	
	return 0;
}

static int ej_available_disk_names_and_sizes(int eid, webs_t wp, int argc, char_t **argv){
	disk_info_t *disks_info = NULL, *follow_disk;
	int first;
	
	websWrite(wp, "function available_disks(){ return [];}\n\n");
	websWrite(wp, "function available_disk_sizes(){ return [];}\n\n");
	websWrite(wp, "function claimed_disks(){ return [];}\n\n");
	websWrite(wp, "function claimed_disk_interface_names(){ return [];}\n\n");
	websWrite(wp, "function claimed_disk_model_info(){ return [];}\n\n");
	websWrite(wp, "function claimed_disk_total_size(){ return [];}\n\n");
	websWrite(wp, "function claimed_disk_total_mounted_number(){ return [];}\n\n");
	websWrite(wp, "function blank_disks(){ return [];}\n\n");
	websWrite(wp, "function blank_disk_interface_names(){ return [];}\n\n");
	websWrite(wp, "function blank_disk_model_info(){ return [];}\n\n");
	websWrite(wp, "function blank_disk_total_size(){ return [];}\n\n");
	websWrite(wp, "function blank_disk_total_mounted_number(){ return [];}\n\n");
	
	//disks_info = read_disk_data();
	if(DISKS_INFO == NULL)
		DISKS_INFO = read_disk_data();
	disks_info = DISKS_INFO;
	if(disks_info == NULL){
		websWrite(wp, "%s", initial_available_disk_names_and_sizes());
		return -1;
	}
	
	// show name of the foreign disks
	websWrite(wp, "function foreign_disks(){\n");
	websWrite(wp, "    return [");
	first = 1;
	for(follow_disk = disks_info; follow_disk != NULL; follow_disk = follow_disk->next){
		if(first == 1)
			first = 0;
		else
			websWrite(wp, ", ");
		
		websWrite(wp, "\"%s\"", follow_disk->tag);
	}
	websWrite(wp, "];\n");
	websWrite(wp, "}\n\n");
	
	// show interface of the foreign disks
	websWrite(wp, "function foreign_disk_interface_names(){\n");
	websWrite(wp, "    return [");
	first = 1;
	for(follow_disk = disks_info; follow_disk != NULL; follow_disk = follow_disk->next){
		if(first == 1)
			first = 0;
		else
			websWrite(wp, ", ");
		
		websWrite(wp, "\"%s\"", follow_disk->port);
	}
	websWrite(wp, "];\n");
	websWrite(wp, "}\n\n");
	
	// show model info of the foreign disks
	websWrite(wp, "function foreign_disk_model_info(){\n");
	websWrite(wp, "    return [");
	first = 1;
	for(follow_disk = disks_info; follow_disk != NULL; follow_disk = follow_disk->next){
		if(first == 1)
			first = 0;
		else
			websWrite(wp, ", ");
		
		websWrite(wp, "\"");
		
		if(follow_disk->vendor != NULL)
			websWrite(wp, "%s", follow_disk->vendor);
		if(follow_disk->model != NULL){
			if(follow_disk->vendor != NULL)
				websWrite(wp, " ");
			
			websWrite(wp, "%s", follow_disk->model);
		}
		
		websWrite(wp, "\"");
	}
	websWrite(wp, "];\n");
	websWrite(wp, "}\n\n");
	
	// show total_size of the foreign disks
	websWrite(wp, "function foreign_disk_total_size(){\n");
	websWrite(wp, "    return [");
	first = 1;
	for(follow_disk = disks_info; follow_disk != NULL; follow_disk = follow_disk->next){
		if(first == 1)
			first = 0;
		else
			websWrite(wp, ", ");
		
		websWrite(wp, "\"%llu\"", follow_disk->size_in_kilobytes);
	}
	websWrite(wp, "];\n");
	websWrite(wp, "}\n\n");
	
	// show total number of the mounted partition in this foreign disk
	websWrite(wp, "function foreign_disk_total_mounted_number(){\n");
	websWrite(wp, "    return [");
	first = 1;
	for(follow_disk = disks_info; follow_disk != NULL; follow_disk = follow_disk->next){
		if(first == 1)
			first = 0;
		else
			websWrite(wp, ", ");
		
		websWrite(wp, "\"%u\"", follow_disk->mounted_number);
	}
	websWrite(wp, "];\n");
	websWrite(wp, "}\n\n");
	
	//free_disk_data(&disks_info);
	
	return 0;
}

static int ej_safely_remove_disk(int eid, webs_t wp, int argc, char_t **argv){
	char *disk_port = websGetVar(wp, "disk", "");
	disk_info_t *disks_info = NULL, *follow_disk = NULL;
	char *command;
	int disk_num = 0;
	int len = 0;
	int result;
csprintf("disk_port=%s.\n", disk_port);
	if(strlen(disk_port) <= 0){
		show_error_msg("Input20");
		
		websWrite(wp, "<script>\n");
		websWrite(wp, "safely_remove_disk_error(\'%s\');\n", error_msg);
		websWrite(wp, "</script>\n");
		
		clean_error_msg();
		return -1;
	}
	
	//disks_info = read_disk_data();
	if(DISKS_INFO == NULL)
		DISKS_INFO = read_disk_data();
	disks_info = DISKS_INFO;
	if(disks_info == NULL){
		show_error_msg("System2");
		
		websWrite(wp, "<script>\n");
		websWrite(wp, "safely_remove_disk_error(\'%s\');\n", error_msg);
		websWrite(wp, "</script>\n");
		
		clean_error_msg();
		return -1;
	}
	
	for(follow_disk = disks_info; follow_disk != NULL; follow_disk = follow_disk->next, ++disk_num)
		;
	
	/*for(follow_disk = disks_info; follow_disk != NULL; follow_disk = follow_disk->next)
		if(!strcmp(follow_disk->port, disk_port))
			break;
	if(follow_disk == NULL){
		show_error_msg("Input21");
		
		websWrite(wp, "<script>\n");
		websWrite(wp, "safely_remove_disk_error(\'%s\');\n", error_msg);
		websWrite(wp, "</script>\n");
		
		clean_error_msg();
		//free_disk_data(&disks_info);
		return -1;
	}
	//free_disk_data(&disks_info);//*/
	
	/*if(!strcmp(nvram_safe_get("usb_disc0_port"), disk_port))
		result = eval("/sbin/eject_usb1");
	else if(!strcmp(nvram_safe_get("usb_disc1_port"), disk_port))
		result = eval("/sbin/eject_usb2");
	else if(!strcmp(nvram_safe_get("usb_disc2_port"), disk_port))
		result = eval("/sbin/eject_usb3");//*/
	if(!strcmp(disk_port, "1"))
		result = eval("/sbin/eject_usb1");
	else if(!strcmp(disk_port, "2"))
		result = eval("/sbin/eject_usb2");
	else{
		show_error_msg("Input21");
		
		websWrite(wp, "<script>\n");
		websWrite(wp, "safely_remove_disk_error(\'%s\');\n", error_msg);
		websWrite(wp, "</script>\n");
		
		clean_error_msg();
		return -1;
	}
	
	if(result != 0){
		show_error_msg("Action9");
		
		websWrite(wp, "<script>\n");
		websWrite(wp, "safely_remove_disk_error(\'%s\');\n", error_msg);
		websWrite(wp, "</script>\n");
		
		clean_error_msg();
		return -1;
	}
	
	if(disk_num > 1)
		result = eval("/sbin/run_ftpsamba");
	else{
		result = eval("/sbin/stop_ftpsamba");
		result = eval("/sbin/stop_dms");
	}
	
	websWrite(wp, "<script>\n");
	websWrite(wp, "safely_remove_disk_success(\'%s\');\n", error_msg);
	websWrite(wp, "</script>\n");
	
	return -1;
}

// The parameter from web are "disk", "new_pool_name", "new_pool_size", "file_system".
int ej_do_claim_disk(int eid, webs_t wp, int argc, char_t **argv){
	char *disk, *new_pool_name, *new_pool_size, *file_system;
	disk_info_t *disk_info_list, *follow_disks;
	partition_info_t *follow_pools;
	int new_pool_order, fdisk_ok, format_ok, result, i;
	char *device_path;
	char *tmpfile = "/tmp/tmp-fdisk-command";
	FILE *fp;
	
	disk = websGetVar(wp, "disk", "");
	new_pool_name = websGetVar(wp, "new_pool_name", "");
	new_pool_size = websGetVar(wp, "new_pool_size", "");
	file_system = websGetVar(wp, "file_system", "");
	
	if(disk == NULL || strlen(disk) == 0){
		show_error_msg("Input20");
		
		websWrite(wp, "<script>\n");
		websWrite(wp, "claim_error(\'%s\');\n", error_msg);
		websWrite(wp, "</script>\n");
		
		clean_error_msg();
		return -1;
	}
	if(new_pool_name == NULL || strlen(new_pool_name) == 0){
		show_error_msg("Input22");
		
		websWrite(wp, "<script>\n");
		websWrite(wp, "claim_error(\'%s\');\n", error_msg);
		websWrite(wp, "</script>\n");
		
		clean_error_msg();
		return -1;
	}
	if(new_pool_size == NULL || strlen(new_pool_size) == 0){
		show_error_msg("Input24");
		
		websWrite(wp, "<script>\n");
		websWrite(wp, "claim_error(\'%s\');\n", error_msg);
		websWrite(wp, "</script>\n");
		
		clean_error_msg();
		return -1;
	}
	if(file_system == NULL || strlen(file_system) == 0){
		show_error_msg("Input25");
		
		websWrite(wp, "<script>\n");
		websWrite(wp, "claim_error(\'%s\');\n", error_msg);
		websWrite(wp, "</script>\n");
		
		clean_error_msg();
		return -1;
	}
	if(strcmp(file_system, "vfat")
			&& strcmp(file_system, "fat32")
			&& strcmp(file_system, "msdos")
			&& strcmp(file_system, "ext2")
			&& strcmp(file_system, "ext3")){
		show_error_msg("Input26");
		
		websWrite(wp, "<script>\n");
		websWrite(wp, "claim_error(\'%s\');\n", error_msg);
		websWrite(wp, "</script>\n");
		
		clean_error_msg();
		return -1;
	}
	if(!strcmp(file_system, "vfat") && !is_valid_fat32_disk_name(new_pool_name)){
		show_error_msg("Input23");
		
		websWrite(wp, "<script>\n");
		websWrite(wp, "claim_error(\'%s\');\n", error_msg);
		websWrite(wp, "</script>\n");
		
		clean_error_msg();
		return -1;
	}	
	
	// use usb_storage_busy to avoid watchdog check httpd.
	if(nvram_match("usb_storage_busy", "1")){
		show_error_msg("System3");
		
		websWrite(wp, "<script>\n");
		websWrite(wp, "claim_error(\'%s\');\n", error_msg);
		websWrite(wp, "</script>\n");
		
		clean_error_msg();
		return -1;
	}
	nvram_set("usb_storage_busy", "1");

	//disk_info_list = read_disk_data();
	if(DISKS_INFO == NULL)
		DISKS_INFO = read_disk_data();
	disk_info_list = DISKS_INFO;
	if(disk_info_list == NULL){
		show_error_msg("System2");
		
		websWrite(wp, "<script>\n");
		websWrite(wp, "claim_error(\'%s\');\n", error_msg);
		websWrite(wp, "</script>\n");
		
		clean_error_msg();
		nvram_set("usb_storage_busy", "0");
		return -1;
	}
	
	for(follow_disks = disk_info_list; follow_disks != NULL; follow_disks = follow_disks->next)
		if(!strcmp(follow_disks->tag, disk))
			break;
	if(follow_disks == NULL){
		show_error_msg("Input21");
		
		websWrite(wp, "<script>\n");
		websWrite(wp, "claim_error(\'%s\');\n", error_msg);
		websWrite(wp, "</script>\n");
		
		clean_error_msg();
		//free_disk_data(&disk_info_list);
		nvram_set("usb_storage_busy", "0");
		return -1;
	}
	
	new_pool_order = 1;
	for(follow_pools = follow_disks->partitions; follow_pools != NULL; follow_pools = follow_pools->next)
		++new_pool_order;
	
	// umount the target disk first.
	char *command[2];
	if(!strcmp(disk, "LowerUSB"))
		command[0] = "eject_usb1";
	else
		command[0] = "eject_usb2";
	command[1] = NULL;
	result = _eval(command, ">/dev/console", 0, NULL);
	if(result < 0){
		show_error_msg("Action9");
		
		websWrite(wp, "<script>\n");
		websWrite(wp, "claim_error(\'%s\');\n", error_msg);
		websWrite(wp, "</script>\n");
		
		clean_error_msg();
		//free_disk_data(&disk_info_list);
		nvram_set("usb_storage_busy", "0");
		return -1;
	}
	
	if(!strcmp(follow_disks->device, "/dev/scsi/host0/bus0/target0/lun0/disc"))
		device_path = "/dev/discs/disc0/disc";
	else
		device_path = "/dev/discs/disc1/disc";
	//free_disk_data(&disk_info_list);
	
	result = make_fdisk_command(tmpfile, new_pool_order, new_pool_size, file_system);
	if(result < 0){
		printf("Fail to make the fdisk command file!");
		show_error_msg("System1");
		
		websWrite(wp, "<script>\n");
		websWrite(wp, "claim_error(\'%s\');\n", error_msg);
		websWrite(wp, "</script>\n");
		
		clean_error_msg();
		nvram_set("usb_storage_busy", "0");
		return -1;
	}
	
	fdisk_ok = fdisk_disk(device_path, new_pool_order);
	if(fdisk_ok < 0){
		show_error_msg("Action10");
		
		websWrite(wp, "<script>\n");
		websWrite(wp, "claim_error(\'%s\');\n", error_msg);
		websWrite(wp, "</script>\n");
		
		clean_error_msg();
		//free_disk_data(&disk_info_list);
		nvram_set("usb_storage_busy", "0");
		return -1;
	}
	
	if(new_pool_order == 4)
		format_ok = format_disk(device_path, new_pool_order+1, new_pool_name, file_system);
	else
		format_ok = format_disk(device_path, new_pool_order, new_pool_name, file_system);
	if(format_ok < 0){
		show_error_msg("Action12");
		
		websWrite(wp, "<script>\n");
		websWrite(wp, "claim_error(\'%s\');\n", error_msg);
		websWrite(wp, "</script>\n");
		
		clean_error_msg();
		//free_disk_data(&disk_info_list);
		nvram_set("usb_storage_busy", "0");
		
		return -1;
	}
	else{
		websWrite(wp, "<script>\n");
		websWrite(wp, "claim_success();");
		websWrite(wp, "</script>\n");
		
		//free_disk_data(&disk_info_list);
		nvram_set("usb_storage_busy", "0");
		
		return 0;
	}
}

int ej_get_AiDisk_status(int eid, webs_t wp, int argc, char **argv){
	disk_info_t *disks_info, *follow_disk;
	partition_info_t *follow_partition;
	char *follow_info;
	int sh_num;
	char **folder_list = NULL;
	int first_pool, first_folder, result, i;
	
	websWrite(wp, "function get_cifs_status(){\n");
	websWrite(wp, "    return %d;\n", atoi(nvram_safe_get("samba_running")));
	websWrite(wp, "}\n\n");
	
	websWrite(wp, "function get_ftp_status(){\n");
	websWrite(wp, "    return %d;\n", atoi(nvram_safe_get("ftp_running")));
	websWrite(wp, "}\n\n");
	
	websWrite(wp, "function get_dms_status(){\n");
	websWrite(wp, "    return %d;\n", atoi(nvram_safe_get("dms_running")));
	websWrite(wp, "}\n\n");
	
	websWrite(wp, "function get_share_management_status(protocol){\n");
	websWrite(wp, "    if(protocol == \"cifs\")\n");
	websWrite(wp, "        return %d;\n", atoi(nvram_safe_get("st_samba_mode")));
	websWrite(wp, "    else if(protocol == \"ftp\")\n");
	websWrite(wp, "        return %d;\n", atoi(nvram_safe_get("st_ftp_mode")));
	websWrite(wp, "    else\n");
	websWrite(wp, "        return -1;\n");
	websWrite(wp, "}\n\n");
	
	//disks_info = read_disk_data();
	if(DISKS_INFO == NULL)
		DISKS_INFO = read_disk_data();
	disks_info = DISKS_INFO;
	if(disks_info == NULL){
		websWrite(wp, "function get_sharedfolder_in_pool(poolName){}\n");
		return -1;
	}
	
	first_pool = 1;
	websWrite(wp, "function get_sharedfolder_in_pool(poolName){\n");
	for(follow_disk = disks_info; follow_disk != NULL; follow_disk = follow_disk->next)
		for(follow_partition = follow_disk->partitions; follow_partition != NULL; follow_partition = follow_partition->next){
			if(follow_partition->mount_point != NULL && strlen(follow_partition->mount_point) > 0){
				websWrite(wp, "    ");
				
				if(first_pool == 1)
					first_pool = 0;
				else
					websWrite(wp, "else ");
				
				follow_info = rindex(follow_partition->mount_point, '/');
				websWrite(wp, "if(poolName == \"%s\"){\n", follow_info+1);
				websWrite(wp, "        return [");
				
				//result = get_folder_list_in_mount_path(follow_partition->mount_point, &sh_num, &folder_list);
				result = get_all_folder_in_mount_path(follow_partition->mount_point, &sh_num, &folder_list);
				if(result < 0){
					websWrite(wp, "];\n");
					websWrite(wp, "    }\n");
					
					printf("get_AiDisk_status: Can't get the folder list in \"%s\".\n", follow_partition->mount_point);
					
					free_2_dimension_list(&sh_num, &folder_list);
					
					continue;
				}
				
				first_folder = 1;
				for(i = 0; i < sh_num; ++i){
					if(first_folder == 1)
						first_folder = 0;
					else
						websWrite(wp, ", ");
					
					websWrite(wp, "\"%s\"", folder_list[i]);
				}
				
				websWrite(wp, "];\n");
				websWrite(wp, "    }\n");
				
				free_2_dimension_list(&sh_num, &folder_list);
			}
		}
	
	websWrite(wp, "}\n\n");
	
	/*if(disks_info != NULL){
		free_disk_data(&disks_info);
	}//*/
	
	return 0;
}

// The parameter from web are "protocol", "flag".
int ej_set_AiDisk_status(int eid, webs_t wp, int argc, char **argv){
	char *protocol = websGetVar(wp, "protocol", "");
	char *flag = websGetVar(wp, "flag", "");
	int result;
	
	if(strlen(protocol) <= 0){
		show_error_msg("Input1");
		
		websWrite(wp, "<script>\n");
		websWrite(wp, "set_AiDisk_status_error(\'%s\');\n", error_msg);
		websWrite(wp, "</script>\n");
		
		clean_error_msg();
		return -1;
	}
	if(strlen(flag) <= 0){
		show_error_msg("Input18");
		
		websWrite(wp, "<script>\n");
		websWrite(wp, "set_AiDisk_status_error(\'%s\');\n", error_msg);
		websWrite(wp, "</script>\n");
		
		clean_error_msg();
		return -1;
	}
	
	if(!strcmp(protocol, "cifs")){
		if(!strcmp(flag, "on")){
			// run or re-run samba
			nvram_set("samba_enable","1");
			nvram_commit();
			result = eval("/sbin/run_samba");
		}
		else if(!strcmp(flag, "off")){
			if(nvram_match("samba_running", "0"))
				goto SET_AIDISK_STATUS_SUCCESS;
			
			nvram_set("samba_enable","0");
			nvram_commit();
			result = eval("/sbin/stop_samba");
		}
		else{
			show_error_msg("Input19");
			
			websWrite(wp, "<script>\n");
			websWrite(wp, "set_AiDisk_status_error(\'%s\');\n", error_msg);
			websWrite(wp, "</script>\n");
			
			clean_error_msg();
			return -1;
		}
	}
	else if(!strcmp(protocol, "ftp")){
		if(!strcmp(flag, "on")){
			// run or re-run ftp
			nvram_set("ftp_enable","1");
			nvram_commit();
			result = eval("/sbin/run_ftp");
		}
		else if(!strcmp(flag, "off")){
			if(nvram_match("ftp_running", "0"))
				goto SET_AIDISK_STATUS_SUCCESS;
			
			nvram_set("ftp_enable","0");
			nvram_commit();
			result = eval("/sbin/stop_ftp");
		}
		else{
			show_error_msg("Input19");
			
			websWrite(wp, "<script>\n");
			websWrite(wp, "set_AiDisk_status_error(\'%s\');\n", error_msg);
			websWrite(wp, "</script>\n");
			
			clean_error_msg();
			return -1;
		}
	}
	else{
		show_error_msg("Input2");
		
		websWrite(wp, "<script>\n");
		websWrite(wp, "set_AiDisk_status_error(\'%s\');\n", error_msg);
		websWrite(wp, "</script>\n");
		
		clean_error_msg();
		return -1;
	}
	
	if(result != 0){
		show_error_msg("Action8");
		
		websWrite(wp, "<script>\n");
		websWrite(wp, "set_AiDisk_status_error(\'%s\');\n", error_msg);
		websWrite(wp, "</script>\n");
		
		clean_error_msg();
		return -1;
	}
	
SET_AIDISK_STATUS_SUCCESS:
	websWrite(wp, "<script>\n");
	websWrite(wp, "set_AiDisk_status_success();\n");
	websWrite(wp, "</script>\n");
	
	return 0;
}

int ej_get_all_accounts(int eid, webs_t wp, int argc, char **argv){
	int acc_num;
	char **account_list = NULL;
	int result, i, first;
	
	if((result = get_account_list(&acc_num, &account_list)) < 0){
		printf("Failed to get the account list!\n");
		return -1;
	}
	
	first = 1;
	for(i = 0; i < acc_num; ++i){
		if(first == 1)
			first = 0;
		else
			websWrite(wp, ", ");
		
		websWrite(wp, "\"%s\"", account_list[i]);
	}
	
	free_2_dimension_list(&acc_num, &account_list);
	return 0;
}

// The parameter from web are "layer_order".
// ex. return "UpperUSB#0#1" -> "NAME#ORDER#CHILD"
//			"UpperUSB" is meant the name from "layer_order"
//			0 is meant the order
//			1 is meant that there is something in the "UpperUSB"
int ej_get_share_tree(int eid, webs_t wp, int argc, char **argv){
	char *layer_order = websGetVar(wp, "layer_order", "");
	char *follow_info, *follow_info_end, backup;
	int layer = 0, first;
	int disk_count, partition_count, share_count;
	int disk_order = -1, partition_order = -1;
	disk_info_t *disks_info, *follow_disk;
	partition_info_t *follow_partition;
	
	if(strlen(layer_order) <= 0){
		printf("No input \"layer_order\"!\n");
		return -1;
	}
	
	follow_info = index(layer_order, '_');
	while(follow_info != NULL && *follow_info != 0){
		++layer;
		++follow_info;
		if(*follow_info == 0)
			break;
		follow_info_end = follow_info;
		while(*follow_info_end != 0 && isdigit(*follow_info_end))
			++follow_info_end;
		backup = *follow_info_end;
		*follow_info_end = 0;
		
		if(layer == 1)
			disk_order = atoi(follow_info);
		else if(layer == 2)
			partition_order = atoi(follow_info);
		else{
			*follow_info_end = backup;
			printf("Input \"%s\" is incorrect!\n", layer_order);
			return -1;
		}
		
		*follow_info_end = backup;
		follow_info = follow_info_end;
	}
	
	if(layer < 0 || layer > 2)
		return 0;
	
	//disks_info = read_disk_data();
	if(DISKS_INFO == NULL)
		DISKS_INFO = read_disk_data();
	disks_info = DISKS_INFO;
	if(disks_info == NULL){
		printf("Can't read the information of disks.\n");
		return -1;
	}
	
	first = 1;
	disk_count = 0;
	for(follow_disk = disks_info; follow_disk != NULL; follow_disk = follow_disk->next, ++disk_count){
		if(layer > 0 && disk_count != disk_order)
			continue;
		
		partition_count = 0;
		for(follow_partition = follow_disk->partitions; follow_partition != NULL; follow_partition = follow_partition->next){
			if(follow_partition->mount_point != NULL && strlen(follow_partition->mount_point) > 0){
				++partition_count;
				
				if(layer == 0
						|| (layer == 2 && partition_count != partition_order))
					continue;
				
				int i;
				char **folder_list;
				int result;
				
				result = get_all_folder_in_mount_path(follow_partition->mount_point, &share_count, &folder_list);
				if(result < 0){
					printf("get_disk_tree: Can't get the folder list in \"%s\".\n", follow_partition->mount_point);
					
					share_count = 0;
				}
				
				if(layer == 2 && partition_count == partition_order){
					for(i = 0; i < share_count; ++i){
						if(first == 1)
							first = 0;
						else
							websWrite(wp, ", ");
						
						websWrite(wp, "\"%s#%u#0\"", folder_list[i], i);
					}
					
					free_2_dimension_list(&share_count, &folder_list);
					
					break;
				}
				else if(layer == 1){
					if(first == 1)
						first = 0;
					else
						websWrite(wp, ", ");
					
					follow_info = rindex(follow_partition->mount_point, '/');
					websWrite(wp, "\"%s#%u#%u\"", follow_info+1, partition_count, share_count);
				}
				
				free_2_dimension_list(&share_count, &folder_list);
			}
		}
		
		if(layer == 0){
			if(first == 1)
				first = 0;
			else
				websWrite(wp, ", ");
			
			websWrite(wp, "\"%s#%u#%u\"", follow_disk->tag, disk_count, partition_count);
		}
		
		if(layer > 0 && disk_count == disk_order)
			break;
	}
	
	//free_disk_data(&disks_info);
	
	return 0;
}

int ej_get_folder_tree(int eid, webs_t wp, int argc, char **argv){
	char *layer_order = websGetVar(wp, "layer_order", ""), folder_code[1024];
	char *follow_info, *follow_info_end, backup;
	int layer = 0, first;
	int disk_count, partition_count, folder_count1, folder_count2;
	int disk_order = -1, partition_order = -1, folder_order = -1;
	disk_info_t *disks_info, *follow_disk;
	partition_info_t *follow_partition;
	char *pool_mount_dir;
	DIR *dir1, *dir2;
	struct dirent *dp1, *dp2;
	char dir1_Path[4096], dir2_Path[4096];
	
	if(strlen(layer_order) <= 0){
		printf("No input \"layer_order\"!\n");
		return -1;
	}
	
	follow_info = index(layer_order, '_');
	while(follow_info != NULL && *follow_info != 0){
		++layer;
		
		++follow_info;
		if(*follow_info == 0)
			break;
		follow_info_end = follow_info;
		while(*follow_info_end != 0 && isdigit(*follow_info_end))
			++follow_info_end;
		backup = *follow_info_end;
		*follow_info_end = 0;
		
		if(layer == 1)
			disk_order = atoi(follow_info);
		else if(layer == 2)
			partition_order = atoi(follow_info);
		else if(layer == 3)
			folder_order = atoi(follow_info);
		
		*follow_info_end = backup;
		
		if(layer == 3){
			memset(folder_code, 0, 1024);
			strcpy(folder_code, follow_info);
		}
		
		follow_info = follow_info_end;
	}
	follow_info = folder_code;
	
	//disks_info = read_disk_data();
	if(DISKS_INFO == NULL)
		DISKS_INFO = read_disk_data();
	disks_info = DISKS_INFO;
	if(disks_info == NULL){
		printf("Can't read the information of disks.\n");
		return -1;
	}
	
	first = 1;
	disk_count = 0;
	for(follow_disk = disks_info; follow_disk != NULL; follow_disk = follow_disk->next, ++disk_count){
		if(layer == 0){	// get disks.
			partition_count = 0;
			for(follow_partition = follow_disk->partitions; follow_partition != NULL; follow_partition = follow_partition->next, ++partition_count)
				;
			
			if(first == 1)
				first = 0;
			else
				websWrite(wp, ", ");
			
			websWrite(wp, "\"%s#%u#%u\"", follow_disk->tag, disk_count, partition_count);
			
			continue;
		}
		
		if(disk_count != disk_order)
			continue;
		
		partition_count = 0;
		for(follow_partition = follow_disk->partitions; follow_partition != NULL; follow_partition = follow_partition->next, ++partition_count){
			if(follow_partition->mount_point == NULL || strlen(follow_partition->mount_point) <= 0)
				continue;
			
			pool_mount_dir = rindex(follow_partition->mount_point, '/')+1;
			
			if(layer == 1){	// get pools.
				dir2 = opendir(follow_partition->mount_point);
				// pool_mount_dir isn't file.
				if(dir2 == NULL)
					continue;
				
				folder_count2 = 0;
				while((dp2 = readdir(dir2)) != NULL){
					if(dp2->d_name[0] == '.')
						continue;
					
					++folder_count2;
				}
				closedir(dir2);
				
				if(first == 1)
					first = 0;
				else
					websWrite(wp, ", ");
				
				websWrite(wp, "\"%s#%u#%u\"", pool_mount_dir, partition_count, folder_count2);
				
				continue;
			}
			
			if(partition_count != partition_order)
				continue;
			
			memset(dir1_Path, 0, 4096);
			sprintf(dir1_Path, "%s", follow_partition->mount_point);
			dir1 = opendir(dir1_Path);
			if(dir1 == NULL){
				printf("Can't open the directory, %s.\n", dir1_Path);
				
				//free_disk_data(&disks_info);
				
				return -1;
			}
			
			folder_count1 = -1;
			while((dp1 = readdir(dir1)) != NULL){
				if(dp1->d_name[0] == '.')
					continue;
				
				++folder_count1;
				
				if(layer == 2){	// get L1's folders.
					memset(dir2_Path, 0, 4096);
					sprintf(dir2_Path, "%s/%s", dir1_Path, dp1->d_name);
					dir2 = opendir(dir2_Path);
					
					folder_count2 = 0;
					if(dir2 != NULL){
						while((dp2 = readdir(dir2)) != NULL){
							if(dp2->d_name[0] == '.')
								continue;
							
							++folder_count2;
						}
						closedir(dir2);
					}
					
					if(first == 1)
						first = 0;
					else
						websWrite(wp, ", ");
					
					websWrite(wp, "\"%s#%u#%u\"", dp1->d_name, folder_count1, folder_count2);
					
					continue;
				}
				
				if(folder_count1 == folder_order)
					sprintf(dir1_Path, "%s/%s", dir1_Path, dp1->d_name);
			}
			closedir(dir1);
		}
	}
	//free_disk_data(&disks_info);
	layer -= 3;
	
	while(layer >= 0){	// get Ln's folders.
		// get the current folder_code and folder_order.
		follow_info_end = index(follow_info, '_');
		if(follow_info_end != NULL)
			follow_info = follow_info_end+1;
		else
			backup = -1;
		folder_order = atoi(follow_info);
		
		dir1 = opendir(dir1_Path);
		if(dir1 == NULL){
			printf("Can't open the directory, %s.\n", dir1_Path);
			
			return -1;
		}
		
		folder_count1 = -1;
		while((dp1 = readdir(dir1)) != NULL){
			if(dp1->d_name[0] == '.')
				continue;
			
			++folder_count1;
			
			if(layer == 0){
				memset(dir2_Path, 0, 4096);
				sprintf(dir2_Path, "%s/%s", dir1_Path, dp1->d_name);
				dir2 = opendir(dir2_Path);
				folder_count2 = 0;
				if(dir2 != NULL){
					while((dp2 = readdir(dir2)) != NULL){
						if(dp2->d_name[0] == '.')
							continue;
						
						++folder_count2;
					}
					closedir(dir2);
				}
				
				if(first == 1)
					first = 0;
				else
					websWrite(wp, ", ");
				
				websWrite(wp, "\"%s#%u#%u\"", dp1->d_name, folder_count1, folder_count2);
				
				continue;
			}
			
			if(folder_count1 == folder_order)
				sprintf(dir1_Path, "%s/%s", dir1_Path, dp1->d_name);
		}
		closedir(dir1);
		
		--layer;
	}
	
	return 0;
}

int ej_get_share_mode(int eid, webs_t wp, int argc, char **argv){
	int samba_mode, ftp_mode;
	
	websWrite(wp, "function show_cifs_mode(){\n");
	samba_mode = atoi(nvram_safe_get("st_samba_mode"));
	switch(samba_mode){
		case 0:
			websWrite(wp, "    return \"off\";\n");
			break;
		case 1:
		case 3:
			websWrite(wp, "    return \"share\";\n");
			break;
		case 2:
		case 4:
			websWrite(wp, "    return \"account\";\n");
			break;
		default:
			fprintf(stderr, "Samba's mode is incorrect!\n");
			websWrite(wp, "    return \"unknown\";\n");
	}
	websWrite(wp, "}\n\n");
	
	websWrite(wp, "function show_ftp_mode(){\n");
	ftp_mode = atoi(nvram_safe_get("st_ftp_mode"));
	switch(ftp_mode){
		case 0:
			websWrite(wp, "    return \"off\";\n");
			break;
		case 1:
			websWrite(wp, "    return \"share\";\n");
			break;
		case 2:
			websWrite(wp, "    return \"account\";\n");
			break;
		default:
			fprintf(stderr, "FTP's mode is incorrect!\n");
			websWrite(wp, "    return \"unknown\";\n");
	}
	websWrite(wp, "}\n\n");
	
	return 0;
}

// The parameter from web is "protocol", "mode" and "dummyShareway" is optional.
int ej_set_share_mode(int eid, webs_t wp, int argc, char **argv){
	int samba_mode = atoi(nvram_safe_get("st_samba_mode"));
	int ftp_mode = atoi(nvram_safe_get("st_ftp_mode"));
	char *dummyShareway = websGetVar(wp, "dummyShareway", "");
	char *protocol = websGetVar(wp, "protocol", "");
	char *mode = websGetVar(wp, "mode", "");
	int result;
/*printf("dummyShareway: %s.\n", dummyShareway);
printf("protocol: %s.\n", protocol);
printf("mode: %s.\n", mode);//*/
	if(strlen(dummyShareway) > 0)
		nvram_set("dummyShareway", dummyShareway);
	else
		nvram_set("dummyShareway", "0");
	
	if(strlen(protocol) <= 0){
		show_error_msg("Input1");
		
		websWrite(wp, "<script>\n");
		websWrite(wp, "set_share_mode_error(\'%s\');\n", error_msg);
		websWrite(wp, "</script>\n");
		
		clean_error_msg();
		return -1;
	}
	if(strlen(mode) <= 0){
		show_error_msg("Input3");
		
		websWrite(wp, "<script>\n");
		websWrite(wp, "set_share_mode_error(\'%s\');\n", error_msg);
		websWrite(wp, "</script>\n");
		
		clean_error_msg();
		return -1;
	}
	
	if(!strcmp(mode, "share")){
		if(!strcmp(protocol, "cifs")){
			if(samba_mode == 1 || samba_mode == 3)
				goto SET_SHARE_MODE_SUCCESS;
			
			//nvram_set("st_samba_mode", "3");
			nvram_set("st_samba_mode", "1");	// for test
		}
		else if(!strcmp(protocol, "ftp")){
			if(ftp_mode == 1)
				goto SET_SHARE_MODE_SUCCESS;
			
			nvram_set("st_ftp_mode", "1");
		}
		else{
			show_error_msg("Input2");
			
			websWrite(wp, "<script>\n");
			websWrite(wp, "set_share_mode_error(\'%s\');\n", error_msg);
			websWrite(wp, "</script>\n");
			
			clean_error_msg();
			return -1;
		}
	}
	else if(!strcmp(mode, "account")){
		if(!strcmp(protocol, "cifs")){
			if(samba_mode == 2 || samba_mode == 4)
				goto SET_SHARE_MODE_SUCCESS;
			
			nvram_set("st_samba_mode", "4");
		}
		else if(!strcmp(protocol, "ftp")){
			if(ftp_mode == 2)
				goto SET_SHARE_MODE_SUCCESS;
			
			nvram_set("st_ftp_mode", "2");
		}
		else{
			show_error_msg("Input2");
			
			websWrite(wp, "<script>\n");
			websWrite(wp, "set_share_mode_error(\'%s\');\n", error_msg);
			websWrite(wp, "</script>\n");
			
			clean_error_msg();
			return -1;
		}
	}
	else{
		show_error_msg("Input4");
		
		websWrite(wp, "<script>\n");
		websWrite(wp, "set_share_mode_error(\'%s\');\n", error_msg);
		websWrite(wp, "</script>\n");
		
		clean_error_msg();
		return -1;
	}
	nvram_commit();
	
	if(!strcmp(protocol, "cifs"))
		result = eval("/sbin/run_samba");
	else if(!strcmp(protocol, "ftp"))
		result = eval("/sbin/run_ftp");
	else{
		show_error_msg("Input2");
		
		websWrite(wp, "<script>\n");
		websWrite(wp, "set_share_mode_error(\'%s\');\n", error_msg);
		websWrite(wp, "</script>\n");
		
		clean_error_msg();
		return -1;
	}
	
	if(result != 0){
		show_error_msg("Action8");
		
		websWrite(wp, "<script>\n");
		websWrite(wp, "set_share_mode_error(\'%s\');\n", error_msg);
		websWrite(wp, "</script>\n");
		
		clean_error_msg();
		return -1;
	}
	
SET_SHARE_MODE_SUCCESS:
	websWrite(wp, "<script>\n");
	websWrite(wp, "set_share_mode_success();\n");
	websWrite(wp, "</script>\n");
	return 0;
}

int ej_get_permissions_of_account(int eid, webs_t wp, int argc, char **argv){
	disk_info_t *disks_info, *follow_disk;
	partition_info_t *follow_partition;
	int acc_num = 0, sh_num = 0;
	char **account_list = NULL, **folder_list;
	int samba_right, ftp_right, dms_right;
	int result, i, j;
	int first_pool, first_account, first_folder;
	
	//disks_info = read_disk_data();
	if(DISKS_INFO == NULL)
		DISKS_INFO = read_disk_data();
	disks_info = DISKS_INFO;
	if(disks_info == NULL){
		websWrite(wp, "function get_account_permissions_in_pool(account, pool){return [];}\n");
		websWrite(wp, "function get_dms_permissions_in_pool(pool){return [];}\n");
		return -1;
	}
	
	result = get_account_list(&acc_num, &account_list);
	if(result < 0){
		printf("1. Can't get the account list.\n");
		free_2_dimension_list(&acc_num, &account_list);
		//free_disk_data(&disks_info);
	}

	websWrite(wp, "function get_account_permissions_in_pool(account, pool){\n");
	
	if(acc_num <= 0)
		websWrite(wp, "    return [];\n");
	
	first_account = 1;
	for(i = 0; i < acc_num; ++i){
		websWrite(wp, "    ");
		if(first_account == 1)
			first_account = 0;
		else
			websWrite(wp, "else ");
		
		websWrite(wp, "if(account == \"%s\"){\n", account_list[i]);
		
		first_pool = 1;
		for(follow_disk = disks_info; follow_disk != NULL; follow_disk = follow_disk->next){
			for(follow_partition = follow_disk->partitions; follow_partition != NULL; follow_partition = follow_partition->next){
				if(follow_partition->mount_point != NULL && strlen(follow_partition->mount_point) > 0){
					websWrite(wp, "        ");
					if(first_pool == 1)
						first_pool = 0;
					else
						websWrite(wp, "else ");
					
					websWrite(wp, "if(pool == \"%s\"){\n", rindex(follow_partition->mount_point, '/')+1);
				
					websWrite(wp, "            return [");
					
					result = get_all_folder_in_mount_path(follow_partition->mount_point, &sh_num, &folder_list);
					if(result != 0){
						websWrite(wp, "];\n");
						websWrite(wp, "        }\n");
						
						printf("get_permissions_of_account1: Can't get all folders in \"%s\".\n", follow_partition->mount_point);
						
						free_2_dimension_list(&sh_num, &folder_list);
						
						continue;
					}
					
					first_folder = 1;
					for(j = 0; j < sh_num; ++j){
						samba_right = get_permission(account_list[i],
													 follow_partition->mount_point,
													 folder_list[j],
													 "cifs");
						ftp_right = get_permission(account_list[i],
												   follow_partition->mount_point,
												   folder_list[j],
												   "ftp");
						//samba_right = j%4;
						//ftp_right = j%4;
						if(samba_right < 0 || samba_right > 3){
							printf("Can't get the CIFS permission abount \"%s\"!\n", folder_list[j]);
							
							samba_right = DEFAULT_SAMBA_RIGHT;
						}
						
						if(ftp_right < 0 || ftp_right > 3){
							printf("Can't get the FTP permission abount \"%s\"!\n", folder_list[j]);
							
							ftp_right = DEFAULT_FTP_RIGHT;
						}
						
						if(first_folder == 1){
							first_folder = 0;
							websWrite(wp, "[\"%s\", %d, %d]", folder_list[j], samba_right, ftp_right);
						}
						else
							websWrite(wp, "                    [\"%s\", %d, %d]", folder_list[j], samba_right, ftp_right);
						
						if(j != sh_num-1)
							websWrite(wp, ",\n");
					}
					
					websWrite(wp, "];\n");
					websWrite(wp, "        }\n");
				}
			}
		}
		
		websWrite(wp, "    }\n");
	}

	websWrite(wp, "}\n\n");
	
	if(sh_num > 0)
		free_2_dimension_list(&sh_num, &folder_list);
	
	websWrite(wp, "function get_dms_permissions_in_pool(pool){\n");
	
	first_pool = 1;
	for(follow_disk = disks_info; follow_disk != NULL; follow_disk = follow_disk->next){
		for(follow_partition = follow_disk->partitions; follow_partition != NULL; follow_partition = follow_partition->next){
			if(follow_partition->mount_point != NULL && strlen(follow_partition->mount_point) > 0){
				websWrite(wp, "    ");
				if(first_pool == 1)
					first_pool = 0;
				else
					websWrite(wp, "else ");
				websWrite(wp, "if(pool == \"%s\"){\n", rindex(follow_partition->mount_point, '/')+1);
				
				websWrite(wp, "        return [");
				
				result = get_all_folder_in_mount_path(follow_partition->mount_point, &sh_num, &folder_list);
				if(result != 0){
					websWrite(wp, "];\n");
					websWrite(wp, "    }\n");
					
					printf("get_permissions_of_account2: Can't get all folders in \"%s\".\n", follow_partition->mount_point);
					
					free_2_dimension_list(&sh_num, &folder_list);
					
					continue;
				}
				
				first_folder = 1;
				for(j = 0; j < sh_num; ++j){
					dms_right = get_permission("MediaServer",
											   follow_partition->mount_point,
											   folder_list[j],
											   "dms");
					if(dms_right < 0 || dms_right > 1){
						printf("Can't get the DMS permission abount \"%s\"!\n", folder_list[j]);
						
						dms_right = DEFAULT_DMS_RIGHT;
					}
					
					if(first_folder == 1){
						first_folder = 0;
						websWrite(wp, "[\"%s\", %d]", folder_list[j], dms_right);
					}
					else
						websWrite(wp, "                [\"%s\", %d]", folder_list[j], dms_right);
					
					if(j != sh_num-1)
						websWrite(wp, ",\n");
				}
				
				websWrite(wp, "];\n");
				websWrite(wp, "    }\n");
			}
		}
	}
	
	websWrite(wp, "}\n\n");
	
	if(acc_num > 0)
		free_2_dimension_list(&acc_num, &account_list);
	if(sh_num > 0)
		free_2_dimension_list(&sh_num, &folder_list);
	/*if(disks_info != NULL)
		free_disk_data(&disks_info);//*/
	
	return 0;
}

// The parameter from web is "account", "pool", "folder", "protocol", "permission".
int ej_set_account_permission(int eid, webs_t wp, int argc, char **argv){
	char *mount_path;
	char *account = websGetVar(wp, "account", "");
	char *pool = websGetVar(wp, "pool", "");
	char *folder = websGetVar(wp, "folder", "");
	char *protocol = websGetVar(wp, "protocol", "");
	char *permission = websGetVar(wp, "permission", "");
	int right;
	
	if(strlen(account) <= 0){
		show_error_msg("Input5");
		
		websWrite(wp, "<script>\n");
		websWrite(wp, "set_account_permission_error(\'%s\');\n", error_msg);
		websWrite(wp, "</script>\n");
		
		clean_error_msg();
		return -1;
	}
	if(test_if_exist_account(account) != 1){
		show_error_msg("Input6");
		
		websWrite(wp, "<script>\n");
		websWrite(wp, "set_account_permission_error(\'%s\');\n", error_msg);
		websWrite(wp, "</script>\n");
		
		clean_error_msg();
		return -1;
	}
	
	if(strlen(pool) <= 0){
		show_error_msg("Input7");
		
		websWrite(wp, "<script>\n");
		websWrite(wp, "set_account_permission_error(\'%s\');\n", error_msg);
		websWrite(wp, "</script>\n");
		
		clean_error_msg();
		return -1;
	}
	if(get_mount_path(pool, &mount_path) < 0){
		fprintf(stderr, "Can't get the mount_path of %s.\n", pool);
		
		show_error_msg("System1");
		
		websWrite(wp, "<script>\n");
		websWrite(wp, "set_account_permission_error(\'%s\');\n", error_msg);
		websWrite(wp, "</script>\n");
		
		clean_error_msg();
		return -1;
	}
	
	if(strlen(folder) <= 0){
		show_error_msg("Input9");
		
		websWrite(wp, "<script>\n");
		websWrite(wp, "set_account_permission_error(\'%s\');\n", error_msg);
		websWrite(wp, "</script>\n");
		free(mount_path);
		
		clean_error_msg();
		return -1;
	}
	/*if(test_if_exist_folder_in_mount_path(mount_path, folder) != 1){
		show_error_msg("Input11");
		
		websWrite(wp, "<script>\n");
		websWrite(wp, "set_account_permission_error(\'%s\');\n", error_msg);
		websWrite(wp, "</script>\n");
		free(mount_path);
		
		clean_error_msg();
		return -1;
	}//*/
	
	if(strlen(protocol) <= 0){
		show_error_msg("Input1");
		
		websWrite(wp, "<script>\n");
		websWrite(wp, "set_account_permission_error(\'%s\');\n", error_msg);
		websWrite(wp, "</script>\n");
		free(mount_path);
		
		clean_error_msg();
		return -1;
	}
	if(strcmp(protocol, "cifs") && strcmp(protocol, "ftp") && strcmp(protocol, "dms")){
		show_error_msg("Input2");
		
		websWrite(wp, "<script>\n");
		websWrite(wp, "set_account_permission_error(\'%s\');\n", error_msg);
		websWrite(wp, "</script>\n");
		free(mount_path);
		
		clean_error_msg();
		return -1;
	}
	
	if(strlen(permission) <= 0){
		show_error_msg("Input12");
		
		websWrite(wp, "<script>\n");
		websWrite(wp, "set_account_permission_error(\'%s\');\n", error_msg);
		websWrite(wp, "</script>\n");
		free(mount_path);
		
		clean_error_msg();
		return -1;
	}
	right = atoi(permission);
	if(right < 0 || right > 3){
		show_error_msg("Input13");
		
		websWrite(wp, "<script>\n");
		websWrite(wp, "set_account_permission_error(\'%s\');\n", error_msg);
		websWrite(wp, "</script>\n");
		free(mount_path);
		
		clean_error_msg();
		return -1;
	}
	
	if(set_permission(account, mount_path, folder, protocol, right) < 0){
		show_error_msg("Action1");
		
		websWrite(wp, "<script>\n");
		websWrite(wp, "set_account_permission_error(\'%s\');\n", error_msg);
		websWrite(wp, "</script>\n");
		free(mount_path);
		
		clean_error_msg();
		return -1;
	}
	
	websWrite(wp, "<script>\n");
	websWrite(wp, "set_account_permission_success();\n");
	websWrite(wp, "</script>\n");
	free(mount_path);
	return 0;
}

int ej_initial_account(int eid, webs_t wp, int argc, char **argv){
	disk_info_t *disks_info, *follow_disk;
	partition_info_t *follow_partition;
	char *command;
	int len, result;
	
	nvram_set("acc_num", "0");
	nvram_commit();
	
	//disks_info = read_disk_data();
	if(DISKS_INFO == NULL)
		DISKS_INFO = read_disk_data();
	disks_info = DISKS_INFO;
	if(disks_info == NULL){
		show_error_msg("System2");
		
		websWrite(wp, "<script>\n");
		websWrite(wp, "initial_account_error(\'%s\');\n", error_msg);
		websWrite(wp, "</script>\n");
		
		clean_error_msg();
		return -1;
	}
	
	for(follow_disk = disks_info; follow_disk != NULL; follow_disk = follow_disk->next)
		for(follow_partition = follow_disk->partitions; follow_partition != NULL; follow_partition = follow_partition->next)
			if(follow_partition->mount_point != NULL && strlen(follow_partition->mount_point) > 0){
				len = strlen("rm -f ")+strlen(follow_partition->mount_point)+strlen("/.__*");
				command = (char *)malloc(sizeof(char)*(len+1));
				if(command == NULL){
					show_error_msg("System1");
					
					websWrite(wp, "<script>\n");
					websWrite(wp, "initial_account_error(\'%s\');\n", error_msg);
					websWrite(wp, "</script>\n");
					
					clean_error_msg();
					return -1;
				}
				sprintf(command, "rm -f %s/.__*", follow_partition->mount_point);
				command[len] = 0;
				
				result = system(command);
				free(command);
				
				initial_folder_list_in_mount_path(follow_partition->mount_point);
				initial_all_var_file_in_mount_path(follow_partition->mount_point);
			}
	
	//free_disk_data(&disks_info);
	
	if(eval("/sbin/run_ftpsamba") != 0){
		show_error_msg("System1");
		
		websWrite(wp, "<script>\n");
		websWrite(wp, "initial_account_error(\'%s\');\n", error_msg);
		websWrite(wp, "</script>\n");
		
		clean_error_msg();
		return -1;
	}
	
	websWrite(wp, "<script>\n");
	websWrite(wp, "initial_account_success();\n");
	websWrite(wp, "</script>\n");
	
	return 0;
}

// The parameter from web is "account", "password".
int ej_create_account(int eid, webs_t wp, int argc, char **argv){
	char *account = websGetVar(wp, "account", "");
	char *password = websGetVar(wp, "password", "");
/*printf("account: %s.\n", account);
printf("password: %s.\n", password);*/
	if(strlen(account) <= 0){
		show_error_msg("Input5");
		
		websWrite(wp, "<script>\n");
		websWrite(wp, "create_account_error(\'%s\');\n", error_msg);
		websWrite(wp, "</script>\n");
		
		clean_error_msg();
		return -1;
	}
	if(strlen(password) <= 0){
		show_error_msg("Input14");
		
		websWrite(wp, "<script>\n");
		websWrite(wp, "create_account_error(\'%s\');\n", error_msg);
		websWrite(wp, "</script>\n");
		
		clean_error_msg();
		return -1;
	}
	
	if(add_account(account, password) < 0){
		show_error_msg("Action2");
		
		websWrite(wp, "<script>\n");
		websWrite(wp, "create_account_error(\'%s\');\n", error_msg);
		websWrite(wp, "</script>\n");
		
		clean_error_msg();
		return -1;
	}
	
	websWrite(wp, "<script>\n");
	websWrite(wp, "create_account_success();\n");
	websWrite(wp, "</script>\n");
	
	return 0;
}

// The parameter from web is "account".
int ej_delete_account(int eid, webs_t wp, int argc, char **argv){
	char *account = websGetVar(wp, "account", "");
	
	if(strlen(account) <= 0){
		show_error_msg("Input5");
		
		websWrite(wp, "<script>\n");
		websWrite(wp, "delete_account_error(\'%s\');\n", error_msg);
		websWrite(wp, "</script>\n");
		
		clean_error_msg();
		return -1;
	}
	
	if(del_account(account) < 0){
		show_error_msg("Action3");
		
		websWrite(wp, "<script>\n");
		websWrite(wp, "delete_account_error(\'%s\');\n", error_msg);
		websWrite(wp, "</script>\n");
		
		clean_error_msg();
		return -1;
	}
	
	websWrite(wp, "<script>\n");
	websWrite(wp, "delete_account_success();\n");
	websWrite(wp, "</script>\n");
	return 0;
}

// The parameter from web is "account", "new_account", "new_password".
int ej_modify_account(int eid, webs_t wp, int argc, char **argv){
	char *account = websGetVar(wp, "account", "");
	char *new_account = websGetVar(wp, "new_account", "");
	char *new_password = websGetVar(wp, "new_password", "");
	
	if(strlen(account) <= 0){
		show_error_msg("Input5");
		
		websWrite(wp, "<script>\n");
		websWrite(wp, "modify_account_error(\'%s\');\n", error_msg);
		websWrite(wp, "</script>\n");
		
		clean_error_msg();
		return -1;
	}
	if(strlen(new_account) <= 0 && strlen(new_password) <= 0){
		show_error_msg("Input16");
		
		websWrite(wp, "<script>\n");
		websWrite(wp, "modify_account_error(\'%s\');\n", error_msg);
		websWrite(wp, "</script>\n");
		
		clean_error_msg();
		return -1;
	}
	
	if(mod_account(account, new_account, new_password) < 0){
		show_error_msg("Action4");
		
		websWrite(wp, "<script>\n");
		websWrite(wp, "modify_account_error(\'%s\');\n", error_msg);
		websWrite(wp, "</script>\n");
		
		clean_error_msg();
		return -1;
	}
	
	websWrite(wp, "<script>\n");
	websWrite(wp, "modify_account_success();\n");
	websWrite(wp, "</script>\n");
	return 0;
}

// The parameter from web is "pool", "folder".
int ej_create_sharedfolder(int eid, webs_t wp, int argc, char **argv){
	char *pool = websGetVar(wp, "pool", "");
	char *folder = websGetVar(wp, "folder", "");
	char *mount_path;
	
	if(strlen(pool) <= 0){
		show_error_msg("Input7");
		
		websWrite(wp, "<script>\n");
		websWrite(wp, "create_sharedfolder_error(\'%s\');\n", error_msg);
		websWrite(wp, "</script>\n");
		
		clean_error_msg();
		return -1;
	}
	if(strlen(folder) <= 0){
		show_error_msg("Input9");
		
		websWrite(wp, "<script>\n");
		websWrite(wp, "create_sharedfolder_error(\'%s\');\n", error_msg);
		websWrite(wp, "</script>\n");
		
		clean_error_msg();
		return -1;
	}
	
	if(get_mount_path(pool, &mount_path) < 0){
		fprintf(stderr, "Can't get the mount_path of %s.\n", pool);
		
		show_error_msg("System1");
		
		websWrite(wp, "<script>\n");
		websWrite(wp, "create_sharedfolder_error(\'%s\');\n", error_msg);
		websWrite(wp, "</script>\n");
		
		clean_error_msg();
		return -1;
	}
	
	if(add_folder(mount_path, folder) < 0){
		show_error_msg("Action5");
		
		websWrite(wp, "<script>\n");
		websWrite(wp, "create_sharedfolder_error(\'%s\');\n", error_msg);
		websWrite(wp, "</script>\n");
		free(mount_path);
		
		clean_error_msg();
		return -1;
	}
	free(mount_path);
	
	if(eval("/sbin/run_samba") != 0){
		show_error_msg("Action5");
		
		websWrite(wp, "<script>\n");
		websWrite(wp, "create_sharedfolder_error(\'%s\');\n", error_msg);
		websWrite(wp, "</script>\n");
		free(mount_path);
		
		clean_error_msg();
		return -1;
	}
	
	websWrite(wp, "<script>\n");
	websWrite(wp, "create_sharedfolder_success();\n");
	websWrite(wp, "</script>\n");
	return 0;
}

// The parameter from web is "pool", "folder".
int ej_delete_sharedfolder(int eid, webs_t wp, int argc, char **argv){
	char *pool = websGetVar(wp, "pool", "");
	char *folder = websGetVar(wp, "folder", "");
	char *mount_path;
	
	if(strlen(pool) <= 0){
		show_error_msg("Input7");
		
		websWrite(wp, "<script>\n");
		websWrite(wp, "delete_sharedfolder_error(\'%s\');\n", error_msg);
		websWrite(wp, "</script>\n");
		
		clean_error_msg();
		return -1;
	}
	if(strlen(folder) <= 0){
		show_error_msg("Input9");
		
		websWrite(wp, "<script>\n");
		websWrite(wp, "delete_sharedfolder_error(\'%s\');\n", error_msg);
		websWrite(wp, "</script>\n");
		
		clean_error_msg();
		return -1;
	}
	
	if(get_mount_path(pool, &mount_path) < 0){
		show_error_msg("System1");
		
		websWrite(wp, "<script>\n");
		websWrite(wp, "delete_sharedfolder_error(\'%s\');\n", error_msg);
		websWrite(wp, "</script>\n");
		
		clean_error_msg();
		return -1;
	}
	
	if(del_folder(mount_path, folder) < 0){
		show_error_msg("Action6");
		
		websWrite(wp, "<script>\n");
		websWrite(wp, "delete_sharedfolder_error(\'%s\');\n", error_msg);
		websWrite(wp, "</script>\n");
		free(mount_path);
		
		clean_error_msg();
		return -1;
	}
	free(mount_path);
	
	if(eval("/sbin/run_ftpsamba") != 0){
		show_error_msg("Action6");
		
		websWrite(wp, "<script>\n");
		websWrite(wp, "delete_sharedfolder_error(\'%s\');\n", error_msg);
		websWrite(wp, "</script>\n");
		free(mount_path);
		
		clean_error_msg();
		return -1;
	}
	
	websWrite(wp, "<script>\n");
	websWrite(wp, "delete_sharedfolder_success();\n");
	websWrite(wp, "</script>\n");
	return 0;
}

// The parameter from web is "pool", "folder", "new_folder".
int ej_modify_sharedfolder(int eid, webs_t wp, int argc, char **argv){
	char *pool = websGetVar(wp, "pool", "");
	char *folder = websGetVar(wp, "folder", "");
	char *new_folder = websGetVar(wp, "new_folder", "");
	char *mount_path;
	
	if(strlen(pool) <= 0){
		show_error_msg("Input7");
		
		websWrite(wp, "<script>\n");
		websWrite(wp, "modify_sharedfolder_error(\'%s\');\n", error_msg);
		websWrite(wp, "</script>\n");
		
		clean_error_msg();
		return -1;
	}
	if(strlen(folder) <= 0){
		show_error_msg("Input9");
		
		websWrite(wp, "<script>\n");
		websWrite(wp, "modify_sharedfolder_error(\'%s\');\n", error_msg);
		websWrite(wp, "</script>\n");
		
		clean_error_msg();
		return -1;
	}
	if(strlen(new_folder) <= 0){
		show_error_msg("Input17");
		
		websWrite(wp, "<script>\n");
		websWrite(wp, "modify_sharedfolder_error(\'%s\');\n", error_msg);
		websWrite(wp, "</script>\n");
		
		clean_error_msg();
		return -1;
	}
	
	if(get_mount_path(pool, &mount_path) < 0){
		show_error_msg("System1");
		
		websWrite(wp, "<script>\n");
		websWrite(wp, "modify_sharedfolder_error(\'%s\');\n", error_msg);
		websWrite(wp, "</script>\n");
		
		clean_error_msg();
		return -1;
	}
	
	if(mod_folder(mount_path, folder, new_folder) < 0){
		show_error_msg("Action7");
		
		websWrite(wp, "<script>\n");
		websWrite(wp, "modify_sharedfolder_error(\'%s\');\n", error_msg);
		websWrite(wp, "</script>\n");
		free(mount_path);
		
		clean_error_msg();
		return -1;
	}
	free(mount_path);
	
	if(eval("/sbin/run_ftpsamba") != 0){
		show_error_msg("Action7");
		
		websWrite(wp, "<script>\n");
		websWrite(wp, "modify_sharedfolder_error(\'%s\');\n", error_msg);
		websWrite(wp, "</script>\n");
		free(mount_path);
		
		clean_error_msg();
		return -1;
	}
	
	websWrite(wp, "<script>\n");
	websWrite(wp, "modify_sharedfolder_success();\n");
	websWrite(wp, "</script>\n");
	return 0;
}
#endif

int ej_shown_time(int eid, webs_t wp, int argc, char **argv){
	time_t t1;
	
	time(&t1);
	
	websWrite(wp, "%e", t1);
	
	return 0;
}

int ej_shown_language_option(int eid, webs_t wp, int argc, char **argv){
#ifdef TRANSLATE_ON_FLY
	struct language_table *pLang = NULL;
	char lang[4];
	int i, len;
	FILE *fp = fopen("EN.dict", "r");
	char buffer[1024], key[16], target[16];
	char *follow_info, *follow_info_end;
	
	if(fp == NULL){
		fprintf(stderr, "No English dictionary!\n");
		return 0;
	}
	
	memset(lang, 0, 4);
	strcpy(lang, nvram_safe_get("preferred_lang"));
	
	for(i = 0; i < 20; ++i){
		memset(buffer, 0, sizeof(buffer));
		if((follow_info = fgets(buffer, sizeof(buffer), fp)) != NULL){
			if(strncmp(follow_info, "LANG_", 5))	// 5 = strlen("LANG_")
				continue;
			
			follow_info += 5;
			follow_info_end = strstr(follow_info, "=");
			len = follow_info_end-follow_info;
			memset(key, 0, sizeof(key));
			strncpy(key, follow_info, len);
			
			for(pLang = language_tables; pLang->Lang != NULL; ++pLang){
				if(strcmp(key, pLang->Target_Lang))
					continue;
				
				follow_info = follow_info_end+1;
				follow_info_end = strstr(follow_info, "\n");
				len = follow_info_end-follow_info;
				memset(target, 0, sizeof(target));
				strncpy(target, follow_info, len);
				
				if(!strcmp(key, lang))
					websWrite(wp, "<option value=\"%s\" selected>%s</option>\\n", key, target);
				else
					websWrite(wp, "<option value=\"%s\">%s</option>\\n", key, target);
				break;
			}
		}
		else
			break;
	}
	fclose(fp);
#else
	websWrite(wp, "<option value=\"EN\">English</option>\\n");
#endif
	return 0;
}

static int
ej_nvram_char_to_ascii(int eid, webs_t wp, int argc, char_t **argv)
{
	char *sid, *name, *c;
	int ret = 0;
	
	if (ejArgs(argc, argv, "%s %s", &sid, &name) < 2) {
		websError(wp, 400, "Insufficient args\n");
		return -1;
	}
	
	char tmpstr[256];
	memset(tmpstr, 0x0, sizeof(tmpstr));
	char_to_ascii(tmpstr, nvram_safe_get_x(sid, name));
	ret += websWrite(wp, "%s", tmpstr);
	
	return ret;
}
// 2007.10 James }

static int
apply_cgi(webs_t wp, char_t *urlPrefix, char_t *webDir, int arg,
		char_t *url, char_t *path, char_t *query)
{
	int sid;
	char *value;
	char *current_url;
	char *next_url;
	char *sid_list;
	char *value1;
	char *script;
	char groupId[64];
	char urlStr[64];
	char *groupName;
	char *host;

	//if(!query)
	//	goto footer;
	
	urlStr[0] = 0;
	
	value = websGetVar(wp, "action", "");
	
	//if (strcmp(value, "")==0)
	value = websGetVar(wp, "action_mode","");
	
	//next_host = websGetVar(wp, "next_host", "");
	//cprintf("host:%s\n", next_host);
	current_url = websGetVar(wp, "current_page", "");
	next_url = websGetVar(wp, "next_page", "");
	script = websGetVar(wp, "action_script","");
	
	//printf("Script: %s\n", script);
	//cprintf("Apply: %s %s %s %s\n", value, current_url, next_url, websGetVar(wp, "group_id", ""));
	
	if(!strcmp(value," Refresh "))
	{
		strcpy(SystemCmd, websGetVar(wp,"SystemCmd",""));
printf("1. SystemCmd: %s.\n", SystemCmd);
		websRedirect(wp, current_url);
		return 0;
	}
	else if(!strcmp(value," Clear "))
	{
		// current only syslog implement this button
		unlink("/tmp/syslog.log-1");
		unlink("/tmp/syslog.log");
		websRedirect(wp, current_url);
		return 0;
	}
	else if(!strcmp(value,"NEXT"))
	{
		websRedirect(wp, next_url);
		return 0;
	}
	else if(!strcmp(value, "Save&Restart "))
	{
		/*action = ACTION_SAVE_RESTART;*/
		//websRedirect(wp, "Restarting.asp");
		websApply(wp, "Restarting.asp");
		nvram_set_f("General", "x_Setting", "1");
		firsttime = -1; //2009.06 James
		nvram_set("httpd_die_reboot", "1");
		
		eval("early_convert");
		
		sys_commit(NULL);
		//sys_script("bcm_set");
		//system("/web/script/eject-usb.sh");
		sys_reboot();
		return(0);
	}
	else if(!strcmp(value, " Restart "))
	{
		websApply(wp, "Restarting.asp");
		nvram_set("httpd_die_reboot", "1");
		sys_reboot();
		return(0);
	}
	else if(!strcmp(value, "Restore"))
	{
		/* action = ACTION_RESET_DEFAULT;*/
		//websRedirect(wp, "Restarting.asp");
		websApply(wp, "Restarting.asp");
		//system("/web/script/eject-usb.sh");
		sys_default();
		sys_reboot();
		return(0);
	}
	else if(!strcmp(value, "WlanUpdate "))
	{
		/*action = ACTION_SAVE_RESTART;*/
		/*websRedirect(wp, "Restarting.asp");*/
		if(strcmp(script, ""))
		{
			system(script);
		}
		websApply(wp, "Restarting.asp");
		sys_reboot();
		return(0);
	}
	else
	{
		sid_list = websGetVar(wp, "sid_list", "");
		while((serviceId = svc_pop_list(sid_list, ';')))
		{
			sid = 0;
			while(GetServiceId(sid) != 0)
			{
				if(!strcmp(GetServiceId(sid), serviceId))
					break;
				
				++sid;
			}
			
			if(serviceId != NULL)
			{
				if(!strcmp(value, " Restore "))
				{
					//sys_restore(serviceId);
				}
				else if(!strcmp(value, "  Save  ") || !strcmp(value, " Apply "))
				{
#ifndef NOUSB
					strcpy(groupId, websGetVar(wp, "group_id", ""));
					if(!strcmp(current_url, "Advanced_StorageUser_Content.asp") &&
							!strcmp(groupId, "Storage_UserRight_List"))
					{
						char tmp_value1[145], tmp_value2[145], tmp_value3[145], tmp_value4[145];
						
						strcpy(tmp_value1, websGetVar(wp, "sh_name_x",""));
						strcpy(tmp_value2, websGetVar(wp, "sh_comment_x",""));
						strcpy(tmp_value3, websGetVar(wp, "sh_nameb_x",""));
						strcpy(tmp_value4, websGetVar(wp, "sh_commentb_x",""));
						setSharedEntry(tmp_value1, tmp_value2, tmp_value3, tmp_value4);
					}
					
					if(!strcmp(current_url, "Advanced_Password_Content.asp"))
						nvram_set("temp_lang", websGetVar(wp, "preferred_lang", ""));
#endif
					validate_cgi(wp, sid, TRUE);
				}
				else if(!strcmp(value, "Set") || !strcmp(value, "Unset") || !strcmp(value, "Update") || !strcmp(value, "Eject")
#ifdef ASUS_DDNS
						|| !strcmp(value, "Check")
#endif
						)
				{
					validate_cgi(wp, sid, TRUE);
				}
				else if(!strcmp(value," Finish "))
				{
					validate_cgi(wp, sid, TRUE);
				}
				else
				{
					cprintf("group ?\n");
					strcpy(groupId,websGetVar(wp, "group_id", ""));
					
					if((value1 = websGetVar(wp, "action_mode", NULL)) != NULL)
					{
						groupName = groupId;
						
						//if (!strcmp(GetServiceId(sid), groupId))
						{
							if(!strncmp(value1, " Delete ", 8))
							{
								apply_cgi_group(wp, sid, NULL, groupName, GROUP_FLAG_DELETE);
							}
// 2005.12.22 Jiahao add for 500gP
#ifndef NOUSB
							else if(!strncmp(value1, " Mkdir ", 7))
							{
								if(!strcmp(current_url, "Advanced_StorageRight_Content.asp") &&
										!strcmp(groupId, "Storage_SharedList"))
								{
									sprintf(OpenDirPath, "%s%s", CurrentDirPath, websGetVar(wp, "sh_path_0",""));
									umask(0000);
									if(mkdir(OpenDirPath, 0777) == NULL)
									{
										apply_cgi_group(wp, sid, NULL, groupName, GROUP_FLAG_ADD);
										initSharedEntry();
									}
								}
							}
#endif
							else if(!strncmp(value1, " Add ", 5))
							{
#ifndef NOUSB
								if(!strcmp(current_url, "Advanced_StorageRight_Content.asp") &&
										!strcmp(groupId, "Storage_SharedList"))
								{
									apply_cgi_group(wp, sid, NULL, groupName, GROUP_FLAG_ADD);
									initSharedEntry();
								}
								else if(!strcmp(current_url, "Advanced_StorageUser_Content.asp") &&
										!strcmp(groupId, "Storage_UserRight_List"))
								{
									apply_right_group(wp, sid, NULL, groupName, GROUP_FLAG_ADD);
								}
								else
#endif
									apply_cgi_group(wp, sid, NULL, groupName, GROUP_FLAG_ADD);
							}
							else if(!strncmp(value1, " Del ", 5))
							{
#ifdef DLM	// 2005.12.16 Yau add for 500gP
								if(!strcmp(current_url, "Advanced_StorageRight_Content.asp") &&
										!strcmp(groupId, "Storage_SharedList"))
								{
									delSharedEntry(delMap);
								}
								else if(!strcmp(current_url, "Advanced_StorageUser_Content.asp") &&
										!strcmp(groupId, "Storage_UserRight_List"))
								{
									apply_right_group(wp, sid, NULL, groupName, GROUP_FLAG_REMOVE);
								}
								else
									apply_cgi_group(wp, sid, NULL, groupName, GROUP_FLAG_REMOVE);
#else
								apply_cgi_group(wp, sid, NULL, groupName, GROUP_FLAG_REMOVE);
#endif
							}
							else if(!strncmp(value1, " Refresh ", 9))
							{
								apply_cgi_group(wp, sid, NULL, groupName, GROUP_FLAG_REFRESH);
							}
#ifndef NOUSB
							else if(!strncmp(value1, " Edit ", 6))
							{
								//apply_cgi_group(wp, sid, NULL, groupName, GROUP_FLAG_REFRESH);
								cprintf("edit %d\n", delMap[0]);
								if(delMap[0] == -1)
									websRedirect(wp, current_url);
								else
								{
									getSharedEntry(delMap[0]);
									websRedirect(wp, "Advanced_StorageUser_Content.asp");
								}
								
								return 0;
							}
							else if(!strncmp(value1, " Modify ", 8))
							{
								if(!strcmp(current_url, "Advanced_StorageUser_Content.asp") &&
										!strcmp(groupId, "Storage_UserRight_List"))
								{
									modify_right_group(wp, sid, NULL, groupName);
									websRedirect(wp, "Advanced_StorageUser_Content.asp");
								}
								
								return 0;
							}
#endif
							sprintf(urlStr, "%s#%s", current_url, groupName);
							validate_cgi(wp, sid, FALSE);
						}
					}
				}
			}
			
			sid_list = sid_list+strlen(serviceId)+1;
		}
		
		//printf("apply????\n");
		
		/* Add for Internet Access Quick Setup */
		//special_handler(wp, url);
		
		/* Add for NVRAM mapping if necessary */
		//nvMap(current_url);
		
		/* Add for EMI Test page */
		if(strcmp(script, ""))
		{
#ifdef DLM	// 2005.12.23 Jiahao add for 500gP
			if(!strcmp(script, "usbfs_check"))
			{
				nvram_set("st_tool_t", "/bin/fscheck");
				nvram_set("st_toolstatus_t", "USBstarting");
				nvram_set("st_time_t", "3");
				//system("/bin/fscheck");
				websRedirect(wp, "Checking.asp");
				deliver_signal("/var/run/infosvr.pid", SIGUSR1);
				return 0;
			}
			else if(!strcmp(script, "usbfs_eject"))
				umount_disc_parts(1);
			else if(!strcmp(script, "usbfs_eject2"))
				umount_disc_parts(2);
			else
#endif
				sys_script(script);
		}
		
		/*if(!strcmp(value, "TOS_Confirm")){
//		if(!strcmp(current_url, "Advanced_ASUSDDNSTOS_Content.asp")){
			nvram_set("asusddns_tos_agreement", "1");
			nvram_commit();
//		}

			strcpy(urlcache, next_url);
			websRedirect(wp, next_url);
		}//*/
		
		if(!strcmp(value, "  Save  ") || !strcmp(value, " Apply "))
		{
			strcpy(urlcache, next_url);
			websRedirect(wp, next_url);
		}
		else if(!strcmp(value, " Finish "))
			websRedirect(wp, "SaveRestart.asp");
		else if(urlStr[0] == 0)
			websRedirect(wp, current_url);
		else
			websRedirect(wp, urlStr);
		
		cprintf("apply ok\n");
		return 0;
	}
	
footer:
#ifndef NOUSB
	websWrite(wp, (char_t *) apply_footer);
	websFooter(wp);
	websDone(wp, 200);
	
#ifndef FLASH_EMULATOR
	if(action == RESTART)
		sys_restart();
	else if(action == REBOOT)
		sys_reboot();
#endif
#endif
	return 1;
}

#ifdef REMOVE_WL600
void nvram_add_group_list(webs_t wp, struct variable *v, int count)
{
    struct variable *gv;
    char *value;
    char name[64];
    int i;
    
    if (v->argv[0]==NULL) 
    {
       return;
    }
    
    i = 0;
    for (gv = (struct variable *)v->argv[0]; gv->name!=NULL; gv++)
    {    		    	
    	sprintf(name, "%s_%s_%d", serviceId, gv->name, count+1);    	
//Yau
//printf("name: %s\n",name);
        if ((value=websGetVar(wp, name, NULL)))
        {           	
            if (groupItem[i]==0) {
    		groupItem[i]=(char*)malloc(MAX_LINE_SIZE);
    		sprintf(groupItem[i],"%s;",value);
    	    }  	
    	    else sprintf(groupItem[i],"%s%s;",groupItem[i], value);
            /* Don't check validate. Left validate to JavaScript */                        
//Yau
//printf("groupItem1 :: %s\n",groupItem[i]);
        }   
        else
        {
            if (i==0) return;	
            if (groupItem[i]==0) {
    		groupItem[i]=(char*)malloc(MAX_LINE_SIZE);
    		sprintf(groupItem[i],";");
    	    }	
            else sprintf(groupItem[i],"%s;",groupItem[i]);
//Yau
//printf("groupItem2 :: %s\n",groupItem[i]);
        }   
        i++;
    }   
    return;
}

void nvram_apply_group_list(webs_t wp, struct variable *v)
{
    struct variable *gv;
    int i;
        
    if (v->argv[0]==NULL) 
    {
       return;
    }

    i=0;
    for (gv = (struct variable *)v->argv[0]; gv->name!=NULL; gv++)
    {
    	if (groupItem[i]!=NULL)
    	{
    	   nvram_set_x(serviceId, gv->name, groupItem[i]);
    	   free(groupItem[i]);
    	   groupItem[i]=NULL;
    	}   
    	i++;
    }   
    return;
}
#endif

void nvram_add_group_item(webs_t wp, struct variable *v, int sid)
{
    struct variable *gv;
    char *value;
    char name[64], cstr[5];    
    int count;
    
    if (v->argv[0]==NULL) 
    {
       return;
    }
                
//Yau
//printf("+++ add_group_item ++\n");
    count = atoi(nvram_safe_get_x(serviceId, v->argv[3]));
    cprintf("count: %d\n", count);
    
    for (gv = (struct variable *)v->argv[0]; gv->name!=NULL; gv++)
    {    	    		    	    	
    	sprintf(name, "%s_0", gv->name);    	
    	    	    	    	    	    	
        if ((value=websGetVar(wp, name, NULL)))
        {                    
            /*SetGroupVariable(sid, v, gv->name, value, "Add");*/
            nvram_add_lists_x(serviceId, gv->name, value, count);            
            cprintf("Add: %s %s\n", gv->name, value);
        }   
        else
        {        	
            /*SetGroupVariable(sid, v, gv->name, "", "Add");  */
            nvram_add_lists_x(serviceId, gv->name, "", count);             
        }                      
    }   
    
    count++;    
    sprintf(cstr, "%d", count);       
    nvram_set_x(serviceId, v->argv[3], cstr);
    return;
}


void nvram_remove_group_item(webs_t wp, struct variable *v, int sid, int *delMap)
{
    struct variable *gv;
    char *value;
    char glist[MAX_LINE_SIZE], *itemPtr, *glistPtr, cstr[5];    
    int item, i, count;
//Yau
//printf("+++ remove group item +++\n");
    if (v->argv[0]==NULL) 
    {
       return;
    }
    
    count = atoi(nvram_safe_get_x(serviceId, v->argv[3]));    
    for (gv = (struct variable *)v->argv[0]; gv->name!=NULL; gv++)
    {    	    		    	     	    	
//Yau
//printf("gv->name: %s, delMap[0]=%d\n",gv->name,delMap[0]);
    	nvram_del_lists_x(serviceId, gv->name, delMap);    		    	       
    }           
    
    i=0;
    while(delMap[i]!=-1) i++;
    
    count-=i;
    sprintf(cstr, "%d", count);
//Yau
//printf("del cstr=%s\n",cstr);
    nvram_set_x(serviceId, v->argv[3], cstr);
    return;
}

#ifndef NOUSB
void nvram_add_right_item(webs_t wp, struct variable *v, int sid)
{
    struct variable *gv;
    char *value1, *value2;
    char name[64], cstr[5];
    int count;
                                                                                                                                              
    if (v->argv[0]==NULL)
    {
       return;
    }
                                                                                                                                              
//Yau
//printf("+++ add_right_item ++\n");
    count = atoi(nvram_safe_get_x(serviceId, v->argv[3]));
//    printf("count: %d\n", count);

    for (gv = (struct variable *)v->argv[0]; gv->name!=NULL; gv++)
    {
        sprintf(name, "%s_0", gv->name);

        if ((value1=websGetVar(wp, name, NULL)))
        {
//Yau
//printf("Get Web: %s = %s\n",name, value1);

            	if ((value2=websGetVar(wp, "sh_acc_right_0", NULL)))
        	{
//Yau
//printf("Get Web: sh_acc_right = %s\n",value2);
            		nvram_add_right_list(gv->name, value1, value2);
        	}
    	}
    }                                                                                                                                          
    count++;
    sprintf(cstr, "%d", count);
    nvram_set_x(serviceId, v->argv[3], cstr);
    return;
}

void nvram_remove_right_item(webs_t wp, struct variable *v, int sid, int *delMap)
{
    struct variable *gv;
    char *value;
    char glist[MAX_LINE_SIZE], *itemPtr, *glistPtr, cstr[5];
    int item, i, count;
//Yau
//printf("+++ remove right item +++\n");
    if (v->argv[0]==NULL)
    {
       return;
    }
                                                                                                                                              
//    for (gv = (struct variable *)v->argv[0]; gv->name!=NULL; gv++)
//    {
//Yau
//printf("remove right_item delMap[0]=%d\n",delMap[0]);
        nvram_remove_right_list(delMap);
//    }
                                                                                                                                              
    return;
}


struct utf8_table {
	int     cmask;
	int     cval;
	int     shift;
	long    lmask;
	long    lval;
};

static struct utf8_table utf8_table[] =
{
    {0x80,  0x00,   0*6,    0x7F,           0,         /* 1 byte sequence */},
    {0xE0,  0xC0,   1*6,    0x7FF,          0x80,      /* 2 byte sequence */},
    {0xF0,  0xE0,   2*6,    0xFFFF,         0x800,     /* 3 byte sequence */},
    {0xF8,  0xF0,   3*6,    0x1FFFFF,       0x10000,   /* 4 byte sequence */},
    {0xFC,  0xF8,   4*6,    0x3FFFFFF,      0x200000,  /* 5 byte sequence */},
    {0xFE,  0xFC,   5*6,    0x7FFFFFFF,     0x4000000, /* 6 byte sequence */},
    {0,						       /* end of table    */}
};

int
utf8_mbtowc(unsigned short int *p, const unsigned char *s, int n)
{
	long l;
	int c0, c, nc;
	struct utf8_table *t;
  
	nc = 0;
	c0 = *s;
	l = c0;
	for (t = utf8_table; t->cmask; t++) {
		nc++;
		if ((c0 & t->cmask) == t->cval) {
			l &= t->lmask;
			if (l < t->lval)
				return -1;
			*p = l;
			return nc;
		}
		if (n <= nc)
			return -1;
		s++;
		c = (*s ^ 0x80) & 0xFF;
		if (c & 0xC0)
			return -1;
		l = (l << 6) | c;
	}
	return -1;
}

int
utf8_mbstowcs(unsigned short int *pwcs, const unsigned char *s, int n)
{
	unsigned short int *op;
	const unsigned char *ip;
	int size;

	op = pwcs;
	ip = s;
	while (*ip && n > 0) {
		if (*ip & 0x80) {
			size = utf8_mbtowc(op, ip, n);
			if (size == -1) {
				/* Ignore character and move on */
				ip++;
				n--;
			} else {
				op++;
				ip += size;
				n -= size;
			}
		} else {
			*op++ = *ip++;
		}
	}
	return (op - pwcs);
}
#endif

/* Rule for table: 
 * 1. First field can not be NULL, or it is treat as empty
 * 2. 
 */

static int 
nvram_add_group_table(webs_t wp, char *serviceId, struct variable *v, int count)
{
    struct variable *gv;
    char buf[MAX_LINE_SIZE+MAX_LINE_SIZE];
    char bufs[MAX_LINE_SIZE+MAX_LINE_SIZE];    
    int i, j, fieldLen, rowLen, fieldCount, value;
    unsigned short int wstr[33];
    int addlen=0;
    int temp=0;
    
//Yau
//printf("+++ add group table +++\n");
    if (v->argv[0]==NULL) 
    {
       return 0;
    }
    
    bufs[0] = 0x0;
    rowLen = atoi(v->argv[2]);      
	
    if (count==-1)
    {    
       for(i=0;i<rowLen;i++)
       {     
       	   bufs[i] = ' ';
       }
       value = -1;	       	       	             	
       bufs[i] = 0x0;             	
    	
       goto ToHTML;	
    }
              
                    
    fieldCount = 0;
    
    value = count;
            
    for (gv = (struct variable *)v->argv[0]; gv->name!=NULL; gv++)
    {    		    	    	    	
    	strcpy(buf, nvram_get_list_x(serviceId, gv->name, count));
//    	printf("bufs len:  %d\n", strlen(bufs));
//    	printf("name: %s\n", gv->name);
//    	printf("buf:  %s\n", buf);
#ifndef NOUSB
    	if(strcmp(gv->name, "sh_path")==0 || strcmp(gv->name, "sh_name")==0)
    	{
    		addlen=(strlen(buf)-utf8_mbstowcs(wstr, buf, 1024))/2;
  		temp=strlen(buf);
		for(i=0;i<addlen;i++)
			(buf+temp+1)[i]=' ';
		(buf+temp)[i]=NULL;
//    		printf("additional length: %d\n", addlen);
    	}
    	else
#endif
    		addlen=0;
    	
    	fieldLen = atoi(gv->longname)+addlen;
    	rowLen+=addlen;
    	    	    	       	        	    
    	if (fieldLen!=0)
    	{    	
    	   if (strlen(buf)>fieldLen)
    	   {    	     
    	      buf[fieldLen] = 0x0; 	
    	   }   
    	   else
    	   {
    	      i = strlen(buf);	
    	      j = i;
    	      for(;i<fieldLen;i++)
    	      {
    	         buf[j++] = ' ';	
    	      }       	      
    	      buf[j] = 0x0;
    	   }      
    	}   
    	
    	if (fieldCount==0)
    	   sprintf(bufs,"%s", buf);
    	else
    	   sprintf(bufs,"%s%s",bufs, buf);
    	       	   
    	fieldCount++;
    }                                              
    
    if (strlen(bufs)> rowLen)
       bufs[rowLen] = 0x0;
       
ToHTML:       
    /* Replace ' ' to &nbsp; */
    buf[0] = 0;
    j = 0;
    
    for (i=0; i<strlen(bufs);i++)
    {
    	if (bufs[i] == ' ')
    	{
    	   buf[j++] = '&';
    	   buf[j++] = 'n';
    	   buf[j++] = 'b';
    	   buf[j++] = 's';
    	   buf[j++] = 'p';
    	   buf[j++] = ';';    	   	
    	} 
    	else buf[j++] = bufs[i];
    }          
    buf[j] = 0x0;
       
    return(websWrite(wp, "<option value=\"%d\">%s</option>", value, buf));
}

#ifndef NOUSB
void add_space(char *buf, char *value, int Len)
{
        int i, j;

	strcpy(buf, value);
//Yau
//printf("Before=%s\n",buf);
        if (strlen(value)>Len)
        {
                buf[Len] = 0x0;
        }
        else
        {
                i = strlen(value);
                j = i;
                for(;i<Len;i++)
                {
	           buf[j++] = '&';
        	   buf[j++] = 'n';
	           buf[j++] = 'b';
        	   buf[j++] = 's';
	           buf[j++] = 'p';
        	   buf[j++] = ';';
                }
                buf[j] = 0x0;
        }
//Yau
//printf("After=%s\n",buf);
}

static int
nvram_add_right_table(webs_t wp, char *serviceId, struct variable *v, int count)
{
    struct variable *gv;
    char buf_user[MAX_LINE_SIZE+MAX_LINE_SIZE];
    char buf_rright[MAX_LINE_SIZE+MAX_LINE_SIZE];
    char buf_wright[MAX_LINE_SIZE+MAX_LINE_SIZE];
    char out[3800], userbuf[256], rightbuf[256];
    char *user, *ruser, *wuser;
    int  j, k, fieldLen, rowLen, fieldCount, value, sh_edit;
    int  i=0;
    char tmpstr[1024];
//Yau
//printf("+++ add User Right table +++\n");

    rowLen = atoi(v->argv[2]);
                                                                                                                                              
    fieldCount = 0;
                                                                                                                                              
    value = count;

    sh_edit = atoi(nvram_get("sh_edit_x"));
//Yau
//printf("sh_edit=%d\n",sh_edit);
                                                                                                                                              
        strcpy(buf_user, nvram_get_list_x(serviceId, "sh_acc_user", sh_edit));
	if (strcmp(buf_user,"")==0)
	{
		j = 0;
		for (k=0; k<rowLen;k++)
		{
			tmpstr[j++] = '&';
			tmpstr[j++] = 'n';
			tmpstr[j++] = 'b';
			tmpstr[j++] = 's';
			tmpstr[j++] = 'p';
			tmpstr[j++] = ';';    	   	
		}
    	  	tmpstr[j] = 0x0;
		return(websWrite(wp, "<option value=\"-1\">%s</option>", tmpstr));
	}
		
        strcpy(buf_rright, nvram_get_list_x(serviceId, "sh_rright", sh_edit));
        strcpy(buf_wright, nvram_get_list_x(serviceId, "sh_wright", sh_edit));

//Yau
//printf(" %s\n %s\n %s\n",buf_user,buf_rright, buf_wright);

	user = strtok(buf_user, ";");
//Yau
//printf("first user:%s\n",user);
        ruser = findpattern(buf_rright, user);
        wuser = findpattern(buf_wright, user);
//Yau
//printf("ruser=%s, wuser=%s\n",ruser,wuser);

//Yau
//user - 36
//right- 28

	if (wuser==NULL)
	{
		add_space(userbuf, user, 42);
		add_space(rightbuf, "Read Only", 37);
		sprintf(out, "<option value=\"%d\">%s%s</option>", i, userbuf, rightbuf);
	}
	else
	{
                add_space(userbuf, user, 42);
                add_space(rightbuf, "Read/Write", 37);
                sprintf(out, "<option value=\"%d\">%s%s</option>", i, userbuf, rightbuf);
	}

	while(user=strtok(NULL,";"))
	{
	    i++;
//Yau
//printf("later user:%s\n",user);
	    if(strcmp(user,"")!=0)
	    {
	        ruser = findpattern(buf_rright, user);
        	wuser = findpattern(buf_wright, user);
//Yau
//printf("ruser=%s, wuser=%s\n",ruser,wuser);
                                                                                                                                              
	        if (wuser==NULL)
	        {
        	        add_space(userbuf, user, 42);
                	add_space(rightbuf, "Read Only", 37);
	                sprintf(out, "%s<option value=\"%d\">%s%s</option>", out, i, userbuf, rightbuf);
        	}
	        else
        	{
                	add_space(userbuf, user, 42);
	                add_space(rightbuf, "Read/Write", 37);
        	        sprintf(out, "%s<option value=\"%d\">%s%s</option>", out, i, userbuf, rightbuf);
        	}
	    }
	}
//Yau
//printf("Out=%s\n",out);
	return(websWrite(wp, out));
}

#endif

static int
apply_cgi_group(webs_t wp, int sid, struct variable *var, char *groupName, int flag)
{	
   struct variable *v;
   char *value, *value1;
   char item[64];
   int i, groupCount;
         
   //Yau del c    
//   printf("==apply_cgi_group== This group limit is %d %d\n", flag, sid);

   if (var!=NULL) v = var;
   else
   {         
       /* Validate and set vairables in table order */              
       for (v = GetVariables(sid); v->name != NULL; v++) 
       {
       	   //printf("Find group : %s %s\n", v->name, groupName);
       	   if(!strcmp(groupName, v->name)) break;
       }	                    
	//Yau
//	printf("Find group : %s %s\n", v->name, groupName);	                    
   }    
   
   if (v->name == NULL) return 0;    
   
   groupCount = atoi(v->argv[1]);

   if (flag == GROUP_FLAG_ADD)/* if (!strcmp(value, " Refresh ")) || Save condition */
   {    
   	nvram_add_group_item(wp, v, sid);
   	
   	return 1;	// 2007.11 James
   }	  
   else if (flag == GROUP_FLAG_REMOVE)/* if (!strcmp(value, " Refresh ")) || Save condition */
   {    
   	/*nvram_remove_group_item(wp, v, sid); 	*/
   	/*sprintf(item, "%s_s", websGetVar(wp, "group_id", ""));*/
        /*value1 = websGetVar(wp, item, NULL);*/
        /* TO DO : change format from 'x=1&x=2&...' to 'x=1 2 3 ..'*/
   	nvram_remove_group_item(wp, v, sid, delMap);
   	
   	return 1;	// 2007.11 James
   }
   
	return 0;	// 2007.11 James
}

#ifndef NOUSB
static int
apply_right_group(webs_t wp, int sid, struct variable *var, char *groupName, int flag)
{
   struct variable *v;
   char *value, *value1;
   char item[64];
   int i, groupCount;
                                                                                                                                              
   //Yau 
//   printf("==apply_right_group== flag:%d, sid:%d\n", flag, sid);
                                                                                                                                              
   if (var!=NULL) v = var;
   else
   {
       /* Validate and set vairables in table order */
       for (v = GetVariables(sid); v->name != NULL; v++)
       {
           if(!strcmp(groupName, v->name)) break;
       }
        //Yau
//        printf("Find right group : %s %s\n", v->name, groupName);
   }
                                                                                                                                              
   if (v->name == NULL) return 0;
                                                                                                                                              
   groupCount = atoi(v->argv[1]);
                                                                                                                                              
   if (flag == GROUP_FLAG_ADD)/* if (!strcmp(value, " Refresh ")) || Save condition */
   {
        nvram_add_right_item(wp, v, sid);
   }
   else if (flag == GROUP_FLAG_REMOVE)/* if (!strcmp(value, " Refresh ")) || Save condition */
   {
        nvram_remove_right_item(wp, v, sid, delMap);
   }
   return(1);
}

static int
modify_right_group(webs_t wp, int sid, struct variable *var, char *groupName)
{
   struct variable *v;
   char *user;
   int right;

   //Yau
//   printf("== modify_right_group == sid:%d\n", sid);
                                                                                                                                              
   if (var!=NULL) v = var;
   else
   {
       /* Validate and set vairables in table order */
       for (v = GetVariables(sid); v->name != NULL; v++)
       {
           if(!strcmp(groupName, v->name)) break;
       }
        //Yau
//        printf("Find right group : %s %s\n", v->name, groupName);
   }
                                                                                                                                              
   if (v->name == NULL) return 0;
                                                                                                                                              
   user = websGetVar(wp, "sh_acc_user_0", NULL);  
   right= atoi(websGetVar(wp, "sh_acc_right_0", NULL));
//Yau
//printf("acc_user: %s, acc_right:%d\n",user,right);
   nvram_modify_right_list(user,right);

   return(1);
}

#endif

static int
nvram_generate_table(webs_t wp, char *serviceId, char *groupName)
{	
   struct variable *v;
   int i, groupCount, ret, r, sid;
//Yau
//printf("=== generate_table === group:%s\n",groupName);     
   
   sid = LookupServiceId(serviceId);
         
   if (sid==-1) return 0;
    
   /* Validate and set vairables in table order */              
   for (v = GetVariables(sid); v->name != NULL; v++) 
   {
      /* printf("Find group : %s %s\n", v->name, groupName);*/
      if(!strcmp(groupName, v->name)) break;                       
   }    
   //Yau
//   printf("Find group :: %s %s\n", v->name, groupName);
   
   if (v->name == NULL) return 0;    

   groupCount = atoi(nvram_safe_get_x(serviceId, v->argv[3]));               
            
//Yau
//printf("groupCount=%d\n",groupCount);

#ifndef NOUSB
   if(!strcmp(groupName,"Storage_UserRight_List"))
   {
	ret = nvram_add_right_table(wp, serviceId, v, 1);
   }
   else
#endif
   if (groupCount==0) ret = nvram_add_group_table(wp, serviceId, v, -1);
   else
   {
   
      ret = 0;
   
      for (i=0; i<groupCount; i++)
      {       	            	
        r = nvram_add_group_table(wp, serviceId, v, i);
        if (r!=0)
           ret += r;
        else break;
      }    
   }   
   
   return(ret);
}


#ifdef WEBS
mySecurityHandler(webs_t wp, char_t *urlPrefix, char_t *webDir, int arg, char_t *url, char_t *path, char_t *query)
{
	char_t *user, *passwd, *digestCalc;
	int flags, nRet;


	user = websGetRequestUserName(wp);
	passwd = websGetRequestPassword(wp);
	flags = websGetRequestFlags(wp);


	nRet = 0;

	if (user && *user)
	{
		if (strcmp(user, "admin")!=0)
		{
			websError(wp, 401, T("Wrong User Name"));
			nRet = 1;
		}
		else if (passwd && *passwd)
		{
			if (strcmp(passwd, websGetPassword())!=0)
			{
				websError(wp, 401, T("Wrong Password"));
				nRet = 1;
			}
		}
#ifdef DIGEST_ACCESS_SUPPORT		
		else if (flags & WEBS_AUTH_DIGEST)
		{
			wp->password = websGetPassword();

			a_assert(wp->digest);
			a_assert(wp->nonce);
			a_assert(wp->password);

			digestCalc = websCalcDigest(wp);
			a_assert(digestCalc);		

			if (gstrcmp(wp->digest, digestCalc)!=0)
			{
				websError(wp, 401, T("Wrong Password"));
				nRet = 1;
			}
			bfree(B_L, digestCalc);
		}
#endif	// DIGEST_ACCESS_SUPPORT
	}
	else 
	{
#ifdef DIGEST_ACCESS_SUPPORT
		wp->flags |= WEBS_AUTH_DIGEST;
#endif	// DIGEST_ACCESS_SUPPORT
		websError(wp, 401, T(""));
		nRet = 1;
	}
	return nRet;

}
#else	// WEBS
static void
do_auth(char *userid, char *passwd, char *realm)
{
	time_t tm;
	
	if(strcmp(ProductID,"") == 0)
	{
		strncpy(ProductID, nvram_safe_get_x("general.log", "productid"), 32);
	}
	
	if(strcmp(UserID, "") == 0)
	{
		strncpy(UserID, nvram_safe_get_x("General", "http_username"), 32);
	}
	
// 2008.03 James. {
	//if(strcmp(UserPass, "") == 0)
	if(strcmp(UserPass, "") == 0 || reget_passwd == 1)
	{
		reget_passwd = 0;
// 2008.03 James. }
		strncpy(UserPass, nvram_safe_get_x("General", "http_passwd"), 32);
	}
	
	//time(&tm);
	
	strncpy(userid, UserID, AUTH_MAX);
	//if(redirect || !is_auth())
	if(!is_auth())
	{
		strcpy(passwd, "");
	}
	else
	{
		strncpy(passwd, UserPass, AUTH_MAX);
	}
	
	strncpy(realm, ProductID, AUTH_MAX);	
}
#endif	// WEBS

static void
do_apply_cgi(char *url, FILE *stream)
{
	/*char *path, *query;
	
	cprintf(" Before Apply : %s\n", url);
	
	websScan(url);
	query = url;
	path = strsep(&query, "?")?:url;
#ifndef HANDLE_POST
	init_cgi(query);
#endif
	apply_cgi(stream, NULL, NULL, 0, url, path, query);
#ifndef HANDLE_POST
	init_cgi(NULL);
#endif*/
	
	apply_cgi(stream, NULL, NULL, 0, url, NULL, NULL);
}

#ifdef HANDLE_POST
static char post_buf[10000] = { 0 };
#endif

static void
do_apply_cgi_post(char *url, FILE *stream, int len, char *boundary)
{
	//printf("In : %s %s\n", url, boundary);
#ifdef HANDLE_POST
	if(!fgets(post_buf, MIN(len+1, sizeof(post_buf)), stream))
		return;
	
	len -= strlen(post_buf);
	init_cgi(post_buf);
	while(len--)
		(void)fgetc(stream);
#endif
}

#ifdef REMOVE_WL600
static void
do_internal_cgi(char *url, FILE *stream)
{
	char *path, *query;

	query = url;
	path = strsep(&query, "?") ? : url;
	init_cgi(query);
	internal_cgi(stream, NULL, NULL, 0, url, path, query);
}
#endif

#if defined(linux)

//#if defined(ASUS_MIMO) && defined(TRANSLATE_ON_FLY)
#ifdef TRANSLATE_ON_FLY
static int refresh_title_asp = 0;

static void
do_lang_cgi(char *url, FILE *stream)
{
        if (refresh_title_asp)  {
                // Request refreshing pages from browser.
                websHeader(stream);
                websWrite(stream, "<head></head><title>REDIRECT TO INDEX.ASP</title>");

                // The text between <body> and </body> content may be rendered in Opera browser.
                websWrite(stream, "<body onLoad='if (navigator.appVersion.indexOf(\"Firefox\")!=-1||navigator.appName == \"Netscape\"){top.location=\"index.asp\";}else{top.location.reload(true);}'></body>");
                websFooter(stream);
                websDone(stream, 200);
        } else {
                // Send redirect-page if and only if refresh_title_asp is true.
                // If we do not send Title.asp, Firefox reload web-pages again and again.
                // This trick had been deprecated due to compatibility issue with Netscape and Mozilla browser.
                websRedirect(stream, "Title.asp");
        }
}


static void
do_lang_post(char *url, FILE *stream, int len, char *boundary)
{
        int c;
        char *p, *p1;
        char orig_lang[4], new_lang[4];

        if (url == NULL)
                return;

        p = strstr (url, "preferred_lang_menu");
        if (p == NULL)
                return;
        memset (new_lang, 0, sizeof (new_lang));
        strncpy (new_lang, p + strlen ("preferred_lang_menu") + 1, 2);

        memset (orig_lang, 0, sizeof (orig_lang));
        p1 = nvram_safe_get_x ("", "preferred_lang");
        if (p1[0] != '\0')      {
                strncpy (orig_lang, p1, 2);
        } else {
                strncpy (orig_lang, "EN", 2);
        }

        // read remain data
        if (feof (stream))      {
                while ((c = fgetc(stream) != EOF))      {
                        ;       // fall through
                }
        }


        cprintf ("lang: %s --> %s\n", orig_lang, new_lang);
        refresh_title_asp = 0;
        if (strcmp (orig_lang, new_lang) != 0 || is_firsttime ())       {
                // If language setting is different or first change language
                nvram_set_x ("", "preferred_lang", new_lang);
                if (is_firsttime ())    {
                        cprintf ("set x_Setting --> 1\n");
                        nvram_set_x ("", "x_Setting", "1");
			firsttime = -1; //2009.06 James
                }
                cprintf ("!!!!!!!!!Commit new language settings.\n");
                refresh_title_asp = 1;
                sys_commit(NULL);
        }
}
//#endif  // defined(ASUS_MIMO) && defined(TRANSLATE_ON_FLY)
#endif // TRANSLATE_ON_FLY


static void
do_webcam_cgi(char *url, FILE *stream)
{
	#define MAX_RECORD_IMG 12
	char *query, *path, *last;
	char pic[32];
	int query_idx, last_idx;
	        
	query = url;
	path = strsep(&query, "?") ? : url;
	cprintf("WebCam CGI\n");
	//ret = fcntl(fileno(stream), F_GETOWN, 0);
	query_idx = atoi(query);
	last_idx = atoi(nvram_get_f("webcam.log", "WebPic"));
	//pic = nvram_get_f("webcam.log","WebPic");
			
	if (query_idx==0) sprintf(pic, "../var/tmp/display.jpg");
	else sprintf(pic, "../var/tmp/record%d.jpg", (query_idx>last_idx+1) ? (last_idx+1+MAX_RECORD_IMG-query_idx):(last_idx+1-query_idx));
	
	cprintf("\nWebCam: %s\n", pic);
	do_file(pic, stream);
}

static void
do_upgrade_post(char *url, FILE *stream, int len, char *boundary)
{
	#define MAX_VERSION_LEN 64
	char upload_fifo[] = "/tmp/linux.trx";
	FILE *fifo = NULL;
	/*char *write_argv[] = { "write", upload_fifo, "linux", NULL };*/
	pid_t pid, ppid;
	char buf[4096];
	int count, ret = EINVAL, ch;
	int eno, cnt;
	long filelen, *filelenptr;
	int hwmajor=0, hwminor=0;
	char version[MAX_VERSION_LEN], cmpHeader;
	char *hwver;

	int dbg_ret;
	
printf("Start upgrade!!! url=%s.\n", url);
printf("killall networkmap!\n");
	eval("killall", "networkmap");
printf("Start get the PID...");
	pid = getpid();
	ppid = getppid();
printf("%d, PPID=%d.\n", pid, ppid);
printf("stop service......");
	eval("stopservice", "99");
printf("Start get the PID...");
	pid = getpid();
	ppid = getppid();
printf("%d, PPID=%d.\n", pid, ppid);
	nvram_set("reboot", "0");
printf("test 1. len=%d.\n");
	/* Look for our part */
	while (len > 0) 
	{
		if (!fgets(buf, MIN(len + 1, sizeof(buf)), stream))
		{
printf("error 1.\n");
			goto err;
		}
		
		len -= strlen(buf);
		
		if (!strncasecmp(buf, "Content-Disposition:", 20) &&
				strstr(buf, "name=\"file\""))
			break;
	}
printf("test 2.\n");
	/* Skip boundary and headers */
	while (len > 0)
	{
printf("test 3.\n");
		if (!fgets(buf, MIN(len + 1, sizeof(buf)), stream))
		{
printf("error 2.\n");
			goto err;
		}
		
		len -= strlen(buf);
		if (!strcmp(buf, "\n") || !strcmp(buf, "\r\n"))
		{
			break;
		}
	}
printf("test 4. open file=%s.\n", upload_fifo);
	if (!(fifo = fopen(upload_fifo, "a+"))){
printf("error 3.\n");
		goto err;
	}
	
	filelen = len;
	cnt = 0;
	
	/* Pipe the rest to the FIFO */
	//printf("Upgrading %d\n", len);
	cmpHeader = 0;
printf("test 5. len=%d, filelen=%d.\n", len, filelen);
	while (len > 0 && filelen>0)
	{
		//if (waitfor(fileno(stream), 10) <= 0)
		if (waitfor(conn_fd, 10) <= 0)
		{
			/*printf("Break while len=%x filelen=%x\n", len, filelen);*/
			break;
		}
		
		count = fread(buf, 1, MIN(len, sizeof(buf)), stream);
//printf("read %d bytes.\n", count);
		if (cnt == 0 && count>8)
		{
			if (strncmp(buf, IMAGE_HEADER, 4)!=0)
			{
				/*printf("Header %x %x %x %x\n", buf[0], buf[1], buf[2], buf[3]);*/
				len-=count;
printf("\nerror 4.\n");
				goto err;
			}
			else
				cmpHeader = 1;
			
			filelenptr = (buf+4);
			filelen = *filelenptr;
			/*printf("Filelen: %x %x %x %x %x %x\n", filelen, count, (unsigned long)(buf+4), (unsigned long)(buf+7), buf[5], buf[4]);*/
			cnt ++;
		}
		
		filelen-=count;
		len-=count;
#ifndef WRITE2FLASH
		fwrite(buf, 1, count, fifo);
#else
		if (!flashwrite(buf, count, filelen)){
printf("\nerror 5.\n");
			goto err;
		}
#endif
		//printf(".");
	}
	
	if (!cmpHeader){
printf("error 6.\n");
		goto err;
	}
printf("\ntest 6.\n");
	/* Slurp anything remaining in the request */
	while (len-- > 0)
	{
		ch = fgetc(stream);
		
		if (filelen>0)
		{
#ifndef WRITE2FLASH
			fwrite(&ch, 1, 1, fifo);
			filelen--;
#else
			flashwrite(buf, 1, --filelen);
#endif
		}
	}
printf("test 7.\n");
#ifndef	WRITE2FLASH
	for(cnt=0;cnt<MAX_VERSION_LEN;cnt++)
		version[cnt] = 0;
	
	fseek(fifo, -MAX_VERSION_LEN, SEEK_END);
	fread(version, 1, MAX_VERSION_LEN, fifo);
	
	sscanf(nvram_get_x("general.log", "HardwareVer"), "%d.%d", &hwmajor, &hwminor);
	cprintf("Hardware : %d.%d %s.\n", hwmajor, hwminor, version+4);
#if 0
	if ((strncmp(ProductID, version+4, strlen(ProductID))==0 && strncmp(version+4, "WL500gx", 7)!=0) ||
			(strncmp(ProductID, "WL500g.Premium", 14)==0 && strncmp(version+4, "WL500gp", 7)==0) ||
			(strncmp(ProductID, "WL500g.Deluxe", 13)==0 && strncmp(version+4, "WL500gx", 7)==0) &&
			checkVersion(version,hwmajor,hwminor))
#endif
	if (strncmp(ProductID, version+4, strlen(ProductID))==0)
		ret = 0;
#else
	ret = 0;
#endif
	if (ret == 0) {
		correct_image = 1;
		return;
	}
	else
		goto err;

printf("test 8. ret=%d.\n", ret);
	fseek(fifo, 0, SEEK_END);
	fclose(fifo);
	fifo = NULL;
printf("done\n");


err:
	if (fifo)
		fclose(fifo);
	
	/* Slurp anything remaining in the request */
	while (len-- > 0)
		ch = fgetc(stream);
	
#ifdef REMOVE_WL600
	unlink(upload_fifo);
#endif
	printf("Error : %d\n", ret);
/*
	dbg_ret = -ret;
	fcntl(fileno(stream), F_SETOWN, -ret);
*/

	correct_image = 0;
printf("10. Start get the PID...");
	pid = getpid();
	ppid = getppid();
printf("%d, PPID=%d, dbg_int= %d.\n", pid, ppid, dbg_ret);
}

static void
do_upgrade_cgi(char *url, FILE *stream)
{
	int ret;
	pid_t pid, ppid;
	
printf("Upgrade CGI\n");
printf("Start get the PID...");
//	ret = fcntl(fileno(stream), F_GETOWN, 0);
printf("11. Start get the PID...");
        pid = getpid();
        ppid = getppid();
printf("%d, PPID=%d.\n", pid, ppid);

//printf("%s.\n", (ret == 0?"Success":"Failed"));
printf("%s.\n", (correct_image == 0?"Success":"Failed"));

	/* Reboot if successful */
//	if(ret == 0)
	if (correct_image == 1)
	{
#ifndef WRITE2FLASH
printf("Send the page, \"Updating.asp\".\n");
		websApply(stream, "Updating.asp");
printf("Write the image.\n");
		sys_upgrade("/tmp/linux.trx");
#else
		websApply(stream, "Restarting.asp");
#endif
printf("Reboot.\n");
		sys_reboot();
	}
	else
	{
printf("Send the error page, \"UpdateError.asp\".\n");
		websApply(stream, "UpdateError.asp");
		unlink("/tmp/linux.trx");
printf("Reboot.\n");
		sys_reboot();
	}
}

static void
do_upload_post(char *url, FILE *stream, int len, char *boundary)
{
	#define MAX_VERSION_LEN 64
	char upload_fifo[] = "/tmp/settings_u.prf";
	FILE *fifo = NULL;
	/*char *write_argv[] = { "write", upload_fifo, "linux", NULL };*/
	pid_t pid;
	char buf[1024];
	int count, ret = EINVAL, ch;
	int eno, cnt;
	long filelen, *filelenptr;
	int hwmajor=0, hwminor=0;
	char version[MAX_VERSION_LEN], cmpHeader;
	char *hwver;
	
	cprintf("Start upload\n");
	eval("stopservice", "99");
	
	/* Look for our part */
	while(len > 0){
		if(!fgets(buf, MIN(len+1, sizeof(buf)), stream)){
			goto err;
		}			
		
		len -= strlen(buf);
		
		if(!strncasecmp(buf, "Content-Disposition:", 20)
				&& strstr(buf, "name=\"file\""))
			break;			
	}
	
	/* Skip boundary and headers */
	while(len > 0){
		if(!fgets(buf, MIN(len+1, sizeof(buf)), stream)){
			goto err;
		}
		
		len -= strlen(buf);
		
		if(!strcmp(buf, "\n") || !strcmp(buf, "\r\n")){
			break;
		}
	}
	
	if(!(fifo = fopen(upload_fifo, "a+")))
		goto err;
	
	filelen = len;
	cnt = 0;
	
	/* Pipe the rest to the FIFO */
	cprintf("Upgrading %d\n", len);
	cmpHeader = 0;
	
	while(len > 0 && filelen > 0){
		//if(waitfor(fileno(stream), 10) <= 0){
		if(waitfor(conn_fd, 10) <= 0){
			//printf("Break while len=%x filelen=%x\n", len, filelen);
			break;
		}
		
		count = fread(buf, 1, MIN(len, sizeof(buf)), stream);
		
		if(cnt == 0 && count > 8){
/*
			if(strncmp(buf, PROFILE_HEADER, 4) != 0){
				//printf("Header %x %x %x %x\n", buf[0], buf[1], buf[2], buf[3]);
				len-=count;
				goto err;
			}
			else
				cmpHeader = 1;
			
			filelenptr = (buf+4);
			filelen = *filelenptr;
			//printf("Filelen: %x %x %x %x %x %x\n", filelen, count, (unsigned long)(buf+4), (unsigned long)(buf+7), buf[5], buf[4]);
*/
			printf("Filelen: %x %x %x %x %x %x\n", filelen, count, (unsigned long)(buf+4), (unsigned long)(buf+7), buf[5], buf[4]);
                        if (!strncmp(buf, PROFILE_HEADER, 4))
                        {
                                filelenptr = (buf + 4);
                                filelen = *filelenptr;

                                cprintf("Upgrading original text cfg of length %x\n", filelen);
                        }
                        else if (!strncmp(buf, PROFILE_HEADER_NEW, 4))
                        {
                                filelenptr = (buf + 4);
                                filelen = *filelenptr;
                                filelen = filelen & 0xffffff;

                                cprintf("Upgrading non-text cfg of length %x\n", filelen);
                        }
                        else
                        {
                                len -= count;
                                goto err;
                        }

                        cmpHeader = 1;
			++cnt;
		}
		
		filelen -= count;
		len -= count;
		
		fwrite(buf, 1, count, fifo);
		printf(".");
	}
	
	if(!cmpHeader)
		goto err;
	
	/* Slurp anything remaining in the request */
	while(len-- > 0){
		ch = fgetc(stream);
		
		if(filelen > 0){
			fwrite(&ch, 1, 1, fifo);
			--filelen;
		}
	}
	
#ifdef REMOVE_WL600
	for(cnt = 0; cnt < MAX_VERSION_LEN; ++cnt)
		version[cnt] = 0;
	
	fseek(fifo, -MAX_VERSION_LEN, SEEK_END);
	fread(version, 1, MAX_VERSION_LEN, fifo);
	
	sscanf(nvram_get_x("general.log", "HardwareVer"), "%d.%d", &hwmajor, &hwminor);
	cprintf("Hardware : %d.%d", hwmajor, hwminor);
	
	if(checkVersion(version,hwmajor,hwminor))
		ret = 0;
#else
	ret = 0;
#endif
	
	fseek(fifo, 0, SEEK_END);
	fclose(fifo);
	fifo = NULL;
	/*printf("done\n");*/

	if (ret == 0) {
		correct_setting = 1;
		return;
	}
	else
		goto err;
	
err:
	if(fifo)
		fclose(fifo);
	
	/* Slurp anything remaining in the request */
	while(len-- > 0)	
		ch = fgetc(stream);
	
#ifdef REMOVE_WL600
	unlink(upload_fifo);
#endif
	//printf("Error : %d\n", ret);
//	fcntl(fileno(stream), F_SETOWN, -ret);
	correct_setting = 0;
}

static void
do_upload_cgi(char *url, FILE *stream)
{
	int ret;
	
	//printf("Upload CGI\n");
//	ret = fcntl(fileno(stream), F_GETOWN, 0);
	
	// Reboot if successful
//	if(ret == 0){
	if (correct_setting == 1) {
		websApply(stream, "Uploading.asp");
		sys_upload("/tmp/settings_u.prf");
		nvram_set("httpd_die_reboot", "1");
		sys_commit();
		sys_reboot();
	}
	else{
		websApply(stream, "UploadError.asp");
		unlink("/tmp/settings_u.prf");
	}
}

static void
do_prf_file(char *url, FILE *stream)
{
	sys_commit(NULL);
	sys_download("/tmp/settings");
	do_file(url, stream);
	//unlink("/tmp/settings");
}


#elif defined(vxworks)

static void
do_upgrade_post(char *url, FILE *stream, int len, char *boundary)
{
}

static void
do_upgrade_cgi(char *url, FILE *stream)
{
}

#endif

static char no_cache_IE7[] =
"X-UA-Compatible: IE=EmulateIE7\r\n"
"Cache-Control: no-cache\r\n"
"Pragma: no-cache\r\n"
"Expires: 0"
;

static char no_cache[] =
"Cache-Control: no-cache\r\n"
"Pragma: no-cache\r\n"
"Expires: 0"
;

static char log_headers[] = 
"Content-Disposition: attachment; filename=\"SYSLOG.DAT\"\r\n"
"Cache-Control: no-cache\r\n"
"Pragma: no-cache\r\n"
"Expires: 0"
;

static void 
do_log_cgi(char *path, FILE *stream)
{
printf("do_log_cgi: path = %s.\n", path);
	dump_file(stream, "/tmp/syslog.log-1");
	dump_file(stream, "/tmp/syslog.log");
	fputs("\r\n", stream); /* terminator */
	fputs("\r\n", stream); /* terminator */
}

#ifndef NOUSB
//2006.03.21 added by Jiahao to handle UTF8
int getval(char c)
{
	if ( (c<=57) && (c>=48) )
		return (c-48);
	return (c-55);
}

//2005.12.15 Yau add 500gP
static void
do_selectdir_cgi(char *url, FILE *stream)
{
        char *dirpath;
                                                                                                                                              
        dirpath=strchr(url, '=');
        strcpy(NewDirPath, (dirpath+1));
//2006.03.21 added by Jiahao to handle UTF8
        char tmppath[256];
        int i, j, len;
        len = strlen(NewDirPath);
        for(i=0,j=0;i<len;i++)
        {
        	if (NewDirPath[i]==37)
        	{
        		tmppath[j]=getval(NewDirPath[i+1])*16+getval(NewDirPath[i+2]);
        		i+=2;
        	}
        	else
        		tmppath[j]=NewDirPath[i];
        	j++;
        }
        tmppath[j]=NULL;
        strcpy(NewDirPath, tmppath);
}

static void
do_selectdir_ej(char *url, FILE *stream)
{
        do_ej("Advanced_StorageSelectFolder_Content.asp", stream);
}
#endif

#ifdef WEBS
void
initHandlers(void)
{
	websAspDefine("nvram_get_x", ej_nvram_get_x);
	websAspDefine("nvram_get_f", ej_nvram_get_f);
	websAspDefine("nvram_get_list_x", ej_nvram_get_list_x);
	websAspDefine("nvram_get_buf_x", ej_nvram_get_buf_x);
	websAspDefine("nvram_get_table_x", ej_nvram_get_table_x);
	websAspDefine("nvram_match_x", ej_nvram_match_x);
	websAspDefine("nvram_double_match_x", ej_nvram_double_match_x);
	websAspDefine("nvram_match_both_x", ej_nvram_match_both_x);
	websAspDefine("nvram_match_list_x", ej_nvram_match_list_x);
	websAspDefine("select_channel", ej_select_channel);
	websAspDefine("select_country", ej_select_country);
	websAspDefine("urlcache", ej_urlcache);
	websAspDefine("uptime", ej_uptime);
	websAspDefine("nvram_dump", ej_dump);
	websAspDefine("load_script", ej_load);
	websSecurityDelete();
	websUrlHandlerDefine("", NULL, 0, mySecurityHandler, WEBS_HANDLER_FIRST);
	websUrlHandlerDefine("/apply.cgi", NULL, 0, apply_cgi, 0);
	websSetPassword(nvram_safe_get_x("General", "x_Password"));
	websSetRealm(nvram_safe_get_x("general.log", "ProductID"));
#ifdef ASUS_DDNS //2007.03.27 Yau add
	websAspDefine("nvram_get_ddns", ej_nvram_get_ddns);
#endif
}

#else
struct mime_handler mime_handlers[] = {

	//{ "*.dict", "text/html", NULL, NULL, do_ej, NULL },
	{ "Nologin.asp", "text/html", no_cache_IE7, do_html_post_and_get, do_ej, NULL },
	{ "error_page.htm*", "text/html", no_cache_IE7, do_html_post_and_get, do_ej, NULL },
	{ "gotoHomePage.htm", "text/html", no_cache_IE7, do_html_post_and_get, do_ej, NULL },
	{ "**.htm*", "text/html", no_cache_IE7, do_html_post_and_get, do_ej, do_auth },
	{ "**.asp*", "text/html", no_cache_IE7, do_html_post_and_get, do_ej, do_auth },
	
	/*{ "**.css", "text/css", NULL, NULL, do_file, do_auth },
	{ "**.png", "image/png", NULL, NULL, do_file, do_auth },
	{ "**.gif", "image/gif", NULL, NULL, do_file, do_auth },
	{ "**.jpg", "image/jpeg", NULL, NULL, do_file, do_auth },//*/
	{ "**.css", "text/css", NULL, NULL, do_file, NULL },
	{ "**.png", "image/png", NULL, NULL, do_file, NULL },
	{ "**.gif", "image/gif", NULL, NULL, do_file, NULL },
	{ "**.jpg", "image/jpeg", NULL, NULL, do_file, NULL },
//#if defined(ASUS_MIMO) && defined(TRANSLATE_ON_FLY)
#ifdef TRANSLATE_ON_FLY
	/* Only general.js and quick.js are need to translate. (reduce translation time) */
	{ "general.js|quick.js",  "text/javascript", no_cache_IE7, NULL, do_ej, do_auth },
//#endif  // defined(ASUS_MIMO) && defined(TRANSLATE_ON_FLY)
#endif //TRANSLATE_ON_FLY
	
	{ "**.js",  "text/javascript", no_cache_IE7, NULL, do_ej, do_auth },
	{ "**.cab", "text/txt", NULL, NULL, do_file, do_auth },
	{ "**.CFG", "text/txt", NULL, NULL, do_prf_file, do_auth },
	
	{ "apply.cgi*", "text/html", no_cache_IE7, do_html_post_and_get, do_apply_cgi, do_auth },
	{ "upgrade.cgi*", "text/html", no_cache_IE7, do_upgrade_post, do_upgrade_cgi, do_auth},
	{ "upload.cgi*", "text/html", no_cache_IE7, do_upload_post, do_upload_cgi, do_auth },
 	{ "syslog.cgi*", "text/txt", no_cache_IE7, do_html_post_and_get, do_log_cgi, do_auth },
//#ifdef TRANSLATE_ON_FLY
#ifdef TRANSLATE_ON_FLY
	{ "change_lang.cgi*", "text/html", no_cache_IE7, do_lang_post, do_lang_cgi, do_auth },
//#endif // TRANSLATE_ON_FLY
#endif //TRANSLATE_ON_FLY
	{ "webcam.cgi*", "text/html", no_cache_IE7, NULL, do_webcam_cgi, do_auth },
#ifndef NOUSB
	{ "Advanced_StorageSelectFolder_Content.cgi**", "text/html", no_cache_IE7, do_selectdir_cgi, do_selectdir_ej, do_auth },//2005.12.15 Yau add for 500gP
#endif
	{ NULL, NULL, NULL, NULL, NULL, NULL }
};

#ifndef NOUSB
static int
ej_select_folder(int eid, webs_t wp, int argc, char_t **argv)
{
	char *sid;
	int ret = 0;	
	char buf[8], out[1024]; //2005.12.15 Yau change out[64] to out[1024]	
	int i, channel;
	FILE *fp;
	DIR *dir;
	DIR *dir2;
	struct dirent *entry;
	char OpenDirPath2[128];
	char convDir[128];
	char *keyword = "\%20";
        char *temp;

	if (ejArgs(argc, argv, "%s", &sid) < 1) {
		websError(wp, 400, "Insufficient args\n");
		return -1;
	}
	sprintf(OpenDirPath,"%s%s",CurrentDirPath,NewDirPath);
        /* display folder in first level */
        dir = opendir(OpenDirPath);
        
        if (dir==NULL)
        {
        	strcpy(CurrentDirPath, "/tmp/harddisk");
        	strcpy(NewDirPath, "");
        	sprintf(OpenDirPath,"%s%s",CurrentDirPath,NewDirPath);
        	dir = opendir(OpenDirPath);
        }
                                                                	
        if (dir!=NULL) 
        {
                strcpy(out,NewDirPath);
                while((entry=readdir(dir))!=NULL)
                {
			sprintf(OpenDirPath2, "%s/%s", OpenDirPath, entry->d_name);
			dir2 = opendir(OpenDirPath2);
			if (dir2!=NULL)
			{
//				printf("%s\n", OpenDirPath2);
				closedir(dir2);
				sprintf(out,"%s,%s",out,entry->d_name);
			}
		}
		
		ret+=websWrite(wp, out);
                closedir(dir);
        }
	return ret;
}
#endif

static int
ej_select_list(int eid, webs_t wp, int argc, char_t **argv)
{
	char *id;
	int ret = 0;	
	char buf[8], out[64], idxstr[12], tmpstr[64], tmpstr1[64];
	int i, channel, curr, hit, noneFlag;
	char *ref1, *ref2, *refnum;

	if (ejArgs(argc, argv, "%s", &id) < 1) {
		websError(wp, 400, "Insufficient args\n");
		return -1;
	}

//	printf("select_list:: id: %s\n", id);

        if (strcmp(id, "Storage_x_SharedPath")==0)
        {
		ref1 = "sh_path_x";
		ref2 = "sh_path";
		refnum = "sh_num";
                curr = atoi(nvram_get(ref1));
                sprintf(idxstr, "%d", curr);
                strcpy(tmpstr1, nvram_get(strcat_r(ref2, idxstr, tmpstr)));
                //sprintf(out, "<option value=\"%d\"", curr);
                //sprintf(out, "%s selected", out);
                //sprintf(out,"%s>%s</option>", out, tmpstr1);
                sprintf(out, "%s", tmpstr1);                                                                                                                              
                ret += websWrite(wp, out);
                return ret;
        }
	else if (strncmp(id, "Storage_x_AccUser", 17)==0)
	{
		sprintf(tmpstr, "sh_accuser_x%s", id + 17);
		ref2 = "acc_username";
		refnum = "acc_num";

		curr = atoi(nvram_get(tmpstr));
		noneFlag =1;
	}
	else if (strcmp(id, "Storage_x_AccAny")==0)
	{
		
                 ret = websWrite(wp, "<option value=\"Guest\">Guest</option>");
		 return ret;
	}
	else if (strcmp(id, "Storage_AllUser_List")==0)
	{
//Yau
//printf("Storage_AllUser_List\n");

		strcpy(out, "<option value=\"Guest\">Guest</option>");
		ret += websWrite(wp, out);

	        for(i=0;i<atoi(nvram_get("acc_num"));i++)
        	{
                	 sprintf(idxstr, "%d", i);
	                 strcpy(tmpstr1, nvram_get(strcat_r("acc_username", idxstr, tmpstr)));
        	         sprintf(out, "<option value=\"%s\">%s</option>", tmpstr1, tmpstr1);
	                 ret += websWrite(wp, out);
        	}
		return ret;
	}
	else 
	{
//printf("here\n");
		 return ret;     
	}
        

	//printf("out %s\n", curr);
	hit = 0;
 
	for(i=0;i<atoi(nvram_get(refnum));i++)
	{     
 		 sprintf(idxstr, "%d", i);
		 strcpy(tmpstr1, nvram_get(strcat_r(ref2, idxstr, tmpstr)));
             	 sprintf(out, "<option value=\"%d\"", i);

                 if (i==curr) 
		 {
			hit = 1;
			sprintf(out, "%s selected", out);
		 }
                 sprintf(out,"%s>%s</option>", out, tmpstr1);       
 
//		 printf("out: %s\n", out);

                 ret += websWrite(wp, out);
	}     
//printf("ret= %s\n",ret);

	if (noneFlag)
	{
		cprintf("hit : %d\n", hit);
		if (!hit) sprintf(out, "<option value=\"99\" selected>None</option>");
		else sprintf(out, "<option value=\"99\">None</option>");

		ret += websWrite(wp, out);
	}	
//printf("ret= %s\n",ret);
	return ret;
}

// 11N
/* Common function */
static int
wl_phytype_get(webs_t wp, int *phytype)
{
        char *name =NULL;
        char tmp[NVRAM_BUFSIZE], prefix[] = "wlXXXXXXXXXX_";
                                                                                                  
        if (!make_wl_prefix(prefix,sizeof(prefix),0,NULL)) {
                websError(wp, 400, "unit number variable doesn't exist\n");
                return -1;
        }
                                                                                                  
        name = nvram_safe_get(strcat_r(prefix, "ifname", tmp));
                                                                                                  
        /* Get configured phy type */
        wl_ioctl(name, WLC_GET_PHYTYPE, phytype, sizeof(int));
                                                                                                  
        return 0;
}
static char *
make_wl_prefix(char *prefix,int prefix_size, int mode, char *ifname)
{
        int unit=-1,subunit=-1;
        char *wl_unit=NULL;
                                                                                                  
        assert(prefix);
        assert(prefix_size);
                                                                                                  
        if (ifname){
                assert(*ifname);
                wl_unit=ifname;
        }else{
                wl_unit=nvram_get("wl_unit");
                                                                                                  
                if (!wl_unit)
                        return NULL;
        }
                                                                                                  
        if (get_ifname_unit(wl_unit,&unit,&subunit) < 0 )
                return NULL;
                                                                                                  
        if (unit < 0) return NULL;
                                                                                                  
        if  ((mode) && (subunit > 0 ))
                snprintf(prefix, prefix_size, "wl%d.%d_", unit,subunit);
        else
                snprintf(prefix, prefix_size, "wl%d_", unit);
                                                                                                  
        return prefix;
}
static int
wl_print_channel_list(webs_t wp, char *name, char *phytype, char *abbrev, int band)
{
        int j, status = 0;
        wl_channels_in_country_t *cic = (wl_channels_in_country_t *)malloc(WLC_IOCTL_MAXLEN);
                                                                                                  
        assert(name);
        assert(phytype);
        assert(abbrev);
                                                                                                  
        if (!cic) {
                status = -1;
                goto exit;
        }
                                                                                                  
        cic->buflen = WLC_IOCTL_MAXLEN;
        strcpy(cic->country_abbrev, abbrev);
        cic->band = band;
                                                                                                  
        if (wl_ioctl(name, WLC_GET_CHANNELS_IN_COUNTRY, cic, cic->buflen) == 0) {
                if (cic->count == 0) {
                        status = 0;
                        goto exit;
                }
                websWrite(wp, "\t\tif (country == \"%s\")\n\t\t\tchannels = new Array(0",
                        abbrev);
                for(j = 0; j < cic->count; j++)
                        websWrite(wp, ", %d", cic->channel[j]);
                websWrite(wp,");\n");
        }
                                                                                                  
exit:
        if (cic)
                free((void *)cic);
        return status;
}
static int
wl_print_chanspec_list(webs_t wp, char *name, char *phytype, char *abbrev, int band, int bw,
                       int ctrlsb)
{
        chanspec_t c = 0, *chanspec;
        char *buf = (char *)malloc(WLC_IOCTL_MAXLEN);
        int buflen;
        wl_uint32_list_t *list;
        int ret = 0, i = 0;
                                                                                                  
        strcpy(buf, "chanspecs");
        buflen = strlen(buf) + 1;
                                                                                                  
        if (band == WLC_BAND_5G)
                c |= WL_CHANSPEC_BAND_5G;
        else
                c |= WL_CHANSPEC_BAND_2G;
        if (bw == 20)
                c |= WL_CHANSPEC_BW_20;
        else
                c |= WL_CHANSPEC_BW_40;
                                                                                                  
        chanspec = (chanspec_t *)(buf + buflen);
        *chanspec = c;
        buflen += (sizeof(chanspec_t));
        strncpy(buf + buflen, abbrev, WLC_CNTRY_BUF_SZ);
        buflen += WLC_CNTRY_BUF_SZ;
                                                                                                  
        /* Add list */
        list = (wl_uint32_list_t *)(buf + buflen);
        list->count = WL_NUMCHANSPECS;
        buflen += sizeof(uint32)*(WL_NUMCHANSPECS + 1);
                                                                                                  
        ret = wl_ioctl(name, WLC_GET_VAR, buf, buflen);
        if (ret < 0)
                return ret;
                                                                                                  
        list = (wl_uint32_list_t *)buf;
                                                                                                  
        if (list->count == 0) {
                ret = 0;
                goto exit;
        }
                                                                                                  
        websWrite(wp, "\t\tif (country == \"%s\")\n\t\t\tchannels = new Array(0",
                  abbrev);
        for (i = 0; i < list->count; i++) {
                c = (chanspec_t)list->element[i];
                /* Skip upper.. (take only one of the lower or upper) */
                if (bw == 40 && ((c & WL_CHANSPEC_CTL_SB_MASK) != ctrlsb))
                        continue;
                /* Create the actual control channel number from sideband */
                if (bw == 40) {
                        int chan = c & WL_CHANSPEC_CHAN_MASK;
                        chan += ((ctrlsb == WL_CHANSPEC_CTL_SB_UPPER)? (+2):(-2));
                        websWrite(wp, ", %d", chan);
                } else
                        websWrite(wp, ", %d", c & WL_CHANSPEC_CHAN_MASK);
        }
        websWrite(wp,");\n");
exit:
        if (buf)
                free((void *)buf);
        return ret;
}                                                                                                  

/* Return whether the current phytype is NPHY */
static int
ej_wl_nphy_set(int eid, webs_t wp, int argc, char_t **argv)
{
        int phytype, status;
                                                                                                  
        /* Get configured phy type */
        if ((status = wl_phytype_get(wp, &phytype)) != 0)
                return status;
                                                                                                  
        return websWrite(wp, "%d", (phytype == WLC_PHY_TYPE_N));
}
static int
ej_wl_channel_list(int eid, webs_t wp, int argc, char_t **argv)
{
        int  i, status = 0;
        char *name=NULL;
        char tmp[NVRAM_BUFSIZE], prefix[] = "wlXXXXXXXXXX_";
        char *phytype = NULL;
        int cur_phytype;
        wl_country_list_t *cl = (wl_country_list_t *)malloc(WLC_IOCTL_MAXLEN);
        char *abbrev=NULL;
        int band = WLC_BAND_ALL;
        int bw = 0;
        int csb = WL_CHANSPEC_CTL_SB_UPPER;
        char *sb = NULL;
                                                                                                  
        if (!cl) {
                status = -1;
                goto exit;
        }
                                                                                                  
        if (ejArgs(argc, argv, "%s %d %d %s", &phytype, &band, &bw, &sb) < 1) {
                websError(wp, 400, "Insufficient args\n");
                status = -1;
                goto exit;
        }
                                                                                                  
        if (!make_wl_prefix(prefix,sizeof(prefix),0,NULL)) {
                websError(wp, 400, "unit number variable doesn't exist\n");
                status = -1;
                goto exit;
        }
                                                                                                  
        name = nvram_safe_get(strcat_r(prefix, "ifname", tmp));
                                                                                                  
        /* Get configured phy type */
        wl_ioctl(name, WLC_GET_PHYTYPE, &cur_phytype, sizeof(cur_phytype));
                                                                                                  
        cl->buflen = WLC_IOCTL_MAXLEN;
        cl->band_set = TRUE;
                                                                                                  
        if (!strcmp(phytype, "a") && (cur_phytype != WLC_PHY_TYPE_N))
                cl->band = WLC_BAND_5G;
        else if (((!strcmp(phytype, "b")) || (!strcmp(phytype, "g"))) &&
                 (cur_phytype != WLC_PHY_TYPE_N))
                cl->band = WLC_BAND_2G;
        else if ((!strcmp(phytype, "l")) && (cur_phytype == WLC_PHY_TYPE_LP)) {
                wl_ioctl(name, WLC_GET_BAND, &band, sizeof(band));
                cl->band = band;
        } else if ((!strcmp(phytype, "n")) && (cur_phytype == WLC_PHY_TYPE_N)) {
                /* Need to have additional argument of the band */
                if (argc < 3 || (band != WLC_BAND_2G && band != WLC_BAND_5G)) {
                        status = -1;
                        goto exit;
                }
                /* For 40, sideband is needed */
                if (bw == 40 && argc < 4) {
                        status = -1;
                        goto exit;
                }
                cl->band = band;
                if (bw == 40) {
                        if (strcmp(sb, "upper") == 0)
                                csb = WL_CHANSPEC_CTL_SB_UPPER;
                        else
                                csb = WL_CHANSPEC_CTL_SB_LOWER;
                }
        } else {
                status = -1;
                goto exit;
        }
                                                                                                  
        band = cl->band;
                                                                                                  
        if (wl_ioctl(name, WLC_GET_COUNTRY_LIST, cl, cl->buflen) == 0) {
                for(i = 0; i < cl->count; i++) {
                        abbrev = &cl->country_abbrev[i*WLC_CNTRY_BUF_SZ];
                        /* Use chanspec iovar for N phy */
                        if ((!strcmp(phytype, "n")) && (cur_phytype == WLC_PHY_TYPE_N))
                                wl_print_chanspec_list(wp, name, phytype, abbrev, band, bw, csb);
                        else
                                wl_print_channel_list(wp, name, phytype, abbrev, band);
                }
        }
                                                                                                  
                                                                                                  
exit:
        if (cl)
                free((void *)cl);
        return status;
}
// end 11N

#ifndef NOUSB
static int nvMap(char_t *url)
{
	cprintf("MAP:%s\n", url);
	if (strcmp(url, "Advanced_StorageUser_Content.asp")==0)
	{
		setSharedEntry(atoi(nvram_get("sh_path_x")));	
	}
	
	return 0;
}
#endif

struct ej_handler ej_handlers[] = {
	{ "nvram_get_x", ej_nvram_get_x},
	{ "nvram_get_f", ej_nvram_get_f},
	{ "nvram_get_list_x", ej_nvram_get_list_x},
	{ "nvram_get_buf_x", ej_nvram_get_buf_x},
	{ "nvram_get_table_x", ej_nvram_get_table_x},
	{ "nvram_match_x", ej_nvram_match_x},
	{ "nvram_double_match_x", ej_nvram_double_match_x},
	{ "nvram_match_both_x", ej_nvram_match_both_x},
	{ "nvram_match_list_x", ej_nvram_match_list_x},
	{ "select_channel", ej_select_channel},
	{ "select_country", ej_select_country},
	{ "urlcache", ej_urlcache},
	{ "uptime", ej_uptime},
	{ "nvram_dump", ej_dump},
	{ "load_script", ej_load},
#ifndef NOUSB
	{ "select_folder", ej_select_folder},
#endif
	{ "select_list", ej_select_list},
#ifdef ASUS_DDNS //2007.03.27 Yau add
	{ "nvram_get_ddns", ej_nvram_get_ddns},
#endif
// 2007.10 James {
	{ "update_variables", update_variables_ex},
	{ "convert_asus_variables", convert_asus_variables},
	{ "asus_nvram_commit", asus_nvram_commit},
	{ "notify_services", ej_notify_services},
	{ "detect_if_wan", detect_if_wan},
	{ "detect_wan_connection", detect_wan_connection},
	{ "detect_dhcp_pppoe", detect_dhcp_pppoe},
	{ "get_wan_status_log", get_wan_status_log},
	{ "wanlink", wanlink_hook},
	{ "wan_action", wan_action_hook},
	{ "get_parameter", ej_get_parameter},
	{ "login_state_hook", login_state_hook},
	{ "dumpleases", ej_dumpleases},
	{ "get_nvram_list", ej_get_nvram_list},
	{ "dhcp_leases", ej_dhcp_leases},
	{ "get_arp_table", ej_get_arp_table},
	{ "get_arl_table", ej_get_arl_table},
	{ "get_static_client", ej_get_static_client},
	{ "get_changed_status", ej_get_changed_status},
	{ "wl_auth_list", ej_wl_auth_list},
	{ "get_printer_info", ej_get_printer_info},
#ifdef DLM
	{ "disk_pool_mapping_info", ej_disk_pool_mapping_info}, 
	{ "available_disk_names_and_sizes", ej_available_disk_names_and_sizes},
	{ "safely_remove_disk", ej_safely_remove_disk},
	{ "do_claim_disk", ej_do_claim_disk},
	{ "get_AiDisk_status", ej_get_AiDisk_status},
	{ "set_AiDisk_status", ej_set_AiDisk_status},
	{ "get_all_accounts", ej_get_all_accounts},
	{ "get_share_tree", ej_get_share_tree},
	{ "get_folder_tree", ej_get_folder_tree},
	{ "get_share_mode", ej_get_share_mode},
	{ "set_share_mode", ej_set_share_mode},
	{ "get_permissions_of_account", ej_get_permissions_of_account},
	{ "set_account_permission", ej_set_account_permission},
	{ "initial_account", ej_initial_account},
	{ "create_account", ej_create_account},
	{ "delete_account", ej_delete_account},
	{ "modify_account", ej_modify_account},
	{ "create_sharedfolder", ej_create_sharedfolder},
	{ "delete_sharedfolder", ej_delete_sharedfolder},
	{ "modify_sharedfolder", ej_modify_sharedfolder},
#endif
	{ "shown_time", ej_shown_time},
	{ "shown_language_option", ej_shown_language_option},
	{ "nvram_char_to_ascii", ej_nvram_char_to_ascii},
// 11N
	{ "wl_nphy_set", ej_wl_nphy_set },
	{ "wl_channel_list", ej_wl_channel_list },
// 2007.10 James }
	{ NULL, NULL }
};

#endif /* !WEBS */

void websSetVer(void)
{
	FILE *fp;
	char buf[1];
	unsigned long *imagelen;
	char dataPtr[4];
	char verPtr[64];
	char productid[13];
	char fwver[12];
	int i;

	strcpy(productid, "WLHDD");
	strcpy(fwver, "0.1.0.1");

	if ((fp = fopen("/dev/mtd/1", "rb"))!=NULL)
	{
		if (fseek(fp, 4, SEEK_SET)!=0) goto write_ver;
		fread(dataPtr, 1, 4, fp);
		imagelen = (unsigned long *)dataPtr;

		cprintf("image len %x\n", *imagelen);
		if (fseek(fp, *imagelen - 64, SEEK_SET)!=0) goto write_ver;
		cprintf("seek\n");
		if (!fread(verPtr, 1, 64, fp)) goto write_ver;
		cprintf("ver %x %x", verPtr[0], verPtr[1]);
		strncpy(productid, verPtr + 4, 12);
		productid[12] = 0;
		sprintf(fwver, "%d.%d.%d.%d", verPtr[0], verPtr[1], verPtr[2], verPtr[3]);

		cprintf("get fw ver: %s\n", productid);
		fclose(fp);
	}
write_ver:
	nvram_set_f("general.log", "productid", productid);
	nvram_set_f("general.log", "firmver", fwver);	
}

static int
ej_select_country(int eid, webs_t wp, int argc, char_t **argv)
{
	char *country, *sid;	
	country_name_t *cntry;
	int ret=0;

	if (ejArgs(argc, argv, "%s", &sid) < 1) {
		websError(wp, 400, "Insufficient args\n");
		return -1;
	}
	
        country=nvram_safe_get_x("", "wl_country_code");
                                
	for(cntry = country_names; cntry->name; cntry++)
	{ 
		if (strcmp(country, cntry->abbrev)!=0)
                   ret += websWrite(wp, "<option value=\"%s\">%s</option>", cntry->abbrev, cntry->name);
                else
                   ret += websWrite(wp, "<option value=\"%s\" selected>%s</option>", cntry->abbrev, cntry->name);
        }                                             
	return ret;
}

int
wl_channels_in_country_asus(char *abbrev, int channels[])
{
	int i, j;
	wl_channels_in_country_t *cic = (wl_channels_in_country_t *)malloc(WLC_IOCTL_MAXLEN);
	char var[256], *next;
	int log_level, log_drop, log_accept;

	i=0;
	
	if (nvram_match_x("", "wl_chan_list", "")) {
		for(i=0;i<14;i++)
		{
			channels[i] = i+1;
			channels[i+1] = -1;
		}	
	}
	else {
		foreach(var, nvram_safe_get_x("", "wl_chan_list"), next) {
			channels[i]=atoi(var);
			channels[i+1]=-1;
			i++;
		}
	}
	return i;
}

int
wl_channels_in_country(char *abbrev, int channels[])
{
	int i, j, unit;
	wl_channels_in_country_t *cic = (wl_channels_in_country_t *)malloc(WLC_IOCTL_MAXLEN);
	char tmp[100], prefix[] = "wlXXXXXXXXXX_";
	char *name;
	channel_info_t ci;

	if ((unit = atoi(nvram_safe_get_x("", "wl_unit"))) < 0)
		return -1;

	snprintf(prefix, sizeof(prefix), "wl%d_", unit);
	name = nvram_safe_get_x("", strcat_r(prefix, "ifname", tmp));		
	

	i = 0;
	channels[i++] = 0;
	channels[i] = -1;

	if (strlen(abbrev)==0) return 0;

//	cic->buflen = sizeof(ibuf2);
	cic->buflen = WLC_IOCTL_MAXLEN;
	strcpy(cic->country_abbrev, abbrev);
	cic->band = WLC_BAND_2G;
	cic->count = 0;


	if (wl_ioctl(name, WLC_GET_CHANNELS_IN_COUNTRY, cic, cic->buflen) == 0) 	{
		if (cic->count == 0)
			return 0;

		for(j = 0; j < cic->count; j++)
		{
			channels[i++] = cic->channel[j];
		}

		channels[i] = -1;
	}
	return cic->count;

}

/* 
 * Kills process whose PID is stored in plaintext in pidfile
 * @param	pidfile	PID file, signal
 * @return	0 on success and errno on failure
 */
int
kill_pidfile_s(char *pidfile, int sig)
{
	FILE *fp = fopen(pidfile, "r");
	char buf[256];
	extern errno;

	if (fp && fgets(buf, sizeof(buf), fp)) {
		pid_t pid = strtoul(buf, NULL, 0);
		fclose(fp);
		return kill(pid, sig);
  	} else
		return errno;
}

void replace (char *Input){
        char *temp;
        char p[128];
        int i = 0;
        int j = 0;

	temp = Input;
        while (*(temp+i))
        {
                if(*(temp+i) == '%'){
                        if ((*(temp+i+1) == '2') && (*(temp+i+2) == '0')){
                                p[j] = ' ';
                                i=i+2;
                                j=j+1;
                        }
                }
                else
                        p[j++] = *(temp+i);
		p[j]=0;
                i++;
        }
	strcpy(NewDirPath, p);
}

void logmessage(char *logheader, char *fmt, ...)
{
  va_list args;
  char buf[512];

  va_start(args, fmt);

  vsnprintf(buf, sizeof(buf), fmt, args);
  openlog(logheader, 0, 0);
  syslog(0, buf);
  closelog();
  va_end(args);
}

#ifdef DLM
void
umount_disc_parts(int usb_port)
{
	if (usb_port==1)
		eval("eject_usb1");
	else if (usb_port==2)
		eval("eject_usb2");
}
#endif
