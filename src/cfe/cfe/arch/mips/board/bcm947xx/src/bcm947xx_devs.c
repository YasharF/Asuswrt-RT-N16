/*  *********************************************************************
    *  Broadcom Common Firmware Environment (CFE)
    *  
    *  Board device initialization		File: bcm947xx_devs.c
    *  
    *  This is the "C" part of the board support package.  The
    *  routines to create and initialize the console, wire up 
    *  device drivers, and do other customization live here.
    *  
    *  Author:  Mitch Lichtenberg (mpl@broadcom.com)
    *  
    *********************************************************************  
    *
    *  XX Copyright 2000,2001
    *  Broadcom Corporation. All rights reserved.
    *
    *  BROADCOM PROPRIETARY AND CONFIDENTIAL
    *  
    *  This software is furnished under license and may be used and 
    *  copied only in accordance with the license.
    ********************************************************************* */



#include "sbmips.h"
#include "lib_types.h"
#include "lib_printf.h"
#include "lib_physio.h"
#include "cfe_iocb.h"
#include "cfe_device.h"
#include "cfe_timer.h"
#include "cfe.h"
#include "ui_command.h"
#include "bsp_config.h"
#include "dev_newflash.h"
#include "env_subr.h"
#include "pcivar.h"
#include "pcireg.h"
#include "../../../../../dev/ns16550.h"
#include "net_ebuf.h"
#include "net_ether.h"
#include "net_api.h"

#include <typedefs.h>
#include <osl.h>
#include <bcmutils.h>
#include <hndsoc.h>
#include <siutils.h>
#include <sbchipc.h>
#include <hndpci.h>
#include "bsp_priv.h"
#include <trxhdr.h>
#include <bcmdevs.h>
#include <bcmnvram.h>
#include <hndcpu.h>
#include <hndchipc.h>
#include <epivers.h>

#define MAX_WAIT_TIME 20	/* seconds to wait for boot image */
#define MIN_WAIT_TIME 3 	/* seconds to wait for boot image */

#define RESET_DEBOUNCE_TIME	(500*1000)	/* 500 ms */

/* Defined as sih by bsp_config.h for convenience */
si_t *bcm947xx_sih = NULL;

/* Configured devices */
#if (CFG_FLASH || CFG_SFLASH) && CFG_XIP
#error "XIP and Flash cannot be defined at the same time"
#endif

extern cfe_driver_t ns16550_uart;
#if CFG_FLASH
extern cfe_driver_t newflashdrv;
#endif
#if CFG_SFLASH
extern cfe_driver_t sflashdrv;
#endif
#if CFG_ET
extern cfe_driver_t bcmet;
#endif
#if CFG_WL
extern cfe_driver_t bcmwl;
#endif
#if CFG_BCM57XX
extern cfe_driver_t bcm5700drv;
#endif

/* Reset NVRAM */
static int restore_defaults = 0;

static void
board_console_add(void *regs, uint irq, uint baud_base, uint reg_shift)
{
	physaddr_t base;

	/* The CFE NS16550 driver expects a physical address */
	base = PHYSADDR((physaddr_t) regs);
	cfe_add_device(&ns16550_uart, base, baud_base, &reg_shift);
}

#if CFG_FLASH || CFG_SFLASH
#if !CFG_SIM
static void
reset_release_wait (void)
{
	int gpio;

	if ((gpio = nvram_resetgpio_init ((void *)sih)) < 0)
		return;

	while (1) {
		/* Reset button is active low */
		if (si_gpioin(sih) & ((uint32)1 << gpio)) {
			OSL_DELAY(RESET_DEBOUNCE_TIME);

			if (si_gpioin(sih) & ((uint32)1 << gpio))
				break;
		}
	} 
}
#endif
#endif

/*  *********************************************************************
    *  board_console_init()
    *  
    *  Add the console device and set it to be the primary
    *  console.
    *  
    *  Input parameters: 
    *  	   nothing
    *  	   
    *  Return value:
    *  	   nothing
    ********************************************************************* */

