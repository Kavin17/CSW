// Vision.cpp: implementation of the CVision class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CgVisionMilThread.h"

#include "resource.h"			// view에 접근하려면 해줘야함
#include "FlyCapture2_EXDlg.h"	// view에 접근하려면 해줘야함
//#include "VisWeb.h"
//#include "VisWebDoc.h"
//#include "VisWebView.h"
//#include "Vision.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
extern int m_nMaxFrameCount;


CVision::CVision()
{
	//m_pParent = pwnd;
	m_nBufIndexGrabStt[0]=0;
	m_nBufIndexGrabStt[1]=0;

	m_nBufIndexGrabEnd[0]=0;
	m_nBufIndexGrabEnd[1]=0;

	m_nBufIndexCopy   =0;
//	m_pLogProcess = new CLogData(PATH_LOG_GRABPROCESS);
//	m_pLogProcess->SetLogWriteEnable(FALSE);
	m_nCrossCount =0;
	m_nFrameCount = 0;
	m_bOpen = FALSE;
	m_nGrabStats = 0;
	m_nCaminit[0] = -2;
	m_nCaminit[1] = -2;
	m_bContinue = FALSE;
}

CVision::~CVision()
{
//	delete m_pLogProcess;
/*
	MdispFree(MilDisplay);
	for(int i=0; i<MAX_BUFFER; i++)
		MbufFree(MilBuffer[i]);
	MbufFree(MilSaveBuf);
	MbufFree(MilImage);
	MbufFree(MilClipImage);
	MbufFree(MilDisplay);
	MdigFree(MilDigitizer);
	
	MsysFree(MilSystem);
	MappFree(MilApplication);
	*/
	Close();
}

BOOL CVision::Open(LPVOID pParam)
{
	m_bOpen=TRUE;
	m_pParent = pParam;
	CFlyCapture2_EXDlg *pView = (CFlyCapture2_EXDlg*)m_pParent;
	//CVisWebDoc *pDoc = (CVisWebDoc*)pView->GetDocument();

	// 188Line -> 133Line
	MappAlloc(M_DEFAULT, &MilApplication);

	for(int j=0;j<MAX_CAMERAS;j++)
	{

//		MsysAlloc(M_SYSTEM_SOLIOS,  M_DEFAULT, M_DEFAULT, &MilSystem[j]);	
		MsysAlloc(M_SYSTEM_SOLIOS,  j, M_DEFAULT, &MilSystem[j]);	
		if(j==0){
			if(M_NULL == MdigAlloc(MilSystem[j], M_DEV0, "C:\\Glim\\Glim1.dcf", M_DEFAULT, &MilDigitizer[j])){
				m_bOpen = FALSE;
				return m_bOpen;
			}
			MdispAlloc(MilSystem[j], M_DEV0, "M_DEFAULT", M_WINDOWED, &MilDisplay[j]);	

		}
		else if(j==1)
		{
			if(M_NULL == MdigAlloc(MilSystem[j], M_DEV0, "C:\\Glim\\Glim2.dcf", M_DEFAULT, &MilDigitizer[j])){
				m_bOpen = FALSE;
				return m_bOpen;
			}
			MdispAlloc(MilSystem[j], M_DEV0, "M_DEFAULT", M_WINDOWED, &MilDisplay[j]);	

		}
		MdigControl(MilDigitizer[j], M_GRAB_TIMEOUT, M_INFINITE);
		// CC1 Output를 정의해 주지 않으면, CC출력이 되지 않음. 반드시 추가해 주어야 함.
		MdigControl(MilDigitizer[j], M_CAMERALINK_CC1_SOURCE, M_GRAB_EXPOSURE+M_TIMER1);
		MdigControl(MilDigitizer[j], M_GRAB_MODE, M_ASYNCHRONOUS);
		m_nSizeX = CAMERA_WIDTH;
		m_nSizeY = CAMERA_LINE;
		m_nBand = 8;
		MbufAlloc2d(MilSystem[j], m_nSizeX, m_nSizeY, 8+M_UNSIGNED, M_IMAGE+M_DISP+M_GRAB, &MilImage[j]);

		for(int i=0; i<MAX_BUFFER; i++)
		{
			MbufAlloc2d(MilSystem[j], m_nSizeX, m_nSizeY, 8+M_UNSIGNED, M_IMAGE+M_GRAB+M_DISP, &MilBuffer[j][i]); //!!
			//		MbufClear(MilBuffer[i], 0xff);
		}

		MdigInquire(MilDigitizer[j], M_SIZE_BAND, &m_nBand);
		MdigInquire(MilDigitizer[j], M_SIZE_X, &m_nSizeX);
		MdigInquire(MilDigitizer[j], M_SIZE_Y, &m_nSizeY);

		MbufAlloc2d(MilSystem[j], m_nSizeX, m_nSizeY, 8+M_UNSIGNED, M_IMAGE+M_DISP, &MilImage[j]);

		MbufClear(MilImage[j], 0);
		//	MbufClear(MilClipImage, 0);

//		MdispAlloc(MilSystem, M_DEV0, "M_DEFAULT", M_WINDOWED, &MilDisplay);	

	}

	MappControl(M_ERROR , M_PRINT_DISABLE);
	
  	return m_bOpen;
}

