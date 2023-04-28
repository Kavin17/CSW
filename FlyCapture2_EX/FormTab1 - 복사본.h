#pragma once
#include "afxcmn.h"


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
	float m_dEditPara1;
	float m_dEditPara2;
	float m_dEditPara3;
	CListCtrl m_pList;	

	float m_dPara1;//거리
	float m_dPara2;//공차
	float m_dPara3;//검출값

	void GetProductData();
	
	afx_msg void OnBnClickedButtonSelectTab1();
	afx_msg void OnNMDblclkListProductTab1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonDeleteTab1();
};
