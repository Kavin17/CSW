
#include "Label1.h"


#pragma once
#include "afxwin.h"


// CLightMap ��ȭ �����Դϴ�.

class CLightMap : public CDialog
{
	DECLARE_DYNAMIC(CLightMap)

public:
	CLightMap(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CLightMap();

	CWnd *m_hParent;

	int		m_nPackMode;//0�������� 1������
	CRect	m_rtMap;
	int		m_nMapHeight;
	CPoint CLightMap::CheckPt(CPoint ptCheck);
	CPoint CLightMap::ScaleTrans(int nCam,CPoint point,int direction);
	void CLightMap::DrawDotLine(COLORREF lColor, int nLineWidth);
	int		m_nLightPoint[20];
	int		m_nLightDefectCount;

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_LIGHTMAP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonHide();
	afx_msg void OnPaint();
	afx_msg void OnBnClickedButtonNormal();
	afx_msg void OnBnClickedButtonRepack();
};
