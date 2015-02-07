/*
 *  Copyright (C) 2006 Benjamin Herrenschmidt <benh@kernel.crashing.org>
 *		       IBM, Corp.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#undef DEBUG

#include <linux/kernel.h>
#include <linux/mm.h>
#include <linux/pci.h>
#include <asm/io.h>
#include <asm/machdep.h>
#include <asm/pci-bridge.h>
#include <asm/ppc-pci.h>


#define SPIDER_PCI_REG_BASE		0xd000
#define SPIDER_PCI_VCI_CNTL_STAT	0x0110
#define SPIDER_PCI_DUMMY_READ		0x0810
#define SPIDER_PCI_DUMMY_READ_BASE	0x0814

#define SPIDER_DISABLE_PREFETCH

#define MAX_SPIDERS	3

static struct spider_pci_bus {
	void __iomem	*regs;
	unsigned long	mmio_start;
	unsigned long	mmio_end;
	unsigned long	pio_vstart;
	unsigned long	pio_vend;
} spider_pci_busses[MAX_SPIDERS];
static int spider_pci_count;

static struct spider_pci_bus *spider_pci_find(unsigned long vaddr,
					      unsigned long paddr)
{
	int i;

	for (i = 0; i < spider_pci_count; i++) {
		struct spider_pci_bus *bus = &spider_pci_busses[i];
		if (paddr && paddr >= bus->mmio_start && paddr < bus->mmio_end)
			return bus;
		if (vaddr && vaddr >= bus->pio_vstart && vaddr < bus->pio_vend)
			return bus;
	}
	return NULL;
}

static void spider_io_flush(const volatile void __iomem *addr)
{
	struct spider_pci_bus *bus;
	int token;

	/* Get platform token (set by ioremap) from address */
	token = PCI_GET_ADDR_TOKEN(addr);

	if (token && token <= spider_pci_count)
		bus = &spider_pci_busses[token - 1];
	else {
		unsigned long vaddr, paddr;
		pte_t *ptep;

		/* Fixup physical address */
		vaddr = (unsigned long)PCI_FIX_ADDR(addr);

		/* Check if it's in allowed range for  PIO */
		if (vaddr < PHBS_IO_BASE || vaddr >= IMALLOC_BASE)
			return;

		/* Try to find a PTE. If not, clear the paddr, we'll do
		 * a vaddr only lookup (PIO only)
		 */
		ptep = find_linux_pte(init_mm.pgd, vaddr);
		if (ptep == NULL)
			paddr = 0;
		else
			paddr = pte_pfn(*ptep) << PAGE_SHIFT;

		bus = spider_pci_find(vaddr, paddr);
		if (bus == NULL)
			return;
	}

	(void)in_be32(bus->regs + SPIDER_PCI_DUMMY_READ);
}

static u8 spider_readb(const volatile void __iomem *addr)
{
	u8 val = __do_readb(addr);
	spider_io_flush(addr);
	return val;
}

static u16 spider_readw(const volatile void __iomem *addr)
{
	u16 val = __do_readw(addr);
	spider_io_flush(addr);
	return val;
}

static u32 spider_readl(const volatile void __iomem *addr)
{
	u32 val = __do_readl(addr);
	spider_io_flush(addr);
	return val;
}

static u64 spider_readq(const volatile void __iomem *addr)
{
	u64 val = __do_readq(addr);
	spider_io_flush(addr);
	return val;
}

static u16 spider_readw_be(const volatile void __iomem *addr)
{
	u16 val = __do_readw_be(addr);
	spider_io_flush(addr);
	return val;
}

static u32 spider_readl_be(const volatile void __iomem *addr)
{
	u32 val = __do_readl_be(addr);
	spider_io_flush(addr);
	return val;
}

static u64 spider_readq_be(const volatile void __iomem *addr)
{
	u64 val = __do_readq_be(addr);
	spider_io_flush(addr);
	return val;
}

static void spider_readsb(const volatile void __iomem *addr, void *buf,
			  unsigned long count)
{
	__do_readsb(addr, buf, count);
	spider_io_flush(addr);
}

static void spider_readsw(const volatile void __iomem *addr, void *buf,
			  unsigned long count)
{
	__do_readsw(addr, buf, count);
	spider_io_flush(addr);
}

static void spider_readsl(const volatile void __iomem *addr, void *buf,
			  unsigned long count)
{
	__do_readsl(addr, buf, count);
	spider_io_flush(addr);
}

static void spider_memcpy_fromio(void *dest, const volatile void __iomem *src,
				 unsigned long n)
{
	__do_memcpy_fromio(dest, src, n);
	spider_io_flush(src);
}


