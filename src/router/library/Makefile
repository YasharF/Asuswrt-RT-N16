#---------------
SUB =
SUB += fileupload
SUB += cgi-lib
SUB += libupnp-1.2.1
SUB += libiconv-1.8
#.EXPORT_ALL_VARIABLES:
#---------------
all:	${SUB}
#---------------
# ${SUB}::
#	${MAKE} ${MFLAGS} -C $@ || exit 1
#---------------
all romfs:
	for i in $(SUB) ; do $(MAKE) -C $$i $@ || exit $? ; done
#---------------
clean:
	rm -f *.[oa] *~ core
	for i in $(SUB) ; do ${MAKE} -C $$i clean || exit 1; done

#---------------
# End of file Makefile
