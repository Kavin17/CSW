#pragma once
#include "Label1.h"
#include "Img.h"
#include "CgProject.h"
#include "afxwin.h"
// FormTab4 대화 상자입니다.

class FormTab4 : public CDialog
{
	DECLARE_DYNAMIC(FormTab4)

public:
	FormTab4(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~FormTab4();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_Form_Tab_4 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	int		m_nSelectImgaeNo;
	void FormTab4::ShowHide(BOOL Show);
	CString m_strfile;
	CWnd *m_pParent;
	int m_nRadioNum;
	CRect	m_trTabRange;
	int		m_nTabTol;//
	int		m_nRegYpos;
	int		m_nRegPos[2];
	int		m_nRegYpos1;
	int		m_nRegPos1[2];

	void UpdateIni(BOOL bLoad = TRUE);
	CRect	m_rtTacker[MAX_TRACKER];
	CString  m_strLastFile;
	CString  m_strBeforeImg;
	CEvent	m_ev_rbutton_down;

	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMenuDelete();

	int m_nCamShutter;
	afx_msg void OnBnClickedButtonAdjust();
	BOOL m_bGrabContinue;
	afx_msg void OnBnClickedCheckContinuegrab();
	afx_msg void OnBnClickedButtonGrab();
	int m_nGrabDelay;
	BOOL m_bImageReverseX;
	afx_msg void OnBnClickedCheckReversex();
	BOOL m_bImageReverseY;
	afx_msg void OnBnClickedCheckReversey();
	BOOL m_bShowInspTime;
	afx_msg void OnBnClickedCheckShowinsptime();
	BOOL m_bFullSave;
	afx_msg void OnBnClickedCheckFullsave();
	BOOL m_bNgSave;
	afx_msg void OnBnClickedCheckNgSave();
	BOOL m_bOverlaySave;
	afx_msg void OnBnClickedCheckOverlaysave();
	BOOL m_bSaveJpeg;
	afx_msg void OnBnClickedCheckJpegsave();
	double m_dCamScale;

	afx_msg void OnTimer(UINT_PTR nIDEvent);
	int m_nCheckCanValue;
	BOOL m_bMagneticUse;
	afx_msg void OnBnClickedCheck5();
	afx_msg void OnBnClickedButtonLoad();
	afx_msg void OnBnClickedButtonTest();
	BOOL m_bShowEdge;
	afx_msg void OnBnClickedCheckShowedge();
	double m_dTabTol;
	afx_msg void OnBnClickedButtonupdate();
	BOOL m_bShowTabArea;
	afx_msg void OnBnClickedCheckshowtabarea();
	BOOL m_bTapImageShow;
	afx_msg void OnBnClickedCheckTapimgshow();
	double m_dTapTolY1;
	double m_dTapSize;
	int m_nTopCapEdgeValue;
	int m_nTapEdge;
	double m_dTopCapSize;
	int m_nHoleTh;
	int m_nMorphCount;
	afx_msg void OnBnClickedButtonFoldertest();
	BOOL m_bCkAuto;
	afx_msg void OnBnClickedCheck8();
	afx_msg void OnBnClickedButtonSaveimage();
	int m_nWeldingSize;
	double m_dLeftArea;
	double m_dRightArea;
	double m_dLeftArea1;
	double m_dRightArea1;
	int m_nV1;
	int m_nV2;
	int m_nV3;
	int m_nTopCapValue;
	int m_nCapReverseV;
	BOOL m_bTapInspect;
	BOOL m_bHoleInspect;
	afx_msg void OnBnClickedCheckHoleinspect();
	int m_nTopCapEdgeValue1;
	int m_nTapEdge1;
	BOOL m_bHoleLeftCheck;
	afx_msg void OnBnClickedCheck19();
	int m_nCogDist;
	BOOL m_bImageModify;
	afx_msg void OnBnClickedCheck11();
	BOOL m_bCheckView;
	int m_nBlackArea;
	BOOL m_bBlackArea;
	afx_msg void OnBnClickedCheck13();
	BOOL m_bWhiteCheck;
	afx_msg void OnBnClickedCheck15();
	BOOL m_bHoleSearchArea;
	afx_msg void OnBnClickedCheck20();
	int m_nSkipLeft;
	int m_nSkipRight;
	BOOL m_bRoiSet;
	afx_msg void OnBnClickedCheck16();
	int m_nCylinderCloseTime;
	BOOL m_bTapWhiteSkip;
	afx_msg void OnBnClickedCheck17();
	BOOL m_bLowBlackSkip;
	afx_msg void OnBnClickedCheck21();
	BOOL m_bHoleTopSearch;
	afx_msg void OnBnClickedCheck22();
	int m_nCylinderOpenTime;
	BOOL m_bParamAnd;
	afx_msg void OnBnClickedCheck26();
	BOOL m_bParamOr;
	afx_msg void OnBnClickedCheck27();
	BOOL m_bHoleEdgeSkip;
	afx_msg void OnBnClickedCheck28();
	BOOL m_b2Tab;
	afx_msg void OnBnClickedCheck29();
	BOOL m_bHoleEdgeCheck;
	afx_msg void OnBnClickedCheck31();
	double m_dHoleEdgemm;
	double m_dTapTolY2;
	BOOL m_bShowHole;
	afx_msg void OnBnClickedCheck32();
	double m_dTabTol1;
	BOOL m_bRegMode;
	afx_msg void OnBnClickedCheck34();
	BOOL m_bLineWelding;
	afx_msg void OnBnClickedCheck35();
	double m_dMinHeight;
	double m_dMaxHeight;
	BOOL m_bCircleFit;
	afx_msg void OnBnClickedCheck36();
	int m_nInnerCiircle;
	BOOL m_bCircleShow;
	afx_msg void OnBnClickedCheck37();
	double m_dCenterOffset;
	BOOL m_bUseCapCanY;
	afx_msg void OnBnClickedCheck39();
	afx_msg void OnBnClickedCheck2lineWelding();
	BOOL m_b2LineWelding;
	BOOL m_bTabTopRefind;
	afx_msg void OnBnClickedCheckTapTopRefind();
	afx_msg void OnBnClickedCheckTabBottomAbsMode();
	void HideParam(BOOL bFlag);
	BOOL m_bTabBottomAbsMode;
	int m_nTabTopTh;
	BOOL m_bUseTabTopTh;
	afx_msg void OnBnClickedCheckUseTabtopTh();
	afx_msg void OnBnClickedCheckWeldingPosXFix();
	BOOL m_bWeldingPosXFix;
	BOOL m_bWeldingRange;
	double m_dWeldingDistanceY;
	double m_dWeldingDistanceRange[2];
	afx_msg void OnBnClickedCheckWeldingRange();
	BOOL m_bWeldingRangeNgSignal;
	afx_msg void OnBnClickedButtonregpos();
	int m_nTopCapWhiteCut;
	double m_dWhiteCut;
	double m_dCenterOffset2;
	afx_msg void OnBnClickedButton10();
	afx_msg void OnBnClickedButton11();
	afx_msg void OnBnClickedButtonregpos2();
	int m_nTabCornerDist;
	BOOL m_bTabCornerDist;
	afx_msg void OnBnClickedCheck10();
	double m_dDistWeldMin;
	double m_dDistWeldMax;
	BOOL m_bWeldDist;
	afx_msg void OnBnClickedCheckWelddist();
	BOOL m_bWeldDistCap;
	afx_msg void OnBnClickedCheckWelddist2();
	double m_dWeldDistCapMin;
	double m_dWeldDistCapMax;
	BOOL m_bWeakWeld;
	int m_nWeakWeld;
	afx_msg void OnBnClickedCheckWeakweld();
	afx_msg void OnBnClickedCheckFrame();
	BOOL m_bFrame;
	int m_nCenterEdgeValue;
	int m_nWeakWeld2;
	BOOL m_bTwoTab;
	afx_msg void OnBnClickedChecktwotab();
	int m_nTabEdgeX1;
	int m_nTabEdgeX2;
	int m_nTabEdgeY1;
	int m_nTabEdgeY2;
	double m_dMidWeld;
	BOOL m_bMidWeld;
	afx_msg void OnBnClickedCheckMidweld();
	int m_n2TabOffsetY;
	// 용접 유무 확인
	int m_nWeldCheck;
	afx_msg void OnBnClickedCheckbottomtab();
	BOOL m_bBottomTab;
	afx_msg void OnBnClickedCheckWelddist3();
	double m_dTabTopDistCapMin;
	double m_dTabTopDistCapMax;
	BOOL m_bTabTopDistCap;
	int m_dTopCapCenterCount;
	double m_dCapCandistX;
	int m_nAiFrame;
	int m_nCheckDamage;
	int m_nCheckDamageTh;
	BOOL m_bCheckDamage;
	afx_msg void OnBnClickedCheckParamCheckDamage();
	double m_dHoleEdgeINmm;
	int m_nBottomTabTop;
	int m_nBottomTabBot;
	int m_nBottomTabLeft;
	int m_nBottomTabRight;
	int m_nBottomTabFrame;
	int m_nBottomLeftTh;
	int m_nBottomRightTh;
	BOOL m_bCanDist;
	afx_msg void OnBnClickedCheckWelddist4();
	double m_dCanDistMin;
	double m_dCanDistMax;
	afx_msg void OnBnClickedCheckPlcOnoff();
	BOOL m_bPlcOnOff;
	afx_msg void OnBnClickedCheckWeakWeldPass();
	BOOL m_bUseAiIns;
	BOOL m_bWeakWeldOutputUse;
	afx_msg void OnBnClickedButton3();
//	afx_msg void OnBnClickedCheckWeakWeldPass2();
	afx_msg void OnBnClickedCheckPasstab();
	BOOL m_bPassTab;
	BOOL m_bTabExistChk;
	int m_nTabExist;
	int m_nTabExistTH;
	int m_nTabExistX1;
	int m_nTabExistX2;
	int m_nTabExistY1;
	int m_nTabExistY2;
	double m_dCanToCapL;
	double m_dCanToCapR;
	double m_dCanToCapLMin;
	double m_dCanToCapRMin;
};
