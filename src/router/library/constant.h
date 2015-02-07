#ifndef _CONSTANT_H_
#define _CONSTANT_H_

#define	LINE_SIZE				256

#define	FSH_MaxUser				100
#define	FSH_MaxGroup			100
#define	FSH_MaxShare			100

#define	FSH_MaxUserLen			20
#define	FSH_MaxShareLen			12
#define	FSH_MaxGroupLen			20
#define	FSH_MaxCommentLen		48
#define	FSH_MaxDirLen			256
#define	FSH_MaxPassLen			15
#define	FSH_MaxIPLen			16
#define	FSH_MaxWorkGroupLen		16
#define	FSH_MaxZoneLen			32
#define	FSH_MaxPrinterLen		12
#define	FSH_MaxEnableLen		4			//"yes","no"
#define	FSH_DefaultCodePage		437
#define	FSH_ADMINISTRATOR_GID	500
#define	FSH_EVERYONE_GID		501

#define	FSH_ADMIN_UID			500
#define	FSH_GUEST_UID			501
#define	FSH_ADMINISTRATOR_UID	502

#define	FSH_ILLEGAL_ZONE_CHAR			"\\]/\">[<.:;,|=+?`'"
#define	FSH_ILLEGAL_WG_CHAR				"\"/\\[]:;|=.,+*?<>"
#define	FSH_ILLEGAL_PRINTER_CHAR		"/\\[]:;|=., +*?<>'`\""
#define	FSH_SPECIAL_CHAR				"\"$\\`"

#define	FSH_DISK_GROUP_INFO		"/etc/usrgrp.info"

#define FSH_MNTHD_CONF			"/share/hdd/conf"
#define FSH_MNTHD_SHARE_CONF		"/share/hdd/conf/share/share.info"
#define FSH_MNTHD_SMB_CONF		"/share/hdd/conf/share/smb.conf"
#define FSH_MNTHD			"/share/hdd/data"
#define FSH_MNTFD			"/share/flash/data"
#define FSH_MNTFD_CONF			"/share/flash/conf"
#define FSH_MNTFD_SHARE_CONF		"/share/flash/conf/share/share.info"
#define FSH_MNTFD_SMB_CONF		"/share/flash/conf/share/smb.info"
#define FSH_SMB_CONF			"/etc/samba/smb.conf"
#define FSH_USER_SMB_CONF			"/etc/samba/user_smb.conf"
#define FSH_SHARE_CONF			"/etc/share.info"
#define	FSH_WIN_SHARE_CONF			"/etc/win_share.info"
#define FSH_SHARE_CONF_NAME		"share.info"
#define	FSH_NETWORK_SEC			"network"
#define	FSH_SYS_PASSWD			"/etc/passwd"
#define	FSH_SMB_PASSWD			"/etc/samba/smbpasswd"
#define	FSH_WEB_PASSWD			"/home/httpd/html/Management/.htpasswd"
#define	FSH_SYS_GROUP			"/etc/group"
#define	FSH_DS_CONF_FILE		"/etc/CGI_ds.conf"
#define	FSH_DEF_DS_CONF_FILE		"/usr/local/CGI_ds.conf"
#define	FSH_QUOTA_TMPFILE		"/tmp/edquota.tmp"
#define	FSH_XINETD_FTP_CONF		"/usr/local/xinetd.d/ftp"
#define	FSH_PASSWD_SALT			"sc"
#define	FSH_EVERYONE_GRP		"everyone"
#define TMPFILE_MODE			"/share/hdd/conf/tmp/tmpfile.XXXXXX"
#define	TMPDIR				"/share/hdd/conf/tmp"

#define	FSH_FLASH_SHARE_DIR		"/share/flash/data"
#define	FSH_ADMIN_GRP			"administrators"
#define	FSH_ADMINISTRATOR_USER	"admin"
#define	FSH_GUEST_USER			"guest"
#define FSH_HDD_SHARE			"ADMIN"
#define FSH_PUBLIC_SHARE		"DISK"

#define FSH_FLASH_SHARE			"FLASH"
#define FSH_HARD_SHARE			"HDD"

#define	FSH_DEFAULT_PASS		"zGf8Ikl50e3aPM"

//Backup file flag
#define	fPass			1					//passwd
#define	fGrp			2					//group
#define	fShare			4					//share.info
#define	fUser			8					//usrgrp.info
#define	fSmb			0x10				//smb.conf
#define	fSmbPass		0x20				//smbpasswd
#define	fWebPass		0x40				//.htpasswd
#define	fGroups			(fGrp|fUser|fShare|fSmb)
#define	fUsers			(fPass|fGrp|fUser|fSmbPass)
#define	fShares			(fSmb|fShare)

#define	UID_Start       2000
#define	GID_Start       2000


