#Linux Makefile

include $(LINUXDIR)/.config
include $(TOP)/.config

BLDTYPE = release
#BLDTYPE = debug

export CFLAGS += -DBCMUPNP

ifeq ($(BLDTYPE),debug)
export CXXFLAGS += -Wall -D_REENTRANT -D_TUDEBUGTRACE -D__linux__ -DWPS_MODE_AP
export CFLAGS += -Os -Wall -Wnested-externs  -D_REENTRANT -D_TUDEBUGTRACE -D__linux__ 
else
export CXXFLAGS += -Os -Wall -D_REENTRANT -D__linux__ -DWPS_MODE_AP
export CFLAGS += -O2 -Wall -Wnested-externs -D_REENTRANT -D__linux__
endif



export LIBPATH  = -L$(TOP)/libbcmcrypto -L$(INSTALLDIR)/libbcmcrypto/usr/lib
export LIBPATH += -L$(TOP)/libbcm -L$(INSTALLDIR)/libbcm/usr/lib
export LIBPATH += -L$(TOP)/shared -L$(INSTALLDIR)/shared/usr/lib
export LIBPATH += -L$(TOP)/nvram -L$(INSTALLDIR)/nvram/usr/lib
export LIBPATH += -L$(SRCBASE)/router/wps/lib -L$(TOOLCHAIN)/lib
export LIBPATH += -L$(SRCBASE)/router/wps/prebuilt
export LIBPATH += -L$(TOP)/netconf -L$(PLATFORMDIR)/install/netconf/usr/lib -lnetconf


export LDFLAGS = -r

BRCMBASE = ../..

WPSSRC = $(SRCBASE)/wps
WPSCOMSRC = $(WPSSRC)/common
WPSAPPSSRC = $(WPSSRC)/brcm_apps
OBJDIR = obj

export INCLUDE = -I$(TOOLCHAIN)/include -I$(SRCBASE)/include -I$(SRCBASE)/bcmcrypto/opensslinc -I$(SRCBASE)/bcmcrypto \
		 -I$(SRCBASE)/router/libbcmcrypto -I$(SRCBASE)/router/shared -I$(BRCMBASE)/wps/common/include -I$(BRCMBASE)/wps/brcm_apps/include \
		 -I$(BRCMBASE)/wps/linux/inc -I$(BRCMBASE)/router/libbcm -I$(SRCBASE)/router/eapd \
		 -I$(SRCBASE)/router/bcmupnp/include -I$(SRCBASE)/router/bcmupnp/device -I$(SRCBASE)/router/bcmupnp/device/WFADevice

#
# wps library variants
#
WPSM_AP_LIB = wps_ap
WPSM_AP_LIB_FILE = lib/libwps_ap.a
WPSM_STA_LIB = wps_sta
WPSM_STA_LIB_FILE = lib/libwps_sta.a
WPSM_APSTA_LIB = wps_apsta
WPSM_APSTA_LIB_FILE = lib/libwps_apsta.a

WPSM_LIBS = -lnvram -lbcm -lshared -lbcmcrypto -lm -lc


#
# wps common objects
#
WPSM_OBJS = $(OBJDIR)/shared/tutrace.o $(OBJDIR)/shared/dev_config.o \
	$(OBJDIR)/shared/slist.o $(OBJDIR)/enrollee/enr_reg_sm.o \
	$(OBJDIR)/shared/reg_proto_utils.o $(OBJDIR)/shared/reg_proto_msg.o \
	$(OBJDIR)/shared/tlv.o $(OBJDIR)/shared/state_machine.o \
	$(OBJDIR)/shared/buffobj.o

# wps common ap objects
WPSM_AP_OBJS = $(OBJDIR)/ap/ap_api.o $(OBJDIR)/ap/ap_eap_sm.o \
	$(OBJDIR)/ap/ap_upnp_sm.o $(OBJDIR)/registrar/reg_sm.o

# wps common sta enrollee objects
WPSM_STA_OBJS = $(OBJDIR)/enrollee/enr_api.o $(OBJDIR)/sta/sta_eap_sm.o \
	$(OBJDIR)/enrollee/enr_reg_sm.o $(OBJDIR)/registrar/reg_sm.o 


#
# wps monitor objects
#
WPSM_OBJS +=  $(OBJDIR)/application/apps/wps_monitor.o $(OBJDIR)/application/apps/wps_aplockdown.o \
	$(OBJDIR)/application/apps/wps_pb.o $(OBJDIR)/application/apps/wps_led.o \
	$(OBJDIR)/application/apps/wps_eap.o $(OBJDIR)/application/apps/wps_ie.o \
	$(OBJDIR)/application/apps/wps_ui.o $(OBJDIR)/application/arch/bcm947xx/wps_gpio.o \
	$(OBJDIR)/application/arch/bcm947xx/wps_hal.o $(OBJDIR)/application/arch/bcm947xx/wps_wl.o

# wps monitor ap objects
WPSM_AP_OBJS += $(OBJDIR)/application/apps/wps_ap.o $(OBJDIR)/application/apps/wps_upnp.o

# wps monitor sta enrollee objects
WPSM_STA_OBJS += $(OBJDIR)/application/apps/wps_enr_core.o $(OBJDIR)/application/arch/bcm947xx/wps_sta_wl.o