void
board_console_init(void)
{
#if !CFG_SIM
	unsigned long mipsclock = 0, siclock = 0, pciclock = 0;
	char *clkfreq;
#endif

#if !CFG_MINIMAL_SIZE
	cfe_set_console(CFE_BUFFER_CONSOLE);
#endif

	/* Initialize SB access */
	sih = si_kattach(SI_OSH);
	ASSERT(sih);

	/* Figure out current MIPS clock speed */
	if ((cfe_cpu_speed = si_cpu_clock(sih)) == 0)
		cfe_cpu_speed = 100000000;

#if !CFG_SIM
	/* Check whether NVRAM reset needs be done */
	if (nvram_reset((void *)sih) > 0)
		restore_defaults = 1;
#endif
	
	/* Initialize NVRAM access accordingly. In case of invalid NVRAM, load defaults */
	if (nvram_init((void *)sih) > 0)
		restore_defaults = 1;

#if CFG_SIM
	restore_defaults = 0;
#else /* CFG_SIM */
	if (!restore_defaults) {
		char *end;

		/* MIPS clock speed override */
		if ((clkfreq = nvram_get("clkfreq"))) {
			mipsclock = bcm_strtoul(clkfreq, &end, 0) * 1000000;
			if (*end == ',') {
				clkfreq = ++end;
				siclock = bcm_strtoul(clkfreq, &end, 0) * 1000000;
				if (*end == ',') {
					clkfreq = ++end;
					pciclock = bcm_strtoul(clkfreq, &end, 0) * 1000000;
				}
			}
		}

		if (mipsclock) {
			/* Set current MIPS clock speed */
			si_mips_setclock(sih, mipsclock, siclock, pciclock);
		}
	}
#endif

	if ((CHIPID(sih->chip) == BCM4716_CHIP_ID) ||
	    (CHIPID(sih->chip) == BCM47162_CHIP_ID)) {
		uint32 reg, new;
		char *boardpwrctl;
		uint origidx = si_coreidx(sih);
		chipcregs_t *cc = si_setcoreidx(sih, SI_CC_IDX);

		/* Adjust regulator settings */
		W_REG(osh, &cc->regcontrol_addr, 2);
		/* Readback to ensure completion of the write */
		(void)R_REG(osh, &cc->regcontrol_addr);
		reg = R_REG(osh, &cc->regcontrol_data);
		/* Make the regulator frequency to 1.2MHz
		 *   00 1.2MHz
		 *   01 200kHz
		 *   10 600kHz
		 *   11 2.4MHz
		 */
		reg &= ~0x00c00000;
		/* Take 2.5v regulator output down one notch,
		 * officially to 2.45, but in reallity to be
		 * closer to 2.5 than the default.
		 */
		reg |= 0xf0000000;

		/* Bits corresponding to mask 0x00078000
		 * controls 1.3v source
		 *	Value           Voltage
	         *	========================
	         *	0xf0000000  1.2 V (default)
		 * 	0xf0008000  0.975
		 *	0xf0010000  1
		 *	0xf0018000  1.025
		 *	0xf0020000  1.05
		 *	0xf0028000  1.075
		 *	0xf0030000  1.1
		 *	0xf0038000  1.125
		 *	0xf0040000  1.15
		 *	0xf0048000  1.175
		 *	0xf0050000  1.225
		 *	0xf0058000  1.25
		 *	0xf0060000  1.275
		 *	0xf0068000  1.3
		 *	0xf0070000  1.325
		 *	0xf0078000  1.35
		 */
		if ((boardpwrctl = nvram_get("boardpwrctl"))) {
			uint32 pwrctl = bcm_strtoul(boardpwrctl, NULL, 0);

			reg &= ~0xf0c78000;
			reg |= (pwrctl & 0xf0c78000);
		}

		W_REG(osh, &cc->regcontrol_data, reg);

		/* Turn off unused PLLs */
		W_REG(osh, &cc->pllcontrol_addr, 6);
		(void)R_REG(osh, &cc->pllcontrol_addr);
		new = reg = R_REG(osh, &cc->pllcontrol_data);
		if (sih->chippkg == BCM4716_PKG_ID)
			new |= 0x68;	/* Channels 3, 5 & 6 off in 4716 */
		if ((sih->chipst & 0x00000c00) == 0x00000400)
			new |= 0x10;	/* Channel 4 if MII mode */
		if (new != reg) {
			/* apply new value */
			W_REG(osh, &cc->pllcontrol_data, new);
			(void)R_REG(osh, &cc->pllcontrol_data);
			W_REG(osh, &cc->pmucontrol,
			      PCTL_PLL_PLLCTL_UPD | PCTL_NOILP_ON_WAIT |
			      PCTL_HT_REQ_EN | PCTL_ALP_REQ_EN | PCTL_LPO_SEL);
		}
		si_setcoreidx(sih, origidx);
	}

	/* Initialize clocks and interrupts */
	si_mips_init(sih, 0);

	/* Initialize UARTs */
	si_serial_init(sih, board_console_add);

	if (cfe_finddev("uart0"))
		cfe_set_console("uart0");
}

