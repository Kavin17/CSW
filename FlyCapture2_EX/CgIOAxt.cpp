#include "StdAfx.h"
#include "CgIOAxt.h"
#include "resource.h"
#include "FlyCapture2_EXDlg.h"				//maindlg헤더파일을 include해야함.

/* 
남경2호기
out
XC0	ready		out0
xc1	Run			out1
xc2	Trouble		out2
xc3	End			out3
xc4	Hole OK		out4
xc5	Hole NG		out5
xd4	tab ok		out6
xd5	tab ng		out7
in
trigger			in0
count reset		in1
Model change`	in2
*/

#define IOOUT_TIME		100
#ifdef HANAIO
bool CALLBACK Callback(BYTE *_pbyBuffer, DWORD _nNbyte);
#endif
CFlyCapture2_EXDlg* pMainDlg = NULL;

CgIO::CgIO()
{
	m_bGrabComplete = FALSE;
	m_bInspComplete = FALSE;
	m_nBadResult1 = m_nBadResult2 = 0;
	for(int i=0;i<MAX_INPUT;i++)
		m_bFlag[i] = FALSE;
	m_nIOTime = 100;
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
		Sleep(1);
	}
	return Retval;
}
UINT CgIO::threadIO2(LPVOID pParam)
{
	CgIO* pIO = (CgIO*)pParam;
	int Retval=0;

	while(pIO->m_bRead)
	{
		pIO->GetIOData2();
		Sleep(1);
	}
	return Retval;
}
UINT CgIO::threadOut1(LPVOID pParam)//Grab 완료
{
	CgIO* pIO = (CgIO*)pParam;
	CFlyCapture2_EXDlg* pDlg = (CFlyCapture2_EXDlg*)pIO->m_pMain;
	int Retval=0;

	while(pIO->m_bRead)//io ok
	{
		//Auto Check

		if(pIO->m_bPgmAuto)
		{
			if(pIO->m_bGrabComplete)
			{
				pIO->m_bGrabComplete = FALSE;
				/*pIO->WriteOutputPortPulse(3,(pDlg->m_pDlgSet->m_FormTab2->m_nIoDelay)-50);//Grab End woo*/
			}

		}
		Sleep(1);
	}
	return Retval;
}

UINT CgIO::threadOut3(LPVOID pParam)//start
{
	CgIO* pIO = (CgIO*)pParam;
	BOOL	bStats = FALSE;
	CFlyCapture2_EXDlg* pDlg = (CFlyCapture2_EXDlg*)pIO->m_pMain;

	static double dtt2=0;

	while(pIO->m_bRead)//io ok
	{
		//Auto Check

		if(pIO->m_bPgmAuto)
		{
			if(bStats == FALSE)
			{
				CString str1;
				double dt11 = GetTickCount();
				str1.Format("Pulse %.0f",dt11 - dtt2);
//				pDlg->AddtoIOLog(str1);
				dtt2 = dt11;
				bStats = TRUE;
				pIO->WriteOutputPort(1,1);////PGM ON	

			}
			else{
				bStats = FALSE;
				pIO->WriteOutputPort(1,0);////PGM ON
			}

			pDlg->Wait(500);
//			Sleep(500);
		}
		Sleep(5);
	}
	return 0;
}

UINT CgIO::threadOut7(LPVOID pParam)//start
{//오창8
	CgIO* pIO = (CgIO*)pParam;
	BOOL	bStats = FALSE;
	CFlyCapture2_EXDlg* pDlg = (CFlyCapture2_EXDlg*)pIO->m_pMain;//성호야 봐라 이거 추가했다 

	while(pIO->m_bRead)//io ok
	{
		//Auto Check
		if(pIO->m_bPgmAuto)
		{
			if (pDlg->m_pDlgSet->m_FormTab2->m_bCompleteOnOff==0)//성호야 봐라 이거 추가했다 
			{
				pIO->WriteOutputPort(2,1);////pgm ON
			}
			
			if(bStats == FALSE)
			{
				bStats = TRUE;
				pIO->WriteOutputPort(3,1);////auto ON
			}
			else{
				bStats = FALSE;
				pIO->WriteOutputPort(3,0);////auto ON
			}
			Sleep(500);
		}
		Sleep(5);
	}
	return 0;
}

