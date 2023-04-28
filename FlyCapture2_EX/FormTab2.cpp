// FormTab2.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "FlyCapture2_EX.h"
#include "FlyCapture2_EXDlg.h"
#include "FormTab2.h"
#include "DlgSet.h"
#include "ini.h"


// CFormTab2 대화 상자입니다.

IMPLEMENT_DYNAMIC(CFormTab2, CDialog)



CFormTab2::CFormTab2(CWnd* pParent /*=NULL*/)
	: CDialog(CFormTab2::IDD, pParent)
	//, m_bTrig(FALSE)
	
	, m_nCamShutter(2000)
	, m_nGrabDelay(0)
	, m_bGrabContinue(FALSE)
	, m_b2LineWelding(FALSE)
	, m_bLineWelding(FALSE)
	, m_bTabTopRefind(FALSE)
	, m_dRightArea(0)
	, m_dRightArea1(0)
	, m_nSkipRight(0)
	, m_nInnerCiircle(0)
	, m_dCamScale(0)
	, m_dTabTol(2.3)
	, m_dTabTol1(3)
	, m_dTapTolY1(1)
	, m_dTapTolY2(0)
	, m_nCylinderCloseTime(0)
	, m_nCylinderOpenTime(0)
	, m_bShowInspTime(FALSE)
	, m_bShowEdge(FALSE)
	, m_bShowTabArea(FALSE)
	, m_bShowHole(FALSE)
	, m_bCircleShow(FALSE)
	, m_bFullSave(FALSE)
	, m_bOverlaySave(FALSE)
	, m_bNgSave(FALSE)
	, m_bSaveJpeg(FALSE)
	, m_bTapImageShow(FALSE)
	, m_bUseLeft(FALSE)
	, m_bUseRight(FALSE)
	, m_bWeldCenterPos(TRUE)
	,m_bWeldCenterPos2(TRUE)
	, m_bRed(FALSE)
	, m_bGreen(FALSE)
	, m_bBlue(FALSE)
	, m_bYellow(FALSE)
	, m_bWhite(FALSE)
	, m_nWeldLeftOffset(10)
	, m_bCsvSave(FALSE)
	, m_nIoDelay(0)
	, m_bIODuration(FALSE)
	, m_nTabTopOffset(40)
	, m_bMergeImageSave(FALSE)
	, m_nTopCapCenterFrame(2)
	, m_nCanPosFrame(2)
	, m_nCapCenterBright(65)
	, m_bDrawGraph(FALSE)
	, m_nWeldInTabpos(9)
	, m_dWeldInTabWhiteTh(1.6)
	, m_bWeldInTab(FALSE)
	, m_bTabSummation(FALSE)
	, m_bUseAiReadFail(FALSE)
	, m_bUseSaveCrop(FALSE)
	, m_bTabReFind(FALSE)
	, m_bUseCellApi(FALSE)
	, m_dWeldPercent(10)
	, m_bPLCLotchange(FALSE)
	, m_bMorphShow(FALSE)
	, m_nWeldHeight(30)
	, m_nPCNo(0)
	, m_nWeakWeldFrame(0)
	, m_bCompleteOnOff(FALSE)
	, m_nTabEdgeVFrame(0)
	, m_bUseRtos(FALSE)
	, m_bUseServer(FALSE)
	, m_bUseAiImageSave(FALSE)
	, m_nLineNumber(1)
	, m_nLaneNumber(1)
	, m_bImgReverse(FALSE)
	, m_bUseTabTopFrame(FALSE)
	, m_nStartFrameL(2)
	, m_nEndFrameL(4)
	, m_nStartFrameR(5)
	, m_nEndFrameR(7)
{
//	m_hParent = pParent;
	m_pParent =pParent;// hParent;
	for(int i=0;i<=1;i++)
		m_nTabTopSelectFrame[i] = 9;
	for(int i=0;i<=20;i++)
		m_nMultiShutter[i] = 500;
}

CFormTab2::~CFormTab2()
{
}

