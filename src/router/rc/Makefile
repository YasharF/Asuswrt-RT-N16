#
# Router rc control scripts Makefile
#
# Copyright 2004, Broadcom Corporation
# All Rights Reserved.
# 
# THIS SOFTWARE IS OFFERED "AS IS", AND BROADCOM GRANTS NO WARRANTIES OF ANY
# KIND, EXPRESS OR IMPLIED, BY STATUTE, COMMUNICATION OR OTHERWISE. BROADCOM
# SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS
# FOR A SPECIFIC PURPOSE OR NONINFRINGEMENT CONCERNING THIS SOFTWARE.
#
# $Id: Makefile,v 1.12 2008/09/15 07:57:29 james26_jang Exp $
#

include $(TOP)/.config
include $(TOP)/.config.asus

CFLAGS	+= -I. -I$(TOP)/shared -I$(SRCBASE)/include -Wall -DASUS_EXT $(GLOBAL_OPTIONS) -DGPQOS -DQOS #-I$(TOP)/libbcmcrypto

#CFLAGS += -DQOS_DEBUG
#CFLAGS	+= -g -DDEBUG

CFLAGS	+= -s -O2 -DASUS_DDNS -DLINUX26 -DWEBSTRFILTER -I$(LINUXDIR)/include

#LDFLAGS	+= -L$(TOP)/nvram -lnvram -L$(TOP)/shared -lshared# -lcrypt
LDFLAGS	+= -L$(TOP)/nvram -L$(PLATFORMDIR)/install/nvram/usr/lib -lnvram -L$(TOP)/shared -L$(PLATFORMDIR)/install/shared/usr/lib -lshared -lcrypt

ifeq ($(LANGUAGE),EN)
CFLAGS += -DLANGUAGE_EN
endif

ifeq ($(LANGUAGE),TW)
CFLAGS += -DLANGUAGE_TW
endif

ifeq ($(LANGUAGE),CN)
CFLAGS += -DLANGUAGE_CN
endif

ifeq ($(LANGUAGE),ML)
CFLAGS += -DLANGUAGE_CN
endif

ifeq ($(LANGUAGE),KR)
CFLAGS += -DLANGUAGE_KR
endif

ifeq ($(LANGUAGE),JP)
CFLAGS += -DLANGUAGE_JP
endif

OBJS := rc.o init.o interface.o network.o services.o udhcpc.o ppp.o http.o stats.o crc.o mtd.o common_ex.o network_ex.o services_ex.o manu_ex.o http_ex.o watchdog.o udhcpc_ex.o ntp.o qosutils.o qos.o speedtest.o 

ifeq ($(CONFIG_PBCENR),y)
OBJS += pbc_enr_monitor.o monitor.o wlscan.o
CFLAGS += -DPBCENR
endif

ifeq ($(CONFIG_NETCONF),y)
ifeq ($(CONFIG_NOIPTABLES), y)
OBJS += firewall_noipt.o firewall_ex_noipt.o
#LDFLAGS += -L$(TOP)/netconf -lnetconf
LDFLAGS += -L$(TOP)/netconf -L$(PLATFORMDIR)/install/netconf/usr/lib -lnetconf
else
OBJS += firewall.o firewall_ex.o
LDFLAGS += -L$(TOP)/netconf -lnetconf
endif
endif
ifeq ($(CONFIG_DLM), y)
OBJS += rcamd.o waveserver.o sendalarm.o dns.o dns_decode.o
endif

ifeq ($(CONFIG_BTNSETUP),y)
OBJS += btnsetup.o
LDFLAGS += -L$(TOP)/libbcmcrypto -lbcmcrypto
CFLAGS += -I$(TOP)/openssl/include
#LDFLAGS += -L$(TOP)/openssl/lib -lcrypto
endif

OBJS += ILibParsers.o

vpath %.c $(TOP)/shared $(SRCBASE)/rts/src $(SRCBASE)/shared

all: rc

clean:
	rm -f *.o rc

