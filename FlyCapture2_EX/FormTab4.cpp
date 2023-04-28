// FormTab4.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "FlyCapture2_EX.h"
#include "FlyCapture2_EXDlg.h"
#include "FormTab4.h"
#include "WeakWeldDlg.h"

// FormTab4 대화 상자입니다.

IMPLEMENT_DYNAMIC(FormTab4, CDialog)

FormTab4::FormTab4(CWnd* pParent /*=NULL*/)
	: CDialog(FormTab4::IDD, pParent)	
	, m_nCamShutter(2000)
	, m_bGrabContinue(FALSE)
	, m_nGrabDelay(0)
	, m_bImageReverseX(FALSE)
	, m_bImageReverseY(FALSE)
	, m_bShowInspTime(FALSE)
	, m_bFullSave(FALSE)
	, m_bNgSave(FALSE)
	, m_bOverlaySave(FALSE)
	, m_bSaveJpeg(FALSE)
	, m_dCamScale(0)
	, m_nCheckCanValue(0)
	, m_bMagneticUse(FALSE)
	, m_bShowEdge(FALSE)
	, m_dTabTol(2.3)
	, m_bShowTabArea(FALSE)
	, m_bTapImageShow(FALSE)
	, m_dTapTolY1(1)
	, m_dTapSize(3)
	, m_nTopCapEdgeValue(0)
	, m_nTapEdge(0)
	, m_dTopCapSize(18)
	, m_nHoleTh(88)
	, m_nMorphCount(2)
	, m_bCkAuto(FALSE)
	, m_nWeldingSize(0)
	, m_dLeftArea(0)
	, m_dRightArea(0)
	, m_dLeftArea1(0)
	, m_dRightArea1(0)
	, m_nV1(0)
	, m_nV2(0)
	, m_nV3(0)
	, m_nTopCapValue(0)
	, m_nCapReverseV(0)
	, m_bTapInspect(TRUE)
	, m_bHoleInspect(TRUE)
	, m_nTopCapEdgeValue1(0)
	, m_nTapEdge1(0)
	, m_bHoleLeftCheck(TRUE)
	, m_nCogDist(0)
	, m_bImageModify(FALSE)
	, m_bCheckView(FALSE)
	, m_nBlackArea(0)
	, m_bBlackArea(FALSE)
	, m_bWhiteCheck(FALSE)
	, m_bHoleSearchArea(TRUE)
	, m_nSkipLeft(0)
	, m_nSkipRight(0)
	, m_bRoiSet(FALSE)
	, m_nCylinderCloseTime(0)
	, m_bTapWhiteSkip(FALSE)
	, m_bLowBlackSkip(FALSE)
	, m_bHoleTopSearch(FALSE)
	, m_nCylinderOpenTime(0)
	, m_bParamAnd(FALSE)
	, m_bParamOr(TRUE)
	, m_bHoleEdgeSkip(TRUE)
	, m_b2Tab(FALSE)
	, m_bHoleEdgeCheck(FALSE)
	, m_dHoleEdgemm(0)
	, m_dTapTolY2(0)
	, m_bShowHole(FALSE)
	, m_dTabTol1(3)
	, m_bRegMode(TRUE)
	, m_bLineWelding(FALSE)
	, m_dMinHeight(0)
	, m_dMaxHeight(0)
	, m_bCircleFit(FALSE)
	, m_nInnerCiircle(0)
	, m_bCircleShow(FALSE)
	, m_dCenterOffset(10)
	, m_bUseCapCanY(FALSE)
	, m_b2LineWelding(FALSE)
	, m_bTabTopRefind(FALSE)
	, m_bTabBottomAbsMode(TRUE)
	, m_nTabTopTh(20)
	, m_bUseTabTopTh(FALSE)
	, m_bWeldingPosXFix(TRUE)
	, m_bWeldingRange(FALSE)
	, m_dWeldingDistanceY(2.6)
	, m_bWeldingRangeNgSignal(FALSE)
	, m_nTopCapWhiteCut(0)
	, m_dWhiteCut(0)
	, m_dCenterOffset2(1)
	, m_nTabCornerDist(20)
	, m_bTabCornerDist(FALSE)
	, m_dDistWeldMin(0)
	, m_dDistWeldMax(20)
	, m_bWeldDist(FALSE)
	, m_bWeldDistCap(FALSE)
	, m_dWeldDistCapMin(0)
	, m_dWeldDistCapMax(10)
	, m_bWeakWeld(FALSE)
	, m_nWeakWeld(0)
	, m_bFrame(FALSE)
	, m_nCenterEdgeValue(30)
	, m_nWeakWeld2(3000)
	, m_bTwoTab(FALSE)
	, m_nTabEdgeX1(50)
	, m_nTabEdgeX2(50)
	, m_nTabEdgeY1(80)
	, m_nTabEdgeY2(-30)
	, m_dMidWeld(500)
	, m_bMidWeld(FALSE)
	, m_nTabExistX1(40)
	, m_nTabExistX2(40)
	, m_nTabExistY1(-40)
	, m_nTabExistY2(70)
	, m_n2TabOffsetY(100)
	, m_nWeldCheck(0)
	, m_bBottomTab(FALSE)
	, m_dTabTopDistCapMin(0)
	, m_dTabTopDistCapMax(0)
	, m_bTabTopDistCap(FALSE)
	, m_dTopCapCenterCount(0)
	, m_dCapCandistX(0)
	, m_nAiFrame(7)
	, m_nCheckDamage(50)
	, m_nCheckDamageTh(1000)
	, m_bCheckDamage(FALSE)
	, m_dHoleEdgeINmm(0)
	, m_nBottomTabTop(50)
	, m_nBottomTabBot(30)
	, m_nBottomTabLeft(200)
	, m_nBottomTabRight(200)
	, m_nBottomTabFrame(2)
	, m_nBottomLeftTh(30)
	, m_nBottomRightTh(30)
	, m_bCanDist(FALSE)
	, m_dCanDistMin(0)
	, m_dCanDistMax(0)
	, m_bPlcOnOff(FALSE)
	, m_bUseAiIns(FALSE)
	, m_bWeakWeldOutputUse(FALSE)
	, m_bPassTab(FALSE)
	, m_bTabExistChk(FALSE)
	, m_nTabExist(100)
	, m_nTabExistTH(10)
	, m_dCanToCapL(3)
	, m_dCanToCapR(3)
	, m_dCanToCapLMin(0)
	, m_dCanToCapRMin(0)
	{
	m_pParent = pParent;
	m_nSelectImgaeNo = 0;
	for(int i=0; i<2; i++){
		m_dWeldingDistanceRange[i] = 1;
	}
	m_nRegPos[0]=0;
	m_nRegPos[1]=0;
	m_nRegPos1[0]=0;
	m_nRegPos1[1]=0;
}

FormTab4::~FormTab4()
{
}

