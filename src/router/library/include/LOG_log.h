#ifndef  _LOG_MSG_
#define  _LOG_MSG_

#ifndef OK
#define OK   0
#endif

#define LOG_FILE  "/var/log/server.log"
#define HDD_LOG_FILE	"/share/hdd/conf/server.log"
#define FLASH_LOG_FILE	"/share/flash/conf/server.log"
#define LOG_SEC   "[LOG]"

#define LOG_LEVEL_0  0  /* Log all */
#define LOG_LEVEL_1  1  /* Fatal error (H/W error) */
#define LOG_LEVEL_2  2  /* Error */
#define LOG_LEVEL_3  3  /* Warning */
#define LOG_LEVEL_4  4  /* Status report */
#define LOG_LEVEL_5  5  /* Debug */
#define LOG_LEVEL_N -1  /* Do not log */
int LOG_LogMsgFLASH(char *msg);
int LOG_LogMsgHDD(char *msg);

int  LOG_LogMsg(char *task, int level, char *msg);
int  LOG_ClearLog(void);
long LOG_GetLogSize(void);
long LOG_GetLog(long start, char *buf, long size, int level);

typedef struct s_notify {
   char fmAddr[64];    /* From E-mail address */
   char toAddr[3][64]; /* 3 to E-mail address */
   char subject[64];   /* Subject string */
   int  log_level; /* LOG_LEVEL_0~5=Enable, LOG_LEVEL_N=Disable */
   int  not_level; /* LOG_LEVEL_0~5=Enable, LOG_LEVEL_N=Disable */
                   /* default=LOG_LEVEL_1. Messages with the level number */
                   /* are smaller than or equal to this value will be notified. */
} s_notify;

int LOG_SetNotify(s_notify *conf);
int LOG_GetNotify(s_notify *conf);
int LOG_MailNotify(char *to_addr, char *subject, char *msg);

// configuration in /etc/CGI_ds.conf
// [LOG]
// fm=    # from e-mail address
// 1to=   # to e-mail address 1
// 2to=   # to e-mail address 2
// 3to=   # to e-mail address 3
// sub=   # subject
// log=   # log level (enable/disable)
// not=   # notify level

#endif
