EXEC = reg
OBJS = reg.o
CFLAGS += -I$(LINUXDIR)/drivers/net/rt_rdm -I$(LINUXDIR)/include

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $(OBJS)

install: all
	install -D $(EXEC) $(INSTALLDIR)/bin/$(EXEC)
	$(STRIP) $(INSTALLDIR)/bin/$(EXEC)

clean:
	-rm -f $(EXEC) *.elf *.gdb *.o
