#pragma once
#include "afxcmn.h"


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
	float m_dEditPara1;
	float m_dEditPara2;
	float m_dEditPara3;
	CListCtrl m_pList;	

	float m_dPara1;//�Ÿ�
	float m_dPara2;//����
	float m_dPara3;//���Ⱚ

	void GetProductData();
	
	afx_msg void OnBnClickedButtonSelectTab1();
	afx_msg void OnNMDblclkListProductTab1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonDeleteTab1();
};
