#ifndef _DISK_SHARE_
#define _DISK_SHARE_

#define MAX_ACCOUNT_NUM 6
#define DEFAULT_SAMBA_RIGHT 3
#define DEFAULT_FTP_RIGHT 3
#define DEFAULT_DMS_RIGHT 1

#define BASE_LAYER 2
#define MOUNT_LAYER BASE_LAYER+1
#define SHARE_LAYER MOUNT_LAYER+1

extern int get_account_list(int *, char ***);
extern int get_folder_list_in_mount_path(const char *const, int *, char ***);
extern int get_all_folder_in_mount_path(const char *const, int *, char ***);
extern void free_2_dimension_list(int *, char ***);
extern int initial_folder_list_in_mount_path(const char *const);
extern int initial_one_var_file_in_mount_path(const char *const, const char *const);
extern int initial_all_var_file_in_mount_path(const char *const);
extern int test_of_var_files_in_mount_path(const char *const);

extern int create_if_no_var_files(const char *const);
extern int modify_if_exist_new_folder(const char *const, const char *const);

extern int get_permission(const char *const, const char *const, const char *const, const char *const);
extern int set_permission(const char *const, const char *const, const char *const, const char *const, const int);
extern int add_account(const char *const, const char *const);
extern int del_account(const char *const);
extern int mod_account(const char *const, const char *const, const char *const);
extern int add_folder(const char *const, const char *const);
extern int del_folder(const char *const, const char *const);
extern int mod_folder(const char *const, const char *const, const char *const);

extern int test_if_exist_account(const char *const);
extern int test_if_exist_folder_in_mount_path(const char *const, const char *const);

extern int how_many_layer(const char *const, char **, char **);

#endif // _DISK_SHARE_
