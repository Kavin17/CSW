// DrawMonitor.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "FlyCapture2_EX.h"
#include "DrawBadState.h"


// DrawBadState 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDrawBadState, CDialog)

CDrawBadState::CDrawBadState(CWnd* pParent /*=NULL*/)
	: CDialog(CDrawBadState::IDD, pParent)
{
	m_bRedrawTime = FALSE;
	m_ptrTotalCnt = NULL;
	m_ptrBadCnt = NULL;
	m_ptrBadCntBar = NULL;
	m_nGroupCnt = 0;
	m_nDataCnt = 0;
	m_nMaxUnit = 0;
	nLableWidth = 40;
	nLableHeight = 40;
	m_bStrNameInit = FALSE;
	m_nTotal = 0;
}

CDrawBadState::~CDrawBadState()
{
}

void CDrawBadState::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDrawBadState, CDialog)
	ON_BN_CLICKED(IDOK, &CDrawBadState::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDrawBadState::OnBnClickedCancel)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CDrawBadState 메시지 처리기입니다.

void CDrawBadState::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//OnOK();
}

void CDrawBadState::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//OnCancel();
}
void CDrawBadState::SetGraphType(int nVal)
{
	m_nGraghType = nVal;
}

void CDrawBadState::SetRow(int nVal)
{
	nRow = nVal;
}
void CDrawBadState::SetCol(int nVal)
{
	nCol = nVal;
}

void CDrawBadState::SetDrawEanble(BOOL isSet)
{
	m_bRedrawTime = isSet;
}

BOOL CDrawBadState::GetDrawEnable()
{
	return m_bRedrawTime;
}

void CDrawBadState::SetGroupCnt(int nGroupCnt, int nDataCnt)
{
	m_nGroupCnt = nGroupCnt;
	m_nDataCnt = nDataCnt;
	DeAllocCntMem();
	AllocCntMem(nGroupCnt, nDataCnt);

	int nNewCol = 0;
	if(m_nGraghType == 1){
		nNewCol = nGroupCnt * nDataCnt + nGroupCnt + 1;
		SetCol(nNewCol);
		
	}
	
}

void CDrawBadState::SetMaxUnit(int nMaxUnit)
{
	m_nMaxUnit = nMaxUnit;
	
}

void CDrawBadState::SetLableWH(int nW, int nH)
{
	nLableWidth = nW;
	nLableHeight = nH;
}

void CDrawBadState::InitAreaFit()
{
	int nOffsetX = 0;
	int nOffsetY = 0;

	nOffsetX = m_rtArea.Width() % nCol;
	nOffsetY = m_rtArea.Height() % nRow;

	if(nOffsetX > 1){
	}
	else{
		nOffsetX = 0;
	}

	if(nOffsetY > 1){
	}
	else{
		nOffsetY = 0;
	}
	//m_rtAreaWithLable = m_rtArea;
	m_rtAreaWithLable.SetRect(nLableWidth + nOffsetX, 0, m_rtArea.Width(), m_rtArea.Height() - nLableHeight - nOffsetY);

}


void CDrawBadState::AllocCntMem(int nGroupCnt, int nDataCnt)
{
	if(m_ptrTotalCnt == NULL){
		m_ptrTotalCnt = new float*[nGroupCnt];
		for(int i=0; i<m_nGroupCnt; i++){
			m_ptrTotalCnt[i] = new float[nDataCnt];
		}
	}

	if(m_ptrBadCnt == NULL){
		m_ptrBadCnt = new float*[nGroupCnt];
		for(int i=0; i<m_nGroupCnt; i++){
			m_ptrBadCnt[i] = new float[nDataCnt];
		}
	}

	if(m_nGraghType == 1){
		if(m_bStrNameInit == FALSE){
			for(int i=0; i<m_nGroupCnt; i++){
				m_ptrStrGroupName[i] = new CString();
			}
		}
	}

	ResetCntMem();
}

