/* cgi-lib.h - header file for cgi-lib.c
   Eugene Kim, <eekim@eekim.com>
   $Id: file-upload.h,v 1.1.1.1 2008/07/21 09:18:44 james26_jang Exp $

   Copyright (C) 1996, 1997 Eugene Eric Kim
   All Rights Reserved
*/

#ifndef _CGI_LIB
#define _CGI_LIB 1

#include <stdlib.h>

/* change this if you are using HTTP upload */
//#ifndef UPLOADDIR
//#define UPLOADDIR "/tmp"
//#endif
#define WRITE_FILE		1
#define NO_WRITE_FILE	0

/* CGI Environment Variables */
#define CONTENT_TYPE getenv("CONTENT_TYPE")
#define CONTENT_LENGTH getenv("CONTENT_LENGTH")
#define HTTP_USER_AGENT getenv("HTTP_USER_AGENT")
typedef struct {
  char *name;
  char *value;
} entrytype;

typedef struct _node {
  entrytype entry;
  struct _node* next;
} node;

typedef struct {
  node* head;
} llist;

int read_file_upload(llist* entries,char *pUploadDir,int filesize,int flag);
char *cgi_val(llist l,char *name);

char *newstr(char *str);
char *lower_case(char *buffer);
void show_html_page(char *loc);

void list_create(llist *l);
short on_list(llist *l,node *w);
node* list_insafter(llist* l, node* w, entrytype item);
void clear_list(llist* l);
#endif
