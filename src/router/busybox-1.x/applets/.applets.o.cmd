cmd_applets/applets.o := mipsel-uclibc-gcc -Wp,-MD,applets/.applets.o.d   -std=gnu99 -Iinclude -Ilibbb  -I/root/RT-N16.1014/RT-N16/src/router/busybox-1.x/libbb -include include/autoconf.h -D_GNU_SOURCE -DNDEBUG -D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64 -D"BB_VER=KBUILD_STR(1.7.2)" -DBB_BT=AUTOCONF_TIMESTAMP -D_FORTIFY_SOURCE=2 -DBCMWPA2 -DBCMWPS -D__CONFIG_EMF__ -D__CONFIG_IGMP_PROXY__ -DBCMWPA2 -DBCMWPS -D__CONFIG_EMF__ -D__CONFIG_IGMP_PROXY__ -DBCMWPA2 -DBCMWPS -D__CONFIG_EMF__ -D__CONFIG_IGMP_PROXY__ -Wall -Wshadow -Wwrite-strings -Wundef -Wstrict-prototypes -Wold-style-definition -Wmissing-prototypes -Wmissing-declarations -Os -fno-builtin-strlen -finline-limit=0 -fomit-frame-pointer -ffunction-sections -fdata-sections -fno-guess-branch-probability -funsigned-char -static-libgcc -falign-functions=1 -falign-jumps=1 -falign-labels=1 -falign-loops=1 -Wdeclaration-after-statement -Wno-pointer-sign    -D"KBUILD_STR(s)=\#s" -D"KBUILD_BASENAME=KBUILD_STR(applets)"  -D"KBUILD_MODNAME=KBUILD_STR(applets)" -c -o applets/applets.o applets/applets.c

