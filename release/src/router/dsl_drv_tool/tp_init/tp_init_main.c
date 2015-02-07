// WIN32 notice
// DO NOT BIND OTHER PROTOCOLS
// Some protocols did not write well and no pass response to ADSLPROT.SYS.
// For best test result, please unbind all other protocols.

#ifndef WIN32
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>
#include <linux/if_packet.h>
#include <linux/socket.h>
#include <linux/net.h>
#include <linux/if_ether.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <net/if.h>
#include "basedef.h"
#include "string.h"
#include <fcntl.h>
#include <ctype.h>
#else
#include "stdafx.h"
#include "windows.h"
#include "cprotdrv.h"
#include <conio.h>
static ProtDrv* m_PtDrv;
static BOOLEAN m_AdaptOpened;
#endif

#include "../msg_define.h"
#include <shared.h> //Ren
#include "shutils.h"
#include <stdarg.h>
#include "bcmnvram.h"
#include "scanner.h"
#include "tp_msg.h"
#include "tp_init.h"
#include "fw_conf.h"
#include "ra_reg_rw.h"

static UCHAR m_RespBuf[MAX_RESP_BUF_NUM][MAX_TC_RESP_BUF_LEN];
static USHORT m_RespLen[MAX_RESP_BUF_NUM];

//static UCHAR m_RespBufSave[MAX_RESP_BUF_NUM][MAX_TC_RESP_BUF_LEN];
//static USHORT m_RespLenSave[MAX_RESP_BUF_NUM];


static UCHAR m_AdslMacAddr[6] = {0x00, 0xaa, 0xbb, 0x01, 0x23, 0x45};
static UCHAR m_RouterMacAddr[6];

#ifndef WIN32
static struct sockaddr_ll m_SocketAddr;
#endif

static unsigned int m_AdaptIdx;
static int m_SocketSend = 0;
// those 4 default values are FALSE, FALSE, FALSE, FALSE
static int m_EnableDbgOutput = FALSE;
static int m_ShowPktLog  = FALSE;
static int m_DbgOutputRedirectToFile = FALSE;
static int g_AlwaysIgnoreAdslCmd = FALSE;
static int m_Exit = 0;

extern void reload_dsl_setting(void);
extern void reload_pvc(void);
extern void SetPollingTimer();
extern int CreateMsgQ();
extern int RcvMsgQ();
extern void enable_polling(void);
extern void disable_polling(void); //Ren
extern void wait_polling_stop(void); //Ren
extern int ReadyForInternet();
extern void nvram_adslsts(char* sts);
extern void nvram_adslsts_detail(char* sts);
extern int nvram_get_dbg_flag(void);
extern void add_rc_support(char *feature);

#ifdef RTCONFIG_DSL_TCLINUX
int HandleInfoAdsl(void);
int tftp_get_file(char* remote_full_path, char* local_full_path);
#ifdef RTCONFIG_VDSL
void AddPtm(int idx, int ext_idx, int vlan_active, int vlan_id);
#endif
#endif

#define MAX_ERR_MSG_LEN 1000
static char m_ErrMsgBuf[MAX_ERR_MSG_LEN+1]; // add one for zero
static int m_CurrErrMsgLen = 0;


typedef struct {
    char LineState[30];
} ADSL_LINK_STATUS;

typedef struct {
    char LanTx[10];
    char LanRx[10];
    char AdslTx[10];
    char AdslRx[10];
} ADSL_SYS_TRAFFIC;

typedef struct {
    char CrcDown[10];
    char CrcUp[10];
    char FecDown[10];
    char FecUp[10];
    char HecDown[10];
    char HecUp[10];
} ADSL_STATS;

typedef struct {
    char LineState[10];
    char Modulation[30];
    char SnrMarginUp[20];
    char SnrMarginDown[20];
    char LineAttenuationUp[20];
    char LineAttenuationDown[20];
    char DataRateUp[20];
    char DataRateDown[20];
    char MaxRateUp[20];
    char MaxRateDown[20];
    char PowerUp[20];
    char PowerDown[20];
} ADSL_SYS_STATUS;

typedef struct {
    char annexMode[10];
} ADSL_ACTUAL_ANNEX_MODE;

typedef struct {
    char FwVer[80];
    char AdslFwVer[80];
    char AdslFwVerDisp[32];
} ADSL_SYS_INFO;

typedef struct {
    char IpAddr[80];
} ADSL_LAN_IP;

typedef struct {
	char modulationtype[16];
	char annextypea[16];
	char sraactive[8];
	char snrmoffset[8];
	char vdsltargetsnrm[8];
	char bitswapactive[8];
	char vdslrxautogainctrl[8];
	char vdsltxpowergainoffset[8];
	char vdsluppowerbackoff[8];
} ADSL_ENTRY_DATA;

typedef struct {
	char linestate[16];
	char opmode[32];
	char snrmargindown[16];
	char snrmarginup[16];
	char attendown[16];
	char attenup[16];
	char dataratedown[16];
	char datarateup[16];
	char crcdown[16];
	char crcup[16];
	char powerdown[16];
	char powerup[16];
	char attaindown[16];
	char attainup[16];
	char adsltype[16];
	//char adsluptime[128];
} INFO_ADSL_DATA;

typedef enum{
	DSL_MODE_NOT_UP = 0
	, DSL_MODE_ADSL_ANSI
	, DSL_MODE_ADSL_OPEN_GDMT
	, DSL_MODE_ADSL_OPEN_GLITE
	, DSL_MODE_ADSL2
	, DSL_MODE_ADSL2PLUS
	, DSL_MODE_VDSL2
}DSL_Modulation;


static ADSL_LINK_STATUS m_LinkSts;
static ADSL_SYS_INFO m_SysInfo;
static ADSL_LAN_IP m_IpAddr;
#ifndef RTCONFIG_DSL_TCLINUX
static ADSL_SYS_TRAFFIC m_SysTraffic;
static ADSL_STATS m_Stats;
static ADSL_SYS_STATUS m_SysSts;
static ADSL_ACTUAL_ANNEX_MODE m_AnnexSts;
#else
int m_AdslCommitFlag = 0;
static ADSL_ENTRY_DATA m_AdslEntry;
static INFO_ADSL_DATA m_InfoAdsl;
#endif

//Ren.B
#define END_SYMBOL "\x0d\x0a"
#define FILE_NAME_GET_BPC "/tmp/adsl/tc_bits_per_carrier.txt"
#define FILE_NAME_GET_SNR "/tmp/adsl/tc_snr.txt"
#define FILE_NAME_GET_ADSL1 "/tmp/adsl/tc_snr_adsl1.txt"
#define FILE_NAME_GET_ADSL2_PLUS "/tmp/adsl/tc_snr_adsl2_plus.txt"
#define TMP_FILE_NAME_GET_BPC "/tmp/adsl/tc_bits_per_carrier.tmp"
#define TMP_FILE_NAME_GET_SNR "/tmp/adsl/tc_snr.tmp"
#define TMP_FILE_NAME_GET_ADSL1 "/tmp/adsl/tc_snr_adsl1.tmp"
#define TMP_FILE_NAME_GET_ADSL2_PLUS "/tmp/adsl/tc_snr_adsl2_plus.tmp"
//Ren.E

time_t timestamp=0;
static int uptime_Saved = 0;

/* Paul add 2012/12/25 */
#include <sys/sysinfo.h>
long adsluptime(void)
{
	struct sysinfo info;
	sysinfo(&info);

	return info.uptime;
}

char* strcpymax(char *to, const char*from, int max_to_len)
{
    int i;
    if (max_to_len < 1) return to;

    for (i=0; i<max_to_len-1; i++)
    {
        if (*from == 0) break;
        *to++ = *from++;
    }

    *to++ = 0;

    return to;
}

void PutErrMsg(char* msg)
{
    // copy error msg to buffer and change 0 to new line
    int NewLen;
    strcpymax(&m_ErrMsgBuf[m_CurrErrMsgLen], msg, MAX_ERR_MSG_LEN - m_CurrErrMsgLen);
    NewLen = m_CurrErrMsgLen + strlen(msg);
    if ((NewLen + 1) > MAX_ERR_MSG_LEN) m_CurrErrMsgLen = MAX_ERR_MSG_LEN - 1;
    else m_CurrErrMsgLen = NewLen;
    m_ErrMsgBuf[m_CurrErrMsgLen]='\n';
    m_CurrErrMsgLen++;
    m_ErrMsgBuf[m_CurrErrMsgLen]=0;
}

char* GetErrMsg(int* len)
{
    *len = m_CurrErrMsgLen;
    return m_ErrMsgBuf;
}

void myprintf(const char *fmt, ...)
{
	if (m_DbgOutputRedirectToFile)
	{
		char buf[1024];
		FILE* fp;
		if (m_EnableDbgOutput == FALSE) return;
		fp = fopen("/tmp/adsl/adsllog.log","a+");
		va_list arg;
		va_start(arg, fmt);
		vsprintf(buf, fmt, arg);
		fprintf(fp, buf);
		va_end(arg);
		fclose(fp);
	}
	else
	{
		// console
		if (m_EnableDbgOutput == FALSE) return;
		va_list arg;
		va_start(arg, fmt);
		vprintf(fmt, arg);
		va_end(arg);
	}
}

char* GET_RESP_STR(PUCHAR x)
{
    if (*(x+14) != 0x07)
    {
        return NULL;
    }
    return (char*)(x+15);
}

int GET_RESP_LEN(int len)
{
    if (len < 15)
    {
        myprintf("TC response critical error\r\n");
        return 0;
    }
    return (len-15);
}


void SleepMs(int ms)
{
#ifndef WIN32
    usleep(ms*1000); //convert to microseconds
#else
    Sleep(ms);
#endif
}

BOOLEAN SendPkt(PUCHAR SendBuf, USHORT SendBufLen)
{
#ifndef WIN32
	m_SocketAddr.sll_family   = PF_PACKET;
	// we don't use a protocoll above ethernet layer just use anything here
	m_SocketAddr.sll_protocol = htons(ETH_P_IP);
	m_SocketAddr.sll_ifindex = m_AdaptIdx;
	// use any thing here
	m_SocketAddr.sll_hatype   = 0x1234;
	// target is another host
	m_SocketAddr.sll_pkttype  = PACKET_OTHERHOST;
	// address length
	m_SocketAddr.sll_halen    = ETH_ALEN;
	int send_result = 0;
	// send the packet
	send_result = sendto(m_SocketSend, SendBuf, SendBufLen, 0, (struct sockaddr*)&m_SocketAddr, sizeof(m_SocketAddr));

	if (send_result < 0)
	{
		myprintf("%s %d: %s\n", __func__, __LINE__, strerror(errno));
		return FALSE;
	}
	else
		return TRUE;
#else


    BOOLEAN RetVal = m_PtDrv->SendPkt(SendBuf, SendBufLen);
    if (RetVal == FALSE)
    {
        wprintf(L"Err send one pkt");
        return FALSE;
    }
    return TRUE;

#endif
}

// DA 6 + SA 6 + Eth Type 2 + TC type 1 = 15
#define TC_RESP_DATA_IDX	15

BOOLEAN RcvPkt(PUCHAR RcvBuf, USHORT MaxRcvBufSize, PUSHORT pRcvBufLen)
{
#ifndef WIN32
#ifndef RTCONFIG_RALINK

	ssize_t recv_len;
	u_int8_t rcv_buffer[MAX_TC_RESP_BUF_LEN];
	char *endStr = TC_PROMPT_SYMBOL;

	*pRcvBufLen = 0;
	*rcv_buffer = '\0';

	*pRcvBufLen = recvfrom(m_SocketSend, RcvBuf, MaxRcvBufSize, 0, NULL, NULL);
	if (*pRcvBufLen == 65535) {
		myprintf("%s %d: %s\n", __func__, __LINE__, strerror(errno));
		return FALSE;
	}

	*pRcvBufLen = strlen((char*)RcvBuf+TC_RESP_DATA_IDX) + TC_RESP_DATA_IDX;

	if(*(RcvBuf + TC_RESP_DATA_IDX - 1) != 0x07)
		return TRUE;
	else if(*(RcvBuf+ *pRcvBufLen - 1) == '\n')
		return TRUE;
	else if(strlen(GET_RESP_STR(RcvBuf)) >= strlen(endStr)) {	//receive endStr
		if(!strncmp((char*)RcvBuf + *pRcvBufLen - strlen(endStr), endStr, strlen(endStr)))
			return TRUE;
	}

	while(!m_Exit) {
		recv_len = recvfrom(m_SocketSend, rcv_buffer, MAX_TC_RESP_BUF_LEN, 0, NULL, NULL);
		if (recv_len < 0) {
			myprintf("%s %d: %s\n", __func__, __LINE__, strerror(errno));
			return FALSE;
		}

		if (*pRcvBufLen + strlen(GET_RESP_STR(rcv_buffer)) > MaxRcvBufSize) {
			myprintf("%s %d: Out of buffer %u/%u.\n", __func__, __LINE__, *pRcvBufLen + strlen(GET_RESP_STR(rcv_buffer)), MaxRcvBufSize);
			return FALSE;
		}
		else {
			memcpy(RcvBuf + *pRcvBufLen, GET_RESP_STR(rcv_buffer), strlen(GET_RESP_STR(rcv_buffer)));
			*pRcvBufLen += strlen(GET_RESP_STR(rcv_buffer));
		}

		if(*(RcvBuf+ *pRcvBufLen - 1) == '\n')
			break;
		else if(strlen(GET_RESP_STR(RcvBuf)) >= strlen(endStr)) {	//receive endStr
			if(!strncmp((char*)RcvBuf + *pRcvBufLen - strlen(endStr), endStr, strlen(endStr)))
				break;
		}
	}
	return TRUE;

#else
    int ret = switch_rcv_okt(RcvBuf, MaxRcvBufSize, pRcvBufLen);
    if (ret == 0)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
#endif
#else

    BOOLEAN RetVal = m_PtDrv->RcvPkt(RcvBuf, MaxRcvBufSize, pRcvBufLen);
    if (RetVal)
    {
        return TRUE;
    }
    else
    {
        wprintf(L"RCV failed !!");
        return FALSE;
    }

#endif
}


VOID LogPktToConsole(int direction, unsigned char* data , size_t Size)
{
    if (direction == 0)
    {
        myprintf("\r\n  PC ---> ADSL , Len:%u\r\n", Size);
    }
    else
    {
        myprintf("\r\n  Adsl ---> PC , Len:%u\r\n", Size);
    }

	myprintf("              0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F   0123456789ABCDEF\r\n");

    int i;
    int j;
    // log data
    for(i=0 ; i < Size ; i++)
    {
        if( i!=0 && i%16==0)   //if one line of hex printing is complete...
        {
            myprintf(" | ",i);
            for(j=i-16 ; j<i ; j++)
            {
                if(data[j]>=32 && data[j]<=128)
                    myprintf("%c",(unsigned char)data[j]); //if its a number or alphabet

                else myprintf("."); //otherwise print a dot
            }
            myprintf("\n");
        }

        if(i%16==0) myprintf("0x%08X |", i);

        myprintf(" %02X",(unsigned int)data[i]);

        if( i==Size-1)  //print the last spaces
        {
            for(j=0; j<15-i%16; j++) myprintf("   "); //extra spaces

            myprintf(" | ");

            for(j=i-i%16 ; j<=i ; j++)
            {
                if(data[j]>=32 && data[j]<=128) myprintf("%c",(unsigned char)data[j]);
                else myprintf(".");
            }
            myprintf("\n");
        }
    }
}

void CreateAdslErrorFile(char* fn, UCHAR cmd, PUCHAR pData, USHORT DataLen)
{
    FILE* fpBoot;
    char tmp[128];
    sprintf(tmp,"/tmp/adsl/ADSL_FATAL_%s_ERROR.txt",fn);
    fpBoot = fopen(tmp,"wb");
    if (fpBoot != NULL)
    {
        sprintf(tmp, "cmd=%d,len=%d",cmd,DataLen);
        fputs(tmp,fpBoot);
        fputs("\n",fpBoot);
        if (MAC_RTS_CONSOLE_CMD==cmd)
        {
            fputs((const char*)pData,fpBoot);
        }
        fclose(fpBoot);
    }
}

BOOLEAN SendCmdAndWaitResp(PUCHAR pResp[], USHORT MaxRespLen, PUSHORT pRespLen[], USHORT MaxResp, PUSHORT pRcvPktNum, PUCHAR pDst, UCHAR Cmd, PUCHAR pData, USHORT DataLen, USHORT WaitRespNumInput)
{
    USHORT WaitRespNum = WaitRespNumInput; //Ren: the original type is UCHAR which is wrong.
    UCHAR PktBuf[1600];
    USHORT Idx = 0;
    unsigned int PktBufLen = 0;

    if (g_AlwaysIgnoreAdslCmd) return FALSE;

    if (WaitRespNum == 0) WaitRespNum = (USHORT)MaxResp;

    memcpy(&PktBuf[Idx], pDst, MAC_LEN);
    Idx+=MAC_LEN;
    memcpy(&PktBuf[Idx], m_RouterMacAddr, MAC_LEN);
    Idx+=MAC_LEN;
    memcpy(&PktBuf[Idx], ETH_TYPE, ETH_TYPE_LEN);
    Idx+=ETH_TYPE_LEN;
    PktBuf[Idx] = Cmd;
    Idx+=sizeof(Cmd);
    if (DataLen>0 && pData != NULL)
    {
        if (Idx+DataLen < sizeof(PktBuf))
        {
            memcpy(&PktBuf[Idx], pData, DataLen);
            Idx+=DataLen;
        }
    }

    PktBufLen = Idx;
    BOOLEAN RetVal = SendPkt(PktBuf, PktBufLen);
    if (RetVal == FALSE)
    {
        //g_AlwaysIgnoreAdslCmd = TRUE;
        CreateAdslErrorFile("SEND", Cmd, pData, DataLen);
        myprintf("TP_INIT : Err send\n");
        return FALSE;
    }

    if (m_ShowPktLog)
    {
        LogPktToConsole(DIR_PC_TO_ADSL, PktBuf, PktBufLen);
    }


    USHORT RetRespLen;
    *pRcvPktNum = 0;
    //*pRespLen = 0;

    int RespCnt = 0;

    int TimeoutCnt;
    for (TimeoutCnt = 0; TimeoutCnt<1200; TimeoutCnt++) //Ren:100 -> 1200
    {
        memset(pResp[RespCnt], 0, MAX_TC_RESP_BUF_LEN );//Ren
        RetVal = RcvPkt(pResp[RespCnt], MaxRespLen, &RetRespLen);
        if (RetVal)
        {
            if (RetRespLen == 0)
            {
                SleepMs(100);
                continue;
            }
            else
            {
                if (m_ShowPktLog)
                {
                    LogPktToConsole(DIR_ADSL_TO_PC, pResp[RespCnt], RetRespLen);
                }
                int PromptReceived = FALSE;
                if (WaitRespNumInput == 0)
                {
                    scanner_set(GET_RESP_STR(pResp[RespCnt]), GET_RESP_LEN(RetRespLen));
                    while (1)
                    {
                        token tok = scanner();
                        if (tok == TOKEN_EOF)
                        {
                            break;
                        }
                        if (tok == TOKEN_STRING)
                        {
                            char* pRespStr = scanner_get_str();
                            if (strcmp(pRespStr,"tc>") == 0)
                            {
                                PromptReceived = TRUE;
                            }
                        }
                    }
                }
                (*pRcvPktNum)+=1;
                *(pRespLen[RespCnt])=RetRespLen;
                RespCnt++;
                if (RespCnt >= MaxResp) break;
                // if we have received enough packet
                if ((*pRcvPktNum) >= WaitRespNum || PromptReceived)
                {
#if 0
                    //we wait 100 ms
                    SleepMs(100);
                    BOOLEAN CheckPendingRetVal = RcvPkt(pResp[RespCnt], MaxRespLen, &RetRespLen);
                    if (CheckPendingRetVal && RetRespLen > 0)
                    {
                        myprintf("*** Pended packet to received ***\r\n");
                    }
#endif
                    break;
                }
                SleepMs(100);
                continue;
            }
        }
        else
        {
            //g_AlwaysIgnoreAdslCmd = TRUE;
            CreateAdslErrorFile("RECV", Cmd, pData, DataLen);
            myprintf("TP_INIT : RCV failed !!\n");
            return FALSE;
        }
    }

    if (m_ShowPktLog)
    {
        myprintf("*** RESP : %d\n",*pRcvPktNum);
    }

    return TRUE;
}



BOOLEAN SendCmdAndWaitResp2(PUCHAR pResp[], USHORT MaxRespLen, PUSHORT pRespLen[], USHORT MaxResp, PUSHORT pRcvPktNum, PUCHAR pDst, UCHAR Cmd, PUCHAR pData, USHORT DataLen, USHORT WaitRespNumInput)
{
    USHORT WaitRespNum = WaitRespNumInput;
    UCHAR PktBuf[1600];
    USHORT Idx = 0;
    unsigned int PktBufLen = 0;

    if (g_AlwaysIgnoreAdslCmd) return FALSE;

    if (WaitRespNum == 0) WaitRespNum = (USHORT)MaxResp;

    memcpy(&PktBuf[Idx], pDst, MAC_LEN);
    Idx+=MAC_LEN;
    memcpy(&PktBuf[Idx], m_RouterMacAddr, MAC_LEN);
    Idx+=MAC_LEN;
    memcpy(&PktBuf[Idx], ETH_TYPE, ETH_TYPE_LEN);
    Idx+=ETH_TYPE_LEN;
    PktBuf[Idx] = Cmd;
    Idx+=sizeof(Cmd);
    if (DataLen>0 && pData != NULL)
    {
        if (Idx+DataLen < sizeof(PktBuf))
        {
            memcpy(&PktBuf[Idx], pData, DataLen);
            Idx+=DataLen;
        }
    }

    PktBufLen = Idx;
    BOOLEAN RetVal = SendPkt(PktBuf, PktBufLen);
    if (RetVal == FALSE)
    {
        //g_AlwaysIgnoreAdslCmd = TRUE;
        CreateAdslErrorFile("SEND", Cmd, pData, DataLen);
        myprintf("TP_INIT : Err send\n");
        return FALSE;
    }

    if (m_ShowPktLog)
    {
        LogPktToConsole(DIR_PC_TO_ADSL, PktBuf, PktBufLen);
    }


    USHORT RetRespLen;
    *pRcvPktNum = 0;
    //*pRespLen = 0;

    int RespCnt = 0;

    int TimeoutCnt;
    for (TimeoutCnt = 0; TimeoutCnt<1200; TimeoutCnt++) //Ren
    {
        memset(pResp[RespCnt], 0, MAX_TC_RESP_BUF_LEN );//Ren
        RetVal = RcvPkt(pResp[RespCnt], MaxRespLen, &RetRespLen);
        if (RetVal)
        {
            if (RetRespLen == 0)
            {
                SleepMs(100);
                continue;
            }
            else
            {
                if (m_ShowPktLog)
                {
                    LogPktToConsole(DIR_ADSL_TO_PC, pResp[RespCnt], RetRespLen);
                }
                int PromptReceived = FALSE;
                if (WaitRespNumInput == 0)
                {
                    scanner_set(GET_RESP_STR(pResp[RespCnt]), GET_RESP_LEN(RetRespLen));
                    while (1)
                    {
                        token tok = scanner();
                        if (tok == TOKEN_EOF)
                        {
                            break;
                        }
                        if (tok == TOKEN_STRING)
                        {
                            char* pRespStr = scanner_get_str();
                            if (strcmp(pRespStr,"504~511") == 0)
                            {
                                PromptReceived = TRUE;
                            }
                        }
                    }
                }
                (*pRcvPktNum)+=1;
                *(pRespLen[RespCnt])=RetRespLen;
                RespCnt++;
                if (RespCnt >= MaxResp) break;
                // if we have received enough packet
                if ((*pRcvPktNum) >= WaitRespNum || PromptReceived)
                {
#if 0
                    //we wait 100 ms
                    SleepMs(100);
                    BOOLEAN CheckPendingRetVal = RcvPkt(pResp[RespCnt], MaxRespLen, &RetRespLen);
                    if (CheckPendingRetVal && RetRespLen > 0)
                    {
                        myprintf("*** Pended packet to received ***\r\n");
                    }
#endif
                    break;
                }
                SleepMs(100);
                continue;
            }
        }
        else
        {
            //g_AlwaysIgnoreAdslCmd = TRUE;
            CreateAdslErrorFile("RECV", Cmd, pData, DataLen);
            myprintf("TP_INIT : RCV failed !!\n");
            return FALSE;
        }
    }

    if (m_ShowPktLog)
    {
        myprintf("*** RESP : %d\n",*pRcvPktNum);
    }
    return TRUE;
}



