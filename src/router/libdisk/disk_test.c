#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/vfs.h>
#include <shutils.h>	// for eval()
#include <bcmnvram.h>//*/

#include "disk_io_tools.h"
#include "disk_initial.h"

/*#define LONG_MAX    ((long)(~0UL>>1))
#define LONG_MIN    (-LONG_MAX - 1)

#define PARTITION_FILE "/proc/partitions"
#define USB_INFO_DIR "/proc/scsi/usb-storage"
#define IDE_INFO_DIR "/proc/ide/ide1"
#define SCSI_FILE "/proc/scsi/scsi"
#define MOUNT_FILE "/proc/mounts"

#define DEFAULT_SATE_TAG "SATA disk"
#define DEFAULT_E_SATE_TAG "e-SATA disk"
#define DEFAULT_FRONT_USB_TAG "Front USB"
#define DEFAULT_BACK_TOP_USB_TAG "Back side top USB"
#define DEFAULT_BACK_FOOT_USB_TAG "Back side foot USB"

#define PARTITION_TABLE_ENTRIES 63
#define PROC_PARTITIONS_HEADER "major minor  #blocks  name"

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;

typedef struct disk_info_t disk_info_t;
typedef struct partition_info_t partition_info_t;

struct disk_info_t{
	char *tag;
	char *vendor;
	char *model;
	char *device;
	u32 mounted_number;	// 2007.12 James. 0: unmounted, N: mounted number.
	u32 device_order;
	u64 size_in_kilobytes;
	partition_info_t *partitions;
	disk_info_t *next;
};

struct partition_info_t{
	char *device;
	disk_info_t *disk;
	u32 partition_number;
	char *mount_point;
	char *file_system;
	u64 size_in_kilobytes;
	u64 used_kilobytes;
	partition_info_t *next;
};

extern char *read_whole_file(const char *);
extern void strntrim(char *, size_t);

extern disk_info_t *read_disk_data();
extern void free_disk_data(disk_info_t **);

extern int is_partition_name(const char *, u32 *);
extern partition_info_t *create_record_for_existing_partition(disk_info_t *, char *, u64, u32, u32);
extern char *find_disk_usb_port(u32 *);
extern char *find_mount_point(const char *, const char *const);
extern char *find_file_system(const char *, const char *const);
extern int check_disk_free(const char *, u64 *, u64 *);

#define csprintf(fmt, args...) do{\
	FILE *cp = fopen("/dev/console", "w");\
	if(cp) {\
		fprintf(cp, fmt, ## args);\
		fclose(cp);\
	}\
}while(0)

extern char *read_whole_file(const char *target){
	FILE *fp = fopen(target, "r");
	char *buffer, *new_str;
	int i;
	unsigned int read_bytes = 0;
	unsigned int each_size = 1024;
	
	if(fp == NULL)
		return NULL;
	
	buffer = (char *)malloc(sizeof(char)*each_size+read_bytes);
	if(buffer == NULL){
		csprintf("No memory!!(buffer)\n");
		fclose(fp);
		return NULL;
	}
	memset(buffer, 0, sizeof(char)*each_size+read_bytes);
	
	while((i = fread(buffer+read_bytes, sizeof(char), each_size, fp)) == each_size){
		read_bytes += each_size;
		new_str = (char *)malloc(sizeof(char)*each_size+read_bytes);
		if(new_str == NULL){
			csprintf("No memory!!(new_str)\n");
			free(buffer);
			fclose(fp);
			return NULL;
		}
		memset(new_str, 0, sizeof(char)*each_size+read_bytes);
		memcpy(new_str, buffer, read_bytes);
		
		free(buffer);
		buffer = new_str;
	}
	
	fclose(fp);
	return buffer;
}

extern void strntrim(char *str, size_t len){
	register char *start = str;
	register char *end = start+len-1;

	while(start <  end && isspace(*start))
		++start;
	while(start <= end && isspace(*end))
		--end;

	end++;

	if((int)(end - start) < len){
		memcpy(str, start, (end-start));
		str[end-start] = 0;
	}

	return;
}

extern disk_info_t *read_disk_data(){
	disk_info_t *disk_info_list = NULL, *new_disk_info, **follow_disk_info_list;
	FILE *fp = fopen(PARTITION_FILE, "r");
	char line[32], device_path[64], device_name[64];
	u32 major, minor, device_order, partition_number;
	u64 size_in_kilobytes;
	
	if(fp == NULL){
		csprintf("Failed to open \"%s\"!!\n", PARTITION_FILE);
		return disk_info_list;
	}
	
	memset(line, 0, 32);
	if(fgets(line, 32, fp) == NULL){
		csprintf("Failed to read \"%s\"!!\n", PARTITION_FILE);
		fclose(fp);
		return disk_info_list;
	}

	if(strncmp(line, PROC_PARTITIONS_HEADER, strlen(PROC_PARTITIONS_HEADER))){
		csprintf("\"%s\" is incorrect: invalid header.\n", PARTITION_FILE);
		fclose(fp);
		return disk_info_list;
	}
	fgets(line, 32, fp);
	
	memset(device_name, 0, 64);
	while(fgets(line, 32, fp) != NULL){
		if(sscanf(line, " %u %u %llu %[^\n ]", &major, &minor, &size_in_kilobytes, device_name) != 4)
			continue;
		if(major != 22 && major != 8)	// Harddisk's major = 22, USBdisk's major = 8.
			continue;
		
		if(!isdigit(device_name[strlen(device_name)-1])){	// Disk
			// Found a disk device.
			memset(device_path, 0, 64);
			sprintf(device_path, "/dev/%s", device_name);
			
			// start get disk information
			new_disk_info = (disk_info_t *)malloc(sizeof(disk_info_t));
			if(new_disk_info == NULL){
				csprintf("No memory!!(new_disk_info)\n");
				fclose(fp);
				return disk_info_list;
			}
			
			int device_path_length = strlen(device_path);
			new_disk_info->device = (char *)malloc(sizeof(char)*device_path_length+1);
			if(new_disk_info->device == NULL){
				csprintf("No memory!!(new_disk_info->device)\n");
				free(new_disk_info);
				fclose(fp);
				return disk_info_list;
			}
			strcpy(new_disk_info->device, device_path);
			new_disk_info->device[device_path_length] = 0;
			
			FILE *info_fp;
			char *ide_model_file, ide_model[64];
			int len;
			
			device_order = 0;
			if(!strcmp(device_name, "hdd")){
				len = strlen(IDE_INFO_DIR)+strlen("//model")+strlen(device_name);
				ide_model_file = (char *)malloc(sizeof(char)*len+1);
				if(ide_model_file == NULL){
					csprintf("No memory!!(ide_model_file)\n");
					free(new_disk_info);
					fclose(fp);
					return disk_info_list;
				}
				sprintf(ide_model_file, "%s/%s/model", IDE_INFO_DIR, device_name);
				ide_model_file[len] = 0;
				
				info_fp = fopen(ide_model_file, "r");
				memset(ide_model, 0, 64);
				if(info_fp != NULL && fgets(ide_model, 64, info_fp) != NULL){
					len = strlen(ide_model)-1;	// del the char "\n".
					new_disk_info->tag = (char *)malloc(sizeof(char)*len+1);
					if(new_disk_info->tag == NULL){
						csprintf("No memory!!(new_disk_info->tag)\n");
						free(new_disk_info);
						fclose(fp);
						return disk_info_list;
					}
					strncpy(new_disk_info->tag, ide_model, len);
					new_disk_info->tag[len] = 0;
					
					new_disk_info->model = (char *)malloc(sizeof(char)*len+1);
					if(new_disk_info->model == NULL){
						csprintf("No memory!!(new_disk_info->model)\n");
						free(new_disk_info);
						fclose(fp);
						return disk_info_list;
					}
					strncpy(new_disk_info->model, ide_model, len);
					new_disk_info->model[len] = 0;
				}
				else{
					len = strlen(DEFAULT_SATE_TAG);
					new_disk_info->tag = (char *)malloc(sizeof(char)*len+1);
					if(new_disk_info->tag == NULL){
						csprintf("No memory!!(new_disk_info->tag)\n");
						free(new_disk_info);
						fclose(fp);
						return disk_info_list;
					}
					strcpy(new_disk_info->tag, DEFAULT_SATE_TAG);
					new_disk_info->tag[len] = 0;
				}
			}
			else if(!strcmp(device_name, "hdc")){
				len = strlen(IDE_INFO_DIR)+strlen("//model")+strlen(device_name);
				ide_model_file = (char *)malloc(sizeof(char)*len+1);
				if(ide_model_file == NULL){
					csprintf("No memory!!(ide_model_file)\n");
					free(new_disk_info);
					fclose(fp);
					return disk_info_list;
				}
				sprintf(ide_model_file, "%s/%s/model", IDE_INFO_DIR, device_name);
				ide_model_file[len] = 0;
				
				info_fp = fopen(ide_model_file, "r");
				memset(ide_model, 0, 64);
				if(info_fp != NULL && fgets(ide_model, 64, info_fp) != NULL){
					len = strlen(ide_model)-1;	// del the char "\n".
					new_disk_info->tag = (char *)malloc(sizeof(char)*len+1);
					if(new_disk_info->tag == NULL){
						csprintf("No memory!!(new_disk_info->tag)\n");
						free(new_disk_info);
						fclose(fp);
						return disk_info_list;
					}
					strncpy(new_disk_info->tag, ide_model, len);
					new_disk_info->tag[len] = 0;
					
					new_disk_info->model = (char *)malloc(sizeof(char)*len+1);
					if(new_disk_info->model == NULL){
						csprintf("No memory!!(new_disk_info->model)\n");
						free(new_disk_info);
						fclose(fp);
						return disk_info_list;
					}
					strncpy(new_disk_info->model, ide_model, len);
					new_disk_info->model[len] = 0;
				}
				else{
					len = strlen(DEFAULT_E_SATE_TAG);
					new_disk_info->tag = (char *)malloc(sizeof(char)*len+1);
					if(new_disk_info->tag == NULL){
						csprintf("No memory!!(new_disk_info->tag)\n");
						free(new_disk_info);
						fclose(fp);
						return disk_info_list;
					}
					strcpy(new_disk_info->tag, DEFAULT_E_SATE_TAG);
					new_disk_info->tag[len] = 0;
				}
			}
			else if(!strncmp(device_name, "sd", 2)){
				// get the USB device order
				char *SCSI_FILE_INFO, *follow_info, target[32];
				
				memset(target, 0, 32);
				sprintf(target, "Diskname: %s Host: ", device_name);
				
				SCSI_FILE_INFO = read_whole_file(SCSI_FILE);
				if(SCSI_FILE_INFO == NULL){
					csprintf("Failed to open \"%s\"!!\n", SCSI_FILE);
					free(new_disk_info);
					fclose(fp);
					return disk_info_list;
				}
				
				if((follow_info = strstr(SCSI_FILE_INFO, target)) == NULL){
					csprintf("There was not \"%s\" in \"%s\"!!\n", device_name, SCSI_FILE);
					free(SCSI_FILE_INFO);
					free(new_disk_info);
					fclose(fp);
					return disk_info_list;
				}
				follow_info += strlen(target)+4;	// 4 = strlen("scsi")
				
				if(sscanf(follow_info, "%u", &device_order) != 1){
					csprintf("Couldn't get the device order of %s!!\n", device_path);
					free(SCSI_FILE_INFO);
					free(new_disk_info);
					fclose(fp);
					return disk_info_list;
				}
				
				// start get model
				char *vendor_follow, *model_follow, *rev_follow;
				char *vendor, *model;
				u32 vendor_len, model_len;
				
				vendor_follow = strstr(follow_info, "Vendor: ");
				if(vendor_follow == NULL){
					csprintf("\"%s\" is incorrect!\n", SCSI_FILE);
					free(SCSI_FILE_INFO);
					free(new_disk_info);
					fclose(fp);
					return disk_info_list;
				}
				model_follow = strstr(vendor_follow, " Model: ");
				if(model_follow == NULL){
					csprintf("\"%s\" is incorrect!\n", SCSI_FILE);
					free(SCSI_FILE_INFO);
					free(new_disk_info);
					fclose(fp);
					return disk_info_list;
				}
				rev_follow = strstr(model_follow, " Rev: ");
				if(rev_follow == NULL){
					csprintf("\"%s\" is incorrect!\n", SCSI_FILE);
					free(SCSI_FILE_INFO);
					free(new_disk_info);
					fclose(fp);
					return disk_info_list;
				}
				vendor_follow += strlen("Vendor: ");
				model_follow += strlen(" Model: ");
				rev_follow += strlen(" Rev: ");
				
				vendor_len = model_follow-vendor_follow-strlen(" Model: ");
				if(vendor_len >= 0){
					vendor = (char *)malloc(sizeof(char)*vendor_len+1);
					if(vendor == NULL){
						csprintf("No memory!!(vendor)\n");
						free(SCSI_FILE_INFO);
						free(new_disk_info);
						fclose(fp);
						return disk_info_list;
					}
					strncpy(vendor, vendor_follow, vendor_len);
					vendor[vendor_len] = 0;
					strntrim(vendor, vendor_len);
				}
				else
					vendor = NULL;
				new_disk_info->vendor = vendor;
				
				model_len = rev_follow-model_follow-strlen(" Rev: ");
				if(model_len >= 0){
					model = (char *)malloc(sizeof(char)*model_len+1);
					if(model == NULL){
						csprintf("No memory!!(model)\n");
						free(SCSI_FILE_INFO);
						free(new_disk_info);
						fclose(fp);
						return disk_info_list;
					}
					strncpy(model, model_follow, model_len);
					model[model_len] = 0;
					strntrim(model, model_len);
					
					model_len = strlen(model);
					new_disk_info->tag = (char *)malloc(sizeof(char)*model_len+1);
					if(new_disk_info->tag == NULL){
						csprintf("No memory!!(new_disk_info->tag)\n");
						free(SCSI_FILE_INFO);
						free(new_disk_info);
						fclose(fp);
						return disk_info_list;
					}
					strcpy(new_disk_info->tag, model);
					new_disk_info->tag[model_len] = 0;
				}
				else{
					model = NULL;
					
					new_disk_info->tag = find_disk_usb_port(&device_order);
				}
				new_disk_info->model = model;
				
				free(SCSI_FILE_INFO);
			}
			
			new_disk_info->mounted_number = 0;
			new_disk_info->device_order = device_order;
			new_disk_info->size_in_kilobytes = size_in_kilobytes;
			new_disk_info->partitions = NULL;
			new_disk_info->next = NULL;
			
			follow_disk_info_list = &disk_info_list;
			while(*follow_disk_info_list != NULL)
				follow_disk_info_list = &((*follow_disk_info_list)->next);
			*follow_disk_info_list = new_disk_info;
		}
		else if(is_partition_name(device_name, &partition_number) == 1){	// Partition
			// Found a partition device.
			disk_info_t *parent_disk_info;
			partition_info_t *new_partition_info;
			
			// Find the parent disk.
			parent_disk_info = disk_info_list;
			while(1){
				const char *disk_device;
				
				if(parent_disk_info == NULL){
					csprintf("Error while parsing %s: found "
									"partition '%s' but haven't seen the disk device "
									"of which it is a part.\n", PARTITION_FILE, device_name);
					fclose(fp);
					return disk_info_list;
				}
				
				disk_device = parent_disk_info->device;
				if(!strncmp(device_name, disk_device+5, 3)){
					device_order = parent_disk_info->device_order;
					break;
				}
				parent_disk_info = parent_disk_info->next;
			}
			
			new_partition_info = create_record_for_existing_partition(
					parent_disk_info, device_name, size_in_kilobytes, device_order, partition_number);
			if (new_partition_info == NULL){
				csprintf("Can't read the partition's information out!!");
				fclose(fp);
				return disk_info_list;
			}
		}
	}
	
	fclose(fp);
	return disk_info_list;
}

extern void free_disk_data(disk_info_t **disk_info_list){
	disk_info_t *follow_disk;
	
	follow_disk = *disk_info_list;
	while(follow_disk != NULL){
		disk_info_t *old_disk = follow_disk;
		partition_info_t *follow_partition;
		
		free(old_disk->tag);
		if(follow_disk->vendor != NULL)
			free(old_disk->vendor);
		if(follow_disk->model != NULL)
			free(old_disk->model);
		free(follow_disk->device);
		
		follow_partition = follow_disk->partitions;
		while(follow_partition != NULL){
			partition_info_t *old_partition = follow_partition;
			
			free(follow_partition->device);
			if(follow_partition->mount_point != NULL)
				free(follow_partition->mount_point);
			if(follow_partition->file_system != NULL && strcmp(follow_partition->file_system, "unknown"))
				free(follow_partition->file_system);
			
			follow_partition = follow_partition->next;
			free(old_partition);
		}
		
		follow_disk = follow_disk->next;
		free(old_disk);
	}
	
	*disk_info_list = NULL;
}

extern int is_partition_name(const char *device_name, u32 *partition_number){
	// get the partition number in the device_name
	*partition_number = (u32)strtol(device_name+3, NULL, 10);
	
	if(*partition_number <= 0 || *partition_number == LONG_MIN || *partition_number == LONG_MAX)
		return 0;
	
	return 1;
}

extern partition_info_t *create_record_for_existing_partition(
		disk_info_t *disk, char *device_name, u64 size_in_kilobytes,
		u32 device_order, u32 partition_number)
{
	u64 total_kilobytes = 0, used_kilobytes = 0;
	partition_info_t *result;
	partition_info_t **follow_partition_info_list;
	char *mount_info = read_whole_file(MOUNT_FILE);
	int len;
	
	if(mount_info == NULL){
		csprintf("Failed to open \"%s\"!!\n", MOUNT_FILE);
		return NULL;
	}
	
	result = (partition_info_t *)malloc(sizeof(partition_info_t));
	if(result == NULL){
		csprintf("No memory!!(result)\n");
		free(mount_info);
		return NULL;
	}
	
	len = strlen("/dev/")+strlen(device_name);
	result->device = (char *)malloc(sizeof(char)*len+1);
	if(result->device == NULL){
		csprintf("No memory!!(result->device)\n");
		free(mount_info);
		free(result);
		return NULL;
	}
	sprintf(result->device, "/dev/%s", device_name);
	(result->device)[len] = 0;
	result->disk = disk;
	result->partition_number = partition_number;
	
	result->mount_point = find_mount_point(device_name, mount_info);
	if(result->mount_point != NULL){
		++(disk->mounted_number);
		result->file_system = find_file_system(result->mount_point, mount_info);
		check_disk_free(result->mount_point, &total_kilobytes, &used_kilobytes);
	}
	else{
		result->file_system = "unknown";
		used_kilobytes = 0;
	}
	free(mount_info);
	
	result->used_kilobytes = used_kilobytes;
	result->size_in_kilobytes = size_in_kilobytes;
	result->next = NULL;
	
	// Insert the new record at the end of the partition list.
	follow_partition_info_list = &(disk->partitions);
	while(*follow_partition_info_list != NULL)
		follow_partition_info_list = &((*follow_partition_info_list)->next);
	*follow_partition_info_list = result;
	
	return result;
}

extern char *find_disk_usb_port(u32 *device_order){
	char usb_info_file[32], *usb_info, *follow_info;
	char port[4];
	char *port_name, target[16];
	int len;
	
	memset(usb_info_file, 0, 32);
	sprintf(usb_info_file, "%s/%u", USB_INFO_DIR, *device_order);
	
	usb_info = read_whole_file(usb_info_file);
	if(usb_info == NULL){
		csprintf("Cannot open the file \"%s\".\n", usb_info_file);
		return NULL;
	}
	
	if((follow_info = strstr(usb_info, "Port: ")) == NULL){
		csprintf("There was not the Port information with scsi%u.\n", *device_order);
		free(usb_info);
		return NULL;
	}
	follow_info += strlen("Port: ");
	
	memset(port, 0, 4);
	if(sscanf(follow_info, "%s", port) != 1){
		csprintf("There was not enough information in the usb status file!!\n");
		free(usb_info);
		return NULL;
	}
	free(usb_info);
	
	memset(target, 0, 16);
	if(!strcmp(port, "1")){
		strcpy(target, DEFAULT_FRONT_USB_TAG);
	}
	else if(!strcmp(port, "1.3")){
		strcpy(target, DEFAULT_BACK_TOP_USB_TAG);
	}
	else if(!strcmp(port, "1.4")){
		strcpy(target, DEFAULT_BACK_FOOT_USB_TAG);
	}
	
	len = strlen(target);
	port_name = (char *)malloc(sizeof(len)+1);
	if(port_name == NULL){
		csprintf("No memory!!(port_name)\n");
		return NULL;
	}
	strcpy(port_name, target);
	port_name[len] = 0;
	
	return port_name;
}

extern char *find_mount_point(const char *device_name, const char *const mount_info){
	char *start, *follow_info, *mount_point;
	int len;
	
	start = strstr(mount_info, device_name);
	if(start == NULL){
		csprintf("\"%s\" don't in \"%s\"!\n", device_name, MOUNT_FILE);
		return NULL;
	}
	start += strlen(device_name)+1;
	follow_info = start;
	while(*follow_info != 0 && !isspace(*follow_info))
		++follow_info;
	*follow_info = 0;
	
	len = strlen(start);
	mount_point = (char *)malloc(sizeof(char)*len+1);
	if(mount_point == NULL){
		csprintf("No memory!!(mount_point)\n");
		return NULL;
	}
	strncpy(mount_point, start, len);
	mount_point[len] = 0;
	
	return mount_point;
}

extern char *find_file_system(const char *mount_point, const char *const mount_info){
	char *start, *follow_info, *file_system;
	int len;
	
	start = strstr(mount_info, mount_point);
	if(start == NULL){
		csprintf("\"%s\" is incorrect or\n", MOUNT_FILE);
		csprintf("\"%s\" don't in \"%s\"!\n", mount_point, MOUNT_FILE);
		return NULL;
	}
	start += strlen(mount_point)+1;
	follow_info = start;
	while(*follow_info != 0 && !isspace(*follow_info))
		++follow_info;
	*follow_info = 0;
	
	len = strlen(start);
	file_system = (char *)malloc(sizeof(char)*len+1);
	if(file_system == NULL){
		csprintf("No memory!!(file_system)\n");
		return NULL;
	}
	strncpy(file_system, start, len);
	file_system[len] = 0;
	
	return file_system;
}

extern int check_disk_free(const char *mount_point, u64 *total_kilobytes, u64 *used_kilobytes){
	u64 total_size, free_size, used_size;
	struct statfs fsbuf;

	if(statfs(mount_point, &fsbuf)){
		csprintf("Can't get device's stat!!\n");
		return 0;
	}
	
	total_size = (u64)fsbuf.f_blocks*(u64)fsbuf.f_bsize;
	free_size = (u64)fsbuf.f_bfree*(u64)fsbuf.f_bsize;
	used_size = total_size-free_size;
	
	*total_kilobytes = total_size/1024;
	*used_kilobytes = used_size/1024;
	if(fsbuf.f_bfree == 0)
		return 0;
	else
		return 1;
}//*/

