#ifndef _USER_GROUP_H_
#define _USER_GROUP_H_

#include "user.h"

int AddUsersToGroup (char *pGroup,char **ppUser,int no);
int AddUserToGroups(char *pUser,char **ppGroup,int no);
int DelUsersFromGroup(char *pGroup,char **ppUser,int no);
int DelUserFromGroups(char *pUser,char **ppGroup,int no);
int GetGroupUserInfo(grp_user *pUser);
int GetUserGroupInfo(u_group *pGroup);
void FreeUserGroupInfo(u_group *pGroup);
void FreeGroupUserInfo(grp_user *pUser);
int UserInGroup(char *pUser,char *pGrp);

#endif
