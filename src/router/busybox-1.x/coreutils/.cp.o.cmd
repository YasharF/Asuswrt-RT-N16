cmd_coreutils/cp.o := mipsel-uclibc-gcc -Wp,-MD,coreutils/.cp.o.d   -std=gnu99 -Iinclude -Ilibbb  -I/root/RT-N16.1014/RT-N16/src/router/busybox-1.x/libbb -include include/autoconf.h -D_GNU_SOURCE -DNDEBUG -D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64 -D"BB_VER=KBUILD_STR(1.7.2)" -DBB_BT=AUTOCONF_TIMESTAMP -D_FORTIFY_SOURCE=2 -DBCMWPA2 -DBCMWPS -D__CONFIG_EMF__ -D__CONFIG_IGMP_PROXY__ -DBCMWPA2 -DBCMWPS -D__CONFIG_EMF__ -D__CONFIG_IGMP_PROXY__ -DBCMWPA2 -DBCMWPS -D__CONFIG_EMF__ -D__CONFIG_IGMP_PROXY__ -Wall -Wshadow -Wwrite-strings -Wundef -Wstrict-prototypes -Wold-style-definition -Wmissing-prototypes -Wmissing-declarations -Os -fno-builtin-strlen -finline-limit=0 -fomit-frame-pointer -ffunction-sections -fdata-sections -fno-guess-branch-probability -funsigned-char -static-libgcc -falign-functions=1 -falign-jumps=1 -falign-labels=1 -falign-loops=1 -Wdeclaration-after-statement -Wno-pointer-sign    -D"KBUILD_STR(s)=\#s" -D"KBUILD_BASENAME=KBUILD_STR(cp)"  -D"KBUILD_MODNAME=KBUILD_STR(cp)" -c -o coreutils/cp.o coreutils/cp.c

deps_coreutils/cp.o := \
  coreutils/cp.c \
    $(wildcard include/config/selinux.h) \
  include/libbb.h \
    $(wildcard include/config/locale/support.h) \
    $(wildcard include/config/feature/shadowpasswds.h) \
    $(wildcard include/config/lfs.h) \
    $(wildcard include/config/feature/buffers/go/on/stack.h) \
    $(wildcard include/config/buffer.h) \
    $(wildcard include/config/ubuffer.h) \
    $(wildcard include/config/feature/buffers/go/in/bss.h) \
    $(wildcard include/config/feature/ipv6.h) \
    $(wildcard include/config/ture/ipv6.h) \
    $(wildcard include/config/feature/prefer/applets.h) \
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
    $(wildcard include/config/.h) \
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
  coreutils/libcoreutils/coreutils.h \

coreutils/cp.o: $(deps_coreutils/cp.o)

$(deps_coreutils/cp.o):
