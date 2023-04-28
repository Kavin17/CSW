
#include "Label1.h"


#pragma once
#include "afxwin.h"


// CLightMap 대화 상자입니다.

class CLightMap : public CDialog
{
	DECLARE_DYNAMIC(CLightMap)

public:
	CLightMap(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CLightMap();

	CWnd *m_hParent;

	int		m_nPackMode;//0정상포장 1재포장
	CRect	m_rtMap;
	int		m_nMapHeight;
	CPoint CLightMap::CheckPt(CPoint ptCheck);
	CPoint CLightMap::ScaleTrans(int nCam,CPoint point,int direction);
	void CLightMap::DrawDotLine(COLORREF lColor, int nLineWidth);
	int		m_nLightPoint[20];
	int		m_nLightDefectCount;

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_LIGHTMAP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonHide();
	afx_msg void OnPaint();
	afx_msg void OnBnClickedButtonNormal();
	afx_msg void OnBnClickedButtonRepack();
};
