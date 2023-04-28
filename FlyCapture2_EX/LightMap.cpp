// LightMap.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "FlyCapture2_EX.h"
#include "FlyCapture2_EXDlg.h"
#include "LightMap.h"

// CLightMap 대화 상자입니다.

IMPLEMENT_DYNAMIC(CLightMap, CDialog)

CLightMap::CLightMap(CWnd* pParent /*=NULL*/)
	: CDialog(CLightMap::IDD, pParent)
{

	m_hParent = pParent;

	//m_nMapHeight = 500;
	//for(int i=0;i<20;i++)
	//	m_nLightPoint[i] = 0;
	//m_nLightDefectCount = 0;

}

CLightMap::~CLightMap()
{
}

void CLightMap::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CLightMap, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_HIDE, &CLightMap::OnBnClickedButtonHide)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON_NORMAL, &CLightMap::OnBnClickedButtonNormal)
	ON_BN_CLICKED(IDC_BUTTON_REPACK, &CLightMap::OnBnClickedButtonRepack)
END_MESSAGE_MAP()


// CLightMap 메시지 처리기입니다.

void CLightMap::OnBnClickedButtonHide()
{
	//for(int i=0;i<20;i++)
	//	m_nLightPoint[i] = 0;
	//m_nLightDefectCount = 0;

	ShowWindow(SW_HIDE);

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CLightMap::DrawDotLine(COLORREF lColor, int nLineWidth)
{
////	CDC* pDC1=GetDC();
////
////	CPen pen,*pOldPen;
////	pen.CreatePen(PS_DOT,1, lColor);
////	pOldPen = (CPen*)pDC1->SelectObject(&pen);
////
////	CRect rcMain,rcmap;
////	GetWindowRect(rcMain);
////
////	GetDlgItem(IDC_STATIC_LIGHTMAP)->GetWindowRect(rcmap);
////
////	m_nMapHeight = 5000;
////
////	m_rtMap.left = rcmap.left - rcMain.left;
////	m_rtMap.top = rcmap.top - rcMain.top;
////	m_rtMap.right = rcmap.Width();//right - rcMain.right;
////	m_rtMap.bottom = rcmap.Height();//bottom - rcMain.bottom;
////
//////	m_rtMap =rcMain;//.SetRect()
////	//m_rtMap = rcmap;
////
////	CPoint p1,p2,p3;
////	CString str,str2;
////	int nn ;//= CAMERA_WIDTH*2/1024;
////	int k=17;
////	for(int i=0;i<9;i++)
////	{
////		p1.SetPoint((i)*1000,0);
////		p2 = ScaleTrans(0,p1,1);
////		p1.SetPoint((i)*1000,m_nMapHeight);
////		p3 = ScaleTrans(0,p1,1);
////		pDC1->MoveTo(p2);
////		pDC1->LineTo(p3);
////		str.Format("%dcm",(i)*10);
////		pDC1->TextOut(p3.x-20,p3.y,str);
////	}
////	for(int i=9;i<17;i++)
////	{
////		p1.SetPoint((i)*1000,0);
////		p2 = ScaleTrans(0,p1,1);
////		p1.SetPoint((i)*1000,m_nMapHeight);
////		p3 = ScaleTrans(0,p1,1);
////		pDC1->MoveTo(p2);
////		pDC1->LineTo(p3);
////		str.Format("%dcm",(16-i)*10);
////		pDC1->TextOut(p3.x-20,p3.y,str);
////	}
////	//top
////	p1.SetPoint(0,0);
////	p2 = ScaleTrans(0,p1,1);
////	p1.SetPoint((16)*1000,0);
////	p3 = ScaleTrans(0,p1,1);
////	pDC1->MoveTo(p2);
////	pDC1->LineTo(p3);
////
//////bottom
////	p1.SetPoint(0,m_nMapHeight);
////	p2 = ScaleTrans(0,p1,1);
////	p1.SetPoint((16)*1000,m_nMapHeight);
////	p3 = ScaleTrans(0,p1,1);
////	pDC1->MoveTo(p2);
////	pDC1->LineTo(p3);
////
////	DeleteObject(pen);
//////불량이 있을 경우
////	CPen pen1;
////	pen1.CreatePen(PS_SOLID,2, COLOR_RED);
////	pOldPen = (CPen*)pDC1->SelectObject(&pen1);
////
////	for(int i=0;i<m_nLightDefectCount;i++)
////	{
////		if(m_nLightPoint[i]<10 || m_nLightPoint[i] > 8000*2)
////			continue;
////		//m_nLightPoint
////		p1.SetPoint(m_nLightPoint[i],0);
////		p2 = ScaleTrans(0,p1,1);
////		p1.SetPoint(m_nLightPoint[i],m_nMapHeight);
////		p3 = ScaleTrans(0,p1,1);
////		pDC1->MoveTo(p2);
////		pDC1->LineTo(p3);
////		str.Format("150 매 표리반전",(16-i)*10);
////		pDC1->TextOut(p3.x-20,p3.y,str);
////	}
////
////	DeleteObject(pen1);
////	ReleaseDC(pDC1);

}


CPoint CLightMap::ScaleTrans(int nCam,CPoint point,int direction)
{
	CPoint point_original(point);
	CPoint point_trans;

	//int width = m_rtMap.Width();
	//int height = m_rtMap.Height();
	//int nw = CAMERA_WIDTH*2;
	//int nh = m_nMapHeight;//CAMERA_HEIGHT;


	//point_trans.x = m_rtMap.left + int(((float)(width) / (float)(nw)) * (float)(point_original.x));
	//point_trans.y = m_rtMap.top + int(((float)(height) / (float)(nh)) * (float)(point_original.y));
	//point_trans = CheckPt(point_trans);

	return point_trans;
}

CPoint CLightMap::CheckPt(CPoint ptCheck)
{
	//if(ptCheck.x<m_rtMap.left)	ptCheck.x = m_rtMap.left;
	//if(ptCheck.x>m_rtMap.right)	ptCheck.x = m_rtMap.right-2;
	//if(ptCheck.y<m_rtMap.top)	ptCheck.y = m_rtMap.top+1;
	//if(ptCheck.y>m_rtMap.bottom)ptCheck.y = m_rtMap.bottom;

	return ptCheck;
}
void CLightMap::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	// 그리기 메시지에 대해서는 CDialog::OnPaint()을(를) 호출하지 마십시오.
//	DrawDotLine(COLOR_BLUE,2);
}

void CLightMap::OnBnClickedButtonNormal()
{//정상포장
	CFlyCapture2_EXDlg *pView = (CFlyCapture2_EXDlg*)m_hParent;
	m_nPackMode = 0;
	pView->StartInsp();
	ShowWindow(SW_HIDE);

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CLightMap::OnBnClickedButtonRepack()
{//재포장
	CFlyCapture2_EXDlg *pView = (CFlyCapture2_EXDlg*)m_hParent;
	m_nPackMode = 1;
	pView->StartInsp();
	ShowWindow(SW_HIDE);

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
