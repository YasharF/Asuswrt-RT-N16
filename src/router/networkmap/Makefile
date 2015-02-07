INC_LINUX=$(LINUXDIR)/include
WIRELESS_DRIVER_PATH=$(BRCMDRIVERS_DIR)/broadcom/net/wl/bcm9$(BRCM_CHIP)
INC_WIRELESS_DRIVER_PATH=$(WIRELESS_DRIVER_PATH)/include
TOP=..

CFLAGS  += -I.  -I$(INC_WIRELESS_DRIVER_PATH) -I$(INC_LINUX) -Wall
LDFLAGS += -L$(TOP)/nvram -lnvram 

ifeq ($(strip $(PROFILE)), WL-600g)
	CFLAGS += -DWL_600g
endif
ifeq ($(strip $(PROFILE)), WL-AM604g)
	CFLAGS += -DWL_AM604g
endif
ifeq ($(strip $(PROFILE)), DSL-G31)
	CFLAGS += -DDSL_G31
endif

#CFLAGS  += -DDEBUG -DDEBUG_MORE
 
all: networkmap arpstorm

arpstorm: shutils.o arpstorm.o
	$(CC) -o $@ $^ $(LDFLAGS) $(CFLAGS)

networkmap: shutils.o function.o networkmap.o
	$(CC) -o $@ $^ $(LDFLAGS) $(CFLAGS)

ipscan: function.o ipscan.o 
	$(CC) -o $@ $^ $(LDFLAGS) $(CFLAGS)

ctrlpt: ctrlpt.o
	$(CC) -o $@ $^ $(LDFLAGS) $(CFLAGS)

upnp:   upnp.o
	$(CC) -o $@ $^ $(LDFLAGS) $(CFLAGS)

install: all
	install -m 755 networkmap $(INSTALL_DIR)/bin/networkmap
	$(STRIP) $(INSTALL_DIR)/bin/networkmap

dynamic: all install

clean: 
	rm -rf *.o networkmap
