// FlyCapture2_EXDlg.cpp : 구현 파일
//

//#include "math.h"
#include "stdafx.h"
#include "FlyCapture2_EX.h"
#include "FlyCapture2_EXDlg.h"
#include <Afxmt.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "json\json.h"
#include "ImageFormat.h"
#include "iphlpapi.h"
#include "WeakWeldDlg.h"
#include <io.h>	
#include <thread>


#pragma comment(lib, "imm32.lib")
#pragma comment (lib, "gdiplus.lib")

#include "imm.h"

#include "NeProtocol.h"

#define	IMAGE_PATH_NAS					"D:\\NAS\\DATA\\IMAGE\\"

#pragma comment(lib, "Neserial.lib")

typedef int(*fp_DAP_Initialize_Func)();
typedef int(*fp_DAP_Inference_Func)(unsigned char*, int, int, BOOL, ReturnParams*, const char*, const char*);
typedef int(*fp_DAP_Release_Func)();

auto DAP_Inference_DLL = LoadLibrary("C:\\inferWIN\\USER_LIBRARY\\INTERFACE\\CYLINDER_CSW\\DAP_INTERFACE.dll");
//auto DAP_Inference_DLL = LoadLibrary("C:\\inferWIN\\USER_LIBRARY\\INTERFACE\\CYLINDER\\CTW\\DAP_INTERFACE.dll");
auto DAP_Inference_Func = (fp_DAP_Inference_Func)GetProcAddress(DAP_Inference_DLL, "dap_inference");
auto DAP_Initialize_Func = (fp_DAP_Initialize_Func)GetProcAddress(DAP_Inference_DLL, "dap_initialize");
auto DAP_Release_Func = (fp_DAP_Release_Func)GetProcAddress(DAP_Inference_DLL, "dap_release");

/////////////////////LAMP//////////////////////////////
//extern "C" __declspec(dllimport) void Usb_Qu_Open();
//extern "C" __declspec(dllimport) void Usb_Qu_Close();
//extern "C" __declspec(dllimport) int Usb_Qu_Getstate();
//extern "C" __declspec(dllimport) bool Usb_Qu_write(byte Qu_index, char Qu_type,
//												   char *pData);
//#define   C_index	0           // 
//#define   C_type	0 
//
//#define   D_not	      100            // Don't care  // Do not change before state
//#define   C_lampoff	   0 
//#define   C_lampon	    1 
//#define   C_lampblink	2 
///////////////////////////////////////////////////////

//============================================================================
// Thread Process Caller
//============================================================================


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


#define DIO_OUTPUT_0			0
#define DIO_OUTPUT_1			1
#define DIO_OUTPUT_2			2


#define GRID_COL_NO			0
#define GRID_COL_AVE		1
#define GRID_COL_TH_UP		2
#define GRID_COL_MAX_V		3
#define GRID_COL_TH_DN		4
#define GRID_COL_MIN_V		5
#define GRID_COL_SIZE_X		6
#define GRID_COL_SIZE_Y		7
#define GRID_COL_TYPE		8
#define TEST_TIMER			1
#define TEST_TIMER2			2

// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.
extern UINT ThreadInspect(LPVOID lParam);//@chcho
extern UINT ThreadSaveOverlay(LPVOID lParam);//
extern UINT ThreadSaveNG(LPVOID lParam);//
extern UINT ThreadCPUCheck(LPVOID lParam);
extern UINT ThreadInspectCheck(LPVOID lParam);
extern UINT ThreadStatus(LPVOID lParam);
extern UINT ThreadCamStatus(LPVOID lParam);
extern UINT ThreadUploadData(LPVOID lParam);
extern UINT ThreadDLIns(LPVOID lParam);

void ThreadInsDL(LPVOID lParamm, int nCam, unsigned char* fm);


CFlyCapture2_EXDlg* pTempLoad = NULL;


class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};


CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CFlyCapture2_EXDlg 대화 상자


CFlyCapture2_EXDlg::CFlyCapture2_EXDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFlyCapture2_EXDlg::IDD, pParent)
	, m_bAuto(false)
	, m_fFps(0)
	, m_nStats(0)
	, m_bRun(true)
	, m_nScreen(0)
	, m_strBarCode(_T(""))
	, m_bCylindrUse(TRUE)
{
	m_nIOTriErrCount = 0;
	m_nComleteNgCnt = 0;
	m_nTabEdgeCnt = 0;
	m_nCellCount = 0;
	m_nNgCode = NG_TAP_POS;
	m_nNgType = 0;
	m_bInsDone = false;
	m_bInsComplete =TRUE;
	m_nIOTriCount = 0;
	m_nIOTriErrCount = 0;
	m_bResultIP =FALSE;
	m_nFrameName = 0;
	m_dMidWeldV = 0;
	m_nTabTopEdge = 0;
	m_nTabBottomLeft = 0;
	m_nTabBottomRight = 0;
	m_nTabBottomTop = 0;
	m_dCanDistY = 0;
	for(int i=0;i<5;i++)		m_nMorphCount[i] = 0;
	for(int i=0;i<2;i++)		m_dTabBottomSize[i] =0;
	m_nMaxThFrame[0]=m_nMaxThFrame[1]=0;
	m_rtWeldBlob.SetRect(0,0,0,0);
	m_nWeldCount = 0;
	m_bTestMode = FALSE;
	m_ptWeldCenterPos.SetPoint(0,0);
	m_nTopCapCenterBrightness = 0;
	m_nTimeCheckCount=0;
	m_nTopCapCheck = 0;
	m_nMaxFrame = 11;
	m_nMinus = 0;
	m_bTesting = FALSE;
	m_dTactTimeOld = 0;
	m_dTactTime = 0;
	m_nInspTime = 0;
	m_nFrameCheck = 0;
	m_bStarting = FALSE;
	nHour1= 0;
	m_strFileName = "";
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pDlgSet = NULL;
	m_DlgBarcode = NULL;
	m_pDlgAiView= NULL;
	m_LightMap = NULL;
	m_CamDisplay = NULL;
	m_nModelCount = NULL;
	m_pDlgPassword = NULL;
	//m_MainView = NULL;
	m_strStats = "";
	m_dScaleFactorX = 1;//03;//0.0068250;//0.0069;//0.0071444
	m_bExit = false;
	m_nShowSet = 0;
	m_bSaveImg[0] = FALSE;
	m_bSaveImg[1] = FALSE;
	m_nReworkAlarmCount = 0;
	m_bSaveImg1 = FALSE;
	m_bPlcWrite = FALSE;
	m_nCSVCount=0;
	m_nOKCount=0;
	m_nNg1Count=0;
	m_nNg2Count=0;
	m_nNgCount = 0;
	m_nTactOver = 0;
	m_nCandiCount[0]=m_nCandiCount[1]=0;
	m_nTotalInsCount = 0;
	m_nNg1Count = 0;
	m_nNg2Count = 0;
	m_nNgCount = 0;
	m_dYield = 0;
	m_dTactTime = 0;
	m_strLot = "TEST";
	m_nSaveCount = 0;
	m_nSAveResult = 0;
	m_bInspection = FALSE;
	m_nNewCount = 0;
	m_nBufCount[0]=m_nBufCount[1] = 0;
	m_nSelectFrame = 0;
	m_dLastWriteTime = 0;
	m_nInch =55;
	m_nAlarmCnt = 0;
	m_dTactTimeAve[0]=m_dTactTimeAve[1]=m_dTactTimeAve[2] = 0;

	m_nZoomSize = 1;
	m_nOKNG[0]=m_nOKNG[1] = -1;
	m_nSendIO = 0;
	for(int i=0;i<15;i++)
		m_nPackCount[i] = -1;

	for(int i=0; i<50; i++)
		m_bNgPosition[i] = FALSE;

	for(int i=0;i<8;i++)m_nIoStats[i] = 0;
	m_strModelType = "D";
	m_nInspMode = 1;
	m_nLimiteTime = 1600;
	m_nPlcCnt = 0;
	m_bCheckHdd = FALSE;
	m_bRework = FALSE;
	m_nSkipCount[0]=m_nSkipCount[1] = 0;
	m_nIOLimitTime = 1600;
	m_nCheckCam[0]=m_nCheckCam[1]=0;
	m_bCamStats[0] = FALSE;
	m_bCamStats[1] = FALSE;
	m_nFullFrameCount = 0;
	m_nGrabComplete = 0;
	m_nSelectCam = 0;
	m_nSelectLight = 0;
	m_nBrightModel = 1;
	m_nRetryCount = 0;
	for(int i=0; i<10; i++) m_dTime[i]=0.0;
	for(int i=0;i<100;i++)
	{
		m_dData[i] = 0;
		m_nResult[i] = 0;
		m_bResult[i] = FALSE;
		m_bInsp[i] = -1;
		m_nInspStats[i] = -1;//-1초기화 0정상 1이상 불량
	}
	m_nInspCount = 0;
	m_bSheetCount = 200;
	m_nIsSheet = 0;//0isnot 1 is
	m_nIoCount = 0;
	m_nMachine = 1;
	m_bCaptureSaving = FALSE;
	m_nCalLine[0]=m_nCalLine[1]=0;
	m_nResetCount = 0;
	m_bHoleBad[0][0]=m_bHoleBad[0][1]=m_bHoleBad[1][0]=m_bHoleBad[1][1] = FALSE;
	m_dTabDist[0]=m_dTabDist[1]=0;
	m_nCanCheckAve = 0;
	m_nCapCheckAve = 0;
	m_nLight = 0;
	m_nTimeReset = 0;
	for(int i=0;i<24;i++)
	{
		m_dTimeRate[i] = 0.0;
		//m_nTimeNo[i] = 0;
		//m_nTimeNoBad[i] = 0;
		//m_nBadHole[i]=0;	//홀불량
		//m_nBadEdge[i]=0;	//에지불량
		//m_nBadPos[i]=0;	//위치불량
		//m_nBadexistence[i]=0;	//미부착
		//m_nBadArea[i]=0;
		//m_nBadNotBead[i]=0;
		for(int j=0;j<10;j++)
			m_nTimeCount[i][j] = 0;
	}
	m_nCanBlackCount = 0;
	m_nCapCheckAve = 0;
	m_nTopCapV = 0;

	mDrawBadState[0] = NULL;//time rate
	mDrawBadState[1] = NULL;//Bad count Bar
	mDrawBadState[2] = NULL;//Total rate
	m_nWeakWeld = -1;
	m_nWhiteWeldCheck = 0;

	m_nWeakWeldTotalCnt = 0;
	m_nWeakWeldNgCnt = 0;
	m_nAICnt=0;
	pTempLoad = this;
	m_strCellID = "NA";
	m_strLotID = "NA";
	m_dNgValue = 0.0;
	m_dCapCanDist = 0;
	m_dCapCanDistX = 0;
	m_dCapCanDistY = 0;
	m_nDefectPosX = 0;
	m_nDefectPosY = 0;
	m_nAiInsOK = -1;
	m_nAiImageWidth = 194;
	m_nAiImageHeight = 88;
	m_strAiOk = _T("AI OK");
	m_strAiNG = _T("AI NG");
	m_strAiPASS = _T("AI PASS");
	m_bCellTraceConeect = false;
	m_strEqpID = "";
	m_strEqpInspID = "";
	m_bUseAI = FALSE;
	m_bUseAIResult = FALSE;
	m_nDLResult = FALSE;
	m_nRBResult = RET_OK;
	m_bFinalRet = FALSE;
	m_nDLNgCount = 0;
	m_nRBNgCount = 0;
	m_dDLCapCanX = 0.0;
	m_dDLCapCanX = 0;
	m_dDLCapCanY = 0;
	m_dDLTabEdgeGapY = 0;
	m_dDLCapWeldX = 0;
	m_dDLCapWeldY = 0;
	m_dDLWeldLenth = 0;
	m_dDLWeldWidth = 0;
	m_dDLCapTabLeftX = 0;
	m_dDLCapTabRightX = 0;
	m_dDLCapTabTopY = 0;
	m_dDLWeldCanY = 0;
	m_bisTC = 1;
	m_bisTC_frontback = 1;
	m_bisTC_slatned = 1;
	m_bisTC_CID = 1;
	m_bisCAN = 1;
	m_bisTAB = 1;
	m_bisTABerror = 1;
	m_bisETCerror = 1;
	m_bisYJ_fromTAB = 1;
	m_bisYakYJ = 1;
	m_bisGwaYJ = 1;
	m_bisGeuYJ = 1;
	m_nGrabCount = 0;
	m_nDLNgType = -1;
	for (int i = 0; i < 4; i++)
		m_nDLBadCount[i] = 0;
}

void CFlyCapture2_EXDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_LOG2, m_cLogList);
	DDX_Control(pDX, IDC_ST_IMAGE, m_ctMainImg);
	DDX_Control(pDX, IDC_STATICSTATS, m_StaticStats);
	DDX_Control(pDX, IDC_FOLDING, m_btFolding);
	DDX_Control(pDX, IDC_STATIC_LABEL1, m_lbViewData[0]); // Pgm Version
	DDX_Control(pDX, IDC_STATIC_LABEL2, m_lbViewData[1]);
	DDX_Control(pDX, IDC_STATIC_LABEL3, m_lbViewData[2]);
	DDX_Control(pDX, IDC_STATIC_LABEL4, m_lbViewData[3]);
	DDX_Control(pDX, IDC_STATIC_LABEL5, m_lbViewData[4]);
	DDX_Control(pDX, IDC_STATIC_LABEL6, m_lbViewData[5]);
	DDX_Control(pDX, IDC_STATIC_LABEL7, m_lbViewData[6]);
	DDX_Control(pDX, IDC_STATIC_LABEL8, m_lbViewData[7]);
	DDX_Radio(pDX, IDC_RADIO_FULL_SCREEN, m_nScreen);
	DDX_Control(pDX, IDC_STATIC_LABEL_NGMARK, m_LabelWeldNg);
	DDX_Text(pDX, IDC_EDIT_BARCODE, m_strBarCode);
	DDX_Control(pDX, IDC_STATIC_LABEL_BCR, m_LabelTabNg);
	DDX_Control(pDX, IDC_STATIC_LABEL_BCODE, m_LabelNotCap);
	DDX_Control(pDX, IDC_STATIC_LABEL_BCODE2, m_LabelTimeCheck);
	DDX_Control(pDX, IDC_STATIC_LABEL_NONSENSE, m_LabelNonSense);
	DDX_Control(pDX, IDC_STATIC_LABEL_BADCOUNT, m_LabelBadCount);
	DDX_Control(pDX, IDC_STATIC_LABEL_YEILD, m_LabelCapNg);
	DDX_Control(pDX, IDC_STATIC_LABEL_YEILD2, m_LabelBadCount2);
	DDX_Control(pDX, IDC_STATIC_LABEL_REMAINHDD, m_LabelRemainHDD);
	DDX_Control(pDX, IDC_STATIC_LABEL_REMAINHDD2, m_LabelRemainHdd1);
	DDX_Control(pDX, IDC_STATIC_LABEL_BRIGHT, m_LabelDistNg);
	DDX_Control(pDX, IDC_STATIC_FINAL_RESULT, m_Label_FinalResult);
	DDX_Control(pDX, IDC_STATIC_OK_NG_RESULT, m_Label_OKNGResult);
	DDX_Control(pDX, IDC_STATIC_DL_OK_NG_RESULT, m_Label_DLOKNGResult);
	DDX_Control(pDX, IDC_STATIC_INS_DATA, m_Label_Ins_Data);
	DDX_Control(pDX, IDC_STATIC_INS_DATA2, m_Label_Ins_Data2[0]);
	DDX_Control(pDX, IDC_STATIC_INS_DATA3, m_Label_Ins_Data2[1]);
	DDX_Control(pDX, IDC_STATIC_INS_DATA4, m_Label_Ins_Data2[2]);
	DDX_Control(pDX, IDC_STATIC_INS_DATA5, m_Label_Ins_Data2[3]);
	DDX_Control(pDX, IDC_STATIC_INS_DATA6, m_Label_Ins_Data2[4]);
	DDX_Control(pDX, IDC_STATIC_INS_DATA7, m_Label_Ins_Data2[5]);
	DDX_Control(pDX, IDC_STATIC_INS_DATA8, m_Label_Ins_Data2[6]);
	DDX_Control(pDX, IDC_STATIC_INS_DATA9, m_Label_Ins_Data2[7]);
	DDX_Control(pDX, IDC_STATIC_INS_DATA10, m_Label_Ins_Data2[8]);
	DDX_Control(pDX, IDC_STATIC_INS_DATA11, m_Label_Ins_Data2[9]);
	DDX_Control(pDX, IDC_STATIC_INS_DATA12, m_Label_Ins_Data2[10]);
	DDX_Control(pDX, IDC_STATIC_INS_DATA13, m_Label_Ins_Data2[11]);
	DDX_Control(pDX, IDC_STATIC_INS_DATA14, m_Label_Ins_Data2[12]);
	DDX_Control(pDX, IDC_STATIC_INS_DATA15, m_Label_Ins_Data2[13]);
	DDX_Control(pDX, IDC_STATIC_INS_DATA16, m_Label_Ins_Data2[14]);
	DDX_Control(pDX, IDC_STATIC_INS_DATA17, m_Label_Ins_Data2[15]);
	DDX_Control(pDX, IDC_STATIC_INS_DATA18, m_Label_Ins_Data2[16]);
	DDX_Control(pDX, IDC_STATIC_INS_DATA19, m_Label_Ins_Data2[17]);
	DDX_Control(pDX, IDC_STATIC_INS_DATA20, m_Label_Ins_Data2[18]);
	DDX_Control(pDX, IDC_STATIC_INS_DATA21, m_Label_Ins_Data2[19]);
	DDX_Control(pDX, IDC_STATIC_INS_DATA22, m_Label_Ins_Data2[20]);
	DDX_Control(pDX, IDC_STATIC_INS_DATA23, m_Label_Ins_Data2[21]);
	DDX_Control(pDX, IDC_STATIC_INS_DATA24, m_Label_Ins_Data2[22]);
	DDX_Control(pDX, IDC_STATIC_INS_DATA25, m_Label_Ins_Data2[23]);
	DDX_Control(pDX, IDC_STATIC_INS_DATA26, m_Label_Ins_Data2[24]);
	DDX_Control(pDX, IDC_STATIC_INS_DATA27, m_Label_Ins_Data2[25]);
	DDX_Control(pDX, IDC_STATIC_INS_DATA28, m_Label_Ins_Data2[26]);
	DDX_Control(pDX, IDC_STATIC_INS_DATA29, m_Label_Ins_Data2[27]);
	DDX_Control(pDX, IDC_STATIC_INS_DATA30, m_Label_Ins_Data2[28]);
	DDX_Control(pDX, IDC_STATIC_INS_DATA31, m_Label_Ins_Data2[29]);
	DDX_Control(pDX, IDC_STATIC_INS_DATA32, m_Label_Ins_Data2[30]);
	//DDX_Control(pDX, IDC_STATIC_INS_DATA35, m_Label_Ins_Data2[33]);
	//DL
	DDX_Control(pDX, IDC_STATIC_DL_INS_DATA_1, m_Label_DL_Ins_Data[0]);
	DDX_Control(pDX, IDC_STATIC_DL_INS_DATA_2, m_Label_DL_Ins_Data[1]);
	DDX_Control(pDX, IDC_STATIC_DL_INS_DATA_3, m_Label_DL_Ins_Data[2]);
	DDX_Control(pDX, IDC_STATIC_DL_INS_DATA_4, m_Label_DL_Ins_Data[3]);
	DDX_Control(pDX, IDC_STATIC_DL_INS_DATA_5, m_Label_DL_Ins_Data[4]);
	DDX_Control(pDX, IDC_STATIC_DL_INS_DATA_6, m_Label_DL_Ins_Data[5]);
	DDX_Control(pDX, IDC_STATIC_DL_INS_DATA_7, m_Label_DL_Ins_Data[6]);
	DDX_Control(pDX, IDC_STATIC_DL_INS_DATA_8, m_Label_DL_Ins_Data[7]);
	DDX_Control(pDX, IDC_STATIC_DL_INS_DATA_9, m_Label_DL_Ins_Data[8]);
	DDX_Control(pDX, IDC_STATIC_DL_INS_DATA_10, m_Label_DL_Ins_Data[9]);
	DDX_Control(pDX, IDC_STATIC_DL_INS_DATA_11, m_Label_DL_Ins_Data[10]);
	DDX_Control(pDX, IDC_STATIC_DL_INS_DATA_12, m_Label_DL_Ins_Data[11]);
	DDX_Control(pDX, IDC_STATIC_DL_INS_DATA_13, m_Label_DL_Ins_Data[12]);
	DDX_Control(pDX, IDC_STATIC_DL_INS_DATA_14, m_Label_DL_Ins_Data[13]);
	DDX_Control(pDX, IDC_STATIC_DL_INS_DATA_15, m_Label_DL_Ins_Data[14]);
	DDX_Control(pDX, IDC_STATIC_DL_INS_DATA_16, m_Label_DL_Ins_Data[15]);
	DDX_Control(pDX, IDC_STATIC_DL_INS_DATA_17, m_Label_DL_Ins_Data[16]);
	DDX_Control(pDX, IDC_STATIC_DL_INS_DATA_18, m_Label_DL_Ins_Data[17]);
	DDX_Control(pDX, IDC_STATIC_DL_INS_DATA_19, m_Label_DL_Ins_Data[18]);
	DDX_Control(pDX, IDC_STATIC_DL_INS_DATA_20, m_Label_DL_Ins_Data[19]);
	DDX_Control(pDX, IDC_STATIC_DL_INS_DATA_21, m_Label_DL_Ins_Data[20]);
	DDX_Control(pDX, IDC_STATIC_DL_INS_DATA_22, m_Label_DL_Ins_Data[21]);
	DDX_Control(pDX, IDC_STATIC_DL_INS_DATA_23, m_Label_DL_Ins_Data[22]);

	DDX_Control(pDX, IDC_STATIC_LABEL_YEILD5, m_ctInsp);
	DDX_Control(pDX, IDC_STATIC_LABEL_TACTTIME3, m_LabelNgTime[0]);
	DDX_Control(pDX, IDC_STATIC_LABEL_TACTTIME4, m_LabelNgTime[1]);
	DDX_Control(pDX, IDC_STATIC_LABEL_TACTTIME5, m_LabelNgTime[2]);
	DDX_Control(pDX, IDC_STATIC_LABEL_TACTTIME6, m_LabelNgTime[4]);
	DDX_Control(pDX, IDC_STATIC_LABEL_TACTTIME7, m_LabelNgTime[3]);
	DDX_Control(pDX, IDC_STATIC_LABEL_TACTTIME8, m_LabelBadName[0]);
	DDX_Control(pDX, IDC_STATIC_LABEL_TACTTIME9, m_LabelBadName[1]);
	DDX_Control(pDX, IDC_STATIC_LABEL_TACTTIME10, m_LabelBadName[2]);
	DDX_Control(pDX, IDC_STATIC_LABEL_TACTTIME12, m_LabelBadName[3]);
	DDX_Control(pDX, IDC_STATIC_LABEL_TACTTIME11, m_LabelBadName[4]);
	DDX_Control(pDX, IDC_LIST_LOG5, m_cInspLog);
	DDX_Check(pDX, IDC_CHECK5, m_bCylindrUse);

	DDX_Control(pDX, IDC_MFCBUTTON_API, m_btCellApi);
	DDX_Control(pDX, IDC_MFCBUTTON_TIME, m_btTime);
	DDX_Control(pDX, IDC_MFCBUTTON_MODEL, m_btModel);
	DDX_Control(pDX, IDC_MFCBUTTON_START, m_btStart);
	DDX_Control(pDX, IDC_MFCBUTTON_STOP, m_btStop);
	DDX_Control(pDX, IDC_MFCBUTTON_SETTING, m_btSetting);
	DDX_Control(pDX, IDC_MFCBUTTON_RESET, m_btReset);
	DDX_Control(pDX, IDC_MFCBUTTON_EXIT, m_btExit);
	DDX_Control(pDX, IDC_MFCBUTTON_INFO, m_btInfo);
	DDX_Control(pDX, IDC_MFCBUTTON_VERSION, m_btVersion);


	DDX_Control(pDX, IDC_STATIC_LABEL_RULL_BASE, m_LabelRB);
	DDX_Control(pDX, IDC_STATIC_LABEL_DEEL_LEARNING, m_LabelDL);
	DDX_Control(pDX, IDC_STATIC_LABEL_DL_CAP_NG, m_LabelDLNgCategory[0]);
	DDX_Control(pDX, IDC_STATIC_LABEL_DL_DIST_NG, m_LabelDLNgCategory[1]);
	DDX_Control(pDX, IDC_STATIC_LABEL_DL_WELD_NG, m_LabelDLNgCategory[2]);
	DDX_Control(pDX, IDC_STATIC_LABEL_DL_TAB_NG, m_LabelDLNgCategory[3]);
	
}

BEGIN_MESSAGE_MAP(CFlyCapture2_EXDlg, CDialog)
	ON_MESSAGE(UM_RECEIVE1, OnReceive)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_BN_CLICKED(IDC_FOLDING, &CFlyCapture2_EXDlg::OnBnClickedFolding)
	ON_BN_CLICKED(IDC_BUTTON5, &CFlyCapture2_EXDlg::OnBnClickedButton5)
	ON_WM_MOUSEWHEEL()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_RADIO_FULL_SCREEN, &CFlyCapture2_EXDlg::OnBnClickedRadioFullScreen)
	ON_BN_CLICKED(IDC_RADIO_CHAR_SCREEN, &CFlyCapture2_EXDlg::OnBnClickedRadioFullScreen)
	ON_BN_CLICKED(IDC_RADIO_ETC, &CFlyCapture2_EXDlg::OnBnClickedRadioFullScreen)
	ON_BN_CLICKED(IDC_RADIO_ETC2, &CFlyCapture2_EXDlg::OnBnClickedRadioFullScreen)
	ON_STN_CLICKED(IDC_DRAW_VIEW, &CFlyCapture2_EXDlg::OnStnClickedDrawView)
	ON_LBN_SELCHANGE(IDC_LIST_LOG2, &CFlyCapture2_EXDlg::OnLbnSelchangeListLog2)
	ON_WM_CHAR()
	ON_WM_INPUT()
	ON_WM_KEYDOWN()
	ON_WM_GETDLGCODE()
	ON_WM_KEYUP()
	ON_WM_SIZECLIPBOARD()
	ON_WM_UNICHAR()
	ON_WM_CLIPBOARDUPDATE()
	ON_WM_COPYDATA()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_SETCURSOR()
	ON_BN_CLICKED(IDC_CHECK5, &CFlyCapture2_EXDlg::OnBnClickedCheck5)
	ON_BN_CLICKED(IDC_BUTTON1, &CFlyCapture2_EXDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_MFCBUTTON_API, &CFlyCapture2_EXDlg::OnBnClickedMfcbuttonApi)
	ON_BN_CLICKED(IDC_MFCBUTTON_STOP, &CFlyCapture2_EXDlg::OnBnClickedMfcbuttonStop)
	ON_BN_CLICKED(IDC_MFCBUTTON_START, &CFlyCapture2_EXDlg::OnBnClickedMfcbuttonStart)
	ON_BN_CLICKED(IDC_MFCBUTTON_SETTING, &CFlyCapture2_EXDlg::OnBnClickedMfcbuttonSetting)
	ON_BN_CLICKED(IDC_MFCBUTTON_RESET, &CFlyCapture2_EXDlg::OnBnClickedMfcbuttonReset)
	ON_BN_CLICKED(IDC_MFCBUTTON_EXIT, &CFlyCapture2_EXDlg::OnBnClickedMfcbuttonExit)
END_MESSAGE_MAP()


CFlyCapture2_EXDlg::~CFlyCapture2_EXDlg()
{
	
	//for(int i=0;i<MAX_CAMERAS;i++)
	//{
	//	if(m_bCamStats[i]==TRUE)
	//		m_Vision[i]->Stop();
	//	if(m_Vision[i])delete m_Vision[i];
	//}
	if(m_nMachine==2){
		for(int i=0;i<8;i++)
			m_IO.WriteOutputPort(i,0);//ready
	}
	Sleep(100);
	ExitProcess(0);
}

UINT ThreadInspect(LPVOID lParam)
{
	CFlyCapture2_EXDlg *pView = (CFlyCapture2_EXDlg *)lParam;
	while(pView->m_bRun)
	{
		if(pView->m_bAuto)
		{
			while(WaitForSingleObject((HANDLE)pView->m_evInsp[0], 100) != WAIT_TIMEOUT )
			{
				//				pView->m_evInsp[0].ResetEvent();
				pView->Test(0);
				//				pView->	DrawDisplay(0);
			}
			Sleep(1);
		}
		Sleep(1);
	}

	return 1;
}

UINT ThreadMultiFullSave(LPVOID lParam)
{
	CFlyCapture2_EXDlg *pView = (CFlyCapture2_EXDlg *)lParam;

	while(pView->m_bRun)
	{
		if(pView->m_bAuto)
		{
			while(WaitForSingleObject((HANDLE)pView->m_evFullSave, 100) != WAIT_TIMEOUT )
			{				
				pView->m_pDlgSet->m_FormTab3->MultiFrameFullSave();
				
				Sleep(1);
			}
			Sleep(1);
		}
		Sleep(1);
	}

	return 1;
}

UINT ThreadAutoInspect(LPVOID lParam)
{
	CFlyCapture2_EXDlg *pView = (CFlyCapture2_EXDlg *)lParam;
	int nGrabDelay = pView->m_pDlgSet->m_FormTab1->m_nAgingDelay;
	while (TRUE)
	{
		if (pView->m_bAuto) {
			if (pView->m_pDlgSet->m_FormTab1->m_bUseAging) {
				pView->m_pDlgSet->m_FormTab3->m_LightCtrl.LightSetMode(1);
				//pView->Test(0);
				Sleep(nGrabDelay);
			}
		}
		Sleep(1);
	}
	return 1;
}


void ThreadInsDL(LPVOID lParam, int nCam, unsigned char* fm)
{
	CFlyCapture2_EXDlg *pView = (CFlyCapture2_EXDlg *)lParam;
	double dScale = double(pView->returnparams[0]->okNgParams.scale) / 1000;
	
	pView->m_dTime[7] = pView->m_Project.GetPrecisionTime();
	DAP_Inference_Func(fm, CAMERA_WIDTH, CAMERA_HEIGHT, FALSE, pView->returnparams[0], _T("OC_CB"), _T("CSW"));
	pView->m_dTime[8] = pView->m_Project.GetPrecisionTime();

	ReturnParams* DLData = pView->returnparams[0];

	//m_nDLResult = returnparams[0]->judgeResult;
	//치수
	pView->m_nDLResult = DLData->cswParams.measureOKNG;
	pView->m_dDLCapCanX = DLData->cswParams.from_TCcenter_to_CANcenter_x * dScale; //TC-CAN X
	pView->m_dDLCapCanY = DLData->cswParams.from_TCcenter_to_CANcetner_y * dScale; //TC-CAN Y
	pView->m_dDLTabEdgeGapY = DLData->cswParams.from_TABright_to_TABleft_y * dScale; //Tab Edge Gap Y
	 //m_dDLTabEdgeGapY = abs(DLData->cswParams.TABrighttop_y - DLData->cswParams.TABlefttop_y) * dScale; //Tab Edge Gap Y
	pView->m_dDLCapWeldX = DLData->cswParams.from_TCcenter_to_YJcenter_x * dScale; //TC-WELD X
	pView->m_dDLCapWeldY = DLData->cswParams.from_TCcenter_to_YJcetner_y * dScale; //TC-WELD Y
	pView->m_dDLWeldLenth = DLData->cswParams.YJ_length * dScale;
	pView->m_dDLWeldWidth = DLData->cswParams.YJ_width * dScale;
	pView->m_dDLCapTabLeftX = DLData->cswParams.from_TCcenter_to_TABleft_x * dScale; //TC-TAB LEFT
	pView->m_dDLCapTabRightX = DLData->cswParams.from_TCcenter_to_TABright_x * dScale; //TC-TAB RIGHT
	pView->m_dDLCapTabTopY = DLData->cswParams.from_TCcenter_to_TABupper_y * dScale; //TC-TAB TOP Y
	pView->m_dDLWeldCanY = DLData->cswParams.from_YJcenter_to_Can_y  * dScale; //WELD-CAN Y
	
	//외관
	pView->m_bisTC = DLData->cswParams.isTC;						//TC유무
	pView->m_bisTC_frontback = DLData->cswParams.isTC_frontback;	//TC뒤집힘
	pView->m_bisTC_slatned = DLData->cswParams.isTC_slatned;		//TC기울어짐
	pView->m_bisTC_CID = DLData->cswParams.isTC_CID;				//CID필터
	pView->m_bisCAN = DLData->cswParams.isCAN;						//CAN유무
	pView->m_bisTAB = DLData->cswParams.isTAB;						//TAB유무
	pView->m_bisTABerror = DLData->cswParams.isTABerror;			//TAB불량
	pView->m_bisETCerror = DLData->cswParams.isETCerror;			//오염불량
	pView->m_bisYJ_fromTAB = DLData->cswParams.isYJ_fromTAB;		//용접탭벗어남불량
	pView->m_bisYakYJ = DLData->cswParams.isYakYJ;					//약용접불량
	pView->m_bisGwaYJ = DLData->cswParams.isGwaYJ;					//과용접불량
	pView->m_bisGeuYJ = DLData->cswParams.isGeuYJ;					//그을음불량

	if (!pView->m_bisTC || !pView->m_bisTC_frontback || !pView->m_bisTC_slatned || !pView->m_bisTC_CID || !pView->m_bisCAN || !pView->m_bisTAB || !pView->m_bisTABerror || !pView->m_bisETCerror || !pView->m_bisYJ_fromTAB || !pView->m_bisYakYJ || !pView->m_bisGwaYJ || !pView->m_bisGeuYJ)
		pView->m_nDLResult = 0;

	if (pView->m_pDlgSet->m_FormTab5->m_bDLSaveAll || pView->m_pDlgSet->m_FormTab5->m_bDLSaveNg) {
		memcpy(pView->m_DisplayDLOverlay.m_pImg->GetImagePtr(), pView->returnparams[0]->overlayImageData, CAMERA_WIDTH * CAMERA_HEIGHT_960 * 3);
		if (pView->m_pDlgSet->m_FormTab5->m_bDLSaveAll)
			pView->m_DisplayDLOverlay.SaveDLImg(pView->m_nDLResult, pView->m_nGrabCount);
		else if (pView->m_pDlgSet->m_FormTab5->m_bDLSaveNg && pView->m_nDLResult == 0)
			pView->m_DisplayDLOverlay.SaveDLImg(pView->m_nDLResult, pView->m_nGrabCount);

		//if (DLData->overlayImageData != NULL) {
		//	free(DLData->overlayImageData);
		//	pView->returnparams[0]->overlayImageData = NULL;
		//}
	}

	int nOffset = 50;
	CRect rtDLCrop;
	rtDLCrop.SetRect(DLData->cswParams.YJcenter_x, DLData->cswParams.YJcenter_y - nOffset, DLData->cswParams.YJcenter_x, DLData->cswParams.YJcenter_y - nOffset);
	rtDLCrop.InflateRect(AI_IMAGE_WIDTH / 2, AI_IMAGE_HEIGHT / 2);
	if (rtDLCrop.Width() > AI_IMAGE_WIDTH || rtDLCrop.Height() > AI_IMAGE_HEIGHT)
		;
	else
		pView->m_pDlgAiView->ImageView2(pView->m_Project.m_MultiBuf[pView->m_pDlgSet->m_FormTab4->m_nAiFrame], rtDLCrop);
}

UINT ThreadDLIns(LPVOID lParam)
{
	CFlyCapture2_EXDlg *pView = (CFlyCapture2_EXDlg *)lParam;

	while (TRUE) {
		if (pView->m_test == TRUE) {
			pView->m_test = FALSE;
			double dScale = double(pView->returnparams[0]->okNgParams.scale) / 1000;
			pView->m_dTime[7] = pView->m_Project.GetPrecisionTime();
			DAP_Inference_Func(pView->m_Project.m_MultiBuf[pView->m_pDlgSet->m_FormTab4->m_nAiFrame], CAMERA_WIDTH, CAMERA_HEIGHT, FALSE, pView->returnparams[0], _T("OC_CB"), _T("CSW"));
			pView->m_dTime[8] = pView->m_Project.GetPrecisionTime();
			ReturnParams* DLData = pView->returnparams[0];
			//m_nDLResult = returnparams[0]->judgeResult;
			//치수
			pView->m_nDLResult = DLData->cswParams.measureOKNG;
			pView->m_dDLCapCanX = DLData->cswParams.from_TCcenter_to_CANcenter_x * dScale; //TC-CAN X
			pView->m_dDLCapCanY = DLData->cswParams.from_TCcenter_to_CANcetner_y * dScale; //TC-CAN Y
			pView->m_dDLTabEdgeGapY = DLData->cswParams.from_TABright_to_TABleft_y * dScale; //Tab Edge Gap Y
			//m_dDLTabEdgeGapY = abs(DLData->cswParams.TABrighttop_y - DLData->cswParams.TABlefttop_y) * dScale; //Tab Edge Gap Y
			pView->m_dDLCapWeldX = DLData->cswParams.from_TCcenter_to_YJcenter_x * dScale; //TC-WELD X
			pView->m_dDLCapWeldY = DLData->cswParams.from_TCcenter_to_YJcetner_y * dScale; //TC-WELD Y
			pView->m_dDLWeldLenth = DLData->cswParams.YJ_length * dScale;
			pView->m_dDLWeldWidth = DLData->cswParams.YJ_width * dScale;
			pView->m_dDLCapTabLeftX = DLData->cswParams.from_TCcenter_to_TABleft_x * dScale; //TC-TAB LEFT
			pView->m_dDLCapTabRightX = DLData->cswParams.from_TCcenter_to_TABright_x * dScale; //TC-TAB RIGHT
			pView->m_dDLCapTabTopY = DLData->cswParams.from_TCcenter_to_TABupper_y * dScale; //TC-TAB TOP Y
			pView->m_dDLWeldCanY = DLData->cswParams.from_YJcenter_to_Can_y  * dScale; //WELD-CAN Y

			//외관
			pView->m_bisTC = DLData->cswParams.isTC;						//TC유무
			pView->m_bisTC_frontback = DLData->cswParams.isTC_frontback;	//TC뒤집힘
			pView->m_bisTC_slatned = DLData->cswParams.isTC_slatned;		//TC기울어짐
			pView->m_bisTC_CID = DLData->cswParams.isTC_CID;				//CID필터
			pView->m_bisCAN = DLData->cswParams.isCAN;						//CAN유무
			pView->m_bisTAB = DLData->cswParams.isTAB;						//TAB유무
			pView->m_bisTABerror = DLData->cswParams.isTABerror;			//TAB불량
			pView->m_bisETCerror = DLData->cswParams.isETCerror;			//오염불량
			pView->m_bisYJ_fromTAB = DLData->cswParams.isYJ_fromTAB;		//용접탭벗어남불량
			pView->m_bisYakYJ = DLData->cswParams.isYakYJ;					//약용접불량
			pView->m_bisGwaYJ = DLData->cswParams.isGwaYJ;					//과용접불량
			pView->m_bisGeuYJ = DLData->cswParams.isGeuYJ;					//그을음불량

			//TC
			if (!pView->m_bisTC || !pView->m_bisTC || !pView->m_bisTC_slatned || !pView->m_bisTC_CID)
				pView->m_nDLNgType = 0;// pView->m_nDLBadCount[0]++;
			//Tab
			else if (!pView->m_bisTAB || !pView->m_bisTABerror || !pView->m_bisETCerror) 
				pView->m_nDLNgType = 1;//pView->m_nDLBadCount[1]++;
			//Weld
			else if (!pView->m_bisYJ_fromTAB || !pView->m_bisYakYJ || !pView->m_bisGwaYJ || !pView->m_bisGeuYJ) 
				pView->m_nDLNgType = 2;//pView->m_nDLBadCount[2]++;
			//Dist
			else if (!pView->m_nDLResult)
				pView->m_nDLNgType = 3;//pView->m_nDLBadCount[3]++;

			if (!pView->m_bisTC || !pView->m_bisTC_frontback || !pView->m_bisTC_slatned || !pView->m_bisTC_CID || !pView->m_bisCAN || !pView->m_bisTAB || !pView->m_bisTABerror || !pView->m_bisETCerror || !pView->m_bisYJ_fromTAB || !pView->m_bisYakYJ || !pView->m_bisGwaYJ || !pView->m_bisGeuYJ)
				pView->m_nDLResult = 0;

			if (pView->m_pDlgSet->m_FormTab5->m_bDLSaveAll || pView->m_pDlgSet->m_FormTab5->m_bDLSaveNg) {
				memcpy(pView->m_DisplayDLOverlay.m_pImg->GetImagePtr(), DLData->overlayImageData, CAMERA_WIDTH * CAMERA_HEIGHT_960 * 3);
				if (pView->m_pDlgSet->m_FormTab5->m_bDLSaveAll)
					pView->m_DisplayDLOverlay.SaveDLImg(pView->m_nDLResult, pView->m_nGrabCount);
				else if (pView->m_pDlgSet->m_FormTab5->m_bDLSaveNg && pView->m_nDLResult == 0)
					pView->m_DisplayDLOverlay.SaveDLImg(pView->m_nDLResult, pView->m_nGrabCount);

				/*free(DLData->overlayImageData);
				free(DLData->overlayImageData);*/
			}
			int nOffset = 50;
			CRect rtDLCrop;
			rtDLCrop.SetRect(DLData->cswParams.YJcenter_x, DLData->cswParams.YJcenter_y - nOffset, DLData->cswParams.YJcenter_x, DLData->cswParams.YJcenter_y - nOffset);
			rtDLCrop.InflateRect(AI_IMAGE_WIDTH / 2, AI_IMAGE_HEIGHT / 2);
			rtDLCrop = pView->m_Project.CheckROI(rtDLCrop, CRect(0, 0, CAMERA_WIDTH, CAMERA_HEIGHT));
			if (rtDLCrop.Width() > AI_IMAGE_WIDTH || rtDLCrop.Height() > AI_IMAGE_HEIGHT)
				;
			else
				pView->m_pDlgAiView->ImageView2(pView->m_Project.m_MultiBuf[pView->m_pDlgSet->m_FormTab4->m_nAiFrame], rtDLCrop);
		}
		Sleep(1);
	}
	return 1;
}

UINT ThreadSaveNG(LPVOID lParam)
{
	CFlyCapture2_EXDlg *pView = (CFlyCapture2_EXDlg *)lParam;


	CString str,strName;
	while(pView->m_bRun)
	{		
		if(pView->m_bAuto)
		{
			while(WaitForSingleObject((HANDLE)pView->m_evNgSave, 50) != WAIT_TIMEOUT )
			{

				strName.Format("%d_NG",pView->m_nOKCount+pView->m_nNgCount);
				if(5.0 < pView->m_dRemainHDDSpace2){
					memcpy(pView->m_Display[0].m_pImgbuf->GetImagePtr(),pView->m_Project.m_MultiBuf[8],CAMERA_WIDTH*CAMERA_HEIGHT);

					pView->m_Display[0].SaveImgNG1(strName,0,pView->m_pDlgSet->m_FormTab2->m_bCsvSave, pView->m_pDlgSet->m_FormTab2->m_bImgReverse);
				}
			}
			Sleep(1);

		}
		Sleep(1);
	}
	return 1;
}

UINT ThreadDone(LPVOID lParam)
{
	CFlyCapture2_EXDlg *pView = (CFlyCapture2_EXDlg *)lParam;
	int nLine = pView->m_pDlgSet->m_FormTab2->m_nLineNumber +1;
	int nLane = pView->m_pDlgSet->m_FormTab2->m_nLaneNumber +1;
	int nVisionType = static_cast<int>(JsonVisionType::TC_WELDING);
	CString strVisionType = _keyVisionType[nVisionType];
	CString strServerIP = pView->m_pDlgSet->m_FormTab1->m_strServerIP;
	CString strImgUploadPath =  pView->m_strUploadStandByPath[static_cast<int>(JsonClassification::_INSPECTION)];
	CString strServerImgPath="";
	CString strDefectName="";
	CString strEqpID="";
	CString str,str1;
	BOOL bNg = FALSE;
	BOOL bDefectNg = FALSE;
	while(pView->m_bRun)
	{		
		if(pView->m_bAuto )
		{
			//save // fulln image
			if(pView->m_bInsDone)
			{
				pView->m_bInsDone = FALSE;
				bDefectNg = FALSE;

				bNg = pView->m_nRBResult;
				GetLocalTime(&pView->m_timeVisionOutput);
				str1.Format("%d",pView->m_nOKCount+pView->m_nNgCount);

				//server
				if (pView->m_pDlgSet->m_FormTab2->m_bUseServer) {
					if(bNg)
						strServerImgPath.Format(_T("%s\\NG\\"), pView->m_strPathServerImg);
					else
						strServerImgPath.Format(_T("%s\\OK\\"), pView->m_strPathServerImg);
					CreateDirectory(strServerImgPath,NULL);

					if(bNg && pView->m_nNgType == NG_TYPE_APPEARANCE )
						bDefectNg = TRUE;

					if(5.0 < pView->m_dRemainHDDSpace2)
					{

						nLine = pView->m_pDlgSet->m_FormTab2->m_nLineNumber +1;
						nLane = pView->m_pDlgSet->m_FormTab2->m_nLaneNumber +1;

						pView->m_DisplayServer[0].SaveServerImg(strVisionType, pView->m_strEqpID, nLane, 0, 0, bNg, strServerImgPath, pView->m_timeVisionOutput, strImgUploadPath, pView->m_strCellID, pView->m_strLotID);

						if(bDefectNg && pView->m_pDlgSet->m_FormTab2->m_bUseSaveCrop)
							strDefectName = pView->m_DisplayNgCrop.SaveCropImg(strVisionType, pView->m_timeVisionOutput, pView->m_strEqpID, pView->m_strLotID, pView->m_strCellID, nLane, bNg, pView->m_nNgCode, pView->m_nDefectPosX, pView->m_nDefectPosY);
						
						pView->WriteJsonInsp(0,pView->m_strPathServerJson[1], pView->m_timeVisionOutput, nLine, nLane, bDefectNg, strDefectName, bNg);

					}
					//reset
					pView->ResetDimensionData();
					
				}
			}
		}
		Sleep(1);
	}
	return 1;
}

UINT ThreadUploadData(LPVOID lParam)
{
	CFlyCapture2_EXDlg *pView = (CFlyCapture2_EXDlg *)lParam;
	CFileFind finder;
	bool bWorking = false;
	CString strPathSrc;
	CString strPathDst;
	CString strFileName;
	CString strTemp;
	CString str;
	int nIndex = 0;
	bool bIsConnected = false;
	CString strServerIP =  pView->m_pDlgSet->m_FormTab1->m_strServerIP;

	while(pView->m_bRun) {
		if (pView->m_bAuto) {
			if (pView->m_pDlgSet->m_FormTab2->m_bUseServer) {
				if (pView->PingCheck(strServerIP))
					bIsConnected = true;
				else
					bIsConnected = false;

				for (int i = 0; i < static_cast<int>(JsonClassification::_TOTALCOUNT); i++) {
					if (i == static_cast<int>(JsonClassification::_STATUS))
						continue;

					strPathSrc.Format(_T("%s\\*.*"), pView->m_strUploadStandByPath[i]);
					bWorking = finder.FindFile(strPathSrc);

					while (bWorking && bIsConnected && pView->m_pDlgSet->m_FormTab2->m_bUseServer) {
						bWorking = finder.FindNextFile();

						//파일 일때
						if (finder.IsArchived()) {
							strFileName = finder.GetFileName();
							strPathSrc = finder.GetFilePath();

							if (i == static_cast<int>(JsonClassification::_INSPECTION)) {
								nIndex = strFileName.ReverseFind('.');
								strTemp = strFileName.Mid(nIndex + 1);
								if (strTemp == _T("jpg")) {
									nIndex = strFileName.ReverseFind('_');
									strTemp = strFileName.Left(nIndex);
									str = strTemp.Right(2);
									if (str != _T("OK") && str != _T("NG"))
										str = _T("NG"); // Crop 이미지는 항상 NG 폴더

									nIndex = strFileName.Find("_") + 2;
									strPathDst.Format(_T("\\\\%s\\DAT\\%s\\IMG\\%s\\%s\\%s\\%s\\%s\\"), strServerIP, strFileName.Left(nIndex - 2), _keyCategory[i], str, strFileName.Mid(nIndex + 1, 6), strFileName.Mid(nIndex + 7, 2), strFileName.Mid(nIndex + 9, 2));

									str = strFileName;
									for (int i = 0; i < 3; i++) {
										strTemp = str.Mid(nIndex + 1);
										nIndex = strTemp.Find("_");
										str = strTemp;
									}

									nIndex = strTemp.Find("_");
									str = strTemp.Left(nIndex);
									strPathDst = strPathDst + str + _T("\\");

									int bIsSucceed = pView->MakeDirectory(strPathDst);
									if (bIsSucceed) {
										strPathDst += strFileName;
										if (bIsConnected && pView->PingCheck(strServerIP)) {
											if (CopyFile(strPathSrc, strPathDst, false) != 0)
												DeleteFile(strPathSrc);
										}
										else {
											bIsConnected = false;
										}
									}
									else {
										bIsConnected = false;
									}
								}
								else if (strTemp == _T("json")) {
									nIndex = strFileName.Find("_") + 2;
									strPathDst.Format(_T("\\\\%s\\DAT\\%s\\JSON\\%s\\%s\\%s\\%s\\"), strServerIP, strFileName.Left(nIndex - 2), _keyCategory[i], strFileName.Mid(nIndex + 1, 6), strFileName.Mid(nIndex + 7, 2), strFileName.Mid(nIndex + 9, 2));

									str = strFileName;
									for (int i = 0; i < 4; i++) {
										strTemp = str.Mid(nIndex + 1);
										nIndex = strTemp.Find("_");
										str = strTemp;
									}

									nIndex = strTemp.Find("_");
									str = strTemp.Left(nIndex);
									strPathDst = strPathDst + str + _T("\\");

									int bIsSucceed = pView->MakeDirectory(strPathDst);
									if (bIsSucceed) {
										strPathDst += strFileName;
										if (bIsConnected && pView->PingCheck(strServerIP)) {
											if (CopyFile(strPathSrc, strPathDst, false) != 0)
												DeleteFile(strPathSrc);
										}
										else {
											bIsConnected = false;
										}
									}
									else {
										bIsConnected = false;
									}
								}
							}
							else if (i == static_cast<int>(JsonClassification::_SPEC_PARA)) {
								strPathDst.Format(_T("%s%s"), pView->m_strPathServerJson[i], strFileName);
								if (bIsConnected && pView->PingCheck(strServerIP)) {
									if (CopyFile(strPathSrc, strPathDst, false) != 0)
										DeleteFile(strPathSrc);
								}
								else {
									bIsConnected = false;
								}
							}
							else if (i == static_cast<int>(JsonClassification::_ALARM)) {
								;
							}
						}

						Sleep(1);
					}
				}
			}

			Sleep(100);
		}
	}
	return 0;
}

UINT ThreadSaveOverlay(LPVOID lParam)
{
	CFlyCapture2_EXDlg *pView = (CFlyCapture2_EXDlg *)lParam;

	CString strlog;
	CString str,str1;
	while(pView->m_bRun)
	{		

		if(pView->m_bAuto)
		{
			//save
			while(WaitForSingleObject((HANDLE)pView->m_eSaveCapture, 50) != WAIT_TIMEOUT )
			{
				pView->m_bCaptureSaving = TRUE;
				pView->capture();
				pView->m_bCaptureSaving = FALSE;
			}
		}
		Sleep(5);
	}
	return 1;
}

UINT ThreadCPUCheck(LPVOID lParam)
{
	CFlyCapture2_EXDlg *pView = (CFlyCapture2_EXDlg *)lParam;

	int nn = 0;
	int nCom=0;
	if(pView->m_strComname.Find("A")>=0)
		nCom=74;
	else if(pView->m_strComname.Find("B")>0)
		nCom=75;
	else
		nCom=76;
	while(0)//pView->m_bRun)
	{		
		if(pView->m_bAuto)
		{

			WSADATA wsadata;
			CString strIP,strIP2; // 이 변수에 IP주소가 저장된다. WOO
			strIP = "";

			if( !WSAStartup( DESIRED_WINSOCK_VERSION, &wsadata ) )
			{
				if( wsadata.wVersion >= MINIMUM_WINSOCK_VERSION )
				{
					HOSTENT *p_host_info;
					IN_ADDR in;
					char host_name[128]={0, };

					gethostname(host_name, 128);
					p_host_info = gethostbyname( host_name );

					if( p_host_info != NULL )
					{
						for( int i = 0; p_host_info->h_addr_list[i]; i++ )
						{
							memcpy( &in, p_host_info->h_addr_list[i], 4 );
							pView->m_pDlgSet->m_FormTab2->m_strIPCheck = inet_ntoa( in );
							if(pView->m_pDlgSet->m_FormTab2->m_strIPCheck.Find("192.168.80.7")>=0)
								break;
						}
					}
				}
				WSACleanup();
			}
			pView->m_pDlgSet->m_FormTab2->m_strIPCheck2.Format("192.168.80.%d",nCom);
		}
	}
	

	return 1;
}
//
UINT ThreadInspectCheck(LPVOID lParam)
{
	CFlyCapture2_EXDlg *pView = (CFlyCapture2_EXDlg *)lParam;

	while(pView->m_bRun)
	{		
		if(pView->m_bAuto && pView->m_pDlgSet->m_FormTab2->m_bUseRtos && pView->m_bResultIP == TRUE)
		{
			pView->m_bResultIP=FALSE;
			int nResult=0;
			for(int i=1;i<20;i++){
				if (pView->m_bResult[i]==TRUE){
					nResult = i;
					break;
				}
			}

			pView->m_nCSVCount++;
			pView->capture2();
			pView->WriteTextData(nResult);//csv파일 저장 201103 woo	
		}
	}
	Sleep(1);

	return 1;
}

UINT ThreadCamStatus(LPVOID lParam)
{
	CFlyCapture2_EXDlg *pView = (CFlyCapture2_EXDlg *)lParam;
	bool bIsConnected = false;
	CString strCamIP = "192.168.11.11";
	static int nDisConnectCount = 0;
	bool bCamStatus;
	while (true) {
		if (pView->m_bAuto) {
			bCamStatus = pView->m_Vision[0]->CamStatus();

			if (bCamStatus == 0) { //connect ok
				strCamIP = strCamIP;
			}
			else { //connect error
				pView->m_IO.m_bPgmAuto = FALSE;
				if (IDOK == AfxMessageBox(_T("카메라 연결 끊김 감지. \n카메라 케이블 확인 및 검사기 재시작 필요.")))
				{
					Sleep(1);
					break;
				}
			}
		}
		Sleep(500);
	}
	return 0;
}


UINT ThreadStatus(LPVOID lParam)
{
	CFlyCapture2_EXDlg *pView = (CFlyCapture2_EXDlg *)lParam;
	bool bIsConnected = false;

	while (true) {
		if (pView->m_bAuto && pView->m_pDlgSet->m_FormTab2->m_bUseServer) {
			if (pView->PingCheck(pView->m_pDlgSet->m_FormTab1->m_strServerIP)) {
				if (bIsConnected == false) {
					for (int nCam = 0; nCam < MAX_CAMERAS; nCam++)
						pView->AddtoList(_T("Server Connect"),0);
				}
				bIsConnected = true;
				pView->WriteJsonStatus(8, pView->m_pDlgSet->m_FormTab2->m_nLaneNumber +1);
				Sleep(1000 * 9);
			}
			else {
				if (bIsConnected) {
					for (int nCam = 0; nCam < MAX_CAMERAS; nCam++)
						pView->AddtoList( _T("Server Disconnect"),0);
				}
				bIsConnected = false;
			}
		}
		Sleep(1000 * 1);
	}
}

// CFlyCapture2_EXDlg 메시지 처리기
BOOL CFlyCapture2_EXDlg::OnInitDialog()
{
	//WinExec("C:\\Users\\TFX5470UC\\Desktop\\DAP-VIM_Release\\DAP-VIM.exe",SW_SHOW);

	CDialog::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다. 응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	//m_ctMainImg.MoveWindow(FORM_START_X,FORM_START_Y,CAMERA_WIDTH,CAMERA_HEIGHT);

	ShowWindow(SW_MAXIMIZE);
	MoveWindow(CRect(0,0,1280,1024));

	m_nType = 2;//testing
	//Usb_Qu_Open();  //Lamp COnnect
	m_strComname = ComputerName();
	CheckMachine();	
	m_Display[0].m_nMachine = m_nMachine;



	AddtoList("Start - Init Control",1);
	InitDisplay();
	AddtoList("Start - Init Display",1);
	InitClass();//cgproject // dialog
	AddtoList("Start - Init Class",1);
	//if(m_nMachine ==8)
	//	InitSocket(); //woo socket
	//m_pDlgSet->UpdateIni(m_strModel,TRUE);

	InitIO();//io init
  	AddtoList("Start - Init IO",1);
	InitCam();//matrox board
	AddtoList("Start - Init Cam",1);
	m_Project.InitProject(this);	//InitDisplay 보다 뒤에 와야한다. //display buffer와 검사 buffer size matching
	AddtoList("Start - Init Project",1);
	//m_pDlgSet->m_FormTab5->OpenPlc2(0);
	LabelResult(-1, AI_READY);

	m_dScaleFactorX = m_pDlgSet->m_dScaleFactor;
	InitControl();


	if (m_pDlgSet->m_FormTab1->m_bUseDeepLearning) {
		returnparams[0] = new ReturnParams();
		returnparams[0]->overlayImageData = new unsigned char[CAMERA_WIDTH * CAMERA_HEIGHT_960 * 3];
		if (DAP_Inference_DLL == NULL) {
			AddtoInspLog("DL Init Ng");
		}
		else {
			AddtoInspLog("DL Init Ok");
			DAP_Initialize_Func();		
			SetAiSpec();
		}
		
	}
	
	if(m_pDlgSet->m_FormTab2->m_bRed)
		m_btModel.SetFaceColor(COLOR_RED);
	else if(m_pDlgSet->m_FormTab2->m_bGreen)
		m_btModel.SetFaceColor(COLOR_GREEN);
	else if(m_pDlgSet->m_FormTab2->m_bBlue)
		m_btModel.SetFaceColor(COLOR_BLUE);
	else if(m_pDlgSet->m_FormTab2->m_bYellow)
		m_btModel.SetFaceColor(COLOR_YELLOW);
	else if(m_pDlgSet->m_FormTab2->m_bWhite)
		m_btModel.SetFaceColor(COLOR_WHITE);

	if(m_strComname.Right(1)=="A")m_nPcName = 1;//left
	else if(m_strComname.Right(1)=="B")m_nPcName = 2;//center
	else m_nPcName = 3;//right

	m_nMasterMode = 0;
	AddtoList("Program Start",1);

	m_pDlgSet->m_FormTab3->m_strComname = m_strComname;
	AfxBeginThread(ThreadDLIns, this,THREAD_PRIORITY_NORMAL,0);
	AfxBeginThread(ThreadInspect, this,THREAD_PRIORITY_NORMAL,0);//
	AfxBeginThread(ThreadSaveOverlay, this,THREAD_PRIORITY_NORMAL,0);//
	AfxBeginThread(ThreadSaveNG, this,THREAD_PRIORITY_NORMAL,0);//
	AfxBeginThread(ThreadDone, this,THREAD_PRIORITY_NORMAL,0);//
	AfxBeginThread(ThreadInspectCheck, this,THREAD_PRIORITY_NORMAL,0);
	AfxBeginThread(ThreadMultiFullSave, this,THREAD_PRIORITY_NORMAL,0);
	AfxBeginThread(ThreadStatus, this,THREAD_PRIORITY_NORMAL,0);
	AfxBeginThread(ThreadCamStatus, this, THREAD_PRIORITY_NORMAL, 0);
	AfxBeginThread(ThreadAutoInspect, this,THREAD_PRIORITY_NORMAL,0);

	
	//io 초기화
	m_Project.m_dAnglepol = 0;
	CheckHdd();

	if (m_dRemainHDDSpace2<m_pDlgSet->m_FormTab3->m_nRemainHddSpace && m_bCheckHdd==FALSE)
	{
		CString str123;
		str123.Format("Hdd Check %.1f%%",m_dRemainHDDSpace2);
		AddtoList(str123,1);
		HddAlarm(0);
		SetTimer(1456,1000,NULL);
	}

//	InitBadInfo();
	SetTimer(1412,1000,NULL);//시간
	SetTimer(131,1000,NULL);//hdd

//	SetTimer(1416,100,NULL);
	m_dwWite = GetTickCount();

	InitBadInfo();

	BadNgCount(0, RET_OK, RET_OK);
	BadNgCount(1, RET_OK, RET_OK);

	for(int i=0;i<10;i++)m_nMakeAlphachannel[i] = 0;
	DataReset();
	
	m_strVersion.Format("Ver.23.1.31 2023-04-18");
	//OnBnClickedStart();

	if(m_nMachine ==2){
		if(m_strComname.Find("3A")>=0 || m_strComname.Find("3B")>=0 ||m_strComname.Find("3C")>=0 
			|| m_strComname.Find("4A")>=0 || m_strComname.Find("4B")>=0 ||m_strComname.Find("4C")>=0
			|| m_strComname.Find("5A")>=0 || m_strComname.Find("5B")>=0 ||m_strComname.Find("5C")>=0
			|| m_strComname.Find("6A")>=0 || m_strComname.Find("6B")>=0 ||m_strComname.Find("6C")>=0
			|| m_strComname.Find("7A")>=0 || m_strComname.Find("7B")>=0 ||m_strComname.Find("7C")>=0
			|| m_strComname.Find("8A")>=0 || m_strComname.Find("8B")>=0 ||m_strComname.Find("8C")>=0){
			m_pDlgSet->m_FormTab3->OnBnClickedButtonLightTurnOnCh1();
			m_pDlgSet->m_FormTab3->OnBnClickedButtonLightTurnOnCh2();
		}
		m_IO.WriteOutputPort(0,1);//ready
	}

	CString strtmp;
	strtmp.Format("d:\\NAS\\");
	CreateDirectory(strtmp,NULL);
	strtmp.Format("d:\\NAS\\DATA\\");
	CreateDirectory(strtmp,NULL);
	strtmp.Format("d:\\NAS\\DATA\\IMAGE\\");
	CreateDirectory(strtmp,NULL);

	strtmp.Format("c:\\Data\\");
	CreateDirectory(strtmp, NULL);

	SetEqipID(m_pDlgSet->m_FormTab2->m_nLineNumber +1, m_pDlgSet->m_FormTab2->m_nLaneNumber +1);
	
	CString strVersion;
	m_lbViewData[0].SetText(strVersion);
	m_btVersion.SetWindowTextA(m_strVersion);

	m_bStarting = TRUE;

	if (m_pDlgSet->m_FormTab4->m_bFrame)
		m_nMaxFrame = 10;

	InitDirectory();

	if (m_pDlgSet->m_FormTab2->m_bUseServer) 
		if (PingCheck(m_pDlgSet->m_FormTab1->m_strServerIP))
			WriteJsonSpecPara(0, 1, m_pDlgSet->m_FormTab2->m_nLaneNumber +1);

	AfxBeginThread(ThreadUploadData, this,THREAD_PRIORITY_NORMAL,0);

	GetDlgItem(IDC_MFCBUTTON_START)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_MFCBUTTON_STOP)->ShowWindow(SW_HIDE);

	//셀추적
	CellServerInit();

	if (m_strStats.Find("Error") == -1)
		OnBnClickedMfcbuttonStart();

	ResetDimensionData();
	//m_Vision[0]->m_pBaslerCamera->IsCameraDeviceRemoved();
	//6호기 크래비스
	if(m_pDlgSet->m_FormTab2->m_nLineNumber + 1 == 6){
		m_nAiImageWidth = 120;
		m_nAiImageHeight = 80;
		m_strAiOk = _T("OK");
		m_strAiNG = _T("NG");
		m_strAiPASS = _T("PASS");
	}
	m_pDlgSet->m_FormTab3->m_LightCtrl.LightOnVisioncowork(m_pDlgSet->m_FormTab3->m_nLightValueCh1, 1);

	/*returnparams[0] = new ReturnParams();
	SetAiSpec();*/

	return FALSE;//TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}
void CFlyCapture2_EXDlg::SetAiSpec()
{
	CFormTab5* pFormTab5 = m_pDlgSet->m_FormTab5;
	
	returnparams[0]->okNgParams.scale = pFormTab5->m_nDLScale;
	returnparams[0]->okNgParams.cap_canX = pFormTab5->m_dDLSpecTcCanX[1];
	returnparams[0]->okNgParams.cap_canY_lower = pFormTab5->m_dDLSpecTcCanY[0];
	returnparams[0]->okNgParams.cap_canY_upper = pFormTab5->m_dDLSpecTcCanY[1];
	returnparams[0]->okNgParams.tab_corner_gapY = pFormTab5->m_dDLSpecTabEdgeGapY[1];
	returnparams[0]->okNgParams.weld_canY_lower = pFormTab5->m_dDLSpecWeldCanY[0];
	returnparams[0]->okNgParams.weld_canY_upper = pFormTab5->m_dDLSpecWeldCanY[1];
	returnparams[0]->okNgParams.weld_capY_lower = pFormTab5->m_dDLSpecTcWeldY[0];
	returnparams[0]->okNgParams.weld_capY_upper = pFormTab5->m_dDLSpecTcWeldY[1];
	returnparams[0]->okNgParams.tccenter_tabedgeY = pFormTab5->m_dDLSpecTcTabY[1]; 
	returnparams[0]->okNgParams.tccenter_tabedgeX_left = pFormTab5->m_dDLSpecTcTabLeft[1];
	returnparams[0]->okNgParams.tccenter_tabedgeX_right = pFormTab5->m_dDLSpecTcTabRight[1];
	returnparams[0]->okNgParams.strongwelding_area_min = pFormTab5->m_dDLSpecStrongWeldArea[1];
	returnparams[0]->okNgParams.strongwelding_length_min = pFormTab5->m_dDLSpecStrongWeldLength[1];
	returnparams[0]->okNgParams.soot_area_min = pFormTab5->m_dDLSpecSootWeldArea[1];
	returnparams[0]->okNgParams.soot_length_min = pFormTab5->m_dDLSpecSootWeldLength[1];
	returnparams[0]->okNgParams.welding_length_min = pFormTab5->m_dDLSpecWeldLength[1];
	returnparams[0]->okNgParams.welding_width_min = pFormTab5->m_dDLSpecWeldArea[1];
}

void CFlyCapture2_EXDlg::DeleteAllFile( CString strDir ) 
{ 
	CString strName; 
	strName.Format( "%s\\*.*", strDir ); 
	CFileFind ff; 
	BOOL bFind = ff.FindFile( strName ); 
	while( bFind ) { 
		bFind = ff.FindNextFile(); 
		if( ff.IsDots() == TRUE || ff.IsDirectory() == TRUE ) 
			continue; 
		DeleteFile( ff.GetFilePath() ); 
	} 
	ff.Close(); 
}

void CFlyCapture2_EXDlg::InitSocket(void)
{
	bool bConnection = true;
	//m_pSocConnect = new CConnect(this);


// 	if (false) {
// 		//	if(m_pDlgTab3->m_bServer == TRUE){
// 		//서버
// 		//소켓생성-서버
// 		//m_pSocConnect->CreateSocketModeServer(this);
// 	}
// 	else {
// 		//클라이언트
// 		//소켓생성-클라이언트
// 		//	m_pSocConnect->CreateSocketModeClient(this);
// 	}
	
	// 클라이언트
	bConnection &= m_pSocHunterlapTop->CreateSocketModeClient(this, _T("192.168.80.90"), 0);  //지범
	
	if(bConnection)
		m_bConnectionStauts[4] = true;
	
}

LPARAM CFlyCapture2_EXDlg::OnReceive(WPARAM wParam, LPARAM lParam)
{
	CString recvData;
	char pTmp[RECV_PACKET_LENGTH];
	int recvDataLength;	
	int nIndex = (int)wParam;
	double dL = 0.0, dA = 0.0, dB = 0.0;


	// 	if(m_pDlgTab3->m_bServer == TRUE){
	// 		recvDataLength = m_connect->m_socService->m_socServer[nIndex]->m_pSocCom->Receive(pTmp, RECV_PACKET_LENGTH);
	// 	}
	// 	else
	{
		recvDataLength = m_pSocHunterlapTop->m_socService->m_socClient->Receive(pTmp, RECV_PACKET_LENGTH); //상부
	}

	//받은데이터 크기만큼 string 형으로 만듬
	//recvData = CharToString(pTmp, recvDataLength);
	//recvDataLength=300;
	//memset(pTmp+recvDataLength,0,RECV_PACKET_LENGTH-recvDataLength);
	memset(pTmp+recvDataLength,0,RECV_PACKET_LENGTH-recvDataLength);
	CString strRecv,strlog, strMergedFullData;
	strRecv.Format(_T("%s"),pTmp);

	SYSTEMTIME new_time;
	SetSystemTimePrivilege();
	GetLocalTime(&new_time);

	CString strY,strM,strD,strH,strMinute,strSecond;
	strY.Format(_T("%c%c%c%c"),pTmp[14],pTmp[15],pTmp[16],pTmp[17]);
	new_time.wYear=atoi(strY);;//(char)strY;//strRecv.Mid(14,4);

	strM.Format(_T("%c%c"),pTmp[19],pTmp[20]);
 	new_time.wMonth=atoi(strM);

	strD.Format(_T("%c%c"),pTmp[22],pTmp[23]);
 	new_time.wDay=atoi(strD);

	strH.Format(_T("%c%c"),pTmp[25],pTmp[26]);
 	new_time.wHour=atoi(strH);

	strMinute.Format(_T("%c%c"),pTmp[28],pTmp[29]);
 	new_time.wMinute=atoi(strMinute);

	strSecond.Format(_T("%c%c"),pTmp[31],pTmp[32]);
 	new_time.wSecond=atoi(strSecond);
	
	SetLocalTime(&new_time);

	//m_pDlgTab3->Decoding(strRecv);
	//	strlog.Format(_T("[%s]:%s"), m_pDlgConnect->m_pConnection[nIndex].name, pTmp);
	if(MergeSocketData(strRecv, strMergedFullData)){


	}
	//	AddtoList(strlog);

	//TRACE(strRecv);

	//AddtoList(strRecv);

	//	Decoding(recvData);

	return TRUE;
}
bool CFlyCapture2_EXDlg::MergeSocketData( CString strSocketData, CString& strMergedData )
{
	static CString strMerge[2]={_T(""),};
	static int nCnt = 0;
	int nIndex = 0;
	static bool bStart = true;
	static enum { eStrOld, eStrNew};
	bool bRet = true;

	if(bStart){
		if((nIndex=strSocketData.Find("0x02")) >= 0){
			strMerge[eStrOld] += strSocketData.Mid(nIndex,strSocketData.GetLength()-nIndex);
			bStart = false;
		}	
		bRet = false;
	}
	else{
		if((nIndex=strSocketData.Find("")) >= 0){
			strMerge[eStrOld] += strSocketData.Mid(0,nIndex);		// str1
			strMerge[eStrNew] = strSocketData.Mid(nIndex,strSocketData.GetLength()-nIndex); // str2
			if(strMerge[eStrOld].Compare(_T(""))!=0)
			{
				strMergedData = strMerge[eStrOld];
				strMerge[eStrOld] = strMerge[eStrNew];
				bRet = true;
			}
		}
		else{
			strMerge[eStrOld] += strSocketData;
			bRet = false;
		}
	}

	return bRet;
}
int CFlyCapture2_EXDlg::getIsFocus()
{
	CEdit* pFocus = (CEdit*)GetDlgItem(IDC_EDIT_BARCODE);
	CEdit* pIsFocus = (CEdit*)GetFocus();
	
	if(pFocus == pIsFocus)
		return 0;
	else
		return 1;
}

void CFlyCapture2_EXDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다. 문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CFlyCapture2_EXDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	CRect rect;
	GetClientRect(&rect);
	CBrush myBrush(COLOR_BG); // dialog background color.

	CBrush *pOld = dc.SelectObject(&myBrush);
	BOOL bRes = dc.PatBlt(0, 0, rect.Width(), rect.Height(), PATCOPY);
	dc.SelectObject(pOld); // restore old brush

	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;
		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
//		if(m_bAuto)
		if(m_pDlgSet != NULL)
			DrawDisplay2(0);

		CDialog::OnPaint();
	}
	

}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CFlyCapture2_EXDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

UINT CFlyCapture2_EXDlg::LoopGrab(int nCam,unsigned char* fm)
{
	CTime ttime;
	int nGrabCnt = 0;
	double d1 =  m_Project.GetPrecisionTime();
	if (m_Project.m_nMultiFrame == 0) {
		m_nDLResult = RET_INIT; //DL 초기화. 검사종료 후 초기화 시 딥러닝검사 늦게 끝나는경우 꼬일가능성.
		m_dTime[0] = m_dTime[1] = m_dTime[2] = m_dTime[3] = m_dTime[4] = 0.0;
		m_dTime[0] = d1;
	}
	double dGrabGap = (d1-m_dLastTime)*1000;
	
	CString strmsg;
	if(m_Project.m_nMultiFrame >0 && dGrabGap > 100)//종우야 여기 확인해봐 ??
	{
		m_bResult[1] = TRUE;
		m_nTimeCheckCount++;
		strmsg.Format("Time Check %d , %s, %.1f, %d, %d",(m_nOKCount+m_nNgCount),m_strFolderName, dGrabGap, m_Project.m_nMultiFrame, m_nTimeCheckCount);
		m_IO.m_nBadResult1 = m_IO.m_nBadResult2 = 1;//OK

		AddtoList(strmsg,1);
		m_Project.m_nMultiFrame = 0;
	}
	//else if(dGrabGap>200){
	//	strmsg.Format("Cycle Check %d , %s, %.1f, %d, %d",(m_nOKCount+m_nNgCount),m_strFolderName, dGrabGap, m_Project.m_nMultiFrame, m_nTimeCheckCount);
	//	AddtoList(strmsg,1);
	//	m_Project.m_nMultiFrame = 0;

	//}
	m_dLastTime = d1;
	m_Project.m_dInspectTime[1] = d1;//dw1;
	if(m_nMachine==2 || m_nMachine==3)
		m_IO.m_bGrabComplete = TRUE;
	if(m_Project.m_nMultiFrame==0 ){
		m_nCanCheckAve=0;
	}
	if(m_Project.m_nMultiFrame>=19)
		m_Project.m_nMultiFrame = 0;

	 if(m_Project.m_nMultiFrame <m_nMaxFrame)
		m_Vision[nCam]->SetExpose(m_pDlgSet->m_FormTab2->m_nMultiShutter[m_Project.m_nMultiFrame+1]);

//	BYTE * fmCamdisp = m_CamDisplay->m_OriginDisp->GetImagePtr();
	int TargetCam = 0;
	
	BOOL checkTh1=TRUE;

	if(m_pDlgSet->m_FormTab4->m_bImageReverseX)
	{
		if(m_pDlgSet->m_FormTab4->m_bImageReverseY)
		{
			for(int i=0;i<CAMERA_HEIGHT;i++)
			{
				for(int j=0;j<CAMERA_WIDTH;j++)
					*(m_Project.m_MultiBuf[m_Project.m_nMultiFrame]+CAMERA_WIDTH*(CAMERA_HEIGHT-i-1)+(CAMERA_WIDTH-j-1)) = *(fm+i*CAMERA_WIDTH+j);
				//*(m_Project.m_fm[TargetCam]+CAMERA_WIDTH*(CAMERA_HEIGHT-i-1)+(CAMERA_WIDTH-j-1)) = *(fm+i*CAMERA_WIDTH+j);
			}
		}
		else
		{
			for(int i=0;i<CAMERA_HEIGHT;i++)
			{
				memcpy(m_Project.m_MultiBuf[m_Project.m_nMultiFrame]+CAMERA_WIDTH*(CAMERA_HEIGHT-i-1),fm+i*CAMERA_WIDTH,CAMERA_WIDTH);
				//memcpy(m_Project.m_fm[TargetCam]+CAMERA_WIDTH*(CAMERA_HEIGHT-i-1),fm+i*CAMERA_WIDTH,CAMERA_WIDTH);
			}
	//		memcpy(m_CamDisplay->m_OriginDisp->GetImagePtr(),fm,CAMERA_WIDTH*CAMERA_HEIGHT);
		}
	}
	else
	{
		if(checkTh1)
		{
			memcpy(m_Project.m_MultiBuf[m_Project.m_nMultiFrame],fm,CAMERA_WIDTH*CAMERA_HEIGHT);
//			memcpy(m_Project.m_fm[TargetCam],fm,CAMERA_WIDTH*CAMERA_HEIGHT);
// 			for(int i=0;i<CAMERA_HEIGHT;i++){
// 				memcpy(fmCamdisp+CAMERA_WIDTH*(CAMERA_HEIGHT-i-1),fm+i*CAMERA_WIDTH,CAMERA_WIDTH);
// 			}
		}
		else
		{
			for(int i=0;i<CAMERA_HEIGHT;i++){
				for(int j=0;j<CAMERA_WIDTH;j++)
					*(m_Project.m_MultiBuf[m_Project.m_nMultiFrame]+CAMERA_WIDTH*(CAMERA_HEIGHT-i-1)+(CAMERA_WIDTH-j-1)) = *(fm+i*CAMERA_WIDTH+j);
//				*(m_Project.m_fm[TargetCam]+CAMERA_WIDTH*(CAMERA_HEIGHT-i-1)+(CAMERA_WIDTH-j-1)) = *(fm+i*CAMERA_WIDTH+j);
			}
		}
	}

	double d2 = m_Project.GetPrecisionTime();

	m_Project.m_dInspectTime[2] = d2;//dw2;

	int nmode = 0;

	CRect rtchek;
	if(m_nMachine==1)//1 오창  2,남경1호 3남경2
		rtchek.SetRect(240,940,500,957);
	else if(m_nMachine==3){
		rtchek.SetRect(200,900,1000,950);
		nmode = 2;
	}
	else{
		//rtchek.SetRect(600,900,680,950);
		rtchek = m_Display[0].m_rtImgTracker[0];
		nmode = 2;
	}
	m_Project.m_rtCanCheckROI = rtchek;
	double dtmp1 = 0;
	if(m_Project.m_nMultiFrame==9){
		rtchek = m_Project.CheckROI(rtchek,CRect(10,10,CAMERA_WIDTH-10,CAMERA_HEIGHT-10));
		m_Project.m_rtCanCheckROI = rtchek;
		m_nCanCheckAve = m_Project.AreaAverage2(nmode,m_Project.m_MultiBuf[m_Project.m_nMultiFrame],CAMERA_WIDTH,rtchek.left,rtchek.top,rtchek.right,rtchek.bottom,15,&m_nCanBlackCount,&dtmp1);

		if(m_pDlgSet->m_FormTab2->m_bCompleteOnOff){// woo 201006체크 되어있으면 Ready신호를 Complete처럼 사용 (성호야 봐라이거추가했다)
			m_IO.WriteOutputPort(2,FALSE);	
		}

	
	}

	double d3 = m_Project.GetPrecisionTime();

	m_Project.m_dInspectTime[3] = d3;
	CString str;
	str.Format("DrawDisplay %d, %.2f",m_Project.m_nMultiFrame,(d3-d1)*1000);
	//AddtoInspLog(str);
	
	if(m_pDlgSet->m_FormTab1->m_bUseDeepLearning && m_pDlgSet->m_FormTab4->m_bUseAiIns && m_Project.m_nMultiFrame == m_pDlgSet->m_FormTab4->m_nAiFrame){
		//딥러닝검사
		std::thread ThreadLoop(ThreadInsDL, this, nCam, m_Project.m_MultiBuf[m_Project.m_nMultiFrame]);
		ThreadLoop.detach();
	}

	if(m_Project.m_nMultiFrame == 7)
		memcpy(m_DisplayServer[TargetCam].m_pImg->GetImagePtr(),m_Project.m_MultiBuf[m_Project.m_nMultiFrame],CAMERA_WIDTH*CAMERA_HEIGHT);


	m_Project.m_nMultiFrame++;
	//save시 한장 빼줘야함
	if(m_Project.m_nMultiFrame >= m_nMaxFrame)
	{
		m_nGrabCount++;
		GetLocalTime(&m_timeVisionInput);
		GetLocalTime(&m_timeInspStart);

		m_dTime[1] = m_Project.GetPrecisionTime();
		double dGrabTime = (m_dTime[1] - m_dTime[0]) * 1000;
		if (dGrabTime < 0) dGrabTime = 0;
		str.Format(_T("Grab Complete_%d[%.1f]"), m_nGrabCount, dGrabTime);
		AddtoInspLog1(str, 1);

		if (m_nCanCheckAve > m_pDlgSet->m_FormTab4->m_nCheckCanValue)
			m_Project.m_nFrameCount++;

		//tact time check
		if (m_dTactTimeOld > 0) {
			m_dTactTime = (m_Project.GetPrecisionTime() - m_dTactTimeOld);
			m_dTactTimeOld = m_Project.GetPrecisionTime();
		}
		else
			m_dTactTimeOld = m_Project.GetPrecisionTime();

		m_Project.m_nMultiFrame = 0;
		m_Vision[nCam]->SetExpose(m_pDlgSet->m_FormTab2->m_nMultiShutter[0]);
		CTime ttime = CTime::GetCurrentTime();
		CString kk1 = ttime.Format("%Y%m%d_%H%M%S");
		m_strFolderName.Format("%s_%d",kk1,m_nOKCount+m_nNgCount);//m_Project.m_nFrameCount);
		//m_strFolderName = kk1;
		int nv1 = m_pDlgSet->m_FormTab4->m_nCheckCanValue;
		
		if(m_bAuto && m_nCanCheckAve<nv1 && m_nFrameCheck==0)		
			m_evInsp[0].SetEvent();
		else if(m_bAuto && m_nCanCheckAve>nv1){
			memcpy(m_Display[TargetCam].m_pImg->GetImagePtr(),m_Project.m_MultiBuf[m_Project.m_nMultiFrame],CAMERA_WIDTH*CAMERA_HEIGHT);
			m_IO.m_nBadResult1 = 2;
			m_IO.m_nBadResult2 = 1; // ALL NG
			m_IO.m_bInspComplete = TRUE; //io send

			m_bInsComplete = TRUE;
			m_nBadCount[0]++;
			m_nRBResult = m_nNgCode = NG_NOT_CAN;
			LabelResult(m_nRBResult, AI_RET_PASS);
			BadNgCount(1, TRUE, TRUE);
			DrawDisplay2(0);

			//GetLocalTime(&m_timeInspEnd);
			//m_evNgSave.SetEvent();
			//m_bInsDone = true;
		}
	}
	return 0;
}

int CFlyCapture2_EXDlg::MakeAlphaImage(BYTE * fmsrc,BYTE * fmdst,int nFrame)
{
	//
	return 0;
}

UINT CFlyCapture2_EXDlg::LoopIO(bool* bpData)
{
	static BOOL bPing1=FALSE;
	static BOOL bPing2=FALSE;
	static BOOL bPing3 = FALSE;
	static BOOL bPing4 = FALSE;

	//IO IN 0
	if(m_bAuto && m_IO.m_bInputData[0] && bPing1==FALSE && m_bInsComplete == TRUE)//검사정상적으로 끝났을때
	{
		m_nIOTriCount++;
		CString strIoTriCount;
		bPing1 = TRUE;
		strIoTriCount.Format(_T("IO Tri Count %d"),m_nIOTriCount);
		AddtoList(strIoTriCount,0);
		m_bInsComplete = FALSE;
	}
	else if(m_bAuto && m_IO.m_bInputData[0] && bPing1==FALSE && m_bInsComplete == FALSE) //검사안끝났을때
	{
		m_nIOTriErrCount++;
		CString strTriCheck;
		bPing1 = TRUE;
		strTriCheck.Format(_T("Trigger Io In Overlab_%d(%d)"), m_nIOTriCount, m_nIOTriErrCount);
		AddtoInspLog1(strTriCheck, 1);
	}
	if(m_IO.m_bInputData[0]==FALSE)bPing1 = m_IO.m_bInputData[0];

	//IO IN 1
	if(m_bAuto && m_IO.m_bInputData[1] && bPing2==FALSE && m_nMachine==2)
	{
		bPing2 = TRUE;

	}
	if(m_IO.m_bInputData[1]==FALSE)bPing2 = m_IO.m_bInputData[1];

	//IO IN 2
	if(m_bAuto && m_IO.m_bInputData[2] && bPing3==FALSE && m_nMachine==2)
	{
		bPing3 = TRUE;
		CString strComplete;
		double dComplete = 0.0;

		m_dTime[5] = m_Project.GetPrecisionTime();
		dComplete = (m_dTime[5] - m_dTime[4]) * 1000;
		if (dComplete <= 0) dComplete = 0;
		if (m_dTime[4] != 0) {
			strComplete.Format(_T("Complete Io In_%d[%.1f]"), m_nGrabCount, dComplete);
			AddtoInspLog1(strComplete, 1);
		}
	}
	if(m_IO.m_bInputData[2]==FALSE)bPing3 = m_IO.m_bInputData[2];

	//IO IN 3 (미완료)
	if (m_bAuto && m_IO.m_bInputData[3] && bPing4 == FALSE)
	{
		bPing4 = TRUE;
		m_nComleteNgCnt++;
		CString strCompleteNg;
		double dCompleteNg = 0.0;

		m_dTime[6] = m_Project.GetPrecisionTime();
		dCompleteNg = (m_dTime[6] - m_dTime[4]) * 1000;
		if (dCompleteNg <= 0) dCompleteNg = 0;
		strCompleteNg.Format(_T("Complete NG Io In_%d_%d[%.1f]"), m_nGrabCount, m_nComleteNgCnt, dCompleteNg);
		AddtoInspLog1(strCompleteNg, 1);
	}
	if (m_IO.m_bInputData[3] == FALSE)bPing4 = m_IO.m_bInputData[3];

	return 0;
}

  
int CFlyCapture2_EXDlg::Test(int nCam)
{
	CString strInsTime;
	double dInsTime;
	m_dTime[2] = m_Project.GetPrecisionTime();
	dInsTime = (m_dTime[2] - m_dTime[1]) * 1000;
	if (dInsTime <= 0) dInsTime = 0;
	if (m_dTime[1] != 0) {
		strInsTime.Format(_T("Ins Start_%d[%.1f]"), m_nGrabCount, dInsTime);
		AddtoInspLog1(strInsTime, 1);
	}

	int nDLDelay = m_pDlgSet->m_FormTab5->m_nDLDelayCount;
	int nRet = RET_OK;
	m_nRBResult = RET_OK;
	BOOL bNgSave = m_pDlgSet->m_FormTab2->m_bNgSave;

	m_ptWeldCenterPos.SetPoint(0,0);
	m_nTabTopEdge = 0;

	m_pDlgSet->m_FormTab3->m_nTabResult = 1; 
	m_pDlgSet->m_FormTab3->m_ntabresult = 1;
	m_Project.m_nHoleOkNg = 0;

	if(m_pDlgSet->m_FormTab2->m_bCompleteOnOff){//검사 시작시 컴플리트신호 켬 (성호야 이거 추가했ㅅ다)
		m_IO.WriteOutputPort(2,TRUE);
	}
	
	if (m_dRemainHDDSpace2<m_pDlgSet->m_FormTab3->m_nRemainHddSpace&& m_nMachine==8){
		m_IO.WriteOutputPort(2,FALSE);
		AfxMessageBox("D드라이브 잔량 부족");
		Sleep(1000*5);
		m_IO.WriteOutputPort(2,TRUE);
	}

	m_rtWeldBlob.SetRect(0,0,0,0);
	m_Project.ReadyData();
	m_Project.m_ncapblackcount = 0;
	m_nWeldCount = 0;
	for(int i=0;i<5;i++)		m_nMorphCount[i] = 0;
	for(int i=0;i<2;i++)		m_dTabBottomSize[i] =0;
	m_strBadInfo1[0]=m_strBadInfo1[1] = "";

	m_nRBResult  = m_pDlgSet->m_FormTab3->OnBnClickedButtonTest();

	GetLocalTime(&m_timeInspEnd);

	m_dTime[3] = m_Project.GetPrecisionTime();
	dInsTime = (m_dTime[3] - m_dTime[2]) * 1000;
	if (dInsTime <= 0) dInsTime = 0;
	strInsTime.Format(_T("Ins End_%d[%.1f]"), m_nGrabCount, dInsTime);
	AddtoInspLog1(strInsTime, 1);

	if(m_nRBResult == NG_NOT_CAN){//PASS
		if(bNgSave)
			m_evNgSave.SetEvent();
	}
	else{
		m_nTotalInsCount++;

		if (m_pDlgSet->m_FormTab3->m_bALLOK) {
			m_nRBResult = RET_OK;
			m_nDLResult = AI_RET_OK;
			m_nOKCount++;
		}
		else if (m_pDlgSet->m_FormTab3->m_bALLNG) {
			m_nRBResult = NG_TAP_POS;
			m_nDLResult = AI_RET_OK;
			m_nNgCount++;
			m_nRBNgCount++;
		}
		else if(m_nRBResult == RET_OK){
			m_bFinalRet = FINAL_RET_OK;
			if (m_pDlgSet->m_FormTab4->m_bUseAiIns) {
				//while (m_nDLResult == RET_INIT) {
				if (m_nDLResult == RET_INIT) {
					for (int i = 0; i < nDLDelay; i++) 
						Sleep(1);

				}
				if (m_nDLResult != 1) {
					m_nDLNgCount++;
					m_nNgCount++;
					m_bFinalRet = FINAL_RET_NG;

					if (bNgSave)
						m_evNgSave.SetEvent();
				}
				else if (m_nDLResult == AI_RET_OK)
					m_nOKCount++;				
			}
			else 
				m_nOKCount++;
		}
		else if(m_nRBResult != RET_OK){
			m_nNgCount++;
			m_nRBNgCount++;
			m_bFinalRet = FINAL_RET_NG;

			if(bNgSave)
				m_evNgSave.SetEvent();

			//NG분류
			NgClassify(m_strBadInfo1[1]); // FinalRet 대체하기
		}
		
		SetOutputResult(nCam, m_nRBResult, m_nDLResult);
		BadNgCount(1, m_nRBResult, m_nDLResult);
		ViewData();

		m_nCellCount = m_nOKCount+m_nNgCount;
		m_IO.m_bInspComplete = TRUE; //IO
		m_bInsComplete = TRUE;

		if (!m_pDlgSet->m_FormTab2->m_bUseCellApi)
			m_strCellID = m_strLotID = _T("NA");
		WriteID(m_nCellCount, m_nRBResult, m_strCellID, m_strLotID);

		dInsTime = (m_dTime[8] - m_dTime[7]) * 1000;
		if (dInsTime <= 0) dInsTime = 0;
		strInsTime.Format(_T("DL Ins End_%d[%.1f]"), m_nGrabCount, dInsTime);
		AddtoInspLog1(strInsTime, 1);
	}

	if (m_pDlgSet->m_FormTab2->m_bCsvSave)
		WriteTextData1(m_nRBResult, m_nDLResult);

	memcpy(m_Display[0].m_pImg->GetImagePtr(), m_Project.m_MultiBuf[m_pDlgSet->m_FormTab2->m_nTopCapCenterFrame], CAMERA_WIDTH*CAMERA_HEIGHT);

	if (m_bFinalRet == FINAL_RET_NG) {
		if (m_pDlgSet->m_FormTab2->m_bNgSave) 
			m_evFullSave.SetEvent();	
	}
	else if (m_pDlgSet->m_FormTab2->m_bFullSave) 
		m_evFullSave.SetEvent();

	LabelResult(m_nRBResult, m_nDLResult);
	DrawDisplay2(nCam);
	m_bInsDone = true;

	if(m_pDlgSet->m_FormTab2->m_bOverlaySave)
		m_eSaveCapture.SetEvent();

	return 0;
}


int CFlyCapture2_EXDlg::TestNew(int nCam)
{
	int nret = 0;
	m_nType = 3;
	m_Display[0].ResetArray();
	m_Project.m_ptCanTop[0].SetPoint(0,0);
	m_Project.m_ptCanTop[1].SetPoint(0,0);
	m_Project.m_ptLeftTop.SetPoint(0,0);
	m_Project.m_ptRightTop=m_Project.m_ptLeftTop;

	m_Project.m_fm[0] =m_Display[0].m_pImg->GetImagePtr();
	BYTE * fm = m_Display[0].m_pImg->GetImagePtr();
	int nth = 150,nEdgeth=70;
	CRect rttab,rttmp;
	int nHisto[256]={0,};
	rttmp.SetRect(10,10,CAMERA_WIDTH-10,CAMERA_HEIGHT-10);

	//top center find //m_Project.m_MultiBuf[m_Project.m_nMultiFrame]
	double d1[10]={0,};
	d1[0] = m_Project.GetPrecisionTime();
	rttab = m_Project.CircleFind2(m_Project.m_MultiBuf[1]);//Tracker search
	int nLeft1,nRight1,nCenter1;
	int nCanCheck = m_Project.FindCanPosition(m_Project.m_MultiBuf[0], m_Display[0].m_rtImgTracker[0],30,&nLeft1, &nRight1, &nCenter1);

	CRect rttmp11,rttmp1;
	rttmp11 = m_Display[0].m_rtImgTracker[5];
	int nw1 = rttmp11.Width();//8의 배수로 만들어야 함.
	int nh1 = rttmp11.Height();//8의 배수로 만들어야 함.
	nw1 -= nw1%8;nh1 -= nh1%8;
	rttmp1.SetRect(m_Project.m_ptCapCenter.x-m_Project.m_nRegWeldPos[0],m_Project.m_ptCapCenter.y+m_Project.m_nRegWeldPos[1],m_Project.m_ptCapCenter.x-m_Project.m_nRegWeldPos[0]+nw1,m_Project.m_ptCapCenter.y+m_Project.m_nRegWeldPos[1]+nh1);
	//	pView->m_Display[0].AddArrayRect(rttmp1);
	m_Project.m_rtWeldPos = rttmp1;


	if(m_nMakeAlphachannel[0]==0 && m_nMakeAlphachannel[3]==0){
		m_pDlgSet->m_FormTab3->fnYSum1(9,10,0,rttmp,0,0,2,1);//top
	}
	if(m_nMakeAlphachannel[1]==0 && m_nMakeAlphachannel[4]==0){
		m_pDlgSet->m_FormTab3->fnXSum1(3,5,1,rttmp,0,0,2);//left
	}
	if(m_nMakeAlphachannel[2]==0 && m_nMakeAlphachannel[5]==0){
		m_pDlgSet->m_FormTab3->fnXSum1(6,8,2,rttmp,0,0,2);//right
	}
	double d11 = m_Project.GetPrecisionTime();
	int nCheck = 1;
	while(1){
		Sleep(1);
		nCheck = 1;
		for(int i=0;i<3;i++){
			if(m_nMakeAlphachannel[i]==0)nCheck=0;
		}
		if(nCheck==1)break;
		if((m_Project.GetPrecisionTime() - d11)*1000 > 200)//0.2sec delay
			break;
	}
//	if(nCheck==1)
		m_pDlgSet->m_FormTab3->fnImageSummary(0,0);
// 	else//ng
// 		;

	m_Project.fnTabFind(m_Display[0].m_pImg->GetImagePtr(),rttab,1);

//weld inspect


//cam1	6	can2	10	cam3 9
	if(m_strComname.Find("A")>=0)		m_nSelectFrame = 6;
	else if(m_strComname.Find("A")>=0)	m_nSelectFrame = 10;
	else if(m_strComname.Find("A")>=0)	m_nSelectFrame = 9;
	else								m_nSelectFrame = 10;


//	m_pDlgSet->m_FormTab3->OnBnClickedButtonTest();
	for(int i=0;i<10;i++)m_nMakeAlphachannel[i] = 0;
	return nret;

}

BOOL CFlyCapture2_EXDlg::ImageShiftBad()
{	
	unsigned char *fmTmp1 = m_DisplayBad[0].m_pImg->GetImagePtr();
	unsigned char *fmTmp2 = m_DisplayBad[1].m_pImg->GetImagePtr();
	unsigned char *fmTmp3 = m_DisplayBad[2].m_pImg->GetImagePtr();
	unsigned char *fmTmp4 = m_DisplayBad[3].m_pImg->GetImagePtr();
	unsigned char *fmTmp5 = m_DisplayBad[4].m_pImg->GetImagePtr();
	unsigned char *fmTmp6 = new unsigned char[300*192];

	for(int i=3;i>=0;i--){
		memcpy(m_DisplayBad[i+1].m_pImg->GetImagePtr(),m_DisplayBad[i].m_pImg->GetImagePtr(),300*192);//200*128);
		m_strBadInfo[i+1][0] = m_strBadInfo[i][0];
		m_strBadInfo[i+1][1] = m_strBadInfo[i][1];
	}

	//recopy
	//rect reset
	CRect rt1;
	rt1.SetRect(m_Project.m_rtHoleRect.left-50,m_Project.m_rtHoleRect.top-32,m_Project.m_rtHoleRect.right+50,m_Project.m_rtHoleRect.bottom+32);

	rt1.SetRect(m_Project.m_ptCapCenter.x-150,m_Project.m_ptCapCenter.y-48,m_Project.m_ptCapCenter.x+150,m_Project.m_ptCapCenter.y+144);
	rt1 = m_Project.CheckROI(rt1,CRect(10,10,CAMERA_WIDTH-10,CAMERA_HEIGHT-10));
//	m_Project.m_Pro.ImageSelectCopy(m_Project.m_fm[0],fmTmp6,CAMERA_WIDTH,CAMERA_HEIGHT,rt1,300,192,0,0,0);
	m_Project.m_Pro.ImageSelectCopy(m_Project.m_MultiBuf[m_nSelectFrame],fmTmp6,CAMERA_WIDTH,CAMERA_HEIGHT,rt1,300,192,0,0,0);

	for(int i=0;i<192;i++)
		memcpy(fmTmp1+i*300,fmTmp6+(192-i-1)*300,300); 

	delete fmTmp6;

//	memcpy(m_DisplayBad[0].m_pImg->GetImagePtr(),m_Project.m_fmHole,200*128);
	//save
	CString str1;
	str1.Format("%d_NG_%s",m_nOKCount+m_nNgCount,m_strBadInfo1[1]);
	if(1.0 < m_dRemainHDDSpace2){
		m_DisplayBad[0].SaveJpgImgBadImage(str1);
	}

	m_strBadInfo[0][0] = m_strBadInfo1[0];
	m_strBadInfo[0][1] = m_strBadInfo1[1];

	for(int i=0;i<5;i++)
	{
		m_LabelNgTime[i].SetText(m_strBadInfo[i][0]);
		m_LabelBadName[i].SetText(m_strBadInfo[i][1]);
	}
	
	return TRUE;
}

void  CFlyCapture2_EXDlg::StopInsp()
{
	CString str11;
	str11 = m_strComname;
	double dTactTime = 0;
	if(m_dTactTimeAve[1]>0)
		dTactTime =(m_dTactTimeAve[0]/m_dTactTimeAve[1])/1000;

	int n1,n2;
	n1 = m_EndTimer.GetTime() - m_StartTimer.GetTime();
	n2 = n1/60;
	BOOL bStats = FALSE;
	m_nInspMinute = n2;
	CString strtype;

	m_nRetryCount = 0;
//	AlarmView(-1);
	for(int i=0;i<100;i++){
		m_dData[i] = 0;
		m_nResult[i] = 0;
		m_bResult[i] = 0;
	}

	strtype = m_strModelType;
	m_nNewCount= 0;
	
	//
	m_nMasterMode = 0;
	fnWriteEndFile();
	OnBnClickedMfcbuttonReset();
}

void  CFlyCapture2_EXDlg::StartInsp()
{
	if(m_nStats==2)
	{
		AfxMessageBox(_T("카메라 연결 상태를 확인해 주세요!!!"));
	}
	else
	{
		m_nMasterMode = 0;
	}
}

void CFlyCapture2_EXDlg::OnDestroy()
{
	CDialog::OnDestroy();

//	Usb_Qu_Close();

	//m_pDlgSet->KillTimer(1545);
	//
	//Sleep(500);
	//for(int i=0;i<MAX_CAMERAS;i++)
	//{
	//	if(m_bCamStats[i]==TRUE)
	//		m_Vision[i]->Stop();
	//	if(m_Vision[i])delete m_Vision[i];
	//}


	//m_bRun = FALSE;
	//Sleep(100);
	//m_cComm[0].Close();
	////for(int i=0;i<12;i++)delete m_cComm[i];

	//excel.excel.ReleaseExcel();
	//m_Project.Destroy();
	//if(m_pDlgSet)		delete m_pDlgSet;
	//if(m_DlgBarcode)	delete m_DlgBarcode;
	//if(m_LightMap) delete m_LightMap;
	//if(m_CamDisplay) delete m_CamDisplay;
	//if(m_nModelCount)	delete m_nModelCount;
	//if(m_pDlgPassword)	delete m_pDlgPassword;
//	if(m_Multi) delete m_Multi;
	DAP_Release_Func();
}

void CFlyCapture2_EXDlg::OnTimer(UINT_PTR nIDEvent)
{
	CString str,str12;

	if(nIDEvent==0)
	{
		
	}

	if(nIDEvent==1923){
		KillTimer(1923);
		if(m_nPcName==1 && m_pDlgSet->m_FormTab2->m_bDrawGraph){
			//copy file
			for(int i=0;i<3;i++){
				if(m_bTestMode==FALSE)
					CopyTimeFile(i+1);//File Copy
			}
			ReadCamTimeRate(0);//Data Read -> Data Set
		}
		SetTimer(1923,60*1000*10,NULL);
	}
	if(nIDEvent==1412)
	{
		if(nHour1 >= 24){
			nHour1 = 0;
		}
		CTime ttime = CTime::GetCurrentTime();
		
		CString kk2;
		kk2.Format(_T("CSW \n%d. %d. %d / %d:%d:%d   "), ttime.GetYear(),ttime.GetMonth(),ttime.GetDay(),ttime.GetHour(),ttime.GetMinute(),ttime.GetSecond());
		m_nHour = ttime.GetHour();

		m_btTime.SetWindowTextA(kk2);

		if(ttime.GetMinute()%2 ==0){//2분마다 1회씩
			//WriteTimeCount(m_nPcName);
		}

		if(m_nHour==0 && m_nHour != m_nHourOld){
			m_nCellCount = 0;
			m_nOKCount = 0;
			m_nNgCount = 0;
			//시간 개수 초기화 1일 1회
			for(int i=0;i<24;i++){
				for(int j=0;j<10;j++){
					m_nTimeCount[i][j] = 0;
					m_nTotalCount[0][j]=0;
					m_nTotalCount[1][j]=0;
					m_nTotalCount[2][j]=0;
				}
			}
		}
		m_nHourOld = m_nHour;
		
//  		if(mDrawBadState[0] != NULL ){
//  			
//  			mDrawBadState[0]->RedrawMap();
//  		}
//  
// 		if(mDrawBadState[1] != NULL ){
// 
// 			mDrawBadState[1]->RedrawMap();
// 		}
// 		if(mDrawBadState[2] != NULL ){
// 
// 			mDrawBadState[2]->RedrawMap();
// 		}
//   
//  		//2.해당 시간 데이터 그래프에 Set
// 		mDrawBadState[0]->SetData(0, nHour1, m_GraphTestData[0][nHour1]);
// 		mDrawBadState[1]->SetData(0, nHour1, m_GraphTestData[1][nHour1]);
// 		mDrawBadState[2]->SetData(0, nHour1, m_GraphTestData[0][nHour1]);
// 
// 		
// 		nHour1++;

	
		
	}
	
	if(nIDEvent == 1115)
	{
		KillTimer(1115);
		m_pDlgPassword->ShowWindow(SW_SHOW);
	}

	if(nIDEvent == 125)//bcr check
	{

	}

	if(nIDEvent == 131)
	{
		CheckHdd();
	}

	if(nIDEvent ==1456)//hdd 상태 check
	{
		m_nAlarmCnt++;
		m_bCheckHdd = TRUE;
		
		if(m_dRemainHDDSpace2 > m_pDlgSet->m_FormTab3->m_nRemainHddSpace){
			HddAlarm(1);
			m_nAlarmCnt = 0;
			m_bCheckHdd = FALSE;
			KillTimer(1456);
		}
		else{
			HddAlarm(m_nAlarmCnt%2);
		}
//		WriteTimeRate();
	}

	if(nIDEvent==1015)
	{
		int processID=4324;
		int SystemWideCpuUsage = usageA.GetCpuUsage();
		int ProcessCpuUsageByName = usageB.GetCpuUsage("HOLE검사기");
//		int ProcessCpuUsageByID = usageC.GetCpuUsage(processID);

		CString strcpu;
//		strcpu.Format("%d , %d, %d",SystemWideCpuUsage,ProcessCpuUsageByName,ProcessCpuUsageByID);
		strcpu.Format("Win:%d ,PGM:%d",SystemWideCpuUsage,int(ProcessCpuUsageByName*0.25));
		if(SystemWideCpuUsage>=m_pDlgSet->m_FormTab3->m_nWinCpu || ProcessCpuUsageByName*0.25 >=m_pDlgSet->m_FormTab3->m_nPGMCpu)
			AddtoList(strcpu,3);
	}

	if(nIDEvent == 2001)
	{
// 		if(m_nIoCount==0){
// 			m_IO.WriteOutputPort(1,1);
// 			m_nIoCount = 1;
// 		}
// 		else{
// 			m_IO.WriteOutputPort(1,0);
// 			m_nIoCount = 0;
// 		}
	}

	CDialog::OnTimer(nIDEvent);
}

LRESULT CFlyCapture2_EXDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if(wParam==0)
		int n = 0;

	if(message == 322)
	{

	}

	if(message == 4000)
	{


	}
	if(message == 5000)
	{


	}

	return CDialog::WindowProc(message, wParam, lParam);
}

void CFlyCapture2_EXDlg::InitControl()
{

	CString str;

	CString strTmp[MAX_VIEW_DATA];
	strTmp[0] = "  Version ",strTmp[1] = "  COUNT:",strTmp[2] = "  개별피치평균:",strTmp[3] = "  개별피치최대:";
	strTmp[4] = "  TACTTIME:",strTmp[5] = "",strTmp[6] = "  개별피치표준편차:",strTmp[7] = "  개별피치최소:";
	for(int i=0;i<MAX_VIEW_DATA;i++)
	{
		m_lbViewData[i].SetTextColor(COLOR_WHITE);
		m_lbViewData[i].SetBkColor(COLOR_MAIN_LABEL);
		m_lbViewData[i].SetFontBold(TRUE);
		m_lbViewData[i].SetFontName(_T("굴림"));
		m_lbViewData[i].SetFontSize(16);
		m_lbViewData[i].SetText(strTmp[i]);
	}
	CFont fontBtn;
	fontBtn.CreateFont(25, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, CLIP_CHARACTER_PRECIS, PROOF_QUALITY, DEFAULT_PITCH, _T("Verdana"));

	m_btTime.EnableWindowsTheming(FALSE);
	m_btTime.SetFaceColor(COLOR_MAIN_LABEL);
	m_btTime.SetTextColor(COLOR_WHITE);
	GetDlgItem(IDC_MFCBUTTON_TIME)->SetFont(&fontBtn);

	m_btStart.EnableWindowsTheming(FALSE);
	m_btStart.SetFaceColor(COLOR_MAIN_LABEL);
	m_btStart.SetTextColor(COLOR_WHITE);
	GetDlgItem(IDC_MFCBUTTON_START)->SetFont(&fontBtn);

	m_btStop.EnableWindowsTheming(FALSE);
	m_btStop.SetFaceColor(COLOR_MAIN_LABEL);
	m_btStop.SetTextColor(COLOR_WHITE);
	GetDlgItem(IDC_MFCBUTTON_STOP)->SetFont(&fontBtn);

	m_btSetting.EnableWindowsTheming(FALSE);
	m_btSetting.SetFaceColor(COLOR_MAIN_LABEL);
	m_btSetting.SetTextColor(COLOR_WHITE);
	GetDlgItem(IDC_MFCBUTTON_SETTING)->SetFont(&fontBtn);

	m_btReset.EnableWindowsTheming(FALSE);
	m_btReset.SetFaceColor(COLOR_MAIN_LABEL);
	m_btReset.SetTextColor(COLOR_WHITE);
	GetDlgItem(IDC_MFCBUTTON_RESET)->SetFont(&fontBtn);

	m_btExit.EnableWindowsTheming(FALSE);
	m_btExit.SetFaceColor(COLOR_MAIN_LABEL);
	m_btExit.SetTextColor(COLOR_WHITE);
	GetDlgItem(IDC_MFCBUTTON_EXIT)->SetFont(&fontBtn);

	fontBtn.Detach();

	fontBtn.CreateFont(15, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, CLIP_CHARACTER_PRECIS, PROOF_QUALITY, DEFAULT_PITCH, _T("Verdana"));
	m_btCellApi.SetWindowTextA(_T("CELL\nAPI"));
	GetDlgItem(IDC_MFCBUTTON_API)->SetFont(&fontBtn);

	m_btInfo.EnableWindowsTheming(FALSE);
	m_btInfo.SetFaceColor(COLOR_MAIN_LABEL);
	m_btInfo.SetTextColor(COLOR_WHITE);
	GetDlgItem(IDC_MFCBUTTON_INFO)->SetFont(&fontBtn);

	m_btVersion.EnableWindowsTheming(FALSE);
	m_btVersion.SetFaceColor(COLOR_MAIN_LABEL);
	m_btVersion.SetTextColor(COLOR_WHITE);
	GetDlgItem(IDC_MFCBUTTON_VERSION)->SetFont(&fontBtn);
	
	fontBtn.Detach();

	fontBtn.CreateFont(20, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, CLIP_CHARACTER_PRECIS, PROOF_QUALITY, DEFAULT_PITCH, _T("Verdana"));
	m_btModel.SetTextColor(COLOR_BLACK);
	GetDlgItem(IDC_MFCBUTTON_MODEL)->SetFont(&fontBtn);
	fontBtn.Detach();

	m_Label_OKNGResult.SetTextColor(COLOR_BLACK);
	m_Label_OKNGResult.SetBkColor(COLOR_YELLOW);
	m_Label_OKNGResult.SetFontBold(TRUE);
	m_Label_OKNGResult.SetFontName(_T("굴림"));
	m_Label_OKNGResult.SetFontSize(15);
	m_Label_OKNGResult.SetText("R/B READY");

	m_Label_DLOKNGResult.SetTextColor(COLOR_BLACK);
	m_Label_DLOKNGResult.SetBkColor(COLOR_YELLOW);
	m_Label_DLOKNGResult.SetFontBold(TRUE);
	m_Label_DLOKNGResult.SetFontName(_T("굴림"));
	m_Label_DLOKNGResult.SetFontSize(15);
	m_Label_DLOKNGResult.SetText("D/L READY");
	
	m_Label_FinalResult.SetTextColor(COLOR_BLACK);
	m_Label_FinalResult.SetBkColor(COLOR_YELLOW);
	m_Label_FinalResult.SetFontBold(TRUE);
	m_Label_FinalResult.SetFontName(_T("굴림"));
	m_Label_FinalResult.SetFontSize(48);
	m_Label_FinalResult.SetText("READY");
	

	m_Label_Ins_Data.SetTextColor(COLOR_GREEN);
	m_Label_Ins_Data.SetBkColor(COLOR_BLACK);
	m_Label_Ins_Data.SetFontBold(TRUE);
	m_Label_Ins_Data.SetFontName(_T("굴림"));
	m_Label_Ins_Data.SetFontSize(13);

	for(int i=0;i<=30;i++)
	{
		m_Label_Ins_Data2[i].SetTextColor(COLOR_GREEN);
		m_Label_Ins_Data2[i].SetBkColor(COLOR_BLACK);
		m_Label_Ins_Data2[i].SetFontBold(TRUE);
		m_Label_Ins_Data2[i].SetFontName(_T("굴림"));
		m_Label_Ins_Data2[i].SetFontSize(12);
		//m_Label_Ins_Data.SetText("Data");
	}
	for (int i = 0; i <= 22; i++)
	{
		m_Label_DL_Ins_Data[i].SetTextColor(COLOR_GREEN);
		m_Label_DL_Ins_Data[i].SetBkColor(COLOR_BLACK);
		m_Label_DL_Ins_Data[i].SetFontBold(TRUE);
		m_Label_DL_Ins_Data[i].SetFontName(_T("굴림"));
		m_Label_DL_Ins_Data[i].SetFontSize(12);
	}

	m_LabelBadCount.SetTextColor(COLOR_WHITE);
	m_LabelBadCount.SetBkColor(COLOR_MAIN_LABEL);
	m_LabelBadCount.SetFontBold(TRUE);
	m_LabelBadCount.SetFontName(_T("굴림"));
	m_LabelBadCount.SetFontSize(16);
	m_LabelBadCount.SetText("  OK/NG:");

	m_LabelRemainHDD.SetTextColor(COLOR_WHITE);
	m_LabelRemainHDD.SetBkColor(RGB(80,100,115));
	m_LabelRemainHDD.SetFontBold(TRUE);
	m_LabelRemainHDD.SetFontName(_T("굴림"));
	m_LabelRemainHDD.SetFontSize(16);
	m_LabelRemainHDD.SetText(" HDD 1:");

	m_LabelRemainHdd1.SetTextColor(COLOR_WHITE);
	m_LabelRemainHdd1.SetBkColor(RGB(80,100,115));
	m_LabelRemainHdd1.SetFontBold(TRUE);
	m_LabelRemainHdd1.SetFontName(_T("굴림"));
	m_LabelRemainHdd1.SetFontSize(16);
	m_LabelRemainHdd1.SetText(" HDD 2:");

	m_StaticStats.SetTextColor(COLOR_BLACK);
	m_StaticStats.SetBkColor(COLOR_WHITE);
	m_StaticStats.SetFontBold(TRUE);
	m_StaticStats.SetFontName(_T("굴림"));
	m_StaticStats.SetFontSize(13);
	str.Format(_T("상태"));
	m_StaticStats.SetText(str);

	m_StaticStats.SetTextColor(COLOR_BLACK);
	m_StaticStats.SetBkColor(COLOR_WHITE);
	m_StaticStats.SetFontBold(TRUE);
	m_StaticStats.SetFontName(_T("굴림"));
	m_StaticStats.SetFontSize(13);
	str.Format(_T("상태"));
	m_StaticStats.SetText(str);

	int nsize = 16;
	m_LabelNonSense.SetTextColor(COLOR_BLACK);
	m_LabelNonSense.SetBkColor(COLOR_GRAY);
	m_LabelNonSense.SetFontBold(TRUE);
	m_LabelNonSense.SetFontName(_T("굴림"));
	m_LabelNonSense.SetFontSize(nsize);
	str.Format(_T("Inspect 1"));
	m_LabelNonSense.SetText(str);

	m_ctInsp.SetTextColor(COLOR_BLACK);
	m_ctInsp.SetBkColor(COLOR_GRAY);
	m_ctInsp.SetFontBold(TRUE);
	m_ctInsp.SetFontName(_T("굴림"));
	m_ctInsp.SetFontSize(nsize);
	str.Format(_T("검    사"));
	m_ctInsp.SetText(str);

	if(m_strComname.Find("3A")>=0 || m_strComname.Find("3B")>=0 ||m_strComname.Find("3C")>=0 
		|| m_strComname.Find("4A")>=0 || m_strComname.Find("4B")>=0 ||m_strComname.Find("4C")>=0
		|| m_strComname.Find("5A")>=0 || m_strComname.Find("5B")>=0 ||m_strComname.Find("5C")>=0
		|| m_strComname.Find("6A")>=0 || m_strComname.Find("6B")>=0 ||m_strComname.Find("6C")>=0
		|| m_strComname.Find("7A")>=0 || m_strComname.Find("7B")>=0 ||m_strComname.Find("7C")>=0
		|| m_strComname.Find("8A")>=0 || m_strComname.Find("8B")>=0 ||m_strComname.Find("8C")>=0)

	m_ctInsp.SetTextColor(COLOR_BLACK);
	m_ctInsp.SetBkColor(COLOR_GREEN);
	m_ctInsp.SetFontBold(TRUE);
	m_ctInsp.SetFontName(_T("굴림"));
	m_ctInsp.SetFontSize(nsize);
	str.Format(_T("Tab OK"));
	m_ctInsp.SetText(str);

	m_LabelNonSense.SetTextColor(COLOR_BLACK);
	m_LabelNonSense.SetBkColor(COLOR_GREEN);
	m_LabelNonSense.SetFontBold(TRUE);
	m_LabelNonSense.SetFontName(_T("굴림"));
	m_LabelNonSense.SetFontSize(nsize);
	str.Format(_T("Hole OK"));
	m_LabelNonSense.SetText(str);

	GetDlgItem(IDC_STATIC_INS_DATA)->MoveWindow(3, 65, 1020, 240);
	GetDlgItem(IDC_STATIC_INS_DATA2)->MoveWindow(10, 80, 350, 30);
	GetDlgItem(IDC_STATIC_INS_DATA3)->MoveWindow(10, 100, 350, 30);
	GetDlgItem(IDC_STATIC_INS_DATA4)->MoveWindow(10, 120, 350, 30);
	GetDlgItem(IDC_STATIC_INS_DATA5)->MoveWindow(10, 140, 350, 30);
	GetDlgItem(IDC_STATIC_INS_DATA6)->MoveWindow(10, 160, 350, 30);
	GetDlgItem(IDC_STATIC_INS_DATA7)->MoveWindow(10, 180, 350, 30);
	GetDlgItem(IDC_STATIC_INS_DATA8)->MoveWindow(10, 200, 350, 30);
	GetDlgItem(IDC_STATIC_INS_DATA9)->MoveWindow(10, 220, 350, 30);
	GetDlgItem(IDC_STATIC_INS_DATA10)->MoveWindow(10, 240, 350, 30);
	GetDlgItem(IDC_STATIC_INS_DATA11)->MoveWindow(10, 260, 350, 30);
	GetDlgItem(IDC_STATIC_INS_DATA12)->MoveWindow(270, 80, 350, 30);
	GetDlgItem(IDC_STATIC_INS_DATA13)->MoveWindow(270, 100, 350, 30);
	GetDlgItem(IDC_STATIC_INS_DATA14)->MoveWindow(270, 120, 350, 30);
	GetDlgItem(IDC_STATIC_INS_DATA15)->MoveWindow(270, 140, 150, 30);
	GetDlgItem(IDC_STATIC_INS_DATA16)->MoveWindow(400, 140, 100, 30);
	GetDlgItem(IDC_STATIC_INS_DATA17)->MoveWindow(460, 140, 100, 30);
	GetDlgItem(IDC_STATIC_INS_DATA18)->MoveWindow(270, 160, 200, 30);
	GetDlgItem(IDC_STATIC_INS_DATA19)->MoveWindow(450, 160, 200, 30);
	GetDlgItem(IDC_STATIC_INS_DATA20)->MoveWindow(270, 200, 350, 30);
	GetDlgItem(IDC_STATIC_INS_DATA21)->MoveWindow(270, 220, 350, 30);

	GetDlgItem(IDC_STATIC_INS_DATA22)->MoveWindow(450, 200, 100, 30);
	GetDlgItem(IDC_STATIC_INS_DATA23)->MoveWindow(450, 220, 100, 30);
	GetDlgItem(IDC_STATIC_INS_DATA24)->MoveWindow(450, 200, 200, 30);
	GetDlgItem(IDC_STATIC_INS_DATA25)->MoveWindow(450, 220, 200, 30);

	GetDlgItem(IDC_STATIC_INS_DATA26)->MoveWindow(270, 240, 350, 30);
	GetDlgItem(IDC_STATIC_INS_DATA27)->MoveWindow(270, 260, 350, 30);
	GetDlgItem(IDC_STATIC_INS_DATA28)->MoveWindow(450, 260, 100, 30);

	GetDlgItem(IDC_STATIC_INS_DATA29)->MoveWindow(270, 280, 180, 15);
	GetDlgItem(IDC_STATIC_INS_DATA30)->MoveWindow(420, 80, 180, 15);
	GetDlgItem(IDC_STATIC_INS_DATA31)->MoveWindow(270, 80, 150, 15);
	GetDlgItem(IDC_STATIC_INS_DATA32)->MoveWindow(10, 280, 350, 15);

	//DL
	GetDlgItem(IDC_STATIC_DL_INS_DATA_1)->MoveWindow(600, 80, 180, 15);
	GetDlgItem(IDC_STATIC_DL_INS_DATA_2)->MoveWindow(600, 100, 180, 15);
	GetDlgItem(IDC_STATIC_DL_INS_DATA_3)->MoveWindow(600, 120, 180, 15);
	GetDlgItem(IDC_STATIC_DL_INS_DATA_4)->MoveWindow(600, 140, 180, 15);
	GetDlgItem(IDC_STATIC_DL_INS_DATA_5)->MoveWindow(600, 160, 180, 15);
	GetDlgItem(IDC_STATIC_DL_INS_DATA_6)->MoveWindow(600, 180, 180, 15);
	GetDlgItem(IDC_STATIC_DL_INS_DATA_7)->MoveWindow(600, 200, 180, 15);
	GetDlgItem(IDC_STATIC_DL_INS_DATA_8)->MoveWindow(600, 220, 200, 15);
	GetDlgItem(IDC_STATIC_DL_INS_DATA_9)->MoveWindow(785, 220, 80, 15);
	GetDlgItem(IDC_STATIC_DL_INS_DATA_10)->MoveWindow(600, 240, 180, 15);
	GetDlgItem(IDC_STATIC_DL_INS_DATA_11)->MoveWindow(600, 260, 180, 15);
	
	//DL외관
	GetDlgItem(IDC_STATIC_DL_INS_DATA_12)->MoveWindow(870, 70, 150, 14);
	GetDlgItem(IDC_STATIC_DL_INS_DATA_13)->MoveWindow(870, 90, 150, 14);
	GetDlgItem(IDC_STATIC_DL_INS_DATA_14)->MoveWindow(870, 110, 150, 14);
	GetDlgItem(IDC_STATIC_DL_INS_DATA_15)->MoveWindow(870, 130, 150, 14);
	GetDlgItem(IDC_STATIC_DL_INS_DATA_16)->MoveWindow(870, 150, 150, 14);
	GetDlgItem(IDC_STATIC_DL_INS_DATA_17)->MoveWindow(870, 170, 150, 14);
	GetDlgItem(IDC_STATIC_DL_INS_DATA_18)->MoveWindow(870, 190, 150, 14);
	GetDlgItem(IDC_STATIC_DL_INS_DATA_19)->MoveWindow(870, 210, 150, 14);
	GetDlgItem(IDC_STATIC_DL_INS_DATA_20)->MoveWindow(870, 230, 150, 14);
	GetDlgItem(IDC_STATIC_DL_INS_DATA_21)->MoveWindow(870, 250, 150, 14);
	GetDlgItem(IDC_STATIC_DL_INS_DATA_22)->MoveWindow(870, 270, 150, 14);
	GetDlgItem(IDC_STATIC_DL_INS_DATA_23)->MoveWindow(870, 290, 150, 14);

	
		
	int nControlWidth = 250;
	GetDlgItem(IDC_STATIC_FINAL_RESULT)->MoveWindow(1030, 70, nControlWidth, 100);
	GetDlgItem(IDC_STATIC_OK_NG_RESULT)->MoveWindow(1030, 173, 125, 50);
	GetDlgItem(IDC_STATIC_DL_OK_NG_RESULT)->MoveWindow(1155, 173, 125, 50);
	int nGap = 5;
	GetDlgItem(IDC_STATICSTATS)->MoveWindow(1030, 221 + nGap, nControlWidth, 50);
	GetDlgItem(IDC_STATIC_LABEL2)->MoveWindow(1030, 272 + nGap, nControlWidth, 50);
	GetDlgItem(IDC_STATIC_LABEL_BADCOUNT)->MoveWindow(1030, 323 + nGap, nControlWidth, 50);
	GetDlgItem(IDC_STATIC_LABEL5)->MoveWindow(1030, 374 + nGap, nControlWidth, 50);
	GetDlgItem(IDC_STATIC_LABEL_YEILD2)->MoveWindow(1030, 425 + nGap, nControlWidth, 50);
	GetDlgItem(IDC_LIST_LOG2)->MoveWindow(1030, 550, nControlWidth, 125);
	GetDlgItem(IDC_LIST_LOG5)->MoveWindow(1030, 675, nControlWidth, 140);


	int CountLabX = 746, CountLabW = 270, CountLabH = 25;
	int nLabelPosY = 855;
	int nGap2 = 27;
	int nDLLabelPosX = 1020;
	//GetDlgItem(IDC_STATIC_LABEL_BCODE2)->MoveWindow(CountLabX + (CountLabW / 2), 820, CountLabW, CountLabH);

	GetDlgItem(IDC_STATIC_LABEL_RULL_BASE)->MoveWindow(CountLabX, nLabelPosY - nGap2 - 7, CountLabW, CountLabH + 8);
	GetDlgItem(IDC_STATIC_LABEL_BCODE)->MoveWindow(CountLabX, nLabelPosY, nDLLabelPosX, CountLabH);
	GetDlgItem(IDC_STATIC_LABEL_YEILD)->MoveWindow(CountLabX, nLabelPosY + nGap2, CountLabW, CountLabH);
	GetDlgItem(IDC_STATIC_LABEL_BRIGHT)->MoveWindow(CountLabX, nLabelPosY + (nGap2 *2), CountLabW, CountLabH);
	GetDlgItem(IDC_STATIC_LABEL_NGMARK)->MoveWindow(CountLabX, nLabelPosY + (nGap2 * 3), CountLabW, CountLabH);
	GetDlgItem(IDC_STATIC_LABEL_BCR)->MoveWindow(CountLabX, nLabelPosY + (nGap2 * 4), CountLabW, CountLabH);

	GetDlgItem(IDC_STATIC_LABEL_DEEL_LEARNING)->MoveWindow(nDLLabelPosX, nLabelPosY - nGap2 - 7, CountLabW, CountLabH +8);
	GetDlgItem(IDC_STATIC_LABEL_DL_CAP_NG)->MoveWindow(nDLLabelPosX, nLabelPosY + nGap2, CountLabW, CountLabH);
	GetDlgItem(IDC_STATIC_LABEL_DL_DIST_NG)->MoveWindow(nDLLabelPosX, nLabelPosY + (nGap2 * 2), CountLabW, CountLabH);
	GetDlgItem(IDC_STATIC_LABEL_DL_WELD_NG)->MoveWindow(nDLLabelPosX, nLabelPosY + (nGap2 * 3), CountLabW, CountLabH);
	GetDlgItem(IDC_STATIC_LABEL_DL_TAB_NG)->MoveWindow(nDLLabelPosX, nLabelPosY + (nGap2 * 4), CountLabW, CountLabH);
	
	LabelStats(m_strStats, m_nStats);

}

void CFlyCapture2_EXDlg::LabelStats(CString str,int stats)
{
	if(stats)
	{
		m_StaticStats.SetTextColor(COLOR_RED);
		m_StaticStats.SetText(str);
	}
	else
	{
		m_StaticStats.SetTextColor(COLOR_GREEN);
		m_StaticStats.SetText(str);
	}
}

void CFlyCapture2_EXDlg::LabelResult(int nResult, int nDLResult)
{
	BOOL bUseDL = m_pDlgSet->m_FormTab4->m_bUseAiIns;
	BOOL bUseDLOutput = m_pDlgSet->m_FormTab4->m_bWeakWeldOutputUse;
	if(nResult == NG_NOT_CAN) // PASS
	{
		m_Label_OKNGResult.SetBkColor(COLOR_BLUE);
		m_Label_OKNGResult.SetText("R/B PASS");

		m_Label_DLOKNGResult.SetBkColor(COLOR_BLUE);
		m_Label_DLOKNGResult.SetText("R/B PASS");
	}
	else {
		if (nResult == -1) {
			m_Label_OKNGResult.SetBkColor(COLOR_YELLOW);
			m_Label_OKNGResult.SetText("R/B READY");
		}
		else if (nResult == RET_OK) {
			m_Label_OKNGResult.SetBkColor(COLOR_GREEN);
			m_Label_OKNGResult.SetText("R/B OK");
		}
		else {
			m_Label_OKNGResult.SetBkColor(COLOR_RED);
			m_Label_OKNGResult.SetText("R/B NG");
		}

		if (bUseDL && bUseDLOutput) {
			if (nDLResult == AI_READY) {
				m_Label_DLOKNGResult.SetBkColor(COLOR_YELLOW);
				m_Label_DLOKNGResult.SetText("D/L READY");
			}
			else if (nDLResult == AI_RET_OK) {
				m_Label_DLOKNGResult.SetBkColor(COLOR_GREEN);
				m_Label_DLOKNGResult.SetText("D/L OK");
			}
			else if (nDLResult == RET_INIT) {
				m_Label_DLOKNGResult.SetBkColor(COLOR_RED);
				m_Label_DLOKNGResult.SetText("D/L ERR");
			}
			else {
				m_Label_DLOKNGResult.SetBkColor(COLOR_RED);
				m_Label_DLOKNGResult.SetText("D/L NG");
			}
		}
		else {
			m_Label_DLOKNGResult.SetBkColor(COLOR_WHITE);
			m_Label_DLOKNGResult.SetText("D/L NOT USE");
		}
	}
	BOOL bFinalRet = RET_INIT;
	if (bUseDL && bUseDLOutput) {
		if (nDLResult == AI_RET_OK && nResult == RET_OK)
			bFinalRet = RET_OK;
	}
	else {
		if (nResult == RET_OK)
			bFinalRet = RET_OK;
	}

	if (bFinalRet == RET_OK) {
		m_Label_FinalResult.SetBkColor(COLOR_GREEN);
		m_Label_FinalResult.SetText("O    K");
	}
	else if (nResult == NG_NOT_CAN) {
		m_Label_FinalResult.SetBkColor(COLOR_BLUE);
		m_Label_FinalResult.SetText("PASS");
	}
	else if (nResult == -1 || nDLResult == AI_READY) {
		m_Label_FinalResult.SetBkColor(COLOR_YELLOW);
		m_Label_FinalResult.SetText("READY");
	}
	else {
		m_Label_FinalResult.SetBkColor(COLOR_RED);
		m_Label_FinalResult.SetText("N   G");
	}
	
}

void CFlyCapture2_EXDlg::OnBnClickedButtonSettung()
{
	if(m_pDlgSet->IsWindowVisible())
	{
		m_pDlgSet->ShowWindow(SW_HIDE);
		m_pDlgPassword->ShowWindow(SW_HIDE);
		m_nShowSet = 0;
		m_pDlgSet->m_FormTab4->m_bRoiSet = FALSE;
		m_Display[0].m_bTrackerUse = FALSE;
		m_pDlgSet->m_FormTab4->UpdateData(FALSE);
	}
	else
	{
	//	m_pDlgSet->ShowWindow(SW_SHOW);	
		m_pDlgPassword->m_strPassword1 = "";
		m_pDlgPassword->UpdateData(TRUE);
		m_pDlgPassword->ShowWindow(SW_SHOW);
		m_nShowSet = 1;
	}
}

void CFlyCapture2_EXDlg::Dirtest() 
{
	CFile file;
	CString str,strng,strok;
	TCHAR	spath[100];
	BROWSEINFO bi;
	memset(&bi,0,sizeof(bi));
	bi.hwndOwner = GetSafeHwnd();

	bi.ulFlags=BIF_RETURNONLYFSDIRS;
	
	LPITEMIDLIST lpifq=SHBrowseForFolder(&bi);

	if(lpifq)
	{
		SHGetPathFromIDList(lpifq,spath);
		LPMALLOC lpMalloc=NULL;
		HRESULT hr = SHGetMalloc(&lpMalloc);
		if(SUCCEEDED(hr))
		{
			lpMalloc->Free(lpifq);
			lpifq = NULL;
			lpMalloc->Release();
			lpMalloc = NULL;
		}
	}

	CString src, tar,strDevice;
	HANDLE hFind;				// FindFirstFile 핸들
	WIN32_FIND_DATA	fd;			// 검색한 화일의 정보
	BOOL bRet = TRUE;
	CString strfile;
	str = LPCTSTR(spath);
	str += "\\*.bmp";
	int ntmp =0;
	CString strMonth;
	CString strtime;

	hFind = FindFirstFile( str, &fd);
	strDevice = strfile.Left(3);
	while(hFind != INVALID_HANDLE_VALUE && bRet)
	{			
//		m_strBeforeImg = m_strLastFile;

		strfile.Format(_T("%s\\%s"), spath, fd.cFileName);

		if( !(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) )		// Not directory, which means file.
		{

			Wait(5);

		}
		else if( strfile == "." || strfile == "..")
		{
			// do nothing
		}
		else	// it is directory
		{
			// do nothing	choi-sh@koreno.co.kr
		}
		bRet= FindNextFile(hFind, &fd);
	}
	::FindClose(hFind);
	AfxMessageBox(_T("Directory Inspect Complete!!!"));	
}

BOOL PeekAndPump()
{
	static MSG msg;
	while (::PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)) 
	{
		if (!AfxGetApp()->PumpMessage()) 
		{
			::PostQuitMessage(0);
			return FALSE;
		}
		Sleep(1);
	}
	return TRUE;
}

void CFlyCapture2_EXDlg::Wait(DWORD dwT)
{
	DWORD dwST = GetTickCount();
	DWORD val = GetTickCount() - dwST;
    while( val < dwT)
	{
		PeekAndPump();
		Sleep(1);
		val = GetTickCount()-dwST;
	}
}

BEGIN_EVENTSINK_MAP(CFlyCapture2_EXDlg, CDialog)

END_EVENTSINK_MAP()


void CFlyCapture2_EXDlg::InitClass(void)
{

	if(m_pDlgSet == NULL)
	{
		m_pDlgSet = new CDlgSet(this);
		m_pDlgSet->Create(IDD_DIALOG_SET, NULL);
/*		m_pDlgSet->MoveWindow(730, 200, 550,650); */
		m_pDlgSet->ShowWindow(SW_HIDE);
	}
	if(m_DlgBarcode == NULL)
	{
		m_DlgBarcode = new CDlgCode(this);
		m_DlgBarcode->Create(IDD_DIALOG_BARCODE, NULL);
		m_DlgBarcode->MoveWindow(50, 200, 900,650);
		m_DlgBarcode->ShowWindow(SW_HIDE);
	}
	if(m_pDlgAiView == NULL)
	{
		m_pDlgAiView = new DlgAiView(this);
		m_pDlgAiView->Create(IDD_DIALOG_AI_VIEW, NULL);
		m_pDlgAiView->ShowWindow(SW_SHOW);
		m_pDlgAiView->init();
		if(!m_pDlgSet->m_FormTab4->m_bUseAiIns)
			m_pDlgAiView->ShowWindow(SW_HIDE);
	}
	if(m_LightMap == NULL)
	{
		m_LightMap = new CLightMap(this);
		m_LightMap->Create(IDD_DIALOG_LIGHTMAP,NULL);
		m_LightMap->MoveWindow(450	,200,750,180);
		m_LightMap->ShowWindow(SW_HIDE);
	}
	if(m_CamDisplay == NULL)
	{
		m_CamDisplay = new CCameraDisplay(this);
		m_CamDisplay->Create(IDD_DIALOG_CAMIMAGE, NULL);
		m_CamDisplay->MoveWindow(1280, 10, 1280,1004);
		if(m_pDlgSet->m_FormTab2->m_bDrawGraph)
			m_CamDisplay->ShowWindow(SW_SHOW);
		else
			m_CamDisplay->ShowWindow(SW_HIDE);

//		m_CamDisplay->ShowWindow(SW_HIDE);
	}

	if(m_nModelCount == NULL)
	{
		m_nModelCount = new CModelCount(this);
		m_nModelCount->Create(IDD_DIALOG_MODELCOUNT, NULL);
		m_nModelCount->MoveWindow(400, 200, 800,300); //465);
		m_nModelCount->ShowWindow(SW_HIDE);
	}

	if(m_pDlgPassword == NULL)
	{
		m_pDlgPassword = new CPassWord(this);
		m_pDlgPassword->Create(IDD_DIALOG_PASSWORD, NULL);
		m_pDlgPassword->ShowWindow(SW_HIDE);
	}

	if(mDrawBadState[0] == NULL){
		mDrawBadState[0] = new CDrawBadState(this);
		mDrawBadState[0]->Create(IDD_DRAW_BAD_STATE, NULL);
		CRect rFrame,rFrame1;
		m_CamDisplay->GetDlgItem(IDC_STATIC_GRAPH1)->GetWindowRect(rFrame);

		mDrawBadState[0]->m_rtArea = rFrame;
		mDrawBadState[0]->MoveWindow(mDrawBadState[0]->m_rtArea.left,mDrawBadState[0]->m_rtArea.top,mDrawBadState[0]->m_rtArea.Width(),mDrawBadState[0]->m_rtArea.Height());
		if(m_pDlgSet->m_FormTab2->m_bDrawGraph)
			mDrawBadState[0]->ShowWindow(SW_SHOW);
		else		mDrawBadState[0]->ShowWindow(SW_HIDE);

		int nGridH = 24;
		int nGridV = 5;
		int nMaxUnit = 5;
		mDrawBadState[0]->CreateGraph(0, nGridV, nGridH, nMaxUnit);
		mDrawBadState[0]->SetGroupCnt(3, 25);	//group, data
		mDrawBadState[0]->SetDrawEanble(TRUE);
	}

	if(mDrawBadState[1] == NULL){
		mDrawBadState[1] = new CDrawBadState(this);
		mDrawBadState[1]->Create(IDD_DRAW_BAD_STATE, NULL);
		CRect rFrame;
		m_CamDisplay->GetDlgItem(IDC_STATIC_GRAPH2)->GetWindowRect(rFrame);

		mDrawBadState[1]->m_rtArea = rFrame;
		mDrawBadState[1]->MoveWindow(mDrawBadState[1]->m_rtArea.left,mDrawBadState[1]->m_rtArea.top,
			mDrawBadState[1]->m_rtArea.Width(),mDrawBadState[1]->m_rtArea.Height());
		if(m_pDlgSet->m_FormTab2->m_bDrawGraph)
			mDrawBadState[1]->ShowWindow(SW_SHOW);
		else 		mDrawBadState[1]->ShowWindow(SW_HIDE);

		int nGridH = 0;
		int nGridV = 5;
		int nMaxUnit = 10;
		mDrawBadState[1]->CreateGraph(1, nGridV, nGridH, nMaxUnit);
		mDrawBadState[1]->SetGroupCnt(4, 3);	//defectType, data
		mDrawBadState[1]->SetDrawEanble(TRUE);
	}

	if(mDrawBadState[2] == NULL){
		mDrawBadState[2] = new CDrawBadState(this);
		mDrawBadState[2]->Create(IDD_DRAW_BAD_STATE, NULL);
		CRect rFrame,rFrame1;
		m_CamDisplay->GetDlgItem(IDC_STATIC_GRAPH3)->GetWindowRect(rFrame);

		mDrawBadState[2]->m_rtArea = rFrame;
		mDrawBadState[2]->MoveWindow(mDrawBadState[2]->m_rtArea.left,mDrawBadState[2]->m_rtArea.top,mDrawBadState[2]->m_rtArea.Width(),mDrawBadState[2]->m_rtArea.Height());
		if(m_pDlgSet->m_FormTab2->m_bDrawGraph)
			mDrawBadState[2]->ShowWindow(SW_SHOW);
		else		mDrawBadState[2]->ShowWindow(SW_HIDE);

		mDrawBadState[2]->m_nTotal = 1;
		int nGridH = 24;
		int nGridV = 5;
		int nMaxUnit = 5;
		mDrawBadState[2]->CreateGraph(0, nGridV, nGridH, nMaxUnit);
		mDrawBadState[2]->SetGroupCnt(3, 25);	//group, data
		mDrawBadState[2]->SetDrawEanble(TRUE);
	}
	m_pSocHunterlapTop = new CConnect(this);

// 	if(m_pDlgSet->m_FormTab2->m_bDrawGraph)
// 		NgTestData();


}

int CFlyCapture2_EXDlg::InitCam(void)
{
	putenv("PYLON_GIGE_HEARTBEAT=5000");
	BOOL bRet = TRUE;
	CString str;
	for (int i = 0; i < MAX_CAMERAS; i++)
	{
		switch (i)
		{
		case 0:
			m_Vision[i] = new CGigECamera("192.168.11.11", i,1);
			break;

		case 1:
			m_Vision[i] = new CGigECamera("192.168.12.11", i,1);	//COLOR
			break;

		case 2:
			m_Vision[i] = new CGigECamera("192.168.13.1", i,1);
			break;

		case 3:
			m_Vision[i] = new CGigECamera("192.168.14.1", i,1);
			break;

		default:
			break;
		}

		if (m_Vision[i] != NULL)
		{
			if (m_Vision[i]->Open() == true)
			{
				str.Format("%d Connection OK",i+1);
				//m_Vision[i]->SetAOI(0,0,CAMERA_HEIGHT,CAM_WIDTH);
				//m_Vision[i]->SetAOI(0,0,CAM_WIDTH,CAMERA_HEIGHT); 
				m_Vision[i]->InitGigE(this);
				//m_Vision[i]->SetMode(MODE_SOFTTRIGGER);
				m_Vision[i]->SetMode(MODE_HARDTRIGGER);
				//m_Vision[i]->SetMode(MODE_CONTINUE);//MODE_SOFTTRIGGER);//

//				m_Vision[i]->SetExpose(14000);

				if(m_pDlgSet->m_FormTab2->m_nCamShutter<50)
					m_pDlgSet->m_FormTab2->m_nCamShutter = 70;
//				m_Vision[i]->SetExpose(m_pDlgSet->m_FormTab2->m_nCamShutter);
				m_bCamStats[i] = TRUE;
			}
			else
			{
				str.Format("%d Connection Fail",i+1);
				delete m_Vision[i];
				m_Vision[i] = NULL;
				bRet = FALSE;
				m_bCamStats[i] = FALSE;
			}
			//AddtoList(str);
		}
	}
	if(bRet)
	{
		m_strStats += " Camera OK";
		LabelStats(m_strStats,m_nStats);	
	}
	else
	{
		m_strStats += " Camera Error";
		m_nStats++;
		LabelStats(m_strStats,m_nStats);	
	}

	return 0;
}

void CFlyCapture2_EXDlg::OnBnClickedFolding()
{
	CFlyCapture2_EXApp* pApp = (CFlyCapture2_EXApp*)AfxGetApp(); 
	pApp->m_pMainWnd->ShowWindow(SW_SHOWMINIMIZED);
}

void CFlyCapture2_EXDlg::OnBnClickedButton5()
{
	m_bAuto = false;

	if(m_nStats==2)
	{
		AfxMessageBox(_T("Camera Connection Check!"));
	}
	else
	{
		GetDlgItem(IDC_MFCBUTTON_START)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_MFCBUTTON_STOP)->ShowWindow(SW_HIDE);
	}
}

void CFlyCapture2_EXDlg::ViewData(void)
{
	CString str;
	
	str.Format(_T("  COUNT : %d EA"), m_nOKCount + m_nNgCount);
	//str.Format(_T("  COUNT : %d EA / IO Trg(%d)"), m_nOKCount + m_nNgCount, m_nIOTriCount);
	m_lbViewData[1].SetText(str);	
	
	str.Format(_T("  Tact Time : %.0f ms"),m_dTactTime*1000);
	m_lbViewData[4].SetText(str);
//	m_lbViewData[4].ShowWindow(SW_HIDE);

	str.Format(_T("  OK/NG: %d / %d(R/B:%d, D/L:%d)"),m_nOKCount,m_nNgCount, m_nRBNgCount, m_nDLNgCount);
	m_LabelBadCount.SetText(str);

	if(m_Project.m_nTapOkNg==RET_OK){
		m_ctInsp.SetBkColor(COLOR_GREEN);
		m_ctInsp.SetText("Tab OK");
	}
	else
	{
		m_ctInsp.SetBkColor(COLOR_RED);
		m_ctInsp.SetText("Tab NG");
	}

	if(m_Project.m_nHoleOkNg==RET_OK){
		m_LabelNonSense.SetBkColor(COLOR_GREEN);
		m_LabelNonSense.SetText("HOLE OK");
	}
	else
	{
		m_LabelNonSense.SetBkColor(COLOR_RED);
		m_LabelNonSense.SetText("HOLE NG");
	}
}

void CFlyCapture2_EXDlg::InitIO(void)
{
	long Ret = m_IO.InitIO(this,0);

	if(Ret==0){
		m_strStats.Format(_T("I/O Card Error"));
		m_nStats=1;
	}
	else
	{
		m_strStats.Format(_T("I/O Card OK"));
		m_nStats = 0;
	}
}

void CFlyCapture2_EXDlg::WriteJsonFile(){//woo json file
	Json::Value root;
	Json::Value encoding;
	CTime time = CTime::GetCurrentTime();
	CString strEquipmentID, strMeter, strData;

	SYSTEMTIME cur_time; 
	GetLocalTime(&cur_time); 
	CString strPCTime;

	strPCTime.Format("%04d%02d%02d%02d%02d%02d",
		cur_time.wYear,
		cur_time.wMonth,
		cur_time.wDay,
		cur_time.wHour,
		cur_time.wMinute, 
		cur_time.wSecond);

	root["Time"] = strPCTime.GetBuffer();
	root["deviceId"] = "A0000001";
	

	m_pDlgSet->m_FormTab4->m_dTapSize; //Tab사이즈
	m_pDlgSet->m_FormTab4->m_dTabTol;//TAB Left
	m_pDlgSet->m_FormTab4->m_dTabTol1;//Right
	m_pDlgSet->m_FormTab4->m_dTapTolY1;//Top
	m_pDlgSet->m_FormTab4->m_dTapTolY2;//Bottom
	m_pDlgSet->m_FormTab4->m_nTabTopTh;//TabTopTh
	m_pDlgSet->m_FormTab4->m_dLeftArea;//용접 Blur
	m_pDlgSet->m_FormTab4->m_dLeftArea1;//용접 Strong
	m_pDlgSet->m_FormTab4->m_nWeldingSize;//weldingBoxSize
	m_pDlgSet->m_FormTab4->m_nWeldCheck;//WeldCheck
	m_pDlgSet->m_FormTab4->m_dHoleEdgeINmm;//HoleEdgeIN
	m_pDlgSet->m_FormTab4->m_dHoleEdgemm;//HoleEdgeOUT
	m_pDlgSet->m_FormTab4->m_dCenterOffset2;//탑캡에서 캔까지 거리Min
	m_pDlgSet->m_FormTab4->m_dCenterOffset;//탑캡에서 캔까지 거리Max
	m_pDlgSet->m_FormTab4->m_dWeldDistCapMin;//탑캡에서 용접까지 거리 min
	m_pDlgSet->m_FormTab4->m_dWeldDistCapMax;//탑캡에서 용접까지 거리 max
	m_pDlgSet->m_FormTab4->m_dDistWeldMin;//용접에서 캔까지 min
	m_pDlgSet->m_FormTab4->m_dDistWeldMax;//용접에서 캔까지 max
	m_pDlgSet->m_FormTab4->m_nCheckCanValue;//Check Can
	m_pDlgSet->m_FormTab4->m_nTopCapValue;//Check Cap
	m_pDlgSet->m_FormTab4->m_nCenterEdgeValue;//TabEdgeV
	m_pDlgSet->m_FormTab4->m_dCapCandistX;//CAP-CAN X
	m_pDlgSet->m_FormTab4->m_nCapReverseV;//CAP EDGE V
	m_pDlgSet->m_FormTab4->m_nWeakWeld;//Weak V Min
	m_pDlgSet->m_FormTab4->m_nWeakWeld2;//Weak V Max
	m_pDlgSet->m_FormTab4->m_nTabCornerDist;//Tab Coner Gap

	CString strFolderPath;
	CString strFolder, strTopBack1;
	CString strFileName;

	strFolder = _T("Jsonnnnnnnnn");

	strFolderPath.Format(_T("D:\\Data"));
	CreateDirectory(strFolderPath, NULL);
	strFolderPath.Format(_T("D:\\Data\\%s"), time.Format(_T("%Y%m%d")));
	CreateDirectory(strFolderPath, NULL);
	strFolderPath.Format(_T("D:\\Data\\%s\\%s"), time.Format(_T("%Y%m%d")), time.Format(_T("%H")));
	CreateDirectory(strFolderPath, NULL);
	strFolderPath.Format(_T("D:\\Data\\%s\\%s\\%s\\"), time.Format(_T("%Y%m%d")), time.Format(_T("%H")), strFolder);	
	CreateDirectory(strFolderPath, NULL);


	strFileName.Format("test.json");

	Json::StyledWriter writer;
	std::string outputConfig = writer.write( root );
	bool result = WriteToFile(strFolderPath+strFileName, outputConfig.c_str(), outputConfig.length());
	

}
bool CFlyCapture2_EXDlg::WriteToFile(const char* filename, const char* buffer, int len)
{
	FILE* r = fopen(filename, "wb");
	if (NULL == r)
		return false;
	size_t fileSize = fwrite(buffer, 1, len, r);
	fclose(r);
	return true;
}

void CFlyCapture2_EXDlg::InitDisplay(void)
{
	m_bMono[0] = TRUE;
	CRect rcMain,rcDest[2];
	GetWindowRect(rcMain);

	//전체화면 이미지 사이즈
	GetDlgItem(IDC_DRAW_VIEW)->MoveWindow(3,304,1020,513);
	GetDlgItem(IDC_DRAW_VIEW2)->MoveWindow(3,304,1020,513);
	GetDlgItem(IDC_DRAW_VIEW)->GetWindowRect(rcDest[0]);
	GetDlgItem(IDC_DRAW_VIEW2)->GetWindowRect(rcDest[1]);	
	//	m_rtDisplayView[0] = rtDest[0];
	//	m_rtDisplayView[1] = rtDest[1];

	//	rcDest1.OffsetRect(-rcMain.left,-rcMain.top);
	for(int i=0; i<MAX_CAMERAS; i++)
		rcDest[i].OffsetRect(-rcMain.left,-rcMain.top);		

//	GetDlgItem(IDC_STATIC_OK_NG_RESULT)->MoveWindow(981,70,299,150);
//	GetDlgItem(IDC_STATICSTATS)->MoveWindow(981,221,299,50);
//	GetDlgItem(IDC_STATIC_LABEL2)->MoveWindow(981,272,299,50);
//	GetDlgItem(IDC_STATIC_LABEL_BADCOUNT)->MoveWindow(981,323,299,50);
////	GetDlgItem(IDC_STATIC_LABEL5)->MoveWindow(981,374,299,50);
//	GetDlgItem(IDC_STATIC_LABEL_YEILD2)->MoveWindow(981,374,299,50);//(981,425,299,50);
//
//
////	GetDlgItem(IDC_LIST_LOG2)->MoveWindow(981,486,299,165);
////	GetDlgItem(IDC_LIST_LOG5)->MoveWindow(981,651,299,165);
//	GetDlgItem(IDC_LIST_LOG2)->MoveWindow(981,486-50,299,165-50);
//	GetDlgItem(IDC_LIST_LOG5)->MoveWindow(981,651-50,299,165+50);
//

	int CountLabX=746, CountLabW=534, CountLabH=33;
	GetDlgItem(IDC_STATIC_LABEL_BCODE)->MoveWindow(CountLabX,820,CountLabW,CountLabH);
	GetDlgItem(IDC_STATIC_LABEL_YEILD)->MoveWindow(CountLabX,854,CountLabW,CountLabH);
	GetDlgItem(IDC_STATIC_LABEL_BRIGHT)->MoveWindow(CountLabX,888,CountLabW,CountLabH);
	GetDlgItem(IDC_STATIC_LABEL_NGMARK)->MoveWindow(CountLabX,922,CountLabW,CountLabH);
	GetDlgItem(IDC_STATIC_LABEL_BCR)->MoveWindow(CountLabX,956,CountLabW,CountLabH);

	int BadY=820, BadW=140, BadH=140;
	GetDlgItem(IDC_STATIC_BAD1)->MoveWindow(3,BadY,BadW,BadH);
	GetDlgItem(IDC_STATIC_BAD2)->MoveWindow(153,BadY,BadW,BadH);	
	GetDlgItem(IDC_STATIC_BAD3)->MoveWindow(303,BadY,BadW,BadH);
	GetDlgItem(IDC_STATIC_BAD4)->MoveWindow(453,BadY,BadW,BadH);	
	GetDlgItem(IDC_STATIC_BAD5)->MoveWindow(603,BadY,BadW,BadH);
	//약용접
	GetDlgItem(IDC_STATIC_BAD6)->MoveWindow(3,680,70,70);

	int BadLabelY=963, BadLW=140, BadLH=20;
	GetDlgItem(IDC_STATIC_LABEL_TACTTIME8)->MoveWindow(3,BadLabelY,BadLW,BadLH);
	GetDlgItem(IDC_STATIC_LABEL_TACTTIME9)->MoveWindow(153,BadLabelY,BadLW,BadLH);
	GetDlgItem(IDC_STATIC_LABEL_TACTTIME10)->MoveWindow(303,BadLabelY,BadLW,BadLH);
	GetDlgItem(IDC_STATIC_LABEL_TACTTIME11)->MoveWindow(453,BadLabelY,BadLW,BadLH);
	GetDlgItem(IDC_STATIC_LABEL_TACTTIME12)->MoveWindow(603,BadLabelY,BadLW,BadLH);

	for(int i=0; i<MAX_CAMERAS; i++)
		rcDest[i].OffsetRect(-rcMain.left,-rcMain.top);		

//전체화면 이미지 사이즈
	CRect rtFullImg(0,0,CAMERA_WIDTH,CAMERA_HEIGHT);
	CRect rtRoi(rtFullImg);

	CRect rtCharImg(0,0,int(CAMERA_WIDTH/m_Project.m_nReduceScale),int(CAMERA_HEIGHT/m_Project.m_nReduceScale));
	CRect rtCharRoi(rtCharImg);

	CRect rtEtcImg(0,0,CHAR_AREA_WIDTH,CHAR_AREA_HEIGHT);
	CRect rtEtcRoi(rtEtcImg);

	
	//rtRoi.SetRect(0,0,CAMERA_WIDTH,600);
	for(int i=0;i<MAX_CAMERAS;i++){

		m_Display[i].m_bTrackerUse = TRUE;

		m_Display[i].InitDisplay(this,m_bMono[i],rcDest[i],rtFullImg,rtRoi);
		m_DisplaySave[i].InitDisplay(this,m_bMono[i],rcDest[0],rtFullImg,rtRoi);
		m_DisplayServer[i].InitDisplay(this,m_bMono[i],rcDest[0],rtFullImg,rtRoi);
		
	}
	m_SaveNG.InitDisplay(this,1,rcDest[0],rtFullImg,rtRoi);
	for(int i=0;i<6;i++){
		CRect rtbad(0,0,300,192);
		GetDlgItem(IDC_STATIC_BAD1+i)->GetWindowRect(rcDest[0]);
		m_DisplayBad[i].InitDisplay(this,1,rcDest[0],rtbad,rtbad);
	}
	rtRoi.SetRect(0,0,NG_CROP_SIZE_X,NG_CROP_SIZE_Y);
	m_DisplayNgCrop.InitDisplay(this,1,rcDest[0],rtRoi,rtRoi);

	rtRoi.SetRect(0, 0, CAMERA_WIDTH, CAMERA_HEIGHT_960);
	m_DisplayDLOverlay.InitDisplay(this, FALSE, rtRoi, rtRoi, rtRoi);
}

void CFlyCapture2_EXDlg::ReLoadDisplay(void)
{

	CRect rcMain,rcDest,rcDest1;
	GetWindowRect(rcMain);
	GetDlgItem(IDC_DRAW_VIEW)->GetWindowRect(rcDest);
	GetDlgItem(IDC_DRAW_VIEW2)->GetWindowRect(rcDest1);
	rcDest.OffsetRect(-rcMain.left,-rcMain.top);
	rcDest1.OffsetRect(-rcMain.left,-rcMain.top);

	int nNewHeight = CAMERA_HEIGHT;
	//전체화면 이미지 사이즈
	CRect rtFullImg(0,0,CAMERA_WIDTH,nNewHeight);
	CRect rtRoi(rtFullImg);

	CRect rtCharImg(0,0,int(CAMERA_WIDTH/m_Project.m_nReduceScale),int(nNewHeight/m_Project.m_nReduceScale));
	CRect rtCharRoi(rtCharImg);
	CRect rtEtcImg(0,0,CHAR_AREA_WIDTH,CHAR_AREA_HEIGHT);
	CRect rtEtcRoi(rtEtcImg);

//기존 Display 를 날린다.

//새로 생성.
	//rtRoi.SetRect(0,0,CAMERA_WIDTH,600);
	for(int i=0;i<MAX_CAMERAS;i++)
	{
		m_Display[i].InitDisplay(this,1,rcDest[i],rtFullImg,rtRoi);
//	m_Display1.InitDisplay(this,rcDest1,rtFullImg,rtRoi);
		m_DisplaySave[i].InitDisplay(this,1,rcDest[0],rtFullImg,rtRoi);
	}
}


BOOL CFlyCapture2_EXDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{

	CgDisplay* pDisplay = GetScreen();

	pDisplay->SetZoom(zDelta,pt);
	DrawDisplay2(0);

	return CDialog::OnMouseWheel(nFlags, zDelta, pt);
}


void CFlyCapture2_EXDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	CgDisplay* pDisplay = GetScreen();
	pDisplay->SetMousePt(point);
	pDisplay->TrackerCtrl(this,point);

	CDialog::OnLButtonDown(nFlags, point);
}

void CFlyCapture2_EXDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	CgDisplay* pDisplay = GetScreen();

	if((nFlags & MK_LBUTTON)==MK_LBUTTON)
	{
		pDisplay->SetMove(point);
		DrawDisplay2(0);
//		m_ptMouseStt = point;
	}
	int nValue;
	CPoint pt;
	CString str;
	pt = pDisplay->GetPos(point,&nValue);
	if(pt.x>CAMERA_WIDTH || pt.x<0||pt.y>CAMERA_HEIGHT||pt.y<0)
	{

	}
	else
	{
		str.Format("x=%d y=%d v=%d",pt.x,pt.y,nValue);
		m_btInfo.SetWindowTextA(str);

	}
	CDialog::OnMouseMove(nFlags, point);
}

CgDisplay* CFlyCapture2_EXDlg::GetScreen()
{
	CgDisplay* pDisplay; 

	pDisplay = &m_Display[0];
	return pDisplay;

//전체화면, 문자화면 ,기타 화면 분기
	switch (m_nScreen)
	{
	case 0:		
		//라디오 버튼 전체화면일 경우
		pDisplay = &m_Display[0];
		break;

	case 1:		
		//라디오 버튼 문자화면일 경우
//		pDisplay = &m_Display_Char;
		//m_Display_Char.DrawImage(pDC);
		break;

	case 2:		
		//라디오 버튼 기타화면일 경우
		//m_Display_AreaChar[0].DrawImage(pDC);
		break;
	case 3:		
		//라디오 버튼 기타화면일 경우
		//m_Display_AreaChar[1].DrawImage(pDC);
		break;
	}

	return pDisplay;
}
void CFlyCapture2_EXDlg::DrawTime()
{
	CDC* pDC=GetDC();

	CString strtime;
	CPoint pttime;
	int noffset = 30;
	for(int i=0;i<9;i++)if(m_Project.m_dInspectTime[i]<0)m_Project.m_dInspectTime[i] = 0;
	pttime.SetPoint(1000,700);
	strtime.Format("Total %.0fms",m_Project.m_dInspectTime[8]-m_Project.m_dInspectTime[0]);
	m_Display[0].DrawText(pDC,pttime,strtime,1,COLOR_GREEN,COLOR_BLACK);

	pttime.SetPoint(1000,700+noffset);
	strtime.Format("1. %.0fms Grab",m_Project.m_dInspectTime[1]-m_Project.m_dInspectTime[0]);
	m_Display[0].DrawText(pDC,pttime,strtime,1,COLOR_GREEN,COLOR_BLACK);
	pttime.SetPoint(1000,700+noffset*2);
	strtime.Format("2. %.0fms memcopy",m_Project.m_dInspectTime[2]-m_Project.m_dInspectTime[1]);
	m_Display[0].DrawText(pDC,pttime,strtime,1,COLOR_GREEN,COLOR_BLACK);
	pttime.SetPoint(1000,700+noffset*3);
	strtime.Format("3. %.0fms Usrdisplay",m_Project.m_dInspectTime[3]-m_Project.m_dInspectTime[2]);
	m_Display[0].DrawText(pDC,pttime,strtime,1,COLOR_GREEN,COLOR_BLACK);
	pttime.SetPoint(1000,700+noffset*4);
	strtime.Format("4. %.0fms TapInsp",m_Project.m_dInspectTime[5]-m_Project.m_dInspectTime[4]);
	m_Display[0].DrawText(pDC,pttime,strtime,1,COLOR_GREEN,COLOR_BLACK);
	pttime.SetPoint(1000,700+noffset*5);
	strtime.Format("5. %.0fms Hole Insp",m_Project.m_dInspectTime[6]-m_Project.m_dInspectTime[5]);
	m_Display[0].DrawText(pDC,pttime,strtime,1,COLOR_GREEN,COLOR_BLACK);
	pttime.SetPoint(1000,700+noffset*6);
	strtime.Format("6. %.0fms Display",m_Project.m_dInspectTime[7]-m_Project.m_dInspectTime[6]);
	m_Display[0].DrawText(pDC,pttime,strtime,1,COLOR_GREEN,COLOR_BLACK);
	pttime.SetPoint(1000,700+noffset*7);
	strtime.Format("7. %.0fms DataWrite",m_Project.m_dInspectTime[8]-m_Project.m_dInspectTime[7]);
	m_Display[0].DrawText(pDC,pttime,strtime,1,COLOR_GREEN,COLOR_BLACK);
	if(m_Project.m_dInspectTime[7]-m_Project.m_dInspectTime[6]<0)
		strtime = strtime;

	ReleaseDC(pDC);
}
void CFlyCapture2_EXDlg::SetOutputResult(int nCam, int nRBResult, int nDLResult)
{
	if(nRBResult == RET_OK){
		m_IO.m_nBadResult1 = 1;
		m_IO.m_nBadResult2 = 1;

		//딥러닝결과 확인
		if (m_pDlgSet->m_FormTab4->m_bWeakWeldOutputUse){
			if (nDLResult != 1) {
				m_IO.m_nBadResult1 = 1;
				m_IO.m_nBadResult2 = 2;// hole ok signal 
			}
		}
		
		//DeepLearning Result Check
		//if( (pView->m_pDlgSet->m_FormTab4->m_bUseAiIns && pView->m_pDlgSet->m_FormTab4->m_bWeakWeldOutputUse) && pView->m_nAiInsOK == 0 )
		//{	
		//	bNG = TRUE;
		//	pView->m_IO.m_nBadResult1 = 2;
		//	pView->m_nNgCount++;
		//	pView->LabelResult(0);
		//}
		//else
		//{
		//	pView->m_nTimeCount[pView->m_nHour][0]++;//ng
		//	pView->m_nOKCount++;
		//	pView->m_IO.m_nBadResult1 = pView->m_IO.m_nBadResult2 = 1;
		//	pView->LabelResult(1);
		//}

		////딥러닝사용 , Read fail 켜져있을때
		//if((pView->m_pDlgSet->m_FormTab4->m_bUseAiIns && pView->m_pDlgSet->m_FormTab4->m_bWeakWeldOutputUse) && 
		//	pView->m_pDlgSet->m_FormTab2->m_bUseAiReadFail && pView->m_nAiInsOK == -1)	{	
		//		bNG = TRUE;
		//		pView->m_IO.m_nBadResult1 = 2;
		//		pView->m_nNgCount++;
		//		pView->LabelResult(0);
		//}
	}
	else{
		m_IO.m_nBadResult1 = 1; 
		m_IO.m_nBadResult2 = 2;// hole ok signal 

		m_nTimeCount[m_nHour][1]++;//ng
		m_nTimeCount[m_nHour][2]++;//not cap ///0ok	1ng	2notcap 3tab	4hole 5blur 6dist

	}


	if(m_pDlgSet->m_FormTab3->m_bALLOK ){
		m_IO.m_nBadResult1 = 1;
		m_IO.m_nBadResult2 = 1;
	}
	else if(m_pDlgSet->m_FormTab3->m_bALLNG)
	{
		m_IO.m_nBadResult1 = 2;
		m_IO.m_nBadResult2 = 1; // ALL NG
	}
	else if(m_pDlgSet->m_FormTab3->m_bAllNgHole)
	{
		m_IO.m_nBadResult1 =1; 
		m_IO.m_nBadResult2 = 2;// hole ok signal
	}
	else if(m_pDlgSet->m_FormTab3->m_bAllNGDual)
	{
		if(m_Project.m_nFrameCount%2==0){
			m_IO.m_nBadResult1 =2, 
			m_IO.m_nBadResult2 = 1;// hole ok signal
		}
		else{
			m_IO.m_nBadResult1 =1, 
			m_IO.m_nBadResult2 = 2;// hole ok signal
		}
	}
}
void CFlyCapture2_EXDlg::DrawDisplay(int nCam)
{
	m_dData[26] = 1;
	m_dData[28] = 1;
	if(m_bStarting==FALSE)return;
	CDC* pDC=GetDC();
	if(m_nScreen==0)
	{
		m_Display[nCam].DrawImage(pDC);

		//m_Display[nCam].DrawLine(pDC,CPoint(m_Project.m_nStdPointX[2],m_Project.m_nStdPointY[2]-5),CPoint(m_Project.m_nStdPointX[2],m_Project.m_nStdPointY[2]+5),"",COLOR_BLUE,1);
		// NG Thumbnail
		for(int i=0;i<5;i++)
			m_DisplayBad[i].DrawImage(pDC);
		// Top Cap Edge 그리기
		if(m_pDlgSet->m_FormTab2->m_bShowEdge){
			for(int i=0;i<CAMERA_HEIGHT;i++){
				if(m_Project.m_ptDrawCapEdge[0][i].x>0)
					m_Display[0].DrawDotMark(pDC,m_Project.m_ptDrawCapEdge[0][i],COLOR_GREEN,1);
				if(m_Project.m_ptDrawCapEdge[1][i].x>0)
					m_Display[0].DrawDotMark(pDC,m_Project.m_ptDrawCapEdge[1][i],COLOR_GREEN,1);
			}
		}
	
		int ngap=20;
		// Top Cap 중심 +마크 그리기
		m_Display[0].DrawCross(pDC,CRect(m_Project.m_ptCapCenter.x-ngap,m_Project.m_ptCapCenter.y-ngap,m_Project.m_ptCapCenter.x+ngap,m_Project.m_ptCapCenter.y+ngap),COLOR_RED,2,"");
		m_Display[0].DrawRect(pDC,CRect(m_Project.m_ptCapCenter.x-ngap,m_Project.m_ptCapCenter.y-ngap,m_Project.m_ptCapCenter.x+ngap,m_Project.m_ptCapCenter.y+ngap),COLOR_RED,2,"");
		//		m_Display[0].DrawCross(pDC,CRect(m_Project.m_ptCapCenter.x-ngap,m_Project.m_ptCapCenter.y-ngap,m_Project.m_ptCapCenter.x+ngap,m_Project.m_ptCapCenter.y+ngap),COLOR_WHITE,2,"");
		//		m_Display[0].DrawCross(pDC,CRect(m_Project.m_ptCapCenter1.x-ngap,m_Project.m_ptCapCenter1.y-ngap,m_Project.m_ptCapCenter1.x+ngap,m_Project.m_ptCapCenter1.y+ngap),COLOR_RED,2,"");
		m_Display[0].DrawCross(pDC,CRect(m_Project.m_ptLeftTop.x-ngap,m_Project.m_ptLeftTop.y-ngap,m_Project.m_ptLeftTop.x+ngap,m_Project.m_ptLeftTop.y+ngap),COLOR_RED,2,"");
		m_Display[0].DrawCross(pDC,CRect(m_Project.m_ptRightTop.x-ngap,m_Project.m_ptRightTop.y-ngap,m_Project.m_ptRightTop.x+ngap,m_Project.m_ptRightTop.y+ngap),COLOR_RED,2,"");

		CPoint ptcantop;
		ptcantop.SetPoint((m_Project.m_ptCanTop[0].x+m_Project.m_ptCanTop[1].x)*0.5,(m_Project.m_ptCanTop[0].y+m_Project.m_ptCanTop[1].y)*0.5);
		m_ptCanTop.x=ptcantop.x;
		m_ptCanTop.y=ptcantop.y;
		m_Display[0].DrawCross(pDC,CRect(m_Project.m_ptCanTop[0].x-ngap,m_Project.m_ptCanTop[0].y-ngap,m_Project.m_ptCanTop[0].x+ngap,m_Project.m_ptCanTop[0].y+ngap),COLOR_RED,2,"");
		m_Display[0].DrawCross(pDC,CRect(m_Project.m_ptCanTop[1].x-ngap,m_Project.m_ptCanTop[1].y-ngap,m_Project.m_ptCanTop[1].x+ngap,m_Project.m_ptCanTop[1].y+ngap),COLOR_RED,2,"");
		m_Display[0].DrawCross(pDC,CRect(ptcantop.x-ngap,ptcantop.y-ngap,ptcantop.x+ngap,ptcantop.y+ngap),COLOR_RED,2,"");

		m_Display[0].DrawLine(pDC,CPoint(300,m_nTabBottomTop-m_pDlgSet->m_FormTab4->m_nBottomTabTop),CPoint(CAMERA_WIDTH-300,m_nTabBottomTop-m_pDlgSet->m_FormTab4->m_nBottomTabTop),"",COLOR_GREEN,1);
		m_Display[0].DrawLine(pDC,CPoint(300,m_nTabBottomTop-m_pDlgSet->m_FormTab4->m_nBottomTabBot),CPoint(CAMERA_WIDTH-300,m_nTabBottomTop-m_pDlgSet->m_FormTab4->m_nBottomTabBot),"",COLOR_RED,1);

		m_Display[0].DrawCross(pDC,CRect(m_nTabBottomLeft-10,m_nTabBottomTop-10,m_nTabBottomLeft+10,m_nTabBottomTop+10),COLOR_YELLOW,2,"");
		m_Display[0].DrawCross(pDC,CRect(m_nTabBottomRight-10,m_nTabBottomTop-10,m_nTabBottomRight+10,m_nTabBottomTop+10),COLOR_SKYBLUE,2,"");

		m_Display[0].DrawLine(pDC,CPoint(0,ptcantop.y),CPoint(CAMERA_WIDTH,ptcantop.y),"",COLOR_GRAY,2);

		// Circle Fitting용 Cap Edge랑 Tab 찾는 원 그리기
		if(m_pDlgSet->m_FormTab2->m_bCircleShow){
			int noffset = m_pDlgSet->m_FormTab2->m_nInnerCiircle;
			m_Display[0].DrawCircle(pDC,m_Project.m_ptCapCenter,noffset,COLOR_GREEN,1,"");
			m_Display[0].DrawCircle(pDC,m_Project.m_ptCapCenter,m_Project.m_dCabRadius,COLOR_GREEN,1,"");
		}

		if(m_pDlgSet->m_FormTab2->m_bShowTabArea)
		{
			//			m_Display[0].DrawArrayRect(pDC);
			CRect rtCheck;
			rtCheck.SetRect(m_Project.m_ptCapCenter.x-m_pDlgSet->m_FormTab4->m_nTabEdgeX1,m_Project.m_ptCapCenter.y-m_pDlgSet->m_FormTab4->m_nTabEdgeY1,m_Project.m_ptCapCenter.x+m_pDlgSet->m_FormTab4->m_nTabEdgeX2,m_Project.m_ptCapCenter.y+m_pDlgSet->m_FormTab4->m_nTabEdgeY2);
			//Tab Edge V 영역그리기
			m_Display[0].DrawRect(pDC,rtCheck,COLOR_YELLOW,2,"");
			// Tab OK 영역 그리기
			m_Display[0].DrawRect(pDC,m_Project.m_rtTabOkRange,COLOR_BLUE,2,"OK Range");

			CPoint pt1,pt2,pt3,pt4;
			for(int i=0;i<2;i++){
				// Tab 하단 좌우 경계 그리기. 일자로 그어버림

				pt1 = m_Project.m_ptLeftTop;
				pt2.SetPoint(m_Project.m_ptLeftTop.x,pt1.y+30);
				m_Display[0].DrawLine(pDC,pt1,pt2,"",COLOR_WHITE,1);
				pt2.SetPoint(m_Project.m_ptLeftTop.x+30,pt1.y);
				m_Display[0].DrawLine(pDC,pt1,pt2,"",COLOR_WHITE,1);

				pt1 = m_Project.m_ptRightTop;
				pt2.SetPoint(m_Project.m_ptRightTop.x,pt1.y+30);
				m_Display[0].DrawLine(pDC,pt1,pt2,"",COLOR_WHITE,1);
				pt2.SetPoint(m_Project.m_ptRightTop.x-30,pt1.y);
				m_Display[0].DrawLine(pDC,pt1,pt2,"",COLOR_WHITE,1);
			}

			// Tab 상단 Top 경계 그리기. 일자로 그어버림
			// 			pt1.SetPoint(m_Project.m_nTopEdge[0],m_Project.m_nTopPos[0]);
			// 			pt2.SetPoint(m_Project.m_nTopEdge[1],m_Project.m_nTopPos[0]);
			// 			m_Display[0].DrawLine(pDC,pt1,pt2,"",COLOR_WHITE,1);

			// 3,4호기 안씀
			if(m_pDlgSet->m_FormTab4->m_bCheckView){
				pt1.SetPoint(m_Project.m_nTopEdge[0],m_Project.m_nBlackStart);
				pt2.SetPoint(m_Project.m_nTopEdge[1],m_Project.m_nBlackStart);
				m_Display[0].DrawLine(pDC,pt1,pt2,"",COLOR_YELLOW,1);
				pt1.SetPoint(m_Project.m_nTopEdge[0],m_Project.m_nBlackEnd);
				pt2.SetPoint(m_Project.m_nTopEdge[1],m_Project.m_nBlackEnd);
				m_Display[0].DrawLine(pDC,pt1,pt2,"",COLOR_BLUE,1);
			}
			//pt1.SetPoint(m_Project.m_nTopEdge[0],m_Project.m_nCadidateY);
			//pt2.SetPoint(m_Project.m_nTopEdge[1],m_Project.m_nCadidateY);
			//m_Display[0].DrawLine(pDC,pt1,pt2,"",COLOR_RED,1);

			// 각 용접 중심 찍기(흰점)
			for(int i=0;i<m_Project.m_nHoleCandiCount;i++)
			{
				//				m_Display[0].DrawPoint(pDC,m_Project.m_ptHole[i],COLOR_RED,2);
				m_Display[0].DrawPoint(pDC,m_Project.m_ptHole1[i],COLOR_WHITE,2);
				//				if(m_pDlgSet->m_FormTab4->m_bTapImageShow)
				//					m_Display[0].DrawPoint(pDC,m_Project.m_ptHole2[i],COLOR_WHITE,2);
			}

			// Hole 영역 그리기(초록박스)
			//			if(m_Project.m_nTapOkNg==1 && m_pDlgSet->m_FormTab2->m_bShowHole)
			{
				for(int i=0;i<2;i++){
					if(i==1 && m_pDlgSet->m_FormTab2->m_b2LineWelding==FALSE && m_pDlgSet->m_FormTab2->m_bLineWelding==TRUE)
						continue;

					m_Display[0].DrawRect(pDC,m_Project.m_rtWeldRect[i],COLOR_GREEN,1,"");
				}
			}
			m_Display[0].DrawRect(pDC,m_Project.m_rtWeldPos,COLOR_GREEN,1,"");
			if(m_pDlgSet->m_FormTab4->m_bMidWeld){
				m_Display[0].DrawRect(pDC,m_Project.m_rtMidWeldPos,COLOR_JOO,1,"");
			}
			// Can Info
			CString str1;
			str1.Format("%d [%d]",m_nCanCheckAve,m_pDlgSet->m_FormTab4->m_nCheckCanValue);
			m_Display[0].DrawRect(pDC,m_Project.m_rtCanCheckROI,COLOR_YELLOW,1,str1);

			int nx = 800;
			double dCapCanDist = 0;
			BOOL bstats = FALSE;
			dCapCanDist = (abs(m_Project.m_ptCapCenter.y- (ptcantop.y))*m_pDlgSet->m_FormTab2->m_dCamScale);

			str1.Format("Select Frame : %d",m_nSelectFrame);//
			m_Label_Ins_Data2[0].SetText(str1);

			m_dCapCanDistX = m_dData[1] = (m_Project.m_ptCapCenter.x- (ptcantop.x)) * m_pDlgSet->m_FormTab2->m_dCamScale;
			str1.Format("Cap-Can X : %.2f(%.2f)",m_dCapCanDistX ,m_pDlgSet->m_FormTab4->m_dCapCandistX);//m_nCanCheckAve, //pView->m_nMorphCount[0]
			//if (m_dCapCanDistX > m_pDlgSet->m_FormTab4->m_dCapCandistX && m_pDlgSet->m_FormTab4->m_dCapCandistX>0)
			if (m_nRBResult == NG_CAN_CAP_DIST_X)
			{
				m_Label_Ins_Data2[1].SetTextColor(COLOR_RED);
				m_Label_Ins_Data2[1].SetText(str1);
				//m_Display[0].DrawText(pDC,CPoint(400,50),str1,0,COLOR_BLACK,COLOR_RED);
				m_bResult[1] = TRUE;
				m_dData[28] = 0;

			}
			else{
				m_Label_Ins_Data2[1].SetTextColor(COLOR_GREEN);
				m_Label_Ins_Data2[1].SetText(str1);
				//m_Display[0].DrawText(pDC,CPoint(400,50),str1);
				m_bResult[1] = FALSE;
			}

			m_dData[2] = (ptcantop.y-m_Project.m_ptCapCenter.y);//220518 픽셀로 변환 중
			if (m_pDlgSet->m_FormTab4->m_bUseCapCanY)
			{
				m_dCapCanDistY = abs(m_Project.m_ptCapCenter.y- ptcantop.y)*m_pDlgSet->m_FormTab2->m_dCamScale;
 				str1.Format("Cap-Can Y : %.2f(%.2f~%.2f)",m_dCapCanDistY,m_pDlgSet->m_FormTab4->m_dCenterOffset2,m_pDlgSet->m_FormTab4->m_dCenterOffset);
				if (dCapCanDist>m_pDlgSet->m_FormTab4->m_dCenterOffset || (dCapCanDist<m_pDlgSet->m_FormTab4->m_dCenterOffset2 && dCapCanDist>0))
				{
					m_Label_Ins_Data2[2].SetTextColor(COLOR_RED);
					m_Label_Ins_Data2[2].SetText(str1);
					//m_Display[0].DrawText(pDC,CPoint(400,80),str1,0,COLOR_BLACK,COLOR_RED);
					m_bResult[2] = TRUE;
					m_dData[28] = 0;

				}
				else{
					m_Label_Ins_Data2[2].SetTextColor(COLOR_GREEN);
					m_Label_Ins_Data2[2].SetText(str1);
					//m_Display[0].DrawText(pDC,CPoint(400,80),str1);
					m_bResult[2] = FALSE;
				}

			}

			if(m_pDlgSet->m_FormTab2->m_bTabReFind ){
				str1.Format("Tab Corner L %d  R %d  Gap %d",m_Project.m_ptLeftTop.y,m_Project.m_ptRightTop.y,m_Project.m_ptLeftTop.y-m_Project.m_ptRightTop.y);
				if(m_pDlgSet->m_FormTab4->m_bTabCornerDist && m_pDlgSet->m_FormTab4->m_nTabCornerDist <= abs(m_Project.m_ptLeftTop.y-m_Project.m_ptRightTop.y))
				{
					m_Label_Ins_Data2[3].SetTextColor(COLOR_RED);
					m_Label_Ins_Data2[3].SetText(str1);
					//m_Display[0].DrawText(pDC,CPoint(400,120),str1,0,COLOR_BLACK,COLOR_RED);
					m_dData[28] = 0;

				}
				else
				{
					m_Label_Ins_Data2[3].SetTextColor(COLOR_GREEN);
					m_Label_Ins_Data2[3].SetText(str1);
					//m_Display[0].DrawText(pDC,CPoint(400,120),str1);
				}

			}
			if(m_pDlgSet->m_FormTab2->m_bShowHole && m_pDlgSet->m_FormTab2->m_bWeldInTab && m_pDlgSet->m_FormTab4->m_bWeldDist ){
				ngap = 10;
				double ddist1 = ( ptcantop.y-m_ptWeldCenterPos.y)*m_pDlgSet->m_FormTab2->m_dCamScale;
				//m_dData[3] = ddist1;
				m_dData[3] = ptcantop.y-m_ptWeldCenterPos.y;//220518 픽셀로 변환 중
				//				m_Display[0].DrawCross(pDC,CRect(m_ptWeldCenterPos.x-ngap,m_ptWeldCenterPos.y-ngap,m_ptWeldCenterPos.x+ngap,m_ptWeldCenterPos.y+ngap),COLOR_BLUE,3,"");
				str1.Format("Weld-Can Dist. P(%d) %.1fmm  (%.1f~%.1f)",ptcantop.y-m_ptWeldCenterPos.y,ddist1,m_pDlgSet->m_FormTab4->m_dDistWeldMin,m_pDlgSet->m_FormTab4->m_dDistWeldMax);
				if((ddist1 > m_pDlgSet->m_FormTab4->m_dDistWeldMax || ddist1<m_pDlgSet->m_FormTab4->m_dDistWeldMin) && ddist1>0){
					m_Label_Ins_Data2[4].SetTextColor(COLOR_RED);
					m_Label_Ins_Data2[4].SetText(str1);
					//m_Display[0].DrawText(pDC,CPoint(400,140),str1,0,COLOR_BLACK,COLOR_RED);
					m_bResult[3] = TRUE;
					m_dData[26] = 0;

				}
				else{		
					m_Label_Ins_Data2[4].SetTextColor(COLOR_GREEN);
					m_Label_Ins_Data2[4].SetText(str1);
					//m_Display[0].DrawText(pDC,CPoint(400,140),str1);
					m_bResult[3] = FALSE;
				}
			}
			double ddist2 = (m_ptWeldCenterPos.y-m_Project.m_ptCapCenter.y)*m_pDlgSet->m_FormTab2->m_dCamScale;
			//m_dData[4] = ddist2; 
			m_dData[4] = (m_ptWeldCenterPos.y-m_Project.m_ptCapCenter.y);//220518 픽셀로 변환 중
			//m_Display[0].DrawCross(pDC,CRect(m_ptWeldCenterPos.x-ngap,m_ptWeldCenterPos.y-ngap,m_ptWeldCenterPos.x+ngap,m_ptWeldCenterPos.y+ngap),COLOR_BLUE,3,"");
			if(m_pDlgSet->m_FormTab4->m_bWeldDistCap)
			{
				str1.Format("Weld-TopCap Dist. P(%d) %.1fmm  (%.1f~%.1f)",m_ptWeldCenterPos.y-m_Project.m_ptCapCenter.y,ddist2,m_pDlgSet->m_FormTab4->m_dWeldDistCapMin,m_pDlgSet->m_FormTab4->m_dWeldDistCapMax);
				if((ddist2 > m_pDlgSet->m_FormTab4->m_dWeldDistCapMax || ddist2<m_pDlgSet->m_FormTab4->m_dWeldDistCapMin)&& ddist2>0){
					m_Label_Ins_Data2[5].SetTextColor(COLOR_RED);
					m_Label_Ins_Data2[5].SetText(str1);
					//m_Display[0].DrawText(pDC,CPoint(400,160),str1,0,COLOR_BLACK,COLOR_RED);
					m_bResult[4] = TRUE;
					m_dData[26] = 0;

				}
				else{		
					m_Label_Ins_Data2[5].SetTextColor(COLOR_GREEN);
					m_Label_Ins_Data2[5].SetText(str1);
					//m_Display[0].DrawText(pDC,CPoint(400,160),str1);
					m_bResult[4] = FALSE;
				}
			}
			if (m_pDlgSet->m_FormTab4->m_bCanDist)//Can 높이 woo 
			{
				str1.Format("Can Dist. %.1fmm  (%.1f~%.1f)",m_dCanDistY,m_pDlgSet->m_FormTab4->m_dCanDistMin,m_pDlgSet->m_FormTab4->m_dCanDistMax);
				if((m_dCanDistY > m_pDlgSet->m_FormTab4->m_dCanDistMax || m_dCanDistY<m_pDlgSet->m_FormTab4->m_dCanDistMin)&& m_dCanDistY>0)
				{
					m_Label_Ins_Data2[6].SetTextColor(COLOR_RED);
					m_Label_Ins_Data2[6].SetText(str1);
					//m_Display[0].DrawText(pDC,CPoint(400,100),str1,0,COLOR_BLACK,COLOR_RED);
					m_dData[28] = 0;

				}
				else
				{
					m_Label_Ins_Data2[6].SetTextColor(COLOR_GREEN);
					m_Label_Ins_Data2[6].SetText(str1);
					//m_Display[0].DrawText(pDC,CPoint(400,100),str1);
				}
			}


			if(m_pDlgSet->m_FormTab4->m_nCapReverseV>0){// && m_nTopCapV > m_pDlgSet->m_FormTab4->m_nCapReverseV>0)
				m_nTopCapV = m_dData[5];
				str1.Format("Cap Edge V:  %d  (%d)",m_nTopCapV,m_pDlgSet->m_FormTab4->m_nCapReverseV);
				if(m_nTopCapV > m_pDlgSet->m_FormTab4->m_nCapReverseV){
					m_Label_Ins_Data2[7].SetTextColor(COLOR_RED);
					m_Label_Ins_Data2[7].SetText(str1);
					//m_Display[0].DrawText(pDC,CPoint(400,200),str1,0,COLOR_BLACK,COLOR_RED);
					m_bResult[5] = TRUE;
					m_dData[23] = 0;
				}
				else{
					m_Label_Ins_Data2[7].SetTextColor(COLOR_GREEN);
					m_Label_Ins_Data2[7].SetText(str1);
					//m_Display[0].DrawText(pDC,CPoint(400,200),str1);
					m_bResult[5] = FALSE;
					m_dData[23] = 1;
				}
			}
			str1.Format("Weak V:  %d  (%d ~ %d)",m_nWeakWeld,m_pDlgSet->m_FormTab4->m_nWeakWeld,m_pDlgSet->m_FormTab4->m_nWeakWeld2);
			m_dData[6] = m_nWeakWeld;
			if(m_pDlgSet->m_FormTab4->m_bWeakWeld && m_nWeakWeld>=0 && m_nWeakWeld < m_pDlgSet->m_FormTab4->m_nWeakWeld || m_nWeakWeld > m_pDlgSet->m_FormTab4->m_nWeakWeld2){
				m_Label_Ins_Data2[8].SetTextColor(COLOR_RED);
				m_Label_Ins_Data2[8].SetText(str1);
				//m_Display[0].DrawText(pDC,CPoint(400,230),str1,0,COLOR_BLACK,COLOR_RED);
				m_bResult[6] = TRUE;
				m_dData[24] = 0;
			}
			else{
				m_Label_Ins_Data2[8].SetTextColor(COLOR_GREEN);
				m_Label_Ins_Data2[8].SetText(str1);
				//m_Display[0].DrawText(pDC,CPoint(400,230),str1);
				m_bResult[6] = FALSE;
				m_dData[24] = 1;
			}

			//			if(!m_pDlgSet->m_FormTab4->m_bTwoTab){ //woo
			str1.Format("Tab Edge V:  %d  ( %d )",m_nTabTopEdge,m_pDlgSet->m_FormTab4->m_nCenterEdgeValue);
			m_dData[7] = m_nTabTopEdge;
			if(m_nTabTopEdge >m_pDlgSet->m_FormTab4->m_nCenterEdgeValue  || m_nTabTopEdge<0){
				m_Label_Ins_Data2[9].SetTextColor(COLOR_RED);
				m_Label_Ins_Data2[9].SetText(str1);
				//m_Display[0].DrawText(pDC,CPoint(400,260),str1,0,COLOR_BLACK,COLOR_RED);
				m_bResult[7] = TRUE;
				m_dData[25] = 0;
			}
			else{
				m_Label_Ins_Data2[9].SetTextColor(COLOR_GREEN);
				m_Label_Ins_Data2[9].SetText(str1);
				//m_Display[0].DrawText(pDC,CPoint(400,260),str1);
				m_bResult[7] = FALSE;
				m_dData[25] = 1;
			}
			//			}

			if(!m_pDlgSet->m_FormTab4->m_bTwoTab){	
				str1.Format("WeldCheck:  %d  ( %d )",m_nWhiteWeldCheck,m_pDlgSet->m_FormTab4->m_nWeldCheck);
				m_dData[8] = m_nWhiteWeldCheck;
				if(m_nWhiteWeldCheck <m_pDlgSet->m_FormTab4->m_nWeldCheck  || m_nTabTopEdge<0){
					m_Label_Ins_Data2[10].SetTextColor(COLOR_RED);
					m_Label_Ins_Data2[10].SetText(str1);
					//m_Display[0].DrawText(pDC,CPoint(400,290),str1,0,COLOR_BLACK,COLOR_RED);
					m_bResult[8] = TRUE;

				}
				else{
					m_Label_Ins_Data2[10].SetTextColor(COLOR_GREEN);
					m_Label_Ins_Data2[10].SetText(str1);
					//m_Display[0].DrawText(pDC,CPoint(400,290),str1);
					m_bResult[8] = FALSE;
				}
			}
			// Can Info
			if(TRUE)
			{
				int ncenterpos = CAMERA_WIDTH*0.5;
				// Can 흑점갯수랑 Top Cap 중심이 어느정도 벗어났는지
				//				str1.Format("Can %d [%d], Cap Center %d [%.0f]",m_nCanCheckAve,m_pDlgSet->m_FormTab4->m_nCheckCanValue,abs(ncenterpos-m_Project.m_ptCapCenter.x),m_pDlgSet->m_FormTab4->m_dCenterOffset);//m_nCanCheckAve,
				str1.Format("Can %d [%d]",m_nCanCheckAve,m_pDlgSet->m_FormTab4->m_nCheckCanValue);//m_nCanCheckAve,
				m_dData[9] = m_nCanCheckAve;
				//if(m_nNgCode == NG_NOT_CAN)
				if(m_nCanCheckAve > m_pDlgSet->m_FormTab4->m_nCheckCanValue)
				{
					
					bstats = TRUE;
					m_Label_Ins_Data2[11].SetTextColor(COLOR_RED);
					m_Label_Ins_Data2[11].SetText(str1);
					//m_Display[0].DrawText(pDC,CPoint(nx,20),str1,0,COLOR_BLACK,COLOR_RED);
					m_bResult[9] = TRUE;
					m_dData[21] = 0;
				}
				else{
					m_bResult[9] = FALSE;
					m_Label_Ins_Data2[11].SetTextColor(COLOR_GREEN);
					m_Label_Ins_Data2[11].SetText(str1);
					//m_Display[0].DrawText(pDC,CPoint(nx,20),str1);
					m_dData[21] = 1;
				}

				str1.Format("Cap Brightness %d [%d]",m_nTopCapCheck,m_pDlgSet->m_FormTab4->m_nTopCapValue);//m_nCanCheckAve,
				m_dData[17] = m_nTopCapCheck;
				if(m_nRBResult == NG_NOT_CAP)
				{
					bstats = TRUE;
					m_Label_Ins_Data2[12].SetTextColor(COLOR_RED);
					m_bResult[17] = TRUE;
					m_dData[22] = 0;

					m_nBadCount[0]++;
					m_strBadInfo1[1] = "NotCap"; //woo 저장이름 추가
				}
				else{
					m_Label_Ins_Data2[12].SetTextColor(COLOR_GREEN);
					//m_Display[0].DrawText(pDC,CPoint(nx+200,20),str1);
					m_bResult[17] = FALSE;
					m_dData[22] = 1;
				}
				m_Label_Ins_Data2[12].SetText(str1);

			}
			else
			{
				//	Can 유무 확인 안한다면 면적, cx,cy,w,h,ratio 표시
				//str1.Format("CX %.0f, CY %.0f, W %.0f, H %.0f",m_Project.m_dMaxChainData[1],m_Project.m_dMaxChainData[2],m_Project.m_dMaxChainData[3],m_Project.m_dMaxChainData[4]);
				str1.Format("CX %.0f, CY %.0f, W %.2f, H %.2f",m_Project.m_dMaxChainData[1],m_Project.m_dMaxChainData[2],m_Project.m_dMaxChainData[3]*m_pDlgSet->m_FormTab2->m_dCamScale,m_Project.m_dMaxChainData[4]*m_pDlgSet->m_FormTab2->m_dCamScale);
				m_Label_Ins_Data2[11].SetTextColor(COLOR_GREEN);
				m_Label_Ins_Data2[11].SetText(str1);
				//m_Display[0].DrawText(pDC,CPoint(nx,20),str1);
			}

			// Top Cap 중심기준 Tab의 좌우경계와 중심간 거리. 그리고 Tab Top과 중심간 거리
			//m_dTabDist[0] = ddist1;m_dTabDist[1]=ddist2;
			if(m_pDlgSet->m_FormTab4->m_bTapInspect)
			{
				str1.Format("Tab(mm) : L %.3f,",m_dTabDist[0]); // Top Cap 중심 ~ Tab 좌
				//m_dData[10] = m_dTabDist[0];
				m_dData[10] = m_Project.m_nEdgeDist[0];//220518 픽셀로 변환중
				if(m_dTabDist[0] >= m_pDlgSet->m_FormTab4->m_dTabTol){
					m_Label_Ins_Data2[13].SetTextColor(COLOR_RED);
					m_Label_Ins_Data2[13].SetText(str1);
					//m_Display[0].DrawText(pDC,CPoint(nx,100),str1,0,COLOR_BLACK,COLOR_RED);
					m_bResult[10] = TRUE;
					m_dData[28] = 0;

				}
				else{
					m_Label_Ins_Data2[13].SetTextColor(COLOR_GREEN);
					m_Label_Ins_Data2[13].SetText(str1);
					//m_Display[0].DrawText(pDC,CPoint(nx,100),str1);
					m_bResult[10] = FALSE;

				}

				str1.Format("R %.3f,",m_dTabDist[1]); // Top Cap 중심 ~ Tab 우
				//m_dData[11]=m_dTabDist[1];
				m_dData[11] = m_Project.m_nEdgeDist[1];//220518 픽셀로 변환중
				if(m_dTabDist[1] >= m_pDlgSet->m_FormTab4->m_dTabTol1){//right
					m_Label_Ins_Data2[14].SetTextColor(COLOR_RED);
					m_Label_Ins_Data2[14].SetText(str1);
					//m_Display[0].DrawText(pDC,CPoint(nx+220,100),str1,0,COLOR_BLACK,COLOR_RED);
					m_bResult[11] = TRUE;
					m_dData[28] = 0;

				}
				else{
					m_Label_Ins_Data2[14].SetTextColor(COLOR_GREEN);
					m_Label_Ins_Data2[14].SetText(str1);
					//m_Display[0].DrawText(pDC,CPoint(nx+220,100),str1);
					m_bResult[11] = FALSE;

				}
				// Top Cap 중심 ~ Tab 상
				//double dtabtop = (m_Project.m_ptCapCenter.y-m_Project.m_nTopPos[0])*m_pDlgSet->m_FormTab2->m_dCamScale;
				//str1.Format("T %.2f",dtabtop);

				double dValue = 0.0;;
				dValue = (m_Project.m_ptCapCenter.y-m_Project.m_nTopPos[0])*m_pDlgSet->m_FormTab2->m_dCamScale;
				m_dCapCanDist = dValue;
				str1.Format("T %.2f",dValue);

				//m_dData[12] = dtabtop;
				m_dData[12] = (m_Project.m_ptCapCenter.y-m_Project.m_nTopPos[0]);//220518 픽셀로 변환중
				if(((m_Project.m_ptCapCenter.y-m_Project.m_nTopPos[0])*m_pDlgSet->m_FormTab2->m_dCamScale) > m_pDlgSet->m_FormTab4->m_dTapTolY1 ||((m_Project.m_nTopPos[0]-m_Project.m_ptCapCenter.y)*m_pDlgSet->m_FormTab2->m_dCamScale) > m_pDlgSet->m_FormTab4->m_dTapTolY2)
				{
					m_Label_Ins_Data2[15].SetTextColor(COLOR_RED);
					m_Label_Ins_Data2[15].SetText(str1);
					//m_Display[0].DrawText(pDC,CPoint(nx+320,100),str1,0,COLOR_BLACK,COLOR_RED);
					m_bResult[12] = TRUE;
					m_dData[28] = 0;

				}
				else{
					m_Label_Ins_Data2[15].SetTextColor(COLOR_GREEN);
					m_Label_Ins_Data2[15].SetText(str1);
					//m_Display[0].DrawText(pDC,CPoint(nx+320,100),str1); 
					m_bResult[12] = FALSE;

				}

				//하단 탭 좌~ 우 woo
				if (m_pDlgSet->m_FormTab4->m_bBottomTab==TRUE)
				{
					str1.Format("BottomTab(mm) : L %.3f,",m_dTabBottomSize[0]); // Top Cap 중심 ~ BottomTab 좌
					if(m_dTabBottomSize[0] >= m_pDlgSet->m_FormTab4->m_dTabTol)
					{
						m_Label_Ins_Data2[16].SetTextColor(COLOR_RED);
						m_Label_Ins_Data2[16].SetText(str1);
						//m_Display[0].DrawText(pDC,CPoint(nx,60),str1,0,COLOR_BLACK,COLOR_RED);
						m_dData[28] = 0;

					}
					else
					{
						m_Label_Ins_Data2[16].SetTextColor(COLOR_RED);
						m_Label_Ins_Data2[16].SetText(str1);
						//m_Display[0].DrawText(pDC,CPoint(nx,60),str1);
						m_dData[28] = 0;

					}

					str1.Format("R %.3f,",m_dTabBottomSize[1]); // Top Cap 중심 ~ BottomTab 우
					if(m_dTabBottomSize[1] >= m_pDlgSet->m_FormTab4->m_dTabTol1)//right
					{
						m_Label_Ins_Data2[17].SetTextColor(COLOR_RED);
						m_Label_Ins_Data2[17].SetText(str1);
						//m_Display[0].DrawText(pDC,CPoint(nx+220,60),str1,0,COLOR_BLACK,COLOR_RED);
						m_dData[28] = 0;

					}
					else
					{
						m_Label_Ins_Data2[17].SetTextColor(COLOR_GREEN);
						m_Label_Ins_Data2[17].SetText(str1);
						//m_Display[0].DrawText(pDC,CPoint(nx+220,60),str1);
						m_dData[28] = 0;

					}
				}
			}

			int nYOffset = 40;
			//str1.Format("Area:%.0f  Ratio:%.1f",m_Project.m_dMaxChainData[0],m_Project.m_dMaxChainData[5]);
			//m_Display[0].DrawText(pDC,CPoint(nx,60),str1);

			// 용접 Info
			m_dData[27] = 1;//과용접 ok

			CString strfrmae[2];
			strfrmae[0].Format("%d",m_nMaxThFrame[0]);
			strfrmae[1].Format("%d",m_nMaxThFrame[1]);
			//			m_dData[20] = atoi(m_strResultValue[0]);
			if(m_nRBResult==RET_OK || bstats){
				m_Label_Ins_Data2[18].SetTextColor(COLOR_GREEN);
				m_Label_Ins_Data2[18].SetText(m_strResultValue[0]);
				m_Label_Ins_Data2[19].SetTextColor(COLOR_GREEN);
				m_Label_Ins_Data2[19].SetText(m_strResultValue[1]);
				m_Label_Ins_Data2[20].SetTextColor(COLOR_GREEN);
				m_Label_Ins_Data2[20].SetText(strfrmae[0]);
				m_Label_Ins_Data2[21].SetTextColor(COLOR_GREEN);
				m_Label_Ins_Data2[21].SetText(strfrmae[1]);
				//m_Display[0].DrawText(pDC,CPoint(nx,100+nYOffset),m_strResultValue[0]); // R1
				//m_Display[0].DrawText(pDC,CPoint(nx,140+nYOffset),m_strResultValue[1]); // S1
				//m_Display[0].DrawText(pDC,CPoint(nx+400,100+nYOffset),strfrmae[0]); // R1
				//m_Display[0].DrawText(pDC,CPoint(nx+400,140+nYOffset),strfrmae[1]); // S1

				// hoho
				if(m_pDlgSet->m_FormTab2->m_bLineWelding == TRUE || m_pDlgSet->m_FormTab2->m_b2LineWelding == TRUE){
					;
				}
				else{
					m_Display[0].DrawText(pDC,CPoint(nx+235,100+nYOffset),m_strResultValue[2]); // R2
					m_Display[0].DrawText(pDC,CPoint(nx+235,140+nYOffset),m_strResultValue[3]); // S2
				}
			}
			else
			{
				int off1 = 235;
				if(	m_bHoleBad[0][0] )//=m_bHoleBad[1][0]=m_bHoleBad[1][1] = TRUE;//좌상
				{
					m_bResult[13]=TRUE;
					m_Label_Ins_Data2[18].SetTextColor(COLOR_RED);
					m_Label_Ins_Data2[18].SetText(m_strResultValue[0]);
					//m_Display[0].DrawText(pDC,CPoint(nx,100+nYOffset),m_strResultValue[0],0,COLOR_BLACK,COLOR_RED);

				}
				else if(m_bHoleBad[0][0]==FALSE ){
					m_bResult[13]=FALSE;
					m_Label_Ins_Data2[18].SetTextColor(COLOR_GREEN);
					m_Label_Ins_Data2[18].SetText(m_strResultValue[0]);
					//m_Display[0].DrawText(pDC,CPoint(nx,100+nYOffset),m_strResultValue[0]);
				}

				if(m_bHoleBad[1][0] ){
					m_Label_Ins_Data2[19].SetTextColor(COLOR_RED);
					m_Label_Ins_Data2[19].SetText(m_strResultValue[1]);
					//m_Display[0].DrawText(pDC,CPoint(nx,140+nYOffset),m_strResultValue[1],0,COLOR_BLACK,COLOR_RED);
					m_bResult[14]=TRUE;
					m_dData[27] = 0;
				}
				else if(m_bHoleBad[1][0]==FALSE  ){
					m_Label_Ins_Data2[19].SetTextColor(COLOR_GREEN);
					m_Label_Ins_Data2[19].SetText(m_strResultValue[1]);
					//m_Display[0].DrawText(pDC,CPoint(nx,140+nYOffset),m_strResultValue[1]);
					m_bResult[14]=FALSE;
				}

				m_Label_Ins_Data2[20].SetTextColor(COLOR_GREEN);
				m_Label_Ins_Data2[20].SetText(strfrmae[0]);
				m_Label_Ins_Data2[21].SetTextColor(COLOR_GREEN);
				m_Label_Ins_Data2[21].SetText(strfrmae[1]);
				//m_Display[0].DrawText(pDC,CPoint(nx+400,100+nYOffset),strfrmae[0]); // R1
				//m_Display[0].DrawText(pDC,CPoint(nx+400,140+nYOffset),strfrmae[1]); // S1

				// hoho
				if(m_pDlgSet->m_FormTab2->m_bLineWelding == TRUE|| m_pDlgSet->m_FormTab2->m_b2LineWelding == TRUE){
					;
				}
				else{
					if(m_bHoleBad[0][1] == TRUE){
						m_Label_Ins_Data2[22].SetTextColor(COLOR_RED);
						m_Label_Ins_Data2[22].SetText(m_strResultValue[2]);
						//m_Display[0].DrawText(pDC,CPoint(nx+off1,100+nYOffset),m_strResultValue[2],0,COLOR_BLACK,COLOR_RED);
					}
					else{
						m_Label_Ins_Data2[22].SetTextColor(COLOR_GREEN);
						m_Label_Ins_Data2[22].SetText(m_strResultValue[2]);
						//m_Display[0].DrawText(pDC,CPoint(nx+off1,100+nYOffset),m_strResultValue[2]);
					}

					if(m_bHoleBad[1][1] == TRUE){
						m_Label_Ins_Data2[23].SetTextColor(COLOR_RED);
						m_Label_Ins_Data2[23].SetText(m_strResultValue[3]);
						m_dData[27] = 0;
						//m_Display[0].DrawText(pDC,CPoint(nx+off1,140+nYOffset),m_strResultValue[3],0,COLOR_BLACK,COLOR_RED);
					}
					else{
						m_Label_Ins_Data2[23].SetTextColor(COLOR_GREEN);
						m_Label_Ins_Data2[23].SetText(m_strResultValue[3]);
						//m_Display[0].DrawText(pDC,CPoint(nx+off1,140+nYOffset),m_strResultValue[3]);
					}
				}

			}


			// 			// 용접 후보 영역(빨간박스) 안에서 가장 큰 Blob 면적
			// 			str1.Format("Area : %.0f [%d]",m_Project.m_dMaxChainData[0],m_pDlgSet->m_FormTab4->m_nBlackArea);
			// 			
			// 			if(m_pDlgSet->m_FormTab4->m_bBlackArea  &&  m_pDlgSet->m_FormTab4->m_nBlackArea <m_Project.m_dMaxChainData[0])
			// 				m_Display[0].DrawText(pDC,CPoint(nx,180+nYOffset),str1,0,COLOR_BLACK,COLOR_RED);
			// 			else
			// 				m_Display[0].DrawText(pDC,CPoint(nx,180+nYOffset),str1);
			str1.Format("Center : %.d ",m_nTopCapCenterBrightness); //woo 센터 평균 밝기와 밝기보다 낮은 갯수 count display 
			m_Label_Ins_Data2[24].SetTextColor(COLOR_GREEN);
			m_Label_Ins_Data2[24].SetText(str1);
			
			if(m_pDlgSet->m_FormTab4->m_dTopCapCenterCount>0){
				CRect rtcenter;
				rtcenter.SetRect(m_Project.m_ptCapCenter.x-40,m_Project.m_ptCapCenter.y,m_Project.m_ptCapCenter.x+40,m_Project.m_ptCapCenter.y+75);//영역그리기
				m_Display[0].DrawRect(pDC,rtcenter,COLOR_GREEN,1,"");
			}			
			//			str1.Format("Left %.2f , Right %.2f",m_dTabDist[0],m_dTabDist[1]);
			//			m_Display[0].DrawText(pDC,CPoint(nx,260),str1);


			// 용접 좌 ~ Tab 좌, 용접 우 ~ Tab 우 간의 거리
			double dcheckmm = m_pDlgSet->m_FormTab4->m_dHoleEdgemm / m_pDlgSet->m_FormTab2->m_dCamScale;
			double dcheckmm1 = m_pDlgSet->m_FormTab4->m_dHoleEdgeINmm/ m_pDlgSet->m_FormTab2->m_dCamScale;
			double dl1,dr1,dw1,dw2;
			CString strWeldingWidth,strTabWidth;
			dl1 = (m_Project.m_nTopEdge[0] - m_rtWeldBlob.left)*m_pDlgSet->m_FormTab2->m_dCamScale;
			dr1 = (m_rtWeldBlob.right-(m_Project.m_nTopEdge[1]))*m_pDlgSet->m_FormTab2->m_dCamScale;
			dw1 = (m_rtWeldBlob.right-m_rtWeldBlob.left)*m_pDlgSet->m_FormTab2->m_dCamScale; //용접 길이
			dw2 = (m_Project.m_nTopEdge[1]-m_Project.m_nTopEdge[0])*m_pDlgSet->m_FormTab2->m_dCamScale; //Tab 폭
			str1.Format("Welding(mm) : Left %.2f, Right %.2f",dl1,dr1);
			strWeldingWidth.Format("Welding Width : %.2f mm",dw1);
			// 			strTabWidth.Format("Tab Width : %.2f mm",dw2);
			strTabWidth.Format("Tab Width : %.2f mm",m_Project.m_dTabTopLen);
			// 			AddtoInspLog(str1);
			if(m_pDlgSet->m_FormTab4->m_bHoleEdgeCheck)
			{
				// hoho 판정에 맞게 수정
				str1.Format("Welding(mm) : Left %.2f,",dl1);
				//m_dData[15] = dl1;  m_ptCapCenter.x //tc 중심
				m_dData[19] = m_Project.m_ptCapCenter.x-((m_rtWeldBlob.right+m_rtWeldBlob.left)/2);
				m_dData[20] = m_rtWeldBlob.right-m_rtWeldBlob.left;
				m_dData[15] = (m_Project.m_nTopEdge[0] - m_rtWeldBlob.left);
				if(m_rtWeldBlob.left< m_Project.m_nTopEdge[0]-dcheckmm || m_rtWeldBlob.left> m_Project.m_nTopEdge[0]+dcheckmm1){
					m_Label_Ins_Data2[25].SetTextColor(COLOR_RED);
					m_Label_Ins_Data2[25].SetText(str1);
					//m_Display[0].DrawText(pDC,CPoint(nx,220+nYOffset),str1,0,COLOR_BLACK,COLOR_RED);
					m_bResult[15]=TRUE;
					m_dData[26] = 0;

				}
				else if(m_rtWeldBlob.left >= m_Project.m_nTopEdge[0]-dcheckmm){
					m_Label_Ins_Data2[25].SetTextColor(COLOR_GREEN);
					m_Label_Ins_Data2[25].SetText(str1);
					//m_Display[0].DrawText(pDC,CPoint(nx,220+nYOffset),str1);
					m_bResult[15]=FALSE;
				}
				str1.Format("Right %.2f",dr1);
				//m_dData[16] = dr1;
				m_dData[16] = (m_rtWeldBlob.right-(m_Project.m_nTopEdge[1]));
				if(m_rtWeldBlob.right> m_Project.m_nTopEdge[1]+dcheckmm || m_rtWeldBlob.right< m_Project.m_nTopEdge[1]-dcheckmm1){
					m_Label_Ins_Data2[26].SetTextColor(COLOR_RED);
					m_Label_Ins_Data2[26].SetText(str1);
					//m_Display[0].DrawText(pDC,CPoint(nx+260,220+nYOffset),str1,0,COLOR_BLACK,COLOR_RED);
					m_bResult[16]=TRUE;
					m_dData[26] = 0;

				}
				else if(m_rtWeldBlob.right<= m_Project.m_nTopEdge[1]+dcheckmm){
					m_Label_Ins_Data2[26].SetTextColor(COLOR_GREEN);
					m_Label_Ins_Data2[26].SetText(str1);
					//m_Display[0].DrawText(pDC,CPoint(nx+260,220+nYOffset),str1);
					m_bResult[16]=FALSE;
				}

				m_Label_Ins_Data2[28].SetTextColor(COLOR_GREEN);
				m_Label_Ins_Data2[28].SetText(strWeldingWidth);
				m_Label_Ins_Data2[29].SetTextColor(COLOR_GREEN);
				m_Label_Ins_Data2[29].SetText(strTabWidth);
				/*m_Display[0].DrawText(pDC,CPoint(nx+260,260+nYOffset),strWeldingWidth);
				m_Display[0].DrawText(pDC,CPoint(nx+260,300+nYOffset),strTabWidth);*/

				//str1.Format("Welding(mm) : Left %.2f,",dl1);
				//if(m_Project.m_rtWeldRect[0].left< m_Project.m_nTopEdge[0]-dcheckmm )//좌/우로 넓힘.
				//	m_Display[0].DrawText(pDC,CPoint(nx,220+nYOffset),str1,0,COLOR_BLACK,COLOR_RED);
				//else if(m_Project.m_rtWeldRect[0].left> m_Project.m_nTopEdge[0]-dcheckmm)
				//	m_Display[0].DrawText(pDC,CPoint(nx,220+nYOffset),str1);
				//str1.Format("Right %.2f",dr1);
				//if(m_Project.m_rtWeldRect[1].right> m_Project.m_nTopEdge[1]+dcheckmm)//좌/우로 넓힘.
				//	m_Display[0].DrawText(pDC,CPoint(nx+305,220+nYOffset),str1,0,COLOR_BLACK,COLOR_RED);
				//else if(m_Project.m_rtWeldRect[1].right< m_Project.m_nTopEdge[1]+dcheckmm)
				//	m_Display[0].DrawText(pDC,CPoint(nx+305,220+nYOffset),str1);
			}
			else
				m_Display[0].DrawText(pDC,CPoint(nx,220+nYOffset),str1);

			if(m_pDlgSet->m_FormTab4->m_bCheckDamage){
				str1.Format(_T("Tab Damage %d (W %d)"), m_Project.m_nBlackValue, m_Project.m_nWhiteValue);
				if(m_Project.m_nBlackValue < m_pDlgSet->m_FormTab4->m_nCheckDamage)
				{
					m_Label_Ins_Data2[27].SetTextColor(COLOR_RED);
					m_Label_Ins_Data2[27].SetText(str1);
					//m_Display[0].DrawText(pDC,CPoint(nx,260+nYOffset),str1,0,COLOR_BLACK,COLOR_RED);
					m_dData[28] = 0;

				}
				else
				{
					m_Label_Ins_Data2[27].SetTextColor(COLOR_GREEN);
					m_Label_Ins_Data2[27].SetText(str1);
					//m_Display[0].DrawText(pDC,CPoint(nx,260+nYOffset),str1);
				}
			}

			if(m_pDlgSet->m_FormTab4->m_bMidWeld){
				str1.Format("Mid Weld :  %.0f",m_dMidWeldV);
				if(m_dMidWeldV>0 &&m_dMidWeldV < m_pDlgSet->m_FormTab4->m_dMidWeld)
					m_Display[0].DrawText(pDC,CPoint(nx,260+nYOffset),str1,0,COLOR_BLACK,COLOR_RED);
				else 
					m_Display[0].DrawText(pDC,CPoint(nx,260+nYOffset),str1);
			}

			int nWeldingBoxGap = 15;
			if(m_pDlgSet->m_FormTab4->m_bWeldingRange){
				str1.Format("Dist %.1f", ((m_Project.m_rtWeldRect[0].top+nWeldingBoxGap) - m_Project.m_ptCapCenter.y)*m_pDlgSet->m_FormTab2->m_dCamScale);
				if(m_Project.m_rtWeldRect[0].top+nWeldingBoxGap < m_Project.m_rtWeldingOkRange.top || m_Project.m_rtWeldRect[0].top+nWeldingBoxGap > m_Project.m_rtWeldingOkRange.bottom )
					m_Display[0].DrawText(pDC,CPoint(nx+180,260+nYOffset),str1,0,COLOR_BLACK,COLOR_RED);
				else
					m_Display[0].DrawText(pDC,CPoint(nx+180,260+nYOffset),str1);
			}

			// 라인 용접일때, 용접 굵기 표시
			// hoho, 일단 빼버림
			if(FALSE && m_pDlgSet->m_FormTab2->m_bLineWelding == TRUE && m_pDlgSet->m_FormTab2->m_b2LineWelding == FALSE)
			{
				// 높이 최소값을 가지는 위치X와 높이 최소값
				str1.Format("MinX %d - Height %d[%.0f]",m_Project.m_nLinMinMax[2],m_Project.m_nLinMinMax[0],m_pDlgSet->m_FormTab4->m_dMinHeight);
				if(m_pDlgSet->m_FormTab4->m_dMinHeight> m_Project.m_nLinMinMax[0])
					m_Display[0].DrawText(pDC,CPoint(nx,260+nYOffset),str1,0,COLOR_BLACK,COLOR_RED);
				else
					m_Display[0].DrawText(pDC,CPoint(nx,260+nYOffset),str1);

				// 높이 최대값을 가지는 위치X와 높이 최대값
				str1.Format("MaxX %d - Height %d[%.0f]",m_Project.m_nLinMinMax[3],m_Project.m_nLinMinMax[1],m_pDlgSet->m_FormTab4->m_dMaxHeight);
				if( m_pDlgSet->m_FormTab4->m_dMaxHeight<m_Project.m_nLinMinMax[1])
					m_Display[0].DrawText(pDC,CPoint(nx,300+nYOffset),str1,0,COLOR_BLACK,COLOR_RED);
				else
					m_Display[0].DrawText(pDC,CPoint(nx,300+nYOffset),str1);

				// 높이 최소값을 가지는 위치는 노란색으로 찍어주고,
				CPoint ptline1,ptline2;
				ptline1.SetPoint(m_Project.m_rtHoleRect.left+m_Project.m_nLinMinMax[2],m_Project.m_rtHoleRect.top-5);
				ptline2.SetPoint(m_Project.m_rtHoleRect.left+m_Project.m_nLinMinMax[2],m_Project.m_rtHoleRect.top+15);
				m_Display[0].DrawLine(pDC,ptline1,ptline2,"",COLOR_YELLOW,2);
				// 높이 최대값을 가지는 위치는 파란색으로 찍어준다
				ptline1.SetPoint(m_Project.m_rtHoleRect.left+m_Project.m_nLinMinMax[3],m_Project.m_rtHoleRect.top-5);
				ptline2.SetPoint(m_Project.m_rtHoleRect.left+m_Project.m_nLinMinMax[3],m_Project.m_rtHoleRect.top+15);
				m_Display[0].DrawLine(pDC,ptline1,ptline2,"",COLOR_BLUE,2);
			}
		}
		if(m_pDlgSet->m_FormTab4->m_bWeldingRange)
		{
			// Welding OK 영역 그리기
			CString str;
			str.Format("%.1lfmm", m_pDlgSet->m_FormTab4->m_dWeldingDistanceY - m_pDlgSet->m_FormTab4->m_dWeldingDistanceRange[0]);
			m_Display[0].DrawLine(pDC,CPoint(300,m_Project.m_rtWeldingOkRange.top),CPoint(1000,m_Project.m_rtWeldingOkRange.top),str,COLOR_WHITE,1);
			str.Format("%.1lfmm", m_pDlgSet->m_FormTab4->m_dWeldingDistanceY + m_pDlgSet->m_FormTab4->m_dWeldingDistanceRange[1]);
			m_Display[0].DrawLine(pDC,CPoint(300,m_Project.m_rtWeldingOkRange.bottom),CPoint(1000,m_Project.m_rtWeldingOkRange.bottom),str,COLOR_WHITE,1);
			//m_Display[0].DrawRect(pDC,m_Project.m_rtWeldingOkRange,COLOR_JOO,1,"OK Range");
		}
		if(m_pDlgSet->m_FormTab2->m_bShowHole && m_pDlgSet->m_FormTab2->m_bWeldInTab){
			ngap = 10;
			m_Display[0].DrawCross(pDC,CRect(m_ptWeldCenterPos.x-ngap,m_ptWeldCenterPos.y-ngap,m_ptWeldCenterPos.x+ngap,m_ptWeldCenterPos.y+ngap),COLOR_BLUE,3,"");

			m_Display[0].DrawRect(pDC,m_rtWeldBlob,COLOR_RED,2,"");

		}
	}
	else
	{
		m_Display[0].DrawImage(pDC);
		for(int i=0;i<5;i++)
			m_DisplayBad[i].DrawImage(pDC);
	}

	// hoho, 캘리브레이션 할때 캡 좌우 경계 찍어서 보여주기
	if(m_pDlgSet->m_FormTab5->m_bCalibration)
	{
		CPoint ptcal,ptcal1;
		ptcal.SetPoint(m_Project.m_nTopCapleft, m_Project.m_ptCapCenter1.y - 20);
		ptcal1.SetPoint(m_Project.m_nTopCapleft, m_Project.m_ptCapCenter1.y + 20);
		m_Display[0].DrawLine(pDC,ptcal,ptcal1,"",COLOR_BLACK,3);

		ptcal.SetPoint(m_Project.m_nTopCapRight, m_Project.m_ptCapCenter1.y - 20);
		ptcal1.SetPoint(m_Project.m_nTopCapRight, m_Project.m_ptCapCenter1.y + 20);
		m_Display[0].DrawLine(pDC,ptcal,ptcal1,"",COLOR_BLACK,3);


		ptcal.SetPoint(m_Project.m_nTopCapleft1, m_Project.m_ptCapCenter1.y - 20);
		ptcal1.SetPoint(m_Project.m_nTopCapleft1, m_Project.m_ptCapCenter1.y + 20);
		m_Display[0].DrawLine(pDC,ptcal,ptcal1,"",COLOR_GREEN,3);

		ptcal.SetPoint(m_Project.m_nTopCapRight1, m_Project.m_ptCapCenter1.y - 20);
		ptcal1.SetPoint(m_Project.m_nTopCapRight1, m_Project.m_ptCapCenter1.y + 20);
		m_Display[0].DrawLine(pDC,ptcal,ptcal1,"",COLOR_GREEN,3);

		CString strTopCapWidth;
		strTopCapWidth.Format("TopCap Width : %.3f",m_Project.m_dCapWidth);
		m_Display[0].DrawText(pDC,CPoint(800,300),strTopCapWidth);
	}
	
	ReleaseDC(pDC);
}

void CFlyCapture2_EXDlg::DrawDisplay2(int nCam)
{
	m_dData[26] = 1;
	m_dData[28] = 1;
	CRect rtCheck;
	CString strData ="";
	double dValue = 0;
	int nValue = 0;
	CFormTab5* pFormTab5 = m_pDlgSet->m_FormTab5;

	if(m_bStarting==FALSE)
		return;
	CDC* pDC=GetDC();
	if(m_nScreen==0)
	{
		m_Display[nCam].DrawImage(pDC);

		// NG Thumbnail
		for(int i=0;i<5;i++)
			m_DisplayBad[i].DrawImage(pDC);
		// Top Cap Edge 그리기
		if(m_pDlgSet->m_FormTab2->m_bShowEdge){
			for(int i=0;i<CAMERA_HEIGHT;i++){
				if(m_Project.m_ptDrawCapEdge[0][i].x>0)
					m_Display[0].DrawDotMark(pDC,m_Project.m_ptDrawCapEdge[0][i],COLOR_GREEN,1);
				if(m_Project.m_ptDrawCapEdge[1][i].x>0)
					m_Display[0].DrawDotMark(pDC,m_Project.m_ptDrawCapEdge[1][i],COLOR_GREEN,1);
			}
		}
	
		int ngap=20;
		// Top Cap 중심 +마크 그리기
		m_Display[0].DrawCross(pDC,CRect(m_Project.m_ptCapCenter.x-ngap,m_Project.m_ptCapCenter.y-ngap,m_Project.m_ptCapCenter.x+ngap,m_Project.m_ptCapCenter.y+ngap),COLOR_RED,2,"");
		m_Display[0].DrawRect(pDC,CRect(m_Project.m_ptCapCenter.x-ngap,m_Project.m_ptCapCenter.y-ngap,m_Project.m_ptCapCenter.x+ngap,m_Project.m_ptCapCenter.y+ngap),COLOR_RED,2,"");
		m_Display[0].DrawCross(pDC,CRect(m_Project.m_ptLeftTop.x-ngap,m_Project.m_ptLeftTop.y-ngap,m_Project.m_ptLeftTop.x+ngap,m_Project.m_ptLeftTop.y+ngap),COLOR_RED,2,"");
		m_Display[0].DrawCross(pDC,CRect(m_Project.m_ptRightTop.x-ngap,m_Project.m_ptRightTop.y-ngap,m_Project.m_ptRightTop.x+ngap,m_Project.m_ptRightTop.y+ngap),COLOR_RED,2,"");

		CPoint ptcantop;
		ptcantop.SetPoint((m_Project.m_ptCanTop[0].x+m_Project.m_ptCanTop[1].x)*0.5,(m_Project.m_ptCanTop[0].y+m_Project.m_ptCanTop[1].y)*0.5);
		m_ptCanTop.x=ptcantop.x;
		m_ptCanTop.y=ptcantop.y;
		m_Display[0].DrawCross(pDC,CRect(m_Project.m_ptCanTop[0].x-ngap,m_Project.m_ptCanTop[0].y-ngap,m_Project.m_ptCanTop[0].x+ngap,m_Project.m_ptCanTop[0].y+ngap),COLOR_RED,2,"");
		m_Display[0].DrawCross(pDC,CRect(m_Project.m_ptCanTop[1].x-ngap,m_Project.m_ptCanTop[1].y-ngap,m_Project.m_ptCanTop[1].x+ngap,m_Project.m_ptCanTop[1].y+ngap),COLOR_RED,2,"");
		m_Display[0].DrawCross(pDC,CRect(ptcantop.x-ngap,ptcantop.y-ngap,ptcantop.x+ngap,ptcantop.y+ngap),COLOR_RED,2,"");




		m_Display[0].DrawLine(pDC,CPoint(0,ptcantop.y),CPoint(CAMERA_WIDTH,ptcantop.y),"",COLOR_GRAY,2);

		int nCanX = (m_Project.m_ptCanTop[1].x + m_Project.m_ptCanTop[0].x)*0.5;
		CRect rtDisplay;
		rtDisplay.SetRect(nCanX - m_pDlgSet->m_FormTab4->m_nBottomTabLeft, m_nTabBottomTop - m_pDlgSet->m_FormTab4->m_nBottomTabTop, nCanX + m_pDlgSet->m_FormTab4->m_nBottomTabRight, m_nTabBottomTop - m_pDlgSet->m_FormTab4->m_nBottomTabBot);
		rtDisplay = m_Project.CheckROI(rtDisplay, CRect(0, 0, CAMERA_WIDTH, CAMERA_HEIGHT));
		if (nCanX != 0 && m_pDlgSet->m_FormTab4->m_bBottomTab) {
			m_Display[0].DrawRect(pDC, rtDisplay, COLOR_GREEN, 1, "");
			m_Display[0].DrawCross(pDC, CRect(m_nTabBottomLeft - 10, m_nTabBottomTop - 10, m_nTabBottomLeft + 10, m_nTabBottomTop + 10), COLOR_YELLOW, 2, "");
			m_Display[0].DrawCross(pDC, CRect(m_nTabBottomRight - 10, m_nTabBottomTop - 10, m_nTabBottomRight + 10, m_nTabBottomTop + 10), COLOR_SKYBLUE, 2, "");
		}

		// Circle Fitting용 Cap Edge랑 Tab 찾는 원 그리기
		if(m_pDlgSet->m_FormTab2->m_bCircleShow){
			int noffset = m_pDlgSet->m_FormTab2->m_nInnerCiircle;
			m_Display[0].DrawCircle(pDC,m_Project.m_ptCapCenter,noffset,COLOR_GREEN,1,"");
			m_Display[0].DrawCircle(pDC,m_Project.m_ptCapCenter,m_Project.m_dCabRadius,COLOR_GREEN,1,"");
		}

		if(m_pDlgSet->m_FormTab2->m_bShowTabArea)
		{
			//Tab Edge V 영역그리기
			rtCheck.SetRect(m_Project.m_ptCapCenter.x-m_pDlgSet->m_FormTab4->m_nTabEdgeX1,m_Project.m_ptCapCenter.y-m_pDlgSet->m_FormTab4->m_nTabEdgeY1,m_Project.m_ptCapCenter.x+m_pDlgSet->m_FormTab4->m_nTabEdgeX2,m_Project.m_ptCapCenter.y+m_pDlgSet->m_FormTab4->m_nTabEdgeY2);
			rtCheck = m_Project.CheckROI(rtCheck , CRect(0,0,CAMERA_WIDTH,CAMERA_HEIGHT));
			m_Display[0].DrawRect(pDC,rtCheck,COLOR_YELLOW,2,"");

			// Tab OK 영역 그리기
			m_Display[0].DrawRect(pDC,m_Project.m_rtTabOkRange,COLOR_BLUE,2,"OK Range");

			//Tab Exist
			rtCheck.SetRect(m_Project.m_ptCapCenter.x-m_pDlgSet->m_FormTab4->m_nTabExistX1,m_Project.m_ptCapCenter.y-m_pDlgSet->m_FormTab4->m_nTabExistY1,m_Project.m_ptCapCenter.x+m_pDlgSet->m_FormTab4->m_nTabExistX2,m_Project.m_ptCapCenter.y+m_pDlgSet->m_FormTab4->m_nTabExistY2);
			m_Display[0].DrawRect(pDC,rtCheck,COLOR_GREEN,2,"");

			CPoint pt1,pt2,pt3,pt4;

			// 각 용접 중심 찍기(흰점)
			for(int i=0;i<m_Project.m_nHoleCandiCount;i++)
				m_Display[0].DrawPoint(pDC,m_Project.m_ptHole1[i],COLOR_WHITE,2);			

			for(int i=0;i<2;i++){
				if(i==1 && m_pDlgSet->m_FormTab2->m_b2LineWelding==FALSE && m_pDlgSet->m_FormTab2->m_bLineWelding==TRUE)
					continue;

				m_Display[0].DrawRect(pDC,m_Project.m_rtWeldRect[i],COLOR_GREEN,1,"");
			}
			m_Display[0].DrawRect(pDC,m_Project.m_rtWeldPos,COLOR_GREEN,1,"");

			// Can Info
			strData.Format("%d [%d]",m_nCanCheckAve,m_pDlgSet->m_FormTab4->m_nCheckCanValue);
			m_Display[0].DrawRect(pDC, m_Display[0].m_rtImgTracker[0], COLOR_YELLOW, 1, strData);
			//m_Display[0].DrawRect(pDC, m_Project.m_rtCanCheckROI, COLOR_YELLOW, 1, strData);

			int nx = 800;
			BOOL bstats = FALSE;

			strData.Format("Select Frame : %d",m_nSelectFrame);//
			m_Label_Ins_Data2[0].SetText(strData);

			m_dCapCanDistX = m_dData[1] = (m_Project.m_ptCapCenter.x- (ptcantop.x)) * m_pDlgSet->m_FormTab2->m_dCamScale;
			strData.Format("Cap-Can X : %.2f(%.2f)",m_dCapCanDistX ,m_pDlgSet->m_FormTab4->m_dCapCandistX);//m_nCanCheckAve, //pView->m_nMorphCount[0]
			if (m_nRBResult == NG_CAN_CAP_DIST_X)
			{
				m_Label_Ins_Data2[1].SetTextColor(COLOR_RED);
				m_bResult[1] = TRUE;
				m_dData[28] = 0;
			}
			else{
				m_Label_Ins_Data2[1].SetTextColor(COLOR_GREEN);
				m_bResult[1] = FALSE;
			}
			m_Label_Ins_Data2[1].SetText(strData);

			m_dData[2] = (ptcantop.y-m_Project.m_ptCapCenter.y);//220518 픽셀로 변환 중
			if (m_pDlgSet->m_FormTab4->m_bUseCapCanY)
			{
 				strData.Format("Cap-Can Y : %.2f(%.2f~%.2f)",m_dCapCanDistY,m_pDlgSet->m_FormTab4->m_dCenterOffset2,m_pDlgSet->m_FormTab4->m_dCenterOffset);
				if(m_nRBResult == NG_CAN_CAP_DIST_Y)
				{
					m_Label_Ins_Data2[2].SetTextColor(COLOR_RED);
					m_bResult[2] = TRUE;
					m_dData[28] = 0;
				}
				else{
					m_Label_Ins_Data2[2].SetTextColor(COLOR_GREEN);
					m_bResult[2] = FALSE;
				}
				m_Label_Ins_Data2[2].SetText(strData);

			}

			strData.Format("Can %d [%d]",m_nCanCheckAve,m_pDlgSet->m_FormTab4->m_nCheckCanValue);//m_nCanCheckAve,
			m_dData[9] = m_nCanCheckAve;
			if(m_nRBResult == NG_NOT_CAN)
				//if(m_nCanCheckAve > m_pDlgSet->m_FormTab4->m_nCheckCanValue)
			{
				bstats = TRUE;
				m_Label_Ins_Data2[11].SetTextColor(COLOR_RED);
				m_bResult[9] = TRUE;
				m_dData[21] = 0;
			}
			else{
				m_bResult[9] = FALSE;
				m_Label_Ins_Data2[11].SetTextColor(COLOR_GREEN);
				m_dData[21] = 1;
			}
			m_Label_Ins_Data2[11].SetText(strData);


			strData.Format("Cap Brightness %d [%d]",m_nTopCapCheck,m_pDlgSet->m_FormTab4->m_nTopCapValue);//m_nCanCheckAve,
			m_dData[17] = m_nTopCapCheck;
			if(m_nRBResult == NG_NOT_CAP)
			{
				bstats = TRUE;
				m_Label_Ins_Data2[12].SetTextColor(COLOR_RED);
				m_bResult[17] = TRUE;
				m_dData[22] = 0;

				m_strBadInfo1[1] = "NotCap"; //woo 저장이름 추가
			}
			else{
				m_Label_Ins_Data2[12].SetTextColor(COLOR_GREEN);
				m_bResult[17] = FALSE;
				m_dData[22] = 1;
			}
			m_Label_Ins_Data2[12].SetText(strData);

			if(m_pDlgSet->m_FormTab2->m_bTabReFind ){
				strData.Format("Tab Corner L %d  R %d  Gap %d",m_Project.m_ptLeftTop.y,m_Project.m_ptRightTop.y,m_Project.m_ptLeftTop.y-m_Project.m_ptRightTop.y);
				if(m_nRBResult == NG_TAP_L_R_GAP)
				{
					m_Label_Ins_Data2[3].SetTextColor(COLOR_RED);
					m_dData[28] = 0;
				}
				else
				{
					m_Label_Ins_Data2[3].SetTextColor(COLOR_GREEN);
				}
				m_Label_Ins_Data2[3].SetText(strData);

			}
			if(m_pDlgSet->m_FormTab2->m_bShowHole && m_pDlgSet->m_FormTab2->m_bWeldInTab && m_pDlgSet->m_FormTab4->m_bWeldDist ){
				ngap = 10;
				double ddist1 = ( ptcantop.y-m_ptWeldCenterPos.y)*m_pDlgSet->m_FormTab2->m_dCamScale;
				m_dData[3] = ptcantop.y-m_ptWeldCenterPos.y;//220518 픽셀로 변환 중
				//				m_Display[0].DrawCross(pDC,CRect(m_ptWeldCenterPos.x-ngap,m_ptWeldCenterPos.y-ngap,m_ptWeldCenterPos.x+ngap,m_ptWeldCenterPos.y+ngap),COLOR_BLUE,3,"");
				strData.Format("Weld-Can Dist %.1fmm (%.1f~%.1f)",ddist1,m_pDlgSet->m_FormTab4->m_dDistWeldMin,m_pDlgSet->m_FormTab4->m_dDistWeldMax);
				//if ((ddist1 > m_pDlgSet->m_FormTab4->m_dDistWeldMax || ddist1 < m_pDlgSet->m_FormTab4->m_dDistWeldMin) && ddist1 > 0) {
				if (m_nRBResult == NG_WELD_CAN_DIST) {
					m_Label_Ins_Data2[4].SetTextColor(COLOR_RED);
					m_bResult[3] = TRUE;
					m_dData[26] = 0;
				}
				else{		
					m_Label_Ins_Data2[4].SetTextColor(COLOR_GREEN);
					m_bResult[3] = FALSE;
				}
				m_Label_Ins_Data2[4].SetText(strData);

			}
			double ddist2 = (m_ptWeldCenterPos.y-m_Project.m_ptCapCenter.y)*m_pDlgSet->m_FormTab2->m_dCamScale;
			//m_dData[4] = ddist2; 
			m_dData[4] = (m_ptWeldCenterPos.y-m_Project.m_ptCapCenter.y);//220518 픽셀로 변환 중
			//m_Display[0].DrawCross(pDC,CRect(m_ptWeldCenterPos.x-ngap,m_ptWeldCenterPos.y-ngap,m_ptWeldCenterPos.x+ngap,m_ptWeldCenterPos.y+ngap),COLOR_BLUE,3,"");
			if(m_pDlgSet->m_FormTab4->m_bWeldDistCap)
			{
				strData.Format("Weld-TopCap Dist%.1fmm  (%.1f~%.1f)",ddist2,m_pDlgSet->m_FormTab4->m_dWeldDistCapMin,m_pDlgSet->m_FormTab4->m_dWeldDistCapMax);
				if(m_nRBResult == NG_WELD_CAP_DIST){
					m_Label_Ins_Data2[5].SetTextColor(COLOR_RED);
					m_bResult[4] = TRUE;
					m_dData[26] = 0;
				}
				else{		
					m_Label_Ins_Data2[5].SetTextColor(COLOR_GREEN);
					m_bResult[4] = FALSE;
				}
				m_Label_Ins_Data2[5].SetText(strData);
			}
			if (m_pDlgSet->m_FormTab4->m_bCanDist)//Can 높이 woo 
			{
				strData.Format("Can Dist. %.1fmm  (%.1f~%.1f)",m_dCanDistY,m_pDlgSet->m_FormTab4->m_dCanDistMin,m_pDlgSet->m_FormTab4->m_dCanDistMax);
				if(m_nRBResult == NG_CAN_DIST)
				{
					m_Label_Ins_Data2[6].SetTextColor(COLOR_RED);
					m_dData[28] = 0;
				}
				else
				{
					m_Label_Ins_Data2[6].SetTextColor(COLOR_GREEN);
				}
				m_Label_Ins_Data2[6].SetText(strData);
			}

			m_dData[5] = m_nTopCapV;
			strData.Format("Cap Edge V:  %d  (%d)",m_nTopCapV,m_pDlgSet->m_FormTab4->m_nCapReverseV);
			if(m_nRBResult == NG_CAP_REVERSE){
				m_Label_Ins_Data2[7].SetTextColor(COLOR_RED);
				m_bResult[5] = TRUE;
				m_dData[23] = 0;
			}
			else{
				m_Label_Ins_Data2[7].SetTextColor(COLOR_GREEN);
				m_bResult[5] = FALSE;
				m_dData[23] = 1;
			}
			m_Label_Ins_Data2[7].SetText(strData);


			strData.Format("Weak V:  %d  (%d ~ %d)",m_nWeakWeld,m_pDlgSet->m_FormTab4->m_nWeakWeld,m_pDlgSet->m_FormTab4->m_nWeakWeld2);
			m_dData[6] = m_nWeakWeld;
			if(m_nRBResult == NG_WEAK_WELD){
				m_Label_Ins_Data2[8].SetTextColor(COLOR_RED);
				m_bResult[6] = TRUE;
				m_dData[24] = 0;
			}
			else{
				m_Label_Ins_Data2[8].SetTextColor(COLOR_GREEN);
				m_bResult[6] = FALSE;
				m_dData[24] = 1;
			}
			m_Label_Ins_Data2[8].SetText(strData);


			//			if(!m_pDlgSet->m_FormTab4->m_bTwoTab){ //woo
			strData.Format("Tab Edge V:  %d  ( %d )",m_nTabTopEdge,m_pDlgSet->m_FormTab4->m_nCenterEdgeValue);
			m_dData[7] = m_nTabTopEdge;
			if(m_nRBResult == NG_TAP_FOLD){
				m_Label_Ins_Data2[9].SetTextColor(COLOR_RED);
				//m_Display[0].DrawText(pDC,CPoint(400,260),str1,0,COLOR_BLACK,COLOR_RED);
				m_bResult[7] = TRUE;
				m_dData[25] = 0;
			}
			else{
				m_Label_Ins_Data2[9].SetTextColor(COLOR_GREEN);
				//m_Display[0].DrawText(pDC,CPoint(400,260),str1);
				m_bResult[7] = FALSE;
				m_dData[25] = 1;
			}
			m_Label_Ins_Data2[9].SetText(strData);

			if(m_pDlgSet->m_FormTab4->m_bTabExistChk){
				strData.Format("Tab Exist Cnt :  %d  ( %d )",m_nTabEdgeCnt,m_pDlgSet->m_FormTab4->m_nTabExist);
				if(m_nRBResult == NG_NOT_TAB)
					m_Label_Ins_Data2[30].SetTextColor(COLOR_RED);
				else
					m_Label_Ins_Data2[30].SetTextColor(COLOR_GREEN);

				m_Label_Ins_Data2[30].SetText(strData);
			}

			/*if(!m_pDlgSet->m_FormTab4->m_bTwoTab){	
			strData.Format("WeldCheck:  %d  ( %d )",m_nWhiteWeldCheck,m_pDlgSet->m_FormTab4->m_nWeldCheck);
				m_dData[8] = m_nWhiteWeldCheck;
				if(m_nWhiteWeldCheck <m_pDlgSet->m_FormTab4->m_nWeldCheck  || m_nTabTopEdge<0){
					m_Label_Ins_Data2[10].SetTextColor(COLOR_RED);
					m_bResult[8] = TRUE;

				}
				else{
					m_Label_Ins_Data2[10].SetTextColor(COLOR_GREEN);
					m_bResult[8] = FALSE;
				}
				m_Label_Ins_Data2[10].SetText(strData);

			}*/
			// Can Info
			int ncenterpos = CAMERA_WIDTH*0.5;
			// Can 흑점갯수랑 Top Cap 중심이 어느정도 벗어났는지
			//				str1.Format("Can %d [%d], Cap Center %d [%.0f]",m_nCanCheckAve,m_pDlgSet->m_FormTab4->m_nCheckCanValue,abs(ncenterpos-m_Project.m_ptCapCenter.x),m_pDlgSet->m_FormTab4->m_dCenterOffset);//m_nCanCheckAve,


			// Top Cap 중심기준 Tab의 좌우경계와 중심간 거리. 그리고 Tab Top과 중심간 거리
			//m_dTabDist[0] = ddist1;m_dTabDist[1]=ddist2;
			if(m_pDlgSet->m_FormTab4->m_bTapInspect)
			{
				strData.Format("Tab(mm) : L %.3f,",m_dTabDist[0]); // Top Cap 중심 ~ Tab 좌
				//m_dData[10] = m_dTabDist[0];
				m_dData[10] = m_Project.m_nEdgeDist[0];//220518 픽셀로 변환중
				if(m_nRBResult == NG_CAP_TAB_DIST_L){
					m_Label_Ins_Data2[13].SetTextColor(COLOR_RED);
					m_bResult[10] = TRUE;
					m_dData[28] = 0;

				}
				else{
					m_Label_Ins_Data2[13].SetTextColor(COLOR_GREEN);
					m_bResult[10] = FALSE;

				}
				m_Label_Ins_Data2[13].SetText(strData);

				strData.Format("R %.3f,",m_dTabDist[1]); // Top Cap 중심 ~ Tab 우
				//m_dData[11]=m_dTabDist[1];
				m_dData[11] = m_Project.m_nEdgeDist[1];//220518 픽셀로 변환중
				if(m_nRBResult == NG_CAP_TAB_DIST_R){//right
					m_Label_Ins_Data2[14].SetTextColor(COLOR_RED);
					m_bResult[11] = TRUE;
					m_dData[28] = 0;
				}
				else{
					m_Label_Ins_Data2[14].SetTextColor(COLOR_GREEN);
					m_bResult[11] = FALSE;
				}
				m_Label_Ins_Data2[14].SetText(strData);

				// Top Cap 중심 ~ Tab 상
				//double dtabtop = (m_Project.m_ptCapCenter.y-m_Project.m_nTopPos[0])*m_pDlgSet->m_FormTab2->m_dCamScale;
				//str1.Format("T %.2f",dtabtop);

				double dValue = 0.0;;
				dValue = (m_Project.m_ptCapCenter.y-m_Project.m_nTopPos[0])*m_pDlgSet->m_FormTab2->m_dCamScale;
				m_dCapCanDist = dValue;
				strData.Format("T %.2f",dValue);

				//m_dData[12] = dtabtop;
				m_dData[12] = (m_Project.m_ptCapCenter.y-m_Project.m_nTopPos[0]);//220518 픽셀로 변환중
				if(m_nRBResult == NG_TAP_POS)
				{
					m_Label_Ins_Data2[15].SetTextColor(COLOR_RED);
					m_bResult[12] = TRUE;
					m_dData[28] = 0;
				}
				else{
					m_Label_Ins_Data2[15].SetTextColor(COLOR_GREEN);
					m_bResult[12] = FALSE;
				}
				m_Label_Ins_Data2[15].SetText(strData);

				//하단 탭 좌~ 우 woo
				m_dData[28] = 0;
				if (m_nRBResult == NG_BOTTOM_TAB)
				{
					strData.Format("BottomTab(mm) : L %.3f,",m_dTabBottomSize[0]); // Top Cap 중심 ~ BottomTab 좌
					m_Label_Ins_Data2[16].SetTextColor(COLOR_RED);
					m_Label_Ins_Data2[16].SetText(strData);

					strData.Format("R %.3f,",m_dTabBottomSize[1]); // Top Cap 중심 ~ BottomTab 우
					m_Label_Ins_Data2[17].SetTextColor(COLOR_RED);
					m_Label_Ins_Data2[17].SetText(strData);
				}
				else{
 					strData.Format("BottomTab(mm) : L %.3f,",m_dTabBottomSize[0]); // Top Cap 중심 ~ BottomTab 좌
					m_Label_Ins_Data2[16].SetTextColor(COLOR_GREEN);
					m_Label_Ins_Data2[16].SetText(strData);

					strData.Format("R %.3f,",m_dTabBottomSize[1]); // Top Cap 중심 ~ BottomTab 우
					m_Label_Ins_Data2[17].SetTextColor(COLOR_GREEN);
					m_Label_Ins_Data2[17].SetText(strData);
				}
			}

			int nYOffset = 40;
			//str1.Format("Area:%.0f  Ratio:%.1f",m_Project.m_dMaxChainData[0],m_Project.m_dMaxChainData[5]);
			//m_Display[0].DrawText(pDC,CPoint(nx,60),str1);

			// 용접 Info
			m_dData[27] = 1;//과용접 ok

			CString strfrmae[2];
			strfrmae[0].Format("%d",m_nMaxThFrame[0]);
			strfrmae[1].Format("%d",m_nMaxThFrame[1]);
			//			m_dData[20] = atoi(m_strResultValue[0]);
			if(m_nRBResult==RET_OK || bstats){
				m_Label_Ins_Data2[18].SetTextColor(COLOR_GREEN);
				m_Label_Ins_Data2[18].SetText(m_strResultValue[0]);
				m_Label_Ins_Data2[19].SetTextColor(COLOR_GREEN);
				m_Label_Ins_Data2[19].SetText(m_strResultValue[1]);
				m_Label_Ins_Data2[20].SetTextColor(COLOR_GREEN);
				m_Label_Ins_Data2[20].SetText(strfrmae[0]);
				m_Label_Ins_Data2[21].SetTextColor(COLOR_GREEN);
				m_Label_Ins_Data2[21].SetText(strfrmae[1]);
				//m_Display[0].DrawText(pDC,CPoint(nx,100+nYOffset),m_strResultValue[0]); // R1
				//m_Display[0].DrawText(pDC,CPoint(nx,140+nYOffset),m_strResultValue[1]); // S1
				//m_Display[0].DrawText(pDC,CPoint(nx+400,100+nYOffset),strfrmae[0]); // R1
				//m_Display[0].DrawText(pDC,CPoint(nx+400,140+nYOffset),strfrmae[1]); // S1

				// hoho
				if(m_pDlgSet->m_FormTab2->m_bLineWelding == TRUE || m_pDlgSet->m_FormTab2->m_b2LineWelding == TRUE){
					;
				}
				else{
					m_Label_Ins_Data2[22].SetText(m_strResultValue[2]);
					m_Label_Ins_Data2[23].SetText(m_strResultValue[3]);

					//m_Display[0].DrawText(pDC,CPoint(nx+235,100+nYOffset),m_strResultValue[2]); // R2
					//m_Display[0].DrawText(pDC,CPoint(nx+235,140+nYOffset),m_strResultValue[3]); // S2
				}
			}
			else
			{
				int off1 = 235;
				if(	m_bHoleBad[0][0] )//=m_bHoleBad[1][0]=m_bHoleBad[1][1] = TRUE;//좌상
				{
					m_bResult[13]=TRUE;
					m_Label_Ins_Data2[18].SetTextColor(COLOR_RED);
					m_Label_Ins_Data2[18].SetText(m_strResultValue[0]);
					//m_Display[0].DrawText(pDC,CPoint(nx,100+nYOffset),m_strResultValue[0],0,COLOR_BLACK,COLOR_RED);

				}
				else{
					m_bResult[13]=FALSE;
					m_Label_Ins_Data2[18].SetTextColor(COLOR_GREEN);
					m_Label_Ins_Data2[18].SetText(m_strResultValue[0]);
					//m_Display[0].DrawText(pDC,CPoint(nx,100+nYOffset),m_strResultValue[0]);
				}

				if(m_bHoleBad[1][0] ){
					m_Label_Ins_Data2[19].SetTextColor(COLOR_RED);
					m_Label_Ins_Data2[19].SetText(m_strResultValue[1]);
					//m_Display[0].DrawText(pDC,CPoint(nx,140+nYOffset),m_strResultValue[1],0,COLOR_BLACK,COLOR_RED);
					m_bResult[14]=TRUE;
					m_dData[27] = 0;
				}
				else{
					m_Label_Ins_Data2[19].SetTextColor(COLOR_GREEN);
					m_Label_Ins_Data2[19].SetText(m_strResultValue[1]);
					//m_Display[0].DrawText(pDC,CPoint(nx,140+nYOffset),m_strResultValue[1]);
					m_bResult[14]=FALSE;
				}

				m_Label_Ins_Data2[20].SetTextColor(COLOR_GREEN);
				m_Label_Ins_Data2[20].SetText(strfrmae[0]);
				m_Label_Ins_Data2[21].SetTextColor(COLOR_GREEN);
				m_Label_Ins_Data2[21].SetText(strfrmae[1]);
				//m_Display[0].DrawText(pDC,CPoint(nx+400,100+nYOffset),strfrmae[0]); // R1
				//m_Display[0].DrawText(pDC,CPoint(nx+400,140+nYOffset),strfrmae[1]); // S1

				// hoho
				if(m_pDlgSet->m_FormTab2->m_bLineWelding == TRUE|| m_pDlgSet->m_FormTab2->m_b2LineWelding == TRUE){
					;
				}
				else{
					if(m_bHoleBad[0][1]){
						m_Label_Ins_Data2[22].SetTextColor(COLOR_RED);
						m_Label_Ins_Data2[22].SetText(m_strResultValue[2]);
						//m_Display[0].DrawText(pDC,CPoint(nx+off1,100+nYOffset),m_strResultValue[2],0,COLOR_BLACK,COLOR_RED);
					}
					else{
						m_Label_Ins_Data2[22].SetTextColor(COLOR_GREEN);
						m_Label_Ins_Data2[22].SetText(m_strResultValue[2]);
						//m_Display[0].DrawText(pDC,CPoint(nx+off1,100+nYOffset),m_strResultValue[2]);
					}

					if(m_bHoleBad[1][1]){
						m_Label_Ins_Data2[23].SetTextColor(COLOR_RED);
						m_Label_Ins_Data2[23].SetText(m_strResultValue[3]);
						m_dData[27] = 0;
						//m_Display[0].DrawText(pDC,CPoint(nx+off1,140+nYOffset),m_strResultValue[3],0,COLOR_BLACK,COLOR_RED);
					}
					else{
						m_Label_Ins_Data2[23].SetTextColor(COLOR_GREEN);
						m_Label_Ins_Data2[23].SetText(m_strResultValue[3]);
						//m_Display[0].DrawText(pDC,CPoint(nx+off1,140+nYOffset),m_strResultValue[3]);
					}
				}

			}


			// 			// 용접 후보 영역(빨간박스) 안에서 가장 큰 Blob 면적
			// 			str1.Format("Area : %.0f [%d]",m_Project.m_dMaxChainData[0],m_pDlgSet->m_FormTab4->m_nBlackArea);
			// 			
			// 			if(m_pDlgSet->m_FormTab4->m_bBlackArea  &&  m_pDlgSet->m_FormTab4->m_nBlackArea <m_Project.m_dMaxChainData[0])
			// 				m_Display[0].DrawText(pDC,CPoint(nx,180+nYOffset),str1,0,COLOR_BLACK,COLOR_RED);
			// 			else
			// 				m_Display[0].DrawText(pDC,CPoint(nx,180+nYOffset),str1);
			strData.Format("Center : %.d ",m_nTopCapCenterBrightness); //woo 센터 평균 밝기와 밝기보다 낮은 갯수 count display 
			m_Label_Ins_Data2[24].SetTextColor(COLOR_GREEN);
			m_Label_Ins_Data2[24].SetText(strData);

			if(m_pDlgSet->m_FormTab4->m_dTopCapCenterCount>0){
				CRect rtcenter;
				rtcenter.SetRect(m_Project.m_ptCapCenter.x-40,m_Project.m_ptCapCenter.y,m_Project.m_ptCapCenter.x+40,m_Project.m_ptCapCenter.y+75);//영역그리기
				m_Display[0].DrawRect(pDC,rtcenter,COLOR_GREEN,1,"");
			}			
			//			str1.Format("Left %.2f , Right %.2f",m_dTabDist[0],m_dTabDist[1]);
			//			m_Display[0].DrawText(pDC,CPoint(nx,260),str1);


			// 용접 좌 ~ Tab 좌, 용접 우 ~ Tab 우 간의 거리
			double dcheckmm = m_pDlgSet->m_FormTab4->m_dHoleEdgemm / m_pDlgSet->m_FormTab2->m_dCamScale;
			double dcheckmm1 = m_pDlgSet->m_FormTab4->m_dHoleEdgeINmm/ m_pDlgSet->m_FormTab2->m_dCamScale;
			double dl1,dr1,dw1,dw2;
			CString strWeldingWidth,strTabWidth;
			dl1 = (m_Project.m_nTopEdge[0] - m_rtWeldBlob.left)*m_pDlgSet->m_FormTab2->m_dCamScale;
			dr1 = (m_rtWeldBlob.right-(m_Project.m_nTopEdge[1]))*m_pDlgSet->m_FormTab2->m_dCamScale;
			dw1 = (m_rtWeldBlob.right-m_rtWeldBlob.left)*m_pDlgSet->m_FormTab2->m_dCamScale; //용접 길이
			dw2 = (m_Project.m_nTopEdge[1]-m_Project.m_nTopEdge[0])*m_pDlgSet->m_FormTab2->m_dCamScale; //Tab 폭
			strData.Format("Welding(mm) : Left %.2f, Right %.2f",dl1,dr1);
			strWeldingWidth.Format("Weld Width : %.2fmm",dw1);
			// 			strTabWidth.Format("Tab Width : %.2f mm",dw2);
			strTabWidth.Format("Tab Width : %.2fmm",m_Project.m_dTabTopLen);
			// 			AddtoInspLog(str1);
			if(m_pDlgSet->m_FormTab4->m_bHoleEdgeCheck)
			{
				// hoho 판정에 맞게 수정
				strData.Format("Welding(mm) : Left %.2f,",dl1);
				//m_dData[15] = dl1;  m_ptCapCenter.x //tc 중심
				m_dData[19] = m_Project.m_ptCapCenter.x-((m_rtWeldBlob.right+m_rtWeldBlob.left)/2);
				m_dData[20] = m_rtWeldBlob.right-m_rtWeldBlob.left;
				m_dData[15] = (m_Project.m_nTopEdge[0] - m_rtWeldBlob.left);
				if(m_nRBResult == NG_WELD_TAB_DIST){
					m_Label_Ins_Data2[25].SetTextColor(COLOR_RED);
					m_bResult[15]=TRUE;
					m_dData[26] = 0;
				}
				else{
					m_Label_Ins_Data2[25].SetTextColor(COLOR_GREEN);
					m_bResult[15]=FALSE;
				}
				/*if(m_rtWeldBlob.left< m_Project.m_nTopEdge[0]-dcheckmm || m_rtWeldBlob.left> m_Project.m_nTopEdge[0]+dcheckmm1){
					m_Label_Ins_Data2[25].SetTextColor(COLOR_RED);
					m_bResult[15]=TRUE;
					m_dData[26] = 0;
				}
				else if(m_rtWeldBlob.left >= m_Project.m_nTopEdge[0]-dcheckmm){
					m_Label_Ins_Data2[25].SetTextColor(COLOR_GREEN);
					m_bResult[15]=FALSE;
				}*/
				m_Label_Ins_Data2[25].SetText(strData);

				strData.Format("Right %.2f",dr1);
				//m_dData[16] = dr1;
				m_dData[16] = (m_rtWeldBlob.right-(m_Project.m_nTopEdge[1]));
				/*if(m_rtWeldBlob.right> m_Project.m_nTopEdge[1]+dcheckmm || m_rtWeldBlob.right< m_Project.m_nTopEdge[1]-dcheckmm1){
					m_Label_Ins_Data2[26].SetTextColor(COLOR_RED);
					m_bResult[16]=TRUE;
					m_dData[26] = 0;
				}
				else if(m_rtWeldBlob.right<= m_Project.m_nTopEdge[1]+dcheckmm){
					m_Label_Ins_Data2[26].SetTextColor(COLOR_GREEN);
					m_bResult[16]=FALSE;
				}*/
				if(m_nRBResult == NG_WELD_TAB_DIST){
					m_Label_Ins_Data2[26].SetTextColor(COLOR_RED);
					m_bResult[16]=TRUE;
					m_dData[26] = 0;
				}
				else{
					m_Label_Ins_Data2[26].SetTextColor(COLOR_GREEN);
					m_bResult[16]=FALSE;
				}
				m_Label_Ins_Data2[26].SetText(strData);

				m_Label_Ins_Data2[28].SetTextColor(COLOR_GREEN);
				m_Label_Ins_Data2[28].SetText(strWeldingWidth);
				m_Label_Ins_Data2[29].SetTextColor(COLOR_GREEN);
				m_Label_Ins_Data2[29].SetText(strTabWidth);
			}
			else
				m_Display[0].DrawText(pDC,CPoint(nx,220+nYOffset),strData);

			if(m_pDlgSet->m_FormTab4->m_bCheckDamage){
				strData.Format(_T("Tab Damage %d (W %d)"), m_Project.m_nBlackValue, m_Project.m_nWhiteValue);
				if(m_nRBResult == NG_TAP_DAMAGE)
				{
					m_Label_Ins_Data2[27].SetTextColor(COLOR_RED);
					m_dData[28] = 0;
				}
				else
				{
					m_Label_Ins_Data2[27].SetTextColor(COLOR_GREEN);
				}
				m_Label_Ins_Data2[27].SetText(strData);

			}

			if(m_pDlgSet->m_FormTab4->m_bMidWeld){
				strData.Format("Mid Weld :  %.0f",m_dMidWeldV);
				if(m_dMidWeldV>0 &&m_dMidWeldV < m_pDlgSet->m_FormTab4->m_dMidWeld)
					m_Display[0].DrawText(pDC,CPoint(nx,260+nYOffset),strData,0,COLOR_BLACK,COLOR_RED);
				else 
					m_Display[0].DrawText(pDC,CPoint(nx,260+nYOffset),strData);
			}

			int nWeldingBoxGap = 15;
			if(m_pDlgSet->m_FormTab4->m_bWeldingRange){
				strData.Format("Dist %.1f", ((m_Project.m_rtWeldRect[0].top+nWeldingBoxGap) - m_Project.m_ptCapCenter.y)*m_pDlgSet->m_FormTab2->m_dCamScale);
				if(m_Project.m_rtWeldRect[0].top+nWeldingBoxGap < m_Project.m_rtWeldingOkRange.top || m_Project.m_rtWeldRect[0].top+nWeldingBoxGap > m_Project.m_rtWeldingOkRange.bottom )
					m_Display[0].DrawText(pDC,CPoint(nx+180,260+nYOffset),strData,0,COLOR_BLACK,COLOR_RED);
				else
					m_Display[0].DrawText(pDC,CPoint(nx+180,260+nYOffset),strData);
			}

			// 라인 용접일때, 용접 굵기 표시
			// hoho, 일단 빼버림
			if(FALSE && m_pDlgSet->m_FormTab2->m_bLineWelding == TRUE && m_pDlgSet->m_FormTab2->m_b2LineWelding == FALSE)
			{
				// 높이 최소값을 가지는 위치X와 높이 최소값
				strData.Format("MinX %d - Height %d[%.0f]",m_Project.m_nLinMinMax[2],m_Project.m_nLinMinMax[0],m_pDlgSet->m_FormTab4->m_dMinHeight);
				if(m_pDlgSet->m_FormTab4->m_dMinHeight> m_Project.m_nLinMinMax[0])
					m_Display[0].DrawText(pDC,CPoint(nx,260+nYOffset),strData,0,COLOR_BLACK,COLOR_RED);
				else
					m_Display[0].DrawText(pDC,CPoint(nx,260+nYOffset),strData);

				// 높이 최대값을 가지는 위치X와 높이 최대값
				strData.Format("MaxX %d - Height %d[%.0f]",m_Project.m_nLinMinMax[3],m_Project.m_nLinMinMax[1],m_pDlgSet->m_FormTab4->m_dMaxHeight);
				if( m_pDlgSet->m_FormTab4->m_dMaxHeight<m_Project.m_nLinMinMax[1])
					m_Display[0].DrawText(pDC,CPoint(nx,300+nYOffset),strData,0,COLOR_BLACK,COLOR_RED);
				else
					m_Display[0].DrawText(pDC,CPoint(nx,300+nYOffset),strData);

				// 높이 최소값을 가지는 위치는 노란색으로 찍어주고,
				CPoint ptline1,ptline2;
				ptline1.SetPoint(m_Project.m_rtHoleRect.left+m_Project.m_nLinMinMax[2],m_Project.m_rtHoleRect.top-5);
				ptline2.SetPoint(m_Project.m_rtHoleRect.left+m_Project.m_nLinMinMax[2],m_Project.m_rtHoleRect.top+15);
				m_Display[0].DrawLine(pDC,ptline1,ptline2,"",COLOR_YELLOW,2);
				// 높이 최대값을 가지는 위치는 파란색으로 찍어준다
				ptline1.SetPoint(m_Project.m_rtHoleRect.left+m_Project.m_nLinMinMax[3],m_Project.m_rtHoleRect.top-5);
				ptline2.SetPoint(m_Project.m_rtHoleRect.left+m_Project.m_nLinMinMax[3],m_Project.m_rtHoleRect.top+15);
				m_Display[0].DrawLine(pDC,ptline1,ptline2,"",COLOR_BLUE,2);
			}
		}
		if(m_pDlgSet->m_FormTab4->m_bWeldingRange)
		{
			// Welding OK 영역 그리기
			CString str;
			str.Format("%.1lfmm", m_pDlgSet->m_FormTab4->m_dWeldingDistanceY - m_pDlgSet->m_FormTab4->m_dWeldingDistanceRange[0]);
			m_Display[0].DrawLine(pDC,CPoint(300,m_Project.m_rtWeldingOkRange.top),CPoint(1000,m_Project.m_rtWeldingOkRange.top),str,COLOR_WHITE,1);
			str.Format("%.1lfmm", m_pDlgSet->m_FormTab4->m_dWeldingDistanceY + m_pDlgSet->m_FormTab4->m_dWeldingDistanceRange[1]);
			m_Display[0].DrawLine(pDC,CPoint(300,m_Project.m_rtWeldingOkRange.bottom),CPoint(1000,m_Project.m_rtWeldingOkRange.bottom),str,COLOR_WHITE,1);
			//m_Display[0].DrawRect(pDC,m_Project.m_rtWeldingOkRange,COLOR_JOO,1,"OK Range");
		}
		if(m_pDlgSet->m_FormTab2->m_bShowHole && m_pDlgSet->m_FormTab2->m_bWeldInTab){
			ngap = 10;
			m_Display[0].DrawCross(pDC,CRect(m_ptWeldCenterPos.x-ngap,m_ptWeldCenterPos.y-ngap,m_ptWeldCenterPos.x+ngap,m_ptWeldCenterPos.y+ngap),COLOR_BLUE,3,"");

			m_Display[0].DrawRect(pDC,m_rtWeldBlob,COLOR_RED,2,"");
		}

		if (m_pDlgSet->m_FormTab1->m_bUseDeepLearning) {
			double dDLValue = 0.0;
			double dDLUsl = 0.0;
			double dDLLsl = 0.0;

			//1.Cap-Can X
			dDLUsl = pFormTab5->m_dDLSpecTcCanX[1];
			dDLValue = m_dDLCapCanX;
			strData.Format("Cap-Can X : %.2f", dDLValue);
			if (dDLValue > dDLUsl) 
				m_Label_DL_Ins_Data[0].SetTextColor(COLOR_RED);
			else
				m_Label_DL_Ins_Data[0].SetTextColor(COLOR_GREEN);
			m_Label_DL_Ins_Data[0].SetText(strData);

			//2.Cap-Can Y
			dDLLsl = pFormTab5->m_dDLSpecTcCanY[0];
			dDLUsl = pFormTab5->m_dDLSpecTcCanY[1];
			dDLValue = m_dDLCapCanY;
			strData.Format("Cap-Can Y : %.2fmm", dDLValue);
			if (dDLValue > dDLUsl || dDLValue < dDLLsl)
				m_Label_DL_Ins_Data[1].SetTextColor(COLOR_RED);
			else
				m_Label_DL_Ins_Data[1].SetTextColor(COLOR_GREEN);
			m_Label_DL_Ins_Data[1].SetText(strData);

			//3.Tab Corner Gap Y
			dDLUsl = pFormTab5->m_dDLSpecTabEdgeGapY[1];
			dDLValue = m_dDLTabEdgeGapY;
			strData.Format("Tab Corner Gap Y: %.2fmm", dDLValue);
			if (dDLValue > dDLUsl)
				m_Label_DL_Ins_Data[2].SetTextColor(COLOR_RED);
			else
				m_Label_DL_Ins_Data[2].SetTextColor(COLOR_GREEN);
			m_Label_DL_Ins_Data[2].SetText(strData);

			//4.Cap-Weld X
			/*dDLLsl = pFormTab5->m_dDLSpecTcWeldY[0];
			dDLUsl = pFormTab5->m_dDLSpecTcWeldY[1];*/
			dDLValue = m_dDLCapWeldX;
			strData.Format("Cap-Weld Y: %.2f", dDLValue);
			/*if (dDLValue > dDLUsl || dDLValue < dDLLsl)
				m_Label_DL_Ins_Data[3].SetTextColor(COLOR_RED);
			else*/
			m_Label_DL_Ins_Data[3].SetTextColor(COLOR_GREEN);
			m_Label_DL_Ins_Data[3].SetText(strData);

			//5.Cap-Weld Y
			dDLLsl = pFormTab5->m_dDLSpecTcWeldY[0];
			dDLUsl = pFormTab5->m_dDLSpecTcWeldY[1];
			dDLValue = m_dDLCapWeldY;
			strData.Format("Cap-Weld Y: %.2fmm", dDLValue);
			if (dDLValue > dDLUsl || dDLValue < dDLLsl)
				m_Label_DL_Ins_Data[4].SetTextColor(COLOR_RED);
			else
				m_Label_DL_Ins_Data[4].SetTextColor(COLOR_GREEN);
			m_Label_DL_Ins_Data[4].SetText(strData);

			//6.Cap-Tab Top Y
			dDLUsl = pFormTab5->m_dDLSpecTcTabY[1];
			dDLValue = m_dDLCapTabTopY;
			strData.Format("Cap-Tab Y: %.2fmm", dDLValue);
			if (abs(dDLValue) > dDLUsl)
				m_Label_DL_Ins_Data[5].SetTextColor(COLOR_RED);
			else
				m_Label_DL_Ins_Data[5].SetTextColor(COLOR_GREEN);
			m_Label_DL_Ins_Data[5].SetText(strData);
			
			//7.Weld-Can Y
			dDLLsl = pFormTab5->m_dDLSpecWeldCanY[0];
			dDLUsl = pFormTab5->m_dDLSpecWeldCanY[1];
			dDLValue = m_dDLWeldCanY;
			strData.Format("Weld-Can Y: %.2fmm", dDLValue);
			if (dDLValue > dDLUsl || dDLValue < dDLLsl)
				m_Label_DL_Ins_Data[6].SetTextColor(COLOR_RED);
			else
				m_Label_DL_Ins_Data[6].SetTextColor(COLOR_GREEN);
			m_Label_DL_Ins_Data[6].SetText(strData);
		
			//8.Cap Center - Tab Edge X Left
			dDLLsl = pFormTab5->m_dDLSpecTcTabLeft[0];
			dDLUsl = pFormTab5->m_dDLSpecTcTabLeft[1];
			dDLValue = m_dDLCapTabLeftX;
			strData.Format("Cap-Tab Edge X L %.2fmm", dDLValue);
			if (dDLValue > dDLUsl || dDLValue < dDLLsl)
				m_Label_DL_Ins_Data[7].SetTextColor(COLOR_RED);
			else
				m_Label_DL_Ins_Data[7].SetTextColor(COLOR_GREEN);
			m_Label_DL_Ins_Data[7].SetText(strData);

			//9.Cap Center - Tab Edge X Right
			dDLLsl = pFormTab5->m_dDLSpecTcTabRight[0];
			dDLUsl = pFormTab5->m_dDLSpecTcTabRight[1];
			dDLValue = m_dDLCapTabRightX;
			strData.Format("R %.2fmm", dDLValue);
			if (dDLValue > dDLUsl || dDLValue < dDLLsl)
				m_Label_DL_Ins_Data[8].SetTextColor(COLOR_RED);
			else
				m_Label_DL_Ins_Data[8].SetTextColor(COLOR_GREEN);
			m_Label_DL_Ins_Data[8].SetText(strData);

			//10.Weld Length
			dDLLsl = pFormTab5->m_dDLSpecWeldLength[0];
			dDLUsl = pFormTab5->m_dDLSpecWeldLength[1];
			dDLValue = m_dDLWeldLenth;
			strData.Format("Weld Length: %.2fmm", dDLValue);
			if (dDLValue > dDLUsl || dDLValue < dDLLsl)
				m_Label_DL_Ins_Data[9].SetTextColor(COLOR_RED);
			else
				m_Label_DL_Ins_Data[9].SetTextColor(COLOR_GREEN);
			m_Label_DL_Ins_Data[9].SetText(strData);
			
			//11.Weld Width
			dDLUsl = pFormTab5->m_dDLSpecWeldArea[1];
			dDLValue = m_dDLWeldWidth;
			strData.Format("Weld Wdith: %.2fmm", dDLValue);
			if (dDLValue > dDLUsl)
				m_Label_DL_Ins_Data[10].SetTextColor(COLOR_RED);
			else
				m_Label_DL_Ins_Data[10].SetTextColor(COLOR_GREEN);
			m_Label_DL_Ins_Data[10].SetText(strData);

			//외관
			//12.is tc
			dDLValue = m_bisTC;
			strData.Format("TC Present OK");
			m_Label_DL_Ins_Data[11].SetTextColor(COLOR_GREEN);
			if (dDLValue != 1) {
				strData.Format("TC Present NG");
				m_Label_DL_Ins_Data[11].SetTextColor(COLOR_RED);
			}
			m_Label_DL_Ins_Data[11].SetText(strData);

			dDLValue = m_bisTC_frontback;
			strData.Format("TC Front/Back OK");
			m_Label_DL_Ins_Data[12].SetTextColor(COLOR_GREEN);
			if (dDLValue != 1) {
				strData.Format("TC Front/Back NG");
				m_Label_DL_Ins_Data[12].SetTextColor(COLOR_RED);
			}
			m_Label_DL_Ins_Data[12].SetText(strData);

			dDLValue = m_bisTC_slatned;
			strData.Format("TC Slatned OK");
			m_Label_DL_Ins_Data[13].SetTextColor(COLOR_GREEN);
			if (dDLValue != 1) {
				strData.Format("TC Slatned NG");
				m_Label_DL_Ins_Data[13].SetTextColor(COLOR_RED);
			}
			m_Label_DL_Ins_Data[13].SetText(strData);

			dDLValue = m_bisTC_CID;
			strData.Format("TC CID OK");
			m_Label_DL_Ins_Data[14].SetTextColor(COLOR_GREEN);
			if (dDLValue != 1) {
				strData.Format("TC CID NG");
				m_Label_DL_Ins_Data[14].SetTextColor(COLOR_RED);
			}
			m_Label_DL_Ins_Data[14].SetText(strData);

			dDLValue = m_bisCAN;
			strData.Format("CAN Present OK");
			m_Label_DL_Ins_Data[15].SetTextColor(COLOR_GREEN);
			if (dDLValue != 1) {
				strData.Format("CAN Present NG");
				m_Label_DL_Ins_Data[15].SetTextColor(COLOR_RED);
			}
			m_Label_DL_Ins_Data[15].SetText(strData);

			dDLValue = m_bisTAB;
			strData.Format("TAB Present OK");
			m_Label_DL_Ins_Data[16].SetTextColor(COLOR_GREEN);
			if (dDLValue != 1) {
				strData.Format("TAB Present NG");
				m_Label_DL_Ins_Data[16].SetTextColor(COLOR_RED);
			}
			m_Label_DL_Ins_Data[16].SetText(strData);

			dDLValue = m_bisTABerror;
			strData.Format("TAB Damage OK");
			m_Label_DL_Ins_Data[17].SetTextColor(COLOR_GREEN);
			if (dDLValue != 1) {
				strData.Format("TAB Damage NG");
				m_Label_DL_Ins_Data[17].SetTextColor(COLOR_RED);
			}
			m_Label_DL_Ins_Data[17].SetText(strData);

			dDLValue = m_bisETCerror;
			strData.Format("Contaminated OK");
			m_Label_DL_Ins_Data[18].SetTextColor(COLOR_GREEN);
			if (dDLValue != 1) {
				strData.Format("Contaminated NG");
				m_Label_DL_Ins_Data[18].SetTextColor(COLOR_RED);
			}
			m_Label_DL_Ins_Data[18].SetText(strData);

			dDLValue = m_bisYJ_fromTAB;
			strData.Format("Welding OK");
			m_Label_DL_Ins_Data[19].SetTextColor(COLOR_GREEN);
			if (dDLValue != 1) {
				strData.Format("Weld NG");
				m_Label_DL_Ins_Data[19].SetTextColor(COLOR_RED);
			}
			m_Label_DL_Ins_Data[19].SetText(strData);

			dDLValue = m_bisYakYJ;
			strData.Format("Weak Weld OK");
			m_Label_DL_Ins_Data[20].SetTextColor(COLOR_GREEN);
			if (dDLValue != 1) {
				strData.Format("Weak Weld NG");
				m_Label_DL_Ins_Data[20].SetTextColor(COLOR_RED);
			}
			m_Label_DL_Ins_Data[20].SetText(strData);

			dDLValue = m_bisGwaYJ;
			strData.Format("Strong Weld OK");
			m_Label_DL_Ins_Data[21].SetTextColor(COLOR_GREEN);
			if (dDLValue != 1) {
				strData.Format("Strong Weld NG");
				m_Label_DL_Ins_Data[21].SetTextColor(COLOR_RED);
			}
			m_Label_DL_Ins_Data[21].SetText(strData);

			dDLValue = m_bisGeuYJ;
			strData.Format("Soot OK");
			m_Label_DL_Ins_Data[22].SetTextColor(COLOR_GREEN);
			if (dDLValue != 1) {
				strData.Format("Soot NG");
				m_Label_DL_Ins_Data[22].SetTextColor(COLOR_RED);
			}
			m_Label_DL_Ins_Data[22].SetText(strData);
		}
	}
	else
	{
		m_Display[0].DrawImage(pDC);
		for(int i=0;i<5;i++)
			m_DisplayBad[i].DrawImage(pDC);
	}

	// hoho, 캘리브레이션 할때 캡 좌우 경계 찍어서 보여주기
	if(m_pDlgSet->m_FormTab5->m_bCalibration)
	{
		CPoint ptcal,ptcal1;
		ptcal.SetPoint(m_Project.m_nTopCapleft, m_Project.m_ptCapCenter1.y - 20);
		ptcal1.SetPoint(m_Project.m_nTopCapleft, m_Project.m_ptCapCenter1.y + 20);
		m_Display[0].DrawLine(pDC,ptcal,ptcal1,"",COLOR_BLACK,3);

		ptcal.SetPoint(m_Project.m_nTopCapRight, m_Project.m_ptCapCenter1.y - 20);
		ptcal1.SetPoint(m_Project.m_nTopCapRight, m_Project.m_ptCapCenter1.y + 20);
		m_Display[0].DrawLine(pDC,ptcal,ptcal1,"",COLOR_BLACK,3);


		ptcal.SetPoint(m_Project.m_nTopCapleft1, m_Project.m_ptCapCenter1.y - 20);
		ptcal1.SetPoint(m_Project.m_nTopCapleft1, m_Project.m_ptCapCenter1.y + 20);
		m_Display[0].DrawLine(pDC,ptcal,ptcal1,"",COLOR_GREEN,3);

		ptcal.SetPoint(m_Project.m_nTopCapRight1, m_Project.m_ptCapCenter1.y - 20);
		ptcal1.SetPoint(m_Project.m_nTopCapRight1, m_Project.m_ptCapCenter1.y + 20);
		m_Display[0].DrawLine(pDC,ptcal,ptcal1,"",COLOR_GREEN,3);

		CString strTopCapWidth;
		strTopCapWidth.Format("TopCap Width : %.3f",m_Project.m_dCapWidth);
		m_Display[0].DrawText(pDC,CPoint(800,300),strTopCapWidth);
	}

	ReleaseDC(pDC);
}


BOOL CFlyCapture2_EXDlg::OnEraseBkgnd(CDC* pDC)
{
//	CEdit* pFocus = (CEdit*)GetDlgItem(IDC_EDIT_BARCODE);

//	pFocus->SetFocus();

//	return 0;
	CString strPath;
	strPath.Format(_T("c:\\GLIM\\DWBG.BMP"));

	CRect rcClient;
	GetClientRect(rcClient);

	CDC MemDC;
	CBitmap MemBitmap;
	MemDC.CreateCompatibleDC(pDC);
	MemBitmap.CreateCompatibleBitmap(pDC, rcClient.right, rcClient.bottom);
	CBitmap* pOldMem=MemDC.SelectObject(&MemBitmap);

	m_Display[0].DrawBMP(strPath, &MemDC, 0, 0, SRCCOPY);
	pDC->BitBlt(0, 0, rcClient.right, rcClient.bottom, &MemDC, 0,0, SRCCOPY);
	
	MemDC.DeleteDC();
	MemBitmap.DeleteObject();	


	return TRUE;

	//return CDialog::OnEraseBkgnd(pDC);
}

void CFlyCapture2_EXDlg::OnBnClickedRadioFullScreen()
{
	UpdateData(TRUE);
	Invalidate();
}

void CFlyCapture2_EXDlg::OnStnClickedDrawView()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
#define TOTAL_LOG_LIST_COL		30

void CFlyCapture2_EXDlg::AddtoList(CString strData,int mode,BOOL bWrite)
{
	CString strLog;
	GetLocalTime(&m_time);
	strLog.Format(_T("[%02d:%02d:%02d:%03ld]"), m_time.wHour, m_time.wMinute, m_time.wSecond, m_time.wMilliseconds);
	strData = strLog + strData;

	if(mode==1)
		m_Multi.WriteLog(strData);
	else if(mode==2)
		WriteMesLog(strData);//mes
	else if(mode==3)
		m_Multi.WriteCpuLog(strData);//cpu
	else if(mode==4)
		WriteInspLog(strData,strData);//cpu
	else{
		m_Multi.WriteLog(strData);
		WriteMesLog(strData);//mes
	}

	///*	
	m_cLogList.InsertString(-1,strData);
	int x = m_cLogList.GetCount();
	m_cLogList.SetTopIndex(x-1);
	m_cLogList.SendMessage(WM_VSCROLL,SB_BOTTOM);
	if(x > TOTAL_LOG_LIST_COL)
	{
		m_cLogList.DeleteString(0);
		m_cLogList.SetCurSel(TOTAL_LOG_LIST_COL-1);
	}
	//*/
	
}
void CFlyCapture2_EXDlg::AddtoListCPU(CString strData)
{
	//if(bWrite)

	//	WriteLog(strData);
	CString str;
	CTime time = CTime::GetCurrentTime();
	strData = time.Format("[%H:%M:%S]") + strData;	
	m_Multi.WriteCpuLog(strData);	
}

void CFlyCapture2_EXDlg::AddtoInspLog(CString strData,BOOL bWrite)
{
	CString strLog2;
	GetLocalTime(&m_timeInsp);
	strLog2.Format(_T("[%02d:%02d:%02d:%03ld]"), m_timeInsp.wHour, m_timeInsp.wMinute, m_timeInsp.wSecond, m_timeInsp.wMilliseconds);

	if(bWrite)
		WriteInspLog("TEST",strData);
		
	//m_cInspLog.InsertString(-1,strData);
	//m_cInspLog.SetTopIndex(x-1);
	//m_cInspLog.SendMessage(WM_VSCROLL,SB_BOTTOM);
	//if(x > TOTAL_LOG_LIST_COL)
	//{
	//}	
	m_cInspLog.InsertString(-1,strData);
	int x = m_cInspLog.GetCount();
	m_cInspLog.SetTopIndex(x-1);
	m_cInspLog.SendMessage(WM_VSCROLL,SB_BOTTOM);
	if(x > TOTAL_LOG_LIST_COL)
	{
		m_cInspLog.DeleteString(0);
		m_cInspLog.SetCurSel(TOTAL_LOG_LIST_COL-1);
	}	
}

void CFlyCapture2_EXDlg::AddtoInspLog1(CString strData, int nMode)
{
	CString str;
	GetLocalTime(&m_timeInsp2);
	str.Format(_T("[%02d:%02d:%02d:%03ld]"), m_timeInsp2.wHour, m_timeInsp2.wMinute, m_timeInsp2.wSecond, m_timeInsp2.wMilliseconds);
	strData = str + strData;

	if (nMode == 0)
		WriteInspLog("TabPos", strData);
	else if (nMode == 1)
		WriteInspLog("Vison Time", strData);
	else if (nMode == 2)
		WriteInspLog("Ai Time", strData);

	//m_cInspLog.InsertString(-1,strData);
	//m_cInspLog.SetTopIndex(x-1);
	//m_cInspLog.SendMessage(WM_VSCROLL,SB_BOTTOM);
	//if(x > TOTAL_LOG_LIST_COL)
	//{
	//}	
	m_cInspLog.InsertString(-1,strData);
	int x = m_cInspLog.GetCount();
	m_cInspLog.SetTopIndex(x-1);
	m_cInspLog.SendMessage(WM_VSCROLL,SB_BOTTOM);
	if(x > TOTAL_LOG_LIST_COL)
	{
		m_cInspLog.DeleteString(0);
		m_cInspLog.SetCurSel(TOTAL_LOG_LIST_COL-1);
	}	
}


void CFlyCapture2_EXDlg::WriteInspLog(CString name,CString str)
{
	CTime time = CTime::GetCurrentTime();

	CString strFile,strpath;
	strpath.Format("c:\\Data\\LOG");
	CreateDirectory(strpath,NULL);
	strpath.Format("%s%s\\","c:\\Data\\LOG\\",time.Format("%Y%m%d"));
	CreateDirectory(strpath,NULL);
	strFile.Format("%s%s\\%s_LOG.txt","c:\\Data\\LOG\\",time.Format("%Y%m%d"),name);
	//strFile.Format("%s%s_LOG.txt",LOG_PATH,time.Format("%Y%m%d"));

	FILE *fp;
	fp = fopen((LPCSTR)strFile, "a");

	if(!fp)
		return;
	char* buf = new char[str.GetLength()+1];
	memcpy(buf, str.LockBuffer(), str.GetLength());
	buf[str.GetLength()] = '\0';

	fputs(buf, fp);
	fputs("\n",fp);
	delete []buf;
	fclose(fp);
}
void CFlyCapture2_EXDlg::WriteInspLogC(CString name,CString str)
{
	CTime time = CTime::GetCurrentTime();
	CString kk1 = time.Format("[%H:%M:%S]");
	str = kk1+str;

	CString strFile,strpath;
	strpath.Format("c:\\CLOG");
	CreateDirectory(strpath,NULL);
	strFile.Format("%s\\LOG.txt","c:\\CLOG\\",name);

	FILE *fp;
	fp = fopen((LPCSTR)strFile, "a");

	if(!fp)
		return;
	char* buf = new char[str.GetLength()+1];
	memcpy(buf, str.LockBuffer(), str.GetLength());
	buf[str.GetLength()] = '\0';

	fputs(buf, fp);
	fputs("\n",fp);
	delete []buf;
	fclose(fp);
}

void CFlyCapture2_EXDlg::WriteInspLogD(CString name,CString str)
{
	CTime time = CTime::GetCurrentTime();

	CString strFile,strpath;
	strpath.Format("D:\\DLOG");
	CreateDirectory(strpath,NULL);
	strFile.Format("%s\\LOG.txt","D:\\DLOG\\",name);

	FILE *fp;
	fp = fopen((LPCSTR)strFile, "a");

	if(!fp)
		return;
	char* buf = new char[str.GetLength()+1];
	memcpy(buf, str.LockBuffer(), str.GetLength());
	buf[str.GetLength()] = '\0';

	fputs(buf, fp);
	fputs("\n",fp);
	delete []buf;
	fclose(fp);
}
void CFlyCapture2_EXDlg::WriteMesLog(CString str)
{
	CTime time = CTime::GetCurrentTime();

	CString strFile,strpath;
	strpath.Format("%s%s\\","c:\\Glim\\LOG\\",time.Format("%Y%m%d"));
	CreateDirectory(strpath,NULL);
	strFile.Format("%s%s\\IO_COUNT_LOG.txt","c:\\Glim\\LOG\\",time.Format("%Y%m%d"));
	//strFile.Format("%s%s_LOG.txt",LOG_PATH,time.Format("%Y%m%d"));

	FILE *fp;
	fp = fopen((LPCSTR)strFile, "a");

	if(!fp)
		return;
	char* buf = new char[str.GetLength()+1];
	memcpy(buf, str.LockBuffer(), str.GetLength());
	buf[str.GetLength()] = '\0';

	fputs(buf, fp);
	fputs("\n",fp);
	delete []buf;
	fclose(fp);
}


void CFlyCapture2_EXDlg::OnLbnSelchangeListLog2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

BOOL CFlyCapture2_EXDlg::Getini(LPCSTR stSECTION, LPCSTR stKEY, LPCSTR stInIFile,LPSTR retVALUE)
{
	char	retVal[255];
	DWORD	worked;
	char	CurrentDIR[MAX_PATH];
	char	stFullInIFile[MAX_PATH];
	GetCurrentDirectory(MAX_PATH,CurrentDIR);
	sprintf(stFullInIFile,"%s\\%s",CurrentDIR,stInIFile);
	worked = ::GetPrivateProfileString( stSECTION ,stKEY ,"" , retVal, 255 ,stFullInIFile);
	if ( worked == 0 )
	{
		strcpy(retVALUE,"");
		return FALSE;
	}
	strcpy(retVALUE, retVal);
	return TRUE;
}

void CFlyCapture2_EXDlg::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	if(nChar)
	{
		int n = 0;
	}
	CDialog::OnChar(nChar, nRepCnt, nFlags);
}

void CFlyCapture2_EXDlg::OnRawInput(UINT nInputcode, HRAWINPUT hRawInput)
{
	// 이 기능을 사용하려면 Windows XP 이상이 필요합니다.
	// _WIN32_WINNT 기호는 0x0501보다 크거나 같아야 합니다.
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	int i = 0;
	CDialog::OnRawInput(nInputcode, hRawInput);
}

void CFlyCapture2_EXDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	int n = 0;
	CDialog::OnKeyDown(nChar, nRepCnt, nFlags);
}

UINT CFlyCapture2_EXDlg::OnGetDlgCode()
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	int n = 0;

	return CDialog::OnGetDlgCode();
}

void CFlyCapture2_EXDlg::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	int i = 0;
	CDialog::OnKeyUp(nChar, nRepCnt, nFlags);
}

void CFlyCapture2_EXDlg::OnSizeClipboard(CWnd* pClipAppWnd, HGLOBAL hRect)
{
	CDialog::OnSizeClipboard(pClipAppWnd, hRect);
	int n = 0;

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

void CFlyCapture2_EXDlg::OnUniChar(UINT Char, UINT nRepCnt, UINT nFlags)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	int n = 0;
	CDialog::OnUniChar(Char, nRepCnt, nFlags);
}

void CFlyCapture2_EXDlg::OnClipboardUpdate()
{
	// 이 기능을 사용하려면 Windows Vista 이상이 있어야 합니다.
	// _WIN32_WINNT 기호는 0x0600보다 크거나 같아야 합니다.
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	int n = 0;
	CDialog::OnClipboardUpdate();
}

BOOL CFlyCapture2_EXDlg::OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	int n = 0;
	return CDialog::OnCopyData(pWnd, pCopyDataStruct);
}

BOOL CFlyCapture2_EXDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	int n = 0;

	if(pMsg->message == WM_KEYDOWN)
	{

		if(pMsg->wParam == VK_CAPITAL)
		{
			;
		}

		if(pMsg->wParam  == VK_RETURN)
			int k = 0;
		return 0;

/*		if(pMsg->message == 256 && m_nShowSet==0)
		{
			CEdit* pFocus = (CEdit*)GetDlgItem(IDC_EDIT_BARCODE);
			
			pFocus->SetFocus();
			
		}
		*/
	//Set focus

	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CFlyCapture2_EXDlg::WriteData()
{
	CStdioFile   file;
	CString filename;
	//strFile.Format("%s\\%s.csv","c:\\Glim\\Log\\LotData\\",strDate);
	CTime ttime = CTime::GetCurrentTime();
	CString kk = ttime.Format("%y%m%d");
	CString kk1 = ttime.Format("%Y%m");
	CString strpath("C:\\Data\\");
	CreateDirectory(strpath,NULL);
	strpath.Format("C:\\Data\\LOG\\");
	CreateDirectory(strpath,NULL);
	strpath.Format("C:\\Data\\LOG\\LotData\\");
	CreateDirectory(strpath,NULL);

	CString strheader,str;
	CString strstime = m_strInspStartTime;
	CString strETime = m_strInspEndTime;
	int nInsptime = m_nInspMinute;//m_Project.m_dInsTime;
	double dTactTime = m_Project.m_dInsTime/1000;//m_dTactTimeAve[0]=m_dTactTimeAve[1]=m_dTactTimeAve[2] = 0;
	int nOkCount = m_nOKCount;//m_nModifyCount;//m_nOKCount;
	int nNgCount = m_nNgCount;
	int nMarkCount = m_nNg2Count;
//	int nSumCount =nOkCount+nNgCount+nMarkCount;
	int nSumCount =m_nFullFrameCount;//m_nOKCount+nNgCount+nMarkCount;
	if(m_dTactTimeAve[1]>0)
		 dTactTime =(m_dTactTimeAve[0]/m_dTactTimeAve[1])/1000;
	int nblobcnt = 0;
	if(m_Project.m_nBlobCnt[0]>m_Project.m_nBlobCnt[1])
		nblobcnt = m_Project.m_nBlobCnt[0];
	else
		nblobcnt = m_Project.m_nBlobCnt[1];
	CString strmode;
	if(m_LightMap->m_nPackMode==0)
		strmode.Format("정상투입");
	else
		strmode.Format("재투입");
	//시작시간 Barcode	PartNo 상하	인치 종료시간 양/불 MES전송	높이 PSsheet 위치  표리반전 위치 

	filename.Format(_T("%s%s.CSV"),strpath,kk);
	strheader.Format("\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",\"%s\", "
		,"시간","검사모드","CODE","PartNo","상/하","인치","종료시간","양/불","MES전송","높이","불량개수","불량명","불량위치","Value","검사횟수");

//	str.Format("1","2","%s","%s","%d","%s","%s","%d","%d","%d","%d","%d",m_DlgBarcode->m_strPartNo,m_DlgBarcode->m_strOCVCode,m_Project.m_nFrameCount,
//		strstime,strETime,nInsptime,nTactTime,nOkCount,nNgCount,nMarkCount);
	CString strokng;
	strokng.Format("양품");
	if(!file.Open(filename, CFile::modeReadWrite|CFile::typeText)) 
	{		
		if(file.Open(filename, CFile::modeCreate|CFile::modeReadWrite|CFile::typeText))
		{
			file.WriteString(strheader);	
			file.WriteString(_T("\n"));
			if(m_Project.m_nMarkResult>0)

			file.Close();
		}
	}
	else
	{
		file.SeekToEnd();
		file.WriteString(_T("\n"));

		file.Close(); 
	}
}

void CFlyCapture2_EXDlg::CheckHdd()
{
	long lTotal, lOccupied, lRemaining;
	GetHDDSpace("C:\\", &lTotal, &lOccupied, &lRemaining);
	m_dRemainHDDSpace1 = ((double)lRemaining / lTotal)*100;
	GetHDDSpace("d:\\", &lTotal, &lOccupied, &lRemaining);	//chcho
	m_dRemainHDDSpace2 = ((double)lRemaining / lTotal)*100;


	//view
	CString str;
	str.Format(" HDD 1: %.1f%%",m_dRemainHDDSpace1);
	m_LabelRemainHDD.SetText(str);
	str.Format(" HDD 2: %.1f%%",m_dRemainHDDSpace2);
	m_LabelRemainHdd1.SetText(str);

	if (m_dRemainHDDSpace2<m_pDlgSet->m_FormTab3->m_nRemainHddSpace && m_bCheckHdd==FALSE)
	{
		str.Format("Hdd Check %.1f%%",m_dRemainHDDSpace2);
		AddtoList(str,1);
		HddAlarm(0);
		SetTimer(1456,1000,NULL);
	}
}


#define KILO	1024
void CFlyCapture2_EXDlg::GetHDDSpace(CString strPath, long *total, long *occupied, long *remaining)
{
	ULARGE_INTEGER lpFree;
	ULARGE_INTEGER lpTotal;
	ULARGE_INTEGER lpOccupied;

	double d_longtotal = 0;
	double d_longremaining = 0;
	double d_longoccupied = 0;

	GetDiskFreeSpaceEx(strPath, &lpFree, &lpTotal, &lpOccupied);

	d_longremaining = ((double)lpFree.HighPart * UINT_MAX) + lpFree.LowPart; // 사용한 공간
	d_longtotal = ((double)lpTotal.HighPart * UINT_MAX) + lpTotal.LowPart; // 총 드라이브 크기
	d_longoccupied = d_longtotal - d_longremaining; // 사용한 공간

	// mega based
	*occupied = (unsigned int)(d_longoccupied / (KILO*KILO));
	*remaining = (unsigned int)(d_longremaining / (KILO*KILO));
	*total = (unsigned int)(d_longtotal / (KILO*KILO));
}

void CFlyCapture2_EXDlg::BufCopy(int nCam)
{
	int neven = m_nBufCount[nCam]%2;
	m_nBufCount[nCam]++;
	m_Display[nCam].m_nCamera_Width = CAMERA_WIDTH;
	m_Display[nCam].m_nCamera_Height = CAMERA_HEIGHT;//CAMERA_LINE*m_pDlgSet->m_FormTab2->m_nFrameLength;
	m_Display[nCam].m_rtFullImg.SetRect(0,0,CAMERA_WIDTH,CAMERA_HEIGHT);
	m_Display[nCam].m_rtRoiImg.SetRect(0,0,CAMERA_WIDTH,CAMERA_HEIGHT);
	m_Display[nCam].m_rtZoom.SetRect(0,0,CAMERA_WIDTH,CAMERA_HEIGHT);
	m_Display[nCam].m_nZoom = 1;

	if(nCam==1)
		nCam = nCam;
	unsigned char* fmtmp = m_Display[nCam].m_pImg->GetImagePtr();
//	memcpy(m_Project.m_fm[nCam],m_Project.m_fm[nCam][m_nNewCount%2],CAMERA_WIDTH*CAMERA_LINE*m_pDlgSet->m_FormTab2->m_nFrameLength);
//	memcpy(m_Display[nCam].m_pImg->GetImagePtr(),m_Project.m_fm[nCam][m_nNewCount%2],CAMERA_WIDTH*CAMERA_LINE*m_pDlgSet->m_FormTab2->m_nFrameLength);
	memcpy(fmtmp,m_Project.m_fm[nCam],CAMERA_WIDTH*CAMERA_HEIGHT);
	memcpy(m_DisplaySave[nCam].m_pImg->GetImagePtr(),m_Display[nCam].m_pImg->GetImagePtr(),CAMERA_WIDTH*CAMERA_HEIGHT);
//
	m_Project.m_fm[nCam] = m_Display[nCam].m_pImg->GetImagePtr();
//	m_Project.m_fmInsp[nCam] = m_Display[nCam].m_pImg->GetImagePtr();;
//	m_nNewCount++;
}
void CFlyCapture2_EXDlg::BufCopy1()
{

}
void CFlyCapture2_EXDlg::CheckKeyMode()
{

	//IMM의 API를 이용 하시면 됩니다
	HIMC data; //IMM핸들 변수를 설정하고
	// 현재 윈도우의 IMM 핸들러을 얻습니다.
	data=ImmGetContext(this->m_hWnd);
	// 한영상태를 얻는다.
	// hanFlag가 1이면 한글 
	//0이면 영문
	BOOL hanFlag=ImmGetOpenStatus(data);

	if(hanFlag)
	{
		ImmSetConversionStatus(data,0,0); //두번째 인자 1:한글	0:영문
		AddtoList("영문으로 수정합니다.",1);

	}//		TRACE("\n한글");
	else
	{
	}//TRACE("\n영어");


	// IMM핸들을 해제 합니다
	ImmReleaseContext(this->m_hWnd,data);
}


int CFlyCapture2_EXDlg::CheckMes(int check)
{
	CString str;
	CStdioFile   file;
	CString filename;
	str = "";
	int n=0;
	filename.Format(_T("c:\\Glim\\Mes.glm"));

	if(check==1)//Read
	{
		if(file.Open(filename, CFile::modeReadWrite|CFile::typeText)) 
		{	
			file.ReadString(str);
			n = atoi(str);
			file.Close();
		}
	}
	else
	{
		if(n) str = "1";
		else str = "0";

		if(file.Open(filename, CFile::modeCreate|CFile::modeReadWrite|CFile::typeText))
		{	
			file.WriteString(str);
			file.Close();
		}
	}

	return n;
}

void CFlyCapture2_EXDlg::SetNumLock( BOOL bState )
{
	BYTE keyState[256];

	GetKeyboardState((LPBYTE)&keyState);
	if( (bState && !(keyState[VK_CAPITAL] & 1)) ||
		(!bState && (keyState[VK_CAPITAL] & 1)) )
	{
		// Simulate a key press
		keybd_event( VK_CAPITAL,	0x45,	KEYEVENTF_EXTENDEDKEY | 0,	0 );

		Sleep(10);
		// Simulate a key release
		keybd_event( VK_CAPITAL,	0x45,	KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP,0);
	}
}

void CFlyCapture2_EXDlg::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	m_nZoomSize *=2;
	if(m_nZoomSize >8)
		m_nZoomSize = 1;

	short zDelta = m_nZoomSize;
	if(m_nZoomSize==1)
		zDelta = 0;
	CgDisplay* pDisplay = GetScreen();

	pDisplay->SetZoom(zDelta,point);

//	pDisplay->SetZoomClick(m_nZoomSize,point);

	DrawDisplay2(0);
	CDialog::OnLButtonDblClk(nFlags, point);
}

CString CFlyCapture2_EXDlg::ComputerName()
{
	TCHAR szComputerName[MAX_COMPUTERNAME_LENGTH + 1] = "\0" ;
	DWORD dwSize = MAX_COMPUTERNAME_LENGTH + 1 ;
	GetComputerName( szComputerName, &dwSize ) ;
 
	CString str;
	str.Format("%s", &szComputerName);
	
	//if(str.Find("CH")>=0 || str.Find("HO")>=0 || str.Find("WOO")>=0 || str.Find("OH") >= 0 || str.Find("DESKTOP") >= 0 )
	//{
	//	// Nanjing1 : CSW1, Nanjing2 : CSW1/2, Nanjing3~6 : Nanjing3A/3B/3C, 
	//	str = "Nanjing7B";
	//	m_bTestMode = TRUE;
	//}

	return str;
}

CString CFlyCapture2_EXDlg::ComputerUserName()
{
	TCHAR szComputerUserName[MAX_COMPUTERNAME_LENGTH + 1] = "\0" ;
	DWORD dwSize = MAX_COMPUTERNAME_LENGTH + 1 ;
	GetUserName( szComputerUserName, &dwSize);

	CString str;
	str.Format("%s", &szComputerUserName);

	return str;
}

void CFlyCapture2_EXDlg::fnReset()
{
	//변수
	for(int i=0;i<10;i++)m_nBadCount[i] = m_Project.m_dInspectTime[i]=0;

	for (int i = 0; i < 4; i++)
		m_nDLBadCount[i] = 0;

	m_nCanCheckAve  = 0;//캔유무  밝기값

	m_Project.m_nFrameCount = 0;// 검사개수
	m_nGrabCount = 0;

	m_nWeakWeldTotalCnt = 0;
	m_nWeakWeldNgCnt = 0;


	///project
	for(int i =0;i<CAMERA_HEIGHT;i++)
	{
		m_Project.m_ptDrawCapEdge[0][i].x = m_Project.m_ptDrawCapEdge[1][i].x =m_Project.m_ptDrawCapEdge[0][i].y = m_Project.m_ptDrawCapEdge[1][i].y = 0;//에지포인트
	}
	m_Project.m_ptCapCenter.x=m_Project.m_ptCapCenter.y = 0;//탑캡센터
	m_Project.m_nBottomEdge[0]=m_Project.m_nBottomEdge[1]=0;
	m_Project.m_nTopEdge[0]=m_Project.m_nTopEdge[1] = 0;
	m_Project.m_nEdgeDist[0] = m_Project.m_nEdgeDist[1] = 0;

	m_Project.m_nStdLine1 = m_Project.m_nStdLine2=0;
	m_Project.m_rtHoleRect.SetRect(0,0,0,0);

	m_dTactTime = 0;
	m_nOKCount = m_nNgCount = 0;
	m_nCSVCount=0;

	ViewData();
	BadNgCount(0, RET_OK, RET_OK);

	//imagw
	for(int i=0;i<5;i++)
	{
		memset(m_DisplayBad[1].m_pImg->GetImagePtr(),0,300*192);//200*128);
		m_strBadInfo[i][0] =m_strBadInfo[i][1] = "";
		m_strBadInfo1[0]=m_strBadInfo1[1] = "";
		m_LabelNgTime[i].SetText("");
		m_LabelBadName[i].SetText("");
	}

	for(int i=0;i<24;i++)
		m_dTimeRate[i] = 0;


	//////////check
	m_fFps = 0;
	m_Project.ResetFrame();
	m_nSendIO = 0;
	m_nOKNG[0]=m_nOKNG[1] = -1;

	m_dTactTimeAve[0]=m_dTactTimeAve[1]=m_dTactTimeAve[2] = 0;
		//control Reset
	m_Project.m_dBrightness = 0;
		//Label 초기화
	LabelResult(-1, AI_READY);

	m_strModelName = "";
	m_nNewCount = 0;
	m_Project.m_nNgCount=m_Project.m_nFrameCount = 0;
	m_Project.m_dInsTime = 0;
	m_nSaveCount = 0;
	m_strIsCode = "";
	m_nNg1Count=m_nNg2Count = 0;
	m_nNgCount = 0;
	m_nOKCount = 0;
	m_nCSVCount =0;
	m_dYield = 0;
	m_dTactTime = 0;


	return;


}

void CFlyCapture2_EXDlg::fnWriteEndFile()
{
	//설 비 ID (호기 구분자)	제품군	품 명	제품LOT	검사수량	시작시간	종료시간	작업시간(분)	TACT(매/초)	양품매수	NG발생매수	보류발생매수
	CStdioFile   file;
	CString filename;
	//strFile.Format("%s\\%s.csv","c:\\Glim\\Log\\LotData\\",strDate);
	CTime ttime = CTime::GetCurrentTime();
	CString kk = ttime.Format("%y%m%d");
	CString kk1 = ttime.Format("%Y%m");
	CString strpath("d:\\Data\\LotEnd\\");
	CreateDirectory(strpath,NULL);

	CString strheader,str;
	CString strstime = m_strInspStartTime;
	CString strETime = m_strInspEndTime;

	filename.Format(_T("%s%s.end"),strpath,m_strIsCode);//m_strModelName);
	//d:\\Data\\Image\\
//년월	일	m_strModelName	m_strInspStartTime
	if(!file.Open(filename, CFile::modeReadWrite|CFile::typeText)) 
	{		
		if(file.Open(filename, CFile::modeCreate|CFile::modeReadWrite|CFile::typeText))
		{
			for (int i=0;i<5;i++)
			{
				if(m_DisplaySave[0].m_strImagePath[i] == "")		break;
				file.WriteString(m_DisplaySave[0].m_strImagePath[i]);	
				file.WriteString(_T("\n"));
			}
			file.Close();
		}
	}
	else
	{
		file.SeekToEnd();
		file.WriteString(_T("\n"));
		file.WriteString(str);
		file.Close(); 
	}
}

void CFlyCapture2_EXDlg::fnWriteMesendFile(CString strendtime,CString striscode,CString strnewlot,int framecnt,int nngcount,int mescount,CString strtype)
{
	CStdioFile   file;
	CString filename;
	//strFile.Format("%s\\%s.csv","c:\\Glim\\Log\\LotData\\",strDate);
	CTime ttime = CTime::GetCurrentTime();
	CString kk = ttime.Format("%y%m%d");
	CString kk1 = ttime.Format("%Y%m");
	CString strpath("c:\\Glim\\MesEnd\\");
	CreateDirectory(strpath,NULL);

	CString strheader,str;

	filename.Format(_T("%sLastMes.end"),strpath);//m_strModelName);
	//d:\\Data\\Image\\
	//년월	일	m_strModelName	m_strInspStartTime
	
	if(file.Open(filename, CFile::modeCreate|CFile::modeReadWrite|CFile::typeText))
	{
		file.WriteString(strendtime);
		file.WriteString(_T("\n"));

		file.WriteString(striscode);
		file.WriteString(_T("\n"));

		file.WriteString(strnewlot);
		file.WriteString(_T("\n"));

		str.Format("%d",framecnt);
		file.WriteString(str);
		file.WriteString(_T("\n"));

		str.Format("%d",nngcount);
		file.WriteString(str);
		file.WriteString(_T("\n"));

		str.Format("%d",mescount);
		file.WriteString(str);
		file.WriteString(_T("\n"));

		file.WriteString(strtype);
		file.WriteString(_T("\n"));

		file.Close();
	}
}

void CFlyCapture2_EXDlg::HddAlarm(int n)
{
	CString str;
	if(n==1)//ok
	{
		m_LabelRemainHdd1.SetTextColor(COLOR_WHITE);
		m_LabelRemainHdd1.SetBkColor(RGB(80,100,115));
		m_LabelRemainHdd1.SetFontBold(TRUE);
		m_LabelRemainHdd1.SetFontName(_T("굴림"));
		m_LabelRemainHdd1.SetFontSize(16);
		m_LabelRemainHdd1.SetText(" HDD 2:");
		str.Format(" HDD 2: %.1f%%",m_dRemainHDDSpace2);
		m_LabelRemainHdd1.SetText(str);

	}
	else//ng
	{
		m_LabelRemainHdd1.SetTextColor(COLOR_BLUE);
		m_LabelRemainHdd1.SetBkColor(COLOR_RED);
		m_LabelRemainHdd1.SetFontBold(TRUE);
		m_LabelRemainHdd1.SetFontName(_T("굴림"));
		m_LabelRemainHdd1.SetFontSize(16);
		m_LabelRemainHdd1.SetText(" HDD 2:");
		str.Format(" HDD 2: %.1f%%",m_dRemainHDDSpace2);
		m_LabelRemainHdd1.SetText(str);
	}
}

CString CFlyCapture2_EXDlg::GetPgmVersion()
{
	// 확인 필요사항!--> C:\Program Files\Microsoft Visual Studio\VC98\Lib 의
	// version.lib 를 프로젝트에 포함해야 한다.
	BYTE       *block;
	DWORD FAR  *translation;
	DWORD FAR  *buffer;
	DWORD        handle;
	UINT        bytes;
	TCHAR           *lpszFileName  = _T("c:\\Glim\\VisWeb.exe");
	TCHAR        name[512];
	TCHAR        data[256];
	CString        szString;

	//// 버전 정보의 실제 크기를 가져온다.
	//bytes = (UINT)::GetFileVersionInfoSize( lpszFileName, &handle);

	//if(bytes) {
	//	block = new BYTE[bytes];

	//	// 버전 정보에 대한 실제 블록을 가져온다.
	//	if( ::GetFileVersionInfo( lpszFileName, handle, bytes, block) ) {
	//		if( ::VerQueryValue( block, _T("\\VarFileInfo\\Translation"), (LPVOID *)&translation,
	//			(UINT FAR *)&bytes) ) {

	//				// 버전 정보
	//				wsprintf(name, _T("\\StringFileInfo\\%04x%04x\\FileVersion"), 
	//					LOWORD(*translation), HIWORD(*translation) );

	//				if( ::VerQueryValue(block, name, (LPVOID *)&buffer, (UINT FAR *)&bytes) ) {
	//					szString.Format(_T("Version %s"), (LPCTSTR)buffer );
	//					//				MessageBox(szString);           //여기서 버전을 얻어온다. 
	//					delete[] block;
	//					return szString;

	//				}
	//				else
	//					szString.Format(_T("Unable to get Version Info"), data);

	//				// Product Name 정보
	//				wsprintf(name, _T("\\StringFileInfo\\%04x%04x\\ProductName"), LOWORD(*translation),          HIWORD(*translation) );

	//				if( ::VerQueryValue(block, name, (LPVOID *)&buffer, (UINT FAR *)&bytes) ) {
	//					szString.Format( _T("%s"), (LPCTSTR)buffer );
	//				}
	//				else
	//					szString.Format( _T("Unable to get Product Name") );
	//		}
	//		else 
	//			szString.Format( _T("(Unable to get translation type)") );
	//	}
	//	else 
	//		szString.Format( _T("(Unable to get FileVersionInfo)") );

	//	delete [] block;
	//}
	//else {
	//	szString.Format( _T("(Unable to get FileVersionInfo size)") );
	//}

	return szString;
}

void CFlyCapture2_EXDlg::WriteFolderData(int nCam,CString path)
{
	CStdioFile   file;
	CString filename;
	CString strpath("c:\\Data\\");
	CreateDirectory(strpath,NULL);
	CTime ttime = CTime::GetCurrentTime();
	CString kk = ttime.Format("%Y%m%d");
	CString kk2 = ttime.Format("%Y%m");
	CString kk3 = ttime.Format("%d");
	CString kk1 = ttime.Format("%Y%m%d_%H%M%S");
	CString kk11 = ttime.Format("%y");
	CString kk12 = ttime.Format("%m");
	CString kk13 = ttime.Format("%d");
	CString kk14 = ttime.Format("%H");
	CString kk15 = ttime.Format("%M");
	CString kk16 = ttime.Format("%S");

	CString kk17 = kk11+"년"+kk12+"월"+kk13+"일"+ kk14+"시"+ kk15+"분"+ kk16+"초";

	CString strheader,str;
	int n1 = m_nOKCount;
	int n2 = m_DlgBarcode->m_nMakeCount+1;

	CreateDirectory(strpath,NULL);
	strpath.Format("c:\\Data\\Log\\");
	CreateDirectory(strpath,NULL);
	strpath.Format("c:\\Data\\Log\\%s\\",kk2);//년월
	CreateDirectory(strpath,NULL);
	strpath.Format("c:\\Data\\Log\\%s\\%s\\",kk2,kk3);//day
	CreateDirectory(strpath,NULL);
	strpath.Format("c:\\Data\\Log\\%s\\%s\\%s_LotData_%d.txt",kk2,kk3,m_strIsCode,n1/n2);//m_strLot);//m_nOKCount/pView->m_DlgBarcode->m_nMakeCount);

	filename.Format(_T("%s"),strpath);
	strheader.Format("Date\tNo\tCam\tCount\tAve\tSizeX\tSizeY\tV1\tV2\tRatio\tPath");

	str.Format("%s\t%d\tCam%d\t%d\t%.1f\t%.2f\t%.2f\t%d\t%d\t%.1f\t%s"
		,kk1,m_nOKCount+m_nNgCount,nCam+1,m_Project.m_nBlobCnttest[nCam],m_Project.m_dBadAve[0],m_Project.m_dSizeX[0],m_Project.m_dSizeY[0], m_Project.m_nMinV[0],m_Project.m_nMaxV[0],m_Project.m_dRatio[0],path);

	if(!file.Open(filename, CFile::modeReadWrite|CFile::typeText)) 
	{		
		if(file.Open(filename, CFile::modeCreate|CFile::modeReadWrite|CFile::typeText))
		{
			file.WriteString(strheader);	
			file.WriteString(_T("\n"));
			file.WriteString(str);	
			file.Close();
		}
	}
	else
	{
		file.SeekToEnd();
		file.WriteString(_T("\n"));
		file.WriteString(str);
		file.Close(); 
	}

}

void CFlyCapture2_EXDlg::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	//if(m_pDlgSet->m_nTestMode==1)
	//{
	//	m_pDlgSet->m_ev_rbutton_down.SetEvent();
	//}
	m_pDlgSet->m_FormTab3->m_ev_rbutton_down.SetEvent();
	CDialog::OnRButtonDown(nFlags, point);
}

int CFlyCapture2_EXDlg::fnSetPort() 
{
	int i;
	int ret = 1,nport;
	m_nPort = 9;
	CString str;

	//for(i=m_nPort;i<m_nPort+1;i++)
	//{
	//	m_cComm[i-m_nPort].Close();

	//	nport = i+1;
	//	m_cComm[i-m_nPort].SetPortSetting(CBR_19200,8,ONESTOPBIT,NOPARITY);		//통신설정
	//	if(!m_cComm[i-m_nPort].Open(nport)){
	//		ret = 0;
	//		AddtoList("조명연결 NG",1);
	//	}
	//	else
	//	{
	//		i = i;
	//		AddtoList("조명연결 OK",1);
	//	}
	//	SetCurrentData(i-m_nPort,'S','A','0');
	//	Sleep(500);
	//	SetCurrentData(i-m_nPort,'S','A','1');
	//	SetCurrentData(i-m_nPort,'S','A','0');
	//	Sleep(500);


	//}
	//	Lamp = (char)m_CurrentElectric;
	return ret;
}

void CFlyCapture2_EXDlg::SetCurrentData(int nport,char cCommand,char cChannel,char data ) 
{
	//m_cComm[nport].ClearRXBuffer();
	//	m_Electric[nport-DEFAULTPORT] = ReadLightData('A',nport);					//전류
	//	m_Voltage[nport-DEFAULTPORT] = ReadLightData('V',nport);						//전압 
	//	m_CurrentElectric[nport-DEFAULTPORT] = ReadLightData('P',nport)*10;			//현재설정값 
	//m_Electric[nport] = ReadLightData('0',nport);					//전류
	//m_Voltage[nport] = ReadLightData('1',nport);						//전압 
	//m_CurrentElectric[nport] = ReadLightData('P',nport)*10;			//현재설정값 
	//COMMAND (문자형)
	//	W---Write,  S---Switch,  R---Read
	//	CH.NO.(문자형)
	//	1---Ch.No 1,  2---Ch.No 2,  3---Ch.No 3,  4---Ch.No 4,  A---모든 채널
	//	DATA(문자 또는 십진수)
	//	COMMAND 가 W 일때 0-100 까지의 십진수.---최대 전류 값의 %
	//	COMMAND 가 S 일때 0,1 의 문자.--- 0-off,  1-on
	//	COMMAND 가 R 일때 A,V의 문자.--- A-ampere,  V-voltage

	SetLightData(cCommand,cChannel,data,nport);
}


double CFlyCapture2_EXDlg::SetLightData(char cCommand,char cChannel,char data,int nport)
{//STX___”W”___”1”___(50)___ETX 
	BYTE temp = 0;
	double value = 0;
	char pCommand[5];
	pCommand[0] = 0x02;		// STX
	pCommand[1] = cCommand;//'R';		// Read명령 //	W---Write,  S---Switch,  R---Read
	pCommand[2] = cChannel;
	pCommand[3] = data;		// 'A' - 전류값
	pCommand[4] = 0x03;		// ETX

	//m_cComm[nport].Write(pCommand,5);

	//char pRead[4];
	//pRead[0] = 0x00;		// STX
	//pRead[1] = 0x00;		// Read명령
	//pRead[2] = 0x00;		// 'A' - 전류값
	//pRead[3] = 0x00;		// ETX
	//m_cComm[nport].Read(pRead,4);
	//temp = pRead[2];
	//value = (double)temp/10;
	return 0;
}

double CFlyCapture2_EXDlg::SetLightLevel(char cChannel,int value)
{//STX___”W”___”1”___(50)___ETX 
	BYTE temp = 0;
	char pCommand[5];
	char pdata[3];
	pCommand[0] = 0x02;		// STX
	pCommand[1] = 'W';//'R';		// Read명령 //	W---Write,  S---Switch,  R---Read
	pCommand[2] = cChannel;
	sprintf(pdata,"%c",value);
	pCommand[3] = pdata[0];
	pCommand[4] = 0x03;		// ETX
	//m_cComm[0].Write(pCommand,5);
	
	return 0;
}

BOOL CFlyCapture2_EXDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CgDisplay* pDisplay = GetScreen();

	if(pWnd==this&&pDisplay->TrackerSetCursor(pWnd,nHitTest))

	return CDialog::OnSetCursor(pWnd, nHitTest, message);
}

BOOL CFlyCapture2_EXDlg::DestroyWindow()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	int i;
	m_pDlgSet->KillTimer(1545);
	m_pDlgSet->m_FormTab4->KillTimer(2001);//live

	if(m_bCamStats[0])
		m_Vision[0]->Stop();
	Sleep(500);
	for(int i=0;i<MAX_CAMERAS;i++)
	{
		if(m_bCamStats[i]==TRUE)
			m_Vision[i]->Stop();
		if(m_Vision[i])delete m_Vision[i];
	}

	m_pDlgSet->m_FormTab3->OnBnClickedButtonLightTurnOffCh1();
	m_pDlgSet->m_FormTab3->OnBnClickedButtonLightTurnOffCh2();

	m_bRun = FALSE;
	m_IO.Destroy();
	Sleep(100);
	//m_cComm[0].Close();
	//for(int i=0;i<12;i++)delete m_cComm[i];

//	excel.excel.ReleaseExcel();
	m_Project.Destroy();
	if(m_pDlgSet)		delete m_pDlgSet;
	if(m_DlgBarcode)	delete m_DlgBarcode;
	if(m_LightMap) delete m_LightMap;
	if(m_CamDisplay) delete m_CamDisplay;
	if(m_nModelCount)	delete m_nModelCount;
//	if(m_pDlgPassword)	delete m_pDlgPassword;
//	if(mDrawBadState) delete mDrawBadState;
	ExitProcess(0);

	return CDialog::DestroyWindow	();
}

int CFlyCapture2_EXDlg::CycleGrab()
{
	int nRet = 0;
	//영상 촬영 및 검사 영역
	m_CamDisplay->ShowWindow(SW_HIDE);


	CString strLog;
	strLog.Format("Grab Start",m_Project.m_nFrameCount);
	WriteDetailLog(strLog);

	m_rtInsp;
	return nRet;
}

void CFlyCapture2_EXDlg::WriteDetailLog(CString strdata)
{
	CStdioFile   file;
	CString filename;
	CTime ttime = CTime::GetCurrentTime();
	CString kk2 = ttime.Format("%Y%m");
	CString kk3 = ttime.Format("%d");
	CString kk1 = ttime.Format("[%Y%m%d_%H%M%S]");

	CString str,str1;
	str.Format("c:\\Data\\");
	CreateDirectory(str,NULL);
	str.Format("c:\\Data\\Image\\");
	CreateDirectory(str,NULL);
	str.Format("c:\\Data\\Image\\%s\\",kk2);//년월
	CreateDirectory(str,NULL);
	str.Format("c:\\Data\\Image\\%s\\%s\\",kk2,kk3);//일
	CreateDirectory(str,NULL);
	str.Format("c:\\Data\\Image\\%s\\%s\\%s_%s\\",kk2,kk3,m_Display[0].m_strTimePath,m_Display[0].m_strMesLot);//년월/일/시간
	CreateDirectory(str,NULL);


	str1.Format("%s %s",kk1,strdata);

	filename.Format(_T("%s%s.txt"),str,m_strLot);

	if(!file.Open(filename, CFile::modeReadWrite|CFile::typeText)) 
	{		
		if(file.Open(filename, CFile::modeCreate|CFile::modeReadWrite|CFile::typeText))
		{
			file.WriteString(str1);	
			file.Close();
		}
	}
	else
	{
		file.SeekToEnd();
		file.WriteString(_T("\n"));
		file.WriteString(str1);
		file.Close(); 
	}
}

void CFlyCapture2_EXDlg::AlarmView(int count)
{
	CString str;

}

void CFlyCapture2_EXDlg::AlarmMode(int count)
{
	CString str;

}


//투과안됨  -> 60000
//투과		-> 30000


void CFlyCapture2_EXDlg::capture2()
{
	if(m_dRemainHDDSpace1 <1) 
		return;
	if(m_dRemainHDDSpace2 <1) 
		return;
	CClientDC  dc(this); //dc생성
	DWORD d1 = GetTickCount();
	GetDlgItem(IDC_DRAW_VIEW)->GetWindowRect(m_rtSaveRect);
	CRect rc = m_rtSaveRect;//(0,0,1280,1024); 
	rc.top-=234; // 캠영역 960 -> 660으로 변경, 상단 300에 데이터 디스플레이 2개 다 전체캡쳐
	//전체화면 구역설정
	//m_rtSaveRect//
	CBitmap  captureBmp; 
	captureBmp.CreateCompatibleBitmap(&dc,  rc.Width(),  rc.Height()); 

	CDC  dcMem; 
	dcMem.CreateCompatibleDC(&dc); 
	CBitmap  *pOldBitmap  =  dcMem.SelectObject(&captureBmp); 

	//  복사한다. 
	//     dcMem.BitBlt(0,0,rc.Width(),  rc.Height(),  &dc,  0,0,SRCCOPY); 
	dcMem.BitBlt(0,0,rc.Width(),  rc.Height(),  &dc,  rc.left,rc.top,SRCCOPY); 
	dcMem.SelectObject(pOldBitmap); 

	CString str,strpath;
	CString strRtosIP= "194.195.196.70";
	CTime ttime = CTime::GetCurrentTime();
	CString kk = ttime.Format("%Y%m%d");
	CString kk10 = ttime.Format("%Y");
	CString kk11 = ttime.Format("%m");
	CString kk2 = ttime.Format("%Y%m");
	CString kk3 = ttime.Format("%d");
	CString kk1 = ttime.Format("%y%m%d_%H%M%S");
	CString kk4 = ttime.Format("%H%M%S");
	CString kk5 = ttime.Format("%H");

	//if(m_strTimePath == "")
	//	m_strTimePath.Format("C-%s",kk4);
	//년월	일	m_strModelName	m_strInspStartTime

	int nn=0;
	if(m_strComname.Find("CH")>=0)
		nn = 1;

	CString strnas;
	if(m_pDlgSet->m_FormTab2->m_bUseRtos){

		int nCom=0;
		if(m_strComname.Find("A")>=0)
			nCom=1;
		else if(m_strComname.Find("B")>0)
			nCom=2;
		else
			nCom=3;
		strpath.Format(_T("\\\\%s\\DATA\\CSW\\"),strRtosIP);
		CreateDirectory(strpath,NULL);
		strpath.Format(_T("\\\\%s\\DATA\\CSW\\CSW_WELD%d\\"),strRtosIP,nCom);
		CreateDirectory(strpath,NULL);
		strpath.Format(_T("\\\\%s\\DATA\\CSW\\CSW_WELD%d\\%s\\"),strRtosIP,nCom,kk10);
		CreateDirectory(strpath,NULL);
		strpath.Format(_T("\\\\%s\\DATA\\CSW\\CSW_WELD%d\\%s\\%s\\"),strRtosIP,nCom,kk10,kk11);
		CreateDirectory(strpath,NULL);
		strpath.Format(_T("\\\\%s\\DATA\\CSW\\CSW_WELD%d\\%s\\%s\\%s\\"),strRtosIP,nCom,kk10,kk11,kk3);
		CreateDirectory(strpath,NULL);
		strpath.Format(_T("\\\\%s\\DATA\\CSW\\CSW_WELD%d\\%s\\%s\\%s\\%s\\"),strRtosIP,nCom,kk10,kk11,kk3,kk5);
		CreateDirectory(strpath,NULL);
		strpath.Format(_T("\\\\%s\\DATA\\CSW\\CSW_WELD%d\\%s\\%s\\%s\\%s\\Overlay\\"),strRtosIP,nCom,kk10,kk11,kk3,kk5);
		CreateDirectory(strpath,NULL);

		if(m_nRBResult==RET_OK)
			strpath.Format("\\\\%s\\DATA\\CSW\\CSW_WELD%d\\%s\\%s\\%s\\%s\\Overlay\\%s_%010d.jpg",strRtosIP,nCom,kk10,kk11,kk3,kk5,kk1,m_nOKCount+m_nNgCount);//time
		else{
			strpath.Format("\\\\%s\\DATA\\CSW\\CSW_WELD%d\\%s\\%s\\%s\\%s\\Overlay\\NG\\",strRtosIP,nCom,kk10,kk11,kk3,kk5);//time
			CreateDirectory(strpath,NULL);

			strpath.Format("\\\\%s\\DATA\\CSW\\CSW_WELD%d\\%s\\%s\\%s\\%s\\Overlay\\NG\\%s_%010d.jpg",strRtosIP,nCom,kk10,kk11,kk3,kk5,kk1,m_nOKCount+m_nNgCount);//time
		}
	}
	ImageFormatPtr img = ImageFormat::create();
	//	img->loadAndSaveAs((HBITMAP)captureBmp, 0, L"jpg", L"c:\\test.jpg");

	wchar_t* pStr;
	//멀티 바이트 크기 계산 길이 반환
	int strSize = MultiByteToWideChar(CP_ACP, 0,strpath, -1, NULL, NULL);

	//wchar_t 메모리 할당
	pStr = new WCHAR[strSize];
	//형 변환
	MultiByteToWideChar(CP_ACP, 0,strpath, strlen(strpath)+1, pStr, strSize);

	img->loadAndSaveAs((HBITMAP)captureBmp, 0, L"jpg",pStr);
	delete pStr;

	////////////////////////////////////////////
	// if(hDIB != NULL)
	//{
	//WriteDIB(str, hDIB);
	//		CloseHandle(hDIB);
	//GlobalFree(hDIB);
	//}
	//	 m_bIsSaving = TRUE;
	DeleteObject(dcMem);
	captureBmp.DeleteObject();
}

void CFlyCapture2_EXDlg::capture()
{
	if(m_dRemainHDDSpace1 <1) 
		return;
	if(m_dRemainHDDSpace2 <1) 
		return;
	CClientDC  dc(this); //dc생성
	//     CRect rc(0,0,GetSystemMetrics(SM_CXMAXTRACK),GetSystemMetrics(SM_CYMAXTRACK)); 
	//     CRect rc(0,0,FORM_IMG_WIDTH,FORM_IMG_HEIGHT); 
	//	 	m_rtCirSearch = m_pTracker[5]->m_rect;
	//	m_rtSaveRect = m_pTracker[5]->m_rect;//.InflateRect(600,600);
	//	m_rtSaveRect.InflateRect(100,100);
	DWORD d1 = GetTickCount();
	GetDlgItem(IDC_DRAW_VIEW)->GetWindowRect(m_rtSaveRect);

	//	m_rtSaveRect.SetRect(0,0,1280,1024);

	CRect rc = m_rtSaveRect;//(0,0,1280,1024); 
	rc.top-=234; // 캠영역 960 -> 660으로 변경, 상단 300에 데이터 디스플레이 2개 다 전체캡쳐
	//전체화면 구역설정
	//m_rtSaveRect//
	CBitmap  captureBmp; 
	captureBmp.CreateCompatibleBitmap(&dc,  rc.Width(),  rc.Height()); 

	CDC  dcMem; 
	dcMem.CreateCompatibleDC(&dc); 
	CBitmap  *pOldBitmap  =  dcMem.SelectObject(&captureBmp); 

	//  복사한다. 
	//     dcMem.BitBlt(0,0,rc.Width(),  rc.Height(),  &dc,  0,0,SRCCOPY); 
	dcMem.BitBlt(0,0,rc.Width(),  rc.Height(),  &dc,  rc.left,rc.top,SRCCOPY); 
	dcMem.SelectObject(pOldBitmap); 

	//    HANDLE hDIB = DDBToDIB(captureBmp, BI_RGB, NULL); // 이 함수들은 여기! bu.cpp//BI_JPEG
	CString str;//,strpath;
	CTime ttime = CTime::GetCurrentTime();
	CString kk = ttime.Format("%Y%m%d");
	CString kk10 = ttime.Format("%Y");
	CString kk11 = ttime.Format("%m");
	CString kk2 = ttime.Format("%Y%m");
	CString kk3 = ttime.Format("%d");
	CString kk1 = ttime.Format("%y%m%d_%H%M%S");
	CString kk4 = ttime.Format("%H%M%S");
	CString kk5 = ttime.Format("%H");
	CString strMin = ttime.Format("%M");
	CString strResult;
	int nMin = atoi(strMin)/10;

	str.Format("d:\\Data\\");
	CreateDirectory(str,NULL);
	str.Format("d:\\Data\\Image\\");
	CreateDirectory(str,NULL);
	str.Format("d:\\Data\\Image\\%s\\",kk10);//년
	CreateDirectory(str,NULL);
	str.Format("d:\\Data\\Image\\%s\\%s\\",kk10, kk11);//월
	CreateDirectory(str,NULL);
	str.Format("d:\\Data\\Image\\%s\\%s\\Overlay\\",kk10, kk11);
	CreateDirectory(str,NULL);

	if(m_bFinalRet == FINAL_RET_OK){
		str.Format("d:\\Data\\Image\\%s\\%s\\Overlay\\OK\\",kk10, kk11); //ok or ng
		CreateDirectory(str,NULL);
		str.Format("d:\\Data\\Image\\%s\\%s\\Overlay\\OK\\%s\\",kk10, kk11, kk3); //일
		CreateDirectory(str,NULL);
		str.Format("d:\\Data\\Image\\%s\\%s\\Overlay\\OK\\%s\\%s\\",kk10, kk11, kk3, kk5); //시
		CreateDirectory(str,NULL);
		str.Format("d:\\Data\\Image\\%s\\%s\\Overlay\\OK\\%s\\%s\\%s_%d.jpg",kk10, kk11, kk3, kk5, kk1, m_nOKCount+m_nNgCount); //시
	}
	else{
		str.Format("d:\\Data\\Image\\%s\\%s\\Overlay\\NG\\",kk10, kk11); //ok or ng
		CreateDirectory(str,NULL);
		str.Format("d:\\Data\\Image\\%s\\%s\\Overlay\\NG\\%s\\",kk10, kk11, kk3); //일
		CreateDirectory(str,NULL);
		str.Format("d:\\Data\\Image\\%s\\%s\\Overlay\\NG\\%s\\%s_%d.jpg",kk10, kk11, kk3, kk1, m_nOKCount+m_nNgCount); //일
	}
	

	ImageFormatPtr img = ImageFormat::create();
	//	img->loadAndSaveAs((HBITMAP)captureBmp, 0, L"jpg", L"c:\\test.jpg");

	wchar_t* pStr;
	//멀티 바이트 크기 계산 길이 반환
	int strSize = MultiByteToWideChar(CP_ACP, 0,str, -1, NULL, NULL);

	//wchar_t 메모리 할당
	pStr = new WCHAR[strSize];
	//형 변환
	MultiByteToWideChar(CP_ACP, 0,str, strlen(str)+1, pStr, strSize);

	img->loadAndSaveAs((HBITMAP)captureBmp, 0, L"jpg",pStr);
	delete pStr;

	DeleteObject(dcMem);
	captureBmp.DeleteObject();
	
}

HANDLE CFlyCapture2_EXDlg::DDBToDIB(CBitmap &bitmap, DWORD dwCompression, CPalette *pPal)
{
	BITMAP   bm;
	BITMAPINFOHEADER bi;
	LPBITMAPINFOHEADER  lpbi;
	DWORD   dwLen;
	HANDLE   hDIB;
	HANDLE   handle;
	HDC    hDC;
	HPALETTE  hPal;


	ASSERT( bitmap.GetSafeHandle() );

	// The function has no arg for bitfields
	if( dwCompression == BI_BITFIELDS )
		return NULL;

	// If a palette has not been supplied use defaul palette
	hPal = (HPALETTE) pPal->GetSafeHandle();
	if (hPal==NULL)
		hPal = (HPALETTE) GetStockObject(DEFAULT_PALETTE);

	// Get bitmap information
	bitmap.GetObject(sizeof(bm),(LPSTR)&bm);

	// Initialize the bitmapinfoheader
	bi.biSize  = sizeof(BITMAPINFOHEADER);
	bi.biWidth  = bm.bmWidth;
	bi.biHeight   = bm.bmHeight;
	bi.biPlanes   = 1;
	bi.biBitCount  = bm.bmPlanes * bm.bmBitsPixel;
	bi.biCompression = dwCompression;
	bi.biSizeImage  = 0;
	bi.biXPelsPerMeter = 0;
	bi.biYPelsPerMeter = 0;
	bi.biClrUsed  = 0;
	bi.biClrImportant = 0;

	// Compute the size of the  infoheader and the color table
	int nColors = (1 << bi.biBitCount);
	if( nColors > 256 )
		nColors = 0;
	dwLen  = bi.biSize + nColors * sizeof(RGBQUAD);

	// We need a device context to get the DIB from
	hDC = ::GetDC(NULL);
	hPal = SelectPalette(hDC,hPal,FALSE);
	RealizePalette(hDC);

	// Allocate enough memory to hold bitmapinfoheader and color table
	hDIB = GlobalAlloc(GMEM_FIXED,dwLen);

	if (!hDIB){
		SelectPalette(hDC,hPal,FALSE);
		::ReleaseDC(NULL,hDC);
		return NULL;
	}

	lpbi = (LPBITMAPINFOHEADER)hDIB;

	*lpbi = bi;

	// Call GetDIBits with a NULL lpBits param, so the device driver 
	// will calculate the biSizeImage field 
	GetDIBits(hDC, (HBITMAP)bitmap.GetSafeHandle(), 0L, (DWORD)bi.biHeight,
		(LPBYTE)NULL, (LPBITMAPINFO)lpbi, (DWORD)DIB_RGB_COLORS);

	bi = *lpbi;

	// If the driver did not fill in the biSizeImage field, then compute it
	// Each scan line of the image is aligned on a DWORD (32bit) boundary
	if (bi.biSizeImage == 0){
		bi.biSizeImage = ((((bi.biWidth * bi.biBitCount) + 31) & ~31) / 8)
			* bi.biHeight;

		// If a compression scheme is used the result may infact be larger
		// Increase the size to account for this.
		if (dwCompression != BI_RGB)
			bi.biSizeImage = (bi.biSizeImage * 3) / 2;
	}

	// Realloc the buffer so that it can hold all the bits
	dwLen += bi.biSizeImage;
	if (handle = GlobalReAlloc(hDIB, dwLen, GMEM_MOVEABLE))
		hDIB = handle;
	else{
		GlobalFree(hDIB);

		// Reselect the original palette
		SelectPalette(hDC,hPal,FALSE);
		::ReleaseDC(NULL,hDC);
		return NULL;
	}

	// Get the bitmap bits
	lpbi = (LPBITMAPINFOHEADER)hDIB;

	// FINALLY get the DIB
	BOOL bGotBits = GetDIBits( hDC, (HBITMAP)bitmap.GetSafeHandle(),
		0L,    // Start scan line
		(DWORD)bi.biHeight,  // # of scan lines
		(LPBYTE)lpbi    // address for bitmap bits
		+ (bi.biSize + nColors * sizeof(RGBQUAD)),
		(LPBITMAPINFO)lpbi,  // address of bitmapinfo
		(DWORD)DIB_RGB_COLORS);  // Use RGB for color table

	if( !bGotBits )
	{
		GlobalFree(hDIB);

		SelectPalette(hDC,hPal,FALSE);
		::ReleaseDC(NULL,hDC);
		return NULL;
	}

	SelectPalette(hDC,hPal,FALSE);
	::ReleaseDC(NULL,hDC);
	return hDIB;

}



//bool CAreaIMIDlg::WriteDIB(LPTSTR szFile, HANDLE hDIB)
bool CFlyCapture2_EXDlg::WriteDIB(CString szFile, HANDLE hDIB)
{
	BITMAPFILEHEADER    hdr;
	LPBITMAPINFOHEADER  lpbi;

	if(!hDIB)
		return FALSE;

	CFile file;

	if(!file.Open( szFile, CFile::modeWrite|CFile::modeCreate) )
		return FALSE;
	lpbi = (LPBITMAPINFOHEADER)hDIB;
	int nColors = 1 << lpbi->biBitCount;

	// Fill in the fields of the file header 
	hdr.bfType      = ((WORD) ('M' << 8) | 'B');    // is always "BM"
	hdr.bfSize      = GlobalSize (hDIB) + sizeof(hdr);
	hdr.bfReserved1 = 0;
	hdr.bfReserved2 = 0;
	hdr.bfOffBits   = (DWORD) (sizeof( hdr ) + lpbi->biSize + nColors * sizeof(RGBQUAD));

	// Write the file header 
	file.Write(&hdr, sizeof(hdr));

	// Write the DIB header and the bits 
	file.Write(lpbi, GlobalSize(hDIB));

	return TRUE;	
}

void CFlyCapture2_EXDlg::BadInfo()
{
//m_LabelNgTime1

	for (int i=0;i<5;i++)
	{
		m_LabelNgTime[i].SetText(m_strBadInfo[4-i][0]);
		m_LabelBadName[i].SetText(m_strBadInfo[4-i][1]);
	}
}

void CFlyCapture2_EXDlg::InitBadInfo()
{

	CString str;
	for(int i=0;i<5;i++)
	{
		m_LabelBadName[i].SetTextColor(COLOR_BLACK);
		m_LabelBadName[i].SetBkColor(COLOR_WHITE);
		m_LabelBadName[i].SetFontBold(TRUE);
		m_LabelBadName[i].SetFontName(_T("굴림"));
		m_LabelBadName[i].SetFontSize(14);
		str.Format(_T("BAD"));
		m_LabelBadName[i].SetText(str);

		m_LabelNgTime[i].SetTextColor(COLOR_BLACK);
		m_LabelNgTime[i].SetBkColor(COLOR_WHITE);
		m_LabelNgTime[i].SetFontBold(TRUE);
		m_LabelNgTime[i].SetFontName(_T("굴림"));
		m_LabelNgTime[i].SetFontSize(12);
		str.Format(_T("TIME"));
		m_LabelNgTime[i].SetText(str);

		m_strBadInfo[i][1] = "BAD";
		m_strBadInfo[i][0] = "Time";

	}

}

int CFlyCapture2_EXDlg::CheckMachine()
{
	CString str;
	CStdioFile   file;
	CString filename;
	str = "";
	int n=0;
	m_nMachine = 0;
	filename.Format(_T("c:\\Glim\\Machine.glm"));

	if(file.Open(filename, CFile::modeReadWrite|CFile::typeText)) 
	{	
		file.ReadString(str);
		m_nMachine = atoi(str);
		file.Close();
	}
	else
	{
		m_nMachine  = 3;
	}

	return m_nMachine;
}


void CFlyCapture2_EXDlg::WriteProfileLog(int nindex,int posx,int count,int *nprofile,CString strdata)
{
	CStdioFile   file;
	CString filename;
	CTime ttime = CTime::GetCurrentTime();
	CString kk2 = ttime.Format("%Y%m");
	CString kk3 = ttime.Format("%d");
	CString kk1 = ttime.Format("[%Y%m%d_%H%M%S]");
	CString str,str1;
	str.Format("c:\\Data\\");
	CreateDirectory(str,NULL);
	str.Format("c:\\Data\\Image\\");
	CreateDirectory(str,NULL);
	str.Format("c:\\Data\\Image\\%s\\",kk2);//년월
	CreateDirectory(str,NULL);
	str.Format("c:\\Data\\Image\\%s\\%s\\",kk2,kk3);//일
	CreateDirectory(str,NULL);

	str1.Format("%s %s",kk1,strdata);

	filename.Format(_T("%sProfile_F%d_X%d.txt"),str,nindex,posx);


	if(file.Open(filename, CFile::modeCreate|CFile::modeReadWrite|CFile::typeText))
	{
		for(int i=0;i<count;i++)
		{
			str1.Format("%d - %d",i,nprofile[i]);
			file.WriteString(str1);				file.WriteString("\n");	
		}
		file.Close();
	}

}
void CFlyCapture2_EXDlg::DisplayNgCrop(int nCam, int nNgType, CString strNgName, int nNgCountType, CPoint ptPos)
{
	CTime ttime = CTime::GetCurrentTime();
	CString kk1 = ttime.Format("%mm%dd");
	CString kk2 = ttime.Format("%H:%M:%S");
	CString kk3 = ttime.Format("%H");
	
	int nCropW = 150;
	int nCropH = 96;
	//

	m_Project.m_rtHoleRect.SetRect(ptPos.x-nCropW,ptPos.y-nCropH,ptPos.x+nCropW,ptPos.y+nCropH);
	m_strBadInfo1[1] = _T(strNgName);
	m_strBadInfo1[0].Format("%s (%d)",kk2,m_nOKCount+m_nNgCount);
	m_nBadCount[nNgCountType]++;
	ImageShiftBad();

	////

	//ptcantop.SetPoint((m_Project.m_ptCanTop[0].x+m_Project.m_ptCanTop[1].x)*0.5,(m_Project.m_ptCanTop[0].y+m_Project.m_ptCanTop[1].y)*0.5);
	//double dValue = abs(m_Project.m_ptCapCenter.x- ptcantop.x) * m_pDlgSet->m_FormTab2->m_dCamScale;
	//if(m_pDlgSet->m_FormTab4->m_bUseCapCanY == 1 &&  dValue > m_pDlgSet->m_FormTab4->m_dCapCandistX &&
	//	m_pDlgSet->m_FormTab4->m_dCapCandistX>0){
	//		nret = 0;
	//		m_Project.m_rtHoleRect.SetRect(m_Project.m_ptCapCenter.x-150,m_Project.m_ptCapCenter.y-96,m_Project.m_ptCapCenter.x+150,m_Project.m_ptCapCenter.y+96);
	//		m_strBadInfo1[1] = "Can-Cap X Dist";
	//		m_strBadInfo1[0].Format("%s (%d)",kk2,m_nOKCount+m_nNgCount);
	//		m_nBadCount[4]++;
	//		ImageShiftBad();
	//}
	//else if(m_pDlgSet->m_FormTab4->m_bUseCapCanY == 1 && (
	//	(( abs(m_Project.m_ptCapCenter.y- (m_Project.m_ptCanTop[0].y+m_Project.m_ptCanTop[1].y)*0.5)*m_pDlgSet->m_FormTab2->m_dCamScale)> m_pDlgSet->m_FormTab4->m_dCenterOffset)
	//	||(( abs(m_Project.m_ptCapCenter.y- (m_Project.m_ptCanTop[0].y+m_Project.m_ptCanTop[1].y)*0.5)*m_pDlgSet->m_FormTab2->m_dCamScale)< m_pDlgSet->m_FormTab4->m_dCenterOffset2))){
	//		nret = 0;
	//		m_Project.m_rtHoleRect.SetRect(m_Project.m_ptCapCenter.x-150,m_Project.m_ptCapCenter.y-96,m_Project.m_ptCapCenter.x+150,m_Project.m_ptCapCenter.y+96);
	//		m_strBadInfo1[1] = "Can-Cap Y Dist";
	//		m_strBadInfo1[0].Format("%s (%d)",kk2,m_nOKCount+m_nNgCount);
	//		m_nBadCount[4]++;
	//		ImageShiftBad();
	//}
	//else if (m_pDlgSet->m_FormTab4->m_bTabTopDistCap)//탭 상단에서 탑캡 중심 woo
	//{
	//}
	//else if(m_pDlgSet->m_FormTab4->m_dTopCapCenterCount <m_nTopCapCenterLowCount && m_pDlgSet->m_FormTab4->m_dTopCapCenterCount >0){
	//	nret = 0;
	//	m_nBadCount[1]++;
	//	m_Project.m_rtHoleRect.SetRect(m_Project.m_ptCapCenter.x-150,m_Project.m_ptCapCenter.y-96,m_Project.m_ptCapCenter.x+150,m_Project.m_ptCapCenter.y+96);
	//	m_strBadInfo1[1] = "Tab";
	//	m_strBadInfo1[0].Format("%s (%d)",kk2,m_nOKCount+m_nNgCount);
	//	ImageShiftBad();
	//}




	////좌우 공차 확인
	//double ddist1 = m_Project.m_nEdgeDist[0]*m_pDlgSet->m_FormTab2->m_dCamScale;
	//double ddist2 = m_Project.m_nEdgeDist[1]*m_pDlgSet->m_FormTab2->m_dCamScale;
	//m_dTabDist[0] = ddist1;m_dTabDist[1]=ddist2;
	////	if((ddist1 >= m_pDlgSet->m_FormTab4->m_dTabTol) ||(ddist2 >= m_pDlgSet->m_FormTab4->m_dTabTol) || fabs((m_Project.m_ptCapCenter.y-m_Project.m_nTopPos[0])*m_pDlgSet->m_FormTab4->m_dCamScale) > m_pDlgSet->m_FormTab4->m_dTapTolY1)
	//if((ddist1 >= m_pDlgSet->m_FormTab4->m_dTabTol) ||(ddist2 >= m_pDlgSet->m_FormTab4->m_dTabTol1) || 
	//	((m_Project.m_ptCapCenter.y-m_Project.m_nTopPos[0])*m_pDlgSet->m_FormTab2->m_dCamScale) > m_pDlgSet->m_FormTab4->m_dTapTolY1 ||((m_Project.m_nTopPos[0]-m_Project.m_ptCapCenter.y)*m_pDlgSet->m_FormTab2->m_dCamScale) > m_pDlgSet->m_FormTab4->m_dTapTolY2)
	//{
	//	if(nret==1 && m_pDlgSet->m_FormTab4->m_bPassTab == 1)
	//		m_pDlgSet->m_FormTab3->m_nTabResult = 0; 

	//	if (nret==1 && m_pDlgSet->m_FormTab4->m_bPassTab == 0)
	//	{
	//		nret = 0;
	//		m_nBadCount[1]++;
	//		m_Project.m_rtHoleRect.SetRect(m_Project.m_ptCapCenter.x-150,m_Project.m_ptCapCenter.y-96,m_Project.m_ptCapCenter.x+150,m_Project.m_ptCapCenter.y+96);
	//		m_strBadInfo1[1] = "TAB";
	//		m_strBadInfo1[0].Format("%s (%d)",kk2,m_nOKCount+m_nNgCount);
	//		ImageShiftBad();
	//	}
	//}
	//else if(m_pDlgSet->m_FormTab4->m_nCapReverseV > 0 && m_nTopCapV > m_pDlgSet->m_FormTab4->m_nCapReverseV){//부등로 조정
	//	if(nret==1){
	//		nret = 0;
	//		m_nBadCount[1]++;
	//		//m_Project.m_rtHoleRect.SetRect(m_Project.m_ptCapCenter.x-150,m_Project.m_ptCapCenter.y-96,m_Project.m_ptCapCenter.x+150,m_Project.m_ptCapCenter.y+96);
	//		m_Project.m_rtHoleRect.SetRect(CAMERA_WIDTH*0.5-150,CAMERA_HEIGHT*0.5-96,CAMERA_WIDTH*0.5+150,CAMERA_HEIGHT*0.5+96);
	//		m_strBadInfo1[1] = "CAP";
	//		m_strBadInfo1[0].Format("%s (%d)",kk2,m_nOKCount+m_nNgCount);
	//		ImageShiftBad();
	//	}
	//}
	//else if(m_pDlgSet->m_FormTab4->m_bTabCornerDist && m_pDlgSet->m_FormTab2->m_bTabReFind && m_pDlgSet->m_FormTab4->m_nTabCornerDist <= abs(m_Project.m_ptLeftTop.y-m_Project.m_ptRightTop.y)){
	//	if(nret==1){
	//		m_nBadCount[1]++;
	//		nret = 0;
	//		//m_Project.m_rtHoleRect.SetRect(m_Project.m_ptCapCenter.x-150,m_Project.m_ptCapCenter.y-96,m_Project.m_ptCapCenter.x+150,m_Project.m_ptCapCenter.y+96);
	//		m_Project.m_rtHoleRect.SetRect(CAMERA_WIDTH*0.5-150,CAMERA_HEIGHT*0.5-96,CAMERA_WIDTH*0.5+150,CAMERA_HEIGHT*0.5+96);
	//		m_strBadInfo1[1] = "TAB";
	//		m_strBadInfo1[0].Format("%s (%d)",kk2,m_nOKCount+m_nNgCount);
	//		ImageShiftBad();
	//	}
	//}

	//else if(m_Project.m_ptCapCenter.y >760){//파라미터
	//	if(nret==1){
	//		m_nBadCount[1]++;
	//		nret = 0;
	//		m_strBadInfo1[1] = "TopCap";
	//		m_Project.m_rtHoleRect.SetRect(m_Project.m_ptCapCenter.x-150,m_Project.m_ptCapCenter.y-96,m_Project.m_ptCapCenter.x+150,m_Project.m_ptCapCenter.y+96);
	//		//		m_strBadInfo1[1] = "TAB";
	//		m_strBadInfo1[0].Format("%s (%d)",kk2,m_nOKCount+m_nNgCount);
	//		ImageShiftBad();
	//	}
	//}
}

int CFlyCapture2_EXDlg::judgeTap()
{
	int nret = 1;
 	CTime ttime = CTime::GetCurrentTime();
	CString kk1 = ttime.Format("%mm%dd");
	CString kk2 = ttime.Format("%H:%M:%S");
	CString kk3 = ttime.Format("%H");
	int nn1 = atoi(kk3);
	CPoint ptcantop;
	CRect rt;

	ptcantop.SetPoint((m_Project.m_ptCanTop[0].x+m_Project.m_ptCanTop[1].x)*0.5,(m_Project.m_ptCanTop[0].y+m_Project.m_ptCanTop[1].y)*0.5);
	double dValue = abs(m_Project.m_ptCapCenter.x- ptcantop.x) * m_pDlgSet->m_FormTab2->m_dCamScale;
	if(m_pDlgSet->m_FormTab4->m_bUseCapCanY == 1 &&  dValue > m_pDlgSet->m_FormTab4->m_dCapCandistX &&
		m_pDlgSet->m_FormTab4->m_dCapCandistX>0){
			nret = 0;
			m_Project.m_rtHoleRect.SetRect(m_Project.m_ptCapCenter.x-150,m_Project.m_ptCapCenter.y-96,m_Project.m_ptCapCenter.x+150,m_Project.m_ptCapCenter.y+96);
			m_strBadInfo1[1] = "Can-Cap X Dist";
			m_strBadInfo1[0].Format("%s (%d)",kk2,m_nOKCount+m_nNgCount);
			m_nBadCount[4]++;
			ImageShiftBad();
	}
	else if(m_pDlgSet->m_FormTab4->m_bUseCapCanY == 1 && (
		(( abs(m_Project.m_ptCapCenter.y- (m_Project.m_ptCanTop[0].y+m_Project.m_ptCanTop[1].y)*0.5)*m_pDlgSet->m_FormTab2->m_dCamScale)> m_pDlgSet->m_FormTab4->m_dCenterOffset)
		||(( abs(m_Project.m_ptCapCenter.y- (m_Project.m_ptCanTop[0].y+m_Project.m_ptCanTop[1].y)*0.5)*m_pDlgSet->m_FormTab2->m_dCamScale)< m_pDlgSet->m_FormTab4->m_dCenterOffset2))){
			nret = 0;
			m_Project.m_rtHoleRect.SetRect(m_Project.m_ptCapCenter.x-150,m_Project.m_ptCapCenter.y-96,m_Project.m_ptCapCenter.x+150,m_Project.m_ptCapCenter.y+96);
			m_strBadInfo1[1] = "Can-Cap Y Dist";
			m_strBadInfo1[0].Format("%s (%d)",kk2,m_nOKCount+m_nNgCount);
			m_nBadCount[4]++;
			ImageShiftBad();
	}
	else if (m_pDlgSet->m_FormTab4->m_bTabTopDistCap)//탭 상단에서 탑캡 중심 woo
	{
	}
	//else if(m_pDlgSet->m_FormTab4->m_dTopCapCenterCount <m_nTopCapCenterLowCount && m_pDlgSet->m_FormTab4->m_dTopCapCenterCount >0){
	//	nret = 0;
	//	m_nBadCount[1]++;
	//	m_Project.m_rtHoleRect.SetRect(m_Project.m_ptCapCenter.x-150,m_Project.m_ptCapCenter.y-96,m_Project.m_ptCapCenter.x+150,m_Project.m_ptCapCenter.y+96);
	//	m_strBadInfo1[1] = "Tab";
	//	m_strBadInfo1[0].Format("%s (%d)",kk2,m_nOKCount+m_nNgCount);
	//	ImageShiftBad();
	//}




	//좌우 공차 확인
	double ddist1 = m_Project.m_nEdgeDist[0]*m_pDlgSet->m_FormTab2->m_dCamScale;
	double ddist2 = m_Project.m_nEdgeDist[1]*m_pDlgSet->m_FormTab2->m_dCamScale;
	m_dTabDist[0] = ddist1;m_dTabDist[1]=ddist2;
//	if((ddist1 >= m_pDlgSet->m_FormTab4->m_dTabTol) ||(ddist2 >= m_pDlgSet->m_FormTab4->m_dTabTol) || fabs((m_Project.m_ptCapCenter.y-m_Project.m_nTopPos[0])*m_pDlgSet->m_FormTab4->m_dCamScale) > m_pDlgSet->m_FormTab4->m_dTapTolY1)
	if((ddist1 >= m_pDlgSet->m_FormTab4->m_dTabTol) ||(ddist2 >= m_pDlgSet->m_FormTab4->m_dTabTol1) || 
		((m_Project.m_ptCapCenter.y-m_Project.m_nTopPos[0])*m_pDlgSet->m_FormTab2->m_dCamScale) > m_pDlgSet->m_FormTab4->m_dTapTolY1 ||((m_Project.m_nTopPos[0]-m_Project.m_ptCapCenter.y)*m_pDlgSet->m_FormTab2->m_dCamScale) > m_pDlgSet->m_FormTab4->m_dTapTolY2)
	{
		if(nret==1 && m_pDlgSet->m_FormTab4->m_bPassTab == 1)
			m_pDlgSet->m_FormTab3->m_nTabResult = 0; 
		
		if (nret==1 && m_pDlgSet->m_FormTab4->m_bPassTab == 0)
		{
			nret = 0;
			m_nBadCount[1]++;
			m_Project.m_rtHoleRect.SetRect(m_Project.m_ptCapCenter.x-150,m_Project.m_ptCapCenter.y-96,m_Project.m_ptCapCenter.x+150,m_Project.m_ptCapCenter.y+96);
			m_strBadInfo1[1] = "TAB";
			m_strBadInfo1[0].Format("%s (%d)",kk2,m_nOKCount+m_nNgCount);
			ImageShiftBad();
		}
	}
	else if(m_pDlgSet->m_FormTab4->m_nCapReverseV > 0 && m_nTopCapV > m_pDlgSet->m_FormTab4->m_nCapReverseV){//부등로 조정
		if(nret==1){
			nret = 0;
			m_nBadCount[1]++;
			//m_Project.m_rtHoleRect.SetRect(m_Project.m_ptCapCenter.x-150,m_Project.m_ptCapCenter.y-96,m_Project.m_ptCapCenter.x+150,m_Project.m_ptCapCenter.y+96);
			m_Project.m_rtHoleRect.SetRect(CAMERA_WIDTH*0.5-150,CAMERA_HEIGHT*0.5-96,CAMERA_WIDTH*0.5+150,CAMERA_HEIGHT*0.5+96);
			m_strBadInfo1[1] = "CAP";
			m_strBadInfo1[0].Format("%s (%d)",kk2,m_nOKCount+m_nNgCount);
			ImageShiftBad();
		}
	}
	else if(m_pDlgSet->m_FormTab4->m_bTabCornerDist && m_pDlgSet->m_FormTab2->m_bTabReFind && m_pDlgSet->m_FormTab4->m_nTabCornerDist <= abs(m_Project.m_ptLeftTop.y-m_Project.m_ptRightTop.y)){
		if(nret==1){
			m_nBadCount[1]++;
			nret = 0;
			//m_Project.m_rtHoleRect.SetRect(m_Project.m_ptCapCenter.x-150,m_Project.m_ptCapCenter.y-96,m_Project.m_ptCapCenter.x+150,m_Project.m_ptCapCenter.y+96);
			m_Project.m_rtHoleRect.SetRect(CAMERA_WIDTH*0.5-150,CAMERA_HEIGHT*0.5-96,CAMERA_WIDTH*0.5+150,CAMERA_HEIGHT*0.5+96);
			m_strBadInfo1[1] = "TAB";
			m_strBadInfo1[0].Format("%s (%d)",kk2,m_nOKCount+m_nNgCount);
			ImageShiftBad();
		}
	}

	else if(m_Project.m_ptCapCenter.y >760){//파라미터
		if(nret==1){
			m_nBadCount[1]++;
			nret = 0;
			m_strBadInfo1[1] = "TopCap";
			m_Project.m_rtHoleRect.SetRect(m_Project.m_ptCapCenter.x-150,m_Project.m_ptCapCenter.y-96,m_Project.m_ptCapCenter.x+150,m_Project.m_ptCapCenter.y+96);
			//		m_strBadInfo1[1] = "TAB";
			m_strBadInfo1[0].Format("%s (%d)",kk2,m_nOKCount+m_nNgCount);
			ImageShiftBad();
		}
	}

	//can에서 topcap 중심 판단 woo

	if (m_pDlgSet->m_FormTab3->m_ntabresult != 0)
		m_pDlgSet->m_FormTab3->m_ntabresult = nret; 

	return nret;
}

int CFlyCapture2_EXDlg::judgeHole()
{
	int nret = 1;
	CString strResult;
	CTime ttime = CTime::GetCurrentTime();
	CString kk1 = ttime.Format("%mm%dd");
	CString kk2 = ttime.Format("%H:%M:%S");
	CString kk3 = ttime.Format("%H");
	int nn1 = atoi(kk3);

	//면적의 % 확인
	m_Project.m_dBlackPer[0] = m_Project.m_nBlackCount[0]/(double)m_Project.m_nRectSize*100; // R1
	m_Project.m_dBlackPer[1] = m_Project.m_nBlackCount[1]/(double)m_Project.m_nRectSize*100; // R2
	m_Project.m_dBlackPer1[0] = m_Project.m_nBCount[0]/(double)m_Project.m_nRectSize*100; // S1
	m_Project.m_dBlackPer1[1] = m_Project.m_nBCount[1]/(double)m_Project.m_nRectSize*100; // S2

	for(int i=0;i<4;i++)	m_strResultValue[i] = "";
	CString str;
	str.Format("B1:%.0f,B2:%.0f,SB1:%.0f,SB2:%.0f",m_Project.m_dBlackPer[0],m_Project.m_dBlackPer[1],m_Project.m_dBlackPer1[0],m_Project.m_dBlackPer1[1]);
	AddtoInspLog(str);
	
	if(m_pDlgSet->m_FormTab2->m_bLineWelding == TRUE || m_pDlgSet->m_FormTab2->m_b2LineWelding == TRUE){
		m_strResultValue[0].Format("R(Line1) : %.1f(%.0f)",m_Project.m_dBlackPer[0],m_pDlgSet->m_FormTab4->m_dLeftArea);
		m_strResultValue[1].Format("S(Line1) : %.1f(%.0f)",m_Project.m_dBlackPer1[0],m_pDlgSet->m_FormTab4->m_dLeftArea1);
		m_strResultValue[2].Format("R(Line2) : %.1f(%.0f)",m_Project.m_dBlackPer[1],m_pDlgSet->m_FormTab4->m_dRightArea);
		m_strResultValue[3].Format("S(Line2) : %.1f(%.0f)",m_Project.m_dBlackPer1[1],m_pDlgSet->m_FormTab4->m_dRightArea1);
	}
	else{
		m_strResultValue[0].Format("R(Left) : %.1f(%.0f)",m_Project.m_dBlackPer[0],m_pDlgSet->m_FormTab4->m_dLeftArea);
		m_strResultValue[1].Format("S(Left) : %.1f(%.0f)",m_Project.m_dBlackPer1[0],m_pDlgSet->m_FormTab4->m_dLeftArea1);
		m_strResultValue[2].Format("R(Right) : %.1f(%.0f)",m_Project.m_dBlackPer[1],m_pDlgSet->m_FormTab4->m_dRightArea);
		m_strResultValue[3].Format("S(Right) : %.1f(%.0f)",m_Project.m_dBlackPer1[1],m_pDlgSet->m_FormTab4->m_dRightArea1);
	}

	int nBad[2];
	nBad[0]=nBad[1]=0;
	int nindex = 0;
	CString strbadtime;
	int nflow = 0;
	if(m_Project.m_rtWeldRect[0].Width()<10 ||m_Project.m_rtWeldRect[1].Width()<10)
	{
		if(m_Project.m_rtWeldRect[0].Width()<10 ) nindex = 0;
		else									nindex = 1;
		m_nBadHoleCount++;m_nBadArea[nn1]++;
		nret = 0;nflow = 4;
		strResult.Format(_T("ROI1"));//영역에러
		strbadtime.Format(_T("%s (%d)"),kk2,m_nOKCount+m_nNgCount);
		m_nBadCount[4]++;

		m_bHoleBad[0][0]=m_bHoleBad[0][1]=m_bHoleBad[1][0]=m_bHoleBad[1][1] = TRUE;

	}
	
	if((m_Project.m_dBlackPer[0]>m_pDlgSet->m_FormTab4->m_dLeftArea  ||m_Project.m_dBlackPer[1]>m_pDlgSet->m_FormTab4->m_dRightArea) && nret ==1)
	{
		if(m_Project.m_dBlackPer[0]>m_pDlgSet->m_FormTab4->m_dLeftArea)
		{
			if(m_pDlgSet->m_FormTab4->m_nSkipLeft !=0 && m_Project.m_dBlackPer[0]>m_pDlgSet->m_FormTab4->m_dLeftArea && m_pDlgSet->m_FormTab4->m_nSkipLeft>m_Project.m_dBlackPer1[0] && m_Project.m_dBlackPer[0]<93)
			{	
				;
			}
			else
			{
				if(m_Project.m_dBlackPer[0]>m_pDlgSet->m_FormTab4->m_dLeftArea) nindex = 0;
				else									nindex = 1;
				m_nBadHoleCount++;m_nBadArea[nn1]++;
				nret = 0;nflow = 3;
				strResult.Format(_T("Blur"));//퍼짐
				strbadtime.Format(_T("%s (%d)"),kk2,m_nOKCount+m_nNgCount);
				m_nBadCount[3]++;
				nBad[0]++;
				if(m_Project.m_dBlackPer[0]>m_pDlgSet->m_FormTab4->m_dLeftArea )
					m_bHoleBad[0][0]=TRUE;//왼쪽 불량

			}
		}
		if(m_Project.m_dBlackPer[1]>m_pDlgSet->m_FormTab4->m_dRightArea)
		{
			if(m_pDlgSet->m_FormTab4->m_nSkipRight !=0 && m_Project.m_dBlackPer[1]>m_pDlgSet->m_FormTab4->m_dRightArea && m_pDlgSet->m_FormTab2->m_nSkipRight>m_Project.m_dBlackPer1[1] && m_Project.m_dBlackPer[1]<93)
			{	
				;
			}
			else
			{
				if(m_Project.m_dBlackPer[0]>m_pDlgSet->m_FormTab4->m_dLeftArea) nindex = 0;
				else									nindex = 1;
				m_nBadHoleCount++;m_nBadArea[nn1]++;
				nret = 0;nflow = 3;
				strResult.Format(_T("Blur"));//퍼짐
				strbadtime.Format(_T("%s (%d)"),kk2,m_nOKCount+m_nNgCount);
				m_nBadCount[3]++;
				nBad[0]++;
				if(m_Project.m_dBlackPer[1]>m_pDlgSet->m_FormTab4->m_dRightArea)
					m_bHoleBad[0][1]=TRUE;//우측 불량

			}
		}

	}
	//1point re check
	if(m_nMachine==3 && m_Project.m_nTopEdge[1] - m_Project.m_rtWeldRect[1].left<=5 && m_Project.m_dBlackPer[1]>5 && nret==1 )
	{
		if(m_Project.m_nTopEdge[0] > m_Project.m_rtWeldRect[0].left+m_Project.m_rtWeldRect[0].Width()*0.9) nindex = 0;
		else									nindex = 1;
		m_nBadHoleCount++;	m_nBadHole[nn1]++;
		nret = 0;nflow = 5;
		strResult.Format(" 1Point");//강흑점");
		strbadtime.Format("%s (%d)",kk2,m_nOKCount+m_nNgCount);
		m_nBadCount[5]++;

	}
	else if(m_nMachine==3 && m_Project.m_nTopEdge[1] - m_Project.m_rtWeldRect[1].left<=15 && m_Project.m_dBlackPer[1]>16 && nret==1 )
	{
		if(m_Project.m_nTopEdge[0] > m_Project.m_rtWeldRect[0].left+m_Project.m_rtWeldRect[0].Width()*0.9) nindex = 0;
		else									nindex = 1;
		m_nBadHoleCount++;	m_nBadHole[nn1]++;
		nret = 0;nflow = 5;
		strResult.Format(" 1Point");//강흑점");
		strbadtime.Format("%s (%d)",kk2,m_nOKCount+m_nNgCount);
		m_nBadCount[5]++;
	}
	else if(m_nMachine==3 && m_Project.m_nTopEdge[1] - m_Project.m_rtWeldRect[1].left<=20 && m_Project.m_dBlackPer[1]>20 && nret==1 )
	{
		if(m_Project.m_nTopEdge[0] > m_Project.m_rtWeldRect[0].left+m_Project.m_rtWeldRect[0].Width()*0.9) nindex = 0;
		else									nindex = 1;
		m_nBadHoleCount++;	m_nBadHole[nn1]++;
		nret = 0;nflow = 5;
		strResult.Format(" 1Point");//강흑점");
		strbadtime.Format("%s (%d)",kk2,m_nOKCount+m_nNgCount);
		m_nBadCount[5]++;
	}
	else if(m_nMachine==3 && m_Project.m_nTopEdge[1] - m_Project.m_rtWeldRect[1].left<=25 && m_Project.m_dBlackPer[1]>25 && nret==1 )
	{
		if(m_Project.m_nTopEdge[0] > m_Project.m_rtWeldRect[0].left+m_Project.m_rtWeldRect[0].Width()*0.9) nindex = 0;
		else									nindex = 1;
		m_nBadHoleCount++;	m_nBadHole[nn1]++;
		nret = 0;nflow = 5;
		strResult.Format(" 1Point");//강흑점");
		strbadtime.Format("%s (%d)",kk2,m_nOKCount+m_nNgCount);
		m_nBadCount[5]++;
	}
	if(m_pDlgSet->m_FormTab4->m_bParamAnd )
	{
		if((m_Project.m_dBlackPer1[0]>m_pDlgSet->m_FormTab4->m_dLeftArea1 ||m_Project.m_dBlackPer1[1]>m_pDlgSet->m_FormTab4->m_dRightArea1 ) && nret==0) ;
		else if(nret==0)
		{
			nret = 1;
			m_nBadHoleCount--;m_nBadArea[nn1]--;
			m_nBadCount[3]--;
		}
	}
	else{
		if(((m_Project.m_dBlackPer1[0])>m_pDlgSet->m_FormTab4->m_dLeftArea1 ||(m_Project.m_dBlackPer1[1])>m_pDlgSet->m_FormTab4->m_dRightArea1 ) && nret==1)
		{
			if(m_Project.m_dBlackPer1[0]>m_pDlgSet->m_FormTab4->m_dLeftArea1){
				nindex = 0;
				m_bHoleBad[1][0]=TRUE;//우측 불량
			}
			else{
				nindex = 1;
				m_bHoleBad[1][1]=TRUE;//우측 불량
			}
			m_nBadHoleCount++;	m_nBadHole[nn1]++;
			nret = 0;nflow = 2;
			strResult.Format("Hole");//강흑점");
			strbadtime.Format("%s (%d)",kk2,m_nOKCount+m_nNgCount);
			m_nBadCount[2]++;
		}
	}
	if(m_nMachine !=3 && m_pDlgSet->m_FormTab4->m_bHoleTopSearch && ((m_Project.m_nTopEdge[0] > m_Project.m_rtWeldRect[0].left+m_Project.m_rtWeldRect[0].Width()*0.7) ||(m_Project.m_nTopEdge[1] < m_Project.m_rtWeldRect[1].right-m_Project.m_rtWeldRect[1].Width()*0.7))&&nret==1)
	{
		if(m_nMachine==3)
		{
			if((m_Project.m_nTopEdge[0] > m_Project.m_rtWeldRect[0].left+m_Project.m_rtWeldRect[0].Width()*0.9 )|| (m_Project.m_nTopEdge[1] < m_Project.m_rtWeldRect[1].right-m_Project.m_rtWeldRect[1].Width()*0.9))
			{
				if(m_Project.m_nTopEdge[0] > m_Project.m_rtWeldRect[0].left+m_Project.m_rtWeldRect[0].Width()*0.9) nindex = 0;
				else									nindex = 1;
				m_nBadHoleCount++;	m_nBadHole[nn1]++;
				nret = 0;nflow = 5;
				strResult.Format(" 1Point");//강흑점");
				strbadtime.Format("%s (%d)",kk2,m_nOKCount+m_nNgCount);
				m_nBadCount[5]++;
			}
		}
		else
		{
			if(m_Project.m_nTopEdge[0] > m_Project.m_rtWeldRect[0].left+m_Project.m_rtWeldRect[0].Width()*0.7) nindex = 0;
			else									nindex = 1;
			m_nBadHoleCount++;	m_nBadHole[nn1]++;
			nret = 0;nflow = 5;
			strResult.Format(" 1Point");//강흑점");
			strbadtime.Format("%s (%d)",kk2,m_nOKCount+m_nNgCount);
			m_nBadCount[5]++;
		}
	}

	//위치에러
	if(!m_pDlgSet->m_FormTab4->m_b2Tab &&(m_Project.m_rtWeldRect[0].top <m_Project.m_ptCapCenter.y || m_Project.m_rtWeldRect[1].top <m_Project.m_ptCapCenter.y)&&nret==1)
	{
		if(m_pDlgSet->m_FormTab4->m_nSkipLeft==100 ||m_pDlgSet->m_FormTab4->m_nSkipRight==100)
			;
		else
		{
			if(m_Project.m_rtWeldRect[0].top <m_Project.m_ptCapCenter.y) nindex = 0;
			else									nindex = 1;
			m_nBadHoleCount++;	m_nBadHole[nn1]++;
			nret = 0;nflow = 4;
			strResult.Format("ROI2");//d위치에러");
			strbadtime.Format("%s (%d)",kk2,m_nOKCount+m_nNgCount);
			m_nBadCount[4]++;
		}
	}
	//if(m_pDlgSet->m_FormTab4->m_bHoleLeftCheck && nret == 0 && nindex==0 && nflow ==3)
	if(m_pDlgSet->m_FormTab4->m_bHoleLeftCheck && nret == 0 && nflow ==3)
	{//홀이 에지의 걸려있는지 확인한후 과검 체크
		BOOL bholecheck1=0;
		BOOL bholecheck2=0;
		if(m_Project.m_rtWeldRect[0].left< m_Project.m_nTopEdge[0])
		{
			if(m_Project.m_nCogDist[0][1]>0 && m_Project.m_nCogDist[0][0]<1000&& m_Project.m_nCogDist[0][1]-m_Project.m_nCogDist[0][0] < m_pDlgSet->m_FormTab4->m_nCogDist){
				if(m_nMachine==1 )
				{
					if( m_Project.m_nCogDist[1][1]-m_Project.m_nCogDist[1][0] >m_pDlgSet->m_FormTab4->m_nCogDist){

					}
					else
					{
						if(nflow<3)m_nBadArea[nn1]--;
						else m_nBadHole[nn1]--;
						m_nBadCount[nflow]--;
						m_nBadHoleCount--;	
						nret = 1;
					}
				}
				else
				{
					bholecheck1=1;
				}
			}
			str.Format("CogDist : %d",m_Project.m_nCogDist[0][1]-m_Project.m_nCogDist[0][0]);
			AddtoInspLog(str);
		}
		if(m_Project.m_rtWeldRect[1].right> m_Project.m_nTopEdge[1])
		{
			if(m_Project.m_nCogDist[1][1]>0 && m_Project.m_nCogDist[1][0]<1000&& m_Project.m_nCogDist[1][1]-m_Project.m_nCogDist[1][0] < m_pDlgSet->m_FormTab4->m_nCogDist){
					if(m_nMachine==1 )
					{
						if( m_Project.m_nCogDist[1][1]-m_Project.m_nCogDist[1][0] >m_pDlgSet->m_FormTab4->m_nCogDist){

						}
						else
						{
							if(nflow<3)m_nBadArea[nn1]--;
							else m_nBadHole[nn1]--;
							m_nBadCount[nflow]--;
							m_nBadHoleCount--;	
							nret = 1;
						}
					}
					else
					{
						bholecheck2=1;
					}
			}
			str.Format("CogDist : %d",m_Project.m_nCogDist[0][1]-m_Project.m_nCogDist[0][0]);
			AddtoInspLog(str);
		}
		if(!(bholecheck1==0||bholecheck2==0))
		{
			if(nflow<3)m_nBadArea[nn1]--;
			else m_nBadHole[nn1]--;
			m_nBadCount[nflow]--;
			m_nBadHoleCount--;	
			nret = 1;
		}

	}

	if(m_nMachine==3 && m_pDlgSet->m_FormTab4->m_bHoleTopSearch && m_Project.m_n1Hole>0 && nret==1)
	{
		m_nBadHoleCount++;	m_nBadHole[nn1]++;
		nret = 0;nflow = 5;
		strResult.Format(" 1Point");//강흑점");
		strbadtime.Format("%s (%d)",kk2,m_nOKCount+m_nNgCount);
		m_nBadCount[5]++;
	}

	if(m_pDlgSet->m_FormTab4->m_bBlackArea && (m_Project.m_dMaxChainData[0] > m_pDlgSet->m_FormTab4->m_nBlackArea )&&nret==1)
	{
		if(m_Project.m_rtWeldRect[0].top <m_Project.m_ptCapCenter.y) nindex = 0;
		else									nindex = 1;
		m_nBadHoleCount++;	m_nBadHole[nn1]++;
		nret = 0;nflow = 4;
		strResult.Format("AreaBlur");//강흑점");
		strbadtime.Format("%s (%d)",kk2,m_nOKCount+m_nNgCount);
		m_nBadCount[3]++;
	}
	if(m_pDlgSet->m_FormTab4->m_bHoleEdgeCheck &&nret==1)
	{
		double dcheckmm = m_pDlgSet->m_FormTab4->m_dHoleEdgemm / m_pDlgSet->m_FormTab2->m_dCamScale;

		if(m_rtWeldBlob.left< m_Project.m_nTopEdge[0]+dcheckmm || m_rtWeldBlob.right> m_Project.m_nTopEdge[1]-dcheckmm)
		{//ng
			m_nBadHoleCount++;	m_nBadHole[nn1]++;
			nret = 0;nflow = 5;
			strResult.Format(" TabEdge");//강흑점");
			strbadtime.Format("%s (%d)",kk2,m_nOKCount+m_nNgCount);
			m_nBadCount[4]++;

		}
	}
//1point check

	if(FALSE && m_pDlgSet->m_FormTab2->m_bLineWelding)
	{
		//m_dMinHeight //m_dMaxHeight
		if(m_pDlgSet->m_FormTab4->m_dMinHeight> m_Project.m_nLinMinMax[0] || m_pDlgSet->m_FormTab4->m_dMaxHeight<m_Project.m_nLinMinMax[1])
		{
			m_nBadHoleCount++;	m_nBadHole[nn1]++;
			nret = 0;nflow = 4;
			strResult.Format("Line");//강흑점");
			strbadtime.Format("%s (%d)",kk2,m_nOKCount+m_nNgCount);
			m_nBadCount[2]++;
		}
	}

	if(m_pDlgSet->m_FormTab4->m_bWeldingRange && m_pDlgSet->m_FormTab4->m_bWeldingRangeNgSignal){
		int nWeldingBoxGap = 15;
		if(m_Project.m_rtWeldRect[0].top+nWeldingBoxGap < m_Project.m_rtWeldingOkRange.top || m_Project.m_rtWeldRect[0].top+nWeldingBoxGap > m_Project.m_rtWeldingOkRange.bottom ){
			m_nBadHoleCount++;	m_nBadHole[nn1]++;
			nret = 0;nflow = 4;
			strResult.Format("WeldingRange");
			strbadtime.Format("%s (%d)",kk2,m_nOKCount+m_nNgCount);
			m_nBadCount[2]++;
		}
	}

	if(nret==0)
	{
//		m_DisplaySave[0].m_strImagePath[0] = strResult;
		m_strBadInfo1[1] = strResult;
		m_strBadInfo1[0] = strbadtime;
		ImageShiftBad();
		//time bad count
	}
	
	return nret;
}

int CFlyCapture2_EXDlg::reCheckHole(int nindex,CRect rt)
{
	int nret = 0;


	return  nret;
}

void CFlyCapture2_EXDlg::BadNgCount(int nNgCode, BOOL bRetRB, BOOL bRetDL)
{
	CString str;
	int nsize = 16;
	//m_LabelTimeCheck.SetTextColor(COLOR_WHITE);
	//m_LabelTimeCheck.SetBkColor(COLOR_MAIN_LABEL);
	//m_LabelTimeCheck.SetFontBold(TRUE);
	//m_LabelTimeCheck.SetFontName(_T("굴림"));
	//m_LabelTimeCheck.SetFontSize(nsize);
	//str.Format(" Time Chk Cnt : %d",m_nTimeCheckCount);
	//m_LabelTimeCheck.SetText(str);

	if(nNgCode==0)
	{
		//not cap
		m_LabelNotCap.SetTextColor(COLOR_WHITE);
		m_LabelNotCap.SetBkColor(COLOR_MAIN_LABEL);// COLOR_TOP_BK);
		m_LabelNotCap.SetFontBold(TRUE);
		m_LabelNotCap.SetFontName(_T("굴림"));
		m_LabelNotCap.SetFontSize(nsize);
		m_LabelNotCap.SetText(_T(" Not Cap"));

		//Cap ng		
		m_LabelCapNg.SetTextColor(COLOR_WHITE);
		m_LabelCapNg.SetBkColor(COLOR_MAIN_LABEL);
		m_LabelCapNg.SetFontBold(TRUE);
		m_LabelCapNg.SetFontName(_T("굴림"));
		m_LabelCapNg.SetFontSize(nsize);
		m_LabelCapNg.SetText(_T(" Cap Ng"));

		//Dist Ng
		m_LabelDistNg.SetTextColor(COLOR_WHITE);
		m_LabelDistNg.SetBkColor(COLOR_MAIN_LABEL);
		m_LabelDistNg.SetFontBold(TRUE);
		m_LabelDistNg.SetFontName(_T("굴림"));
		m_LabelDistNg.SetFontSize(nsize);
		m_LabelDistNg.SetText(_T(" Dist Ng"));

		//Weld Ng
		m_LabelWeldNg.SetTextColor(COLOR_WHITE);
		m_LabelWeldNg.SetBkColor(COLOR_MAIN_LABEL);
		m_LabelWeldNg.SetFontBold(TRUE);
		m_LabelWeldNg.SetFontName(_T("굴림"));
		m_LabelWeldNg.SetFontSize(nsize);
		m_LabelWeldNg.SetText(_T(" Weld Ng"));

		//Tab Ng
		m_LabelTabNg.SetTextColor(COLOR_WHITE);
		m_LabelTabNg.SetBkColor(COLOR_MAIN_LABEL);
		m_LabelTabNg.SetFontBold(TRUE);
		m_LabelTabNg.SetFontName(_T("굴림"));
		m_LabelTabNg.SetFontSize(nsize);
		m_LabelTabNg.SetText(_T(" Tab Ng"));

		m_LabelRB.SetTextColor(COLOR_WHITE);
		m_LabelRB.SetBkColor(COLOR_MAIN_LABEL);
		m_LabelRB.SetFontBold(TRUE);
		m_LabelRB.SetFontName(_T("굴림"));
		m_LabelRB.SetFontSize(25);

		m_LabelDL.SetTextColor(COLOR_WHITE);
		m_LabelDL.SetBkColor(COLOR_MAIN_LABEL);
		m_LabelDL.SetFontBold(TRUE);
		m_LabelDL.SetFontName(_T("굴림"));
		m_LabelDL.SetFontSize(25);
		
		for (int i = 0; i < 4; i++) {
			m_LabelDLNgCategory[i].SetTextColor(COLOR_WHITE);
			m_LabelDLNgCategory[i].SetBkColor(COLOR_MAIN_LABEL);
			m_LabelDLNgCategory[i].SetFontBold(TRUE);
			m_LabelDLNgCategory[i].SetFontName(_T("굴림"));
			m_LabelDLNgCategory[i].SetFontSize(nsize);
		}


		//불량율
		m_LabelBadCount2.SetTextColor(COLOR_WHITE);
		m_LabelBadCount2.SetBkColor(COLOR_MAIN_LABEL);
		m_LabelBadCount2.SetFontBold(TRUE);
		m_LabelBadCount2.SetFontName(_T("굴림"));
		m_LabelBadCount2.SetFontSize(20);
		m_LabelBadCount2.SetText("  Yield");
	}
	else
	{
		//DL Count
		if (m_nDLNgType == -1)
			;
		else if (bRetRB == RET_OK)
			m_nDLBadCount[m_nDLNgType]++;

		double nbadCount = m_nBadCount[1]+m_nBadCount[2]+m_nBadCount[3]+m_nBadCount[4]+m_nBadCount[5] + m_nDLBadCount[0] + m_nDLBadCount[1] + m_nDLBadCount[2] + m_nDLBadCount[3];
		double nTotalCount = nbadCount+m_nOKCount;
		if(nbadCount!=0)
			str.Format(" Yield   :    %.2f %%",(double)m_nOKCount/double(nTotalCount)*100);
//		str.Format(" Yield   :    %.2f %%",(double)nbadCount/double(nTotalCount)*100);
		else	
			str.Format("  Yield   :   100 %%");
		m_LabelBadCount2.SetText(str);

		if(nbadCount<1)nbadCount = 1;
		if(nTotalCount<1)nTotalCount = 1;
		
		str.Format(_T(" Not Cap: %d ea"), m_nBadCount[0]);
		m_LabelNotCap.SetText(str);

		str.Format(" Cap Ng: %.2f%%  (%d)ea", m_nBadCount[1] / nbadCount * 100, m_nBadCount[1]);
		m_LabelCapNg.SetText(str);

		str.Format(" Tab Ng: %.2f%%  (%d)ea", m_nBadCount[2] / nbadCount * 100, m_nBadCount[2]);
		m_LabelDistNg.SetText(str);

		str.Format(" Weld Ng: %.2f%%  (%d)ea", m_nBadCount[3] / nbadCount * 100, m_nBadCount[3]);
		m_LabelWeldNg.SetText(str);

		str.Format(" Dist Ng: %.2f%%  (%d)ea", m_nBadCount[4] / nbadCount * 100, m_nBadCount[4]);
		m_LabelTabNg.SetText(str);


		str.Format(" Cap Ng: %.2f%%  (%d)ea", m_nDLBadCount[0] / nbadCount * 100, m_nDLBadCount[0]);
		m_LabelDLNgCategory[0].SetText(str);

		str.Format(" Tab Ng: %.2f%%  (%d)ea", m_nDLBadCount[1] / nbadCount * 100, m_nDLBadCount[1]);
		m_LabelDLNgCategory[1].SetText(str);

		str.Format(" Weld Ng: %.2f%%  (%d)ea", m_nDLBadCount[2] / nbadCount * 100, m_nDLBadCount[2]);
		m_LabelDLNgCategory[2].SetText(str);

		str.Format(" Dist Ng: %.2f%%  (%d)ea", m_nDLBadCount[3] / nbadCount * 100, m_nDLBadCount[3]);
		m_LabelDLNgCategory[3].SetText(str);

	}
	m_nDLNgType = -1;
	////NG
	//if (bNgTab || bNgHole)
	//	m_nTimeCount[m_nHour][1]++;//ng

	//if (bNgTab)	//Tab NG
	//	m_nTimeCount[m_nHour][3]++;
	//else if (bNgHole)//Hole NG
	//	m_nTimeCount[m_nHour][4]++;
	//else if (m_bHoleBad[0][0] == TRUE)
	//	m_nTimeCount[m_nHour][5]++;
	//else
	//	m_nTimeCount[m_nHour][6]++;

}
void CFlyCapture2_EXDlg::ResetNgCnt()
{
	TotalResetCnt();
	TotalNgCnt();

}

void CFlyCapture2_EXDlg::NgPosition(BOOL status)
{
	/*if( m_nMachine==2 &&	m_strComname.Find("1")>=0){
		for(int i=20; i>=0; i=i-2){
			m_bNgPosition[i] = m_bNgPosition[i-2];
		}
		m_bNgPosition[0] = status;
	}

	else if( m_nMachine==2 && m_strComname.Find("2")>=0){
		for(int i=21; i>0; i=i-2){
			m_bNgPosition[i] = m_bNgPosition[i-2];
		}
		m_bNgPosition[1] = status;
	}*/

	int nmax = 10;

	if(m_nMachine==3)
		nmax = 30;
	else if(m_strComname.Find("3C")>=0 || m_strComname.Find("4C")>=0)
		nmax = 9;
	else if(m_strComname.Find("3A")>=0 || m_strComname.Find("3B")>=0 || m_strComname.Find("4A")>=0 || m_strComname.Find("4B")>=0 
		|| m_strComname.Find("5A")>=0 || m_strComname.Find("6A")>=0 || m_strComname.Find("7A")>=0 || m_strComname.Find("8A")>=0)
		nmax	= 8;
	else if(m_strComname.Find("5C")>=0 || m_strComname.Find("6C")>=0 
		|| m_strComname.Find("5B")>=0 || m_strComname.Find("6B")>=0 || m_strComname.Find("7C")>=0 || m_strComname.Find("8C")>=0)
		nmax = 7;

	for(int i=nmax; i>0; i--){
		m_bNgPosition[i] = m_bNgPosition[i-1];
	}

	m_bNgPosition[0] = status;


	m_CamDisplay->ShowNgPosition();
}

void CFlyCapture2_EXDlg::WriteDailyData()
{
	CStdioFile   file;
	CString filename;
	CString strpath("d:\\Data\\");
	CTime ttime = CTime::GetCurrentTime();
	CString kk = ttime.Format("%Y%m%d");
	CString kk2 = ttime.Format("%Y%m");
	CString kk3 = ttime.Format("%d");
	CString kk1 = ttime.Format("%Y%m%d_%H%M%S");
	CString kk11 = ttime.Format("%y");
	CString kk12 = ttime.Format("%m");
	CString kk13 = ttime.Format("%d");
	CString kk14 = ttime.Format("%H");
	CString kk15 = ttime.Format("%M");
	CString kk16 = ttime.Format("%S");

	CString kk17 = kk11+"년"+kk12+"."+kk13+"."+ kk14+"."+ kk15+"."+ kk16+".";

	CString strheader,str;
	int n1 = m_nOKCount;
	int n2 = m_DlgBarcode->m_nMakeCount+1;

	CreateDirectory	(strpath,NULL);
	strpath.Format("d:\\Data\\");
	CreateDirectory(strpath,NULL);
	strpath.Format("d:\\Data\\Log\\");
	CreateDirectory(strpath,NULL);
	strpath.Format("d:\\Data\\Log\\%s\\",kk2);//년월
	CreateDirectory(strpath,NULL);
	strpath.Format("d:\\Data\\Log\\%s\\%s_DailyData_%d.txt",kk2,kk,m_nResetCount);//m_strLot);//m_nOKCount/pView->m_DlgBarcode->m_nMakeCount);

	CString str1;
	filename.Format(_T("%s"),strpath);
	double dok = m_nOKCount;
	double dng = m_nNgCount;

	if(file.Open(filename, CFile::modeCreate|CFile::modeReadWrite|CFile::typeText))
	{
		if(dok<1)dok = 1;
		str.Format("Total : %d",m_nOKCount+m_nNgCount);							file.WriteString(str);	file.WriteString("\n");	
		str.Format("OK : %.0f(%.1f)",dok,100-(dng/dok)*100);					file.WriteString(str);	file.WriteString("\n");	
		str.Format("MG : %.0f(%.1f)",dng,(dng/dok)*100);						file.WriteString(str);	file.WriteString("\n");	
		str.Format("NotCap : %d",m_nBadCount[0]);								file.WriteString(str);	file.WriteString("\n");	
		if(dng<1)dng = 1;
		str.Format("Tab : %d(%.1f)",m_nBadCount[1],m_nBadCount[1]/dng*100);		file.WriteString(str);	file.WriteString("\n");	
		str.Format("Hole : %d(%.1f)",m_nBadCount[2],m_nBadCount[2]/dng*100);	file.WriteString(str);	file.WriteString("\n");	
		str.Format("Blue : %d(%.1f)",m_nBadCount[3],m_nBadCount[3]/dng*100);	file.WriteString(str);	file.WriteString("\n");	
		str.Format("1Point : %d(%.1f)",m_nBadCount[5],m_nBadCount[5]/dng*100);	file.WriteString(str);	file.WriteString("\n");	
		str.Format("ROI : %d(%.1f)",m_nBadCount[4],m_nBadCount[4]/dng*100);		file.WriteString(str);	file.WriteString("\n");	

		file.Close();
	}

}

void CFlyCapture2_EXDlg::WriteTimeRate()
{
	CStdioFile   file;
	CString filename;
	CString strpath("d:\\Data\\");
	CTime ttime = CTime::GetCurrentTime();
	CString kk = ttime.Format("%Y%m%d");
	CString kk2 = ttime.Format("%Y%m");
	CString kk3 = ttime.Format("%d");
	CString kk1 = ttime.Format("%Y%m%d_%H%M%S");
	CString kk11 = ttime.Format("%y");
	CString kk12 = ttime.Format("%m");
	CString kk13 = ttime.Format("%d");
	CString kk14 = ttime.Format("%H");
	CString kk15 = ttime.Format("%M");
	CString kk16 = ttime.Format("%S");

	CString kk17 = kk11+"년"+kk12+"."+kk13+"."+ kk14+"."+ kk15+"."+ kk16+".";

	CString strheader,str;
	int n1 = m_nOKCount;
	int n2 = m_DlgBarcode->m_nMakeCount+1;

	CreateDirectory(strpath,NULL);
	strpath.Format("d:\\Data\\");
	CreateDirectory(strpath,NULL);
	strpath.Format("d:\\Data\\Log\\");
	CreateDirectory(strpath,NULL);
	strpath.Format("d:\\Data\\Log\\%s\\",kk2);//년월
	CreateDirectory(strpath,NULL);
	strpath.Format("d:\\Data\\Log\\%s\\%s_TimeRate_%d.txt",kk2,kk,m_nResetCount);//m_strLot);//m_nOKCount/pView->m_DlgBarcode->m_nMakeCount);

	CString str1;
	filename.Format(_T("%s"),strpath);
	double dok = m_nOKCount;
	double dng = m_nNgCount;

	if(file.Open(filename, CFile::modeCreate|CFile::modeReadWrite|CFile::typeText))
	{
		for(int i=0;i<24;i++){
			str.Format("%.2f",m_dTimeRate[i]);
			file.WriteString(str);	file.WriteString("\n");				
		}
		file.Close();
	}

}


void CFlyCapture2_EXDlg::ReadTimeRate()
{
	CStdioFile   file;
	CString filename;
	CString strpath("d:\\Data\\");
	CTime ttime = CTime::GetCurrentTime();
	CString kk = ttime.Format("%Y%m%d");
	CString kk2 = ttime.Format("%Y%m");
	CString kk3 = ttime.Format("%d");
	CString kk1 = ttime.Format("%Y%m%d_%H%M%S");
	CString kk11 = ttime.Format("%y");
	CString kk12 = ttime.Format("%m");
	CString kk13 = ttime.Format("%d");
	CString kk14 = ttime.Format("%H");
	CString kk15 = ttime.Format("%M");
	CString kk16 = ttime.Format("%S");

	CString kk17 = kk11+"년"+kk12+"."+kk13+"."+ kk14+"."+ kk15+"."+ kk16+".";

	CString strheader,str;
	int n1 = m_nOKCount;
	int n2 = m_DlgBarcode->m_nMakeCount+1;

	CreateDirectory(strpath,NULL);
	strpath.Format("d:\\Data\\");
	CreateDirectory(strpath,NULL);
	strpath.Format("d:\\Data\\Log\\");
	CreateDirectory(strpath,NULL);
	strpath.Format("d:\\Data\\Log\\%s\\",kk2);//년월
	CreateDirectory(strpath,NULL);
	strpath.Format("d:\\Data\\Log\\%s\\%s_TimeRate_0.txt",kk2,kk);//m_strLot);//m_nOKCount/pView->m_DlgBarcode->m_nMakeCount);

	CString str1;
	filename.Format(_T("%s"),strpath);
	double dok = m_nOKCount;
	double dng = m_nNgCount;

	if(file.Open(filename, CFile::modeReadWrite|CFile::typeText))
	{
		for(int i=0;i<24;i++){
			file.ReadString(str);
			if(ttime.GetHour()!=i)
				m_dTimeRate[i] = atof(str);
			else
				m_dTimeRate[i] = 0;
		}
		file.Close();
	}
	else
	{
		for(int i=0;i<24;i++){
			m_dTimeRate[i] = 0;
		}
	}
}

void CFlyCapture2_EXDlg::DataReset()
{

	fnReset();

	InitBadInfo();

	m_nFullFrameCount = 0;
	//		int staddr=20;
	m_fFps = 0;
	m_Project.ResetFrame();
	m_nSendIO = 0;
	m_nOKNG[0]=m_nOKNG[1] = -1;
	m_nBufCount[0] = m_nBufCount[1] = 0;


	CDC* pDC=GetDC();

	for (int k=0;k<5;k++){
		m_DisplaySave[0].m_strImagePath[k] = "";
		memset( m_DisplayBad[k].m_pImg->GetImagePtr(),0,300*192);
		m_DisplayBad[k].DrawImage(pDC);
	}
	ReleaseDC(pDC);

	m_dTactTimeAve[0]=m_dTactTimeAve[1]=m_dTactTimeAve[2] = 0;
	//control Reset
	m_Project.m_dBrightness = 0;
	//Label 초기화
	LabelResult(-1, AI_READY);

	m_strIsCode = "";
	m_strModelName = "";
	m_strUpperLow = "";
	m_nNewCount = 0;
	m_Project.m_nNgCount=m_Project.m_nFrameCount = 0;
	m_Project.m_dInsTime = 0;
	m_nSaveCount = 0;
	m_strIsCode = "";
	m_nNg1Count=m_nNg2Count = 0;
	m_nNgCount = 0;
	m_nOKCount = 0;
	m_dYield = 0;
	m_dTactTime = 0;

	m_nRWFrameCount =  0;
	m_nRWOkCount = 0;
	m_nRWNgCount = 0;
	m_bRework = FALSE;
	m_nIOTriCount=0;
	m_nIOTriErrCount=0;

}
void CFlyCapture2_EXDlg::SetSystemTimePrivilege() 
{ 
	HANDLE hToken; 
	TOKEN_PRIVILEGES tp; 
	LUID luid; 
	OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | 
		TOKEN_QUERY, &hToken); 
	LookupPrivilegeValue(NULL,SE_SYSTEMTIME_NAME,&luid); 
	tp.PrivilegeCount = 1;  
	tp.Privileges[0].Luid = luid; 
	tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED; 
	AdjustTokenPrivileges(hToken, FALSE, &tp, 0, NULL, NULL); 
} 
void CFlyCapture2_EXDlg::WriteTextData(int nresult)
{
	CStdioFile   file;
	CString filename;
	CString strpath;
	CString strheader;
	//////////////////////////////////////////////////////////////////////////
	// 	SYSTEMTIME new_time;
	// 	SetSystemTimePrivilege();
	// 	GetLocalTime(&new_time);
	// 	new_time.wHour = 1;
	// 	SetLocalTime(&new_time);
	//////////////////////////////////////////////////////////////////////////
	//0922
	CTime ttime = CTime::GetCurrentTime();
	CString kk = ttime.Format("%y%m%d");//연월일
	CString kk1 = ttime.Format("%y%m");
	CString kk2 = ttime.Format("%Y");
	CString kk3 = ttime.Format("%m");
	CString kk4 = ttime.Format("%d");
	CString kk5 = ttime.Format("%H%M%S");//시분초
	CString kk6 = ttime.Format("%y,%m,%d,%H,%M,%S");//년,월,일,시,분,초

	CString strRtosIP= "194.195.196.70";

	int nCom=0;
	if(m_strComname.Find("A")>=0)
		nCom=1;
	else if(m_strComname.Find("B")>0)
		nCom=2;
	else
		nCom=3;
	int nTmp = atoi(ttime.Format("%H"));
	//저장위치 ? C:\DATA\YY\MM\DD\Vision#\LOG
	if(m_pDlgSet->m_FormTab2->m_bUseRtos)
	{
		strpath.Format(_T("\\\\%s\\DATA\\CSW\\CSW_WELD%d\\"),strRtosIP,nCom);
		CreateDirectory(strpath,NULL);
		strpath.Format(_T("\\\\%s\\DATA\\CSW\\CSW_WELD%d\\%s\\"),strRtosIP,nCom,kk2);
		CreateDirectory(strpath,NULL);
		strpath.Format(_T("\\\\%s\\DATA\\CSW\\CSW_WELD%d\\%s\\%s\\"),strRtosIP,nCom,kk2,kk3);
		CreateDirectory(strpath,NULL);
		strpath.Format(_T("\\\\%s\\DATA\\CSW\\CSW_WELD%d\\%s\\%s\\%s\\"),strRtosIP,nCom,kk2,kk3,kk4);
		CreateDirectory(strpath,NULL);
	}


	if((m_nCSVCount)%1000==1){
		m_Project.m_strTimeHMS=kk5;
		//WriteJsonFile();
	}
	filename.Format(_T("%s%s_%s_00001.csv"),strpath,kk,m_Project.m_strTimeHMS);
	CString str12,str13;

	CString str11,strTab,strWelding,strNGNum;
	int ncx1 = (m_Project.m_rtWeldRect[0].right+m_Project.m_rtWeldRect[0].left)*0.5;
	int ncy1 = (m_Project.m_rtWeldRect[0].top+m_Project.m_rtWeldRect[0].bottom)*0.5;
	int ncx2 = (m_Project.m_rtWeldRect[1].right+m_Project.m_rtWeldRect[1].left)*0.5;
	int ncy2 = (m_Project.m_rtWeldRect[1].top+m_Project.m_rtWeldRect[1].bottom)*0.5;
	double dCenterTabL = abs(m_Project.m_ptCapCenter.x-m_Project.m_nTopEdge[0])*m_pDlgSet->m_FormTab2->m_dCamScale;
	double dCenterTabR = abs(m_Project.m_ptCapCenter.x-m_Project.m_nTopEdge[1])*m_pDlgSet->m_FormTab2->m_dCamScale;
	double dCenterTabT  = (m_Project.m_ptCapCenter.y-m_Project.m_nTopPos[0])*m_pDlgSet->m_FormTab2->m_dCamScale;

	strNGNum.Format("NG_NUM_%d",nresult);
	//AddtoInspLog(strNGNum);
	strTab.Format(_T("%.3f,%.3f,%.3f"),dCenterTabL,dCenterTabR,dCenterTabT);
	strWelding.Format(_T("%.1f,%.1f"),m_Project.m_dBlackPer[0],m_Project.m_dBlackPer1[0]);
	// str11.Format(_T("%s,%s,%d,%s,%s,%d,%d,%d,%d,%d,%d,%.1f,%d,%d,%d,%d,%d,%.1f,%.1f,%.1f,%.1f,%.1f,"),
	// kk6,m_nOKCount+m_nNgCount,str12,m_nCapCheckAve,m_Project.m_ptCapCenter.x,m_Project.m_ptCapCenter.y,m_Project.m_nTopEdge[0],m_Project.m_nTopEdge[1],
	// double(m_Project.m_nTopEdge[1]-m_Project.m_nTopEdge[0])*m_pDlgSet->m_FormTab2->m_dCamScale,m_Project.m_nTopPos[0],ncx1,ncy1,ncx2,ncy2,m_Project.m_dBlackPer[0],m_Project.m_dBlackPer[1],m_Project.m_dBlackPer1[0],m_Project.m_dBlackPer1[1],m_Project.m_dMaxChainData[0]);
	str11.Format(_T("%s,%010d,%d,%d,%d,%d,%d,%.1f,%.1f,%.1f,%.1f,%.1f,%.1f,%.1f,%.1f,%.1f,%.1f,%.1f,%.1f,%.1f,%.1f,%.1f,%.1f,%.1f"),	kk6,m_nOKCount+m_nNgCount,m_nInspTime,m_nTopCapCenterBrightness,70,3,nresult,m_dData[1],m_dData[2],m_dData[3],m_dData[4],m_dData[5],m_dData[6],m_dData[7],m_dData[8],m_dData[9],m_dData[10],m_dData[11],m_dData[12],m_dData[13],m_dData[14],m_dData[15],m_dData[16],m_dData[17]);
	//년월일시분초,프레임,검사시간,밝기,샤프니스값,CAM 3,NG, CAP_CAN_X,CAP_CAN_Y,WELD_CAN_DIST,WELD_TOPCAP_DIST,CAP_EDGE_V,EWAK_V,TAB_EDGE_V,WELD_CHECK,CAN,TAB_L,TAB_R,TAB_T,LINE1_R,LINE1_S,WELDING_LEFT,WELDING_RIGHT,CAP_BRIGHTNESS
	//		1		2		3	4	5		6		7		8		9	10		11		12 13   14       15	     16      17     18    19      20      21    22
	//년월일,시분초,프레임,결과,불량명, 캔밝기,캡밝기, 센터x,센터y, 좌측탭,우측탭,탭폭,탭상,BeadCx1,BeadCy1,BeadCx2,BeadCy2,Per1-1,Per2-1,Per1-2,Per2-2,blackarea,
	//	str11.Format(_T("%s,%s,%d,%s,%s,%d,%d,%d,%d,%d,%d,"),
	//		kk,kk5,m_Project.m_nFrameCount,str12,str13,m_nCanCheckAve,m_nCapCheckAve,m_Project.m_ptCapCenter.x,m_Project.m_ptCapCenter.y,m_Project.m_nTopEdge[0],m_Project.m_nTopEdge[1]);
	//		double(m_Project.m_nTopEdge[1]-m_Project.m_nTopEdge[0])*m_pDlgSet->m_FormTab4->m_dCamScale,m_Project.m_nTopPos[0],ncx1,ncy1,ncx2,ncy2,m_Project.m_dBlackPer[0],m_Project.m_dBlackPer[1],m_Project.m_dBlackPer1[0],m_Project.m_dBlackPer1[1],m_Project.m_dMaxChainData[0]);
	if(!file.Open(filename, CFile::modeReadWrite|CFile::typeText)) 
	{		
		if(file.Open(filename, CFile::modeCreate|CFile::modeReadWrite|CFile::typeText))
		{

			strheader.Format(_T("년,월,일,시,분,초,일련번호,Process Time,Gray Level,Sharpness,Cam_Number,NG,CAP_CAN_X,CAP_CAN_Y,WELD_CAN_DIST,WELD_TOPCAP_DIST,CAP_EDGE_V,EWAK_V,TAB_EDGE_V,WELD_CHECK,CAN,TAB_L,TAB_R,TAB_T,LINE1_R,LINE1_S,WELDING_LEFT,WELDING_RIGHT,CAP_BRIGHTNESS"));

			//file.WriteString(strheader);	
			//file.WriteString(_T("\n"));	
			//			file.WriteString(str);	
			file.WriteString(str11);	
			file.Close();
		}
	}
	else
	{
		file.SeekToEnd();
		file.WriteString(_T("\n"));
		//		file.WriteString(str);
		file.WriteString(str11);	
		file.Close(); 
	}	
}

void CFlyCapture2_EXDlg::WriteTextData1(int nRBResult, int nDLResult)
{
	BOOL bDeepLearning = m_pDlgSet->m_FormTab4->m_bUseAiIns;

	CStdioFile   file;
	CString filename;
	CString strpath;
	CString strheader;
	//0922
	CTime ttime = CTime::GetCurrentTime();
	CString kk = ttime.Format("%y%m%d");//연월일
	CString kk1 = ttime.Format("%y%m");
	CString kk2 = ttime.Format("%Y");
	CString kk3 = ttime.Format("%m");
	CString kk4 = ttime.Format("%d");
	CString kk5 = ttime.Format("%H%M%S");//시분초
	CString kk6 = ttime.Format("%y,%m,%d,%H,%M,%S");//년,월,일,시,분,초
	CString kk7 = ttime.Format("%H");

	int nCom=0;
	if(m_strComname.Find("A")>=0)
		nCom=1;
	else if(m_strComname.Find("B")>0)
		nCom=2;
	else
		nCom=3;
	int nTmp = atoi(ttime.Format("%H"));
	//저장위치 ? C:\DATA\YY\MM\DD\Vision#\LOG
	strpath.Format(_T("D:\\storage\\"));
	CreateDirectory(strpath,NULL);
	strpath.Format(_T("D:\\storage\\CSW\\"));
	CreateDirectory(strpath,NULL);
	strpath.Format(_T("D:\\storage\\CSW\\CSW_WELD%d\\"),nCom);
	CreateDirectory(strpath,NULL);
	strpath.Format(_T("D:\\storage\\CSW\\CSW_WELD%d\\%s\\"),nCom,kk2);
	CreateDirectory(strpath,NULL);
	strpath.Format(_T("D:\\storage\\CSW\\CSW_WELD%d\\%s\\%s\\"),nCom,kk2,kk3);
	CreateDirectory(strpath,NULL);
	strpath.Format(_T("D:\\storage\\CSW\\CSW_WELD%d\\%s\\%s\\%s\\"),nCom,kk2,kk3,kk4);
	CreateDirectory(strpath,NULL);

	static int nNOCnt = 1;
	if(m_strHour != kk7){
		m_strHour=kk7;
		//WriteJsonFile();
	}
	m_strHour = kk7;
	filename.Format(_T("%s%s_%s_%d.csv"),strpath,kk,m_strHour,nCom);

	CString str11,strTab,strWelding,strNGNum,strAINg,str33;
	int ncx1 = (m_Project.m_rtWeldRect[0].right+m_Project.m_rtWeldRect[0].left)*0.5;
	int ncy1 = (m_Project.m_rtWeldRect[0].top+m_Project.m_rtWeldRect[0].bottom)*0.5;
	int ncx2 = (m_Project.m_rtWeldRect[1].right+m_Project.m_rtWeldRect[1].left)*0.5;
	int ncy2 = (m_Project.m_rtWeldRect[1].top+m_Project.m_rtWeldRect[1].bottom)*0.5;
	double dCenterTabL = abs(m_Project.m_ptCapCenter.x-m_Project.m_nTopEdge[0])*m_pDlgSet->m_FormTab2->m_dCamScale;
	double dCenterTabR = abs(m_Project.m_ptCapCenter.x-m_Project.m_nTopEdge[1])*m_pDlgSet->m_FormTab2->m_dCamScale;
	double dCenterTabT  = (m_Project.m_ptCapCenter.y-m_Project.m_nTopPos[0])*m_pDlgSet->m_FormTab2->m_dCamScale;
	
	strNGNum.Format(_T("NG_NUM_%d"), nRBResult);
	//AddtoInspLog(strNGNum);

	strNGNum.Format(_T("Vision OK"));
	if(nRBResult != RET_OK)	
		strNGNum.Format(_T("Vision NG"));

	CString strDLDefectData;
	CString strDLDemesionData;
	if (bDeepLearning) {
		CString strDLRet;
		strDLRet.Format(_T("OK"));
		if (m_nDLResult != 1)	
			strDLRet.Format(_T("NG"));

		strDLDefectData.Format(_T("%s,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d"), strDLRet, m_bisTC, m_bisTC_frontback, m_bisTC_slatned, m_bisTC_CID, m_bisCAN, m_bisTAB, m_bisTABerror, m_bisETCerror, m_bisYJ_fromTAB, m_bisYakYJ, m_bisGwaYJ, m_bisGeuYJ);
		strDLDemesionData.Format(_T("%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f"), m_dDLCapCanX, m_dDLCapCanY, m_dDLTabEdgeGapY, m_dDLCapWeldX, m_dDLCapWeldY, m_dDLWeldLenth, m_dDLWeldWidth, m_dDLCapTabLeftX, m_dDLCapTabRightX, m_dDLCapTabTopY, m_dDLWeldCanY);
	}

	strTab.Format(_T("%.3f,%.3f,%.3f"),dCenterTabL,dCenterTabR,dCenterTabT);
	strWelding.Format(_T("%.1f,%.1f"),m_Project.m_dBlackPer[0],m_Project.m_dBlackPer1[0]);

	CString strtemp;
	
	m_dData[2] *= m_pDlgSet->m_FormTab2->m_dCamScale;
	m_dData[4] *= m_pDlgSet->m_FormTab2->m_dCamScale;
	m_dData[20]*= m_pDlgSet->m_FormTab2->m_dCamScale;
	m_dData[10]*= m_pDlgSet->m_FormTab2->m_dCamScale;
	m_dData[11]*= m_pDlgSet->m_FormTab2->m_dCamScale; 
	m_dData[1]*= m_pDlgSet->m_FormTab2->m_dCamScale;
	m_dData[19]*= m_pDlgSet->m_FormTab2->m_dCamScale;
	m_dData[12]*= m_pDlgSet->m_FormTab2->m_dCamScale;
	m_dData[3]*= m_pDlgSet->m_FormTab2->m_dCamScale;
	m_dData[18]*= m_pDlgSet->m_FormTab2->m_dCamScale;

	CString strFinal;
	strFinal.Format(_T("OK"));

	if (bDeepLearning) {
		if (nRBResult != RET_OK || nDLResult != 1)
			strFinal.Format(_T("NG"));
	}
	else {
		if (nRBResult != RET_OK)
			strFinal.Format(_T("NG"));
	}
	//str11.Format(_T("%s,%010d,%d,%d,%d,%d,%s,%s,%.1f,%.1f,%.1f,%.1f,%.1f,%.1f,%.1f,%.1f,%.1f,%.1f,%.1f,%.1f,%.1f,%.1f,%.1f,%.1f,%.1f,%.1f"),	kk6,m_nOKCount+m_nNgCount,m_nInspTime,m_nTopCapCenterAve,70,3,strNGNum, strAINg, m_dData[1],m_dData[2],m_dData[3],m_dData[4],m_dData[5],m_dData[6],m_dData[7],m_dData[8],m_dData[9],m_dData[10],m_dData[11],m_dData[12],m_dData[13],m_dData[14],m_dData[15],m_dData[16],m_dData[17],m_dData[18]);

	str11.Format(_T("%s,%s,%010d,%s,%.1f,%.1f,%s,%s,%.1f,%.1f,%s,%.1f,%.1f,%.1f,%.1f,%s,%.1f,%.1f,%.1f,%.1f,%.1f,%.1f,%.1f,%.1f,%.1f,%.1f"),
		strFinal, kk6, m_nOKCount + m_nNgCount - 1, strNGNum, m_dData[22], m_dData[23], "x", "x", m_dData[21], m_dData[25], "x", m_dData[28], m_dData[26], m_dData[24], m_dData[27], "x", m_dData[2], m_dData[4], m_dData[20], m_dData[10], m_dData[11], m_dData[1], m_dData[19], m_dData[12], m_dData[3], m_dData[18]);

	if (bDeepLearning)
		str11.Format(_T("%s,%s,%010d,%s,%.1f,%.1f,%s,%s,%.1f,%.1f,%s,%.1f,%.1f,%.1f,%.1f,%s,%.1f,%.1f,%.1f,%.1f,%.1f,%.1f,%.1f,%.1f,%.1f,%.1f,%s,%s"),
			strFinal,kk6,m_nOKCount+m_nNgCount-1,strNGNum,m_dData[22],m_dData[23],"x","x",m_dData[21],m_dData[25],"x",m_dData[28],m_dData[26],m_dData[24],m_dData[27],"x",m_dData[2],m_dData[4],m_dData[20],m_dData[10],m_dData[11], m_dData[1],m_dData[19],m_dData[12],m_dData[3],m_dData[18], strDLDefectData, strDLDemesionData);
	
	
	//년월일시분초,프레임,검사시간,밝기,샤프니스값,CAM 3,NG, CAP_CAN_X,CAP_CAN_Y,WELD_CAN_DIST,WELD_TOPCAP_DIST,CAP_EDGE_V,EWAK_V,TAB_EDGE_V,WELD_CHECK,CAN,TAB_L,TAB_R,TAB_T,LINE1_R,LINE1_S,WELDING_LEFT,WELDING_RIGHT,CAP_BRIGHTNESS, Tab_Gap_Y
	//		1		2		3		4		5		6	7		8		9				10		11					12		13		14       15			16    17     18    19    20      21    22				23			24				25

	if(!file.Open(filename, CFile::modeReadWrite|CFile::typeText)) 
	{		
		if(file.Open(filename, CFile::modeCreate|CFile::modeReadWrite|CFile::typeText))
		{

			//strheader.Format(_T("년,월,일,시,분,초,일련번호,Process Time,Gray Level,Sharpness,Cam_Number,Result,AI Result, CAP_CAN_X,CAP_CAN_Y,WELD_CAN_DIST,WELD_TOPCAP_DIST,CAP_EDGE_V,EWAK_V,TAB_EDGE_V,WELD_CHECK,CAN,TAB_L,TAB_R,TAB_T,LINE1_R,LINE1_S,WELDING_LEFT,WELDING_RIGHT,CAP_BRIGHTNESS,Tab_Gap_Y"));
			//strheader.Format(_T("년,월,일,시,분,초,일련번호,Process Time,Gray Level,Sharpness,Cam_Number,Result,AI Result, CAP_CAN_X,CAP_CAN_Y,WELD_CAN_DIST,WELD_TOPCAP_DIST,CAP_EDGE_V,EWAK_V,TAB_EDGE_V,WELD_CHECK,CAN,TAB_L,TAB_R,TAB_T,LINE1_R,LINE1_S,WELDING_LEFT,WELDING_RIGHT,CAP_BRIGHTNESS,Tab_Gap_Y"));
			//strheader.Format(_T("NO,최종판정,년,월,일,시,분,초,V_ID,V_판정결과,V_TC유무,V_TC뒤집힘,V_TC기울어짐,V_TC_CID필터유무,V_CAN유무,V_TAB유무,V_TAB뜯김,V_상태불량(오염등),V_용접영역TAB벗어남,V_약용접_유무,V_과용접_유무,V_그을음_유무,V_Tc_to_Can_Y,V_Tc_to_Weld_Y,V_Weld_With,V_Tc_to_Tab_L,V_Tc_to_Tab_R,V_Tc_to_Can_X,V_Tc_to_Weld_X,V_Tc_to_TabTop_Y,V_Weld_to_Can_Y,V_Tab끝단(왼)-Tab끝단(오)_Y축편차,D_ID,D_판정결과,D_TC유무,D_TC 뒤집힘,D_TC기울어짐,D_TC_CID필터유무,D_CAN유무,D_TAB유무,D_TAB뜯김,D_상태불량(오염등),D_용접영역TAB 벗어남,D_약용접_유무,D_과용접_유무,D_그을음_유무,D_Tc_to_Can_Y,D_Tc_to_Weld_Y,D_Weld_With,D_Tc_to_Tab_L,D_Tc_to_Tab_R,D_Tc_to_Can_X,D_Tc_to_Weld_X ,D_Tc_to_TabTop_Y,D_Weld_to_Can_Y,D_Tab끝단(왼)-Tab끝단(오)_Y축편차,용접면적/길이,탭 좌상단 부분과 용접 좌상단 부분의 x축거리,탭 우상단부분과 용접 우상단 부분의 x축거리"));
			strheader.Format(_T("NO,최종판정,년,월,일,시,분,초,V_ID,V_판정결과,V_TC유무,V_TC뒤집힘,V_TC기울어짐,V_TC_CID필터유무,V_CAN유무,V_TAB유무,V_TAB뜯김,V_상태불량(오염등),V_용접영역TAB벗어남,V_약용접_유무,V_과용접_유무,V_그을음_유무,V_Tc_to_Can_Y,V_Tc_to_Weld_Y,V_Weld_With,V_Tc_to_Tab_L,V_Tc_to_Tab_R,V_Tc_to_Can_X,V_Tc_to_Weld_X,V_Tc_to_TabTop_Y,V_Weld_to_Can_Y,V_Tab끝단(왼)-Tab끝단(오)_Y축편차"));
			if(bDeepLearning)
				strheader.Format(_T("NO,최종판정,년,월,일,시,분,초,V_ID,V_판정결과,V_TC유무,V_TC뒤집힘,V_TC기울어짐,V_TC_CID필터유무,V_CAN유무,V_TAB유무,V_TAB뜯김,V_상태불량(오염등),V_용접영역TAB벗어남,V_약용접_유무,V_과용접_유무,V_그을음_유무,V_Tc_to_Can_Y,V_Tc_to_Weld_Y,V_Weld_With,V_Tc_to_Tab_L,V_Tc_to_Tab_R,V_Tc_to_Can_X,V_Tc_to_Weld_X,V_Tc_to_TabTop_Y,V_Weld_to_Can_Y,V_Tab끝단(왼)-Tab끝단(오)_Y축편차,D_RET,D_isTC ,D_TC Reverse,D_TC Slatned,D_TC_isCID,D_isCAN,D_isTAB,D_TAB_Defect,D_ETC,D_WELD_Pos,D_WELD_Weak,D_WELD_Strong,D_WELD_Soot,TC_CAN_X,TC_CAN_Y,Tab_Edge_Gap_Y,TC_WELD_X,TC_WELD_Y,YJ_Length,YJ_Width,TC_TAB_LEFT,TC_TAB_RIGHT,TC_TAB_TOP)Y,WELD_CAN_Y"));

			file.WriteString(strheader);	
			nNOCnt =1;
			strtemp.Format(_T("\n%d,"),nNOCnt);
			file.WriteString(strtemp);	
			//file.WriteString(str);	
			file.WriteString(str11);	
			file.Close();
			
		}
	}
	else
	{
		file.SeekToEnd();
		strtemp.Format(_T("\n%d,"),nNOCnt);
		file.WriteString(strtemp);
		//		file.WriteString(str);
		file.WriteString(str11);
		file.Close(); 
	}
	nNOCnt++;

}

void CFlyCapture2_EXDlg::WriteID(int nCount, BOOL bResult, CString strCellID, CString strLotID)
{
	CStdioFile   file;
	CString filename;
	CString strpath;
	CString strheader;
	CString strTmp;
	CString strResult=_T("OK");
	
	//CTime ttime = CTime::GetCurrentTime();
	//CString kk = ttime.Format("%Y%m%d");
	//CString kk2 = ttime.Format("%Y%m");
	//CString kk3 = ttime.Format("%d");
	//CString kk1 = ttime.Format("%Y%m%d_%H%M%S");
	//CString kk4 = ttime.Format("%H%M%S");
	//CString kk5 = ttime.Format("%H");

	SYSTEMTIME sysTime;
	GetLocalTime(&sysTime);
	CString strYearMonsy; 
	CString strDay;
	CString strHour;
	CString strMS;
	strYearMonsy.Format(_T("%04d%02d"), sysTime.wYear, sysTime.wMonth);
	strDay.Format(_T("%02d"), sysTime.wDay);
	strHour.Format(_T("%02d"), sysTime.wHour);
	strMS.Format(_T("%04d%02d%02d_%02d%02d%02d%03d"), sysTime.wYear, sysTime.wMonth, sysTime.wDay, sysTime.wHour, sysTime.wMinute, sysTime.wSecond, sysTime.wMilliseconds);

	//CString kk3 = ttime.Format("%d");
	//CString kk1 = ttime.Format("%Y%m%d_%H%M%S");
	//CString kk4 = ttime.Format("%H%M%S");
	//CString kk5 = ttime.Format("%H");

	strpath.Format(_T("d:\\Data\\"));
	CreateDirectory(strpath,NULL);
	strpath.Format(_T("d:\\Data\\ID\\"));
	CreateDirectory(strpath,NULL);
	strpath.Format(_T("d:\\Data\\ID\\%s\\"),strYearMonsy);//년월
	CreateDirectory(strpath,NULL);
	strpath.Format(_T("d:\\Data\\ID\\%s\\%s\\"),strYearMonsy,strDay);//일
	CreateDirectory(strpath,NULL);
	strpath.Format(_T("d:\\Data\\ID\\%s\\%s\\%s\\"),strYearMonsy,strDay,strHour);//time
	CreateDirectory(strpath,NULL);

	//strpath.Format("d:\\Data\\Image\\%s\\%s\\%s\\ID.csv",kk2,kk3,kk5,kk1);//time
	if(bResult)
		strResult = _T("NG");

	if(strCellID == "")
		strCellID = _T("NA");

	if(strLotID == "")
		strLotID = _T("NA");
	filename.Format(_T("%sID.csv"),strpath);
	
	CString strData;
	strData.Format(_T("%s,%d,%s,%s,%s"),strMS, nCount, strResult, strLotID, strCellID);

	static int nCnt = 1;
	if(!file.Open(filename, CFile::modeReadWrite|CFile::typeText)) 
	{		
		if(file.Open(filename, CFile::modeCreate|CFile::modeReadWrite|CFile::typeText))
		{

			strheader.Format(_T("Time,Count,Result,LOT ID, Cell ID"));
			file.WriteString(strheader);	
			nCnt =1;
			strTmp.Format(_T("\n"));
			file.WriteString(strTmp);
			file.WriteString(strData);

			file.Close();
		}
	}
	else
	{
		file.SeekToEnd();
		strTmp.Format(_T("\n"));
		file.WriteString(strTmp);
		file.WriteString(strData);	
		file.Close(); 

 
	}	
	nCnt++;

}

void CFlyCapture2_EXDlg::OnBnClickedCheck5()
{//m_bCylindrUse
	UpdateData();
	if(m_bCylindrUse)
		AddtoList("Cylinder Use Check",1);
	else
		AddtoList("Cylinder Use Un-Check",1);
}

void CFlyCapture2_EXDlg::TotalResetCnt()
{
	float nRData[3][24];
	 	for(int i=0; i<3; i++){
	 		for(int ii=0; ii<24; ii++){
	 			nRData[i][ii] = 0;
	 		}
	 	}
}


void CFlyCapture2_EXDlg::TotalNgCnt()
{
	float nNgData[3][24];
	for(int i=0; i<3; i++){
		for(int ii=0; ii<24; ii++){
			nNgData[i][ii] = 0;
		}
	}

}


//변수에다 테스트데이터 씀
void CFlyCapture2_EXDlg::NgTestData()
{
	
	float nNgData[3][24];

	for(int i=0; i<3; i++){
		for(int ii=0; ii<24; ii++){
			nNgData[i][ii] = 0.0;
		}
	}

	nNgData[0][0] = 5.0;
	nNgData[0][1] = 2.4;
	nNgData[0][2] = 1.8;
	nNgData[0][3] = 1.7;
	nNgData[0][4] = 1.7;
	nNgData[0][5] = 1.4;
	nNgData[0][6] = 2.4;
	nNgData[0][7] = 2.2;
	nNgData[0][8] = 1.3;
	nNgData[0][9] = 1.6;
	nNgData[0][10] = 2.1;
	nNgData[0][11] = 1.4;
	nNgData[0][12] = 1.8;
	nNgData[0][13] = 2.7;
	nNgData[0][14] = 1.7;
	nNgData[0][15] = 1.4;
	nNgData[0][16] = 1.4;
	nNgData[0][17] = 1.2;
	nNgData[0][18] = 1.3;
	nNgData[0][19] = 1.6;
	nNgData[0][20] = 1.4;
	nNgData[0][21] = 1.2;
	nNgData[0][22] = 1.3;
	nNgData[0][23] = 1.6;
	nNgData[0][24]=1.9;

	nNgData[1][0] = 2.6;
	nNgData[1][1] = 2.1;
	nNgData[1][2] = 1.2;
	nNgData[1][3] = 2.3;
	nNgData[1][4] = 2.1;
	nNgData[1][5] = 2.1;
	nNgData[1][6] = 2.9;
	nNgData[1][7] = 4.3;
	nNgData[1][8] = 2.1;
	nNgData[1][9] = 1.6;
	nNgData[1][10] = 1.6;
	nNgData[1][11] = 2.1;
	nNgData[1][12] = 1.2;
	nNgData[1][13] = 1.3;
	nNgData[1][14] = 2.1;
	nNgData[1][15] = 2.1;
	nNgData[1][16] = 1.9;
	nNgData[1][17] = 1.3;
	nNgData[1][18] = 2.1;
	nNgData[1][19] = 1.6;

	nNgData[2][0] = 1.8;
	nNgData[2][1] = 2.2;
	nNgData[2][2] = 1.5;
	nNgData[2][3] = 1.3;
	nNgData[2][4] = 1.5;
	nNgData[2][5] = 1.4;
	nNgData[2][6] = 1.4;
	nNgData[2][7] = 2.2;
	nNgData[2][8] = 1.3;
	nNgData[2][9] = 1.9;
	nNgData[2][10] = 1.8;
	nNgData[2][11] = 1.4;
	nNgData[2][12] = 1.8;
	nNgData[2][13] = 0.7;
	nNgData[2][14] = 1.7;
	nNgData[2][15] = 1.4;
	nNgData[2][16] = 0.4;
	nNgData[2][17] = 1.2;
	nNgData[2][18] = 1.3;
	nNgData[2][19] = 1.9;



	for(int i=0; i<3; i++){
		for(int ii=0; ii<24; ii++){
			m_GraphTestData[i][ii] = nNgData[i][ii];
			//m_ptrBadCnt[i][ii] = nData2;
			//nData2-=0.05;
		}
	}
}

void CFlyCapture2_EXDlg::ResetData()
{
	m_Project.m_nMultiFrame = 0;
	fnReset();

	InitBadInfo();

	m_nFullFrameCount = 0;
	//		int staddr=20;
	m_fFps = 0;
	m_Project.ResetFrame();
	m_nSendIO = 0;
	m_nOKNG[0]=m_nOKNG[1] = -1;
	m_nBufCount[0] = m_nBufCount[1] = 0;


	m_dTactTimeAve[0]=m_dTactTimeAve[1]=m_dTactTimeAve[2] = 0;
	//control Reset
	m_Project.m_dBrightness = 0;
	//Label 초기화
	LabelResult(-1, AI_READY);

	m_strIsCode = "";
	m_strModelName = "";
	m_strUpperLow = "";
	m_nNewCount = 0;
	m_Project.m_nNgCount=m_Project.m_nFrameCount = 0;
	m_Project.m_dInsTime = 0;
	m_nSaveCount = 0;
	m_strIsCode = "";
	m_nNg1Count=m_nNg2Count = 0;
	m_nNgCount = 0;
	m_nOKCount = 0;
	m_dYield = 0;
	m_dTactTime = 0;

	m_nRWFrameCount =  0;
	m_nRWOkCount = 0;
	m_nRWNgCount = 0;
	m_bRework = FALSE;
}

void CFlyCapture2_EXDlg::ResetDimensionData()
{
	m_nNgCode = 0;
	m_nNgType = 0;
	m_strCodeReal = "";
	m_strLotID = "NA";
	m_strCellID = "NA";

	for(int nDimensionCnt=0; nDimensionCnt<3; nDimensionCnt++){
		m_DimensionData[nDimensionCnt].strDimensiontName = _keyInspInfoJellyRoll[nDimensionCnt][0];
		m_DimensionData[nDimensionCnt].bExceptionNg = TRUE;
		m_DimensionData[nDimensionCnt].dValue = 0.0;
		m_DimensionData[nDimensionCnt].ptStartPos = CPoint(0,0);
		m_DimensionData[nDimensionCnt].ptEndPos = CPoint(0,0);
		m_DimensionData[nDimensionCnt].bResultNg = FALSE;
		if(nDimensionCnt == 0){
			m_DimensionData[nDimensionCnt].dUslValue = m_pDlgSet->m_FormTab4->m_dTapTolY1;
			m_DimensionData[nDimensionCnt].dLslValue = m_pDlgSet->m_FormTab4->m_dTapTolY2 * -1;
		}
		else if(nDimensionCnt == 1){
			m_DimensionData[nDimensionCnt].dUslValue = m_pDlgSet->m_FormTab4->m_dCapCandistX;
			m_DimensionData[nDimensionCnt].dLslValue = 0.0;
		}
		else if(nDimensionCnt == 2){
			m_DimensionData[nDimensionCnt].dUslValue = m_pDlgSet->m_FormTab4->m_dCenterOffset;
			m_DimensionData[nDimensionCnt].dLslValue = m_pDlgSet->m_FormTab4->m_dCenterOffset2;
		}
	}
}

void CFlyCapture2_EXDlg::SetDimensionData(int nInsNum, BOOL bException, double dDimensionValue, CPoint ptStart, CPoint ptEnd, BOOL bResult)
{
	//m_DimensionData[nInsNum].strDimensiontName = _keyInspInfoJellyRoll[nInsNum][0];
	m_DimensionData[nInsNum].bExceptionNg = bException;
	m_DimensionData[nInsNum].dValue = dDimensionValue;
	m_DimensionData[nInsNum].ptStartPos = ptStart;
	m_DimensionData[nInsNum].ptEndPos = ptEnd;
	m_DimensionData[nInsNum].bResultNg = bResult;	
}


void CFlyCapture2_EXDlg::ReadCamTimeRate(int nCam)//time\temp\폴더로 옴긴후 실행해야함.
{
	CStdioFile   file;
	CString filename;
	CString strpath("d:\\Data\\");
	CTime ttime = CTime::GetCurrentTime();
	CString kk = ttime.Format("%Y%m%d");
	CString kk2 = ttime.Format("%Y%m");
	CString kk3 = ttime.Format("%d");
	CString kk1 = ttime.Format("%Y%m%d_%H%M%S");
	CString kk11 = ttime.Format("%y");
	CString kk12 = ttime.Format("%m");
	CString kk13 = ttime.Format("%d");
	CString kk14 = ttime.Format("%H");
	CString kk15 = ttime.Format("%M");
	CString kk16 = ttime.Format("%S");

	CString kk17 = kk11+"년"+kk12+"."+kk13+"."+ kk14+"."+ kk15+"."+ kk16+".";

	CString strheader,str;
	int n1 = m_nOKCount;
	int n2 = m_DlgBarcode->m_nMakeCount+1;

	CreateDirectory(strpath,NULL);
	strpath.Format("d:\\Data\\");
	CreateDirectory(strpath,NULL);
	strpath.Format("d:\\Data\\Time\\");
	CreateDirectory(strpath,NULL);
	strpath.Format("d:\\Data\\Time\\Temp\\");
	CreateDirectory(strpath,NULL);
	strpath.Format("d:\\Data\\Time\\Temp\\%s\\",kk2);//년월
	CreateDirectory(strpath,NULL);
//	strpath.Format("d:\\Data\\Time\\Temp\\%s\\%s_TimeCount_0.txt",kk2,kk);//m_strLot);//m_nOKCount/pView->m_DlgBarcode->m_nMakeCount);

	CFileFind finder;
	BOOL isfind[3];
	CString strfile[3];
	int ntmp[20] = {0,};
	for(int k=0;k<3;k++){//3대 pc
		strfile[k].Format("d:\\Data\\Time\\Temp\\%s\\%s_TimeCount_%d.txt",kk2,kk,k+1);
		isfind[k] = finder.FindFile(strfile[k]);
		if(isfind[k]){
			if(file.Open(strfile[k], CFile::modeReadWrite|CFile::typeText))
			{
				int nCnt1= 0;
				//첫줄은 토탈
				file.ReadString(str);
				SepparateString(str,&nCnt1,ntmp);
				if(nCnt1<6){
					file.Close();return;
				}
				for(int i=0;i<nCnt1;i++)
					m_nTotalCount[k][i] = ntmp[i];//cam //hour //count //0ok	1ng	2tab	3hole 4blur 5dist	6notcap 
				for(int i=0;i<24;i++){
					file.ReadString(str);
					if(str == "") {
						file.Close();return;
					}
					SepparateString(str,&nCnt1,ntmp);
					if(nCnt1<8){
						file.Close();return;
					}
					for(int j=1;j<nCnt1;j++){
						m_nTimeCount[ntmp[0]][j-1] = ntmp[j];//cam //hour //count //0ok	1ng	2tab	3hole 4blur 5dist	6notcap 
					}
				}
				file.Close();
			}
			else
			{
				for(int i=0;i<10;i++)
					m_nTotalCount[k][i]=0;
				for(int i=0;i<24;i++){
					for(int j=0;j<10;j++)
						m_nTimeCount[i][j] = 0;
				}
			}
		}
		//NG 수율
		int nBadCount[5] = {0,};
		double dngrate = 0;
		//Graph time rate
		for(int i=0;i<24;i++){
			if(m_nTimeCount[i][0]+m_nTimeCount[i][1]>0)
				dngrate = (m_nTimeCount[i][1]-m_nTimeCount[i][2])/(double)(m_nTimeCount[i][0]+m_nTimeCount[i][1])*100;
			else dngrate = 0;
			nBadCount[0] +=  m_nTimeCount[i][2];//not cap
			nBadCount[1] +=  m_nTimeCount[i][3];//tab
			nBadCount[2] +=  m_nTimeCount[i][4];//hole
			nBadCount[3] +=  m_nTimeCount[i][5];//blur
			nBadCount[4] +=  m_nTimeCount[i][6];//dist
			m_GraphTestData[k][i] = dngrate;
			mDrawBadState[0]->SetData(k, i, m_GraphTestData[k][i]);//24 불량 율
		}
		
		mDrawBadState[0]->RedrawMap();
		//Graph Bar
		float nData2[5][5];
		nData2[0][k] = nBadCount[1];//tab
		nData2[1][k] = nBadCount[2];//tabnData2[0][nCam] = nBadCount[1];//hole
		nData2[2][k] = nBadCount[3];//tabnData2[0][nCam] = nBadCount[1];//blur
		nData2[3][k] = nBadCount[4];//tabnData2[0][nCam] = nBadCount[1];//dist

		for(int i=0; i<mDrawBadState[1]->m_nGroupCnt; i++){//4
			//		for(int ii=0; ii<mDrawBadState[1]->m_nDataCnt; ii++){//3
			//m_ptrTotalCnt[i][ii] = nData;
			mDrawBadState[1]->m_ptrBadCnt[i][k] = nData2[i][k];
			//		}
			//		mDrawBadState[1]->SetData(nCam, nHour1, m_GraphTestData[1][nHour1]);
		}
		for (int i=0;i<4;i++)//불량 카운트가 기존값 넘어가면 +5 woo
		{
			if (nData2[i][k]>=mDrawBadState[1]->m_nMaxUnit-3)
			{
				mDrawBadState[1]->m_nMaxUnit =mDrawBadState[1]->m_nMaxUnit+5;
				mDrawBadState[1]->Invalidate();//maxUnit이 변할때마다 실행
			}	

		}
		
		mDrawBadState[1]->RedrawMap();
		

		double dsum1 = 0,dsum2 = 0;
		for(int i=0;i<24;i++){
			if(m_nTimeCount[i][0]+m_nTimeCount[i][1]>0)
				dngrate = (m_nTimeCount[i][1]-m_nTimeCount[i][2])/(double)(m_nTimeCount[i][0]+m_nTimeCount[i][1])*100;
			else dngrate = 0;
			dsum1 += dngrate;
			dsum2 = (dsum1/(i+1));
			nBadCount[0] +=  m_nTimeCount[i][2];//not cap
			nBadCount[1] +=  m_nTimeCount[i][3];//tab
			nBadCount[2] +=  m_nTimeCount[i][4];//hole
			nBadCount[3] +=  m_nTimeCount[i][5];//blur
			nBadCount[4] +=  m_nTimeCount[i][6];//dist
			m_GraphTestData[k][i] = dsum2;
			mDrawBadState[2]->SetData(k, i, m_GraphTestData[k][i]);//24 불량 율
			// 		mDrawBadState[1]->SetData(0, nHour1, m_GraphTestData[1][nHour1]);
		}
		mDrawBadState[2]->RedrawMap();
		
	}
	

}
int CFlyCapture2_EXDlg::SepparateString(CString strinput, int * ncnt , int * tmpdata)
{
	for(int i=0;i<20;i++)tmpdata[i] = 0;//20개 고정
	*ncnt = 0;
	int nret = 1;
	CString strTok;    
	int sepCount =0;
	for(int i=0;i<strinput.GetLength();i++)	if(strinput.GetAt(i) == ',')	sepCount++;
	CString* temp1 = new CString[sepCount+1]; 
	int cnt=0;//,ndata = 7;//정확히는 
	while(AfxExtractSubString(strTok, strinput, cnt, ','))  
		temp1[cnt++] = strTok;// 해당 배열에 순차적으로 넣어준다.
	if(cnt<8){
		delete[] temp1;
		return 0;//error
	}
	for(int i=0;i<cnt;i++)
		tmpdata[i] = atoi(temp1[i]);
	delete[] temp1;
	*ncnt = cnt;
	return nret;
}


void CFlyCapture2_EXDlg::WriteTimeCount(int nCam)//0 1 2
{
	CStdioFile   file;
	CString filename;
	CString strpath("d:\\Data\\");
	CTime ttime = CTime::GetCurrentTime();
	CString kk = ttime.Format("%Y%m%d");
	CString kk2 = ttime.Format("%Y%m");
	CString kk3 = ttime.Format("%d");
	CString kk1 = ttime.Format("%Y%m%d_%H%M%S");
	CString kk11 = ttime.Format("%y");
	CString kk12 = ttime.Format("%m");
	CString kk13 = ttime.Format("%d");
	CString kk14 = ttime.Format("%H");
	CString kk15 = ttime.Format("%M");
	CString kk16 = ttime.Format("%S");

	CString kk17 = kk11+"년"+kk12+"."+kk13+"."+ kk14+"."+ kk15+"."+ kk16+".";

	CString strheader,str;
	int n1 = m_nOKCount;
	int n2 = m_DlgBarcode->m_nMakeCount+1;

	CreateDirectory(strpath,NULL);
	strpath.Format("d:\\Data\\");
	CreateDirectory(strpath,NULL);
	strpath.Format("d:\\Data\\Time\\");
	CreateDirectory(strpath,NULL);
	strpath.Format("d:\\Data\\Time\\%s\\",kk2);//년월
	CreateDirectory(strpath,NULL);
	strpath.Format("d:\\Data\\Time\\%s\\%s_TimeCount_%d.txt",kk2,kk,nCam);//m_strLot);//m_nOKCount/pView->m_DlgBarcode->m_nMakeCount);

	CString str1;
	filename.Format(_T("%s"),strpath);
	double dok = m_nOKCount;
	double dng = m_nNgCount;

//	시간,ok,ng,tab,hole,blur,Dist,notcap

	if(file.Open(filename, CFile::modeCreate|CFile::modeReadWrite|CFile::typeText))
	{//처음 24는 전체 개수 뒤 24는 시간별 개수
		str.Format("%d,%d,%d,%d,%d,%d,%d,%d",m_nOKCount+m_nNgCount,m_nOKCount,m_nNgCount,m_nBadCount[1],m_nBadCount[2],m_nBadCount[3],m_nBadCount[4],m_nBadCount[0]);
		file.WriteString(str);	file.WriteString("\n");				
		for(int i=0;i<24;i++){
			str.Format("%d,%d,%d,%d,%d,%d,%d,%d",i,m_nTimeCount[i][0],m_nTimeCount[i][1],m_nTimeCount[i][2],m_nTimeCount[i][3],m_nTimeCount[i][4],m_nTimeCount[i][5],m_nTimeCount[i][6]);
			file.WriteString(str);	file.WriteString("\n");				
		}
		file.Close();
	}

}

int	CFlyCapture2_EXDlg::CopyTimeFile(int nCam)
{
	int nret = 1;
	CTime ttime = CTime::GetCurrentTime();
	CString kk = ttime.Format("%Y%m%d");
	CString kk2 = ttime.Format("%Y%m");
//	strpath.Format("d:\\Data\\Time\\%s\\%s_TimeCount_%d.txt",kk2,kk,nCam);//m_strLot);//m_nOKCount/pView->m_DlgBarcode->m_nMakeCount);

	CString strsrc,strdst,str1;
	if(nCam==1)str1.Format("A");
	else if(nCam==2)str1.Format("B");
	else str1.Format("C");
	CString strcom;
	int nLen = m_strComname.GetLength();
	if(nLen<=0)return 0;
	strcom = m_strComname.Left(nLen-1);

	strsrc.Format("\\\\100.0.0.1%d\\Data\\Time\\%s\\%s_TimeCount_%d.txt",nCam,kk2,kk,nCam);//ipcheck 100.0.0.11 ~13
	strsrc.Format("\\\\%s%s\\Data\\Time\\%s\\%s_TimeCount_%d.txt",strcom,str1,kk2,kk,nCam);//ipcheck 100.0.0.11 ~13
	strdst.Format("d:\\Data\\Time\\Temp\\%s\\%s_TimeCount_%d.txt",kk2,kk,nCam);
	BOOL fc = CopyFile(strsrc,strdst,0);
	if(fc){
		CString str;
		str.Format("%s_TimeCount_%d.txt Copy",kk,nCam);
		AddtoList(str,1);

	}
	nret = fc;
	return nret;
}

void CFlyCapture2_EXDlg::OnBnClickedButton1()
{

}
void CFlyCapture2_EXDlg::WriteJsonInsp(int nTargetCam, CString strServerPath, SYSTEMTIME sysTime, int nLine, int nLane, BOOL bDefectNg, CString strDefectFIleName, BOOL bNG)
{
	int nVisionType = 8;//m_nVisionType;
	int nJsonClassification = static_cast<int>(JsonClassification::_INSPECTION);
	CString str(_T(""));
	double dTemp = 0;
	CString strTime;
	//strTime.Format(_T("%04d%02d%02d%02d%02d%02d%03d"), m_timeVisionOutput.wYear, m_timeVisionOutput.wMonth, m_timeVisionOutput.wDay, m_timeVisionOutput.wHour, m_timeVisionOutput.wMinute, m_timeVisionOutput.wSecond, m_timeVisionOutput.wMilliseconds);
	strTime.Format(_T("%04d%02d%02d%02d%02d%02d%03d"), sysTime.wYear, sysTime.wMonth, sysTime.wDay, sysTime.wHour, sysTime.wMinute, sysTime.wSecond, sysTime.wMilliseconds);

	rapidjson::StringBuffer strBuffer;
	rapidjson::Document jsonDoc;
	jsonDoc.SetObject();

	CString strValue(_T(""));
	rapidjson::Value valData; // Data
	rapidjson::Value valObjectMain; // Object-Main
	valObjectMain.SetObject();
	rapidjson::Value valObjectSub; // Object-Sub
	valObjectSub.SetObject();
	rapidjson::Value valArray; // Array
	valArray.SetArray();


	if(m_Project.m_ptCapCenter.x < 0)
		m_Project.m_ptCapCenter.x = 0;
	if(m_Project.m_ptCapCenter.y < 0)
		m_Project.m_ptCapCenter.y = 0;
	// 주의
	// Object, Array는 AddMember, PushBack, Move 의 매개변수로 한번 사용하면 Null 이 되어버림. 사용 불가 상태.
	// 다시 Set- 으로 지정해야함.

	// Doc- Basic
	strValue.Format(_T("")); valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
	jsonDoc.AddMember(_T("TXN_ID"), valData, jsonDoc.GetAllocator());

	strValue.Format(_T("IN_DATA")); valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
	jsonDoc.AddMember(_T("inDTName"), valData, jsonDoc.GetAllocator());

	strValue.Format(_T("")); valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
	jsonDoc.AddMember(_T("outDTName"), valData, jsonDoc.GetAllocator());

	strValue.Format(_T("%s"), _keyActID[nJsonClassification]); valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
	jsonDoc.AddMember(_T("actID"), valData, jsonDoc.GetAllocator());

	// Doc- Common Contents
	// (Main1) Object에 멤버 쭉 넣고
	strValue.Format(_T("%s"), _keyCategory[nJsonClassification]); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
	valObjectMain.AddMember(_T("CATEGORY"), valData, jsonDoc.GetAllocator());

	str.Format(_T("%04d%02d%02d%02d%02d%02d%03d"), m_timeVisionInput.wYear, m_timeVisionInput.wMonth, m_timeVisionInput.wDay, m_timeVisionInput.wHour, m_timeVisionInput.wMinute, m_timeVisionInput.wSecond, m_timeVisionInput.wMilliseconds);
	strValue.Format(str); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
	valObjectMain.AddMember(_T("VISION_INPUT_TIME"), valData, jsonDoc.GetAllocator());

	str.Format(strTime);
	strValue.Format(str); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
	valObjectMain.AddMember(_T("VISION_OUTPUT_TIME"), valData, jsonDoc.GetAllocator());

	dTemp = (m_timeVisionOutput.wSecond + m_timeVisionOutput.wMilliseconds * 0.001) - (m_timeVisionInput.wSecond + m_timeVisionInput.wMilliseconds * 0.001);
	strValue.Format(_T("%.3f"), dTemp); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
	valObjectMain.AddMember(_T("INSP_TACTTIME"), valData, jsonDoc.GetAllocator());

	dTemp = (m_timeInspEnd.wSecond + m_timeInspEnd.wMilliseconds * 0.001) - (m_timeInspStart.wSecond + m_timeInspStart.wMilliseconds * 0.001);
	strValue.Format(_T("%.3f"), dTemp); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
	valObjectMain.AddMember(_T("INSP_PROCESSING_TIME"), valData, jsonDoc.GetAllocator());

	strValue.Format(m_pDlgSet->m_strSelectProduct); valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
	valObjectMain.AddMember(_T("RECIPE_ID"), valData, jsonDoc.GetAllocator());

	strValue.Format(m_pDlgSet->m_FormTab1->m_strPgmVersion); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
	valObjectMain.AddMember(_T("INSP_SW_VERSION"), valData, jsonDoc.GetAllocator());

	strValue.Format(_T("")); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
	valObjectMain.AddMember(_T("AGENT_SW_VERSION"), valData, jsonDoc.GetAllocator());

	strValue.Format(_T("Y")); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
	valObjectMain.AddMember(_T("NG_OUT"), valData, jsonDoc.GetAllocator());

	strValue.Format(_T("%s"),m_strEqpID); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
	valObjectMain.AddMember(_T("EQP_ID"), valData, jsonDoc.GetAllocator());

	strValue.Format(_T("%s"),m_strEqpInspID); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
	valObjectMain.AddMember(_T("EQP_INSP_ID"), valData, jsonDoc.GetAllocator());

	strValue.Format(_T("ASSEMBLY")); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
	valObjectMain.AddMember(_T("PROCESS_GROUP"), valData, jsonDoc.GetAllocator());

	strValue.Format(_T("CSW")); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
	valObjectMain.AddMember(_T("PROCESS_NAME"), valData, jsonDoc.GetAllocator());

	strValue.Format(_T("%d"), m_pDlgSet->m_FormTab2->m_nLineNumber + 1); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
	valObjectMain.AddMember(_T("LINE_NUMBER"), valData, jsonDoc.GetAllocator());

	strValue.Format(_T("1")); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
	valObjectMain.AddMember(_T("MACHINE_NUMBER"), valData, jsonDoc.GetAllocator());

	strValue.Format(_T("%d"), m_pDlgSet->m_FormTab2->m_nLaneNumber + 1); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
	valObjectMain.AddMember(_T("LANE_NUMBER"), valData, jsonDoc.GetAllocator());

	strValue.Format(_T("%s"), _keyVisionType[nVisionType]); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
	valObjectMain.AddMember(_T("VISION_TYPE"), valData, jsonDoc.GetAllocator());

	strValue.Format(_T("RIGHT")); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
	valObjectMain.AddMember(_T("PROCESS_DIRECTION"), valData, jsonDoc.GetAllocator());

	strValue.Format(_T("%s"), m_strLotID); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
	valObjectMain.AddMember(_T("LOT_ID"), valData, jsonDoc.GetAllocator());

	strValue.Format(_T("%s"), m_strCellID); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
	valObjectMain.AddMember(_T("CELL_ID"), valData, jsonDoc.GetAllocator());

	strValue.Format(_T("%d"), m_nCellCount); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
	valObjectMain.AddMember(_T("CELL_COUNT_NO"), valData, jsonDoc.GetAllocator());

	str.Format(_T("OK"));
	if(m_nRBResult != RET_OK)
		str.Format(_T("NG"));
	strValue.Format(str); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
	valObjectMain.AddMember(_T("CELL_FINAL_JUDGE"), valData, jsonDoc.GetAllocator());

	// Doc- IQ Contents
	int nCamNum = 0; 
	int nForRange = 1;//m_nUseCameraCnt;

	int nFrameTotalCount = 1;//m_nTotalFrameCount[nCamNum];

	for (int nFrameCnt = 0; nFrameCnt < nFrameTotalCount; nFrameCnt++) {
		strValue.Format(_T("SIDE"));		valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
		valObjectSub.AddMember(_T("IQ_CAMERA_LOCATION"), valData, jsonDoc.GetAllocator());

		strValue.Format(_T("MONO-AREA"));
		valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
		valObjectSub.AddMember(_T("IQ_CAMERA_TYPE"), valData, jsonDoc.GetAllocator());

		strValue.Format(_T("%d"), nCamNum + 1); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
		valObjectSub.AddMember(_T("IQ_CAMERA_NUMBER"), valData, jsonDoc.GetAllocator());

		strValue.Format(_T("%d"), nFrameCnt + 1); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
		valObjectSub.AddMember(_T("IQ_SCREEN_NUMBER"), valData, jsonDoc.GetAllocator());

		int nImgWidth = CAMERA_WIDTH;
		strValue.Format(_T("%d"), nImgWidth); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
		valObjectSub.AddMember(_T("IQ_SCREEN_IMAGE_SIZE_X"), valData, jsonDoc.GetAllocator());

		int nImgHeight = CAMERA_HEIGHT;
		strValue.Format(_T("%d"), nImgHeight); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
		valObjectSub.AddMember(_T("IQ_SCREEN_IMAGE_SIZE_Y"), valData, jsonDoc.GetAllocator());

		strValue.Format(_T("50.000")); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
		valObjectSub.AddMember(_T("IQ_FOCUS_VALUE"), valData, jsonDoc.GetAllocator());

		strValue.Format(_T("50.000")); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
		valObjectSub.AddMember(_T("IQ_BRIGHT_VALUE"), valData, jsonDoc.GetAllocator());

		strValue.Format(_T("%.3f"), m_pDlgSet->m_FormTab2->m_dCamScale); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
		valObjectSub.AddMember(_T("IQ_RESOL_X_VALUE"), valData, jsonDoc.GetAllocator());

		strValue.Format(_T("%.3f"), m_pDlgSet->m_FormTab2->m_dCamScale); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
		valObjectSub.AddMember(_T("IQ_RESOL_Y_VALUE"), valData, jsonDoc.GetAllocator());

		strValue.Format(_T("0.000")); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
		valObjectSub.AddMember(_T("IQ_CAM_ANGLE_VALUE"), valData, jsonDoc.GetAllocator());

		strValue.Format(_T("0.000")); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
		valObjectSub.AddMember(_T("IQ_CAMERA_GAIN"), valData, jsonDoc.GetAllocator());

		strValue.Format(_T("%.3f"), double(m_pDlgSet->m_FormTab2->m_nMultiShutter[0])); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
		valObjectSub.AddMember(_T("IQ_EXPOSURE_TIME"), valData, jsonDoc.GetAllocator());

		str.Format(_T("OK"));
		if (m_nRBResult != RET_OK)
			str.Format(_T("NG"));

		strValue.Format(str); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
		valObjectSub.AddMember(_T("IMAGE_JUDGE"), valData, jsonDoc.GetAllocator());

		strValue.Format(_T("%s.jpg"), m_DisplayServer[0].m_strImgName); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
		valObjectSub.AddMember(_T("IMAGE_FILE_NAME"), valData, jsonDoc.GetAllocator());

		valArray.PushBack(valObjectSub, jsonDoc.GetAllocator());
		valObjectSub.SetObject();
	}

	valObjectMain.AddMember(_T("IQ_INFO"), valArray, jsonDoc.GetAllocator());
	valArray.SetArray();

	// Doc- Dimension Contents
	strValue.Format(_T("OK"));
	if (bNG)
		strValue.Format(_T("NG"));

	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
	valObjectMain.AddMember(_T("DIMENSION_JUDGE_RESULT"), valData, jsonDoc.GetAllocator());
	
	strValue.Format(_T("0"));
	if (bNG)
		strValue.Format(_T("1"));
	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
	valObjectMain.AddMember(_T("TOTAL_DIMENSION_NG_COUNT"), valData, jsonDoc.GetAllocator());

	if(bNG) {
		if(m_nNgCode == NG_TAP_POS)
			strValue.Format(_T("TABPOS_U1")); 	
		else if(m_nNgCode == NG_CAN_CAP_DIST_X)
			strValue.Format(_T("BCCDST_C1")); 	
		else if(m_nNgCode == NG_CAN_CAP_DIST_Y)
			strValue.Format(_T("BCCDST_C2")); 

		valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
		valArray.PushBack(valData, jsonDoc.GetAllocator());
		valObjectMain.AddMember(_T("DIMENSION_REASON_ALL"), valArray, jsonDoc.GetAllocator());
		valArray.SetArray();

		valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
		valArray.PushBack(valData, jsonDoc.GetAllocator());
		valObjectMain.AddMember(_T("DIMENSION_REASON_ALL_REAL"), valArray, jsonDoc.GetAllocator());
		valArray.SetArray();

		valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
		valObjectMain.AddMember(_T("DIMENSION_REASON_MAIN"), valData, jsonDoc.GetAllocator());

		valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
		valObjectMain.AddMember(_T("DIMENSION_REASON_MAIN_REAL"), valData, jsonDoc.GetAllocator());
		
		strValue.Format(_T("%.3f"),m_dNgValue);  	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
		valObjectMain.AddMember(_T("DIMENSION_MAIN_NG_VALUE"), valData, jsonDoc.GetAllocator());
	}
	else {
		valObjectMain.AddMember(_T("DIMENSION_REASON_ALL"), valArray, jsonDoc.GetAllocator());
		valArray.SetArray();

		valObjectMain.AddMember(_T("DIMENSION_REASON_ALL_REAL"), valArray, jsonDoc.GetAllocator());
		valArray.SetArray();

		strValue.Format(_T("")); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
		valObjectMain.AddMember(_T("DIMENSION_REASON_MAIN"), valData, jsonDoc.GetAllocator());

		strValue.Format(_T("")); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
		valObjectMain.AddMember(_T("DIMENSION_REASON_MAIN_REAL"), valData, jsonDoc.GetAllocator());

		strValue.Format(_T("")); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
		valObjectMain.AddMember(_T("DIMENSION_MAIN_NG_VALUE"), valData, jsonDoc.GetAllocator());
	}

	for (int nDimensionCnt = 0; nDimensionCnt < 3; nDimensionCnt++) {
		//[1]탭위치
		strValue.Format(_T("%s"), m_DimensionData[nDimensionCnt].strDimensiontName); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
		valObjectSub.AddMember(_T("DIMENSION_NAME"), valData, jsonDoc.GetAllocator());

		strValue.Format(_T("SIDE")); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
		valObjectSub.AddMember(_T("DIMENSION_CAMERA_LOCATION"), valData, jsonDoc.GetAllocator());

		strValue.Format(_T("1")); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
		valObjectSub.AddMember(_T("DIMENSION_CAMERA_NUMBER"), valData, jsonDoc.GetAllocator());

		strValue.Format(_T("1")); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
		valObjectSub.AddMember(_T("DIMENSION_SCREEN_NUMBER"), valData, jsonDoc.GetAllocator());

		strValue.Format(_T("TOP")); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
		valObjectSub.AddMember(_T("DIMENSION_INSP_REGION"), valData, jsonDoc.GetAllocator());

		if(m_DimensionData[nDimensionCnt].bExceptionNg){
			strValue.Format(_T("N")); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
			valObjectSub.AddMember(_T("DIMENSION_COMPLETE"), valData, jsonDoc.GetAllocator());

			strValue.Format(_T("")); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
			valObjectSub.AddMember(_T("DIMENSION_VALUE"), valData, jsonDoc.GetAllocator());

			strValue.Format(_T("%.3f"), m_DimensionData[nDimensionCnt].dUslValue); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
			valObjectSub.AddMember(_T("DIMENSION_USL"), valData, jsonDoc.GetAllocator());

			strValue.Format(_T("%.3f"), m_DimensionData[nDimensionCnt].dLslValue); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
			valObjectSub.AddMember(_T("DIMENSION_LSL"), valData, jsonDoc.GetAllocator());

			strValue.Format(_T("")); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
			valObjectSub.AddMember(_T("DIMENSION_START_POINT_X"), valData, jsonDoc.GetAllocator());

			strValue.Format(_T(""));  	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
			valObjectSub.AddMember(_T("DIMENSION_END_POINT_X"), valData, jsonDoc.GetAllocator());

			strValue.Format(_T(""));  	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
			valObjectSub.AddMember(_T("DIMENSION_START_POINT_Y"), valData, jsonDoc.GetAllocator());

			strValue.Format(_T(""));  	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
			valObjectSub.AddMember(_T("DIMENSION_END_POINT_Y"), valData, jsonDoc.GetAllocator());
		}
		else{
			strValue.Format(_T("Y")); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
			valObjectSub.AddMember(_T("DIMENSION_COMPLETE"), valData, jsonDoc.GetAllocator());

			strValue.Format(_T("%.3f"), m_DimensionData[nDimensionCnt].dValue); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
			valObjectSub.AddMember(_T("DIMENSION_VALUE"), valData, jsonDoc.GetAllocator());

			strValue.Format(_T("%.3f"), m_DimensionData[nDimensionCnt].dUslValue); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
			valObjectSub.AddMember(_T("DIMENSION_USL"), valData, jsonDoc.GetAllocator());

			strValue.Format(_T("%.3f"), m_DimensionData[nDimensionCnt].dLslValue); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
			valObjectSub.AddMember(_T("DIMENSION_LSL"), valData, jsonDoc.GetAllocator());

			strValue.Format(_T("%.3f"), m_DimensionData[nDimensionCnt].ptStartPos.x); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
			valObjectSub.AddMember(_T("DIMENSION_START_POINT_X"), valData, jsonDoc.GetAllocator());

			strValue.Format(_T("%.3f"), m_DimensionData[nDimensionCnt].ptEndPos.x);  	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
			valObjectSub.AddMember(_T("DIMENSION_END_POINT_X"), valData, jsonDoc.GetAllocator());

			strValue.Format(_T("%.3f"), m_DimensionData[nDimensionCnt].ptStartPos.y);  	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
			valObjectSub.AddMember(_T("DIMENSION_START_POINT_Y"), valData, jsonDoc.GetAllocator());

			strValue.Format(_T("%.3f"), m_DimensionData[nDimensionCnt].ptEndPos.y);  	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
			valObjectSub.AddMember(_T("DIMENSION_END_POINT_Y"), valData, jsonDoc.GetAllocator());
		}

		if(m_DimensionData[nDimensionCnt].bResultNg)
			strValue.Format(_T("NG"));
		else
			strValue.Format(_T("OK")); 	
		valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
		valObjectSub.AddMember(_T("DIMENSION_JUDGE"), valData, jsonDoc.GetAllocator());

		strValue.Format(_T("OUT"));
		valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
		valObjectSub.AddMember(_T("DIMENSION_INOUT"), valData, jsonDoc.GetAllocator());

		strValue.Format(_T("")); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
		valObjectSub.AddMember(_T("DIMENSION_GL"), valData, jsonDoc.GetAllocator());

		strValue.Format(_T("")); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
		valObjectSub.AddMember(_T("DIMENSION_OFFSET"), valData, jsonDoc.GetAllocator());

		valArray.PushBack(valObjectSub, jsonDoc.GetAllocator());
		valObjectSub.SetObject();

	}
	//[1]탭위치
	strValue.Format(_T("TABPOS_U1")); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
	valObjectSub.AddMember(_T("DIMENSION_NAME"), valData, jsonDoc.GetAllocator());

	strValue.Format(_T("SIDE")); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
	valObjectSub.AddMember(_T("DIMENSION_CAMERA_LOCATION"), valData, jsonDoc.GetAllocator());

	strValue.Format(_T("1")); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
	valObjectSub.AddMember(_T("DIMENSION_CAMERA_NUMBER"), valData, jsonDoc.GetAllocator());

	strValue.Format(_T("1")); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
	valObjectSub.AddMember(_T("DIMENSION_SCREEN_NUMBER"), valData, jsonDoc.GetAllocator());

	strValue.Format(_T("TOP")); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
	valObjectSub.AddMember(_T("DIMENSION_INSP_REGION"), valData, jsonDoc.GetAllocator());

	strValue.Format(_T("")); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
	valObjectSub.AddMember(_T("DIMENSION_COMPLETE"), valData, jsonDoc.GetAllocator());

	strValue.Format(_T("%.3f"), m_dCapCanDist); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
	valObjectSub.AddMember(_T("DIMENSION_VALUE"), valData, jsonDoc.GetAllocator());

	strValue.Format(_T("%.3f"), m_pDlgSet->m_FormTab4->m_dTapTolY1); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
	valObjectSub.AddMember(_T("DIMENSION_USL"), valData, jsonDoc.GetAllocator());

	strValue.Format(_T("%.3f"), m_pDlgSet->m_FormTab4->m_dTapTolY2 * -1); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
	valObjectSub.AddMember(_T("DIMENSION_LSL"), valData, jsonDoc.GetAllocator());

	if(m_Project.m_ptCapCenter.x < 0) m_Project.m_ptCapCenter.x = 0;
	strValue.Format(_T("%.3f"), m_Project.m_ptCapCenter.x); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
	valObjectSub.AddMember(_T("DIMENSION_START_POINT_X"), valData, jsonDoc.GetAllocator());

	if(m_Project.m_ptCapCenter.x < 0) m_Project.m_ptCapCenter.x = 0;
	strValue.Format(_T("%.3f"),  m_Project.m_ptCapCenter.x);  	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
	valObjectSub.AddMember(_T("DIMENSION_END_POINT_X"), valData, jsonDoc.GetAllocator());

	if(m_Project.m_ptCapCenter.y < 0) m_Project.m_ptCapCenter.y = 0;
	strValue.Format(_T("%.3f"), m_Project.m_ptCapCenter.y);  	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
	valObjectSub.AddMember(_T("DIMENSION_START_POINT_Y"), valData, jsonDoc.GetAllocator());

	if(m_Project.m_nTopPos[0] < -999) m_Project.m_nTopPos[0] = 0;
	strValue.Format(_T("%.3f"), m_Project.m_nTopPos[0]);  	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
	valObjectSub.AddMember(_T("DIMENSION_END_POINT_Y"), valData, jsonDoc.GetAllocator());

	if(m_nNgCode == NG_TAP_POS)
		strValue.Format(_T("NG"));
	else
		strValue.Format(_T("OK")); 	
	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
	valObjectSub.AddMember(_T("DIMENSION_JUDGE"), valData, jsonDoc.GetAllocator());

	strValue.Format(_T("OUT"));
	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
	valObjectSub.AddMember(_T("DIMENSION_INOUT"), valData, jsonDoc.GetAllocator());

	strValue.Format(_T("")); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
	valObjectSub.AddMember(_T("DIMENSION_GL"), valData, jsonDoc.GetAllocator());

	strValue.Format(_T("")); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
	valObjectSub.AddMember(_T("DIMENSION_OFFSET"), valData, jsonDoc.GetAllocator());

	valArray.PushBack(valObjectSub, jsonDoc.GetAllocator());
	valObjectSub.SetObject();

	//[2]CAN-CAP 중심거리 X
	strValue.Format(_T("BCCDST_C1")); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
	valObjectSub.AddMember(_T("DIMENSION_NAME"), valData, jsonDoc.GetAllocator());

	strValue.Format(_T("SIDE")); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
	valObjectSub.AddMember(_T("DIMENSION_CAMERA_LOCATION"), valData, jsonDoc.GetAllocator());

	strValue.Format(_T("1")); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
	valObjectSub.AddMember(_T("DIMENSION_CAMERA_NUMBER"), valData, jsonDoc.GetAllocator());

	strValue.Format(_T("1")); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
	valObjectSub.AddMember(_T("DIMENSION_SCREEN_NUMBER"), valData, jsonDoc.GetAllocator());

	strValue.Format(_T("TOP")); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
	valObjectSub.AddMember(_T("DIMENSION_INSP_REGION"), valData, jsonDoc.GetAllocator());

	strValue.Format(_T("")); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
	valObjectSub.AddMember(_T("DIMENSION_COMPLETE"), valData, jsonDoc.GetAllocator());

	strValue.Format(_T("%.3f"), m_dCapCanDistX); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
	valObjectSub.AddMember(_T("DIMENSION_VALUE"), valData, jsonDoc.GetAllocator());

	strValue.Format(_T("%.3f"), m_pDlgSet->m_FormTab4->m_dCapCandistX); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
	valObjectSub.AddMember(_T("DIMENSION_USL"), valData, jsonDoc.GetAllocator());

	strValue.Format(_T("")); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
	valObjectSub.AddMember(_T("DIMENSION_LSL"), valData, jsonDoc.GetAllocator());

	if(m_Project.m_ptCapCenter.x < 0) m_Project.m_ptCapCenter.x = 0;
	strValue.Format(_T("%.3f"), m_Project.m_ptCapCenter.x); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
	valObjectSub.AddMember(_T("DIMENSION_START_POINT_X"), valData, jsonDoc.GetAllocator());

	if(m_ptCanTop.x < 0) m_ptCanTop.x = 0;
	strValue.Format(_T("%.3f"),  m_ptCanTop.x);  	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
	valObjectSub.AddMember(_T("DIMENSION_END_POINT_X"), valData, jsonDoc.GetAllocator());

	if(m_Project.m_ptCapCenter.y < 0) m_Project.m_ptCapCenter.y = 0;
	strValue.Format(_T("%.3f"), m_Project.m_ptCapCenter.y);  	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
	valObjectSub.AddMember(_T("DIMENSION_START_POINT_Y"), valData, jsonDoc.GetAllocator());

	if(m_ptCanTop.y < 0) m_ptCanTop.y = 0;
	strValue.Format(_T("%.3f"), m_ptCanTop.y);  	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
	valObjectSub.AddMember(_T("DIMENSION_END_POINT_Y"), valData, jsonDoc.GetAllocator());

	if( m_nNgCode == NG_CAN_CAP_DIST_X)
		strValue.Format(_T("NG"));
	else
		strValue.Format(_T("OK")); 	
	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
	valObjectSub.AddMember(_T("DIMENSION_JUDGE"), valData, jsonDoc.GetAllocator());

	strValue.Format(_T("OUT"));
	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
	valObjectSub.AddMember(_T("DIMENSION_INOUT"), valData, jsonDoc.GetAllocator());

	strValue.Format(_T("")); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
	valObjectSub.AddMember(_T("DIMENSION_GL"), valData, jsonDoc.GetAllocator());

	strValue.Format(_T("")); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
	valObjectSub.AddMember(_T("DIMENSION_OFFSET"), valData, jsonDoc.GetAllocator());

	valArray.PushBack(valObjectSub, jsonDoc.GetAllocator());
	valObjectSub.SetObject();
	//[3]CAN-CAP 중심거리 Y
	strValue.Format(_T("BCCDST_C2")); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
	valObjectSub.AddMember(_T("DIMENSION_NAME"), valData, jsonDoc.GetAllocator());

	strValue.Format(_T("SIDE")); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
	valObjectSub.AddMember(_T("DIMENSION_CAMERA_LOCATION"), valData, jsonDoc.GetAllocator());

	strValue.Format(_T("1")); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
	valObjectSub.AddMember(_T("DIMENSION_CAMERA_NUMBER"), valData, jsonDoc.GetAllocator());

	strValue.Format(_T("1")); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
	valObjectSub.AddMember(_T("DIMENSION_SCREEN_NUMBER"), valData, jsonDoc.GetAllocator());

	strValue.Format(_T("TOP")); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
	valObjectSub.AddMember(_T("DIMENSION_INSP_REGION"), valData, jsonDoc.GetAllocator());

	strValue.Format(_T("")); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
	valObjectSub.AddMember(_T("DIMENSION_COMPLETE"), valData, jsonDoc.GetAllocator());

	strValue.Format(_T("%.3f"), m_dCapCanDistY); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
	valObjectSub.AddMember(_T("DIMENSION_VALUE"), valData, jsonDoc.GetAllocator());

	strValue.Format(_T("%.3f"), m_pDlgSet->m_FormTab4->m_dCenterOffset); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
	valObjectSub.AddMember(_T("DIMENSION_USL"), valData, jsonDoc.GetAllocator());

	strValue.Format(_T("%.3f"), m_pDlgSet->m_FormTab4->m_dCenterOffset2); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
	valObjectSub.AddMember(_T("DIMENSION_LSL"), valData, jsonDoc.GetAllocator());

	if(m_Project.m_ptCapCenter.x < 0) m_Project.m_ptCapCenter.x = 0;
	strValue.Format(_T("%.3f"), m_Project.m_ptCapCenter.x); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
	valObjectSub.AddMember(_T("DIMENSION_START_POINT_X"), valData, jsonDoc.GetAllocator());

	if(m_ptCanTop.x < 0) m_ptCanTop.x = 0;
	strValue.Format(_T("%.3f"),  m_ptCanTop.x);  	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
	valObjectSub.AddMember(_T("DIMENSION_END_POINT_X"), valData, jsonDoc.GetAllocator());

	if(m_Project.m_ptCapCenter.y < 0) m_Project.m_ptCapCenter.y = 0;
	strValue.Format(_T("%.3f"), m_Project.m_ptCapCenter.y);  	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
	valObjectSub.AddMember(_T("DIMENSION_START_POINT_Y"), valData, jsonDoc.GetAllocator());

	if(m_ptCanTop.y < 0) m_ptCanTop.y = 0;
	strValue.Format(_T("%.3f"), m_ptCanTop.y);  	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
	valObjectSub.AddMember(_T("DIMENSION_END_POINT_Y"), valData, jsonDoc.GetAllocator());

	if( m_nNgCode == NG_CAN_CAP_DIST_Y)
		strValue.Format(_T("NG"));
	else
		strValue.Format(_T("OK")); 	
	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
	valObjectSub.AddMember(_T("DIMENSION_JUDGE"), valData, jsonDoc.GetAllocator());

	strValue.Format(_T("OUT"));
	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
	valObjectSub.AddMember(_T("DIMENSION_INOUT"), valData, jsonDoc.GetAllocator());

	strValue.Format(_T("")); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
	valObjectSub.AddMember(_T("DIMENSION_GL"), valData, jsonDoc.GetAllocator());

	strValue.Format(_T("")); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
	valObjectSub.AddMember(_T("DIMENSION_OFFSET"), valData, jsonDoc.GetAllocator());

	valArray.PushBack(valObjectSub, jsonDoc.GetAllocator());
	valObjectSub.SetObject();

	valObjectMain.AddMember(_T("DIMENSION_INFO"), valArray, jsonDoc.GetAllocator());
	valArray.SetArray();

	// Doc- Appearance Contents
	strValue.Format(_T("OK")); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
	valObjectMain.AddMember(_T("APPEARANCE_JUDGE_RESULT"), valData, jsonDoc.GetAllocator());

	strValue.Format(_T("0")); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
	valObjectMain.AddMember(_T("TOTAL_APPEARANCE_NG_COUNT"), valData, jsonDoc.GetAllocator());

	strValue.Format(_T("0")); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
	valObjectMain.AddMember(_T("HIDDEN_APPEARANCE_NG_COUNT"), valData, jsonDoc.GetAllocator());

	if (FALSE) {
		strValue.Format(_T("HOAR")); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
		valArray.PushBack(valData, jsonDoc.GetAllocator());

		valObjectMain.AddMember(_T("APPEARANCE_REASON_ALL"), valArray, jsonDoc.GetAllocator());
		valArray.SetArray();
	}
	else {
		valObjectMain.AddMember(_T("APPEARANCE_REASON_ALL"), valArray, jsonDoc.GetAllocator());
		valArray.SetArray();
		valObjectMain.AddMember(_T("APPEARANCE_REASON_ALL_REAL"), valArray, jsonDoc.GetAllocator());
		valArray.SetArray();
	}
	strValue.Format(_T("")); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
	valObjectMain.AddMember(_T("APPEARANCE_REASON_MAIN"), valData, jsonDoc.GetAllocator());

	strValue.Format(_T("")); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
	valObjectMain.AddMember(_T("APPEARANCE_REASON_MAIN_REAL"), valData, jsonDoc.GetAllocator());


	if(bDefectNg){
			strValue.Format(_T("1")); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
			valObjectSub.AddMember(_T("DEFECT_INDEX"), valData, jsonDoc.GetAllocator());

			strValue.Format(_T("CANMSS")); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
			valObjectSub.AddMember(_T("DEFECT_TYPE_NAME"), valData, jsonDoc.GetAllocator());

			strValue.Format(_T("%s"),m_strCodeReal); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
			valObjectSub.AddMember(_T("DEFECT_TYPE_NAME_REAL"), valData, jsonDoc.GetAllocator());

			strValue.Format(_T("CANMSS")); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
			valObjectSub.AddMember(_T("DEFECT_TYPE_RULE_BASE_NAME"), valData, jsonDoc.GetAllocator());

			strValue.Format(_T("%s"),m_strCodeReal); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
			valObjectSub.AddMember(_T("DEFECT_TYPE_RULE_BASE_NAME_REAL"), valData, jsonDoc.GetAllocator());

			strValue.Format(_T("OUT")); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
			valObjectSub.AddMember(_T("DEFECT_INOUT"), valData, jsonDoc.GetAllocator());

			strValue.Format(_T("SIDE")); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
			valObjectSub.AddMember(_T("DEFECT_CAMERA_LOCATION"), valData, jsonDoc.GetAllocator());

			strValue.Format(_T("1")); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
			valObjectSub.AddMember(_T("DEFECT_CAMERA_NUMBER"), valData, jsonDoc.GetAllocator());

			strValue.Format(_T("1")); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
			valObjectSub.AddMember(_T("DEFECT_SCREEN_NUMBER"), valData, jsonDoc.GetAllocator());

			strValue.Format(_T("TOP")); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
			valObjectSub.AddMember(_T("DEFECT_INSP_REGION"), valData, jsonDoc.GetAllocator());

			strValue.Format(_T("TOPCAP")); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
			valObjectSub.AddMember(_T("DEFECT_INSP_POSITION"), valData, jsonDoc.GetAllocator());

			strValue.Format(_T("%.3f"), double(m_pDlgSet->m_FormTab4->m_rtTacker[0].CenterPoint().x)); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
			valObjectSub.AddMember(_T("DEFECT_CRITERIA_POINT_X"), valData, jsonDoc.GetAllocator());

			strValue.Format(_T("%.3f"), double(m_pDlgSet->m_FormTab4->m_rtTacker[0].CenterPoint().y));	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
			valObjectSub.AddMember(_T("DEFECT_CRITERIA_POINT_Y"), valData, jsonDoc.GetAllocator());

			strValue.Format(_T("0.000")); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
			valObjectSub.AddMember(_T("DEFECT_RELATIVE_POINT_R_PX"), valData, jsonDoc.GetAllocator());

			strValue.Format(_T("0.000")); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
			valObjectSub.AddMember(_T("DEFECT_RELATIVE_POINT_ANGLE"), valData, jsonDoc.GetAllocator());

			strValue.Format(_T("%.3f"), double(m_pDlgSet->m_FormTab4->m_rtTacker[0].CenterPoint().x)); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
			valObjectSub.AddMember(_T("DEFECT_ABSOLUTE_IMG_X"), valData, jsonDoc.GetAllocator());

			strValue.Format(_T("%.3f"), double(m_pDlgSet->m_FormTab4->m_rtTacker[0].CenterPoint().y)); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
			valObjectSub.AddMember(_T("DEFECT_ABSOLUTE_IMG_Y"), valData, jsonDoc.GetAllocator());
			
			strValue.Format(_T("%.3f"), double(m_pDlgSet->m_FormTab4->m_rtTacker[0].Width())); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
			valObjectSub.AddMember(_T("DEFECT_IMG_WIDTH"), valData, jsonDoc.GetAllocator());

			strValue.Format(_T("%.3f"), double(m_pDlgSet->m_FormTab4->m_rtTacker[0].Height())); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
			valObjectSub.AddMember(_T("DEFECT_IMG_LENGTH"), valData, jsonDoc.GetAllocator());

			strValue.Format(_T("%.3f"), double(m_pDlgSet->m_FormTab4->m_rtTacker[0].Width())); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
			valObjectSub.AddMember(_T("DEFECT_AREA_PIXELS"), valData, jsonDoc.GetAllocator());

			strValue.Format(_T("%.3f"), double(m_nCanCheckAve)); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
			valObjectSub.AddMember(_T("DEFECT_GRAY_LV_AVG"), valData, jsonDoc.GetAllocator());

			strValue.Format(_T("255.000")); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
			valObjectSub.AddMember(_T("DEFECT_GRAY_LV_MAX"), valData, jsonDoc.GetAllocator());

			strValue.Format(_T("0.000")); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
			valObjectSub.AddMember(_T("DEFECT_GRAY_LV_MIN"), valData, jsonDoc.GetAllocator());

			strValue.Format(_T("0.000")); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
			valObjectSub.AddMember(_T("DEFECT_GRAY_LV_RANGE"), valData, jsonDoc.GetAllocator());

			strValue.Format(_T("0.000")); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
			valObjectSub.AddMember(_T("DEFECT_GRAY_LV_STDDEV"), valData, jsonDoc.GetAllocator());

			strValue.Format(_T("0.000")); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
			valObjectSub.AddMember(_T("DEFECT_RATIO_WIDTH_LENGTH"), valData, jsonDoc.GetAllocator());

			strValue.Format(_T("0.000")); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
			valObjectSub.AddMember(_T("DEFECT_ANGLE"), valData, jsonDoc.GetAllocator());

			strValue.Format(_T("0.000")); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
			valObjectSub.AddMember(_T("DEFECT_ELONGATION"), valData, jsonDoc.GetAllocator());

			strValue.Format(_T("0.000")); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
			valObjectSub.AddMember(_T("DEFECT_BOUNDARY_PIXEL_LENGTH"), valData, jsonDoc.GetAllocator());

			strValue.Format(_T("255.000")); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
			valObjectSub.AddMember(_T("DEFECT_ROI_GRAY_LV_MAX"), valData, jsonDoc.GetAllocator());

			strValue.Format(_T("%.3f"), double(m_nCanCheckAve));  	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
			valObjectSub.AddMember(_T("DEFECT_ROI_GRAY_LV_AVG"), valData, jsonDoc.GetAllocator());

			strValue.Format(_T("0.000")); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
			valObjectSub.AddMember(_T("DEFECT_ROI_GRAY_LV_MIN"), valData, jsonDoc.GetAllocator());

			strValue.Format(_T("0.000")); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
			valObjectSub.AddMember(_T("DEFECT_ROI_GRAY_LV_MEDIAN"), valData, jsonDoc.GetAllocator());

			strValue.Format(_T("0.000")); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
			valObjectSub.AddMember(_T("DEFECT_ROI_GRAY_LV_STDDEV"), valData, jsonDoc.GetAllocator());

			strValue.Format(_T("%s"),strDefectFIleName); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
			valObjectSub.AddMember(_T("DEFECT_CROP_IMAGE_FILE_NAME"), valData, jsonDoc.GetAllocator());		
		
		valArray.PushBack(valObjectSub, jsonDoc.GetAllocator());
		valObjectSub.SetObject();

		valObjectMain.AddMember(_T("DEFECT_INFO"), valArray, jsonDoc.GetAllocator());
		valArray.SetArray();
	}

	// (Main2) 그걸 Array에 집어넣음
	valArray.PushBack(valObjectMain, jsonDoc.GetAllocator());

	// (Main3) 그 Array 를 Object 멤버로 넣음
	valObjectMain.SetObject();
	valObjectMain.AddMember(_T("IN_DATA"), valArray, jsonDoc.GetAllocator());

	jsonDoc.AddMember(_T("refDS"), valObjectMain, jsonDoc.GetAllocator());

	SYSTEMTIME sys_time;
	GetLocalTime(&sys_time);

	// File 생성
	CString strPath;
	strPath.Format(_T("%s\\"), m_strUploadStandByPath[nJsonClassification]);

	CString strFinalJudge;
	if(bNG)
		strFinalJudge.Format(_T("NG"));
	else
		strFinalJudge.Format(_T("OK"));
	//if (m_nCellFinalJudge[0] != RET_OK || m_nCellFinalJudge[1] != RET_OK)
	//	strFinalJudge.Format(_T("NG"));

	CString strFileName;
	strFileName.Format(_T("%s_%d_%s_%d_%s_%s_%s_%s.json"), _keyVisionType[nVisionType], nLane, strTime, nLine, m_strEqpID, m_strLotID, m_strCellID, strFinalJudge);
	//strFileName.Format(_T("%s_%d_%s_%d_%s_NA_NA_%s.json"), _keyVisionType[nVisionType], nLane, strTime, nLine, strEqpID, strFinalJudge);
	strPath += strFileName;

	FILE* fp = fopen(strPath, "w");
	if (!fp)
		AddtoList(_T("JsonOpen Failed"),nTargetCam);
	rapidjson::FileWriteStream fs(fp, (char*)strBuffer.GetString(), strBuffer.GetLength());
	rapidjson::PrettyWriter<rapidjson::FileWriteStream> writer(fs);
	jsonDoc.Accept(writer);
	fclose(fp);
}
void CFlyCapture2_EXDlg::WriteJsonSpecPara(int nTargetCam, int nType, int nLane)
{
	int nLine = m_pDlgSet->m_FormTab2->m_nLineNumber +1;
	int nJsonClassification = static_cast<int>(JsonClassification::_SPEC_PARA);
	SYSTEMTIME sys_time;
	GetLocalTime(&sys_time);
	CString strTime;
	strTime.Format(_T("%04d%02d%02d%02d%02d%02d%03d"), sys_time.wYear, sys_time.wMonth, sys_time.wDay, sys_time.wHour, sys_time.wMinute, sys_time.wSecond, sys_time.wMilliseconds);
	CString str;

	rapidjson::StringBuffer strBuffer;
	rapidjson::Document jsonDoc;
	jsonDoc.SetObject();

	CString strValue(_T(""));
	rapidjson::Value valData; // Data
	rapidjson::Value valObjectMain; // Object-Main
	valObjectMain.SetObject();
	rapidjson::Value valObjectSub; // Object-Sub
	valObjectSub.SetObject();
	rapidjson::Value valArray; // Array
	valArray.SetArray();

	// 주의
	// Object, Array는 AddMember, PushBack, Move 의 매개변수로 한번 사용하면 Null 이 되어버림. 사용 불가 상태.
	// 다시 Set- 으로 지정해야함.

	// Doc- Basic
	strValue.Format(_T("")); valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
	jsonDoc.AddMember(_T("TXN_ID"), valData, jsonDoc.GetAllocator());

	strValue.Format(_T("IN_DATA")); valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
	jsonDoc.AddMember(_T("inDTName"), valData, jsonDoc.GetAllocator());

	strValue.Format(_T("")); valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
	jsonDoc.AddMember(_T("outDTName"), valData, jsonDoc.GetAllocator());

	strValue.Format(_T("%s"), _keyActID[nJsonClassification]); valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
	jsonDoc.AddMember(_T("actID"), valData, jsonDoc.GetAllocator());

	// Doc- Common Contents
	// (Main1) Object에 멤버 쭉 넣고
	strValue.Format(_T("%s"), _keyCategory[nJsonClassification]); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
	valObjectMain.AddMember(_T("CATEGORY"), valData, jsonDoc.GetAllocator());

	strValue.Format(_T("%s"),m_strEqpID); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
	valObjectMain.AddMember(_T("EQP_ID"), valData, jsonDoc.GetAllocator());
	
	strValue.Format(_T("%s"),m_strEqpInspID); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
	valObjectMain.AddMember(_T("EQP_INSP_ID"), valData, jsonDoc.GetAllocator());

	strValue.Format(m_pDlgSet->m_strSelectProduct); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
	valObjectMain.AddMember(_T("RECIPE_ID"), valData, jsonDoc.GetAllocator());

	strValue.Format(_T("%s"), m_strLotID); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
	valObjectMain.AddMember(_T("LOT_ID"), valData, jsonDoc.GetAllocator());

	strValue.Format(m_pDlgSet->m_FormTab1->m_strPgmVersion); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
	valObjectMain.AddMember(_T("INSP_SW_VERSION"), valData, jsonDoc.GetAllocator());

	strValue.Format(strTime); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
	valObjectMain.AddMember(_T("MODIFY_TIME"), valData, jsonDoc.GetAllocator());

	if (nType == 0) {
		strValue.Format(_T("NA")); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
		valObjectMain.AddMember(_T("MODIFIER"), valData, jsonDoc.GetAllocator());

		strValue.Format(_T("RESTART")); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
		valObjectMain.AddMember(_T("MODIFY_REASON"), valData, jsonDoc.GetAllocator());
	}
	else {
		strValue.Format(_T("lgensol")); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
		valObjectMain.AddMember(_T("MODIFIER"), valData, jsonDoc.GetAllocator());

		strValue.Format(_T("Improving inspection capabilities")); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
		valObjectMain.AddMember(_T("MODIFY_REASON"), valData, jsonDoc.GetAllocator());
	}

	// Doc- Para_Info Contents
	CCellData cellData;
	cellData.Init(this);
	cellData.GetSpecParam(1);
	int nCategoryCount = 7;
	for (int i = 0; i < nCategoryCount; i++) {
		strValue.Format(_T("%s"), _keyInspType[cellData.m_structSpecParamData[i].nInspType]); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
		valObjectSub.AddMember(_T("PARA_INSP_TYPE"), valData, jsonDoc.GetAllocator());

		strValue.Format(_T("%d"), i); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
		valObjectSub.AddMember(_T("PARA_NO"), valData, jsonDoc.GetAllocator());

		strValue.Format(_T("SIDE")); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
		valObjectSub.AddMember(_T("PARA_CAM_POSITION"), valData, jsonDoc.GetAllocator());

		strValue.Format(_T("%s"), cellData.m_structSpecParamData[i].cDetectionName); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
		valObjectSub.AddMember(_T("PARA_DETECTION_NAME"), valData, jsonDoc.GetAllocator());

		strValue.Format(_T("%.3f"), (double)(cellData.m_structSpecParamData[i].rtROI.left)); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
		valObjectSub.AddMember(_T("ROI_SX"), valData, jsonDoc.GetAllocator());

		strValue.Format(_T("%.3f"), (double)(cellData.m_structSpecParamData[i].rtROI.right)); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
		valObjectSub.AddMember(_T("ROI_EX"), valData, jsonDoc.GetAllocator());

		strValue.Format(_T("%.3f"), (double)(cellData.m_structSpecParamData[i].rtROI.top)); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
		valObjectSub.AddMember(_T("ROI_SY"), valData, jsonDoc.GetAllocator());

		strValue.Format(_T("%.3f"), (double)(cellData.m_structSpecParamData[i].rtROI.bottom)); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
		valObjectSub.AddMember(_T("ROI_EY"), valData, jsonDoc.GetAllocator());

		valArray.PushBack(valObjectSub, jsonDoc.GetAllocator());
		valObjectSub.SetObject();
	}

	valObjectMain.AddMember(_T("PARA_INFO"), valArray, jsonDoc.GetAllocator());
	valArray.SetArray();

	// Doc- Spec_Info Contents
	for (int i = 0; i < nCategoryCount; i++) {
		strValue.Format(_T("%s"), _keyInspType[cellData.m_structSpecParamData[i].nInspType]); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
		valObjectSub.AddMember(_T("SPEC_INSP_TYPE"), valData, jsonDoc.GetAllocator());

		strValue.Format(_T("SIDE")); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
		valObjectSub.AddMember(_T("SPEC_CAM_POSITION"), valData, jsonDoc.GetAllocator());

		strValue.Format(_T("%s"), cellData.m_structSpecParamData[i].cDetectionName); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
		valObjectSub.AddMember(_T("SPEC_DETECTION_NAME"), valData, jsonDoc.GetAllocator());

		strValue.Format(_T("%.3f"), cellData.m_structSpecParamData[i].dSpec[0]); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
		valObjectSub.AddMember(_T("SPEC_LSL"), valData, jsonDoc.GetAllocator());

		strValue.Format(_T("%.3f"), cellData.m_structSpecParamData[i].dSpec[1]); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
		valObjectSub.AddMember(_T("SPEC_USL"), valData, jsonDoc.GetAllocator());

		strValue.Format(_T("")); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
		valObjectSub.AddMember(_T("SPEC_OFFSET"), valData, jsonDoc.GetAllocator());

		valArray.PushBack(valObjectSub, jsonDoc.GetAllocator());
		valObjectSub.SetObject();
	}

	valObjectMain.AddMember(_T("SPEC_INFO"), valArray, jsonDoc.GetAllocator());
	valArray.SetArray();

	// Doc- System_Para Contents
	strValue.Format(_T("ALL")); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
	valObjectSub.AddMember(_T("SYSTEM_CLASSIFICATION"), valData, jsonDoc.GetAllocator());

	//show insp time
	strValue.Format(_T("%d"), m_pDlgSet->m_FormTab2->m_bShowInspTime); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
	valObjectSub.AddMember(_T("SYSTEM_PARAM_1"), valData, jsonDoc.GetAllocator());

	//show edge
	strValue.Format(_T("%d"), m_pDlgSet->m_FormTab2->m_bShowEdge); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
	valObjectSub.AddMember(_T("SYSTEM_PARAM_2"), valData, jsonDoc.GetAllocator());

	//show tab area
	strValue.Format(_T("%d"), m_pDlgSet->m_FormTab2->m_bShowTabArea); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
	valObjectSub.AddMember(_T("SYSTEM_PARAM_3"), valData, jsonDoc.GetAllocator());

	//show hole
	strValue.Format(_T("%d"), m_pDlgSet->m_FormTab2->m_bShowHole); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
	valObjectSub.AddMember(_T("SYSTEM_PARAM_4"), valData, jsonDoc.GetAllocator());

	//rtos
	strValue.Format(_T("%d"), m_pDlgSet->m_FormTab2->m_bUseRtos); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
	valObjectSub.AddMember(_T("SYSTEM_PARAM_5"), valData, jsonDoc.GetAllocator());

	//Full Save
	strValue.Format(_T("%d"), m_pDlgSet->m_FormTab2->m_bFullSave); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
	valObjectSub.AddMember(_T("SYSTEM_PARAM_6"), valData, jsonDoc.GetAllocator());
	
	//NG Save
	strValue.Format(_T("%d"), m_pDlgSet->m_FormTab2->m_bNgSave); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
	valObjectSub.AddMember(_T("SYSTEM_PARAM_7"), valData, jsonDoc.GetAllocator());
	
	//Overlay Save
	strValue.Format(_T("%d"), m_pDlgSet->m_FormTab2->m_bOverlaySave); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
	valObjectSub.AddMember(_T("SYSTEM_PARAM_8"), valData, jsonDoc.GetAllocator());

	//OK Jpg Save
	strValue.Format(_T("%d"), m_pDlgSet->m_FormTab2->m_bSaveJpeg); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
	valObjectSub.AddMember(_T("SYSTEM_PARAM_9"), valData, jsonDoc.GetAllocator());

	//AI Save
	strValue.Format(_T("%d"), m_pDlgSet->m_FormTab2->m_bUseAiImageSave); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
	valObjectSub.AddMember(_T("SYSTEM_PARAM_10"), valData, jsonDoc.GetAllocator());

	//CSV Save
	strValue.Format(_T("%d"), m_pDlgSet->m_FormTab2->m_bCsvSave); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
	valObjectSub.AddMember(_T("SYSTEM_PARAM_11"), valData, jsonDoc.GetAllocator());

	//Circle Show
	strValue.Format(_T("%d"), m_pDlgSet->m_FormTab2->m_bCircleShow); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
	valObjectSub.AddMember(_T("SYSTEM_PARAM_12"), valData, jsonDoc.GetAllocator());

	//Tab image Show
	strValue.Format(_T("%d"), m_pDlgSet->m_FormTab2->m_bTapImageShow); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
	valObjectSub.AddMember(_T("SYSTEM_PARAM_13"), valData, jsonDoc.GetAllocator());

	//Complete Signal
	strValue.Format(_T("%d"), m_pDlgSet->m_FormTab2->m_bCompleteOnOff); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
	valObjectSub.AddMember(_T("SYSTEM_PARAM_13"), valData, jsonDoc.GetAllocator());

	

	valArray.PushBack(valObjectSub, jsonDoc.GetAllocator());
	valObjectSub.SetObject();

	valObjectMain.AddMember(_T("SYSTEM_PARA"), valArray, jsonDoc.GetAllocator());
	valArray.SetArray();


	// (Main2) 그걸 Array에 집어넣음
	valArray.PushBack(valObjectMain, jsonDoc.GetAllocator());

	// (Main3) 그 Array 를 Object 멤버로 넣음
	valObjectMain.SetObject();
	valObjectMain.AddMember(_T("IN_DATA"), valArray, jsonDoc.GetAllocator());

	jsonDoc.AddMember(_T("refDS"), valObjectMain, jsonDoc.GetAllocator());

	// File 생성
	CString strPath;
	strPath.Format(_T("%s\\%s_%d_PARAM.json"), m_strPathServerJson[2], strTime, nLane);
	//strPath.Format(_T("%s\\%s_PARAM.json"), m_strUploadStandByPath[nJsonClassification], strTime);

	// File 생성
	FILE* fp = fopen(strPath, "w");
	if (!fp)
		AddtoList(_T("JsonOpen Failed"), nTargetCam);
	rapidjson::FileWriteStream fs(fp, (char*)strBuffer.GetString(), strBuffer.GetLength());
	rapidjson::PrettyWriter<rapidjson::FileWriteStream> writer(fs);
	jsonDoc.Accept(writer);
	fclose(fp);
}
void CFlyCapture2_EXDlg::WriteJsonStatus(int nVisionType, int nLane)
{
	int nLine = m_pDlgSet->m_FormTab2->m_nLineNumber +1;
	int nJsonClassification = static_cast<int>(JsonClassification::_STATUS);
	SYSTEMTIME sys_time;
	GetLocalTime(&sys_time);
	CString strTime;
	strTime.Format(_T("%04d%02d%02d%02d%02d%02d%03d"), sys_time.wYear, sys_time.wMonth, sys_time.wDay, sys_time.wHour, sys_time.wMinute, sys_time.wSecond, sys_time.wMilliseconds);

	rapidjson::StringBuffer strBuffer;
	rapidjson::Document jsonDoc;
	jsonDoc.SetObject();

	CString strValue(_T(""));
	rapidjson::Value valData; // Data
	rapidjson::Value valObjectMain; // Object-Main
	valObjectMain.SetObject();
	rapidjson::Value valObjectSub; // Object-Sub
	valObjectSub.SetObject();
	rapidjson::Value valArray; // Array
	valArray.SetArray();

	// 주의
	// Object, Array는 AddMember, PushBack, Move 의 매개변수로 한번 사용하면 Null 이 되어버림. 사용 불가 상태.
	// 다시 Set- 으로 지정해야함.

	// Doc- Basic
	strValue.Format(_T("")); valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
	jsonDoc.AddMember(_T("TXN_ID"), valData, jsonDoc.GetAllocator());

	strValue.Format(_T("IN_DATA")); valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
	jsonDoc.AddMember(_T("inDTName"), valData, jsonDoc.GetAllocator());

	strValue.Format(_T("")); valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
	jsonDoc.AddMember(_T("outDTName"), valData, jsonDoc.GetAllocator());

	strValue.Format(_T("%s"), _keyActID[nJsonClassification]); valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
	jsonDoc.AddMember(_T("actID"), valData, jsonDoc.GetAllocator());

	// Doc- Common Contents
	// (Main1) Object에 멤버 쭉 넣고
	strValue.Format(_T("%s"), _keyCategory[nJsonClassification]); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
	valObjectMain.AddMember(_T("CATEGORY"), valData, jsonDoc.GetAllocator());

	strValue.Format(_T("%s"), strTime); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
	valObjectMain.AddMember(_T("TRANSFER_TIME"), valData, jsonDoc.GetAllocator());

	strValue.Format(_T("%s"),m_strEqpInspID); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
	valObjectMain.AddMember(_T("EQP_INSP_ID"), valData, jsonDoc.GetAllocator());

	strValue.Format(_T("%s"),m_strEqpID); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
	valObjectMain.AddMember(_T("EQP_ID"), valData, jsonDoc.GetAllocator());

	strValue.Format(_T("")); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
	valObjectMain.AddMember(_T("AGENT_SW_VERSION"), valData, jsonDoc.GetAllocator());

	strValue.Format(_T("%s"), _keyVisionType[nVisionType]); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
	valObjectMain.AddMember(_T("VISION_TYPE"), valData, jsonDoc.GetAllocator());

	strValue.Format(_T("1")); 	valData.SetString(strValue, strValue.GetLength(), jsonDoc.GetAllocator());
	valObjectMain.AddMember(_T("INSP_STATUS"), valData, jsonDoc.GetAllocator());

	// (Main2) 그걸 Array에 집어넣음
	valArray.PushBack(valObjectMain, jsonDoc.GetAllocator());

	// (Main3) 그 Array 를 Object 멤버로 넣음
	valObjectMain.SetObject();
	valObjectMain.AddMember(_T("IN_DATA"), valArray, jsonDoc.GetAllocator());

	jsonDoc.AddMember(_T("refDS"), valObjectMain, jsonDoc.GetAllocator());

	// File 생성
	CString strPath;
	strPath.Format(_T("\\\\%s\\DAT\\%s\\JSON\\%s\\%04d%02d\\"), m_pDlgSet->m_FormTab1->m_strServerIP, _keyVisionType[nVisionType], _keyCategory[nJsonClassification], sys_time.wYear, sys_time.wMonth);
	MakeDirectory(strPath);

	CString strFileName;
	strFileName.Format(_T("%s_%d_%s.json"), strTime, nLane, _keyCategory[nJsonClassification]);
	strPath += strFileName;

	FILE* fp = fopen(strPath, "w");
	if (!fp)
		AddtoList(_T("JsonOpen Failed"),0);
	rapidjson::FileWriteStream fs(fp, (char*)strBuffer.GetString(), strBuffer.GetLength());
	rapidjson::PrettyWriter<rapidjson::FileWriteStream> writer(fs);
	jsonDoc.Accept(writer);
	fclose(fp);

}

bool CFlyCapture2_EXDlg::PingCheck(CString strIP)
{
	HANDLE hIcmpFile;
	DWORD dwRetVal = 0;
	char  SendData[] = "Data Buffer";
	char  szReplyBuffer[1500];
	hIcmpFile = IcmpCreateFile();

	dwRetVal = IcmpSendEcho(hIcmpFile, inet_addr(strIP), SendData, sizeof(SendData), NULL, szReplyBuffer, sizeof(szReplyBuffer), 100);

	IcmpCloseHandle(hIcmpFile);

	if (dwRetVal != 1)
		return false;

	return true;
}

void CFlyCapture2_EXDlg::NgClassify(CString strBadInfo)
{
	if(strBadInfo=="CAP"|| strBadInfo=="NotCap"){
		m_nNgCode = NG_NOT_CAP;
	}
	else if(strBadInfo=="TAB" ||strBadInfo=="Tab" || strBadInfo=="Tab1" || strBadInfo=="BottomTab" || strBadInfo=="H-Edge" || strBadInfo=="TabDamage"){
		m_nNgCode = NG_TAP_POS;
		m_dNgValue = m_dCapCanDist;
	}
	else if(strBadInfo=="Strong"){
		m_nNgCode = NG_STRONG_WELD;
	}
	else if(strBadInfo=="Can-Cap X Dist"){
		m_nNgCode = NG_CAN_CAP_DIST_X;
		m_dNgValue = m_dCapCanDistX;
	}
	else if(strBadInfo=="Can-Cap Y Dist"){
		m_nNgCode = NG_CAN_CAP_DIST_Y;
		m_dNgValue = m_dCapCanDistY;
	}
	else if(strBadInfo=="Blur" || strBadInfo=="Weld")
		m_nNgCode = NG_WEAK_WELD;
	else if(strBadInfo=="WeldCheck")
		m_nNgCode = NG_BLUR_WELD;
	else{
		m_nNgCode = NG_TAP_POS;
	}

	m_strCodeReal = strBadInfo;

	if(m_nNgCode!=NG_TAP_POS && m_nNgCode!=NG_CAN_CAP_DIST_X && m_nNgCode!=NG_CAN_CAP_DIST_Y )
		m_nNgType = NG_TYPE_APPEARANCE;
}

bool CFlyCapture2_EXDlg::MakeDirectory(CString strPath)
{
	bool bSucceed = false;
	int nCount = 0;
	int find = strPath.ReverseFind('\\') + 1;
	std::string filePath = strPath.Mid(0, find);
	std::string fileName = strPath.Mid(find, strPath.GetLength() - find);
	std::string Path = filePath;

	char DirName[256];
	const char* p = Path.c_str();
	char* q = DirName;
	while (*p) {
		if (('\\' == *p) || ('/' == *p)) {
			if (':' != *(p - 1) && nCount != 0 && '.' != *(p - 4)) {
				CreateDirectory((CString)DirName, NULL);
			}
		}
		nCount++;

		*q++ = *p++;
		*q = '\0';
	}
	CreateDirectory((CString)DirName, NULL);

	if (INVALID_FILE_ATTRIBUTES == ::GetFileAttributes(strPath)) // 폴더 확인
		bSucceed = false;
	else
		bSucceed = true;

	return bSucceed;
}

void CFlyCapture2_EXDlg::InitDirectory(){
// Server Directory
	int nVisionType = static_cast<int>(JsonVisionType::TC_WELDING);
	if (PingCheck(m_pDlgSet->m_FormTab1->m_strServerIP)) {
		for (int i = 0; i < static_cast<int>(JsonClassification::_TOTALCOUNT); i++) {
			if (i == static_cast<int>(JsonClassification::_SPEC_PARA))
				m_strPathServerJson[i].Format(_T("\\\\%s\\DAT\\%s\\PARAM\\"), m_pDlgSet->m_FormTab1->m_strServerIP, _keyVisionType[nVisionType]);
			else
				m_strPathServerJson[i].Format(_T("\\\\%s\\DAT\\%s\\JSON\\%s\\"), m_pDlgSet->m_FormTab1->m_strServerIP, _keyVisionType[nVisionType], _keyCategory[i]);

			MakeDirectory(m_strPathServerJson[i]);
		}
		m_strPathServerImg.Format(_T("\\\\%s\\DAT\\%s\\IMG\\INSP\\"), m_pDlgSet->m_FormTab1->m_strServerIP, _keyVisionType[nVisionType]);
		MakeDirectory(m_strPathServerImg);
	}

	for (int i = 0; i < static_cast<int>(JsonClassification::_TOTALCOUNT); i++) {
		if (i == static_cast<int>(JsonClassification::_STATUS))
			continue;

		m_strUploadStandByPath[i].Format(_T("D:\\Upload\\%s\\"), _keyCategory[i]);
		MakeDirectory(m_strUploadStandByPath[i]);

		//CreateDirectory(m_strUploadStandByPath[i], NULL);
	}
}

void CFlyCapture2_EXDlg::SetEqipID(int nLine, int nLane)
{
	int nEqpInspEndNum = 25;
	int nEqpInspNum = 10;

	if (nLine <= 10) {
		if (nLane == 1)
			nEqpInspEndNum = 25;
		else if (nLane == 2)
			nEqpInspEndNum = 26;
		else if (nLane == 3)
			nEqpInspEndNum = 27;
	}
	else{
		if (nLane == 1)
			nEqpInspEndNum = 10;
		else if (nLane == 2)
			nEqpInspEndNum = 11;
		else if (nLane == 3)
			nEqpInspEndNum = 12;
	}

	if (nLine == 10)
		m_strEqpID.Format(_T("M1AASB206"));
	else if (nLine == 9)
		m_strEqpID.Format(_T("M1AASB205"));
	else if (nLine == 8)
		m_strEqpID.Format(_T("M1AASB203"));
	else if (nLine == 7)
		m_strEqpID.Format(_T("M1AASB202"));
	else if (nLine == 6)
		m_strEqpID.Format(_T("M1AASB201"));
	else if (nLine == 3){
		m_strEqpID.Format(_T("M1AASB103"));
		if(nLane == 1)		
			nEqpInspEndNum = 20;
		else if(nLane == 2)	
			nEqpInspEndNum = 21;
		nEqpInspNum =7;
	}
	else if (nLine == 2){
		m_strEqpID.Format(_T("M1AASB102"));
		if(nLane == 1)		
			nEqpInspEndNum = 22;
		else if(nLane == 2)	
			nEqpInspEndNum = 23;
	}
	else if (nLine == 15)
		m_strEqpID.Format(_T("N1AASB605"));
	else if (nLine == 16)
		m_strEqpID.Format(_T("N1AASB606"));
	else if (nLine == 17)
		m_strEqpID.Format(_T("N1AASB607"));
	else if (nLine == 18)
		m_strEqpID.Format(_T("N1AASB608"));
	else if (nLine == 19)
		m_strEqpID.Format(_T("N1AASB609"));
	else if (nLine == 20)
		m_strEqpID.Format(_T("N1AASB610"));
	else if (nLine == 21)
		m_strEqpID.Format(_T("N1AASB611"));
	else if (nLine == 22)
		m_strEqpID.Format(_T("N1AASB612"));
	else if (nLine == 23)
		m_strEqpID.Format(_T("N1AASB613"));
	else if (nLine == 24)
		m_strEqpID.Format(_T("N1AASB614"));
	else if (nLine == 25)
		m_strEqpID.Format(_T("N1AASB615"));
	else if (nLine == 26)
		m_strEqpID.Format(_T("N1AASB616"));
	else if (nLine == 27)
		m_strEqpID.Format(_T("N1AASB617"));
	else if (nLine == 28)
		m_strEqpID.Format(_T("N1AASB618"));
	else if (nLine == 29)
		m_strEqpID.Format(_T("N1AASB512"));
	else if (nLine == 30)
		m_strEqpID.Format(_T("N1AASB513"));
	else
		m_strEqpID.Format(_T("M1AASB205"));

	m_strEqpInspID.Format(_T("%s-%d-%d"),m_strEqpID, nEqpInspNum, nEqpInspEndNum);
}

#define	WM_CONNECT_STATUS_CHANGED	(WM_USER+1)


UINT WINAPI CFlyCapture2_EXDlg::DllSocket_ConnectStatus(char* strEventContents, int nStatus, int nErrorCode)
{
	//gui display 함수를 여기에서 호출하지 마세요.
	//pTempLoad->PostConnectStatus(strEventContents, nStatus, nErrorCode);

	if (NULL != pTempLoad)
	{
		pTempLoad->PostConnectStatus(strEventContents, nStatus, nErrorCode);
	}

	return 0;
}

UINT WINAPI CFlyCapture2_EXDlg::DllSocket_ReceiveDataBuffer(unsigned char* pReceiveBuffer, int nReceiveLengthByByte)
{
	//gui display 함수를 여기에서 호출하지 마세요.
	if (NULL != pReceiveBuffer && nReceiveLengthByByte > 0 && pTempLoad)
	{
		pTempLoad->SubReceiveCallBackProcess(pReceiveBuffer, nReceiveLengthByByte);
		//TRACE(_T("%s:%ld\r\n"), pReceiveBuffer, nReceiveLengthByByte);
	}

	return 0;
}
void CFlyCapture2_EXDlg::PostConnectStatus(char* strEventContents, int nStatus, int nErrorCode)
{
	const bool bConnected = enConnectEvent::OnConnected == nStatus;
	if (NULL != strEventContents && m_blastOldConnectStatus != bConnected)
	{
		m_strSocketEvent = strEventContents;
		m_blastOldConnectStatus = bConnected;
		if (::IsWindow(GetSafeHwnd()))
		{
			PostMessage(WM_CONNECT_STATUS_CHANGED, m_blastOldConnectStatus, (LPARAM)&m_strSocketEvent);
		}
	}
}


void CFlyCapture2_EXDlg::SubReceiveCallBackProcess(unsigned char* pReceiveBuffer, int nReceiveLengthWithByte)
{
	int nLOTIDLength = 10;
	int nLength = 0;
	int nResult = 0;
	char arrData[1000] = { 0x00, };
	CString strSort = _T("");
	CString strOut = _T("");
	CString strTemp = _T("");
	std::string strData;

	if (m_LoadVTECSDll.DllSocket_IsASCIITypeDll())
	{
#ifdef _UNICODE
		CStringA strA = (char*)pReceiveBuffer;
		CStringW strOut;
		strOut = strA;
#else
		CString strOut = (char*)pReceiveBuffer;
#endif // _UNICODE
		TRACE(_T("%s:%ld\r\n"), strOut, nReceiveLengthWithByte);
	}
	else
	{
		if (!m_pDlgSet->m_FormTab2->m_bUseCellApi)
			return;
		else {
			for (int i = 0; i < nReceiveLengthWithByte; i++)
			{
				strTemp.Format(_T("%02x"), pReceiveBuffer[i]);
				nResult = strtol(strTemp, NULL, 16);
				if (nResult >= 48 && nResult <= 122) {
					strData = strtoul(strTemp, NULL, 16);
					strOut += strData.c_str();
				}
			}
			nLength = strOut.GetLength();
			memcpy(arrData, strOut.GetBuffer(), strOut.GetLength());
			for (int i = 0; i < nLength; i++) {
				if (isalpha(arrData[i]) || isdigit(arrData[i])) {
					strSort += _T(arrData[i]);
				}
			}
			m_strLotID = strSort.Mid(0, nLOTIDLength);
			m_strCellID = strSort.Mid(nLOTIDLength);
			//TRACE(_T("Lot:%s / Cell:%s"), m_strLotID,m_strCellID);
		}
	}
}

int CALLBACK BrowseCallbackProc2(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
{
	switch (uMsg)
	{
		// 폴더선택 다이얼로그의 초기화가 끝난 경우     
	case BFFM_INITIALIZED:       
		SendMessage(hwnd, BFFM_SETSELECTION, TRUE, lpData);       
		break;    
	}    
	return 0;
} 

void CFlyCapture2_EXDlg::CellServerInit(){
	m_CriticalDLL.Lock();
	CString strInitPath;

	strInitPath = CELL_API_PATH;//spath;
	if (m_LoadVTECSDll.DllSocket_Initialize(strInitPath, DllSocket_ConnectStatus, DllSocket_ReceiveDataBuffer, false))
	{
		m_CurrentDllStatus.bInitialzed = true;
		m_bCellTraceConeect = true;
		AddtoList("API Init OK",1);
		m_btCellApi.SetFaceColor(COLOR_GREEN);

		
	}
	else{
		AddtoList("API Init NG",1);
		m_btCellApi.SetFaceColor(COLOR_RED);
	}
	//CFileDialog Picker(TRUE, NULL, NULL, OFN_SHAREAWARE, "All Files(*.*)|*.*||");
	//CFileDialog Picker(TRUE, NULL, NULL, OFN_FILEMUSTEXIST, 0);
	//CFileDialog Picker(strDllPathName, OFN_FILEMUSTEXIST, NULL, 0);
	//CFolderPickerDialog	Picker(strDllPathName, OFN_FILEMUSTEXIST, NULL, 0);
	//if (Picker.DoModal() == IDOK)
	//{
	//	strDllPathName = Picker.GetPathName();
	//	if (m_LoadVTECSDll.DllSocket_Initialize(strDllPathName, DllSocket_ConnectStatus, DllSocket_ReceiveDataBuffer, false))
	//	{
	//		m_CurrentDllStatus.bInitialzed = true;
	//		//SubDisplayUpdate();//화면에 dll 상태표시
	//	}
	//}

	//SetDlgItemText(IDC_STATIC_DLL_PATH, strDllPathName);

	//ConnectionStatusCallFuntion : 통신 연결 상태 변경시 호출되는 call back 함수
	//ReceiveFromEthernet : 수신이 되면 호출되는 call back 함수


	m_CriticalDLL.Unlock();
}

void CFlyCapture2_EXDlg::CellServerView(){

	m_CriticalDLL.Lock();

	m_LoadVTECSDll.DllSocket_IPConfigToggleShow(m_bIPConfigShow, false);
	m_bIPConfigShow = !m_bIPConfigShow;

	m_CriticalDLL.Unlock();
}


void CFlyCapture2_EXDlg::SetDefectCropInfo(unsigned char* fmOriginSrc, unsigned char* fmCropDst, CString strNgName, int nOriginImgWidth, int nOriginImgHeight, DEFECTDATA* dataDefect, CPoint ptPos, CRect rtRoi, bool bHasDefectData, int nDefectSizeX, int nDefectSizeY, int nDefectArea, double dDefectValAvg, int nDefectValMax, int nDefectValMin, double dDefectValStdDev)
{
	// Reset
	memset(fmCropDst, 0, NG_CROP_SIZE_X*NG_CROP_SIZE_Y);

	// Crop
	//CRect rtCrop;
	//rtCrop.SetRect(ptPos.x - NG_CROP_SIZE_X * 0.5, ptPos.y - NG_CROP_SIZE_Y * 0.5, ptPos.x + NG_CROP_SIZE_X * 0.5, ptPos.y + NG_CROP_SIZE_Y * 0.5);
	////rtCrop = CheckROIoffset(rtCrop, CRect(0, 0, nOriginImgWidth, nOriginImgHeight));
	//pView->m_Project.m_Pro.ImageSelectCopy(fmOriginSrc, fmCropDst, nOriginImgWidth, nOriginImgHeight, rtCrop, NG_CROP_SIZE_X, NG_CROP_SIZE_Y, nScale);

	// ROI Gray Data
	double dAvg = 0;
	int nMax = 0;
	int nMin = 255;
	int nVal = 0;
	int nCount = 0;
	double dStdDev = 0;
	for (int nY = rtRoi.top; nY < rtRoi.bottom; nY ++) {
		for (int nX = rtRoi.left; nX < rtRoi.right; nX++) {
			nVal = *(fmOriginSrc + nOriginImgWidth*nY + nX);
			dAvg += nVal;
			nCount++;

			if (nVal > nMax)
				nMax = nVal;
			if (nVal < nMin)
				nMin = nVal;
		}
	}
	if (nCount <= 1)
		nCount = 1;

	dAvg /= nCount;

	for (int nY = rtRoi.top; nY < rtRoi.bottom; nY++) {
		for (int nX = rtRoi.left; nX < rtRoi.right; nX++) {
			nVal = *(fmOriginSrc + nOriginImgWidth*nY + nX);
			dStdDev += pow(nVal - dAvg, 2);
		}
	}
	dStdDev /= nCount;
	dStdDev = sqrt(dStdDev);

	//불량코드필요
	dataDefect->strDefectName = strNgName;
	dataDefect->dRoiValAvg = dAvg;
	dataDefect->nRoiValMax = nMax;
	dataDefect->nRoiValMin = nMin;
	dataDefect->dRoiValMedian = (nMax + nMin) / 2;
	dataDefect->dRoiValStdDev = dStdDev;
	dataDefect->ptPos = ptPos;
	dataDefect->ptCriteriaPos = CPoint(0, 0);

	// Defect Gray Data
	// 전달되는 데이터가 있으면 받아서 기입
	if (bHasDefectData) {
		dataDefect->nWidth = nDefectSizeX;
		dataDefect->nHeight = nDefectSizeY;
		dataDefect->nPixelArea = nDefectArea;
		dataDefect->dDefectValAvg = dDefectValAvg;
		dataDefect->nDefectValMax = nDefectValMax;
		dataDefect->nDefectValMin = nDefectValMin;
		dataDefect->nDefectValRange = nDefectValMax - nDefectValMin;
		dataDefect->dDefectValStdDev = dDefectValStdDev;
	}
	// 전달되는 데이터가 없으면 ROI꺼 그대로 가져다가 씀
	else {
		dataDefect->nWidth = rtRoi.Width();
		dataDefect->nHeight = rtRoi.Height();
		dataDefect->nPixelArea = rtRoi.Width() * rtRoi.Height();
		dataDefect->dDefectValAvg = dAvg;
		dataDefect->nDefectValMax = nMax;
		dataDefect->nDefectValMin = nMin;
		dataDefect->nDefectValRange = nMax - nMin;
		dataDefect->dDefectValStdDev = dStdDev;
	}
}

void CFlyCapture2_EXDlg::OnBnClickedMfcbutton1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CFlyCapture2_EXDlg::OnBnClickedMfcbuttonApi()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CellServerView();
}


void CFlyCapture2_EXDlg::OnBnClickedMfcbuttonStop()
{
	m_bAuto = false;
	CString str11;
	str11 = m_strComname;

	if (m_nStats == 2)
	{
		AfxMessageBox(_T("Camera Connection Check!"));
	}
	else
	{

		m_IO.m_bPgmAuto = FALSE; // IO pulse thread
		for (int i = 0; i < MAX_CAMERAS; i++)
			if (m_bCamStats[i])
			{
				m_Vision[i]->Stop();//startgrabbing
									//m_Vision[0]->SoftTriggerSend();
			}
		//		KillTimer(2001);//io pulse
		GetDlgItem(IDC_MFCBUTTON_START)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_MFCBUTTON_STOP)->ShowWindow(SW_HIDE);
	}
}


void CFlyCapture2_EXDlg::OnBnClickedMfcbuttonStart()
{
	if (m_nStats == 2)
	{
		AfxMessageBox(_T("Check Camera!"));
	}
	else
	{
		m_nIoCount = 0;
		m_dLastTime = 0;
		//SetTimer(2001,500,NULL);//io pulse
		m_pDlgSet->m_FormTab2->m_bGrabContinue = FALSE;
		m_pDlgSet->m_FormTab2->UpdateData(FALSE);

		m_pDlgSet->m_FormTab2->KillTimer(2001);//live stop
		if (m_Vision[0]->IsGrabbing())
			m_Vision[0]->Stop();
		Wait(100);

		GetDlgItem(IDC_MFCBUTTON_START)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_MFCBUTTON_STOP)->ShowWindow(SW_SHOW);

		m_bAuto = true;
		BOOL bStats = FALSE;
		m_dTactTimeOld = m_dTactTime = 0;

		m_Project.m_nFrameCount = 0;
		for (int i = 0; i < MAX_CAMERAS; i++)
			if (m_bCamStats[i])
			{
				m_Vision[i]->Grab();//startgrabbing
									//m_Vision[0]->SoftTriggerSend();
				m_Vision[i]->SetExpose(m_pDlgSet->m_FormTab2->m_nMultiShutter[0]);
			}
		m_IO.m_bPgmAuto = TRUE; // IO pulse thread
		m_nSelectFrame = 9;


		return;
	}
}


void CFlyCapture2_EXDlg::OnBnClickedMfcbuttonSetting()
{
	if (m_pDlgSet->IsWindowVisible())
	{
		m_pDlgSet->ShowWindow(SW_HIDE);
		m_pDlgPassword->ShowWindow(SW_HIDE);
		m_nShowSet = 0;
		m_pDlgSet->m_FormTab4->m_bRoiSet = FALSE;
		m_Display[0].m_bTrackerUse = FALSE;
		m_pDlgSet->m_FormTab4->UpdateData(FALSE);
	}
	else
	{
		//	m_pDlgSet->ShowWindow(SW_SHOW);	
		m_pDlgPassword->m_strPassword1 = "";
		m_pDlgPassword->UpdateData(TRUE);
		m_pDlgPassword->ShowWindow(SW_SHOW);
		m_nShowSet = 1;
	}
}


void CFlyCapture2_EXDlg::OnBnClickedMfcbuttonReset()
{
	LONG vlist = 0;
	CString str;
	str.Format("Do You want to Data Reset?");
	if (MessageBox(str, "GLIM", MB_OKCANCEL) == IDOK)
	{

		m_Project.m_nMultiFrame = 0;
		fnReset();

		InitBadInfo();

		m_nFullFrameCount = 0;
		//		int staddr=20;
		m_fFps = 0;
		m_Project.ResetFrame();
		m_nSendIO = 0;
		m_nTimeCheckCount = 0;
		m_nOKNG[0] = m_nOKNG[1] = -1;
		m_nBufCount[0] = m_nBufCount[1] = 0;
		CDC* pDC = GetDC();

		for (int k = 0; k < 5; k++) {
			m_DisplaySave[0].m_strImagePath[k] = "";
			memset(m_DisplayBad[k].m_pImg->GetImagePtr(), 0, 300 * 192);
			m_DisplayBad[k].DrawImage(pDC);
		}
		ReleaseDC(pDC);


		m_dTactTimeAve[0] = m_dTactTimeAve[1] = m_dTactTimeAve[2] = 0;
		//control Reset
		m_Project.m_dBrightness = 0;
		//Label 초기화
		LabelResult(-1, AI_READY);

		m_strIsCode = "";
		m_strModelName = "";
		m_strUpperLow = "";
		m_nNewCount = 0;
		m_Project.m_nNgCount = m_Project.m_nFrameCount = 0;
		m_Project.m_dInsTime = 0;
		m_nSaveCount = 0;
		m_strIsCode = "";
		m_nNg1Count = m_nNg2Count = 0;
		m_nNgCount = 0;
		m_nOKCount = 0;
		m_nCSVCount = 0;
		m_dYield = 0;
		m_dTactTime = 0;

		m_nRWFrameCount = 0;
		m_nRWOkCount = 0;
		m_nRWNgCount = 0;
		m_bRework = FALSE;

		m_nDLNgCount = 0;
		m_nRBNgCount = 0;

	}

	return;
}


void CFlyCapture2_EXDlg::OnBnClickedMfcbuttonExit()
{
	CString str;
	str.Format("                 Exit Program?");
	if (MessageBox(str, "GLIM", MB_OKCANCEL) == IDOK)
	{
		//		Usb_Qu_Close();

		for (int i = 0; i < MAX_CAMERAS; i++)
			if (m_bCamStats[i])
				m_Vision[i]->Stop();

		Sleep(300);

		m_bExit = true;
		m_bRun = FALSE;
		m_bAuto = false;
		Wait(1000);

		OnOK();
	}
}