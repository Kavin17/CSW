// Axt 1.1


#pragma once
//#include "AxtLIB.h"
//#include "AxtDIO.h"
#include "AXD.h"
#include "AXL.h"
#define	 MAX_INPUT	8
//Lib load
//#pragma comment(lib, "AxtLib.lib")

#include "cdio.h"
#include "EGIOT.h"
#define	HANAIO

#ifdef HANAIO
#include "DLL_IOModule.h"
#endif
class CgIO
{
public:
	CgIO();
public:
	~CgIO(void);
protected:
	BOOL	m_bRead;
	LPVOID	m_pMain;
	BOOL	m_bFlag[MAX_INPUT];

public:
	static UINT threadIO(LPVOID pParam);
	static UINT threadIO2(LPVOID pParam);
	static UINT threadOut1(LPVOID pParam);
	static UINT threadOut2(LPVOID pParam);
	static UINT threadOut3(LPVOID pParam);
	static UINT threadOut4(LPVOID pParam);
	static UINT threadOut8(LPVOID pParam);//8호기
	static UINT threadOut7(LPVOID pParam);//8호기
	static UINT threadOut9(LPVOID pParam);//8호기
	short	m_nDIODevice;

	bool	m_bInputData[MAX_INPUT];
	int		m_nIOTime;

	int		m_nIOSelect;
	///하나텍 IO관련
	bool	m_bInputPC;
	BOOL	m_bOpen;

	/////

	BOOL InitIO(LPVOID pParam,int nDeviceid);
	void GetIOData();
	void GetIOData2();
	void Destroy(void);
	void WriteOutputPort(int port, bool bValue);
	void WriteOutputPortPulse(int port,int ms);
	int ReadInputPort(int port);
	bool* ReadInput(int nRet);
	BOOL	m_bGrabComplete;
	BOOL	m_bInspComplete;
	BOOL	m_bPgmAuto;

	int		m_nBadResult1;//1 hole ok 2hole ng 3
	int		m_nBadResult2;//1 tab ok 2tab ng 3

#ifdef	HANAIO
	HINSTANCE 	hinst;
	int (__stdcall* COM_Open)(int nPort, CallbackFunc* Callback, bool _bThreadUseFlag);
	bool (__stdcall* COM_Close)();
	bool (__stdcall* COM_IsOpened)();
	void (__stdcall* COM_Read)();
	void (__stdcall* Get_State_Info)();
	void (__stdcall* Set_DO_Port_Ctrl)(BYTE cPort, int _nOnOffCtrl);
	void (__stdcall* KeyCode)(unsigned char *_pData, unsigned char _cDataSize, unsigned char *_pBuf, PINT _cBufSize);
#else if 	
	static UINT threadIO(LPVOID pParam);
#endif


};
