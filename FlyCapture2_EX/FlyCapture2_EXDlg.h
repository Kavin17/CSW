// FlyCapture2_EXDlg.h : 헤더 파일
//
#include "Label1.h"
#include "afxwin.h"
#include "DlgSet.h"
#include "DlgAiView.h"
#include <Afxmt.h>
#include "afxext.h"
#include "BtnST.h"
#include "cttext.h"
#include "CgDisplay.h"
#include "CgProject.h"
#include "CgMulti.h"
#include "CgMulti.h"
#include "Connect.h"
#include "afxbutton.h"

//IO카드--------------------------------------
//#include "CgIOEgt.h"
//#include "CgIOContec.h"
#include "CgIOAxt.h"
//--------------------------------------------


//카메라---------------------------------------
//#include "CgVisionMil.h"
//#include "CgVisionMilThread.h"
//#include "CgVisionFly.h"
//#include "CgVisionGrab.h"
//#include "CgVisionJai.h"
#include "GigECamera.h"

//---------------------------------------------

#include "WinSerial.h"	// Added by ClassView


#include "DlgCode.h"

#include "LightMap.h"
#include "PassWord.h"

#include "CpuUsage.h"

//#include "ViewMain.h"

#include  "ModelCount.h"

#include "CameraDisplay.h"

#include "DrawBadState.h"

#include "LoadVTECSDll.h"


#pragma once
#include "CellData.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/filewritestream.h"
//////////////////////////////////////////
#define	MAX_PITCH		200
#define	MAX_VIEW_DATA	8

//define Color

#define DIO_INPUT_0				0
#define DIO_INPUT_1				1
#define DIO_OUTPUT_0			0
#define DIO_OUTPUT_1			1

#define	MAX_BAD_INFO			6
#define	MAX_SAVE_IMG			100


typedef struct OK_NG_Param
{
	int scale;
	double cap_canX;
	double cap_canY_lower;
	double cap_canY_upper;
	double tab_corner_gapY;
	double weld_canY_lower;
	double weld_canY_upper;
	double weld_capY_lower;
	double weld_capY_upper;
	double tccenter_tabedgeY;
	double tccenter_tabedgeX_left;
	double tccenter_tabedgeX_right;
	double strongwelding_area_min;
	double strongwelding_length_min;
	double soot_area_min;
	double soot_length_min;
	double welding_length_min;
	double welding_width_min;
	
}OK_NG_Param;

typedef struct CSW_Param
{
	int measureOKNG;
	double	from_TCcenter_to_CANcetner_y;
	double	from_TCcenter_to_YJcetner_y;
	double	YJ_length;
	double	from_TCcenter_to_TABleft_x;
	double	from_TCcenter_to_TABright_x;
	double	from_TCcenter_to_CANcenter_x;
	double	from_TCcenter_to_YJcenter_x;
	double	from_TCcenter_to_TABupper_y;
	double	from_YJcenter_to_Can_y;
	double  from_TABright_to_TABleft_y;
	double	YJ_width;
	double	TCcenter_x;
	double	TCcenter_y;
	double	CANcenter_x;
	double	CANcenter_y;
	double	TABcenter_x;
	double	TABcenter_y;
	double	TABlefttop_x;
	double	TABlefttop_y;
	double	TABrighttop_x;
	double	TABrighttop_y;
	double	YJcenter_x;
	double	YJcenter_y;
	double	CANleft_x;
	double	CANleft_y;
	double	CANright_x;
	double	CANright_y;
	int	isTC;
	int	isTC_frontback;
	int	isTC_slatned;
	int	isTC_CID;
	int	isCAN;
	int	isTAB;
	int	isTABerror;
	int	isETCerror;
	int	isYJ_fromTAB;
	int	isYakYJ;
	int	isGwaYJ;
	int	isGeuYJ;
}CSW_Param;

typedef struct return_params
{
	int judgeResult;				// OK, NG 판정결과
	OK_NG_Param okNgParams;
	CSW_Param cswParams;
	unsigned char* overlayImageData;
	int	imageWidth;
	int imageHeight;
	int imageBit;

} ReturnParams;

