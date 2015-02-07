/* cgi-lib.c - C routines that make writing CGI scripts in C a breeze
   Eugene Kim, <eekim@eekim.com>
   $Id: file-upload.c,v 1.1.1.1 2008/07/21 09:18:44 james26_jang Exp $
   Motivation: Perl is a much more convenient language to use when
     writing CGI scripts.  Unfortunately, it is also a larger drain on
     the system.  Hopefully, these routines will make writing CGI
     scripts just as easy in C.

   Copyright (C) 1996,1997 Eugene Eric Kim
   All Rights Reserved
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "file-upload.h"


char *newstr(char *str)
{
	char *tempstr = (char *)malloc(sizeof(char) * strlen(str) + 1);

	if (tempstr != NULL)
		strcpy(tempstr,str);
	return tempstr;
}

char *lower_case(char *buffer)
{
	char *tempstr = buffer;

	while (*buffer != '\0') {
		if (isupper(*buffer))
			*buffer = tolower(*buffer);
		buffer++;
	}
	return tempstr;
}

void show_html_page(char *loc)
{
	printf("Location: %s\n\n",loc);
}

void list_create(llist *l)
{
	l->head = 0;
}

short on_list(llist *l, node* w)
{
	return (w != 0);
}

node* list_insafter(llist* l, node* w, entrytype item)
{
	node* newnode = (node *)malloc(sizeof(node));

	newnode->entry.name = newstr(item.name);
	newnode->entry.value = newstr(item.value);
	if (l->head == 0) {
		newnode->next = 0;
		l->head = newnode;
	}
	else if (!on_list(l,w))
		/* ERROR: can't insert item after w since w is not on l */
		exit(1);
	else {
		/* insert node after */
		if (newnode == 0) /* can assume that w != NULL */
		/* ERROR: nothing to insert after */
			exit(1);
		else {
			newnode->next = w->next;
			w->next = newnode;
		}
	}
	return newnode;
}

void clear_list(llist* l)
{
	node* nextnode;
	node* current;

	current = l->head;
	while (current) {
		nextnode=current->next;
		if (current->entry.name)
			free(current->entry.name);
		if (current->entry.value)
			free(current->entry.value);
		free(current);
		current=nextnode;
	}
}

int getline(char s[], int lim)
{
	int c=0, i=0, num;

	for (i=0; (i<lim) && ((c=getchar())!=EOF) && (c!='\n'); i++) {
		s[i] = c;
	}
	if (c == '\n') 
		s[i] = c;
	if ((i==0) && (c!='\n'))
		num = 0;
	else if (i == lim)
		num = i;
	else
		num = i+1;
	return num;
}