UINT CgIO::threadOut8(LPVOID pParam)//start
{//오창8
/*
	#define DIO_INPUT_0				0	//해당 Trigger
	#define DIO_INPUT_2				2	//2 TRIGGER
	#define DIO_INPUT_3				3	//BYPASS

	#define DIO_OUTPUT_0			0	//NG	
	#define DIO_OUTPUT_1			1	//ok
	#define DIO_OUTPUT_2			2	//Program on
	#define DIO_OUTPUT_3			3	//Auto
	#define DIO_OUTPUT_4			4	//Error
	#define DIO_OUTPUT_5			5	//magnetic
	#define DIO_OUTPUT_6			6//
*/
	CgIO* pIO = (CgIO*)pParam;
	CFlyCapture2_EXDlg* pDlg = (CFlyCapture2_EXDlg*)pIO->m_pMain;
	CString strTemp;
	double dResultOutTime = 0.0;
	while(pIO->m_bRead)//io ok
	{
		if(pIO->m_bPgmAuto && pIO->m_bInspComplete)//stop  start
		{
			pIO->m_bInspComplete = FALSE;
			if(pIO->m_nBadResult1==2 || pIO->m_nBadResult2==2){
				pIO->WriteOutputPort(1,FALSE);// hole ok
				pIO->WriteOutputPort(0,TRUE);// hole ng
				//strTemp.Format("I/O NG Out(%.0f)",(d - pDlg->m_dJudgeTime)*1000.);
			}
			else if(pIO->m_nBadResult1==1 || pIO->m_nBadResult2==1)
			{
				pIO->WriteOutputPort(1,TRUE);// hole ok	
				pIO->WriteOutputPort(0,FALSE);// hole ng
			}
			//pDlg->AddtoInspLog(strTemp);
			
			//if (pDlg->m_pDlgSet->m_FormTab2->m_bIODuration == TRUE)
			//{
			//		Sleep(pDlg->m_pDlgSet->m_FormTab2->m_nIoDelay); //Delay이후에 신호를 다 죽임
			//		pIO->WriteOutputPort(1,FALSE);	
			//		pIO->WriteOutputPort(0,FALSE);
			//}
			pDlg->m_dTime[4] = pDlg->m_Project.GetPrecisionTime();
			dResultOutTime = (pDlg->m_dTime[4] - pDlg->m_dTime[3]) * 1000;
			if (dResultOutTime <= 0) dResultOutTime = 0;
			strTemp.Format(_T("IO Out_%d[%.1f]"), pDlg->m_nGrabCount, dResultOutTime);
			pDlg->AddtoInspLog1(strTemp, 1);

			Sleep(200);
			pIO->WriteOutputPort(0,FALSE);
			pIO->WriteOutputPort(1,FALSE);
		}
		Sleep(1);
	}
	return 0;
}


