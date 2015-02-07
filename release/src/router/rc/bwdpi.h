
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <syslog.h>
#include <getopt.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <net/if.h>
#include <bcmnvram.h>
#include <bcmparams.h>
#include <utils.h>
#include <shutils.h>
#include <shared.h>
#include <rtstate.h>

#define DATABASE	"/tmp/bwdpi/rule.trf"
#define QOS_CONF	"/tmp/bwdpi/qosd.conf"
#define WRS_CONF	"/tmp/bwdpi/wred.conf"
#define APP_SET_CONF	"/tmp/bwdpi/app_partol.conf"
#define APP_CLEAN_CONF	"/tmp/bwdpi/app_partol_clean.conf"
#define USR_BWDPI	"/usr/bwdpi/"
#define TMP_BWDPI	"/tmp/bwdpi/"

//iqos.c
extern char *dev_wan;
extern char *dev_lan;
extern void setup_qos_conf();
extern void stop_tm_qos();
extern void start_tm_qos();
extern int tm_qos_main(char *cmd);
extern void start_qosd();
extern void stop_qosd();
extern int qosd_main(char *cmd);

//wrs.c
extern void setup_wrs_conf();
extern void stop_wrs();
extern void start_wrs();
extern int wrs_main(char *cmd);

//stat.c
extern int stat_main(int timeval, char *action);

//dpi.c
extern int dpi_main(char *cmd);