int read_file_upload(llist* entries,char *pUploadDir,int filesize,int flag)
{
	long content_length;
	entrytype entry;
	node* window;
	char *uploadfname=NULL, *tempstr=NULL, *boundary=NULL,*p=NULL;
	char *buffer = (char *)malloc(sizeof(char) * BUFSIZ + 2);
	char *prevbuf = (char *)malloc(sizeof(char)*BUFSIZ + 3);
	short isfile,done,start;
	int j,fd=0,largefile=0;
	int bytesread,prevbytesread=0;
	int buffersize;

	if(!pUploadDir||!pUploadDir[0])
		return -1;
	if (CONTENT_LENGTH != NULL)
		content_length = atol(CONTENT_LENGTH);
	else
		return 0;
	if(content_length>(filesize*1024*1024)){
		flag=NO_WRITE_FILE;
		largefile=1;
	}
	/* get boundary */
	tempstr = newstr(CONTENT_TYPE);
	boundary = strstr(tempstr,"boundary=");
	boundary += (sizeof(char) * 9);
	/* create list */
	list_create(entries);
	window = entries->head;
	/* ignore first boundary; this isn't so robust; improve it later */
	getline(buffer,BUFSIZ);
	/* now start parsing */
	while ((bytesread=getline(buffer,BUFSIZ)) != 0) {
		start = 1;
		/* this assumes that buffer contains no binary characters.
		if the buffer contains the first valid header, then this
		is a safe assumption.  however, it should be improved for
		robustness sake. */
		buffer[bytesread] = '\0';
		entry.name = (char *)malloc(sizeof(char) * BUFSIZ + 1);
		if(!entry.name)
			goto err;
		memset(entry.name,'\0',sizeof(char) * BUFSIZ + 1);
		strcpy(entry.name,buffer+38);
		entry.value = (char *)malloc(sizeof(char) * BUFSIZ + 1);
		if(!entry.value){
			free(entry.name);
			goto err;	
		}	
		memset(entry.value,'\0',sizeof(char) * BUFSIZ + 1);
		buffersize = BUFSIZ;
		p=strchr(entry.name,'\"');
		if(p)
			*p='\0';
		if (strstr(buffer,"filename=\"") != NULL) {
			isfile = 1;
			strcpy(entry.value,strstr(buffer,"filename=\"")+10);
			p=strchr(entry.value,'\"');
			if(p)
				*p='\0';
			/* Netscape's Windows browsers handle paths differently from its
			UNIX and Mac browsers.  It delivers a full path for the uploaded
			file (which it shouldn't do), and it uses backslashes rather than
			forward slashes.  No need to worry about Internet Explorer, since
			it doesn't support HTTP File Upload at all. */
			if (strstr(lower_case(HTTP_USER_AGENT),"win") != 0) {  
				p = strrchr(entry.value, '\\');
				if (p) {
					p++;
					strcpy(entry.value,p);
				}
			}
			window = list_insafter(entries,window,entry);
			if(flag==WRITE_FILE){
				uploadfname = (char *)malloc(strlen(pUploadDir)+strlen(entry.value)+3);
				if(!uploadfname){
					free(entry.name);
					free(entry.value);	
					goto err;
				}
				sprintf(uploadfname,"%s/%s",pUploadDir,entry.value);
				free(entry.name);
				free(entry.value);
				if ( (fd = open(uploadfname,O_WRONLY|O_CREAT,S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH)) == -1)
					goto err;
			}
			else{
				free(entry.name);
				free(entry.value);
			}
		}
		else{
			free(entry.name);
			free(entry.value);	
			goto err;
		}
		/* ignore rest of headers and first blank line */
		while (getline(buffer, BUFSIZ) > 1) {
			/* DOS style blank line? */
			if ((buffer[0] == '\r') && (buffer[1] == '\n'))
				break;
   	 	}
		done = 0;
		j = 0;

		while (!done) {
			bytesread = getline(buffer,BUFSIZ);
			buffer[bytesread] = '\0';
			if (bytesread && strstr(buffer,boundary) == NULL) {
				if(flag==WRITE_FILE){
					if (start) {
						memcpy(prevbuf,buffer,bytesread);
						prevbytesread = bytesread;
						start = 0;
					}
					else {
						write(fd,prevbuf,prevbytesread);
						memcpy(prevbuf,buffer,bytesread);
						prevbytesread = bytesread;
					}
				}
			}
			else {
				done = 1;
				if(flag==WRITE_FILE){
					if(prevbytesread>2)
						write(fd,prevbuf,prevbytesread-2);
				}
			}
		}
		if(flag==WRITE_FILE){
			close(fd);
			free(uploadfname);
		}
	} 
	if(prevbuf)	
		free(prevbuf);
	if(buffer)
		free(buffer);
	if(largefile)
		return 1;
	return 0;
err:
	if(prevbuf)	
		free(prevbuf);
	if(buffer)
		free(buffer);
	if(tempstr)	
		free(tempstr);
	return -1;	
}

char *cgi_val(llist l, char *name)
{
	short FOUND = 0;
	node* window;

	window = l.head;
	while ( (window != 0) && (!FOUND) )
		if (!strcmp(window->entry.name,name))
			FOUND = 1;
		else
			window = window->next;
	if (FOUND)
		return window->entry.value;
	else
    	return NULL;
}

