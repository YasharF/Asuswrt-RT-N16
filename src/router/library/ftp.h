#ifndef _FTP_H_
#define _FTP_H_

int ResetFTP(void);
int ResetBroadcast(void);
int ResetUPnPAV(void);
int ResetItune(void);
int StopMediaProcess(int hard_index, char *sh_prefix, int disable);
int StartMediaProcess(int num);
#endif
