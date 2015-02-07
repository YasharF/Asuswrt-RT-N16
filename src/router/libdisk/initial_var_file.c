#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <bcmnvram.h>
#include "disk_io_tools.h"
#include "disk_share.h"

int main(int argc, char *argv[]){
	char *mount_path;
	
	if(argc != 2){
		fprintf(stderr, "usage: %s MOUNT_DIR\n", argv[0]);
		return -1;
	}
	
	// 1. get the mount_path of the mount_path
	if(get_mount_path(argv[1], &mount_path) == -1){
		fprintf(stderr, "Can't get the mount_path of %s.\n", argv[1]);
		return -1;
	}
	
	initial_all_var_file_in_mount_path(mount_path);
	
	free(mount_path);
	
	return 0;
}
