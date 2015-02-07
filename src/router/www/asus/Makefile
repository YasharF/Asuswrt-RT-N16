#
# Broadcom web pages
#
# Copyright 2003, Broadcom Corporation
# All Rights Reserved.                
#                                     
# This is UNPUBLISHED PROPRIETARY SOURCE CODE of Broadcom Corporation;   
# the contents of this file may not be disclosed to third parties, copied
# or duplicated in any form, in whole or in part, without the prior      
# written permission of Broadcom Corporation.                            
#
# $Id: Makefile,v 1.1.1.1 2008/07/21 09:20:26 james26_jang Exp $
#

PAGES := *
PAGES_DIR := graph/*

all: pages.mk

pages.mk:
	echo "PAGES := $(PAGES)" > $@

install:
	./mkweb WL500gx "EN"
	install -d $(INSTALLDIR)/www
	install -m 644 web_WL500gx/*.asp $(INSTALLDIR)/www
	install -m 644 web_WL500gx/*.js $(INSTALLDIR)/www
	install -m 644 web_WL500gx/*.css $(INSTALLDIR)/www
	install -d $(INSTALLDIR)/www/graph
	install -m 644 web_WL500gx/$(PAGES_DIR).jpg $(INSTALLDIR)/www/graph
	install -m 644 web_WL500gx/$(PAGES_DIR).gif $(INSTALLDIR)/www/graph
	rm -rf web
	rm -rf web_WL500gx

install-%:
	./mkweb $* "$(LANGUAGE)"
	rm -rf www_$*	
	install -d www_$*
	install -m 644 web_$*/*.asp www_$*
	install -m 644 web_$*/*.js www_$*
	install -m 644 web_$*/*.css www_$*
#	( if [ "$*" != "WL520g" ] && [ "$*" != "WL550gE" ] && [ "$*" != "WL500bl" ] && [ "$*" != "WL331gl" ]; then \
		install -m 644 web_$*/*.cab www_$*;\
	fi )
	( if [ "$*" = "WL550gE" ]; then \
		install -d www_$*/slider; \
		install -m 644 web_$*/slider/*.js www_$*/slider; \
		install -m 644 web_$*/slider/*.gif www_$*/slider; \
	fi )
	install -d www_$*/graph
	
	install -m 644 web_$*/$(PAGES_DIR).jpg www_$*/graph
	install -m 644 web_$*/$(PAGES_DIR).gif www_$*/graph

	ln -sf /tmp/settings www_$*/Settings.CFG
	cp ../../httpd/*.dict www_$*/ -f
	( if [ "$*" = "WL500gpv2" ]; then \
		install -d www_$*/slider; \
		install -m 644 web_$*/slider/*.js www_$*/slider; \
		install -m 644 web_$*/slider/*.gif www_$*/slider; \
	fi )

	ln -sf /var/tmp/display.jpg www_$*/display.jpg

	rm -rf web
	rm -rf web_$*

clean:
	rm -f pages.mk