void CDrawBadState::DeAllocCntMem()
{
	if(m_ptrTotalCnt != NULL){
		for(int i=0; i<m_nGroupCnt; i++){
			delete[] m_ptrTotalCnt[i];
		}
		m_ptrTotalCnt = NULL;
	}
	if(m_ptrBadCnt != NULL){
		for(int i=0; i<m_nGroupCnt; i++){
			delete[] m_ptrBadCnt[i];
		}
		delete[] m_ptrBadCnt;
		m_ptrBadCnt = NULL;
	}
	if(m_nGraghType == 1){
		if(m_bStrNameInit == TRUE){
			for(int i=0; i<m_nGroupCnt; i++){
				//m_ptrStrGroupName[i]->Delete();
				delete[] m_ptrStrGroupName[i];
			}
			m_bStrNameInit = FALSE;
			
		}
	}
}

void CDrawBadState::ResetCntMem()
{
	for(int i=0; i<m_nGroupCnt; i++){
		for(int ii=0; ii<m_nDataCnt; ii++){
			m_ptrTotalCnt[i][ii] = 0;
			m_ptrBadCnt[i][ii] = 0;
		}
	}

	if(m_nGraghType == 1){
		for(int i=0; i<m_nGroupCnt; i++){
			m_ptrStrGroupName[i]->Empty();
		}
	}
}

void CDrawBadState::SetGroupLableTest()
{
	CString str[10];

	str[0].Format("TAB");
	str[1].Format("HOLE");
	str[2].Format("BLUR");
	str[3].Format("DIST");
// 	str[4].Format("불량D");
// 	str[5].Format("불량D");

	for(int i=0; i<m_nGroupCnt; i++){
		SetGroupLable(i, str[i]);
	}

}

void CDrawBadState::SetGroupLable(int nIndex, CString str)
{
	m_ptrStrGroupName[nIndex]->Format("%s", str);
}


void CDrawBadState::SetData(int nCam, int nHour, int nValue)
{
	// 	float nData = 5;
	// 	float nData2[5][5];
	// 	for(int i=0; i<m_nGroupCnt; i++){
	// 		for(int ii=0; ii<m_nDataCnt; ii++){
	// 			nData2[i][ii] = 0.0;
	// 		}
	// 	}
	//mDrawBadState[0]->SetData(int nCam, int nHour, int nValue);
	m_ptrBadCnt[nCam][nHour] = nValue;




}

void CDrawBadState::SetDataTest()
{
	float nData[3][25];

	for(int i=0; i<m_nGroupCnt; i++){
		for(int ii=0; ii<m_nDataCnt; ii++){
			nData[i][ii] = 0.0;
		}
	}

	nData[0][0] = 5.0;
	nData[0][1] = 2.4;
	nData[0][2] = 1.8;
	nData[0][3] = 1.7;
	nData[0][4] = 1.7;
	nData[0][5] = 1.4;
	nData[0][6] = 2.4;
	nData[0][7] = 2.2;
	nData[0][8] = 1.3;
	nData[0][9] = 1.6;
	nData[0][10] = 2.1;
	nData[0][11] = 1.4;
	nData[0][12] = 1.8;
	nData[0][13] = 2.7;
	nData[0][14] = 1.7;
	nData[0][15] = 1.4;
	nData[0][16] = 1.4;
	nData[0][17] = 1.2;
	nData[0][18] = 1.3;
	nData[0][19] = 1.6;

	nData[1][0] = 4.0;
	nData[1][1] = 2.1;
	nData[1][2] = 1.2;
	nData[1][3] = 2.3;
	nData[1][4] = 2.1;
	nData[1][5] = 2.1;
	nData[1][6] = 2.9;
	nData[1][7] = 4.3;
	nData[1][8] = 2.1;
	nData[1][9] = 1.6;
	nData[1][10] = 1.6;
	nData[1][11] = 2.1;
	nData[1][12] = 1.2;
	nData[1][13] = 1.3;
	nData[1][14] = 2.1;
	nData[1][15] = 2.1;
	nData[1][16] = 1.9;
	nData[1][17] = 1.3;
	nData[1][18] = 2.1;
	nData[1][19] = 1.6;

	nData[2][0] = 1.8;
	nData[2][1] = 2.2;
	nData[2][2] = 1.5;
	nData[2][3] = 1.3;
	nData[2][4] = 1.5;
	nData[2][5] = 1.4;
	nData[2][6] = 1.4;
	nData[2][7] = 2.2;
	nData[2][8] = 1.3;
	nData[2][9] = 1.9;
	nData[2][10] = 1.8;
	nData[2][11] = 1.4;
	nData[2][12] = 1.8;
	nData[2][13] = 0.7;
	nData[2][14] = 1.7;
	nData[2][15] = 1.4;
	nData[2][16] = 0.4;
	nData[2][17] = 1.2;
	nData[2][18] = 1.3;
	nData[2][19] = 1.9;



	if(m_nTotal==1){
		double dsum = 0;
		for(int i=0; i<m_nGroupCnt; i++){
			 dsum = 0;
			for(int ii=0; ii<m_nDataCnt; ii++){
				dsum += nData[i][ii];
				m_ptrBadCnt[i][ii] = dsum/(ii+1);
//				m_ptrBadCnt[i][ii] = nData[i][ii];
				//m_ptrBadCnt[i][ii] = nData2;
				//nData2-=0.05;
			}
		}
	}
	else{
		for(int i=0; i<m_nGroupCnt; i++){
			for(int ii=0; ii<m_nDataCnt; ii++){
				m_ptrBadCnt[i][ii] = nData[i][ii];
				//m_ptrBadCnt[i][ii] = nData2;
				//nData2-=0.05;
			}
		}
	}

}
	