/*
long MFTYPE GrabStart(long HookType, MIL_ID EventId, void *DataStruct)
{
	CVisWebView *pView = (CVisWebView*)DataStruct;
	CVision		*pVision = pView->GetDocument()->m_pVision;

	pVision->m_nBufIndexGrabStt++;

	pVision->SingleGrab(pVision->m_nBufIndexGrabStt%MAX_BUFFER) ;
	return 0;
}

long MFTYPE GrabEnd(long HookType, MIL_ID EventId, void *DataStruct)
{
	CVisWebView *pView = (CVisWebView*)DataStruct;
	CVisWebDoc *pDoc = (CVisWebDoc*)pView->GetDocument();

	CVision		*pVision = pView->GetDocument()->m_pVision;
	
	if(!pView->m_bAuto)		//Auto 모드가 아니면 촬상을 끝낸다
	{
		pVision->m_nBufIndexGrabStt = 0;
		pVision->m_nBufIndexGrabEnd = 0;
		pVision->m_nBufIndexCopy	= 0;
		return 0;
	}

	pVision->m_nBufIndexGrabEnd++;
	pView->CopyMilImg();	// 이거 수정

	return 0;
}
*/
unsigned char* CVision::GetImgPtr(int nCam,int nIndex)
{
	unsigned char * pImage;
	pImage = (unsigned char*)MbufInquire(MilBuffer[nCam][nIndex], M_HOST_ADDRESS, M_NULL);
	return pImage;
}


void  CVision::ImageClip(int index)
{
//	MbufCopyClip(MilBuffer[m_nBufIndexCopy%MAX_BUFFER],MilClipImage,0,256*index);
}


unsigned char* CVision::GetOriImgPtr(int nCam,int nFrameNo)
{
	unsigned char * pImage=NULL;

	return pImage;
}


void CVision::HookStart()
{
//	CVisWebView *pView = (CVisWebView*)m_pParent;


//	m_nBufIndexGrabStt=0;
//m_nMaxFrameCount
	m_nGrabStats = 1;
	for(int i=0;i<MAX_CAMERAS;i++)
	{
		if(i==0)
			MdigProcess(MilDigitizer[i], MilBuffer[i], MAX_BUFFER, 
				M_SEQUENCE+M_COUNT(m_nModifyFrame[0]+1 ), M_ASYNCHRONOUS+M_TRIGGER_FOR_FIRST_GRAB,MIL_BUF_HOOK_FUNCTION_PTR(ProcessingFunction1), (void*)m_pParent); //First Frame 트리거 사용시
		else if(i==1)
			MdigProcess(MilDigitizer[i], MilBuffer[i], MAX_BUFFER, 
				M_SEQUENCE+M_COUNT(m_nModifyFrame[1]+1 ), M_ASYNCHRONOUS+M_TRIGGER_FOR_FIRST_GRAB,MIL_BUF_HOOK_FUNCTION_PTR(ProcessingFunction2), (void*)m_pParent); //First Frame 트리거 사용시
	}
//	MdigProcess(MilDigitizer, MilBuffer, MAX_BUFFER, 
//		M_SEQUENCE+M_COUNT(m_nMaxFrameCount+1 ), M_ASYNCHRONOUS+M_TRIGGER_FOR_FIRST_GRAB,MIL_BUF_HOOK_FUNCTION_PTR(ProcessingFunction), (void*)m_pParent); //First Frame 트리거 사용시

}

