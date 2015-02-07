#ifndef _DISK_IO_TOOLS_
#define _DISK_IO_TOOLS_

#include <stdio.h>	// for FILE

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;

#define POOL_MOUNT_ROOT "/tmp/harddisk"
#define WWW_MOUNT_ROOT "/www"

#define csprintf(fmt, args...) do{\
	FILE *cp = fopen("/dev/console", "w");\
	if(cp) {\
		fprintf(cp, fmt, ## args);\
		fclose(cp);\
	}\
}while(0)

#define lprintf(fmt, args...) do{\
	FILE *ffp = fopen("/tmp/ftp_wrong_log.txt", "a");\
	if(ffp) {\
		fprintf(ffp, fmt, ## args);\
		fclose(ffp);\
	}\
}while(0)

extern char *read_whole_file(const char *);
extern int get_mount_path(const char *const, char **);
extern int mkdir_if_none(char *);
extern int delete_file_or_dir(char *);
extern int test_if_file(const char *);
extern int test_if_dir(const char *);
extern int test_if_mount_point_of_pool(const char *);
extern int test_if_System_folder(const char *);
extern int test_mounted_disk_size_status(char *);

extern char *get_upper_str(const char *const);
extern int upper_strcmp(const char *const, const char *const);
extern int upper_strncmp(const char *const, const char *const, int);
extern char *upper_strstr(const char *const, const char *const);
extern void strntrim(char *, size_t);
extern void write_escaped_value(FILE *, const char *);

#endif // _DISK_IO_TOOLS_
