#ifndef _DOWNLOAD_H_

#define _DOWNLOAD_H_

typedef	void	VOID ;
typedef	char	CHAR;
typedef	unsigned	char	BYTE;
typedef	unsigned	short	WORD;
typedef	unsigned	long	DWORD;
typedef	int	INT;
//typedef	short	INT;
typedef	unsigned	int	UINT;
//typedef	unsigned	short	UINT;
typedef	long	LONG;
typedef	unsigned long	ULONG;

//#define _DEBUG_
#define INTERFACE	"ixp0"
#define TIMEOUT		4
#define xchgs(x) (WORD)((((WORD)((WORD)x)&0x0ff)<<8)|(((WORD)((WORD)x)&0x0ff00)>>8))
#define PACKET_SIZE         512

#define  MIN_PACKET_LENGTH  64
#define  DL_ACK_LEN 80
#define PACK
#define PACKED __attribute__((packed))

typedef struct{
	BYTE dbDestAddress[6];
	BYTE dbSrcAddress[6];
	WORD dwID;
	WORD dwCmd;
	WORD dwSeq;
	WORD dwOff;
	WORD dwSeg;
	WORD dwLen;
	BYTE dbData[512];
}PACKED DOWNLOADHead;

typedef struct S_HWID {
#ifdef _BIG_ENDIAN_
  /*----- Interface --------------------------*/
  BYTE  fPrn      :2;  /* Printer */
  BYTE  fMIO      :1;  /* MIO */
  BYTE  fChinese  :1;  /* Chinese */
  BYTE  fSCSI     :2;  /* SCSI */
  BYTE  fPrnIn    :2;  /* Printer port in */

  BYTE  fNetworkNo:4;  /* Number of network */
  BYTE  fSerialNo :4;  /* Number of serial port */

  /*----- System -----------------------------*/
  BYTE  fRomType  :1;  /* 0= Fladsh, 1= OTROM */
  BYTE  fRAM      :3;  /* 000= 32K, 001= 128K, 010= 256K, 011= 1024K */
  BYTE  fNVRAM    :2;  /* 00= 256B, 01= 512B, 10= 8192B */
  BYTE  fRsv_2    :2;

  BYTE  fCPU      :3;  /* 000= 188, 001= 186, 010= 386DX */

  BYTE  fSpeed    :2;  /*      188/186   386DX
                           00   12MHz    16MHz
                           01   16MHz    25MHz
                           10   25MHz    33MHz
                           11   40MHz    40MHz  */
  BYTE  fRomSize  :3;  /* 000= 64K, 001= 128K, 010= 256K, 011= 512K */

  /*----- Card number ------------------------*/
  BYTE  bVersion  :4;
  BYTE  bRsv_1    :4;

  BYTE  fProduct2 :4;
  BYTE  fDetail   :4;

  BYTE  fProduct0 :4;
  BYTE  fProduct1 :4;
#else
  /*----- Card number ------------------------*/
  BYTE  fProduct1 :4;
  BYTE  fProduct0 :4;

  BYTE  fDetail   :4;
  BYTE  fProduct2 :4;

  BYTE  bRsv_1    :4;
  BYTE  bVersion  :4;

  /*----- System -----------------------------*/
  BYTE  fRomSize  :3;  /* 000= 64K, 001= 128K, 010= 256K, 011= 512K */
  BYTE  fSpeed    :2;  /*      188/186   386DX
                           00   12MHz    16MHz
                           01   16MHz    25MHz
                           10   25MHz    33MHz
                           11   40MHz    40MHz  */
  BYTE  fCPU      :3;  /* 000= 188, 001= 186, 010= 386DX */

  BYTE  fRsv_2    :2;
  BYTE  fNVRAM    :2;  /* 00= 256B, 01= 512B, 10= 8192B */
  BYTE  fRAM      :3;  /* 000= 32K, 001= 128K, 010= 256K, 011= 1024K */
  BYTE  fRomType  :1;  /* 0= Fladsh, 1= OTROM */

  /*----- Interface --------------------------*/
  BYTE  fSerialNo :4;  /* Number of serial port */
  BYTE  fNetworkNo:4;  /* Number of network */

  BYTE  fPrnIn    :2;  /* Printer port in */
  BYTE  fSCSI     :2;  /* SCSI */
  BYTE  fChinese  :1;  /* Chinese */
  BYTE  fMIO      :1;  /* MIO */
  BYTE  fPrn      :2;  /* Printer */
#endif

  WORD  wRsv_3;

  /*----- description ------------------------*/
  BYTE  bComment[23];

} PACKED HWID;

typedef struct S_PID {

  /*----- PID version ------------------------*/
  WORD  wPidVersion;        /* Motorola format */

  /*----- D/L control ------------------------*/
  BYTE  fReserved;
#ifdef _BIG_ENDIAN_
  BYTE  fRsv        :  4;
  BYTE  fChkFunction:  1;
  BYTE  fChkProtocol:  1;
  BYTE  fChkProduct:   1;
  BYTE  fChkHw:        1;
#else
  BYTE  fChkHw:        1;
  BYTE  fChkProduct:   1;
  BYTE  fChkProtocol:  1;
  BYTE  fChkFunction:  1;
  BYTE  fRsv        :  4;
#endif
  /*----- Hardware ID ------------------------*/
  HWID  hw;

  /*----- Reserver ---------------------------*/
  WORD  wRsv;

  /*----- Product ID & mask ------------------*/
  WORD  wProductID;         /* Motorola format */
  WORD  wProductMask;       /* Motorola format */

  /*----- Protocol ID & mask -----------------*/
  WORD  wProtocolID;        /* Motorola format */
  WORD  wProtocolMask;      /* Motorola format */

  /*----- Function ID & mask -----------------*/
  WORD  wFunctionID;        /* Motorola format */
  WORD  wFunctionMask;      /* Motorola format */

  /*----- F/W version ------------------------*/
  WORD  wFwVersion;         /* Motorola format */

  /*----- Code address -----------------------*/
  WORD  wSegment;           /* Intel format */
  WORD  wOffset;            /* Intel format */

} PACKED PID;

typedef  struct  VCI  {
    WORD    VerControl;
    WORD    DownControl;
    BYTE    Hid[32];
    WORD    Hver;
    WORD    ProdID;
    WORD    ProdIDmask;
    WORD    ProtID;
    WORD    ProtIDmask;
    WORD    FuncID;
    WORD    FuncIDmask;
    WORD    Fver;
    WORD    Cseg;
	WORD    Csize;
} PACKED VCI_TABLE;

#define  VCI_LEN         56

#endif
