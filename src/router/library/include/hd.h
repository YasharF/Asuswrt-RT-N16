#ifndef _HARDDISK_API_H_
#define _HARDDISK_API_H_

#define DSK_NUM			2

#define DSK_USB			0 // USB HDD
#define DSK_FLASH		1 // Flash Disk

#define ACTION_NOBACKUP   3
#define ACTION_BACKUP   2
#define ACTION_ADD		1
#define ACTION_DEL    	0

#define FLAG 		".dongle"
#define SWAP_SIZE		128000		//K

#define DSK_PARTITION_LEN		10	//partition name
#define DSK_XFS					1	//filesystem xfs
#define DSK_EXT2				2	//filesystem ext2
#define DSK_TYPE_LEN			30	//harddisk type name

#define DSK_STAT_UMOUNT_NOACT		4
#define DSK_STAT_UMOUNT_SCANNING	3
#define DSK_STAT_UMOUNT_FDISKING	2
#define DSK_STAT_NONE			1
#define DSK_STAT_READY			0

#define HD_ACT_SPIN_ENABLE		0
#define HD_ACT_SPIN_DISABLE		1
#define HD_ACT_PWR_ENABLE		2

#define DSK_CHK_NONE			0
#define DSK_CHK_ERROR			1
#define DSK_CHK					2

#define	DSK_OK				0	// OK
#define	DSK_NO_DISK			-1
#define	DSK_ERR_TYPR		-2	// Incorrect partition type


#define DSK_ERROR			1;


typedef struct  partition_info {
        unsigned long   total_size;     /** unit is K bytes **/
        unsigned long   usage_size;     /** unit is K bytes **/
        unsigned long   free_size;      /** unit is K bytes **/
        unsigned long 	free_percent;
} partition_info;

typedef struct dsk_info {
	char	dsk_type[DSK_TYPE_LEN];	 // type of the hard disk.
	unsigned long	dsk_cylinders;   // cylinders of the hard disk.
	unsigned long	dsk_heads;    	 // heads of the hard disk
	unsigned long	dsk_sectors;  	 // sectors of the hard disk
	unsigned long	dsk_size;      	 // hard disk size in bytes
} dsk_info;

typedef struct hd_scan_schl{
	int action;			// 0-None,1-Check error,2-Not check error;
	int date;			//0-None, 1-Monday, 2-Tuesday, 3-Wednesday, 4-Thursday, 5-Friday,
					//6-Saturday, 7-Sunday, 8-weekdays
	int hour;			//24 hour
	int minute;
}hd_scan_schl;

typedef struct hd_idle_hdd{
	int idle_hdd;
}hd_idle_hdd;

#define 	MAX_SCHLSCAN_NUM	7
#define 	MAX_SCHLSMT_NUM		7
int Add_Schl_Scandisk(int number,hd_scan_schl scan_shl[],int d_no,int act_type);
int DSK_DetectAndMountDisk(int valid_num[DSK_NUM],int nvalid_num[DSK_NUM],int disk_num);
int DSK_DetectSysDisk(int valid_num[DSK_NUM],int nvalid_num[DSK_NUM]);
int DSK_Format(int nDskNum);
int DSK_GetDiskStatus(int nDskNum,int *status, int *details);
int DSK_GetDskInfo (int nDskNum, dsk_info *pDskInfo);
int DSK_GetIdleHD(hd_idle_hdd *pIdle);
int DSK_MountDisk(int nDskNum, char *mount_point, int partition);
int DSK_Partion(int nDskNum);
int DSK_PartitionAndFormat(int nDskNum);
int DSK_ScanDisk(int action, int scan_method, int nDskNum,int delaytime, int  *result);
int DSK_Schl_ScanDisk(int act_type, int schl_num, hd_scan_schl scan_shl[], int NDskNum);
int DSK_SetDiskOptions(int action, int delaytime, int nDskNum);
int DSK_SetIdleHD(hd_idle_hdd *pIdle);
int DSK_Swap(int nDskNum);
int DSK_UnmountDisk(int nDskNum,int partition);
int DSK_UpdateSysDiskInfo(int action,int hditem,int partition);
int PartitionInfo(int nDskNum,struct partition_info *nptr);
int Read_Schl_Scandisk(int number,hd_scan_schl scan_shl[],char *i_name);
int CheckFlashPart(void);
int DSK_Schl_Backup(int act_type, int schl_num, hd_scan_schl backup_shl[]);
int DSK_GetUserDiskInfo(struct partition_info *nptr);
int DSK_UnmoutFormatDisk();

#endif