UINT CgIO::threadOut2(LPVOID pParam)//Result
{//남경
	CgIO* pIO = (CgIO*)pParam;
	CFlyCapture2_EXDlg* pDlg = (CFlyCapture2_EXDlg*)pIO->m_pMain;
	double dResultOutTime = 0.0;
	CString strTemp;
	int Retval = 0;
/*
		XC0	ready		out0
		xc1	Run			out1
		xc2	Trouble		out2
		xc3	End			out3
		xc4	Hole OK		out4
		xc5	Hole NG		out5
		xd4	tab ok		out6
		xd5	tab ng		out7

*/
	while(pIO->m_bRead)//io ok
	{
		if(pIO->m_bPgmAuto && pIO->m_bInspComplete)//stop  start
		{
			pIO->m_bInspComplete = FALSE;
			if (pIO->m_nBadResult1 == 1)
				pIO->WriteOutputPortPulse(4, pDlg->m_pDlgSet->m_FormTab2->m_nIoDelay);
			else if (pIO->m_nBadResult1 == 2)
				pIO->WriteOutputPortPulse(5, pDlg->m_pDlgSet->m_FormTab2->m_nIoDelay);
			if (pIO->m_nBadResult2 == 1)
				pIO->WriteOutputPortPulse(6, pDlg->m_pDlgSet->m_FormTab2->m_nIoDelay);
			else if (pIO->m_nBadResult2 == 2)
				pIO->WriteOutputPortPulse(7, pDlg->m_pDlgSet->m_FormTab2->m_nIoDelay);
			pIO->WriteOutputPortPulse(3, pDlg->m_pDlgSet->m_FormTab2->m_nIoDelay);//Grab End woo

			 //Sharpness Alram
			if(pDlg->m_Project.m_dSharpness < 100)
				pIO->WriteOutputPortPulse(2, pDlg->m_pDlgSet->m_FormTab2->m_nIoDelay);

			 //Bright Alram
			if (pDlg->m_nLight < 200)
				pIO->WriteOutputPortPulse(3, pDlg->m_pDlgSet->m_FormTab2->m_nIoDelay);

			pIO->m_nBadResult1 = pIO->m_nBadResult2 = 0;

			pDlg->m_dTime[4] = pDlg->m_Project.GetPrecisionTime();
			dResultOutTime = (pDlg->m_dTime[4] - pDlg->m_dTime[3]) * 1000;
			if (dResultOutTime <= 0) dResultOutTime = 0;
			strTemp.Format(_T("IO Out_%d[%.1f]"), pDlg->m_nGrabCount, dResultOutTime);
			pDlg->AddtoInspLog1(strTemp, 1);
		}
		Sleep(1);
	}
	return Retval;
}
/*  오창 pilot 21700
0	pgm  on
1	run pulse
2	trouble
3	end
4	hole ok
5	hole ng
6	tab ok
7	tab ng

*/
UINT CgIO::threadOut4(LPVOID pParam)//Result
{//오창
	CgIO* pIO = (CgIO*)pParam;
	int Retval=0;

	while(pIO->m_bRead)//io ok
	{
		if(pIO->m_bPgmAuto && pIO->m_bInspComplete)//stop  start
		{
			pIO->m_bInspComplete = FALSE;
			if(pIO->m_nBadResult1==1)
				pIO->WriteOutputPort(2,TRUE);//ok
			else if(pIO->m_nBadResult1==2)
				pIO->WriteOutputPort(3,TRUE);//ng
			if(pIO->m_nBadResult2==1)
				pIO->WriteOutputPort(4,TRUE);//ok
			else if(pIO->m_nBadResult2==2)
				pIO->WriteOutputPort(5,TRUE);//ng

			pIO->m_nBadResult1 = pIO->m_nBadResult2=0;

			Sleep(pIO->m_nIOTime);
			pIO->WriteOutputPort(2,FALSE);
			pIO->WriteOutputPort(3,FALSE);
			pIO->WriteOutputPort(4,FALSE);
			pIO->WriteOutputPort(5,FALSE);
		}
		Sleep(1);
	}
	return Retval;
}

