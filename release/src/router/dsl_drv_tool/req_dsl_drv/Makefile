include ../../common.mak

CFLAGS += -Wall -O

ifeq ($(RTCONFIG_BCMARM),y)
CFLAGS += $(EXTRA_FLAG)
endif

CFLAGS += -DASUS
CFLAGS += -I$(TOP)/shared -I$(SRCBASE)/include
LDFLAGS = -L$(TOP)/shared -lshared -L$(TOP)/nvram${BCMEX} -lnvram
INSTALL = install

OBJS = req_dsl_drv.o

all: req_dsl_drv

req_dsl_drv: $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

install: all
	$(STRIP) req_dsl_drv
	$(INSTALL) -d $(INSTALLDIR)/usr/sbin 
	$(INSTALL) req_dsl_drv $(INSTALLDIR)/usr/sbin

clean:
	rm -f req_dsl_drv *.o .*.depend

%.o: %.c .%.depend
	@echo " [req_dsl_drv] CC $@"
	@$(CC) $(CFLAGS) -o $@ -c $<

.%.depend: %.c
	@$(CC) $(CFLAGS) -M $< > $@

-include $(OBJS:%.o=.%.depend)
