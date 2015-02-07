#
# U2EC Makefile
#
# Copyright (C) 2008 ASUSTek Corporation
#
# $Id: Makefile,v 1.4 2008/10/31 02:27:48 james26_jang Exp $
#

ifeq ($(strip $(CROSS_COMPILE)),)
CROSS_COMPILE := mipsel-linux-
TOP := $(shell pwd)/..
SRCBASE := $(TOP)/..
INSTALLDIR := $(TOP)/mipsel/install
CC := $(CROSS_COMPILE)gcc
AR := $(CROSS_COMPILE)ar
AS := $(CROSS_COMPILE)as
LD := $(CROSS_COMPILE)ld
NM := $(CROSS_COMPILE)nm
RANLIB := $(CROSS_COMPILE)ranlib
STRIP := $(CROSS_COMPILE)strip
SIZE := $(CROSS_COMPILE)size
endif

PROGS	= u2ec
OBJS	= usbsock.o testusb.o
DEBUG	= n

ifeq ($(DEBUG),y)
	DEBFLAGS = -O -g -DU2EC_DEBUG
	DEBFLAGS += -DPDEBUG_SENDSECV
	DEBFLAGS += -DPDEBUG_DECODE
	OBJS += debug.o decode.o
else
	DEBFLAGS = -s -O2
endif

ifeq ($(strip $(CROSS_COMPILE)),)
	CC = gcc
	DEBFLAGS += -DU2EC_ONPC
	LDFLAGS = -L/usr/local/lib -lusb -lpthread -Wl,--rpath -Wl,/usr/local/lib
else
	CFLAGS = -DSUPPORT_LPRng
	CFLAGS += -I$(TOP)/shared -I$(SRCBASE)/linux/linux/include -I$(SRCBASE)/include
	LDFLAGS = -lpthread
	LDFLAGS += -L$(TOP)/libusb/.libs -L$(INSTALLDIR)/libusb/usr/lib -lusb
	LDFLAGS += -L$(TOP)/shared -L$(INSTALLDIR)/shared/usr/lib -lshared
	LDFLAGS += -L$(TOP)/nvram -L$(INSTALLDIR)/nvram/usr/lib -lnvram
	LDFLAGS += -L$(TOP)/netconf -L$(INSTALLDIR)/nvram/usr/lib -lnetconf
endif

CFLAGS += -Wall -I.
CFLAGS += $(DEBFLAGS)

all: $(PROGS)

$(PROGS): $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

ifneq ($(strip $(CROSS_COMPILE)),)
install: $(PROGS)
	install -D $(PROGS) $(INSTALLDIR)/usr/sbin/$(PROGS)
	$(STRIP) $(INSTALLDIR)/usr/sbin/$(PROGS)
endif

clean:
	rm -f $(PROGS) *.o *~