void CgIO::GetIOData()
{
	CFlyCapture2_EXDlg* pDlg = (CFlyCapture2_EXDlg*)m_pMain;
	ReadInput(0);
	pDlg->LoopIO(m_bInputData);
	Sleep(1);
}
void CgIO::GetIOData2()
{
	CFlyCapture2_EXDlg* pDlg = (CFlyCapture2_EXDlg*)m_pMain;
	int nret = ReadInputPort(8);
	DWORD dw = GetTickCount();
	int ntime = 500;
	CString strtime;
	if(nret && pDlg->m_pDlgSet->m_FormTab4->m_bHoleInspect )
	{
		int nret = ReadInputPort(8);
		while(nret)
		{
			nret = ReadInputPort(8);
			pDlg->Wait(5);
			int nret1 = ReadInputPort(9);
			if(GetTickCount()-dw>ntime)
			{	
				WriteOutputPortPulse(8,50);
//				WriteOutputPortPulse(2,50);//trouble
				strtime.Format("Terminate Cylinder. %.0f S1:%d S2:%d",double(GetTickCount()-dw),nret1,nret);
				pDlg->AddtoList("Cylinder Sensor Check",0);
				pDlg->AddtoList(strtime,0);
				nret = 0;
			}
		}		
	}
}
#ifdef HANAIO
bool CALLBACK Callback(BYTE *_pbyBuffer, DWORD _nNbyte)
{
//	static int nc = 0;

	CString strTemp,strTemp1,strTemp2;
	for(DWORD i=0; i<_nNbyte;i++)
	{
		strTemp1.Format(_T("[%02X]"), _pbyBuffer[i]);
		strTemp = strTemp + strTemp1;
	}
	strTemp2.Format("Trigger %d",_pbyBuffer[0]);
	//pMainDlg->AddtoIOLog(strTemp2);

	if(_pbyBuffer[0] == 0) return TRUE;
 	if(atoi(strTemp2) == 128)
 		pMainDlg->m_IO.m_bInputPC = true;
 	else
 		pMainDlg->m_IO.ReadInput(atoi(strTemp2));
//	if(nc %2==0)
 		pMainDlg->LoopIO(pMainDlg->m_IO.m_bInputData);
//	nc++;
	return true;
}
#endif

