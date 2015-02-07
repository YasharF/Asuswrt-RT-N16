/*
 * Frontend command-line utility for Linux NVRAM layer
 *
 * Copyright 2004, Broadcom Corporation
 * All Rights Reserved.
 *
 * THIS SOFTWARE IS OFFERED "AS IS", AND BROADCOM GRANTS NO WARRANTIES OF ANY
 * KIND, EXPRESS OR IMPLIED, BY STATUTE, COMMUNICATION OR OTHERWISE. BROADCOM
 * SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A SPECIFIC PURPOSE OR NONINFRINGEMENT CONCERNING THIS SOFTWARE.
 *
 * $Id: main.c,v 1.2 2008/11/03 03:56:51 james26_jang Exp $
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <typedefs.h>
#include <bcmnvram.h>

static void
usage(void)
{
	fprintf(stderr, "usage: nvram [get name] [set name=value] [unset name] [show]\n");
	exit(0);
}

#ifdef ASUS
#define PROFILE_HEADER 		"HDR1"
#define PROFILE_HEADER_NEW	"HDR2"

/* added by Jiahao */
static char *NLS_NVRAM_U2C = "asusnlsu2c";
static char *NLS_NVRAM_C2U = "asusnlsc2u";

// save nvram to file
#if 0
int nvram_save(char *file, char *buf)
{
	FILE *fp;
	char *name;
	unsigned long count, filelen, i;

	if ((fp = fopen(file, "w")) == NULL) return -1;

	count = 0;
	for (name = buf; *name; name += strlen(name) + 1)
	{
		puts(name);
		count = count + strlen(name) + 1;
	}

	filelen = count + (1024 - count % 1024);
	fwrite(PROFILE_HEADER, 1, 4, fp);
	fwrite(&filelen, 1, 4, fp);
	fwrite(buf, 1, count, fp);
	for (i = count; i < filelen; i++) fwrite(name, 1, 1, fp);
	fclose(fp);
	return 0;
}
#endif

unsigned char get_rand()
{
	unsigned char buf[1];
	FILE *fp;
	size_t rc;
 
	fp = fopen("/dev/urandom", "r");
	if (fp == NULL) {
#ifdef ASUS_DEBUG
		fprintf(stderr, "Could not open /dev/urandom.\n");
#endif
		return 0;
	}
	fread(buf, 1, 1, fp);
	fclose(fp);

	return buf[0];
}

int nvram_save_new(char *file, char *buf)
{
	FILE *fp;
	char *name;
	unsigned long count, filelen, i;
	unsigned char rand = 0, temp;

	if ((fp = fopen(file, "w")) == NULL) return -1;

	count = 0;
	for (name = buf; *name; name += strlen(name) + 1)
	{
#ifdef ASUS_DEBUG
		puts(name);
#endif
		count = count + strlen(name) + 1;
	}

	filelen = count + (1024 - count % 1024);
	rand = get_rand() % 30;
#ifdef ASUS_DEBUG
	fprintf(stderr, "random number: %x\n", rand);
#endif
	fwrite(PROFILE_HEADER_NEW, 1, 4, fp);
	fwrite(&filelen, 1, 3, fp);
	fwrite(&rand, 1, 1, fp);
#ifdef ASUS_DEBUG
	for (i = 0; i < 4; i++)
	{
		fprintf(stderr, "%2x ", PROFILE_HEADER_NEW[i]);
	}
	for (i = 0; i < 3; i++)
	{
		fprintf(stderr, "%2x ", ((char *)&filelen)[i]);
	}
	fprintf(stderr, "%2x ", ((char *)&rand)[0]);
#endif
	for (i = 0; i < count; i++)
	{
		if (buf[i] == 0x0)
			buf[i] = 0xfd + get_rand() % 3;
		else
			buf[i] = 0xff - buf[i] + rand;
	}
	fwrite(buf, 1, count, fp);
#ifdef ASUS_DEBUG
	for (i = 0; i < count; i++)
	{
		if (i % 16 == 0) fprintf(stderr, "\n");
		fprintf(stderr, "%2x ", (unsigned char) buf[i]);
	}
#endif
	for (i = count; i < filelen; i++)
	{
		temp = 0xfd + get_rand() % 3;
		fwrite(&temp, 1, 1, fp);
#ifdef ASUS_DEBUG
		if (i % 16 == 0) fprintf(stderr, "\n");
		fprintf(stderr, "%2x ", (unsigned char) temp);
#endif
	}
	fclose(fp);
	return 0;
}

int issyspara(char *p)
{
	extern struct nvram_tuple router_defaults[];
	struct nvram_tuple *t, *u;

	for (t = router_defaults; t->name; t++)
	{
		if (strstr(p, t->name))
			break;
	}

	if (t->name) return 1;
	else return 0;
}