void CFormTab2::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);


	//form tap4 복사중
	DDX_Text(pDX, IDC_EDIT_SHUTTER, m_nCamShutter);
	DDX_Text(pDX, IDC_EDIT_GRABDELAY, m_nGrabDelay);
	DDX_Check(pDX, IDC_CHECK_CONTINUEGRAB, m_bGrabContinue);
	DDX_Check(pDX, IDC_CHECK_2LINE_WELDING, m_b2LineWelding);
	DDX_Check(pDX, IDC_CHECK35, m_bLineWelding);
	DDX_Check(pDX, IDC_CHECK_TAP_TOP_REFIND, m_bTabTopRefind);
	DDX_Text(pDX, IDC_EDIIT_HOLEBLACK2, m_dRightArea);
	DDX_Text(pDX, IDC_EDIIT_HOLEBLACK4, m_dRightArea1);
	DDX_Text(pDX, IDC_EDIIT_HOLEBLACK6, m_nSkipRight);
	DDX_Text(pDX, IDC_EDIT2, m_nInnerCiircle);
	DDX_Text(pDX, IDC_EDIT_SCALE, m_dCamScale);
	DDX_Text(pDX, IDC_EDITTABTOL, m_dTabTol);
	DDX_Text(pDX, IDC_EDITTABTOL4, m_dTabTol1);
	DDX_Text(pDX, IDC_EDITTABTOL2, m_dTapTolY1);
	DDX_Text(pDX, IDC_EDITTABTOL3, m_dTapTolY2);
	DDX_Text(pDX, IDC_EDIT_GRABDELAY2, m_nCylinderCloseTime);
	DDX_Text(pDX, IDC_EDIT_GRABDELAY3, m_nCylinderOpenTime);
	DDX_Check(pDX, IDC_CHECK_SHOWINSPTIME, m_bShowInspTime);
	DDX_Check(pDX, IDC_CHECK_SHOWEDGE, m_bShowEdge);
	DDX_Check(pDX, IDC_CHECKSHOWTABAREA, m_bShowTabArea);
	DDX_Check(pDX, IDC_CHECK32, m_bShowHole);
	DDX_Check(pDX, IDC_CHECK37, m_bCircleShow);
	DDX_Check(pDX, IDC_CHECK_FULLSAVE, m_bFullSave);
	DDX_Check(pDX, IDC_CHECK_OVERLAYSAVE, m_bOverlaySave);
	DDX_Check(pDX, IDC_CHECK_NG_SAVE, m_bNgSave);
	DDX_Check(pDX, IDC_CHECK_JPEGSAVE, m_bSaveJpeg);
	DDX_Check(pDX, IDC_CHECK_TAPIMGSHOW, m_bTapImageShow);

	DDX_Text(pDX, IDC_EDIT_SHUTTER1, m_nMultiShutter[0]);
	DDX_Text(pDX, IDC_EDIT_SHUTTER2, m_nMultiShutter[1]);
	DDX_Text(pDX, IDC_EDIT_SHUTTER3, m_nMultiShutter[2]);
	DDX_Text(pDX, IDC_EDIT_SHUTTER4, m_nMultiShutter[3]);
	DDX_Text(pDX, IDC_EDIT_SHUTTER5, m_nMultiShutter[4]);
	DDX_Text(pDX, IDC_EDIT_SHUTTER6, m_nMultiShutter[5]);
	DDX_Text(pDX, IDC_EDIT_SHUTTER7, m_nMultiShutter[6]);
	DDX_Text(pDX, IDC_EDIT_SHUTTER8, m_nMultiShutter[7]);
	DDX_Text(pDX, IDC_EDIT_SHUTTER9, m_nMultiShutter[8]);
	DDX_Text(pDX, IDC_EDIT_SHUTTER10, m_nMultiShutter[9]);
	DDX_Text(pDX, IDC_EDIT_SHUTTER11, m_nMultiShutter[10]);
	DDX_Check(pDX, IDC_CHECK_LEFT, m_bUseLeft);
	DDX_Check(pDX, IDC_CHECK_RIGHT, m_bUseRight);
	DDX_Check(pDX, IDC_CHECK_WELDCENTER, m_bWeldCenterPos);
	DDX_Check(pDX, IDC_CHECK_WELDCENTER2, m_bWeldCenterPos2);
	DDX_Check(pDX, IDC_CHECK_RED, m_bRed);
	DDX_Check(pDX, IDC_CHECK_GREEN, m_bGreen);
	DDX_Check(pDX, IDC_CHECK_BLUE, m_bBlue);
	DDX_Check(pDX, IDC_CHECK_YELLOW, m_bYellow);
	DDX_Check(pDX, IDC_CHECK_WHITE, m_bWhite);
	DDX_Text(pDX, IDC_EDIT_WELDLEFTOFFSET, m_nWeldLeftOffset);
	DDX_Check(pDX, IDC_CHECK_SAVE_NAS, m_bCsvSave);
	DDX_Check(pDX, IDC_CHECK_USE_RTOS, m_bUseRtos);
	DDX_Check(pDX, IDC_CHECK_USE_SERVER, m_bUseServer);
	DDX_Check(pDX, IDC_CHECK_USE_AI_READY, m_bUseAiReadyAlram);
	
	DDX_Check(pDX, IDC_CHECK_USE_AI_SAVE, m_bUseAiImageSave);
	DDX_Text(pDX, IDC_EDIT_IODELAY2, m_nIoDelay);
	DDX_Check(pDX, IDC_CHECK_IODUREATION, m_bIODuration);
	DDX_Text(pDX, IDC_EDIT_TABTOPOFFSET, m_nTabTopOffset);
	DDX_Check(pDX, IDC_CHECK_SAVE1, m_bSaveFrame[0]);
	DDX_Check(pDX, IDC_CHECK_SAVE2, m_bSaveFrame[1]);
	DDX_Check(pDX, IDC_CHECK_SAVE3, m_bSaveFrame[2]);
	DDX_Check(pDX, IDC_CHECK_SAVE4, m_bSaveFrame[3]);
	DDX_Check(pDX, IDC_CHECK_SAVE5, m_bSaveFrame[4]);
	DDX_Check(pDX, IDC_CHECK_SAVE6, m_bSaveFrame[5]);
	DDX_Check(pDX, IDC_CHECK_SAVE7, m_bSaveFrame[6]);
	DDX_Check(pDX, IDC_CHECK_SAVE8, m_bSaveFrame[7]);
	DDX_Check(pDX, IDC_CHECK_SAVE9, m_bSaveFrame[8]);
	DDX_Check(pDX, IDC_CHECK_SAVE10, m_bSaveFrame[9]);
	DDX_Check(pDX, IDC_CHECK_SAVE11, m_bSaveFrame[10]);
	DDX_Check(pDX, IDC_CHECK_SAVE_MERGE, m_bMergeImageSave);
	DDX_Text(pDX, IDC_EDIT_TOPCAPCENTERFRAME, m_nTopCapCenterFrame);
	DDV_MinMaxInt(pDX, m_nTopCapCenterFrame, 0, 10);
	DDX_Text(pDX, IDC_EDIT_CANPOSFRAME, m_nCanPosFrame);
	DDV_MinMaxInt(pDX, m_nCanPosFrame, 0, 10);
	DDX_Text(pDX, IDC_EDIT_CAPCENTERBRIGHT, m_nCapCenterBright);
	DDX_Text(pDX, IDC_EDIT_TABTOPSTART, m_nTabTopSelectFrame[0]);
	DDX_Text(pDX, IDC_EDIT_TABTOPEND, m_nTabTopSelectFrame[1]);
	DDX_Check(pDX, IDC_CHECK_DRAWGRAPH, m_bDrawGraph);
	DDX_Text(pDX, IDC_EDIT_TABWELDPOS, m_nWeldInTabpos);
	DDX_Text(pDX, IDC_EDIT_WELDWHITETH, m_dWeldInTabWhiteTh);
	DDX_Check(pDX, IDC_CHECK_WELDINTAB, m_bWeldInTab);
	DDX_Check(pDX, IDC_CHECK_TABSUM, m_bTabSummation);
	DDX_Check(pDX, IDC_CHECK_USE_AI_READ_FAIL, m_bUseAiReadFail);
	DDX_Check(pDX, IDC_CHECK_USE_SAVE_CROP, m_bUseSaveCrop);
	
	DDX_Check(pDX, IDC_CHECK_TABREFIND, m_bTabReFind);
	DDX_Check(pDX, IDC_CHECK_USE_CELL_API_TEST, m_bUseCellApi);
	DDX_Text(pDX, IDC_EDIT_WELDWHCEKFRAME1, m_nWeldCheckFrame[0]);
	DDX_Text(pDX, IDC_EDIT_WELDWHCEKFRAME2, m_nWeldCheckFrame[1]);
	DDX_Text(pDX, IDC_EDIT_WELDWHCEKFRAME3, m_nWeldCheckFrame[2]);
	DDX_Text(pDX, IDC_EDIT_WELDWHCEKFRAME4, m_nWeldCheckFrame[3]);
	DDX_Text(pDX, IDC_EDIT3, m_dWeldPercent);
	DDX_Check(pDX, IDC_CHECK_LOTCHANGE, m_bPLCLotchange);
	DDX_Check(pDX, IDC_CHECK_SHOWWELDIMAGE, m_bMorphShow);
	DDX_Text(pDX, IDC_EDIT_WELDHEIGHT, m_nWeldHeight);
	DDX_Text(pDX, IDC_EDIT_PCNO, m_nPCNo);
	DDX_Text(pDX, IDC_EDIT_WEAKWELDFRAME, m_nWeakWeldFrame);
	DDX_Check(pDX, IDC_CHECK_COMPLETEONOFF, m_bCompleteOnOff);
	DDX_Check(pDX, IDC_CHECK_IMG_REVERSE, m_bImgReverse);
	DDX_Text(pDX, IDC_EDIT_TABEDGEV_FRAME, m_nTabEdgeVFrame);
	DDX_Control(pDX, IDC_COMBO_LINE_NUMBER, m_comboLineNumber);
	DDX_Control(pDX, IDC_COMBO_LANE_NUMBER, m_comboLaneNumber);
	DDX_Check(pDX, IDC_CHECK_TAB_TOP_FRAME_USE, m_bUseTabTopFrame);
	DDX_Text(pDX, IDC_EDIT_TAB_TOP_L_START_FRAME, m_nStartFrameL);
	DDX_Text(pDX, IDC_EDIT_TAB_TOP_L_END_FRAME, m_nEndFrameL);
	DDX_Text(pDX, IDC_EDIT_TAB_TOP_R_START_FRAME, m_nStartFrameR);
	DDX_Text(pDX, IDC_EDIT_TAB_TOP_R_END_FRAME, m_nEndFrameR);

}