int CgIO::InitIO(LPVOID pParam,int nDeviceid)
{
	m_pMain = pParam;
	m_nDIODevice = nDeviceid;

//	m_nIOSelect = 1;
	//long nRet = EGIOT_Open(m_nDIODevice);
	CFlyCapture2_EXDlg* pDlg = (CFlyCapture2_EXDlg*)m_pMain;
	pMainDlg = pDlg;
	int nRet = 0;
	int  nmaxport= 4;

	if(pDlg->m_nMachine==8)
	{
		m_nIOSelect = 1;//4
// 		if(m_nIOSelect==1){
// //			nRet = DioInit(_T("DIO000"), &m_nDIODevice);
// //			if(nRet!=0)	return 0;
// 		}
// 		else if(m_nIOSelect==2){
// //			nRet = EGIOT_Open(m_nDIODevice);
// //			if(nRet==0)		return 0;
// 		}
// 		
// 		else if(m_nIOSelect==4)
		{
#ifdef	HANAIO
			hinst = LoadLibraryA("DLL_IOModule.dll");

			COM_Open  = (int (__stdcall*)(int   nPort, CallbackFunc* Callback, bool _bThreadUseFlag))GetProcAddress(hinst, "COM_Open");
			COM_Close = (bool (__stdcall*)())GetProcAddress(hinst, "COM_Close");
			COM_IsOpened  = (bool (__stdcall*)())GetProcAddress(hinst, "COM_IsOpened");
			COM_Read = (void (__stdcall*)())GetProcAddress(hinst, "COM_Read");
			Get_State_Info = (void (__stdcall*)())GetProcAddress(hinst, "Get_State_Info");
			Set_DO_Port_Ctrl = (void (__stdcall*)(BYTE cPort, int _nOnOffCtrl))GetProcAddress(hinst, "Set_DO_Port_Ctrl");
			KeyCode = (void (__stdcall*)(unsigned char *_pData, unsigned char _cDataSize, unsigned char *_pBuf, PINT _cBufSize))GetProcAddress(hinst, "KeyCode");

			bool	bUseThreadFlag = true;
			if(COM_Open(5, Callback, bUseThreadFlag) != 1)
				nRet=0;
			else
				nRet=1;	
#endif

		}

	}
	else
	{
		m_nIOSelect = 3;//ajin
		nmaxport = 8;
		DWORD Code = AxlOpenNoReset(7);
		if ( Code == AXT_RT_SUCCESS)
		{
			TRACE("Library is initialized .\n");
			DWORD dwStatus;
			Code = AxdInfoIsDIOModule(&dwStatus);
			if(dwStatus == STATUS_EXIST)
			{
				TRACE("DIO module exists.\n");
				long IModuleCounts;
				Code = AxdInfoGetModuleCount(&IModuleCounts);
				if(Code == AXT_RT_SUCCESS)
					TRACE("Number of DIO module: %d\n",IModuleCounts);
				else
					TRACE("AxdInfoGetModuleCount() : ERROR code Ox%x\n",Code);

				long IInputCounts;
				long IOutputCounts;
				long IBoardNo;
				long IModulePos;
				DWORD dwModuleID;

				CString strData;
				for(int ModuleNo=0; ModuleNo < IModuleCounts; ModuleNo++)
				{
					AxdInfoGetInputCount(ModuleNo, &IInputCounts);
					AxdInfoGetOutputCount(ModuleNo, &IOutputCounts);
					if(AxdInfoGetModule(ModuleNo, &IBoardNo, &IModulePos, &dwModuleID) == AXT_RT_SUCCESS)
					{
						switch(dwModuleID)
						{						
						case AXT_SIO_DI32:
							strData.Format("[BD No:%d - MD No:%d] SIO_DI32", IBoardNo, ModuleNo);
							break;
						case AXT_SIO_RDI32:
							strData.Format("[BD No:%d - MD No:%d] SIO_RDI32", IBoardNo, ModuleNo);
							break;
						case AXT_SIO_DO32P:
							strData.Format("[BD No:%d - MD No:%d] SIO_DO32P", IBoardNo, ModuleNo);
							break;
						case AXT_SIO_DB32P:
							strData.Format("[BD No:%d - MD No:%d] SIO_DB32P", IBoardNo, ModuleNo);
							break;
						case AXT_SIO_DO32T:
							strData.Format("[BD No:%d - MD No:%d] SIO_DO32T", IBoardNo, ModuleNo);
							break;
						case AXT_SIO_RDO32:
							strData.Format("[BD No:%d - MD No:%d] SIO_RDO32", IBoardNo, ModuleNo);
							break;
						case AXT_SIO_DB32T:
							strData.Format("[BD No:%d - MD No:%d] SIO_DB32T", IBoardNo, ModuleNo);
							break;
						case AXT_SIO_RDB128MLII:
							strData.Format("[BD No:%d - MD No:%d] SIO_RDB128MLII", IBoardNo, ModuleNo);
							break;
						case AXT_SIO_RSIMPLEIOMLII:
							strData.Format("[BD No:%d - MD No:%d] SIO_RSIMPLEIOMLII", IBoardNo, ModuleNo);
							break;
						case AXT_SIO_RDI16MLII:
							strData.Format("[BD No:%d - MD No:%d] SIO_RDI16MLII", IBoardNo, ModuleNo);
							break;
						case AXT_SIO_RDO16AMLII:
							strData.Format("[BD No:%d - MD No:%d] SIO_RDO16AMLII", IBoardNo, ModuleNo);
							break;
						case AXT_SIO_RDO16BMLII:
							strData.Format("[BD No:%d - MD No:%d] SIO_RDO16BMLII", IBoardNo, ModuleNo);
							break;
						case AXT_SIO_RDB96MLII:
							strData.Format("[BD No:%d - MD No:%d] SIO_RDB96MLII", IBoardNo, ModuleNo);
							break;
						case AXT_SIO_RDO32RTEX:
							strData.Format("[BD No:%d - MD No:%d] SIO_RDO32RTEX", IBoardNo, ModuleNo);
							break;
						case AXT_SIO_RDI32RTEX:
							strData.Format("[BD No:%d - MD No:%d] SIO_RDI32RTEX", IBoardNo, ModuleNo);
							break;
						case AXT_SIO_RDB32RTEX:
							strData.Format("[BD No:%d - MD No:%d] SIO_RDB32RTEX", IBoardNo, ModuleNo);
							break;
						case AXT_SIO_DI32_P:
							strData.Format("[BD No:%d - MD No:%d] SIO_DI32_P", IBoardNo, ModuleNo);
							break;
						case AXT_SIO_DO32T_P:
							strData.Format("[BD No:%d - MD No:%d] SIO_DO32T_P", IBoardNo, ModuleNo);
							break;
						case AXT_SIO_RDB32T:
							strData.Format("[BD No:%d - MD No:%d] SIO_RDB32T", IBoardNo, ModuleNo);
							break;
						}
					}
				}
			}
			else
			{
				AfxMessageBox("Module not exist.");
				return FALSE;
			}
		}
		else
		{
//			if(pDlg->m_strComname.Find("CH")<0)
//				AfxMessageBox("Failed initialization.");
			return FALSE;
		}
	}

	m_bRead = TRUE;
	m_bPgmAuto=FALSE; 

	//io init

	for(int i=0;i<nmaxport;i++)
		WriteOutputPort(i,FALSE);
	if( pDlg->m_nMachine!=8)
		CWinThread *pThread = ::AfxBeginThread(threadIO, this); // thread 생성 read 용

	if(pDlg->m_strComname.Find("PI")>=0)
	{
		::AfxBeginThread(threadOut1, this); // 남경 Grab complete	out 3
		::AfxBeginThread(threadOut2, this); // 남경					out 4/5/6/7
		::AfxBeginThread(threadOut3, this); // Start boutton		out 1

	}
	else
	{
		if(pDlg->m_nMachine==1)//1 오창  3남경1호 2남경2
		{
			::AfxBeginThread(threadOut3, this); // Start boutton			out 1
			::AfxBeginThread(threadOut4, this); // 오창						out 2/3/4/5
		}
		else if(pDlg->m_nMachine==8)//8오창8호기
		{
			::AfxBeginThread(threadOut8, this); // result ok/ng				out 1/0
			::AfxBeginThread(threadOut7, this); // pgm on start				out 2/3
		}
		else if(pDlg->m_nMachine==2)
		{
			::AfxBeginThread(threadOut1, this); // 남경 Grab complete	out 3
			::AfxBeginThread(threadOut2, this); // 남경					out 4/5/6/7
			::AfxBeginThread(threadOut3, this); // Start boutton		out 1
		}
		else
		{
			CWinThread *pThread2 = ::AfxBeginThread(threadIO2, this); // Sensor check

			::AfxBeginThread(threadOut1, this); // 남경 Grab complete
			::AfxBeginThread(threadOut2, this); // 남경
			::AfxBeginThread(threadOut3, this); // Start boutton
		}
	}

	return m_bRead;
}