# wps monitor osl objects
WPSM_OSL_OBJS = $(OBJDIR)/application/linux/wps_linux_main.o $(OBJDIR)/application/linux/wps_linux_osl.o
WPSM_STA_OSL_OBJS = $(OBJDIR)/application/linux/wps_enr_linux_osl.o


ifneq ($(CONFIG_WL_USE_STA)$(CONFIG_WL_USE_STA_ONCHIP_G)$(CONFIG_WL_USE_APSTA)$(CONFIG_WL_USE_APSTA_ONCHIP_G),)
  ifneq ($(CONFIG_WL_USE_STA)$(CONFIG_WL_USE_STA_ONCHIP_G),)
    export CXXFLAGS += -DBCMWPSAPSTA
    export CFLAGS += -DBCMWPSAPSTA
    WPSM_LIBS += -l$(WPSM_STA_LIB)
    WPSM_OSL_OBJS += $(WPSM_STA_OSL_OBJS)
  else
    export CXXFLAGS += -DBCMWPSAP -DBCMWPSAPSTA
    export CFLAGS += -DBCMWPSAP -DBCMWPSAPSTA
    WPSM_LIBS += -l$(WPSM_APSTA_LIB)
    WPSM_OSL_OBJS += $(WPSM_STA_OSL_OBJS)
  endif
else
   export CXXFLAGS += -DBCMWPSAP
   export CFLAGS += -DBCMWPSAP
   WPSM_LIBS += -l$(WPSM_AP_LIB)
endif

ifneq ($(wildcard ./prebuilt),)
WPSM_LIB_FILES =
all: dirs wps_monitor
else
WPSM_LIB_FILES = $(WPSM_AP_LIB_FILE) $(WPSM_STA_LIB_FILE) $(WPSM_APSTA_LIB_FILE)
all: mkversion libs wps_monitor
endif

mkversion :
	sh $(SRCBASE)/router/wps/mkversion.sh $(WPSSRC)/common/include/version.h.in $(WPSSRC)/common/include/wps_version.h "$(PROD_TAG)"

# all library variants
libs : dirs $(WPSM_LIB_FILES)

dirs :
	mkdir -p $(OBJDIR)/ap
	mkdir -p $(OBJDIR)/enrollee
	mkdir -p $(OBJDIR)/registrar
	mkdir -p $(OBJDIR)/shared
	mkdir -p $(OBJDIR)/sta
	mkdir -p $(OBJDIR)/bcmcrypto
	mkdir -p $(OBJDIR)/application/apps
	mkdir -p $(OBJDIR)/application/linux
	mkdir -p $(OBJDIR)/application/arch/bcm947xx
	mkdir -p lib

# library rules
$(WPSM_AP_LIB_FILE) : $(WPSM_OBJS) $(WPSM_AP_OBJS)
	$(AR) cr $@ $^

$(WPSM_STA_LIB_FILE) : $(WPSM_OBJS) $(WPSM_STA_OBJS)
	$(AR) cr $@ $^

$(WPSM_APSTA_LIB_FILE) : $(WPSM_OBJS) $(WPSM_AP_OBJS) $(WPSM_STA_OBJS)
	$(AR) cr $@ $^

# target
wps_monitor : $(WPSM_OSL_OBJS) $(WPSM_LIB_FILES)
	$(CC) $(WPSM_OSL_OBJS) $(LIBPATH) $(WPSM_LIBS) -o $@

# object rules
$(OBJDIR)/application/linux/%.o : $(WPSAPPSSRC)/linux/%.c
	$(CC) -c $(CFLAGS) $(INCLUDE) -o $@ $<

$(OBJDIR)/application/apps/%.o : $(WPSAPPSSRC)/apps/%.c
	$(CC) -c $(CFLAGS) $(INCLUDE) -o $@ $<

$(OBJDIR)/application/arch/bcm947xx/%.o : $(WPSAPPSSRC)/arch/bcm947xx/%.c
	$(CC) -c $(CFLAGS) $(INCLUDE) -o $@ $<

$(OBJDIR)/shared/%.o : $(WPSCOMSRC)/shared/%.c
	$(CC) -c $(CFLAGS) $(INCLUDE) $< -o $@

$(OBJDIR)/ap/%.o : $(WPSCOMSRC)/ap/%.c
	$(CC) -c $(CFLAGS) $(INCLUDE) $< -o $@

$(OBJDIR)/enrollee/%.o : $(WPSCOMSRC)/enrollee/%.c
	$(CC) -c $(CFLAGS) $(INCLUDE) $< -o $@

$(OBJDIR)/registrar/%.o : $(WPSCOMSRC)/registrar/%.c
	$(CC) -c $(CFLAGS) $(INCLUDE) $< -o $@

$(OBJDIR)/sta/%.o : $(WPSCOMSRC)/sta/%.c
	$(CC) -c $(CFLAGS) $(INCLUDE) $< -o $@

# clean
clean:
	rm -rf $(OBJDIR)
	rm -rf lib
	find -name wps_monitor | xargs rm -f

# install
install: all
	install -d $(INSTALLDIR)/bin
	install wps_monitor $(INSTALLDIR)/bin
	$(STRIP) $(INSTALLDIR)/bin/wps_monitor

.PHONY: all clean install libs dirs