static void __iomem * spider_ioremap(unsigned long addr, unsigned long size,
				     unsigned long flags)
{
	struct spider_pci_bus *bus;
	void __iomem *res = __ioremap(addr, size, flags);
	int busno;

	pr_debug("spider_ioremap(0x%lx, 0x%lx, 0x%lx) -> 0x%p\n",
		 addr, size, flags, res);

	bus = spider_pci_find(0, addr);
	if (bus != NULL) {
		busno = bus - spider_pci_busses;
		pr_debug(" found bus %d, setting token\n", busno);
		PCI_SET_ADDR_TOKEN(res, busno + 1);
	}
	pr_debug(" result=0x%p\n", res);

	return res;
}

static void __init spider_pci_setup_chip(struct spider_pci_bus *bus)
{
#ifdef SPIDER_DISABLE_PREFETCH
	u32 val = in_be32(bus->regs + SPIDER_PCI_VCI_CNTL_STAT);
	pr_debug(" PVCI_Control_Status was 0x%08x\n", val);
	out_be32(bus->regs + SPIDER_PCI_VCI_CNTL_STAT, val | 0x8);
#endif

	out_be32(bus->regs + SPIDER_PCI_DUMMY_READ_BASE, 0x80000000);
}

static void __init spider_pci_add_one(struct pci_controller *phb)
{
	struct spider_pci_bus *bus = &spider_pci_busses[spider_pci_count];
	struct device_node *np = phb->arch_data;
	struct resource rsrc;
	void __iomem *regs;

	if (spider_pci_count >= MAX_SPIDERS) {
		printk(KERN_ERR "Too many spider bridges, workarounds"
		       " disabled for %s\n", np->full_name);
		return;
	}

	/* Get the registers for the beast */
	if (of_address_to_resource(np, 0, &rsrc)) {
		printk(KERN_ERR "Failed to get registers for spider %s"
		       " workarounds disabled\n", np->full_name);
		return;
	}

	/* Mask out some useless bits in there to get to the base of the
	 * spider chip
	 */
	rsrc.start &= ~0xfffffffful;

	/* Map them */
	regs = ioremap(rsrc.start + SPIDER_PCI_REG_BASE, 0x1000);
	if (regs == NULL) {
		printk(KERN_ERR "Failed to map registers for spider %s"
		       " workarounds disabled\n", np->full_name);
		return;
	}

	spider_pci_count++;

	/* We assume spiders only have one MMIO resource */
	bus->mmio_start = phb->mem_resources[0].start;
	bus->mmio_end = phb->mem_resources[0].end + 1;

	bus->pio_vstart = (unsigned long)phb->io_base_virt;
	bus->pio_vend = bus->pio_vstart + phb->pci_io_size;

	bus->regs = regs;

	printk(KERN_INFO "PCI: Spider MMIO workaround for %s\n",np->full_name);

	pr_debug(" mmio (P) = 0x%016lx..0x%016lx\n",
		 bus->mmio_start, bus->mmio_end);
	pr_debug("  pio (V) = 0x%016lx..0x%016lx\n",
		 bus->pio_vstart, bus->pio_vend);
	pr_debug(" regs (P) = 0x%016lx (V) = 0x%p\n",
		 rsrc.start + SPIDER_PCI_REG_BASE, bus->regs);

	spider_pci_setup_chip(bus);
}

static struct ppc_pci_io __initdata spider_pci_io = {
	.readb = spider_readb,
	.readw = spider_readw,
	.readl = spider_readl,
	.readq = spider_readq,
	.readw_be = spider_readw_be,
	.readl_be = spider_readl_be,
	.readq_be = spider_readq_be,
	.readsb = spider_readsb,
	.readsw = spider_readsw,
	.readsl = spider_readsl,
	.memcpy_fromio = spider_memcpy_fromio,
};

static int __init spider_pci_workaround_init(void)
{
	struct pci_controller *phb;

	if (!machine_is(cell))
		return 0;

	/* Find spider bridges. We assume they have been all probed
	 * in setup_arch(). If that was to change, we would need to
	 * update this code to cope with dynamically added busses
	 */
	list_for_each_entry(phb, &hose_list, list_node) {
		struct device_node *np = phb->arch_data;
		const char *model = of_get_property(np, "model", NULL);

		/* If no model property or name isn't exactly "pci", skip */
		if (model == NULL || strcmp(np->name, "pci"))
			continue;
		/* If model is not "Spider", skip */
		if (strcmp(model, "Spider"))
			continue;
		spider_pci_add_one(phb);
	}

	/* No Spider PCI found, exit */
	if (spider_pci_count == 0)
		return 0;

	/* Setup IO callbacks. We only setup MMIO reads. PIO reads will
	 * fallback to MMIO reads (though without a token, thus slower)
	 */
	ppc_pci_io = spider_pci_io;

	/* Setup ioremap callback */
	ppc_md.ioremap = spider_ioremap;

	return 0;
}
arch_initcall(spider_pci_workaround_init);
