/*****************************************************************************/
/*      PROJECT         :                                                    */
/*      PROGRAM         :       DLL_IOModule.h                               */
/*      VERSION         :       1.0                                          */
/*      COMPLIER        :       Borland C++ Builder Ver6.0                   */
/*      PROGRAMMER      :       Lee Chan-Woo                                 */
/*****************************************************************************/

#pragma once

#define SOH         0x01
#define STX         0x02
#define ETX         0x03
#define EOT         0x04
#define ENQ         0x05
#define ACK         0x06
#define BEL         0x07
#define LF          0x0a
#define CR          0x0d
#define SO          0x0e
#define SI          0x0f
#define DLE         0x10
#define TIME        0x11
#define FULL        0x12
#define BLD         0x13
 #define NAK         0x15
#define SYN         0x16
#define FAIL        0x18
#define EOM         0x19
#define ESC         0x1b
#define FS          0x1c

#define	MAX_TCMSGNUM			    1000
#define	NUM_RECVHEAP			    1024*100
#define	NUM_COMMHEAP			    1024*100
#define	NUM_COMMBUF				    1024*20
#define	NUM_TXBUF				    1024*100
#define	NUM_RXBUF				    1024*100
#define COMM_MSG_BUF			    1024*2

// Error Code Define
#define E_SERIAL_BASE_ERROR			0
#define E_ALREADYOPEN				E_SERIAL_BASE_ERROR + 1		// Serial Open�� ��� �̹� �������� ��� ���� �ڵ�
#define E_SETTIMEOUT				E_SERIAL_BASE_ERROR + 2		// TimeOut �ð� ���� ����
#define E_INVALIDBAUDRATE			E_SERIAL_BASE_ERROR + 3		// �������� �ʴ� BAUDRATE
#define E_SERIALCONFSET				E_SERIAL_BASE_ERROR + 4		// Serial Configuration ���� ����
#define E_MAINPROCEVENT				E_SERIAL_BASE_ERROR + 5		// ����Ÿ ���Ž� ���� �̺�Ʈ�� ���� �̺�Ʈ �ڵ��� ����
#define E_FILECREATE				E_SERIAL_BASE_ERROR + 6		// ��Ʈ ���� ����
#define E_PORTINVALID				E_SERIAL_BASE_ERROR + 7		// Invalid Port
#define E_CLASSCREATE				E_SERIAL_BASE_ERROR + 8		// Serial Class ��ü ���� ����
#define E_THREADCREATE				E_SERIAL_BASE_ERROR + 9		// Serial Class �� ������ ���� ����
#define E_TOOLARGEDATA				E_SERIAL_BASE_ERROR + 10	// �ѹ��� ������ �����Ͱ� Linked List �Ѱ��� ���ۺ��� ū ����
#define E_BUFFER_ALLOC				E_SERIAL_BASE_ERROR + 11	// ���۰� ����
#define E_CRC_CHECK					E_SERIAL_BASE_ERROR + 12	// CRC üũ�� �̻� �߻�
#define E_CHECKSUM_CHECK			E_SERIAL_BASE_ERROR + 13	// CheckSum üũ�� �̻� �߻�

#define COM1    1
#define COM2    2
#define COM3    3
#define COM4    4
#define COM5    5
#define COM6    6
#define COM7    7
#define COM8    8
#define COM9    9
#define COM10   10
#define COM11   11
#define COM12   12
#define COM13   13
#define COM14   14
#define COM15   15
#define COM16   16

extern  bool                m_bOpened;
extern  HANDLE              m_hCommPort;

extern  COMMTIMEOUTS        CommTimeouts;
extern  COMMTIMEOUTS        OldCommTimeouts;

extern  DCB                 m_DCBData;
extern  int                 m_nPort;
extern  DWORD               m_dwBaudRate;       // 110, 300, 600, 1200, 2400, 4800, 9600, 19200, 38400, 57600, 115200, 256000
extern  int                 m_nDataBit;         //  5, 6, 7, 8 byte
extern  char                m_szPort_parity;    // 'E', 'O', 'M', 'N'
extern  BYTE                m_cPort_Stop_Bit;   // 1==> 1, 2==>(2), else  (1.5)
extern  DWORD               m_dwTimeOut;

extern  HANDLE              m_hRecvEndEvt;                  // ���� ������ ���� �̺�Ʈ
extern  HANDLE              m_hRecvEvt;                     // ������ ���� �̺�Ʈ

extern  OVERLAPPED          m_osWrite;
extern  OVERLAPPED          m_osRead;

extern  HANDLE              m_hHeapRecv_Main;					// RECV COMM THREAD HEAP


//-----------------------------------------------------------------
// ���μ����� ��� �޽��� ���� ����ü ����
typedef struct tagRS_COMM_MSG
{
	BYTE				cTransCode;								// �޽��� �ڵ�
	BYTE				cData[512];							    // ���۵Ǵ� ������
} RS_COMM_MSG, *PRS_COMM_MSG;


