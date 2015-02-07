
#define MAC_RTS_START 1
#define MAC_RTS_RESPONSE 2
#define MAC_RTS_STOP 3
#define MAC_RTS_ON 4
#define MAC_RTS_CONSOLE_ON 5
#define MAC_RTS_CONSOLE_CMD 6
#define MAC_RTS_CONSOLE_DATA 7

#define DIR_PC_TO_ADSL 0
#define DIR_ADSL_TO_PC 1

#define MAX_RESP_BUF_NUM 1024 //Ren:24 -> 1024
#define MAX_RESP_BUF_SIZE 512

#define ETH_P_TCCONSOLE	0xaaaa

#define TC_PROMPT_SYMBOL	"tc>"

#define BROADCAST_ADDR (PUCHAR)"\xff\xff\xff\xff\xff\xff"
//#define TEST_MAC_ADDR (PUCHAR)"\x00\x11\x22\x33\x44\x55"
#define CMD_CRLF (PUCHAR)"\x0d\x0a"
#define CMD_CRLF_LEN 2
#define MAC_LEN 6
#define ETH_TYPE "\xaa\xaa"
#define ETH_TYPE_LEN 2
#define GET_LEN(x) (sizeof(x)-1)


#define PSWD "admin\x0d\x0a"
#define SYSINFO "sys tpget sys show sysinfo\x0d\x0a"
#define SYSSTATUS "sys tpget sys show sysstatus\x0d\x0a"
#define SYSTRAFFIC "sys tpget sys show systraffic\x0d\x0a"
#define SHOW_WAN "sys tpget sys show wan\x0d\x0a"
#define SHOW_LAN "sys tpget sys show lan\x0d\x0a"
#define SHOW_ADSL "sys tpget sys show adsl\x0d\x0a"
#define SYS_DEFAULT "sys default\x0d\x0a"

#define SET_ADSL "sys tpset wan atm pvc add 2 1234 0 35 0 0\x0d\x0a"

#define PVC_DISP "sys tpget wan atm pvc disp\x0d\x0a"
#define GET_ADSL "sys tpget wan adsl status\x0d\x0a"

#define SET_ADSL_MODE "sys tpset wan adsl mode "
#define SET_ADSL_TYPE "sys tpset wan adsl type "
#define SHOW_ADSL_STATS "sys tpget wan adsl statistics\x0d\x0a"
#define SHOW_ADSL_ACTUAL_ANNEX_MODE "w ghs show op\x0d\x0a" //Paul add 2012/4/21

/* Paul add 2012/9/24, for SNR Margin tweaking. */
#define SET_SNRM_Offset_ADSL "w dmt set snrm "
#define SET_SNRM_Offset_ADSL2 "w dmt2 set snrm "

#define SET_SRA_ADSL2 "w dmt2 set olr " /* Paul add 2012/10/15, for setting SRA. */

#define SYSVER "sys ver\x0d\x0a"

/* Paul add 2013/10/23, for ADSL1 & ADSL2/ADSL2+ Bitswap control. */
#define SET_Bitswap_ADSL1 "w dmt aoc bitswap " //US + DS Bit swap
#define SET_Bitswap_ADSL2 "w dmt2 set olr " //Specifically for DS Bit swap only

#define CON_START_RESP_NUM 1
#define CON_ON_RESP_NUM 1
#define CON_CRLF_RESP_NUM 2
#define CON_PSWD_RESP_NUM 4
/*
#define CON_SYSINFO_RESP_NUM 3
#define CON_SYSSTATUS_RESP_NUM 3
#define CON_SYSTRAFFIC_RESP_NUM 3 
#define CON_SHOWAN_RESP_NUM 3
#define CON_SHOWADSL_RESP_NUM 3
#define CON_SHOWLAN_RESP_NUM 3 
#define CON_SETADSL_RESP_NUM 3 
#define CON_PVCDISP_RESP_NUM 11
#define CON_GETADSL_RESP_NUM 3 
*/

enum {
    QOS_UBR_NO_PCR,
    QOS_UBR,    
    QOS_CBR,
    QOS_VBR,
    QOS_GFR,
    QOS_NRT_VBR
};

#define DEFAULT_IPADDR "169.254.0.1"

#ifdef RTCONFIG_DSL_TCLINUX
#define REMOTE_FILE_FWVER "/etc/fwver.conf"
#define LOCAL_FILE_FWVER "/tmp/adsl/fwver.conf"
#define REMOTE_FILE_ADSL_FWVER "/proc/tc3162/adsl_fwver"
#define LOCAL_FILE_ADSL_FWVER "/tmp/adsl/adsl_fwver"
#define REMOTE_FILE_ADSL_CONF "/var/tmp/adsl.conf"
#define LOCAL_FILE_ADSL_CONF "/tmp/adsl/adsl.conf"
#define REMOTE_FILE_INFO_ADSL "/var/tmp/info_adsl.txt"
#define LOCAL_FILE_INFO_ADSL "/tmp/adsl/info_adsl.txt"
#define REMOTE_FILE_AUTOPVC_COMMON "/var/tmp/autopvc_common.txt"
#define LOCAL_FILE_AUTOPVC_COMMON "/tmp/adsl/autopvc_common.txt"
#define REMOTE_FILE_TCLINUX_HEADER "/var/tmp/tc_hdr.bin"
#define LOCAL_FILE_TCLINUX_HEADER "/tmp/adsl/tc_hdr.bin"
#define REMOTE_FILE_CMD_RESULT "/var/tmp/cmd.txt"
#define LOCAL_FILE_CMD_RESULT "/tmp/adsl/cmd.txt"
#define REMOTE_FILE_CMD_SCRIPT "/var/tmp/cmd.sh"
#define LOCAL_FILE_CMD_SCRIPT "/tmp/adsl/cmd.sh"
#define REMOTE_FILE_DSL_PERF "/proc/tc3162/adsl_perf"
#define LOCAL_FILE_DSL_PERF "/tmp/adsl/adsl_perf"
#define REMOTE_FILE_ADSL_BPC_US "/proc/tc3162/adsl_showbpc_us"
#define REMOTE_FILE_VDSL_BPC_US "/proc/tc3162/vdsl_showbpc_us"
#define LOCAL_FILE_DSL_BPC_US "/var/tmp/spectrum-bpc-us"
#define REMOTE_FILE_ADSL_BPC_DS "/proc/tc3162/adsl_showbpc_ds"
#define REMOTE_FILE_VDSL_BPC_DS "/proc/tc3162/vdsl_showbpc_ds"
#define LOCAL_FILE_DSL_BPC_DS "/var/tmp/spectrum-bpc-ds"
#define REMOTE_FILE_ADSL_SNR "/proc/tc3162/adsl_showsnr"
#define REMOTE_FILE_VDSL_SNR "/proc/tc3162/vdsl_showsnr"
#define LOCAL_FILE_DSL_SNR "/var/tmp/spectrum-snr"
#endif
