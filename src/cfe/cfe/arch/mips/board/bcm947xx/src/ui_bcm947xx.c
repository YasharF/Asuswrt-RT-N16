/*  *********************************************************************
    *  Broadcom Common Firmware Environment (CFE)
    *  
    *  Test commands				File: cfe_tests.c
    *  
    *  A temporary sandbox for misc test routines and commands.
    *  
    *  Author:  Mitch Lichtenberg (mpl@broadcom.com)
    *  
    *********************************************************************  
    *
    *  Copyright 2000,2001
    *  Broadcom Corporation. All rights reserved.
    *  
    *  This software is furnished under license and may be used and 
    *  copied only in accoddance with the following terms and 
    *  conditions.  Subject to these conditions, you may download, 
    *  copy, install, use, modify and distribute modified or unmodified 
    *  copies of this software in source and/or binary form.  No title 
    *  or ownership is transferred hereby.
    *  
    *  1) Any source code used, modified or distributed must reproduce 
    *     and retain this copyright notice and list of conditions as 
    *     they appear in the source file.
    *  
    *  2) No right is granted to use any trade name, trademark, or 
    *     logo of Broadcom Corporation. Neither the "Broadcom 
    *     Corporation" name nor any trademark or logo of Broadcom 
    *     Corporation may be used to endorse or promote products 
    *     derived from this software without the prior written 
    *     permission of Broadcom Corporation.
    *  
    *  3) THIS SOFTWARE IS PROVIDED "AS-IS" AND ANY EXPRESS OR
    *     IMPLIED WARRANTIES, INCLUDING BUT NOT LIMITED TO, ANY IMPLIED 
    *     WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR 
    *     PURPOSE, OR NON-INFRINGEMENT ARE DISCLAIMED. IN NO EVENT 
    *     SHALL BROADCOM BE LIABLE FOR ANY DAMAGES WHATSOEVER, AND IN 
    *     PARTICULAR, BROADCOM SHALL NOT BE LIABLE FOR DIRECT, INDIRECT, 
    *     INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES 
    *     (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE 
    *     GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
    *     BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY 
    *     OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR 
    *     TORT (INCLUDING NEGLIGENCE OR OTHERWISE), EVEN IF ADVISED OF 
    *     THE POSSIBILITY OF SUCH DAMAGE.
    ********************************************************************* */


#include "lib_types.h"
#include "lib_string.h"
#include "lib_queue.h"
#include "lib_malloc.h"
#include "lib_printf.h"
#include "cfe.h"
#include "cfe_iocb.h"
#include "cfe_devfuncs.h"
#include "cfe_ioctl.h"
#include "cfe_error.h"
#include "cfe_fileops.h"
#include "cfe_loader.h"
#include "ui_command.h"
#include "bsp_config.h"

#include <typedefs.h>
#include <osl.h>
#include <bcmdevs.h>
#include <bcmutils.h>
#include <hndsoc.h>
#include <siutils.h>
#include <sbchipc.h>
#include <sbmemc.h>
#include <dmemc_core.h>
#include <bcmendian.h>
#include <bcmnvram.h>
#include <hndcpu.h>
#include <trxhdr.h>
#include "addrspace.h"
#include "initdata.h"

#include "bsp_priv.h"

static int
ui_cmd_reboot(ui_cmdline_t *cmd, int argc, char *argv[])
{
	hnd_cpu_reset(sih);
	return 0;
}


static int
ui_cmd_nvram(ui_cmdline_t *cmd, int argc, char *argv[])
{
	char *command, *name, *value;
	char *buf;
	size_t size;
	int ret;

	if (!(command = cmd_getarg(cmd, 0)))
		return CFE_ERR_INV_PARAM;

	if (!strcmp(command, "get")) {
		if ((name = cmd_getarg(cmd, 1)))
			if ((value = nvram_get(name)))
				printf("%s\n", value);
	} else if (!strcmp(command, "set")){
		if ((name = cmd_getarg(cmd, 1))) {
			if ((value = strchr(name, '=')))
				*value++ = '\0';
			else if ((value = cmd_getarg(cmd, 2))) {
				if (*value == '=')
					value = cmd_getarg(cmd, 3);
			}
			if (value)
				nvram_set(name, value);
		}
	} else if (!strcmp(command, "unset")) {
		if ((name = cmd_getarg(cmd, 1)))
			nvram_unset(name);
	} else if (!strcmp(command, "commit")) {
		nvram_commit();
	} else if (!strcmp(command, "erase")) {
		if ((ret = cfe_open("flash0.nvram")) < 0)
			return ret;
		if (!(buf = KMALLOC(NVRAM_SPACE, 0)))
			return CFE_ERR_NOMEM;
		memset(buf, 0xff, NVRAM_SPACE);
		cfe_writeblk(ret, 0, (unsigned char *)buf, NVRAM_SPACE);
		cfe_close(ret);
		KFREE(buf);
	} else if (!strcmp(command, "show") || !strcmp(command, "getall")) {
		if (!(buf = KMALLOC(NVRAM_SPACE, 0)))
			return CFE_ERR_NOMEM;
		nvram_getall(buf, NVRAM_SPACE);
		for (name = buf; *name; name += strlen(name) + 1)
			printf("%s\n", name);
		size = sizeof(struct nvram_header) + ((uintptr)name - (uintptr)buf);
		printf("size: %d bytes (%d left)\n", size, NVRAM_SPACE - size);
		KFREE(buf);
	}

	return 0;
}

