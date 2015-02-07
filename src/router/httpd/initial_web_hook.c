#include <stdio.h>
#include "initial_web_hook.h"

extern char *initial_foreign_share_info(){
char *desc = "function foreign_shares()\n"
"{\n"
"    return [];\n"
"}\n"
"\n"
"function foreign_share_disks()\n"
"{\n"
"    return [];\n"
"}\n"
"\n"
"function foreign_share_partitions()\n"
"{\n"
"    return [];\n"
"}\n"
"\n"
"function foreign_share_file_system()\n"
"{\n"
"    return [];\n"
"}\n"
"\n"
"function foreign_share_kilobytes()\n"
"{\n"
"    return [];\n"
"}\n"
"\n"
"function foreign_share_kilobytes_in_use()\n"
"{\n"
"    return [];\n"
"}\n"
"\n"
"function foreign_share_cifs_enables()\n"
"{\n"
"    return [];\n"
"}\n"
"\n"
"function foreign_share_cifs_write_enables()\n"
"{\n"
"    return [];\n"
"}\n"
"\n"
"function foreign_share_nfs_enables()\n"
"{\n"
"    return [];\n"
"}\n"
"\n"
"function foreign_share_nfs_write_enables()\n"
"{\n"
"    return [];\n"
"}\n"
"function foreign_share_ftp_enables()\n"
"{\n"
"    return [];\n"
"}\n"
"function foreign_share_ftp_create_enables()\n"
"{\n"
"    return [];\n"
"}\n"
"\n"
"function foreign_share_ftp_delete_enables()\n"
"{\n"
"    return [];\n"
"}\n"
"function foreign_share_http_enables()\n"
"{\n"
"    return [];\n"
"}\n"
"function foreign_share_http_create_enables()\n"
"{\n"
"    return [];\n"
"}\n"
"\n"
"function foreign_share_http_delete_enables()\n"
"{\n"
"    return [];\n"
"}\n";

	return desc;
}

extern char *initial_query_users(){
	char *desc = "function per_user_foreign_permissions(){\n"
				 "    return [];\n"
				 "}\n";

	return desc;
}

extern char *initial_disk_pool_mapping_info(){
	char *desc = "function total_disk_sizes() { return [];}\n\
function disk_interface_names() { return []; }\n\
function pool_names() { return [];}\n\
function pool_types() { return [];}\n\
function pool_mirror_counts() { return [];}\n\
function pool_status() { return [];}\n\
function pool_kilobytes() { return [];}\n\
function pool_encryption_password_is_missing() { return [];}\n\
function pool_kilobytes_in_use() { return [];}\n\
function pool_usage_kilobytes() { return [];}\n\
function disk_usage() { return [];}\n\
function per_pane_pool_usage_kilobytes(pool_num, disk_num) { return [];}\n";

	return desc;
}

extern char *initial_available_disk_names_and_sizes(){
	char *desc = "function foreign_disks() { return [];}\n\
function foreign_disk_interface_names() { return [];}\n\
function foreign_disk_model_info() { return [];}\n\
function foreign_disk_total_size() { return [];}\n\
function foreign_disk_total_mounted_number() { return [];}\n";

	return desc;
}
