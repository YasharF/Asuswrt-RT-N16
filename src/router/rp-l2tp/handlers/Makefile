# Generated automatically from Makefile.in by configure.
# $Id: Makefile.in,v 1.1.1.1 2008/07/21 09:19:34 james26_jang Exp $
# Makefile for LNS handlers
# LIC: GPL

prefix=/usr/local
exec_prefix=${prefix}
mandir=${prefix}/man
docdir=/usr/local/doc/rp-pppoe-$(VERSION)
install=/usr/bin/install -c
install_dir=/usr/bin/install -c -d
sbindir=${exec_prefix}/sbin

HANDLERS=sync-pppd.so cmd.so

OBJS=pty.o sync-pppd.o dstring.o
SRCS=$(OBJS:.o=.c)
CFLAGS=-g -O2 -I.. -I../libevent -I../../libevent -I. -Wall

all: $(HANDLERS) l2tp-control

l2tp-control: cmd-control.o
	mipsel-uclibc-gcc -o l2tp-control cmd-control.o

cmd-control.o: cmd-control.c
	mipsel-uclibc-gcc -c -o $@ $^

%.o: %.c
	mipsel-uclibc-gcc $(CFLAGS) -fPIC -c -o $@ $<

sync-pppd.so: pty.o sync-pppd.o
	mipsel-uclibc-gcc -shared -o $@ $^

cmd.so: cmd.o dstring.o
	mipsel-uclibc-gcc -shared -o $@ $^

clean:
	rm -f *.so *.o *~
	rm -f l2tp-control

install: all
	-mkdir -p $(RPM_INSTALL_ROOT)$(sbindir)
	-mkdir -p $(RPM_INSTALL_ROOT)$(prefix)/lib/l2tp/plugins
	$(install) -m 755 -s l2tp-control $(RPM_INSTALL_ROOT)$(sbindir)
	$(install) -m 755 $(HANDLERS) $(RPM_INSTALL_ROOT)$(prefix)/lib/l2tp/plugins
distclean: clean