#if CFG_FLASH || CFG_SFLASH
static void
flash_init(void)
{
	newflash_probe_t fprobe;
	chipcregs_t *cc = NULL;
	uint32 fltype, bootsz, *bisz;
	cfe_driver_t *drv;

	memset(&fprobe, 0, sizeof(fprobe));

	if ((cc = (chipcregs_t *)si_setcoreidx(sih, SI_CC_IDX))) {
		fltype = R_REG(NULL, &cc->capabilities) & CC_CAP_FLASH_MASK;
		fprobe.flash_phys = SI_FLASH2;
	} else
		return;

	switch (fltype) {
#if CFG_FLASH
	case PFLASH:
		drv = &newflashdrv;
		fprobe.flash_flags = FLASH_FLG_BUS16 | FLASH_FLG_DEV16;
		if (!(R_REG(NULL, &cc->flash_config) & CC_CFG_DS))
			fprobe.flash_flags = FLASH_FLG_BUS8 | FLASH_FLG_DEV16;
		break;
#endif
#if CFG_SFLASH
	case SFLASH_ST:
	case SFLASH_AT:
		ASSERT(cc);
		drv = &sflashdrv;
		/* Overload cmdset field */
		fprobe.flash_cmdset = (int)cc;
		break;
#endif
	default:
		/* No flash or unsupported flash */
		return;
	}

	/* Default is 256K boot partition */
	bootsz = 256 * 1024;

	/* Do we have a self-describing binary image? */
	bisz = (uint32 *)PHYS_TO_K1(fprobe.flash_phys + BISZ_OFFSET);
	if (bisz[BISZ_MAGIC_IDX] == BISZ_MAGIC) {
		int isz = bisz[BISZ_DATAEND_IDX] - bisz[BISZ_TXTST_IDX];

		if (isz < (128 * 1024))
			bootsz = 128 * 1024;

	}
	printf("Boot partition size = %d(0x%x)\n", bootsz, bootsz);

	/* Because CFE can only boot from the beginning of a partition */
	fprobe.flash_nparts = 4;
	fprobe.flash_parts[0].fp_size = bootsz;
	fprobe.flash_parts[0].fp_name = "boot";
	fprobe.flash_parts[1].fp_size = sizeof(struct trx_header);
	fprobe.flash_parts[1].fp_name = "trx";
	fprobe.flash_parts[2].fp_size = 0;
	fprobe.flash_parts[2].fp_name = "os";
	fprobe.flash_parts[3].fp_size = NVRAM_SPACE;
	fprobe.flash_parts[3].fp_name = "nvram";
	cfe_add_device(drv, 0, 0, &fprobe);

	/* Because CFE can only flash an entire partition */
	fprobe.flash_nparts = 3;
	fprobe.flash_parts[0].fp_size = bootsz;
	fprobe.flash_parts[0].fp_name = "boot";
	fprobe.flash_parts[1].fp_size = 0;
	fprobe.flash_parts[1].fp_name = "trx";
	fprobe.flash_parts[2].fp_size = NVRAM_SPACE;
	fprobe.flash_parts[2].fp_name = "nvram";
	cfe_add_device(drv, 0, 0, &fprobe);

	/* Because sometimes we want to program the entire device */
	fprobe.flash_nparts = 0;
	cfe_add_device(drv, 0, 0, &fprobe);
}
#endif

/*  *********************************************************************
    *  board_device_init()
    *  
    *  Initialize and add other devices.  Add everything you need
    *  for bootstrap here, like disk drives, flash memory, UARTs,
    *  network controllers, etc.
    *  
    *  Input parameters: 
    *  	   nothing
    *  	   
    *  Return value:
    *  	   nothing
    ********************************************************************* */

void
board_device_init(void)
{
	unsigned int unit;
#if CFG_ET || CFG_WL || CFG_BCM57XX
	void *regs;
#endif

	/* Set by board_console_init() */
	ASSERT(sih);

#if CFG_FLASH || CFG_SFLASH
	flash_init();
#endif

	for (unit = 0; unit < SI_MAXCORES; unit++) {
#if CFG_ET
		if ((regs = si_setcore(sih, ENET_CORE_ID, unit)))
			cfe_add_device(&bcmet, BCM47XX_ENET_ID, unit, regs);

#if CFG_GMAC
		if ((regs = si_setcore(sih, GMAC_CORE_ID, unit)))
			cfe_add_device(&bcmet, BCM47XX_GMAC_ID, unit, regs);
#endif
#endif
#if CFG_WL
		if ((regs = si_setcore(sih, D11_CORE_ID, unit)))
			cfe_add_device(&bcmwl, BCM4306_D11G_ID, unit, regs);
#endif
#if CFG_BCM57XX
		if((regs = si_setcore(sih, GIGETH_CORE_ID, unit)))
			cfe_add_device(&bcm5700drv, BCM47XX_GIGETH_ID, unit, regs);
#endif
	}
}

