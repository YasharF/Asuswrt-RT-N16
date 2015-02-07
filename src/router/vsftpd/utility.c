/*
 * Part of Very Secure FTPd
 * Licence: GPL v2
 * Author: Chris Evans
 * utility.c
 */

#include "utility.h"
#include "sysutil.h"
#include "str.h"
#include "defs.h"
#include <stdarg.h>	// Jiahao
//#include <iconv.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <bcmnvram.h>
#include "tunables.h"

const char* NLS_NVRAM_U2C="asusnlsu2c";	// Jiahao
const char* NLS_NVRAM_C2U="asusnlsc2u";
static char *xfr_buf=NULL;

extern char *nvram_xfr(char *);

//static int xfr_buf_init=0;
char tmp[1024];

#define DIE_DEBUG

void
die(const char* p_text)
{
#ifdef DIE_DEBUG
  bug(p_text);
#endif
  vsf_sysutil_exit(1);
}

void
die2(const char* p_text1, const char* p_text2)
{
  struct mystr die_str = INIT_MYSTR;
  str_alloc_text(&die_str, p_text1);
  str_append_text(&die_str, p_text2);
  die(str_getbuf(&die_str));
}

void
bug(const char* p_text)
{
  /* Rats. Try and write the reason to the network for diagnostics */
  vsf_sysutil_activate_noblock(VSFTP_COMMAND_FD);
  (void) vsf_sysutil_write_loop(VSFTP_COMMAND_FD, "500 OOPS: ", 10);
  (void) vsf_sysutil_write_loop(VSFTP_COMMAND_FD, p_text,
                                vsf_sysutil_strlen(p_text));
  (void) vsf_sysutil_write_loop(VSFTP_COMMAND_FD, "\r\n", 2);
  vsf_sysutil_exit(1);
}

void
vsf_exit(const char* p_text)
{
  (void) vsf_sysutil_write_loop(VSFTP_COMMAND_FD, p_text,
                                vsf_sysutil_strlen(p_text));
  vsf_sysutil_exit(0);
}

char * 
local2remote(const char *buf) 
{
	if (tunable_enable_iconv == 0) return NULL;
	char *p;

//	if ((xfr_buf==NULL) && (xfr_buf_init==0))
//	{
		xfr_buf = (char *)malloc(2048);
//		if (xfr_buf!=NULL)
//			xfr_buf_init=1;
//	}
	memset(tmp, 0, 1024);
	sprintf(tmp, "%s%s_%s", NLS_NVRAM_U2C, tunable_remote_charset, buf);
	if((p = (char *)nvram_xfr(tmp)) != NULL){
		strcpy(xfr_buf, p);
		return xfr_buf;
	}
	else
		return NULL;
}

char * 
remote2local(const char *buf) 
{
	if (tunable_enable_iconv == 0) return NULL;
	char *p;

//	if ((xfr_buf==NULL) && (xfr_buf_init==0))
//	{
		xfr_buf = (char *)malloc(2048);
//		if (xfr_buf!=NULL)
//			xfr_buf_init=1;
//	}
	memset(tmp, 0, 1024);
	sprintf(tmp, "%s%s_%s", NLS_NVRAM_C2U, tunable_remote_charset, buf);
	if((p = (char *)nvram_xfr(tmp)) != NULL){
		strcpy(xfr_buf, p);
		return xfr_buf;
	}
	else
		return NULL;
}
