#include <stdio.h>
#include "disk_io_tools.h"
#include "disk_share.h"

int main(int argc, char *argv[]){
	if(argc != 2){
		fprintf(stderr, "usage: %s MOUNT_PATH\n", argv[0]);
		return -1;
	}
	
	create_if_no_var_files(argv[1]);
	
	return 0;
}
