// CameraDisplay.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "FlyCapture2_EX.h"
#include "FlyCapture2_EXDlg.h"
#include "CameraDisplay.h"


// CCameraDisplay 대화 상자입니다.

IMPLEMENT_DYNAMIC(CCameraDisplay, CDialog)

CCameraDisplay::CCameraDisplay(CWnd* pParent /*=NULL*/)
	: CDialog(CCameraDisplay::IDD, pParent)
{
	m_pParent = pParent;

	m_OriginDisp = NULL;

}

CCameraDisplay::~CCameraDisplay()
{
	if(m_OriginDisp)			delete m_OriginDisp;
}

void CCameraDisplay::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_BADIMAGE, m_LabelBadImage);
	DDX_Control(pDX, IDC_STATIC_BADINFO, m_ctBadInfo[0]);
	DDX_Control(pDX, IDC_STATIC_BADINFO2, m_ctBadInfo[1]);
	DDX_Control(pDX, IDC_STATIC_BADINFO3, m_ctBadInfo[2]);
	DDX_Control(pDX, IDC_STATIC_BADINFO4, m_ctBadInfo[3]);
	DDX_Control(pDX, IDC_STATIC_BADINFO5, m_ctBadInfo[4]);
	DDX_Control(pDX, IDC_STATIC_BADINFO6, m_ctBadInfo[5]);
	DDX_Control(pDX, IDC_STATIC_BADINFO7, m_ctBadInfo[6]);
	DDX_Control(pDX, IDC_STATIC_BADINFO8, m_ctBadInfo[7]);
	DDX_Control(pDX, IDC_STATIC_BADINFO9, m_ctBadInfo[8]);
	DDX_Control(pDX, IDC_STATIC_BADINFO10, m_ctBadInfo[9]);
	DDX_Control(pDX, IDC_STATIC_STATS, m_ctLabrlStats);
	DDX_Control(pDX, IDC_STATIC_OKNG, m_LabelUsrOKNG);
	DDX_Control(pDX, IDC_STATIC_BADINFO12, m_lbTitle[0]);
	DDX_Control(pDX, IDC_STATIC_BADINFO13, m_lbTitle[1]);
	DDX_Control(pDX, IDC_STATIC_BADINFO14, m_lbTitle[2]);
}


BEGIN_MESSAGE_MAP(CCameraDisplay, CDialog)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


// CCameraDisplay 메시지 처리기입니다.

BOOL CCameraDisplay::OnInitDialog()
{
	CDialog::OnInitDialog();

	CFlyCapture2_EXDlg *pDlg = (CFlyCapture2_EXDlg*)m_pParent;

	CString str;
	str = pDlg->m_strComname;
	// TODO:  여기에 추가 초기화 작업을 추가합니다.



	m_LabelBadImage.SetTextColor(COLOR_BLACK);
	m_LabelBadImage.SetBkColor(COLOR_TOP_BK);
	m_LabelBadImage.SetFontBold(TRUE);
	m_LabelBadImage.SetFontName(_T("굴림"));
	m_LabelBadImage.SetFontSize(16);
	str.Format("불량 이미지");
	m_LabelBadImage.SetText(str);

	for(int i=0;i<1;i++)
	{
		m_ctBadInfo[i].SetTextColor(COLOR_BLACK);
		m_ctBadInfo[i].SetFontBold(TRUE);
		m_ctBadInfo[i].SetFontName(_T("굴림"));
		m_ctBadInfo[i].SetFontSize(32);
		str.Format("Data Summary");

		m_ctBadInfo[i].SetText(str);
	}

	for(int i=0;i<3;i++){
		m_lbTitle[i].SetTextColor(COLOR_BLACK);
		m_lbTitle[i].SetBkColor(COLOR_TOP_BK);
		m_lbTitle[i].SetFontBold(TRUE);
		m_lbTitle[i].SetFontName(_T("굴림"));
		m_lbTitle[i].SetFontSize(32);
		if(i==0) 		str.Format("Total Trend");
		if(i==1) 		str.Format("Time Trend");
		if(i==2) 		str.Format("Daily Bad Count");
		m_lbTitle[i].SetText(str);
	}


	m_OriginDisp = new CImg();
	m_OriginDisp->Create(CAMERA_WIDTH, CAMERA_HEIGHT, 8);
	m_OriginDisp->InitDIB();
	memset(m_OriginDisp->GetImagePtr(),0,CAMERA_WIDTH*CAMERA_HEIGHT);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CCameraDisplay::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	// 그리기 메시지에 대해서는 CDialog::OnPaint()을(를) 호출하지 마십시오.
//	DrawSheet(COLOR_BLUE,2);
	CFlyCapture2_EXDlg *pDlg = (CFlyCapture2_EXDlg*)m_pParent;
	
//	DrawOriginImage();
// 	if(pDlg->m_nMachine == 2)
// 		ShowNgPosition();
}