static int
check_trx(void)
{
	int ret;
	fileio_ctx_t *fsctx;
	void *ref;
	struct trx_header trx;
	uint32 crc, buf[128];
	unsigned int len, count;

	/* Open header */
	ret = fs_init("raw", &fsctx, "flash0.trx");
	if (ret)
		return ret;

	ret = fs_open(fsctx, &ref, "", FILE_MODE_READ);
	if (ret) {
		fs_uninit(fsctx);
		return ret;
	}

	/* Read header */
	ret = fs_read(fsctx, ref, (unsigned char *) &trx, sizeof(struct trx_header));
	if (ret != sizeof(struct trx_header)) {
		ret = CFE_ERR_IOERR;
		goto done;
	}
	
	/* Verify magic number */
	if (ltoh32(trx.magic) != TRX_MAGIC) {
		ret = CFE_ERR_INVBOOTBLOCK;
		goto done;
	}

	/* Checksum over header */
	crc = hndcrc32((uint8 *) &trx.flag_version,
		    sizeof(struct trx_header) - OFFSETOF(struct trx_header, flag_version),
		    CRC32_INIT_VALUE);

	fs_close(fsctx, ref);
	fs_uninit(fsctx);

	/* Open data */
	ret = fs_init("raw", &fsctx, "flash0.os");
	if (ret)
		return ret;

	ret = fs_open(fsctx, &ref, "", FILE_MODE_READ);
	if (ret) {
		fs_uninit(fsctx);
		return ret;
	}

	for (len = ltoh32(trx.len) - sizeof(struct trx_header); len; len -= count) {
		count = MIN(len, sizeof(buf));

		/* Read data */
		ret = fs_read(fsctx, ref, (unsigned char *) &buf, count);
		if (ret != count) {
			ret = CFE_ERR_IOERR;
			goto done;
		}

		/* Checksum over data */
		crc = hndcrc32((uint8 *) &buf, count, crc);
	}

	/* Verify checksum */
	if (ltoh32(trx.crc32) != crc) {
		ret = CFE_ERR_BOOTPROGCHKSUM;
		goto done;
	}

	ret = 0;

 done:
	fs_close(fsctx, ref);
	fs_uninit(fsctx);
	if (ret)
		xprintf("%s\n", cfe_errortext(ret));
	return ret;
}

/*  *********************************************************************
    *  ui_get_loadbuf(bufptr, bufsize)
    *  
    *  Figure out the location and size of the staging buffer.
    *  
    *  Input parameters:
    *	   bufptr - address to return buffer location
    *	   bufsize - address to return buffer size
    ********************************************************************* */
static void ui_get_loadbuf(uint8_t **bufptr, int *bufsize)
{
	int size = CFG_FLASH_STAGING_BUFFER_SIZE;

	/*	
	 * Get the address of the staging buffer.  We can't
	 * allocate the space from the heap to store the 
	 * new flash image, because the heap may not be big
	 * enough.  So, if FLASH_STAGING_BUFFER_SIZE is non-zero
	 * then just use it and FLASH_STAGING_BUFFER; else
	 * use the larger of (mem_bottomofmem - FLASH_STAGING_BUFFER)
	 * and (mem_totalsize - mem_topofmem).
	 */

	if (size > 0) {
		*bufptr = (uint8_t *) KERNADDR(CFG_FLASH_STAGING_BUFFER_ADDR);
		*bufsize = size;
	} else {
		int below, above;

		below = PHYSADDR(mem_bottomofmem) - CFG_FLASH_STAGING_BUFFER_ADDR;
		above = (mem_totalsize << 10) - PHYSADDR(mem_topofmem);

		if (below > above) {
			*bufptr = (uint8_t *) KERNADDR(CFG_FLASH_STAGING_BUFFER_ADDR);
			*bufsize = below;
		} else {
			*bufptr = (uint8_t *) KERNADDR(mem_topofmem);
			*bufsize = above;
		}
	}
}

