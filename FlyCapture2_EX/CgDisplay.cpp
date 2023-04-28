#include "StdAfx.h"
#include "CgDisplay.h"


CgDisplay::CgDisplay(void)
{
	m_pImg = NULL;
	m_pImgbuf = NULL;
	m_nZoom = 1;
	m_bDisplayRun = FALSE;
	m_bTrackerUse = TRUE;
	m_NotSaveImage=FALSE;
	m_bDummyDraw=FALSE;
	m_bAutoSetOn=TRUE;
	m_brtDetect=FALSE;
	m_fm=NULL;
	m_bTrackshow = TRUE;
	for(int i =0; i<100;i++)
	{
		m_rtins[i]=0;
	}
	for (int i=0;i<5;i++)	m_strImagePath[i] = "";
	m_strTimePath = "";
	m_strMesLot = "";
	m_bHoleSearchArea = FALSE;
	m_bRegMode = TRUE;
	m_nUseTracker = MAX_TRACKER;
	m_strAIName = "";
	m_strImgName = "";
}

CgDisplay::~CgDisplay(void)
{
	m_bDisplayRun = FALSE;
	Sleep(100);
	if(m_pImgbuf)
		delete m_pImgbuf;
	if(m_pImg)			
		delete m_pImg;

	//delete m_ptDefault[0];
	//delete m_ptDefault[1];
	//delete m_pProfileX;
	//GetExitCodeProcess
	//TerminateProcess()
}

void CgDisplay::InitDisplay(LPVOID pParam,BOOL bMono,CRect rtDisplay,CRect rtFullImg,CRect rtRoiImg)
{
	int scale = 1;
	if(bMono==FALSE)
		scale = 3;

	m_ptDefault[0] = new CPoint[MAX_POINT];
	m_ptDefault[1] = new CPoint[MAX_POINT];
	m_pProfileX = new int[rtFullImg.Width()];
	ResetArray();

	m_rtZoom = m_rtRoiImg = rtRoiImg;
	m_rtFullImg = rtFullImg;
	m_rtDisplay = rtDisplay;

	m_pImg = new CImg();
	m_pImg->Create(rtFullImg.Width(), rtFullImg.Height(), 8*scale);
	m_pImg->InitDIB();
	memset(m_pImg->GetImagePtr(),0,rtFullImg.Width()*rtFullImg.Height()*scale);

	m_pImgbuf = new CImg();
	m_pImgbuf->Create(rtFullImg.Width(), rtFullImg.Height(), 8*scale);
	m_pImgbuf->InitDIB();
	memset(m_pImgbuf->GetImagePtr(),0,rtFullImg.Width()*rtFullImg.Height()*scale);
		
	m_bDisplayRun = TRUE;

	//if(m_bTrackerUse)
	//{
		for(int i=0;i<MAX_TRACKER;i++)
		{
			if(i==0)
				m_strTrackerName[i].Format(" ");
			if(i==1)
				m_strTrackerName[i].Format("[하단 검사영역]");
			if(i==2)
				m_strTrackerName[i].Format("[필름꺾임 검사영역]");
			m_rtImgTracker[i].SetRect(10+i*100,10,110+i*(100),110);
			m_Tracker[i].m_rect.SetRect(ScaleTrans(m_rtImgTracker[i].TopLeft(),1),ScaleTrans(m_rtImgTracker[i].BottomRight(),1));
			m_Tracker[i].m_nStyle = CRectTracker::solidLine;//|CRectTracker::resizeInside;
			m_Tracker[i].m_nHandleSize=20;
		}
		m_rtMaster = m_rtImgTracker[0];
	m_bTrackerUse=TRUE;
}


void CgDisplay::SetZoom(short zDelta, CPoint pt)
{
	pt = ScaleTrans(pt,0);
	if(zDelta==0)		m_nZoom=0;
	else if(zDelta>0)	m_nZoom++;
	else				m_nZoom--;

	if(m_nZoom<1)		m_nZoom=1;

	double nhalfwid = int((float)m_rtRoiImg.Width()/((float)m_nZoom*1.5));
	double nhalfhei = int((float)m_rtRoiImg.Height()/((float)m_nZoom*1.5));

//	CPoint ptZoomCenter(m_rtRoiImg.CenterPoint());
	CPoint ptZoomCenter(pt);
	m_rtZoom.left = ptZoomCenter.x - nhalfwid;
	m_rtZoom.right = ptZoomCenter.x + nhalfwid;
	m_rtZoom.top = ptZoomCenter.y - nhalfhei;
	m_rtZoom.bottom = ptZoomCenter.y + nhalfhei;
	CheckScreen();
}

void CgDisplay::CheckScreen()
{

	double dhalfwid = int((float)m_rtRoiImg.Width()/((float)m_nZoom*2.0));
	double dhalfhei = int((float)m_rtRoiImg.Height()/((float)m_nZoom*2.0));

	if(m_rtZoom.left <m_rtFullImg.left)	
		m_rtZoom.left = m_rtFullImg.left,m_rtZoom.right = dhalfwid*2;
	else if(m_rtZoom.right >=m_rtFullImg.right)	
		m_rtZoom.left = m_rtFullImg.right-dhalfwid*2,m_rtZoom.right = m_rtFullImg.right;

	if(m_rtZoom.top <m_rtFullImg.top)			
		m_rtZoom.top = m_rtFullImg.top,m_rtZoom.bottom = dhalfhei*2;
	else if(m_rtZoom.bottom >=m_rtFullImg.bottom)
		m_rtZoom.top = m_rtFullImg.bottom-dhalfhei*2,m_rtZoom.bottom = m_rtFullImg.bottom;

	for(int i=0;i<MAX_TRACKER;i++)
	{
		CPoint p1(m_rtImgTracker[i].TopLeft()),p2(m_rtImgTracker[i].BottomRight());
		p1 = ScaleTrans(p1,1);
		p2 = ScaleTrans(p2,1);
		m_Tracker[i].m_rect.SetRect(p1,p2);
	}
}

void CgDisplay::SetMousePt(CPoint ptMouseStt)
{
	m_ptMouseStt = ptMouseStt;
}

CPoint CgDisplay::SetMove(CPoint ptCur)
{
	CPoint ptMove,pt1,pt2;
	pt1 = ScaleTrans(m_ptMouseStt,0);
	pt2 = ScaleTrans(ptCur,0);
	ptMove.x = pt2.x - pt1.x;
	ptMove.y = pt1.y - pt2.y;

	m_rtZoom.OffsetRect(-ptMove.x,ptMove.y);
	m_ptMouseStt = ptCur;
	CheckScreen();

	return pt2;
}

CPoint CgDisplay::GetPos(CPoint ptCur,int* nValue)
{
	if(m_bDisplayRun==FALSE)
		return CPoint(0,0);
	CPoint pt2;
	pt2 = ScaleTrans(ptCur,0);

	if(m_pImg->IsDataNull()) return pt2;
	if(m_rtFullImg.PtInRect(pt2) && pt2.x >0 && pt2.y>0)
	{
		unsigned char* fm = m_pImg->GetImagePtr();
		*nValue = 0;
		*nValue = fm[pt2.x + pt2.y*CAMERA_WIDTH];
		//*nProfile = m_pProfileX[pt2.x];
	}

	return pt2;
}

