#pragma once
#include "afxwin.h"


// CCameraDisplay ��ȭ �����Դϴ�.

class CCameraDisplay : public CDialog
{
	DECLARE_DYNAMIC(CCameraDisplay)

public:
	CCameraDisplay(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	CWnd*	m_pParent;	

	void CCameraDisplay::DrawOriginImage();
	void CCameraDisplay::ShowLabel();
	void CCameraDisplay::ShowNgPosition();



	CImg* m_OriginDisp;

	int		m_nDefectCount;
	CRect	m_rtSheet;
	CPoint CCameraDisplay::CheckPt(CPoint ptCheck);
	CPoint CCameraDisplay::ScaleTrans(int nCam,CPoint point,int direction);
	void CCameraDisplay::DrawSheet(COLORREF lColor, int nLineWidth);
	int		m_nMapHeight;
	int		m_nSheetCount;

	virtual ~CCameraDisplay();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_CAMIMAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	CLabel m_LabelBadImage;
	CLabel m_ctBadInfo[10];
	CLabel m_ctLabrlStats;
	CLabel m_LabelUsrOKNG;

	CLabel	m_lbTitle[5];
};