static int
ui_cmd_go(ui_cmdline_t *cmd, int argc, char *argv[])
{
	int ret = 0;
	char buf[512];
	struct trx_header *file_buf;
	uint8_t *ptr;
	char *val;
	uint32 osaddr;
	int bufsize = 0;
	int retry = 0;

	val = nvram_get("os_ram_addr");
	if (val)
		osaddr = bcm_strtoul(val, NULL, 16);
	else
		osaddr = 0x80001000;

	if (check_trx()) {
		/* Wait for CFE_ERR_TIMEOUT_LIMIT for an image */
		while (1) {
			if ((ret = ui_docommand("flash -noheader : flash1.trx")) != CFE_ERR_TIMEOUT)
				break;
			if (++retry == CFE_ERR_TIMEOUT_LIMIT) {
				ret = CFE_ERR_INTR;
				break;
			}
		}
	} else if (!nvram_invmatch("boot_wait", "on")) {
		ui_get_loadbuf(&ptr, &bufsize);
		/* Load the image */
		sprintf(buf, "load -raw -addr=0x%x -max=0x%x :", ptr, bufsize);
		ret = ui_docommand(buf);

		/* Load was successful. Check for the TRX magic.
		 * If it's a TRX image, then proceed to flash it, else try to boot
		 * Note: To boot a TRX image directly from the memory, address will need to be
		 * load address + trx header length.
		 */
		if (ret == 0) {
			file_buf = (struct trx_header *)ptr;
			/* If it's a TRX, then proceed to writing to flash else,
			 * try to boot from memory
			 */
			if (file_buf->magic != TRX_MAGIC) {
				sprintf(buf, "boot -raw -z -addr=0x%x -max=0x%x -fs=memory :0x%x",
				        osaddr, bufsize, ptr);
				return ui_docommand(buf);
			}
			/* Flash the image from memory directly */
			sprintf(buf, "flash -noheader -mem -size=0x%x 0x%x flash1.trx",
			        file_buf->len, ptr);
			ret = ui_docommand(buf);
		}
	}

	if (ret == CFE_ERR_INTR)
		return ret;

	/* Boot the image */
	bufsize = PHYSADDR(mem_bottomofmem) - PHYSADDR(osaddr);
	sprintf(buf, "boot -raw -z -addr=0x%x -max=0x%x flash0.os:", osaddr, bufsize);
	return ui_docommand(buf);
}

static int
ui_cmd_clocks(ui_cmdline_t *cmd, int argc, char *argv[])
{
	chipcregs_t *cc = (chipcregs_t *)si_setcoreidx(sih, SI_CC_IDX);
	uint32 ccc = R_REG(NULL, &cc->capabilities);
	uint32 pll = ccc & CC_CAP_PLL_MASK;

	if (pll != PLL_NONE) {
		uint32 n = R_REG(NULL, &cc->clockcontrol_n);
		printf("Current clocks for pll=0x%x:\n", pll);
		printf("    mips: %d\n", si_clock_rate(pll, n, R_REG(NULL, &cc->clockcontrol_m3)));
		printf("    sb: %d\n", si_clock_rate(pll, n, R_REG(NULL, &cc->clockcontrol_sb)));
		printf("    pci: %d\n", si_clock_rate(pll, n, R_REG(NULL, &cc->clockcontrol_pci)));
		printf("    mipsref: %d\n", si_clock_rate(pll, n, R_REG(NULL, &cc->clockcontrol_m2)));
	} else {
		printf("Current clocks: %d/%d/%d/%d Mhz.\n",
		       si_cpu_clock(sih) / 1000000,
		       si_mem_clock(sih) / 1000000,
		       si_clock(sih) / 1000000,
		       si_alp_clock(sih) / 1000000);
	}
	return 0;
}


int
ui_init_bcm947xxcmds(void)
{
	cmd_addcmd("reboot",
		   ui_cmd_reboot,
		   NULL,
		   "Reboot.",
		   "reboot\n\n"
		   "Reboots.",
		   "");
	cmd_addcmd("nvram",
		   ui_cmd_nvram,
		   NULL,
		   "NVRAM utility.",
		   "nvram [command] [args..]\n\n"
		   "Access NVRAM.",
		   "get [name];Gets the value of the specified variable|"
		   "set [name=value];Sets the value of the specified variable|"
		   "unset [name];Deletes the specified variable|"
		   "commit;Commit variables to flash|"
		   "erase;Erase all nvram|"
		   "show;Shows all variables|");
	cmd_addcmd("go",
		   ui_cmd_go,
		   NULL,
		   "Verify and boot OS image.",
		   "go\n\n"
		   "Boots OS image if valid. Waits for a new OS image if image is invalid\n"
		   "or boot_wait is unset or not on.",
		   "");
	cmd_addcmd("show clocks",
		   ui_cmd_clocks,
		   NULL,
		   "Show current values of the clocks.",
		   "show clocks\n\n"
		   "Shows the current values of the clocks.",
		   "");
#if CFG_ET
	et_addcmd();
#endif
#if CFG_WLU
	wl_addcmd();
#endif

	return 0;
}
