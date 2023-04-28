#pragma once

//#include "CgDisplay.h"
#include "Img.h"
// DlgAiView 대화 상자입니다.

class DlgAiView : public CDialog
{
	DECLARE_DYNAMIC(DlgAiView)

public:
	DlgAiView(CWnd* pParent = NULL);   // 표준 생성자입니다.
	CWnd *m_hParent;

	virtual ~DlgAiView();
	
	// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_AI_VIEW };


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	//CgDisplay m_DisplayAIImage;
	//CLabel		m_StaticLabel;
	CImg*		m_pImg;
	CImg*		m_pImgDisplay;

	void init();
	void ImageView(CString strPath);
	void ImageView2(unsigned char *fmAiImage, CRect rtIns);
	

	afx_msg void OnPaint();
//	afx_msg void OnStnClickedStaticResult();
};
