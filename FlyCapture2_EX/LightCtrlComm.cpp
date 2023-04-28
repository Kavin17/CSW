// LFineCont.cpp : implementation file
//

#include "stdafx.h"
#include "Afxmt.h"
#include "LightCtrlComm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// LFineCont

CLightCtrlComm::CLightCtrlComm()
{
	m_hwnd= NULL;
	m_bConnected = FALSE;
}

CLightCtrlComm::~CLightCtrlComm()
{
	if(m_bConnected) 
		Close();
}

#define MAXBUF	4096

static CEvent evtComKill;

UINT CLightCtrlComm::ComThread(LPVOID pParam) 
{
	TCHAR	RxData[MAXBUF];
	DWORD	ErrorFlag = NULL;
	DWORD	ReadBytes = NULL;
	COMSTAT	ComStat;

	CLightCtrlComm *LightCtrl = (CLightCtrlComm*)pParam;

	while (WaitForSingleObject(evtComKill.m_hObject, 10) != WAIT_OBJECT_0) 
	{
		ClearCommError(LightCtrl->m_ComHandle, &ErrorFlag, &ComStat);
		while(ComStat.cbInQue >= 10)					// PC에서 읽는 데이터 사이즈 (고정)
		{
			ReadFile(LightCtrl->m_ComHandle, RxData, ComStat.cbInQue, &ReadBytes, NULL);

			if(LightCtrl->m_hwnd)
				::SendMessage(LightCtrl->m_hwnd, WM_RECEIVEDATA, (UINT)RxData, ReadBytes);
			
			ClearCommError(LightCtrl->m_ComHandle, &ErrorFlag, &ComStat);
			if (LightCtrl->m_bConnected) 
				break;
		} 
	}
	return NULL;
}

void CLightCtrlComm::TxData(LPSTR TxChar, int len) 
{
	DWORD dwWriteBytes = NULL;
	if(m_bConnected) 
		WriteFile(m_ComHandle, (LPCTSTR)TxChar, len, &dwWriteBytes, NULL);
}

bool CLightCtrlComm::Open(CString strPort) 
{
	m_strPort = strPort;

	if(m_bConnected)
		Close();
	
	m_ComHandle = CreateFile(m_strPort, GENERIC_READ|GENERIC_WRITE, 
			NULL, NULL, OPEN_EXISTING,	FILE_ATTRIBUTE_NORMAL, NULL);
	if(m_ComHandle == INVALID_HANDLE_VALUE) 
		return false;
	
	DCB	dcb;
	GetCommState(m_ComHandle, &dcb);

	dcb.BaudRate = 19200;					// 9600으로 변경시 9600 Baudrate로 동작 
	dcb.ByteSize = 8;
	dcb.Parity = NOPARITY;
	dcb.StopBits = ONESTOPBIT;
	dcb.fNull = FALSE;	

	SetCommState(m_ComHandle, &dcb);

	DWORD evtMask = EV_RXCHAR|EV_TXEMPTY;
	SetCommMask(m_ComHandle, evtMask);
	
	m_bConnected = true;
	evtComKill.ResetEvent();
	m_pComThread = AfxBeginThread(ComThread, this);

	return true;
}

void CLightCtrlComm::Close() 
{
	m_bConnected=FALSE; 
	evtComKill.SetEvent();
	CloseHandle(m_ComHandle);
}

void CLightCtrlComm::WriteString( CString strString )
{
	DWORD dwWriteBytes = NULL;
	if(m_bConnected) 
		WriteFile(m_ComHandle, strString, strString.GetLength(), &dwWriteBytes, NULL);
}