void CCameraDisplay::DrawSheet(COLORREF lColor, int nLineWidth)
{
	CDC* pDC1=GetDC();
	CFlyCapture2_EXDlg *pDlg = (CFlyCapture2_EXDlg*)m_pParent;

	m_nMapHeight = pDlg->m_bSheetCount;
	CPen pen,*pOldPen;
	pen.CreatePen(PS_SOLID,1, lColor);
	pOldPen = (CPen*)pDC1->SelectObject(&pen);

	CRect rcMain,rcmap;
	GetWindowRect(rcMain);

	GetDlgItem(IDC_STATIC_SHEET)->GetWindowRect(rcmap);

	m_rtSheet.left = rcmap.left - rcMain.left;
	m_rtSheet.top = rcmap.top - rcMain.top;
	m_rtSheet.right = rcmap.Width();//right - rcMain.right;
	m_rtSheet.bottom = rcmap.Height();//bottom - rcMain.bottom;

	//	m_rtMap =rcMain;//.SetRect()
	//m_rtMap = rcmap;
	m_nSheetCount = pDlg->m_bSheetCount;
	CPoint p1,p2,p3;
	CString str,str2;
	int nn ;//= CAMERA_WIDTH*2/1024;
	int k=17;
	int nh = m_rtSheet.Height();
	int nw = 2000;
	double nmod = m_nMapHeight/m_nSheetCount;
	//외각 그리기
	pDC1->Rectangle(m_rtSheet);
	for(int i=0;i<pDlg->m_bSheetCount;i++)
	{
		p1.SetPoint(0,nmod*i);
		p2 = ScaleTrans(0,p1,1);
		p1.SetPoint(nw,nmod*i);
		p3 = ScaleTrans(0,p1,1);
		pDC1->MoveTo(p2);
		pDC1->LineTo(p3);
		//str.Format("%dcm",(i)*10);
		//pDC1->TextOut(p3.x-20,p3.y,str);
	}



	DeleteObject(pen);
	//불량이 있을 경우
	CPen pen1;
	pen1.CreatePen(PS_SOLID,2, COLOR_RED);
	pOldPen = (CPen*)pDC1->SelectObject(&pen1);

	m_nDefectCount = pDlg->m_Project.m_nMarkResult;
	for(int i=0;i<m_nDefectCount;i++)
	{
		pDlg->m_Display[0].m_dBadSize[i];
		p1.SetPoint(0,pDlg->m_Project.m_nSheetPoint[i]);
		p2 = ScaleTrans(0,p1,1);
		p1.SetPoint(nw,pDlg->m_Project.m_nSheetPoint[i]);
		p3 = ScaleTrans(0,p1,1);
		pDC1->MoveTo(p2);
		pDC1->LineTo(p3);
		if(pDlg->m_Project.m_nSheetPoint[i]<=0)
			pDlg->m_Project.m_nSheetPoint[i]=1;
		if(pDlg->m_Project.m_nBadFlag[i]==0)
			str.Format("%d매 PS Sheet",pDlg->m_Project.m_nSheetPoint[i]);
		else
			str.Format("%d매 표리반전",pDlg->m_Project.m_nSheetPoint[i]);
		pDC1->TextOut(p3.x+30,p3.y-10,str);

		if(i<10){
			str.Format("%d매 %d",pDlg->m_Project.m_nSheetPoint[i],pDlg->m_Project.m_nBadFlag[i]);
			m_ctBadInfo[i].SetText(str);
		}
	}

	DeleteObject(pen1);


	DrawOriginImage();
	ReleaseDC(pDC1);
}


CPoint CCameraDisplay::ScaleTrans(int nCam,CPoint point,int direction)
{
	CPoint point_original(point);
	CPoint point_trans;

	int width = m_rtSheet.Width();
	int height = m_rtSheet.Height();
	int nw = 2000;//*2;
	int nh = m_nMapHeight;//CAMERA_HEIGHT;


	point_trans.x = m_rtSheet.left + int(((float)(width) / (float)(nw)) * (float)(point_original.x));
	point_trans.y = m_rtSheet.top + int(((float)(height) / (float)(nh)) * (float)(point_original.y));
	point_trans = CheckPt(point_trans);

	return point_trans;
}

CPoint CCameraDisplay::CheckPt(CPoint ptCheck)
{
	if(ptCheck.x<m_rtSheet.left)	ptCheck.x = m_rtSheet.left;
	if(ptCheck.x>m_rtSheet.right)	ptCheck.x = m_rtSheet.right-2;
	if(ptCheck.y<m_rtSheet.top)	ptCheck.y = m_rtSheet.top+1;
	if(ptCheck.y>m_rtSheet.bottom)ptCheck.y = m_rtSheet.bottom;

	return ptCheck;
}

void CCameraDisplay::DrawOriginImage()
{
	CFlyCapture2_EXDlg *pDlg = (CFlyCapture2_EXDlg*)m_pParent;
//	CPaintDC dc(this); // device context for painting	

	CWnd *pWnd;
	CDC *pdc;
	//	CRect rect(0,NAMALIZE_CHAR_SIZE,NAMALIZE_CHAR_SIZE,0),rect2;
	CRect rect,rect2;
	CString str;

	for(int i=0;i<1;i++)
	{
		rect.SetRect(0,0,CAM_WIDTH,CAMERA_HEIGHT);
		pWnd = GetDlgItem(IDC_STATIC_BAD1+i);
		pdc = pWnd->GetDC();	
		pWnd->GetClientRect(&rect2);
//		pDlg->m_Project.m_pPatLoad[j][m_nRadioNum*10+i]->Draw(pdc->GetSafeHdc(),&rect,&rect2);
		//pDlg->m_Project.m_pBadImage[i]->Draw(pdc->GetSafeHdc(),&rect,&rect2);
		m_OriginDisp->Draw(pdc->GetSafeHdc(),&rect,&rect2);
		pWnd->ReleaseDC(pdc);
//		str.Format("%c",pDlg->m_Project.GetIndex2Char(m_nRadioNum*10+i));
//		m_PatNo[i].SetText(str);
	}
	ShowLabel();
	//m_ctLabrlStats
}

void CCameraDisplay::OnLButtonDown(UINT nFlags, CPoint point)
{
	CFlyCapture2_EXDlg *pDlg = (CFlyCapture2_EXDlg*)m_pParent;
	CDialog::OnLButtonDown(nFlags, point);
}

void CCameraDisplay::ShowLabel()
{
	CFlyCapture2_EXDlg *pDlg = (CFlyCapture2_EXDlg*)m_pParent;

	CString str;

	if(pDlg->m_pDlgSet->m_FormTab3->m_bWeldingLabel){
		if(pDlg->m_pDlgSet->m_FormTab3->m_nWeldingColor == 0)
			m_ctBadInfo[0].SetBkColor(COLOR_RED);
		else if(pDlg->m_pDlgSet->m_FormTab3->m_nWeldingColor == 1)
			m_ctBadInfo[0].SetBkColor(COLOR_GREEN);
		else if(pDlg->m_pDlgSet->m_FormTab3->m_nWeldingColor == 2)
			m_ctBadInfo[0].SetBkColor(COLOR_BLUE);
		else
			m_ctBadInfo[0].SetBkColor(COLOR_YELLOW);
	}
	else{
		m_ctBadInfo[0].SetTextColor(COLOR_BLACK);

		if(pDlg->m_strComname.Find("3A")>=0 || pDlg->m_strComname.Find("4A")>=0 
			|| pDlg->m_strComname.Find("5C")>=0 || pDlg->m_strComname.Find("6A")>=0
			|| pDlg->m_strComname.Find("7B")>=0 || pDlg->m_strComname.Find("8A")>=0)
			m_ctBadInfo[0].SetBkColor(COLOR_RED);
		else if(pDlg->m_strComname.Find("3B")>=0 || pDlg->m_strComname.Find("4B")>=0 
			|| pDlg->m_strComname.Find("5B")>=0 || pDlg->m_strComname.Find("6B")>=0
			|| pDlg->m_strComname.Find("7A")>=0 || pDlg->m_strComname.Find("8B")>=0)
			m_ctBadInfo[0].SetBkColor(COLOR_YELLOW);
		else if(pDlg->m_strComname.Find("3C")>=0 || pDlg->m_strComname.Find("4C")>=0 
			|| pDlg->m_strComname.Find("6C")>=0
			|| pDlg->m_strComname.Find("7C")>=0 || pDlg->m_strComname.Find("8C")>=0)
			m_ctBadInfo[0].SetBkColor(COLOR_GREEN);
		else if(pDlg->m_strComname.Find("5A")>=0){
			m_ctBadInfo[0].SetBkColor(COLOR_BLUE);
			m_ctBadInfo[0].SetTextColor(COLOR_WHITE);
		}
		else
			m_ctBadInfo[0].SetBkColor(COLOR_TOP_BK);
	}

	m_ctBadInfo[0].SetFontBold(TRUE);
	m_ctBadInfo[0].SetFontName(_T("굴림"));
	m_ctBadInfo[0].SetFontSize(32);

	if(pDlg->m_pDlgSet->m_FormTab3->m_bWeldingLabel){
		str.Format("Laser Welding #%d", pDlg->m_pDlgSet->m_FormTab3->m_nWeldingNum+1);
	}
	else{
		if(pDlg->m_strComname.Find("2")>=0)
			str.Format("Camera 2");
		else
			str.Format("Camera 1");

		if(pDlg->m_strComname.Find("3A")>=0 || pDlg->m_strComname.Find("4A")>=0)
			str.Format("Camera 1");
		else if(pDlg->m_strComname.Find("3B")>=0 || pDlg->m_strComname.Find("4B")>=0)
			str.Format("Camera 2");
		else if(pDlg->m_strComname.Find("3C")>=0 || pDlg->m_strComname.Find("4C")>=0)
			str.Format("Camera 3");
		else if(pDlg->m_strComname.Find("5A")>=0 || pDlg->m_strComname.Find("6A")>=0)
			str.Format("Laser Welding #3 (Cam 1)");
		else if(pDlg->m_strComname.Find("5B")>=0 || pDlg->m_strComname.Find("6B")>=0)
			str.Format("Laser Welding #2 (Cam 2)");
		else if(pDlg->m_strComname.Find("5C")>=0 || pDlg->m_strComname.Find("6C")>=0)
			str.Format("Laser Welding #1 (Cam 3)");
		//7호기,8호기
		else if(pDlg->m_strComname.Find("7A")>=0 || pDlg->m_strComname.Find("8A")>=0)
			str.Format("Laser Welding #3 (Cam 1)");
		else if(pDlg->m_strComname.Find("7B")>=0 || pDlg->m_strComname.Find("8B")>=0)
			str.Format("Laser Welding #2 (Cam 2)");
		else if(pDlg->m_strComname.Find("7C")>=0 || pDlg->m_strComname.Find("8C")>=0)
			str.Format("Laser Welding #1 (Cam 3)");
	}

	m_ctBadInfo[0].SetText(str);

	//str.Format("Frame:%d       Trigger count : %d           Ave:%d",pDlg->m_Project.m_nFrameCount,pDlg->m_nTriggerCount,pDlg->m_nCanCheckAve);
	//m_ctLabrlStats.SetText(str);
}

