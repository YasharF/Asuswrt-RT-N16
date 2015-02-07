OBJS = disk_io_tools.o disk_initial.o disk_share.o# disk_swap.o

CFLAGS += -I. -I$(TOP)/shared -I$(SRCBASE)/include -Wall
LDFLAGS = -L. -ldisk -L$(TOP)/nvram -lnvram -L$(TOP)/shared -lshared -L$(TOP)/netconf -lnetconf

all: libdisk.so initial_var_file write_smb_conf test_of_var_files_in_mount_path create_if_no_var_files initial_folder_list_in_mount_path disk_test

disk_test: disk_test.o
	$(CC) -Wall -L. -ldisk -L$(TOP)/nvram -lnvram -o $@ $^

libdisk.so: $(OBJS) $(TOP)/shared/shutils.o
	$(LD) -shared -o $@ $^

initial_var_file: initial_var_file.o libdisk.so
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

write_smb_conf: write_smb_conf.o libdisk.so
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

test_of_var_files_in_mount_path: test_of_var_files_in_mount_path.o libdisk.so
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

create_if_no_var_files: create_if_no_var_files.o libdisk.so
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

initial_folder_list_in_mount_path: initial_folder_list_in_mount_path.o libdisk.so
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

install: all
	install -D libdisk.so $(INSTALLDIR)/usr/lib/libdisk.so
	install -D initial_var_file $(INSTALLDIR)/sbin/initial_var_file
	install -D write_smb_conf $(INSTALLDIR)/sbin/write_smb_conf
	install -D test_of_var_files_in_mount_path $(INSTALLDIR)/sbin/test_of_var_files_in_mount_path

installb: all
	cp -f libdisk.so disk_test /mnt/share/

clean:
	rm -f *.o libdisk.so initial_var_file write_smb_conf test_of_var_files_in_mount_path create_if_no_var_files initial_folder_list_in_mount_path disk_test
