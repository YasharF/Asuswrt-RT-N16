#
# Toplevel Makefile for the BCM947xx Linux Router release
#
# Copyright (C) 2009, Broadcom Corporation
# All Rights Reserved.
# 
# THIS SOFTWARE IS OFFERED "AS IS", AND BROADCOM GRANTS NO WARRANTIES OF ANY
# KIND, EXPRESS OR IMPLIED, BY STATUTE, COMMUNICATION OR OTHERWISE. BROADCOM
# SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS
# FOR A SPECIFIC PURPOSE OR NONINFRINGEMENT CONCERNING THIS SOFTWARE.
#
# $Id: toplevel-release.mk,v 1.34 2009/04/23 10:10:37 Exp $
#

SRCBASE := $(shell pwd)
RELEASEDIR := $(shell (cd $(SRCBASE)/.. && pwd -P))

install: all
	install -d $(RELEASEDIR)/image
	$(MAKE) -C router install
	-cp router/mipsel/linux.trx $(RELEASEDIR)/image/linux-glibc.trx
	-cp router/mipsel-uclibc/linux.trx $(RELEASEDIR)/image/linux.trx
	-cp router/mipsel-uclibc/linux-lzma.trx $(RELEASEDIR)/image/linux-lzma.trx
	-cp router/mipsel-uclibc/linux-trim.trx $(RELEASEDIR)/image/linux-trim.trx
	-cp router/mipsel-uclibc/linux.bin $(RELEASEDIR)/image/linux.bin
ifneq ($(wildcard cfe),)
	cp cfe/build/broadcom/bcm947xx/cfe.bin $(RELEASEDIR)/image/
	cp cfe/build/broadcom/bcm947xx/compressed/cfez.bin $(RELEASEDIR)/image/
	# Clean and make one more time linked at 3MB
	$(MAKE) -C cfe clean
	$(MAKE) -C cfe CFG_TEXT_START=0x80300000
	cp cfe/build/broadcom/bcm947xx/cfe.bin $(RELEASEDIR)/image/cfe4.bin
	cp cfe/build/broadcom/bcm947xx/compressed/cfez.bin $(RELEASEDIR)/image/cfez4.bin
	# Clean and make one more time with GigE
	$(MAKE) -C cfe clean
	$(MAKE) -C cfe CFG_PCI=1
	cp cfe/build/broadcom/bcm947xx/cfe.bin $(RELEASEDIR)/image/cfe-gige.bin
	cp cfe/build/broadcom/bcm947xx/compressed/cfez.bin $(RELEASEDIR)/image/cfez-gige.bin
	# Clean and make one more time with GMAC
	$(MAKE) -C cfe clean
	$(MAKE) -C cfe CFG_GMAC=1
	cp cfe/build/broadcom/bcm947xx/cfe.bin $(RELEASEDIR)/image/cfe-gmac.bin
	cp cfe/build/broadcom/bcm947xx/compressed/cfez.bin $(RELEASEDIR)/image/cfez-gmac.bin
endif
	cp shared/nvram/*.txt $(RELEASEDIR)/image/

all clean:
	$(MAKE) -C router $@
ifneq ($(wildcard cfe),)
	$(MAKE) -C cfe $@
endif

.PHONY: all clean install
