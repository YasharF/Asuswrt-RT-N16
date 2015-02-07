#ifndef _USER_H_
#define _USER_H_
#include "constant.h"

typedef struct u_list{
	char	name[FSH_MaxUserLen+1];				// name of user
#ifdef SHARE_USER
	int	nAccess;			// users that can access the share
#endif
	struct	u_list *pNext;
}u_list;

typedef struct user_info{
	char	name[FSH_MaxUserLen+1];				// name of  user
	char	comment[FSH_MaxCommentLen+1];			// comment of user
	char	password[FSH_MaxPassLen+1];			// password of user
	long	use_disk;							// limit disk usage (limit disk usage (MB), 0=no limit)
	int		nUid;								// id of the user
}u_info;

typedef struct u_group{
	char	name[FSH_MaxUserLen+1];		// name of  user
	int		nGroups;					// number of groups the user belongs to
	char	**ppGroup;					// groups the user belongs to
}u_group;

typedef struct user_list{
	u_info	u_info;						// user info (name, comment, quota, passwd)
	u_group	*pGroup;					// group info of the user
	struct	user_list *pNext;			// info of next user
}user_list;

typedef struct grp_member{
	char	name[FSH_MaxGroupLen+1];		// name of  group
	int		nUsers;						// number of users the group includes
	char	**ppUser;					// users the group includes
}grp_user;

#ifdef _QUOTA_
int SetUserQuota(char *user,long use_disk);
#endif
int AddUser(u_info *pUser);
int DelUser(char *user);
int DelAllUser(void);
int UpdateUser(char *pName,u_info *pUser);
int GetUserNameList(u_list **ppList);
void FreeUserNameList(u_list *pList);
void FreeUserList(user_list *pList);
int UserExist(char *name);
int GetNumOfUsers(void);
int GetAllUser(user_list **ppList);
int GetUserInfo(u_info *pInfo);
int ResetGuestPass(void);
#endif