deps_applets/applets.o := \
  applets/applets.c \
    $(wildcard include/config/static.h) \
    $(wildcard include/config/show/usage.h) \
    $(wildcard include/config/feature/compress/usage.h) \
    $(wildcard include/config/feature/sh/standalone.h) \
    $(wildcard include/config/ture/suid.h) \
    $(wildcard include/config/feature/suid/config.h) \
    $(wildcard include/config/feature/suid.h) \
    $(wildcard include/config/feature/suid/config/quiet.h) \
    $(wildcard include/config/quiet.h) \
    $(wildcard include/config/feature/installer.h) \
    $(wildcard include/config/feature/autowidth.h) \
    $(wildcard include/config/locale/support.h) \
  /root/toolchains/hndtools-mipsel-linux-uclibc-4.1.2-32/bin/../lib/gcc/mipsel-linux-uclibc/4.1.2/../../../../mipsel-linux-uclibc/sys-include/assert.h \
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
  include/busybox.h \
    $(wildcard include/config/feature/prefer/applets.h) \
  include/libbb.h \
    $(wildcard include/config/selinux.h) \
    $(wildcard include/config/feature/shadowpasswds.h) \
    $(wildcard include/config/lfs.h) \
    $(wildcard include/config/feature/buffers/go/on/stack.h) \
    $(wildcard include/config/buffer.h) \
    $(wildcard include/config/ubuffer.h) \
    $(wildcard include/config/feature/buffers/go/in/bss.h) \
    $(wildcard include/config/feature/ipv6.h) \
    $(wildcard include/config/ture/ipv6.h) \
    $(wildcard include/config/busybox/exec/path.h) \
    $(wildcard include/config/getopt/long.h) \
    $(wildcard include/config/feature/pidfile.h) \
    $(wildcard include/config/feature/syslog.h) \
    $(wildcard include/config/route.h) \
    $(wildcard include/config/gunzip.h) \
    $(wildcard include/config/ktop.h) \
    $(wildcard include/config/ioctl/hex2str/error.h) \
    $(wildcard include/config/feature/editing.h) \
    $(wildcard include/config/feature/editing/history.h) \
    $(wildcard include/config/ture/editing/savehistory.h) \
    $(wildcard include/config/feature/editing/savehistory.h) \
    $(wildcard include/config/feature/tab/completion.h) \
    $(wildcard include/config/feature/username/completion.h) \
    $(wildcard include/config/feature/editing/vi.h) \
    $(wildcard include/config/inux.h) \
    $(wildcard include/config/feature/devfs.h) \
  include/platform.h \
    $(wildcard include/config/werror.h) \
    $(wildcard include/config///.h) \
    $(wildcard include/config//nommu.h) \
    $(wildcard include/config//mmu.h) \
  /root/toolchains/hndtools-mipsel-linux-uclibc-4.1.2-32/bin/../lib/gcc/mipsel-linux-uclibc/4.1.2/../../../../mipsel-linux-uclibc/sys-include/byteswap.h \
  /root/toolchains/hndtools-mipsel-linux-uclibc-4.1.2-32/bin/../lib/gcc/mipsel-linux-uclibc/4.1.2/../../../../mipsel-linux-uclibc/sys-include/bits/byteswap.h \
  /root/toolchains/hndtools-mipsel-linux-uclibc-4.1.2-32/bin/../lib/gcc/mipsel-linux-uclibc/4.1.2/../../../../mipsel-linux-uclibc/sys-include/endian.h \
  /root/toolchains/hndtools-mipsel-linux-uclibc-4.1.2-32/bin/../lib/gcc/mipsel-linux-uclibc/4.1.2/../../../../mipsel-linux-uclibc/sys-include/bits/endian.h \
  /root/toolchains/hndtools-mipsel-linux-uclibc-4.1.2-32/bin/../lib/gcc/mipsel-linux-uclibc/4.1.2/../../../../mipsel-linux-uclibc/sys-include/arpa/inet.h \
  /root/toolchains/hndtools-mipsel-linux-uclibc-4.1.2-32/bin/../lib/gcc/mipsel-linux-uclibc/4.1.2/../../../../mipsel-linux-uclibc/sys-include/netinet/in.h \
  /root/toolchains/hndtools-mipsel-linux-uclibc-4.1.2-32/bin/../lib/gcc/mipsel-linux-uclibc/4.1.2/../../../../mipsel-linux-uclibc/sys-include/stdint.h \
  /root/toolchains/hndtools-mipsel-linux-uclibc-4.1.2-32/bin/../lib/gcc/mipsel-linux-uclibc/4.1.2/../../../../mipsel-linux-uclibc/sys-include/bits/wordsize.h \
  /root/toolchains/hndtools-mipsel-linux-uclibc-4.1.2-32/bin/../lib/gcc/mipsel-linux-uclibc/4.1.2/../../../../mipsel-linux-uclibc/sys-include/bits/types.h \
  /root/toolchains/hndtools-mipsel-linux-uclibc-4.1.2-32/bin/../lib/gcc/mipsel-linux-uclibc/4.1.2/include/stddef.h \
  /root/toolchains/hndtools-mipsel-linux-uclibc-4.1.2-32/bin/../lib/gcc/mipsel-linux-uclibc/4.1.2/../../../../mipsel-linux-uclibc/sys-include/bits/kernel_types.h \
  /root/toolchains/hndtools-mipsel-linux-uclibc-4.1.2-32/bin/../lib/gcc/mipsel-linux-uclibc/4.1.2/../../../../mipsel-linux-uclibc/sys-include/bits/pthreadtypes.h \
  /root/toolchains/hndtools-mipsel-linux-uclibc-4.1.2-32/bin/../lib/gcc/mipsel-linux-uclibc/4.1.2/../../../../mipsel-linux-uclibc/sys-include/bits/sched.h \
  /root/toolchains/hndtools-mipsel-linux-uclibc-4.1.2-32/bin/../lib/gcc/mipsel-linux-uclibc/4.1.2/../../../../mipsel-linux-uclibc/sys-include/bits/socket.h \
  /root/toolchains/hndtools-mipsel-linux-uclibc-4.1.2-32/bin/../lib/gcc/mipsel-linux-uclibc/4.1.2/include/limits.h \
  /root/toolchains/hndtools-mipsel-linux-uclibc-4.1.2-32/bin/../lib/gcc/mipsel-linux-uclibc/4.1.2/include/syslimits.h \
  /root/toolchains/hndtools-mipsel-linux-uclibc-4.1.2-32/bin/../lib/gcc/mipsel-linux-uclibc/4.1.2/../../../../mipsel-linux-uclibc/sys-include/limits.h \
  /root/toolchains/hndtools-mipsel-linux-uclibc-4.1.2-32/bin/../lib/gcc/mipsel-linux-uclibc/4.1.2/../../../../mipsel-linux-uclibc/sys-include/bits/posix1_lim.h \
  /root/toolchains/hndtools-mipsel-linux-uclibc-4.1.2-32/bin/../lib/gcc/mipsel-linux-uclibc/4.1.2/../../../../mipsel-linux-uclibc/sys-include/bits/local_lim.h \
  /root/toolchains/hndtools-mipsel-linux-uclibc-4.1.2-32/bin/../lib/gcc/mipsel-linux-uclibc/4.1.2/../../../../mipsel-linux-uclibc/sys-include/linux/limits.h \
  /root/toolchains/hndtools-mipsel-linux-uclibc-4.1.2-32/bin/../lib/gcc/mipsel-linux-uclibc/4.1.2/../../../../mipsel-linux-uclibc/sys-include/bits/posix2_lim.h \
  /root/toolchains/hndtools-mipsel-linux-uclibc-4.1.2-32/bin/../lib/gcc/mipsel-linux-uclibc/4.1.2/../../../../mipsel-linux-uclibc/sys-include/bits/xopen_lim.h \
  /root/toolchains/hndtools-mipsel-linux-uclibc-4.1.2-32/bin/../lib/gcc/mipsel-linux-uclibc/4.1.2/../../../../mipsel-linux-uclibc/sys-include/bits/stdio_lim.h \
  /root/toolchains/hndtools-mipsel-linux-uclibc-4.1.2-32/bin/../lib/gcc/mipsel-linux-uclibc/4.1.2/../../../../mipsel-linux-uclibc/sys-include/sys/types.h \
  /root/toolchains/hndtools-mipsel-linux-uclibc-4.1.2-32/bin/../lib/gcc/mipsel-linux-uclibc/4.1.2/../../../../mipsel-linux-uclibc/sys-include/time.h \
  /root/toolchains/hndtools-mipsel-linux-uclibc-4.1.2-32/bin/../lib/gcc/mipsel-linux-uclibc/4.1.2/../../../../mipsel-linux-uclibc/sys-include/sys/select.h \
  /root/toolchains/hndtools-mipsel-linux-uclibc-4.1.2-32/bin/../lib/gcc/mipsel-linux-uclibc/4.1.2/../../../../mipsel-linux-uclibc/sys-include/bits/select.h \
  /root/toolchains/hndtools-mipsel-linux-uclibc-4.1.2-32/bin/../lib/gcc/mipsel-linux-uclibc/4.1.2/../../../../mipsel-linux-uclibc/sys-include/bits/sigset.h \
  /root/toolchains/hndtools-mipsel-linux-uclibc-4.1.2-32/bin/../lib/gcc/mipsel-linux-uclibc/4.1.2/../../../../mipsel-linux-uclibc/sys-include/bits/time.h \
  /root/toolchains/hndtools-mipsel-linux-uclibc-4.1.2-32/bin/../lib/gcc/mipsel-linux-uclibc/4.1.2/../../../../mipsel-linux-uclibc/sys-include/sys/sysmacros.h \
  /root/toolchains/hndtools-mipsel-linux-uclibc-4.1.2-32/bin/../lib/gcc/mipsel-linux-uclibc/4.1.2/../../../../mipsel-linux-uclibc/sys-include/bits/sockaddr.h \
  /root/toolchains/hndtools-mipsel-linux-uclibc-4.1.2-32/bin/../lib/gcc/mipsel-linux-uclibc/4.1.2/../../../../mipsel-linux-uclibc/sys-include/asm/socket.h \
  /root/toolchains/hndtools-mipsel-linux-uclibc-4.1.2-32/bin/../lib/gcc/mipsel-linux-uclibc/4.1.2/../../../../mipsel-linux-uclibc/sys-include/asm/sockios.h \
  /root/toolchains/hndtools-mipsel-linux-uclibc-4.1.2-32/bin/../lib/gcc/mipsel-linux-uclibc/4.1.2/../../../../mipsel-linux-uclibc/sys-include/asm/ioctl.h \
  /root/toolchains/hndtools-mipsel-linux-uclibc-4.1.2-32/bin/../lib/gcc/mipsel-linux-uclibc/4.1.2/../../../../mipsel-linux-uclibc/sys-include/bits/in.h \
  /root/toolchains/hndtools-mipsel-linux-uclibc-4.1.2-32/bin/../lib/gcc/mipsel-linux-uclibc/4.1.2/include/stdbool.h \
  /root/toolchains/hndtools-mipsel-linux-uclibc-4.1.2-32/bin/../lib/gcc/mipsel-linux-uclibc/4.1.2/../../../../mipsel-linux-uclibc/sys-include/sys/mount.h \
  /root/toolchains/hndtools-mipsel-linux-uclibc-4.1.2-32/bin/../lib/gcc/mipsel-linux-uclibc/4.1.2/../../../../mipsel-linux-uclibc/sys-include/sys/ioctl.h \
  /root/toolchains/hndtools-mipsel-linux-uclibc-4.1.2-32/bin/../lib/gcc/mipsel-linux-uclibc/4.1.2/../../../../mipsel-linux-uclibc/sys-include/bits/ioctls.h \
  /root/toolchains/hndtools-mipsel-linux-uclibc-4.1.2-32/bin/../lib/gcc/mipsel-linux-uclibc/4.1.2/../../../../mipsel-linux-uclibc/sys-include/asm/ioctls.h \
  /root/toolchains/hndtools-mipsel-linux-uclibc-4.1.2-32/bin/../lib/gcc/mipsel-linux-uclibc/4.1.2/../../../../mipsel-linux-uclibc/sys-include/bits/ioctl-types.h \
  /root/toolchains/hndtools-mipsel-linux-uclibc-4.1.2-32/bin/../lib/gcc/mipsel-linux-uclibc/4.1.2/../../../../mipsel-linux-uclibc/sys-include/sys/ttydefaults.h \
  /root/toolchains/hndtools-mipsel-linux-uclibc-4.1.2-32/bin/../lib/gcc/mipsel-linux-uclibc/4.1.2/../../../../mipsel-linux-uclibc/sys-include/ctype.h \
  /root/toolchains/hndtools-mipsel-linux-uclibc-4.1.2-32/bin/../lib/gcc/mipsel-linux-uclibc/4.1.2/../../../../mipsel-linux-uclibc/sys-include/bits/uClibc_touplow.h \
  /root/toolchains/hndtools-mipsel-linux-uclibc-4.1.2-32/bin/../lib/gcc/mipsel-linux-uclibc/4.1.2/../../../../mipsel-linux-uclibc/sys-include/dirent.h \
  /root/toolchains/hndtools-mipsel-linux-uclibc-4.1.2-32/bin/../lib/gcc/mipsel-linux-uclibc/4.1.2/../../../../mipsel-linux-uclibc/sys-include/bits/dirent.h \
  /root/toolchains/hndtools-mipsel-linux-uclibc-4.1.2-32/bin/../lib/gcc/mipsel-linux-uclibc/4.1.2/../../../../mipsel-linux-uclibc/sys-include/errno.h \
  /root/toolchains/hndtools-mipsel-linux-uclibc-4.1.2-32/bin/../lib/gcc/mipsel-linux-uclibc/4.1.2/../../../../mipsel-linux-uclibc/sys-include/bits/errno.h \
  /root/toolchains/hndtools-mipsel-linux-uclibc-4.1.2-32/bin/../lib/gcc/mipsel-linux-uclibc/4.1.2/../../../../mipsel-linux-uclibc/sys-include/bits/errno_values.h \
  /root/toolchains/hndtools-mipsel-linux-uclibc-4.1.2-32/bin/../lib/gcc/mipsel-linux-uclibc/4.1.2/../../../../mipsel-linux-uclibc/sys-include/fcntl.h \
  /root/toolchains/hndtools-mipsel-linux-uclibc-4.1.2-32/bin/../lib/gcc/mipsel-linux-uclibc/4.1.2/../../../../mipsel-linux-uclibc/sys-include/bits/fcntl.h \
  /root/toolchains/hndtools-mipsel-linux-uclibc-4.1.2-32/bin/../lib/gcc/mipsel-linux-uclibc/4.1.2/../../../../mipsel-linux-uclibc/sys-include/sys/stat.h \
  /root/toolchains/hndtools-mipsel-linux-uclibc-4.1.2-32/bin/../lib/gcc/mipsel-linux-uclibc/4.1.2/../../../../mipsel-linux-uclibc/sys-include/bits/stat.h \
  /root/toolchains/hndtools-mipsel-linux-uclibc-4.1.2-32/bin/../lib/gcc/mipsel-linux-uclibc/4.1.2/../../../../mipsel-linux-uclibc/sys-include/inttypes.h \
  /root/toolchains/hndtools-mipsel-linux-uclibc-4.1.2-32/bin/../lib/gcc/mipsel-linux-uclibc/4.1.2/../../../../mipsel-linux-uclibc/sys-include/mntent.h \
  /root/toolchains/hndtools-mipsel-linux-uclibc-4.1.2-32/bin/../lib/gcc/mipsel-linux-uclibc/4.1.2/../../../../mipsel-linux-uclibc/sys-include/stdio.h \
  /root/toolchains/hndtools-mipsel-linux-uclibc-4.1.2-32/bin/../lib/gcc/mipsel-linux-uclibc/4.1.2/../../../../mipsel-linux-uclibc/sys-include/paths.h \
  /root/toolchains/hndtools-mipsel-linux-uclibc-4.1.2-32/bin/../lib/gcc/mipsel-linux-uclibc/4.1.2/../../../../mipsel-linux-uclibc/sys-include/netdb.h \
  /root/toolchains/hndtools-mipsel-linux-uclibc-4.1.2-32/bin/../lib/gcc/mipsel-linux-uclibc/4.1.2/../../../../mipsel-linux-uclibc/sys-include/rpc/netdb.h \
  /root/toolchains/hndtools-mipsel-linux-uclibc-4.1.2-32/bin/../lib/gcc/mipsel-linux-uclibc/4.1.2/../../../../mipsel-linux-uclibc/sys-include/bits/siginfo.h \
  /root/toolchains/hndtools-mipsel-linux-uclibc-4.1.2-32/bin/../lib/gcc/mipsel-linux-uclibc/4.1.2/../../../../mipsel-linux-uclibc/sys-include/bits/netdb.h \
  /root/toolchains/hndtools-mipsel-linux-uclibc-4.1.2-32/bin/../lib/gcc/mipsel-linux-uclibc/4.1.2/../../../../mipsel-linux-uclibc/sys-include/setjmp.h \
  /root/toolchains/hndtools-mipsel-linux-uclibc-4.1.2-32/bin/../lib/gcc/mipsel-linux-uclibc/4.1.2/../../../../mipsel-linux-uclibc/sys-include/bits/setjmp.h \
  /root/toolchains/hndtools-mipsel-linux-uclibc-4.1.2-32/bin/../lib/gcc/mipsel-linux-uclibc/4.1.2/../../../../mipsel-linux-uclibc/sys-include/signal.h \
  /root/toolchains/hndtools-mipsel-linux-uclibc-4.1.2-32/bin/../lib/gcc/mipsel-linux-uclibc/4.1.2/../../../../mipsel-linux-uclibc/sys-include/bits/signum.h \
  /root/toolchains/hndtools-mipsel-linux-uclibc-4.1.2-32/bin/../lib/gcc/mipsel-linux-uclibc/4.1.2/../../../../mipsel-linux-uclibc/sys-include/bits/sigaction.h \
  /root/toolchains/hndtools-mipsel-linux-uclibc-4.1.2-32/bin/../lib/gcc/mipsel-linux-uclibc/4.1.2/../../../../mipsel-linux-uclibc/sys-include/bits/sigcontext.h \
  /root/toolchains/hndtools-mipsel-linux-uclibc-4.1.2-32/bin/../lib/gcc/mipsel-linux-uclibc/4.1.2/../../../../mipsel-linux-uclibc/sys-include/asm/sigcontext.h \
  /root/toolchains/hndtools-mipsel-linux-uclibc-4.1.2-32/bin/../lib/gcc/mipsel-linux-uclibc/4.1.2/../../../../mipsel-linux-uclibc/sys-include/asm/sgidefs.h \
  /root/toolchains/hndtools-mipsel-linux-uclibc-4.1.2-32/bin/../lib/gcc/mipsel-linux-uclibc/4.1.2/../../../../mipsel-linux-uclibc/sys-include/bits/sigstack.h \
  /root/toolchains/hndtools-mipsel-linux-uclibc-4.1.2-32/bin/../lib/gcc/mipsel-linux-uclibc/4.1.2/../../../../mipsel-linux-uclibc/sys-include/ucontext.h \
  /root/toolchains/hndtools-mipsel-linux-uclibc-4.1.2-32/bin/../lib/gcc/mipsel-linux-uclibc/4.1.2/../../../../mipsel-linux-uclibc/sys-include/sys/ucontext.h \
  /root/toolchains/hndtools-mipsel-linux-uclibc-4.1.2-32/bin/../lib/gcc/mipsel-linux-uclibc/4.1.2/../../../../mipsel-linux-uclibc/sys-include/bits/sigthread.h \
  /root/toolchains/hndtools-mipsel-linux-uclibc-4.1.2-32/bin/../lib/gcc/mipsel-linux-uclibc/4.1.2/../../../../mipsel-linux-uclibc/sys-include/bits/uClibc_stdio.h \
  /root/toolchains/hndtools-mipsel-linux-uclibc-4.1.2-32/bin/../lib/gcc/mipsel-linux-uclibc/4.1.2/../../../../mipsel-linux-uclibc/sys-include/bits/uClibc_mutex.h \
  /root/toolchains/hndtools-mipsel-linux-uclibc-4.1.2-32/bin/../lib/gcc/mipsel-linux-uclibc/4.1.2/../../../../mipsel-linux-uclibc/sys-include/pthread.h \
  /root/toolchains/hndtools-mipsel-linux-uclibc-4.1.2-32/bin/../lib/gcc/mipsel-linux-uclibc/4.1.2/../../../../mipsel-linux-uclibc/sys-include/sched.h \
  /root/toolchains/hndtools-mipsel-linux-uclibc-4.1.2-32/bin/../lib/gcc/mipsel-linux-uclibc/4.1.2/../../../../mipsel-linux-uclibc/sys-include/bits/uClibc_clk_tck.h \
  /root/toolchains/hndtools-mipsel-linux-uclibc-4.1.2-32/bin/../lib/gcc/mipsel-linux-uclibc/4.1.2/../../../../mipsel-linux-uclibc/sys-include/bits/initspin.h \
  /root/toolchains/hndtools-mipsel-linux-uclibc-4.1.2-32/bin/../lib/gcc/mipsel-linux-uclibc/4.1.2/../../../../mipsel-linux-uclibc/sys-include/bits/uClibc_pthread.h \
  /root/toolchains/hndtools-mipsel-linux-uclibc-4.1.2-32/bin/../lib/gcc/mipsel-linux-uclibc/4.1.2/include/stdarg.h \
  /root/toolchains/hndtools-mipsel-linux-uclibc-4.1.2-32/bin/../lib/gcc/mipsel-linux-uclibc/4.1.2/../../../../mipsel-linux-uclibc/sys-include/stdlib.h \
  /root/toolchains/hndtools-mipsel-linux-uclibc-4.1.2-32/bin/../lib/gcc/mipsel-linux-uclibc/4.1.2/../../../../mipsel-linux-uclibc/sys-include/bits/waitflags.h \
  /root/toolchains/hndtools-mipsel-linux-uclibc-4.1.2-32/bin/../lib/gcc/mipsel-linux-uclibc/4.1.2/../../../../mipsel-linux-uclibc/sys-include/bits/waitstatus.h \
  /root/toolchains/hndtools-mipsel-linux-uclibc-4.1.2-32/bin/../lib/gcc/mipsel-linux-uclibc/4.1.2/../../../../mipsel-linux-uclibc/sys-include/alloca.h \
  /root/toolchains/hndtools-mipsel-linux-uclibc-4.1.2-32/bin/../lib/gcc/mipsel-linux-uclibc/4.1.2/../../../../mipsel-linux-uclibc/sys-include/string.h \
  /root/toolchains/hndtools-mipsel-linux-uclibc-4.1.2-32/bin/../lib/gcc/mipsel-linux-uclibc/4.1.2/../../../../mipsel-linux-uclibc/sys-include/sys/mman.h \
  /root/toolchains/hndtools-mipsel-linux-uclibc-4.1.2-32/bin/../lib/gcc/mipsel-linux-uclibc/4.1.2/../../../../mipsel-linux-uclibc/sys-include/bits/mman.h \
  /root/toolchains/hndtools-mipsel-linux-uclibc-4.1.2-32/bin/../lib/gcc/mipsel-linux-uclibc/4.1.2/../../../../mipsel-linux-uclibc/sys-include/sys/socket.h \
  /root/toolchains/hndtools-mipsel-linux-uclibc-4.1.2-32/bin/../lib/gcc/mipsel-linux-uclibc/4.1.2/../../../../mipsel-linux-uclibc/sys-include/sys/uio.h \
  /root/toolchains/hndtools-mipsel-linux-uclibc-4.1.2-32/bin/../lib/gcc/mipsel-linux-uclibc/4.1.2/../../../../mipsel-linux-uclibc/sys-include/bits/uio.h \
  /root/toolchains/hndtools-mipsel-linux-uclibc-4.1.2-32/bin/../lib/gcc/mipsel-linux-uclibc/4.1.2/../../../../mipsel-linux-uclibc/sys-include/sys/statfs.h \
  /root/toolchains/hndtools-mipsel-linux-uclibc-4.1.2-32/bin/../lib/gcc/mipsel-linux-uclibc/4.1.2/../../../../mipsel-linux-uclibc/sys-include/bits/statfs.h \
  /root/toolchains/hndtools-mipsel-linux-uclibc-4.1.2-32/bin/../lib/gcc/mipsel-linux-uclibc/4.1.2/../../../../mipsel-linux-uclibc/sys-include/sys/time.h \
  /root/toolchains/hndtools-mipsel-linux-uclibc-4.1.2-32/bin/../lib/gcc/mipsel-linux-uclibc/4.1.2/../../../../mipsel-linux-uclibc/sys-include/sys/wait.h \
  /root/toolchains/hndtools-mipsel-linux-uclibc-4.1.2-32/bin/../lib/gcc/mipsel-linux-uclibc/4.1.2/../../../../mipsel-linux-uclibc/sys-include/sys/resource.h \
  /root/toolchains/hndtools-mipsel-linux-uclibc-4.1.2-32/bin/../lib/gcc/mipsel-linux-uclibc/4.1.2/../../../../mipsel-linux-uclibc/sys-include/bits/resource.h \
  /root/toolchains/hndtools-mipsel-linux-uclibc-4.1.2-32/bin/../lib/gcc/mipsel-linux-uclibc/4.1.2/../../../../mipsel-linux-uclibc/sys-include/termios.h \
  /root/toolchains/hndtools-mipsel-linux-uclibc-4.1.2-32/bin/../lib/gcc/mipsel-linux-uclibc/4.1.2/../../../../mipsel-linux-uclibc/sys-include/bits/termios.h \
  /root/toolchains/hndtools-mipsel-linux-uclibc-4.1.2-32/bin/../lib/gcc/mipsel-linux-uclibc/4.1.2/../../../../mipsel-linux-uclibc/sys-include/unistd.h \
  /root/toolchains/hndtools-mipsel-linux-uclibc-4.1.2-32/bin/../lib/gcc/mipsel-linux-uclibc/4.1.2/../../../../mipsel-linux-uclibc/sys-include/bits/posix_opt.h \
  /root/toolchains/hndtools-mipsel-linux-uclibc-4.1.2-32/bin/../lib/gcc/mipsel-linux-uclibc/4.1.2/../../../../mipsel-linux-uclibc/sys-include/bits/environments.h \
  /root/toolchains/hndtools-mipsel-linux-uclibc-4.1.2-32/bin/../lib/gcc/mipsel-linux-uclibc/4.1.2/../../../../mipsel-linux-uclibc/sys-include/bits/confname.h \
  /root/toolchains/hndtools-mipsel-linux-uclibc-4.1.2-32/bin/../lib/gcc/mipsel-linux-uclibc/4.1.2/../../../../mipsel-linux-uclibc/sys-include/bits/getopt.h \
  /root/toolchains/hndtools-mipsel-linux-uclibc-4.1.2-32/bin/../lib/gcc/mipsel-linux-uclibc/4.1.2/../../../../mipsel-linux-uclibc/sys-include/utime.h \
  include/pwd_.h \
    $(wildcard include/config/use/bb/pwd/grp.h) \
  /root/toolchains/hndtools-mipsel-linux-uclibc-4.1.2-32/bin/../lib/gcc/mipsel-linux-uclibc/4.1.2/../../../../mipsel-linux-uclibc/sys-include/pwd.h \
  include/grp_.h \
  /root/toolchains/hndtools-mipsel-linux-uclibc-4.1.2-32/bin/../lib/gcc/mipsel-linux-uclibc/4.1.2/../../../../mipsel-linux-uclibc/sys-include/grp.h \
  /root/toolchains/hndtools-mipsel-linux-uclibc-4.1.2-32/bin/../lib/gcc/mipsel-linux-uclibc/4.1.2/../../../../mipsel-linux-uclibc/sys-include/sys/param.h \
  /root/toolchains/hndtools-mipsel-linux-uclibc-4.1.2-32/bin/../lib/gcc/mipsel-linux-uclibc/4.1.2/../../../../mipsel-linux-uclibc/sys-include/linux/param.h \
  /root/toolchains/hndtools-mipsel-linux-uclibc-4.1.2-32/bin/../lib/gcc/mipsel-linux-uclibc/4.1.2/../../../../mipsel-linux-uclibc/sys-include/asm/param.h \
  include/xatonum.h \
  include/applets.h \
    $(wildcard include/config/feature/verbose/usage.h) \
    $(wildcard include/config/ture/installer.h) \
    $(wildcard include/config/ture/prefer/applets.h) \
    $(wildcard include/config/install/no/usr.h) \
    $(wildcard include/config/t.h) \
    $(wildcard include/config/group.h) \
    $(wildcard include/config/user.h) \
    $(wildcard include/config/timex.h) \
    $(wildcard include/config/applet.h) \
    $(wildcard include/config/ing.h) \
    $(wildcard include/config/ename.h) \
    $(wildcard include/config/onfig.h) \
    $(wildcard include/config/h.h) \
    $(wildcard include/config/zip2.h) \
    $(wildcard include/config/v.h) \
    $(wildcard include/config/ttr.h) \
    $(wildcard include/config/on.h) \
    $(wildcard include/config/rp.h) \
    $(wildcard include/config/od.h) \
    $(wildcard include/config/wn.h) \
    $(wildcard include/config/asswd.h) \
    $(wildcard include/config/st.h) \
    $(wildcard include/config/oot.h) \
    $(wildcard include/config/um.h) \
    $(wildcard include/config/ar.h) \
    $(wildcard include/config/m.h) \
    $(wildcard include/config/applet/noexec.h) \
    $(wildcard include/config/o.h) \
    $(wildcard include/config/nd.h) \
    $(wildcard include/config/ntab.h) \
    $(wildcard include/config/ptpw.h) \
    $(wildcard include/config/yhack.h) \
    $(wildcard include/config/e.h) \
    $(wildcard include/config/llocvt.h) \
    $(wildcard include/config/fsd.h) \
    $(wildcard include/config//dhcprelay.h) \
    $(wildcard include/config/f.h) \
    $(wildcard include/config/name.h) \
    $(wildcard include/config/sg.h) \
    $(wildcard include/config/2unix.h) \
    $(wildcard include/config/g.h) \
    $(wildcard include/config/g/deb.h) \
    $(wildcard include/config/pkmap.h) \
    $(wildcard include/config//dumpleases.h) \
    $(wildcard include/config/sck.h) \
    $(wildcard include/config/abel.h) \
    $(wildcard include/config/ture/grep/egrep/alias.h) \
    $(wildcard include/config/ct.h) \
    $(wildcard include/config/dir.h) \
    $(wildcard include/config/uidgid.h) \
    $(wildcard include/config/er/wake.h) \
    $(wildcard include/config/and.h) \
    $(wildcard include/config/r.h) \
    $(wildcard include/config/eidentd.h) \
    $(wildcard include/config/se.h) \
    $(wildcard include/config/et.h) \
    $(wildcard include/config/lush.h) \
    $(wildcard include/config/ormat.h) \
    $(wildcard include/config/sk.h) \
    $(wildcard include/config/ture/grep/fgrep/alias.h) \
    $(wildcard include/config/dfs.h) \
    $(wildcard include/config/eramdisk.h) \
    $(wildcard include/config/k.h) \
    $(wildcard include/config/k/minix.h) \
    $(wildcard include/config/get.h) \
    $(wildcard include/config/put.h) \
    $(wildcard include/config/er.h) \
    $(wildcard include/config/enforce.h) \
    $(wildcard include/config/opt.h) \
    $(wildcard include/config/sebool.h) \
    $(wildcard include/config/ty.h) \
    $(wildcard include/config/p.h) \
    $(wildcard include/config/zip.h) \
    $(wildcard include/config/arm.h) \
    $(wildcard include/config/dump.h) \
    $(wildcard include/config/tid.h) \
    $(wildcard include/config/tname.h) \
    $(wildcard include/config/pd.h) \
    $(wildcard include/config/lock.h) \
    $(wildcard include/config/pdown.h) \
    $(wildcard include/config/td.h) \
    $(wildcard include/config/mod.h) \
    $(wildcard include/config/tall.h) \
    $(wildcard include/config/feature/ip/address.h) \
    $(wildcard include/config/feature/ip/route.h) \
    $(wildcard include/config/feature/ip/link.h) \
    $(wildcard include/config/feature/ip/tunnel.h) \
    $(wildcard include/config/feature/ip/rule.h) \
    $(wildcard include/config/ddr.h) \
    $(wildcard include/config/alc.h) \
    $(wildcard include/config/rm.h) \
    $(wildcard include/config/s.h) \
    $(wildcard include/config/ink.h) \
    $(wildcard include/config/oute.h) \
    $(wildcard include/config/ule.h) \
    $(wildcard include/config/unnel.h) \
    $(wildcard include/config/l.h) \
    $(wildcard include/config/lall.h) \
    $(wildcard include/config/lall5.h) \
    $(wildcard include/config/gd.h) \
    $(wildcard include/config/gth.h) \
    $(wildcard include/config/arch.h) \
    $(wildcard include/config/ture/initrd.h) \
    $(wildcard include/config/d/policy.h) \
    $(wildcard include/config/dfont.h) \
    $(wildcard include/config/dkmap.h) \
    $(wildcard include/config/ger.h) \
    $(wildcard include/config/in.h) \
    $(wildcard include/config/read.h) \
    $(wildcard include/config/etup.h) \
    $(wildcard include/config/zma.h) \
    $(wildcard include/config/edevs.h) \
    $(wildcard include/config/chpathcon.h) \
    $(wildcard include/config/sum.h) \
    $(wildcard include/config/ir.h) \
    $(wildcard include/config/2fs.h) \
    $(wildcard include/config/ifo.h) \
    $(wildcard include/config/s/minix.h) \
    $(wildcard include/config/wap.h) \
    $(wildcard include/config/emp.h) \
    $(wildcard include/config/probe.h) \
    $(wildcard include/config/nt.h) \
    $(wildcard include/config/sktop.h) \
    $(wildcard include/config/ntpoint.h) \
    $(wildcard include/config/eif.h) \
    $(wildcard include/config/stat.h) \
    $(wildcard include/config/ter.h) \
    $(wildcard include/config/up.h) \
    $(wildcard include/config/ookup.h) \
    $(wildcard include/config/nvt.h) \
    $(wildcard include/config/swd.h) \
    $(wildcard include/config/ch.h) \
    $(wildcard include/config/of.h) \
    $(wildcard include/config/g6.h) \
    $(wildcard include/config/e/progress.h) \
    $(wildcard include/config/ot/root.h) \
    $(wildcard include/config/ntenv.h) \
    $(wildcard include/config/ntf.h) \
    $(wildcard include/config/an.h) \
    $(wildcard include/config/dautorun.h) \
    $(wildcard include/config/te.h) \
    $(wildcard include/config/dahead.h) \
    $(wildcard include/config/dlink.h) \
    $(wildcard include/config/dprofile.h) \
    $(wildcard include/config/lpath.h) \
    $(wildcard include/config/ice.h) \
    $(wildcard include/config/ize.h) \
    $(wildcard include/config/torecon.h) \
    $(wildcard include/config/applet/nofork.h) \
    $(wildcard include/config/2cpio.h) \
    $(wildcard include/config//parts.h) \
    $(wildcard include/config/con.h) \
    $(wildcard include/config/level.h) \
    $(wildcard include/config/sv.h) \
    $(wildcard include/config/svdir.h) \
    $(wildcard include/config/inuxenabled.h) \
    $(wildcard include/config/console.h) \
    $(wildcard include/config/files.h) \
    $(wildcard include/config/keycodes.h) \
    $(wildcard include/config/logcons.h) \
    $(wildcard include/config/sid.h) \
    $(wildcard include/config/ture/sh/is/ash.h) \
    $(wildcard include/config/ture/sh/is/hush.h) \
    $(wildcard include/config/ture/sh/is/lash.h) \
    $(wildcard include/config/ture/sh/is/msh.h) \
    $(wildcard include/config/1sum.h) \
    $(wildcard include/config/ttach.h) \
    $(wildcard include/config/ep.h) \
    $(wildcard include/config/tlimit.h) \
    $(wildcard include/config/it.h) \
    $(wildcard include/config/rt/stop/daemon.h) \
    $(wildcard include/config/ings.h) \
    $(wildcard include/config/y.h) \
    $(wildcard include/config/ogin.h) \
    $(wildcard include/config/ogd.h) \
    $(wildcard include/config/ponoff.h) \
    $(wildcard include/config/tch/root.h) \
    $(wildcard include/config/sysctl.h) \
    $(wildcard include/config/logd.h) \
    $(wildcard include/config/kset.h) \
    $(wildcard include/config/svd.h) \
    $(wildcard include/config/net.h) \
    $(wildcard include/config/netd.h) \
    $(wildcard include/config/feature/tftp/get.h) \
    $(wildcard include/config/feature/tftp/put.h) \
    $(wildcard include/config/ceroute.h) \
    $(wildcard include/config/size.h) \
    $(wildcard include/config/e2fs.h) \
    $(wildcard include/config//udhcpc.h) \
    $(wildcard include/config//udhcpd.h) \
    $(wildcard include/config/unt.h) \
    $(wildcard include/config/me.h) \
    $(wildcard include/config/ompress.h) \
    $(wildcard include/config/xpand.h) \
    $(wildcard include/config/q.h) \
    $(wildcard include/config/x2dos.h) \
    $(wildcard include/config/ip.h) \
    $(wildcard include/config/ime.h) \
    $(wildcard include/config/eep.h) \
    $(wildcard include/config/ecode.h) \
    $(wildcard include/config/ncode.h) \
    $(wildcard include/config/nfig.h) \
    $(wildcard include/config/ck.h) \
    $(wildcard include/config/chdog.h) \
    $(wildcard include/config/ami.h) \
    $(wildcard include/config/gs.h) \
  include/usage_compressed.h \
  include/unarchive.h \

applets/applets.o: $(deps_applets/applets.o)

$(deps_applets/applets.o):
