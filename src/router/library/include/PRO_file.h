#ifndef _PROFILE_H_
#define _PROFILE_H_

#include <stdio.h>

#ifndef OK
#define OK   0
#endif

typedef char Char128[128];

#define MAX_ITEM_LEN	24
extern FILE *ifp;

typedef	struct multi_items {
	char key[MAX_ITEM_LEN];
	char *value;
	int flag;
} multi_items;

/* single attribute access routines */
int PRO_GetInt(char *sect, char *key, int *val, FILE *fp);
int PRO_GetStr(char *sect, char *key, char *val, int size, FILE* fp);
int PRO_GetStrLen(char *sect, char *key, FILE* fp);
int PRO_GetLong(char *sect, char *key, long *val, FILE *fp);

int PRO_SetInt(char *sect, char *key, int val, char *fname);
int PRO_SetStr(char *sect, char *key, char *val, char *fname);
int PRO_SetStr1(char *sect, char *key, char *val, char *fname);

int PRO_DelSect(char *sect,char *fname);
int PRO_RenSect(char *old_sect, char *new_sect, char *fname);
int PRO_SetMultiItemStr(char *sect, multi_items *val,int itemnum, char *fname);
int PRO_DelKey(char *sect, char *key,char *fn);
int GetLineFromFile(char **lbuf,FILE *fp);
/* batch access routines */
/* returns a temp file pointer */
FILE *PRO_uopen(char *fn);

/* for ctrl of u_to_sect() */
#define PRO_NWSEC   0
#define PRO_WRSEC   1

/* If you need to change the section name, set ctrl to PRO_NWSEC */
/* PRO_WRSEC will keep the same section name. If ctrl is set to  */
/* PRO_WRSEC and the section does not exist, the section will be created.*/
int PRO_u2sect(char *sect, int ctrl);

/* for ctrl of u_close() */
#define PRO_WRIRST     1
#define PRO_GIVEUP     2
#define PRO_GOWRIRST   3

/* PRO_GIVEUP-give up the update. PRO_GOWRIRST-give up original */
/* section info and write the rest lines that are not read.     */
/* PRO_WRIRST-write the rest lines that are not read.           */
int PRO_uclose(char *fn, int mode, int ctrl);

int PRO_2sect(char *sect, FILE *fp);

/* message routines */
int PRO_GetMsgByIndex(char *sect, int start, int num, Char128 *msg, char *fn);

void DBCS_CAP2SJIS(char *str,int bs2); // transfer 3 bytes -> 2 bytes
void DBCS_SJIS2CAP(char *str,int bs2); // transfer 2 bytes -> 3 bytes
int  DBCS_CheckFileName(char *str);
int  DBCS_AnyDBCSInStr(char *str);
void DBCS_SpecialChar(char *old, char *new,char *str);

void code_transfer(unsigned char *input,int flag);
#endif
