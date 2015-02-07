# Makefile for libiconv/src

#### Start of system configuration section. ####

# Directories used by "make":
srcdir = .

# Directories used by "make install":
prefix = /usr/local
local_prefix = /usr/local
exec_prefix = ${prefix}
bindir = ${exec_prefix}/bin
libdir = ${exec_prefix}/lib
datadir = ${prefix}/share
localedir = $(datadir)/locale

# Programs used by "make":
CC = gcc
CFLAGS = -g -O2
CPPFLAGS = 
LDFLAGS = 
INCLUDES = -I. -I$(srcdir) -I../include -I$(srcdir)/../include
LIBTOOL = $(SHELL) $(top_builddir)/libtool
LIBTOOL_COMPILE = $(LIBTOOL) --mode=compile
LIBTOOL_LINK = $(LIBTOOL) --mode=link
LIBTOOL_INSTALL = $(LIBTOOL) --mode=install
LIBTOOL_UNINSTALL = $(LIBTOOL) --mode=uninstall
RM = rm -f


# Programs used by "make install":
INSTALL = /usr/bin/install -c
INSTALL_PROGRAM = ${INSTALL}
INSTALL_DATA = ${INSTALL} -m 644

#### End of system configuration section. ####

SHELL = /bin/sh

# Needed by $(LIBTOOL).
top_builddir = ..

# We cannot link with libintl until libiconv is installed. (When we call
# libtool with arguments "../lib/libiconv.la -lintl", libtool will call ld
# with "../lib/.libs/libiconv.so $libdir/libintl.so $libdir/libiconv.so",
# (remember that $libdir/libintl.la lists $libdir/libiconv.so as a dependency),
# and this gives a fatal linker error on Solaris because the two libiconv.so
# files are different but have the same soname.
# So we can link the iconv executable only after we have installed libiconv,
# i.e. during "make install". The intermediate 'iconv' executable is built
# without internationalization and not linked with libintl.

all : iconv_no_i18n iconv.o
	chmod 777 .

# This is the temporary iconv executable, without internationalization.
iconv_no_i18n : iconv_no_i18n.o ../lib/libiconv.la
	$(LIBTOOL_LINK) $(CC) $(LDFLAGS) iconv_no_i18n.o ../lib/libiconv.la -o $@

iconv_no_i18n.o : $(srcdir)/iconv_no_i18n.c $(srcdir)/iconv.c
	$(CC) -c $(INCLUDES) -I../lib $(CFLAGS) $(CPPFLAGS) -DLOCALEDIR=\"$(localedir)\" $(srcdir)/iconv_no_i18n.c

iconv.o : $(srcdir)/iconv.c
	$(CC) -c $(INCLUDES) -I../lib $(CFLAGS) $(CPPFLAGS) -DLOCALEDIR=\"$(localedir)\" $(srcdir)/iconv.c

# The following rule is necessary to avoid a toplevel "make -n check" failure.
../lib/libiconv.la :
	cd ../lib && $(MAKE) libiconv.la

# During "make install", we can build the final iconv executable.
# On HP-UX, in order to ensure that the new libiconv.sl will override the old
# one that is hardcoded in libintl.sl, we need to mention libiconv.sl before
# libintl.sl on the link command line. We have to bypass libtool in order to
# achieve this.
install : all force
	if [ ! -d $(DESTDIR)$(prefix) ] ; then mkdir $(DESTDIR)$(prefix) ; fi
	if [ ! -d $(DESTDIR)$(exec_prefix) ] ; then mkdir $(DESTDIR)$(exec_prefix) ; fi
	if [ ! -d $(DESTDIR)$(bindir) ] ; then mkdir $(DESTDIR)$(bindir) ; fi
	case "linux-gnu" in \
	  hpux*) $(CC) $(LDFLAGS) iconv.o -L$(libdir) -liconv  -o iconv;; \
	  *) $(LIBTOOL_LINK) $(CC) $(LDFLAGS) iconv.o $(libdir)/libiconv.la  -o iconv;; \
	esac
	$(LIBTOOL_INSTALL) $(INSTALL_PROGRAM) iconv $(DESTDIR)$(bindir)/iconv

installdirs : force
	if [ ! -d $(DESTDIR)$(prefix) ] ; then mkdir $(DESTDIR)$(prefix) ; fi
	if [ ! -d $(DESTDIR)$(exec_prefix) ] ; then mkdir $(DESTDIR)$(exec_prefix) ; fi
	if [ ! -d $(DESTDIR)$(bindir) ] ; then mkdir $(DESTDIR)$(bindir) ; fi

uninstall : force
	$(LIBTOOL_UNINSTALL) $(RM) $(DESTDIR)$(bindir)/iconv

check : all

mostlyclean : clean

clean : force
	$(RM) *.o *.lo iconv_no_i18n iconv core
	$(RM) -r .libs _libs

distclean : clean
	$(RM) Makefile config.h

maintainer-clean : distclean

force :