void CgIO::Destroy(void)
{
	m_bRead = FALSE;
	Sleep(100);
	for(int i=0;i<8;i++)
		WriteOutputPort(i,FALSE);
	//if(m_nIOSelect==2)
	//	EGIOT_Close(m_nDIODevice);
	if(m_nIOSelect==3)
	{
		if (AxlIsOpened())		AxlClose();
	}
	else if(m_nIOSelect==4)
		if(m_bOpen)
			COM_Close();

}

void CgIO::WriteOutputPort(int port, bool bValue)
{
	//EGIOT_WriteOutputPort(m_nDIODevice, port, bValue);

	//	DIOwrite_outport(port,  bValue);
	// 출력(Output) 포트로에 1비트의 데이터를 써넣는다. 지정한 모듈의 점수 단위
	//AxdoWriteOutportBit(m_nDIODevice, port, bValue);
// 	if(m_nIOSelect==1)
// 		DioOutBit(m_nDIODevice, port, bValue);
// 	else if(m_nIOSelect==2)
// 		EGIOT_WriteOutputPort(m_nDIODevice, port, bValue);
// 	else if(m_nIOSelect==3)
// 		AxdoWriteOutport(port,bValue);
// 	else if(m_nIOSelect==4)
	if(m_nIOSelect==3){
		AxdoWriteOutport(port,bValue);
	}
	else
	{
		BYTE 	cPort = 0x00;
		if(port == 0) cPort = 0x00;
		if(port == 1) cPort = 0x01;
		if(port == 2) cPort = 0x02;
		if(port == 3) cPort = 0x03;
		Set_DO_Port_Ctrl(cPort, bValue);
	}
//	pMainDlg->m_pDlgSet->m_FormTab5->m_bOut[port] = bValue;

	//	DIOwrite_outport_bit(m_nDIODevice, port,  bValue);

}

