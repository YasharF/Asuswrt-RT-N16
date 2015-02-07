#Makefile
# CC=gcc
CFLAGS +=-fPIC -Wall -c
# AR=ar
ARFLAGS=rc

objs=file-upload.o

COMM-LIB-PATH = ../lib
COMM-INC-PATH = ../include
TARGET-LIB=../../target/usr/lib

all: $(objs)
	$(CC) -shared -L $(TARGET-LIB) -Wl,-soname,libfileupload.so.0 -o libfileupload.so.0.0 $(objs)
	arm-linux-strip libfileupload.so.0.0
	ln -sf libfileupload.so.0.0 libfileupload.so.0
	ln -sf libfileupload.so.0 libfileupload.so
	cp -afv libfileupload* $(COMM-LIB-PATH)
	cp -f *.h $(COMM-INC-PATH)

file_upload.o:    file-upload.c file-upload.h
	$(CC) $(CFLAGS) -I $(COMM-INC-PATH) $<

clean:
	echo Cleaning up
	rm -f *.o *.bak 
	rm -f libfileupload*
	rm -f $(COMM-LIB-PATH)/libfileupload*
	rm -f $(COMM-INC-PATH)/file-upload.h

romfs:
	$(ROMFSINST) libfileupload.so.0.0 /lib
	$(ROMFSINST) -s libfileupload.so.0.0 /lib/libfileupload.so.0
	$(ROMFSINST) -s libfileupload.so.0 /lib/libfileupload.so
#install:
#	mkdir -p $(TARGET-LIB)
#	cp -pdR $(COMM-LIB-PATH)/libfileupload* $(TARGET-LIB)


# End of Makefile.
