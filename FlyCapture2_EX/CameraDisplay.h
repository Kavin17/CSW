#pragma once
#include "afxwin.h"


// CCameraDisplay 대화 상자입니다.

class CCameraDisplay : public CDialog
{
	DECLARE_DYNAMIC(CCameraDisplay)

public:
	CCameraDisplay(CWnd* pParent = NULL);   // 표준 생성자입니다.
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

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_CAMIMAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

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
