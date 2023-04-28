#pragma once

//#include "CgDisplay.h"
#include "Img.h"
// DlgAiView ��ȭ �����Դϴ�.

class DlgAiView : public CDialog
{
	DECLARE_DYNAMIC(DlgAiView)

public:
	DlgAiView(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	CWnd *m_hParent;

	virtual ~DlgAiView();
	
	// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_AI_VIEW };


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

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
