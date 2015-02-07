/*
 * ----------------------------------------------------------------------
 * ChangeList:
 * 2000/12/21			Vincent 002
 *   Add _PRINTER_FILTER_ define.
 *   We need to read the "printer_client=laserwriter" from "/etc/CGI_ds.conf".
 * If it were "laserwriter", we would write ":if=/share/spool/filter:" in
 * "/etc/printcap" file. Otherwise, we need to remove this line.
 *   This action was used to be done by the Perl CGI program. Now the C CGI
 * won't do this action. We will do it in the Samba/Netatalk Module.
 * 2002/5/3			Vincent 001
 *   Add _LINKSYS_G2_ define. If Linksys G2 server, use LKGxxxxxx as its default
 * server name. (Also default printer name)
 * ----------------------------------------------------------------------
 */
#ifndef _NET_SAMBA_
#define _NET_SAMBA_

/* ---------- Global Parameter ----------*/
#ifndef _NET_SMBERRNO_
#define _NET_SMBERRNO_

#define SMB_OK            0     	/* action succeed */
#define SMB_ERROR        -1		/* action fail */
#define SMB_EFILEOPEN    -2		/* file open error */
#define SMB_EFILEREAD    -3     	/* file read error */
#define SMB_EFILEWRITE   -4     	/* file write error */
#define SMB_ESECTION     -5		/* input section error */
#define SMB_EFIELD       -6		/* input field error */
#define SMB_ELEN         -7		/* input length error */
#define SMB_EWGROUP      -8     	/* workgroup invalid */
#define SMB_EPRINTER     -9     	/* printer parameter invalid */

#endif

/* ---------- Global Parameter ----------*/
#include "NET_global.h"		/* our global definition */

#define DEF_SMB_WORKGROUP       "workgroup"     /* default value */
/* FIXME: The default printer name should be the same as our 1st boot setting: SCxxxxxx_p1. "lp" is for backup reason. (If we can't get DS default server name.) It should declare as the same as Netatalk Module. (NET_netatalk.h) */
#ifdef _LINKSYS_G2_		/* Tony 001 - add #ifdef _LINKSYS_G2_ */
#define DEF_SMB_PRINTERNAME_LEN  12             /* LKGxxxxxx_p1 */
#else
#define DEF_SMB_PRINTERNAME_LEN  11             /* SCxxxxxx_p1 */
#endif
#define DEF_SMB_PRINTERNAME     "lp"
#ifdef _FW_MACNICA_
#define DEF_SMB_CODEPAGE        "932"           /* Japanese */
#else
#define DEF_SMB_CODEPAGE        "437"           /* English */
#endif
#define DEF_SMB_RESOLVEORDER    "wins bcast"
				/* files: configuration */
#define SMB_SAMBA_CONF    "/etc/samba/smb.conf"
#define SMB_PRINTCAP_CONF "/etc/printcap"

				/* files: executive */
#ifndef _OFF_STDOUT_
#define SMB_SAMBA_EXE "/etc/rc.d/rc.samba"
#else
#define SMB_SAMBA_EXE "/etc/rc.d/rc.samba &>/dev/null"
#endif

/* ---------- Structure Declaration ---------- */

/* ----- Samba information ----- */
#define SMB_COMMENT_LEN      48
#define SMB_WORKGROUP_LEN    15		/* linux support up to 16 */
#define SMB_CODEPAGE_LEN     5		/* dhcp or none/static */
/* the following 2 declarations MUST be the same in samba/netatalk */
#define SMB_PRINTER_LEN  12	/* it MUST be the same in samba/netatalk */
#define SMB_PRINTERCOMMENT_LEN 80       /* comment for SMB information */
#ifndef	NO_PRINTER
	#define NO_PRINTER
#endif

typedef struct {
	char SAMBA[STR_BOOLEAN_LEN+1];            /* msn_enable */
	char COMMENT[SMB_COMMENT_LEN+1];          /* disk_server_comment */
	char WORKGROUP[SMB_WORKGROUP_LEN+1];      /* w_d_name */
	char CODEPAGE[SMB_CODEPAGE_LEN+1];        /* code_page */
#ifndef	NO_PRINTER
	char PRINTERNAME[SMB_PRINTER_LEN+1];      /* print_name */
	char SMBPRINTERCOMMENT[SMB_PRINTERCOMMENT_LEN+1]; /* print_comment */
#endif
	char SMBWINSSERVER[STR_IPADDR_LEN+1];     /* wins_server */
} SMBInfo;

#ifdef _PRINTER_FILTER_		/* Vincent 002 - add #ifdef _PRINTER_FILTER_ */
/* The following declaration MUST be the same as NET_netatalk.h */
#define SMB_APPLE_LASERWRITER "LaserWriter"
#define SMB_MAC_PRINTNAME_LEN 32	    /* LaserWriter or OtherPrn */

typedef struct {
	char SMBMACPRINTERNAME[SMB_MAC_PRINTNAME_LEN+1]; /* printer_client */
} SMBPrinterInfo;
int SMBWritePrinterFilterData(SMBPrinterInfo *info);
#endif

/* ----------------------------------------------------------------------*/
/* purpose : Set samba  daemon                                           */
/* return : 0 -success                                                   */
/*          others - failed                                              */
/* Note : Enable/Disable samba daemon, pass "enable" or "disable".       */
/*        If pass "yes" or "no",it will return SMB_ERROR(-1).            */
/*-----------------------------------------------------------------------*/
int SMBSetSamba(SMBInfo *info);

#endif
