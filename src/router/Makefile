#
# Broadcom Linux Router Makefile
# 
# Copyright (C) 2008, Broadcom Corporation
# All Rights Reserved.
# 
# THIS SOFTWARE IS OFFERED "AS IS", AND BROADCOM GRANTS NO WARRANTIES OF ANY
# KIND, EXPRESS OR IMPLIED, BY STATUTE, COMMUNICATION OR OTHERWISE. BROADCOM
# SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS
# FOR A SPECIFIC PURPOSE OR NONINFRINGEMENT CONCERNING THIS SOFTWARE.
#
# $Id: Makefile,v 1.164 2008/12/02 23:53:34 Exp $
#

include .config
include .config.asus

export CONFIG_MULTICONFIG := y

#
# Paths
#


CPU ?=
LINUX_VERSION ?= 2_6

# Source bases
export PLATFORM LIBDIR USRLIBDIR
export TOP := $(shell pwd)
export SRCBASE := $(shell (cd $(TOP)/.. && pwd -P))

ifeq ($(LINUX_VERSION),2_6)
export 	LINUXDIR := $(SRCBASE)/linux/linux-2.6
else
export 	LINUXDIR := $(SRCBASE)/linux/linux
endif


#
# Cross-compile environment variables
#

# Build platform
export BUILD := i386-pc-linux-gnu
export HOSTCC := gcc

# uClibc wrapper
ifeq ($(CONFIG_UCLIBC),y)
export PLATFORM := $(PLATFORM)-uclibc
endif

ifeq ($(PLATFORM),mipsel)
export CROSS_COMPILE := mipsel-linux-
export CONFIGURE := ./configure mipsel-linux --build=$(BUILD)
export TOOLCHAIN := $(shell cd $(dir $(shell which $(CROSS_COMPILE)gcc))/../mipsel-linux && pwd -P)
endif

ifeq ($(PLATFORM),mipsel-uclibc)
export CROSS_COMPILE := mipsel-uclibc-
export CONFIGURE := ./configure mipsel-linux --build=$(BUILD)
export TOOLCHAIN := $(shell cd $(dir $(shell which $(CROSS_COMPILE)gcc))/.. && pwd -P)
endif


ifeq ($(CONFIG_BCMWPA2),y)
export CFLAGS += -DBCMWPA2
endif

ifeq ($(CONFIG_WSCCMD),y)
export CONFIG_WSCCMD
export CFLAGS += -DBCMWPS

endif

ifeq ($(CONFIG_EMF),y)
export CFLAGS += -D__CONFIG_EMF__
endif

ifeq ($(CONFIG_IGMP_PROXY),y)
export CFLAGS += -D__CONFIG_IGMP_PROXY__
endif


export CC := $(CROSS_COMPILE)gcc
export AR := $(CROSS_COMPILE)ar
export AS := $(CROSS_COMPILE)as
export LD := $(CROSS_COMPILE)ld
export NM := $(CROSS_COMPILE)nm
export RANLIB := $(CROSS_COMPILE)ranlib
export STRIP := $(CROSS_COMPILE)strip
export SIZE := $(CROSS_COMPILE)size

#
# Install and target directories
#

export PLATFORMDIR := $(TOP)/$(PLATFORM)
export INSTALLDIR := $(PLATFORMDIR)/install
export TARGETDIR := $(PLATFORMDIR)/target

define STRIP_DEBUG_SYMBOLS
	@dbgsymf=$(basename $(1))_dbgsym$(suffix $(1)); \
	if [ "$(1)" -nt "$${dbgsymf}" ]; then \
	   echo "#- $0"; \
	   ls -ln $1 | awk '{printf "Orig  size: %10d bytes, %s\n",$$5,$$NF}'; \
	   cp -p -v $1 $$dbgsymf; $(STRIP) -d $(1); touch $$dbgsymf; \
	   ls -ln $1 | awk '{printf "Strip size: %10d bytes, %s\n",$$5,$$NF}'; \
	fi
endef



# USB AP support
# note : the dongle target is only for after pre-build 
obj-$(CONFIG_USBAP) += bmac libusb bcmdl dongle

# always build libbcmcrypto
obj-y += libbcmcrypto

#
# Configuration
#

obj-$(CONFIG_NVRAM) += nvram
obj-$(CONFIG_SHARED) += shared
obj-$(CONFIG_LIBBCM) += libbcm
obj-$(CONFIG_HTTPD) += httpd
obj-$(CONFIG_RC) += rc
obj-$(CONFIG_WWW) += www
obj-$(CONFIG_GLIBC) += lib
obj-$(CONFIG_UCLIBC) += lib
obj-$(CONFIG_WLCONF) += wlconf
obj-$(CONFIG_BRIDGE) += bridge
obj-$(CONFIG_BUSYBOX) += busybox
obj-$(CONFIG_DNSMASQ) += dnsmasq
obj-$(CONFIG_IPTABLES) += iptables
obj-$(CONFIG_LIBIPT) += iptables
obj-$(CONFIG_NAS) += nas
obj-$(CONFIG_SES) += ses/ses
obj-$(CONFIG_SES_CL) += ses/ses_cl
obj-$(CONFIG_EZC) += ezc
obj-$(CONFIG_NETCONF) += netconf
obj-$(CONFIG_NTP) += ntpclient
obj-$(CONFIG_UDHCPD) += udhcpd
obj-$(CONFIG_UPNP) += bcmupnp
obj-$(CONFIG_UTILS) += utils
obj-$(CONFIG_ETC) += etc
obj-$(CONFIG_VLAN) += vlan
obj-$(CONFIG_WSCCMD) += wps
obj-$(CONFIG_EMF) += emf
obj-$(CONFIG_EMF) += igs
obj-$(CONFIG_IGMP_PROXY) += igmp
ifeq ($(LINUXDIR), $(SRCBASE)/linux/linux-2.6)
obj-y += udev
obj-y += hotplug2
endif
obj-$(CONFIG_LLD2D) += lltd/wrt54g-linux

