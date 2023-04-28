#pragma once


// CFormTab5 ��ȭ �����Դϴ�.
#define PLC_MAX	1

class CFormTab5 : public CDialog
{
	DECLARE_DYNAMIC(CFormTab5)

public:
	CFormTab5(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CFormTab5();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_Form_Tab_5 };
	CWnd *m_hParent;
	double	m_dSkipEdge1;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()

public:
	//melsec - FORMTAB3�� E71TCP ��Ʈ���� �߰��ؾ���.
	BOOL m_bCalibration;

	double m_nDLScale;
	//0���� 1����
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
