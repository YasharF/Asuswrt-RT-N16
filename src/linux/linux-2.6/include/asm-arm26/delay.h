#ifndef __ASM_ARM_DELAY_H
#define __ASM_ARM_DELAY_H

/*
 * Copyright (C) 1995 Russell King
 *
 * Delay routines, using a pre-computed "loops_per_second" value.
 */

extern void __delay(int loops);

extern void udelay(unsigned long usecs);

static inline unsigned long muldiv(unsigned long a, unsigned long b, unsigned long c)
{
	return a * b / c;
}

	

#endif /* defined(_ARM_DELAY_H) */
