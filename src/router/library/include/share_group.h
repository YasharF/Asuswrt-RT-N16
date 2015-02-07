#ifndef __SHARE_GROUP_H__
#define __SHARE_GROUP_H__

int AddGroupToShares (char *pGroup, char **ppShare, int nNo, int nReadonly);
int AddGroupsToShare (char *pShare, char **ppGroup, int nNo, int nReadonly);
int DelGroupFromShares (char *pGroup, char **ppShare, int nNo);
int DelGroupsFromShare (char *pShare, char **ppGroup, int nNo);
int GetShareGroup (sh_group * sh);
int GetGroupShareInfo (grp_share * pShare);
int EveryoneInDiskSh(int hard_index);

#endif	