void GetMacAddr(void)
{
#ifndef WIN32
    int fd;
    struct ifreq ifr;


    fd = socket(PF_INET, SOCK_PACKET, htons(ETH_P_ALL)); /* open socket */
#if DSL_N55U == 1
	strcpy(ifr.ifr_name, "eth2"); /* assuming we want eth0 */
#else
	strcpy(ifr.ifr_name, "eth0");
#endif

    ioctl(fd, SIOCGIFHWADDR, &ifr); /* retrieve MAC address */

    /*
    * at this point, the interface's HW address is in
    * 6 bytes in ifr.ifr_hwaddr.sa_data. You can copy them
    * out with memcpy() or something similar. You'll have to
    * translate the bytes into a readable hex notation.
    */

    close(fd);

    memcpy(m_RouterMacAddr, ifr.ifr_hwaddr.sa_data, 6);
#else

    memcpy(m_RouterMacAddr, m_PtDrv->GetMacAddr(), 6);

#endif

    myprintf("ROUTER MAC : %02x-%02x-%02x-%02x-%02x-%02x\n", m_RouterMacAddr[0], m_RouterMacAddr[1], m_RouterMacAddr[2],m_RouterMacAddr[3],m_RouterMacAddr[4],m_RouterMacAddr[5]);


}

/*
BOOLEAN SendOnePktVlan(PUCHAR pDst, USHORT VlanId, PUCHAR pData, USHORT DataLen)
{
    UCHAR PktBuf[1600];
    USHORT Idx = 0;
    unsigned int PktBufLen = 0;
    USHORT VlanTag = 0x0081;

    memcpy(&PktBuf[Idx], pDst, MAC_LEN);
    Idx+=MAC_LEN;
    memcpy(&PktBuf[Idx], m_RouterMacAddr, MAC_LEN);
    Idx+=MAC_LEN;
    memcpy(&PktBuf[Idx], &VlanTag, sizeof(VlanTag));
    Idx+=sizeof(VlanTag);
    PktBuf[Idx++] = VlanId >> 8;
    PktBuf[Idx++] = VlanId & 0xff;

    if (DataLen>0 && pData != NULL)
    {
        if (Idx+DataLen < sizeof(PktBuf))
        {
            memcpy(&PktBuf[Idx], pData, DataLen);
            Idx+=DataLen;
        }
    }

    PktBufLen = Idx;
    int Cnt;
    for (Cnt = 0; Cnt < 100; Cnt++)
    {
        BOOLEAN RetVal = SendPkt(PktBuf, PktBufLen);
        if (RetVal == FALSE)
        {
            //AfxMessageBox(L"Err send one pkt");
            return FALSE;
        }
    }
    return TRUE;
}
*/


#define declare_resp_handling_vars(pRespBuf, pRespLen, RespPktNum, tok, pRespStr) \
    PUCHAR pRespBuf[MAX_RESP_BUF_NUM]; \
    PUSHORT pRespLen[MAX_RESP_BUF_NUM]; \
    USHORT RespPktNum; \
    token tok; \
    char* pRespStr; \
    init_resp_buf (pRespBuf, pRespLen)


void init_resp_buf(PUCHAR pRespBuf[], PUSHORT pRespLen[])
{
    int i;
    for (i=0; i<MAX_RESP_BUF_NUM; i++)
    {
        memset( &m_RespBuf[i], 0, sizeof(m_RespBuf[i]) );//Ren: Fix bug#226.
        pRespBuf[i] = &m_RespBuf[i][0];
        pRespLen[i] = &m_RespLen[i];
    }
}

void tok_assert(token tok, token expected_tok)
{
    if (tok != expected_tok)
    {
        PutErrMsg("tok is not expected value");
        myprintf("tok is not expected value\r\n");
        myprintf("%d",tok);
        myprintf("\r\n");
    }
}

void tok_assert_two(token tok, token expected_tok0, token expected_tok1)
{
    if (tok == expected_tok0 || tok == expected_tok1)
    {
    }
    else
    {
        PutErrMsg("tok is not expected value0 or value1");
        myprintf("tok is not expected value0 or value1\r\n");
        myprintf("%d",tok);
        myprintf("\r\n");
    }
}


void tok_assert_buf(char* tok_buf, char* tok_buf_expected)
{
    if (strcmp(tok_buf,tok_buf_expected) != 0)
    {
        PutErrMsg("tok buf is not expected");
        myprintf("tok buf is not expected\r\n");
        myprintf(tok_buf);
        myprintf("\r\n");
    }
}

int HandleAdslShowLan(ADSL_LAN_IP* pLanIp)
{
    declare_resp_handling_vars(pRespBuf, pRespLen, RespPktNum, tok, pRespStr);
#ifndef RTCONFIG_DSL_TCLINUX
    SendCmdAndWaitResp(pRespBuf, MAX_RESP_BUF_SIZE, pRespLen, MAX_RESP_BUF_NUM, &RespPktNum, m_AdslMacAddr,
        MAC_RTS_CONSOLE_CMD, (PUCHAR)SHOW_LAN, GET_LEN(SHOW_LAN), 0);
    int i;
    for(i=0; i<RespPktNum; i++)
    {
        scanner_set(GET_RESP_STR(pRespBuf[i]), GET_RESP_LEN(*pRespLen[i]));
        while (1)
        {
            tok = scanner();
            if (tok == TOKEN_EOF)
            {
                break;
            }
            if (tok == TOKEN_STRING)
            {
                pRespStr = scanner_get_str();
                if (strcmp(pRespStr,"lanip") == 0)
                {
                    tok = scanner();
                    tok_assert(tok,TOKEN_EQUAL);
                    tok = scanner_get_ip();
                    strcpymax(pLanIp->IpAddr, scanner_get_str(), sizeof(pLanIp->IpAddr));
                }
            }
        }
    }
#else
	int i;
	char tccmd[MAX_RESP_BUF_SIZE] = {0};

	sprintf(tccmd, "tcapi get Info_Ether ip\x0d\x0a");
	SendCmdAndWaitResp(pRespBuf, MAX_RESP_BUF_SIZE, pRespLen, MAX_RESP_BUF_NUM, &RespPktNum, m_AdslMacAddr,
		MAC_RTS_CONSOLE_CMD, (PUCHAR)tccmd, GET_LEN(tccmd), 0);
	for(i=0; i<RespPktNum; i++)
	{
		scanner_set(GET_RESP_STR(pRespBuf[i]), GET_RESP_LEN(*pRespLen[i]));
		while (1)
		{
			tok = scanner();
			if (tok == TOKEN_EOF)
			{
				break;
			}
			if (tok == TOKEN_NUMBER_LITERAL)
			{
				pRespStr = scanner_get_str();
				if(inet_addr(pRespStr) < 0)
					strcpymax(pLanIp->IpAddr, DEFAULT_IPADDR, sizeof(pLanIp->IpAddr));
				else
					strcpymax(pLanIp->IpAddr, pRespStr, sizeof(pLanIp->IpAddr));
				eval("route", "add", "-host", pLanIp->IpAddr, "dev", "vlan2");
			}
		}
	}
#endif
    return 0;
}

int SetAdslMode(int EnumAdslModeValue, int FromAteCmd)
{
#ifndef RTCONFIG_DSL_TCLINUX
    declare_resp_handling_vars(pRespBuf, pRespLen, RespPktNum, tok, pRespStr);
    SendCmdAndWaitResp(pRespBuf, MAX_RESP_BUF_SIZE, pRespLen, MAX_RESP_BUF_NUM, &RespPktNum, m_AdslMacAddr,
        MAC_RTS_CONSOLE_CMD, (PUCHAR)SHOW_ADSL, GET_LEN(SHOW_ADSL),0);

    int i;
    BOOLEAN SetNewValue = FALSE;
    for(i=0; i<RespPktNum; i++)
    {
        scanner_set(GET_RESP_STR(pRespBuf[i]), GET_RESP_LEN(*pRespLen[i]));
        while (1)
        {
            tok = scanner();
            if (tok == TOKEN_EOF)
            {
                break;
            }
            if (tok == TOKEN_STRING)
            {
                pRespStr = scanner_get_str();
                if (strcmp(pRespStr,"adslmode") == 0)
                {
                    tok = scanner();
                    tok_assert(tok,TOKEN_EQUAL);
                    tok = scanner();
                    tok_assert(tok,TOKEN_NUMBER_LITERAL);
                    if (EnumAdslModeValue != atoi(scanner_get_str()))
                    {
                        SetNewValue = TRUE;
                    }
                }
            }
        }
    }

    if (SetNewValue == FALSE)
    {
        //printf("Old value equal to new value. skip\n");
        return 0;
    }

    char InputCmd[10];
    char UserCmd[256];
		InputCmd[0] = '\0';
		UserCmd[0] = '\0';
    sprintf(InputCmd, "%d", EnumAdslModeValue);
    strcpy(UserCmd, SET_ADSL_MODE);
    strcat(UserCmd, InputCmd);
    strcat(UserCmd, "\x0d\x0a");
    SendCmdAndWaitResp(pRespBuf, MAX_RESP_BUF_SIZE, pRespLen, MAX_RESP_BUF_NUM, &RespPktNum, m_AdslMacAddr,
        MAC_RTS_CONSOLE_CMD, (PUCHAR)UserCmd, strlen(UserCmd), 0);


    SendCmdAndWaitResp(pRespBuf, MAX_RESP_BUF_SIZE, pRespLen, MAX_RESP_BUF_NUM, &RespPktNum, m_AdslMacAddr,
        MAC_RTS_CONSOLE_CMD, (PUCHAR)SHOW_ADSL, GET_LEN(SHOW_ADSL),0);

    for(i=0; i<RespPktNum; i++)
    {
        scanner_set(GET_RESP_STR(pRespBuf[i]), GET_RESP_LEN(*pRespLen[i]));
        while (1)
        {
            tok = scanner();
            if (tok == TOKEN_EOF)
            {
                break;
            }
            if (tok == TOKEN_STRING)
            {
                pRespStr = scanner_get_str();
                if (strcmp(pRespStr,"adslmode") == 0)
                {
                    tok = scanner();
                    tok_assert(tok,TOKEN_EQUAL);
                    tok = scanner();
                    tok_assert(tok,TOKEN_NUMBER_LITERAL);
                    if (EnumAdslModeValue != atoi(scanner_get_str()))
                    {
                        char buf[80];
                        sprintf(buf,"SetAdslMode,%d",EnumAdslModeValue);
                        PutErrMsg(buf);
                        return -1;
                    }
                }
            }
        }
    }
#else
	PUCHAR pRespBuf[MAX_RESP_BUF_NUM];
	PUSHORT pRespLen[MAX_RESP_BUF_NUM];
	USHORT RespPktNum;
	init_resp_buf (pRespBuf, pRespLen);

	char tccmd[MAX_RESP_BUF_SIZE] = {0};
	char setAnnexMode[16] = {0};

	switch(EnumAdslModeValue) {
		case 0:
			strncpy(setAnnexMode, "T1.413", sizeof(setAnnexMode));
			break;
		case 1:
			strncpy(setAnnexMode, "G.lite", sizeof(setAnnexMode));
			break;
		case 2:
			strncpy(setAnnexMode, "G.DMT", sizeof(setAnnexMode));
			break;
		case 3:
			strncpy(setAnnexMode, "ADSL2", sizeof(setAnnexMode));
			break;
		case 4:
			strncpy(setAnnexMode, "ADSL2+", sizeof(setAnnexMode));
			break;
		case 5:
			strncpy(setAnnexMode, "Auto Sync-Up", sizeof(setAnnexMode));
			break;
		case 6:
			strncpy(setAnnexMode, "VDSL2", sizeof(setAnnexMode));
			break;
		default:
			strncpy(setAnnexMode, "Auto Sync-Up", sizeof(setAnnexMode));
			break;
	}

	if( strcmp(m_AdslEntry.modulationtype, setAnnexMode) || FromAteCmd)
	{
		myprintf("%s [%s]!=[%s]\n", __FUNCTION__, m_AdslEntry.modulationtype, setAnnexMode);
		memset(tccmd, 0, sizeof(tccmd));
		sprintf(tccmd, "tcapi set Adsl_Entry MODULATIONTYPE \"%s\"\x0d\x0a", setAnnexMode);
		SendCmdAndWaitResp(pRespBuf, MAX_RESP_BUF_SIZE, pRespLen, MAX_RESP_BUF_NUM, &RespPktNum, m_AdslMacAddr,
			MAC_RTS_CONSOLE_CMD, (PUCHAR)tccmd, GET_LEN(tccmd), 1);
		m_AdslCommitFlag = 1;
	}
#endif
    return 0;
}

int SetAdslType(int EnumAdslTypeValue, int FromAteCmd)
{
#ifdef RTCONFIG_DSL_ANNEX_B /* Paul add 2012/8/22, for Annex B if set adsltype 0 again will change to Annex A. Which is a bug with ADSL driver, so just skip. */
	return 0;
#endif
#ifndef RTCONFIG_DSL_TCLINUX
    declare_resp_handling_vars(pRespBuf, pRespLen, RespPktNum, tok, pRespStr);
    char InputCmd[10];
    char UserCmd[256];
		InputCmd[0] = '\0';
		UserCmd[0] = '\0';
    SendCmdAndWaitResp(pRespBuf, MAX_RESP_BUF_SIZE, pRespLen, MAX_RESP_BUF_NUM, &RespPktNum, m_AdslMacAddr,
        MAC_RTS_CONSOLE_CMD, (PUCHAR)SHOW_ADSL, GET_LEN(SHOW_ADSL),0);

    int i;

    /* Paul comment 2012/7/17, just issue set adsltype again, in order to have the Annex mode stick to that adsl type. */
    /*BOOLEAN SetNewValue = FALSE;
    for(i=0; i<RespPktNum; i++)
    {
        scanner_set(GET_RESP_STR(pRespBuf[i]), GET_RESP_LEN(*pRespLen[i]));
        while (1)
        {
            tok = scanner();
            if (tok == TOKEN_EOF)
            {
                break;
            }
            if (tok == TOKEN_STRING)
            {
                pRespStr = scanner_get_str();
                if (strcmp(pRespStr,"adslmode") == 0)
                {
                    tok = scanner();
                    tok_assert(tok,TOKEN_EQUAL);
                    tok = scanner();
                    tok_assert(tok,TOKEN_NUMBER_LITERAL);
                    tok = scanner();
                    tok_assert(tok,TOKEN_COMMA);

                    tok = scanner();
                    tok_assert(tok,TOKEN_STRING);
                    tok_assert_buf(scanner_get_str(), "adsltype");
                    tok = scanner();
                    tok_assert(tok,TOKEN_EQUAL);
                    tok = scanner();
                    tok_assert(tok,TOKEN_NUMBER_LITERAL);
                    if (EnumAdslTypeValue != atoi(scanner_get_str()))
                    {
                        SetNewValue = TRUE;
                    }
                }
            }
        }
    }

    if (SetNewValue == FALSE)
    {
        //printf("Old value equal to new value. skip\n");
        return 0;
    }*/

    sprintf(InputCmd, "%d", EnumAdslTypeValue);
    strcpy(UserCmd, SET_ADSL_TYPE);
    strcat(UserCmd, InputCmd);
    strcat(UserCmd, "\x0d\x0a");
    SendCmdAndWaitResp(pRespBuf, MAX_RESP_BUF_SIZE, pRespLen, MAX_RESP_BUF_NUM, &RespPktNum, m_AdslMacAddr,
        MAC_RTS_CONSOLE_CMD, (PUCHAR)UserCmd, strlen(UserCmd), 0);

    SendCmdAndWaitResp(pRespBuf, MAX_RESP_BUF_SIZE, pRespLen, MAX_RESP_BUF_NUM, &RespPktNum, m_AdslMacAddr,
        MAC_RTS_CONSOLE_CMD, (PUCHAR)SHOW_ADSL, GET_LEN(SHOW_ADSL),0);

    for(i=0; i<RespPktNum; i++)
    {
        scanner_set(GET_RESP_STR(pRespBuf[i]), GET_RESP_LEN(*pRespLen[i]));
        while (1)
        {
            tok = scanner();
            if (tok == TOKEN_EOF)
            {
                break;
            }
            if (tok == TOKEN_STRING)
            {
                pRespStr = scanner_get_str();
                if (strcmp(pRespStr,"adslmode") == 0)
                {
                    tok = scanner();
                    tok_assert(tok,TOKEN_EQUAL);
                    tok = scanner();
                    tok_assert(tok,TOKEN_NUMBER_LITERAL);
                    tok = scanner();
                    tok_assert(tok,TOKEN_COMMA);

                    tok = scanner();
                    tok_assert(tok,TOKEN_STRING);
                    tok_assert_buf(scanner_get_str(), "adsltype");
                    tok = scanner();
                    tok_assert(tok,TOKEN_EQUAL);
                    tok = scanner();
                    tok_assert(tok,TOKEN_NUMBER_LITERAL);
                    if (EnumAdslTypeValue != atoi(scanner_get_str()))
                    {
                        char buf[80];
                        sprintf(buf,"SetAdslType,%d",EnumAdslTypeValue);
                        PutErrMsg(buf);
                        return -1;
                    }
                }
            }
        }
    }
#else
	PUCHAR pRespBuf[MAX_RESP_BUF_NUM];
	PUSHORT pRespLen[MAX_RESP_BUF_NUM];
	USHORT RespPktNum;
	init_resp_buf (pRespBuf, pRespLen);

	char tccmd[MAX_RESP_BUF_SIZE] = {0};
	char setAnnexType[16] = {0};

	switch(EnumAdslTypeValue) {
		case 0:
			strncpy(setAnnexType, "ANNEX A", sizeof(setAnnexType));
			break;
		case 1:
			strncpy(setAnnexType, "ANNEX I", sizeof(setAnnexType));
			break;
		case 2:
			strncpy(setAnnexType, "ANNEX A/L", sizeof(setAnnexType));
			break;
		case 3:
			strncpy(setAnnexType, "ANNEX M", sizeof(setAnnexType));
			break;
		case 4:
			strncpy(setAnnexType, "ANNEX A/I/J/L/M", sizeof(setAnnexType));
			break;
		case 5:
			strncpy(setAnnexType, "ANNEX B", sizeof(setAnnexType));
			break;
		case 6:
			strncpy(setAnnexType, "ANNEX B/J/M", sizeof(setAnnexType));
			break;
	}

	if( strcmp(m_AdslEntry.annextypea, setAnnexType) || FromAteCmd)
	{
		myprintf("%s [%s]!=[%s]\n", __FUNCTION__, m_AdslEntry.annextypea, setAnnexType);
		memset(tccmd, 0, sizeof(tccmd));
		sprintf(tccmd, "tcapi set Adsl_Entry ANNEXTYPEA \"%s\"\x0d\x0a", setAnnexType);
		SendCmdAndWaitResp(pRespBuf, MAX_RESP_BUF_SIZE, pRespLen, MAX_RESP_BUF_NUM, &RespPktNum, m_AdslMacAddr,
			MAC_RTS_CONSOLE_CMD, (PUCHAR)tccmd, GET_LEN(tccmd), 1);
		m_AdslCommitFlag = 1;
	}
#endif
    return 0;
}

/* Paul add start 2012/9/24, for SNR Margin tweaking. */
int SetSNRMOffset(int EnumSNRMOffsetValue, int FromAteCmd)
{
#ifndef RTCONFIG_DSL_TCLINUX
		if(EnumSNRMOffsetValue == 0) //Disabled
			return 0;
	
    declare_resp_handling_vars(pRespBuf, pRespLen, RespPktNum, tok, pRespStr);

    char InputCmd[10];
    char UserCmd[64];
		InputCmd[0] = '\0';
		UserCmd[0] = '\0';
    
    sprintf(InputCmd, "%d", EnumSNRMOffsetValue);
    strcpy(UserCmd, SET_SNRM_Offset_ADSL);
    strcat(UserCmd, InputCmd);
    strcat(UserCmd, "\x0d\x0a");
    
    SendCmdAndWaitResp(pRespBuf, MAX_RESP_BUF_SIZE, pRespLen, MAX_RESP_BUF_NUM, &RespPktNum, m_AdslMacAddr,
        MAC_RTS_CONSOLE_CMD, (PUCHAR)UserCmd, strlen(UserCmd), 0);

		UserCmd[0] = '\0';
    strcpy(UserCmd, SET_SNRM_Offset_ADSL2);
    strcat(UserCmd, InputCmd);
    strcat(UserCmd, " ");
    strcat(UserCmd, InputCmd);
    strcat(UserCmd, "\x0d\x0a");

    SendCmdAndWaitResp(pRespBuf, MAX_RESP_BUF_SIZE, pRespLen, MAX_RESP_BUF_NUM, &RespPktNum, m_AdslMacAddr,
        MAC_RTS_CONSOLE_CMD, (PUCHAR)UserCmd, strlen(UserCmd), 0);
#else
	PUCHAR pRespBuf[MAX_RESP_BUF_NUM];
	PUSHORT pRespLen[MAX_RESP_BUF_NUM];
	USHORT RespPktNum;
	init_resp_buf (pRespBuf, pRespLen);

	char tccmd[MAX_RESP_BUF_SIZE] = {0};
	char setOffset[8] = {0};

	sprintf(setOffset, "%d", EnumSNRMOffsetValue);

	if(strcmp(m_AdslEntry.snrmoffset, setOffset))
	{
		myprintf("%s [%s]!=[%s]\n", __FUNCTION__, m_AdslEntry.snrmoffset, setOffset);
		memset(tccmd, 0, sizeof(tccmd));
		sprintf(tccmd, "tcapi set Adsl_Entry SNRMOffset %s\x0d\x0a", setOffset);
		SendCmdAndWaitResp(pRespBuf, MAX_RESP_BUF_SIZE, pRespLen, MAX_RESP_BUF_NUM, &RespPktNum, m_AdslMacAddr,
			MAC_RTS_CONSOLE_CMD, (PUCHAR)tccmd, GET_LEN(tccmd), 1);
		m_AdslCommitFlag = 1;
	}
#endif
    return 0;
}

/* Paul add start 2012/10/15, for setting SRA. */
int SetSRA(int EnumSRAValue, int FromAteCmd)
{
#ifndef RTCONFIG_DSL_TCLINUX
    declare_resp_handling_vars(pRespBuf, pRespLen, RespPktNum, tok, pRespStr);
    char InputCmd[10];
    char UserCmd[64];
		InputCmd[0] = '\0';
		UserCmd[0] = '\0';
    
    //SET :0/1/2/3:OlrOff/OlrOn/SRAOFF/SRAON
		if(EnumSRAValue == 1)
			EnumSRAValue = 3;
		else
			EnumSRAValue = 2;

    sprintf(InputCmd, "%d", EnumSRAValue);
    strcpy(UserCmd, SET_SRA_ADSL2);
    strcat(UserCmd, InputCmd);
    strcat(UserCmd, "\x0d\x0a");
    
    SendCmdAndWaitResp(pRespBuf, MAX_RESP_BUF_SIZE, pRespLen, MAX_RESP_BUF_NUM, &RespPktNum, m_AdslMacAddr,
        MAC_RTS_CONSOLE_CMD, (PUCHAR)UserCmd, strlen(UserCmd), 0);
#else
	PUCHAR pRespBuf[MAX_RESP_BUF_NUM];
	PUSHORT pRespLen[MAX_RESP_BUF_NUM];
	USHORT RespPktNum;
	init_resp_buf (pRespBuf, pRespLen);

	char tccmd[MAX_RESP_BUF_SIZE] = {0};
	char setSRAActive[4] = {0};

	if(EnumSRAValue)
		strncpy(setSRAActive, "Yes", sizeof(setSRAActive));
	else
		strncpy(setSRAActive, "No", sizeof(setSRAActive));

	if(strcmp(m_AdslEntry.sraactive, setSRAActive))
	{
		myprintf("%s [%s]!=[%s]\n", __FUNCTION__, m_AdslEntry.sraactive, setSRAActive);
		memset(tccmd, 0, sizeof(tccmd));
		sprintf(tccmd, "tcapi set Adsl_Entry SRAActive %s\x0d\x0a", setSRAActive);
		SendCmdAndWaitResp(pRespBuf, MAX_RESP_BUF_SIZE, pRespLen, MAX_RESP_BUF_NUM, &RespPktNum, m_AdslMacAddr,
			MAC_RTS_CONSOLE_CMD, (PUCHAR)tccmd, GET_LEN(tccmd), 1);
		m_AdslCommitFlag = 1;
	}
#endif
    return 0;
}

