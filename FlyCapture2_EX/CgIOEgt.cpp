#include "StdAfx.h"
#include "CgIOEgt.h"
#include "resource.h"
#include "FlyCapture2_EXDlg.h"				//maindlg헤더파일을 include해야함.

CgIO::CgIO()
{

}

CgIO::~CgIO(void)
{
}

UINT CgIO::threadIO(LPVOID pParam)
{
	CgIO* pIO = (CgIO*)pParam;
	int Retval=0;

	while(pIO->m_bRead)
	{
		pIO->GetIOData();
		Sleep(0);
	}
	return Retval;
}

void CgIO::GetIOData()
{
	CFlyCapture2_EXDlg* pDlg = (CFlyCapture2_EXDlg*)m_pMain;

	ReadInput();
	pDlg->LoopIO(m_bInputData);
}

int CgIO::InitIO(LPVOID pParam,int nDeviceid)
{
	m_pMain = pParam;
	m_nDIODevice = nDeviceid;
	long nRet = EGIOT_Open(m_nDIODevice);

	if (1)
	{
		m_bRead = TRUE;
		CWinThread *pThread = ::AfxBeginThread(threadIO, this); // thread 생성
	}
	return nRet;
}

void CgIO::Destroy(void)
{
	m_bRead = FALSE;
	Sleep(100);
	EGIOT_Close(m_nDIODevice);
}

void CgIO::WriteOutputPort(int port, bool bValue)
{
	EGIOT_WriteOutputPort(m_nDIODevice, port, bValue);
}

void CgIO::WriteOutputPortPulse(int port,int ms)
{
	EGIOT_WriteOutputPort(m_nDIODevice, port, TRUE);
	Sleep(ms);
	EGIOT_WriteOutputPort(m_nDIODevice, port, FALSE);
}

int CgIO::ReadInputPort(int port)
{
	int nRet = EGIOT_ReadInputPort(m_nDIODevice, port);
	return nRet;
}

bool* CgIO::ReadInput()
{
	int nRet = EGIOT_ReadInput(m_nDIODevice);
	int mask = 0x01;
	for(int i=0;i<MAX_INPUT;i++)
	{
		if(mask<<i)	m_bInputData[i] = true;
		else		m_bInputData[i] = false;
	}
	return m_bInputData;
}