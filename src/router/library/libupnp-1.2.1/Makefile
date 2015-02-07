#---------------
SUB =
SUB += threadutil
SUB += ixml
SUB += upnp

COMM-LIB-PATH = ../lib

#.EXPORT_ALL_VARIABLES:
#---------------
all:	${SUB}
#---------------
# ${SUB}::
#	${MAKE} ${MFLAGS} -C $@ || exit 1
#---------------
all:
	for i in $(SUB) ; do $(MAKE) -C $$i $@ || exit $? ; done
	cp upnp/bin/* ../lib
#---------------
clean:
	rm -f *.[oa] *~ core
	for i in $(SUB) ; do ${MAKE} -C $$i clean || exit 1; done
	rm -f $(COMM-LIB-PATH)/libixml.so $(COMM-LIB-PATH)/libupnp.so $(COMM-LIB-PATH)/libthreadutil.so
romfs:
	$(ROMFSINST) upnp/bin/libixml.so /lib
	$(ROMFSINST) upnp/bin/libthreadutil.so /lib
	$(ROMFSINST) upnp/bin/libupnp.so /lib
#---------------
# End of file Makefile