typedef int(*fp_DAP_Initialize_Func)();
typedef int(*fp_DAP_Inference_Func)(unsigned char*, int, int, BOOL, ReturnParams*, const char*, const char*);
typedef int(*fp_DAP_Release_Func)();


namespace LOADVTECSDll_GROUP
{
#pragma pack(1)
	struct stDllStatus
	{
		bool bIsConnected;
		bool bInitialzed;

		stDllStatus()
		{
			bIsConnected = false;
			bInitialzed = false;
		}

		BOOL operator!=(const stDllStatus& Input)
		{
			return 0 != memcmp(this, &Input, sizeof(*this));
		}

		stDllStatus& operator=(const stDllStatus& Input)
		{
			memcpy(this, &Input, sizeof(stDllStatus));
			return *this;
		}
	};
#pragma pack()
}

using namespace LOADVTECSDll_GROUP;


// CFlyCapture2_EXDlg 대화 상자
class CFlyCapture2_EXDlg : public CDialog
{
//	DECLARE_DYNAMIC(CFlyCapture2_EXDlg)

// 생성입니다.
public:
	CFlyCapture2_EXDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.


	virtual ~CFlyCapture2_EXDlg();

// 대화 상자 데이터입니다.
//	enum { IDD = IDD_FLYCAPTURE2_EX_DIALOG };
	enum { IDD = IDD_FLYCAPTURE2_EX_DIALOG_2CH };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	afx_msg LPARAM OnReceive(WPARAM wParam, LPARAM lParam);
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CConnect*	 m_pSocHunterlapTop;
	bool m_bConnectionStauts[7];
public:
	/////////////////Graph
	bool MergeSocketData(CString strSocketData, CString& strMergedData);
	void CFlyCapture2_EXDlg::ReadCamTimeRate(int nCam);
	void CFlyCapture2_EXDlg::WriteTimeCount(int nCam);//0 1 2
	int CFlyCapture2_EXDlg::SepparateString(CString strinput, int * ncnt , int * tmpdata);
	int		m_nTimeCount[24][10];//time //0ok	1ng	2notcap 3tab	4hole 5blur 6dist	
	int		m_nHour;//현재 시간
	int		m_nHourOld;
	int		m_nMinute;
	BOOL	m_bTestMode;
	CRect	m_rtWeldBlob;
	int		m_nMaxThFrame[2];//0 blur 1 strong
	int	m_nTabTopEdge;
	int	m_nTabBottomLeft;
	int	m_nTabBottomRight;
	int	m_nTabBottomTop;
	double	m_dTabBottomSize[2];
	double	m_dMidWeldV;
	double	m_dCanDistY;

	int		m_nMorphCount[5];
	int		m_nWeakWeld;//약용접 개수
	int		m_nWhiteWeldCheck; // 용접 유무 확인 Pixel
	double  m_dJudgeTime;

	
	int		m_nTotalCount[3][10];//cam  //count //0ok	1ng	2tab	3hole 4blur 5dist	6notcap 

	CPoint m_ptWeldCenterPos;//용접 중심 위치

	int		m_nWeldCount;//용접의 white개수

	int	CFlyCapture2_EXDlg::CopyTimeFile(int nCam);
	///////////////////
	int		m_nPcName;
	int	m_nTopCapCenterBrightness;
	int		m_nTopCapCheck;
	int m_nMaxFrame;
	int m_nMinus;
	BOOL	m_bTesting;
	BOOL	m_bStarting;
	CString m_strVersion;
	int		m_nFrameCheck;

	double m_dLastTime;
	int nHour1;
	CDrawBadState* mDrawBadState[3];
	CString m_strFolderName;
	CString m_strFileName;
	int		m_nMakeAlphachannel[10];
	int CFlyCapture2_EXDlg::TestNew(int nCam);
	void CFlyCapture2_EXDlg::ResetData();

	int		m_nSelectFrame;
	BOOL	m_bHoleBad[2][2];//left/ right , blur / hole 
	int		m_nTopCapV;//topcap brightness
	int CFlyCapture2_EXDlg::MakeAlphaImage(BYTE * fmsrc,BYTE * fmdst,int nFrame);

	int m_nType;
	///////////LGC
	int		m_nCanBlackCount;
	double m_dTabDist[2];
	int		m_nResetCount;//plc data reeset
	int		m_nTimeReset;
	void CFlyCapture2_EXDlg::WriteTimeRate();
	void CFlyCapture2_EXDlg::WriteDailyData();
	void CFlyCapture2_EXDlg::ReadTimeRate();
	void CFlyCapture2_EXDlg::DataReset();
	void CFlyCapture2_EXDlg::WriteTextData(int nresult);
	void CFlyCapture2_EXDlg::WriteTextData1(int nresult, int nDLResult);
	void CFlyCapture2_EXDlg::WriteID(int nCount, BOOL bResult, CString strCellID, CString strLotID);
	void CFlyCapture2_EXDlg::SetSystemTimePrivilege();
	void CFlyCapture2_EXDlg::ResetNgCnt();
	int		m_nCalLine[2];
	BOOL	m_bCaptureSaving;
	CEvent m_eSaveCapture;
	CEvent m_evFullSave;
	CEvent m_evNgSave;
	CString	m_strResultValue[4];
	CgDisplay m_DisplayBad[5];	
	CgDisplay m_DisplayNgCrop;	
	
	CgDisplay m_SaveNG;	
	CgDisplay m_DisplayDLOverlay;

	void CFlyCapture2_EXDlg::DrawTime();
	void CFlyCapture2_EXDlg::BadNgCount(int nn, BOOL bNgRB, BOOL bNgDL);
	void CFlyCapture2_EXDlg::NgPosition(BOOL status);
	int CFlyCapture2_EXDlg::reCheckHole(int nindex,CRect rt);
	int		m_nBadTapCount;
	int		m_nBadHoleCount;
	
	//0: cap유무 
	//1: Tap(Tab top, Tab-Cap L, Tab-Cap R)
	//2: Cap(Tab-Can X, Tab-Can Y, Cap접힘)
	//3: Weld(Strong)	
	//4: Weld(Weak)	
	//5: Weld(그을음)	
	//6: AI
	int		m_nBadCount[10];
	int		m_nDLBadCount[4]; // cap, tab, weld, dist
	int		m_nDLNgType;
//hour
	int		m_nBadHole[24];	//홀불량
	int		m_nBadEdge[24];	//에지불량
	int		m_nBadPos[24];	//위치불량
	int		m_nBadexistence[24];	//미부착Bad
	int		m_nBadArea[24];
	int		m_nBadNotBead[24];	//미용접
	
	BOOL m_bNgPosition[50];

	//약용접
	int		m_nWeakWeldTotalCnt;
	int		m_nWeakWeldNgCnt;

	void CFlyCapture2_EXDlg::WriteProfileLog(int nindex,int posx,int count,int *nprofile,CString strdata);
	int CFlyCapture2_EXDlg::CheckMachine();
	int		m_nMachine; // 1 : 오창, 3 : 남경1,  2 : 남경2~6 
	int		m_nGrabCount;
	int		m_nComleteNgCnt;

	int		m_nLight;
	int		m_nCanCheckAve;
	int		m_nCapCheckAve;
	CgIO	m_IO;
	double	m_dTimeRate[24];
	double	m_dDailyRate[31];

	void CFlyCapture2_EXDlg::BadInfo();
	void CFlyCapture2_EXDlg::InitBadInfo();

	CString m_strBadInfo[5][2];//0-time 1 bad name
	CString m_strBadInfo1[2];//0-time 1 bad name
	int	m_nIoCount;

	int CFlyCapture2_EXDlg::judgeTap();

