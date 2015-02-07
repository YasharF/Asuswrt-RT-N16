DESTDIR=
SBINDIR=/usr/sbin
CONFDIR=/etc/iproute2
DOCDIR=/usr/share/doc/iproute2
MANDIR=/usr/share/man

KERNEL_INCLUDE=$(LINUXDIR)/include

# Path to db_185.h include
DBM_INCLUDE:=/usr/include

DEFINES= -DRESOLVE_HOSTNAMES

#options if you have a bind>=4.9.4 libresolv (or, maybe, glibc)
LDLIBS=-lresolv
ADDLIB=

#options for decnet
ADDLIB+=dnet_ntop.o dnet_pton.o

#options for ipx
ADDLIB+=ipx_ntop.o ipx_pton.o

HOSTCC = gcc
CCOPTS = -D_GNU_SOURCE -O2 -Wstrict-prototypes -Wall
CFLAGS = $(CCOPTS) -I../include $(DEFINES)
YACCFLAGS = -d -t -v

LDLIBS += -L../lib -lnetlink -lutil

SUBDIRS=lib ip tc misc netem

LIBNETLINK=../lib/libnetlink.a ../lib/libutil.a

all: Config
	@for i in $(SUBDIRS); \
	do $(MAKE) $(MFLAGS) -C $$i; done

Config:
	sh configure $(KERNEL_INCLUDE)

install: all
	install -D ip/ip $(INSTALLDIR)/sbin/ip
	install -D tc/tc $(INSTALLDIR)/usr/sbin/tc
	$(STRIP) $(INSTALLDIR)/sbin/ip
	$(STRIP) $(INSTALLDIR)/usr/sbin/tc

clean:
	rm -f cscope.*
	@for i in $(SUBDIRS) doc; \
	do $(MAKE) $(MFLAGS) -C $$i clean; done

clobber: clean
	rm -f Config

distclean: clobber

cscope:
	cscope -b -q -R -Iinclude -sip -slib -smisc -snetem -stc

.EXPORT_ALL_VARIABLES:
