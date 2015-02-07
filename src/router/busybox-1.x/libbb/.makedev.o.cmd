cmd_libbb/makedev.o := mipsel-uclibc-gcc -Wp,-MD,libbb/.makedev.o.d   -std=gnu99 -Iinclude -Ilibbb  -I/root/RT-N16.1014/RT-N16/src/router/busybox-1.x/libbb -include include/autoconf.h -D_GNU_SOURCE -DNDEBUG -D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64 -D"BB_VER=KBUILD_STR(1.7.2)" -DBB_BT=AUTOCONF_TIMESTAMP -D_FORTIFY_SOURCE=2 -DBCMWPA2 -DBCMWPS -D__CONFIG_EMF__ -D__CONFIG_IGMP_PROXY__ -DBCMWPA2 -DBCMWPS -D__CONFIG_EMF__ -D__CONFIG_IGMP_PROXY__ -DBCMWPA2 -DBCMWPS -D__CONFIG_EMF__ -D__CONFIG_IGMP_PROXY__ -Wall -Wshadow -Wwrite-strings -Wundef -Wstrict-prototypes -Wold-style-definition -Wmissing-prototypes -Wmissing-declarations -Os -fno-builtin-strlen -finline-limit=0 -fomit-frame-pointer -ffunction-sections -fdata-sections -fno-guess-branch-probability -funsigned-char -static-libgcc -falign-functions=1 -falign-jumps=1 -falign-labels=1 -falign-loops=1 -Wdeclaration-after-statement -Wno-pointer-sign    -D"KBUILD_STR(s)=\#s" -D"KBUILD_BASENAME=KBUILD_STR(makedev)"  -D"KBUILD_MODNAME=KBUILD_STR(makedev)" -c -o libbb/makedev.o libbb/makedev.c

deps_libbb/makedev.o := \
  libbb/makedev.c \
  /root/toolchains/hndtools-mipsel-linux-uclibc-4.1.2-32/bin/../lib/gcc/mipsel-linux-uclibc/4.1.2/../../../../mipsel-linux-uclibc/sys-include/features.h \
    $(wildcard include/config/c99.h) \
    $(wildcard include/config/ix.h) \
    $(wildcard include/config/ix2.h) \
    $(wildcard include/config/ix199309.h) \
    $(wildcard include/config/ix199506.h) \
    $(wildcard include/config/en.h) \
    $(wildcard include/config/en/extended.h) \
    $(wildcard include/config/x98.h) \
    $(wildcard include/config/en2k.h) \
    $(wildcard include/config/gefile.h) \
    $(wildcard include/config/gefile64.h) \
    $(wildcard include/config/e/offset64.h) \
    $(wildcard include/config/.h) \
    $(wildcard include/config/d.h) \
    $(wildcard include/config/c.h) \
    $(wildcard include/config/ntrant.h) \
    $(wildcard include/config/i.h) \
    $(wildcard include/config/ern/inlines.h) \
  /root/toolchains/hndtools-mipsel-linux-uclibc-4.1.2-32/bin/../lib/gcc/mipsel-linux-uclibc/4.1.2/../../../../mipsel-linux-uclibc/sys-include/bits/uClibc_config.h \
    $(wildcard include/config/mips/isa/1//.h) \
    $(wildcard include/config/mips/isa/2//.h) \
    $(wildcard include/config/mips/isa/3//.h) \
    $(wildcard include/config/mips/isa/4//.h) \
    $(wildcard include/config/mips/isa/mips32//.h) \
    $(wildcard include/config/mips/isa/mips64//.h) \
    $(wildcard include/config//.h) \
    $(wildcard include/config//vfprintf//.h) \
  /root/toolchains/hndtools-mipsel-linux-uclibc-4.1.2-32/bin/../lib/gcc/mipsel-linux-uclibc/4.1.2/../../../../mipsel-linux-uclibc/sys-include/sys/cdefs.h \
    $(wildcard include/config/espaces.h) \
    $(wildcard include/config/tify/level.h) \
  /root/toolchains/hndtools-mipsel-linux-uclibc-4.1.2-32/bin/../lib/gcc/mipsel-linux-uclibc/4.1.2/../../../../mipsel-linux-uclibc/sys-include/sys/sysmacros.h \

libbb/makedev.o: $(deps_libbb/makedev.o)

$(deps_libbb/makedev.o):