	void CFlyCapture2_EXDlg::DisplayNgCrop(int nCam, int nNgType, CString strNgName, int nNgCountType, CPoint ptPos);
	int CFlyCapture2_EXDlg::judgeHole();
	//capture
	void CFlyCapture2_EXDlg::capture();
	void CFlyCapture2_EXDlg::capture2();
	HANDLE CFlyCapture2_EXDlg::DDBToDIB(CBitmap &bitmap, DWORD dwCompression, CPalette *pPal);
	bool CFlyCapture2_EXDlg::WriteDIB(CString szFile, HANDLE hDIB);
	CRect m_rtSaveRect;
	////////////////////
	int		m_nWaitResult;
	int		m_nIsSheet;//제품유무
	int	m_nInspStats[100];
	void CFlyCapture2_EXDlg::AlarmMode(int count);
	int		m_nSelectLight;
	int		m_bSheetCount;
	int		m_nInspCount;
	BOOL	m_bInsp[100];
	int		m_nResult[100];
	BOOL	m_bResult[100];
	double m_dData[100];
	CString m_strAIData;
	CString m_strHour;
	int m_nReworkCount;
	void CFlyCapture2_EXDlg::AlarmView(int count);
	int	m_nRetryCount;
	CString m_strRetryCode[10];
	int	m_nBrightModel;//1빍은 제품 2 어두운 제품
	int	m_nSelectCam;
	int m_nSendMes;
	CString m_strymd;
	CString m_strMesLot;
	void CFlyCapture2_EXDlg::WriteDetailLog(CString strdata);
	int m_nGrabComplete;
	CRect m_rtInsp;
	int	m_nFullFrameCount;
	int CycleGrab();
	void	BufCopy1();
	int		m_nCheckCam[MAX_CAMERAS];
	BOOL	m_bMono[MAX_CAMERAS];

	CCameraDisplay	*m_CamDisplay;

	CLightMap	*m_LightMap;
	BOOL	m_bCamStats[MAX_CAMERAS]; // Camera 연결 상태. True : 연결, False : X

	//CComPort m_cComm[12];

	int m_nPort;//9    // 장치 관리자 포트 10번
	int fnSetPort() ;
	double	m_CurrentElectric[12];
	double	m_Voltage[12];
	double	m_Electric[12];
	//	void SetLightPower(int nport,char cCh,char cOnOff);
	//	double ReadLightData(char cCommand,char cChannel,char data,int nport);
	double CFlyCapture2_EXDlg::SetLightData(char cCommand,char cChannel,char data,int nport);

	void CFlyCapture2_EXDlg::SetCurrentData(int nport,char cCommand,char cChannel,char data) ;
	double CFlyCapture2_EXDlg::SetLightLevel(char cChannel,int value);

