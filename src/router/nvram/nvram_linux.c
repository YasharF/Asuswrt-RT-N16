/*
 * NVRAM variable manipulation (Linux user mode half)
 *
 * Copyright 2004, Broadcom Corporation
 * All Rights Reserved.
 * 
 * THIS SOFTWARE IS OFFERED "AS IS", AND BROADCOM GRANTS NO WARRANTIES OF ANY
 * KIND, EXPRESS OR IMPLIED, BY STATUTE, COMMUNICATION OR OTHERWISE. BROADCOM
 * SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A SPECIFIC PURPOSE OR NONINFRINGEMENT CONCERNING THIS SOFTWARE.
 *
 * $Id: nvram_linux.c,v 1.1.1.1 2008/07/21 09:19:18 james26_jang Exp $
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <error.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>

#include <typedefs.h>
#include <bcmnvram.h>

#define PATH_DEV_NVRAM "/dev/nvram"

/* Globals */
static int nvram_fd = -1;
static char *nvram_buf = NULL;
static char *nvram_xfr_buf = NULL;

int file_to_buf(char *path, char *buf, int len)
{
        FILE *fp;
                                                                                                 
        memset(buf, 0 , len);
                                                                                                 
        if ((fp = fopen(path, "r"))) {
                fgets(buf, len, fp);
                fclose(fp);
                return 1;
        }
                                                                                                 
        return 0;
}

int
nvram_init(void *unused)
{
	if ((nvram_fd = open(PATH_DEV_NVRAM, O_RDWR)) < 0)
		goto err;

	/* Map kernel string buffer into user space */
	if ((nvram_buf = mmap(NULL, NVRAM_SPACE, PROT_READ, MAP_SHARED, nvram_fd, 0)) == MAP_FAILED) {
		close(nvram_fd);
		nvram_fd = -1;
		goto err;
	}

	return 0;

 err:
	perror(PATH_DEV_NVRAM);
	return errno;
}

char *
nvram_get(const char *name)
{
	size_t count = strlen(name) + 1;
	char tmp[100], *value;
	unsigned long *off = (unsigned long *) tmp;

	/* +++ Cherry Cho added in 2007/2/27. +++ */
	pid_t pid_lld2d, pid = getpid();
	char pid_buf[32], tmp1[130];
	memset(pid_buf, 0x0, 32);
	memset(tmp1, 0x0, 130);
	
	if( file_to_buf("/var/run/lld2d-br0.pid", pid_buf, sizeof(pid_buf)) && atoi(pid_buf)){
		pid_lld2d=atoi(pid_buf);
	    
		if((pid==pid_lld2d) && !strcmp(name,"wan_hostname"))
		{	
			name = "lltd_device_name";//Get value of lltd_device_name if wan_hostname is gotten by lltd
			count = strlen(name) + 1;
			
		}			
	}
	/* --- Cherry Cho added in 2007/2/27. --- */
	
	if (nvram_fd < 0)
		if (nvram_init(NULL))
			return NULL;

	if (count > sizeof(tmp)) {
		if (!(off = malloc(count)))
			return NULL;
	}

	/* Get offset into mmap() space */
	strcpy((char *) off, name);

	count = read(nvram_fd, off, count);

	if (count == sizeof(unsigned long))
		value = &nvram_buf[*off];
	else
		value = NULL;

	if (count < 0)
		perror(PATH_DEV_NVRAM);

	if (off != (unsigned long *) tmp)
		free(off);

	return value;
}

int
nvram_getall(char *buf, int count)
{
	int ret;

	if (nvram_fd < 0)
		if ((ret = nvram_init(NULL)))
			return ret;

	if (count == 0)
		return 0;

	/* Get all variables */
	*buf = '\0';

	ret = read(nvram_fd, buf, count);

	if (ret < 0)
		perror(PATH_DEV_NVRAM);

	return (ret == count) ? 0 : ret;
}

static int
_nvram_set(const char *name, const char *value)
{
	size_t count = strlen(name) + 1;
	char tmp[100], *buf = tmp;
	int ret;
	
	
	#if 1/* Cherry Cho added for fixing bug resulted from LLTD in 2007/1/17. */
	char tmp1[130];
	pid_t pid_lld2d, pid = getpid();
	char pid_buf[32];
	memset(pid_buf, 0x0, 32);
	memset(tmp1, 0x0, 130);
	
	if( file_to_buf("/var/run/lld2d-br0.pid", pid_buf, sizeof(pid_buf)) && atoi(pid_buf)){
		pid_lld2d=atoi(pid_buf);
	    
		if((pid==pid_lld2d) && !strcmp(name,"wan_hostname"))/* Cherry Cho added in 2007/2/26. */
		{	
			if(!strncmp(value,"BRCM_ROUTER",11)){
				sprintf(tmp1,"%s","ASUS_ROUTER");
				value = tmp1;
				name = "lltd_device_name";
				count = strlen(name) + 1;
			}
		}		
		
	}
	#endif
	
	if (nvram_fd < 0)
		if ((ret = nvram_init(NULL)))
			return ret;

	/* Unset if value is NULL */
	if (value)
		count += strlen(value) + 1;

	if (count > sizeof(tmp)) {
		if (!(buf = malloc(count)))
			return -ENOMEM;
	}

	if (value)
		sprintf(buf, "%s=%s", name, value);
	else
		strcpy(buf, name);

	ret = write(nvram_fd, buf, count);

	if (ret < 0)
		perror(PATH_DEV_NVRAM);

	if (buf != tmp)
		free(buf);

	return (ret == count) ? 0 : ret;
}

int
nvram_set(const char *name, const char *value)
{
	return _nvram_set(name, value);
}

int
nvram_unset(const char *name)
{
	return _nvram_set(name, NULL);
}

int
nvram_commit(void)
{
	int ret;

	if (nvram_fd < 0)
		if ((ret = nvram_init(NULL)))
			return ret;

	ret = ioctl(nvram_fd, NVRAM_MAGIC, NULL);

	if (ret < 0)
		perror(PATH_DEV_NVRAM);

	return ret;
}

/*
int
nvram_xfr(char *buf)
{
	int ret;
	char tmpbuf[1024];

	strcpy(tmpbuf, buf);

	if (nvram_fd < 0)
		if ((ret = nvram_init(NULL)))
			return ret;

	ret = ioctl(nvram_fd, NVRAM_MAGIC, tmpbuf);

	if (ret < 0)
		perror(PATH_DEV_NVRAM);

	printf("output: %s\n", tmpbuf);

	return ret;	
}
*/

char *			/* added by Jiahao for WL500gP */
nvram_xfr(char *buf)
{
	size_t count = strlen(buf)*1.5 + 1;
	int ret;
	char tmpbuf[1024];

	if (nvram_fd < 0)
		if ((ret = nvram_init(NULL)))
			return NULL;

	if (count > sizeof(tmpbuf))
		return NULL;
	
	strcpy(tmpbuf, buf);
	
	if (!nvram_xfr_buf)
		nvram_xfr_buf=(char *)malloc(1024+1);
	if (!nvram_xfr_buf)
		return NULL;

	ret = ioctl(nvram_fd, NVRAM_MAGIC, tmpbuf);

	if (ret < 0)
	{
		perror(PATH_DEV_NVRAM);
		return NULL;
	}
	else
	{
		strcpy(nvram_xfr_buf, tmpbuf);
		return nvram_xfr_buf;
	}
}