/* Paul add start 2013/10/23, for Bitswap control. */
int SetBitswap(int EnumBitswapValue, int FromAteCmd)
{
#ifndef RTCONFIG_DSL_TCLINUX
    declare_resp_handling_vars(pRespBuf, pRespLen, RespPktNum, tok, pRespStr);
    char InputCmd[10];
    char UserCmd[64];
		InputCmd[0] = '\0';
		UserCmd[0] = '\0';
    
    //SET :0/1:OlrOff/OlrOn
    sprintf(InputCmd, "%d", EnumBitswapValue);
    strcpy(UserCmd, SET_Bitswap_ADSL2);
    strcat(UserCmd, InputCmd);
    strcat(UserCmd, "\x0d\x0a");
    
    SendCmdAndWaitResp(pRespBuf, MAX_RESP_BUF_SIZE, pRespLen, MAX_RESP_BUF_NUM, &RespPktNum, m_AdslMacAddr,
        MAC_RTS_CONSOLE_CMD, (PUCHAR)UserCmd, strlen(UserCmd), 0);

		UserCmd[0] = '\0';
		strcpy(UserCmd, SET_Bitswap_ADSL1);
		if(EnumBitswapValue == 0)
			strcat(UserCmd, "off off");
		else
			strcat(UserCmd, "on on");
		
		strcat(UserCmd, "\x0d\x0a");

    SendCmdAndWaitResp(pRespBuf, MAX_RESP_BUF_SIZE, pRespLen, MAX_RESP_BUF_NUM, &RespPktNum, m_AdslMacAddr,
        MAC_RTS_CONSOLE_CMD, (PUCHAR)UserCmd, strlen(UserCmd), 0);
#else
	PUCHAR pRespBuf[MAX_RESP_BUF_NUM];
	PUSHORT pRespLen[MAX_RESP_BUF_NUM];
	USHORT RespPktNum;
	init_resp_buf (pRespBuf, pRespLen);

	char tccmd[MAX_RESP_BUF_SIZE] = {0};
	char setBitSwapActive[4] = {0};

	if(EnumBitswapValue)
		strncpy(setBitSwapActive, "Yes", sizeof(setBitSwapActive));
	else
		strncpy(setBitSwapActive, "No", sizeof(setBitSwapActive));

	if(strcmp(m_AdslEntry.bitswapactive, setBitSwapActive))
	{
		myprintf("%s [%s]!=[%s]\n", __FUNCTION__, m_AdslEntry.bitswapactive, setBitSwapActive);
		memset(tccmd, 0, sizeof(tccmd));
		sprintf(tccmd, "tcapi set Adsl_Entry BitSwapActive %s\x0d\x0a", setBitSwapActive);
		SendCmdAndWaitResp(pRespBuf, MAX_RESP_BUF_SIZE, pRespLen, MAX_RESP_BUF_NUM, &RespPktNum, m_AdslMacAddr,
			MAC_RTS_CONSOLE_CMD, (PUCHAR)tccmd, GET_LEN(tccmd), 1);
		m_AdslCommitFlag = 1;
	}
#endif
    return 0;
}

int HandleAdslSysInfo(ADSL_SYS_INFO* pInfo)
{
#ifndef RTCONFIG_DSL_TCLINUX
    char AdslFwVerBuf[80];
    declare_resp_handling_vars(pRespBuf, pRespLen, RespPktNum, tok, pRespStr);
    SendCmdAndWaitResp(pRespBuf, MAX_RESP_BUF_SIZE, pRespLen, MAX_RESP_BUF_NUM, &RespPktNum, m_AdslMacAddr,
        MAC_RTS_CONSOLE_CMD, (PUCHAR)SYSINFO, GET_LEN(SYSINFO), 0);
    int i;
    int j;
    for(i=0; i<RespPktNum; i++)
    {
        scanner_set(GET_RESP_STR(pRespBuf[i]), GET_RESP_LEN(*pRespLen[i]));
        while (1)
        {
            tok = scanner();
            if (tok == TOKEN_EOF)
            {
                break;
            }
            if (tok == TOKEN_STRING)
            {
                pRespStr = scanner_get_str();
                if (strcmp(pRespStr,"syspwd") == 0)
                {
                    tok = scanner();
                    tok_assert(tok,TOKEN_EQUAL);
                    tok = scanner();
                    tok_assert_two(tok,TOKEN_NUMBER_LITERAL, TOKEN_STRING);
                    tok = scanner();
                    tok_assert(tok,TOKEN_COMMA);

                    tok = scanner();
                    tok_assert(tok,TOKEN_STRING);
                    tok = scanner();
                    tok_assert(tok,TOKEN_EQUAL);
                    for (j=0; j<6; j++)
                    {
                        tok = scanner();
                        tok_assert_two(tok,TOKEN_NUMBER_LITERAL, TOKEN_STRING);
                        tok = scanner();
                        tok_assert_two(tok,TOKEN_COLON,TOKEN_COMMA);
                    }

                    tok = scanner();
                    tok_assert(tok,TOKEN_STRING);
                    tok_assert_buf(scanner_get_str(), "fwver");
                    tok = scanner();
                    tok_assert(tok,TOKEN_EQUAL);
                    tok = scanner();
                    tok_assert(tok,TOKEN_STRING);
                    strcpymax(pInfo->FwVer, scanner_get_str(), sizeof(pInfo->FwVer));
                    tok = scanner();
                    tok_assert(tok,TOKEN_COMMA);
                    tok = scanner();
                    tok_assert(tok,TOKEN_STRING);
                    tok_assert_buf(scanner_get_str(), "adslver");
                    tok = scanner();
                    tok_assert(tok,TOKEN_EQUAL);
                    tok = scanner_get_line();
                    strcpymax(pInfo->AdslFwVer, scanner_get_str(), sizeof(pInfo->AdslFwVer));

                    strcpymax(AdslFwVerBuf, pInfo->AdslFwVer, sizeof(AdslFwVerBuf));
                    scanner_set(AdslFwVerBuf, strlen(AdslFwVerBuf));
                    tok = scanner();
                    tok = scanner();
                    tok_assert_two(tok,TOKEN_EQUAL,TOKEN_COLON);
                    tok = scanner();
                    if (tok == TOKEN_NUMBER_LITERAL)
                    {
                        strcpymax(pInfo->AdslFwVerDisp, scanner_get_str(), sizeof(pInfo->AdslFwVerDisp));
                    }
                    break;
                }
            }
        }
    }
#else
	FILE *fp;
	char buf[64] = {0};
	char tccmd[MAX_RESP_BUF_SIZE];
	PUCHAR pRespBuf[MAX_RESP_BUF_NUM];
	PUSHORT pRespLen[MAX_RESP_BUF_NUM];
	USHORT RespPktNum;
	init_resp_buf (pRespBuf, pRespLen);

	tftp_get_file(REMOTE_FILE_FWVER, LOCAL_FILE_FWVER);
	fp = fopen(LOCAL_FILE_FWVER, "r");
	if(fp) {
		fgets(buf, sizeof(buf), fp);
		nvram_set("dsllog_fwver", buf);
		strncpy(pInfo->FwVer, buf, sizeof(pInfo->FwVer));
		fclose(fp);
	}

	tftp_get_file(REMOTE_FILE_ADSL_FWVER, LOCAL_FILE_ADSL_FWVER);
	fp = fopen(LOCAL_FILE_ADSL_FWVER, "r");
	if(fp) {
		fgets(buf, sizeof(buf), fp);
		nvram_set("dsllog_drvver", buf);
		strncpy(pInfo->AdslFwVer, buf, sizeof(pInfo->AdslFwVer));
		fclose(fp);
	}

	memset(tccmd, 0, sizeof(tccmd));
	sprintf(tccmd, "mtd readflash %s 256 0 tclinux\x0d\x0a ", REMOTE_FILE_TCLINUX_HEADER);
	SendCmdAndWaitResp(pRespBuf, MAX_RESP_BUF_SIZE, pRespLen, MAX_RESP_BUF_NUM, &RespPktNum, m_AdslMacAddr,
		MAC_RTS_CONSOLE_CMD, (PUCHAR)tccmd, GET_LEN(tccmd), 1);

	tftp_get_file(REMOTE_FILE_TCLINUX_HEADER, LOCAL_FILE_TCLINUX_HEADER);
#endif
    return 0;
}

int HandleAdslSysTraffic(ADSL_SYS_TRAFFIC* pTraffic)
{
    declare_resp_handling_vars(pRespBuf, pRespLen, RespPktNum, tok, pRespStr);
#ifndef RTCONFIG_DSL_TCLINUX
    SendCmdAndWaitResp(pRespBuf, MAX_RESP_BUF_SIZE, pRespLen, MAX_RESP_BUF_NUM, &RespPktNum, m_AdslMacAddr,
        MAC_RTS_CONSOLE_CMD, (PUCHAR)SYSTRAFFIC, GET_LEN(SYSTRAFFIC), 0);
    int i;
    for(i=0; i<RespPktNum; i++)
    {
        scanner_set(GET_RESP_STR(pRespBuf[i]), GET_RESP_LEN(*pRespLen[i]));
        while (1)
        {
            tok = scanner();
            if (tok == TOKEN_EOF)
            {
                break;
            }
            if (tok == TOKEN_STRING)
            {
                pRespStr = scanner_get_str();
                if (strcmp(pRespStr,"lantx") == 0)
                {
                    tok = scanner();
                    tok_assert(tok,TOKEN_EQUAL);
                    tok = scanner();
                    tok_assert(tok,TOKEN_NUMBER_LITERAL);
                    strcpymax(pTraffic->LanTx, scanner_get_str(), sizeof(pTraffic->LanTx));
                    tok = scanner();
                    tok_assert(tok,TOKEN_COMMA);

                    tok = scanner();
                    tok_assert(tok,TOKEN_STRING);
                    tok_assert_buf(scanner_get_str(), "lanrx");
                    tok = scanner();
                    tok_assert(tok,TOKEN_EQUAL);
                    tok = scanner();
                    tok_assert(tok,TOKEN_NUMBER_LITERAL);
                    strcpymax(pTraffic->LanRx, scanner_get_str(), sizeof(pTraffic->LanRx));
                    tok = scanner();
                    tok_assert(tok,TOKEN_COMMA);

                    tok = scanner();
                    tok_assert(tok,TOKEN_STRING);
                    tok_assert_buf(scanner_get_str(), "adsltx");
                    tok = scanner();
                    tok_assert(tok,TOKEN_EQUAL);
                    tok = scanner();
                    tok_assert(tok,TOKEN_NUMBER_LITERAL);
                    strcpymax(pTraffic->AdslTx, scanner_get_str(), sizeof(pTraffic->AdslTx));
                    tok = scanner();
                    tok_assert(tok,TOKEN_COMMA);

                    tok = scanner();
                    tok_assert(tok,TOKEN_STRING);
                    tok_assert_buf(scanner_get_str(), "adslrx");
                    tok = scanner();
                    tok_assert(tok,TOKEN_EQUAL);
                    tok = scanner();
                    tok_assert(tok,TOKEN_NUMBER_LITERAL);
                    strcpymax(pTraffic->AdslRx, scanner_get_str(), sizeof(pTraffic->AdslRx));
                }
            }
        }
    }
#else
	int i;
	char tccmd[MAX_RESP_BUF_SIZE] = {0};

	//lan

	//adsl
	sprintf(tccmd, "tcapi get Info_Adsl outPkts\x0d\x0a");
	SendCmdAndWaitResp(pRespBuf, MAX_RESP_BUF_SIZE, pRespLen, MAX_RESP_BUF_NUM, &RespPktNum, m_AdslMacAddr,
		MAC_RTS_CONSOLE_CMD, (PUCHAR)tccmd, GET_LEN(tccmd), 0);
	for(i=0; i<RespPktNum; i++)
	{
		scanner_set(GET_RESP_STR(pRespBuf[i]), GET_RESP_LEN(*pRespLen[i]));
		while (1)
		{
			tok = scanner();
			if (tok == TOKEN_EOF)
			{
				break;
			}
			if (tok == TOKEN_NUMBER_LITERAL)
			{
				pRespStr = scanner_get_str();
				strncpy(pTraffic->AdslTx, pRespStr, sizeof(pTraffic->AdslTx));
				break;
			}
		}
	}

	memset(tccmd, 0 , sizeof(tccmd));
	sprintf(tccmd, "tcapi get Info_Adsl inPkts\x0d\x0a");
	SendCmdAndWaitResp(pRespBuf, MAX_RESP_BUF_SIZE, pRespLen, MAX_RESP_BUF_NUM, &RespPktNum, m_AdslMacAddr,
		MAC_RTS_CONSOLE_CMD, (PUCHAR)tccmd, GET_LEN(tccmd), 0);
	for(i=0; i<RespPktNum; i++)
	{
		scanner_set(GET_RESP_STR(pRespBuf[i]), GET_RESP_LEN(*pRespLen[i]));
		while (1)
		{
			tok = scanner();
			if (tok == TOKEN_EOF)
			{
				break;
			}
			if (tok == TOKEN_NUMBER_LITERAL)
			{
				pRespStr = scanner_get_str();
				strncpy(pTraffic->AdslRx, pRespStr, sizeof(pTraffic->AdslRx));
				break;
			}
		}
	}
#endif
    return 0;
}

int HandleAdslSysStatus(ADSL_SYS_STATUS* pSts)
{
    declare_resp_handling_vars(pRespBuf, pRespLen, RespPktNum, tok, pRespStr);
#ifndef RTCONFIG_DSL_TCLINUX
    SendCmdAndWaitResp(pRespBuf, MAX_RESP_BUF_SIZE, pRespLen, MAX_RESP_BUF_NUM, &RespPktNum, m_AdslMacAddr,
        MAC_RTS_CONSOLE_CMD, (PUCHAR)SYSSTATUS, GET_LEN(SYSSTATUS), 0);

    int i;
    for(i=0; i<RespPktNum; i++)
    {
        scanner_set(GET_RESP_STR(pRespBuf[i]), GET_RESP_LEN(*pRespLen[i]));
        while (1)
        {
            tok = scanner();
            if (tok == TOKEN_EOF)
            {
                break;
            }
            if (tok == TOKEN_STRING)
            {
                pRespStr = scanner_get_str();
                if (strcmp(pRespStr,"index") == 0)
                {
                    tok = scanner();
                    tok_assert(tok,TOKEN_EQUAL);
                    tok = scanner();
                    tok_assert(tok,TOKEN_NUMBER_LITERAL);
                    tok = scanner();
                    tok_assert(tok,TOKEN_COMMA);

                    tok = scanner();
                    tok_assert(tok,TOKEN_STRING);
                    tok = scanner();
                    tok_assert(tok,TOKEN_EQUAL);
                    tok = scanner();
                    tok_assert(tok,TOKEN_NUMBER_LITERAL);
                    tok = scanner();
                    tok_assert(tok,TOKEN_COMMA);

                    tok = scanner();
                    tok_assert(tok,TOKEN_STRING);
                    tok = scanner();
                    tok_assert(tok,TOKEN_EQUAL);
                    tok = scanner();
                    tok_assert(tok,TOKEN_NUMBER_LITERAL);
                    tok = scanner();
                    tok_assert(tok,TOKEN_COMMA);

                    tok = scanner();
                    tok_assert(tok,TOKEN_STRING);
                    tok = scanner();
                    tok_assert(tok,TOKEN_EQUAL);
                    tok = scanner();
                    tok_assert(tok,TOKEN_NUMBER_LITERAL);
                    tok = scanner();
                    tok_assert(tok,TOKEN_COMMA);

                    tok = scanner();
                    tok_assert(tok,TOKEN_STRING);
                    tok_assert_buf(scanner_get_str(), "linestate");
                    tok = scanner();
                    tok_assert(tok,TOKEN_EQUAL);
                    tok = scanner();
                    tok_assert(tok,TOKEN_NUMBER_LITERAL);
                    strcpymax(pSts->LineState, scanner_get_str(), sizeof(pSts->LineState));
                    tok = scanner();
                    tok_assert(tok,TOKEN_COMMA);

                    tok = scanner();
                    tok_assert(tok,TOKEN_STRING);
                    tok_assert_buf(scanner_get_str(), "modulation");
                    tok = scanner();
                    tok_assert(tok,TOKEN_EQUAL);
                    tok = scanner();
                    tok_assert(tok,TOKEN_NUMBER_LITERAL);
                    strcpymax(pSts->Modulation, scanner_get_str(), sizeof(pSts->Modulation));
                    tok = scanner();
                    tok_assert(tok,TOKEN_COMMA);


                    tok = scanner();
                    tok_assert(tok,TOKEN_STRING);
                    tok_assert_buf(scanner_get_str(), "snrup");
                    tok = scanner();
                    tok_assert(tok,TOKEN_EQUAL);
                    tok = scanner();
                    tok_assert_two(tok,TOKEN_NUMBER_LITERAL,TOKEN_STRING);
                    strcpymax(pSts->SnrMarginUp, scanner_get_str(), sizeof(pSts->SnrMarginUp));
                    tok = scanner();
                    tok_assert(tok,TOKEN_COMMA);

                    tok = scanner();
                    tok_assert(tok,TOKEN_STRING);
                    tok_assert_buf(scanner_get_str(), "snrdown");
                    tok = scanner();
                    tok_assert(tok,TOKEN_EQUAL);
                    tok = scanner();
                    tok_assert_two(tok,TOKEN_NUMBER_LITERAL,TOKEN_STRING);
                    strcpymax(pSts->SnrMarginDown, scanner_get_str(), sizeof(pSts->SnrMarginDown));
                    tok = scanner();
                    tok_assert(tok,TOKEN_COMMA);

                    tok = scanner();
                    tok_assert(tok,TOKEN_STRING);
                    tok_assert_buf(scanner_get_str(), "attenup");
                    tok = scanner();
                    tok_assert(tok,TOKEN_EQUAL);
                    tok = scanner();
                    tok_assert_two(tok,TOKEN_NUMBER_LITERAL, TOKEN_STRING);
                    strcpymax(pSts->LineAttenuationUp, scanner_get_str(), sizeof(pSts->LineAttenuationUp));
                    tok = scanner();
                    tok_assert(tok,TOKEN_COMMA);

                    tok = scanner();
                    tok_assert(tok,TOKEN_STRING);
                    tok_assert_buf(scanner_get_str(), "attendown");
                    tok = scanner();
                    tok_assert(tok,TOKEN_EQUAL);
                    tok = scanner();
                    tok_assert_two(tok,TOKEN_NUMBER_LITERAL, TOKEN_STRING);
                    strcpymax(pSts->LineAttenuationDown, scanner_get_str(), sizeof(pSts->LineAttenuationDown));
                    tok = scanner();
                    tok_assert(tok,TOKEN_COMMA);

                    tok = scanner();
                    tok_assert(tok,TOKEN_STRING);
                    tok_assert_buf(scanner_get_str(), "datarateup");
                    tok = scanner();
                    tok_assert(tok,TOKEN_EQUAL);
                    tok = scanner();
                    tok_assert_two(tok,TOKEN_NUMBER_LITERAL, TOKEN_STRING);
                    strcpymax(pSts->DataRateUp, scanner_get_str(), sizeof(pSts->DataRateUp));
                    tok = scanner();
                    tok_assert(tok,TOKEN_COMMA);

                    tok = scanner();
                    tok_assert(tok,TOKEN_STRING);
                    tok_assert_buf(scanner_get_str(), "dataratedown");
                    tok = scanner();
                    tok_assert(tok,TOKEN_EQUAL);
                    tok = scanner();
                    tok_assert_two(tok,TOKEN_NUMBER_LITERAL, TOKEN_STRING);
                    strcpymax(pSts->DataRateDown, scanner_get_str(), sizeof(pSts->DataRateDown));
                    break;
                }
            }
        }
    }
#else
	int i;
	char tccmd[MAX_RESP_BUF_SIZE] = {0};

	sprintf(tccmd, "tcapi get Info_Adsl Opmode\x0d\x0a");
	SendCmdAndWaitResp(pRespBuf, MAX_RESP_BUF_SIZE, pRespLen, MAX_RESP_BUF_NUM, &RespPktNum, m_AdslMacAddr,
		MAC_RTS_CONSOLE_CMD, (PUCHAR)tccmd, GET_LEN(tccmd), 0);
	for(i=0; i<RespPktNum; i++)
	{
		scanner_set(GET_RESP_STR(pRespBuf[i]), GET_RESP_LEN(*pRespLen[i]));
		while (1)
		{
			tok = scanner_get_line();
			if (tok == TOKEN_EOF)
			{
				break;
			}
			if (tok == TOKEN_STRING)
			{
				pRespStr = scanner_get_str();
				if(!strncmp(pRespStr, TC_PROMPT_SYMBOL, sizeof(TC_PROMPT_SYMBOL)))
					break;
				if (!strstr(pRespStr,"Opmode"))
				{
					strncpy(pSts->Modulation, pRespStr, strcspn(pRespStr, "\r"));
					break;
				}
			}
		}
	}
	memset(tccmd, 0 , sizeof(tccmd));
	sprintf(tccmd, "tcapi get Info_Adsl lineState\x0d\x0a");
	SendCmdAndWaitResp(pRespBuf, MAX_RESP_BUF_SIZE, pRespLen, MAX_RESP_BUF_NUM, &RespPktNum, m_AdslMacAddr,
		MAC_RTS_CONSOLE_CMD, (PUCHAR)tccmd, GET_LEN(tccmd), 0);
	for(i=0; i<RespPktNum; i++)
	{
		scanner_set(GET_RESP_STR(pRespBuf[i]), GET_RESP_LEN(*pRespLen[i]));
		while (1)
		{
			tok = scanner_get_line();
			if (tok == TOKEN_EOF)
			{
				break;
			}
			if (tok == TOKEN_STRING)
			{
				pRespStr = scanner_get_str();
				if(!strncmp(pRespStr, TC_PROMPT_SYMBOL, sizeof(TC_PROMPT_SYMBOL)))
					break;
				if (!strstr(pRespStr,"lineState"))
				{
					strncpy(pSts->LineState, pRespStr, strcspn(pRespStr, "\r"));
					break;
				}
			}
		}
	}

	memset(tccmd, 0 , sizeof(tccmd));
	sprintf(tccmd, "tcapi get Info_Adsl SNRMarginUp\x0d\x0a");
	SendCmdAndWaitResp(pRespBuf, MAX_RESP_BUF_SIZE, pRespLen, MAX_RESP_BUF_NUM, &RespPktNum, m_AdslMacAddr,
		MAC_RTS_CONSOLE_CMD, (PUCHAR)tccmd, GET_LEN(tccmd), 0);
	for(i=0; i<RespPktNum; i++)
	{
		scanner_set(GET_RESP_STR(pRespBuf[i]), GET_RESP_LEN(*pRespLen[i]));
		while (1)
		{
			tok = scanner();
			if (tok == TOKEN_EOF)
			{
				break;
			}
			if (tok == TOKEN_NUMBER_LITERAL)
			{
				pRespStr = scanner_get_str();
				strncpy(pSts->SnrMarginUp, pRespStr, sizeof(pSts->SnrMarginUp));
				break;
			}
		}
	}

	memset(tccmd, 0 , sizeof(tccmd));
	sprintf(tccmd, "tcapi get Info_Adsl SNRMarginDown\x0d\x0a");
	SendCmdAndWaitResp(pRespBuf, MAX_RESP_BUF_SIZE, pRespLen, MAX_RESP_BUF_NUM, &RespPktNum, m_AdslMacAddr,
		MAC_RTS_CONSOLE_CMD, (PUCHAR)tccmd, GET_LEN(tccmd), 0);
	for(i=0; i<RespPktNum; i++)
	{
		scanner_set(GET_RESP_STR(pRespBuf[i]), GET_RESP_LEN(*pRespLen[i]));
		while (1)
		{
			tok = scanner();
			if (tok == TOKEN_EOF)
			{
				break;
			}
			if (tok == TOKEN_NUMBER_LITERAL)
			{
				pRespStr = scanner_get_str();
				strncpy(pSts->SnrMarginDown, pRespStr, sizeof(pSts->SnrMarginDown));
				break;
			}
		}
	}

	memset(tccmd, 0 , sizeof(tccmd));
	sprintf(tccmd, "tcapi get Info_Adsl AttenUp\x0d\x0a");
	SendCmdAndWaitResp(pRespBuf, MAX_RESP_BUF_SIZE, pRespLen, MAX_RESP_BUF_NUM, &RespPktNum, m_AdslMacAddr,
		MAC_RTS_CONSOLE_CMD, (PUCHAR)tccmd, GET_LEN(tccmd), 0);
	for(i=0; i<RespPktNum; i++)
	{
		scanner_set(GET_RESP_STR(pRespBuf[i]), GET_RESP_LEN(*pRespLen[i]));
		while (1)
		{
			tok = scanner();
			if (tok == TOKEN_EOF)
			{
				break;
			}
			if (tok == TOKEN_NUMBER_LITERAL)
			{
				pRespStr = scanner_get_str();
				strncpy(pSts->LineAttenuationUp, pRespStr, sizeof(pSts->LineAttenuationUp));
				break;
			}
		}
	}

	memset(tccmd, 0 , sizeof(tccmd));
	sprintf(tccmd, "tcapi get Info_Adsl AttenDown\x0d\x0a");
	SendCmdAndWaitResp(pRespBuf, MAX_RESP_BUF_SIZE, pRespLen, MAX_RESP_BUF_NUM, &RespPktNum, m_AdslMacAddr,
		MAC_RTS_CONSOLE_CMD, (PUCHAR)tccmd, GET_LEN(tccmd), 0);
	for(i=0; i<RespPktNum; i++)
	{
		scanner_set(GET_RESP_STR(pRespBuf[i]), GET_RESP_LEN(*pRespLen[i]));
		while (1)
		{
			tok = scanner();
			if (tok == TOKEN_EOF)
			{
				break;
			}
			if (tok == TOKEN_NUMBER_LITERAL)
			{
				pRespStr = scanner_get_str();
				strncpy(pSts->LineAttenuationDown, pRespStr, sizeof(pSts->LineAttenuationDown));
				break;
			}
		}
	}

	memset(tccmd, 0 , sizeof(tccmd));
	sprintf(tccmd, "tcapi get Info_Adsl DataRateUp\x0d\x0a");
	SendCmdAndWaitResp(pRespBuf, MAX_RESP_BUF_SIZE, pRespLen, MAX_RESP_BUF_NUM, &RespPktNum, m_AdslMacAddr,
		MAC_RTS_CONSOLE_CMD, (PUCHAR)tccmd, GET_LEN(tccmd), 0);
	for(i=0; i<RespPktNum; i++)
	{
		scanner_set(GET_RESP_STR(pRespBuf[i]), GET_RESP_LEN(*pRespLen[i]));
		while (1)
		{
			tok = scanner();
			if (tok == TOKEN_EOF)
			{
				break;
			}
			if (tok == TOKEN_NUMBER_LITERAL)
			{
				pRespStr = scanner_get_str();
				strncpy(pSts->DataRateUp, pRespStr, sizeof(pSts->DataRateUp));
				break;
			}
		}
	}

	memset(tccmd, 0 , sizeof(tccmd));
	sprintf(tccmd, "tcapi get Info_Adsl DataRateDown\x0d\x0a");
	SendCmdAndWaitResp(pRespBuf, MAX_RESP_BUF_SIZE, pRespLen, MAX_RESP_BUF_NUM, &RespPktNum, m_AdslMacAddr,
		MAC_RTS_CONSOLE_CMD, (PUCHAR)tccmd, GET_LEN(tccmd), 0);
	for(i=0; i<RespPktNum; i++)
	{
		scanner_set(GET_RESP_STR(pRespBuf[i]), GET_RESP_LEN(*pRespLen[i]));
		while (1)
		{
			tok = scanner();
			if (tok == TOKEN_EOF)
			{
				break;
			}
			if (tok == TOKEN_NUMBER_LITERAL)
			{
				pRespStr = scanner_get_str();
				strncpy(pSts->DataRateDown, pRespStr, sizeof(pSts->DataRateDown));
				break;
			}
		}
	}

	memset(tccmd, 0 , sizeof(tccmd));
	sprintf(tccmd, "tcapi get Info_Adsl AttainUp\x0d\x0a");
	SendCmdAndWaitResp(pRespBuf, MAX_RESP_BUF_SIZE, pRespLen, MAX_RESP_BUF_NUM, &RespPktNum, m_AdslMacAddr,
		MAC_RTS_CONSOLE_CMD, (PUCHAR)tccmd, GET_LEN(tccmd), 0);
	for(i=0; i<RespPktNum; i++)
	{
		scanner_set(GET_RESP_STR(pRespBuf[i]), GET_RESP_LEN(*pRespLen[i]));
		while (1)
		{
			tok = scanner();
			if (tok == TOKEN_EOF)
			{
				break;
			}
			if (tok == TOKEN_NUMBER_LITERAL)
			{
				pRespStr = scanner_get_str();
				strncpy(pSts->MaxRateUp, pRespStr, sizeof(pSts->MaxRateUp));
				break;
			}
		}
	}

	memset(tccmd, 0 , sizeof(tccmd));
	sprintf(tccmd, "tcapi get Info_Adsl AttainDown\x0d\x0a");
	SendCmdAndWaitResp(pRespBuf, MAX_RESP_BUF_SIZE, pRespLen, MAX_RESP_BUF_NUM, &RespPktNum, m_AdslMacAddr,
		MAC_RTS_CONSOLE_CMD, (PUCHAR)tccmd, GET_LEN(tccmd), 0);
	for(i=0; i<RespPktNum; i++)
	{
		scanner_set(GET_RESP_STR(pRespBuf[i]), GET_RESP_LEN(*pRespLen[i]));
		while (1)
		{
			tok = scanner();
			if (tok == TOKEN_EOF)
			{
				break;
			}
			if (tok == TOKEN_NUMBER_LITERAL)
			{
				pRespStr = scanner_get_str();
				strncpy(pSts->MaxRateDown, pRespStr, sizeof(pSts->MaxRateDown));
				break;
			}
		}
	}

	memset(tccmd, 0 , sizeof(tccmd));
	sprintf(tccmd, "tcapi get Info_Adsl PowerUp\x0d\x0a");
	SendCmdAndWaitResp(pRespBuf, MAX_RESP_BUF_SIZE, pRespLen, MAX_RESP_BUF_NUM, &RespPktNum, m_AdslMacAddr,
		MAC_RTS_CONSOLE_CMD, (PUCHAR)tccmd, GET_LEN(tccmd), 0);
	for(i=0; i<RespPktNum; i++)
	{
		scanner_set(GET_RESP_STR(pRespBuf[i]), GET_RESP_LEN(*pRespLen[i]));
		while (1)
		{
			tok = scanner();
			if (tok == TOKEN_EOF)
			{
				break;
			}
			if (tok == TOKEN_NUMBER_LITERAL)
			{
				pRespStr = scanner_get_str();
				strncpy(pSts->PowerUp, pRespStr, sizeof(pSts->PowerUp));
				break;
			}
		}
	}

	memset(tccmd, 0 , sizeof(tccmd));
	sprintf(tccmd, "tcapi get Info_Adsl PowerDown\x0d\x0a");
	SendCmdAndWaitResp(pRespBuf, MAX_RESP_BUF_SIZE, pRespLen, MAX_RESP_BUF_NUM, &RespPktNum, m_AdslMacAddr,
		MAC_RTS_CONSOLE_CMD, (PUCHAR)tccmd, GET_LEN(tccmd), 0);
	for(i=0; i<RespPktNum; i++)
	{
		scanner_set(GET_RESP_STR(pRespBuf[i]), GET_RESP_LEN(*pRespLen[i]));
		while (1)
		{
			tok = scanner();
			if (tok == TOKEN_EOF)
			{
				break;
			}
			if (tok == TOKEN_NUMBER_LITERAL)
			{
				pRespStr = scanner_get_str();
				strncpy(pSts->PowerDown, pRespStr, sizeof(pSts->PowerDown));
				break;
			}
		}
	}
#endif
    return 0;
}

int HandleAdslStats(ADSL_STATS* pStats)
{
    declare_resp_handling_vars(pRespBuf, pRespLen, RespPktNum, tok, pRespStr);
#ifndef RTCONFIG_DSL_TCLINUX
    SendCmdAndWaitResp(pRespBuf, MAX_RESP_BUF_SIZE, pRespLen, MAX_RESP_BUF_NUM, &RespPktNum, m_AdslMacAddr,
        MAC_RTS_CONSOLE_CMD, (PUCHAR)SHOW_ADSL_STATS, GET_LEN(SHOW_ADSL_STATS), 0);

    int i;
    for(i=0; i<RespPktNum; i++)
    {
        scanner_set(GET_RESP_STR(pRespBuf[i]), GET_RESP_LEN(*pRespLen[i]));
        while (1)
        {
            tok = scanner();
            if (tok == TOKEN_EOF)
            {
                break;
            }
            if (tok == TOKEN_STRING)
            {
                pRespStr = scanner_get_str();
                if (strcmp(pRespStr,"crcdown") == 0)
                {
                    tok = scanner();
                    tok_assert(tok,TOKEN_EQUAL);
                    tok = scanner();
                    tok_assert(tok,TOKEN_NUMBER_LITERAL);
                    strcpymax(pStats->CrcDown, scanner_get_str(), sizeof(pStats->CrcDown));
                    tok = scanner();
                    tok_assert(tok,TOKEN_COMMA);

                    tok = scanner();
                    tok_assert(tok,TOKEN_STRING);
                    tok_assert_buf(scanner_get_str(), "crcup");
                    tok = scanner();
                    tok_assert(tok,TOKEN_EQUAL);
                    tok = scanner();
                    tok_assert(tok,TOKEN_NUMBER_LITERAL);
                    strcpymax(pStats->CrcUp, scanner_get_str(), sizeof(pStats->CrcUp));
                    tok = scanner();
                    tok_assert(tok,TOKEN_COMMA);

                    tok = scanner();
                    tok_assert(tok,TOKEN_STRING);
                    tok_assert_buf(scanner_get_str(), "fecdown");
                    tok = scanner();
                    tok_assert(tok,TOKEN_EQUAL);
                    tok = scanner();
                    tok_assert(tok,TOKEN_NUMBER_LITERAL);
                    strcpymax(pStats->FecDown, scanner_get_str(), sizeof(pStats->FecDown));
                    tok = scanner();
                    tok_assert(tok,TOKEN_COMMA);


                    tok = scanner();
                    tok_assert(tok,TOKEN_STRING);
                    tok_assert_buf(scanner_get_str(), "fecup");
                    tok = scanner();
                    tok_assert(tok,TOKEN_EQUAL);
                    tok = scanner();
                    tok_assert_two(tok,TOKEN_NUMBER_LITERAL,TOKEN_STRING);
                    strcpymax(pStats->FecUp, scanner_get_str(), sizeof(pStats->FecUp));
                    tok = scanner();
                    tok_assert(tok,TOKEN_COMMA);

                    tok = scanner();
                    tok_assert(tok,TOKEN_STRING);
                    tok_assert_buf(scanner_get_str(), "hecdown");
                    tok = scanner();
                    tok_assert(tok,TOKEN_EQUAL);
                    tok = scanner();
                    tok_assert_two(tok,TOKEN_NUMBER_LITERAL,TOKEN_STRING);
                    strcpymax(pStats->HecDown, scanner_get_str(), sizeof(pStats->HecDown));
                    tok = scanner();
                    tok_assert(tok,TOKEN_COMMA);

                    tok = scanner();
                    tok_assert(tok,TOKEN_STRING);
                    tok_assert_buf(scanner_get_str(), "hecup");
                    tok = scanner();
                    tok_assert(tok,TOKEN_EQUAL);
                    tok = scanner();
                    tok_assert_two(tok,TOKEN_NUMBER_LITERAL, TOKEN_STRING);
                    strcpymax(pStats->HecUp, scanner_get_str(), sizeof(pStats->HecUp));
                    break;
                }
            }
        }
    }
#else
	int i;
	char tccmd[MAX_RESP_BUF_SIZE] = {0};

	sprintf(tccmd, "tcapi get Info_Adsl CRCDown\x0d\x0a");
	SendCmdAndWaitResp(pRespBuf, MAX_RESP_BUF_SIZE, pRespLen, MAX_RESP_BUF_NUM, &RespPktNum, m_AdslMacAddr,
		MAC_RTS_CONSOLE_CMD, (PUCHAR)tccmd, GET_LEN(tccmd), 0);
	for(i=0; i<RespPktNum; i++)
	{
		scanner_set(GET_RESP_STR(pRespBuf[i]), GET_RESP_LEN(*pRespLen[i]));
		while (1)
		{
			tok = scanner();
			if (tok == TOKEN_EOF)
			{
				break;
			}
			if (tok == TOKEN_NUMBER_LITERAL)
			{
				pRespStr = scanner_get_str();
				strncpy(pStats->CrcDown, pRespStr, sizeof(pStats->CrcDown));
				break;
			}
		}
	}

	memset(tccmd, 0 , sizeof(tccmd));
	sprintf(tccmd, "tcapi get Info_Adsl CRCUp\x0d\x0a");
	SendCmdAndWaitResp(pRespBuf, MAX_RESP_BUF_SIZE, pRespLen, MAX_RESP_BUF_NUM, &RespPktNum, m_AdslMacAddr,
		MAC_RTS_CONSOLE_CMD, (PUCHAR)tccmd, GET_LEN(tccmd), 0);
	for(i=0; i<RespPktNum; i++)
	{
		scanner_set(GET_RESP_STR(pRespBuf[i]), GET_RESP_LEN(*pRespLen[i]));
		while (1)
		{
			tok = scanner();
			if (tok == TOKEN_EOF)
			{
				break;
			}
			if (tok == TOKEN_NUMBER_LITERAL)
			{
				pRespStr = scanner_get_str();
				strncpy(pStats->CrcUp, pRespStr, sizeof(pStats->CrcUp));
				break;
			}
		}
	}
/*
	memset(tccmd, 0 , sizeof(tccmd));
	sprintf(tccmd, "tcapi get Info_Adsl HECDown\x0d\x0a");
	SendCmdAndWaitResp(pRespBuf, MAX_RESP_BUF_SIZE, pRespLen, MAX_RESP_BUF_NUM, &RespPktNum, m_AdslMacAddr,
		MAC_RTS_CONSOLE_CMD, (PUCHAR)tccmd, GET_LEN(tccmd), 0);
	for(i=0; i<RespPktNum; i++)
	{
		scanner_set(GET_RESP_STR(pRespBuf[i]), GET_RESP_LEN(*pRespLen[i]));
		while (1)
		{
			tok = scanner();
			if (tok == TOKEN_EOF)
			{
				break;
			}
			if (tok == TOKEN_NUMBER_LITERAL)
			{
				pRespStr = scanner_get_str();
				strncpy(pStats->HecDown, pRespStr, sizeof(pStats->HecDown));
				break;
			}
		}
	}

	memset(tccmd, 0 , sizeof(tccmd));
	sprintf(tccmd, "tcapi get Info_Adsl HECUp\x0d\x0a");
	SendCmdAndWaitResp(pRespBuf, MAX_RESP_BUF_SIZE, pRespLen, MAX_RESP_BUF_NUM, &RespPktNum, m_AdslMacAddr,
		MAC_RTS_CONSOLE_CMD, (PUCHAR)tccmd, GET_LEN(tccmd), 0);
	for(i=0; i<RespPktNum; i++)
	{
		scanner_set(GET_RESP_STR(pRespBuf[i]), GET_RESP_LEN(*pRespLen[i]));
		while (1)
		{
			tok = scanner();
			if (tok == TOKEN_EOF)
			{
				break;
			}
			if (tok == TOKEN_NUMBER_LITERAL)
			{
				pRespStr = scanner_get_str();
				strncpy(pStats->HecUp, pRespStr, sizeof(pStats->HecUp));
				break;
			}
		}
	}

		memset(tccmd, 0 , sizeof(tccmd));
	sprintf(tccmd, "tcapi get Info_Adsl FECDown\x0d\x0a");
	SendCmdAndWaitResp(pRespBuf, MAX_RESP_BUF_SIZE, pRespLen, MAX_RESP_BUF_NUM, &RespPktNum, m_AdslMacAddr,
		MAC_RTS_CONSOLE_CMD, (PUCHAR)tccmd, GET_LEN(tccmd), 0);
	for(i=0; i<RespPktNum; i++)
	{
		scanner_set(GET_RESP_STR(pRespBuf[i]), GET_RESP_LEN(*pRespLen[i]));
		while (1)
		{
			tok = scanner();
			if (tok == TOKEN_EOF)
			{
				break;
			}
			if (tok == TOKEN_NUMBER_LITERAL)
			{
				pRespStr = scanner_get_str();
				strncpy(pStats->FecDown, pRespStr, sizeof(pStats->FecDown));
				break;
			}
		}
	}

	memset(tccmd, 0 , sizeof(tccmd));
	sprintf(tccmd, "tcapi get Info_Adsl FECUp\x0d\x0a");
	SendCmdAndWaitResp(pRespBuf, MAX_RESP_BUF_SIZE, pRespLen, MAX_RESP_BUF_NUM, &RespPktNum, m_AdslMacAddr,
		MAC_RTS_CONSOLE_CMD, (PUCHAR)tccmd, GET_LEN(tccmd), 0);
	for(i=0; i<RespPktNum; i++)
	{
		scanner_set(GET_RESP_STR(pRespBuf[i]), GET_RESP_LEN(*pRespLen[i]));
		while (1)
		{
			tok = scanner();
			if (tok == TOKEN_EOF)
			{
				break;
			}
			if (tok == TOKEN_NUMBER_LITERAL)
			{
				pRespStr = scanner_get_str();
				strncpy(pStats->FecUp, pRespStr, sizeof(pStats->FecUp));
				break;
			}
		}
	}
*/
#endif
    return 0;
}

//Paul add 2012/4/21
int HandleAdslActualAnnexMode(ADSL_ACTUAL_ANNEX_MODE* pStats)
{
    declare_resp_handling_vars(pRespBuf, pRespLen, RespPktNum, tok, pRespStr);
#ifndef RTCONFIG_DSL_TCLINUX
    SendCmdAndWaitResp(pRespBuf, MAX_RESP_BUF_SIZE, pRespLen, MAX_RESP_BUF_NUM, &RespPktNum, m_AdslMacAddr,
        MAC_RTS_CONSOLE_CMD, (PUCHAR)SHOW_ADSL_ACTUAL_ANNEX_MODE, GET_LEN(SHOW_ADSL_ACTUAL_ANNEX_MODE), 0);

    int i;
    for(i=0; i<RespPktNum; i++)
    {
        scanner_set(GET_RESP_STR(pRespBuf[i]), GET_RESP_LEN(*pRespLen[i]));
        while (1)
        {
            tok = scanner();
            if (tok == TOKEN_EOF)
            {
                break;
            }
            if (tok == TOKEN_STRING)
            {
                pRespStr = scanner_get_str();
                if (strcmp(pRespStr,"Annex") == 0)
                {
                    tok = scanner_get_line();
                    strcpymax(pStats->annexMode, scanner_get_str(), sizeof(pStats->annexMode));
                    break;
                }
            }
        }
    }
#else
	int i;
	char tccmd[MAX_RESP_BUF_SIZE] = {0};
	sprintf(tccmd, "tcapi get Info_Adsl AdslType\x0d\x0a");
	SendCmdAndWaitResp(pRespBuf, MAX_RESP_BUF_SIZE, pRespLen, MAX_RESP_BUF_NUM, &RespPktNum, m_AdslMacAddr,
		MAC_RTS_CONSOLE_CMD, (PUCHAR)tccmd, GET_LEN(tccmd), 0);
	for(i=0; i<RespPktNum; i++)
	{
		scanner_set(GET_RESP_STR(pRespBuf[i]), GET_RESP_LEN(*pRespLen[i]));
		while (1)
		{
			tok = scanner_get_line();
			if (tok == TOKEN_EOF)
			{
				break;
			}
			if (tok == TOKEN_STRING)
			{
				pRespStr = scanner_get_str();
				if(!strncmp(pRespStr, TC_PROMPT_SYMBOL, sizeof(TC_PROMPT_SYMBOL)))
					break;
				if (!strstr(pRespStr,"AdslType"))
				{
					strncpy(pStats->annexMode, pRespStr, strcspn(pRespStr, "\r"));
					break;
				}
			}
		}
	}
#endif
    return 0;
}

int HandleAdslLinkStatus(ADSL_LINK_STATUS* pSts)
{
    declare_resp_handling_vars(pRespBuf, pRespLen, RespPktNum, tok, pRespStr);

    SendCmdAndWaitResp(pRespBuf, MAX_RESP_BUF_SIZE, pRespLen, MAX_RESP_BUF_NUM, &RespPktNum, m_AdslMacAddr,
        MAC_RTS_CONSOLE_CMD, (PUCHAR)GET_ADSL, GET_LEN(GET_ADSL), 0);

    int i;
    for(i=0; i<RespPktNum; i++)
    {
        scanner_set(GET_RESP_STR(pRespBuf[i]), GET_RESP_LEN(*pRespLen[i]));
        while (1)
        {
            tok = scanner();
            if (tok == TOKEN_EOF)
            {
                break;
            }
            if (tok == TOKEN_STRING)
            {
                pRespStr = scanner_get_str();
                if (strcmp(pRespStr,"current") == 0)
                {
                    tok = scanner();
                    tok_assert(tok,TOKEN_STRING);
                    tok_assert_buf(scanner_get_str(), "modem");

                    tok = scanner();
                    tok_assert(tok,TOKEN_STRING);
                    tok_assert_buf(scanner_get_str(), "status");

                    tok = scanner();
                    tok_assert(tok,TOKEN_COLON);

                    tok = scanner();
                    tok_assert(tok,TOKEN_STRING);
                    strcpymax(pSts->LineState, scanner_get_str(), sizeof(pSts->LineState));
                }
            }
        }
    }
    return 0;
}

void GetLinkSts(char* buf, int max_len)
{
    memset(&m_LinkSts,0,sizeof(m_LinkSts));
    HandleAdslLinkStatus(&m_LinkSts);
    strcpymax(buf,m_LinkSts.LineState,max_len);
}

void UpdateAllAdslSts(unsigned int cnt, int from_ate)
{
	if(cnt%5)
		return;

		char timestampstr[32];
#ifndef RTCONFIG_DSL_TCLINUX
    if (from_ate == 0)
    {
        memset(&m_SysTraffic, 0, sizeof(m_SysTraffic));
        HandleAdslSysTraffic(&m_SysTraffic);
        memset(&m_AnnexSts, 0, sizeof(m_AnnexSts));
        HandleAdslActualAnnexMode(&m_AnnexSts); //Paul add 2012/4/21
    }
    memset(&m_Stats, 0, sizeof(m_Stats));
    HandleAdslStats(&m_Stats);
    memset(&m_SysSts, 0, sizeof(m_SysSts));
    HandleAdslSysStatus(&m_SysSts);

    FILE* fp;

    if (from_ate)
    {
#ifdef WIN32
        fp = fopen("\\adsl_stats.txt","wb");
#else
        fp = fopen("/tmp/adsl/adslate.log","wb");
#endif
        if (fp == NULL) return;

        fputs("CRC Down : ",fp);
        fputs(m_Stats.CrcDown,fp);
        fputs("\n",fp);
        fputs("CRC Up : ",fp);
        fputs(m_Stats.CrcUp,fp);
        fputs("\n",fp);
        fputs("FEC Down : ",fp);
        fputs(m_Stats.FecDown,fp);
        fputs("\n",fp);
        fputs("FEC Up : ",fp);
        fputs(m_Stats.FecUp,fp);
        fputs("\n",fp);
        fputs("HEC Down : ",fp);
        fputs(m_Stats.HecDown,fp);
        fputs("\n",fp);
        fputs("HEC Up : ",fp);
        fputs(m_Stats.HecUp,fp);
        fputs("\n",fp);
        fputs("Line State : ",fp);
        fputs(m_SysSts.LineState,fp);
        fputs("\n",fp);
        fputs("Modulation : ",fp);
        fputs(m_SysSts.Modulation,fp);
        fputs("\n",fp);
        fputs("SNR Up : ",fp);
        fputs(m_SysSts.SnrMarginUp,fp);
        fputs("\n",fp);
        fputs("SNR Down : ",fp);
        fputs(m_SysSts.SnrMarginDown,fp);
        fputs("\n",fp);
        fputs("Line Attenuation Up : ",fp);
        fputs(m_SysSts.LineAttenuationUp,fp);
        fputs("\n",fp);
        fputs("Line Attenuation Down : ",fp);
        fputs(m_SysSts.LineAttenuationDown,fp);
        fputs("\n",fp);
        fputs("Data Rate Up : ",fp);
        fputs(m_SysSts.DataRateUp,fp);
        fputs("\n",fp);
        fputs("Data Rate Down : ",fp);
        fputs(m_SysSts.DataRateDown,fp);
        //fputs("\n",fp);

        fclose(fp);
        return;
    }

		/* Paul add 2012/12/25 */
		if (strcmp(m_SysSts.LineState,"3") == 0 && uptime_Saved == 0)
		{
			timestamp = adsluptime();
			memset(timestampstr, 0, 32);
			sprintf(timestampstr, "%lu", timestamp);
			nvram_set("adsl_timestamp", timestampstr);
			uptime_Saved = 1;
		}
		else if (strcmp(m_SysSts.LineState,"3") != 0)
		{
			if(uptime_Saved == 1)
				nvram_set("adsl_timestamp", "");

			uptime_Saved = 0;	
		}

    if (strcmp(m_SysSts.LineState,"0") == 0)
    {
			nvram_adslsts("down");
			nvram_adslsts_detail("down");
    }
    else if (strcmp(m_SysSts.LineState,"1") == 0)
    {
			nvram_adslsts("wait for init"); //Paul modify 2012/6/19
			nvram_adslsts_detail("wait_for_init");
    }
    else if (strcmp(m_SysSts.LineState,"2") == 0)
    {
			nvram_adslsts("init"); //Paul modify 2012/6/19
			nvram_adslsts_detail("init");
    }
    //else if (strcmp(m_SysSts.LineState,"3") == 0) //Paul comment 2012/11/21 for now
    else
    {
			nvram_adslsts("up");
			nvram_adslsts_detail("up");
    }

	if (m_DbgOutputRedirectToFile) return;

//    FILE* fp;
#ifdef WIN32
    fp = fopen("\\adsl_stats.txt","wb");
#else
    fp = fopen("/tmp/adsl/adsllog.log","wb");
#endif
    if (fp == NULL) return;
		
		//Paul comment 2012/12/4, no need counter.
    //char buf[64];
    //sprintf(buf, "Update Counter : %d\n", cnt);
    //fputs(buf,fp);
    fputs("Modulation : ",fp);
    fputs(m_SysSts.Modulation,fp);
    fputs("\n",fp);
    fputs("Annex Mode : Annex ",fp);
    fputs(m_AnnexSts.annexMode,fp); //Paul add 2012/4/21
    fputs("\n",fp);
    fputs("Line State : ",fp);
    fputs(m_SysSts.LineState,fp);
    fputs("\n",fp);
    fputs("Lan Tx : ",fp);
    fputs(m_SysTraffic.LanTx,fp);
    fputs("\n",fp);
    fputs("Lan Rx : ",fp);
    fputs(m_SysTraffic.LanRx,fp);
    fputs("\n",fp);
    fputs("ADSL Tx : ",fp);
    fputs(m_SysTraffic.AdslTx,fp);
    fputs("\n",fp);
    fputs("ADSL Rx : ",fp);
    fputs(m_SysTraffic.AdslRx,fp);
    fputs("\n",fp);
    fputs("CRC Down : ",fp);
    fputs(m_Stats.CrcDown,fp);
    fputs("\n",fp);
    fputs("CRC Up : ",fp);
    fputs(m_Stats.CrcUp,fp);
    fputs("\n",fp);
    fputs("FEC Down : ",fp);
    fputs(m_Stats.FecDown,fp);
    fputs("\n",fp);
    fputs("FEC Up : ",fp);
    fputs(m_Stats.FecUp,fp);
    fputs("\n",fp);
    fputs("HEC Down : ",fp);
    fputs(m_Stats.HecDown,fp);
    fputs("\n",fp);
    fputs("HEC Up : ",fp);
    fputs(m_Stats.HecUp,fp);
    fputs("\n",fp);
    fputs("SNR Up : ",fp);
    fputs(m_SysSts.SnrMarginUp,fp);
    fputs("\n",fp);
    fputs("SNR Down : ",fp);
    fputs(m_SysSts.SnrMarginDown,fp);
    fputs("\n",fp);
    fputs("Line Attenuation Up : ",fp);
    fputs(m_SysSts.LineAttenuationUp,fp);
    fputs("\n",fp);
    fputs("Line Attenuation Down : ",fp);
    fputs(m_SysSts.LineAttenuationDown,fp);
    fputs("\n",fp);
    fputs("Data Rate Up : ",fp);
    fputs(m_SysSts.DataRateUp,fp);
    fputs("\n",fp);
    fputs("Data Rate Down : ",fp);
    fputs(m_SysSts.DataRateDown,fp);
    fputs("\n",fp);

    fclose(fp);
#else
	HandleInfoAdsl();
	//may change to m_InfoAdsl.adsluptime
	if (strcmp(m_InfoAdsl.linestate,"up") == 0 && uptime_Saved == 0)
	{
		timestamp = adsluptime();
		memset(timestampstr, 0, 32);
		sprintf(timestampstr, "%lu", timestamp);
		nvram_set("adsl_timestamp", timestampstr);
		uptime_Saved = 1;
	}
	else if (strcmp(m_InfoAdsl.linestate,"up") != 0)
	{
		if(uptime_Saved == 1)
			nvram_set("adsl_timestamp", "");
		uptime_Saved = 0;
	}
	nvram_adslsts(m_InfoAdsl.linestate);
	nvram_adslsts_detail(m_InfoAdsl.linestate);

	nvram_set("dsllog_opmode", m_InfoAdsl.opmode);
	nvram_set("dsllog_adsltype", m_InfoAdsl.adsltype);
	nvram_set("dsllog_snrmargindown", m_InfoAdsl.snrmargindown);
	nvram_set("dsllog_snrmarginup", m_InfoAdsl.snrmarginup);
	nvram_set("dsllog_attendown", m_InfoAdsl.attendown);
	nvram_set("dsllog_attenup", m_InfoAdsl.attenup);
	nvram_set("dsllog_dataratedown", m_InfoAdsl.dataratedown);
	nvram_set("dsllog_datarateup", m_InfoAdsl.datarateup);
	nvram_set("dsllog_attaindown", m_InfoAdsl.attaindown);
	nvram_set("dsllog_attainup", m_InfoAdsl.attainup);
	nvram_set("dsllog_powerdown", m_InfoAdsl.powerdown);
	nvram_set("dsllog_powerup", m_InfoAdsl.powerup);
	nvram_set("dsllog_crcdown", m_InfoAdsl.crcdown);
	nvram_set("dsllog_crcup", m_InfoAdsl.crcup);
#endif
}

void GetPvcStats(char* outpkts, char* inpkts, char* incrcerr, int vpi, int vci)
{
    declare_resp_handling_vars(pRespBuf, pRespLen, RespPktNum, tok, pRespStr);
    char cmdbuf[80];
    int i;
    *outpkts = 0;
    *inpkts = 0;
    *incrcerr = 0;
    sprintf(cmdbuf, "sys tpget wan hwsar statistics %d %d\x0d\x0a", vpi, vci);
    SendCmdAndWaitResp(pRespBuf, MAX_RESP_BUF_SIZE, pRespLen, MAX_RESP_BUF_NUM, &RespPktNum, m_AdslMacAddr,
        MAC_RTS_CONSOLE_CMD, (PUCHAR)cmdbuf, strlen(cmdbuf), 0);
    for(i=0; i<RespPktNum; i++)
    {
        scanner_set(GET_RESP_STR(pRespBuf[i]), GET_RESP_LEN(*pRespLen[i]));
        while (1)
        {
            tok = scanner();
            if (tok == TOKEN_EOF)
            {
                break;
            }
            if (tok == TOKEN_STRING)
            {
                pRespStr = scanner_get_str();
                if (strcmp(pRespStr,"outPkts") == 0)
                {
                    tok = scanner();
                    tok_assert(tok,TOKEN_EQUAL);
                    tok = scanner();
                    tok_assert(tok,TOKEN_NUMBER_LITERAL_HEX_FORMAT);

                    strcpy(outpkts,scanner_get_str());

                    tok = scanner();
                    tok_assert(tok,TOKEN_COMMA);
                    tok = scanner();
                    tok_assert(tok,TOKEN_STRING);
                    tok_assert_buf(scanner_get_str(), "inPkts");
                    tok = scanner();
                    tok_assert(tok,TOKEN_EQUAL);
                    tok = scanner();
                    tok_assert(tok,TOKEN_NUMBER_LITERAL_HEX_FORMAT);

                    strcpy(inpkts,scanner_get_str());

                    tok = scanner();
                    tok_assert(tok,TOKEN_COMMA);
                    tok = scanner();
                    tok_assert(tok,TOKEN_STRING);
                    tok_assert_buf(scanner_get_str(), "inCrcErr");
                    tok = scanner();
                    tok_assert(tok,TOKEN_EQUAL);
                    tok = scanner();
                    tok_assert(tok,TOKEN_NUMBER_LITERAL_HEX_FORMAT);

                    strcpy(incrcerr,scanner_get_str());
                }
            }
        }
    }
}

int GetAllActivePvc(int idx[], int vlanid[], int vpi[], int vci[], int enc[], int mode[], int qostype[], int pcr[], int scr[], int mbs[])
{
    declare_resp_handling_vars(pRespBuf, pRespLen, RespPktNum, tok, pRespStr);
    SendCmdAndWaitResp(pRespBuf, MAX_RESP_BUF_SIZE, pRespLen, MAX_RESP_BUF_NUM, &RespPktNum, m_AdslMacAddr,
        MAC_RTS_CONSOLE_CMD, (PUCHAR)PVC_DISP, GET_LEN(PVC_DISP), 0);
    int i;
    int pvc_cnt = 0;
    for(i=0; i<RespPktNum; i++)
    {
        scanner_set(GET_RESP_STR(pRespBuf[i]), GET_RESP_LEN(*pRespLen[i]));
        while (1)
        {
            tok = scanner();
            if (tok == TOKEN_EOF)
            {
                break;
            }
            if (tok == TOKEN_STRING)
            {
                pRespStr = scanner_get_str();
                if (strcmp(pRespStr,"index") == 0)
                {
                    tok = scanner();
                    tok_assert(tok,TOKEN_EQUAL);
                    tok = scanner();
                    tok_assert(tok,TOKEN_NUMBER_LITERAL);

                    idx[pvc_cnt] = atoi(scanner_get_str());

                    tok = scanner();
                    tok_assert(tok,TOKEN_COMMA);
                    tok = scanner();
                    tok_assert(tok,TOKEN_STRING);
                    tok_assert_buf(scanner_get_str(), "active");
                    tok = scanner();
                    tok_assert(tok,TOKEN_EQUAL);
                    tok = scanner();
                    tok_assert(tok,TOKEN_NUMBER_LITERAL);
                    if (1==atoi(scanner_get_str()))
                    {
                        tok = scanner();
                        tok_assert(tok,TOKEN_COMMA);
                        tok = scanner();
                        tok_assert(tok,TOKEN_STRING);
                        tok_assert_buf(scanner_get_str(), "encap");
                        tok = scanner();
                        tok_assert(tok,TOKEN_EQUAL);
                        tok = scanner();
                        tok_assert(tok,TOKEN_NUMBER_LITERAL);

                        enc[pvc_cnt] = atoi(scanner_get_str());

                        tok = scanner();
                        tok_assert(tok,TOKEN_COMMA);
                        tok = scanner();
                        tok_assert(tok,TOKEN_STRING);
                        tok_assert_buf(scanner_get_str(), "mode");
                        tok = scanner();
                        tok_assert(tok,TOKEN_EQUAL);
                        tok = scanner();
                        tok_assert(tok,TOKEN_NUMBER_LITERAL);

                        mode[pvc_cnt] = atoi(scanner_get_str());

                        tok = scanner();
                        tok_assert(tok,TOKEN_COMMA);
                        tok = scanner();
                        tok_assert(tok,TOKEN_STRING);
                        tok_assert_buf(scanner_get_str(), "vid");
                        tok = scanner();
                        tok_assert(tok,TOKEN_EQUAL);
                        tok = scanner();
                        tok_assert(tok,TOKEN_NUMBER_LITERAL);

                        vlanid[pvc_cnt] = atoi(scanner_get_str());

                        tok = scanner();
                        tok_assert(tok,TOKEN_COMMA);
                        tok = scanner();
                        tok_assert(tok,TOKEN_STRING);
                        tok_assert_buf(scanner_get_str(), "vpi");
                        tok = scanner();
                        tok_assert(tok,TOKEN_EQUAL);
                        tok = scanner();
                        tok_assert(tok,TOKEN_NUMBER_LITERAL);

                        vpi[pvc_cnt] = atoi(scanner_get_str());

                        tok = scanner();
                        tok_assert(tok,TOKEN_COMMA);
                        tok = scanner();
                        tok_assert(tok,TOKEN_STRING);
                        tok_assert_buf(scanner_get_str(), "vci");
                        tok = scanner();
                        tok_assert(tok,TOKEN_EQUAL);
                        tok = scanner();
                        tok_assert(tok,TOKEN_NUMBER_LITERAL);

                        vci[pvc_cnt] = atoi(scanner_get_str());

                        tok = scanner();
                        tok_assert(tok,TOKEN_COMMA);
                        tok = scanner();
                        tok_assert(tok,TOKEN_STRING);
                        tok_assert_buf(scanner_get_str(), "qostype");
                        tok = scanner();
                        tok_assert(tok,TOKEN_EQUAL);
                        tok = scanner();
                        tok_assert(tok,TOKEN_NUMBER_LITERAL);

                        qostype[pvc_cnt] = atoi(scanner_get_str());

                        tok = scanner();
                        tok_assert(tok,TOKEN_COMMA);
                        tok = scanner();
                        tok_assert(tok,TOKEN_STRING);
                        tok_assert_buf(scanner_get_str(), "pcr");
                        tok = scanner();
                        tok_assert(tok,TOKEN_EQUAL);
                        tok = scanner();
                        tok_assert(tok,TOKEN_NUMBER_LITERAL);

                        pcr[pvc_cnt] = atoi(scanner_get_str());

                        tok = scanner();
                        tok_assert(tok,TOKEN_COMMA);
                        tok = scanner();
                        tok_assert(tok,TOKEN_STRING);
                        tok_assert_buf(scanner_get_str(), "scr");
                        tok = scanner();
                        tok_assert(tok,TOKEN_EQUAL);
                        tok = scanner();
                        tok_assert(tok,TOKEN_NUMBER_LITERAL);

                        scr[pvc_cnt] = atoi(scanner_get_str());

                        tok = scanner();
                        tok_assert(tok,TOKEN_COMMA);
                        tok = scanner();
                        tok_assert(tok,TOKEN_STRING);
                        tok_assert_buf(scanner_get_str(), "mbs");
                        tok = scanner();
                        tok_assert(tok,TOKEN_EQUAL);
                        tok = scanner();
                        tok_assert(tok,TOKEN_NUMBER_LITERAL);

                        mbs[pvc_cnt] = atoi(scanner_get_str());

                        pvc_cnt++;
                    }
                }
            }
        }
    }

    return pvc_cnt;
}

void DispAllPvc()
{
    FILE* fp;
    int i;
    int pvc_cnt;
    int idx[MAX_PVC];
    int vlanid[MAX_PVC];
    int vpi[MAX_PVC];
    int vci[MAX_PVC];
    int enc[MAX_PVC];
    int mode[MAX_PVC];
    int qostype[MAX_PVC];
    int pcr[MAX_PVC];
    int scr[MAX_PVC];
    int mbs[MAX_PVC];
    char outpkts[MAX_PVC][16];
    char inpkts[MAX_PVC][16];
    char incrcerr[MAX_PVC][16];
    char linebuf[120];
    char* ErrMsgBuf;
    int ErrMsgLen;

    pvc_cnt = GetAllActivePvc(idx, vlanid, vpi, vci, enc, mode, qostype, pcr, scr, mbs);
    //printf("disppvc:%d\n",pvc_cnt);
    for (i=0; i<pvc_cnt; i++)
    {
        GetPvcStats(outpkts[i], inpkts[i], incrcerr[i], vpi[i], vci[i]);
    }

#ifdef WIN32
    fp = fopen("\\adsl_stats.txt","wb");
#else
    fp = fopen("/tmp/adsl/adslate.log","wa");
#endif
    if (fp == NULL) return;


    for (i=0; i<pvc_cnt; i++)
    {
        sprintf(linebuf,"idx:%d,vlanid:%d,vpi:%d,vci:%d,enc:%d,mode:%d\n",
            idx[i],vlanid[i],vpi[i],vci[i],enc[i],mode[i]);
        fputs(linebuf, fp);
        sprintf(linebuf,"idx:%d,qostype:%d,pcr:%d,scr:%d,mbs:%d\n",
            idx[i],qostype[i],pcr[i],scr[i],mbs[i]);
        fputs(linebuf, fp);
        fputs("outpkts:", fp);
        fputs(outpkts[i], fp);
        fputs(",inpkts:", fp);
        fputs(inpkts[i], fp);
        fputs(",incrcerr:", fp);
        fputs(incrcerr[i], fp);
        fputs("\n", fp);
    }
    ErrMsgBuf = GetErrMsg(&ErrMsgLen);
    if (ErrMsgLen > 0)
    {
        fputs(ErrMsgBuf, fp);
    }
    fclose(fp);
}

int DelPvc(int vpi, int vci)
{
    declare_resp_handling_vars(pRespBuf, pRespLen, RespPktNum, tok, pRespStr);
    char InputCmd[64];
    char UserCmd[256];
		InputCmd[0] = '\0';
		UserCmd[0] = '\0';
    sprintf(InputCmd, "%d %d", vpi, vci);
    strcpy(UserCmd, "sys tpset wan atm pvc del ");
    strcat(UserCmd, InputCmd);
    strcat(UserCmd, "\x0d\x0a");
    SendCmdAndWaitResp(pRespBuf, MAX_RESP_BUF_SIZE, pRespLen, MAX_RESP_BUF_NUM, &RespPktNum, m_AdslMacAddr,
        MAC_RTS_CONSOLE_CMD, (PUCHAR)UserCmd, strlen(UserCmd), 0);
    int i;
    int ret = -1;
    for(i=0; i<RespPktNum; i++)
    {
        scanner_set(GET_RESP_STR(pRespBuf[i]), GET_RESP_LEN(*pRespLen[i]));
        while (ret == -1)
        {
            tok = scanner();
            if (tok == TOKEN_EOF)
            {
                break;
            }
            if (tok == TOKEN_STRING)
            {
                pRespStr = scanner_get_str();
                if (strcmp(pRespStr,"OK") == 0)
                {
                    ret = 0;
                    break;
                }
                else
                {
                    PutErrMsg("DelPvc");
                }
            }
        }
    }
    return ret;
}


int DelAllPvc()
{
#ifndef RTCONFIG_DSL_TCLINUX
    int i;
    int ret = 0;
    int ret_del_pvc;
    int pvc_cnt;
    int idx[MAX_PVC];
    int vlanid[MAX_PVC];
    int vpi[MAX_PVC];
    int vci[MAX_PVC];
    int enc[MAX_PVC];
    int mode[MAX_PVC];
    int qostype[MAX_PVC];
    int pcr[MAX_PVC];
    int scr[MAX_PVC];
    int mbs[MAX_PVC];

    pvc_cnt = GetAllActivePvc(idx, vlanid, vpi, vci, enc, mode, qostype, pcr, scr, mbs);

    for (i=0; i<pvc_cnt; i++)
    {
        ret_del_pvc = DelPvc(vpi[i],vci[i]);
        if (ret_del_pvc != 0)
        {
            ret = -1;
            break;
        }
    }

    return ret;
#else
	char tccmd[MAX_RESP_BUF_SIZE] = {0};
	int i;
	PUCHAR pRespBuf[MAX_RESP_BUF_NUM];
	PUSHORT pRespLen[MAX_RESP_BUF_NUM];
	USHORT RespPktNum;

	init_resp_buf (pRespBuf, pRespLen);

	for(i=0; i<MAX_PVC; i++)
		sprintf(tccmd + strlen(tccmd), "tcapi set Wan_PVC%d Active No; tcapi commit Wan_PVC%d; ", i, i);
	strcat(tccmd, "\x0d\x0a");

	SendCmdAndWaitResp(pRespBuf, MAX_RESP_BUF_SIZE, pRespLen, MAX_RESP_BUF_NUM, &RespPktNum, m_AdslMacAddr,
		MAC_RTS_CONSOLE_CMD, (PUCHAR)tccmd, GET_LEN(tccmd), 1);

#ifdef RTCONFIG_VDSL
	memset(tccmd, 0, sizeof(tccmd));
	for(i=8; i<10; i++)
		sprintf(tccmd + strlen(tccmd), "tcapi set Wan_PVC%d Active No; tcapi commit Wan_PVC%d; ", i, i);
	strcat(tccmd, "\x0d\x0a");

	SendCmdAndWaitResp(pRespBuf, MAX_RESP_BUF_SIZE, pRespLen, MAX_RESP_BUF_NUM, &RespPktNum, m_AdslMacAddr,
		MAC_RTS_CONSOLE_CMD, (PUCHAR)tccmd, GET_LEN(tccmd), 1);
#endif

	return 0;
#endif
}

/* Paul add 2012/8/7 */
int RestoreDefault()
{
#ifndef RTCONFIG_DSL_TCLINUX
    declare_resp_handling_vars(pRespBuf, pRespLen, RespPktNum, tok, pRespStr);
    char UserCmd[64];
		UserCmd[0] = '\0';
    strcpy(UserCmd, "sys default");
    strcat(UserCmd, "\x0d\x0a");
    SendCmdAndWaitResp(pRespBuf, MAX_RESP_BUF_SIZE, pRespLen, MAX_RESP_BUF_NUM, &RespPktNum, m_AdslMacAddr,
        MAC_RTS_CONSOLE_CMD, (PUCHAR)UserCmd, strlen(UserCmd), 0);
    int i;
    int ret = -1;
    for(i=0; i<RespPktNum; i++)
    {
        scanner_set(GET_RESP_STR(pRespBuf[i]), GET_RESP_LEN(*pRespLen[i]));
        while (ret == -1)
        {
            tok = scanner();
            if (tok == TOKEN_EOF)
            {
                break;
            }
            if (tok == TOKEN_STRING)
            {
                pRespStr = scanner_get_str();
                if (strcmp(pRespStr,"OK") == 0)
                {
                    ret = 0;
                    break;
                }
                else
                {
                    PutErrMsg("sysdefault");
                }
            }
        }
    }
    return ret;
#else
	char tccmd[MAX_RESP_BUF_SIZE] = {0};
	PUCHAR pRespBuf[MAX_RESP_BUF_NUM];
	PUSHORT pRespLen[MAX_RESP_BUF_NUM];
	USHORT RespPktNum;

	init_resp_buf (pRespBuf, pRespLen);

	strcpy(tccmd, "tcapi set System_Entry reboot_type 2; tcapi commit System_Entry; \x0d\x0a");

	SendCmdAndWaitResp(pRespBuf, MAX_RESP_BUF_SIZE, pRespLen, MAX_RESP_BUF_NUM, &RespPktNum, m_AdslMacAddr,
		MAC_RTS_CONSOLE_CMD, (PUCHAR)tccmd, GET_LEN(tccmd), 1);

	return 0;
#endif
}

//Ren.B
int getBitsPerCarrier()
{
#ifndef RTCONFIG_DSL_TCLINUX
	char UserCmd[256] = {0};
	FILE *bpc_fp = NULL;
	int i;

	disable_polling();
	wait_polling_stop();

	strcpy(UserCmd, "wan adsl linedata far");
	strcat(UserCmd, END_SYMBOL);
	declare_resp_handling_vars(pRespBuf, pRespLen, RespPktNum, tok, pRespStr);
	SendCmdAndWaitResp(pRespBuf, MAX_RESP_BUF_SIZE, pRespLen, MAX_RESP_BUF_NUM, &RespPktNum, m_AdslMacAddr,
	MAC_RTS_CONSOLE_CMD, (PUCHAR)UserCmd, strlen(UserCmd), 0);

	unlink(FILE_NAME_GET_BPC);
	bpc_fp = fopen( FILE_NAME_GET_BPC, "w" );
	if( !bpc_fp )
	{
		fprintf(stderr, "Cannot open [%s]\n", FILE_NAME_GET_BPC );
		enable_polling();
		return -1;
	}
	
	for(i=0; i<RespPktNum; i++)
	{
		scanner_set(GET_RESP_STR(pRespBuf[i]), GET_RESP_LEN(*pRespLen[i]));
		if( strncmp( GET_RESP_STR(pRespBuf[i]), "tc>", 3 ) )
		{
			fputs( GET_RESP_STR(pRespBuf[i]), bpc_fp );
		}
	}

	fclose(bpc_fp);

	sprintf(UserCmd, "sed '1,6d' %s > %s", FILE_NAME_GET_BPC, TMP_FILE_NAME_GET_BPC ); //Ren
	system(UserCmd);
	sprintf(UserCmd, "sed -i 's/^..............//' %s", TMP_FILE_NAME_GET_BPC ); //"tone   0- 31: 00 12 34 56" => "00 12 34 56" (cut initial 14 bytes)
	system(UserCmd);
	sprintf(UserCmd, "sed -i 's/ //g' %s", TMP_FILE_NAME_GET_BPC ); //"00 12 34 56" => "00123456"
	system(UserCmd);
	sprintf(UserCmd, "sed -i ':a;N;$!ba;s/\\n//g' %s", TMP_FILE_NAME_GET_BPC ); //delete 'newline'.
	system(UserCmd);
	sprintf(UserCmd, "sed -i 's/./&\\n/g' %s", TMP_FILE_NAME_GET_BPC ); //"00123456" becomes a single column.
	system(UserCmd);
	sprintf(UserCmd, "sed -i '$d' %s", TMP_FILE_NAME_GET_BPC ); //delete last row (empty row).
	system(UserCmd);
	sprintf(UserCmd, "wc -l %s > %s", TMP_FILE_NAME_GET_BPC, "/tmp/adsl/bpclinecount" );
	system(UserCmd);
	
	enable_polling();
	return 0;
#else
	int ret = 0;
	int mode = getModulation();

	disable_polling();
	wait_polling_stop();

	switch(mode)
	{
		case DSL_MODE_ADSL_ANSI:
		case DSL_MODE_ADSL_OPEN_GDMT:
		case DSL_MODE_ADSL_OPEN_GLITE:
		case DSL_MODE_ADSL2:
		case DSL_MODE_ADSL2PLUS:
			if(tftp_get_file(REMOTE_FILE_ADSL_SNR, LOCAL_FILE_DSL_SNR)) ret++;
			break;
		case DSL_MODE_VDSL2:
			if(tftp_get_file(REMOTE_FILE_VDSL_SNR, LOCAL_FILE_DSL_SNR)) ret++;
			break;
		default:
			myprintf("Modem is not up!!\n");
			break;
	}

	enable_polling();

	return ret;
#endif
}

//obsoletes getSNR_ADSL1 and getSNR_ADSL2_PLUS
int getSNR()
{
#ifndef RTCONFIG_DSL_TCLINUX
	char UserCmd[256] = {0};
	FILE *snr_fp = NULL;
	int i;

	disable_polling();
	wait_polling_stop();

	strcpy(UserCmd, "wan adsl snr");
	strcat(UserCmd, END_SYMBOL);
	declare_resp_handling_vars(pRespBuf, pRespLen, RespPktNum, tok, pRespStr);
	SendCmdAndWaitResp(pRespBuf, MAX_RESP_BUF_SIZE, pRespLen, MAX_RESP_BUF_NUM, &RespPktNum, m_AdslMacAddr,
	MAC_RTS_CONSOLE_CMD, (PUCHAR)UserCmd, strlen(UserCmd), 0);

	unlink(FILE_NAME_GET_SNR);
	snr_fp = fopen( FILE_NAME_GET_SNR, "w" );
	if( !snr_fp )
	{
		fprintf(stderr, "Cannot open [%s]\n", FILE_NAME_GET_SNR );
		enable_polling();
		return -1;
	}

	for(i=0; i<RespPktNum; i++)
	{
		scanner_set(GET_RESP_STR(pRespBuf[i]), GET_RESP_LEN(*pRespLen[i]));
		if( strncmp( GET_RESP_STR(pRespBuf[i]), "tc>", 3 ) )
		{
			fputs( GET_RESP_STR(pRespBuf[i]), snr_fp );
		}
	}

	fclose(snr_fp);

	sprintf(UserCmd, "sed -i '1d;2d' %s", FILE_NAME_GET_SNR ); //delete the first two rows.
	system(UserCmd);
	sprintf(UserCmd, "cat %s | awk '{print $1 \"\\t\" $2 \"\\t\" $3 \"\\t\" $4 \"\\t\" $5 \"\\t\" $6 \"\\t\" $7 \"\\t\" $8}' > %s", FILE_NAME_GET_SNR, TMP_FILE_NAME_GET_SNR ); //get "0.00    0.00    40.98   43.49   43.49   43.99   44.49   49.62"
	system(UserCmd);
	sprintf(UserCmd, "sed -i 's/\\t/\\n/g' %s", TMP_FILE_NAME_GET_SNR ); //translate every row to a single column.
	system(UserCmd);
	sprintf(UserCmd, "wc -l %s > %s", TMP_FILE_NAME_GET_SNR, "/tmp/adsl/snrlinecount" );
	system(UserCmd);

	enable_polling();
	
	return 0;
#else
	int ret = 0;
	int mode = getModulation();

	disable_polling();
	wait_polling_stop();

	switch(mode)
	{
		case DSL_MODE_ADSL_ANSI:
		case DSL_MODE_ADSL_OPEN_GDMT:
		case DSL_MODE_ADSL_OPEN_GLITE:
		case DSL_MODE_ADSL2:
		case DSL_MODE_ADSL2PLUS:
			if(tftp_get_file(REMOTE_FILE_ADSL_BPC_US, LOCAL_FILE_DSL_BPC_US)) ret++;
			if(tftp_get_file(REMOTE_FILE_ADSL_BPC_DS, LOCAL_FILE_DSL_BPC_DS)) ret++;
			break;
		case DSL_MODE_VDSL2:
			if(tftp_get_file(REMOTE_FILE_VDSL_BPC_US, LOCAL_FILE_DSL_BPC_US)) ret++;
			if(tftp_get_file(REMOTE_FILE_VDSL_BPC_DS, LOCAL_FILE_DSL_BPC_DS)) ret++;
			break;
		default:
			myprintf("Modem is not up!!\n");
			break;
	}

	enable_polling();

	return ret;
#endif
}

int getSNR_ADSL1()
{
	char UserCmd[256] = {0};
	FILE *snr1_fp = NULL;
	int i;

	disable_polling();
	wait_polling_stop();

	strcpy(UserCmd, "wan dmt show snr");
	strcat(UserCmd, END_SYMBOL);
	declare_resp_handling_vars(pRespBuf, pRespLen, RespPktNum, tok, pRespStr);
	SendCmdAndWaitResp(pRespBuf, MAX_RESP_BUF_SIZE, pRespLen, MAX_RESP_BUF_NUM, &RespPktNum, m_AdslMacAddr,
	MAC_RTS_CONSOLE_CMD, (PUCHAR)UserCmd, strlen(UserCmd), 0);

	unlink(FILE_NAME_GET_ADSL1);
	snr1_fp = fopen( FILE_NAME_GET_ADSL1, "w" );
	if( !snr1_fp )
	{
		fprintf(stderr, "Cannot open [%s]\n", FILE_NAME_GET_ADSL1 );
		enable_polling();
		return -1;
	}

	for(i=0; i<RespPktNum; i++)
	{
		scanner_set(GET_RESP_STR(pRespBuf[i]), GET_RESP_LEN(*pRespLen[i]));
		if( strncmp( GET_RESP_STR(pRespBuf[i]), "tc>", 3 ) )
		{
			fputs( GET_RESP_STR(pRespBuf[i]), snr1_fp );
		}
	}

	fclose(snr1_fp);

	sprintf(UserCmd, "cat %s | awk 'NR>=2{print $1}' > %s", FILE_NAME_GET_ADSL1, TMP_FILE_NAME_GET_ADSL1 );
	system(UserCmd);
	sprintf(UserCmd, "sed -i '1d;$d' %s", TMP_FILE_NAME_GET_ADSL1 ); //delete the first and the last row.
	system(UserCmd);

	enable_polling();

	return 0;
}

int getSNR_ADSL2_PLUS()
{
	char UserCmd[256] = {0};
	FILE *snr2_fp = NULL;
	int i;

	disable_polling();
	wait_polling_stop();

	strcpy(UserCmd, "wan dmt2 show snr");
	strcat(UserCmd, END_SYMBOL);
	declare_resp_handling_vars(pRespBuf, pRespLen, RespPktNum, tok, pRespStr);
	SendCmdAndWaitResp2(pRespBuf, MAX_RESP_BUF_SIZE, pRespLen, MAX_RESP_BUF_NUM, &RespPktNum, m_AdslMacAddr,
	MAC_RTS_CONSOLE_CMD, (PUCHAR)UserCmd, strlen(UserCmd), 0);

	unlink(FILE_NAME_GET_ADSL2_PLUS);
	snr2_fp = fopen( FILE_NAME_GET_ADSL2_PLUS, "w" );
	if( !snr2_fp )
	{
		fprintf(stderr, "Cannot open [%s]\n", FILE_NAME_GET_ADSL2_PLUS );
		enable_polling();
		return -1;
	}

	for(i=0; i<RespPktNum; i++)
	{
		scanner_set(GET_RESP_STR(pRespBuf[i]), GET_RESP_LEN(*pRespLen[i]));
		//if( strncmp( GET_RESP_STR(pRespBuf[i]), "tc>", 3 ) ) //Ren: because the output of this command is different from others.
		{
			fputs( GET_RESP_STR(pRespBuf[i]), snr2_fp );
		}
	}

	fclose(snr2_fp);

	sprintf(UserCmd, "sed -i '1d;2d;3d' %s", FILE_NAME_GET_ADSL2_PLUS ); //delete the first three rows.
	system(UserCmd);
	sprintf(UserCmd, "cat %s | awk '{print $1 \"\\t\" $2 \"\\t\" $3 \"\\t\" $4 \"\\t\" $5 \"\\t\" $6 \"\\t\" $7 \"\\t\" $8}' > %s", FILE_NAME_GET_ADSL2_PLUS, TMP_FILE_NAME_GET_ADSL2_PLUS ); //get "0.00    0.00    40.98   43.49   43.49   43.99   44.49   49.62"
	system(UserCmd);
	sprintf(UserCmd, "sed -i 's/\\t/\\n/g' %s", TMP_FILE_NAME_GET_ADSL2_PLUS ); //translate every row to a single column.
	system(UserCmd);

	enable_polling();

	return 0;
}
//Ren.E

int SetQosToPvc(int idx, int SvcCat, int Pcr, int Scr, int Mbs)
{
#ifndef RTCONFIG_DSL_TCLINUX
//
// Trend-chip document says that we require to send a another command again to query the result
// The system have no error recovery so we do not send another command to confirm the API result
//
    declare_resp_handling_vars(pRespBuf, pRespLen, RespPktNum, tok, pRespStr);
    char InputCmd[64];
    char UserCmd[256];
		InputCmd[0] = '\0';
		UserCmd[0] = '\0';
    sprintf(InputCmd, " %d %d %d %d", Pcr, Scr, Mbs, idx+1);
    strcpy(UserCmd, "sys tpset wan atm setqos ");
    if (QOS_UBR==SvcCat || QOS_UBR_NO_PCR==SvcCat) strcat(UserCmd, "ubr"); /* Paul modify 2012/8/6 */
    else if (QOS_CBR==SvcCat) strcat(UserCmd, "cbr");
    else if (QOS_VBR==SvcCat) strcat(UserCmd, "vbr");
    else if (QOS_GFR==SvcCat) strcat(UserCmd, "gfr");/* Paul modify 2012/8/6 */
    else if (QOS_NRT_VBR==SvcCat) strcat(UserCmd, "nrt_vbr");
    strcat(UserCmd,InputCmd);
    strcat(UserCmd, "\x0d\x0a");
    SendCmdAndWaitResp(pRespBuf, MAX_RESP_BUF_SIZE, pRespLen, MAX_RESP_BUF_NUM, &RespPktNum, m_AdslMacAddr,
        MAC_RTS_CONSOLE_CMD, (PUCHAR)UserCmd, strlen(UserCmd), 0);
    int i;
    int ret = -1;
    for(i=0; i<RespPktNum; i++)
    {
        scanner_set(GET_RESP_STR(pRespBuf[i]), GET_RESP_LEN(*pRespLen[i]));
        while (ret == -1)
        {
            tok = scanner();
            if (tok == TOKEN_EOF)
            {
                break;
            }
            if (tok == TOKEN_STRING)
            {
                pRespStr = scanner_get_str();
                if (strcmp(pRespStr,"OK") == 0)
                {
                    ret = 0;
                    break;
                }
            }
        }
    }
    return ret;
#else
	char tccmd[MAX_RESP_BUF_SIZE] = {0};
	PUCHAR pRespBuf[MAX_RESP_BUF_NUM];
	PUSHORT pRespLen[MAX_RESP_BUF_NUM];
	USHORT RespPktNum;
	init_resp_buf (pRespBuf, pRespLen);

	switch(SvcCat) {
		case QOS_UBR_NO_PCR:
			sprintf(tccmd + strlen(tccmd), "tcapi set Wan_PVC%d QOS ubr; ", idx);
			sprintf(tccmd + strlen(tccmd), "tcapi set Wan_PVC%d PCR 0; ", idx);
			break;
		case QOS_UBR:
			sprintf(tccmd + strlen(tccmd), "tcapi set Wan_PVC%d QOS ubr; ", idx);
			sprintf(tccmd + strlen(tccmd), "tcapi set Wan_PVC%d PCR %d; ", idx, Pcr);
			break;
		case QOS_CBR:
			sprintf(tccmd + strlen(tccmd), "tcapi set Wan_PVC%d QOS cbr; ", idx);
			sprintf(tccmd + strlen(tccmd), "tcapi set Wan_PVC%d PCR %d; ", idx, Pcr);
			break;
		case QOS_VBR:
			sprintf(tccmd + strlen(tccmd), "tcapi set Wan_PVC%d QOS rt-vbr; ", idx);
			sprintf(tccmd + strlen(tccmd), "tcapi set Wan_PVC%d PCR %d; ", idx, Pcr);
			sprintf(tccmd + strlen(tccmd), "tcapi set Wan_PVC%d SCR %d; ", idx, Scr);
			sprintf(tccmd + strlen(tccmd), "tcapi set Wan_PVC%d MBS %d; ", idx, Mbs);
			break;
		//case QOS_GFR:
		case QOS_NRT_VBR:
			sprintf(tccmd + strlen(tccmd), "tcapi set Wan_PVC%d QOS nrt-vbr; ", idx);
			sprintf(tccmd + strlen(tccmd), "tcapi set Wan_PVC%d PCR %d; ", idx, Pcr);
			sprintf(tccmd + strlen(tccmd), "tcapi set Wan_PVC%d SCR %d; ", idx, Scr);
			sprintf(tccmd + strlen(tccmd), "tcapi set Wan_PVC%d MBS %d; ", idx, Mbs);
			break;
	}
	strcat(tccmd, "\x0d\x0a");
	SendCmdAndWaitResp(pRespBuf, MAX_RESP_BUF_SIZE, pRespLen, MAX_RESP_BUF_NUM, &RespPktNum, m_AdslMacAddr,
		MAC_RTS_CONSOLE_CMD, (PUCHAR)tccmd, GET_LEN(tccmd), 1);
	return 0;
#endif
}



int AddPvc(int idx, int vlan_id, int vpi, int vci, int encap, int mode)
{
#ifndef RTCONFIG_DSL_TCLINUX
    declare_resp_handling_vars(pRespBuf, pRespLen, RespPktNum, tok, pRespStr);
    char InputCmd[64];
    char UserCmd[256];
		InputCmd[0] = '\0';
		UserCmd[0] = '\0';
    sprintf(InputCmd, "%d %d %d %d %d %d", idx, vlan_id, vpi, vci, encap, mode);
    strcpy(UserCmd, "sys tpset wan atm pvc add ");
    strcat(UserCmd, InputCmd);
    strcat(UserCmd, "\x0d\x0a");
    SendCmdAndWaitResp(pRespBuf, MAX_RESP_BUF_SIZE, pRespLen, MAX_RESP_BUF_NUM, &RespPktNum, m_AdslMacAddr,
        MAC_RTS_CONSOLE_CMD, (PUCHAR)UserCmd, strlen(UserCmd), 0);
    int i;
    int ret = -1;
    for(i=0; i<RespPktNum; i++)
    {
        scanner_set(GET_RESP_STR(pRespBuf[i]), GET_RESP_LEN(*pRespLen[i]));
        while (ret == -1)
        {
            tok = scanner();
            if (tok == TOKEN_EOF)
            {
                break;
            }
            if (tok == TOKEN_STRING)
            {
                pRespStr = scanner_get_str();
                if (strcmp(pRespStr,"OK") == 0)
                {
                    ret = 0;
                    break;
                }
                else
                {
                    PutErrMsg("AddPvc Error");
                }
            }
        }
    }
    return ret;
#else
	char tccmd[MAX_RESP_BUF_SIZE] = {0};
	PUCHAR pRespBuf[MAX_RESP_BUF_NUM];
	PUSHORT pRespLen[MAX_RESP_BUF_NUM];
	USHORT RespPktNum;
	init_resp_buf (pRespBuf, pRespLen);

	sprintf(tccmd + strlen(tccmd), "tcapi set Wan_PVC%d Active Yes; ", idx);
	sprintf(tccmd + strlen(tccmd), "tcapi set Wan_PVC%d VPI %d; ", idx, vpi);
	sprintf(tccmd + strlen(tccmd), "tcapi set Wan_PVC%d VCI %d; ", idx, vci);

	switch(mode) {
		case 1://pppoa
			sprintf(tccmd + strlen(tccmd), "tcapi set Wan_PVC%d ISP 2; ", idx);
			if(encap)
				sprintf(tccmd + strlen(tccmd), "tcapi set Wan_PVC%d ENCAP \"PPPoA VC-Mux\"; ", idx);
			else
				sprintf(tccmd + strlen(tccmd), "tcapi set Wan_PVC%d ENCAP \"PPPoA LLC\"; ", idx);
			break;
		case 2://ipoa
			sprintf(tccmd + strlen(tccmd), "tcapi set Wan_PVC%d ISP 1; ", idx);
			if(encap)
				sprintf(tccmd + strlen(tccmd), "tcapi set Wan_PVC%d ENCAP \"1483 Routed IP VC-Mux\"; ", idx);
			else
				sprintf(tccmd + strlen(tccmd), "tcapi set Wan_PVC%d ENCAP \"1483 Routed IP LLC(IPoA)\"; ", idx);
			break;
		default:	//pppoe, mer and bridge will all be bridge mode in MT7510
			sprintf(tccmd + strlen(tccmd), "tcapi set Wan_PVC%d ISP 3; ", idx);
			if(encap)
				sprintf(tccmd + strlen(tccmd), "tcapi set Wan_PVC%d ENCAP \"1483 Bridged Only VC-Mux\"; ", idx);
			else
				sprintf(tccmd + strlen(tccmd), "tcapi set Wan_PVC%d ENCAP \"1483 Bridged Only LLC\"; ", idx);
			break;
	}
	sprintf(tccmd + strlen(tccmd), "tcapi commit Wan_PVC%d; ", idx);
	strcat(tccmd, "\x0d\x0a");
	SendCmdAndWaitResp(pRespBuf, MAX_RESP_BUF_SIZE, pRespLen, MAX_RESP_BUF_NUM, &RespPktNum, m_AdslMacAddr,
		MAC_RTS_CONSOLE_CMD, (PUCHAR)tccmd, GET_LEN(tccmd), 1);
	return 0;
#endif
}

void SettingSysDefault()
{
    declare_resp_handling_vars(pRespBuf, pRespLen, RespPktNum, tok, pRespStr);
    SendCmdAndWaitResp(pRespBuf, MAX_RESP_BUF_SIZE, pRespLen, MAX_RESP_BUF_NUM, &RespPktNum, m_AdslMacAddr,
        MAC_RTS_CONSOLE_CMD, (PUCHAR)SYS_DEFAULT, GET_LEN(SYS_DEFAULT), 1);
}

void GetVer(void)
{
    declare_resp_handling_vars(pRespBuf, pRespLen, RespPktNum, tok, pRespStr);
    FILE* fp;
    FILE* fp2;
    char BufStr[160];


#ifdef WIN32
    fp = fopen("\\tc_ver_info.txt","wb");
#else
    fp = fopen("/tmp/adsl/tc_ver_info.txt","wb");
#endif

    if (fp == NULL) return;

    SendCmdAndWaitResp(pRespBuf, MAX_RESP_BUF_SIZE, pRespLen, MAX_RESP_BUF_NUM, &RespPktNum, m_AdslMacAddr,
        MAC_RTS_CONSOLE_CMD, (PUCHAR)SYSVER, GET_LEN(SYSVER), 0);

    int i;
    for(i=0; i<RespPktNum; i++)
    {
        scanner_set(GET_RESP_STR(pRespBuf[i]), GET_RESP_LEN(*pRespLen[i]));
        while (1)
        {
            tok = scanner();
            if (tok == TOKEN_EOF)
            {
                break;
            }
            if (tok == TOKEN_STRING)
            {
                pRespStr = scanner_get_str();
                if (strcmp(pRespStr,"Bootbase") == 0)
                {
                    tok = scanner();
                    tok_assert(tok,TOKEN_STRING);
                    tok = scanner();
                    tok_assert(tok,TOKEN_COLON);
                    tok = scanner_get_line();
                    fputs("Bootbase:",fp);
                    strcpymax(BufStr, scanner_get_str(), sizeof(BufStr));
                    fputs(BufStr,fp);
                    fputs("\n",fp);
                }
                else if (strcmp(pRespStr,"RAS") == 0)
                {
					int buf_idx;
                    tok = scanner();
                    tok_assert(tok,TOKEN_STRING);
                    tok = scanner();
                    tok_assert(tok,TOKEN_STRING);
                    tok = scanner();
                    tok_assert(tok,TOKEN_COLON);
                    tok = scanner_get_line();
                    fputs("RAS:",fp);
                    strcpymax(BufStr, scanner_get_str(), sizeof(BufStr));
                    fputs(BufStr,fp);
                    fputs("\n",fp);
					// save RAS version to a single file
					for (buf_idx=0; buf_idx<sizeof(BufStr); buf_idx++) {
						if (BufStr[buf_idx]==0x0d || BufStr[buf_idx]==0x0a || BufStr[buf_idx]==0x20) BufStr[buf_idx] = 0;
						if (BufStr[buf_idx] == 0) break;
					}
					fp2 = fopen("/tmp/adsl/tc_ras_ver.txt","wb");
					if (fp2 != NULL)
					{
						fputs(BufStr,fp2);
						fclose(fp2);
					}
                }
                else if (strcmp(pRespStr,"System") == 0)
                {
                    tok = scanner();
                    tok_assert(tok,TOKEN_STRING);
                    tok = scanner();
                    tok_assert(tok,TOKEN_STRING);
                    tok = scanner();
                    tok_assert(tok,TOKEN_COLON);
                    tok = scanner_get_line();
                    fputs("System:",fp);
                    strcpymax(BufStr, scanner_get_str(), sizeof(BufStr));
                    fputs(BufStr,fp);
                    fputs("\n",fp);
                }
                else if (strcmp(pRespStr,"DSL") == 0)
                {
                    tok = scanner();
                    tok_assert(tok,TOKEN_STRING);
                    tok = scanner();
                    tok_assert(tok,TOKEN_STRING);
                    tok = scanner();
                    tok_assert(tok,TOKEN_COLON);
                    tok = scanner_get_line();
                    fputs("DSL:",fp);
                    strcpymax(BufStr, scanner_get_str(), sizeof(BufStr));
                    fputs(BufStr,fp);
                    fputs("\n",fp);
                }
                else if (strcmp(pRespStr,"Standard") == 0)
                {
                    tok = scanner();
                    tok_assert(tok,TOKEN_COLON);
                    tok = scanner_get_line();
                    fputs("Standard:",fp);
                    strcpymax(BufStr, scanner_get_str(), sizeof(BufStr));
                    fputs(BufStr,fp);
                    fputs("\n",fp);
                }
                else if (strcmp(pRespStr,"Country") == 0)
                {
                    tok = scanner();
                    tok_assert(tok,TOKEN_STRING);
                    tok = scanner();
                    tok_assert(tok,TOKEN_COLON);
                    tok = scanner_get_line();
                    fputs("Country:",fp);
                    strcpymax(BufStr, scanner_get_str(), sizeof(BufStr));
                    fputs(BufStr,fp);
                    fputs("\n",fp);
                }
                else if (strcmp(pRespStr,"MAC") == 0)
                {
                    tok = scanner();
                    tok_assert(tok,TOKEN_STRING);
                    tok = scanner();
                    tok_assert(tok,TOKEN_COLON);
                    tok = scanner_get_line();
                    fputs("MAC:",fp);
                    strcpymax(BufStr, scanner_get_str(), sizeof(BufStr));
                    fputs(BufStr,fp);
                    fputs("\n",fp);
                }
            }
        }
    }

    fclose(fp);
}

void InitTc(void)
{
#ifndef RTCONFIG_DSL_TCLINUX
    declare_resp_handling_vars(pRespBuf, pRespLen, RespPktNum, tok, pRespStr);
    // this command should wait TC firmware boot completed
    int Retry;
    for (Retry = 0; Retry < 10; Retry++)
    {
        SendCmdAndWaitResp(pRespBuf, MAX_RESP_BUF_SIZE, pRespLen, MAX_RESP_BUF_NUM, &RespPktNum, BROADCAST_ADDR,
            MAC_RTS_START, NULL, 0, CON_START_RESP_NUM);
        if (RespPktNum > 0) break;
    }
    if (RespPktNum > 0)
    {
        memcpy(m_AdslMacAddr,&pRespBuf[0][6],6);
    }
    else
    {
        FILE* fpBoot;
        g_AlwaysIgnoreAdslCmd = TRUE;
        fpBoot = fopen("/tmp/adsl/ADSL_FATAL_BOOT_ERROR.txt","wb");
        if (fpBoot != NULL)
        {
            fputs("init TC failed",fpBoot);
            fputs("\n",fpBoot);
            fclose(fpBoot);
        }
        return;
    }
    myprintf("ADSL MAC IS %02x-%02x-%02x-%02x-%02x-%02x\n",m_AdslMacAddr[0],m_AdslMacAddr[1],m_AdslMacAddr[2],
        m_AdslMacAddr[3],m_AdslMacAddr[4],m_AdslMacAddr[5]);

    SendCmdAndWaitResp(pRespBuf, MAX_RESP_BUF_SIZE, pRespLen, MAX_RESP_BUF_NUM, &RespPktNum, m_AdslMacAddr,
        MAC_RTS_CONSOLE_ON, NULL, 0, CON_ON_RESP_NUM);

    for (Retry = 0; Retry < 10; Retry++)
    {
		SendCmdAndWaitResp(pRespBuf, MAX_RESP_BUF_SIZE, pRespLen, MAX_RESP_BUF_NUM, &RespPktNum, m_AdslMacAddr,
			MAC_RTS_CONSOLE_CMD, CMD_CRLF, CMD_CRLF_LEN, CON_CRLF_RESP_NUM);

		if (RespPktNum > 1)
		{
			scanner_set(GET_RESP_STR(pRespBuf[1]), GET_RESP_LEN(*pRespLen[1]));
			while (1)
			{
				tok = scanner();
				if (tok == TOKEN_EOF)
				{
					break;
				}
				if (tok == TOKEN_STRING)
				{
					pRespStr = scanner_get_str();
					if (strcmp(pRespStr,"Password") == 0)
					{
						SendCmdAndWaitResp(pRespBuf, MAX_RESP_BUF_SIZE, pRespLen, MAX_RESP_BUF_NUM, &RespPktNum, m_AdslMacAddr,
							MAC_RTS_CONSOLE_CMD, (PUCHAR)PSWD, GET_LEN(PSWD), CON_PSWD_RESP_NUM);
					}
				}
			}
		}
	}
#else
	PUCHAR pRespBuf[MAX_RESP_BUF_NUM];
	PUSHORT pRespLen[MAX_RESP_BUF_NUM];
	USHORT RespPktNum;

GetMac:
	init_resp_buf (pRespBuf, pRespLen);

	SendCmdAndWaitResp(pRespBuf, MAX_RESP_BUF_SIZE, pRespLen, MAX_RESP_BUF_NUM, &RespPktNum, BROADCAST_ADDR,
			MAC_RTS_CONSOLE_CMD, CMD_CRLF, CMD_CRLF_LEN, 0);
	if (RespPktNum > 0) {
		memcpy(m_AdslMacAddr,&pRespBuf[0][6],6);
		myprintf("ADSL MAC IS %02x-%02x-%02x-%02x-%02x-%02x\n",m_AdslMacAddr[0],m_AdslMacAddr[1],m_AdslMacAddr[2],
					m_AdslMacAddr[3],m_AdslMacAddr[4],m_AdslMacAddr[5]);
	}
	else {
		myprintf("Get Mac address failed\n");
		if(check_if_file_exist("/jffs/tclinux.bin")) {
			char cmd[80] = {0};
			snprintf(cmd, sizeof(cmd), "cd /jffs; tftp -p -l tclinux.bin %s", DEFAULT_IPADDR);
			myprintf("Try recover DSL firmware\n");
			if(!system(cmd)) {
				myprintf("Upload DSL firmware successfully\n");
				sleep(90);
			}
			goto GetMac;
		}
	}
#endif
}

int check_DSL_spectrum_driver(char *ver)
{
	char buffer[32] = {0};
	char verstr[32] = {0};
	char *ptr = verstr;
	int i;
	unsigned long base = 316181; //version with spectrum support = 3.16.18.1
	unsigned long result = 0;

	snprintf(buffer, sizeof(buffer), "%s", ver);

	i = 0;
	while(buffer[i])
	{
		if(isdigit(buffer[i]))
		{
			*ptr = buffer[i];
			ptr++;
		}
		i++;
	}

	sscanf(verstr, "%lu", &result);

	if(result >= base)
	{
		//this version supports spectrum
		add_rc_support("spectrum");
		nvram_set("spectrum_supported", "1");
		return 1;
	}
	else
	{
		//this version does not support spectrum
		nvram_set("spectrum_supported", "0");
		return 0;
	}
}

void WriteAdslFwInfoToFile()
{
    char buf[32];
    memset(&m_IpAddr, 0, sizeof(m_IpAddr));
    HandleAdslShowLan(&m_IpAddr);
    memset(&m_SysInfo, 0, sizeof(m_SysInfo));
    HandleAdslSysInfo(&m_SysInfo);

    // store IP address to /tmp/adsl/tc_ip_addr.txt
    // for adsl firmware upgrade
    FILE* fp;
#ifdef WIN32
    fp = fopen("\\tc_ip_addr.txt","wb");
#else
    fp = fopen("/tmp/adsl/tc_ip_addr.txt","wb");
#endif
    if (fp == NULL) return;

    fputs(m_IpAddr.IpAddr,fp);
    //fputs("\n",fp);
    fclose(fp);

    // store version to /tmp/adsl/tc_fw_ver.txt
    // for adsl status web page

#ifdef WIN32
    fp = fopen("\\tc_fw_ver.txt","wb");
#else
    fp = fopen("/tmp/adsl/tc_fw_ver.txt","wb");
#endif
    if (fp == NULL) return;

    fputs(m_SysInfo.AdslFwVer,fp);
    //fputs("\n",fp);
    fclose(fp);

#ifdef WIN32
    fp = fopen("\\tc_fw_ver_short.txt","wb");
#else
    fp = fopen("/tmp/adsl/tc_fw_ver_short.txt","wb");
#endif
    if (fp == NULL) return;

#ifndef DSL_TCLINUX
    nvram_set("spectrum_supported", "0");//Ren
    nvram_set("adsldriververshort", m_SysInfo.AdslFwVerDisp);//Ren
    check_DSL_spectrum_driver(m_SysInfo.AdslFwVerDisp);//Ren
#endif

    fputs(m_SysInfo.AdslFwVerDisp,fp);
    //fputs("\n",fp);
    fclose(fp);


#ifdef WIN32
    fp = fopen("\\tc_mac.txt","wb");
#else
    fp = fopen("/tmp/adsl/tc_mac.txt","wb");
#endif
    sprintf(buf, "%02x:%02x:%02x:%02x:%02x:%02x",m_AdslMacAddr[0],m_AdslMacAddr[1],m_AdslMacAddr[2],
        m_AdslMacAddr[3],m_AdslMacAddr[4],m_AdslMacAddr[5]);

    if (fp == NULL) return;
    fputs(buf,fp);
    fputs("\n",fp);
    fclose(fp);
}


void mysigterm()
{
	//printf("I caught the SIGTERM signal!\n");
	m_Exit = 1;
	return;
}


int main(int argc, char* argv[])
{

#ifdef WIN32
	m_ShowPktLog  = TRUE;
	m_EnableDbgOutput = TRUE;
#else
	int dbg_flag = nvram_get_dbg_flag();
	if (dbg_flag == 1)
	{
		m_EnableDbgOutput = TRUE;
		m_ShowPktLog  = FALSE;
		m_DbgOutputRedirectToFile = FALSE;
	}
	else if (dbg_flag == 2)
	{
		m_EnableDbgOutput = TRUE;
		m_ShowPktLog  = FALSE;
		m_DbgOutputRedirectToFile = TRUE;
	}
	else if (dbg_flag == 3)
	{
		m_EnableDbgOutput = TRUE;
		m_ShowPktLog  = TRUE;
		m_DbgOutputRedirectToFile = FALSE;
	}
#endif

#ifndef WIN32
	sigset_t sigs_to_catch;


	// SOME SIGNALS ARE BLOCKED BY RC
	// TO USE TIMER SIGNAL, IT MUST USE THE FOLLOWING CODE TO ENABLE TIMER
	/* set the signal handler */
	sigemptyset(&sigs_to_catch);
	sigaddset(&sigs_to_catch, SIGTERM);
	sigaddset(&sigs_to_catch, SIGALRM);
	sigprocmask(SIG_UNBLOCK, &sigs_to_catch, NULL);

	if (signal(SIGTERM, mysigterm) == SIG_ERR)
	{
		myprintf("Cannot handle SIGTERM!\n");
		return -1;
	}
#ifdef RTCONFIG_RALINK
	if(switch_init() < 0)
	{
		myprintf("err\n");
		return -1;
	}
#endif

    //printf("\n************** TP_INIT ***************\n");

#if DSL_N55U == 1
	m_AdaptIdx = if_nametoindex("eth2.1");
	m_SocketSend = socket(AF_PACKET,SOCK_RAW, htons(ETH_P_ALL));
#elif defined DSL_AC68U
	m_AdaptIdx = if_nametoindex("vlan2");
	m_SocketSend = socket(AF_PACKET,SOCK_RAW, htons(ETH_P_TCCONSOLE));
#endif
	if (m_SocketSend == -1)
	{
		myprintf("%s %d: %s\n", __func__, __LINE__, strerror(errno));
	}

#ifdef DSL_AC68U
	int inet_sock;
	struct ifreq ifr;
	struct sockaddr_in *device_data;

	inet_sock = socket(AF_INET, SOCK_DGRAM, 0);
	strcpy(ifr.ifr_name, "vlan2");
	while(!m_Exit)
	{
		if(inet_sock < 0);
			inet_sock = socket(AF_INET, SOCK_DGRAM, 0);

		if (ioctl(inet_sock, SIOCGIFADDR, &ifr) != -1)
		{
			device_data=(struct sockaddr_in*)&(ifr.ifr_addr);
			if(!strncmp("169.254", inet_ntoa(device_data->sin_addr), 7))
				break;
		}
		//else
			//perror("ioctl");
		sleep(1);
	}
	close(inet_sock);

	struct timeval tv;
	tv.tv_sec = 10;
	tv.tv_usec = 0;
	if(setsockopt(m_SocketSend, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof(struct timeval)))
		myprintf("%s %d: %s\n", __func__, __LINE__, strerror(errno));

#endif
// not working , SNDBUF could not set
/*
    int nReadSend;
    getsockopt(m_SocketSend,SOL_SOCKET,SO_SNDBUF,&nReadSend,sizeof(int));
    myprintf("nRecvSend:%d\n",nReadSend);

    int nNewSend=2*1024;
    setsockopt(m_SocketSend,SOL_SOCKET,SO_SNDBUF,(const char*)&nNewSend,sizeof(int));
    getsockopt(m_SocketSend,SOL_SOCKET,SO_SNDBUF,&nReadSend,sizeof(int));
    myprintf("nNewSend:%d\n",nReadSend);
*/

#else

    m_PtDrv = NULL;
	m_PtDrv = new ProtDrv;
	if (m_PtDrv == NULL)
	{
	    exit(0);
	}
	BOOLEAN RetVal;
	RetVal = m_PtDrv->InitAdapterInfo();
	if (RetVal == FALSE)
	{
		printf("error InitAdapterInfo()...");
		return TRUE;
	}
	int NumCard;
	NumCard = m_PtDrv->GetNumAdapterInfo();
	if (NumCard == 0)
	{
		printf("No card binded to prot drv...");
		return TRUE;
	}
	int AdaptInfoIdx;
	for (AdaptInfoIdx = 0; AdaptInfoIdx < NumCard; AdaptInfoIdx++)
	{
		wprintf(m_PtDrv->GetAdapterConnName(AdaptInfoIdx));
	}
	m_AdaptOpened = FALSE;
	if (NumCard>1)
	{
		wprintf(L"Too much NIC binded to prot drv.");
        wprintf(L"Please unbind prot drv from unused NIC.");
	}
	else
	{
		wprintf(m_PtDrv->GetAdapterConnName(0));
		m_PtDrv->OpenAdapter(0);
		wprintf(L"\r\nOpen adapter......\r\n");
		m_AdaptOpened = TRUE;
    }
        // rcv all pkt
    m_PtDrv->SetRcvMode(0);

#endif

    GetMacAddr();

    if (argc == 2)
    {
    	if (!strcmp(argv[1],"clear_modem_var"))
    	{
	        InitTc();
	        SettingSysDefault();
	        printf("sys default\n");
	        return 0;
        }
    	if (!strcmp(argv[1],"eth_wan_mode_only"))
    	{
			InitTc();
			GetVer();
			WriteAdslFwInfoToFile();
			DelAllPvc();
	        return 0;
        }
    }

    if (argc == 1)
    {
		CreateMsgQ();
		InitTc();
#ifndef RTCONFIG_DSL_TCLINUX
		GetVer();
#else
		memset(&m_AdslEntry, 0, sizeof(m_AdslEntry));
		memset(&m_InfoAdsl, 0, sizeof(m_InfoAdsl));
#endif
		WriteAdslFwInfoToFile();
		SetPollingTimer();
		enable_polling();
		reload_dsl_setting();
		reload_pvc();

#ifndef WIN32

	nvram_adslsts("down");
	nvram_adslsts_detail("down");


	// tell auto detection tp_init is ready
	// this is also for ate/telnet program sync
	// This is for telnetd sync up. While telnetd available, ADSL commands all sent to TC.
	nvram_set("dsltmp_tcbootup","1");

    while(m_Exit == 0)
    {
//        SleepMs(500);
        int ret_rcv_msg;
        ret_rcv_msg = RcvMsgQ();
        if (ret_rcv_msg == -1)
        {
            // error
            //break;	//Sam, 2014/2/6
        }
        else if (ret_rcv_msg == 1)
        {
            // someone ask tp_init quit
            break;
        }
    }

    printf("exit tp_init\n");
    return 0;

#else

    int cnt = 0;
    while(1)
    {
        SleepMs(10*1000);
        UpdateAllAdslSts(cnt++, 0);
    }


#endif

    }
    else
    {
        while (1)
        {
            declare_resp_handling_vars(pRespBuf, pRespLen, RespPktNum, tok, pRespStr);
            char InputChar;
            char InputCmd[100];
            char UserCmd[256];
						InputCmd[0] = '\0';
						UserCmd[0] = '\0';
            //int InputVpi;
            //int InputVci;
            ShowMsgAndInput(&InputChar, InputCmd);

            if (InputChar == '0')
            {
                InitTc();
                // test , modify
//                DelAllPvc();
/*
                m_ShowPktLog = FALSE;
                AddPvc(0,1,0,33,0,0);
                AddPvc(1,2,0,35,0,0);
                AddPvc(2,3,0,36,0,0);
                AddPvc(3,4,0,37,0,0);
                DispAllPvc();
                m_ShowPktLog = TRUE;
*/
				// askey
				// ipoa
                //AddPvc(0,1,0,32,0,2);
				// pppoa
				// 0,38 vc
//                AddPvc(0,1,0,38,1,1);
            }

            if (InputChar == '1')
            {
                SendCmdAndWaitResp(pRespBuf, MAX_RESP_BUF_SIZE, pRespLen, MAX_RESP_BUF_NUM, &RespPktNum, m_AdslMacAddr,
                    MAC_RTS_CONSOLE_CMD, (PUCHAR)SYSINFO, GET_LEN(SYSINFO), 0);
            }

            if (InputChar == '2')
            {
                SendCmdAndWaitResp(pRespBuf, MAX_RESP_BUF_SIZE, pRespLen, MAX_RESP_BUF_NUM, &RespPktNum, m_AdslMacAddr,
                    MAC_RTS_CONSOLE_CMD, (PUCHAR)SYSSTATUS, GET_LEN(SYSSTATUS), 0);
            }

            if (InputChar == '3')
            {
                SendCmdAndWaitResp(pRespBuf, MAX_RESP_BUF_SIZE, pRespLen, MAX_RESP_BUF_NUM, &RespPktNum, m_AdslMacAddr,
                    MAC_RTS_CONSOLE_CMD, (PUCHAR)SYSTRAFFIC, GET_LEN(SYSTRAFFIC), 0);
            }

            if (InputChar == '4')
            {
                SendCmdAndWaitResp(pRespBuf, MAX_RESP_BUF_SIZE, pRespLen, MAX_RESP_BUF_NUM, &RespPktNum, m_AdslMacAddr,
                    MAC_RTS_CONSOLE_CMD, (PUCHAR)SHOW_WAN, GET_LEN(SHOW_WAN),0);
            }

            if (InputChar == '5')
            {
                SendCmdAndWaitResp(pRespBuf, MAX_RESP_BUF_SIZE, pRespLen, MAX_RESP_BUF_NUM, &RespPktNum, m_AdslMacAddr,
                    MAC_RTS_CONSOLE_CMD, (PUCHAR)SHOW_ADSL, GET_LEN(SHOW_ADSL),0);
            }
            if (InputChar == '6')
            {
                SendCmdAndWaitResp(pRespBuf, MAX_RESP_BUF_SIZE, pRespLen, MAX_RESP_BUF_NUM, &RespPktNum, m_AdslMacAddr,
                    MAC_RTS_CONSOLE_CMD, (PUCHAR)SHOW_LAN, GET_LEN(SHOW_LAN),0);
            }
            if (InputChar == '7')
            {
                SendCmdAndWaitResp(pRespBuf, MAX_RESP_BUF_SIZE, pRespLen, MAX_RESP_BUF_NUM, &RespPktNum, m_AdslMacAddr,
                    MAC_RTS_CONSOLE_CMD, (PUCHAR)PVC_DISP, GET_LEN(PVC_DISP), 0);
            }
            if (InputChar == '8')
            {
                SendCmdAndWaitResp(pRespBuf, MAX_RESP_BUF_SIZE, pRespLen, MAX_RESP_BUF_NUM, &RespPktNum, m_AdslMacAddr,
                    MAC_RTS_CONSOLE_CMD, (PUCHAR)GET_ADSL, GET_LEN(GET_ADSL), 0);
            }

            if (InputChar == 'a' || InputChar == 'v')
            {
                strcpy(UserCmd, "sys tpset wan atm pvc add ");
                strcat(UserCmd, InputCmd);
                strcat(UserCmd, "\x0d\x0a");
                SendCmdAndWaitResp(pRespBuf, MAX_RESP_BUF_SIZE, pRespLen, MAX_RESP_BUF_NUM, &RespPktNum, m_AdslMacAddr,
                    MAC_RTS_CONSOLE_CMD, (PUCHAR)UserCmd, strlen(UserCmd), 0);
            }

            if (InputChar == 'c')
            {
                strcpy(UserCmd, InputCmd);
                strcat(UserCmd, "\x0d\x0a");
                SendCmdAndWaitResp(pRespBuf, MAX_RESP_BUF_SIZE, pRespLen, MAX_RESP_BUF_NUM, &RespPktNum, m_AdslMacAddr,
                    MAC_RTS_CONSOLE_CMD, (PUCHAR)UserCmd, strlen(UserCmd), 0);
            }

            /*
            if (InputChar == 's')
            {
                USHORT VlanId = 1234;
                UCHAR SendPktData[] = "\x08\x06\x00\x01\x08\x00\x06\x04\x00\x01\x00\x0a\x79\x60\x16\xb1\x8c\x70\x43\xb0\x00\x00\x00\x00\x00\x00\x8c\x70\x43\xab\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00";
                SendOnePktVlan(BROADCAST_ADDR, VlanId, SendPktData, sizeof(SendPktData)-1);
            }
            */

            if (InputChar == 'i')
            {
                strcpy(UserCmd, "sys tpset wan atm pvc add ");
                strcat(UserCmd, "2 1 0 35 0 0");
                strcat(UserCmd, "\x0d\x0a");
                SendCmdAndWaitResp(pRespBuf, MAX_RESP_BUF_SIZE, pRespLen, MAX_RESP_BUF_NUM, &RespPktNum, m_AdslMacAddr,
                    MAC_RTS_CONSOLE_CMD, (PUCHAR)UserCmd, strlen(UserCmd), 0);
								UserCmd[0] = '\0';
                strcpy(UserCmd, "sys tpset wan atm pvc add ");
                strcat(UserCmd, "3 2 0 36 0 0");
                strcat(UserCmd, "\x0d\x0a");
                SendCmdAndWaitResp(pRespBuf, MAX_RESP_BUF_SIZE, pRespLen, MAX_RESP_BUF_NUM, &RespPktNum, m_AdslMacAddr,
                    MAC_RTS_CONSOLE_CMD, (PUCHAR)UserCmd, strlen(UserCmd), 0);
								UserCmd[0] = '\0';
                strcpy(UserCmd, "sys tpset wan atm pvc add ");
                strcat(UserCmd, "4 3 0 37 0 0");
                strcat(UserCmd, "\x0d\x0a");
                SendCmdAndWaitResp(pRespBuf, MAX_RESP_BUF_SIZE, pRespLen, MAX_RESP_BUF_NUM, &RespPktNum, m_AdslMacAddr,
                    MAC_RTS_CONSOLE_CMD, (PUCHAR)UserCmd, strlen(UserCmd), 0);
            }

            if (InputChar == 's')
            {
                strcpy(UserCmd, "sys tpget wan hwsar statistics ");
                strcat(UserCmd, "0 35");
                strcat(UserCmd, "\x0d\x0a");
                SendCmdAndWaitResp(pRespBuf, MAX_RESP_BUF_SIZE, pRespLen, MAX_RESP_BUF_NUM, &RespPktNum, m_AdslMacAddr,
                    MAC_RTS_CONSOLE_CMD, (PUCHAR)UserCmd, strlen(UserCmd), 0);
								UserCmd[0] = '\0';
                strcpy(UserCmd, "sys tpget wan hwsar statistics ");
                strcat(UserCmd, "0 36");
                strcat(UserCmd, "\x0d\x0a");
                SendCmdAndWaitResp(pRespBuf, MAX_RESP_BUF_SIZE, pRespLen, MAX_RESP_BUF_NUM, &RespPktNum, m_AdslMacAddr,
                    MAC_RTS_CONSOLE_CMD, (PUCHAR)UserCmd, strlen(UserCmd), 0);
								UserCmd[0] = '\0';
                strcpy(UserCmd, "sys tpget wan hwsar statistics ");
                strcat(UserCmd, "0 37");
                strcat(UserCmd, "\x0d\x0a");
                SendCmdAndWaitResp(pRespBuf, MAX_RESP_BUF_SIZE, pRespLen, MAX_RESP_BUF_NUM, &RespPktNum, m_AdslMacAddr,
                    MAC_RTS_CONSOLE_CMD, (PUCHAR)UserCmd, strlen(UserCmd), 0);
            }



            if (InputChar == 'q')
            {
                printf("exit\r\n");
                break;
            }


        }
    }




#ifndef WIN32
#ifdef RTCONFIG_RALINK
    switch_fini();
#endif
    if (m_SocketSend) close(m_SocketSend);

#else
    _getch();
#endif
	return 0;
}

#ifdef RTCONFIG_DSL_TCLINUX
void InitAutoDet(void)
{
	char tccmd[MAX_RESP_BUF_SIZE] = {0};
	PUCHAR pRespBuf[MAX_RESP_BUF_NUM];
	PUSHORT pRespLen[MAX_RESP_BUF_NUM];
	USHORT RespPktNum;

	init_resp_buf (pRespBuf, pRespLen);

	strcat(tccmd, "tcapi set Wan_PVC0 Active Yes; ");
	strcat(tccmd, "killall auto_det; ");
	strcat(tccmd, "auto_det &\x0d\x0a");

	SendCmdAndWaitResp(pRespBuf, MAX_RESP_BUF_SIZE, pRespLen, MAX_RESP_BUF_NUM, &RespPktNum, m_AdslMacAddr,
		MAC_RTS_CONSOLE_CMD, (PUCHAR)tccmd, GET_LEN(tccmd), 1);

}

