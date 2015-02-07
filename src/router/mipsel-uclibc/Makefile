
#*
# ****************************************************************
# *   Copyright 2008, ASUSTek COMPUTER Inc.  All Rights Reserved *
# ****************************************************************
#
# THIS SOFTWARE IS OFFERED "AS IS", AND ASUSTek GRANTS NO WARRANTIES OF ANY
# KIND, EXPRESS OR IMPLIED, BY STATUTE, COMMUNICATION OR OTHERWISE.  ASUSTek
# SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS
# FOR A SPECIFIC PURPOSE OR NONINFRINGEMENT CONCERNING THIS SOFTWARE
#
#
# $Id: Makefile,v 1.7 2008/11/05 02:45:36 james26_jang Exp $
#

include ../.config
#export CHMOD := $(shell (chmod +x mkweb))

# Source bases
export PLATFORM LINUXDIR LIBDIR USRLIBDIR
TOP := $(shell pwd)/..
SRCBASE := $(shell (cd $(TOP)/.. && pwd -P))
RELEASEDIR := $(shell (cd $(SRCBASE)/.. && pwd -P))
PATH := $(RELEASEDIR)/tools:$(PATH)
#
# Install and target directories
#

ifeq ($(PLATFORMDIR), )
PLATFORMDIR := .
endif

#PLATFORMDIR := $(TOP)/$(PLATFORM)-uclibc
INSTALLDIR := $(PLATFORMDIR)/install
TARGETDIR := $(PLATFORMDIR)/target

# Addeb by Joey to include version parameters
include version.conf

all: RT-N16 RT-N12 RT-N10


RT-N16:
	rm -rf www
	( if [ -d $(TARGETDIR)/www ]; then \
		mv $(TARGETDIR)/www .; \
	fi )
	
	cd $(TOP)/www/www.RT-N16 && rm -f `find -name Thumbs.db` && cd $(PLATFORMDIR)
	cp -rf $(TOP)/www/www.RT-N16 $(PLATFORMDIR)/target/www
	touch $(TARGETDIR)/tmp/settings
	cd $(TARGETDIR)/www && ln -sf ../tmp/settings Settings.CFG && cd ../..
	rm -f $(TARGETDIR)/tmp/settings
	cd $(TARGETDIR) && rm -rf `find -name CVS` && cd ..
	rm -rf $(PLATFORMDIR)/www

	$(TOP)/squashfs/mksquashfs $(TARGETDIR) target.squashfs -all-root -noappend -nopad
	addver target.squashfs $(KERNEL_VER) $(N16_FS_VER) RT-N16 $(HWL1) $(HWH1) $(HWL2) $(HWH2) $(HWL3) $(HWH3) $(HWL4) $(HWH4) vmlinuz
	trx -o RT-N16_$(KERNEL_VER).$(N16_FS_VER).trx vmlinuz target.squashfs

RT-N12:
	rm -rf www
	( if [ -d $(TARGETDIR)/www ]; then \
		mv $(TARGETDIR)/www .; \
	fi )
	
	cd $(TOP)/www/www.RT-N12 && rm -f `find -name Thumbs.db` && cd $(PLATFORMDIR)
	cp -rf $(TOP)/www/www.RT-N12 $(PLATFORMDIR)/target/www
	touch $(TARGETDIR)/tmp/settings
	cd $(TARGETDIR)/www && ln -sf ../tmp/settings Settings.CFG && cd ../..
	rm -f $(TARGETDIR)/tmp/settings
	cd $(TARGETDIR) && rm -rf `find -name CVS` && cd ..
	rm -rf $(PLATFORMDIR)/www

	$(TOP)/squashfs/mksquashfs $(TARGETDIR) target.squashfs -all-root -noappend -nopad
	addver target.squashfs $(KERNEL_VER) $(N12_FS_VER) RT-N12 $(HWL1) $(HWH1) $(HWL2) $(HWH2) $(HWL3) $(HWH3) $(HWL4) $(HWH4) vmlinuz-lzma
	trx -o RT-N12_$(KERNEL_VER).$(N12_FS_VER).trx vmlinuz-lzma target.squashfs

image:
	$(TOP)/squashfs/mksquashfs $(TARGETDIR) target.squashfs -all-root -noappend -nopad

	addver target.squashfs $(KERNEL_VER) $(N12_FS_VER) RT-N1X $(HWL1) $(HWH1) $(HWL2) $(HWH2) $(HWL3) $(HWH3) $(HWL4) $(HWH4) vmlinuz
	trx -o RT-N1X_$(KERNEL_VER).$(N1X_FS_VER).trx vmlinuz target.squashfs

RT-N10:
	rm -rf www
	( if [ -d $(TARGETDIR)/www ]; then \
		mv $(TARGETDIR)/www .; \
	fi )
	
	cd $(TOP)/www/www.RT-N10 && rm -f `find -name Thumbs.db` && cd $(PLATFORMDIR)
	cp -rf $(TOP)/www/www.RT-N10 $(PLATFORMDIR)/target/www
	touch $(TARGETDIR)/tmp/settings
	cd $(TARGETDIR)/www && ln -sf ../tmp/settings Settings.CFG && cd ../..
	rm -f $(TARGETDIR)/tmp/settings
	#cp -rf ../www/asus/www_WL500gpv2 $(TARGETDIR)/www
	cd $(TARGETDIR) && rm -rf `find -name CVS` && cd ..
	rm -rf $(PLATFORMDIR)/www
	#cd $(TARGETDIR)/usr/sbin/ && rm -f wl && ln -sf wlconf wl && cd ../../..

	#$(LINUXDIR)/scripts/mkcramfs $(TARGETDIR) target.cramfs
	#$(MAKE) -C $(LINUXDIR)/scripts/squashfs mksquashfs CC=gcc CXX=g++
	#$(LINUXDIR)/scripts/squashfs/mksquashfs $(TARGETDIR) target.cramfs -all-root -noappend -nopad
	$(TOP)/squashfs/mksquashfs $(TARGETDIR) target.squashfs -all-root -noappend -nopad

	addver target.squashfs $(KERNEL_VER) $(N10_FS_VER) RT-N10 $(HWL1) $(HWH1) $(HWL2) $(HWH2) $(HWL3) $(HWH3) $(HWL4) $(HWH4) vmlinuz
	trx -o RT-N10_$(KERNEL_VER).$(N10_FS_VER).trx vmlinuz target.squashfs

clean:
	cd $(PLATFORMDIR) && \
	rm -rf *.trx && \
	rm -rf $(TARGETDIR) target.cramfs && \
	$(MAKE) -C $(LINUXDIR) clean

