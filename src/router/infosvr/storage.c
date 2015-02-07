#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <net/if.h>
#include <bcmnvram.h>
#include <shutils.h>
#include <wlutils.h>
#include <syslog.h>
#include "iboxcom.h"

#define MAXARRAY 64
#define MAXARRSIZE  64

#ifdef NOPARSER
int
parse_array(char *file, char *func, char *strarr)
{
	FILE *fp;
	char line[1024], *s, *l;
	int flag=0, len;
	int i;

	if(!(fp=fopen(file, "r+"))) return 0;
	
	while(fgets(line, sizeof(line), fp))
	{
		if(strstr(line, func)) flag=1;
		if(flag && strstr(line, "return"))
		{
			flag=2;
			break;
		}	
	}
	fclose(fp);

	if (flag!=2) return 0;

	l = strchr(line, ']');
	if(l) *l=0;
	l = strchr(line, '[');
	l++;
	s=l;
	len = strlen(s);
	while (strsep(&s, ","));

	i=0;
	/* Set token values */
	for (s = l; s < &l[len] && *s; s+=strlen(s)+1) 
	{
		strcpy(&strarr[i*MAXARRSIZE], s);
		i++;

		if(i==MAXARRAY) break;
	}
	return i;	
}

int isClaimedIDE(char *model)
{
	char tmparray[MAXARRAY][MAXARRSIZE];
	int tmpidx;
	unsigned long size=0;
	int i;

	tmpidx=parse_array("/tmp/diskstatus", "claimed_disk_interface_names", tmparray);

	for(i=0;i<tmpidx;i++)
	{
		printf("%d:%s\n", i, &tmparray[i][0]);
		if (strstr(&tmparray[i][0], "IDE")) break;
	}

	if (i==tmpidx) return 0;

	tmpidx=parse_array("/tmp/diskstatus", "claimed_disk_total_size", tmparray);
	if (i>=tmpidx) return 0;
	size = atoi(&tmparray[i][0]);
	
	tmpidx=parse_array("/tmp/diskstatus", "claimed_disk_model_info", tmparray);
	if (i>=tmpidx) return 0;
	strcpy(model, &tmparray[i][0]);
	return size;
}

int isBlankIDE(char *model)
{
	char tmparray[MAXARRAY][MAXARRSIZE];
	int tmpidx;
	unsigned long size=0;
	int i;

	tmpidx=parse_array("/tmp/diskstatus", "blank_disk_interface_names", tmparray);

	for(i=0;i<tmpidx;i++)
	{
		printf("%d:%s\n", i, &tmparray[i][0]);
		if (strstr(&tmparray[i][0], "IDE")) break;
	}

	if (i==tmpidx) return 0;

	tmpidx=parse_array("/tmp/diskstatus", "blank_disk_total_size", tmparray);
	if (i>=tmpidx) return 0;
	size = atoi(&tmparray[i][0]);
	
	tmpidx=parse_array("/tmp/diskstatus", "blank_disk_model_info", tmparray);
	if (i>=tmpidx) return 0;
	strcpy(model, &tmparray[i][0]);
	return size;
}


int isPrinter(char *model1, char *model2)
{
	char tmparray[MAXARRAY][MAXARRSIZE];
	int tmpidx;
	unsigned long size=0;
	int i;

	tmpidx=parse_array("/tmp/diskstatus", "printer_models", tmparray);
	if (tmpidx<=0) return 0;

	if (tmpidx>=1)
		strcpy(model1, &tmparray[0][0]);
	if (tmpidx>=2)
		strcpy(model2, &tmparray[1][0]);
	
	return tmpidx;
}
#endif

