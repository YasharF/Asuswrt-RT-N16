#
# milli_httpd Makefile
#
# Copyright (C) 2001 Broadcom Corporation
#
# $Id: Makefile,v 1.15 2008/11/06 08:31:33 james26_jang Exp $
#

.PHONY: all clean sym_link install romfs gen_syms

CFLAGS	+= -I. -I$(TOP)/shared -I$(SRCBASE)/include -Wall -DFLASH_EMULATOR -DLinux -DMIPS $(GLOBAL_OPTIONS)
CFLAGS	+= -s -O2 -DASUS_DDNS -DLINUX26
LDFLAGS	+= -L$(TOP)/nvram -lnvram -L$(TOP)/shared -lshared -L$(TOP)/netconf -lnetconf

ifeq ($(CONFIG_DLM),y)
CFLAGS  += -I$(TOP)/libdisk -DDLM
LDFLAGS += -L$(TOP)/libdisk -ldisk
endif

vpath %.c $(TOP)/shared
vpath %.c $(TOP)/shared_common

ifeq ($(CONFIG_NOUSB),y)
CFLAGS += -DNOUSB
endif

ifeq ($(MODEL_NAME),RT-N12)
CFLAGS += -DRTN12
LANG_FILE = EN.dict TW.dict CN.dict DE.dict CZ.dict PL.dict RU.dict FR.dict MS.dict TH.dict TR.dict
endif

ifeq ($(MODEL_NAME),RT-N16)
CFLAGS += -DRTN16
LANG_FILE = EN.dict TW.dict CN.dict DE.dict CZ.dict PL.dict RU.dict FR.dict MS.dict TH.dict TR.dict
endif

WWWDIR = $(MODEL_NAME)_text

#ifeq ($(LANGUAGE),KR)
#LANG_FILE = EN.dict KR.dict
#else
#LANG_FILE = EN.dict TW.dict CN.dict
#endif

#ifeq ($(MODEL_NAME), WL520GU)
#WWWDIR  = ../www/asus/web_asus_en/WL520gu_text
#else
#WWWDIR = ../www/asus/web_asus_en/WL500gp_text
#LANG_FILE += KR.dict
#endif

vpath translate_%.txt $(WWWDIR)/


all: httpd $(LANG_FILE)
	$(STRIP) httpd

dep:
	@echo "Generate depend file..."
	@for i in `find . -name "*.c"`; do ${CC} -MM ${CFLAGS} "$${i}"; done > $@



clean:
	rm -f *.o *~ httpd
	for f in $(LANG_FILE) ; do rm -f $$f; done
	rm -f *.dict

%.dict: translate_%.txt
	egrep '*' $^ | sed -e 's,<#ZVMODELVZ#>,$(MODEL_NAME),g' -e 's,\r,,' > $@
	cp -f $@ "../www/www.$(MODEL_NAME)/"

install: all
	install -D httpd $(INSTALLDIR)/usr/sbin/httpd

# 2007.10 James. add discover.o
httpd: httpd.o ej.o cgi.o web_ex.o common.o nvram_f.o nvmap.o broadcom.o initial_web_hook.o discover.o ralink.o
	$(CC) -o $@ $^ $(LDFLAGS)