// restore nvram from file
#if 0
int nvram_restore(char *file, char *buf)
{
	FILE *fp;
	char header[8], *p, *v;
	unsigned long count, *filelen;

	if ((fp = fopen(file, "r+")) == NULL) return -1;

	count = fread(header, 1, 8, fp);
	if (count>=8 && strncmp(header, PROFILE_HEADER, 4) == 0)
	{
		filelen = (unsigned long *)(header + 4);
		count = fread(buf, 1, *filelen, fp);
	}
	fclose(fp);

	p = buf;

	while(*p)
	{
		//printf("nv:%s\n", p);
		v = strchr(p, '=');

		if(v != NULL)
		{
			*v++ = NULL;

			if (issyspara(p))
				nvram_set(p, v);

			p = v + strlen(v) + 1;
		}
		else
		{
			nvram_unset(p);
			p = p + 1;
		}
	}

	nvram_set("x_Setting", "1");
	return 0;
}
#endif

int nvram_restore_new(char *file, char *buf)
{
	FILE *fp;
	char header[8], *p, *v;
	unsigned long count, filelen, *filelenptr, i;
	unsigned char rand, *randptr;

	if ((fp = fopen(file, "r+")) == NULL) return -1;

	count = fread(header, 1, 8, fp);
	if (count>=8 && strncmp(header, PROFILE_HEADER, 4) == 0)
	{
		filelenptr = (unsigned long *)(header + 4);
#ifdef ASUS_DEBUG
		fprintf(stderr, "restoring original text cfg of length %x\n", *filelenptr);
#endif
		fread(buf, 1, *filelenptr, fp);
	}
	else if (count>=8 && strncmp(header, PROFILE_HEADER_NEW, 4) == 0)
	{
		filelenptr = (unsigned long *)(header + 4);
		filelen = *filelenptr & 0xffffff;
#ifdef ASUS_DEBUG
		fprintf(stderr, "restoring non-text cfg of length %x\n", filelen);
#endif
		randptr = (unsigned char *)(header + 7);
		rand = *randptr;
#ifdef ASUS_DEBUG
		fprintf(stderr, "non-text cfg random number %x\n", rand);
#endif
		count = fread(buf, 1, filelen, fp);
#ifdef ASUS_DEBUG
		fprintf(stderr, "non-text cfg count %x\n", count);
#endif

		for (i = 0; i < count; i++)
		{
			if ((unsigned char) buf[i] > ( 0xfd - 0x1))
				buf[i] = 0x0;
			else
				buf[i] = 0xff + rand - buf[i];
		}
#ifdef ASUS_DEBUG
		for (i = 0; i < count; i++)
		{
			if (i % 16 == 0) fprintf(stderr, "\n");
			fprintf(stderr, "%2x ", (unsigned char) buf[i]);
		}

		for (i = 0; i < count; i++)
		{
			if (i % 16 == 0) fprintf(stderr, "\n");
			fprintf(stderr, "%c", buf[i]);
		}
#endif
	}
	else
	{
		fclose(fp);
		return 0;
	}
	fclose(fp);

	p = buf;

	while(*p)
	{
		v = strchr(p, '=');

		if(v != NULL)
		{
			*v++ = NULL;

			if (issyspara(p))
				nvram_set(p, v);

			p = v + strlen(v) + 1;
		}
		else
		{
			nvram_unset(p);
			p = p + 1;
		}
	}

	nvram_set("x_Setting", "1");
	return 0;
}
#endif

/* NVRAM utility */
int
main(int argc, char **argv)
{
	char *name, *value, buf[NVRAM_SPACE];
	int size;

	/* Skip program name */
	--argc;
	++argv;

	if (!*argv) 
		usage();

	/* Process the remaining arguments. */
	for (; *argv; argv++) {
		if (!strncmp(*argv, "get", 3)) {
			if (*++argv) {
				if ((value = nvram_get(*argv)))
					puts(value);
			}
		}
		else if (!strncmp(*argv, "set", 3)) {
			if (*++argv) {
				strncpy(value = buf, *argv, sizeof(buf));
				name = strsep(&value, "=");
				nvram_set(name, value);
			}
		}
		else if (!strncmp(*argv, "unset", 5)) {
			if (*++argv)
				nvram_unset(*argv);
		}
		else if (!strncmp(*argv, "commit", 5)) {
			nvram_commit();
		}
#ifdef ASUS
		else if (!strncmp(*argv, "save", 4)) 
		{
			if (*++argv) 
			{
				nvram_getall(buf, sizeof(buf));	
//				nvram_save(*argv, buf);
				nvram_save_new(*argv, buf);
			}
			
		}
		else if (!strncmp(*argv, "restore", 7)) 
		{
			if (*++argv) 
			{
//				nvram_restore(*argv, buf);
				nvram_restore_new(*argv, buf);
			}
			
		}
		else if (!strncmp(*argv, "xfr", 3)) 
		{
			if (*++argv) 
			{				
				if ((value = nvram_xfr(*argv)))
					puts(value);
			}
		}
#endif
		else if (!strncmp(*argv, "show", 4) ||
			   !strncmp(*argv, "getall", 6)) {
			nvram_getall(buf, sizeof(buf));
			for (name = buf; *name; name += strlen(name) + 1)
				puts(name);
			size = sizeof(struct nvram_header) + (int) name - (int) buf;
			fprintf(stderr, "size: %d bytes (%d left)\n", size, NVRAM_SPACE - size);
		}
		if (!*argv)
			break;
	}

	return 0;
}	