	CCpuUsage	usageA;
	CCpuUsage	usageB;
	CCpuUsage	usageC;
	int		m_nSkipCount[MAX_CAMERAS];
	int		m_nMasterMode;//0 init 1 running
	CString	m_strCandidate[MAX_CAMERAS];
	int		m_nReworkAlarmCount;
	int		m_nRWFrameCount;
	int		m_nRWOkCount;
	int		m_nRWNgCount;
	BOOL	m_bRework;
	int		m_nCandiCount[MAX_CAMERAS];
	void	HddAlarm(int n);
	int		m_nAlarmCnt;
	BOOL	m_bCheckHdd;
	int		m_nLimiteTime;
	int		m_nIoStats[8];
	int		m_nBufCount[2];
	DWORD	m_dwWite;
	double	m_dLastWriteTime;
	CString		ComputerName();
	CString		ComputerUserName();
	void	AddtoListCPU(CString strData);
	void	WriteInspLogC(CString name,CString str);
	void	WriteInspLogD(CString name,CString str);
	void	WriteInspLog(CString name,CString str);
	void	AddtoInspLog(CString strData,BOOL bWrite=TRUE);
	void	AddtoInspLog1(CString strData,BOOL bWrite=TRUE);
	void	WriteMesLog(CString str);
	//void	WriteMilSecLog(CString str,BOOL bStartEnd);
	int		m_nStartTime;
	int		m_nStartGrabTime;
	int		m_nStartInspTime;
	int		m_nStartInspPartTime;
	void	StopInsp();
	CString m_strModelType;
	int		m_nInspMode;//0old // 1new
	void	fnWriteEndFile();
	int		m_nPlcCnt;
	CString GetPgmVersion();
	void	WriteFolderData(int nCam,CString path);
	int		m_nIOLimitTime;
	CString	m_strComname;
	void	fnReset();
	int		m_nPackCount[15];//삼성내수	삼성수출	탕정향직납	천안향단프라	천안향직납	LGD	LGD직납용기	탕정향R/W
	void	fnWriteMesendFile(CString strendtime,CString striscode,CString strnewlot,int framecnt,int nngcount,int mescount,CString strtype);
	int		m_nSendIO;
	int		m_nOKNG[MAX_CAMERAS];
	void	SetNumLock( BOOL bState );
	BOOL	m_bSendPlc;
	CString m_strMesEndTime;
	int m_nModifyCount;
	BOOL Getini(LPCSTR stSECTION, LPCSTR stKEY, LPCSTR stInIFile,LPSTR retVALUE);
	//highway101 연결 문자
	char g_ConnectString[30];
	char g_ChannelPrefix[30];
	int		m_nSaveCount;
	int		m_nSAveResult;//0 ok 1 ocv fail , 2 marking , 4 overlap  , 3 1+2
	CString m_strModelName;
	CEvent	m_evInsp[MAX_CAMERAS];	//@chcho
	BOOL	m_bInspection;
	void	BufCopy(int nCam);
	int		m_nNewCount;
	void CheckKeyMode();
	int		m_nInch;
	int		m_nInspMinute;
	CTime	m_StartTimer;
	CTime	m_EndTimer;
	int		CheckMes(int check);
	double	m_dTactTimeAve[3];
	int		m_nZoomSize;
	void	 StartInsp();

	//CLIENT IP, NAME
	char g_IP[20];
	char g_ClientName[20];
	char g_HostName[20];
	char g_Factory[10];
	char g_UserName[20];
	char g_Language;
	char g_Owner[10];
	char g_result;
	long g_Timeout;
	CModelCount		*m_nModelCount;
//	CViewMain	*m_MainView;

	CPassWord *m_pDlgPassword;
	CDlgCode	*m_DlgBarcode;
	CString m_strLot;//barcode
	void     InitSocket(void); //소켓 추가
	void	 DeleteAllFile(CString strDir);
	void	 SetAiSpec();
	int		Test(int nCam);
	BOOL	ImageShiftBad();
	int		m_nShowSet;
	int		getIsFocus();
	CString	m_strIsCode;
	CString m_strUpperLow;//상판 하판
	BOOL	m_bSaveImg[MAX_CAMERAS];
	BOOL	m_bSaveImg1;
	BOOL	m_bPlcWrite;
	void	ReLoadDisplay(void);
	void	WriteData();
	CString m_strInspStartTime;
	CString	m_strInspEndTime;
	double	m_dRemainHDDSpace1;
	double	m_dRemainHDDSpace2;
	void	GetHDDSpace(CString strPath, long *total, long *occupied, long *remaining);
	void	CheckHdd();
	CgProject m_Project;
	CgMulti		m_Multi;
	CgDisplay m_Display[MAX_CAMERAS];	
	CgDisplay m_DisplaySave[MAX_CAMERAS];	
	CgDisplay m_DisplayServer[MAX_CAMERAS];	

	int m_nMaxFrameCount;
	int		m_nTimeCheckCount;
	int		m_nTotalInsCount;
	int		m_nOKCount;
	int		m_nAiInsOK;
	int		m_nNg1Count;
	int		m_nNg2Count;
	int		m_nNgCount;
	int		m_nInspTime;
	BOOL		m_bResultIP;
	int			m_nCSVCount;
	double	m_dYield;
	double	m_dTactTime;
	double	m_dTactTimeOld;
	void	InitDisplay(void);
	void	DrawDisplay(int nCam);
	void	DrawDisplay2(int nCam);
	void	SetOutputResult(int nCam, int nResult, int nDLResult);