BEGIN_MESSAGE_MAP(CFormTab2, CDialog)

	ON_WM_RBUTTONDOWN()

	ON_BN_CLICKED(IDC_CHECK_CONTINUEGRAB, &CFormTab2::OnBnClickedCheckContinuegrab)
	ON_BN_CLICKED(IDC_CHECK_2LINE_WELDING, &CFormTab2::OnBnClickedCheck2lineWelding)
	ON_BN_CLICKED(IDC_CHECK35, &CFormTab2::OnBnClickedCheck35)
	ON_BN_CLICKED(IDC_CHECK_TAP_TOP_REFIND, &CFormTab2::OnBnClickedCheckTapTopRefind)
	ON_BN_CLICKED(IDC_BUTTONUpdate, &CFormTab2::OnBnClickedButtonupdate)
	ON_BN_CLICKED(IDC_BUTTON_GRAB, &CFormTab2::OnBnClickedButtonGrab)
	ON_BN_CLICKED(IDC_BUTTON_ADJUST, &CFormTab2::OnBnClickedButtonAdjust)
	ON_BN_CLICKED(IDC_CHECK_SHOWINSPTIME, &CFormTab2::OnBnClickedCheckShowinsptime)
	ON_BN_CLICKED(IDC_CHECK_SHOWEDGE, &CFormTab2::OnBnClickedCheckShowedge)
	ON_BN_CLICKED(IDC_CHECKSHOWTABAREA, &CFormTab2::OnBnClickedCheckshowtabarea)
	ON_BN_CLICKED(IDC_CHECK32, &CFormTab2::OnBnClickedCheck32)
	ON_BN_CLICKED(IDC_CHECK37, &CFormTab2::OnBnClickedCheck37)
	ON_BN_CLICKED(IDC_CHECK_FULLSAVE, &CFormTab2::OnBnClickedCheckFullsave)
	ON_BN_CLICKED(IDC_CHECK_OVERLAYSAVE, &CFormTab2::OnBnClickedCheckOverlaysave)
	ON_BN_CLICKED(IDC_CHECK_NG_SAVE, &CFormTab2::OnBnClickedCheckNgSave)
	ON_BN_CLICKED(IDC_CHECK_JPEGSAVE, &CFormTab2::OnBnClickedCheckJpegsave)
	ON_BN_CLICKED(IDC_CHECK_TAPIMGSHOW, &CFormTab2::OnBnClickedCheckTapimgshow)
	ON_BN_CLICKED(IDC_BUTTON1, &CFormTab2::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON10, &CFormTab2::OnBnClickedButton10)
	ON_BN_CLICKED(IDC_CHECK_LEFT, &CFormTab2::OnBnClickedCheckLeft)
	ON_BN_CLICKED(IDC_CHECK_RIGHT, &CFormTab2::OnBnClickedCheckRight)
	ON_BN_CLICKED(IDC_CHECK_WELDCENTER, &CFormTab2::OnBnClickedCheckWeldcenter)
	ON_BN_CLICKED(IDC_BUTTON2, &CFormTab2::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_CHECK_RED, &CFormTab2::OnBnClickedCheckRed)
	ON_BN_CLICKED(IDC_CHECK_GREEN, &CFormTab2::OnBnClickedCheckGreen)
	ON_BN_CLICKED(IDC_CHECK_BLUE, &CFormTab2::OnBnClickedCheckBlue)
	ON_BN_CLICKED(IDC_CHECK_YELLOW, &CFormTab2::OnBnClickedCheckYellow)
	ON_BN_CLICKED(IDC_CHECK_WHITE, &CFormTab2::OnBnClickedCheckWhite)
	ON_BN_CLICKED(IDC_BUTTON3, &CFormTab2::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_CHECK_SAVE_NAS, &CFormTab2::OnBnClickedCheckSaveNas)
	ON_BN_CLICKED(IDC_CHECK_IODUREATION, &CFormTab2::OnBnClickedCheckIodureation)
	ON_BN_CLICKED(IDC_CHECK_SAVE3, &CFormTab2::OnBnClickedCheckSave3)
	ON_BN_CLICKED(IDC_CHECK_SAVE1, &CFormTab2::OnBnClickedCheckSave1)
	ON_BN_CLICKED(IDC_CHECK_SAVE2, &CFormTab2::OnBnClickedCheckSave2)
	ON_BN_CLICKED(IDC_CHECK_SAVE4, &CFormTab2::OnBnClickedCheckSave4)
	ON_BN_CLICKED(IDC_CHECK_SAVE5, &CFormTab2::OnBnClickedCheckSave5)
	ON_BN_CLICKED(IDC_CHECK_SAVE6, &CFormTab2::OnBnClickedCheckSave6)
	ON_BN_CLICKED(IDC_CHECK_SAVE7, &CFormTab2::OnBnClickedCheckSave7)
	ON_BN_CLICKED(IDC_CHECK_SAVE8, &CFormTab2::OnBnClickedCheckSave8)
	ON_BN_CLICKED(IDC_CHECK_SAVE9, &CFormTab2::OnBnClickedCheckSave9)
	ON_BN_CLICKED(IDC_CHECK_SAVE10, &CFormTab2::OnBnClickedCheckSave10)
	ON_BN_CLICKED(IDC_CHECK_SAVE11, &CFormTab2::OnBnClickedCheckSave11)
	ON_BN_CLICKED(IDC_CHECK_SAVE_MERGE, &CFormTab2::OnBnClickedCheckSaveMerge)
	ON_BN_CLICKED(IDC_CHECK_DRAWGRAPH, &CFormTab2::OnBnClickedCheckDrawgraph)
	ON_BN_CLICKED(IDC_CHECK_WELDINTAB, &CFormTab2::OnBnClickedCheckWeldintab)
	ON_BN_CLICKED(IDC_CHECK_TABSUM, &CFormTab2::OnBnClickedCheckTabsum)
	ON_BN_CLICKED(IDC_CHECK_TABREFIND, &CFormTab2::OnBnClickedCheckTabrefind)
	ON_BN_CLICKED(IDC_CHECK_LOTCHANGE, &CFormTab2::OnBnClickedCheckLotchange)
	ON_BN_CLICKED(IDC_CHECK_SHOWWELDIMAGE, &CFormTab2::OnBnClickedCheckShowweldimage)
	ON_BN_CLICKED(IDC_CHECK_WELDCENTER2, &CFormTab2::OnBnClickedCheckWeldcenter2)
	ON_BN_CLICKED(IDC_CHECK_COMPLETEONOFF, &CFormTab2::OnBnClickedCheckCompleteonoff)
