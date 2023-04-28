#pragma once


// CDrawBadState 대화 상자입니다.
#define WHITE		RGB(255,255,255)
#define BLACK		RGB(50,50,50)
#define GRAY		RGB(190,190,190)

#define MAGENTA_N	RGB(255,153,000)
#define BLUE_N		RGB(051,102,204)
#define GREEN_N		RGB(102,204,051)
#define RED_N		RGB(255,102,102)

//#define MAGENTA_N	RGB(255,153,000)
//#define BLUE_N		RGB(051,102,204)
//#define GREEN_N		RGB(051,255,000)
//#define RED_N		RGB(255,102,102)

class CDrawBadState : public CDialog
{
	DECLARE_DYNAMIC(CDrawBadState)

public:
	CDrawBadState(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDrawBadState();

	int		m_nTotal;
	CRect	m_rtArea;
	CRect   m_rtAreaWithLable;
	BOOL	m_bRedrawTime;
	void CDrawBadState::DrawBMP(LPCTSTR lpszFileName, CDC *pDC, int nX, int nY, DWORD dwRop);


	int m_nGraghType;	//0:꺾은선형, 1:수직막대그래프
	int nRow;
	int nCol;
	int m_nMaxUnit;

	int nLableWidth;
	int nLableHeight;

	int nBarWidthCnt;

	CPen* oldPen;
	CBrush* oldBrush;
	CPen mPen;
	CBrush mBrush;

	int m_nGroupCnt;
	int m_nDataCnt;
	float** m_ptrTotalCnt;
	float** m_ptrBadCnt;
	float** m_ptrBadCntBar;

	CString* m_ptrStrGroupName[50];
	BOOL m_bStrNameInit;
	


// 대화 상자 데이터입니다.
	enum { IDD = IDD_DRAW_BAD_STATE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnPaint();

	void CDrawBadState::RedrawMap();
	void CDrawBadState::SetDrawEanble(BOOL isSet);
	BOOL CDrawBadState::GetDrawEnable();

	void CDrawBadState::SetLableWH(int nW, int nH);
	void CDrawBadState::InitAreaFit();

	void CDrawBadState::SetGraphType(int nVal);
	void CDrawBadState::SetRow(int nVal);
	void CDrawBadState::SetCol(int nVal);
	void CDrawBadState::SetDataTest();
	void CDrawBadState::SetDataTest2();
	void CDrawBadState::SetGroupLableTest();

	void CDrawBadState::CreateGraph(int nType, int nRow, int nCol, int nMaxUnit);

	void CDrawBadState::DrawGridLineH(CDC *pDC);
	void CDrawBadState::DrawGridLineV(CDC *pDC);
	void CDrawBadState::DrawDataLine(CDC *pDC);
	void CDrawBadState::DrawDataBar(CDC *pDC);
	void CDrawBadState::CreatePen(CDC* pDC, int lType, int lThick, int lColor);
	void CDrawBadState::ReleasePen(CDC* pDC);

	void CDrawBadState::SetGroupCnt(int nGroupCnt, int nDataCnt);
	void CDrawBadState::AllocCntMem(int nGroupCnt, int nDataCnt);
	void CDrawBadState::DeAllocCntMem();
	void CDrawBadState::ResetCntMem();
	void CDrawBadState::SetMaxUnit(int nMaxUnit);

	int CDrawBadState::GetConvertYPos(int nPos);

	void CDrawBadState::DrawLableG1H(CDC *pDC);
	void CDrawBadState::DrawLableG1V(CDC *pDC);
	void CDrawBadState::DrawLableG2H(CDC *pDC);

	void CDrawBadState::SetGroupLable(int nIndex, CString str);

	void CDrawBadState::SetData(int nCam, int nHour, int nValue);
};