void CVision::HookStop(int nCam)
{
//	CVisWebView *pView = (CVisWebView*)m_pParent;

/*	MdigHookFunction(MilDigitizer, M_GRAB_START+M_UNHOOK, GrabStart,(void*)m_pParent);	
	MdigHookFunction(MilDigitizer, M_GRAB_END+M_UNHOOK, GrabEnd,(void*)m_pParent);	
	MdigControl(MilDigitizer, M_GRAB_ABORT, M_DEFAULT);
	MdigProcess(MilDigitizer, MilGrabBufferList, MilGrabBufferListSize,
		M_STOP, M_DEFAULT, ProcessingFunction, this);
*/


	m_nBufIndexGrabStt[nCam] = 0;
	if(nCam==0)
		MdigProcess(MilDigitizer[nCam], MilBuffer[nCam], MAX_BUFFER,M_STOP, M_DEFAULT, MIL_BUF_HOOK_FUNCTION_PTR(ProcessingFunction1), (void*)m_pParent);
	else if(nCam==1)
		MdigProcess(MilDigitizer[nCam], MilBuffer[nCam], MAX_BUFFER,M_STOP, M_DEFAULT, MIL_BUF_HOOK_FUNCTION_PTR(ProcessingFunction2), (void*)m_pParent);
//	MdigProcess(MilDigitizer, MilBuffer, MAX_BUFFER,M_STOP, M_DEFAULT, ProcessingFunction, (void*)m_pParent);
		
//	m_nBufIndexGrabStt=0;
//	m_nBufIndexGrabEnd=0;
//	m_nBufIndexCopy   =0;
}
//
//void CVision::HookStop(int n)
//{
////	CVisWebView *pView = (CVisWebView*)m_pParent;
//	//if(!m_bOpen)
//	//	return;
//	//if(n==1)
//	//{
//	//	for(int i=0;i<MAX_CAMERAS;i++)
//	//		MdigControl(MilDigitizer[i], M_GRAB_ABORT, M_DEFAULT);
//	//}
//
//	//m_nBufIndexGrabStt=0;
//	//m_nBufIndexGrabEnd=0;
//	//m_nBufIndexCopy   =0;
//}

void CVision::SetLiveMode()
{

 }

void CVision::SetTrigMode()
{
}

void CVision::InitOriImage()
{

}

void CVision::MakeImg()
{//Copy MIL Image -> CImage

}
unsigned char* CVision::GetProcessBuf(int nCam)
{
	unsigned char* fm;
//	fm = GetImgPtr((m_nBufIndexGrabStt+MAX_BUFFER)%MAX_BUFFER);
	fm = GetImgPtr(nCam,m_nBufIndexGrabStt[nCam]%MAX_BUFFER);
//	fm = GetImgPtr((m_nBufIndexGrabStt-1+MAX_BUFFER)%MAX_BUFFER);
	m_nBufIndexCopy++;
	return fm;
}
	
void CVision::ChangeVerticalLine(int Line)
{

}
//extern int g_frame_num;
void CVision::SingleGrab(int nBufferNum)
{

}


void CVision::SetExposureTime(int nCam,int nNanoSec,int flow)
{
	static int nOldShutter[2] ={0,0};// 0;
	if(nOldShutter[nCam] != nNanoSec || flow == 1)
	{
		MdigControl(MilDigitizer[nCam], M_GRAB_EXPOSURE_TIME, nNanoSec);
//		MdigControl(MilDigitizer, M_GRAB_EXPOSURE_TIME_DELAY, nDelay);

	}
	nOldShutter[nCam] = nNanoSec;
}

void CVision::GetExposureTime(int nCam, double *pNanoSec)
{
	MdigInquire(MilDigitizer[nCam], M_GRAB_EXPOSURE_TIME, pNanoSec);
}

///////////////////////////////////////////////////////////////////////////////////

void CVision::Close()
{
	if(!m_bOpen)
		return;
	for(int j=0;j<MAX_CAMERAS;j++)
	{
		MdispFree(MilDisplay[j]);
		for(int i=0; i<MAX_BUFFER; i++)
			MbufFree(MilBuffer[j][i]);
		MbufFree(MilSaveBuf[j]);
		MbufFree(MilImage[j]);
		//	MbufFree(MilClipImage);
		MbufFree(MilDisplay[j]);
		MdigFree(MilDigitizer[j]);
		MsysFree(MilSystem[j]);
	}
	MappFree(MilApplication);
	

}

void CVision::ResetFrame()
{
	m_nBufIndexGrabStt[0]=0;
	m_nBufIndexGrabStt[1]=0;

	m_nBufIndexGrabEnd[0]=0;
	m_nBufIndexGrabEnd[1]=0;
	m_nBufIndexCopy   =0;
}

