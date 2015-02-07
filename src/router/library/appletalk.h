#ifndef _APPLETALK_H_
#define _APPLETALK_H_

#ifdef _APPLE_

#include "constant.h"
typedef struct sys_apple{
	char	apple_enable[FSH_MaxEnableLen];       	 //"no" , " yes"
	char	zone_name[FSH_MaxZoneLen];
}sys_apple;

int ReadAppleConf(sys_apple *conf);
int SaveAppleConf(sys_apple *conf);
int SetApple(void);

#endif

#endif
