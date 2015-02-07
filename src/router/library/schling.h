#ifndef __SCHLING_H__
#define __SCHLING_H__

#define SCHL_SCANDISK		0	// Scan disk
#define SCHL_SHUTDOWN		1	// Shutdown
#define SCHL_RESTART		2	// Reboot
#define	SCHL_SH_BACKUP		3	//share backup function
#define SCHL_D2D_SYNC		4	// Driver to Driver Sych

//shutdown/reboot parameter
#define REBOOT		"reboot"
#define SHUTDOWN	"shutdown"

#define SCHL_MON		1
#define SCHL_TUE		2
#define SCHL_WED		3
#define SCHL_THU		4
#define SCHL_FRI		5
#define SCHL_SAT		6
#define SCHL_SUN		7
#define SCHL_WEEKDAYS		8
#define SCHL_EACHDAY		9

//schedul type
#define SCHL_TYP_WEEKLY		0
#define SCHL_TYP_MONTHLY		1
 
typedef struct Schl_Act {
	int task;	// schedule task type
	char *tsk_para;	// task command parameter, if no then null.  	
	int hour;	// the hours of the schedule time,   01~24
	int min;		// the minutes of schedule time,     00~59
	int date;		// the week date/month date of the task schedule
} Schl_Act;

typedef struct DelayAct {
	int task;		// schedule task type  	0 - scandisk
				//			1 - system shutdown or reboot
				// 			2 - smart
				// 			3 - sync
	char *tsk_para;	// task command parameter
	int time;		// delay time, in minute
}DelayAct;

int Schl_Add(Schl_Act * p, int schltype);
int Schl_Del(int type);
int Delay_Act(DelayAct* dact);
			
int Schl_DelKey(char *key);			
			
int Schl_Start (void);
int Schl_Stop  (void );
int Schl_Restart(void);
			
#define SCHL_OK 					0
#define SCHL_ERR_INPUT				-1
#define SCHL_ERR_DUPLCATE_TASK 		-2
#define SCHL_ERR_READ_FILE			-3
#endif
