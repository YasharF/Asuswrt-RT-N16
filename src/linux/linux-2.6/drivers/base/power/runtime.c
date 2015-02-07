/*
 * drivers/base/power/runtime.c - Handling dynamic device power management.
 *
 * Copyright (c) 2003 Patrick Mochel
 * Copyright (c) 2003 Open Source Development Lab
 *
 */

#include <linux/device.h>
#include "power.h"


static void runtime_resume(struct device * dev)
{
	dev_dbg(dev, "resuming\n");
	if (!dev->power.power_state.event)
		return;
	if (!resume_device(dev))
		dev->power.power_state = PMSG_ON;
}



void dpm_runtime_resume(struct device * dev)
{
	down(&dpm_sem);
	runtime_resume(dev);
	up(&dpm_sem);
}
EXPORT_SYMBOL(dpm_runtime_resume);


/**
 *	dpm_runtime_suspend - Put one device in low-power state.
 *	@dev:	Device.
 *	@state:	State to enter.
 */

int dpm_runtime_suspend(struct device * dev, pm_message_t state)
{
	int error = 0;

	down(&dpm_sem);
	if (dev->power.power_state.event == state.event)
		goto Done;

	if (dev->power.power_state.event)
		runtime_resume(dev);

	if (!(error = suspend_device(dev, state)))
		dev->power.power_state = state;
 Done:
	up(&dpm_sem);
	return error;
}
EXPORT_SYMBOL(dpm_runtime_suspend);