	CGigECamera *m_Vision[MAX_CAMERAS];
	//CVision m_Vision;
	int		InitCam(void);
	UINT	LoopGrab(int nCam,unsigned char*);
	void	InitIO(void);
	UINT	LoopIO(bool* bpData);
	CgDisplay* GetScreen();
/////JSON
	void		WriteJsonFile();
	bool    WriteToFile(const char* filename, const char* buffer, int len);
//////////////////////////////////////////////////////////////////////////////////////////////////////////
	bool m_bRun;
	bool m_bAuto;	
	bool m_bInsComplete;	
	bool m_bInsDone;	
	int m_nIOTriCount;	
	int m_nIOTriErrCount;	
	int m_nFrameName;
	bool	m_bExit;
	CString	m_strModel;	
	double	m_dScaleFactorX;
	CString m_strStats;	

	int m_nAiImageWidth;
	int m_nAiImageHeight;
	CString m_strAiOk;
	CString m_strAiNG;
	CString m_strAiPASS;

	int		m_nTactOver;

	CDlgSet	*m_pDlgSet;
	DlgAiView	*m_pDlgAiView;
	void InitClass(void);
	void	InitControl();

	void	ViewData(void);
	void	LabelResult(int nRBResult, int nDLResult);
	void	LabelStats(CString str,int stats);

	void	Dirtest();
	void	Wait(DWORD dwT);
	void	AddtoList(CString strData,int mode,BOOL bWrite=FALSE);

	float m_fFps;
	CStatic m_ctMainImg;
	int m_nStats;
	int m_nAICnt;

	//////////////////////////////////////////
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedButtonSettung();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedFolding();
	
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:



public:
	DECLARE_EVENTSINK_MAP()

public:
	CButtonST	m_btFolding;

	//CButtonST	m_btnApiStatus;B

	CLabel		m_ctTTitle;
	CLabel		m_lbViewData[MAX_VIEW_DATA];	//0:검사갯수 1:토탈피치 2:개별평균 3:개별(L) 4:검사속도 5: 6:개별표준편차 7:개별(R)
	CLabel		m_StaticStats;
	CListBox	m_cLogList;
	CLabel		m_LabelTimeCheck;
	CLabel		m_LabelNonSense;
	CLabel m_LabelBadCount;
	CLabel m_LabelBadCount2;
	CLabel m_LabelRemainHDD;
	CLabel m_LabelRemainHdd1;
	CLabel m_Label_FinalResult;
	CLabel m_Label_OKNGResult;
	CLabel m_Label_DLOKNGResult;
	CLabel m_Label_OKNGResult2;

	CLabel m_Label_Ins_Data2[31];
	CLabel m_Label_DL_Ins_Data[23];
	CLabel m_Label_Ins_Data;

	CLabel m_LabelNotCap;
	CLabel m_LabelDistNg;
	CLabel m_LabelCapNg;
	CLabel m_LabelWeldNg;
	CLabel m_LabelTabNg;

	//unsigned char *m_imgBuf[5];

public:
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	// 전체화면,문자화면 전환용 라디오 버튼
	int m_nScreen;
	afx_msg void OnBnClickedRadioFullScreen();
	afx_msg void OnStnClickedDrawView();
	afx_msg void OnLbnSelchangeListLog2();
	CString m_strBarCode;
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnRawInput(UINT nInputcode, HRAWINPUT hRawInput);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg UINT OnGetDlgCode();
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnSizeClipboard(CWnd* pClipAppWnd, HGLOBAL hRect);
	afx_msg void OnUniChar(UINT Char, UINT nRepCnt, UINT nFlags);
	afx_msg void OnClipboardUpdate();
	afx_msg BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	CLabel m_LabelMap;
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	virtual BOOL DestroyWindow();
	CLabel m_ctInsp;
	CLabel m_LabelNgTime[5];
	CLabel m_LabelBadName[5];
	CListBox m_cInspLog;
	BOOL m_bCylindrUse;
	afx_msg void OnBnClickedCheck5();
	void TotalResetCnt();
	void TotalNgCnt();
	void NgTestData();
	double m_dTime[10];
	double m_GraphTestData[3][24];
	int m_nTabEdgeCnt;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedBtnApiView();
	//	afx_msg void OnStnClickedStaticInsData29();
	CMFCButton m_btTime;
	CMFCButton m_btCellApi;
	CMFCButton m_btModel;
	CMFCButton m_btStart;
	CMFCButton m_btStop;
	CMFCButton m_btSetting;
	CMFCButton m_btReset;
	CMFCButton m_btExit;
	CMFCButton m_btInfo;
	CMFCButton m_btVersion;