//-----------------------------------------------------------------
// �������� ����
typedef struct tagRS_STATE_INFO_RESP_MSG
{
	BYTE				cDO_Ctrl_Sts;							// DO ���� ���� ���� (0~7bit�� DO ����(0:����, 1:����))
	BYTE				cDI_Ctrl_Sts;							// DI ���� ���� ���� (0~7bit�� DI ����(0:����, 1:����))
	BYTE				cTrigger_Event;							// �ܺ� ���ͷ�Ʈ ����(Ʈ���� ����)
	BYTE				cTrigger_Direction;						// �ܺ� ���ͷ�Ʈ �ϰ�(0), ���(1)
	BYTE				cWatchdog;							    // 0: �̻��, 1: ���
	BYTE				cKey_Lock;							    // 0: �̻��, 1: Glim, 2: �̼���, ~~
	BYTE				cTrigger_Pulse;							// Ʈ���� �޽� �ϰ�(0), ���(1)
	BYTE				cTrigger_Time;							// Ʈ���� �ð� ms
} RS_STATE_INFO_RESP_MSG, *PRS_STATE_INFO_RESP_MSG;


//-----------------------------------------------------------------
//
typedef struct tagRS_DO_CTRL_REQ_MSG
{
	BYTE				cDO_Port;
    BYTE				cOnOff_Ctrl;
} RS_DO_CTRL_REQ_MSG, *PRRS_DO_CTRL_REQ_MSG;


//-----------------------------------------------------------------
// DI �̺�Ʈ
typedef struct tagRS_DI_EVENT_RESP_MSG
{
	BYTE				cDI_Ctrl_Sts;
} RS_DI_EVENT_RESP_MSG, *PRS_DI_EVENT_RESP_MSG;


// Type definition for the callback function.
typedef bool CALLBACK CallbackFunc(BYTE *, DWORD);

// Declaration for the CalcResults function.
extern "C" __declspec(dllexport) void __stdcall IOCallBack(BYTE *_pbyBuffer, DWORD _nNbyte);


//extern "C" __declspec(dllexport) void __stdcall fnSetErrorMsg(int _nErrorCode);
void __stdcall fnSetErrorMsg(int _nErrorCode);

/*-------------------------------------------------------------------------*/
/*  Function DCB_Clear()                                                   */
/*  DCB(Device Control Block)                                              */
/*  DCB Initialize                                                         */
/*  Communication���� ó������ Setting �Ѵ�.                               */
/*-------------------------------------------------------------------------*/
//extern "C" __declspec(dllexport) void __stdcall DCB_Clear();


/*-------------------------------------------------------------------------*/
/* Function COM_Open()                                                     */
/* ��� ��Ʈ�� Open�Ѵ�.                                                   */
/* Window NT Comm Port Open                                                */
/* Return Value : 1:OK, else:NG                                            */
/*-------------------------------------------------------------------------*/
extern "C" __declspec(dllexport) int __stdcall COM_Open(int _nPort,
                                                        CallbackFunc* _Callback,
                                                        bool _bThreadUseFlag);


/*-------------------------------------------------------------------------*/
/* Function COM_Close()                                                    */
/* Open�ǿ� �ִ� Comm Port Close                                           */
/* Return Value : 1:OK, else:NG                                            */
/*-------------------------------------------------------------------------*/
extern "C" __declspec(dllexport) bool __stdcall COM_Close();


/*-------------------------------------------------------------------------*/
/* Function IsOpened()                                                     */
/* ��� Port Open���¸� Return �Ѵ�.                                       */
/* Port Open Check                                                         */
/* Return true, false                                                      */
/*-------------------------------------------------------------------------*/
extern "C" __declspec(dllexport) bool __stdcall COM_IsOpened();


/*-------------------------------------------------------------------------*/
/* Function COM_Read()                                                     */
/* ��� Port�� ���� ������ ���� �д´�.                                  */
/*-------------------------------------------------------------------------*/
extern "C" __declspec(dllexport) void __stdcall COM_Read();


extern "C" __declspec(dllexport) int __stdcall COM_ReadEx(int _nInputNo);

/*-------------------------------------------------------------------------*/
/* Function fnReleaseFrame()                                               */
/* ���ŵ� �������� �������� ���� ���θ� �˻��Ѵ�.                          */
/* pMsgBuf   : ���ŵ� ������ �� ���� �����͸� ���͸��� �� �����ϴ� ����    */
/* pMsgSize  : pMsgBuf ���� ������                                         */
/* pRecvBuf  : ���ŵ� ������ ����                                          */
/* nRecvSize : pRecvBuf ���� ������                                        */
/*-------------------------------------------------------------------------*/
int __stdcall fnReleaseFrame(PBYTE pMsgBuf, PINT pnMsgSize, PBYTE pRecvBuf, int nRecvSize);