//	ON_CBN_SELCHANGE(IDC_COMBO1, &CFormTab2::OnCbnSelchangeCombo1)
ON_BN_CLICKED(IDC_BUTTON24, &CFormTab2::OnBnClickedButton24)
ON_BN_CLICKED(IDC_CHECK_USE_CELL_API_TEST, &CFormTab2::OnBnClickedCheckUseCellApiTest)
END_MESSAGE_MAP()


BOOL CFormTab2::OnInitDialog()
{
	CDialog::OnInitDialog();	

	UpdateIni(TRUE);

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

//true 읽기 flase 쓰기
void CFormTab2::UpdateIni(BOOL bLoad)
{
//	CDlgSet *pDlg = (CDlgSet*)m_hParent;	
	CFlyCapture2_EXDlg *pDlg = (CFlyCapture2_EXDlg*)m_pParent;
	if(bLoad==FALSE){
		UpdateData(TRUE);
		m_nLineNumber = m_comboLineNumber.GetCurSel();
		m_nLaneNumber = m_comboLaneNumber.GetCurSel();
	}

	CString strPath;
	strPath.Format("%ssetup.ini",GLIM_PATH);
	strPath.Format("c:\\Glim\\Model\\%s.ini",pDlg->m_pDlgSet->m_strSelectProduct);

	CIni ini1(strPath, "검사설정" , 0 );
	//ini1.SER_GETD(bLoad, m_bTrig, 0);
	if(m_nWeldInTabpos>pDlg->m_nMaxFrame)
		m_nWeldInTabpos = pDlg->m_nMaxFrame-1;
	
	//form4 복사
	ini1.SER_GETD(bLoad, m_nCamShutter	, 4000);
	ini1.SER_GETD(bLoad, m_nGrabDelay	, 50);
	ini1.SER_GETD(bLoad, m_b2LineWelding	, 0);
	ini1.SER_GETD(bLoad, m_bLineWelding	, 0);
	ini1.SER_GETD(bLoad, m_bTabTopRefind	, 0);
	ini1.SER_GETD(bLoad, m_dRightArea	, 6);
	ini1.SER_GETD(bLoad, m_dRightArea1	, 6);
	ini1.SER_GETD(bLoad, m_nSkipRight	, 0);
	ini1.SER_GETD(bLoad, m_nInnerCiircle	, 170);
	ini1.SER_GETD(bLoad, m_dCamScale	, 0.0188);
	ini1.SER_GETD(bLoad, m_dTabTol	, 3.5);
	ini1.SER_GETD(bLoad, m_dTabTol1	, 3);
	ini1.SER_GETD(bLoad, m_dTapTolY1	, 2.3);
	ini1.SER_GETD(bLoad, m_dTapTolY2	, 1.3);
	ini1.SER_GETD(bLoad, m_nCylinderOpenTime	, 100);
	ini1.SER_GETD(bLoad, m_bShowInspTime	, 0);
	ini1.SER_GETD(bLoad, m_bShowTabArea	, 1);
	ini1.SER_GETD(bLoad, m_bShowHole	, 1);
	ini1.SER_GETD(bLoad, m_bFullSave	, 1);
	ini1.SER_GETD(bLoad, m_bOverlaySave	, 1);
	ini1.SER_GETD(bLoad, m_bNgSave	, 1);
	ini1.SER_GETD(bLoad, m_bSaveJpeg	, 0);

	ini1.SER_GETD(bLoad, m_nMultiShutter[0]	, 2000);
	ini1.SER_GETD(bLoad, m_nMultiShutter[1]	, 2000);
	ini1.SER_GETD(bLoad, m_nMultiShutter[2]	, 2000);
	ini1.SER_GETD(bLoad, m_nMultiShutter[3]	, 2000);
	ini1.SER_GETD(bLoad, m_nMultiShutter[4]	, 2000);
	ini1.SER_GETD(bLoad, m_nMultiShutter[5]	, 2000);
	ini1.SER_GETD(bLoad, m_nMultiShutter[6]	, 2000);
	ini1.SER_GETD(bLoad, m_nMultiShutter[7]	, 2000);
	ini1.SER_GETD(bLoad, m_nMultiShutter[8]	, 2000);
	ini1.SER_GETD(bLoad, m_nMultiShutter[9]	, 2000);
	ini1.SER_GETD(bLoad, m_nMultiShutter[10]	, 2000);


	ini1.SER_GETD(bLoad, m_bUseRight, 0);
	ini1.SER_GETD(bLoad, m_bUseLeft, 0);
	ini1.SER_GETD(bLoad, m_bWeldCenterPos, 1);
	ini1.SER_GETD(bLoad, m_bWeldCenterPos2, 1);
	ini1.SER_GETD(bLoad, m_bCompleteOnOff, 0);	
	ini1.SER_GETD(bLoad, m_bRed, 1);
	ini1.SER_GETD(bLoad, m_bGreen, 0);
	ini1.SER_GETD(bLoad, m_bBlue, 0);
	ini1.SER_GETD(bLoad, m_bYellow, 0);
	ini1.SER_GETD(bLoad, m_bWhite, 0);
	ini1.SER_GETD(bLoad, m_nWeldLeftOffset, 10);
	ini1.SER_GETD(bLoad, m_bCsvSave, 1);
	ini1.SER_GETD(bLoad, m_nIoDelay, 1);
	ini1.SER_GETD(bLoad, m_bIODuration, 0);
	ini1.SER_GETD(bLoad, m_bUseRtos, 0);
	ini1.SER_GETD(bLoad, m_bUseServer, 0);
	ini1.SER_GETD(bLoad, m_bUseAiReadyAlram, 0);
	ini1.SER_GETD(bLoad, m_bUseAiImageSave, 0);
	ini1.SER_GETD(bLoad, m_nTabTopOffset, 40);
	ini1.SER_GETD(bLoad, m_bSaveFrame[0]	, 1);
	ini1.SER_GETD(bLoad, m_bSaveFrame[1]	, 1);
	ini1.SER_GETD(bLoad, m_bSaveFrame[2]	, 1);
	ini1.SER_GETD(bLoad, m_bSaveFrame[3]	, 1);
	ini1.SER_GETD(bLoad, m_bSaveFrame[4]	, 1);
	ini1.SER_GETD(bLoad, m_bSaveFrame[5]	, 1);
	ini1.SER_GETD(bLoad, m_bSaveFrame[6]	, 1);
	ini1.SER_GETD(bLoad, m_bSaveFrame[7]	, 1);
	ini1.SER_GETD(bLoad, m_bSaveFrame[8]	, 1);
	ini1.SER_GETD(bLoad, m_bSaveFrame[9]	, 1);
	ini1.SER_GETD(bLoad, m_bSaveFrame[10]	, 1);
	ini1.SER_GETD(bLoad, m_nTabTopSelectFrame[0]	, 8);
	ini1.SER_GETD(bLoad, m_nTabTopSelectFrame[1]	, 9);
	ini1.SER_GETD(bLoad, m_bDrawGraph	, 0);
	ini1.SER_GETD(bLoad, m_bTabSummation	, 1);
	ini1.SER_GETD(bLoad, m_bPLCLotchange	, 0);
	ini1.SER_GETD(bLoad, m_bUseAiReadFail	, 0);
	ini1.SER_GETD(bLoad, m_bUseSaveCrop	, 0);

	if(m_nTopCapCenterFrame>11)m_nTopCapCenterFrame = 11;
	if(m_nCanPosFrame>11)m_nCanPosFrame = 11;
	if(m_nTopCapCenterFrame<0)m_nTopCapCenterFrame = 0;
	if(m_nCanPosFrame<0)m_nCanPosFrame = 0;

	ini1.SER_GETD(bLoad, m_nTopCapCenterFrame	, 1);//0~
	ini1.SER_GETD(bLoad, m_nCanPosFrame	, 1);//0~
	
	ini1.SER_GETD(bLoad, m_nCapCenterBright	, 65);//0~
	ini1.SER_GETD(bLoad, m_nWeldInTabpos	, 9);//Frame
	ini1.SER_GETD(bLoad, m_dWeldInTabWhiteTh	, 1.6);//Percent

	ini1.SER_GETD(bLoad, m_bWeldInTab	, 0);//Percent
	ini1.SER_GETD(bLoad, m_bTabReFind, 0);//Percent
	ini1.SER_GETD(bLoad, m_bUseCellApi, 0);
	
	ini1.SER_GETD(bLoad, m_nWeldCheckFrame[0]	, 0);
	ini1.SER_GETD(bLoad, m_nWeldCheckFrame[1]	, 0);
	ini1.SER_GETD(bLoad, m_nWeldCheckFrame[2]	, 0);
	ini1.SER_GETD(bLoad, m_nWeldCheckFrame[3]	, 0);
	ini1.SER_GETD(bLoad, m_dWeldPercent	, 10);//Percent

	ini1.SER_GETD(bLoad, m_bMorphShow	, 0);//Percent
	ini1.SER_GETD(bLoad, m_nWeldHeight	, 30);//Percent
	ini1.SER_GETD(bLoad, m_nWeakWeldFrame	, 0);//Percent
	ini1.SER_GETD(bLoad, m_nTabEdgeVFrame	, 9);
	ini1.SER_GETD(bLoad, m_nLineNumber	, 1);
	ini1.SER_GETD(bLoad, m_nLaneNumber	, 1);

	ini1.SER_GETD(bLoad, m_bUseTabTopFrame, 0);
	ini1.SER_GETD(bLoad, m_nStartFrameL, 2);
	ini1.SER_GETD(bLoad, m_nEndFrameL, 4);
	ini1.SER_GETD(bLoad, m_nStartFrameR, 5);
	ini1.SER_GETD(bLoad, m_nEndFrameR, 7);

	if(m_nWeldInTabpos>pDlg->m_nMaxFrame)
		m_nWeldInTabpos = pDlg->m_nMaxFrame-1; //용접 프레임이 안넘게 조정
	

	if(m_dCamScale==0)m_dCamScale = 0.0178;
	int ntL = m_dTabTol/m_dCamScale;
	int ntR = m_dTabTol1/m_dCamScale;

	m_nTabTol = m_dTabTol/m_dCamScale;

	if(bLoad)	{
		UpdateData(FALSE);
		m_comboLineNumber.SetCurSel(m_nLineNumber);
		m_comboLaneNumber.SetCurSel(m_nLaneNumber);
	}

// 	if(m_b2LineWelding == TRUE || m_bLineWelding == TRUE){
// 		SetDlgItemText(IDC_STATIC_WELDING_1, _T("Line1"));
// 		SetDlgItemText(IDC_STATIC_WELDING_2, _T("Line2"));
// 	}
// 	else{
// 		SetDlgItemText(IDC_STATIC_WELDING_1, _T("Left"));
// 		SetDlgItemText(IDC_STATIC_WELDING_2, _T("Right"));
// 	}


	
}
void CFormTab2::OnBnClickedCheckContinuegrab()
{
	CFlyCapture2_EXDlg *pDlg = (CFlyCapture2_EXDlg*)m_pParent;
	UpdateData();
	//	if(m_bGrabContinue)//m_bGrabContinue
	if(pDlg->m_nMachine==8)
	{
		if(pDlg->m_bAuto)
			;
		else if(m_bGrabContinue)
		{
			//			pDlg->m_FlyVision[0]->StartCapture();
			//			pDlg->m_FlyVision[0]->CamSetTrig(false);
		}
		else
		{
			//			pDlg->m_FlyVision[0]->StopCapture();
			//			pDlg->m_FlyVision[0]->CamSetTrig(TRUE);
		}
	}
	else
	{
		if(pDlg->m_bAuto)
			;
		else if(m_bGrabContinue)
		{
			pDlg->m_Vision[0]->Stop();
			pDlg->Wait(200);
			pDlg->m_Vision[0]->SetMode(MODE_CONTINUE);
			pDlg->Wait(200);
			pDlg->m_Vision[0]->Grab();
		}
		else
		{
			pDlg->m_Vision[0]->Stop();
			pDlg->Wait(200);
			pDlg->m_Vision[0]->SetMode(MODE_SOFTTRIGGER);
			pDlg->Wait(200);
			pDlg->m_Vision[0]->Grab();
		}
	}
	
}

void CFormTab2::OnBnClickedCheck2lineWelding()
{
	UpdateData();

	if(m_bLineWelding == FALSE)
		m_bLineWelding = TRUE;

	if(m_bLineWelding == TRUE && m_b2LineWelding == FALSE)
		HideParam(FALSE);
	else
		HideParam(TRUE);

	UpdateData(FALSE);
}

void CFormTab2::OnBnClickedCheck35()
{
	UpdateData();

	if(m_bLineWelding == FALSE)
		m_b2LineWelding = FALSE;

	if(m_bLineWelding == TRUE && m_b2LineWelding == FALSE)
		HideParam(FALSE);
	else
		HideParam(TRUE);

	UpdateData(FALSE);
}

void CFormTab2::HideParam(BOOL bFlag)
{
// 	if(m_bLineWelding == FALSE && m_b2LineWelding == FALSE)
// 		bFlag = TRUE;
// 
// 	GetDlgItem(IDC_STATIC_WELDING_2)->ShowWindow(bFlag);
// 	GetDlgItem(IDC_EDIIT_HOLEBLACK2)->ShowWindow(bFlag);
// 	GetDlgItem(IDC_EDIIT_HOLEBLACK4)->ShowWindow(bFlag);
// 	GetDlgItem(IDC_EDIIT_HOLEBLACK6)->ShowWindow(bFlag);
}

void CFormTab2::OnBnClickedCheckTapTopRefind()
{
	UpdateData();
}

void CFormTab2::OnBnClickedButtonupdate()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData();


	if(m_dCamScale==0)m_dCamScale = 0.0178;
	int ntL = m_dTabTol/m_dCamScale;
	int ntR = m_dTabTol1/m_dCamScale;

	m_nTabTol = m_dTabTol/m_dCamScale;

	CFlyCapture2_EXDlg *pDlg = (CFlyCapture2_EXDlg*)m_pParent;
	
}

