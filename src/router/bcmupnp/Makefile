#
# Linux upnp Makefile support WFA device
#
# Copyright (C) 2009, Broadcom Corporation
# All Rights Reserved.
# 
# THIS SOFTWARE IS OFFERED "AS IS", AND BROADCOM GRANTS NO WARRANTIES OF ANY
# KIND, EXPRESS OR IMPLIED, BY STATUTE, COMMUNICATION OR OTHERWISE. BROADCOM
# SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS
# FOR A SPECIFIC PURPOSE OR NONINFRINGEMENT CONCERNING THIS SOFTWARE.
#
# $Id: Makefile,v 1.9 2009/02/17 06:15:37 Exp $
#

ifeq "$(SRCBASE)" ""
	SRCBASE=$(shell cd ../.. ; pwd)
endif

include $(SRCBASE)/router/.config

CFG_BCMIGD ?= 1

UPNP	:= $(SRCBASE)/router/bcmupnp/upnp/linux
CFLAGS += -s -O2 -include bcmconfig.h
#DEBUG	:= 1

ifneq ($(wildcard ./prebuilt),)
all: docopy upnp

docopy:
	install -d $(INSTALLDIR)/usr/sbin
	install -m 755 prebuilt/upnp $(INSTALLDIR)/usr/sbin/upnp
else
all: upnp
endif

install: all
	[ ! -f $(UPNP)/Makefile ] || $(MAKE) -C $(UPNP) CFG_BCMIGD=$(CFG_BCMIGD) install
	$(MAKE) -C $(UPNP) -f Makefile.nat CFG_BCMIGD=$(CFG_BCMIGD) install

clean:
	[ ! -f $(UPNP)/Makefile ] || $(MAKE) -C $(UPNP) clean
	$(MAKE) -C $(UPNP) -f Makefile.nat clean

upnp: FORCE
	[ ! -f $(UPNP)/Makefile ] || $(MAKE) -C $(UPNP) DEBUG=$(DEBUG) CFG_BCMIGD=$(CFG_BCMIGD)
	$(MAKE) -C $(UPNP) -f Makefile.nat DEBUG=$(DEBUG) CFG_BCMIGD=$(CFG_BCMIGD)

FORCE:
