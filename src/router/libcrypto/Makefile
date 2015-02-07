#
# Linux router libcrypto Makefile
#
# Copyright 2004, Broadcom Corporation
# All Rights Reserved.                
#                                     
# This is UNPUBLISHED PROPRIETARY SOURCE CODE of Broadcom Corporation;   
# the contents of this file may not be disclosed to third parties, copied
# or duplicated in any form, in whole or in part, without the prior      
# written permission of Broadcom Corporation.                            
#
# $Id: Makefile,v 1.2 2008/08/05 10:59:39 james26_jang Exp $
#

include $(TOP)/.config

CFLAGS	+= -I. -I../httpd -I$(SRCBASE)/include -Wall
#CFLAGS	+= -g -DDEBUG
CFLAGS	+= -s -O2
LDFLAGS += -L.

vpath %.c $(SRCBASE)/bcmcrypto
vpath %.o $(SRCBASE)/router/libcrypto/prebuilt

OBJS := aes.o aeskeywrap.o rijndael-alg-fst.o dh.o bn.o sha1.o passhash.o prf.o md5.o hmac.o rc4.o

# Find all ELF binaries (including extra libraries)
BINARIES := $(shell find $(TARGETDIR) -path $(TARGETDIR)/lib -prune -o -type f -print | file -f - | grep ELF | cut -d':' -f1)
MAP := .map
UNR := .unresolved
SYM := .symbols

all: libcrypto.so

install: all
	install -d $(INSTALLDIR)/usr/lib
	install -m 755 libcrypto.so $(INSTALLDIR)/usr/lib

clean:
	rm -f *.o *.so *.a $(MAP) $(UNR) $(SYM)

libcrypto.so: $(OBJS)
	$(LD) -shared -o $@ $^
	$(STRIP) libcrypto.so

# rule for removing unneeded symbols in the shared library
optimize: libcrypto.so
	$(NM) -o --defined-only --no-sort libcrypto.so | cut -d' ' -f3 > $(MAP)
	$(NM) --dynamic -u --no-sort $(BINARIES) | sort -u > $(UNR)
	rm -rf $(SYM)
	for symbol in `cat $(UNR)` ; do \
	if grep -q "^$${symbol}$$" $(MAP) ; then echo "-u $${symbol}" >> $(SYM) ;  \
	fi ; done
	# if no symbols are needed then delete the shared lib
	if ls $(SYM) ; then \
	$(AR) cr libcrypto.a $(OBJS) ; \
	xargs -t $(LD) -shared -o libcrypto.so libcrypto.a < $(SYM) ; \
	install libcrypto.so $(TARGETDIR)/usr/lib ; \
	else \
	rm $(TARGETDIR)/usr/lib/libcrypto.so ; fi
