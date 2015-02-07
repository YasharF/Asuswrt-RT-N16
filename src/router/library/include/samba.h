#ifndef _SAMBA_H_
#define _SAMBA_H_
#include "constant.h"

#ifndef	NO_PRINTER
	#define NO_PRINTER
#endif
typedef struct sys_samba{
	char		msn_enable[FSH_MaxEnableLen+1];		//"no" , " yes"
	int			code_page;
	char		wins_enable[FSH_MaxEnableLen+1]; 		//"no" , " yes"
	char		wins_server[FSH_MaxIPLen+1];
	char		w_d_name[FSH_MaxWorkGroupLen+1];

#ifndef	NO_PRINTER
	char		printer_name[FSH_MaxPrinterLen+1];
#endif
}sys_samba;

int ReadSambaConf(sys_samba *conf);
int SaveSambaConf(sys_samba *conf);
int SetSMB(void);
int ResetGuest();
#endif
