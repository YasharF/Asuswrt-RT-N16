OBJS = wanduck.o
BIN = wanduck

CFLAGS  += -I. -I$(LINUXDIR)/include -I$(SRCBASE)/include -I$(TOP)/shared
CFLAGS  += $(GLOBAL_OPTIONS) -Wall -s -O2

LDFLAGS += -L$(TOP)/nvram -lnvram -L$(TOP)/shared -lshared -s
LDFLAGS += -L$(TOP)/netconf -L$(PLATFORMDIR)/install/netconf/usr/lib -lnetconf

all: ${BIN}
	${STRIP} ${BIN}

install: all
	install -D ${BIN} ${INSTALLDIR}/usr/sbin/${BIN}

${BIN}: ${OBJS}
	$(CC) $(LDFLAGS) -o $@ $^

wget: wget.o
	$(CC) -o $@ $^

clean:
	rm -f *.o ${BIN} wget
