#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "shutils.h"

int waitsock(int fd, int sec, int usec){
	struct timeval tv;
	fd_set fdvar;
	int res;
	
	FD_ZERO(&fdvar);
	FD_SET(fd, &fdvar);
	tv.tv_sec = sec;
	tv.tv_usec = usec;
	res = select(fd+1, &fdvar, NULL, NULL, &tv);
	
	return res;
}

char *base64enc(const char *p, char *buf, int len){
	char al[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	char *s = buf;
	
	while(*p){
		if(s >= buf+len-4)
			break;
		
		*(s++) = al[(*p>>2)&0x3F];
		*(s++) = al[((*p<<4)&0x30)|((*(p+1)>>4)&0x0F)];
		*s = *(s+1) = '=';
		*(s+2) = 0;
		if(!*(++p))
			break;
		*(s++) = al[((*p<<2)&0x3C)|((*(p+1)>>6)&0x03)];
		if(!*(++p))
			break;
		*(s++) = al[*(p++)&0x3F];
	}
	
	return buf;
}

int wgetToFile(const char *const server, const char *const filename){
	char url[PATH_MAX], *s;
	char *host = url, *path = "", auth[128], line[PATH_MAX];
	unsigned short port = 80;
	int fd, len = 0;
	FILE *np, *fp;
	struct sockaddr_in sin;
	struct hostent *hostaddr;
	unsigned long ul_hostip;
	int i;
	
	memset(url, 0, PATH_MAX);
	strncpy(url, server, PATH_MAX);
	
	// Parse URL
	if(!strncmp(url, "http://", 7)){
		port = 80;
		host = url+7;
	}
	if((s = strchr(host, '/'))){
		*s++ = '\0';
		path = s;
	}
	if((s = strchr(host, '@'))){
		*s++ = '\0';
		base64enc(host, auth, sizeof(auth));
		host = s;
	}
	if((s = strchr(host, ':'))){
		*s++ = '\0';
		port = atoi(s);
	}
	printf("host: %s!\n", host);
	// Open socket
	if((ul_hostip = inet_addr(host)) != INADDR_NONE){
		//printf("1.\n");
		sin.sin_addr.s_addr = ul_hostip;
	}
	else if((hostaddr = gethostbyname(host)) != NULL){
		//printf("2.\n");
		memcpy(&sin.sin_addr, hostaddr->h_addr, sizeof(sin.sin_addr));
	}
	else{
		printf("Failed to get host, %s!\n", host);
		return -1;
	}
	
	sin.sin_family = AF_INET;
	sin.sin_port = htons(port);
	
	printf("\nConnecting to %s:%u...\n", host, port);
	if((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0
			|| connect(fd, (struct sockaddr *)&sin, sizeof(sin)) < 0
			|| !(np = fdopen(fd, "r"))
			){
		perror(host);
		if(fd >= 0)
			close(fd);
		return -1;
	}
	printf("connected!\n\n");
	
	// Send HTTP request
	memset(line, 0, PATH_MAX);
	sprintf(line, "GET /%s HTTP/1.0\r\n", path);
	sprintf(line, "%sUser-Agent: wget\r\n", line);
	sprintf(line, "%sHost: %s\r\n", line, host);
	sprintf(line, "%sAccept: */*\r\n", line);
	if(strlen(auth) > 0)
		sprintf(line, "%sAuthorization: Basic %s\r\n", line, auth);
	sprintf(line, "%sConnection: close\r\n\r\n", line);
	//sprintf(line, "%sConnection: Keep-Alive\r\n\r\n", line);
	
	// Check HTTP response
	printf("wrote the request:\n");
	printf("******************************\n");
	printf("%s", line);
	printf("******************************\n");//*/
	write(fd, line, strlen(line));
	
	//printf("Got header:\n");
	//printf("******************************\n");
	if(waitsock(fd, 2, 0) && fgets(line, sizeof(line), np)){
		//printf("%s", line);
		for(s = line; *s && !isspace((int)*s); s++)
			;
		
		for(; isspace((int)*s); s++)
			;
		
		switch(atoi(s)){
			case 200:
				break;
			case 206:
				printf("Failed to get the page!\n");
				goto done;
			default:
				printf("Exceptionable error!\n");
				goto done;
		}
	}
	else{
		printf("Failed to get the page!\n");
		close(fd);
		return -1;
	}
	
	while(fgets(line, sizeof(line), np))
		if(strlen(line) == 2)
			break;
	//printf("******************************\n\n");
	if(line == NULL || strlen(line) <= 0){
		printf("Failed to get the page!\n");
		goto done;
	}
	
	printf("Got content:\n");
	//printf("******************************\n");
	if((fp = fopen(filename, "w+")) != NULL){
		//printf("Wrote the file, %s...\n", filename);
		i = 0;
		while(fgets(line, sizeof(line), np)){
			//printf("%s", line);
			//printf("wrote %d rows: ", ++i);
			fputs(line, fp);
			//printf("ok!\n");
		}
		//printf("Finished!\n");//*/
		fclose(fp);
	}
	//printf("******************************\n\n");
	printf("Ok!\n");
	
done:
	// Close socket
	fflush(np);
	fclose(np);
	return len;
}

int main(int argc, char *argv[]){
	if(argc != 3){
		fprintf(stderr, "usage: wget [url] [outpur file]\n");
		return -1;
	}
	
	wgetToFile(argv[1], argv[2]);
	
	return 0;
}