void CDrawBadState::SetDataTest2()
{
	//float nData = 5;
	float nData2[5][5];
// 	for(int i=0; i<m_nGroupCnt; i++){
// 		for(int ii=0; ii<m_nDataCnt; ii++){
// 			nData2[i][ii] = 0.0;
// 		}
// 	}
	
	
			nData2[0][0] = 2;
			nData2[0][1] = 7;
			nData2[0][2] = 3;
			nData2[0][3] = 2;
		
			nData2[1][0] = 6;
			nData2[1][1] = 4;
			nData2[1][2] = 6;
			nData2[1][3] = 3;

			nData2[2][0] = 1;
			nData2[2][1] = 7;
			nData2[2][2] = 4;
			nData2[2][3] = 3;

			nData2[3][0] = 2;
			nData2[3][1] = 3;
			nData2[3][2] = 6;
			nData2[3][3] = 7;

			nData2[4][0] = 4;
			nData2[4][1] = 3;
			nData2[4][2] = 1;
			nData2[4][3] = 2;
 		
	
	
 	for(int i=0; i<m_nGroupCnt; i++){
 		for(int ii=0; ii<m_nDataCnt; ii++){
 			//m_ptrTotalCnt[i][ii] = nData;
 			m_ptrBadCnt[i][ii] = nData2[i][ii];
 			//nData2-=0.05;
 		}
 	}
}


void CDrawBadState::RedrawMap()
{
	if(GetDrawEnable() == TRUE){
		CDC *pDC = GetDC();

		InitAreaFit();

		if(m_nGraghType == 0){
			//graph 1
			DrawGridLineH(pDC);
			DrawGridLineV(pDC);
			DrawLableG1H(pDC);
			DrawLableG1V(pDC);
//			SetDataTest();	//test
			DrawDataLine(pDC);
		}
		else if(m_nGraghType == 1){
			//graph2
			DrawGridLineH(pDC);
			DrawGridLineV(pDC);
//			SetDataTest2();
			SetGroupLableTest();
			DrawLableG1V(pDC);
			DrawLableG2H(pDC);
			DrawDataBar(pDC);
		}
		ReleaseDC(pDC);
	}	
}


void CDrawBadState::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	// 그리기 메시지에 대해서는 CDialog::OnPaint()을(를) 호출하지 마십시오.

	//CDC* pDc = GetDC();

	CString strPath;
	strPath.Format("%s%s", "C:\\GLIM\\","DefectMapBackGround.bmp");
	DrawBMP(strPath, &dc, 0, 0, SRCCOPY);	

	RedrawMap();

	//ReleaseDC(pDc);
}

void CDrawBadState::DrawBMP(LPCTSTR lpszFileName, CDC *pDC, int nX, int nY, DWORD dwRop)
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

void CDrawBadState::CreateGraph(int nType, int nRow, int nCol, int nMaxUnit)
{

	SetGraphType(nType);
	SetRow(nRow);
	SetCol(nCol);
	SetMaxUnit(nMaxUnit);
}

