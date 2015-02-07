
#ifndef _PPC_IO_H
#error "Do not include mpc8260_pci9.h directly."
#endif

#ifdef __KERNEL__
#ifndef __CONFIG_8260_PCI9_DEFS
#define __CONFIG_8260_PCI9_DEFS

#undef readb
#undef readw
#undef readl
#undef insb
#undef insw
#undef insl
#undef inb
#undef inw
#undef inl
#undef memcpy_fromio

extern int readb(volatile unsigned char *addr);
extern int readw(volatile unsigned short *addr);
extern unsigned readl(volatile unsigned *addr);
extern void insb(unsigned port, void *buf, int ns);
extern void insw(unsigned port, void *buf, int ns);
extern void insl(unsigned port, void *buf, int nl);
extern int inb(unsigned port);
extern int inw(unsigned port);
extern unsigned inl(unsigned port);
extern void *memcpy_fromio(void *dest, unsigned long src, size_t count);

#endif /* !__CONFIG_8260_PCI9_DEFS */
#endif /* __KERNEL__ */
