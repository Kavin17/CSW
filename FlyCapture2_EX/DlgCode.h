#pragma once


// CDlgCode 대화 상자입니다.

class CDlgCode : public CDialog
{
	DECLARE_DYNAMIC(CDlgCode)

public:
	CDlgCode(CWnd* pParent = NULL);   // 표준 생성자입니다.
	CWnd*	m_pParent;	

	void	WriteModelData();

	virtual ~CDlgCode();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_BARCODE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonModify();
	afx_msg void OnBnClickedButtonClose();
	void UpdateIni(BOOL bLoad);
	void UpdateIni1(BOOL bLoad);
	void EditControl(BOOL bis);
	void UpdateIni2(BOOL bLoad);
	void UpdatePlc();
	void	UpdateWritePlc();
	void	WritePlc();
	void	EditColtol(BOOL bshow);//기타버튼
	void	EditColtol1(BOOL bshow);//mes
	CString m_strBarCode;
	CString m_strMidCode;
	CString m_strPartNo;
	CString m_strUpLow;
	CString m_strOCVCode;
	CString m_strInch;
	CString m_strLongLen;
	CString m_strShortLen;
	CString	m_strLGSDC;
	int m_nMakeCount;

	afx_msg void OnBnClickedButtonClose2();
	CString m_strPlcLot;
	int m_nPlcMovemm;
	int m_nPlcMoveSpeed;
	int		m_nPlcBlowerCount;//508
	double	m_dUpDelay;//510
	int		m_nVacuumCount;//512
	int		m_nReturnCount;//514

	double m_dPlcBlowerTime;
	double m_dPlcNg2Time;
	double m_dPlcOkAlignTime;
	afx_msg void OnBnClickedButton4();
	double m_dOcvPosX;
	double m_dOcvPosY;
	BOOL m_bShowItem;
	afx_msg void OnBnClickedCheckShow();
	virtual BOOL OnInitDialog();
	CString m_strPLCLongLen;
	CString m_strPLCShortLen;
	CString m_strReadBarcode;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButtonAdjust();
	CString m_strLGSdc;
	int m_nOkDelayCount;
	int m_nLiftCount;
	BOOL m_bAutoHide;
	afx_msg void OnBnClickedCheckAutohide();
	int m_nBlowerCount1;
	double m_dDelayTime1;
	CString m_strSelectModel;
	afx_msg void OnBnClickedButton6();
	int m_nMEsNgCount;
};