void FormTab4::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_SHUTTER, m_nCamShutter);
	DDX_Check(pDX, IDC_CHECK_CONTINUEGRAB, m_bGrabContinue);
	DDX_Text(pDX, IDC_EDIT_GRABDELAY, m_nGrabDelay);
	DDX_Check(pDX, IDC_CHECK_REVERSEX, m_bImageReverseX);
	DDX_Check(pDX, IDC_CHECK_REVERSEY, m_bImageReverseY);
	DDX_Check(pDX, IDC_CHECK_SHOWINSPTIME, m_bShowInspTime);
	DDX_Check(pDX, IDC_CHECK_FULLSAVE, m_bFullSave);
	DDX_Check(pDX, IDC_CHECK_NG_SAVE, m_bNgSave);
	DDX_Check(pDX, IDC_CHECK_OVERLAYSAVE, m_bOverlaySave);
	DDX_Check(pDX, IDC_CHECK_JPEGSAVE, m_bSaveJpeg);
	DDX_Text(pDX, IDC_EDIT_SCALE, m_dCamScale);
	DDX_Text(pDX, IDC_EDIT_CHECKCAN, m_nCheckCanValue);
	DDX_Check(pDX, IDC_CHECK5, m_bMagneticUse);
	DDX_Check(pDX, IDC_CHECK_SHOWEDGE, m_bShowEdge);
	DDX_Text(pDX, IDC_EDITTABTOL, m_dTabTol);
	DDX_Check(pDX, IDC_CHECKSHOWTABAREA, m_bShowTabArea);
	DDX_Check(pDX, IDC_CHECK_TAPIMGSHOW, m_bTapImageShow);
	DDX_Text(pDX, IDC_EDITTABTOL2, m_dTapTolY1);
	DDX_Text(pDX, IDC_EDITAPSIZE, m_dTapSize);
	DDX_Text(pDX, IDC_EDIIT_TOPCAPV, m_nTopCapEdgeValue);
	DDX_Text(pDX, IDC_EDIIT_TAPEDGEV, m_nTapEdge);
	DDX_Text(pDX, IDC_EDITOPCAPSIZE, m_dTopCapSize);
	DDX_Text(pDX, IDC_EDIIT_HOLETH, m_nHoleTh);
	DDX_Text(pDX, IDC_EDIIT_MORPHCOUNT, m_nMorphCount);
	DDX_Check(pDX, IDC_CHECK8, m_bCkAuto);
	DDX_Text(pDX, IDC_EDIIT_HOLESIZE, m_nWeldingSize);
	DDX_Text(pDX, IDC_EDIIT_HOLEBLACK1, m_dLeftArea);
	DDX_Text(pDX, IDC_EDIIT_HOLEBLACK2, m_dRightArea);
	DDX_Text(pDX, IDC_EDIIT_HOLEBLACK3, m_dLeftArea1);
	DDX_Text(pDX, IDC_EDIIT_HOLEBLACK4, m_dRightArea1);
	DDX_Text(pDX, IDC_EDIIT_NV1, m_nV1);
	DDX_Text(pDX, IDC_EDIIT_NV2, m_nV2);
	DDX_Text(pDX, IDC_EDIIT_NV3, m_nV3);
	DDX_Text(pDX, IDC_EDIT_CHECKTOPCAP, m_nTopCapValue);
	DDX_Text(pDX, IDC_EDIT_CHECKTOPCAPREVERSE, m_nCapReverseV);
	DDX_Check(pDX, IDC_CHECK_TAPINSPECT, m_bTapInspect);
	DDX_Check(pDX, IDC_CHECK_HOLEINSPECT, m_bHoleInspect);
	DDX_Text(pDX, IDC_EDIIT_TOPCAPV2, m_nTopCapEdgeValue1);
	DDX_Text(pDX, IDC_EDIIT_TAPEDGEV2, m_nTapEdge1);
	DDX_Check(pDX, IDC_CHECK19, m_bHoleLeftCheck);
	DDX_Text(pDX, IDC_EDIT_CHECKTOPCAPREVERSE2, m_nCogDist);
	DDX_Check(pDX, IDC_CHECK11, m_bImageModify);
	DDX_Text(pDX, IDC_EDIT_CHECKTOPCAP2, m_nBlackArea);
	DDX_Check(pDX, IDC_CHECK13, m_bBlackArea);
	DDX_Check(pDX, IDC_CHECK15, m_bWhiteCheck);
	DDX_Check(pDX, IDC_CHECK20, m_bHoleSearchArea);
	DDX_Text(pDX, IDC_EDIIT_HOLEBLACK5, m_nSkipLeft);
	DDX_Text(pDX, IDC_EDIIT_HOLEBLACK6, m_nSkipRight);
	DDX_Check(pDX, IDC_CHECK16, m_bRoiSet);
	DDX_Text(pDX, IDC_EDIT_GRABDELAY2, m_nCylinderCloseTime);
	DDX_Check(pDX, IDC_CHECK17, m_bTapWhiteSkip);
	DDX_Check(pDX, IDC_CHECK21, m_bLowBlackSkip);
	DDX_Check(pDX, IDC_CHECK22, m_bHoleTopSearch);
	DDX_Text(pDX, IDC_EDIT_GRABDELAY3, m_nCylinderOpenTime);
	DDX_Check(pDX, IDC_CHECK26, m_bParamAnd);
	DDX_Check(pDX, IDC_CHECK27, m_bParamOr);
	DDX_Check(pDX, IDC_CHECK28, m_bHoleEdgeSkip);
	DDX_Check(pDX, IDC_CHECK29, m_b2Tab);
	DDX_Check(pDX, IDC_CHECK31, m_bHoleEdgeCheck);
	DDX_Text(pDX, IDC_EDIIT_HOLESIZE2, m_dHoleEdgemm);
	DDX_Text(pDX, IDC_EDITTABTOL3, m_dTapTolY2);
	DDX_Check(pDX, IDC_CHECK32, m_bShowHole);
	DDX_Text(pDX, IDC_EDITTABTOL4, m_dTabTol1);
	DDX_Check(pDX, IDC_CHECK34, m_bRegMode);
	DDX_Check(pDX, IDC_CHECK35, m_bLineWelding);
	DDX_Text(pDX, IDC_EDIT_CHECKTOPCAPREVERSE3, m_dMinHeight);
	DDX_Text(pDX, IDC_EDIT_CHECKTOPCAPREVERSE4, m_dMaxHeight);
	DDX_Check(pDX, IDC_CHECK36, m_bCircleFit);
	DDX_Text(pDX, IDC_EDIT2, m_nInnerCiircle);
	DDX_Check(pDX, IDC_CHECK37, m_bCircleShow);
	DDX_Text(pDX, IDC_EDIT3, m_dCenterOffset);
	DDX_Check(pDX, IDC_USE_CAP_CAN_Y, m_bUseCapCanY);
	DDX_Check(pDX, IDC_CHECK_2LINE_WELDING, m_b2LineWelding);
	DDX_Check(pDX, IDC_CHECK_TAP_TOP_REFIND, m_bTabTopRefind);
	DDX_Check(pDX, IDC_CHECK_TAB_BOTTOM_ABS_MODE, m_bTabBottomAbsMode);
	DDX_Text(pDX, IDC_EDIT_WELDING_DISTANCE, m_nRegYpos);
	DDX_Text(pDX, IDC_EDIT_WELDING_DISTANCE2, m_nTabTopTh);
	DDX_Check(pDX, IDC_CHECK_USE_TABTOP_TH, m_bUseTabTopTh);
	DDX_Check(pDX, IDC_CHECK_WELDING_POS_X_FIX, m_bWeldingPosXFix);
	DDX_Check(pDX, IDC_CHECK_WELDING_RANGE, m_bWeldingRange);
	DDX_Text(pDX, IDC_EDIT_WELDING_DISTANCE_Y, m_dWeldingDistanceY);
	DDX_Text(pDX, IDC_EDIT_WELDING_DISTANCE_RANGE_TOP, m_dWeldingDistanceRange[0]);
	DDX_Text(pDX, IDC_EDIT_WELDING_DISTANCE_RANGE_BOTTOM, m_dWeldingDistanceRange[1]);
	DDX_Check(pDX, IDC_CHECK_WELDING_RANGE_NG_SIGNAL, m_bWeldingRangeNgSignal);
	DDX_Text(pDX, IDC_EDIT_TOPCAPAVERAGE, m_nTopCapWhiteCut);
	DDX_Text(pDX, IDC_EDIIT_HOLETHWHITE, m_dWhiteCut);
	DDX_Text(pDX, IDC_EDIT7, m_dCenterOffset2);
	DDX_Text(pDX, IDC_EDIT_TABCORNERDIST, m_nTabCornerDist);
	DDX_Check(pDX, IDC_CHECK10, m_bTabCornerDist);
	DDX_Text(pDX, IDC_EDIT_WELDSTART, m_dDistWeldMin);
	DDX_Text(pDX, IDC_EDITWELDEND, m_dDistWeldMax);
	DDX_Check(pDX, IDC_CHECK_WELDDIST, m_bWeldDist);
	DDX_Check(pDX, IDC_CHECK_WELDDIST2, m_bWeldDistCap);
	DDX_Text(pDX, IDC_EDIT_WELDSTART2, m_dWeldDistCapMin);
	DDX_Text(pDX, IDC_EDITWELDEND2, m_dWeldDistCapMax);
	DDX_Check(pDX, IDC_CHECK_WEAKWELD, m_bWeakWeld);
	DDX_Text(pDX, IDC_EDIT_WEAKWELD, m_nWeakWeld);
	DDX_Check(pDX, IDC_CHECK_FRAME, m_bFrame);
	DDX_Text(pDX, IDC_EDIT_CENTEREDGE, m_nCenterEdgeValue);
	DDX_Text(pDX, IDC_EDIT_WEAKWELD2, m_nWeakWeld2);
	DDX_Check(pDX, IDC_CHECKTWOTAB, m_bTwoTab);
	DDX_Text(pDX, IDC_EDIT_CENTEREDGEX1, m_nTabEdgeX1);
	DDV_MinMaxInt(pDX, m_nTabEdgeX1, 0, 200);
	DDX_Text(pDX, IDC_EDIT_CENTEREDGEX2, m_nTabEdgeX2);
	DDV_MinMaxInt(pDX, m_nTabEdgeX2, 0, 200);
	DDX_Text(pDX, IDC_EDIT_CENTEREDGEY1, m_nTabEdgeY1);
	DDX_Text(pDX, IDC_EDIT_CENTEREDGEY2, m_nTabEdgeY2);
	DDX_Text(pDX, IDC_EDIT_2TABMIDWELD, m_dMidWeld);
	DDX_Check(pDX, IDC_CHECK_MIDWELD, m_bMidWeld);
	DDX_Text(pDX, IDC_EDIT_WELDING_DISTANCE3, m_nRegYpos1);
	DDX_Text(pDX, IDC_EDIT_TAB_EXIST_L, m_nTabExistX1);
	DDX_Text(pDX, IDC_EDIT_TAB_EXIST_R, m_nTabExistX2);
	DDX_Text(pDX, IDC_EDIT_TAB_EXIST_T, m_nTabExistY1);
	DDX_Text(pDX, IDC_EDIT_TAB_EXIST_B, m_nTabExistY2);

	DDX_Text(pDX, IDC_EDIT_2TABOFFSET, m_n2TabOffsetY);
	DDX_Text(pDX, IDC_EDIIT_WELDCHECK, m_nWeldCheck);
	DDX_Check(pDX, IDC_CHECKBOTTOMTAB, m_bBottomTab);
	DDX_Text(pDX, IDC_EDIT_WELDSTART3, m_dTabTopDistCapMin);
	DDX_Text(pDX, IDC_EDITWELDEND3, m_dTabTopDistCapMax);
	DDX_Check(pDX, IDC_CHECK_WELDDIST3, m_bTabTopDistCap);
	DDX_Text(pDX, IDC_EDIT_TC_AVE_COUNT, m_dTopCapCenterCount); //woo T/C
	DDX_Text(pDX, IDC_EDIT_CAPCANX, m_dCapCandistX);
	DDX_Text(pDX, IDC_EDIT_AI_FRAME, m_nAiFrame);	
	DDX_Text(pDX, IDC_EDIT_PARAM_CHECK_DAMAGE, m_nCheckDamage);
	DDX_Text(pDX, IDC_EDIT_PARAM_CHECK_DAMAGE_TH, m_nCheckDamageTh);
	DDX_Check(pDX, IDC_CHECK_PARAM_CHECK_DAMAGE, m_bCheckDamage);
	DDX_Text(pDX, IDC_EDIIT_HOLESIZE3, m_dHoleEdgeINmm);
	DDX_Text(pDX, IDC_EDIT_BOTTOM_AREA_TOP, m_nBottomTabTop);
	DDX_Text(pDX, IDC_EDIT_BOTTOM_AREA_BOT, m_nBottomTabBot);
	DDX_Text(pDX, IDC_EDIT_BOTTOM_AREA_LEFT, m_nBottomTabLeft);
	DDX_Text(pDX, IDC_EDIT_BOTTOM_AREA_RIGHT, m_nBottomTabRight);
	DDX_Text(pDX, IDC_EDIT_BOTTOM_TAB_FRAME, m_nBottomTabFrame);
	DDX_Text(pDX, IDC_EDIT_BOTTOM_L_TH, m_nBottomLeftTh);
	DDX_Text(pDX, IDC_EDIT_BOTTOM_R_TH, m_nBottomRightTh);
	DDX_Check(pDX, IDC_CHECK_WELDDIST4, m_bCanDist);
	DDX_Text(pDX, IDC_EDIT_CAN_STT, m_dCanDistMin);
	DDX_Text(pDX, IDC_EDIT_CAN_END, m_dCanDistMax);
	DDX_Check(pDX, IDC_CHECK_PLC_ONOFF, m_bPlcOnOff);
	DDX_Check(pDX, IDC_CHECK_WEAK_WELD_PASS, m_bUseAiIns);
	DDX_Check(pDX, IDC_CHECK_WEAK_WELD_PASS2, m_bWeakWeldOutputUse);
	DDX_Check(pDX, IDC_CHECK_PASSTAB, m_bPassTab);

	DDX_Text(pDX, IDC_EDIT_TAB_EXIST, m_nTabExist);
	DDX_Text(pDX, IDC_EDIT_TAB_EXIST2, m_nTabExistTH);
	DDX_Check(pDX, IDC_CHECK_TAB_EXIST, m_bTabExistChk);

	DDX_Text(pDX, IDC_EDIT_BOTTOM_EDGE_SPEC_L, m_dCanToCapL);
	DDX_Text(pDX, IDC_EDIT_BOTTOM_EDGE_SPEC_R, m_dCanToCapR);
	DDX_Text(pDX, IDC_EDIT_BOTTOM_EDGE_SPEC_L_MIN, m_dCanToCapLMin);
	DDX_Text(pDX, IDC_EDIT_BOTTOM_EDGE_SPEC_R_MIN, m_dCanToCapRMin);
}


