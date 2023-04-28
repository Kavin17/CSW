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
#define E_ALREADYOPEN				E_SERIAL_BASE_ERROR + 1		// Serial Open할 경우 이미 열려있을 경우 에러 코드
#define E_SETTIMEOUT				E_SERIAL_BASE_ERROR + 2		// TimeOut 시간 설정 에러
#define E_INVALIDBAUDRATE			E_SERIAL_BASE_ERROR + 3		// 지원하지 않는 BAUDRATE
#define E_SERIALCONFSET				E_SERIAL_BASE_ERROR + 4		// Serial Configuration 설정 에러
#define E_MAINPROCEVENT				E_SERIAL_BASE_ERROR + 5		// 데이타 수신시 수신 이벤트를 받을 이벤트 핸들이 없음
#define E_FILECREATE				E_SERIAL_BASE_ERROR + 6		// 포트 오픈 에러
#define E_PORTINVALID				E_SERIAL_BASE_ERROR + 7		// Invalid Port
#define E_CLASSCREATE				E_SERIAL_BASE_ERROR + 8		// Serial Class 객체 생성 에러
#define E_THREADCREATE				E_SERIAL_BASE_ERROR + 9		// Serial Class 의 쓰레드 생성 에러
#define E_TOOLARGEDATA				E_SERIAL_BASE_ERROR + 10	// 한번에 전송할 데이터가 Linked List 한개의 버퍼보다 큰 에러
#define E_BUFFER_ALLOC				E_SERIAL_BASE_ERROR + 11	// 버퍼가 없음
#define E_CRC_CHECK					E_SERIAL_BASE_ERROR + 12	// CRC 체크시 이상 발생
#define E_CHECKSUM_CHECK			E_SERIAL_BASE_ERROR + 13	// CheckSum 체크시 이상 발생

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

extern  HANDLE              m_hRecvEndEvt;                  // 수신 쓰레드 종료 이벤트
extern  HANDLE              m_hRecvEvt;                     // 데이터 수신 이벤트

extern  OVERLAPPED          m_osWrite;
extern  OVERLAPPED          m_osRead;

extern  HANDLE              m_hHeapRecv_Main;					// RECV COMM THREAD HEAP


//-----------------------------------------------------------------
// 프로세스간 통신 메시지 공통 구조체 정의
typedef struct tagRS_COMM_MSG
{
	BYTE				cTransCode;								// 메시지 코드
	BYTE				cData[512];							    // 전송되는 데이터
} RS_COMM_MSG, *PRS_COMM_MSG;


//-----------------------------------------------------------------
// 상태정보 응답
typedef struct tagRS_STATE_INFO_RESP_MSG
{
	BYTE				cDO_Ctrl_Sts;							// DO 제어 상태 정보 (0~7bit별 DO 상태(0:꺼짐, 1:켜짐))
	BYTE				cDI_Ctrl_Sts;							// DI 제어 상태 정보 (0~7bit별 DI 상태(0:꺼짐, 1:켜짐))
	BYTE				cTrigger_Event;							// 외부 인터럽트 개수(트리거 시점)
	BYTE				cTrigger_Direction;						// 외부 인터럽트 하강(0), 상승(1)
	BYTE				cWatchdog;							    // 0: 미사용, 1: 사용
	BYTE				cKey_Lock;							    // 0: 미사용, 1: Glim, 2: 미설정, ~~
	BYTE				cTrigger_Pulse;							// 트리거 펄스 하강(0), 상승(1)
	BYTE				cTrigger_Time;							// 트리거 시간 ms
} RS_STATE_INFO_RESP_MSG, *PRS_STATE_INFO_RESP_MSG;


//-----------------------------------------------------------------
//
typedef struct tagRS_DO_CTRL_REQ_MSG
{
	BYTE				cDO_Port;
    BYTE				cOnOff_Ctrl;
} RS_DO_CTRL_REQ_MSG, *PRRS_DO_CTRL_REQ_MSG;


//-----------------------------------------------------------------
// DI 이벤트
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
/*  Communication에서 처음으로 Setting 한다.                               */
/*-------------------------------------------------------------------------*/
//extern "C" __declspec(dllexport) void __stdcall DCB_Clear();


/*-------------------------------------------------------------------------*/
/* Function COM_Open()                                                     */
/* 통신 포트를 Open한다.                                                   */
/* Window NT Comm Port Open                                                */
/* Return Value : 1:OK, else:NG                                            */
/*-------------------------------------------------------------------------*/
extern "C" __declspec(dllexport) int __stdcall COM_Open(int _nPort,
                                                        CallbackFunc* _Callback,
                                                        bool _bThreadUseFlag);


/*-------------------------------------------------------------------------*/
/* Function COM_Close()                                                    */
/* Open되오 있는 Comm Port Close                                           */
/* Return Value : 1:OK, else:NG                                            */
/*-------------------------------------------------------------------------*/
extern "C" __declspec(dllexport) bool __stdcall COM_Close();


/*-------------------------------------------------------------------------*/
/* Function IsOpened()                                                     */
/* 통신 Port Open상태를 Return 한다.                                       */
/* Port Open Check                                                         */
/* Return true, false                                                      */
/*-------------------------------------------------------------------------*/
extern "C" __declspec(dllexport) bool __stdcall COM_IsOpened();


/*-------------------------------------------------------------------------*/
/* Function COM_Read()                                                     */
/* 통신 Port로 부터 들어오는 값을 읽는다.                                  */
/*-------------------------------------------------------------------------*/
extern "C" __declspec(dllexport) void __stdcall COM_Read();


extern "C" __declspec(dllexport) int __stdcall COM_ReadEx(int _nInputNo);

/*-------------------------------------------------------------------------*/
/* Function fnReleaseFrame()                                               */
/* 수신된 데이터의 프로토콜 적합 여부를 검사한다.                          */
/* pMsgBuf   : 수신된 데이터 중 정상 데이터를 필터링한 후 저장하는 버퍼    */
/* pMsgSize  : pMsgBuf 버퍼 사이즈                                         */
/* pRecvBuf  : 수신된 데이터 버퍼                                          */
/* nRecvSize : pRecvBuf 버퍼 사이즈                                        */
/*-------------------------------------------------------------------------*/
int __stdcall fnReleaseFrame(PBYTE pMsgBuf, PINT pnMsgSize, PBYTE pRecvBuf, int nRecvSize);


/*-------------------------------------------------------------------------*/
/* Function fnProcRecvData()                                               */
/* 수신데이터는 fnReleaseFrame에서 필터링을 거친 후                        */
/* fnProcRecvData로 전달됩니다.                                            */
/*-------------------------------------------------------------------------*/
void __stdcall fnProcRecvData(PBYTE pMsgBuf, int nMsgSize);


/*-------------------------------------------------------------------------*/
/* Function fnState_Resp_Proc()                                            */
/* 상태정보 수신 데이터를 구조체에 저장한다.                               */
/*-------------------------------------------------------------------------*/
void __stdcall fnState_Resp_Proc(PRS_COMM_MSG _pRs_Comm_Msg);


/*-------------------------------------------------------------------------*/
/* Function fnTrigger_Event_Proc()                                         */
/* 트리거 이벤트 수신 시 해당 이벤트를 콜백함수로 내보낸다.                */
/*-------------------------------------------------------------------------*/
void __stdcall fnTrigger_Event_Proc();


/*-------------------------------------------------------------------------*/
/* Function COM_Write()                                                    */
/* 통신 Port로 자료를 보낸다.                                              */
/*-------------------------------------------------------------------------*/
bool __stdcall COM_Write(BYTE *_pbyBuffer, DWORD _nNbyte);


/*-------------------------------------------------------------------------*/
/* Function Get_RxBufferSize()                                             */
/* 통신 Port에 들어와 있는 Rx Count를 읽어온다.                            */
/* Return : 수신버퍼에 있는  Rx Count                                      */
/*-------------------------------------------------------------------------*/
extern "C" __declspec(dllexport) DWORD __stdcall Get_RxBufferSize();


/*-------------------------------------------------------------------------*/
/* Function Get_TxBufferSize()                                             */
/* 통신 Port에 들어와 있는 Rx Count를 읽어온다.                            */
/* Return : 수신버퍼에 있는  Rx Count                                      */
/*-------------------------------------------------------------------------*/
//extern "C" __declspec(dllexport) DWORD __stdcall Get_TxBufferSize();


extern "C" __declspec(dllexport) void __stdcall Get_State_Info();
//extern "C" __declspec(dllexport) void __stdcall Set_DO_Port_Ctrl(BYTE _cPort);

extern "C" __declspec(dllexport) void __stdcall Set_DO_Port_Ctrl(BYTE _cPort, int _nOnOffCtrl);


/*-------------------------------------------------------------------------*/
/* Function KeyCode()                                                      */
/* 입력받은 문자열로KeyCode를 생성한 후 리턴한다.                          */
/*-------------------------------------------------------------------------*/
extern "C" __declspec(dllexport) void __stdcall KeyCode(unsigned char *_pData,
                                                        unsigned char _cDataSize,
                                                        unsigned char *_pBuf,
                                                        PINT _cBufSize);

/*-------------------------------------------------------------------------*/
/* Function Make_1Byte_BCC()                                               */
/* 보낼자료를 Check Sum할 수 있게 XOR 만 시킨다.                           */
/* XOR한 값이 0x20보다 작은 경우 0x20를 Add하지 않는다.                    */
/*-------------------------------------------------------------------------*/
//extern "C" __declspec(dllexport) void __stdcall Make_1Byte_BCC(BYTE *_pbyTxBuffer,
//                                                               int _nMakeLength);


/*-------------------------------------------------------------------------*/
/* Function Check_1Byte_BCC()                                              */
/* 받은 자료를 Check Sum을 하여 받은 Check Sum 값하고 비교 한다.           */
/* XOR 만 시킨다.                                                          */
/* XOR한 값이 0x20보다 작은 경우 0x20를 Add하지 않는다.                    */
/*-------------------------------------------------------------------------*/
//extern "C" __declspec(dllexport) bool __stdcall Check_1Byte_BCC(BYTE *_pbyRxBuffer,
//                                                                int _nCheckLength);


/*-------------------------------------------------------------------------*/
/* Function Ascii_To_HexStr()                                              */
/* Ascii 를 2 Byte Hex로 만든다.                                           */
/*-------------------------------------------------------------------------*/
//extern "C" __declspec(dllexport) void __stdcall Ascii_To_HexStr(BYTE *Ret_msg,
//                                                                BYTE *Sor_msg,
//                                                                int length_of_Sor);


/*-------------------------------------------------------------------------*/
/* Function HexStr_To_Ascii()                                              */
/* Hex String을 1Byte Ascii로 만든다.                                      */
/*-------------------------------------------------------------------------*/
//extern "C" __declspec(dllexport) void __stdcall HexStr_To_Ascii(BYTE *Ret_msg,
//                                                                BYTE *Sor_msg,
//                                                                int length_of_Sor);