void CgDisplay::DrawImage(CDC *pDC,int bShowPixel)
{
	//CFlyCapture2_EXDlg *pDlg = (CFlyCapture2_EXDlg*)m_hParent;
	if(m_bDisplayRun==FALSE)
		return;

	HDC hDC = pDC->GetSafeHdc();
	double dhalfwid = int((float)m_rtRoiImg.Width()/((float)m_nZoom*2.0));
	double dhalfhei = int((float)m_rtRoiImg.Height()/((float)m_nZoom*2.0));

	CRect rtDisplay(m_rtDisplay);
	CRect rtZoom(m_rtZoom);
	if (m_rtFullImg.Height()*m_rtFullImg.Width()>12288*4*CAM_WIDTH)	//이미지가 크면 나누어 보여준다.
	{
		int	   nPart = 20;
		double dpitch = (double)(m_rtDisplay.bottom-m_rtDisplay.top)/(double)nPart;
		double dpitch2 = (double)(m_rtZoom.bottom-m_rtZoom.top)/(double)nPart;
		for (int i=0;i<nPart;i++)
		{
			rtDisplay.top = int(m_rtDisplay.top+dpitch*(i+1));
			rtDisplay.bottom = int(m_rtDisplay.top+dpitch*(i));
			rtZoom.top = int(m_rtZoom.top+dpitch2*(i));
			rtZoom.bottom = int(m_rtZoom.top+dpitch2*(i+1));
			m_pImg->Draw(pDC->m_hDC,&rtZoom,&rtDisplay);
		}
	}
	else{
		int temp = rtDisplay.top;
		rtDisplay.top = rtDisplay.bottom;
		rtDisplay.bottom = temp;
		m_pImg->Draw(hDC,&m_rtZoom,&rtDisplay);
	}
	if(m_pImg->GetWidth()<350 || bShowPixel==3)
		return;
	
	//DrawBadArrayRect(pDC);
	//DrawArrayXmark(pDC);
	for(int i=0;i<m_nRtCnt[0];i++)
		DrawRect(pDC,m_rt[0][i],COLOR_BLUE,1,"");//i==0이면 metal부 외곽rect, 1이면 내부 ins rect

	if(bShowPixel)
		DrawArrayPoint(pDC);

	if(m_bTrackerUse){
		for(int i=0;i<MAX_TRACKER&& i<m_nUseTracker;i++){
			CString strtmp;
			if(i==0) strtmp.Format("%d Can Check",i+1);
			else if(i==1) strtmp.Format("%d Cap Check",i+1);
			else if(i==2) strtmp.Format("%d Tab Area",i+1);
			else if(i==3) strtmp.Format("%d Cap Left",i+1);
			else if(i==4) strtmp.Format("%d Cap Right",i+1);
			else if(i==6) strtmp.Format("%d Light",i+1);
			else if(i==7) strtmp.Format("%d Sharpness",i+1);
			else strtmp.Format("%d Hole Area",i+1);

			if(!m_bHoleSearchArea && i==2)continue;
			if(!m_bRegMode && i==5) continue;
			if(!m_bAutoSetOn || i == 6)
				DrawRect(pDC,m_rtImgTracker[i],COLOR_GREEN,2,strtmp);
			else
				DrawRect(pDC,m_rtImgTracker[i],COLOR_YELLOW,2,strtmp);
		}
	}
}

void CgDisplay::ResetArray()
{
	m_nPtCnt[0] = m_nPtCnt[1] = 0;
	for(int i=0;i<MAX_CAMERAS;i++)
	{
		m_nRtCnt[i] = 0;
		m_nBadCnt[i] = 0;
	}
	m_nXmarkCnt[0] = m_nXmarkCnt[1] =0;
}

void CgDisplay::AddArrayXmark(CPoint pt,int idx)
{
	//if(m_nXmarkCnt[idx]<MAX_POINT)
	//	m_ptXmark[idx][m_nXmarkCnt[idx]++] = pt;
}

void CgDisplay::AddArrayPoint(CPoint pt,int idx)
{
	if(m_nPtCnt[idx]<MAX_POINT)
		m_ptDefault[idx][m_nPtCnt[idx]++] = pt;
}

void CgDisplay::AddArrayRect(CRect rt,int idx,CString str)
{
	if(m_nRtCnt[idx]<MAX_RECT)
	{
		m_rt[idx][m_nRtCnt[idx]] = rt;
		m_strRtInfo[idx][m_nRtCnt[idx]] = str;
		m_nRtCnt[idx]++;
	}
}

void CgDisplay::DrawArrayPoint(CDC *pDC)
{
	for (int k=0;k<2;k++)
		for (int i=0;i<m_nPtCnt[k];i++)
		{
			if(k==0)
				DrawPoint(pDC, m_ptDefault[k][i], DEFAULT_COLOR, 2);
			else
				DrawPoint(pDC, m_ptDefault[k][i], COLOR_YELLOW, 2);

		}
}
void CgDisplay::DrawArrayXmark(CDC *pDC)
{
	//BOOL bXmark = FALSE;
	//for (int k=0;k<2;k++)
	//	for (int i=0;i<m_nXmarkCnt[k];i++)
	//	{
	//		CRect rtXmark(m_ptXmark[k][i].x-20,m_ptXmark[k][i].y-20,m_ptXmark[k][i].x+20,m_ptXmark[k][i].y+20);

	//		if(bXmark)
	//		{
	//			if(k==0)	DrawXmark(pDC, rtXmark, DEFAULT_COLOR, 1,"");
	//			else		DrawXmark(pDC, rtXmark, COLOR_YELLOW, 1,"");
	//		}
	//		else
	//		{
	//			if(k==0)	DrawCross(pDC, rtXmark, DEFAULT_COLOR, 1,"");
	//			else		DrawCross(pDC, rtXmark, COLOR_YELLOW, 1,"");
	//		}
	//	}
}

void CgDisplay::DrawArrayRect(CDC *pDC)
{
	CString str;
	for (int k=0;k<MAX_CAMERAS;k++)
		for (int i=0;i<m_nRtCnt[k];i++)
		{
			str.Empty();
			//if(m_nZoom>10)
			str = m_strRtInfo[k][i];
			if(k==0)		DrawRect(pDC, m_rt[k][i], COLOR_RED, 1,str,i);
			else if(k==1)	DrawRect(pDC, m_rt[k][i], COLOR_GREEN, 1,str,i);
			else if(k==2)	DrawRect(pDC, m_rt[k][i], COLOR_BLUE, 1,str,0);
			else			DrawRect(pDC, m_rt[k][i], COLOR_WHITE, 1,str,0);
		}
}
void CgDisplay::DrawBadArrayRect(CDC *pDC)
{
	CString str;
	int TempInt=0;
	double TempDouble=0;
	for (int k=0;k<MAX_CAMERAS;k++)
		for (int i=0;i<m_nBadCnt[k];i++)
		{
			//if(m_nBadVal[i]<-5000)
			//	TempInt=0;
			//else if(m_nBadVal[i]>5000)
			//	TempInt=0;
			//else
			//	TempInt=m_nBadVal[i];
			//TempDouble=m_dBadHeight[i];//m_dBadSize[i];

			str.Format("");//V[%d]%.0f",TempInt,TempDouble);
//			str = m_strRtInfo[k][i];
			if(m_nBadFlag[i]==0)
				DrawRect(pDC, m_rtBad[k][i], COLOR_RED, 1,str,i);
			else if(m_nBadFlag[i]==1)
				DrawRect(pDC, m_rtBad[k][i], COLOR_GREEN, 1,str,i);
			else if(m_nBadFlag[i]==2)
				DrawRect(pDC, m_rtBad[k][i], COLOR_GREEN, 1,str,i);
			else if(m_nBadFlag[i]==3)
				DrawRect(pDC, m_rtBad[k][i], COLOR_BLUE, 1,str,i);
			else if(m_nBadFlag[i]==4)
				DrawRect(pDC, m_rtBad[k][i], COLOR_YELLOW, 1,str,i);
		}
}
void CgDisplay::DrawArrayLine(CDC *pDC,int nNo,double* dx,double* dy,CString* strInfo,COLORREF lColor, int nLineWidth ,int nHei)
{
	CString str("");
	for(int i=0;i<nNo;i++)
	{
		if(strInfo!=NULL)
			str = strInfo[i];
		DrawLine(pDC, CPoint(int(dx[i]),int(dy[i])-nHei), CPoint(int(dx[i]),int(dy[i])+nHei),str,lColor,nLineWidth);
	}
}