# ASUS Applications
obj-$(CONFIG_PPPD) += pppd
obj-$(CONFIG_PPTP) += pptp-client
obj-$(CONFIG_PPPOE) += rp-pppoe
obj-$(CONFIG_PPPOERELAY) += pppoe-relay
obj-$(CONFIG_INFOSVR) += infosvr
obj-$(CONFIG_EZIPUPDATE) += ez-ipupdate
obj-$(CONFIG_JPEG6B) += jpeg-6b
obj-$(CONFIG_RCAMD) += rcamdmips
obj-$(CONFIG_SENDMAIL) += sendmail
obj-$(CONFIG_LPRNG) += LPRng
obj-$(CONFIG_OTHERS) += others
obj-$(CONFIG_OV51X) += ov51x
obj-$(CONFIG_TEST) += test
obj-$(CONFIG_BPALOGIN) += bpalogin
obj-$(CONFIG_IPROUTE2) += iproute2

obj-$(CONFIG_ATTR) += attr
obj-$(CONFIG_ACL) += acl
obj-$(CONFIG_SAMBA) += samba
obj-$(CONFIG_VSFTPD) += vsftpd
obj-$(CONFIG_L2TP) += rp-l2tp
obj-$(CONFIG_IGMPPROXY) += igmpproxy

obj-$(CONFIG_UTELNETD) += utelnetd
obj-$(CONFIG_LIBDISK) += libdisk                # 2007.10 James
obj-$(CONFIG_WANDUCK) += wanduck                # 2007.10 James
obj-$(CONFIG_NETWORKMAP) += networkmap          # 2008.06.25 Yau add
obj-$(CONFIG_FDISK) += fdisk                    # 2008.04 James
obj-$(CONFIG_MKDOSFS) += mkdosfs                # 2008.04 James
obj-$(CONFIG_E2FSPROGS) += e2fsprogs-1.40.8     # 2008.04 James

obj-$(CONFIG_LIBUSB) += libusb
obj-$(CONFIG_U2EC) += u2ec
obj-$(CONFIG_DPROXY) += dproxy


#obj-$(CONFIG_LIBMPLUS) += libmplus
#obj-$(CONFIG_FFMPEG) += ffmpeg
obj-$(CONFIG_LIBUPNP) += libupnp-1.6.6
#obj-$(CONFIG_LIBDLNA) += libdlna-0.2.3
obj-$(CONFIG_USHARE) += ushare-1.1a

obj-y += udpxy


#obj-$(CONFIG_UPNP) += upnp

# always build eap dispatcher
obj-y += eapd/linux


obj-clean := $(foreach obj,$(obj-y) $(obj-n),$(obj)-clean)
obj-install := $(foreach obj,$(obj-y),$(obj)-install)


#
# Basic rules
#

export LANGUAGE := "TW"
export USB_WAN := "NONE"


all: version $(LINUXDIR)/.config $(obj-y)
        # Also build kernel
ifeq ($(LINUXDIR), $(SRCBASE)/linux/linux-2.6)
	if ! grep -q "CONFIG_BLK_DEV_INITRD=y" $(LINUXDIR)/.config ; then \
	    $(MAKE) -C $(LINUXDIR) zImage ; \
	    $(MAKE) -C $(SRCBASE)/router/compressed ; \
	fi
else
	if ! grep -q "CONFIG_EMBEDDED_RAMDISK=y" $(LINUXDIR)/.config ; then \
	    $(MAKE) -C $(LINUXDIR) zImage ; \
	fi
endif
	if grep -q "CONFIG_MODULES=y" $(LINUXDIR)/.config ; then \
	    $(MAKE) -C $(LINUXDIR) modules ; \
	fi
	# Preserve the debug versions of these and strip for release
	$(call STRIP_DEBUG_SYMBOLS,$(LINUXDIR)/vmlinux)
ifeq ($(LINUXDIR), $(SRCBASE)/linux/linux-2.6)
	$(call STRIP_DEBUG_SYMBOLS,$(LINUXDIR)/drivers/net/wl/wl.ko)
	$(call STRIP_DEBUG_SYMBOLS,$(LINUXDIR)/drivers/net/et/et.ko)
	$(call STRIP_DEBUG_SYMBOLS,$(LINUXDIR)/drivers/net/bcm57xx/bcm57xx.ko)
	$(call STRIP_DEBUG_SYMBOLS,$(LINUXDIR)/drivers/net/emf/emf.ko)
	$(call STRIP_DEBUG_SYMBOLS,$(LINUXDIR)/drivers/net/igs/igs.ko)
	$(call STRIP_DEBUG_SYMBOLS,$(LINUXDIR)/drivers/net/ctf/ctf.ko)
else
	$(call STRIP_DEBUG_SYMBOLS,$(LINUXDIR)/drivers/net/wl/wl.o)
	$(call STRIP_DEBUG_SYMBOLS,$(LINUXDIR)/drivers/net/et/et.o)
	$(call STRIP_DEBUG_SYMBOLS,$(LINUXDIR)/drivers/net/bcm57xx/bcm57xx.o)
	$(call STRIP_DEBUG_SYMBOLS,$(LINUXDIR)/drivers/net/emf/emf.o)
	$(call STRIP_DEBUG_SYMBOLS,$(LINUXDIR)/drivers/net/igs/igs.o)
endif


# well, we should always be able to use the BOM, but right now, the last build step on
# the build machine doesn't have it, so we don't rerun this is the file already 
# exists

version:  $(SRCBASE)/router/shared/router_version.h

# this could fail if the bom doesn't exist. We don't care as long as there is a valid router_version.h
# if not, the build will fail anyway.
$(SRCBASE)/router/shared/router_version.h: $(SRCBASE)/router/shared/version.h.in
	[ ! -e $(SRCBASE)/tools/release/linux-router-bom.mk  ] ||  make SRCBASE=$(SRCBASE) -f $(SRCBASE)/tools/release/linux-router-bom.mk version



router-clean: $(obj-clean) config-clean
	rm -rf $(TARGETDIR)
	rm -f $(PLATFORMDIR)/linux.trx $(PLATFORMDIR)/vmlinuz $(PLATFORMDIR)/target.cramfs
	rm -rf $(INSTALLDIR)/busybox

clean: router-clean
	@echo cleaning LINUXDIR = $(LINUXDIR)
ifeq ($(LINUXDIR), $(SRCBASE)/linux/linux-2.6)
	# we need to pass some conf file for cleaning 2.6. The kbuild clean doesn't seem to 
	# to load the .config the way our wl Makefile  is expecting.
	$(MAKE) CONFIG_WL_CONF=wlconfig_lx_router_ap -C $(LINUXDIR) clean
	$(MAKE) -C $(SRCBASE)/router/compressed clean
