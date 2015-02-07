# Under Solaris, you need to 
#    CFLAGS += -xO2 -Xc
#    LDLIBS += -lnsl -lsocket
# Some versions of Linux may need
#    CFLAGS += -D_GNU_SOURCE
# To cross-compile
#    CC = arm-linux-gcc

CFLAGS += -Wall -O

all: ntpclient

test: ntpclient
	./ntpclient -d -r <test.dat

ntpclient: ntpclient.o phaselock.o
	$(CC) -o $@ $^
	$(STRIP) ntpclient

install: ntpclient
	install -D ntpclient $(INSTALLDIR)/usr/sbin/ntpclient
	$(STRIP) $(INSTALLDIR)/usr/sbin/ntpclient

clean:
	rm -f ntpclient *.o