int HandleAutoDet(void)
{
	char tccmd[MAX_RESP_BUF_SIZE] = {0};
	char *pch;
	int ret = 1;
	char buf[80];
	FILE* fp;
	PUCHAR pRespBuf[MAX_RESP_BUF_NUM];
	PUSHORT pRespLen[MAX_RESP_BUF_NUM];
	USHORT RespPktNum;

	init_resp_buf (pRespBuf, pRespLen);

	sprintf(tccmd, "rm -f %s; tcapi show AutoPVC_Common > %s\x0d\x0a",
		REMOTE_FILE_AUTOPVC_COMMON, REMOTE_FILE_AUTOPVC_COMMON);

	SendCmdAndWaitResp(pRespBuf, MAX_RESP_BUF_SIZE, pRespLen, MAX_RESP_BUF_NUM, &RespPktNum, m_AdslMacAddr,
		MAC_RTS_CONSOLE_CMD, (PUCHAR)tccmd, GET_LEN(tccmd), 1);

	tftp_get_file(REMOTE_FILE_AUTOPVC_COMMON, LOCAL_FILE_AUTOPVC_COMMON);

	fp = fopen(LOCAL_FILE_AUTOPVC_COMMON, "r");
	if(fp) {
		while(fgets(buf, sizeof(buf), fp)) {
			pch = strchr(buf, '\n');
			if(pch)
				*pch = '\0';

			if(!strncmp(buf, "dsltmp_autodet_state", 10)) {
				pch = strchr(buf, '=');
				if(pch)
					nvram_set("dsltmp_autodet_state", pch+1);
			}
			else if(!strncmp(buf, "Detect_VPI", 10)) {
				pch = strchr(buf, '=');
				if(pch)
					nvram_set("dsltmp_autodet_vpi", pch+1);
			}
			else if(!strncmp(buf, "Detect_VCI", 10)) {
				pch = strchr(buf, '=');
				if(pch)
					nvram_set("dsltmp_autodet_vci", pch+1);
			}
			else if(!strncmp(buf, "Detect_Encap", 10)) {
				pch = strchr(buf, '=');
				if(pch) {
					if(!strncmp(pch+1, "vc", 2))
						nvram_set("dsltmp_autodet_encap", "1");
					else
						nvram_set("dsltmp_autodet_encap", "0");
				}
			}
			else if(!strncmp(buf, "Detect_XDSL", 10)) {
				pch = strchr(buf, '=');
				if(pch) {
					nvram_set("dsltmp_autodet_wan_type", pch+1);
					if(!strncmp(pch+1, "PTM", 3))
						ret = 0;
				}
			}
		}
		fclose(fp);
	}

	if( nvram_match("dsltmp_autodet_state", "pppoe")
		|| nvram_match("dsltmp_autodet_state", "pppoa")
		|| nvram_match("dsltmp_autodet_state", "dhcp")
		|| nvram_match("dsltmp_autodet_state", "Fail")
	)
		ret = 0;

	return ret;
}

int tftp_get_file(char* remote_full_path, char* local_full_path)
{
	char remote_filename[32] = {0};
	char *pch;
	char tccmd[MAX_RESP_BUF_SIZE] = {0};
	int ret = -1;
	PUCHAR pRespBuf[MAX_RESP_BUF_NUM];
	PUSHORT pRespLen[MAX_RESP_BUF_NUM];
	USHORT RespPktNum;

	if(!remote_full_path || !local_full_path)
		return ret;

	if(strncmp(remote_full_path, "/var/tmp", 8)) {
		sprintf(tccmd, "cp %s /var/tmp\x0d\x0a", remote_full_path);
		init_resp_buf (pRespBuf, pRespLen);
		SendCmdAndWaitResp(pRespBuf, MAX_RESP_BUF_SIZE, pRespLen, MAX_RESP_BUF_NUM, &RespPktNum, m_AdslMacAddr,
			MAC_RTS_CONSOLE_CMD, (PUCHAR)tccmd, GET_LEN(tccmd), 1);
	}

	pch = strrchr(remote_full_path, '/');
	if(pch)
		strncpy(remote_filename, pch+1, sizeof(remote_filename));
	else
		strncpy(remote_filename, remote_full_path, sizeof(remote_filename));

	ret = eval("tftp", "-g", "-r", remote_filename, "-l", local_full_path, m_IpAddr.IpAddr);
	return ret;
}

int CustomTcCmd(char* cmd)
{
	char tccmd[MAX_RESP_BUF_SIZE] = {0};
	PUCHAR pRespBuf[MAX_RESP_BUF_NUM];
	PUSHORT pRespLen[MAX_RESP_BUF_NUM];
	USHORT RespPktNum;
	init_resp_buf (pRespBuf, pRespLen);

	if(cmd && strlen(cmd)) {
		sprintf(tccmd,
				"rm -f %s; "
				"%s >%s; "
				"\x0d\x0a"
				, REMOTE_FILE_CMD_RESULT
				, cmd, REMOTE_FILE_CMD_RESULT
				);
	}
	else {
		if(!eval("cd", "/tmp/adsl", ";", "tftp", "-p", "-l", "cmd.sh", m_IpAddr.IpAddr))
		sprintf(tccmd,
				"rm -f %s; "
				"chmod 755 %s; "
				"%s >%s; "
				"\x0d\x0a"
				, REMOTE_FILE_CMD_RESULT
				, REMOTE_FILE_CMD_SCRIPT
				, REMOTE_FILE_CMD_SCRIPT, REMOTE_FILE_CMD_RESULT
				);
		else
			myprintf("put script file failed\n");
	}

	SendCmdAndWaitResp(pRespBuf, MAX_RESP_BUF_SIZE, pRespLen, MAX_RESP_BUF_NUM, &RespPktNum, m_AdslMacAddr,
		MAC_RTS_CONSOLE_CMD, (PUCHAR)tccmd, strlen(tccmd), 1);

	sleep(1);
	return tftp_get_file(REMOTE_FILE_CMD_RESULT, LOCAL_FILE_CMD_RESULT);
}


int HandleAdslEntry(void)
{
	FILE* fp;
	char buf[80];
	char* pch;
	char tccmd[MAX_RESP_BUF_SIZE];
	PUCHAR pRespBuf[MAX_RESP_BUF_NUM];
	PUSHORT pRespLen[MAX_RESP_BUF_NUM];
	USHORT RespPktNum;
	init_resp_buf (pRespBuf, pRespLen);

	memset(tccmd, 0, sizeof(tccmd));
	sprintf(tccmd, "tcapi show Adsl >%s\x0d\x0a", REMOTE_FILE_ADSL_CONF);
	SendCmdAndWaitResp(pRespBuf, MAX_RESP_BUF_SIZE, pRespLen, MAX_RESP_BUF_NUM, &RespPktNum, m_AdslMacAddr,
		MAC_RTS_CONSOLE_CMD, (PUCHAR)tccmd, GET_LEN(tccmd), 1);

	tftp_get_file(REMOTE_FILE_ADSL_CONF, LOCAL_FILE_ADSL_CONF);
	fp = fopen(LOCAL_FILE_ADSL_CONF, "r");
	if(fp) {
		while(fgets(buf, sizeof(buf), fp)) {
			if(!strncmp(buf, "MODULATIONTYPE", 3)) {
				pch = strchr(buf, '=');
				if(pch)
					strncpy(m_AdslEntry.modulationtype, pch+1, strcspn(pch+1, "\n"));
			}
			else if(!strncmp(buf, "ANNEXTYPEA", 3)) {
				pch = strchr(buf, '=');
				if(pch)
					strncpy(m_AdslEntry.annextypea, pch+1, strcspn(pch+1, "\n"));
			}
			else if(!strncmp(buf, "SRAActive", 3)) {
				pch = strchr(buf, '=');
				if(pch)
					strncpy(m_AdslEntry.sraactive, pch+1, strcspn(pch+1, "\n"));
			}
			else if(!strncmp(buf, "BitSwapActive", 3)) {
				pch = strchr(buf, '=');
				if(pch)
					strncpy(m_AdslEntry.bitswapactive, pch+1, strcspn(pch+1, "\n"));
			}
			else if(!strncmp(buf, "SNRMOffset", 3)) {
				pch = strchr(buf, '=');
				if(pch)
					strncpy(m_AdslEntry.snrmoffset, pch+1, strcspn(pch+1, "\n"));
			}
			else if(!strncmp(buf, "VdslTargetSNRM", 6)) {
				pch = strchr(buf, '=');
				if(pch)
					strncpy(m_AdslEntry.vdsltargetsnrm, pch+1, strcspn(pch+1, "\n"));
			}
			else if(!strncmp(buf, "VdslTxPowerGainOffset", 6)) {
				pch = strchr(buf, '=');
				if(pch)
					strncpy(m_AdslEntry.vdsltxpowergainoffset, pch+1, strcspn(pch+1, "\n"));
			}
			else if(!strncmp(buf, "VdslRxAutoGainCtrl", 6)) {
				pch = strchr(buf, '=');
				if(pch)
					strncpy(m_AdslEntry.vdslrxautogainctrl, pch+1, strcspn(pch+1, "\n"));
			}
			else if(!strncmp(buf, "VdslUpPowerBackOff", 6)) {
				pch = strchr(buf, '=');
				if(pch)
					strncpy(m_AdslEntry.vdsluppowerbackoff, pch+1, strcspn(pch+1, "\n"));
			}
			memset(buf, 0, sizeof(buf));
		}
		fclose(fp);
		return 0;
	}
	return -1;
}

int CommitAdslEntry(void)
{
	PUCHAR pRespBuf[MAX_RESP_BUF_NUM];
	PUSHORT pRespLen[MAX_RESP_BUF_NUM];
	USHORT RespPktNum;
	init_resp_buf (pRespBuf, pRespLen);

	char tccmd[MAX_RESP_BUF_SIZE] = {0};

	if(m_AdslCommitFlag)
	{
		memset(tccmd, 0, sizeof(tccmd));
		strcpy(tccmd, "tcapi commit Adsl_Entry; tcapi save;\x0d\x0a");
		SendCmdAndWaitResp(pRespBuf, MAX_RESP_BUF_SIZE, pRespLen, MAX_RESP_BUF_NUM, &RespPktNum, m_AdslMacAddr,
			MAC_RTS_CONSOLE_CMD, (PUCHAR)tccmd, GET_LEN(tccmd), 1);
		m_AdslCommitFlag = 0;
	}
	return 0;
}

int CommitWanPVC(void)
{
	PUCHAR pRespBuf[MAX_RESP_BUF_NUM];
	PUSHORT pRespLen[MAX_RESP_BUF_NUM];
	USHORT RespPktNum;
	init_resp_buf (pRespBuf, pRespLen);

	char tccmd[MAX_RESP_BUF_SIZE] = {0};

	memset(tccmd, 0, sizeof(tccmd));
	strcpy(tccmd, "tcapi commit Wan\x0d\x0a");
	SendCmdAndWaitResp(pRespBuf, MAX_RESP_BUF_SIZE, pRespLen, MAX_RESP_BUF_NUM, &RespPktNum, m_AdslMacAddr,
		MAC_RTS_CONSOLE_CMD, (PUCHAR)tccmd, GET_LEN(tccmd), 1);

	return 0;
}

int HandleInfoAdsl(void)
{
	FILE* fp;
	char tccmd[MAX_RESP_BUF_SIZE];
	char buf[80];
	char* pch;
	PUCHAR pRespBuf[MAX_RESP_BUF_NUM];
	PUSHORT pRespLen[MAX_RESP_BUF_NUM];
	USHORT RespPktNum;
	init_resp_buf (pRespBuf, pRespLen);

	memset(tccmd, 0, sizeof(tccmd));
	strcat(tccmd, "tcapi get Info_Adsl lineState; ");
	sprintf(tccmd + strlen(tccmd), "rm %s; ", REMOTE_FILE_INFO_ADSL);
	sprintf(tccmd + strlen(tccmd), "tcapi show Info_Adsl >%s; ", REMOTE_FILE_INFO_ADSL);
	strcat(tccmd, "\x0d\x0a");
	SendCmdAndWaitResp(pRespBuf, MAX_RESP_BUF_SIZE, pRespLen, MAX_RESP_BUF_NUM, &RespPktNum, m_AdslMacAddr,
		MAC_RTS_CONSOLE_CMD, (PUCHAR)tccmd, GET_LEN(tccmd), 1);

	tftp_get_file(REMOTE_FILE_INFO_ADSL, LOCAL_FILE_INFO_ADSL);

	fp = fopen(LOCAL_FILE_INFO_ADSL, "r");
	if(fp) {
		while(fgets(buf, sizeof(buf), fp)) {
			pch = strchr(buf, '\n');
			if(pch)
				*pch = '\0';

			if(!strncmp(buf, "lineState", 3)) {
				pch = strchr(buf, '=');
				if(pch)
					strncpy(m_InfoAdsl.linestate, pch+1, sizeof(m_InfoAdsl.linestate)-1);
			}
			//ADSLUpTime=17 min, 29 secs
			//ADSLActiveTime=0 min, 13 secs
			else if(!strncmp(buf, "Opmode", 3)) {
				pch = strchr(buf, '=');
				if(pch)
					strncpy(m_InfoAdsl.opmode, pch+1, sizeof(m_InfoAdsl.opmode)-1);
			}
			else if(!strncmp(buf, "AdslType", 5)) {
				pch = strchr(buf, '=');
				if(pch)
					strncpy(m_InfoAdsl.adsltype, pch+1, sizeof(m_InfoAdsl.adsltype)-1);
			}
			else if(!strncmp(buf, "SNRMarginDown", 10)) {
				pch = strchr(buf, '=');
				if(pch)
					strncpy(m_InfoAdsl.snrmargindown, pch+1, sizeof(m_InfoAdsl.snrmargindown)-1);
			}
			else if(!strncmp(buf, "SNRMarginUp", 10)) {
				pch = strchr(buf, '=');
				if(pch)
					strncpy(m_InfoAdsl.snrmarginup, pch+1, sizeof(m_InfoAdsl.snrmarginup)-1);
			}
			else if(!strncmp(buf, "AttenDown", 6)) {
				pch = strchr(buf, '=');
				if(pch)
					strncpy(m_InfoAdsl.attendown, pch+1, sizeof(m_InfoAdsl.attendown)-1);
			}
			else if(!strncmp(buf, "AttenUp", 6)) {
				pch = strchr(buf, '=');
				if(pch)
					strncpy(m_InfoAdsl.attenup, pch+1, sizeof(m_InfoAdsl.attenup)-1);
			}
			else if(!strncmp(buf, "PowerDown", 6)) {
				pch = strchr(buf, '=');
				if(pch)
					strncpy(m_InfoAdsl.powerdown, pch+1, sizeof(m_InfoAdsl.powerdown)-1);
			}
			else if(!strncmp(buf, "PowerUp", 6)) {
				pch = strchr(buf, '=');
				if(pch)
					strncpy(m_InfoAdsl.powerup, pch+1, sizeof(m_InfoAdsl.powerup)-1);
			}
			else if(!strncmp(buf, "DataRateDown", 9)) {
				pch = strchr(buf, '=');
				if(pch)
					strncpy(m_InfoAdsl.dataratedown, pch+1, sizeof(m_InfoAdsl.dataratedown)-1);
			}
			else if(!strncmp(buf, "DataRateUp", 9)) {
				pch = strchr(buf, '=');
				if(pch)
					strncpy(m_InfoAdsl.datarateup, pch+1, sizeof(m_InfoAdsl.datarateup)-1);
			}
			else if(!strncmp(buf, "AttainDown", 7)) {
				pch = strchr(buf, '=');
				if(pch)
					strncpy(m_InfoAdsl.attaindown, pch+1, sizeof(m_InfoAdsl.attaindown)-1);
			}
			else if(!strncmp(buf, "AttainUp", 7)) {
				pch = strchr(buf, '=');
				if(pch)
					strncpy(m_InfoAdsl.attainup, pch+1, sizeof(m_InfoAdsl.attainup)-1);
			}
			else if(!strncmp(buf, "CRCDown", 4)) {
				pch = strchr(buf, '=');
				if(pch)
					strncpy(m_InfoAdsl.crcdown, pch+1, sizeof(m_InfoAdsl.crcdown)-1);
			}
			else if(!strncmp(buf, "CRCUp", 4)) {
				pch = strchr(buf, '=');
				if(pch)
					strncpy(m_InfoAdsl.crcup, pch+1, sizeof(m_InfoAdsl.crcup)-1);
			}
			memset(buf, 0, sizeof(buf));
		}
		fclose(fp);
		return 0;
	}
	return -1;
}

int GetDslPerf(void)
{
	return tftp_get_file(REMOTE_FILE_DSL_PERF, LOCAL_FILE_DSL_PERF);
}

int AllLedOnOff(char *onoff)
{
	PUCHAR pRespBuf[MAX_RESP_BUF_NUM];
	PUSHORT pRespLen[MAX_RESP_BUF_NUM];
	USHORT RespPktNum;
	init_resp_buf (pRespBuf, pRespLen);

	char tccmd[MAX_RESP_BUF_SIZE];

	if(!onoff)
		return -1;

	if(!strcmp(onoff, "on")) {
		memset(tccmd, 0, sizeof(tccmd));
		strcpy(tccmd, "sys led on\x0d\x0a");
		SendCmdAndWaitResp(pRespBuf, MAX_RESP_BUF_SIZE, pRespLen, MAX_RESP_BUF_NUM, &RespPktNum, m_AdslMacAddr,
			MAC_RTS_CONSOLE_CMD, (PUCHAR)tccmd, GET_LEN(tccmd), 1);
	}
	else if(!strcmp(onoff, "off")) {
		memset(tccmd, 0, sizeof(tccmd));
		strcpy(tccmd, "sys led off\x0d\x0a");
		SendCmdAndWaitResp(pRespBuf, MAX_RESP_BUF_SIZE, pRespLen, MAX_RESP_BUF_NUM, &RespPktNum, m_AdslMacAddr,
			MAC_RTS_CONSOLE_CMD, (PUCHAR)tccmd, GET_LEN(tccmd), 1);
	}
	else
		return -1;

	return 0;
}

#ifdef RTCONFIG_VDSL
void AddPtm(int idx, int ext_idx, int vlan_active, int vlan_id)
{
	char tccmd[MAX_RESP_BUF_SIZE] = {0};
	PUCHAR pRespBuf[MAX_RESP_BUF_NUM];
	PUSHORT pRespLen[MAX_RESP_BUF_NUM];
	USHORT RespPktNum;
	init_resp_buf (pRespBuf, pRespLen);

	sprintf(tccmd + strlen(tccmd), "tcapi set WebCurSet_Entry wan_pvc %d; ", idx);
	sprintf(tccmd + strlen(tccmd), "tcapi set WebCurSet_Entry wan_pvc_ext %d; ", ext_idx);
	strcat(tccmd, "tcapi set Wan_PVC Active Yes; ");
	sprintf(tccmd + strlen(tccmd), "tcapi set Wan_PVC dot1q %s; ", vlan_active?"Yes":"No");
	sprintf(tccmd + strlen(tccmd), "tcapi set Wan_PVC VLANID %d; ", vlan_id);
	strcat(tccmd, "tcapi set Wan_PVC ISP 3; ");
	strcat(tccmd, "tcapi commit Wan_PVC; ");
	strcat(tccmd, "\x0d\x0a");
	SendCmdAndWaitResp(pRespBuf, MAX_RESP_BUF_SIZE, pRespLen, MAX_RESP_BUF_NUM, &RespPktNum, m_AdslMacAddr,
		MAC_RTS_CONSOLE_CMD, (PUCHAR)tccmd, GET_LEN(tccmd), 1);
}

int SetVdslTargetSNRM(int SNRMValue, int FromAteCmd)
{
	PUCHAR pRespBuf[MAX_RESP_BUF_NUM];
	PUSHORT pRespLen[MAX_RESP_BUF_NUM];
	USHORT RespPktNum;
	init_resp_buf (pRespBuf, pRespLen);

	char tccmd[MAX_RESP_BUF_SIZE] = {0};
	char szValue[8] = {0};

	sprintf(szValue, "%d", SNRMValue);

	if(strcmp(m_AdslEntry.vdsltargetsnrm, szValue))
	{
		myprintf("%s [%s]!=[%s]\n", __FUNCTION__, m_AdslEntry.vdsltargetsnrm, szValue);
		memset(tccmd, 0, sizeof(tccmd));
		sprintf(tccmd, "tcapi set Adsl_Entry VdslTargetSNRM %s\x0d\x0a", szValue);
		SendCmdAndWaitResp(pRespBuf, MAX_RESP_BUF_SIZE, pRespLen, MAX_RESP_BUF_NUM, &RespPktNum, m_AdslMacAddr,
			MAC_RTS_CONSOLE_CMD, (PUCHAR)tccmd, GET_LEN(tccmd), 1);
		m_AdslCommitFlag = 1;
	}

	return 0;
}

int SetVdslTxPowerGainOffset(int VdslTxPowerGainOffset, int FromAteCmd)
{
	PUCHAR pRespBuf[MAX_RESP_BUF_NUM];
	PUSHORT pRespLen[MAX_RESP_BUF_NUM];
	USHORT RespPktNum;
	init_resp_buf (pRespBuf, pRespLen);

	char tccmd[MAX_RESP_BUF_SIZE] = {0};
	char szValue[8] = {0};

	sprintf(szValue, "%d", VdslTxPowerGainOffset);

	if(strcmp(m_AdslEntry.vdsltxpowergainoffset, szValue))
	{
		myprintf("%s [%s]!=[%s]\n", __FUNCTION__, m_AdslEntry.vdsltxpowergainoffset, szValue);
		memset(tccmd, 0, sizeof(tccmd));
		sprintf(tccmd, "tcapi set Adsl_Entry VdslTxPowerGainOffset %s\x0d\x0a", szValue);
		SendCmdAndWaitResp(pRespBuf, MAX_RESP_BUF_SIZE, pRespLen, MAX_RESP_BUF_NUM, &RespPktNum, m_AdslMacAddr,
			MAC_RTS_CONSOLE_CMD, (PUCHAR)tccmd, GET_LEN(tccmd), 1);
		m_AdslCommitFlag = 1;
	}
	return 0;
}

int SetVdslRxAutoGainCtrl(int VdslRxAutoGainCtrl, int FromAteCm)
{
	PUCHAR pRespBuf[MAX_RESP_BUF_NUM];
	PUSHORT pRespLen[MAX_RESP_BUF_NUM];
	USHORT RespPktNum;
	init_resp_buf (pRespBuf, pRespLen);

	char tccmd[MAX_RESP_BUF_SIZE] = {0};
	char szValue[8] = {0};

	sprintf(szValue, "%d", VdslRxAutoGainCtrl);

	if(strcmp(m_AdslEntry.vdslrxautogainctrl, szValue))
	{
		myprintf("%s [%s]!=[%s]\n", __FUNCTION__, m_AdslEntry.vdslrxautogainctrl, szValue);
		memset(tccmd, 0, sizeof(tccmd));
		sprintf(tccmd, "tcapi set Adsl_Entry VdslRxAutoGainCtrl %s\x0d\x0a", szValue);
		SendCmdAndWaitResp(pRespBuf, MAX_RESP_BUF_SIZE, pRespLen, MAX_RESP_BUF_NUM, &RespPktNum, m_AdslMacAddr,
			MAC_RTS_CONSOLE_CMD, (PUCHAR)tccmd, GET_LEN(tccmd), 1);
		m_AdslCommitFlag = 1;
	}
	return 0;
}

int SetVdslUpPowerBackOff(char *VdslUpPowerBackOff, int FromAteCm)
{
	PUCHAR pRespBuf[MAX_RESP_BUF_NUM];
	PUSHORT pRespLen[MAX_RESP_BUF_NUM];
	USHORT RespPktNum;
	init_resp_buf (pRespBuf, pRespLen);

	char tccmd[MAX_RESP_BUF_SIZE] = {0};

	if(strcmp(m_AdslEntry.vdsluppowerbackoff, VdslUpPowerBackOff))
	{
		myprintf("%s [%s]!=[%s]\n", __FUNCTION__, m_AdslEntry.vdsluppowerbackoff, VdslUpPowerBackOff);
		memset(tccmd, 0, sizeof(tccmd));
		sprintf(tccmd, "tcapi set Adsl_Entry VdslUpPowerBackOff %s\x0d\x0a", VdslUpPowerBackOff);
		SendCmdAndWaitResp(pRespBuf, MAX_RESP_BUF_SIZE, pRespLen, MAX_RESP_BUF_NUM, &RespPktNum, m_AdslMacAddr,
			MAC_RTS_CONSOLE_CMD, (PUCHAR)tccmd, GET_LEN(tccmd), 1);
		m_AdslCommitFlag = 1;
	}
	return 0;
}

int getModulation(void)
{
	int mode = DSL_MODE_NOT_UP;

	if(nvram_match("dsltmp_adslsyncsts", "up"))
	{
		if(strstr(m_InfoAdsl.opmode, "ANSI T1.413"))
		{
			mode = DSL_MODE_ADSL_ANSI;
		}
		else if(strstr(m_InfoAdsl.opmode, "ITU G.992.1(G.DMT)"))
		{
			mode = DSL_MODE_ADSL_OPEN_GDMT;
		}
		else if(strstr(m_InfoAdsl.opmode, "ITU G.992.2(G.Lite)"))
		{
			mode = DSL_MODE_ADSL_OPEN_GLITE;
		}
		else if(strstr(m_InfoAdsl.opmode, "ITU G.992.3(ADSL2)"))
		{
			mode = DSL_MODE_ADSL2;
		}
		else if(strstr(m_InfoAdsl.opmode, "ITU G.992.5(ADSL2PLUS)"))
		{
			mode = DSL_MODE_ADSL2PLUS;
		}
		else if(strstr(m_InfoAdsl.opmode, "ITU G.993.2(VDSL2)"))
		{
			mode = DSL_MODE_VDSL2;
		}
	}
	else
	{
		mode = DSL_MODE_NOT_UP;
	}

	return mode;
}

#endif
#endif