else
	$(MAKE) -C $(LINUXDIR) clean
endif
	#$(MAKE) -C $(SRCBASE)/cfe/build/broadcom/bcm947xx clean
	cp config.asus/router.config.asus.n12 .config.asus
	rm -rf $(SRCBASE)/router/n10
	rm -rf $(SRCBASE)/router/n12
	rm -rf $(SRCBASE)/router/n16
	# $(MAKE) -C $(SRCBASE)/tools/misc clean

distclean mrproper: clean
	rm -f .config $(LINUXDIR)/.config

install package: $(filter-out lib-install,$(obj-install)) $(LINUXDIR)/.config
        # Install binaries into target directory
	install -d $(TARGETDIR)
	for dir in $(wildcard $(patsubst %,$(INSTALLDIR)/%,$(obj-y))) ; do \
	    (cd $${dir} && tar cpf - .) | (cd $(TARGETDIR) && tar xpf -) \
	done
	# optimize the crypto library by removing unneeded symbols
	[ ! -d libbcmcrypto ] || $(MAKE) -C libbcmcrypto optimize
        # Install (and possibly optimize) C library
	$(MAKE) lib-install
        # Install modules into filesystem
	if grep -q "CONFIG_MODULES=y" $(LINUXDIR)/.config ; then \
	    $(MAKE) -C $(LINUXDIR) modules_install DEPMOD=/bin/true INSTALL_MOD_PATH=$(TARGETDIR) ; \
	fi
ifeq ($(LINUXDIR), $(SRCBASE)/linux/linux-2.6)
	find $(TARGETDIR) -name "wl_*.ko"  | xargs rm -rf
else
	find $(TARGETDIR) -name "wl_*.o"  | xargs rm -rf
endif
        # Prepare filesystem
	cd $(TARGETDIR) && $(TOP)/misc/rootprep.sh
ifeq ($(LINUXDIR), $(SRCBASE)/linux/linux-2.6)
	# Make sure mkcramfs-2.0 is used
	# $(MAKE) -C cramfs mkcramfs
	# '-u 0 -g 0' will set the uid and gid of all the files to 0 (root)
	# These options are currently only available on our version of mkcramfs
	# cramfs/mkcramfs -u 0 -g 0 $(TARGETDIR) $(PLATFORMDIR)/target.cramfs
	# Package kernel and filesystem
	$(MAKE) -C squashfs mksquashfs
	#squashfs/mksquashfs $(TARGETDIR) $(PLATFORMDIR)/target.squashfs -noappend -all-root
	#for i in $(shell ls $(INSTALLDIR)/utils/usr/sbin) ; do \
	#	rm -f $(TARGETDIR)/usr/sbin/$${i} ; \
	#done
	#squashfs/mksquashfs $(TARGETDIR) $(PLATFORMDIR)/target.squashfs.trim -noappend -all-root
	if grep -q "CONFIG_BLK_DEV_INITRD=y" $(LINUXDIR)/.config ; then \
	    cd $(TARGETDIR) ; \
	    find . | cpio -o -H newc | gzip > $(LINUXDIR)/usr/initramfs_data.cpio.gz ; \
	    ls -l $(LINUXDIR)/usr/initramfs_data.cpio.gz ; \
	    $(MAKE) -C $(LINUXDIR) zImage ; \
	    $(MAKE) -C $(SRCBASE)/router/compressed ; \
	else \
	    cp $(SRCBASE)/router/compressed/vmlinuz $(PLATFORMDIR)/ -rf; \
	    cp $(SRCBASE)/router/compressed/vmlinuz-lzma $(PLATFORMDIR)/vmlinuz-lzma -rf; \
	fi
	# Pad self-booting Linux to a 64 KB boundary
	#cp $(SRCBASE)/router/compressed/zImage $(PLATFORMDIR)/
else
	# Make sure mkcramfs-2.0 is used
	# $(MAKE) -C $(LINUXDIR)/scripts/cramfs mkcramfs
	# '-u 0 -g 0' will set the uid and gid of all the files to 0 (root)
	# These options are currently only available on our version of mkcramfs
	# $(LINUXDIR)/scripts/cramfs/mkcramfs -u 0 -g 0 $(TARGETDIR) $(PLATFORMDIR)/target.cramfs
	# Package kernel and filesystem
	$(MAKE) -C squashfs mksquashfs
	squashfs/mksquashfs $(TARGETDIR) $(PLATFORMDIR)/$(ROOT_IMG) -noappend -all-root
	for i in $(shell ls $(INSTALLDIR)/utils/usr/sbin) ; do \
		rm -f $(TARGETDIR)/usr/sbin/$${i} ; \
	done
	squashfs/mksquashfs $(TARGETDIR) $(PLATFORMDIR)/$(ROOT_IMG).trim -noappend -all-root

	if grep -q "CONFIG_EMBEDDED_RAMDISK=y" $(LINUXDIR)/.config ; then \
	    cp $(PLATFORMDIR)/target.cramfs $(LINUXDIR)/arch/mips/ramdisk/$${CONFIG_EMBEDDED_RAMDISK_IMAGE} ; \
	    $(MAKE) -C $(LINUXDIR) zImage ; \
	else \
	    cp $(LINUXDIR)/arch/mips/brcm-boards/bcm947xx/compressed/vmlinuz $(PLATFORMDIR)/ ; \
	    trx -o $(PLATFORMDIR)/linux.trx $(PLATFORMDIR)/vmlinuz $(PLATFORMDIR)/target.cramfs ; \
	    addpattern -i $(PLATFORMDIR)/linux.trx -o $(PLATFORMDIR)/linux_lsys.bin ; \
	fi
	# Pad self-booting Linux to a 64 KB boundary
	cp $(LINUXDIR)/arch/mips/brcm-boards/bcm947xx/compressed/zImage $(PLATFORMDIR)/
endif
	dd conv=sync bs=64k < $(PLATFORMDIR)/vmlinuz > $(PLATFORMDIR)/linux.bin
	# Append filesystem to self-booting Linux
#	cat $(PLATFORMDIR)/target.cramfs >> $(PLATFORMDIR)/linux.bin