#define	FSH_GET_GROUP_ID_ERR			-5			// Fail to get id of the group.
#define	FSH_DEL_KEY_ITEM_ERR			-4			// Fail to delete one item related to one key from one file.
#define	FSH_GET_SHARENUM_ERR			-3			// Fail to get the number of shares in server
#define	FSH_GET_GRPNUM_ERR				-2 			// Fail to get the number of groups in server
#define	FSH_GET_USERNUM_ERR				-1 			// Fail to get the number of users in server
#define	FSH_OK							0			//Succeed
//Samba Error
#define	FSH_DOMAIN_NAME_ERR				1			//Illegal character or characters in domain name.
#define	FSH_MSN_VALUE_ERR				2			//The value of MSN is invalid.
#define	FSH_WINS_VALUE_ERR				3			//The value of WINS is invalid.
#define	FSH_WINS_BLANK_ERR				4			//The WINS Server is enabled but  its IP Address  is blank.
#define	FSH_WINS_IP_ERR					5			//The WINS Server Address is invalid.
#define	FSH_WG_DBCS_ERR					6			//DBCS in workgroup name.
#define	FSH_ILLEGAL_WG_CHAR_ERR			7			//Illegal character or characters in workgroup name.
#define	FSH_WG_BLANK_ERR				8			//The MSN is enabled but  workgroup  is blank.
#define	FSH_ILLEGAL_PRINTER_CHAR_ERR	9 			//Illegal character or characters in printer name.
#define	FSH_PRINTER_DBCS_ERR			10			//DBCS in printer name.
#define	FSH_READ_SMB_CONF_ERR			11			//Fail to read samba configuration info
#define	FSH_SAVE_SMB_CONF_ERR			12			//Fail to save samba configuration info.
#define	FSH_RESET_SMB_ERR				13
//AppleTalk Error
#define	FSH_APPLE_VALUE_ERR				14			//The value of AppleTalk is invalid
#define	FSH_ZONE_BLANK_ERR				15			//The AppleTalk is enabled but  zone name  is blank.
#define	FSH_ZONE_DBCS_ERR				16			//DBCS in zone name.
#define	FSH_ILLEGAL_ZONE_CHAR_ERR		17			//Illegal character or characters in zone name.
#define	FSH_READ_APPLE_CONF_ERR			18			//Fail to read AppleTalk configuration info
#define	FSH_SAVE_APPLE_CONF_ERR			19			//Fail to save AppleTalk configuration info.
//Common Error
#define	FSH_INPUTPARM_ERR				20		// Inputed parameter is wrong.
#define	FSH_FILE_OPEN_ERR				21		// Fail to open file.
#define	FSH_CREATE_TMPFILE_ERR			22    		// Fail to create one temp file
#define	FSH_ALLOCATE_MEM_ERR			23 		// Fail to allocate memory.
#define	FSH_FILE_FORMAT_ERR				24 		// Format of file is wong.
#define	FSH_READ_FILE_ERR				25		// Fail to read content from file.
//User Error
#define	FSH_USER_DUP_ERR				26		// The user has been existing already.
#define	FSH_RESERVE_USER_ERR			27		// Cannot delete default users.
#define	FSH_DEL_USER_ERR				28		// Fail to delete the user.
#define	FSH_GET_ALLUSER_ERR				29		// Fail to get all users' info.
#define	FSH_MOD_USER_ERR				30		// Fail to update the user's info.
#define	FSH_ADD_USER_ERR				31		// Fail to add the user.
#define	FSH_SET_USER_QUOTA_ERR			32		// Fail to set the user's disk quota.
#define	FSH_GET_USERINFO_ERR			33		// Fail to get the user's info.
#define	FSH_ADD_GRP_MEMB_ERR			34		// Fail to add user  to group.
#define	FSH_DEL_GRP_MEMB_ERR			35		// Fail to delete user from group.
#define	FSH_RESET_FTP_ERR				36		// Fail to reset FTP Server.

//Group Error
#define FSH_ADD_GROUP_ERR				37		// Fail to add the group.
#define FSH_DEL_GROUP_ERR				38		// Fail to delete the group.
#define FSH_MOD_GROUP_ERR				39		// Fail to update the group.
#define FSH_DEL_GRP_FROM_SH_ERR			40		// Fail to delete group from share.
#define FSH_GRP_TOOMANYGRP_ERR			41		// Groups are too many.
#define FSH_GRP_RESERVE_ERR				42		// Cannot delete default groups.
#define FSH_GRP_EXIST_ERR				43		// The group has been existing.
//Share Error
#define FSH_DEL_SHARE_ERR				44		// Fail to delete the share.
#define FSH_WRITE_SMB_CONF_ERR			45		// Fail to write file smb.conf
#define FSH_GET_ALL_SHARE_ERR			46		// Fail to get all share info
#define FSH_GET_SHARE_GRP_ERR			47		// Fail to get the share's groups
#define	FSH_SHARE_DUP_ERR				48		// The share has been existing.
#define	FSH_SHARE_NOT_EXIST_ERR			49		// The share does not exist.
#define	FSH_ADD_SHARE_ERR				50		// The share does not exist.
#define FSH_DIRSHARED_ERR               56

#define 	FSH_ADD_GRP_SHARE_ERR		51
#define 	FSH_DEL_GRP_SHARE_ERR		52
#define		FSH_UPDATE_USER_ERR			53
#define 	FSH_USER_EXISTEDIN_GROUP_ERR	54
#define		FSH_GROUP_ACCESSED_SHARE_ERR	55

#define	FSH_RESET_GUEST_ERR				56
#define	FSH_RESET_PRINTER_ERR			57
#endif

