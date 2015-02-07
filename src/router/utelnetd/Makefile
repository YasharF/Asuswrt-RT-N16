# Makefile for utelnetd
# 
# Configure this with the following environment variables:
#

# where to install
INSTDIR		:= /usr/local/bin/

# GNU target string 
#CROSS		:=

# where to find login program
ifneq ("", "$(BSD)")
LOGIN		:= /usr/bin/login
else
LOGIN		:= /bin/login
endif

ifneq ("", "$(BSD)")
CORE		:= utelnetd.core
else
CORE		:= core
endif

# nothing to configure below this line... ---8<---8<---8<---

PROGS     = utelnetd

INSTMODE  = 0755
INSTOWNER = root
INSTGROUP = root

OBJS      = utelnetd.o

#CC        = $(CROSS)gcc
INSTALL   = install

#CFLAGS	 += -I. -pipe -DSHELLPATH=\"$(LOGIN)\" -Wall
CFLAGS	 += -I. -I$(SRCBASE)/include -pipe -DSHELLPATH=\"$(LOGIN)\" -Wall

ifneq ("","$(DEBUG)")
CFLAGS   += -DDEBUG -g -Os
STRIP	  = \#
else
CFLAGS	 += -fomit-frame-pointer
#STRIP	  = $(CROSS)strip 
endif

ifeq ("1", "$(BSD)")
CFLAGS   += -DBSD
endif


all: $(PROGS)

$(PROGS): $(OBJS)
	#$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@
	$(CC) $(LDFLAGS) -L$(TOP)/nvram -lnvram -s  $^ $(LDLIBS) -o $@
#	$(STRIP) --remove-section=.comment --remove-section=.note $@
	$(STRIP) $@

.PHONY: install
install: $(PROGS)
	$(INSTALL) -d $(INSTDIR)
	$(INSTALL) -m $(INSTMODE) -o $(INSTOWNER) -g $(INSTGROUP) $(PROGS) $(INSTDIR)

.PHONY: clean
clean:
	rm -f $(PROGS) *.o $(CORE)
