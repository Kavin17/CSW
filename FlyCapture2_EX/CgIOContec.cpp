#include "StdAfx.h"
#include "CgIOContec.h"
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

BOOL CgIO::InitIO(LPVOID pParam,int nDeviceid)
{
	m_pMain = pParam;
	m_nDIODevice = nDeviceid;
	long nRet = DioInit("DIO000", &m_nDIODevice);
	BOOL bRet = FALSE;

	if (nRet==0)
	{
		bRet = m_bRead = TRUE;
		CWinThread *pThread = ::AfxBeginThread(threadIO, this); // thread 생성
	}
	return bRet;
}

void CgIO::Destroy(void)
{
	m_bRead = FALSE;
	Sleep(100);
	DioExit(m_nDIODevice);
}

void CgIO::WriteOutputPort(int port, bool bValue)
{
	DioOutBit(m_nDIODevice, port, bValue);
}

void CgIO::WriteOutputPortPulse(int port,int ms)
{
	DioOutBit(m_nDIODevice, port, TRUE);
	Sleep(ms);
	DioOutBit(m_nDIODevice, port, FALSE);
}

int CgIO::ReadInputPort(int port)
{
	BYTE data = 0;
	int nRet = DioInpBit(m_nDIODevice, port,&data);
	return data;
}

bool* CgIO::ReadInput()
{
	BYTE data = 0;
	int nRet = DioInpByte(m_nDIODevice,0,&data);
	int mask = 0x01;
	for(int i=0;i<MAX_INPUT;i++)
	{
		if((mask<<i)&data)	m_bInputData[i] = true;
		else				m_bInputData[i] = false;
	}
	return m_bInputData;
}