void CgDisplay::DrawLine(CDC *pDC, CPoint p1, CPoint p2,CString strInfo, COLORREF lColor, int nLineWidth)
{
	CPen pen,*pOldPen;
	if(strInfo=="-10")
		pen.CreatePen(PS_DOT,nLineWidth, lColor);
	else
		pen.CreatePen(PS_SOLID,nLineWidth, lColor);
	pOldPen = (CPen*)pDC->SelectObject(&pen);

	p1 = ScaleTrans(p1,1);
	p2 = ScaleTrans(p2,1);
	pDC->MoveTo(p1);
	pDC->LineTo(p2);

	if(m_nZoom==1)
		pDC->TextOut(p1.x,p1.y-10,strInfo);//pDC->TextOut(p1.x,p1.y-20,strInfo);
	pDC->SelectObject(pOldPen);
	DeleteObject(pen);
}

void CgDisplay::DrawPoint(CDC *pDC, CPoint p1, COLORREF lColor, int nLineWidth)
{
	CPen pen,*pOldPen;
	pen.CreatePen(PS_SOLID,nLineWidth, lColor);
	pOldPen = (CPen*)pDC->SelectObject(&pen);

	p1 = ScaleTrans(p1,1);
	if(nLineWidth==1)
		pDC->SetPixel(p1,lColor);
	else
		pDC->Rectangle(p1.x-nLineWidth,p1.y-nLineWidth,p1.x+nLineWidth,p1.y+nLineWidth);

	pDC->SelectObject(pOldPen);
	DeleteObject(pen);
}
void CgDisplay::DrawDotMark(CDC *pDC, CPoint p1, COLORREF lColor, int nLineWidth)
{
	CPen pen,*pOldPen;
	pen.CreatePen(PS_SOLID,nLineWidth, lColor);
	pOldPen = (CPen*)pDC->SelectObject(&pen);

	p1 = ScaleTrans(p1,1);

	int nOff = 2;
	pDC->Ellipse(p1.x-nOff,p1.y-nOff,p1.x+nOff,p1.y+nOff);

	pDC->SelectObject(pOldPen);
	DeleteObject(pen);
}

void CgDisplay::DrawArrayPoint(CDC *pDC,int nNo,double* dx,double* dy, COLORREF lColor, int nLineWidth)
{
	for(int i=0;i<nNo;i++)
		DrawPoint(pDC, CPoint(int(dx[i]),int(dy[i])),lColor,nLineWidth);
}

void CgDisplay::DrawRect(CDC *pDC, CRect rt, COLORREF lColor, int nLineWidth, CString strInfo,int nFlag)
{
	CPen pen,*pOldPen;
	pen.CreatePen(PS_SOLID,nLineWidth, lColor);
	pOldPen = (CPen*)pDC->SelectObject(&pen);
	CPoint p1,p2;
	//rt.right += 1;
	//rt.bottom += 1;

	p1 = ScaleTrans(rt.TopLeft(),1);
	p2 = ScaleTrans(rt.BottomRight(),1);
	pDC->MoveTo(p1);
	pDC->LineTo(p2.x,p1.y);
	pDC->LineTo(p2);
	pDC->LineTo(p1.x,p2.y);
	pDC->LineTo(p1);

	if(m_nZoom==1)
	{
		pDC->SetBkMode(TRANSPARENT);//OPAQUE);
		if(strInfo.Find("V")>=0 || strInfo == "V")
			pDC->SetTextColor(COLOR_RED);
		else
			pDC->SetTextColor(COLOR_YELLOW);
		if(strInfo.Find("NG_FAIL")>=0 || strInfo == "NG_TIME")
			pDC->SetBkColor(COLOR_RED);
		else if(strInfo.Find("NG_BLOB")>=0 || strInfo.Find("NG_POS")>=0 || strInfo == "NG_WRONG")
			pDC->SetBkColor(COLOR_JOO);
		else if(strInfo.Find("NG_")>=0)
			pDC->SetBkColor(COLOR_GREEN);
		else
			pDC->SetBkColor(COLOR_WHITE);

		if(rt.left>2000 && strInfo.GetLength()>10)	//20131102
			p1.x -= 35;
		if(strInfo.Find("V")>=0 || strInfo == "V")
		{
			if(nFlag%2==0)
				pDC->TextOut(p2.x,p2.y,strInfo);
			else
				pDC->TextOut(p1.x-40,p2.y,strInfo);
		}
		else pDC->TextOut(p1.x,p1.y,strInfo);
	}

	pDC->SelectObject(pOldPen);
	DeleteObject(pen);
}

void CgDisplay::DrawXmark(CDC *pDC, CRect rt, COLORREF lColor, int nLineWidth,CString strInfo)
{

	CPen pen,*pOldPen;
	pen.CreatePen(PS_SOLID,nLineWidth, lColor);
	pOldPen = (CPen*)pDC->SelectObject(&pen);
	CPoint p1,p2;

	p1 = ScaleTrans(CPoint(rt.left,rt.top),1);
	p2 = ScaleTrans(CPoint(rt.right,rt.bottom),1);
	pDC->MoveTo(p1);
	pDC->LineTo(p2);
	p1 = ScaleTrans(CPoint(rt.right,rt.top),1);
	p2 = ScaleTrans(CPoint(rt.left,rt.bottom),1);
	pDC->MoveTo(p1);
	pDC->LineTo(p2);
	if(m_nZoom==1)
		pDC->TextOut(p1.x,p1.y,strInfo);

	pDC->SelectObject(pOldPen);
	DeleteObject(pen);
}

void CgDisplay::DrawCross(CDC *pDC, CRect rt, COLORREF lColor, int nLineWidth,CString strInfo)
{
	CPen pen,*pOldPen;
	pen.CreatePen(PS_SOLID,nLineWidth, lColor);
	pOldPen = (CPen*)pDC->SelectObject(&pen);
	CPoint p1,p2,cp(rt.CenterPoint());

	p1 = ScaleTrans(CPoint(cp.x,rt.top),1);
	p2 = ScaleTrans(CPoint(cp.x,rt.bottom),1);
	pDC->MoveTo(p1);
	pDC->LineTo(p2);
	p1 = ScaleTrans(CPoint(rt.left,cp.y),1);
	p2 = ScaleTrans(CPoint(rt.right,cp.y),1);
	pDC->MoveTo(p1);
	pDC->LineTo(p2);
	if(m_nZoom==1)
		pDC->TextOut(p1.x,p1.y,strInfo);

	pDC->SelectObject(pOldPen);
	DeleteObject(pen);
}

void CgDisplay::DrawCircle(CDC *pDC, CPoint ptC,int nR, COLORREF lColor, int nLineWidth,CString strInfo)
{
	CPen pen,*pOldPen;
	pen.CreatePen(PS_SOLID,nLineWidth, lColor);
	pOldPen = (CPen*)pDC->SelectObject(&pen);
	CPoint p1,p2;
	CRect rt;

	p1 = ScaleTrans(CPoint(ptC.x-nR,ptC.y-nR),1);
	p2 = ScaleTrans(CPoint(ptC.x+nR,ptC.y+nR),1);
	rt.SetRect(p1,p2);

	pDC->Arc(rt, ptC, ptC);

	if(m_nZoom==1)
		pDC->TextOut(rt.right,rt.CenterPoint().y,strInfo);

	pDC->SelectObject(pOldPen);
	DeleteObject(pen);
}
CPoint CgDisplay::CheckPt(CPoint ptCheck)
{
	if(ptCheck.x<m_rtDisplay.left)		ptCheck.x = m_rtDisplay.left;
	if(ptCheck.x>m_rtDisplay.right)		ptCheck.x = m_rtDisplay.right-2;
	if(ptCheck.y<m_rtDisplay.top)		ptCheck.y = m_rtDisplay.top+1;
	if(ptCheck.y>m_rtDisplay.bottom)	ptCheck.y = m_rtDisplay.bottom;

	return ptCheck;
}

