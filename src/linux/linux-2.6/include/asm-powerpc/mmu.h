#ifndef _ASM_POWERPC_MMU_H_
#define _ASM_POWERPC_MMU_H_
#ifdef __KERNEL__

#ifdef CONFIG_PPC64
/* 64-bit classic hash table MMU */
#  include <asm/mmu-hash64.h>
#elif defined(CONFIG_44x)
/* 44x-style software loaded TLB */
#  include <asm/mmu-44x.h>
#else
#  include <asm-ppc/mmu.h>
#endif

#endif /* __KERNEL__ */
#endif /* _ASM_POWERPC_MMU_H_ */
