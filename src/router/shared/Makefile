#
# Linux router shared code Makefile
#
# Copyright (C) 2008, Broadcom Corporation
# All Rights Reserved.
# 
# THIS SOFTWARE IS OFFERED "AS IS", AND BROADCOM GRANTS NO WARRANTIES OF ANY
# KIND, EXPRESS OR IMPLIED, BY STATUTE, COMMUNICATION OR OTHERWISE. BROADCOM
# SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS
# FOR A SPECIFIC PURPOSE OR NONINFRINGEMENT CONCERNING THIS SOFTWARE.
#
# $Id: Makefile,v 1.19 2008/04/18 02:43:39 Exp $
#


CFLAGS	+= -I. -I../httpd -I$(SRCBASE)/include -Wall
#CFLAGS	+= -g -DDEBUG
CFLAGS	+= -s -O2 -DDLM
LDFLAGS += -L.

OBJS := nvparse.o shutils.o wl.o wl_linux.o linux_timer.o defaults.o wlif_utils.o UdpLib.o notify_rc.o semaphore.o

ifeq ($(CONFIG_DLM), y)
OBJ2 := crc_gen
endif

all: libshared.so $(OBJ2)

install: all
	install -d $(INSTALLDIR)/usr/lib
	install -m 755 libshared.so $(INSTALLDIR)/usr/lib
	$(STRIP) $(INSTALLDIR)/usr/lib/libshared.so

clean:
	rm -f *.o *.so

libshared.so: $(OBJS)
	$(LD) -shared -o $@ $^

crc_gen: crc_gen.o
	gcc -o crc_gen crc_gen.o
                                                                                                  
crc_gen.o:
	gcc -c -o crc_gen.o crc_gen.c