void CgDisplay::LoadImg(CString strPath)
{
	m_pImg->Load(strPath);
	//m_pImgbuf->Load(strPath);
	//	memcpy(m_pImgbuf->GetImagePtr()+CAMERA_WIDTH*i, m_pImg->GetImagePtr()+CAMERA_WIDTH*(CAMERA_HEIGHT-i-1), CAMERA_WIDTH);

	//if(m_pImg->m_nImageHeight<950)
	//	return;
	memcpy(m_pImgbuf->GetImagePtr(), m_pImg->GetImagePtr(), CAMERA_WIDTH*CAMERA_HEIGHT);
	if (strPath.Find("bmp") >= 0) {
		for (int i=0;i<CAMERA_HEIGHT;i++)
			memcpy(m_pImg->GetImagePtr()+CAMERA_WIDTH*i, m_pImgbuf->GetImagePtr()+CAMERA_WIDTH*(CAMERA_HEIGHT-i-1), CAMERA_WIDTH);
	}
}
void CgDisplay::LoadImg(CString strPath,int nMode)
{
	m_pImg->Load(strPath);
	//CImg img;
	//img.Create(CAMERA_WIDTH,CAMERA_HEIGHT,8);
	//memcpy(img.GetImagePtr(), m_pImg->GetImagePtr(), CAMERA_WIDTH*CAMERA_HEIGHT );
	//img.InitDIB();
	//img.Save("C:\\TEST\\SAVE.Bmp");

	memcpy(m_pImgbuf->GetImagePtr(),m_pImg->GetImagePtr(),AI_IMAGE_WIDTH*AI_IMAGE_HEIGHT);
	for (int i=0;i<AI_IMAGE_HEIGHT;i++)
		memcpy(m_pImg->GetImagePtr()+AI_IMAGE_WIDTH*i, m_pImgbuf->GetImagePtr()+AI_IMAGE_WIDTH*(AI_IMAGE_HEIGHT-i-1), AI_IMAGE_WIDTH);

	//CImg img;
	//img.Create(AI_IMAGE_WIDTH,AI_IMAGE_HEIGHT,8);
	//memcpy(img.GetImagePtr(), m_pImg->GetImagePtr(), AI_IMAGE_WIDTH*AI_IMAGE_HEIGHT );
	//img.InitDIB();
	//img.Save("C:\\TEST\\SAVE.jpg");
	
}
void CgDisplay::SaveImgEvent(CString strInfo)
{
	m_strSaveInfo = strInfo;
	m_eSave.SetEvent();
}


CString CgDisplay::SaveImg2(CString strLot,CString strInfo)
{
	double dt11 = GetTickCount();
	CTime ttime = CTime::GetCurrentTime();
	CString kk = ttime.Format("%Y%m%d");
	CString kk2 = ttime.Format("%Y%m");
	CString kk3 = ttime.Format("%d");
	CString kk1 = ttime.Format("[%Y%m%d_%H%M%S]");
	CString str;

	if(m_strTimePath == "")
		m_strTimePath.Format("%s",kk1);
	//년월	일	m_strModelName	m_strInspStartTime

	str.Format("c:\\Data\\");
	CreateDirectory(str,NULL);
	str.Format("c:\\Data\\Image\\");
	CreateDirectory(str,NULL);
	str.Format("c:\\Data\\Image\\%s\\",kk2);//년월
	CreateDirectory(str,NULL);
	str.Format("c:\\Data\\Image\\%s\\%s\\",kk2,kk3);//일
	CreateDirectory(str,NULL);
	str.Format("c:\\Data\\Image\\%s\\%s\\%s\\",kk2,kk3,m_strTimePath);//년월/일/시간
	CreateDirectory(str,NULL);
	str.Format("c:\\Data\\Image\\%s\\%s\\%s\\DEFECT\\",kk2,kk3,m_strTimePath);//년월/일/시간
	CreateDirectory(str,NULL);
	str.Format("c:\\Data\\Image\\%s\\%s\\%s\\DEFECT\\%s_%s.bmp",kk2,kk3,m_strTimePath,kk1,strInfo);//일
	//	CreateDirectory(str,NULL);
	///추가


	int nh = m_pImg->GetHeight();
	//	for (int i=0;i<CAMERA_HEIGHT;i++)
	//		memcpy(m_pImgbuf->GetImagePtr()+CAMERA_WIDTH*i, m_pImg->GetImagePtr()+CAMERA_WIDTH*(CAMERA_HEIGHT-i-1), CAMERA_WIDTH);
	//	for(int k=0;k<MAX_CAMERAS;k++)
	{
		for (int i=0;i<nh;i++)
			memcpy(m_pImgbuf->GetImagePtr()+CAMERA_WIDTH*i, m_pImg->GetImagePtr()+CAMERA_WIDTH*(nh-i-1), CAMERA_WIDTH);

		m_pImgbuf->Save(str);

	}//m_pImg->Save(str);
	return "";
}

void CgDisplay::SaveDLImg(BOOL bRet, int nGrabCount, BOOL bPath)
{
	//CTime ttime = CTime::GetCurrentTime();
	//CString kk = ttime.Format("%Y%m%d");
	//CString kk2 = ttime.Format("%Y%m");
	//CString kk3 = ttime.Format("%d");
	//CString kk1 = ttime.Format("[%Y%m%d_%H%M%S]");
	//CString kk4 = ttime.Format("%H%M%S");
	//CString kk5 = ttime.Format("%H");

	//CString str;
	//str.Format("d:\\Data\\");
	//CreateDirectory(str, NULL);
	//str.Format("d:\\Data\\Image\\");
	//CreateDirectory(str, NULL);
	//str.Format("d:\\Data\\Image\\%s\\", kk2);//년월
	//CreateDirectory(str, NULL);
	//str.Format("d:\\Data\\Image\\%s\\%s\\", kk2, kk3);//일
	//CreateDirectory(str, NULL);
	//str.Format("d:\\Data\\Image\\%s\\%s\\%s\\", kk2, kk3, kk5);//time
	//CreateDirectory(str, NULL);
	//str.Format("d:\\Data\\Image\\%s\\%s\\%s\\%s_%s.jpg", kk2, kk3, kk5, kk1, strInfo);//time

	CString str;//,strpath;
	CTime ttime = CTime::GetCurrentTime();
	CString kk = ttime.Format("%Y%m%d");
	CString kk10 = ttime.Format("%Y");
	CString kk11 = ttime.Format("%m");
	CString kk2 = ttime.Format("%Y%m");
	CString kk3 = ttime.Format("%d");
	CString kk1 = ttime.Format("%y%m%d_%H%M%S");
	CString kk4 = ttime.Format("%H%M%S");
	CString kk5 = ttime.Format("%H");
	CString strMin = ttime.Format("%M");
	CString strResult;
	int nMin = atoi(strMin) / 10;

	str.Format("d:\\Data\\");
	CreateDirectory(str, NULL);
	str.Format("d:\\Data\\Image\\");
	CreateDirectory(str, NULL);
	str.Format("d:\\Data\\Image\\%s\\", kk10);//년
	CreateDirectory(str, NULL);
	str.Format("d:\\Data\\Image\\%s\\%s\\", kk10, kk11);//월
	CreateDirectory(str, NULL);
	str.Format("d:\\Data\\Image\\%s\\%s\\Overlay DL\\", kk10, kk11);
	CreateDirectory(str, NULL);

	if (bRet) {
		str.Format("d:\\Data\\Image\\%s\\%s\\Overlay DL\\OK\\", kk10, kk11); //ok or ng
		CreateDirectory(str, NULL);
		str.Format("d:\\Data\\Image\\%s\\%s\\Overlay DL\\OK\\%s\\", kk10, kk11, kk3); //일
		CreateDirectory(str, NULL);
		str.Format("d:\\Data\\Image\\%s\\%s\\Overlay DL\\OK\\%s\\%s\\", kk10, kk11, kk3, kk5); //시
		CreateDirectory(str, NULL);
		str.Format("d:\\Data\\Image\\%s\\%s\\Overlay DL\\OK\\%s\\%s\\%s_%d.jpg", kk10, kk11, kk3, kk5, kk1, nGrabCount); //시
	}
	else {
		str.Format("d:\\Data\\Image\\%s\\%s\\Overlay DL\\NG\\", kk10, kk11); //ok or ng
		CreateDirectory(str, NULL);
		str.Format("d:\\Data\\Image\\%s\\%s\\Overlay DL\\NG\\%s\\", kk10, kk11, kk3); //일
		CreateDirectory(str, NULL);
		str.Format("d:\\Data\\Image\\%s\\%s\\Overlay DL\\NG\\%s\\%s_%d.jpg", kk10, kk11, kk3, kk1, nGrabCount); //일
	}

	m_pImg->Save(str);
}

