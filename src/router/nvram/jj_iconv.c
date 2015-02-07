/*************************************************************************

Convert encoding of given file from one encoding to another.

usage: jj_iconv [INPUT FILE] [OUTPUT FILE] [source locale] [destination locale]
available locales: BIG5, GB2312 & UTF-8

*************************************************************************/

#include <stdio.h>


char tunable_local_charset[5] = "utf8";
char tunable_remote_charset[5] = "cp950";
const char* NLS_NVRAM_U2C="asusnlsu2c";
const char* NLS_NVRAM_C2U="asusnlsc2u";
static char *xfr_buf=NULL;
static int xfr_buf_init=0;

char tmp[1024];

char * 
local2remote(const char *buf) 
{
	char *p;

	if ((xfr_buf==NULL) && (xfr_buf_init==0))
	{
		xfr_buf = (char *)malloc(2048);
		if (xfr_buf!=NULL)
			xfr_buf_init=1;
	}

	sprintf(tmp, "%s%s_%s", NLS_NVRAM_U2C, tunable_local_charset, buf);
	if (p=nvram_xfr(tmp))
	{
		strcpy(xfr_buf, p);
//		fprintf(stderr, "fn: %s, %d\n", xfr_buf, strlen(xfr_buf));
		return xfr_buf;
	}
	else
		return NULL;
}

char * 
remote2local(const char *buf) 
{
	char *p;

	if ((xfr_buf==NULL) && (xfr_buf_init==0))
	{
		xfr_buf = (char *)malloc(2048);
		if (xfr_buf!=NULL)
			xfr_buf_init=1;
	}

	sprintf(tmp, "%s%s_%s", NLS_NVRAM_C2U, tunable_remote_charset, buf);
	if (p=nvram_xfr(tmp))
	{
		strcpy(xfr_buf, p);
//		fprintf(stderr, "param: %s, %d\n", xfr_buf, strlen(xfr_buf));
		return xfr_buf;
	}
	else
		return NULL;
}

int
main(int argc, char **argv)
{
	FILE *fp;
	FILE *fp2;
	char temp[1024];
	char temp2[1024];
	char locale_from[5];
	char locale_to[5];
	char *result;
	char outputfile[256]="";
	
	/* Skip program name */
	--argc;
	++argv;

	if (!*argv) 
	{
		printf("no arg!\n");
		printf("\n");
		printf("Convert encoding of given file from one encoding to another.\n");
		printf("Usage: jj_iconv [INPUT FILE] [OUTPUT FILE] [source locale] [destination locale]\n");
		printf("available locales: BIG5, GB2312 and UTF-8\n");
		return -1;
	}
	else if (argc<4)
	{
		printf("too few arg!\n");
		return -1;
	}
	else
	{
		strcpy(outputfile, *(argv+3));
		
		strcpy(locale_from, *argv++);
		if(strcmp(locale_from, "BIG5")==0)
			strcpy(locale_from, "cp950");
		else if(strcmp(locale_from, "GB2312")==0)
			strcpy(locale_from, "cp936");
		else if(strcmp(locale_from, "UTF-8")==0)
			strcpy(locale_from, "utf8");
		else
		{	
			printf("unknown source locale\n");
			goto error;
		}
		strcpy(tunable_remote_charset, locale_from);
			
		strcpy(locale_to, *argv++);
		if(strcmp(locale_to, "BIG5")==0)
			strcpy(locale_to, "cp950");
		else if(strcmp(locale_to, "GB2312")==0)
			strcpy(locale_to, "cp936");
		else if(strcmp(locale_to, "UTF-8")==0)
			strcpy(locale_to, "utf8");
		else
		{
			printf("unknown destination locale\n");
			goto error;
		}
		strcpy(tunable_local_charset, locale_to);

//		printf("from locale: %s\n", tunable_remote_charset);
//		printf("to locale: %s\n", tunable_local_charset);

		fp = fopen(*argv, "r");
		if(!fp)
		{
			printf("file not found: %s\n", *argv++);
			goto error;
		}
		else
		{
//			printf("opend file to read: %s\n", *argv++);
			fp2 = fopen(outputfile, "w");
			if(!fp2)
			{
				printf("can not open file to write: %s\n", *argv);
				goto error1;
			}
//			printf("opend file to write: %s\n", *argv);

			while (fgets(temp,1024,fp)!=NULL)
			{
				if( !(result=remote2local(temp)) )
					goto error1;
				else
				{
					if(strlen(result)==0)
						goto error1;
					else
						strcpy(temp2, result);					
				}
				if( !(result=local2remote(temp2)) )
					goto error1;
				else
				{
					if(strlen(result)==0)
						goto error1;
					else
						strcpy(temp, result);
				}
				fputs(temp, fp2);
			}

			fclose(fp);
			fclose(fp2);
		}
		return 0;
	}
error1:
	if(fp)
		close(fp);
	if(fp2)
		close(fp2);
error:
	fp=fopen(outputfile, "w");
	if(fp)
	{
		fprintf(fp, "convert error");
		close(fp);
	}
	return -1;
}
