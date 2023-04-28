#pragma once
#include "afxcmn.h"
#define NUM_COLUMNS	2


// CFormTab1 대화 상자입니다.

class CFormTab1 : public CDialog
{
	DECLARE_DYNAMIC(CFormTab1)

public:
	CFormTab1(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CFormTab1();

	virtual void OnCancel(){};
	virtual void OnOK(){};
	

// 대화 상자 데이터입니다.
	enum { IDD = IDD_Form_Tab_1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonProdcutTab1();
	DECLARE_MESSAGE_MAP()
public:
	CWnd *m_hParent;
	CString m_strSelectProduct;
	CString m_strProductName;
	CListCtrl m_pList;	
	BOOL	m_bShowHide;
	CString m_strModel;
	BOOL  m_bProductCheck;//제품 선택여부
	CString	m_strCodeInfo[NUM_COLUMNS];
	int m_nRecipe;
	void ProductData(CString strModel,BOOL bLoad);
	void UpdateSetupIni(BOOL bLoad);

	int DirFileCount(char *pDirName);
	afx_msg void OnBnClickedButtonSelectTab1();
	afx_msg void OnNMDblclkListProductTab1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonDeleteTab1();
	afx_msg void OnBnClickedButtonRefresh();
	CString m_strPgmVersion;
	CString m_strServerIP;
	BOOL m_bUseAging;
	int  m_nAgingDelay;
	BOOL m_bUseDeepLearning;
};
