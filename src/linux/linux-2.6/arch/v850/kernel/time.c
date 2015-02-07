/*
 * linux/arch/v850/kernel/time.c -- Arch-dependent timer functions
 *
 *  Copyright (C) 1991, 1992, 1995, 2001, 2002  Linus Torvalds
 *
 * This file contains the v850-specific time handling details.
 * Most of the stuff is located in the machine specific files.
 *
 * 1997-09-10	Updated NTP code according to technical memorandum Jan '96
 *		"A Kernel Model for Precision Timekeeping" by Dave Mills
 */

#include <linux/errno.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/param.h>
#include <linux/string.h>
#include <linux/mm.h>
#include <linux/interrupt.h>
#include <linux/time.h>
#include <linux/timex.h>
#include <linux/profile.h>

#include <asm/io.h>

#include "mach.h"

#define TICK_SIZE	(tick_nsec / 1000)

/*
 * timer_interrupt() needs to keep up the real-time clock,
 * as well as call the "do_timer()" routine every clocktick
 */
static irqreturn_t timer_interrupt (int irq, void *dummy, struct pt_regs *regs)
{

	/* may need to kick the hardware timer */
	if (mach_tick)
	  mach_tick ();

	do_timer (1);
#ifndef CONFIG_SMP
	update_process_times(user_mode(regs));
#endif
	profile_tick(CPU_PROFILING, regs);

	return IRQ_HANDLED;
}

static int timer_dev_id;
static struct irqaction timer_irqaction = {
	timer_interrupt,
	IRQF_DISABLED,
	CPU_MASK_NONE,
	"timer",
	&timer_dev_id,
	NULL
};

void time_init (void)
{
	mach_gettimeofday (&xtime);
	mach_sched_init (&timer_irqaction);
}
