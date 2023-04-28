// DlgAiView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "FlyCapture2_EX.h"
#include "FlyCapture2_EXDlg.h"
#include "DlgAiView.h"


// DlgAiView 대화 상자입니다.

IMPLEMENT_DYNAMIC(DlgAiView, CDialog)

DlgAiView::DlgAiView(CWnd* pParent /*=NULL*/)
	: CDialog(DlgAiView::IDD, pParent)
{
	m_hParent = pParent;
	m_pImgDisplay = m_pImg = NULL;
}

DlgAiView::~DlgAiView()
{
	if(m_pImg)
		delete m_pImg;
	if(m_pImgDisplay)
		delete m_pImgDisplay;
}

void DlgAiView::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}
void DlgAiView::init()
{
	int nViewSizeX=230;
	int nViewSizeY=190;
	int nDataMargin=40;

	CRect rtViewSize(0,0,AI_IMAGE_WIDTH,AI_IMAGE_HEIGHT);//AI IMAGE SIZE
	CRect rtDest(0,0,nViewSizeX,nViewSizeY);
	MoveWindow(5, 625, nViewSizeX,nViewSizeY);
	GetDlgItem(IDC_DRAW_AI_VIEW)->MoveWindow(0, 0, nViewSizeX,nViewSizeY);

///	m_DisplayAIImage.InitDisplay(this,1,rtDest,rtViewSize,rtViewSize);

	if(m_pImg == NULL)
	{
		m_pImg = new CImg();
		m_pImg->Create(AI_IMAGE_WIDTH, AI_IMAGE_HEIGHT, 24);
		m_pImg->InitDIB();
	}
	if(m_pImgDisplay == NULL)
	{
		m_pImgDisplay = new CImg();
		m_pImgDisplay->Create(AI_IMAGE_WIDTH, AI_IMAGE_HEIGHT, 8);
		m_pImgDisplay->InitDIB();
	}

}
void DlgAiView::ImageView(CString strPath)
{
	CString strLoadPath;
	//strLoadPath.Format("C:\\AI_DATA\\%s",strPath);
	//m_DisplayAIImage.LoadImg(strPath,0);

	m_pImg->Load(strPath);
	memcpy(m_pImgDisplay->GetImagePtr(),m_pImg->GetImagePtr(),AI_IMAGE_WIDTH*AI_IMAGE_HEIGHT);
//	for (int i=0;i<AI_IMAGE_HEIGHT;i++)
//		memcpy(m_pImg->GetImagePtr()+AI_IMAGE_WIDTH*i, m_pImgbuf->GetImagePtr()+AI_IMAGE_WIDTH*(AI_IMAGE_HEIGHT-i-1), AI_IMAGE_WIDTH);

	m_pImg->Free();
	//delete
	//	m_pImg;
	Invalidate();
}
void DlgAiView::ImageView2(unsigned char *fmAiImage, CRect rtIns)
{
	/*m_pImg->Load(strPath);*/
	int nWidth = CAMERA_WIDTH;
	int nAiWidth = AI_IMAGE_WIDTH;
	int nAiWidthHalf = AI_IMAGE_WIDTH / 2;
	int nAiHeightHalf = AI_IMAGE_HEIGHT / 2;
	int nCropX = 0;
	int nCropY = 0;
	unsigned char* fmCrop = new unsigned char[AI_IMAGE_WIDTH * AI_IMAGE_HEIGHT];
	memset(fmCrop, 0, AI_IMAGE_WIDTH * AI_IMAGE_HEIGHT);

	for (int nX = rtIns.left; nX < rtIns.right; nX++) {
		for (int nY = rtIns.top; nY < rtIns.bottom; nY++) {
			nCropX = nX - rtIns.left;
			nCropY = nY - rtIns.top;
			*(fmCrop + nAiWidth * nCropY + nCropX) = *(fmAiImage + nWidth * nY + nX);

		}
	}

	/*CImg img;
	img.Create(AI_IMAGE_WIDTH, AI_IMAGE_HEIGHT, 8);
	memcpy(img.GetImagePtr(), fmCrop, AI_IMAGE_WIDTH*AI_IMAGE_HEIGHT);
	img.InitDIB();
	img.Save("C:\\TEST\\SAVE.Bmp");*/

	memcpy(m_pImgDisplay->GetImagePtr(), fmCrop, AI_IMAGE_WIDTH*AI_IMAGE_HEIGHT);
	delete fmCrop;

	m_pImg->Free();

	Invalidate();
}
BEGIN_MESSAGE_MAP(DlgAiView, CDialog)
	ON_WM_PAINT()
//	ON_STN_CLICKED(IDC_STATIC_RESULT, &DlgAiView::OnStnClickedStaticResult)
END_MESSAGE_MAP()

// DlgAiView 메시지 처리기입니다.

void DlgAiView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CDC* pDC=GetDC();
	//m_DisplayAIImage.DrawImage(pDC,3);
	int nViewSizeX=230;
	int nViewSizeY=190;
	int nDataMargin=40;

	CRect rtViewSize(0,0,AI_IMAGE_WIDTH,AI_IMAGE_HEIGHT);//AI IMAGE SIZE
	CRect rtDest(0,0,nViewSizeX,nViewSizeY);

	HDC hDC = pDC->GetSafeHdc();
	m_pImgDisplay->Draw(hDC,&rtViewSize,&rtDest);
	ReleaseDC(pDC);

}