void CFormTab2::OnBnClickedButtonGrab()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CFlyCapture2_EXDlg *pDlg = (CFlyCapture2_EXDlg*)m_pParent;
	if(!pDlg->m_bCamStats[0]) return;

	if(pDlg->m_nMachine==8)
	{
		// 		pDlg->m_FlyVision[0]->StartCapture();
		//		pDlg->m_FlyVision[0]->CamSetTrig(TRUE);
		//		pDlg->m_FlyVision[0]->SWTrigger();
		// 		pDlg->m_FlyVision[0]->StopCapture();
	}
	else
	{
		if(m_bGrabContinue==FALSE){
			pDlg->m_Vision[0]->Stop();
			pDlg->m_Vision[0]->Grab();
			if(pDlg->m_nMachine==3)
			{
				pDlg->m_IO.WriteOutputPortPulse(8,m_nCylinderCloseTime);
				pDlg->Wait(m_nGrabDelay);
			}
			pDlg->m_Vision[0]->SoftTriggerSend();
			if(!pDlg->m_bAuto)
				pDlg->m_Vision[0]->Stop();
		}
	}
}

void CFormTab2::OnBnClickedButtonAdjust()
{
	CFlyCapture2_EXDlg *pDlg = (CFlyCapture2_EXDlg*)m_pParent;
	UpdateData();	
	if(pDlg->m_nMachine==8)
	{
		//		pDlg->m_FlyVision[0]->CamSetShutter(m_nCamShutter);
	}
	else
	{
		if(pDlg->m_bCamStats[0])
			pDlg->m_Vision[0]->SetExpose(m_nCamShutter);
	}
}

