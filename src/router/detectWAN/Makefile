LIB = libdetectWAN.so
OBJS = detectWAN.o

SHDIR = $(ROOTDIR)/user/shared
USERDIR = $(ROOTDIR)/user

CFLAGS  += -Wall -DASUS -DASUS_NVRAM -DBCMNVRAM -I. -I$(SRCBASE)/include -I$(LINUXDIR)/include -I$(TOP)/shared
#LDFLAGS += -L$(TOP)/shared -lshared  -L$(TOP)/nvram
LDFLAGS += -L$(TOP)/nvram -L$(PLATFORMDIR)/install/nvram/usr/lib -lnvram -L$(TOP)/shared -L$(PLATFORMDIR)/install/shared/usr/lib -lshared
LDFLAGS += -L$(TOP)/netconf -L$(PLATFORMDIR)/install/netconf/usr/lib -lnetconf

all: $(LIB) detectWan	

install: all
	install -D detectWan $(INSTALLDIR)/sbin/detectWan
	install -D $(LIB) $(INSTALLDIR)/lib/$(LIB)

$(LIB): $(OBJS)
	$(CC) -shared -o $@ $^ $(CFLAGS) 
	$(STRIP) $(LIB)
	
detectWan: detectWAN.o
	$(CC) -o $@ $^ $(LDFLAGS) $(CFLAGS)
	$(STRIP) $@

romfs:
	$(ROMFSINST) /lib/$(LIB)
	$(ROMFSINST) /sbin/detectWan
	
.PHONY: clean
clean:
	rm -f detectWan *~ *.o *.so 
