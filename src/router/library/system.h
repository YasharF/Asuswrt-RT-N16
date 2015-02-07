#ifndef _SYSTEM_H_
#define _SYSTEM_H_

#define 	SERVERNAME_LEN     14
#define 	SERVERCOMMENT_LEN  48
#define 	VERSION_LEN	   12
#define 	HARDWARE_ID_LEN    32
#define 	HARDWARE_ADD_LEN   24
#define 	IPADDRESS	   15
#define 	SYS_TIME_LEN	   5	// 00:00 length
#define 	AMPM_LEN	   2	// am/pm length

#define SYS_OK                      0
#define OPENFILEERR          	1        //Fail to open file
#define PROFILEGETERROR       	2        //Fail to get value from config ile
#define PROFILESETERROR        	3        //Fail to set value to config file
#define COMMANDRUNERR         	4        //Fail to run command
#define CHECKENTRYERROR      	5        //Input data error
#define WRITECRONTABERR       	6        //Fail to write /etc/crontab

typedef struct sys_time{
   	int t_timezone;
   	int sys_month;
   	int sys_day;
   	int sys_year;
   	int sys_hour;
   	int sys_min;
   	char sys_ampm[AMPM_LEN+1];
} sys_time;		//system time structure

typedef struct sys_info{
	char server_name[SERVERNAME_LEN+1];
	char server_comment[SERVERCOMMENT_LEN+1];
	char server_default_name[SERVERNAME_LEN+1];
	char ds_sw_version[VERSION_LEN+1];
	char ds_hardware_id[HARDWARE_ID_LEN+1];
	char ds_hardware_address[HARDWARE_ADD_LEN+1];
	char ds_ipaddress[IPADDRESS+1];
}sys_info;

typedef struct log_ser_info{
	char log_ser_en[5];
	char log_ser_ip[IPADDRESS+1];
}log_ser_info;

typedef struct sys_schedule{
	int weekday_action;
	char weekday_time[SYS_TIME_LEN+1];
	int satday_action;
	char satday_time[SYS_TIME_LEN+1];
	int sunday_action;
	char sunday_time[SYS_TIME_LEN+1];
}sys_schedule;

typedef struct shutdown_schl{
	int action;			// shutdown/reboot
	int date;			//0-None, 1-Monday, 2-Tuesday, 3-Wednesday, 4-Thursday, 5-Friday,
					//6-Saturday, 7-Sunday, 8-weekdays
	int hour;			//24 hour
	int minute;
}shutdown_schl;

typedef struct sys_setup{
   	int delay_action;	//2,1,0  shutdown/reboot/none
   	int delay;		//delay minutes: Type, integer
}sys_setup;		//system shutdown(reboot) structure


/**************************************************/
//	NOTE:This function will read system time  //
//	information from config files.		  //
//	input:	NONE				  //
//	output:	p	point to the structure	  //
//	return:	0	success			  //
//		1	failure			  //
/**************************************************/
int ReadSYSTime(sys_time *p);

/**************************************************/
//	NOTE:This function will read system   	  //
//	general information from config files.	  //
//	input:	NONE				  //
//	output:	p	point to the structure	  //
//	return:	0	success			  //
//		1	failure			  //
/**************************************************/
int ReadSYSInfo(sys_info *p);

/**************************************************/
//	NOTE:This function will read system shut  //
//	schedule information from config files.	  //
//	input:	NONE				  //
//	output:	p	point to the structure	  //
//	return:	0	success			  //
//		1	failure			  //
/**************************************************/
int ReadSYSSchl(sys_schedule *p);

/**************************************************/
//	NOTE:This function will read system  	  //
//	setup information from config files.	  //
//	input:	NONE				  //
//	output:	p	point to the structure	  //
//	return:	0	success			  //
//		1	failure			  //
/**************************************************/
int ReadSYSShut(sys_setup *p);

/**************************************************/
//	NOTE:This function will save system  	  //
//	time information to config files.	  //
//	input:	p	system time information	  //
//			saved here.		  //
//		flag	save type:1-timezone	  //
//				  2-time	  //
//				  0-both	  //
//	output:	NONE				  //
//	return:	0	success			  //
//		1	failure			  //
/**************************************************/
int SaveSYSTime(sys_time *p,int flag);

/**************************************************/
//	NOTE:This function will save system shut  //
//	schedule information to config files.	  //
//	input:	p	system time information	  //
//			saved here.		  //
//	output:	NONE				  //
//	return:	0	success			  //
//		1	failure			  //
/**************************************************/
int SaveSYSSchl(sys_schedule *p);

/**************************************************/
//	NOTE:This function will save system  	  //
//	setup information to config files.	  //
//	input:	p	system setup information  //
//			saved here.		  //
//	output:	NONE				  //
//	return:	0	success			  //
//		1	failure			  //
/**************************************************/
int SaveSYSShut(sys_setup *p);

/**************************************************/
//	NOTE:This function will do reboot or  	  //
//	shutdown action after some time according //
//	to config files.			  //
//	input:	NONE				  //
//	output:	NONE				  //
//	return:	0	success			  //
//		1	failure			  //
/**************************************************/
int DelayAction(void);

/**************************************************/
//	NOTE:This function will check the server  //
//  	name and comment, Save them if they are   //
//	changed. 				  //
//	input:server_name     point to the name   //
//	      server_comment  point to the comment//
//	output:	NONE				  //
//	return:	0	success			  //
//		1	failure			  //
/**************************************************/
int SaveServerNameComment(char *server_name, char *server_comment);

int Read_Schl_Shutdown(shutdown_schl *shutdown_schl);
int Add_Schl_Shutdown(shutdown_schl *shutdown_schl, int action);
int saveTZ(sys_time *p);

int ReadLogSerInfo(log_ser_info *p);
int SaveLogSerInfo(log_ser_info *p);

#define REMOTE_INPUTPARM_ERR		7
#define REMOTE_VALUE_ERR			8
#define	REMOTE_IPAddrBlankErr		9			//the IP Address is blank.
#define	REMOTE_IPAddrErr			10			//The IP Address is invalid.
#endif