install: all
	install -d $(INSTALLDIR)/sbin
	install rc $(INSTALLDIR)/sbin
	cd $(INSTALLDIR)/sbin && ln -sf rc init
	cd $(INSTALLDIR)/sbin && ln -sf rc preinit
	cd $(INSTALLDIR)/sbin && ln -sf rc erase
	cd $(INSTALLDIR)/sbin && ln -sf rc write
	cd $(INSTALLDIR)/sbin && ln -sf rc stats
	cd $(INSTALLDIR)/sbin && ln -sf rc hotplug

	# Added by ASUS
	cd $(INSTALLDIR)/sbin && ln -sf rc stopservice
	cd $(INSTALLDIR)/sbin && ln -sf rc ddns_updated
	cd $(INSTALLDIR)/sbin && ln -sf rc watchdog
	cd $(INSTALLDIR)/sbin && ln -sf rc wsrom
	cd $(INSTALLDIR)/sbin && ln -sf rc rsrom
	cd $(INSTALLDIR)/sbin && ln -sf rc wmac
	cd $(INSTALLDIR)/sbin && ln -sf rc ntp	
	cd $(INSTALLDIR)/sbin && ln -sf rc restore
	cd $(INSTALLDIR)/sbin && ln -sf rc start_ddns
	cd $(INSTALLDIR)/sbin && ln -sf rc wlan_update
	cd $(INSTALLDIR)/sbin && ln -sf rc ots
	cd $(INSTALLDIR)/sbin && ln -sf rc gpio
	cd $(INSTALLDIR)/sbin && ln -sf rc radioctrl
ifeq ($(CONFIG_DLM), y)
	cd $(INSTALLDIR)/sbin && ln -sf rc sendalarm
	cd $(INSTALLDIR)/sbin && ln -sf rc rmwebcam
	cd $(INSTALLDIR)/sbin && ln -sf rc rmstorage
	cd $(INSTALLDIR)/sbin && ln -sf rc rcamdmain
	cd $(INSTALLDIR)/sbin && ln -sf rc waveservermain
	cd $(INSTALLDIR)/sbin && ln -sf rc write_disc_status
	cd $(INSTALLDIR)/sbin && ln -sf rc hotplug_usb_mass
	cd $(INSTALLDIR)/sbin && ln -sf rc run_apps
	cd $(INSTALLDIR)/sbin && ln -sf rc run_ftpsamba
	cd $(INSTALLDIR)/sbin && ln -sf rc eject_usb1
	cd $(INSTALLDIR)/sbin && ln -sf rc eject_usb2
# 2007.10 James {
	cd $(INSTALLDIR)/sbin && ln -sf rc speedtest
	cd $(INSTALLDIR)/sbin && ln -sf rc convert_asus_values
	cd $(INSTALLDIR)/sbin && ln -sf rc run_dms
	cd $(INSTALLDIR)/sbin && ln -sf rc run_samba
	cd $(INSTALLDIR)/sbin && ln -sf rc run_ftp
	cd $(INSTALLDIR)/sbin && ln -sf rc stop_dms
	cd $(INSTALLDIR)/sbin && ln -sf rc stop_samba
	cd $(INSTALLDIR)/sbin && ln -sf rc stop_ftp
	cd $(INSTALLDIR)/sbin && ln -sf rc stop_ftpsamba
	cd $(INSTALLDIR)/sbin && ln -sf rc start_dns
	cd $(INSTALLDIR)/sbin && ln -sf rc restart_qos
	cd $(INSTALLDIR)/sbin && ln -sf rc start_wps_proc
	cd $(INSTALLDIR)/sbin && ln -sf rc stop_wps_proc
	cd $(INSTALLDIR)/sbin && ln -sf rc refresh_upnp
endif
#ifeq ($(CONFIG_PBCENR),y)
	cd $(INSTALLDIR)/sbin && ln -sf rc wlscan
	cd $(INSTALLDIR)/sbin && ln -sf rc ure_monitor
	cd $(INSTALLDIR)/sbin && ln -sf rc dhcpc_apply_delayed
	cd $(INSTALLDIR)/sbin && ln -sf rc pbc_enr_monitor
#endif


rc: $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)
	$(STRIP) $@