void CFormTab2::OnBnClickedCheckShowinsptime()
{
	// m_bShowInspTime
	UpdateData();

}
void CFormTab2::OnBnClickedCheckShowedge()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData();
}

void CFormTab2::OnBnClickedCheckshowtabarea()
{
	UpdateData();
}

void CFormTab2::OnBnClickedCheck32()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData();
}

void CFormTab2::OnBnClickedCheck37()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData();
}

void CFormTab2::OnBnClickedCheckFullsave()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
		// m_bFullSave
	UpdateData();
	/*if(m_bFullSave)
	{
		m_bNgSave = TRUE;
		UpdateData(FALSE);
	}*/
}

void CFormTab2::OnBnClickedCheckOverlaysave()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// m_bOverlaySave
	UpdateData();
	/*if(m_bOverlaySave==FALSE)
	{
		m_bFullSave = FALSE;
		UpdateData(FALSE);
	}*/
}

void CFormTab2::OnBnClickedCheckNgSave()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// m_bNgSave
	UpdateData();
	/*if(m_bNgSave==FALSE)
	{
		m_bFullSave = FALSE;
		UpdateData(FALSE);
	}*/

}

void CFormTab2::OnBnClickedCheckJpegsave()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// m_bSaveJpeg
	UpdateData();
}

void CFormTab2::OnBnClickedCheckTapimgshow()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
		UpdateData();
}

void CFormTab2::OnBnClickedButton1()
{
	CFlyCapture2_EXDlg *pDlg = (CFlyCapture2_EXDlg*)m_pParent;
	pDlg->m_pDlgSet->m_FormTab3->OnBnClickedButtonLeft();

}

void CFormTab2::OnBnClickedButton10()
{
	
	CFlyCapture2_EXDlg *pDlg = (CFlyCapture2_EXDlg*)m_pParent;
	pDlg->m_pDlgSet->m_FormTab3->OnBnClickedButtonRight();
}

void CFormTab2::OnBnClickedCheckLeft()
{//m_bUseLeft
	UpdateData();
	if(m_bUseLeft){
		m_bUseRight = FALSE;
		UpdateData(FALSE);
	}
}

void CFormTab2::OnBnClickedCheckRight()
{//m_bUseRight
	UpdateData();
	if(m_bUseRight){
		m_bUseLeft = FALSE;UpdateData(FALSE);
	}
}

