
CROSS =
CC = $(CROSS)gcc
AR = $(CROSS)ar
STRIP = $(CROSS)strip

ALL_OBJS = igmprt.o proxy.o igmpv2.o util.o filter.o

CCFLAGS += -DHND_FIX -Wall -I../../include -I../shared -s -Os -fomit-frame-pointer
LDFLAGS += -Wl,-allow-shlib-undefined 
LDFLAGS += -L$(TOP)/netconf -L$(INSTALLDIR)/netconf/usr/lib -lnetconf

ifneq ($(wildcard ./prebuilt),)
all:
	-cp prebuilt/igmp igmp
else
all: igmp
endif

igmp: $(ALL_OBJS)
	$(CC) $(CCFLAGS) $(LDFLAGS) -o $@ $(ALL_OBJS) $(LDLIBS)
	$(STRIP) $@

$(ALL_OBJS): %.o: %.c
	@echo " "
	@echo "Making $<"
	$(CC) $(CCFLAGS) -c $<

install: all
	install -d $(INSTALLDIR)/igmp/usr/sbin
	install -m 755 igmp $(INSTALLDIR)/igmp/usr/sbin

.PHONY:	clean
clean:
	-rm -f *.o *~ core igmp
