#
# Linux netconf Makefile
#
# Copyright (C) 2008, Broadcom Corporation
# All Rights Reserved.
# 
# THIS SOFTWARE IS OFFERED "AS IS", AND BROADCOM GRANTS NO WARRANTIES OF ANY
# KIND, EXPRESS OR IMPLIED, BY STATUTE, COMMUNICATION OR OTHERWISE. BROADCOM
# SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS
# FOR A SPECIFIC PURPOSE OR NONINFRINGEMENT CONCERNING THIS SOFTWARE.
#
# $Id: Makefile,v 1.16 2007/10/13 01:05:02 Exp $
#

include $(TOP)/.config

CFLAGS	+= -I. -I$(TOP)/shared -I$(SRCBASE)/include -I$(LINUXDIR)/include -Wall
#CFLAGS	+= -g -DDEBUG
CFLAGS	+= -s -O2
ifeq ($(LINUXDIR), $(SRCBASE)/linux/linux-2.6)
CFLAGS += -DLINUX26
CFLAGS  += -I$(TOP)/iptables-1.x/include
LDFLAGS += -L. -L$(TOP)/iptables-1.x/libiptc
else
CFLAGS  += -I$(TOP)/iptables/include
LDFLAGS += -L. -L$(TOP)/iptables/libiptc
endif

all: libnetconf.so netconf

install: all
	install -D libnetconf.so $(INSTALLDIR)/usr/lib/libnetconf.so
	$(STRIP) $(INSTALLDIR)/usr/lib/libnetconf.so

clean:
	rm -f *.o *.a *.so netconf

libnetconf.so: netconf_linux.o
	$(LD) -shared -o $@ $^ $(LDFLAGS) -liptc

netconf: main.o libnetconf.so
	$(CC) -o $@ $< $(LDFLAGS) -lnetconf