/*  *********************************************************************
    *  board_device_reset()
    *  
    *  Reset devices.  This call is done when the firmware is restarted,
    *  as might happen when an operating system exits, just before the
    *  "reset" command is applied to the installed devices.   You can
    *  do whatever board-specific things are here to keep the system
    *  stable, like stopping DMA sources, interrupts, etc.
    *  
    *  Input parameters: 
    *  	   nothing
    *  	   
    *  Return value:
    *  	   nothing
    ********************************************************************* */

void
board_device_reset(void)
{
}

/*  *********************************************************************
    *  board_final_init()
    *  
    *  Do any final initialization, such as adding commands to the
    *  user interface.
    *
    *  If you don't want a user interface, put the startup code here.  
    *  This routine is called just before CFE starts its user interface.
    *  
    *  Input parameters: 
    *  	   nothing
    *  	   
    *  Return value:
    *  	   nothing
    ********************************************************************* */

void
board_final_init(void)
{
	char *addr, *mask, *wait_time;
	char buf[512], *cur = buf;
	char *boot_cfg = NULL;
	char *go_cmd = "go;";
	int commit = 0;
	uint32 ncdl;
#if CFG_WL && CFG_WLU && CFG_SIM
	char *ssid;
#endif

	ui_init_bcm947xxcmds();

	/* Force commit of embedded NVRAM */
	commit = restore_defaults;

	/* Set the SDRAM NCDL value into NVRAM if not already done */
	if ((getintvar(NULL, "sdram_ncdl") == 0) &&
	    ((ncdl = si_memc_get_ncdl(sih)) != 0)) {
		sprintf(buf, "0x%x", ncdl);
		nvram_set("sdram_ncdl", buf);
		commit = 1;
	}

	/* Set the bootloader version string if not already done */
	sprintf(buf, "CFE %s", EPI_VERSION_STR);
	if (strcmp(nvram_safe_get("pmon_ver"), buf) != 0) {
		nvram_set("pmon_ver", buf);
		commit = 1;
	}

#if CFG_FLASH || CFG_SFLASH
#if !CFG_SIM
	/* Commit NVRAM */
	if (commit) {
		printf("Committing NVRAM...");
		nvram_commit();
		printf("done\n");
		if (restore_defaults) {
			printf ("Waiting for reset button release...");
			reset_release_wait ();
			printf("done\n");
		}
	}
#endif

#if !CFG_SIM
	/* Reboot after restoring defaults */
	if (restore_defaults)
		si_watchdog(sih, 1);
#endif	/* !CFG_SIM */
#else
	if (commit)
		printf("Flash not configured, not commiting NVRAM...\n");
#endif

	/*
	 * Read the wait_time NVRAM variable and set the tftp max retries.
	 * Assumption: tftp_rrq_timeout & tftp_recv_timeout are set to 1sec.
	 */
	if ((wait_time = nvram_get("wait_time")) != NULL) {
		tftp_max_retries = atoi(wait_time);
		if (tftp_max_retries > MAX_WAIT_TIME)
			tftp_max_retries = MAX_WAIT_TIME;
		else if (tftp_max_retries < MIN_WAIT_TIME)
			tftp_max_retries = MIN_WAIT_TIME;
	}

	/* Configure network */
	if (cfe_finddev("eth0")) {
		if ((addr = nvram_get("lan_ipaddr")) &&
		    (mask = nvram_get("lan_netmask")))
			sprintf(buf, "ifconfig eth0 -addr=%s -mask=%s",
			        addr, mask);
		else
			sprintf(buf, "ifconfig eth0 -auto");

		ui_docommand(buf);
	}
#if CFG_WL && CFG_WLU && CFG_SIM
	if ((ssid = nvram_get("wl0_ssid"))) {
		sprintf(buf, "wl join %s", ssid);
		ui_docommand(buf);
	}
#endif

#if !CFG_SIM
	/* Try to run boot_config command if configured.
	 * make sure to leave space for "go" command.
	 */
	if ((boot_cfg = nvram_get("boot_config"))) {
		if (strlen(boot_cfg) < (sizeof(buf) - sizeof(go_cmd)))
			cur += sprintf(cur, "%s;",boot_cfg);
		else
			printf("boot_config too long, skipping to autoboot\n");
	}

	/* Boot image */
	cur += sprintf(cur, go_cmd);
#endif	/* !CFG_SIM */

	/* Startup */
	if (cur > buf)
		env_setenv("STARTUP", buf, ENV_FLG_NORMAL);
}