void CCameraDisplay::ShowNgPosition()
{
	CFlyCapture2_EXDlg *pDlg = (CFlyCapture2_EXDlg*)m_pParent;

//	CWnd *pWnd;
	CDC *pDC;


//	pWnd = GetDlgItem(IDC_STATIC_BAD1);
	pDC = GetDC();	

	int nmax = 11;
	if(pDlg->m_nMachine==3)
		nmax = 24;
	else if(pDlg->m_strComname.Find("3C")>=0 || pDlg->m_strComname.Find("4C")>=0)
		nmax = 10;
	else if(pDlg->m_strComname.Find("3A")>=0 || pDlg->m_strComname.Find("3B")>=0 || pDlg->m_strComname.Find("4A")>=0 
		|| pDlg->m_strComname.Find("4B")>=0 || pDlg->m_strComname.Find("5A")>=0 
		|| pDlg->m_strComname.Find("6A")>=0 || pDlg->m_strComname.Find("7A")>=0 || pDlg->m_strComname.Find("8A")>=0)
		nmax = 9;
	else if(pDlg->m_strComname.Find("5C")>=0 || pDlg->m_strComname.Find("6C")>=0 || pDlg->m_strComname.Find("7C")>=0 || pDlg->m_strComname.Find("8C")>=0
		|| pDlg->m_strComname.Find("5B")>=0 || pDlg->m_strComname.Find("6B")>=0 || pDlg->m_strComname.Find("7B")>=0 || pDlg->m_strComname.Find("8B")>=0)
		nmax = 8;

	for(int i=0; i<nmax; i++)
	{
		CPen pen,*pOldPen;

		if(pDlg->m_bNgPosition[i]){
			pen.CreatePen(PS_SOLID, 2, COLOR_RED);
			pOldPen = (CPen*)pDC->SelectObject(&pen);
			pDC->Ellipse(i*20+20+i*10, 80, i*20+40+i*10, 100);
			pDC->SelectObject(pOldPen);
			DeleteObject(pen);
		}
		else{
			pen.CreatePen(PS_SOLID, 2, COLOR_GREEN);
			pOldPen = (CPen*)pDC->SelectObject(&pen);
			pDC->Ellipse(i*20+20+i*10, 80, i*20+40+i*10, 100);
			pDC->SelectObject(pOldPen);
			DeleteObject(pen);
		}
	}

	ReleaseDC(pDC);
}