void CgDisplay::SaveImg(CString strInfo,BOOL bPath)
{
	//for (int i=0;i<CAMERA_HEIGHT;i++)
	//	memcpy(m_pImgbuf->GetImagePtr()+CAMERA_WIDTH*i, m_pImg->GetImagePtr()+CAMERA_WIDTH*(CAMERA_HEIGHT-i-1), CAMERA_WIDTH);
	//m_pImgbuf->Save(str);
	double dt11 = GetTickCount();
	CTime ttime = CTime::GetCurrentTime();
	CString kk = ttime.Format("%Y%m%d");
	CString kk2 = ttime.Format("%Y%m");
	CString kk3 = ttime.Format("%d");
	CString kk1 = ttime.Format("[%Y%m%d_%H%M%S]");
	CString kk4 = ttime.Format("%H%M%S");
	CString kk5 = ttime.Format("%H");

	CString str;

	//if(m_strTimePath == "")
	//	m_strTimePath.Format("C-%s",kk4);
	//년월	일	m_strModelName	m_strInspStartTime

	str.Format("d:\\Data\\");
	CreateDirectory(str,NULL);
	str.Format("d:\\Data\\Image\\");
	CreateDirectory(str,NULL);
	str.Format("d:\\Data\\Image\\%s\\",kk2);//년월
	CreateDirectory(str,NULL);
	str.Format("d:\\Data\\Image\\%s\\%s\\",kk2,kk3);//일
	CreateDirectory(str,NULL);
	str.Format("d:\\Data\\Image\\%s\\%s\\%s\\",kk2,kk3,kk5);//time
	CreateDirectory(str,NULL);
	str.Format("d:\\Data\\Image\\%s\\%s\\%s\\%s_%s.bmp",kk2,kk3,kk5,kk1,strInfo);//time
//	CreateDirectory(str,NULL);
	

	int nh = m_pImg->GetHeight();
	//	for (int i=0;i<CAMERA_HEIGHT;i++)
	//		memcpy(m_pImgbuf->GetImagePtr()+CAMERA_WIDTH*i, m_pImg->GetImagePtr()+CAMERA_WIDTH*(CAMERA_HEIGHT-i-1), CAMERA_WIDTH);
	//	for(int k=0;k<MAX_CAMERAS;k++)
	{
		for (int i=0;i<nh;i++)
			memcpy(m_pImgbuf->GetImagePtr()+CAMERA_WIDTH*i, m_pImg->GetImagePtr()+CAMERA_WIDTH*(nh-i-1), CAMERA_WIDTH);

		m_pImgbuf->Save(str);

	}//m_pImg->Save(str);

}

CString CgDisplay::SaveCropImg(CString strVisionType, SYSTEMTIME sysTime, CString strEqpID, CString strLotID, CString strCellID, BOOL bNG, int nLane, int nNgCode, int nPosX, int nPosY, BOOL bPath)
{
	//for (int i=0;i<CAMERA_HEIGHT;i++)
	//	memcpy(m_pImgbuf->GetImagePtr()+CAMERA_WIDTH*i, m_pImg->GetImagePtr()+CAMERA_WIDTH*(CAMERA_HEIGHT-i-1), CAMERA_WIDTH);
	//m_pImgbuf->Save(str);
	double dt11 = GetTickCount();

	CString strImageTime;
	//strImageTime.Format(_T("%04d%02d%02d%02d%02d%02d%03d"), sys_time.wYear, sys_time.wMonth, sys_time.wDay, sys_time.wHour, sys_time.wMinute, sys_time.wSecond, sys_time.wMilliseconds);
	strImageTime.Format(_T("%04d%02d%02d%02d%02d%02d%03d"), sysTime.wYear, sysTime.wMonth, sysTime.wDay, sysTime.wHour, sysTime.wMinute, sysTime.wSecond, sysTime.wMilliseconds);

	CString str;
	CString strRet;
	CString strFileName;
	CString strInsPos;
	CString strNgCode;

	//if(m_strTimePath == "")
	//	m_strTimePath.Format("C-%s",kk4);
	//년월	일	m_strModelName	m_strInspStartTime

	str.Format("d:\\Upload\\");
	CreateDirectory(str,NULL);

	if(nNgCode == NG_NOT_CAP){
		strInsPos.Format(_T("TOPCAP"));
		strNgCode.Format(_T("TCPMSS"));
		
	}
	else if(nNgCode == NG_NOT_CAN){
		strInsPos.Format(_T("TOPCAP"));
		strNgCode.Format(_T("CANMSS"));
	}
	else {
		strInsPos.Format(_T("TOPCAP"));
		strNgCode.Format(_T("CANMSS"));
	}

	strRet.Format(_T("OK"));
	if(bNG)
		strRet.Format(_T("NG"));

	strFileName.Format(_T("%s_%s_%s_%s_%s_SIDE_1_1_%s_%s_%s_1_%d_%d"),strVisionType, strImageTime, strEqpID, strLotID, strCellID, strInsPos, strRet, strNgCode, nPosX, nPosY);
	/*else
		strFileName.Format(_T("%s_%s_%s_%s_%s_SIDE_1_1_%s_OK_%s_1_%d_%d"),strVisionType, strImageTime, strEqpID, strLotID, strCellID, strInsPos, strNgCode, nPosX, nPosY);*/

	str.Format("d:\\Upload\\Insp\\%s.jpg", strFileName);//time

	int nh = m_pImg->GetHeight();
	for (int i=0;i<nh;i++)
		memcpy(m_pImgbuf->GetImagePtr()+NG_CROP_SIZE_X*i, m_pImg->GetImagePtr()+NG_CROP_SIZE_X*(nh-i-1), NG_CROP_SIZE_X);

	m_pImgbuf->Save(str);

	//m_pImg->Save(str);
	return strFileName;
}


