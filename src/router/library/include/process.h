#ifndef _PROCESS_H_
#define _PROCESS_H_

void StripEndingSpace(char *buf);
int ItemExist (char *pItem, char *pFile);
int DelKeyFromConf (char *key, char *fname);
int GetLineFromFile(char **lbuf,FILE *fp);
int GotoUserName(char *buf,int size,FILE *fh);
int GetUserName(char *buf,int size,int *lastuser,FILE *fh);
int DelKeyItem(char *name,char *fname,char tag);
void BackupConFiles(int flag);
void RestoreConFiles(int flag);
void logerr(char *pMsg);
int readvkey(char *buf,int size,FILE *fh);
int ReadGroup(char *buf,int len,FILE *fh,int *eol);
void UpdateDiskUserGroupInfo();
int FSH_GetStr(char *sect, char *key, char *val, int size, FILE* fp);
int RenameKey(char *strOld,char *strNew,char *fname);
int RenameKey1(char *strOld,char *strNew,char *fname);
int RenameSect(char *strOld,char *strNew,char *fname);

void Unlock(int flag);
int Lock(int flag);
#endif