void CgIO::WriteOutputPortPulse(int port,int ms)
{
// 	if(m_nIOSelect==1)
// 		DioOutBit(m_nDIODevice, port, TRUE);
// 	else if(m_nIOSelect==2)
// 		EGIOT_WriteOutputPort(m_nDIODevice, port, TRUE);
// 	else if(m_nIOSelect==3)
// 		AxdoWriteOutportBit(m_nDIODevice, port, TRUE);
// //	pMainDlg->m_pDlgSet->m_FormTab5->m_bOut[port] = TRUE;

// 	Sleep(ms);
// 	if(m_nIOSelect==1)
// 		DioOutBit(m_nDIODevice, port, FALSE);
// 	else if(m_nIOSelect==2)
// 		EGIOT_WriteOutputPort(m_nDIODevice, port, FALSE);
// 	else if(m_nIOSelect==3)
// 		AxdoWriteOutportBit(m_nDIODevice, port, FALSE);
// 	else if(m_nIOSelect==4)
	if(m_nIOSelect==3){
		AxdoOutPulseOn(0, port, ms);
		/*Sleep(ms);
		AxdoWriteOutportBit(m_nDIODevice, port, FALSE);*/
	}
	else
	{
		WriteOutputPort(port, TRUE);
		Sleep(ms);
		WriteOutputPort(port, FALSE);
	}
	//pMainDlg->m_pDlgSet->m_FormTab5->m_bOut[port] = FALSE;


}

int CgIO::ReadInputPort(int port)
{
	int nRet = 0;//EGIOT_ReadInputPort(m_nDIODevice, port);
	DWORD	dwValue1;
	BYTE data = 0;
// 	if(m_nIOSelect==1){
// 		DioInpBit(m_nDIODevice,port,&data);
// 		nRet = data;
// 	}
// 	else if(m_nIOSelect==2)
// 		nRet = EGIOT_ReadInputPort(m_nDIODevice,port);
// 	else if(m_nIOSelect==3)
// 	{
// 		AxdiReadInport( port, &dwValue1);
// 		nRet = dwValue1;
// 	}
	return nRet;
}

