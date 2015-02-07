#
# Author: Noel V Aguilar
# CGI-LIB Library
# Release 1.4
#
# This library is free softare; you can redistribute it
# and/or modify it.
#
# You can compile this library by simply running 'make'
# and then copy the cgi-lib.a to the your lib directory
# and the cgi-lib.h and html-lib.h files into your
# include directory manually.  This will allow the linker
# to automatically include the library or you can copy the
# files to where your development is being done and link it
# manually.
#
# For updates or to report bugs please go to:
# http://www.geocities.com/SiliconValley/Vista/6493/
#
# This library is distributed in the hope that it will be
# useful but WITHOUT ANY WARRANTY; without even the implied
# warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
:wq
:# PURPOSE.
#

CGI-LIB=cgi-lib.a
# CC=gcc
CFLAGS +=-fPIC -Wall -c
# AR=ar
ARFLAGS=rc
objs=cgi-lib.o html-lib.o list-lib.o


COMM-LIB-PATH = ../lib
COMM-INC-PATH = ../include
TARGET-LIB=../../target/usr/lib

#all: $(CGI-LIB)
#
#$(CGI-LIB): $(OBJS)
#	@echo Building library
#	$(AR) $(ARFLAGS) $@ $(OBJS)
#	@echo Building complete
all: cgilib

cgi-lib.o: cgi-lib.c cgi-lib.h cgi-priv.h
	$(CC) $(CFLAGS) cgi-lib.c

html-lib.o: html-lib.c html-lib.h
	$(CC) $(CFLAGS) html-lib.c

list-lib.o: list-lib.c list-lib.h
	$(CC) $(CFLAGS) list-lib.c

cgilib: $(objs)
	$(CC) -shared -Wl,-soname,libcgi.so.0 -o libcgi.so.0.0 $(objs)
	arm-linux-strip libcgi.so.0.0
	ln -sf libcgi.so.0.0 libcgi.so.0
	ln -sf libcgi.so.0 libcgi.so
	cp -afv libcgi* $(COMM-LIB-PATH)
	cp -f cgi-lib.h $(COMM-INC-PATH)
	cp -f html-lib.h $(COMM-INC-PATH)
	cp -f list-lib.h $(COMM-INC-PATH)

clean:
	echo Cleaning up
	rm -f *.o
	rm -f libcgi*
	rm -f $(COMM-LIB-PATH)/libcgi*
	rm -f $(COMM-INC-PATH)/cgi-lib.h
	rm -f $(COMM-INC-PATH)/html-lib.h
	rm -f $(COMM-INC-PATH)/list-lib.h

romfs:
	$(ROMFSINST) libcgi.so.0.0 /lib
	$(ROMFSINST) -s libcgi.so.0.0 /lib/libcgi.so.0
	$(ROMFSINST) -s libcgi.so.0 /lib/libcgi.so
# install:
#	mkdir -p $(TARGET-LIB)
#	cp -pdR $(COMM-LIB-PATH)/libcgi* $(TARGET-LIB)

