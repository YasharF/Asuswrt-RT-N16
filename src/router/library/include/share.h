#ifndef __SHARE_H__
#define __SHARE_H__

#include "constant.h"
#include "group.h"

typedef struct sh_info{
	char	name[FSH_MaxShareLen+1];		// name of share
	char	comment[FSH_MaxCommentLen+1];	// comment of share
	unsigned char dir[FSH_MaxDirLen+1];	// folder of share
	int		hard_index;
	int 	conflict;					// 0 - no conflict, 1- conflict 
}sh_info;

typedef struct sh_list{
	sh_info		sh;
	grp_list	*pGroup;			// groups that can access the share
#ifdef SHARE_USER
	u_list	*pUser;			// users that can access the share
#endif
	struct 		sh_list *pNext;		// pNext share
}sh_list;

typedef struct shname_list{
    char		name[FSH_MaxShareLen+1];		// name of share
    struct		shname_list *pNext;	    
}shname_list;
// share routins
int AddShare (sh_info * sh);
int DelShare (char *name);
int DelAllShare (int deldir);
int ShareExist (char *name);
int UpdateShare (char *name, sh_info * sh);
int GetAllShare (sh_list **ppList);
void FreeShareList (sh_list * pList);
void FreeShGrpList (grp_list * gl);
int GetShareInfo (sh_info * pSh);
int AddShareWithGroup (sh_list * pSh);
int GetShareWithGroup (sh_list * pList);
int GetShareNameList(shname_list **ppList);
void FreeShareNameList(shname_list *pList);
int GetNumOfShares(void);
char * GetPublicSh(int hard_index);
int AddWinShares(char *sh_name, int dev_no);
int DelWinShares(char *share_name, int dev_no, int mounts);
#endif
