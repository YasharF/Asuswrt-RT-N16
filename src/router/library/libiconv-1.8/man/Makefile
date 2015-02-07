# Makefile for libiconv/man

#### Start of system configuration section. ####

# Directories used by "make":
srcdir = .

# Directories used by "make install":
prefix = /usr/local
local_prefix = /usr/local
exec_prefix = ${prefix}
mandir = ${prefix}/man
docdir = $(prefix)/doc/libiconv
htmldir = $(docdir)

# Programs used by "make":
RM = rm -f


# Programs used by "make install":
INSTALL = /usr/bin/install -c
INSTALL_PROGRAM = ${INSTALL}
INSTALL_DATA = ${INSTALL} -m 644

#### End of system configuration section. ####

SHELL = /bin/sh

all :

install : force
	if [ ! -d $(DESTDIR)$(prefix) ] ; then mkdir $(DESTDIR)$(prefix) ; fi
	if [ ! -d $(DESTDIR)$(mandir) ] ; then mkdir $(DESTDIR)$(mandir) ; fi
	if [ ! -d $(DESTDIR)$(mandir)/man1 ] ; then mkdir $(DESTDIR)$(mandir)/man1 ; fi
	cd $(srcdir) && for f in *.1 ; do echo $(INSTALL_DATA) $$f $(DESTDIR)$(mandir)/man1/$$f ; $(INSTALL_DATA) $$f $(DESTDIR)$(mandir)/man1/$$f ; done
	if [ ! -d $(DESTDIR)$(mandir)/man3 ] ; then mkdir $(DESTDIR)$(mandir)/man3 ; fi
	cd $(srcdir) && for f in *.3 ; do echo $(INSTALL_DATA) $$f $(DESTDIR)$(mandir)/man3/$$f ; $(INSTALL_DATA) $$f $(DESTDIR)$(mandir)/man3/$$f ; done
	if [ ! -d $(DESTDIR)$(prefix)/doc ] ; then mkdir $(DESTDIR)$(prefix)/doc ; fi
	if [ ! -d $(DESTDIR)$(docdir) ] ; then mkdir $(DESTDIR)$(docdir) ; fi
	if [ ! -d $(DESTDIR)$(htmldir) ] ; then mkdir $(DESTDIR)$(htmldir) ; fi
	cd $(srcdir) && for f in *.html ; do echo $(INSTALL_DATA) $$f $(DESTDIR)$(htmldir)/$$f ; $(INSTALL_DATA) $$f $(DESTDIR)$(htmldir)/$$f ; done

installdirs : force
	if [ ! -d $(DESTDIR)$(prefix) ] ; then mkdir $(DESTDIR)$(prefix) ; fi
	if [ ! -d $(DESTDIR)$(mandir) ] ; then mkdir $(DESTDIR)$(mandir) ; fi
	if [ ! -d $(DESTDIR)$(mandir)/man1 ] ; then mkdir $(DESTDIR)$(mandir)/man1 ; fi
	if [ ! -d $(DESTDIR)$(mandir)/man3 ] ; then mkdir $(DESTDIR)$(mandir)/man3 ; fi
	if [ ! -d $(DESTDIR)$(prefix)/doc ] ; then mkdir $(DESTDIR)$(prefix)/doc ; fi
	if [ ! -d $(DESTDIR)$(docdir) ] ; then mkdir $(DESTDIR)$(docdir) ; fi
	if [ ! -d $(DESTDIR)$(htmldir) ] ; then mkdir $(DESTDIR)$(htmldir) ; fi

uninstall : force
	cd $(srcdir) && for f in *.1 ; do echo $(RM) $(DESTDIR)$(mandir)/man1/$$f ; $(RM) $(DESTDIR)$(mandir)/man1/$$f ; done
	cd $(srcdir) && for f in *.3 ; do echo $(RM) $(DESTDIR)$(mandir)/man3/$$f ; $(RM) $(DESTDIR)$(mandir)/man3/$$f ; done
	cd $(srcdir) && for f in *.html ; do echo $(RM) $(DESTDIR)$(htmldir)/$$f ; $(RM) $(DESTDIR)$(htmldir)/$$f ; done

check :

mostlyclean :

clean :

distclean : force
	$(RM) Makefile

maintainer-clean : distclean

force :