void CDrawBadState::CreatePen(CDC* pDC, int lType, int lThick, int lColor)
{
	mPen.CreatePen( lType, lThick, lColor );    
	mBrush.CreateSolidBrush( lColor ); 
	oldPen = pDC->SelectObject( &mPen );
	oldBrush = pDC->SelectObject( &mBrush );
}

void CDrawBadState::ReleasePen(CDC* pDC)
{
	pDC->SelectObject( oldPen );
	pDC->SelectObject( oldBrush );
	mPen.DeleteObject();
	mBrush.DeleteObject();

}
void CDrawBadState::DrawGridLineH(CDC *pDC)
{
	int width = m_rtAreaWithLable.Width();
	int height = m_rtAreaWithLable.Height();

	CString str;
	
	CPoint pos;

	CreatePen(pDC, PS_DOT, 1, GRAY);

	int nDivV = height / nRow;

	for(int i=1; i<=nRow; i++){
		pos.x = m_rtAreaWithLable.left;
		pos.y = nDivV*i;
		pDC->MoveTo(pos.x, pos.y);
		pos.x = m_rtAreaWithLable.right;
		pDC->LineTo(pos.x, pos.y);
	}

	ReleasePen(pDC);

}

void CDrawBadState::DrawGridLineV(CDC *pDC)
{
	int width = m_rtAreaWithLable.Width();
	int height = m_rtAreaWithLable.Height();

	CString str;

	CPoint pos;

	CreatePen(pDC, PS_DOT, 1, GRAY);

	int nDivH = width / nCol;

	for(int i=0; i<=nCol; i++){
		if(i==0){
			pos.x = 0+m_rtAreaWithLable.left;
		}
		else{
			pos.x = nDivH*i+m_rtAreaWithLable.left;
		}
		
		pos.y = 0;
		pDC->MoveTo(pos.x, pos.y);
		pos.y = height;// + nLableHeight;
		pDC->LineTo(pos.x, pos.y);
	}

	ReleasePen(pDC);
}

int CDrawBadState::GetConvertYPos(int nPos)
{
	int nConvertPosY = m_rtAreaWithLable.Height() - nPos;

	return nConvertPosY;
}

void CDrawBadState::DrawDataBar(CDC *pDC)
{
	int width = m_rtAreaWithLable.Width();
	int height = m_rtAreaWithLable.Height();

	CString str;
	CPoint currPos, oldPos;

	float nDivV = float(height) / float(nRow);	//scale
	float nScale = float(m_nMaxUnit) / float(nRow);
	int nDivH = width / nCol;
	CRect rtBar;
	int nPosX = 0;

	for(int i=0; i<m_nGroupCnt; i++){	//defectType
		nPosX++;
		for(int ii=0; ii<m_nDataCnt; ii++){	//cam
			if(ii==0){
				CreatePen(pDC, PS_SOLID, 2, MAGENTA_N);
			}
			else if(ii==1){
				CreatePen(pDC, PS_SOLID, 2, BLUE_N);
			}
			else if(ii==2){
				CreatePen(pDC, PS_SOLID, 2, GREEN_N);
			}
			else{
				CreatePen(pDC, PS_SOLID, 2, RED_N);
			}

			int nValue = (float)m_ptrBadCnt[i][ii];
			currPos.y = nValue / nScale * nDivV;
			currPos.x = nDivH * nPosX + m_rtAreaWithLable.left;
			
			rtBar.SetRect(currPos.x, GetConvertYPos(0), currPos.x + nDivH, GetConvertYPos(currPos.y));

			pDC->Rectangle(rtBar);

			nPosX++;

			ReleasePen(pDC);
		}	
		
	}	

}

void CDrawBadState::DrawLableG1V(CDC *pDC)
{
	int width = m_rtAreaWithLable.Width();
	int height = m_rtAreaWithLable.Height();

	CString str;
	CPoint currPos;

	int nOffsetX = 10;
	int nOffsetY = 8;

	int nDivH = height / nRow;
	float nUnit = float(m_nMaxUnit) / float(nRow);
	
	CreatePen(pDC, PS_SOLID, 2, BLACK);

	for(int ii=0; ii<=nRow; ii++){
		currPos.y = height - (nDivH * ii) - nOffsetY;
		currPos.x = nOffsetX;
		str.Format("%0.1f", nUnit * ii);
		pDC->TextOut(currPos.x, currPos.y, str);
	}	

	ReleasePen(pDC);
}