bool* CgIO::ReadInput(int nRet)
{
	//	int nRet = DIOread_inport_bit(m_nDIODevice,0);	// SIO-DI32:0..31, SIO-DB32:0..15, SIO-DO32:사용불가
	DWORD	dwValue1,dwValue2,dwValue3,dwValue4,dwValue5,dwValue6,dwValue7,dwValue8,dwValue9,dwValue10;
	BYTE	data[8];//1,data2,data3,data4,data5,data6,data7,data8;
	CFlyCapture2_EXDlg* pDlg = (CFlyCapture2_EXDlg*)pMainDlg;
	
	if(m_nIOSelect==3)
	{
		AxdiReadInport( 0, &dwValue1);
		AxdiReadInport( 1, &dwValue2);
		AxdiReadInport( 2, &dwValue3);
		AxdiReadInport( 3, &dwValue4);
		AxdiReadInport( 4, &dwValue5);
		AxdiReadInport( 5, &dwValue6);
		AxdiReadInport( 6, &dwValue7);
		AxdiReadInport( 7, &dwValue8);
		AxdiReadInport( 8, &dwValue9);
		AxdiReadInport( 9, &dwValue10);

		m_bInputData[0] = dwValue1;
		m_bInputData[1] = dwValue2;
		m_bInputData[2] = dwValue3;

		CFlyCapture2_EXDlg* pDlg = (CFlyCapture2_EXDlg*)m_pMain;

		m_bInputData[0] = dwValue1;//
		m_bInputData[1] = dwValue2;//
		m_bInputData[2] = dwValue3;//
		m_bInputData[3] = dwValue4;//
		m_bInputData[4] = dwValue5;//
		m_bInputData[5] = dwValue6;//
		m_bInputData[6] = dwValue7;//
		m_bInputData[7] = dwValue8;//
	}
	else if(m_nIOSelect == 1)
	{
// 		for(int i=0;i<8;i++)
// 			DioInpBit(m_nDIODevice,i,&data[i]);
// 		m_bInputData[0] = data[0];//
// 		m_bInputData[1] = data[1];//
// 		m_bInputData[2] = data[2];//
// 		pDlg->m_pDlgSet->m_FormTab5->m_bIn[0] = data[0];
// 		pDlg->m_pDlgSet->m_FormTab5->m_bIn[1] = data[1];
// 		pDlg->m_pDlgSet->m_FormTab5->m_bIn[2] = data[2];
// 		pDlg->m_pDlgSet->m_FormTab5->m_bIn[3] = data[3];
// 		pDlg->m_pDlgSet->m_FormTab5->m_bIn[4] = data[4];
// 		pDlg->m_pDlgSet->m_FormTab5->m_bIn[5] = data[5];
// 		pDlg->m_pDlgSet->m_FormTab5->m_bIn[6] = data[6];
// 		pDlg->m_pDlgSet->m_FormTab5->m_bIn[7] = data[7];
	}
	else if(m_nIOSelect==2)
	{
// 		for(int i=0;i<8;i++)
// 			m_bInputData[i]=EGIOT_ReadInputPort(m_nDIODevice,i);

// 		pDlg->m_pDlgSet->m_FormTab5->m_bIn[0] = m_bInputData[0];
// 		pDlg->m_pDlgSet->m_FormTab5->m_bIn[1] = m_bInputData[1];
// 		pDlg->m_pDlgSet->m_FormTab5->m_bIn[2] = m_bInputData[2];
// 		pDlg->m_pDlgSet->m_FormTab5->m_bIn[3] = m_bInputData[3];
// 		pDlg->m_pDlgSet->m_FormTab5->m_bIn[4] = m_bInputData[4];
// 		pDlg->m_pDlgSet->m_FormTab5->m_bIn[5] = m_bInputData[5];
// 		pDlg->m_pDlgSet->m_FormTab5->m_bIn[6] = m_bInputData[6];
// 		pDlg->m_pDlgSet->m_FormTab5->m_bIn[7] = m_bInputData[7];
	}
	else if(m_nIOSelect==4)
	{
		int mask = 0x01;
		for(int i=0;i<MAX_INPUT;i++)
		{
			if(nRet==i)//&(mask<<i)){
			{
				m_bInputData[i] = true;
				//pDlg->m_pDlgSet->m_FormTab5->m_bIn[i] = TRUE;
			}
			else{
				m_bInputData[i] = false;
				//pDlg->m_pDlgSet->m_FormTab5->m_bIn[i] = FALSE;
			}
		}
	}
	return m_bInputData;
}