BEGIN_MESSAGE_MAP(FormTab4, CDialog)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(ID_MENU_DELETE, &FormTab4::OnMenuDelete)
	ON_BN_CLICKED(IDC_BUTTON_ADJUST, &FormTab4::OnBnClickedButtonAdjust)
	ON_BN_CLICKED(IDC_CHECK_CONTINUEGRAB, &FormTab4::OnBnClickedCheckContinuegrab)
	ON_BN_CLICKED(IDC_BUTTON_GRAB, &FormTab4::OnBnClickedButtonGrab)
	ON_BN_CLICKED(IDC_CHECK_REVERSEX, &FormTab4::OnBnClickedCheckReversex)
	ON_BN_CLICKED(IDC_CHECK_REVERSEY, &FormTab4::OnBnClickedCheckReversey)
	ON_BN_CLICKED(IDC_CHECK_SHOWINSPTIME, &FormTab4::OnBnClickedCheckShowinsptime)
	ON_BN_CLICKED(IDC_CHECK_FULLSAVE, &FormTab4::OnBnClickedCheckFullsave)
	ON_BN_CLICKED(IDC_CHECK_NG_SAVE, &FormTab4::OnBnClickedCheckNgSave)
	ON_BN_CLICKED(IDC_CHECK_OVERLAYSAVE, &FormTab4::OnBnClickedCheckOverlaysave)
	ON_BN_CLICKED(IDC_CHECK_JPEGSAVE, &FormTab4::OnBnClickedCheckJpegsave)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_CHECK5, &FormTab4::OnBnClickedCheck5)
	ON_BN_CLICKED(IDC_BUTTON_LOAD, &FormTab4::OnBnClickedButtonLoad)
	ON_BN_CLICKED(IDC_BUTTON_TEST, &FormTab4::OnBnClickedButtonTest)
	ON_BN_CLICKED(IDC_CHECK_SHOWEDGE, &FormTab4::OnBnClickedCheckShowedge)
	ON_BN_CLICKED(IDC_BUTTONUpdate, &FormTab4::OnBnClickedButtonupdate)
	ON_BN_CLICKED(IDC_CHECKSHOWTABAREA, &FormTab4::OnBnClickedCheckshowtabarea)
	ON_BN_CLICKED(IDC_CHECK_TAPIMGSHOW, &FormTab4::OnBnClickedCheckTapimgshow)
	ON_BN_CLICKED(IDC_BUTTON_FOLDERTEST, &FormTab4::OnBnClickedButtonFoldertest)
	ON_BN_CLICKED(IDC_CHECK8, &FormTab4::OnBnClickedCheck8)
	ON_BN_CLICKED(IDC_BUTTON_SAVEIMAGE, &FormTab4::OnBnClickedButtonSaveimage)
	ON_BN_CLICKED(IDC_CHECK_HOLEINSPECT, &FormTab4::OnBnClickedCheckHoleinspect)
	ON_BN_CLICKED(IDC_CHECK19, &FormTab4::OnBnClickedCheck19)
	ON_BN_CLICKED(IDC_CHECK11, &FormTab4::OnBnClickedCheck11)
	ON_BN_CLICKED(IDC_CHECK13, &FormTab4::OnBnClickedCheck13)
	ON_BN_CLICKED(IDC_CHECK15, &FormTab4::OnBnClickedCheck15)
	ON_BN_CLICKED(IDC_CHECK20, &FormTab4::OnBnClickedCheck20)
	ON_BN_CLICKED(IDC_CHECK16, &FormTab4::OnBnClickedCheck16)
	ON_BN_CLICKED(IDC_CHECK17, &FormTab4::OnBnClickedCheck17)
	ON_BN_CLICKED(IDC_CHECK21, &FormTab4::OnBnClickedCheck21)
	ON_BN_CLICKED(IDC_CHECK22, &FormTab4::OnBnClickedCheck22)
	ON_BN_CLICKED(IDC_CHECK26, &FormTab4::OnBnClickedCheck26)
	ON_BN_CLICKED(IDC_CHECK27, &FormTab4::OnBnClickedCheck27)
	ON_BN_CLICKED(IDC_CHECK28, &FormTab4::OnBnClickedCheck28)
	ON_BN_CLICKED(IDC_CHECK29, &FormTab4::OnBnClickedCheck29)
	ON_BN_CLICKED(IDC_CHECK31, &FormTab4::OnBnClickedCheck31)
	ON_BN_CLICKED(IDC_CHECK32, &FormTab4::OnBnClickedCheck32)
	ON_BN_CLICKED(IDC_CHECK34, &FormTab4::OnBnClickedCheck34)
	ON_BN_CLICKED(IDC_CHECK35, &FormTab4::OnBnClickedCheck35)
	ON_BN_CLICKED(IDC_CHECK36, &FormTab4::OnBnClickedCheck36)
	ON_BN_CLICKED(IDC_CHECK37, &FormTab4::OnBnClickedCheck37)
	ON_BN_CLICKED(IDC_USE_CAP_CAN_Y, &FormTab4::OnBnClickedCheck39)
	ON_BN_CLICKED(IDC_CHECK_2LINE_WELDING, &FormTab4::OnBnClickedCheck2lineWelding)
	ON_BN_CLICKED(IDC_CHECK_TAP_TOP_REFIND, &FormTab4::OnBnClickedCheckTapTopRefind)
	ON_BN_CLICKED(IDC_CHECK_TAB_BOTTOM_ABS_MODE, &FormTab4::OnBnClickedCheckTabBottomAbsMode)
	ON_BN_CLICKED(IDC_CHECK_USE_TABTOP_TH, &FormTab4::OnBnClickedCheckUseTabtopTh)
	ON_BN_CLICKED(IDC_CHECK_WELDING_POS_X_FIX, &FormTab4::OnBnClickedCheckWeldingPosXFix)
	ON_BN_CLICKED(IDC_CHECK_WELDING_RANGE, &FormTab4::OnBnClickedCheckWeldingRange)
	ON_BN_CLICKED(IDC_BUTTONREGPOS, &FormTab4::OnBnClickedButtonregpos)
	ON_BN_CLICKED(IDC_BUTTON10, &FormTab4::OnBnClickedButton10)
	ON_BN_CLICKED(IDC_BUTTON11, &FormTab4::OnBnClickedButton11)
	ON_BN_CLICKED(IDC_BUTTONREGPOS2, &FormTab4::OnBnClickedButtonregpos2)
	ON_BN_CLICKED(IDC_CHECK10, &FormTab4::OnBnClickedCheck10)
	ON_BN_CLICKED(IDC_CHECK_WELDDIST, &FormTab4::OnBnClickedCheckWelddist)
	ON_BN_CLICKED(IDC_CHECK_WELDDIST2, &FormTab4::OnBnClickedCheckWelddist2)
	ON_BN_CLICKED(IDC_CHECK_WEAKWELD, &FormTab4::OnBnClickedCheckWeakweld)
	ON_BN_CLICKED(IDC_CHECK_FRAME, &FormTab4::OnBnClickedCheckFrame)
	ON_BN_CLICKED(IDC_CHECKTWOTAB, &FormTab4::OnBnClickedChecktwotab)
	ON_BN_CLICKED(IDC_CHECK_MIDWELD, &FormTab4::OnBnClickedCheckMidweld)
	ON_BN_CLICKED(IDC_CHECKBOTTOMTAB, &FormTab4::OnBnClickedCheckbottomtab)
	ON_BN_CLICKED(IDC_CHECK_WELDDIST3, &FormTab4::OnBnClickedCheckWelddist3)
	ON_BN_CLICKED(IDC_CHECK_PARAM_CHECK_DAMAGE, &FormTab4::OnBnClickedCheckParamCheckDamage)
	ON_BN_CLICKED(IDC_CHECK_WELDDIST4, &FormTab4::OnBnClickedCheckWelddist4)
	ON_BN_CLICKED(IDC_CHECK_PLC_ONOFF, &FormTab4::OnBnClickedCheckPlcOnoff)
	ON_BN_CLICKED(IDC_CHECK_WEAK_WELD_PASS, &FormTab4::OnBnClickedCheckWeakWeldPass)
	ON_BN_CLICKED(IDC_BUTTON3, &FormTab4::OnBnClickedButton3)
//	ON_BN_CLICKED(IDC_CHECK_WEAK_WELD_PASS2, &FormTab4::OnBnClickedCheckWeakWeldPass2)
ON_BN_CLICKED(IDC_CHECK_PASSTAB, &FormTab4::OnBnClickedCheckPasstab)
END_MESSAGE_MAP()


// FormTab4 메시지 처리기입니다.

BOOL FormTab4::OnInitDialog()
{
	CDialog::OnInitDialog();

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}


void FormTab4::OnPaint()
{
	CFlyCapture2_EXDlg *pDlg = (CFlyCapture2_EXDlg*)m_pParent;
	CPaintDC dc(this); // device context for painting	
	
}

void FormTab4::OnDestroy()
{
	CDialog::OnDestroy();

}

void FormTab4::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
//위치를 파악하고 메뉴를 보이게한다

	m_ev_rbutton_down.SetEvent();
	
	CFlyCapture2_EXDlg *pDlg = (CFlyCapture2_EXDlg*)m_pParent;
	pDlg->m_pDlgSet->m_FormTab3->m_ev_rbutton_down.SetEvent();