/*-------------------------------------------------------------------------*/
/* Function fnProcRecvData()                                               */
/* ���ŵ����ʹ� fnReleaseFrame���� ���͸��� ��ģ ��                        */
/* fnProcRecvData�� ���޵˴ϴ�.                                            */
/*-------------------------------------------------------------------------*/
void __stdcall fnProcRecvData(PBYTE pMsgBuf, int nMsgSize);


/*-------------------------------------------------------------------------*/
/* Function fnState_Resp_Proc()                                            */
/* �������� ���� �����͸� ����ü�� �����Ѵ�.                               */
/*-------------------------------------------------------------------------*/
void __stdcall fnState_Resp_Proc(PRS_COMM_MSG _pRs_Comm_Msg);


/*-------------------------------------------------------------------------*/
/* Function fnTrigger_Event_Proc()                                         */
/* Ʈ���� �̺�Ʈ ���� �� �ش� �̺�Ʈ�� �ݹ��Լ��� ��������.                */
/*-------------------------------------------------------------------------*/
void __stdcall fnTrigger_Event_Proc();


/*-------------------------------------------------------------------------*/
/* Function COM_Write()                                                    */
/* ��� Port�� �ڷḦ ������.                                              */
/*-------------------------------------------------------------------------*/
bool __stdcall COM_Write(BYTE *_pbyBuffer, DWORD _nNbyte);


/*-------------------------------------------------------------------------*/
/* Function Get_RxBufferSize()                                             */
/* ��� Port�� ���� �ִ� Rx Count�� �о�´�.                            */
/* Return : ���Ź��ۿ� �ִ�  Rx Count                                      */
/*-------------------------------------------------------------------------*/
extern "C" __declspec(dllexport) DWORD __stdcall Get_RxBufferSize();


/*-------------------------------------------------------------------------*/
/* Function Get_TxBufferSize()                                             */
/* ��� Port�� ���� �ִ� Rx Count�� �о�´�.                            */
/* Return : ���Ź��ۿ� �ִ�  Rx Count                                      */
/*-------------------------------------------------------------------------*/
//extern "C" __declspec(dllexport) DWORD __stdcall Get_TxBufferSize();


extern "C" __declspec(dllexport) void __stdcall Get_State_Info();
//extern "C" __declspec(dllexport) void __stdcall Set_DO_Port_Ctrl(BYTE _cPort);

extern "C" __declspec(dllexport) void __stdcall Set_DO_Port_Ctrl(BYTE _cPort, int _nOnOffCtrl);


/*-------------------------------------------------------------------------*/
/* Function KeyCode()                                                      */
/* �Է¹��� ���ڿ���KeyCode�� ������ �� �����Ѵ�.                          */
/*-------------------------------------------------------------------------*/
extern "C" __declspec(dllexport) void __stdcall KeyCode(unsigned char *_pData,
                                                        unsigned char _cDataSize,
                                                        unsigned char *_pBuf,
                                                        PINT _cBufSize);

/*-------------------------------------------------------------------------*/
/* Function Make_1Byte_BCC()                                               */
/* �����ڷḦ Check Sum�� �� �ְ� XOR �� ��Ų��.                           */
/* XOR�� ���� 0x20���� ���� ��� 0x20�� Add���� �ʴ´�.                    */
/*-------------------------------------------------------------------------*/
//extern "C" __declspec(dllexport) void __stdcall Make_1Byte_BCC(BYTE *_pbyTxBuffer,
//                                                               int _nMakeLength);


/*-------------------------------------------------------------------------*/
/* Function Check_1Byte_BCC()                                              */
/* ���� �ڷḦ Check Sum�� �Ͽ� ���� Check Sum ���ϰ� �� �Ѵ�.           */
/* XOR �� ��Ų��.                                                          */
/* XOR�� ���� 0x20���� ���� ��� 0x20�� Add���� �ʴ´�.                    */
/*-------------------------------------------------------------------------*/
//extern "C" __declspec(dllexport) bool __stdcall Check_1Byte_BCC(BYTE *_pbyRxBuffer,
//                                                                int _nCheckLength);


/*-------------------------------------------------------------------------*/
/* Function Ascii_To_HexStr()                                              */
/* Ascii �� 2 Byte Hex�� �����.                                           */
/*-------------------------------------------------------------------------*/
//extern "C" __declspec(dllexport) void __stdcall Ascii_To_HexStr(BYTE *Ret_msg,
//                                                                BYTE *Sor_msg,
//                                                                int length_of_Sor);


/*-------------------------------------------------------------------------*/
/* Function HexStr_To_Ascii()                                              */
/* Hex String�� 1Byte Ascii�� �����.                                      */
/*-------------------------------------------------------------------------*/
//extern "C" __declspec(dllexport) void __stdcall HexStr_To_Ascii(BYTE *Ret_msg,
//                                                                BYTE *Sor_msg,
//                                                                int length_of_Sor);


