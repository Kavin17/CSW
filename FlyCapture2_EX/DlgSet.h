

#pragma once
#include "afxwin.h"
#include "ini.h"
#include "afxcmn.h"
#include <Afxmt.h>
#include "afxext.h"

#include "FormTab1.h"
#include "FormTab2.h"
#include "FormTab3.h"
#include "FormTab4.h"
#include "FormTab5.h"


// CDlgSet ��ȭ �����Դϴ�.

class CDlgSet : public CDialog
{
	DECLARE_DYNAMIC(CDlgSet)

public:
	CDlgSet(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	CWnd*	m_pParent;	
	void ShowPass();
	int  FolderInsp(int nCam);
	void setGain( double dgain);

	CEvent		m_ev_rbutton_down;
	int	m_nTestMode;


virtual ~CDlgSet();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_SET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage( MSG* pMsg );
	afx_msg LRESULT OnProductSelect(WPARAM wParam,LPARAM lParam);
	afx_msg void OnBnClickedButtonAdjust();
	afx_msg void OnBnClickedButtonCacel();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()
	

public:
	//void UpdateIni(CString strModel,BOOL bLoad);
	void	Rotate(BYTE* des,BYTE* src,double angle);
	int		m_nGrabMode;
	void InitDisplay();
	void LoadImage(int nCam,BOOL bSMode);
	void Test();
	CFormTab1 *m_FormTab1;
	CFormTab2 *m_FormTab2;
	CFormTab3 *m_FormTab3;
	FormTab4 *m_FormTab4;
	CFormTab5 *m_FormTab5;
	CWnd*	  m_pwndShow;
	CString m_strCodeInfo[NUM_COLUMNS];
	void	WriteLog(CString str, int n);
	CString	m_strComname;
	BOOL m_bLR_Reverse;
	BOOL m_bTB_Reverse;
	int m_nAngle;
	CString m_strSelectProduct;
	float m_dScaleFactor;
	void  showBarcode();
	CTabCtrl m_Tab;
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);	
	afx_msg void OnDestroy();

	void GetiniData(int nsave);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
