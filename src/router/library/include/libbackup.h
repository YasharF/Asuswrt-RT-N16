#ifndef _LIBBACKUP_H_
#define _LIBBACKUP_H_

#include "constant.h"

#define	MAX_JOB_NAME_LEN	20
#define	MAX_FS_NAME_LEN		20
#define	MAX_FD_NAME_LEN		255
#define	MAX_AMPM_LEN		2
#define	MAX_JOB_NUM		5
#define	BACKUP_CONF		"/etc/backup_sh.conf"
#define DS_CONF	"/etc/CGI_ds.conf"

#define	BACKUP_CMD		"/usr/sbin/backup"


#define DIRECTION_N2F		0
#define	DIRECTION_F2N		1

#define ACTION_COPY	1
#define ACTION_UPDATE 	2
#define	ACTION_SYNC	3

/*****

[job_name]
//job_owner=		//owner of the job
job_mode=		//1,2,3	1-copy,2-update,3,sync
direction=		//0:NAS->FILE SERVER	1:FILE SERVER->NAS.
s_share=		//a share name
d_fs=			//dest file server
d_username=		//user name
d_userpass=		//user password
d_folder=		//dest share/floder
schl_en=		//0,1	0-disable,1-enable
//	when schl_en=1
*	schl_day=	//1,2,3,4,5,6.7,8(weekdays),9(every day...)
*	schl_hour=	//1-12
*	schl_min=	//0-59
*	schl_ampm=	//am/pm
//
*******/

//#define	JOB_OWNER	"job_owner"
#define	JOB_MODE	"job_mode"
#define	JOB_DIRECTION	"job_direction"
#define	JOB_S_SHARE	"s_share"
#define	JOB_SH_FOLDER	"sh_folder"
#define	JOB_D_FS	"d_fs"
#define	JOB_D_AUTH	"d_auth"
#define	JOB_D_USERNAME	"d_username"
#define	JOB_D_USERPASS	"d_userpass"
#define	JOB_FOLDER	"d_folder"
#define	JOB_SCHL_EN	"schl_en"

#define	JOB_SCHL_DAY	"schl_day"
#define	JOB_SCHL_HOUR	"schl_hour"
#define	JOB_SCHL_MIN	"schl_min"
#define	JOB_SCHL_AMPM	"schl_ampm"


typedef struct sh_backup
{
	char job_name[MAX_JOB_NAME_LEN+1];
//	char job_owner[FSH_MaxUserLen+1];
	int job_mode;
	int job_direction;
	char s_share[FSH_MaxShareLen+1];
	char sh_folder[MAX_FD_NAME_LEN+1];
	char d_fs[MAX_FS_NAME_LEN+1];
	int auth;
	char d_username[FSH_MaxUserLen+1];
	char d_userpass[FSH_MaxPassLen+1];
	char d_folder[MAX_FD_NAME_LEN+1];
	int schl_en;

	int schl_day;
	int schl_hour;
	int schl_min;
	char schl_ampm[MAX_AMPM_LEN+1];

	struct sh_backup *next;
}sh_backup;

typedef struct backup_info
{
	char job_name[MAX_JOB_NAME_LEN+1];
	int job_mode;
	int job_direction;
	char s_share[FSH_MaxShareLen+1];
	char sh_folder[MAX_FD_NAME_LEN+1];
	char d_fs[MAX_FS_NAME_LEN+1];
	int auth;
	char d_username[FSH_MaxUserLen+1];
	char d_userpass[FSH_MaxPassLen+1];
	char d_folder[MAX_FD_NAME_LEN+1];
}backup_info;

int GetBackupList(sh_backup **,int *);
int GetBackupInfo(backup_info *pInfo);
int AddABackup(sh_backup *);
int UpdateABackup(char *,sh_backup *);
int DelABackup(char *);
int NumOfBackupJobs(void);
char *transf(char *path,char flag);
int DoBackup(char *);

void FreeBackupList(sh_backup *);

int IsBackupExist(char *);
int IsShareBackuping(char *pShare);
int IsBackupDoing(char *pName);
int SchlAddBackup(sh_backup *pBackup);
int SchlDelBackup(char *job_name);
#define WINDOW_MODE		1
#define	LINUX_MODE		0
int InternalJob(char *pServer)
;
char *transf(char *path,char flag);

#define ERR_OPENFILE	1
#define	ERR_READFILE	2

#define	ERR_MEMALLOC	3
#define	ERR_EXIST	4
#define	ERR_NOT_EXIST	5

#define	ERR_JOB_MODE	6
#define	ERR_JOB_SCHL	7
#define	ERR_JOB_PATH	8
#define	ERR_JOB_DAY	9
#define	ERR_JOB_HOUR	10
#define	ERR_JOB_MIN	11
#define	ERR_JOB_AMPM	12

#define	ERR_WRITEFILE	13
#define	ERR_MAX_JOB	14
#define	ERR_JOB_NAME		15
#define ERR_LOOP	16
#define ERR_JOB_JOBNAME		17
#define	ERR_JOB_HOSTNAME	18
#define	ERR_JOB_USERNAME	19

#define	ERR_JOB_BUSY		20
#define	ERR_FULL_SCHL			21
#define	ERR_DUP_SERVER			22
#define ERR_SH_FOLDER			23
#define	ERR_DUP_FOLDER			24
#define ERR_JOB_DIRECTION		25
#define	ERR_SRC_DEST_LOOP		26
#endif
