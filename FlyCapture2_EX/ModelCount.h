#pragma once
#include "afxwin.h"


// CModelCount 대화 상자입니다.

class CModelCount : public CDialog
{
	DECLARE_DYNAMIC(CModelCount)

public:
	CModelCount(CWnd* pParent = NULL);   // 표준 생성자입니다.

	CWnd*	m_pParent;	
	int		m_nMesResult;

	void ShowLabel(CString str);
	int		m_nModelSelect;
	void	CheckModel(int nModel);
	CString	m_strSelectModel;
	int		m_nOkNGCount[2];

	int		m_nModelShow;
	virtual ~CModelCount();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_MODELCOUNT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonModel1();
	afx_msg void OnBnClickedButtonModel2();
	afx_msg void OnBnClickedButtonModel3();
	afx_msg void OnBnClickedButtonModel4();
	afx_msg void OnBnClickedButtonModel5();
	afx_msg void OnBnClickedButtonModel6();
	afx_msg void OnBnClickedButtonModel7();
	afx_msg void OnBnClickedButtonModel8();
	CLabel m_LabelModelName;
	virtual BOOL OnInitDialog();
	int m_nPackCount;
	afx_msg void OnBnClickedButtonNext();
	afx_msg void OnBnClickedButtonModel9();
	afx_msg void OnBnClickedButtonModel10();
	afx_msg void OnBnClickedButtonModel11();
	afx_msg void OnBnClickedButtonModel12();
	afx_msg void OnBnClickedButtonOk();
	afx_msg void OnBnClickedButtonNg();
	CLabel m_ctClickCheck;
};
