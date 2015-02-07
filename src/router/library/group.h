#ifndef __GROUP_H__
#define __GROUP_H__

#include "user.h"

typedef struct grp_list{
	char	name[FSH_MaxGroupLen+1];		// name of group
	int		nAccess;					// access right : 5=read only 7=r/w
	struct	grp_list *pNext;			// pNext group
}grp_list;

typedef struct grp_share{
	char	name[FSH_MaxGroupLen+1];		// name of  group
	int		nShares;					// number of shares the group can access
	char	**ppShare;					// shares the user can access
	int		nAccess[FSH_MaxShare];		// access right to the shares of the user
}grp_share;

typedef struct group_list{
	char		name[FSH_MaxGroupLen+1];	// name of  group
	int			nGid;					// id of the group;
	grp_user	*pUser;					// user info of the group
	grp_share	*pShare;				// share info of the group
	struct group_list *pNext;
}group_list;

typedef struct sh_group{
	char		name[FSH_MaxShareLen+1];	// name of share
	grp_list	*pGroup;				// groups that can access the share
}sh_group;

int AddGroup (char *name);
int DelGroup (char *name);
int DelAllGroup (void);
int RenameGroup (char *pOld, char *pNew);
int GroupExist (char *name);
int GetNumOfGrps(void);
int GetGrpNameList(grp_list **ppList);
int GetAllGroup (group_list **ppList);
void FreeGrpList (group_list *pList);
void FreeGrpNameList(grp_list *pList);
void FreeGrpShareInfo(grp_share * pSh);
void FreeShareGrpInfo(sh_group * pList);
#endif