int main(int argc, char *argv[]){
	disk_info_t *follow_disk, *disks_info = read_disk_data();
	partition_info_t *follow_partition;
	
	for(follow_disk = disks_info; follow_disk != NULL; follow_disk = follow_disk->next){
		csprintf("              tag: %s.\n", follow_disk->tag);
		csprintf("           vendor: %s.\n", follow_disk->vendor);
		csprintf("            model: %s.\n", follow_disk->model);
		csprintf("           device: %s.\n", follow_disk->device);
		csprintf("            major: %u.\n", follow_disk->major);
		csprintf("            minor: %u.\n", follow_disk->minor);
		csprintf("   mounted_number: %u.\n", follow_disk->mounted_number);
		csprintf("     device_order: %u.\n", follow_disk->device_order);
		csprintf("size_in_kilobytes: %llu.\n", follow_disk->size_in_kilobytes);
		if(follow_disk->partitions == NULL){
			csprintf("\n");
			continue;
		}
		
		for(follow_partition = follow_disk->partitions; follow_partition != NULL; follow_partition = follow_partition->next){
			csprintf("     Partition %u:\n", follow_partition->partition_number);
			csprintf("parent disk's tag: %s.\n", follow_partition->disk->tag);
			csprintf("           device: %s.\n", follow_partition->device);
			csprintf("      mount_point: %s.\n", follow_partition->mount_point);
			csprintf("      file_system: %s.\n", follow_partition->file_system);
			csprintf("size_in_kilobytes: %llu.\n", follow_partition->size_in_kilobytes);
			csprintf("   used_kilobytes: %llu.\n", follow_partition->used_kilobytes);
		}
		
		csprintf("\n");
	}//*/
	
	free_disk_data(&disks_info);
	
	return 0;
}
