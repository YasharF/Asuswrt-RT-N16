#-----------------------------------------------------
#TOPDIR  :=  $(shell /bin/pwd)
#TOPDIR  :=  $(TOPDIR)/../../..

#include $(TOPDIR)/prerules.mk
#-----------------------------------------------------
#CC=arm-linux-gcc
#STRIP=arm-linux-strip
localedir=
CFLAGS +=-mbig-endian -fPIC -I. -I../include -DLOCALEDIR=\"$(localedir)\" -DHAVE_CONFIG_H -DLIBDIR=\"\" -DUSE_DOS
ARFLAGS=rc
OBJS=iconv.o
LIBCHARSET_OBJECTS = ../libcharset/lib/localcharset.o

all: $(OBJS) $(LIBCHARSET_OBJECTS)
	$(CC) $(CFLAGS) -shared -Wl,-soname,libiconv.so.0 -o libiconv.so.0.0 $(OBJS) $(LIBCHARSET_OBJECTS)
	$(STRIP) libiconv.so.0.0
	ln -sf libiconv.so.0.0 libiconv.so.0
	ln -sf libiconv.so.0 libiconv.so
	cp -afv libiconv* ../../lib/

romfs:
	$(ROMFSINST) libiconv.so.0.0 /lib/libiconv.so.0.0
	$(ROMFSINST) -s libiconv.so.0.0 /lib/libiconv.so.0
	$(ROMFSINST) -s libiconv.so.0 /lib/libiconv.so
	
iconv.o: iconv.c
	$(CC) -c $(CFLAGS) iconv.c

clean:
	@echo Cleaning up
	rm -f *.[od]
	rm -f ../libcharset/lib/localcharset.[od]
	rm -f libiconv*
	rm -f $(COMM_LIB_PATH)/libiconv*

#-----------------------------------------------------
#include $(TOPDIR)/rules.mk
#-----------------------------------------------------