void CgDisplay::SaveServerImg(CString strVisionType, CString strEqpID, int nLane, int nCam, int nFrameNum, BOOL bNG, CString strServerPath, SYSTEMTIME sysTime, CString strUploadPath, CString strCellId, CString strLotId, BOOL bPath)
{
	BOOL bIsBmp = false;
	CString strFileName;
	CString strPath;
	CString strCam;
	CTime tTime = CTime::GetCurrentTime();
	CString strYM = tTime.Format("%Y%m");
	CString strD = tTime.Format("%d");
	CString strH = tTime.Format("%H");
	CString strTime = tTime.Format("[%Y%m%d_%H%M%S]");
	CString strCameraLocation = _keyCameraLocation[2];
	CString strFinalJudge;
	CString strRet;
	int nJpegQual = 70;

	CString strImageTime;
	//strImageTime.Format(_T("%04d%02d%02d%02d%02d%02d%03d"), sys_time.wYear, sys_time.wMonth, sys_time.wDay, sys_time.wHour, sys_time.wMinute, sys_time.wSecond, sys_time.wMilliseconds);
	strImageTime.Format(_T("%04d%02d%02d%02d%02d%02d%03d"), sysTime.wYear, sysTime.wMonth, sysTime.wDay, sysTime.wHour, sysTime.wMinute, sysTime.wSecond, sysTime.wMilliseconds);

	if (bNG){
		strFinalJudge = _T("NG");
		strRet = "NG";
	}
	else{
		strFinalJudge = _T("OK");
		strRet = "OK";
	}
	strPath.Format(_T("%s\\%04d%02d\\"),strServerPath, sysTime.wYear, sysTime.wMonth);
	CreateDirectory(strPath,NULL);
	strPath.Format(_T("%s\\%04d%02d\\%02d"),strServerPath, sysTime.wYear, sysTime.wMonth, sysTime.wDay);
	CreateDirectory(strPath,NULL);
	strPath.Format(_T("%s\\%04d%02d\\%02d\\%02d\\"),strServerPath, sysTime.wYear, sysTime.wMonth, sysTime.wDay, sysTime.wHour);
	CreateDirectory(strPath,NULL);
	strPath.Format(_T("%s\\%04d%02d\\%02d\\%02d\\NA\\"),strServerPath, sysTime.wYear, sysTime.wMonth, sysTime.wDay, sysTime.wHour);
	CreateDirectory(strPath,NULL);

	strCam = _T("Cam1");
	strPath.Format(_T("d:\\Upload\\Insp\\"), strUploadPath);
	//strPath.Format(_T("%s\\"), strUploadPath);
	strFileName.Format(_T("%s_%d_%s_%s_%s_%s_SIDE_%d_%d_%s_%s"), strVisionType, nLane, strImageTime, strEqpID, strLotId, strCellId, nCam, nFrameNum, strRet, strFinalJudge);
	//strFileName.Format(_T("%s_%d_%s_%s_NA_NA_SIDE_%d_%d_%s_%s.jpg"), strVisionType, nLane, strImageTime, strEqpID, nCam, nFrameNum, strRet, strFinalJudge);
	//strFinalPath.Format(_T("%s%s.jpg"),strPath,strFileName);

	m_strImgName = strFileName;
	strPath += strFileName + ".jpg";
	int nh = m_pImg->GetHeight();

	/*for (int i=0;i<nh;i++)
		memcpy(m_pImgbuf->GetImagePtr()+CAMERA_WIDTH*i, m_pImg->GetImagePtr()+CAMERA_WIDTH*(nh-i-1), CAMERA_WIDTH);
*/
	//m_pImgbuf->Save(strPath);
	m_pImg->Save(strPath);

}


void CgDisplay::SaveImgNG(CString strInfo,BOOL bPath)
{
	//for (int i=0;i<CAMERA_HEIGHT;i++)
	//	memcpy(m_pImgbuf->GetImagePtr()+CAMERA_WIDTH*i, m_pImg->GetImagePtr()+CAMERA_WIDTH*(CAMERA_HEIGHT-i-1), CAMERA_WIDTH);
	//m_pImgbuf->Save(str);
	double dt11 = GetTickCount();
	CTime ttime = CTime::GetCurrentTime();
	CString kk = ttime.Format("%Y%m%d");
	CString kk2 = ttime.Format("%Y%m");
	CString kk3 = ttime.Format("%d");
	CString kk1 = ttime.Format("[%Y%m%d_%H%M%S]");
	CString kk4 = ttime.Format("%H%M%S");
	CString kk5 = ttime.Format("%H");

	CString str;

	//if(m_strTimePath == "")
	//	m_strTimePath.Format("C-%s",kk4);
	//년월	일	m_strModelName	m_strInspStartTime

	str.Format("d:\\Data\\");
	CreateDirectory(str,NULL);
	str.Format("d:\\Data\\Image\\");
	CreateDirectory(str,NULL);
	str.Format("d:\\Data\\Image\\%s\\",kk2);//년월
	CreateDirectory(str,NULL);
	str.Format("d:\\Data\\Image\\%s\\%s\\",kk2,kk3);//일
	CreateDirectory(str,NULL);
	str.Format("d:\\Data\\Image\\%s\\%s\\%s\\",kk2,kk3,kk5);//time
	CreateDirectory(str,NULL);
	str.Format("d:\\Data\\Image\\%s\\%s\\%s\\NG\\",kk2,kk3,kk5);//time
	CreateDirectory(str,NULL);
	str.Format("d:\\Data\\Image\\%s\\%s\\%s\\NG\\%s_%s.bmp",kk2,kk3,kk5,kk1,strInfo);//time
	//	CreateDirectory(str,NULL);


	int nh = m_pImg->GetHeight();
	//	for (int i=0;i<CAMERA_HEIGHT;i++)
	//		memcpy(m_pImgbuf->GetImagePtr()+CAMERA_WIDTH*i, m_pImg->GetImagePtr()+CAMERA_WIDTH*(CAMERA_HEIGHT-i-1), CAMERA_WIDTH);
	//	for(int k=0;k<MAX_CAMERAS;k++)
	{
		for (int i=0;i<nh;i++)
			memcpy(m_pImgbuf->GetImagePtr()+CAMERA_WIDTH*i, m_pImg->GetImagePtr()+CAMERA_WIDTH*(nh-i-1), CAMERA_WIDTH);

		m_pImgbuf->Save(str);

	}//m_pImg->Save(str);

}

void CgDisplay::SaveImgNG1(CString strInfo,BOOL bPath,BOOL bNas, BOOL bReverse)
{
	double dt11 = GetTickCount();
	CTime ttime = CTime::GetCurrentTime();
	CString kk = ttime.Format("%Y%m%d");
	CString kk2 = ttime.Format("%Y%m");
	CString kk3 = ttime.Format("%d");
	CString kk1 = ttime.Format("[%Y%m%d_%H%M%S]");
	CString kk4 = ttime.Format("%H%M%S");
	CString kk5 = ttime.Format("%H");
	CString str;

	str.Format("d:\\Data\\");
	CreateDirectory(str,NULL);
	str.Format("d:\\Data\\Image\\");
	CreateDirectory(str,NULL);
	str.Format("d:\\Data\\Image\\Gathering(NG)\\");
	CreateDirectory(str,NULL);
	str.Format("d:\\Data\\Image\\Gathering(NG)\\%s\\",kk2);//년월
	CreateDirectory(str,NULL);
	str.Format("d:\\Data\\Image\\Gathering(NG)\\%s\\%s\\",kk2,kk3);//일
	CreateDirectory(str,NULL);
	str.Format("d:\\Data\\Image\\Gathering(NG)\\%s\\%s\\%s_%s.jpg",kk2,kk3,kk1,strInfo);//time


	if(bReverse){
		int nh = m_pImg->GetHeight();
		for (int i=0;i<nh;i++)
			memcpy(m_pImgbuf->GetImagePtr()+CAMERA_WIDTH*i, m_pImg->GetImagePtr()+CAMERA_WIDTH*(nh-i-1), CAMERA_WIDTH);

		m_pImgbuf->Save(str);
	}
	else
		m_pImg->Save(str);
}