int
getStorageStatus(STORAGE_INFO_T *st)
{
	char tmparray[MAXARRAY][MAXARRSIZE];

	st->Capability=0;

	unsigned long apps_status=0;

	// count apps_status
	if (nvram_match("swap_on", "1"))
	{
		apps_status|=APPS_STATUS_SWAP;
	}
	if (nvram_match("apps_comp", "1"))
	{
		apps_status|=APPS_STATUS_FILECOMPLETENESS;
	}
	if (nvram_match("apps_disk_free", "1"))
	{
		apps_status|=APPS_STATUS_FREESPACE;
	}
	if (nvram_match("ftp_running", "1") && nvram_match("st_samba_mode_x", "1"))
	{
		apps_status|=APPS_STATUS_SAMBAMODE;
	}
	if (nvram_match("apps_running", "1") && nvram_match("apps_dl_x", "1"))
	{
		apps_status|=APPS_STATUS_RUNNING;
	}
	if (nvram_invmatch("usb_disc0_dev", "") || nvram_invmatch("usb_disc1_dev", ""))
	{
		apps_status|=APPS_STATUS_DISCONPORT;
	}
	if (nvram_match("apps_dms_usb_port_x", "2"))
	{
		apps_status|=APPS_STATUS_DMPORT;
	}
	if (nvram_match("apps_dlx", "1"))
	{
		apps_status|=APPS_STATUS_DMMODE;
	}
	if (nvram_match("apps_status_checked", "1"))
	{
		apps_status|=APPS_STATUS_CHECKED;
	}
	if ( (nvram_match("usb_disc0_port", "1") || nvram_match("usb_disc0_port", "2")) &&
	     (strcmp(nvram_safe_get("usb_disc0_port"), nvram_safe_get("usb_disc1_port"))!=0)
	   )
	{
		apps_status|=APPS_STATUS_USBPORT1;
	}
	if ( (nvram_match("usb_disc1_port", "1") || nvram_match("usb_disc1_port", "2")) &&
	     (strcmp(nvram_safe_get("usb_disc0_port"), nvram_safe_get("usb_disc1_port"))!=0)
	   )
	{
		apps_status|=APPS_STATUS_USBPORT2;
	}
        if(strcmp(nvram_safe_get("mnt_type"), "ntfs") == 0)
        {
                apps_status|=APPS_STATUS_FS_NTFS;
        }
        if(strcmp(nvram_safe_get("dm_block"), "1") == 0)
        {
                apps_status|=APPS_STATUS_BLOCKED;
        }
        if(!strcmp(nvram_safe_get("st_samba_mode"), "2") || !strcmp(nvram_safe_get("st_samba_mode"), "4"))
        {
                apps_status|=APPS_STATUS_SMBUSER;
        }
	st->AppsStatus=apps_status;

	if(nvram_invmatch("apps_pool", "") && nvram_invmatch("apps_share", ""))
	{		
		st->Capability = (unsigned long) atoi(nvram_safe_get("apps_caps"));
		strcpy(st->AppsPool, nvram_safe_get("apps_pool"));
		strcpy(st->AppsShare, nvram_safe_get("apps_share"));

		strcpy(st->DiskModel, nvram_safe_get("apps_version"));	// 2008.09 James. for apps_version.
	}
	else
	{
		st->Capability=0;
		st->AppsPool[0]=0;
		st->AppsShare[0]=0;

		st->DiskModel[0]=0;	// 2008.09 James. for apps_version.
	}

	st->DiskSize=0;
	st->DiskStatus=DISK_STATUS_NONE;
	//st->DiskModel[0]=0;	// 2008.09 James. for apps_version.
	st->PrinterModel1[0]=0;
	st->PrinterModel2[0]=0;

#ifdef NOPARSER
	// get from page
	eval("wget", "http://127.0.0.1/diskstatus.asp", "-O", "/tmp/diskstatus");
	// find claimed ide
	if ((st->DiskSize=isClaimedIDE(st->DiskModel)))
	{
		st->DiskStatus=DISK_STATUS_CLAIMED;
	}
	else if ((st->DiskSize=isBlankIDE(st->DiskModel)))
	{	
		st->DiskStatus=DISK_STATUS_BLANK;
	}
	isPrinter(st->PrinterModel1, st->PrinterModel2);
#endif

	//printf("Storage: %d %s\n", st->DiskStatus, st->AppsShare);
	return 0;
}

