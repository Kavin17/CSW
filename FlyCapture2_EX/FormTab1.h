#pragma once
#include "afxcmn.h"
#define NUM_COLUMNS	2


// CFormTab1 ��ȭ �����Դϴ�.

class CFormTab1 : public CDialog
{
	DECLARE_DYNAMIC(CFormTab1)

public:
	CFormTab1(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CFormTab1();

	virtual void OnCancel(){};
	virtual void OnOK(){};
	

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_Form_Tab_1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.
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
	BOOL  m_bProductCheck;//��ǰ ���ÿ���
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