void CgDisplay::SaveTestImg(CString strInfo, CString userName)
{
	CString str;
	
	str.Format("c:\\Users\\" + userName + "\\Desktop\\Image Test\\");
	CreateDirectory(str, NULL);

	int k = strInfo.GetLength() - strInfo.Find("\[");

	CString strFileName = strInfo.Right(k);

	str.Format("c:\\Users\\" + userName + "\\Desktop\\Image Test\\%s", strFileName);

	int nh = m_pImg->GetHeight();

	for (int i=0;i<nh;i++)
		memcpy(m_pImgbuf->GetImagePtr()+CAMERA_WIDTH*i, m_pImg->GetImagePtr()+CAMERA_WIDTH*(nh-i-1), CAMERA_WIDTH);

	m_pImgbuf->Save(str);
}

void CgDisplay::FullSave(CString strInfo, CString userName)
{
	CString str,str1;
	str1 = strInfo.Left(8);
	str.Format("c:\\Glim\\TesImg\\");
	CreateDirectory(str, NULL);
	str.Format("c:\\Glim\\TesImg\\%s\\",str1);//년월일시분
	CreateDirectory(str, NULL);
	str.Format("c:\\Glim\\TesImg\\%s\\%s\\",str1,strInfo);//년월일시분
	CreateDirectory(str, NULL);


	str.Format("c:\\Glim\\TesImg\\%s\\%s\\%s",str1, strInfo,userName);

	int nh = m_pImg->GetHeight();

	for (int i=0;i<nh;i++)
		memcpy(m_pImgbuf->GetImagePtr()+CAMERA_WIDTH*i, m_pImg->GetImagePtr()+CAMERA_WIDTH*(nh-i-1), CAMERA_WIDTH);

	m_pImgbuf->Save(str);
}

void CgDisplay::FullSave1(CString strInfo, CString userName, int nType)
{
	CString str,str1;
	if (nType == AI_MODE)
	{
		CTime ttime = CTime::GetCurrentTime();
		CString kk1 = ttime.Format("[%Y%m%d_%H%M%S]");
		str.Format("c:\\AI_InImage\\");
		CreateDirectory(str, NULL);
		str.Format("c:\\AI_InImage\\%s%s.bmp",kk1,userName);//년월일시분
		m_strAIName=kk1+userName;
	}
	else
	{
		CTime ttime = CTime::GetCurrentTime();
		CString kk1 = ttime.Format("%Y");
		CString kk2 = ttime.Format("%m");
		CString kk3 = ttime.Format("%d");
		CString kk4 = ttime.Format("%H");
		CString kk5 = ttime.Format("%Y%m%d_%H%M%S");



		str.Format("D:\\Image\\");
		CreateDirectory(str, NULL);
		str.Format("D:\\Image\\%s",kk1);
		CreateDirectory(str, NULL);
		str.Format("D:\\Image\\%s\\%s",kk1,kk2);
		CreateDirectory(str, NULL);
		str.Format("D:\\Image\\%s\\%s\\%s",kk1,kk2,kk3);
		CreateDirectory(str, NULL);
		str.Format("D:\\Image\\%s\\%s\\%s\\%s",kk1,kk2,kk3,kk4);
		CreateDirectory(str, NULL);
		if (nType == RET_OK)
		{
			str.Format("D:\\Image\\%s\\%s\\%s\\%s\\%s",kk1,kk2,kk3,kk4,strInfo);
			CreateDirectory(str, NULL);
			str.Format("D:\\Image\\%s\\%s\\%s\\%s\\%s\\%s.jpg",kk1,kk2,kk3,kk4,strInfo,userName);
		}		
		else
		{
			str.Format("D:\\Image\\%s\\%s\\%s\\%s\\NG",kk1,kk2,kk3,kk4);
			CreateDirectory(str, NULL);
			str.Format("D:\\Image\\%s\\%s\\%s\\%s\\NG\\%s",kk1,kk2,kk3,kk4,strInfo);
			CreateDirectory(str, NULL);
			str.Format("D:\\Image\\%s\\%s\\%s\\%s\\NG\\%s\\%s.jpg",kk1,kk2,kk3,kk4,strInfo,userName);
		}
		
	}
	int nh = m_pImg->GetHeight();

	BYTE * fmtmp = new BYTE[CAMERA_WIDTH*CAMERA_HEIGHT];
	for (int i=0;i<nh;i++)
		memcpy(fmtmp+CAMERA_WIDTH*i ,m_pImgbuf->GetImagePtr()+CAMERA_WIDTH*(nh-i-1), CAMERA_WIDTH);

	m_pImgbuf->Save(str);
	delete fmtmp;
}
CPoint CgDisplay::ScaleTrans(CPoint point,int direction)
{
	CPoint point_original(point);
	CPoint point_trans;

	int width = m_rtDisplay.Width();
	int height = m_rtDisplay.Height();
	int nw = m_rtZoom.Width();
	int nh = m_rtZoom.Height();

	if(direction == 0) //화면-> 이미지 좌표로 변환
	{
		point_original.x -= m_rtDisplay.left;
		point_original.y -= m_rtDisplay.top;
		float dSfx = (float)(nw)/(float)(width);
		float dSfy = (float)(nh)/(float)(height);

		point_trans.x = m_rtZoom.left + int(dSfx * point_original.x);
		point_trans.y = m_rtZoom.top  + int(dSfy * point_original.y);
	}
	else if(direction == 1)//이미지 ->화면 좌표로 변환
	{
		point_trans.x = m_rtDisplay.left + int(((float)(width) / (float)(nw)) * (float)(point_original.x-m_rtZoom.left));
		point_trans.y = m_rtDisplay.top + int(((float)(height) / (float)(nh)) * (float)(point_original.y-m_rtZoom.top));
		point_trans = CheckPt(point_trans);
	}
	else
	{
		point_trans.x = int(((float)(width) / (float)(nw)) * (float)(point_original.x));
		point_trans.y = (point_original.y);
	}
	return point_trans;
}

void CgDisplay::DrawBMP(LPCTSTR lpszFileName, CDC *pDC, int nX, int nY, DWORD dwRop)
{
    CDC         memDC;
    CBitmap     bmp, *pOldBmp;
    HBITMAP     hBMP;
    BITMAP      bm;

    hBMP = (HBITMAP)LoadImage(NULL, lpszFileName, 
        IMAGE_BITMAP, 0, 0, 
        LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE);

	if(hBMP==NULL)
		return;
    bmp.Attach(hBMP);
	bmp.GetBitmap(&bm);
    memDC.CreateCompatibleDC(pDC);
    pOldBmp = memDC.SelectObject(&bmp);
    pDC->BitBlt(nX, nY, bm.bmWidth, bm.bmHeight, &memDC, 0, 0, dwRop);
    memDC.SelectObject(pOldBmp);

	memDC.DeleteDC();
	bmp.DeleteObject();	
}


void CgDisplay::DrawText(CDC *pDC, CPoint pt, CString str,int bkmode,COLORREF lColor,COLORREF lbkColor)
{
// 	CFont font,*pOldFont;
// 	font.CreatePointFont(200,_T("굴림")); 
// 	pOldFont=(CFont*)pDC->SelectObject(&font);
// 	font.DeleteObject();
// 	pOldFont->DeleteObject();

	CPoint p1;
	p1 = ScaleTrans(pt,1);
	if(bkmode==1)
		pDC->SetBkMode(TRANSPARENT);
	else
		pDC->SetBkMode(OPAQUE),pDC->SetBkColor(lbkColor);
	pDC->SetTextColor(lColor);
	if(m_nZoom==1)
		pDC->TextOut(p1.x,p1.y,str);
	

	
}
BOOL CgDisplay::TrackerSetCursor(CWnd* pWnd, UINT nHitTest)
{
	if(m_bTrackerUse)
		for(int i=0;i<MAX_TRACKER;i++)
			if ( m_Tracker[i].SetCursor(pWnd, nHitTest))
				return TRUE;
	return FALSE;
}