	//IO


	CLabel m_LabelDLNgCategory[4];
	CLabel m_LabelRB;
	CLabel m_LabelDL;

	//AI
	BOOL	m_bFinalRet;
	BOOL		m_bUseAI;
	BOOL		m_bUseAIResult;
	ReturnParams* returnparams[MAX_CAMERAS];
	int		m_nDLResult;
	int		m_nDLNgCount;
	int		m_nRBNgCount;
	BOOL m_test;
	//치수
	double m_dDLCapCanX;
	double m_dDLCapCanY;
	double m_dDLTabEdgeGapY;
	double m_dDLCapWeldX;
	double m_dDLCapWeldY;
	double m_dDLWeldLenth;
	double m_dDLWeldWidth;
	double m_dDLCapTabLeftX;
	double m_dDLCapTabRightX;
	double m_dDLCapTabTopY;
	double m_dDLWeldCanY;
	//외관
	BOOL m_bisTC;
	BOOL m_bisTC_frontback;
	BOOL m_bisTC_slatned;
	BOOL m_bisTC_CID;
	BOOL m_bisCAN;
	BOOL m_bisTAB;
	BOOL m_bisTABerror;
	BOOL m_bisETCerror;
	BOOL m_bisYJ_fromTAB;
	BOOL m_bisYakYJ;
	BOOL m_bisGwaYJ;
	BOOL m_bisGeuYJ;

	//구간별로그
	SYSTEMTIME		m_time;
	SYSTEMTIME		m_timeInsp;
	SYSTEMTIME		m_timeInsp2;

	//상위서버
	SYSTEMTIME		m_timeVisionInput;
	SYSTEMTIME		m_timeVisionOutput;
	SYSTEMTIME		m_timeInspStart;
	SYSTEMTIME		m_timeInspEnd;
	CString			m_strEqpID;
	CString			m_strEqpInspID;
	CString			m_strCellID;
	CString			m_strLotID;
	CString			m_strPathServerImg;
	CString			m_strPathServerJson[static_cast<int>(JsonClassification::_TOTALCOUNT)]; // 0 _STATUS, 1 _INSPECTION, 2 _SPEC_PARA, 3 _ALARM
	CString			m_strUploadStandByPath[static_cast<int>(JsonClassification::_TOTALCOUNT)]; // 0 _STATUS, 1 _INSPECTION, 2 _SPEC_PARA, 3 _ALARM
	CString			m_strCodeReal;
	int				m_nNgCode;
	int				m_nNgType; //0-DIMENSION(치수) , 1-APPEARANCE(외관)
	int				m_nCellCount;
	int				m_nDefectPosX;
	int				m_nDefectPosY;
	int				m_nRBResult;	//R/B 최종결과
	double			m_dNgValue;
	double			m_dCapCanDist;
	double			m_dCapCanDistX;
	double			m_dCapCanDistY;
	double			m_dTabPosY;
	CPoint			m_ptCanTop;