//	CMenu menu, *pSubMenu;
//
//	if(m_nSelectImgaeNo>0)
//	{
////		menu.LoadMenu(IDR_MENU1);
////		pSubMenu = menu.GetSubMenu(0);
////		pSubMenu->TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this);
//
//	}
	CDialog::OnRButtonDown(nFlags, point);
}

void FormTab4::OnMenuDelete()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	//위치 파악
	m_nSelectImgaeNo = 0;
}

void FormTab4::OnBnClickedButtonAdjust()
{
	//m_nCamShutter
	//m_nGrabDelay
	//m_dCamScale
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

void FormTab4::OnBnClickedCheckContinuegrab()
{
	// m_bGrabContinue
	CFlyCapture2_EXDlg *pDlg = (CFlyCapture2_EXDlg*)m_pParent;
	UpdateData();
//	if(m_bGrabContinue)
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

void FormTab4::OnBnClickedButtonGrab()
{
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

void FormTab4::OnBnClickedCheckReversex()
{
	//m_bImageReverseX
	UpdateData();
}

void FormTab4::OnBnClickedCheckReversey()
{
	// m_bImageReverseY
	UpdateData();
}

void FormTab4::OnBnClickedCheckShowinsptime()
{
	// m_bShowInspTime
	UpdateData();
	
}

void FormTab4::OnBnClickedCheckFullsave()
{
	// m_bFullSave
	UpdateData();
	if(m_bFullSave)
	{
		m_bNgSave = m_bOverlaySave= TRUE;
		UpdateData(FALSE);
	}
}

void FormTab4::OnBnClickedCheckNgSave()
{
	// m_bNgSave
	UpdateData();
	if(m_bNgSave==FALSE)
	{
		m_bFullSave = FALSE;
		UpdateData(FALSE);
	}
}

void FormTab4::OnBnClickedCheckOverlaysave()
{
	// m_bOverlaySave
	UpdateData();
	if(m_bOverlaySave==FALSE)
	{
		m_bFullSave = FALSE;
		UpdateData(FALSE);
	}
}

void FormTab4::OnBnClickedCheckJpegsave()
{
	// m_bSaveJpeg
	UpdateData();
}

//true 읽기 flase 쓰기
void FormTab4::UpdateIni(BOOL bLoad)
{
	CFlyCapture2_EXDlg *pDlg = (CFlyCapture2_EXDlg*)m_pParent;

	if(bLoad==FALSE){

		m_rtTacker[0]=pDlg->m_Display[0].m_rtImgTracker[0];
		m_rtTacker[1]=pDlg->m_Display[0].m_rtImgTracker[1];
		m_rtTacker[2]=pDlg->m_Display[0].m_rtImgTracker[2];
		m_rtTacker[3]=pDlg->m_Display[0].m_rtImgTracker[3];
		m_rtTacker[4]=pDlg->m_Display[0].m_rtImgTracker[4];
		m_rtTacker[5]=pDlg->m_Display[0].m_rtImgTracker[5];
		m_rtTacker[6]=pDlg->m_Display[0].m_rtImgTracker[6];
		m_rtTacker[7]=pDlg->m_Display[0].m_rtImgTracker[7];

		//m_rtTacker[0].SetRect(pDlg->m_Display->ScaleTrans(pDlg->m_Display[0].m_Tracker[0].m_rect.TopLeft(),0),pDlg->m_Display->ScaleTrans(pDlg->m_Display[0].m_Tracker[0].m_rect.BottomRight(),0));
		//m_rtTacker[1].SetRect(pDlg->m_Display->ScaleTrans(pDlg->m_Display[0].m_Tracker[1].m_rect.TopLeft(),0),pDlg->m_Display->ScaleTrans(pDlg->m_Display[0].m_Tracker[1].m_rect.BottomRight(),0));
		//m_rtTacker[2].SetRect(pDlg->m_Display->ScaleTrans(pDlg->m_Display[0].m_Tracker[2].m_rect.TopLeft(),0),pDlg->m_Display->ScaleTrans(pDlg->m_Display[0].m_Tracker[2].m_rect.BottomRight(),0));
		//m_rtTacker[3].SetRect(pDlg->m_Display->ScaleTrans(pDlg->m_Display[0].m_Tracker[3].m_rect.TopLeft(),0),pDlg->m_Display->ScaleTrans(pDlg->m_Display[0].m_Tracker[3].m_rect.BottomRight(),0));
		//m_rtTacker[4].SetRect(pDlg->m_Display->ScaleTrans(pDlg->m_Display[0].m_Tracker[4].m_rect.TopLeft(),0),pDlg->m_Display->ScaleTrans(pDlg->m_Display[0].m_Tracker[4].m_rect.BottomRight(),0));
		//m_rtTacker[5].SetRect(pDlg->m_Display->ScaleTrans(pDlg->m_Display[0].m_Tracker[5].m_rect.TopLeft(),0),pDlg->m_Display->ScaleTrans(pDlg->m_Display[0].m_Tracker[5].m_rect.BottomRight(),0));
		//m_rtTacker[6].SetRect(pDlg->m_Display->ScaleTrans(pDlg->m_Display[0].m_Tracker[6].m_rect.TopLeft(),0),pDlg->m_Display->ScaleTrans(pDlg->m_Display[0].m_Tracker[6].m_rect.BottomRight(),0));
		//m_rtTacker[7].SetRect(pDlg->m_Display->ScaleTrans(pDlg->m_Display[0].m_Tracker[7].m_rect.TopLeft(),0),pDlg->m_Display->ScaleTrans(pDlg->m_Display[0].m_Tracker[7].m_rect.BottomRight(),0));

		if(pDlg->m_pDlgSet->m_FormTab3->m_bManualRoi){
			m_rtTacker[0].SetRect(pDlg->m_pDlgSet->m_FormTab3->m_nRoi[0][0],pDlg->m_pDlgSet->m_FormTab3->m_nRoi[0][2],pDlg->m_pDlgSet->m_FormTab3->m_nRoi[0][1],pDlg->m_pDlgSet->m_FormTab3->m_nRoi[0][3]);
			m_rtTacker[1].SetRect(pDlg->m_pDlgSet->m_FormTab3->m_nRoi[1][0],pDlg->m_pDlgSet->m_FormTab3->m_nRoi[1][2],pDlg->m_pDlgSet->m_FormTab3->m_nRoi[1][1],pDlg->m_pDlgSet->m_FormTab3->m_nRoi[1][3]);
			m_rtTacker[2].SetRect(pDlg->m_pDlgSet->m_FormTab3->m_nRoi[2][0],pDlg->m_pDlgSet->m_FormTab3->m_nRoi[2][2],pDlg->m_pDlgSet->m_FormTab3->m_nRoi[2][1],pDlg->m_pDlgSet->m_FormTab3->m_nRoi[2][3]);
			m_rtTacker[3].SetRect(pDlg->m_pDlgSet->m_FormTab3->m_nRoi[3][0],pDlg->m_pDlgSet->m_FormTab3->m_nRoi[3][2],pDlg->m_pDlgSet->m_FormTab3->m_nRoi[3][1],pDlg->m_pDlgSet->m_FormTab3->m_nRoi[3][3]);
			m_rtTacker[4].SetRect(pDlg->m_pDlgSet->m_FormTab3->m_nRoi[4][0],pDlg->m_pDlgSet->m_FormTab3->m_nRoi[4][2],pDlg->m_pDlgSet->m_FormTab3->m_nRoi[4][1],pDlg->m_pDlgSet->m_FormTab3->m_nRoi[4][3]);
			m_rtTacker[5].SetRect(pDlg->m_pDlgSet->m_FormTab3->m_nRoi[5][0],pDlg->m_pDlgSet->m_FormTab3->m_nRoi[5][2],pDlg->m_pDlgSet->m_FormTab3->m_nRoi[5][1],pDlg->m_pDlgSet->m_FormTab3->m_nRoi[5][3]);
		}
		UpdateData(TRUE);
	}

	CString strPath;
	strPath.Format("%ssetup.ini",GLIM_PATH);
	strPath.Format("c:\\Glim\\Model\\%s.ini",pDlg->m_pDlgSet->m_strSelectProduct);

	CIni ini1(strPath, "Insp_Parameter" , 0 );
	//Camset
	ini1.SER_GETD(bLoad, m_nCamShutter	, 14000);
	ini1.SER_GETD(bLoad, m_nGrabDelay	, 50);
	ini1.SER_GETD(bLoad, m_dCamScale	, 0.0188);

	//image set
	ini1.SER_GETD(bLoad, m_bImageReverseX	, 0);
	ini1.SER_GETD(bLoad, m_bImageReverseY	, 0);
	ini1.SER_GETD(bLoad, m_bShowInspTime	, 0);
	ini1.SER_GETD(bLoad, m_bFullSave	, 1);
	ini1.SER_GETD(bLoad, m_bOverlaySave	, 1);
	ini1.SER_GETD(bLoad, m_bNgSave	, 1);
	ini1.SER_GETD(bLoad, m_bSaveJpeg	, 0);
	ini1.SER_GETD(bLoad, m_nCheckCanValue	, 60);//오창 60
	ini1.SER_GETD(bLoad, m_bMagneticUse	, 1);
	ini1.SER_GETD(bLoad, m_dTabTol	, 3.5);
	ini1.SER_GETD(bLoad, m_dTapTolY1	, 2.3);
	ini1.SER_GETD(bLoad, m_dTapTolY2	, 1.3);
	ini1.SER_GETD(bLoad, m_dTapSize	, 3);
	ini1.SER_GETD(bLoad, m_nTopCapEdgeValue	, 10);
	ini1.SER_GETD(bLoad, m_nTopCapEdgeValue1	, 10);
	ini1.SER_GETD(bLoad, m_nTapEdge	, 10);//left tap edge
	ini1.SER_GETD(bLoad, m_nTapEdge1	, 10);//right tap edge

	ini1.SER_GETD(bLoad, m_dTopCapSize	, 18);

	ini1.SER_GETD(bLoad, m_nHoleTh	, 90);
	ini1.SER_GETD(bLoad, m_nMorphCount	, 2);
	ini1.SER_GETD(bLoad, m_nWeldingSize	, 60);
	ini1.SER_GETD(bLoad, m_dLeftArea	, 30);
	ini1.SER_GETD(bLoad, m_dRightArea	, 6);
	ini1.SER_GETD(bLoad, m_dLeftArea1	, 30);
	ini1.SER_GETD(bLoad, m_dRightArea1	, 6);
	ini1.SER_GETD(bLoad, m_nV1	, 12);
	ini1.SER_GETD(bLoad, m_nV2	, 15);
	ini1.SER_GETD(bLoad, m_nV3	, 22);
	ini1.SER_GETD(bLoad, m_nRegYpos	, 80);
	ini1.SER_GETD(bLoad, m_nTopCapValue	, 60);//오창 60
	ini1.SER_GETD(bLoad, m_nCapReverseV	, 60);//오창 60
	ini1.SER_GETD(bLoad, m_bHoleInspect	, 1);//오창 60
	ini1.SER_GETD(bLoad, m_bTapInspect	, 1);//오창 60
	ini1.SER_GETD(bLoad, m_bShowTabArea	, 1);
	ini1.SER_GETD(bLoad, m_bHoleLeftCheck	, 1);
	ini1.SER_GETD(bLoad, m_nCogDist	, 20);
	ini1.SER_GETD(bLoad, m_bBlackArea	, 0);
	ini1.SER_GETD(bLoad, m_nBlackArea	, 5000);
	ini1.SER_GETD(bLoad, m_bWhiteCheck	, 0);
	ini1.SER_GETD(bLoad, m_bHoleSearchArea	, 1);
	ini1.SER_GETD(bLoad, m_bShowHole	, 1);

	pDlg->m_Display[0].m_bHoleSearchArea = m_bHoleSearchArea;

	ini1.SER_GETD(bLoad, m_nSkipLeft	, 0);
	ini1.SER_GETD(bLoad, m_nSkipRight	, 0);
	ini1.SER_GETD(bLoad, m_nCylinderCloseTime	, 300);
	ini1.SER_GETD(bLoad, m_bTapWhiteSkip	, 0);
	ini1.SER_GETD(bLoad, m_bLowBlackSkip	, 0);
	ini1.SER_GETD(bLoad, m_bHoleTopSearch	, 0);
	ini1.SER_GETD(bLoad, m_nCylinderOpenTime	, 100);
	ini1.SER_GETD(bLoad, m_bImageModify	, 0);
	ini1.SER_GETD(bLoad, m_bParamAnd	, 0);
	ini1.SER_GETD(bLoad, m_bParamOr	, 1);
	ini1.SER_GETD(bLoad, m_bHoleEdgeSkip	, 1);
	ini1.SER_GETD(bLoad, m_b2Tab	, 0);
	ini1.SER_GETD(bLoad, m_bHoleEdgeCheck	, 0);
	ini1.SER_GETD(bLoad, m_dHoleEdgemm	, 0.5);
	ini1.SER_GETD(bLoad, m_dHoleEdgeINmm, 1);
	ini1.SER_GETD(bLoad, m_bUseCapCanY	, 0);
	ini1.SER_GETD(bLoad, m_dCenterOffset	, 10);
	ini1.SER_GETD(bLoad, m_dCenterOffset2	, 1);
	ini1.SER_GETD(bLoad, m_rtTacker[0]	, CRect(0,0,100,100));
	if(bLoad && m_rtTacker[0].Width()<=5 )m_rtTacker[0].right = m_rtTacker[0].left+20;
	if(bLoad && m_rtTacker[0].Height()<=5 )m_rtTacker[0].bottom = m_rtTacker[0].top+20;

	ini1.SER_GETD(bLoad, m_rtTacker[1]	, CRect(100,100,200,200));
	if(bLoad && m_rtTacker[1].Width()<=5 )m_rtTacker[1].right = m_rtTacker[1].left+20;
	if(bLoad && m_rtTacker[1].Height()<=5 )m_rtTacker[1].bottom = m_rtTacker[1].top+20;

	if( m_bHoleSearchArea)
		ini1.SER_GETD(bLoad, m_rtTacker[2]	, CRect(200,200,300,300));
	if(bLoad && m_rtTacker[2].Width()<=5 )m_rtTacker[2].right = m_rtTacker[2].left+20;
	if(bLoad && m_rtTacker[2].Height()<=5 )m_rtTacker[2].bottom = m_rtTacker[2].top+20;

	ini1.SER_GETD(bLoad, m_rtTacker[3]	, CRect(100,100,200,200));
	if(bLoad && m_rtTacker[3].Width()<=5 )m_rtTacker[3].right = m_rtTacker[3].left+20;
	if(bLoad && m_rtTacker[3].Height()<=5 )m_rtTacker[3].bottom = m_rtTacker[3].top+20;

	ini1.SER_GETD(bLoad, m_rtTacker[4]	, CRect(100,100,200,200));
	if(bLoad && m_rtTacker[4].Width()<=5 )m_rtTacker[4].right = m_rtTacker[4].left+20;
	if(bLoad && m_rtTacker[4].Height()<=5 )m_rtTacker[4].bottom = m_rtTacker[4].top+20;

	ini1.SER_GETD(bLoad, m_rtTacker[5]	, CRect(100,100,200,200));
	if(bLoad && m_rtTacker[5].Width()<=5 )m_rtTacker[5].right = m_rtTacker[5].left+20;
	if(bLoad && m_rtTacker[5].Height()<=5 )m_rtTacker[5].bottom = m_rtTacker[5].top+20;

	//if(pDlg->m_pDlgSet->m_FormTab3->m_bManualRoi){
		pDlg->m_pDlgSet->m_FormTab3->m_nRoi[0][0]=m_rtTacker[0].left;
		pDlg->m_pDlgSet->m_FormTab3->m_nRoi[0][1]=m_rtTacker[0].right;
		pDlg->m_pDlgSet->m_FormTab3->m_nRoi[0][2]=m_rtTacker[0].top;
		pDlg->m_pDlgSet->m_FormTab3->m_nRoi[0][3]=m_rtTacker[0].bottom;

		pDlg->m_pDlgSet->m_FormTab3->m_nRoi[1][0]=m_rtTacker[1].left;
		pDlg->m_pDlgSet->m_FormTab3->m_nRoi[1][1]=m_rtTacker[1].right;
		pDlg->m_pDlgSet->m_FormTab3->m_nRoi[1][2]=m_rtTacker[1].top;
		pDlg->m_pDlgSet->m_FormTab3->m_nRoi[1][3]=m_rtTacker[1].bottom;

		pDlg->m_pDlgSet->m_FormTab3->m_nRoi[2][0]=m_rtTacker[2].left;
		pDlg->m_pDlgSet->m_FormTab3->m_nRoi[2][1]=m_rtTacker[2].right;
		pDlg->m_pDlgSet->m_FormTab3->m_nRoi[2][2]=m_rtTacker[2].top;
		pDlg->m_pDlgSet->m_FormTab3->m_nRoi[2][3]=m_rtTacker[2].bottom;

		pDlg->m_pDlgSet->m_FormTab3->m_nRoi[3][0]=m_rtTacker[3].left;
		pDlg->m_pDlgSet->m_FormTab3->m_nRoi[3][1]=m_rtTacker[3].right;
		pDlg->m_pDlgSet->m_FormTab3->m_nRoi[3][2]=m_rtTacker[3].top;
		pDlg->m_pDlgSet->m_FormTab3->m_nRoi[3][3]=m_rtTacker[3].bottom;

		pDlg->m_pDlgSet->m_FormTab3->m_nRoi[4][0]=m_rtTacker[4].left;
		pDlg->m_pDlgSet->m_FormTab3->m_nRoi[4][1]=m_rtTacker[4].right;
		pDlg->m_pDlgSet->m_FormTab3->m_nRoi[4][2]=m_rtTacker[4].top;
		pDlg->m_pDlgSet->m_FormTab3->m_nRoi[4][3]=m_rtTacker[4].bottom;

		pDlg->m_pDlgSet->m_FormTab3->m_nRoi[5][0]=m_rtTacker[5].left;
		pDlg->m_pDlgSet->m_FormTab3->m_nRoi[5][1]=m_rtTacker[5].right;
		pDlg->m_pDlgSet->m_FormTab3->m_nRoi[5][2]=m_rtTacker[5].top;
		pDlg->m_pDlgSet->m_FormTab3->m_nRoi[5][3]=m_rtTacker[5].bottom;
	//}



	ini1.SER_GETD(bLoad, m_rtTacker[6]	, CRect(100,100,200,200));
	if(bLoad && m_rtTacker[6].Width()<=5 )m_rtTacker[6].right = m_rtTacker[6].left+20;
	if(bLoad && m_rtTacker[6].Height()<=5 )m_rtTacker[6].bottom = m_rtTacker[6].top+20;
	ini1.SER_GETD(bLoad, m_rtTacker[7]	, CRect(100,100,200,200));
	if(bLoad && m_rtTacker[7].Width()<=5 )m_rtTacker[7].right = m_rtTacker[7].left+20;
	if(bLoad && m_rtTacker[7].Height()<=5 )m_rtTacker[7].bottom = m_rtTacker[7].top+20;
	ini1.SER_GETD(bLoad, m_dTabTol1	, 3);
	ini1.SER_GETD(bLoad, m_dMinHeight	, 20);
	ini1.SER_GETD(bLoad, m_dMaxHeight	, 60);
	ini1.SER_GETD(bLoad, m_bCircleFit	, 1);
	ini1.SER_GETD(bLoad, m_nInnerCiircle	, 170);
	ini1.SER_GETD(bLoad, m_bLineWelding	, 0);
	ini1.SER_GETD(bLoad, m_b2LineWelding	, 0);
	ini1.SER_GETD(bLoad, m_bTabTopRefind	, 0);
	m_bRegMode = 1;
	ini1.SER_GETD(bLoad, m_bRegMode	, 1);
	ini1.SER_GETD(bLoad, m_bTabBottomAbsMode, 1);
	ini1.SER_GETD(bLoad, m_nTabTopTh, 20);
	ini1.SER_GETD(bLoad, m_bUseTabTopTh, 0);
	ini1.SER_GETD(bLoad, m_bWeldingPosXFix, 1);
	ini1.SER_GETD(bLoad, m_bWeldingRange, 0);
	ini1.SER_GETD(bLoad, m_dWeldingDistanceY, 2.6);
	ini1.SER_GETD(bLoad, m_dWeldingDistanceRange[0], 1);
	ini1.SER_GETD(bLoad, m_dWeldingDistanceRange[1], 1);
	ini1.SER_GETD(bLoad, m_bWeldingRangeNgSignal, 0);

	ini1.SER_GETD(bLoad, m_n2TabOffsetY, 100);

	

	ini1.SER_GETD(bLoad, m_nTabCornerDist, 20);
	ini1.SER_GETD(bLoad, m_bTabCornerDist, 0);

	ini1.SER_GETD(bLoad, m_dDistWeldMin, 0);
	ini1.SER_GETD(bLoad, m_dDistWeldMax, 20);
	ini1.SER_GETD(bLoad, m_bWeldDist, 1);
	
	ini1.SER_GETD(bLoad, m_dWeldDistCapMin, 0);
	ini1.SER_GETD(bLoad, m_dWeldDistCapMax, 20);
	ini1.SER_GETD(bLoad, m_bWeldDistCap, 1);
	ini1.SER_GETD(bLoad,m_bCanDist,0);
	ini1.SER_GETD(bLoad,m_bTabTopDistCap,0);
	ini1.SER_GETD(bLoad,m_dTabTopDistCapMin,0);
	ini1.SER_GETD(bLoad,m_dTabTopDistCapMax,20);
	ini1.SER_GETD(bLoad,m_dTopCapCenterCount,0);
	ini1.SER_GETD(bLoad,m_dCapCandistX,0);
	ini1.SER_GETD(bLoad,m_nAiFrame,7);
	ini1.SER_GETD(bLoad,m_nCheckDamage,50);
	ini1.SER_GETD(bLoad,m_nCheckDamageTh,1000);
	ini1.SER_GETD(bLoad,m_bCheckDamage,0);
	ini1.SER_GETD(bLoad,m_dCanDistMin,0);
	ini1.SER_GETD(bLoad,m_dCanDistMax,20);
	ini1.SER_GETD(bLoad,m_bPlcOnOff,0);
	ini1.SER_GETD(bLoad,m_bUseAiIns,0);
	ini1.SER_GETD(bLoad,m_bWeakWeldOutputUse,0);
	ini1.SER_GETD(bLoad,m_bPassTab,0);
	ini1.SER_GETD(bLoad, m_nWeakWeld, 10);
	ini1.SER_GETD(bLoad,m_nWeakWeld2,3000);
	ini1.SER_GETD(bLoad, m_bWeakWeld, 0);
	ini1.SER_GETD(bLoad, m_bFrame, 0);
	ini1.SER_GETD(bLoad, m_nWeldCheck, 0);
	ini1.SER_GETD(bLoad, m_nCenterEdgeValue, 25);
	ini1.SER_GETD(bLoad,m_bTwoTab, 0);
	ini1.SER_GETD(bLoad,m_nTabEdgeX1, 50);
	ini1.SER_GETD(bLoad,m_nTabEdgeX2,50);
	ini1.SER_GETD(bLoad,m_nTabEdgeY1, 80);
	ini1.SER_GETD(bLoad,m_nTabEdgeY2, -30);
	ini1.SER_GETD(bLoad,m_nBottomTabTop, 50);
	ini1.SER_GETD(bLoad,m_nBottomTabBot, 30);
	ini1.SER_GETD(bLoad, m_nBottomTabLeft, 200);
	ini1.SER_GETD(bLoad, m_nBottomTabRight, 200);
	ini1.SER_GETD(bLoad,m_nBottomLeftTh, 30);
	ini1.SER_GETD(bLoad,m_nBottomRightTh, 30);	
	ini1.SER_GETD(bLoad,m_bMidWeld, 0);
	ini1.SER_GETD(bLoad,m_dMidWeld, 500);
	ini1.SER_GETD(bLoad,m_bBottomTab, 0);
	ini1.SER_GETD(bLoad, m_nRegYpos1	, 80);
	ini1.SER_GETD(bLoad, m_nTopCapWhiteCut, 100);// 그리퍼가 못잡을 경우 탑캡 밝아지는 거 스킷용
	ini1.SER_GETD(bLoad, m_dWhiteCut, 130);//백색 용접 상태 컷 스트롱 값 적용
	ini1.SER_GETD(bLoad, m_nBottomTabFrame, 2);
	
	ini1.SER_GETD(bLoad,m_nTabExist,100);
	ini1.SER_GETD(bLoad,m_nTabExistTH,10);
	ini1.SER_GETD(bLoad,m_bTabExistChk,0);
	ini1.SER_GETD(bLoad,m_nTabExistX1, 40);
	ini1.SER_GETD(bLoad,m_nTabExistX2, 40);
	ini1.SER_GETD(bLoad,m_nTabExistY1, -40);
	ini1.SER_GETD(bLoad,m_nTabExistY2, 70);

	ini1.SER_GETD(bLoad,m_dCanToCapL,0);
	ini1.SER_GETD(bLoad,m_dCanToCapR,0);
	ini1.SER_GETD(bLoad,m_dCanToCapLMin,0);
	ini1.SER_GETD(bLoad,m_dCanToCapRMin,0);

	pDlg->m_Display[0].m_bRegMode = m_bRegMode;
	pDlg->m_Display[0].m_bTrackerUse = m_bRoiSet;
		
	ini1.SER_GETD(bLoad, m_nRegPos[0], 10);
	ini1.SER_GETD(bLoad, m_nRegPos[1], 10);
	ini1.SER_GETD(bLoad, m_nRegPos1[0], 10);
	ini1.SER_GETD(bLoad, m_nRegPos1[1], 10);

	pDlg->m_Project.m_nRegWeldPos[0] = m_nRegPos[0];
	pDlg->m_Project.m_nRegWeldPos[1] = m_nRegPos[1];
	pDlg->m_Project.m_nRegWeldPos1[0] = m_nRegPos1[0];
	pDlg->m_Project.m_nRegWeldPos1[1] = m_nRegPos1[1];
	
	if(m_dCamScale==0)m_dCamScale = 0.0178;
	int ntL = m_dTabTol/m_dCamScale;
	int ntR = m_dTabTol1/m_dCamScale;

	m_nTabTol = m_dTabTol/m_dCamScale;

	if(bLoad){
		pDlg->m_Display[0].m_Tracker[0].m_rect.SetRect(pDlg->m_Display->ScaleTrans(m_rtTacker[0].TopLeft(),1),pDlg->m_Display->ScaleTrans(m_rtTacker[0].BottomRight(),1));
		pDlg->m_Display[0].m_Tracker[1].m_rect.SetRect(pDlg->m_Display->ScaleTrans(m_rtTacker[1].TopLeft(),1),pDlg->m_Display->ScaleTrans(m_rtTacker[1].BottomRight(),1));
		if( m_bHoleSearchArea)
			pDlg->m_Display[0].m_Tracker[2].m_rect.SetRect(pDlg->m_Display->ScaleTrans(m_rtTacker[2].TopLeft(),1),pDlg->m_Display->ScaleTrans(m_rtTacker[2].BottomRight(),1));
		pDlg->m_Display[0].m_Tracker[3].m_rect.SetRect(pDlg->m_Display->ScaleTrans(m_rtTacker[3].TopLeft(),1),pDlg->m_Display->ScaleTrans(m_rtTacker[3].BottomRight(),1));
		pDlg->m_Display[0].m_Tracker[4].m_rect.SetRect(pDlg->m_Display->ScaleTrans(m_rtTacker[4].TopLeft(),1),pDlg->m_Display->ScaleTrans(m_rtTacker[4].BottomRight(),1));
		pDlg->m_Display[0].m_Tracker[5].m_rect.SetRect(pDlg->m_Display->ScaleTrans(m_rtTacker[5].TopLeft(),1),pDlg->m_Display->ScaleTrans(m_rtTacker[5].BottomRight(),1));
		pDlg->m_Display[0].m_Tracker[6].m_rect.SetRect(pDlg->m_Display->ScaleTrans(m_rtTacker[6].TopLeft(),1),pDlg->m_Display->ScaleTrans(m_rtTacker[6].BottomRight(),1));
		pDlg->m_Display[0].m_Tracker[7].m_rect.SetRect(pDlg->m_Display->ScaleTrans(m_rtTacker[7].TopLeft(),1),pDlg->m_Display->ScaleTrans(m_rtTacker[7].BottomRight(),1));

		pDlg->m_Display[0].m_rtImgTracker[0] = m_rtTacker[0];
		pDlg->m_Display[0].m_rtImgTracker[1] = m_rtTacker[1];
		if(m_bHoleSearchArea)
			pDlg->m_Display[0].m_rtImgTracker[2] = m_rtTacker[2];
		pDlg->m_Display[0].m_rtImgTracker[3] = m_rtTacker[3];
		pDlg->m_Display[0].m_rtImgTracker[4] = m_rtTacker[4];
		pDlg->m_Display[0].m_rtImgTracker[5] = m_rtTacker[5];
		pDlg->m_Display[0].m_rtImgTracker[6] = m_rtTacker[6];
		pDlg->m_Display[0].m_rtImgTracker[7] = m_rtTacker[7];
		UpdateData(FALSE);
	}

	pDlg->m_pDlgSet->m_FormTab3->UpdateData(FALSE);

	if(pDlg->m_pDlgSet->m_FormTab3->m_bManualRoi){
		pDlg->m_Display[0].m_rtImgTracker[0] = m_rtTacker[0];
		pDlg->m_Display[0].m_rtImgTracker[1] = m_rtTacker[1];
		pDlg->m_Display[0].m_rtImgTracker[2] = m_rtTacker[2];
		pDlg->m_Display[0].m_rtImgTracker[3] = m_rtTacker[3];
		pDlg->m_Display[0].m_rtImgTracker[4] = m_rtTacker[4];
		pDlg->m_Display[0].m_rtImgTracker[5] = m_rtTacker[5];
		CDC* pDC=pDlg->GetDC();
		pDlg->m_Display[0].DrawImage(pDC);
		ReleaseDC(pDC);
	}
	if(m_b2LineWelding == TRUE || m_bLineWelding == TRUE){
		SetDlgItemText(IDC_STATIC_WELDING_1, _T("Line1"));
		SetDlgItemText(IDC_STATIC_WELDING_2, _T("Line2"));
	}
	else{
		SetDlgItemText(IDC_STATIC_WELDING_1, _T("Left"));
		SetDlgItemText(IDC_STATIC_WELDING_2, _T("Right"));
	}
	BOOL bshow = FALSE;
	if(m_bMidWeld)bshow = TRUE;
	GetDlgItem(IDC_EDIT_2TABMIDWELD)->EnableWindow(bshow);
	GetDlgItem(IDC_EDIT_WELDING_DISTANCE3)->EnableWindow(bshow);
	GetDlgItem(IDC_STATIC_MIDPOS12)->EnableWindow(bshow);

}

void FormTab4::OnTimer(UINT_PTR nIDEvent)
{
	CFlyCapture2_EXDlg *pDlg = (CFlyCapture2_EXDlg*)m_pParent;

	CDialog::OnTimer(nIDEvent);
}

void FormTab4::OnBnClickedCheck5()
{//m_bMagneticUse
	CFlyCapture2_EXDlg *pDlg = (CFlyCapture2_EXDlg*)m_pParent;
	UpdateData();
	pDlg->m_IO.WriteOutputPort(11,m_bMagneticUse);
}

void FormTab4::OnBnClickedButtonLoad()
{
	CFlyCapture2_EXDlg *pDlg = (CFlyCapture2_EXDlg*)m_pParent;

	pDlg->m_pDlgSet->m_FormTab3->OnBnClickedButtonLoad3();
	return;

	//	CFileDialog dlg(TRUE, _T("bmp"), NULL, OFN_SHAREAWARE, _T("Bitmap Files (*.bmp)|*.bmp||"));
	CFileDialog dlg(TRUE, NULL, NULL, OFN_SHAREAWARE, "All Files(*.*)|*.*||");//szFilter);

	if(dlg.DoModal() == IDOK)
	{

		pDlg->Invalidate();
		CString strpath,str1,str2;
		strpath = dlg.GetPathName();
		int n = strpath.Find("Cam");
		int l = strpath.GetLength();
		str1 = strpath.Left(n+3);
		str2.Format("%s2%s",str1,strpath.Right(l-n-4));
		pDlg->m_Display[0].LoadImg(dlg.GetPathName());

		pDlg->m_Project.m_fm[0] =pDlg-> m_Display[0].m_pImg->GetImagePtr();
		pDlg->m_Display[0].SetZoom(0,CPoint(0,0));
		//pDlg->Invalidate();
	}
}

void FormTab4::OnBnClickedButtonTest()
{
	CFlyCapture2_EXDlg *pDlg = (CFlyCapture2_EXDlg*)m_pParent;
	
	if(pDlg->m_bAuto)
		return;
	else
		pDlg->Test(0);

	//pDlg->CheckMachine();
}

void FormTab4::OnBnClickedCheckShowedge()
{//m_bShowEdge

	UpdateData();
}

void FormTab4::OnBnClickedButtonupdate()
{
	//m_dTabTol(x)  m_dTapTolY1 //m_dTapSize //m_nTopCapEdgeValue //m_nTapEdge //m_dTopCapSize
	//m_nHoleTh	//m_nMorphCount //m_nWeldingSize
	//, m_dLeftArea(0), m_dRightArea(0), m_dLeftArea1(0), m_dRightArea1(0)
	//m_nTopCapValue//m_nCapReverseV//m_dTabTol
	//m_nInnerCiircle
	UpdateData();


	if(m_dCamScale==0)m_dCamScale = 0.0178;
	int ntL = m_dTabTol/m_dCamScale;
	int ntR = m_dTabTol1/m_dCamScale;

	m_nTabTol = m_dTabTol/m_dCamScale;

	CFlyCapture2_EXDlg *pDlg = (CFlyCapture2_EXDlg*)m_pParent;
/*
	CTime ttime = CTime::GetCurrentTime();
	CString kk = ttime.Format("%Y%m%d");
	CString kk2 = ttime.Format("%Y%m");
	CString kk3 = ttime.Format("%d");
	CString kk1 = ttime.Format("[%Y%m%d_%H%M%S]");

	CString str1;
	str1.Format("d:\\Backup\\");
	CreateDirectory(str1,NULL);
	str1.Format("d:\\Backup\\Model\\");
	CreateDirectory(str1,NULL);
	str1.Format("d:\\Backup\\Model\\%s\\",kk2);
	CreateDirectory(str1,NULL);

	CString strOrigin,strCopy,strcom;
	strOrigin.Format("C:\\Gilm\\Model\\%s.ini","115");
	strCopy.Format("D:\\Backup\\Model\\%s\\%s%s.ini",kk2,kk1,"115");
	BOOL b1 = CopyFile(strOrigin,strCopy,0);
	CFileFind finder;
	BOOL isfind;
	isfind = finder.FindFile(strOrigin);
//C:\Glim\Model
*/

}

void FormTab4::OnBnClickedCheckshowtabarea()
{//m_bShowTabArea
	UpdateData();
}

void FormTab4::OnBnClickedCheckTapimgshow()
{//m_bTapImageShow
	UpdateData();
}

int CALLBACK BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
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

void FormTab4::OnBnClickedButtonFoldertest()
{
	CFile file;
	CString str,strng,strok;
	TCHAR	spath[100];
	BROWSEINFO bi;
	memset(&bi,0,sizeof(bi));
	bi.hwndOwner = GetSafeHwnd();
	bi.ulFlags=BIF_RETURNONLYFSDIRS;
	bi.lpfn = BrowseCallbackProc; 

	CTime ttime = CTime::GetCurrentTime();
	CString kk3 = ttime.Format("%y");
	CString kk4 = ttime.Format("%m");
	CString kk5 = ttime.Format("%d");
	CString strInitPath;
	strInitPath.Format(_T("d:\\IMAGE\\VISION\\DEFECT\\%s\\%s\\%s\\"),kk3,kk4,kk5); 
	bi.lParam = (LPARAM)strInitPath.GetBuffer(strInitPath.GetLength()); 
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
	else
	{
	}

	CString src, tar,strDevice;
	HANDLE hFind;				// FindFirstFile 핸들
	WIN32_FIND_DATA	fd;			// 검색한 화일의 정보
	BOOL bRet = TRUE;
	str = LPCTSTR(spath);
	str += "\\*.bmp";
	int ntmp =0;
	CString strMonth;
	CString strtime;
	int Ret = 0;
	bool bBmp;
	bRet = TRUE;
	hFind = FindFirstFile( str, &fd);
	strDevice = m_strfile.Left(3);
	DWORD dwST,dwED;


	CFlyCapture2_EXDlg *pDlg = (CFlyCapture2_EXDlg*)m_pParent;
	pDlg->m_bAuto = true;

	while(hFind != INVALID_HANDLE_VALUE && bRet)
	{			
		m_strBeforeImg = m_strLastFile;
		m_strfile.Format(_T("%s\\%s"), spath, fd.cFileName);
		if( !(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) )		// Not directory, which means file.
		{
			Ret = 0;
			pDlg->m_Display[0].LoadImg(m_strfile);
			m_strLastFile = m_strfile;
			pDlg->m_Project.m_fm[0] =pDlg-> m_Display[0].m_pImg->GetImagePtr();
			memcpy(pDlg->m_DisplaySave[0].m_pImg->GetImagePtr(),pDlg-> m_Display[0].m_pImg->GetImagePtr(),CAMERA_WIDTH*CAMERA_HEIGHT);
			 
			pDlg->m_Project.m_nFrameCount++;
			Sleep(1);
			dwST = GetTickCount();		
			pDlg->Test(0);
			dwED = GetTickCount();
			CString strtime;
			strtime.Format(_T("%d ms"),dwED-dwST);
//	 		m_labelSendResult.SetText(strtime);

			if(!m_bCkAuto)	
			{
				while( WaitForSingleObject((HANDLE)m_ev_rbutton_down, 50) != WAIT_OBJECT_0 )
				{
					pDlg->Wait(50);

				}
			}
			else
				pDlg->Wait(50);
		}
		else if( m_strfile == "." || m_strfile == "..")
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
	pDlg->m_bAuto = FALSE;

	AfxMessageBox("Directory Inspect Complete!!!");
}

void FormTab4::OnBnClickedCheck8()
{//m_bCkAuto
	UpdateData();
}

#include <math.h>
void FormTab4::OnBnClickedButtonSaveimage()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CFlyCapture2_EXDlg *pDlg = (CFlyCapture2_EXDlg*)m_pParent;
	CString str;
	//str.Format("%s",pDlg->m_strComname);
	str.Format("%s.jpg", m_strfile);
	pDlg->m_Display[0].SaveTestImg(str, pDlg->ComputerUserName());
/*	
	int x1=10,x2=24;

	for(int i=x2-x1;i>0;i--)
	{
		double y1 = i*cos(45*3.14/180);
		double y2 = i*sin(45*3.14/180);
		int y4 = i*cos(45*3.14/180);
		int y5 = i*sin(45*3.14/180);
		int y3 = 0;
	}
*/
}

void FormTab4::OnBnClickedCheckHoleinspect()
{//m_bHoleInspect
	UpdateData();
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void FormTab4::OnBnClickedCheck19()
{//m_bHoleLeftCheck
	UpdateData();
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void FormTab4::OnBnClickedCheck11()
{//m_bImageModify
	UpdateData();
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void FormTab4::OnBnClickedCheck13()
{//m_bBlackArea
	UpdateData();
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void FormTab4::OnBnClickedCheck15()
{//m_bWhiteCheck
	UpdateData();
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void FormTab4::OnBnClickedCheck20()
{//m_bHoleSearchArea
	CFlyCapture2_EXDlg *pDlg = (CFlyCapture2_EXDlg*)m_pParent;
	UpdateData();
	pDlg->m_Display[0].m_bHoleSearchArea = m_bHoleSearchArea;

	pDlg->InvalidateRect(CRect(14,163,715,670),FALSE);
}

void FormTab4::OnBnClickedCheck16()
{//m_bRoiSet
	UpdateData();
	CFlyCapture2_EXDlg *pDlg = (CFlyCapture2_EXDlg*)m_pParent;
	pDlg->m_Display[0].m_bTrackerUse = m_bRoiSet;
	
}

void FormTab4::OnBnClickedCheck17()
{//m_bTapWhiteSkip
	UpdateData();
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void FormTab4::OnBnClickedCheck21()
{//m_bLowBlackSkip
	UpdateData();
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void FormTab4::OnBnClickedCheck22()
{//m_bHoleTopSearch // 용접이 약할경우 체크 8호기
	UpdateData();
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void FormTab4::OnBnClickedCheck26()
{//m_bParamAnd
	UpdateData();
	if(m_bParamAnd)
	{
			if(m_bParamOr){
			m_bParamOr = FALSE;
			UpdateData(FALSE);
		}
	}
	else
	{
		if(!m_bParamOr)
		{
			m_bParamOr = TRUE;
			UpdateData(FALSE);
		}
	}

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void FormTab4::OnBnClickedCheck27()
{//m_bParamOr
	UpdateData();
	if(m_bParamOr)
	{
		if(m_bParamAnd){
			m_bParamAnd = FALSE;
			UpdateData(FALSE);
		}
	}
	else
	{
		if(!m_bParamAnd)
		{
			m_bParamAnd = TRUE;
			UpdateData(FALSE);
		}
	}
}

void FormTab4::OnBnClickedCheck28()
{//m_bHoleEdgeSkip
	UpdateData();
}

void FormTab4::OnBnClickedCheck29()
{//m_b2Tab
	UpdateData();
	if(m_b2Tab){

	}
}

void FormTab4::OnBnClickedCheck31()
{//m_bHoleEdgeCheck
	UpdateData();
}

void FormTab4::ShowHide(BOOL Show)
{
	GetDlgItem(IDC_CHECK26)->EnableWindow(Show);//and
	GetDlgItem(IDC_CHECK27)->EnableWindow(Show);//or
	GetDlgItem(IDC_CHECK_HOLEINSPECT)->EnableWindow(Show);//or
}

void FormTab4::OnBnClickedCheck32()
{//m_bShowHole
	UpdateData();
}

void FormTab4::OnBnClickedCheck34()
{//m_bRegMode
	UpdateData();
	CFlyCapture2_EXDlg *pDlg = (CFlyCapture2_EXDlg*)m_pParent;

	pDlg->m_Display[0].m_bRegMode = m_bRegMode;
}

void FormTab4::OnBnClickedCheck35()
{//m_bLineWelding
	UpdateData();

	if(m_bLineWelding == FALSE)
		m_b2LineWelding = FALSE;

	if(m_bLineWelding == TRUE && m_b2LineWelding == FALSE)
		HideParam(FALSE);
	else
		HideParam(TRUE);

	UpdateData(FALSE);
}

void FormTab4::OnBnClickedCheck36()
{//m_bCircleFit
	UpdateData();
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void FormTab4::OnBnClickedCheck37()
{//m_bCircleShow
	UpdateData();
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void FormTab4::OnBnClickedCheck39()
{//m_bCenterOffset
	UpdateData();
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void FormTab4::OnBnClickedCheck2lineWelding()
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

void FormTab4::OnBnClickedCheckTapTopRefind()
{
	UpdateData();
}

void FormTab4::OnBnClickedCheckTabBottomAbsMode()
{
	UpdateData();
}

void FormTab4::HideParam(BOOL bFlag)
{
	if(m_bLineWelding == FALSE && m_b2LineWelding == FALSE)
		bFlag = TRUE;

	GetDlgItem(IDC_STATIC_WELDING_2)->ShowWindow(bFlag);
	GetDlgItem(IDC_EDIIT_HOLEBLACK2)->ShowWindow(bFlag);
	GetDlgItem(IDC_EDIIT_HOLEBLACK4)->ShowWindow(bFlag);
	GetDlgItem(IDC_EDIIT_HOLEBLACK6)->ShowWindow(bFlag);
}

void FormTab4::OnBnClickedCheckUseTabtopTh()
{
	UpdateData();
}

void FormTab4::OnBnClickedCheckWeldingPosXFix()
{
	UpdateData();
}

void FormTab4::OnBnClickedCheckWeldingRange()
{
	UpdateData();
}

void FormTab4::OnBnClickedButtonregpos()
{
	CFlyCapture2_EXDlg *pDlg = (CFlyCapture2_EXDlg*)m_pParent;

	//find circle
	pDlg->m_Project.CircleFind2(pDlg->m_Project.m_MultiBuf[pDlg->m_nSelectFrame]);//Tracker search
	CRect rttmp;
	rttmp = pDlg->m_Display[0].m_rtImgTracker[5];//weld	
	m_nRegPos[0]=pDlg->m_Project.m_nRegWeldPos[0] = pDlg->m_Project.m_ptCapCenter.x-rttmp.left;
	m_nRegPos[1]=pDlg->m_Project.m_nRegWeldPos[1] = rttmp.top -pDlg->m_Project.m_ptCapCenter.y;
	m_nRegYpos = pDlg->m_Display[0].m_rtImgTracker[5].top -pDlg->m_Project.m_ptCapCenter.y;

	rttmp = pDlg->m_Display[0].m_rtImgTracker[6];//weld	
	m_nRegPos1[0]=pDlg->m_Project.m_nRegWeldPos1[0] = pDlg->m_Project.m_ptCapCenter.x-rttmp.left;
	m_nRegPos1[1]=pDlg->m_Project.m_nRegWeldPos1[1] = rttmp.top -pDlg->m_Project.m_ptCapCenter.y;
	m_nRegYpos1 = pDlg->m_Display[0].m_rtImgTracker[6].top -pDlg->m_Project.m_ptCapCenter.y;

	UpdateData(FALSE);

	// 		pView->m_Display[0].m_rtImgTracker[5].top = m_ptCapCenter.y+pView->m_pDlgSet->m_FormTab4->m_nRegYpos;
	// 		pView->m_Display[0].m_rtImgTracker[5].bottom = m_ptCapCenter.y+pView->m_pDlgSet->m_FormTab4->m_nRegYpos + nH1;

}

void FormTab4::OnBnClickedButton10()
{//left
	CFlyCapture2_EXDlg *pDlg = (CFlyCapture2_EXDlg*)m_pParent;
	pDlg->m_pDlgSet->m_FormTab3->OnBnClickedButtonLeft();

}

void FormTab4::OnBnClickedButton11()
{//right
	CFlyCapture2_EXDlg *pDlg = (CFlyCapture2_EXDlg*)m_pParent;
	pDlg->m_pDlgSet->m_FormTab3->OnBnClickedButtonRight();

}

void FormTab4::OnBnClickedButtonregpos2()
{
	CFlyCapture2_EXDlg *pDlg = (CFlyCapture2_EXDlg*)m_pParent;

	UpdateData();
	CRect rttmp;
	rttmp = pDlg->m_Display[0].m_rtImgTracker[5];//weld
	rttmp.top = pDlg->m_Project.m_ptCapCenter.y+m_nRegYpos;
	rttmp.bottom = rttmp.top + pDlg->m_Display[0].m_rtImgTracker[5].Height();
	pDlg->m_Display[0].m_rtImgTracker[5] = rttmp;//weld
	m_nRegPos[0]=pDlg->m_Project.m_nRegWeldPos[0] = pDlg->m_Project.m_ptCapCenter.x-rttmp.left;
	m_nRegPos[1]=pDlg->m_Project.m_nRegWeldPos[1] = rttmp.top -pDlg->m_Project.m_ptCapCenter.y;

	rttmp = pDlg->m_Display[0].m_rtImgTracker[6];//weld
	rttmp.top = pDlg->m_Project.m_ptCapCenter.y+m_nRegYpos1;
	rttmp.bottom = rttmp.top + pDlg->m_Display[0].m_rtImgTracker[6].Height();
	pDlg->m_Display[0].m_rtImgTracker[6] = rttmp;//weld
	m_nRegPos1[0]=pDlg->m_Project.m_nRegWeldPos1[0] = pDlg->m_Project.m_ptCapCenter.x-rttmp.left;
	m_nRegPos1[1]=pDlg->m_Project.m_nRegWeldPos1[1] = rttmp.top -pDlg->m_Project.m_ptCapCenter.y;

	pDlg->DrawDisplay(0);

}

void FormTab4::OnBnClickedCheck10()
{//m_bTabCornerDist
	UpdateData();
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void FormTab4::OnBnClickedCheckWelddist()
{//m_bWeldDist
	UpdateData();
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void FormTab4::OnBnClickedCheckWelddist2()
{//m_bWeldDistCap
	UpdateData();
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void FormTab4::OnBnClickedCheckWeakweld()
{//m_bWeakWeld
	UpdateData();
}

void FormTab4::OnBnClickedCheckFrame()
{//m_bFrame
	UpdateData();
}

void FormTab4::OnBnClickedChecktwotab()
{//m_bTwoTab
	UpdateData();
}

void FormTab4::OnBnClickedCheckMidweld()
{//m_bMidWeld
	CFlyCapture2_EXDlg *pDlg = (CFlyCapture2_EXDlg*)m_pParent;
	UpdateData();
	BOOL bshow = 0;
	if(m_bMidWeld){
		pDlg->m_Display[0].m_nUseTracker = 8;
		bshow = 1;
	}
	else{
		pDlg->m_Display[0].m_nUseTracker = 8;
	}
	GetDlgItem(IDC_EDIT_2TABMIDWELD)->EnableWindow(bshow);
	GetDlgItem(IDC_EDIT_WELDING_DISTANCE3)->EnableWindow(bshow);
	GetDlgItem(IDC_STATIC_MIDPOS12)->EnableWindow(bshow);
}

void FormTab4::OnBnClickedCheckbottomtab()
{//m_bBottomTab
	UpdateData();
}

void FormTab4::OnBnClickedCheckWelddist3()
{//m_bTabTopDistCap	
	UpdateData();
}

void FormTab4::OnBnClickedCheckParamCheckDamage()
{//m_bCheckDamage
	UpdateData();
}

void FormTab4::OnBnClickedCheckWelddist4()
{//m_bCanDist
	UpdateData();
}

void FormTab4::OnBnClickedCheckPlcOnoff()
{//m_bPlcOnOff
	UpdateData();
}

void FormTab4::OnBnClickedCheckWeakWeldPass()
{//m_bWeakWeldPass
	UpdateData();
		CFlyCapture2_EXDlg *pDlg = (CFlyCapture2_EXDlg*)m_pParent;
	if(m_bUseAiIns)
	{
		pDlg->m_pDlgAiView->ShowWindow(SW_SHOW);
	}
	else
	{
		pDlg->m_pDlgAiView->ShowWindow(SW_HIDE);
		m_bWeakWeldOutputUse =  m_bUseAiIns;
		UpdateData(FALSE);
	}
}

void FormTab4::OnBnClickedButton3()
{

}

void FormTab4::OnBnClickedCheckPasstab()
{
	
}
