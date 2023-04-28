#pragma once
#include "afxwin.h"
//#include "FlyCapture2_EX.h"
//#include "FlyCapture2_EXDlg.h"
#include "afxcmn.h"


// CFormTab2 대화 상자입니다.

class CFormTab2 : public CDialog
{
	DECLARE_DYNAMIC(CFormTab2)

public:
	CFormTab2(CWnd* pParent = NULL);   // 표준 생성자입니다.

	virtual ~CFormTab2();
	virtual void OnCancel(){};
	virtual void OnOK(){};
	CWnd *m_pParent;

// 대화 상자 데이터입니다.
	enum { IDD = IDD_Form_Tab_2 };


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	void UpdateIni(BOOL bLoad);

//form4 복사
	
	
	int m_nCamShutter;
	int m_nGrabDelay;
	BOOL m_bGrabContinue;
	afx_msg void OnBnClickedCheckContinuegrab();
	afx_msg void OnBnClickedCheck2lineWelding();
	BOOL m_b2LineWelding;
	BOOL m_bLineWelding;
	void HideParam(BOOL bFlag);
	BOOL m_bTabTopRefind;
	double m_dRightArea;
	double m_dRightArea1;
	int m_nSkipRight;
	int m_nInnerCiircle;
	double m_dCamScale;
	double m_dTabTol;
	double m_dTabTol1;
	int		m_nTabTol;
	double m_dTapTolY1;
	double m_dTapTolY2;
	int m_nCylinderCloseTime;
	int m_nCylinderOpenTime;
	BOOL m_bShowInspTime;
	BOOL m_bShowEdge;
	BOOL m_bShowTabArea;
	BOOL m_bShowHole;
	BOOL m_bCircleShow;
	BOOL m_bFullSave;
	BOOL m_bOverlaySave;
	BOOL m_bNgSave;
	BOOL m_bSaveJpeg;
	BOOL m_bTapImageShow;


	afx_msg void OnBnClickedCheck35();
	afx_msg void OnBnClickedCheckTapTopRefind();
	afx_msg void OnBnClickedButtonupdate();
	afx_msg void OnBnClickedButtonGrab();
	afx_msg void OnBnClickedButtonAdjust();
	afx_msg void OnBnClickedCheckShowinsptime();
	afx_msg void OnBnClickedCheckShowedge();
	afx_msg void OnBnClickedCheckshowtabarea();
	afx_msg void OnBnClickedCheck32();
	afx_msg void OnBnClickedCheck37();
	afx_msg void OnBnClickedCheckFullsave();
	afx_msg void OnBnClickedCheckOverlaysave();
	afx_msg void OnBnClickedCheckNgSave();
	afx_msg void OnBnClickedCheckJpegsave();
	afx_msg void OnBnClickedCheckTapimgshow();
	int m_nMultiShutter[20];
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton10();
	BOOL m_bUseLeft;
	afx_msg void OnBnClickedCheckLeft();
	BOOL m_bUseRight;
	afx_msg void OnBnClickedCheckRight();
	BOOL m_bWeldCenterPos;
	BOOL m_bWeldCenterPos2;
	afx_msg void OnBnClickedCheckWeldcenter();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedCheckRed();
	afx_msg void OnBnClickedCheckGreen();
	afx_msg void OnBnClickedCheckBlue();
	afx_msg void OnBnClickedCheckYellow();
	afx_msg void OnBnClickedCheckWhite();
	BOOL m_bRed;
	BOOL m_bGreen;
	BOOL m_bBlue;
	BOOL m_bYellow;
	BOOL m_bWhite;
	int m_nWeldLeftOffset;
	afx_msg void OnBnClickedButton3();
	BOOL m_bCsvSave;
	BOOL m_bUseRtos;
	BOOL m_bUseServer;
	BOOL m_bUseAiReadyAlram;
	BOOL m_bUseAiImageSave;
	afx_msg void OnBnClickedCheckSaveNas();
	int m_nIoDelay;
	BOOL m_bIODuration;
	afx_msg void OnBnClickedCheckIodureation();
	int m_nTabTopOffset;
	afx_msg void OnBnClickedCheckSave3();
	BOOL m_bSaveFrame[11];
	afx_msg void OnBnClickedCheckSave1();
	afx_msg void OnBnClickedCheckSave2();
	afx_msg void OnBnClickedCheckSave4();
	afx_msg void OnBnClickedCheckSave5();
	afx_msg void OnBnClickedCheckSave6();
	afx_msg void OnBnClickedCheckSave7();
	afx_msg void OnBnClickedCheckSave8();
	afx_msg void OnBnClickedCheckSave9();
	afx_msg void OnBnClickedCheckSave10();
	afx_msg void OnBnClickedCheckSave11();
	BOOL m_bMergeImageSave;
	afx_msg void OnBnClickedCheckSaveMerge();
	int m_nTopCapCenterFrame;
	int m_nCanPosFrame;
	int m_nCapCenterBright;
	int m_nTabTopSelectFrame[2];
	BOOL m_bDrawGraph;
	afx_msg void OnBnClickedCheckDrawgraph();
	int m_nWeldInTabpos;
	double m_dWeldInTabWhiteTh;
	BOOL m_bWeldInTab;
	afx_msg void OnBnClickedCheckWeldintab();
	BOOL m_bTabSummation;
	BOOL m_bUseAiReadFail;
	BOOL m_bUseSaveCrop;
	afx_msg void OnBnClickedCheckTabsum();
	BOOL m_bTabReFind;
	BOOL m_bUseCellApi;
	afx_msg void OnBnClickedCheckTabrefind();
	int m_nWeldCheckFrame[4];
	double m_dWeldPercent;
	BOOL m_bPLCLotchange;
	afx_msg void OnBnClickedCheckLotchange();
	BOOL m_bMorphShow;
	afx_msg void OnBnClickedCheckShowweldimage();
	int m_nWeldHeight;
	int m_nPCNo;
	int m_nWeakWeldFrame;
	afx_msg void OnBnClickedCheckWeldcenter2();
	BOOL m_bCompleteOnOff;
	BOOL m_bImgReverse;
	afx_msg void OnBnClickedCheckCompleteonoff();
	CString m_strIPCheck;
	CString m_strIPCheck2;
	int m_nTabEdgeVFrame;
	int m_nLineNumber;
	int m_nLaneNumber;
	CComboBox m_comboLineNumber;
	CComboBox m_comboLaneNumber;
	BOOL m_bUseTabTopFrame;
	int m_nStartFrameL;
	int m_nEndFrameL;
	int m_nStartFrameR;
	int m_nEndFrameR;

//	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnBnClickedButton24();
	afx_msg void OnBnClickedCheckUseCellApiTest();
};