void CDrawBadState::DrawLableG1H(CDC *pDC)
{
	int width = m_rtAreaWithLable.Width();
	int height = m_rtAreaWithLable.Height();

	CString str;
	CPoint currPos;
	int nHour = 0;

	int nOffsetX = 10;

	float nDivV = float(height) / float(nRow);	//scale
	float nScale = float(m_nMaxUnit) / float(nRow);
	int nDivH = width / nCol;

	CreatePen(pDC, PS_SOLID, 2, BLACK);

	for(int ii=0; ii<m_nDataCnt; ii++){
		currPos.y = height + (nLableHeight * 0.5);
		currPos.x = nDivH * ii + m_rtAreaWithLable.left - (nDivH*0.5) + nOffsetX;
		str.Format("%02d", nHour);
		pDC->TextOut(currPos.x, currPos.y, str);
		nHour++;
	}	
	
	ReleasePen(pDC);
}

//그룹이름
void CDrawBadState::DrawLableG2H(CDC *pDC)
{
	int width = m_rtAreaWithLable.Width();
	int height = m_rtAreaWithLable.Height();

	CString str;
	CPoint currPos;
	int nHour = 0;

	int nOffsetX = 100;

	float nDivV = float(height) / float(nRow);	//scale
	float nScale = float(m_nMaxUnit) / float(nRow);
	int nDivH = width / nCol;

	nBarWidthCnt = m_nGroupCnt * m_nDataCnt;

	int nGroupAreaDist = (nBarWidthCnt + m_nGroupCnt) / m_nGroupCnt * nDivH ;

	CreatePen(pDC, PS_SOLID, 2, BLACK);

	for(int ii=0; ii<m_nGroupCnt; ii++){
		currPos.y = height + (nLableHeight * 0.5);
		currPos.x = nGroupAreaDist * ii + m_rtAreaWithLable.left - (nDivH*0.5) + nOffsetX;
		str.Format("%s", (*m_ptrStrGroupName[ii]));
		pDC->TextOut(currPos.x, currPos.y, str);
	}	

	ReleasePen(pDC);
}

void CDrawBadState::DrawDataLine(CDC *pDC)
{
	int width = m_rtAreaWithLable.Width();
	int height = m_rtAreaWithLable.Height();

	CString str;
	CPoint currPos, oldPos;

	float nDivV = float(height) / float(nRow);	//scale
	float nScale = float(m_nMaxUnit) / float(nRow);
	int nDivH = width / nCol;

	for(int i=0; i<m_nGroupCnt; i++){	//m_nGroupCnt
		//컬러가져오기
		if(i==0){
			CreatePen(pDC, PS_SOLID, 2, MAGENTA_N);
		}
		else if(i==1){
			CreatePen(pDC, PS_SOLID, 2, BLUE_N);
		}
		else{
			CreatePen(pDC, PS_SOLID, 2, GREEN_N);
		}
		for(int ii=0; ii<m_nDataCnt; ii++){
			float nValue = 0.0;
			float nValue2 = 0.0;
			//if(m_ptrBadCnt[i][ii] != 0){
				nValue = m_ptrBadCnt[i][ii];//(float)m_ptrBadCnt[i][ii] / (float)m_ptrTotalCnt[i][ii];
				nValue2 = (float)nValue;//(float)m_nMaxUnit * (float)nValue;
			//}
			//currPos.y = nValue * height;
			currPos.y = nValue2 / nScale * nDivV;
			currPos.x = nDivH * ii + m_rtAreaWithLable.left;

			if(ii == 0){
				//continue;
			}
			else{
				pDC->MoveTo(oldPos.x, GetConvertYPos(oldPos.y));
				pDC->LineTo(currPos.x, GetConvertYPos(currPos.y));
			}
			oldPos = currPos;
		}	
		ReleasePen(pDC);
	}	
}