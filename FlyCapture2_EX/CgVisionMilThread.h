// Vision.h: interface for the CVision class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VISION_H__BCDEE2C5_BA6F_475A_A4ED_BD3C30038CDC__INCLUDED_)
#define AFX_VISION_H__BCDEE2C5_BA6F_475A_A4ED_BD3C30038CDC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "mil.h"
//#include "LogData.h"	// Added by ClassView
#include "mil.h"
//#pragma comment(lib,"mil.lib")

#define MAX_BUFFER		TOTAL_FRAME_NUM	//2	//TOTAL_FRAME_NUM

//typedef struct UserDataStruct
//{
//	MIL_ID MilApplication;
//	MIL_ID MilSystem;
//	MIL_ID MilDigitizer[MAX_CAMERAS];
//	MIL_ID MilBuffer[MAX_CAMERAS][MAX_BUFFER];
//	MIL_ID MilImage[MAX_CAMERAS];
//	MIL_ID MilDisplay[MAX_CAMERAS];
////	MIL_ID MilClipImage;
//	int		i;
//};
//long MFTYPE GrabStart(long HookType, MIL_ID EventId,void *vMilData);
//long MFTYPE GrabEnd(long HookType, MIL_ID EventId,void *vMilData);
long MFTYPE ProcessingFunction1(long HookType, MIL_ID HookId, void MPTYPE *HookDataPtr);
long MFTYPE ProcessingFunction2(long HookType, MIL_ID HookId, void MPTYPE *HookDataPtr);

class CVision  
{
public:

	BOOL	m_bContinue;
	int		m_nCaminit[MAX_CAMERAS];
	int		m_nModifyFrame[MAX_CAMERAS];
	void ResetFrame();
	void Close();
//	void HookStop(int n);
	void GetExposureTime(int nCam,double* pNanoSec);
	void SetExposureTime(int nCam,int nNanoSec,int flow);
	unsigned char* GetProcessBuf(int nCam);
	void SingleGrab(int nBufferNum);
//	BOOL m_bBufIndex;
	void ChangeVerticalLine(int Line);
	void MakeImg();
	int m_nImgVCount;
	void InitOriImage();
	void SetTrigMode();
	void SetLiveMode();
	void HookStop(int nCam);
	void HookStart();
	unsigned char* GetImgPtr(int nCam,int nIndex);
	unsigned char* GetOriImgPtr(int nCam,int nFrameNo);
//	BOOL Open(HWND hWnd);
	BOOL Open(LPVOID pParam);
//	CVision(CWnd* pwnd=NULL);
	CVision();
	virtual ~CVision();
	MIL_ID MilApplication;//[MAX_CAMERAS];
	MIL_ID MilSystem[MAX_CAMERAS];
	MIL_ID MilDigitizer[MAX_CAMERAS];
	MIL_ID MilBuffer[MAX_CAMERAS][MAX_BUFFER];
	MIL_ID MilImage[MAX_CAMERAS];
//	MIL_ID MilClipImage;
	MIL_ID MilDisplay[MAX_CAMERAS];
	MIL_ID MilSaveBuf[MAX_CAMERAS];
	long m_nBand;
	long m_nSizeX;
	long m_nSizeY;
	LPVOID	m_pParent;	
	BOOL m_bOpen;
//	CWnd*	m_pParent;	
	void ImageClip(int nBufferNum);
	int m_nCrossCount;
	int m_nFrameCount;
	int	m_nGrabStats;
////////////////////////////////////////////////////	

	int		m_nBufIndexGrabStt[MAX_CAMERAS];
	int		m_nBufIndexGrabEnd[MAX_CAMERAS];
	int		m_nBufIndexCopy;
//	int		m_nGrabCount;
};

#endif // !defined(AFX_VISION_H__BCDEE2C5_BA6F_475A_A4ED_BD3C30038CDC__INCLUDED_)