#
# Configuration rules
#

conf mconf:
	$(MAKE) -C config
	@./config/$@ ./config/Config
	# Also configure kernel
	$(MAKE) k$@

oldconf: .config
	$(MAKE) -C config
	@./config/conf -o ./config/Config
	# Also configure kernel
	$(MAKE) k$@

kconf:
	$(MAKE) -C $(LINUXDIR) config

kmconf: $(LINUXDIR)/.config 
	$(MAKE) -C $(LINUXDIR) menuconfig

koldconf: $(LINUXDIR)/.config
	$(MAKE) -C $(LINUXDIR) oldconfig

# Convenience
config: conf

menuconfig: mconf

oldconfig: oldconf

# Default configurations
ifneq ("$(LINUX_VERSION)","2_6")
.config:
	cp config/defconfig $@
	$(MAKE) oldconfig
else
.config:
	cp config/defconfig-2.6 $@
	$(MAKE) oldconfig
endif

ifneq ("$(CONFIG_USBAP)","y")
ifneq ("$(LINUX_VERSION)","2_6")
$(LINUXDIR)/.config:
	cp $(LINUXDIR)/arch/mips/defconfig-bcm947xx $@
else
$(LINUXDIR)/.config:
	cp $(LINUXDIR)/arch/mips/defconfig-bcm947xx-$(if $(CPU),$(CPU)-,)slimrtr $@
endif
else
ifneq ("LINUX_VERSION","2_6")
$(LINUXDIR)/.config:
	cp $(LINUXDIR)/arch/mips/defconfig-bcm947xx-usbap $@
else
$(LINUXDIR)/.config:
	cp $(LINUXDIR)/arch/mips/defconfig-bcm947xx-slimrtr-usbap $@
endif
endif

#
# Overrides
#
# 2007.10 James. add libdisk.

shared:
	$(MAKE) -C $@ install INSTALLDIR=$(INSTALLDIR)/$@

libdisk: nvram shared
	$(MAKE) -C $@ install INSTALLDIR=$(INSTALLDIR)/$@

libdisk-install: libdisk

ifeq ($(LINUXDIR), $(SRCBASE)/linux/linux-2.6)
busybox-1.x/Config.h: dummy
	#cd busybox-1.x && rm -f Config.h && ln -sf include/autoconf.h Config.h && touch Config.h
	#cd busybox-1.x && cp configs/bbconfig-$(CONFIG_BUSYBOX_CONFIG) .config && make clean

busybox: busybox-1.x/Config.h
	$(MAKE) -C busybox-1.x CROSS_COMPILE=mipsel-uclibc- ARCH=mips INSTALL

busybox-install:
	$(MAKE) -C busybox-1.x CROSS_COMPILE=mipsel-uclibc- ARCH=mips CONFIG_PREFIX=$(INSTALLDIR)/busybox install

rc: netconf nvram shared
	make LINUXDIR=$(LINUXDIR)  -C rc
ifneq ($(CONFIG_BUSYBOX),)
rc: busybox-1.x/Config.h
endif
else #linux-2.6
CURBBCFG=$(CONFIG_BUSYBOX_CONFIG).h
OLDBBCFG=$(shell basename $$(readlink busybox/Config.h) 2> /dev/null)
busybox/Config.h: dummy
ifneq ($(OLDBBCFG),$(CURBBCFG))
	cd busybox && rm -f Config.h && ln -sf configs/$(CURBBCFG) Config.h && touch Config.h
endif

busybox: busybox/Config.h
	$(MAKE) -C busybox STRIPTOOL=$(STRIP)

busybox-install:
	$(MAKE) -C busybox STRIPTOOL=$(STRIP) PREFIX=$(INSTALLDIR)/busybox install

rc: netconf nvram shared
ifneq ($(CONFIG_BUSYBOX),)
rc: busybox/Config.h
endif
endif #linux-2.6

rc-install:
	make LINUXDIR=$(LINUXDIR) INSTALLDIR=$(INSTALLDIR)/rc -C rc install

lib-install:
	make LX_VERS=$(LINUX_VERSION) INSTALLDIR=$(INSTALLDIR)/lib -C lib install

www www-%:
	$(MAKE) -C www/$(CONFIG_VENDOR) $* INSTALLDIR=$(INSTALLDIR)/www



# To re-build bcmdl target, uncomment and exchange libusb and  bcmdl targets
# libusb :
#	cd $(SRCBASE)/usbdev/libusb; ./configure  --host=mipsel-linux CC=mipsel-uclibc-gcc; make


# bcmdl :
#	make TARGETENV=linuxmips -C $(SRCBASE)/usbdev/usbdl

# bcmdl-install :
#	install -d $(TARGETDIR)/bin
#	install -D $(SRCBASE)/usbdev/usbdl/bcmdl $(TARGETDIR)/bin/bcmdl

#libusb :
libusb-config:
	cd $(TOP)/libusb; ./configure CC=$(CC) --host=mipsel-linux RANLIB=$(RANLIB) AR=$(AR) STRIP=$(STRIP) CXX=$(CROSS_COMPILE)g++ NM=$(NM) prefix=$(INSTALLDIR)/libusb

rp-l2tp-config:
	cd $(TOP)/rp-l2tp; ./configure CC=$(CC) --host=mipsel-linux RANLIB=$(RANLIB) AR=$(AR) STRIP=$(STRIP) CXX=$(CROSS_COMPILE)g++ NM=$(NM)


bcmdl :

bcmdl-install :	
	install -d $(INSTALLDIR)/dongle/sbin
	install -D $(SRCBASE)/usbdev/usbdl/mips_$(LINUX_VERSION)/bcmdl $(INSTALLDIR)/dongle/sbin/bcmdl

bridge:
ifeq ($(LINUXDIR), $(SRCBASE)/linux/linux-2.6)
	$(MAKE) -C bridge-1.x
else
	$(MAKE) -C bridge brctl/brctl
endif

dongle :

bridge-install: bridge
ifeq ($(LINUXDIR), $(SRCBASE)/linux/linux-2.6)
	install -D bridge-1.x/brctl/brctl $(INSTALLDIR)/bridge/usr/sbin/brctl
else
	install -D bridge/brctl/brctl $(INSTALLDIR)/bridge/usr/sbin/brctl
