#pragma once

#include "CgLight.h"
#include "afxwin.h"
#include "afxcmn.h"

// CFormTab3 대화 상자입니다.

class CFormTab3 : public CDialog
{
	DECLARE_DYNAMIC(CFormTab3)

public:
	CFormTab3(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CFormTab3();
	virtual void OnCancel(){};
	virtual void OnOK(){};

	CWnd *m_hParent;

// 대화 상자 데이터입니다.
	enum { IDD = IDD_Form_Tab_3 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	HANDLE m_LiquidLensComHandle;


	DECLARE_MESSAGE_MAP()
public:
	char g_ConnectString[30];
	char g_ChannelPrefix[30];
	CString m_strIsCode;
	CString	m_strComname;

	CEvent	m_ev_rbutton_down;


	int m_nModeChange;

	BOOL	m_bMesConnect;
	int		m_nLeftStart;//용접왼쪽 시작

	CPoint CFormTab3::FindWeldInTab(BYTE* fm,CRect rt1,CRect rtweld);// tab 내에 용접 중심 포인트
	int CFormTab3::TabTopRefind(BYTE* fm,CPoint ptL,CPoint ptR,int nXoffset,int nYoffset,int * nx1, int * nx2);

///////////////////////////////////
	int CFormTab3::WeldCheckNew(CRect rtLimit,int nstart,int nend,int nwidth,int nheight,CRect rtL,CRect rtR,int wth,double dcutL,double dcutR,int bth,double dbcutL,double dbcutR);
	void CFormTab3::MultiFrameFullNGSave(CString filename1);
	int CFormTab3::WeldCheck(int nstart,int nend,int nwidth,int nheight,CRect rtL,CRect rtR,int nth,double dcutL,double dcutR);
	void CFormTab3::MultiFrameSingleSave(int nfame);
	void CFormTab3::WeldPoint(BYTE * fm,int nwidth,int nheight,CRect rt,int nmiltiful);
	void CFormTab3::fnImageSummary1(BOOL bShow, BOOL bSave,CRect rt);
	void CFormTab3::WeldPosRect(int nStart,int nEnd,double dAmplify,CRect rtweld,BOOL bShow);//용접위치만 잘라내기
	void CFormTab3::FindLeft(BYTE * fm , CRect rt);
	void CFormTab3::FindRight(BYTE * fm , CRect rt);
	void CFormTab3::FindTopTab(BYTE * fm , CRect rt,int *x1, int *y1,int *x2,int *y2);
	void CFormTab3::fnYSum1(int nstart,int nend,int nFrame,CRect rt,BOOL bShow,BOOL bSave,double dAmplify,int nInsp);//Top Edge Detact insert
	void CFormTab3::fnXSum1(int nstart,int nend,int nFrame,CRect rt,BOOL bShow,BOOL bSave,double dAmplify);
	void CFormTab3::fnXSum2(int nstart,int nend,int nFrame,CRect rt,BOOL bShow,BOOL bSave,double dAmplify);
	void CFormTab3::fnImageSummary(BOOL bShow, BOOL bSave);

	void CFormTab3::fnSetLight(int nPort,int nCh, BOOL nOnOff);
	void CFormTab3::fnXSum(int nstart,int nend,int nFrame);
	void CFormTab3::fnYSum(int nstart,int nend,int nFrame);
	int CFormTab3::fnTabFind(BYTE * fmtab,CRect rttab,BOOL bShow);
	void CFormTab3::AlphaFrameFullSave(int nstart,int nEnd);

//////////////////////////////////////////////////
	//CLIENT IP, NAME
	char g_NewID[50];
	char g_IP[20];
	char g_ClientName[50];
	char g_HostName[20];
	char g_Factory[10];
	char g_UserName[20];
	char g_Language;
	char g_Owner[10];
	char g_result;
	long g_Timeout;
	int		CheckMes(int check);
	CString	m_strMesClinetName;
	CString	m_strMesUserNAme;

	void UpdateIni(BOOL bLoad);
	afx_msg void OnBnClickedButtondummy();
	BOOL m_bALLOK;
	BOOL m_bALLNG;
	afx_msg void OnBnClickedCheckAllOk();
	afx_msg void OnBnClickedCheckAllNg();
	int m_nRemainHddSpace;
	BOOL m_bMesUSe;
	afx_msg void OnBnClickedCheckMes();
	virtual BOOL OnInitDialog();
	int m_nWinCpu;
	int m_nPGMCpu;
	int m_nRoi[6][4];
	BOOL m_bAllNgHole;
	BOOL m_bManualRoi;
	afx_msg void OnBnClickedCheckAllNg2();
	BOOL m_bAllNGDual;
	afx_msg void OnBnClickedCheckAllNg3();
	CgLight m_LightCtrl;
	afx_msg void OnBnClickedButtonLightTurnOnCh1();
	afx_msg void OnBnClickedButtonLightTurnOffCh1();
	afx_msg void OnBnClickedButtonLightTurnOnCh2();
	afx_msg void OnBnClickedButtonLightTurnOffCh2();
	int m_nLightValueCh1;
	int m_nLightValueCh2;
	int m_nTabResult;
	int m_ntabresult;
	double m_dCamGain;
	afx_msg void OnBnClickedButtonSetGain();
	CComboBox m_comboWeldingNum;
	CComboBox m_comboWeldingColor;
	int m_nWeldingNum;
	int m_nWeldingColor;
	BOOL m_bWeldingLabel;
	int m_nAngleLightValCh1;
	int m_nAngleLightValCh2;
	afx_msg void OnBnClickedCheckAngleLightCh1No1();
	afx_msg void OnBnClickedCheckAngleLightCh1No2();
	afx_msg void OnBnClickedCheckAngleLightCh1No3();
	afx_msg void OnBnClickedCheckAngleLightCh1No4();
	afx_msg void OnBnClickedCheckAngleLightCh1No5();
	afx_msg void OnBnClickedCheckAngleLightCh1No6();
	afx_msg void OnBnClickedCheckAngleLightCh1No7();
	afx_msg void OnBnClickedCheckAngleLightCh1No8();
	afx_msg void OnBnClickedCheckAngleLightCh1No9();
	afx_msg void OnBnClickedCheckAngleLightCh1No10();
	afx_msg void OnBnClickedCheckAngleLightCh2No1();
	afx_msg void OnBnClickedCheckAngleLightCh2No2();
	afx_msg void OnBnClickedCheckAngleLightCh2No3();
	afx_msg void OnBnClickedCheckAngleLightCh2No4();
	afx_msg void OnBnClickedCheckAngleLightCh2No5();
	afx_msg void OnBnClickedCheckAngleLightCh2No6();
	afx_msg void OnBnClickedCheckAngleLightCh2No7();
	afx_msg void OnBnClickedCheckAngleLightCh2No8();
	double m_dLiquidLensFocusVal;
	CSpinButtonCtrl m_ctSpinFocusBigDigit;
	CSpinButtonCtrl m_ctSpinFocusSmallDigit;
	afx_msg void OnBnClickedButtonApplyLiquidlensFocus();
	BOOL LiquidLensOpen(CString strPort);
	void	LiquidLensClose();
	BOOL m_bLiquidLensConnect;
	afx_msg void OnDeltaposSpinFocusBigDigit(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinFocusSmallDigit(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedEditSetCamGain2();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton10();
	afx_msg void OnBnClickedButton11();
	afx_msg void OnBnClickedButton12();
	afx_msg void OnBnClickedButton13();
	afx_msg void OnBnClickedButton14();
	afx_msg void OnBnClickedButton15();
	afx_msg void OnBnClickedButton16();
	afx_msg void OnBnClickedButton17();
	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedButtonLoad();
	int OnBnClickedButtonTest();
	afx_msg void OnBnClickedButtontopbottom();
	afx_msg void OnBnClickedEditSetFulsave();
	afx_msg void OnBnClickedButtonLoad3();
	afx_msg void OnBnClickedButtonLeft();
	afx_msg void OnBnClickedButtonRight();
	afx_msg void OnBnClickedButton18();
	afx_msg void OnBnClickedCheckAngleLightCh1No11();
	afx_msg void OnBnClickedCheckAngleLightCh1No12();
	afx_msg void OnBnClickedCheckAngleLightCh2No9();
	afx_msg void OnBnClickedCheckAngleLightCh2No10();
	afx_msg void OnBnClickedCheckAngleLightCh2No11();
	afx_msg void OnBnClickedCheckAngleLightCh2No12();
	void	MultiFrameFullSave();
	BOOL m_bCheckType;
	afx_msg void OnBnClickedCheckType();
	afx_msg void OnBnClickedButtonLoad4();
	BOOL m_bFullAlphaImage;
	afx_msg void OnBnClickedCheck5();
	BOOL m_bFolderNGSave;
	afx_msg void OnBnClickedCheckNgsave();
	afx_msg void OnBnClickedButton19();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	BOOL m_bCkAuto;
	afx_msg void OnBnClickedCheckAuto();
	afx_msg void OnBnClickedButton20();
	afx_msg void OnBnClickedButton21();
	afx_msg void OnCbnSelchangeComboWeldingNum();
	afx_msg void OnBnClickedButtonLoad5();
	afx_msg void OnBnClickedButton23();
//	afx_msg void OnBnClickedButton25();
//	afx_msg void OnBnClickedButton26();
	afx_msg void OnBnClickedButton25();
};
