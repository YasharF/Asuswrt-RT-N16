LIBNAME = libmplus
LIBNAME_SHARED = ${LIBNAME}.so
LIBNAME_MAJOR = $(LIBNAME_SHARED).0

LDFLAGS = -D_ISOC99_SOURCE -std=c99 -lm

SRCS =  mplus.c

LOBJS = $(SRCS:.c=.lo)

.SUFFIXES: .c .lo

all: lib_shared

.c.lo:
	$(CC) -c -fPIC $(CFLAGS) -o $@ $<

lib_shared: $(LOBJS)
	$(CC) -shared -Wl,-soname,$(LIBNAME_MAJOR) $(LOBJS) $(LDFLAGS) -o $(LIBNAME_SHARED)

clean:
	rm -rf *.lo *.so*

install: lib_shared
	install -d $(INSTALLDIR)/lib
	install -m 755 $(LIBNAME_SHARED) $(INSTALLDIR)/lib
	$(STRIP) $(INSTALLDIR)/lib/$(LIBNAME_SHARED)