endif
	$(STRIP) $(INSTALLDIR)/bridge/usr/sbin/brctl

dnsmasq-install: dnsmasq
	install -D dnsmasq/dnsmasq $(INSTALLDIR)/dnsmasq/usr/sbin/dnsmasq
	$(STRIP) $(INSTALLDIR)/dnsmasq/usr/sbin/dnsmasq

ifeq ($(LINUXDIR), $(SRCBASE)/linux/linux-2.6)
iptables:
	$(MAKE) -C iptables-1.x BINDIR=/usr/sbin LIBDIR=/usr/lib KERNEL_DIR=$(LINUXDIR)

iptables-install: iptables
ifeq ($(CONFIG_IPTABLES),y)
	install -d $(INSTALLDIR)/iptables/usr/lib/iptables
	install iptables-1.x/extensions/*.so $(INSTALLDIR)/iptables/usr/lib/iptables
	$(STRIP) $(INSTALLDIR)/iptables/usr/lib/iptables/*.so
	install -D iptables-1.x/iptables $(INSTALLDIR)/iptables/usr/sbin/iptables
	$(STRIP) $(INSTALLDIR)/iptables/usr/sbin/iptables
	install -D iptables-1.x/iptables-restore $(INSTALLDIR)/iptables/usr/sbin/iptables-restore
	$(STRIP) $(INSTALLDIR)/iptables/usr/sbin/iptables-restore
else
        # So that generic rule does not take precedence
	@true
endif
iptables-clean:
	-$(MAKE) -C iptables-1.x KERNEL_DIR=$(LINUXDIR) clean
else # linux-2.6
iptables:
	$(MAKE) -C iptables BINDIR=/usr/sbin LIBDIR=/usr/lib KERNEL_DIR=$(LINUXDIR)

iptables-install:
ifeq ($(CONFIG_IPTABLES),y)
	install -d $(INSTALLDIR)/iptables/usr/lib/iptables
	install iptables/extensions/*.so $(INSTALLDIR)/iptables/usr/lib/iptables
	$(STRIP) $(INSTALLDIR)/iptables/usr/lib/iptables/*.so
	install -D iptables/iptables $(INSTALLDIR)/iptables/usr/sbin/iptables
	$(STRIP) $(INSTALLDIR)/iptables/usr/sbin/iptables
else
        # So that generic rule does not take precedence
	@true
endif
iptables-clean:
	-$(MAKE) -C iptables KERNEL_DIR=$(LINUXDIR) clean
endif # linux-2.6


netconf: iptables
	make LINUXDIR=$(LINUXDIR) -C netconf
#ntpclient-install:
#	install -D ntpclient/ntpclient $(INSTALLDIR)/ntpclient/usr/sbin/ntpclient
#	$(STRIP) $(INSTALLDIR)/ntpclient/usr/sbin/ntpclient

ppp ppp-%:
	$(MAKE) -C ppp/pppoecd $* INSTALLDIR=$(INSTALLDIR)/ppp

upnp: netconf nvram shared

bcmupnp: netconf nvram shared

wlconf: nvram shared

vlan:
	$(MAKE) -C vlan CROSS=$(CROSS_COMPILE) STRIPTOOL=$(STRIP)

vlan-install:
	$(MAKE) -C vlan CROSS=$(CROSS_COMPILE) STRIPTOOL=$(STRIP) INSTALLDIR=$(INSTALLDIR) install

vlan-clean:
	$(MAKE) -C vlan clean


emf:
	$(MAKE) -C emf/emfconf CROSS=$(CROSS_COMPILE)

emf-install: emf
ifeq ($(CONFIG_EMF),y)
	install -d $(TARGETDIR)
	$(MAKE) -C emf/emfconf CROSS=$(CROSS_COMPILE) INSTALLDIR=$(INSTALLDIR) install
endif

emf-clean:
	$(MAKE) -C emf/emfconf clean

igs:
	$(MAKE) -C emf/igsconf CROSS=$(CROSS_COMPILE)

igs-install: igs
ifeq ($(CONFIG_EMF),y)
	install -d $(TARGETDIR)
	$(MAKE) -C emf/igsconf CROSS=$(CROSS_COMPILE) INSTALLDIR=$(INSTALLDIR) install
endif

igs-clean:
	$(MAKE) -C emf/igsconf clean
igmp:
	$(MAKE) -C igmp CROSS=$(CROSS_COMPILE)

igmp-install: igmp
ifeq ($(CONFIG_IGMP_PROXY),y)
	install -d $(TARGETDIR)
	$(MAKE) -C igmp CROSS=$(CROSS_COMPILE) INSTALLDIR=$(INSTALLDIR) install
endif

igmp-clean:
	$(MAKE) -C igmp clean

wps: nvram shared
ifeq ($(CONFIG_WSCCMD),y)
	[ ! -f wps/Makefile ] || $(MAKE) -C wps
else
        # Prevent to use generic rules"
	@true
endif

wps-install:
ifeq ($(CONFIG_WSCCMD),y)
	[ ! -f wps/Makefile ] || $(MAKE) -C wps install INSTALLDIR=$(INSTALLDIR)/wps
else
        # Prevent to use generic rules"
	@true
endif

wps-clean:
ifeq ($(CONFIG_WSCCMD),y)
	[ ! -f wps/Makefile ] || $(MAKE) -C wps clean
else
        # Prevent to use generic rules"
	@true
endif


ifeq ($(LINUXDIR), $(SRCBASE)/linux/linux-2.6)
udev:
	$(MAKE) -C udev CROSS_COMPILE=$(CROSS_COMPILE)

udev-install: udev
	install -d $(TARGETDIR)
	$(MAKE) -C udev CROSS_COMPILE=$(CROSS_COMPILE) DESTDIR=$(INSTALLDIR) prefix=/udev install-udevtrigger

udev-clean:
	$(MAKE) -C udev clean

hotplug2:
	$(MAKE) -C hotplug2 CROSS_COMPILE=$(CROSS_COMPILE)

hotplug2-install: hotplug2
	install -d $(TARGETDIR)
	install -d $(INSTALLDIR)/hotplug2
	$(MAKE) -C hotplug2 CROSS_COMPILE=$(CROSS_COMPILE) PREFIX=$(INSTALLDIR) install

hotplug2-clean:
	$(MAKE) -C hotplug2 clean
endif


# ASUS's Applications

dproxy-install:  dproxy
	install -D dproxy/dproxy $(INSTALLDIR)/dproxy/usr/sbin/dproxy
	$(STRIP) $(INSTALLDIR)/dproxy/usr/sbin/dproxy

udhcpd-install: udhcpd
	install -D udhcpd/udhcpd $(INSTALLDIR)/udhcpd/usr/sbin/udhcpd
	$(STRIP) $(INSTALLDIR)/udhcpd/usr/sbin/udhcpd
	cd $(INSTALLDIR)/udhcpd/usr/sbin && ln -sf udhcpd udhcpc

pptp-client-install: pptp-client
	install -D pptp-client/pptp $(INSTALLDIR)/pptp-client/usr/sbin/pptp
	$(STRIP) $(INSTALLDIR)/pptp-client/usr/sbin/pptp

pppd:
	$(MAKE) -C pppd/pppd
	$(MAKE) -C pppd/pppd/plugins


pppd-install: pppd
	$(STRIP) pppd/pppd/pppd
	install -D pppd/pppd/pppd $(INSTALLDIR)/pppd/usr/sbin/pppd
#	install -D pppd/chat/chat $(INSTALLDIR)/pppd/usr/sbin/chat
#	$(STRIP) $(INSTALLDIR)/pppd/usr/sbin/chat
	$(STRIP) pppd/pppd/plugins/rp-pppoe/rp-pppoe.so
	install -D pppd/pppd/plugins/rp-pppoe/rp-pppoe.so $(INSTALLDIR)/pppd/usr/lib/pppd/rp-pppoe.so
	$(STRIP) pppd/pppd/plugins/pppol2tp.so
	install -D pppd/pppd/plugins/pppol2tp.so $(INSTALLDIR)/pppd/usr/lib/pppd/pppol2tp.so
	$(STRIP) pppd/pppd/plugins/pptp/pptp.so
	install -D pppd/pppd/plugins/pptp/pptp.so $(INSTALLDIR)/pppd/usr/lib/pppd/pptp.so	
	

pppd-clean:
	cd pppd && ln -sf linux/Makefile.top Makefile && \
	cd chat && ln -sf Makefile.linux Makefile && cd .. && \
	cd pppd && ln -sf Makefile.linux Makefile && cd plugins && ln -sf Makefile.linux Makefile && cd radius && ln -sf Makefile.linux Makefile && cd ../rp-pppoe && ln -sf Makefile.linux Makefile && cd ../../.. && \
	cd pppdump && ln -sf Makefile.linux Makefile && cd .. && \
	cd pppstats && ln -sf Makefile.linux Makefile && cd .. && \
	cd ..
	$(MAKE) -C pppd/pppd clean
	$(MAKE) -C pppd/pppd/plugins clean

samba: acl attr samba-configure

samba-configure:
	$(MAKE) -C samba configure

#samba-install: samba 


acl: attr

rp-l2tp-install: rp-l2tp
	install -D rp-l2tp/l2tpd $(INSTALLDIR)/rp-l2tp/usr/sbin/l2tpd
	install -D rp-l2tp/handlers/l2tp-control $(INSTALLDIR)/rp-l2tp/usr/sbin/l2tp-control
	install -d $(INSTALLDIR)/rp-l2tp/usr/lib/l2tp/plugins
	install -D rp-l2tp/handlers/*.so $(INSTALLDIR)/rp-l2tp/usr/lib/l2tp/plugins

igmpproxy: 
	$(MAKE) -C igmpproxy/src

igmpproxy-install: igmpproxy
	install -D igmpproxy/src/igmpproxy $(INSTALLDIR)/igmpproxy/usr/sbin/igmpproxy
        
igmpproxy-clean:
	$(MAKE) -C igmpproxy/src clean

utelnetd-install: utelnetd
	install -D utelnetd/utelnetd $(INSTALLDIR)/utelnetd/usr/sbin/utelnetd

N16:
	make image-RT-N16 LANGUAGE=TW 
N12:
	make image-RT-N12 LANGUAGE=TW
N10:
	make image-RT-N10 LANGUAGE=TW
N1X: N12 N10

image-%:
ifeq ($(CONFIG_MULTICONFIG),y)
	( if [ "$*" = "RT-N16" ]; then \
		( if [ -f ./n16 ]; then \
			echo "the same configuration"; \
		else \
			make clean; \
			cp -f ./config.asus/linux.config.n16 ../linux/linux-2.6/.config; \
			cd $(LINUXDIR) && make dep && make modules && cd $(TOP); \
			cp -f ./config.asus/busybox.config.n16 ./busybox-1.x/.config; \
			cp -f ./config.asus/router.config.n16 ./.config; \
			cp -f ./config.asus/router.config.asus.n16 ./.config.asus; \
			cp -f ./config.asus/bcm_upnp.n16 ./bcmupnp/device/InternetGatewayDevice/xml_InternetGatewayDevice.c; \
			make menuconfig; \
			make samba-configure; \
			make netconf; \
			make all; \
			rm -f n16; \
			touch $(TOP)/n16; \
		fi ) \
	elif [ "$*" = "RT-N12" ]; then \
		( if [ -f ./n12 ]; then \
			echo "the same configuration"; \
		elif [ -f ./n10 ]; then \
			echo "switch N12"; \
			cp -f ./config.asus/linux.config.n12 ../linux/linux-2.6/.config; \
			make compressed-clean; \
			rm -f n10; \
			touch $(TOP)/n12; \
		else \
			make clean; \
			cp -f ./config.asus/linux.config.n12 ../linux/linux-2.6/.config; \
			cd $(LINUXDIR) && make dep && make modules && cd $(TOP); \
			cp -f ./config.asus/busybox.config.n12 ./busybox-1.x/.configs; \
			cp -f ./config.asus/router.config.n12 ./.config; \
			cp -f ./config.asus/router.config.asus.n12 ./.config.asus; \
			cp -f ./config.asus/bcm_upnp.n12 ./bcmupnp/device/InternetGatewayDevice/xml_InternetGatewayDevice.c; \
			make menuconfig; \
			make netconf; \
			make all; \
			rm -f n12; \
			touch $(TOP)/n12; \
		fi ) \
	elif [ "$*" = "RT-N10" ]; then \
		( if [ -f ./n10 ]; then \
			echo "the same configuration"; \
		elif [ -f ./n12 ]; then \
			echo "switch N10"; \
			cp -f ./config.asus/linux.config.n10 ../linux/linux-2.6/.config; \
			make compressed-clean; \
			rm -f n12; \
			touch $(TOP)/n10; \
		else \
			make clean; \
			cp -f ./config.asus/linux.config.n10 ../linux/linux-2.6/.config; \
			cd $(LINUXDIR) && make dep && make modules && cd $(TOP); \
			cp -f ./config.asus/busybox.config.n12 ./busybox-1.x/.configs; \
			cp -f ./config.asus/router.config.n12 ./.config; \
			cp -f ./config.asus/router.config.asus.n12 ./.config.asus; \
			make menuconfig; \
			make netconf; \
			make all; \
			rm -f n10; \
			touch $(TOP)/n10; \
		fi ) \
	else \
		( if [ -f ./full ]; then \
			echo "the same configuration"; \
		else \
			cp -f ./config.asus/linux.config.full ../linux/linux/.config; \
			cp -f ./config.asus/router.config.full ./.config.asus; \
			cp -f ./config.asus/busybox.router.h.full ./busybox/configs/router.h; \
			cd ../linux/linux && make dep && cd ../../router; \
			make menuconfig; \
			cp -f ./config.asus/router.config.full ./.config.asus; \
			make clean; \
			make netconf; \
			make all; \
			touch $(TOP)/full; \
		fi ) \
	fi )
endif

	mkdir -p $(TARGETDIR) && cd $(TARGETDIR) && $(TOP)/misc/rootprep.sh

ifeq ($(LINUXDIR), $(SRCBASE)/linux/linux-2.6)
	if ! grep -q "CONFIG_BLK_DEV_INITRD=y" $(LINUXDIR)/.config ; then \
		$(MAKE) -C $(LINUXDIR) zImage ; \
		$(MAKE) -C $(SRCBASE)/router/compressed ; \
	fi
else
	if ! grep -q "CONFIG_EMBEDDED_RAMDISK=y" $(LINUXDIR)/.config ; then \
		$(MAKE) -C $(LINUXDIR) zImage ; \
	fi
endif
	if grep -q "CONFIG_MODULES=y" $(LINUXDIR)/.config ; then \
		$(MAKE) -C $(LINUXDIR) modules ; \
	fi
	$(call STRIP_DEBUG_SYMBOLS,$(LINUXDIR)/vmlinux)
	make rc-clean
	make infosvr-clean
	make LPRng-clean
	make vlan-clean
	#make shared-clean
	make httpd-clean
ifeq ($(CONFIG_LIBDISK),y)
	make libdisk-clean      # 2007.10 James
endif

	cd $(PLATFORMDIR)/install/nas/usr/sbin && ln -sf nas nas4not && cd $(TOP)

	$(MAKE) -C $(LINUXDIR) zImage
	$(MAKE) -C $(LINUXDIR) modules

	cd library/lib && \
	ln -sf libbackup.so.0 libbackup.so && \
	ln -sf libbackup.so.0.0 libbackup.so.0 && \
	ln -sf libfilesharing.so.0 libfilesharing.so && \
	ln -sf libfilesharing.so.0.0 libfilesharing.so.0 && \
	ln -sf libflash.so.0 libflash.so && \
	ln -sf libflash.so.0.0 libflash.so.0 && \
	ln -sf libhd.so.0 libhd.so && \
	ln -sf libhd.so.0.0 libhd.so.0 && \
	ln -sf libiconv.so.0 libiconv.so && \
	ln -sf libiconv.so.0.0 libiconv.so.0 && \
	ln -sf libnetcgi.so.0 libnetcgi.so && \
	ln -sf libnetcgi.so.0.0 libnetcgi.so.0 && \
	ln -sf libnetwork.so.1 libnetwork.so && \
	ln -sf libnetwork.so.1.0 libnetwork.so.1 && \
	ln -sf libntpass.so.0 libntpass.so && \
	ln -sf libntpass.so.0.0 libntpass.so.0 && \
	ln -sf libprofile.so.0 libprofile.so && \
	ln -sf libprofile.so.0.0 libprofile.so.0 && \
	ln -sf libsamba.so.1 libsamba.so && \
	ln -sf libsamba.so.1.0 libsamba.so.1 && \
	ln -sf libschl.so.0 libschl.so && \
	ln -sf libschl.so.0.0 libschl.so.0 && \
	ln -sf libsystem.so.0 libsystem.so && \
	ln -sf libsystem.so.0.0 libsystem.so.0 && \
	cd $(TOP)
	make netconf-install
ifeq ($(CONFIG_LIBDISK),y)
	make libdisk-install
endif
	make libbcmcrypto-install

	( if [ "$*" = "RT-N16" ] && [ "$(USB_WAN)" = "CDMA" ]; then \
		make shared GLOBAL_OPTIONS="-DDLM" CONFIG_DLM=y; \
		make libdisk-install; \
		make rc GLOBAL_OPTIONS="-DWPA2_WMM -DWOB -DRPPPPOE -DBTN_SETUP -DURE -DUSB_SUPPORT -DWL500GP -DDHCP_PPTP -DWSC -DWCN -DQOS -DDLM -DCDMA" CONFIG_BTNSETUP=y CONFIG_DLM=y; \
		make infosvr GLOBAL_OPTIONS="-DPRNINFO -DNO_PARALLEL"; \
		make LPRng GLOBAL_OPTIONS="-DUSBONLY"; \
		make httpd GLOBAL_OPTIONS="-DTRANSLATE_ON_FLY -DDLM -DCDMA" CONFIG_NOUSB=n MODEL_NAME=WL-500gP\ V2; \
	elif [ "$*" = "RT-N16" ]; then \
		make shared GLOBAL_OPTIONS="-DDLM -DWEBSTRFILTER" CONFIG_DLM=y; \
		make libdisk-install; \
		make rc GLOBAL_OPTIONS="-DWPA2_WMM -DWOB -DRPPPPOE -DBTN_SETUP -DURE -DUSB_SUPPORT -DWL500GP -DDHCP_PPTP -DWSC -DWCN -DQOS -DWEB_REDIRECT -DDLM -DU2EC -DRTN16" CONFIG_BTNSETUP=y CONFIG_DLM=y; \
		make infosvr GLOBAL_OPTIONS="-DPRNINFO -DNO_PARALLEL"; \
		make LPRng GLOBAL_OPTIONS="-DUSBONLY"; \
		make httpd GLOBAL_OPTIONS="-DWL500GPV2 -DTRANSLATE_ON_FLY -DDLM -DWSC -DU2EC" CONFIG_NOUSB=n MODEL_NAME=RT-N16 CONFIG_DLM=y; \
		make wanduck GLOBAL_OPTIONS="-DWL500GPV2"; \
	elif [ "$*" = "RT-N12" ]; then \
		make shared GLOBAL_OPTIONS="-DDLM" CONFIG_DLM=n; \
		make rc GLOBAL_OPTIONS="-DWPA2_WMM -DWOB -DRPPPPOE -DBTN_SETUP -DURE -DWL500GP -DDHCP_PPTP -DWSC -DWCN -DQOS -DWEB_REDIRECT -DRTN12" CONFIG_BTNSETUP=y CONFIG_DLM=n CONFIG_PBCENR=y; \
		make infosvr GLOBAL_OPTIONS="-DPRNINFO -DNO_PARALLEL"; \
		make LPRng GLOBAL_OPTIONS="-DUSBONLY"; \
		make httpd GLOBAL_OPTIONS="-DWL500GPV2 -DTRANSLATE_ON_FLY -DWSC -DPBCENR" CONFIG_NOUSB=n MODEL_NAME=RT-N12 CONFIG_DLM=n; \
		make wanduck GLOBAL_OPTIONS="-DWL500GPV2 -DRTN12"; \
	elif [ "$*" = "RT-N10" ]; then \
		make shared GLOBAL_OPTIONS="-DDLM" CONFIG_DLM=n; \
		make rc GLOBAL_OPTIONS="-DWPA2_WMM -DWOB -DRPPPPOE -DBTN_SETUP -DURE -DWL500GP -DDHCP_PPTP -DWSC -DWCN -DQOS -DWEB_REDIRECT -DRTN10" CONFIG_BTNSETUP=y CONFIG_DLM=n CONFIG_PBCENR=y; \
		make infosvr GLOBAL_OPTIONS="-DPRNINFO -DNO_PARALLEL"; \
		make LPRng GLOBAL_OPTIONS="-DUSBONLY"; \
		make httpd GLOBAL_OPTIONS="-DWL500GPV2 -DTRANSLATE_ON_FLY -DWSC -DPBCENR" CONFIG_NOUSB=n MODEL_NAME=RT-N10 CONFIG_DLM=n; \
		make wanduck GLOBAL_OPTIONS="-DWL500GPV2 -DRTN12"; \
	else \
		echo "no such model name"; \
	fi )

	 ( if [ "$*" = "RT-N16" ]; then \
		$(MAKE) -C rc install CONFIG_DLM=y INSTALLDIR=$(INSTALLDIR)/rc; \
	else \
		echo "no such model name"; \
	fi )
	
	make install

	( if [ "$*" = "RT-N16" ]; then \
		rm -rf $(PLATFORMDIR)/target/apps; \
		mkdir $(PLATFORMDIR)/target/apps; \
		mkdir $(PLATFORMDIR)/target/apps/bin; \
		make lib-install; \
		cd $(TARGETDIR) && rm -rf shares && ln -s tmp shares; \
		rm $(PLATFORMDIR)/target/lib/modules/2.6.22/parport/* -f; \
		cp $(TOP)/misc/libstdc++.so.6 $(PLATFORMDIR)/target/lib/ -f; \
	fi )

	( if [ "$*" = "RT-N12" ] || [ "$*" = "RT-N10" ]; then \
		rm -rf $(PLATFORMDIR)/target/apps; \
		rm -rf $(PLATFORMDIR)/target/sbin/ip; \
		rm -rf $(PLATFORMDIR)/target/usr/sbin/iptables; \
		rm -rf $(PLATFORMDIR)/target/usr/sbin/usbtest; \
		rm -rf $(PLATFORMDIR)/target/usr/sbin/utelnetd; \
		rm -rf $(PLATFORMDIR)/target/usr/sbin/lpd; \
		rm -rf $(PLATFORMDIR)/target/usr/sbin/upnpnat; \
		rm -rf $(PLATFORMDIR)/target/lib/modules/2.6.22/kernel/fs; \
		rm -rf $(PLATFORMDIR)/target/lib/modules/2.6.22/kernel/drivers/scsi; \
		rm -rf $(PLATFORMDIR)/target/lib/modules/2.6.22/kernel/drivers/usb; \
		rm -rf $(PLATFORMDIR)/target/lib/modules/2.6.22/kernel/drivers/connector; \
		rm -rf $(PLATFORMDIR)/target/lib/modules/2.6.22/kernel/drivers/net/bcm57xx; \
	fi )

	( if [ $(CONFIG_OTHERS) = "n" ]; then \
		cd $(PLATFORMDIR)/target/etc/ && ln -sf /tmp/resolv.conf resolv.conf; cd -; \
		cd $(PLATFORMDIR)/target/etc/ && ln -sf /tmp/hosts hosts; cd -; \
		cp -f $(TOP)/others/services $(PLATFORMDIR)/target/etc/services; \
		echo "order hosts,bind" > $(PLATFORMDIR)/target/etc/host.conf; \
		echo "hosts: files dns" > $(PLATFORMDIR)/target/etc/nsswitch.conf; \
	fi )

	cd $(PLATFORMDIR) && make $* && cd ..
	rm -rf $(TOP)/trx.* 



#
# Generic rules
#

%:
	[ ! -d $* ] || $(MAKE) -C $*

%-clean:
	[ ! -d $* ] || $(MAKE) -C $* clean

%-install:
	[ ! -d $* ] || $(MAKE) -C $* install INSTALLDIR=$(INSTALLDIR)/$*

$(obj-y) $(obj-n) $(obj-clean) $(obj-install): dummy

.PHONY: all clean distclean mrproper install package
.PHONY: conf mconf oldconf kconf kmconf config menuconfig oldconfig
.PHONY: dummy