void CgDisplay::TrackerCtrl(CWnd* pWnd,CPoint point)
{
	if(m_bTrackerUse==FALSE)
		return;
	if(m_nZoom!=1)
		return;
	int no = -1;
	for(int i=0;i<MAX_TRACKER;i++)
		if(m_Tracker[i].m_rect.PtInRect(point))
			no = i;

	if(no==-1)
		return ;

	OldRect=m_Tracker[no].m_rect;
	m_Tracker[no].GetTrueRect(&OldTrueRect);

	if(m_Tracker[no].HitTest(point) < 0)
	{
		if(m_Tracker[no].TrackRubberBand(pWnd, point))
		{
			m_Tracker[no].m_rect.NormalizeRect();
			m_Tracker[no].m_rect = OldRect;
		}
		else
		{
			pWnd->InvalidateRect(OldTrueRect);
			m_Tracker[no].m_nStyle=0;	
			m_Tracker[no].m_rect=OldRect;
		}
	}
	else if(m_Tracker[no].Track(pWnd, point, FALSE))
	{
		pWnd->InvalidateRect(&OldTrueRect);
		if(OldTrueRect.Width()<=2)
			OldTrueRect = OldRect;
		pWnd->InvalidateRect(&newRect);
		if(newRect.Width()<=2)
			newRect = OldRect;
		m_Tracker[no].GetTrueRect(&newRect);
		if(newRect.Width()<=2)
			newRect = OldRect;
		m_rtImgTracker[no].SetRect(ScaleTrans(m_Tracker[no].m_rect.TopLeft(),0),ScaleTrans(m_Tracker[no].m_rect.BottomRight(),0));

		////jb
		//m_Tracker[no] = m_rtImgTracker[no];
	}
	else{
		m_Tracker[no].m_nStyle = CRectTracker::solidLine;//|CRectTracker::resizeInside;
		pWnd->InvalidateRect(&OldTrueRect);
		if(OldTrueRect.Width()<=2)
			OldTrueRect = OldRect;
		m_Tracker[no].GetTrueRect(&OldTrueRect);
	}

	if(no==3||no==4)
	{
		int n1,n2;
		if(no==3)n1 = 3,n2=4;
		else	n1=4,n2=3;
		m_Tracker[n2].m_rect.top = m_Tracker[n1].m_rect.top;
		m_Tracker[n2].m_rect.bottom = m_Tracker[n1].m_rect.bottom;
		m_rtImgTracker[n2].top = m_rtImgTracker[n1].top;
		m_rtImgTracker[n2].bottom = m_rtImgTracker[n1].bottom;
	}


	if(m_Tracker[no].m_rect.left<0)m_Tracker[no].m_rect.left = 0;
	if(m_Tracker[no].m_rect.top<0)m_Tracker[no].m_rect.top = 0;
	if(m_Tracker[no].m_rect.right>CAM_WIDTH)m_Tracker[no].m_rect.right = CAM_WIDTH;
	if(m_Tracker[no].m_rect.bottom>CAMERA_HEIGHT)m_Tracker[no].m_rect.bottom = CAMERA_HEIGHT;
}

void CgDisplay::SaveJpgImg(CString strInfo,CString strdate,int ncnt)
{
	double dt11 = GetTickCount();
	CTime ttime = CTime::GetCurrentTime();
	CString kk = ttime.Format("%Y%m%d");
	CString kk2 = ttime.Format("%Y%m");
	CString kk3 = ttime.Format("%d");
	CString kk1 = ttime.Format("[%Y%m%d_%H%M%S]");
	CString kk4 = ttime.Format("%H%M%S");
	CString kk5 = ttime.Format("%H");

	CString str;

	//if(m_strTimePath == "")
	//	m_strTimePath.Format("C-%s",kk4);
	//년월	일	m_strModelName	m_strInspStartTime

	str.Format("d:\\Data\\");
	CreateDirectory(str,NULL);
	str.Format("d:\\Data\\Image\\");
	CreateDirectory(str,NULL);
	str.Format("d:\\Data\\Image\\%s\\",kk2);//년월
	CreateDirectory(str,NULL);
	str.Format("d:\\Data\\Image\\%s\\%s\\",kk2,kk3);//일
	CreateDirectory(str,NULL);
	str.Format("d:\\Data\\Image\\%s\\%s\\%s\\",kk2,kk3,kk5);//time
	CreateDirectory(str,NULL);
	str.Format("d:\\Data\\Image\\%s\\%s\\%s\\%s_%s.jpg",kk2,kk3,kk5,kk1,strInfo);//time
	//	CreateDirectory(str,NULL);

	int nh = m_pImg->GetHeight();
	//	for (int i=0;i<CAMERA_HEIGHT;i++)
	//		memcpy(m_pImgbuf->GetImagePtr()+CAMERA_WIDTH*i, m_pImg->GetImagePtr()+CAMERA_WIDTH*(CAMERA_HEIGHT-i-1), CAMERA_WIDTH);
	//	for(int k=0;k<MAX_CAMERAS;k++)
	{
		for (int i=0;i<nh;i++)
			memcpy(m_pImgbuf->GetImagePtr()+CAMERA_WIDTH*i, m_pImg->GetImagePtr()+CAMERA_WIDTH*(nh-i-1), CAMERA_WIDTH);

		m_pImgbuf->Save(str);

	}
	double dt12 = GetTickCount()-dt11;

	//	CString str;
	str.Format("%.0f",dt12);
	//	AfxMessageBox(str);
}


void CgDisplay::SaveJpgImgBadImage(CString strInfo)
{
	double dt11 = GetTickCount();
	CTime ttime = CTime::GetCurrentTime();
	CString kk = ttime.Format("%Y%m%d");
	CString kk2 = ttime.Format("%Y%m");
	CString kk3 = ttime.Format("%d");
	CString kk1 = ttime.Format("[%Y%m%d_%H%M%S]");
	CString kk5 = ttime.Format("%H");

	CString str;

	str.Format("d:\\Data\\");
	CreateDirectory(str,NULL);
	str.Format("d:\\Data\\Image\\");
	CreateDirectory(str,NULL);
	str.Format("d:\\Data\\Image\\%s\\",kk2);//년월
	CreateDirectory(str,NULL);
	str.Format("d:\\Data\\Image\\%s\\%s\\",kk2,kk3);//일
	CreateDirectory(str,NULL);
	str.Format("d:\\Data\\Image\\%s\\%s\\%s\\",kk2,kk3,kk5);//time
	CreateDirectory(str,NULL);
	///추가
	str.Format("d:\\Data\\Image\\%s\\%s\\%s\\BadImage\\",kk2,kk3,kk5);//시작시간
	CreateDirectory(str,NULL);
	str.Format("d:\\Data\\Image\\%s\\%s\\%s\\BadImage\\%s_%s.jpg",kk2,kk3,kk5,kk1,strInfo);//제품lot//이미지
	//	CreateDirectory(str,NULL);



	int nh = m_pImg->GetHeight();
	//	for (int i=0;i<CAMERA_HEIGHT;i++)
	//		memcpy(m_pImgbuf->GetImagePtr()+CAMERA_WIDTH*i, m_pImg->GetImagePtr()+CAMERA_WIDTH*(CAMERA_HEIGHT-i-1), CAMERA_WIDTH);
	//	for(int k=0;k<MAX_CAMERAS;k++)
	{
		for (int i=0;i<nh;i++)
			memcpy(m_pImgbuf->GetImagePtr()+300*i, m_pImg->GetImagePtr()+300*(nh-i-1), 300);

		m_pImgbuf->Save(str);

	}
}