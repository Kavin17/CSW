#pragma once


// CFormTab5 대화 상자입니다.
#define PLC_MAX	1

class CFormTab5 : public CDialog
{
	DECLARE_DYNAMIC(CFormTab5)

public:
	CFormTab5(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CFormTab5();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_Form_Tab_5 };
	CWnd *m_hParent;
	double	m_dSkipEdge1;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()

public:
	//melsec - FORMTAB3에 E71TCP 컨트롤을 추가해야함.
	BOOL m_bCalibration;

	double m_nDLScale;
	//0하한 1상한
	double	m_dDLSpecTcCanX[2];
	double	m_dDLSpecTcCanY[2];
	double	m_dDLSpecTcWeldY[2];
	double	m_dDLSpecTcTabY[2];
	double	m_dDLSpecTcTabLeft[2];
	double	m_dDLSpecTcTabRight[2];
	double	m_dDLSpecWeldCanY[2];
	double	m_dDLSpecTabEdgeGapY[2];
	double	m_dDLSpecStrongWeldArea[2];
	double	m_dDLSpecStrongWeldLength[2];
	double	m_dDLSpecSootWeldArea[2];
	double	m_dDLSpecSootWeldLength[2];
	double	m_dDLSpecWeldArea[2];
	double	m_dDLSpecWeldLength[2];
	int		m_nDLDelayCount;
	BOOL	m_bDLSaveAll;
	BOOL	m_bDLSaveNg;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	void UpdateIni(BOOL bLoad = TRUE);

	afx_msg void OnBnClickedCheckDlSaveAll();
	afx_msg void OnBnClickedCheckDlSaveNg();
};