long MFTYPE ProcessingFunction1(long HookType, MIL_ID HookId, void MPTYPE *HookDataPtr)
{
	CFlyCapture2_EXDlg* pView =(CFlyCapture2_EXDlg*)HookDataPtr;
	CVision		*pVision = &pView->m_Vision;

//	MdigGetHookInfo(HookId, M_MODIFIED_BUFFER+M_BUFFER_ID, &pVision->MilBuffer[pVision->m_nBufIndexGrabStt%MAX_BUFFER]);//MilImage);
	MdigGetHookInfo(HookId, M_MODIFIED_BUFFER+M_BUFFER_ID, &pVision->MilBuffer[0][pVision->m_nBufIndexGrabStt[0]%MAX_BUFFER]);//MilImage);

	if(pVision->m_nBufIndexGrabStt[0]>=pView->m_pDlgSet->m_FormTab2->m_nFrameLength)//TOTAL_FRAME_NUM)
//	if(pVision->m_nBufIndexGrabStt>=m_nMaxFrameCount)
	{//
//		pView->AddtoList("Reset");
//		pVision->ResetFrame();
		pVision->m_nBufIndexGrabStt[0]=0;
//		return 0;
	}	

	pView->LoopGrab(0,pVision->GetProcessBuf(0));	// 이거 수정

	if(pVision->m_nBufIndexGrabStt[0] >= pView->m_pDlgSet->m_FormTab2->m_nFrameLength-1)//	if(pVision->m_nBufIndexGrabStt == m_nMaxFrameCount-1)
	{
		pView->AddtoList("Stop");
		if(!pVision->m_bContinue)
			pVision->HookStop(0);
		if(pView->m_pDlgSet->m_FormTab5->m_bBufferClear)
			for(int k=0;k<MAX_BUFFER;k++)
				MbufClear(pVision->MilBuffer[0][k], 0);

		pVision->m_nBufIndexGrabStt[0]=-1;
	}
	
	pVision->m_nBufIndexGrabStt[0]++;

//	g_frame_num++;
	return 1;
}

long MFTYPE ProcessingFunction2(long HookType, MIL_ID HookId, void MPTYPE *HookDataPtr)
{
	CFlyCapture2_EXDlg* pView =(CFlyCapture2_EXDlg*)HookDataPtr;
	CVision		*pVision = &pView->m_Vision;

	//	MdigGetHookInfo(HookId, M_MODIFIED_BUFFER+M_BUFFER_ID, &pVision->MilBuffer[pVision->m_nBufIndexGrabStt%MAX_BUFFER]);//MilImage);
	MdigGetHookInfo(HookId, M_MODIFIED_BUFFER+M_BUFFER_ID, &pVision->MilBuffer[1][pVision->m_nBufIndexGrabStt[1]%MAX_BUFFER]);//MilImage);

	if(pVision->m_nBufIndexGrabStt[1]>=pView->m_pDlgSet->m_FormTab2->m_nFrameLength)//TOTAL_FRAME_NUM)
		//	if(pVision->m_nBufIndexGrabStt>=m_nMaxFrameCount)
	{//
//		pView->AddtoList("Reset");
		//		pVision->ResetFrame();
		pVision->m_nBufIndexGrabStt[1]=0;
		//		return 0;
	}	

	pView->LoopGrab(1,pVision->GetProcessBuf(1));	// 이거 수정

//	memset(pVision->MilBuffer[1][pVision->m_nBufIndexGrabStt[1]%MAX_BUFFER],0,CAMERA_WIDTH*CAMERA_LINE);
	if(pVision->m_nBufIndexGrabStt[1] >= pView->m_pDlgSet->m_FormTab2->m_nFrameLength-1)//	if(pVision->m_nBufIndexGrabStt == m_nMaxFrameCount-1)
	{
		pView->AddtoList("Stop & Buffer Free");
		if(!pVision->m_bContinue)
			pVision->HookStop(1);
		if(pView->m_pDlgSet->m_FormTab5->m_bBufferClear)
			for(int k=0;k<MAX_BUFFER;k++)
				MbufClear(pVision->MilBuffer[1][k], 0);

		pVision->m_nBufIndexGrabStt[1]=-1;
	}

	pVision->m_nBufIndexGrabStt[1]++;

	//	g_frame_num++;
	return 1;
}
