
		BCM47xx/BCM535x Linux Software and Tools Readme
			


This file documents the directory structure and contents of the Software
and Tools for the BCM47xx/BCM535x family. It also tells how to install
the tools. The development environment is assumed to be PC's running
Linux, RedHat8, RedHat9 and Fedora3 to be more precise. It is possible that
the tools will run on other Linux distributions, but as of now we have only
tested on RedHat8, RedHat9 and Fedora3.

Depending on the release type received, you may or may not have source code
to all components.

1. Installing the tools.

   To install the BCM47xx/535x software tools, the "3.2.3 47xx_535x MIPS
   Cross Compiler and Tools" package is required, and must be obtained
   separately from the Broadcom Customer Support Portal.

   Once you have installed the tools, you should be able to compile C
   programs and build the source distribution. You can test that by
   going to the src directory and typing "make".


2. Contents of the distribution.

   src/linux/linux

	This directory contains a full copy of our Linux kernel sources.

	The bulk of the kernel is based on linux-2.4.20 as modified for
	MIPS CPU's and maintained at linux-mips.org. We are currently using
	a snapshot from the oss tree as of 2003-02-03.

	There are two areas where we have updated beyond that oss
	snapshot:
	    o HNBU has added the necessary low level interface support
	      for MTD support (in drivers/mtd) in files prefixed with
	      "bcm947xx".
	    o PCMCIA card services 3.1.29 from sourceforge.net
	    o Intersil 802.11b drivers 2.4.14 from kernel.org


	The BCM47xx/BCM535x specific code can be found mainly under the
	directory arch/mips/brcm-boards/bcm947xx. The network drivers are
	in separate directories described below.

   src/et

	Sources for the 10/100 Ethernet driver.

   src/cfe

	Source for the boot loader "cfe". This code has been developed by 
	Broadcom. It is a replacement for "pmon". It is more compact, 
	contains greater functionality and is more efficient than "pmon".

   src/router

	User level code for HNBU's reference WAN Agnostic Residential
	Gateway. It includes code written by HNBU plus the following:
	    o busybox-0.60.0 from opensource.lineo.com
	    o ppp-2.4.1-pppoe2 from www.shoshin.uwaterloo.ca/~mostrous
	      with significant modifications by HNBU
	    o udhcp-0.9.5 from opensource.lineo.com

   src/rts

	Sources for utilities.

   src/ses

	Sources/objects for the SES.

   src/shared
   src/include

	Common code and include files used by all of the above.

   src/usbdev

	Sources/objects for the WLAN USB driver.

   src/wl

	Sources/objects for the WLAN driver.

   image

	Contains a binary image for the combined kernel and root file system
	processed with gzip and trx that can be burned directly into
	flash on a BCM947xx/BCM9534x board. Also contains binary images for
	pmon, rnd cfe with additional text files for the supported boards.
	Either cfe or pmon can be used in with the approriate text file to
	create an image that can be burned into flash on any of the supported
	BCM947xx/BCM9534x board.

   tools/visionICE

	Files needed to use WindRiver visionICE with the BCM4710.
