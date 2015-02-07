MACHINE = powerpc
CFLAGS += -fno-builtin
export CFLAGS MACHINE

all:
	$(MAKE) -C source

configure:
	( cd source ; \
	export SMB_BUILD_CC_NEGATIVE_ENUM_VALUES=yes ; \
	export libreplace_cv_READDIR_GETDIRENTRIES=no ; \
	export libreplace_cv_READDIR_GETDENTS=no ; \
	export linux_getgrouplist_ok=no ; \
	export samba_cv_REPLACE_READDIR=no ; \
	export samba_cv_HAVE_WRFILE_KEYTAB=yes ; \
	export samba_cv_HAVE_KERNEL_OPLOCKS_LINUX=yes ; \
	export samba_cv_HAVE_IFACE_IFCONF=yes ; \
	export samba_cv_USE_SETRESUID=yes ; \
	CC=$(CC) ./configure \
		--target=$(MACHINE)-linux \
		--host=$(MACHINE)-linux \
		--build=`/bin/arch`-linux \
		--enable-shared \
		--disable-static \
		--disable-cups \
		--disable-iprint \
		--disable-pie \
		--disable-fam \
		--bindir=/usr/local/samba/bin \
		--sbindir=/usr/local/samba/sbin \
		--libdir=/usr/local/samba/lib \
		--localstatedir=/tmp/samba/lib/ \
		--with-configdir=/tmp/samba/lib/ \
		--with-privatedir=/tmp/samba/private \
		--with-rootsbindir=/usr/local/samba/sbin \
		--with-lockdir=/tmp/samba/var/locks \
		--with-piddir=/tmp/samba/var/locks \
		--without-ldap \
		--without-cifsmount \
		--without-sendfile-support \
		--without-sys-quotas \
		--prefix=/usr/local/samba; \
	)

clean:
	( cd source; make -f Makefile.orig distclean )

distclean: clean
	( cd source; make -f Makefile.orig distclean )
	( find source -name config.h | xargs rm -f )
	( find source -name Makefile | xargs rm -f )


install:
	rm -rf $(INSTALLDIR)
	$(MAKE) -C source install DESTDIR=$(INSTALLDIR)
	install -d $(INSTALLDIR)/lib/
	#install /opt/eldk/ppc_4xx/usr/lib/libpopt.so.0 $(INSTALLDIR)/lib/libpopt.so.0
	install source/bin/libbigballofmud.so $(INSTALLDIR)/lib/libbigballofmud.so.0
	rm -rf $(INSTALLDIR)/usr/local/samba/include/
	rm -rf $(INSTALLDIR)/usr/local/samba/share
	rm -rf $(INSTALLDIR)/usr/local/samba/swat
	rm -rf $(INSTALLDIR)/usr/local/samba/lib
	rm -rf $(INSTALLDIR)/tmp
	rm -f $(INSTALLDIR)/usr/local/samba/lib/*.msg
	rm -rf $(INSTALLDIR)/usr/local/samba/sbin/swat
	rm -rf $(INSTALLDIR)/usr/local/samba/sbin/winbindd
	rm -rf $(INSTALLDIR)/usr/local/samba/bin/findsmb
	rm -rf $(INSTALLDIR)/usr/local/samba/bin/nmblookup
	rm -rf $(INSTALLDIR)/usr/local/samba/bin/rpcclient
	rm -rf $(INSTALLDIR)/usr/local/samba/bin/smbcacls
	rm -rf $(INSTALLDIR)/usr/local/samba/bin/smbspool
	rm -rf $(INSTALLDIR)/usr/local/samba/bin/smbstatus
	rm -rf $(INSTALLDIR)/usr/local/samba/bin/smbtar
	rm -rf $(INSTALLDIR)/usr/local/samba/bin/testparm
	rm -rf $(INSTALLDIR)/usr/local/samba/bin/wbinfo
	rm -rf $(INSTALLDIR)/usr/local/samba/bin/ntlm_auth	
	rm -rf $(INSTALLDIR)/usr/local/samba/bin/pdbedit	
	rm -rf $(INSTALLDIR)/usr/local/samba/bin/profiles
	rm -rf $(INSTALLDIR)/usr/local/samba/bin/smbcquotas
	rm -rf $(INSTALLDIR)/usr/local/samba/bin/smbtree
	rm -rf $(INSTALLDIR)/usr/local/samba/bin/tdbdump
	rm -rf $(INSTALLDIR)/usr/local/samba/bin/smbget
	rm -rf $(INSTALLDIR)/usr/local/samba/bin/eventlogadm
	rm -rf $(INSTALLDIR)/usr/local/samba/bin/tdbtool

	$(STRIP) --strip-all $(INSTALLDIR)/usr/local/samba/sbin/*
	$(STRIP) --strip-all $(INSTALLDIR)/usr/local/samba/bin/*

	install -d $(INSTALLDIR)/etc
	cd $(INSTALLDIR)/etc && ln -sf /tmp/passwd passwd
	cd $(INSTALLDIR)/etc && ln -sf /tmp/group group
