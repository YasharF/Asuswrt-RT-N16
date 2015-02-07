#ifndef _SPARC_TLBFLUSH_H
#define _SPARC_TLBFLUSH_H

#include <linux/mm.h>
// #include <asm/processor.h>


#ifdef CONFIG_SMP

BTFIXUPDEF_CALL(void, local_flush_tlb_all, void)
BTFIXUPDEF_CALL(void, local_flush_tlb_mm, struct mm_struct *)
BTFIXUPDEF_CALL(void, local_flush_tlb_range, struct vm_area_struct *, unsigned long, unsigned long)
BTFIXUPDEF_CALL(void, local_flush_tlb_page, struct vm_area_struct *, unsigned long)

#define local_flush_tlb_all() BTFIXUP_CALL(local_flush_tlb_all)()
#define local_flush_tlb_mm(mm) BTFIXUP_CALL(local_flush_tlb_mm)(mm)
#define local_flush_tlb_range(vma,start,end) BTFIXUP_CALL(local_flush_tlb_range)(vma,start,end)
#define local_flush_tlb_page(vma,addr) BTFIXUP_CALL(local_flush_tlb_page)(vma,addr)

extern void smp_flush_tlb_all(void);
extern void smp_flush_tlb_mm(struct mm_struct *mm);
extern void smp_flush_tlb_range(struct vm_area_struct *vma,
				  unsigned long start,
				  unsigned long end);
extern void smp_flush_tlb_page(struct vm_area_struct *mm, unsigned long page);

#endif /* CONFIG_SMP */

BTFIXUPDEF_CALL(void, flush_tlb_all, void)
BTFIXUPDEF_CALL(void, flush_tlb_mm, struct mm_struct *)
BTFIXUPDEF_CALL(void, flush_tlb_range, struct vm_area_struct *, unsigned long, unsigned long)
BTFIXUPDEF_CALL(void, flush_tlb_page, struct vm_area_struct *, unsigned long)

// Thanks to Anton Blanchard, our pagetables became uncached in 2.4. Wee!
// extern void flush_tlb_pgtables(struct mm_struct *mm,
//     unsigned long start, unsigned long end);
#define flush_tlb_pgtables(mm, start, end)	do{ }while(0)

#define flush_tlb_all() BTFIXUP_CALL(flush_tlb_all)()
#define flush_tlb_mm(mm) BTFIXUP_CALL(flush_tlb_mm)(mm)
#define flush_tlb_range(vma,start,end) BTFIXUP_CALL(flush_tlb_range)(vma,start,end)
#define flush_tlb_page(vma,addr) BTFIXUP_CALL(flush_tlb_page)(vma,addr)

// #define flush_tlb() flush_tlb_mm(current->active_mm)

#define flush_tlb_kernel_range(start, end) flush_tlb_all()

#endif /* _SPARC_TLBFLUSH_H */
