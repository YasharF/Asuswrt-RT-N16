#ifndef	__DISK_TABLE_H__
#define	__DISK_TABLE_H__

#include <sys/types.h>

#include <errno.h>
#include <unistd.h>

#if defined(__GNUC__) || defined(HAS_LONG_LONG)
typedef long long       ext2_loff_t;
#else
typedef long            ext2_loff_t;
#endif

extern ext2_loff_t ext2_llseek (unsigned int, ext2_loff_t, unsigned int);

#ifdef __linux__

#ifdef HAVE_LLSEEK
#include <syscall.h>

#else	/* HAVE_LLSEEK */

#if defined(__alpha__) || defined(__ia64__)

#define my_llseek lseek

#else
#include <linux/unistd.h>	/* for __NR__llseek */

static int _llseek (unsigned int, unsigned long,
		   unsigned long, ext2_loff_t *, unsigned int);

static _syscall5(int,_llseek,unsigned int,fd,unsigned long,offset_high,
		 unsigned long, offset_low,ext2_loff_t *,result,
		 unsigned int, origin)

static ext2_loff_t my_llseek (unsigned int fd, ext2_loff_t offset,
		unsigned int origin)
{
	ext2_loff_t result;
	int retval;

	retval = _llseek (fd, ((unsigned long long) offset) >> 32,
			((unsigned long long) offset) & 0xffffffff,
			&result, origin);
	return (retval == -1 ? (ext2_loff_t) retval : result);
}

#endif /* __alpha__ */

#endif	/* HAVE_LLSEEK */

ext2_loff_t ext2_llseek (unsigned int fd, ext2_loff_t offset,
			 unsigned int origin)
{
	ext2_loff_t result;
	static int do_compat = 0;

	if (!do_compat) {
		result = my_llseek (fd, offset, origin);
		if (!(result == -1 && errno == ENOSYS))
			return result;

		/*
		 * Just in case this code runs on top of an old kernel
		 * which does not support the llseek system call
		 */
		do_compat = 1;
		/*
		 * Now try ordinary lseek.
		 */
	}

	if ((sizeof(off_t) >= sizeof(ext2_loff_t)) ||
	    (offset < ((ext2_loff_t) 1 << ((sizeof(off_t)*8) -1))))
		return lseek(fd, (off_t) offset, origin);

	errno = EINVAL;
	return -1;
}

#else /* !linux */

ext2_loff_t ext2_llseek (unsigned int fd, ext2_loff_t offset,
			 unsigned int origin)
{
	if ((sizeof(off_t) < sizeof(ext2_loff_t)) &&
	    (offset >= ((ext2_loff_t) 1 << ((sizeof(off_t)*8) -1)))) {
		errno = EINVAL;
		return -1;
	}
	return lseek (fd, (off_t) offset, origin);
}

#endif 	/* linux */


#define DEFAULT_SECTOR_SIZE	512
#define MAX_SECTOR_SIZE	2048
#define SECTOR_SIZE	512	/* still used in BSD code */
#define MAXIMUM_PARTS	60

#define ACTIVE_FLAG     0x80

#define EXTENDED        0x05
#define WIN98_EXTENDED  0x0f
#define LINUX_PARTITION 0x81
#define LINUX_SWAP      0x82
#define LINUX_NATIVE    0x83
#define LINUX_EXTENDED  0x85

#define IS_EXTENDED(i) \
	((i) == EXTENDED || (i) == WIN98_EXTENDED || (i) == LINUX_EXTENDED)

#define SIZE(a)	(sizeof(a)/sizeof((a)[0]))




enum failure {  usage, usage2, ioctl_error,unable_to_open, 
                      unable_to_read, unable_to_seek,
	      unable_to_write, out_of_memory
};

enum action {fdisk, require, try_only, create_empty};

struct geom {
	unsigned int heads;
	unsigned int sectors;
	unsigned int cylinders;
};

#define	MAX_ATTACH_DEV	4
#define	MAX_MOUNT_VOL	8
#define	MAX_PORT1_MOUNT_VOL	8

#define		PROC_FLASH_0		"/proc/flash_sda"
#define		PROC_FLASH_1		"/proc/flash_sdb"
#define		PROC_FLASH_2		"/proc/flash_sdc"
#define		PROC_FLASH_3		"/proc/flash_sdd"
#define		PROC_FLASH_4		"/proc/flash_sde"

#define		PROC_HDD_0		"/proc/hdd_sda"
#define		PROC_HDD_1		"/proc/hdd_sdb"
#define		PROC_HDD_2		"/proc/hdd_sdc"
#define		PROC_HDD_3		"/proc/hdd_sdd"
#define		PROC_HDD_4		"/proc/hdd_sde"
struct dev_list{
	char hdd_flag[16];
	char flash_flag[16];
	char media_flag[32];
	char dev_name[16];
	char share_name[12];
	int	mounts;
	int bad_dev;
};

int DSK_MountUserDisk(struct dev_list *devices, int dev_no, int *total_mounts);
int DSK_MountPort1UserDisk(int *bad_dev, char *share_name, int *total_mounts);
int DSK_UnmountUserDisk(char *share_name, int dev_no);
int isUserDisk(char *disk_name);
void InitDev(struct dev_list *devices, int num);
int get_free_space(char *mount_point,char* type,unsigned long *size,int *percent, unsigned long *total);
int get_userdisk_detail(char *mount_point,char *type,unsigned long *size,unsigned long *cylinders,unsigned long *heads,unsigned long *sectors);
int get_userdisk_type(int disk_no,char* type);
int get_userdisk_info(char dev_name[16],unsigned long *size,unsigned long *cylinders,unsigned long *heads,unsigned long *sectors);
#endif

