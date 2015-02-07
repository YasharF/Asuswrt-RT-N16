#ifndef _FLASH_H_
#define _FLASH_H_

#define	SYSCONF	"/etc/CGI_ds.conf"

#define	BOOT_MTD_DEV	"/dev/mtd0"
#define	CONF_MTD_DEV	"/dev/mtd1"
#define	KERN_MTD_DEV	"/dev/mtd2"
#define	SYST_MTD_DEV	"/dev/mtd3"

#define	MTD_BOOT		0x01
#define	MTD_CONFIG		0x02
#define	MTD_KERNEL		0x04
#define	MTD_SYSTEM		0x08

#define	FLASH_SIZE	0x00800000

#define BOOT_SIZE	0x00040000
#define CONFIG_SIZE	0x00020000
#define KERNEL_SIZE	0x00100000
#define SYSTEM_SIZE	0x006a0000

#define	UI_SIZE		0x00100000
#define	UI_OFFSET	(SYSTEM_SIZE-UI_SIZE)
#define	UI_SIZE_BYTE	4

#define	PARTITION_NUM	        4

#define BLOCK_SIZE	            0x00020000

#define NODE_ADDRESS	(BOOT_SIZE-0x50)

int flash_erase_mtd(int which);
int flash_get_configf_size(int which);
int flash_read_config(int which,char *fname,ssize_t size);
int flash_write_config(char *fname,int which);
int flash_write_buf(char *buf, ssize_t size, int which);
int StoreUI(char *filename);
int ReadUI(void);

#define	FL_OPEN_DEV_ERR		1
#define	FL_READ_DEV_ERR		2
#define	FL_WRITE_DEV_ERR	3
#define	FL_ERASE_ERR		4
#define	FL_NOT_CONFIG_DEV	5
#define	FL_TOO_LONG_SIZE	6
#define	FL_BAD_CONFIG		7
#define	FL_NO_ENOUGH_MEM	8
#define	FL_OPEN_FILE_ERR	9
#define	FL_WRITE_FILE_ERR	10
#define	FL_BUF_EMPTY		11

#endif