	void WriteJsonInsp(int nTargetCam, CString strServerPath, SYSTEMTIME sysTime, int nLine, int nLane, BOOL bDefectNg, CString strDefectFIleName, BOOL bNG);
	void WriteJsonSpecPara(int nTargetCam, int nType, int nLane);
	void WriteJsonStatus(int nVisionType, int nLane);
	bool PingCheck(CString strIP);
	bool MakeDirectory(CString strPath);
	void InitDirectory();
	void CellServerInit();
	void CellServerView();
	void NgClassify(CString strBadInfo);
	void SetEqipID(int nLine, int nLane);
	void SetDefectCropInfo(unsigned char* fmOriginSrc, unsigned char* fmCropDst, CString strNgName, int nOriginImgWidth, int nOriginImgHeight, DEFECTDATA* dataDefect, CPoint ptPos, CRect rtRoi, bool bHasDefectData, int nDefectSizeX = 0, int nDefectSizeY = 0, int nDefectArea = 0, double dDefectValAvg = 0.0, int nDefectValMax = 0, int nDefectValMin = 0, double dDefectValStdDev = 0.0);
	void ResetDimensionData();
	void SetDimensionData(int nInsNum, BOOL bException, double dDimensionValue, CPoint ptStart, CPoint ptEnd, BOOL bResult);

	DIMENSIONDATA m_DimensionData[3]; //치수 데이터
	//1-Tab pos, 2-Cap~Can x, 3-Cap~Can y

	//셀추적
	bool	m_bCellTraceConeect;
	public:
		CString				m_strSocketEvent;
		LoadVTECSDll		m_LoadVTECSDll;
		CCriticalSection	m_CriticalDLL;
		stDllStatus			m_CurrentDllStatus, m_OldDllStatus;
		bool				m_bIPConfigShow;
		bool				m_blastOldConnectStatus;
		//typeTest			m_stBinaryTest;
		/*static UINT WINAPI DllSocket_ConnectStatus(char* strEventContents, int nStatus, int nErrorCode);
		static UINT WINAPI DllSocket_ReceiveDataBuffer(unsigned char* pReceiveBuffer, int nReceiveLengthWithByte);*/

		/// <summary>
		/// 통신 연결 변경되면 호출되는 함수
		/// </summary>
		/// <param name="strIPAddress4">통신 이벤트 발생 내역</param>
		/// <param name="nPortNumber">포트 번호</param>
		/// <param name="nStatus">연결 상태를 int로 표현</param>
		/// <param name="nErrorCode">오류 코드 번호</param>
		/// <returns>0</returns>
		static UINT WINAPI DllSocket_ConnectStatus(char* strEventContents, int nStatus, int nErrorCode);

		/// <summary>
		/// 통신 수신이 되면 호출되는 함수
		/// </summary>
		/// <param name="pReceiveBuffer">수신받은 버퍼 접근 주소</param>
		/// <param name="nReceiveLengthWithByte">수신받은 버퍼 개수</param>
		/// <returns>0</returns>
		static UINT WINAPI DllSocket_ReceiveDataBuffer(unsigned char* pReceiveBuffer, int nReceiveLengthWithByte);

		/// <summary>
		/// 통신 연결 상태 gui 표시처리용 함수
		/// </summary>
		/// <param name="strEventContents"></param>
		/// <param name="nStatus"></param>
		/// <param name="nErrorCode"></param>
		void PostConnectStatus(char* strEventContents, int nStatus, int nErrorCode);

		/// <summary>
		/// 통신 수신이 되면 호출되는 하부 처리 함수
		/// </summary>
		/// <param name="pReceiveBuffer">수신받은 버퍼 접근 주소</param>
		/// <param name="nReceiveLengthWithByte">수신받은 버퍼 개수</param>
		void SubReceiveCallBackProcess(unsigned char* pReceiveBuffer, int nReceiveLengthWithByte);
		afx_msg void OnBnClickedMfcbutton1();
		afx_msg void OnBnClickedMfcbuttonApi();
		afx_msg void OnBnClickedMfcbuttonStop();
		afx_msg void OnBnClickedMfcbuttonStart();
		afx_msg void OnBnClickedMfcbuttonSetting();
		afx_msg void OnBnClickedMfcbuttonReset();
		afx_msg void OnBnClickedMfcbuttonExit();
};