void CFormTab2::OnBnClickedCheckWeldcenter()
{//m_bWeldCenterPos
	UpdateData();
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CFormTab2::OnBnClickedButton2()
{
	CFlyCapture2_EXDlg *pDlg = (CFlyCapture2_EXDlg*)m_pParent;

	pDlg->Test(0);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CFormTab2::OnBnClickedCheckRed()
{//m_bRed
	UpdateData();
	if(m_bRed){
		m_bGreen = m_bBlue=m_bYellow=m_bWhite = FALSE;
		UpdateData(FALSE);
		CFlyCapture2_EXDlg *pDlg = (CFlyCapture2_EXDlg*)m_pParent;
		pDlg->m_btModel.SetFaceColor(COLOR_RED);

	}
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
void CFormTab2::OnBnClickedCheckGreen()
{//m_bGreen
	UpdateData();
	if(m_bGreen){
		m_bRed = m_bBlue=m_bYellow=m_bWhite = FALSE;
		UpdateData(FALSE);
		CFlyCapture2_EXDlg *pDlg = (CFlyCapture2_EXDlg*)m_pParent;
		pDlg->m_btModel.SetFaceColor(COLOR_GREEN);

	}
}

void CFormTab2::OnBnClickedCheckBlue()
{//m_bBlue
	UpdateData();
	if(m_bBlue){
		m_bGreen = m_bRed=m_bYellow=m_bWhite = FALSE;
		UpdateData(FALSE);
		CFlyCapture2_EXDlg *pDlg = (CFlyCapture2_EXDlg*)m_pParent;
		pDlg->m_btModel.SetFaceColor(COLOR_BLUE);

	}
}

void CFormTab2::OnBnClickedCheckYellow()
{//m_bYellow
	UpdateData();
	if(m_bYellow){
		m_bGreen = m_bBlue=m_bRed=m_bWhite = FALSE;
		UpdateData(FALSE);
		CFlyCapture2_EXDlg *pDlg = (CFlyCapture2_EXDlg*)m_pParent;
		pDlg->m_btModel.SetFaceColor(COLOR_YELLOW);

	}
}

void CFormTab2::OnBnClickedCheckWhite()
{//m_bWhite
	UpdateData();
	if(m_bWhite){
		m_bGreen = m_bBlue=m_bYellow=m_bRed = FALSE;
		UpdateData(FALSE);
		CFlyCapture2_EXDlg *pDlg = (CFlyCapture2_EXDlg*)m_pParent;
		pDlg->m_btModel.SetFaceColor(COLOR_WHITE);

	}
}

void CFormTab2::OnBnClickedButton3()
{
	CFlyCapture2_EXDlg *pDlg = (CFlyCapture2_EXDlg*)m_pParent;
	pDlg->m_pDlgSet->m_FormTab3->OnBnClickedButtonLoad3();
}

void CFormTab2::OnBnClickedCheckSaveNas()
{//m_bNasSave
	UpdateData();
}

void CFormTab2::OnBnClickedCheckIodureation()
{//m_bIODuration
	UpdateData();
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CFormTab2::OnBnClickedCheckSave3()
{//m_bSaveFrame
	UpdateData();
}

void CFormTab2::OnBnClickedCheckSave1()
{//m_bSaveFrame
	UpdateData();
}

void CFormTab2::OnBnClickedCheckSave2()
{//m_bSaveFrame
	UpdateData();
}

void CFormTab2::OnBnClickedCheckSave4()
{//m_bSaveFrame
	UpdateData();
}

void CFormTab2::OnBnClickedCheckSave5()
{//m_bSaveFrame
	UpdateData();
}

void CFormTab2::OnBnClickedCheckSave6()
{//m_bSaveFrame
	UpdateData();
}

void CFormTab2::OnBnClickedCheckSave7()
{//m_bSaveFrame
	UpdateData();
}

void CFormTab2::OnBnClickedCheckSave8()
{//m_bSaveFrame
	UpdateData();
}

void CFormTab2::OnBnClickedCheckSave9()
{//m_bSaveFrame
	UpdateData();
}

void CFormTab2::OnBnClickedCheckSave10()
{//m_bSaveFrame
	UpdateData();
}

void CFormTab2::OnBnClickedCheckSave11()
{//m_bSaveFrame
	UpdateData();
}

void CFormTab2::OnBnClickedCheckSaveMerge()
{//m_bMergeImageSave
	UpdateData();
}

void CFormTab2::OnBnClickedCheckDrawgraph()
{//m_bDrawGraph
	UpdateData();
	if(m_bDrawGraph==FALSE){
		CFlyCapture2_EXDlg *pDlg = (CFlyCapture2_EXDlg*)m_pParent;
		pDlg->m_CamDisplay->ShowWindow(SW_HIDE);
	}
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CFormTab2::OnBnClickedCheckWeldintab()
{//m_bWeldInTab
	UpdateData();
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CFormTab2::OnBnClickedCheckTabsum()
{
	//m_bTabSummation //좌우 탭 맥스와 피크 의 평균
	UpdateData();
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CFormTab2::OnBnClickedCheckTabrefind()
{//m_bTabReFind
	UpdateData();	//탭 좌상 우상 각자 검색
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CFormTab2::OnBnClickedCheckLotchange()
{//m_bPLCLotchange
	UpdateData();
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CFormTab2::OnBnClickedCheckShowweldimage()
{//m_bMorphShow
	UpdateData();
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CFormTab2::OnBnClickedCheckWeldcenter2()
{//m_bWeldCenterPos2
	UpdateData();
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CFormTab2::OnBnClickedCheckCompleteonoff()
{//m_bCompleteOnOff
	UpdateData();
}

void CFormTab2::OnBnClickedButton24()
{
	CFlyCapture2_EXDlg *pDlg = (CFlyCapture2_EXDlg*)m_pParent;

	return;

	/*int nLine = pDlg->m_pDlgSet->m_FormTab2->m_nLineNumber;
	int nLane = pDlg->m_pDlgSet->m_FormTab2->m_nLaneNumber;
	int nVisionType = static_cast<int>(JsonVisionType::TC_WELDING);
	CString strVisionType = _keyVisionType[nVisionType];
	CString strServerIP = pDlg->m_pDlgSet->m_FormTab1->m_strServerIP;
	CString strImgUploadPath =  pDlg->m_strUploadStandByPath[static_cast<int>(JsonClassification::_INSPECTION)];
	CString strServerImgPath;
	strServerImgPath.Format(_T("%s\\OK\\"), pDlg->m_strPathServerImg);
	CreateDirectory(strServerImgPath,NULL);
	for (int i = 0; i < 25000; i++) {
		GetLocalTime(&pDlg->m_timeVisionInput);
		GetLocalTime(&pDlg->m_timeInspStart);

		Sleep(10);

		GetLocalTime(&pDlg->m_timeInspEnd);

		Sleep(10);
		GetLocalTime(&pDlg->m_timeVisionOutput);

			
		
		pDlg->m_DisplaySave[0].SaveServerImg(strVisionType, nLine, 0, 0, pDlg->m_bOKNG, strServerImgPath, pDlg->m_timeVisionOutput, strImgUploadPath);
		pDlg->WriteJsonInsp(0,pDlg->m_strPathServerJson[1], pDlg->m_timeVisionOutput, nLine, 0 );

	}*/

	//pDlg->WriteJsonSpecPara(0, 0);
	//pDlg->WriteJsonStatus();

	//CString strSendData;
	//strSendData.Format(_T("WRS ZF11736.L %d %s"), 1, _T("1"));
	////pDlg->m_pPlcCommuniction[0]->sendText(strSendData.operator LPCSTR());
	//int nStrLength = strSendData.GetLength();
	//const std::string& msg = strSendData.operator LPCSTR();

	//std::string str = msg;// Client::UnicodeToMultibyte(CP_UTF8, send_msg);

	//size_t txt_size = str.length();
	//char* buf = new char[txt_size + 1];
	//memset(buf, 0, txt_size + 1);
	//memcpy(buf, &str[0], txt_size);
	//CString strSendLog = buf;
	//strSendLog = _T("(s)") + strSendLog;
	//pDlg->AddToLog(0, strSendLog);

	//buf[txt_size] = '\r';

	//CString strId;
	//CString strResult;
	//strResult.Format(_T("17218 17218 12623 13109 13618 17218 17218 12623 13109 13618 17218 17218 12623 13109 13618 17218 17218 12623 13109 13618 00100 00200 00300 00400 00500"));
	//CString strTemp;
	//int i = 0;
	//while (FALSE != AfxExtractSubString(strTemp, strResult, i, ' '))
	//{
	//	if (i >= PLC_CELLID_ADDRESS_COUNT)
	//		break;

	//	pDlg->m_nPlcReadData[i] = atoi(strTemp);
	//	i++;
	//}

	//strId += pDlg->TransToId(true, 0, 20);
	//strId += pDlg->TransToId(false, 20, 20);

	//pDlg->AddToLog(0, strId);

	//pDlg->SaveOverlay(0, 0, FALSE, FALSE, 1, FALSE);
	
	//pDlg->OutputResult(nCam, 0);

	//pDlg->SaveSepaDataCsv(0, 0, RET_OK);
	
	//int nCam = 0;
	//int nImageWidth = pDlg->m_nSingleFrameWidth;
	//int nImageHeight = pDlg->m_nSingleFrameHeight;

	//BYTE* fm;
	//BYTE* fmGray;

	//// Color
	//int nColorScale = 3;
	//fm = new BYTE[nImageWidth*nImageHeight*nColorScale];
	//memcpy(fm, pDlg->m_Project.m_fmSingleFrame[nCam][nBufferNum], nImageWidth*nImageHeight*nColorScale);

	//// Gray
	//int nGrayScale = 1;
	//nImageWidth = CAMERA_WIDTH;
	//pDlg->m_Project.MakeAlphaCh(pDlg->m_DisplaySingleFrame1[nCam][nBufferNum].m_pImg->GetImagePtr(), pDlg->m_Project.m_fmSingleFrameGray[nCam][nBufferNum], nImageWidth, nImageHeight, 0);
	//fmGray = new BYTE[nImageWidth*nImageHeight*nGrayScale];
	//memcpy(fmGray, pDlg->m_Project.m_fmSingleFrameGray[nCam][nBufferNum], nImageWidth*nImageHeight*nGrayScale);

	//CRect rtIns;
	//rtIns.SetRect(500, 800, 1800, 1000);
	//rtIns = pDlg->m_Project.CheckROIoffset(rtIns, CRect(0, 0, nImageWidth, nImageHeight));
	//int nInsWidth = rtIns.Width();
	//int nInsHeight = rtIns.Height();
	//unsigned char* fmBinary = new unsigned char[nInsHeight * nInsWidth];
	//memset(fmBinary, 0, nInsHeight * nInsWidth);

	//int nPostVal = 0;
	//double dAvg = 0;
	//int nCount = 0;
	//int nVal = 0;
	//int nEnhanceVal = 8;
	//for (int nX = rtIns.left; nX < rtIns.right; nX++) {
	//	nCount = 0;
	//	dAvg = 0;

	//	for (int nY = rtIns.top; nY < rtIns.bottom; nY++) {
	//		dAvg += *(fmGray + nImageWidth*nY + nX);
	//		nCount++;
	//	}

	//	if (nCount <= 0)
	//		nCount = 1;

	//	dAvg /= nCount;

	//	for (int nY = rtIns.top; nY < rtIns.bottom; nY++) {
	//		nVal = *(fmGray + nImageWidth*nY + nX) - dAvg;
	//		*(fmBinary + nInsWidth*(nY - rtIns.top) + (nX - rtIns.left)) = 128 + (nVal * nEnhanceVal);

	//		//// 탭 돌출부분
	//		//nPostVal = 250 + (nVal * nEnhanceVal);
	//		//if (nPostVal > 255)
	//		//	nPostVal = 255;
	//		//if (nPostVal < 0)
	//		//	nPostVal = 0;
	//		//*(pDlg->m_DisplaySingleFrame1[nCam][nBufferNum].m_pImg->GetImagePtr() + nImageWidth*nY*nColorScale + nX*nColorScale + IMG_RED) = nPostVal;
	//		//nPostVal = 200 + (nVal * nEnhanceVal);
	//		//if (nPostVal > 255)
	//		//	nPostVal = 255;
	//		//if (nPostVal < 0)
	//		//	nPostVal = 0;
	//		//*(pDlg->m_DisplaySingleFrame1[nCam][nBufferNum].m_pImg->GetImagePtr() + nImageWidth*nY*nColorScale + nX*nColorScale + IMG_GREEN) = nPostVal;
	//		//nPostVal = 130 + (nVal * nEnhanceVal);
	//		//if (nPostVal > 255)
	//		//	nPostVal = 255;
	//		//if (nPostVal < 0)
	//		//	nPostVal = 0;
	//		//*(pDlg->m_DisplaySingleFrame1[nCam][nBufferNum].m_pImg->GetImagePtr() + nImageWidth*nY*nColorScale + nX*nColorScale + IMG_BLUE) = nPostVal;
	//	}
	//}

	//// kjb
	//CImg img;
	//img.Create(nInsWidth, nInsHeight, 8);
	//img.InitDIB();
	//memcpy(img.GetImagePtr(), fmBinary, nInsWidth * nInsHeight);
	//img.Save("c:\\glim\\aaa.bmp");

	//delete fmBinary;
}
void CFormTab2::OnBnClickedCheckUseCellApiTest()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData();

}
