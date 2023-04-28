#include "StdAfx.h"
#include "CgProject.h"
#include "math.h"
//#include "FchainDll.h"

#include "resource.h"			// view�� �����Ϸ��� �������
#include "FlyCapture2_EXDlg.h"	// view�� �����Ϸ��� �������


#define PI 3.14159
#define ANGLE2RADIAN(x)	PI*x/180.
#define RADIAN2ANGLE(x)	(x/PI)*180.


CFlyCapture2_EXDlg* pView;



CgProject::CgProject(void)
{
	m_ptLeftTop.SetPoint(0,0);
	m_ptRightTop.SetPoint(0,0);
	m_dTabTopLen = 0;
	for(int i=0;i<4;i++)m_ptTabCandidate[i].SetPoint(0,0);

	m_nWeldMinMax[0]=2000;
	m_nWeldMinMax[1]=0;
	m_bUseChain : FALSE;
	m_bSLoadMode = 0;
	m_dAvg = 0.;
	m_pImgInsp = NULL;
	m_nReduceScale = 64;
	m_dCabRadius = 0;
	m_nRegYPos = 80;
	m_nRegYPos1 = 80;
	m_nTopCapleft = 0;
	m_nTopCapRight = 0;
	m_nTopCapleft1 = 0;
	m_nTopCapRight1 = 0;
	m_rtTabPos.SetRect(0,0,0,0);
	m_nRegWeldPos[0] = 0;
	m_nRegWeldPos[1] = 0;
	m_ptTopEdge[0].SetPoint(0,0);
	m_ptTopEdge[1].SetPoint(0,0);
	for(int i=0;i<10;i++)		m_ptWeldPos[i].SetPoint(0,0);
	//for(int i=0;i<MAX_DEFECT;i++)
	//{
	//	m_pBadImage[i] = NULL;
	//	m_pTmpImage[i] = NULL;
	//}
	m_pChain = NULL;
	m_pChain1 = NULL;
	m_ptRegWeldPos[0].SetPoint(0,0);
	m_ptRegWeldPos[1].SetPoint(0,0);
	m_ptCanTop[0].SetPoint(0,0);
	m_ptCanTop[1].SetPoint(0,0);

	m_dInsTime = 0.;
	m_nFrameCount = 0;
	m_dTestEdge[0] = m_dTestEdge[1] = 0.;
	m_dSheetHei = 0.;
	m_nVPointCnt = 0;
	m_nHPointCnt = 0;
	m_dAngle = 0.;
	m_ncapblackcount = 0;
	memset(m_nStdPointX,0,sizeof(int));
	memset(m_nStdPointY,0,sizeof(int));
	m_ptCapCenter.SetPoint(CAMERA_WIDTH*0.5, CAMERA_HEIGHT*0.5);
	m_nStdPointX[0] = 200;	m_nStdPointY[0] = 50;
	m_nStdPointX[1] = 0;	m_nStdPointY[1] = 200;
//	m_bInspComplete[0] = FALSE;
	//m_bInspComplete[1] = FALSE;

	m_n1Hole = 0;
	for(int i=0;i<20;i++){
		m_MultiBuf[i] = NULL;
		m_AlphaBuf[i] = NULL;
	}

	for(int i=0;i<5;i++){
		m_WeldBuf[i] = NULL;
	}
	m_rtMarkInsp.SetRect(0,0,0,0);
	m_rtMarkSkip.SetRect(0,0,0,0);
	m_nOverLap = 0;

	m_nMarkPos[0]=m_nMarkPos[1]=0;
	for(int i=0;i<7;i++)m_dMaxChainData[i] = 0;
	for(int i=0; i<3; i++)
		m_nTopPos[i] = 0;
	m_nCharCnt = 5;

	m_dInspTime = 0;
	m_nIndex = 0;

//	m_rtFindArea.SetRect(150,400,2200,500);
//	m_rtInsArea.SetRect(150,350,2200,400);

//	m_fmInsp = NULL;
	for(int i=0;i<MAX_CAMERAS;i++)
	{
		m_nBlobCnt[i] = 0;
		m_nBlobCnt1[i] = 0;
		m_nBlobCnttest[i] = 0;
	//	m_fmInsp[i]=NULL;
	}

	m_pChain = new CChain(100,100000);
	m_pChain1 = new CChain(100,100000);

	m_dSheetHeight = 0;
	m_nInspComplete = 0;//�˻�Ϸ� ����
	for(int i=0;i<400;i++)
		m_nsheetCount[i] = 0;
	m_nHoleCandiCount = 0;
	m_nTapOkNg=1;//ret ok
	m_nHoleOkNg=1;//ret ok

}

CgProject::~CgProject(void)
{
	delete	m_fmTemp;
	if(m_pChain)	delete m_pChain;
	if(m_pChain1)	delete m_pChain1;
}

UINT CgProject::threadInspMark(LPVOID pParam)
{
	//CgProject* pDlg = (CgProject*)pParam;
	//int Retval=0;

	//while(pDlg->m_bStart)
	//{
	//	if(pDlg->m_bStartInsp==TRUE)
	//	{

	//		pDlg->m_bStartInsp = FALSE;
	//		pDlg->m_bInspComplete[0] = TRUE;
	//	}
	//	Sleep(1);
	//}
	return 0;// Retval;
}

void CgProject::Destroy()
{
	if (m_pChain)
		delete m_pChain;
	if (m_pChain1)
		delete m_pChain1;

	if(m_pImgInsp)			delete m_pImgInsp;
	delete		m_fmHole;
	for(int i=0;i<3;i++)delete fmRectTh[i];
	//for(int i=0;i<MAX_DEFECT;i++)
	//{
	//	if(m_pBadImage[i])delete m_pBadImage[i];
	//	if(m_pTmpImage[i])delete m_pTmpImage[i];
	//}
}

void CgProject::InitProject(LPVOID lParam)
{
	pView = (CFlyCapture2_EXDlg*)lParam;
	 
	m_ptLeftTop.SetPoint(0,0);
	m_ptRightTop.SetPoint(0,0);
	m_rtTabPos.SetRect(0,0,0,0);
	for(int i=0;i<4;i++)m_ptTabCandidate[i].SetPoint(0,0);


	m_n1Hole = 0;
	m_pImgInsp = new CImg();
	m_pImgInsp->Create(CAMERA_WIDTH, CAMERA_HEIGHT, 8);
	m_pImgInsp->InitDIB();

	m_fmTemp = new BYTE[CAMERA_HEIGHT*CAMERA_WIDTH];
	memset(m_fmTemp,0,CAMERA_WIDTH*CAMERA_HEIGHT);
	for(int i=0;i<20;i++){
		m_MultiBuf[i] = new BYTE[CAMERA_HEIGHT*CAMERA_WIDTH];
		m_AlphaBuf[i] = new BYTE[CAMERA_HEIGHT*CAMERA_WIDTH];
	}
	for(int i=0;i<5;i++){
		m_WeldBuf[i] = new BYTE[400*400];
		memset(m_WeldBuf[i],0,400*400);
	}
	m_nMultiFrame = 0;
	//for(int i=0;i<MAX_DEFECT;i++)
	//{
	//	m_pBadImage[i] = new CImg();
	//	m_pBadImage[i]->Create(MARK_SIZE*2, MARK_SIZE, 8);
	//	m_pBadImage[i]->InitDIB();
	//	m_pTmpImage[i] = new CImg();
	//	m_pTmpImage[i]->Create(MARK_SIZE*2, MARK_SIZE, 8);
	//	m_pTmpImage[i]->InitDIB();
	//}

	m_nHoleSearchingAreaW = 200;
	m_nHoleSearchingAreaH = 128;

	if(pView->m_nMachine==3){
		m_nHoleSearchingAreaW = 150;
		m_nHoleSearchingAreaH = 100;
	}

	m_fmHole = new BYTE[m_nHoleSearchingAreaW*m_nHoleSearchingAreaH];
	memset(m_fmHole,0,m_nHoleSearchingAreaW*m_nHoleSearchingAreaH);

	for(int i=0;i<3;i++)
	{
		fmRectTh[i] = new BYTE[m_nHoleSearchingAreaW*m_nHoleSearchingAreaH];
		memset(fmRectTh[i],0,m_nHoleSearchingAreaW*m_nHoleSearchingAreaH);
	}
	m_nInspScale = 4;
	m_nCharCnt = 5;


	for(int i=0;i<200;i++)m_nLineHeight[i] = -1;

	InitBuf();


	m_nTopEdge[1] = m_nTopEdge[0] = 0;
	m_nTopPos[0]=m_nTopPos[1]=m_nTopPos[2] = 0;
	m_rtWeldRect[0].SetRect(0,0,0,0);
	m_rtWeldRect[1].SetRect(0,0,0,0);
	m_rtWeldPos.SetRect(0,0,0,0);
	m_rtMidWeldPos.SetRect(0,0,0,0);
	m_nWhiteValue = 0;
	m_nBlackValue = 0;
	m_dBlackPer[0]=m_dBlackPer[1]=m_dBlackPer1[0]=m_dBlackPer1[1]=0;
	for(int i=0;i<7;i++)
		m_dMaxChainData[i]=0;

	//m_PatReg.nematInit(0, 0);
	//m_pNepatReg = NULL;
	//PatLoad();
	ResetFrame();

	for(int i=0;i<4;i++)	m_nLinMinMax[i] = 0;
	m_ncapblackcount = 0;
	m_bStart = TRUE;
//	AfxBeginThread(threadInspMark, this); // thread ����

}
void CgProject::InitBuf()
{
	for(int i=0;i<MAX_CAMERAS;i++)
		m_fm[i] = pView->m_Display[i].m_pImg->GetImagePtr();


}

void CgProject::ResetFrame()
{
	m_nFrameCount = 0;
	m_nNgCount = 0;
	m_nTapOkNg = -1;
	m_nHoleOkNg = -1;
	ReadyData();
}

void CgProject::ReadyData()
{
	for(int i=0; i<2; i++){
		m_nBlackCount[i] = 0;
		m_nWhiteCount[i] = 0;
		m_dBlackPer[i] = 0;
		m_dBlackPer1[i] = 0;
		m_nBCount[i] = 0;
		m_nBlackCount[i] = 0;
		m_ptCanTop[i].SetPoint(0,0);
	}

	m_nWhiteValue = 0;
	m_nBlackValue = 0;

	m_nWeldMinMax[0]=2000;
	m_nWeldMinMax[1]=0;

	m_ptTopEdge[0].SetPoint(0,0);
	m_ptTopEdge[1].SetPoint(0,0);
	m_dTabTopLen = 0;
	for(int i=0;i<10;i++)		m_ptWeldPos[i].SetPoint(0,0);

	for(int i=0;i<5;i++)
		memset(m_WeldBuf[i],0,400*400);

}

int CgProject::TestInspect()
{
	//find circle center
	//find Line
	//find bead
	//check bead


	
/*
int* pHisto = new int[CAMERA_WIDTH];
	memset(pHisto,0,sizeof(int)*CAMERA_WIDTH);

	for(int i=0;i<CAMERA_WIDTH;i++)
		for(int j=0;j<CAMERA_HEIGHT;j++)
		{
			if(m_fm[i+CAMERA_WIDTH*j]<20)
				pHisto[i] ++ ;
		}
		double dMax=0;
		double dTmp=0.;
		CString str,strSum;
		for(int i=2;i<CAMERA_WIDTH/2;i++)
		{
			dTmp = pHisto[i+2] + pHisto[i+1] - pHisto[i-1]  - pHisto[i-2];
			if(dTmp>dMax)
			{
				dMax = dTmp;
				m_dTestEdge[0] = i;
			}
		}

		dMax=0;
		dTmp=0.;
		for(int i=CAMERA_WIDTH-3;i>CAMERA_WIDTH/2;i--)
		{
			dTmp = pHisto[i-2] + pHisto[i-1] - pHisto[i+1]  - pHisto[i+2];

			if(dTmp>dMax)
			{
				dMax = dTmp;
				m_dTestEdge[1] = i;
			}
		}
		delete[] pHisto;

	m_nFrameCount++;
	*/
	return 0;
}

CRect CgProject::FindEdge1(int nCam,BYTE* fm,int width,int height,int* nl,int* nr,int* nt,int* nb,int npos)
{
	CRect rt;
	return rt;
}



int CgProject::FindSkipArea(int nCam,BYTE * fm, CRect * rtinsp, CRect * rtSkip)
{

	return 0 ;
}



int CgProject::Segment(int nCam,int minbound,int maxbound,int blobarea,int blobmin)
{
	return 0;
}

int CgProject::CheckPos(CRect rtFull, CRect rtTarget)
{
	int ret = 1;

	
	return ret;
}


void CgProject::EdgeCorrection(int nCam,unsigned char* fmTmp,int nLeft,int nTop)
{

}

CRect CgProject::FindEdge(int nCam,BYTE* fm,int width,int height,int* nl,int* nr,int* nt,int* nb,int npos)
{
	CRect rt;
	return rt;
}
int CgProject::Segment2(int nCam)
{
	return 0;
}
int CgProject::SplitChar1(int nCam)
{

	return 0;
}

int CgProject::SplitChar(int nCam)
{
	return 0;
}
int CgProject::GetCog(BYTE* fm,int pitch,int height, int ncount)
{//small Char 

	int i,j,k,xsum,ysum,nc,ns,ne;
	CPoint pt1[MAX_SAMLL_CHAR];
	for(i=0;i<MAX_SAMLL_CHAR;i++)	m_rtSChar[i].SetRect(0,0,0,0);
	for(k=0;k<ncount-1;k++){
		xsum=ysum=nc=0;
		ne = m_nSmallPos[k+1];
		ns = m_nSmallPos[k];
		//���� ���̰� ū���
		if(ne-ns > SMALL_CHAR_WIDTH*1.5)
		{//2�� ������.
			for(int l=0;i<(ne-ns)/SMALL_CHAR_WIDTH;i++)
			{
				ncount++;
				xsum=ysum=nc=0;
				ns = ns+(ns*l);
				ne = ns+SMALL_CHAR_WIDTH;
				for(i=ns;i<ne;i++)
				{		
					for(j=0;j<height;j++)
					{
						if(*(fm+j*pitch+i)>200)
						{
							xsum += i;
							ysum += j;
							nc++;
						}
					}
				}
				if(nc<1)nc = 1;
				pt1[k].SetPoint(xsum/nc,ysum/nc);
			}
		}
		else
		{
			for(i=ns;i<ne;i++)
			{		
				for(j=0;j<height;j++)
				{
					if(*(fm+j*pitch+i)>200)
					{
						xsum += i;
						ysum += j;
						nc++;
					}
				}
			}
			if(nc<1)nc = 1;
			pt1[k].SetPoint(xsum/nc,ysum/nc);
		}
	}
	//COG ���� �»� ��ġ ã��
	for(k=0;k<ncount-1;k++)
		m_rtSChar[k].SetRect(pt1[k].x-(SMALL_CHAR_WIDTH*0.5),pt1[k].y-(SMALL_CHAR_HEIGHT*0.5),pt1[k].x+(SMALL_CHAR_WIDTH*0.5),pt1[k].y+(SMALL_CHAR_HEIGHT*0.5));
	return ncount;
}

int CgProject::GetCog1(BYTE* fm,CRect rt,int pitch,int th,int rowcol)
{//small Char 

	int i,j,k,ns,ne,ntmp=0,nctmp=0;
	double xsum=0,ysum=0,nc=0;
	for(k=rt.top;k<rt.bottom-1;k++)
	{
		//���� ���̰� ū���
		for(int h=rt.left;h<rt.right-1;h++)
		{
			ntmp += *(fm+pitch*k+h);
			nctmp++;
			//���� ���̰� ū���
			if(*(fm+pitch*k+h)<th)
			{
				xsum+=h;
				ysum+=k;
				nc++;
				//if(xsum<0 || ysum<0)
				//	xsum = xsum;
			}
		}
	}
	int count = 0;
	if(nc<1)nc = 1;
	if(rowcol==1)//����
		count = ysum/nc;
	else
		count = xsum/nc;
	//COG ���� �»� ��ġ ã��
	return count;
}

//�ƽ�Ű�ڵ�
//'0':48		'9':57	10ea
//'A':65		'Z':90	26ea
/*
int CgProject::GetChar2Index(char c)
{
	if ('0'<=c && c<='9')
	{
		return c - '0';		
	}
	else if ('A'<=c && c<='Z')
	{
		return c - 'A' +10;
	}
}
*/
char CgProject::GetIndex2Char(int idx)
{
	if(0<=idx && idx<=9)
		return '0'+idx;
	else if(10<=idx && idx<MAX_CHAR-1-3)
		return 'A'+idx-10;
	else if(idx == MAX_CHAR-1-3)
		return '-';
	else if(idx == MAX_CHAR-1-2)
		return 'c';
	else if(idx == MAX_CHAR-1-1)
		return 't';
	else if(idx == MAX_CHAR-1)
		return 'w';

	return '=';
}

void CgProject::PatLoad()
{

}
void CgProject::PatLoadSmall()
{

}

void CgProject::Rotate(BYTE *dest, BYTE *src,float x0,float y0,int imagewidth,int imageheight, float radian)
{	
	int i,j;
	float costheta,sintheta,x,y;
	float cxf,cyf,cxsf,cysf;
	PIXEL *d;

//������ �������� ��ȯ.
	radian = ANGLE2RADIAN(radian);

	costheta=(float)cos((double)radian);
	sintheta=(float)sin((double)radian);
	d=dest;

	cxf=imagewidth/2;			
	cyf=imageheight/2;
	cxsf=x0;
	cysf=y0;

	for (y=-cyf,i=0; i<imageheight; i++,y++,d+=imagewidth)	
	{	
		float yc,ys;
		ys=cxsf-y*sintheta;
		yc=cysf+y*costheta;
		//		for (x=-cxf,j=0; j<dest->nx; j++,x++)
		for (x=-cxf,j=0; j<imagewidth; j++,x++)
		{	
			int iy2;
			float y2;	
			iy2=(int)(y2=x*sintheta+yc);
			if ((iy2>0)&&(iy2<imageheight-1))			
			{	
				float x2;
				int ix2=(int)(x2=x*costheta+ys);
				if ((ix2>0)&&(ix2<imagewidth-1))		
				{	
					float dx1,dy1,dx2,dy2;
					PIXEL *s=src+iy2*imagewidth+ix2;
					dx1=x2-ix2;					
					dy1=y2-iy2;
					dx2=1.-dx1;
					dy2=1.-dy1;
					d[j]=(PIXEL)((s[0]*dx2+s[1]*dx1)*dy2+
						(s[imagewidth]*dx2+s[imagewidth+1]*dx1)*dy1);

				}
				else d[j]=80;//255;//0;
			}
			else d[j]=80;//255;//0;
		}
	}
}

CPoint CgProject::RotateRoi(CPoint spt,float angle)
{
	CPoint dpt;
	dpt.SetPoint(0,0);

	float radian;
	radian = ANGLE2RADIAN(angle);

	double x,y,x1,y1;
	double sintheta ,costheta;
	sintheta = sin(radian);
	costheta = cos(radian);

	x1 = spt.x*costheta+spt.y*sintheta;
	y1 = -spt.x*sintheta+spt.y*costheta;
//	y1 = spt.x*costheta+spt.y*sintheta;
//	x1 = -spt.x*sintheta+spt.y*costheta;
	dpt.SetPoint(x1,y1);

	return dpt;
}

double CgProject::fnFindAngle(int nCam,BYTE * fm,CRect rt,int pitch)
{
	double dangle = 0;
	int i=0,nIdx,nscore[2];
	int nx1 = rt.left;
	int nx2 = rt.right;
	int offset = 4;
	int gap = 10;
	int npos1,npos2;
	npos1=npos2 = 0;

	for(int i=rt.top+offset;i<rt.bottom;i++)
	{
		if( *(fm+i*pitch+nx1) <100  && npos1==0)			npos1 = i;
		if(*(fm+i*pitch+nx2) < 100&& npos2==0)			npos2 = i;
		if(npos2 > 0 && npos1> 0)
			break;
	}

	//Pat Angle
 	int nx = (nx1-nx2);
 	int ny = (npos1-npos2);
	 
 	double da1,da2,da3,drega1,drega2;
	drega1 = double(nx)/sqrt(double(nx*nx)+double(ny*ny));
	drega2 = acos(drega1)*180.0/PI;
	da1 = atan(drega1)*180.0/PI;
	da2 = 180.0*atan(2.0/sqrt((float)(nx*nx)+(ny*ny)))/PI;
	da3 = atan(drega1);

	dangle = 180 - drega2;//drega1;//drega2 - da3;
	return dangle;//da3;//dangle;
}

void CgProject::FindLineEdge_ABS(CRect rtInsArea, int k, int Dist, int TrackerNum)//rtInsArea:�˻��ҿ��� TargetDiff:��������� Dist:�ȼ��Ÿ�
{
	
	int i,j,l=0;
	int nCam = 0, nDiff = 0;
//	pView->m_Display[0].m_fm= new unsigned char[CAMERA_HEIGHT*CAM_WIDTH];
//	memcpy(pView->m_Display[0].m_fm,m_fm[0],CAMERA_HEIGHT*CAM_WIDTH);
	unsigned char *fm = m_fm[nCam];
	int nPitch = CAM_WIDTH;

	if(rtInsArea.top<0||rtInsArea.bottom>CAMERA_HEIGHT||rtInsArea.left<0||rtInsArea.right>CAM_WIDTH)
		return;
	double* sum = (double*)malloc(rtInsArea.Height()*sizeof(double));
	double* pHisto = (double*)malloc(rtInsArea.Height()*sizeof(double));
	double* pHisto2 = (double*)malloc(rtInsArea.Height()*sizeof(double));
	memset(sum, 0, rtInsArea.Height()*sizeof(double));
	memset(pHisto, 0, rtInsArea.Height()*sizeof(double));
	memset(pHisto2, 0, rtInsArea.Height()*sizeof(double));
	double rtAve[2000];
	double stdDevArray[2000];
	int pcnt=0;

 	for(i=0; i<2000; i++)
 	{
 		rtAve[i] =0;
 		stdDevArray[i]=0;
 	}

	BOOL edgeflag=FALSE;
	BOOL edgedarkflag= FALSE;
	int edgedartcnt=0;
	int DarkCount=0;
	int startLine=0;
	BOOL eflage = FALSE;
	int endLine=0;
	int LinCNt=0;
	
	int RectCnt=pView->m_Display[0].m_rtImgTracker[TrackerNum].bottom-pView->m_Display[0].m_rtImgTracker[TrackerNum].top;
	double AveLine[CAMERA_HEIGHT];

	for(int i=0; i<CAMERA_HEIGHT; i++)		AveLine[i]=0;
	
	if(pView->m_Display[0].m_rtImgTracker[TrackerNum].left<0)pView->m_Display[0].m_rtImgTracker[TrackerNum].left = 0;
	if(pView->m_Display[0].m_rtImgTracker[TrackerNum].top<0)pView->m_Display[0].m_rtImgTracker[TrackerNum].top = 0;
	if(pView->m_Display[0].m_rtImgTracker[TrackerNum].right>CAM_WIDTH)pView->m_Display[0].m_rtImgTracker[TrackerNum].right = CAM_WIDTH;
	if(pView->m_Display[0].m_rtImgTracker[TrackerNum].bottom>CAMERA_HEIGHT)pView->m_Display[0].m_rtImgTracker[TrackerNum].bottom = CAMERA_HEIGHT;

//	LinCNt = pView->m_Display[0].m_rtImgTracker[0].top;
	for(int indexY=pView->m_Display[0].m_rtImgTracker[TrackerNum].top; indexY<pView->m_Display[0].m_rtImgTracker[TrackerNum].bottom; indexY++)
	{
		if(indexY< m_rtMaster.top || indexY>m_rtMaster.bottom)
			continue;
		
		AveLine[LinCNt]=0;
		pcnt=0;
		for(int IndexX = pView->m_Display[0].m_rtImgTracker[0].left; IndexX<pView->m_Display[0].m_rtImgTracker[0].right; IndexX++)
		{
			AveLine[LinCNt]+=m_fm[0][indexY*nPitch+IndexX];
			pcnt++;
		}
		pView->m_Display[0].m_vp[LinCNt]=AveLine[LinCNt]/pcnt;
//		AveLine[LinCNt]+=m_fm[0][indexY*nPitch+pView->m_Display[0].m_rtImgTracker[TrackerNum].left];
//		pView->m_Display[0].m_vp[LinCNt]=AveLine[LinCNt];
		LinCNt++;
	}	
	pView->m_Display[0].m_brtDetect=TRUE;		
	delete sum;
	delete pHisto;
	delete pHisto2;
	return;
}

void CgProject::Ins_Smoothing(int Sm_count)
{
	double Temp[CAMERA_HEIGHT];
	for(int p=0; p<CAMERA_HEIGHT; p++)
	{
		Temp[p]=0;
	}
	if(Sm_count%2!=0 && Sm_count>1)
	{
		double SmoothingLine[CAMERA_HEIGHT];

		for(int i=0; i<CAMERA_HEIGHT; i++)
		{	
			SmoothingLine[i]=pView->m_Display[0].m_vp[i];
		}
		
		int Change_cnt=Sm_count-(Sm_count/2);
		for(int indexy=0; indexy<CAMERA_HEIGHT-Sm_count; indexy++)
		{	
			for(int cnt=0; cnt<Sm_count; cnt++)
			{
				Temp[indexy]+=SmoothingLine[indexy+cnt];
			}
			
			SmoothingLine[Change_cnt-1]=Temp[indexy]/Sm_count;
			Change_cnt++;
		}
		for(int i=0; i<CAMERA_HEIGHT; i++)
		{	
			pView->m_Display[0].m_vp[i]=SmoothingLine[i];
		}
		pView->m_Display[0].m_brtDetect=TRUE;
	}

// 	double SmoothingLine[CAMERA_HEIGHT];
// 
// 	for(int i=0; i<CAMERA_HEIGHT; i++)
// 	{	
// 		SmoothingLine[i]=pView->m_Display[0].m_vp[i];
// 	}
// 
// 	for(int indexy=0; indexy<CAMERA_HEIGHT-2; indexy++)
// 	{
// 		SmoothingLine[indexy+1]=(SmoothingLine[indexy]+SmoothingLine[indexy+1]+SmoothingLine[indexy+2])/3;
// 	}
// 	for(int i=0; i<CAMERA_HEIGHT; i++)
// 	{	
// 		pView->m_Display[0].m_vp[i]=SmoothingLine[i];
// 	}
// 	pView->m_Display[0].m_brtDetect=TRUE;
// 	pView->DrawCamDisplay(0);
}

int CgProject::FindInsArea(int nIndex,CRect rt,BYTE *fm)
{
	//	m_fm[0][(j)*CAM_WIDTH+i];

	CRect rt1;
	int limit = 1820;
	//Top�������� Bar���� Ȯ��
	rt.SetRect(10,200,limit,300);
	rt.SetRect(10,1200,limit,1300);

	int nth = 20,ntmp=0,ntmp1;
	int nxpos[100],nypos1=0,nypos2=0,nposcount = 0;
	int ndirection = 0; // 1���� //2������
	//������� �˻� ��ȿ ����
	int ntop=0,nbottom=0,nleft=0,nright=0;

	for(int i=0;i<100;i++)nxpos[i] =m_nEdgePoint[i]= 0;
	int nproj[CAM_WIDTH];
	for(int i=0;i<CAM_WIDTH;i++)nproj[i] = 0;
	for(int i=rt.left;i<rt.right;i++)
	{
		for(int j=rt.top;j<rt.bottom;j++)			nproj[i] += fm[j*CAM_WIDTH+i];
		nproj[i] /= rt.Height();
	}
	//white
	for(int i=rt.left;i<rt.right-4;i++){
		if(nposcount>=99)break;
		if(abs(nproj[i]-nproj[i+4])> nth ){//&& (nproj[i]>230 ||nproj[i+4]>230)){
			if(nproj[i]>nproj[i+4] && ndirection==0)ndirection = 1;
			else if(nproj[i]<nproj[i+4] && ndirection==0)ndirection = 2;
			nxpos[nposcount++] = i; i+=32;
		}
	}
	for(int i=0;i<nposcount;i++)
	{
		if(nxpos[i]>0)
		{
			m_nEdgePoint[i] = nxpos[i];
		}
	}

	//�� ��
	if(nposcount==0)
	{

		rt.SetRect(CAMERA_WIDTH*0.5-100,200,CAMERA_WIDTH*0.5+100,1900);
		nleft = 50;nright=limit-50;
	}
	else if(nposcount==1)
	{//�� //�� Ȯ��
		if(ndirection==1)//��� ����
		{
			rt.SetRect(nxpos[0]+(CAMERA_WIDTH-nxpos[0])*0.5-100,200,nxpos[0]+(CAMERA_WIDTH-nxpos[0])*0.5+100,1900);
			nleft = nxpos[0]+50;
			nright = limit -50;
		}
		else if(ndirection==2){
			rt.SetRect(nxpos[0]*0.5-100,200,nxpos[0]*0.5+100,1900);
			nleft = 50;
			nright = nxpos[0]-50;
		}
	}
	else
	{
		if(m_nEdgePoint[0] > limit-m_nEdgePoint[1]){
			rt.SetRect(m_nEdgePoint[0]*0.5-100,200,m_nEdgePoint[0]*0.5+100,1900);
			nleft = 50;nright = m_nEdgePoint[0]-50;
		}
		else{
			rt.SetRect(m_nEdgePoint[1]+(limit-m_nEdgePoint[1])*0.5-100,200,m_nEdgePoint[1]+(limit-m_nEdgePoint[1])*0.5+100,1900);
			nleft = m_nEdgePoint[1]+50;nright=limit-50;
		}
	}
	m_rtMaster = rt;

	int nhproj[CAMERA_HEIGHT];
	for(int i=0;i<CAMERA_HEIGHT;i++)nhproj[i]=0;
	for(int i=rt.top;i<rt.bottom;i++)
	{
		for(int j=rt.left;j<rt.right;j++)
		{
			nhproj[i] +=  fm[i*CAM_WIDTH+j];
		}
		nhproj[i] /= rt.Width();
	}
	int nEdge = 60;
	//	for(int i=rt.top;i<rt.bottom-4;i++)
	for(int i=rt.bottom-104;i>rt.top;i--)
	{
		//���� ��ο� ������
		//		if(nhproj[i-4]<50&&nhproj[i]<50&&nhproj[i+2]>50&&nhproj[i+1]>50&&ntop==0 && nhproj[i+2]-nhproj[i]<nhproj[i+1]-nhproj[i-1])
		if(nhproj[i]<nEdge&&nhproj[i-1]<nEdge&&nhproj[i-2]<nEdge&&nhproj[i-3]<nEdge&& nhproj[i-4]<nEdge)
		{
			ntop = i;break;
		}
	}
	for(int i=limit;i>ntop+50;i--)
	{
		//if(nhproj[i]>200&&nhproj[i-3]<200&&nbottom==0 && nhproj[i]-nhproj[i-2]<nhproj[i-1]-nhproj[i-3])
		if(nhproj[i]<70&&nhproj[i-3]<70&&nbottom==0 &&nhproj[i-4]<70 &&nhproj[i-5]<70 )  //joo
		{
			nbottom = i-2;break;
		}
	}
	if(nbottom==0)nbottom = rt.bottom;

	//��� ���
	//m_dAreaAverage

	
	CString str;
	if(ntop>1500)ntop = 1000;
	if(nbottom>1800)nbottom = 1900;
	m_rtMaster.SetRect(nleft,ntop,nright,nbottom);
	pView->m_Display[0].m_rtMaster = m_rtMaster;
	//tracker ��ġ �̵�

	pView->m_Display[0].m_rtImgTracker[0].left = nleft+(nright-nleft)*0.5;
	pView->m_Display[0].m_rtImgTracker[0].right = nright-50;//

	CPoint p1 = pView->m_Display[0].ScaleTrans(pView->m_Display[0].m_rtImgTracker[0].TopLeft(),1);
	CPoint p2 = pView->m_Display[0].ScaleTrans(pView->m_Display[0].m_rtImgTracker[0].BottomRight(),1);

	pView->m_Display[0].m_Tracker[0].m_rect.left = p1.x;
	pView->m_Display[0].m_Tracker[0].m_rect.right = p2.x;

	return 0;
}


int CgProject::AreaAverage(int nIndex,BYTE *fm,int pitch,int nl,int nt,int nr,int nb)
{
	int nave = 0;
	m_nCapMaxBright = 0;
	//CheckROI()
	if(nr-nl<=1 || nb-nt <=1)
		return 0;
	int nmax = 0;
	int nHisto[256];
	for(int i=0;i<256;i++)nHisto[i] = 0;
	if(nb>CAMERA_HEIGHT)nb=CAMERA_HEIGHT;
	for(int i=nt;i<nb;i++)
		for(int j=nl;j<nr;j++){
			nave += fm[i*pitch+j];
			nHisto[fm[i*pitch+j]]++;
			if(nmax<fm[i*pitch+j])
				nmax=fm[i*pitch+j];
		}
	if(nmax>80){
		for(int i=200;i>0;i--)
			if(nHisto[i]>300)
			{
				m_nCapMaxBright = i;
				break;
			}
	}
	nave /= (nr-nl)*(nb-nt);
	return nave;
}
int CgProject::AreaAverage1(int nIndex,BYTE *fm,int pitch,int nl,int nt,int nr,int nb,int* cnt)
{
	int nave = 0;
	if(nr-nl<=1 || nb-nt <=1)
		return 0;
	int nmax = 0;
	int nHisto[256];
	int nc = 0,ncount = 0;
	for(int i=0;i<255;i++)nHisto[i]=0;
	if(nb>CAMERA_HEIGHT)nb=CAMERA_HEIGHT;
	for(int i=nt;i<nb;i++)
		for(int j=nl;j<nr;j++){
			nave += fm[i*pitch+j];
			nHisto[fm[i*pitch+j]]++;
			if(nmax<fm[i*pitch+j])nmax=fm[i*pitch+j];
			nc++;
		}
	if(nmax>80){
		for(int i=200;i>0;i--)
			if(nHisto[i]>300)
			{
				m_nCapMaxBright = i;
				break;
			}
	}
	if(nc<=0)nc=1;
	nave /= nc;
	for(int i=nt;i<nb;i++)
		for(int j=nl;j<nr;j++){
			if(fm[i*pitch+j]<nave-15)
			{
				ncount++;
			}	
		}
	
	*cnt = ncount;

	return nave;
}


int CgProject::AreaAverage2(int nIndex,BYTE *fm,int pitch,int nl,int nt,int nr,int nb,int th,int* cnt,double* dstd)
{
	int nave = 0;
	if(nr-nl<=1 || nb-nt <=1)
		return 0;
	int nmax = 0;
	int nHisto[256];
	double ntmp = 0;
	int nc = 0,ncount = 0;
	for(int i=0;i<255;i++)nHisto[i]=0;
	if(nb>CAMERA_HEIGHT-1)nb=CAMERA_HEIGHT-1;
	for(int i=nt;i<nb;i++){
		for(int j=nl;j<nr;j++){
			nave += *(fm+i*pitch+j);// fm[i*pitch+j];
			ntmp += *(fm+i*pitch+j)* *(fm+i*pitch+j);//fm[i*pitch+j]*fm[i*pitch+j];
			nHisto[*(fm+i*pitch+j)]++;//fm[i*pitch+j]]++;
			if(nmax<*(fm+i*pitch+j))//fm[i*pitch+j])
				nmax=*(fm+i*pitch+j);//fm[i*pitch+j];
			nc++;
		}
	}

	if(nc<=0)nc=1;
	nave /= nc;

	// ��尡 1�̸�, ������ �������(150) ���� ����
	if(nIndex==1)
	{
		for(int i=nt;i<nb;i++){
			for(int j=nl;j<nr;j++){
				if(*(fm+i*pitch+j)<150)
				{
					ncount++;
				}	
			}
		}
	}
	else // ��尡 1�� �ƴϸ�, ������ (��չ��-Th) ���� ������
	{
		if(nIndex==2)
		{
			for(int i=nt;i<nb;i++){
				for(int j=nl;j<nr;j++){
					if(*(fm+i*pitch+j)<(nmax+nave)*0.5-th)
					{
						ncount++;
					}	
				}
			}
		}
		else
		{
			for(int i=nt;i<nb;i++){
				for(int j=nl;j<nr;j++){
					if(*(fm+i*pitch+j)<nave-th)
					{
						ncount++;
					}	
				}
			}
		}
	}

	// ���� ���� ���� 80���� ũ��,
	if(nmax>80){
		for(int i=200;i>0;i--)
			if(nHisto[i]>300)
			{
				m_nCapMaxBright = i; // ���� ������(300�̻�) && ���� ���� ���
				break;
			}
	}
	
	// �Ⱦ�
	if(nc<1)nc = 2;
	*dstd = sqrt((nc*ntmp-nave*nave)/((double)nc*((double)nc-1)));
	if(*dstd<0)*dstd = 0;

	*cnt = ncount;

	return nave;
}

int CgProject::CircleFind()
{
	int nret = 0;
	CRect rt,rt1;
	double dCx,dCy;
	BYTE * fm = new BYTE[CAMERA_WIDTH*CAMERA_HEIGHT];
	memcpy(fm,m_fm[0],CAMERA_HEIGHT*CAMERA_WIDTH);
	{
		if(pView->m_nMachine == 1 || pView->m_nMachine==8)//ochang
			rt.SetRect(1,400,1279,670);//750);
		else if(pView->m_nMachine==2)//nanzing 2nd
		{
			if(pView->m_strComname.Find("1")>=0)rt.SetRect(70,360,1240,650);
			else			rt.SetRect(10,350,1190,600);
		}
		else if(pView->m_nMachine==3)//nanging 1st
			rt.SetRect(5,400,1230,650);
	}
	//�ʱ�ȭ
	m_ptCapCenter.SetPoint(0,0);
	for(int i =0;i<CAMERA_HEIGHT;i++)		m_ptDrawCapEdge[0][i].x = m_ptDrawCapEdge[1][i].x = m_ptDrawCapEdge[0][i].y = m_ptDrawCapEdge[1][i].y = 0;
	int minx1=CAMERA_WIDTH,minx2=0,miny1=0,miny2=0,miny3=0;
	int nCapEdgeValue = pView->m_pDlgSet->m_FormTab4->m_nTopCapEdgeValue;//left
	double dEdgeX[CAMERA_WIDTH],dEdgeY[CAMERA_WIDTH];
	for(int i=0;i<CAMERA_WIDTH;i++)dEdgeY[i]=dEdgeX[i] = 0;
	int nCnt = 0;
	//������ ���� ã�� // ���� ������ ã�� //1411
	for(int i=rt.top;i<rt.bottom-4;i++)
	{
		for(int j=rt.left;j<rt.right-6;j++)
		{
			if(*(fm+i*CAMERA_WIDTH+j) - *(fm+i*CAMERA_WIDTH+j+6) >=nCapEdgeValue &&*(fm+(i+2)*CAMERA_WIDTH+j) - *(fm+(i+2)*CAMERA_WIDTH+j+6) >=nCapEdgeValue
				&&*(fm+(i+3)*CAMERA_WIDTH+j) - *(fm+(i+3)*CAMERA_WIDTH+j+6) >=nCapEdgeValue &&*(fm+(i+4)*CAMERA_WIDTH+j) - *(fm+(i+4)*CAMERA_WIDTH+j+6) >=nCapEdgeValue) //���
			{
				if(minx1>j)minx1 = j,miny1=i;
				m_ptDrawCapEdge[0][i].SetPoint(j,i);
				dEdgeX[nCnt] = j;dEdgeY[nCnt]=i;nCnt++;
				break;
			}
		}
	}
	nCapEdgeValue = pView->m_pDlgSet->m_FormTab4->m_nTopCapEdgeValue1;//right
	for(int i=rt.top;i<rt.bottom-4;i++)
	{
		for(int j=rt.right;j>rt.left+6;j--)
		{
			if(*(fm+i*CAMERA_WIDTH+j) - *(fm+i*CAMERA_WIDTH+j-6) >=nCapEdgeValue &&*(fm+(i+2)*CAMERA_WIDTH+j) - *(fm+(i+2)*CAMERA_WIDTH+j-6) >=nCapEdgeValue
				&&*(fm+(i+3)*CAMERA_WIDTH+j) - *(fm+(i+3)*CAMERA_WIDTH+j-6) >=nCapEdgeValue &&*(fm+(i+4)*CAMERA_WIDTH+j) - *(fm+(i+4)*CAMERA_WIDTH+j-6) >=nCapEdgeValue) //����
			{
				if(minx2<j)minx2= j,miny2=i;
				m_ptDrawCapEdge[1][i].SetPoint(j,i);
				dEdgeX[nCnt] = j;dEdgeY[nCnt]=i;nCnt++;
				break;
			}
		}
	}

	//	CCircleFitting circleFit;
	double dRetX,dRetY,dRetR;
	circleFit.RobustCircleFit(dEdgeX, dEdgeY, nCnt, &dRetX, &dRetY, &dRetR, int(nCnt*0.5), 1);
	//	for (int i=0;i<nCnt;i++)
	//		pView->m_Display[nCam].AddArrayPoint(CPoint((int)dEdgeX[i],(int)dEdgeY[i]),0);
	//	cx = (int)dRetX;
	//	cy = (int)dRetY;

	m_dCapWidth = minx2-minx1;
	m_ptCapCenter.x = minx1+m_dCapWidth*0.5;
	m_ptCapCenter.y = miny1;
	int	nCapOffset = 450;

	if( pView->m_nMachine==2 &&pView->m_strComname.Find("1")>=0)
	{//������
		m_ptCapCenter.x = minx1+6+nCapOffset;
		m_ptCapCenter.x = minx2-nCapOffset;
		m_ptCapCenter.y  = miny1;
	}
	else if(pView->m_nMachine==2 &&pView->m_strComname.Find("2")>=0)
	{//������
		m_ptCapCenter.x = minx2-nCapOffset;
		m_ptCapCenter.y  = miny2;
	}
	else
	{
		minx2=minx1=0;
		for(int i=rt.top;i<rt.bottom;i++)
		{
			if(m_ptDrawCapEdge[1][i].x==0 || m_ptDrawCapEdge[0][i].x==0)continue;
			if(m_ptDrawCapEdge[1][i].x-m_ptDrawCapEdge[0][i].x >= minx1){
				minx1 = m_ptDrawCapEdge[1][i].x-m_ptDrawCapEdge[0][i].x ;
				miny3 = i;
			}
		}	
		if(miny1 <= miny2 && miny1 <=miny3)
			m_ptCapCenter.y = (miny2+miny3)/2;
		else if(miny2 <= miny1 && miny2 <=miny3)
			m_ptCapCenter.y = (miny1+miny3)/2;
		else if(miny3 <= miny1 && miny3 <=miny2)
			m_ptCapCenter.y = (miny1+miny2)/2;
		else	m_ptCapCenter.y = (miny1+miny2+miny3)/3;
		m_ptCapCenter.x = (m_ptDrawCapEdge[0][miny1].x+m_ptDrawCapEdge[1][miny2].x)*0.5;
		m_ptCapCenter.y +=10;
	}



	CString str;
	str.Format("Diameter:%.0f(%d) Cx:%d , Cy:%d",m_dCapWidth,minx1,m_ptCapCenter.x,m_ptCapCenter.y);
	pView->AddtoInspLog(str);
	if(m_dCapWidth>750)	nret = 1;

	int nTolY = pView->m_pDlgSet->m_FormTab4->m_dTapTolY1/pView->m_pDlgSet->m_FormTab2->m_dCamScale;
	int nTolY1 = pView->m_pDlgSet->m_FormTab4->m_dTapTolY2/pView->m_pDlgSet->m_FormTab2->m_dCamScale;
	//m_dTapTolY2
	//	m_rtTabOkRange.SetRect(m_ptCapCenter.x-pView->m_pDlgSet->m_FormTab4->m_nTabTol,m_ptCapCenter.y-nTolY,m_ptCapCenter.x+pView->m_pDlgSet->m_FormTab4->m_nTabTol,m_ptCapCenter.y+nTolY1);
	int ntL = pView->m_pDlgSet->m_FormTab4->m_dTabTol/pView->m_pDlgSet->m_FormTab2->m_dCamScale;
	int ntR = pView->m_pDlgSet->m_FormTab4->m_dTabTol1/pView->m_pDlgSet->m_FormTab2->m_dCamScale;

	m_rtTabOkRange.SetRect(m_ptCapCenter.x-ntL,m_ptCapCenter.y-nTolY,m_ptCapCenter.x+ntR,m_ptCapCenter.y+nTolY1);
	m_rtTabOkRange = CheckROI(m_rtTabOkRange,CRect(50,50,CAMERA_WIDTH-50,CAMERA_HEIGHT-10));

	delete fm;

	if(m_rtTabOkRange.left<0 ||m_rtTabOkRange.top<0 ||m_rtTabOkRange.right>=CAMERA_WIDTH-1 ||m_rtTabOkRange.bottom>CAMERA_HEIGHT )
		nret = 0;
	return nret;

}


int CgProject::CircleFind1()
{
	int nret = 0;
	CRect rt,rt1;
	double dCx,dCy;
	BYTE * fm = new BYTE[CAMERA_WIDTH*CAMERA_HEIGHT];
	memcpy(fm,m_fm[0],CAMERA_HEIGHT*CAMERA_WIDTH);
	double d1[10]={0,};
	d1[0] = pView->m_Project.GetPrecisionTime();

	m_ptCapCenter1.SetPoint(0,0);
	rt = pView->m_Display[0].m_rtImgTracker[3]; // Top Cap Left Tracker
	rt = CheckROI(rt,CRect(0,0,1280,960));
	rt1 = pView->m_Display[0].m_rtImgTracker[4]; // Top Cap Right Tracker
	rt1 = CheckROI(rt1,CRect(0,0,1280,960));
	double dstd1,dstd2,dvar1,dvar2;
	int nmin1,nmax1,nmin2,nmax2;
	double dave1 = GetRoiInfo(fm,CAMERA_WIDTH,rt,1,1,&dstd1,&nmin1,&nmax1,&dvar1,0);
	double dave2 = GetRoiInfo(fm,CAMERA_WIDTH,rt1,1,1,&dstd2,&nmin2,&nmax2,&dvar2,0);
	m_ptCapCenter.SetPoint(0,0);
	for(int i =0;i<CAMERA_HEIGHT;i++)		m_ptDrawCapEdge[0][i].x = m_ptDrawCapEdge[1][i].x = m_ptDrawCapEdge[0][i].y = m_ptDrawCapEdge[1][i].y = 0;
	int minx1=CAMERA_WIDTH,minx2=0,miny1=0,miny2=0,miny3=0;

	int nCapEdgeValue = pView->m_pDlgSet->m_FormTab4->m_nTopCapEdgeValue; // Top Cap Edge Left
	int noffset = 12;
	double dEdgeX[CAMERA_WIDTH],dEdgeY[CAMERA_WIDTH];
	for(int i=0;i<CAMERA_WIDTH;i++)dEdgeY[i]=dEdgeX[i] = 0;
	int nCnt = 0;
	// Top Cap Edge Left �˻�
	for(int i=rt.top;i<rt.bottom-4;i++)
	{
		for(int j=rt.left;j<rt.right-noffset;j++)
		{
			if(*(fm+i*CAMERA_WIDTH+j) - *(fm+i*CAMERA_WIDTH+j+noffset) >=nCapEdgeValue &&*(fm+(i+2)*CAMERA_WIDTH+j) - *(fm+(i+2)*CAMERA_WIDTH+j+noffset) >=nCapEdgeValue
				&&*(fm+(i+3)*CAMERA_WIDTH+j) - *(fm+(i+3)*CAMERA_WIDTH+j+noffset) >=nCapEdgeValue &&*(fm+(i+4)*CAMERA_WIDTH+j) - *(fm+(i+4)*CAMERA_WIDTH+j+noffset) >=nCapEdgeValue) //���
			{
				//���� �� ����
				if(*(fm+i*CAMERA_WIDTH+j) - *(fm+i*CAMERA_WIDTH+j+6*2) >=nCapEdgeValue && *(fm+(i+2)*CAMERA_WIDTH+j) - *(fm+(i+2)*CAMERA_WIDTH+j+6*2) >=nCapEdgeValue)
				{//�ǿ���� ����
					if(minx1>=j)minx1 = j,miny1=i;
					m_ptDrawCapEdge[0][i].SetPoint(j+noffset,i);
					dEdgeX[nCnt] = j;
					dEdgeY[nCnt]=i;
					nCnt++;
					break;
				}
			}
		}
	}
	d1[1] = pView->m_Project.GetPrecisionTime();

	// Top Cap Edge Right �˻�
	nCapEdgeValue = pView->m_pDlgSet->m_FormTab4->m_nTopCapEdgeValue1; // Top Cap Edge Right
	if(rt1.Width()>100) // ������ ������ Ʈ��Ŀ ũ�� �׷��߰ڴ� 
	{
		for(int i=rt1.top;i<rt1.bottom-4;i++)
		{
			for(int j=rt1.right;j>rt1.left+noffset;j--)
			{
				if(nCnt >= CAMERA_WIDTH)
					break;

				if(*(fm+i*CAMERA_WIDTH+j) - *(fm+i*CAMERA_WIDTH+j-noffset) >=nCapEdgeValue &&*(fm+(i+2)*CAMERA_WIDTH+j) - *(fm+(i+2)*CAMERA_WIDTH+j-noffset) >=nCapEdgeValue
					&&*(fm+(i+3)*CAMERA_WIDTH+j) - *(fm+(i+3)*CAMERA_WIDTH+j-noffset) >=nCapEdgeValue &&*(fm+(i+4)*CAMERA_WIDTH+j) - *(fm+(i+4)*CAMERA_WIDTH+j-noffset) >=nCapEdgeValue) //����
				{
					if(*(fm+i*CAMERA_WIDTH+j) - *(fm+i*CAMERA_WIDTH+j-(6*2)) >=nCapEdgeValue && *(fm+(i+2)*CAMERA_WIDTH+j) - *(fm+(i+2)*CAMERA_WIDTH+j-(6*2)) >=nCapEdgeValue)
					{
						int nv = 1;
						for(int k=j-6;k>j-20;k--)
						{
							if(*(fm+i*CAMERA_WIDTH+j) - *(fm+i*CAMERA_WIDTH+k)>=nCapEdgeValue && nv ==1)
								;
							else{
								nv = 0;
								break;
							}
						}
						// hoho : 3,4ȣ��� ������ �����ϱ�
						if(pView->m_strComname.Find("3A")>=0 ||pView->m_strComname.Find("3B")>=0 ||pView->m_strComname.Find("3C")>=0 
							|| pView->m_strComname.Find("4A")>=0 ||pView->m_strComname.Find("4B")>=0||pView->m_strComname.Find("4C")>=0
							|| pView->m_strComname.Find("5A")>=0 ||pView->m_strComname.Find("5B")>=0||pView->m_strComname.Find("5C")>=0
							|| pView->m_strComname.Find("6A")>=0 ||pView->m_strComname.Find("6B")>=0||pView->m_strComname.Find("6C")>=0
							|| pView->m_strComname.Find("7A")>=0 ||pView->m_strComname.Find("7B")>=0||pView->m_strComname.Find("7C")>=0
							|| pView->m_strComname.Find("8A")>=0 ||pView->m_strComname.Find("8B")>=0||pView->m_strComname.Find("8C")>=0)
							nv = 1;

						if(nv==1){
							if(minx2<j)minx2= j,miny2=i;
							m_ptDrawCapEdge[1][i].SetPoint(j-noffset,i);
							dEdgeX[nCnt] = j;dEdgeY[nCnt]=i;nCnt++;
							break;
						}
					}
				}
			}
		}
		//delete fm;

		m_dCapWidth = minx2-minx1;
	}
	else 
		m_dCapWidth = pView->m_pDlgSet->m_FormTab4->m_dTopCapSize;
	d1[2] = pView->m_Project.GetPrecisionTime();

	// Top Cap �߾� ��ġ(Hard Coding Version)
	m_ptCapCenter.x = minx1+m_dCapWidth*0.5;
	m_ptCapCenter.y = miny1;

	//CCircleFitting circleFit;
	double dRetX,dRetY,dRetR;
	circleFit.RobustCircleFit(dEdgeX, dEdgeY, nCnt, &dRetX, &dRetY, &dRetR, int(nCnt*0.5), 1);

	// Top Cap �߾� ��ġ(Circle Fitting Version)
	m_ptCapCenter1.x = dRetX;
	m_ptCapCenter1.y = dRetY;
	m_dCabRadius = dRetR;

	// Circle Fitting Version ����,
	if(pView->m_pDlgSet->m_FormTab4->m_bCircleFit)
	{
		m_ptCapCenter.x = dRetX;
		m_ptCapCenter.y = dRetY;
		nret = 1;

		if(pView->m_pDlgSet->m_FormTab5->m_bCalibration == TRUE){
			int nY = (int)dRetY;

			//for(int x=dRetX; x>1; x--){
			//	if(*(fm+nY*CAMERA_WIDTH+x) == 255){
			//		m_nTopCapleft = x;
			//		break;
			//	}
			//}

			//for(int x=dRetX; x<CAMERA_WIDTH-1; x++){
			//	if(*(fm+nY*CAMERA_WIDTH+x) == 255){
			//		m_nTopCapRight = x;
			//		break;
			//	}
			//}
			
			for(int x=0; x<CAMERA_WIDTH*0.5; x++){
				if(*(fm+nY*CAMERA_WIDTH+x) < 200 && *(fm+nY*CAMERA_WIDTH+(x+2)) < 200 && *(fm+nY*CAMERA_WIDTH+(x+4)) < 200){
					m_nTopCapleft = x;
					break;
				}
			}

			for(int x=CAMERA_WIDTH; x>CAMERA_WIDTH*0.5; x--){
				if(*(fm+nY*CAMERA_WIDTH+x) < 200 && *(fm+nY*CAMERA_WIDTH+(x-2)) < 200 && *(fm+nY*CAMERA_WIDTH+(x-4)) < 200){
					m_nTopCapRight = x;
					break;
				}
			}

			m_nTopCapleft1 = dRetX-m_dCabRadius;
			m_nTopCapRight1 = dRetX+m_dCabRadius;
			m_dCapWidth = (m_nTopCapRight-m_nTopCapleft) * pView->m_pDlgSet->m_FormTab2->m_dCamScale;
			m_dCapWidth1 = (m_dCabRadius*2) * pView->m_pDlgSet->m_FormTab2->m_dCamScale;
		}
	}
	else{
		// Top Cap �˻� ���. �߾���ġ, žĸ ������ �缳��
		if(minx1==CAMERA_WIDTH && minx2==0) // Top Cap ���� ��� ���� �� ã���� ���,
		{
			m_ptCapCenter.SetPoint(0,0);
			m_dCapWidth = 0;
			m_dCapWidth1 = 0;
			nret = 0;
			delete fm;
			return nret;
		}
		else if(minx1==CAMERA_WIDTH) // Top Cap ������ ��踸 ã���� ���,
		{
			m_ptCapCenter.x = minx2 - (pView->m_pDlgSet->m_FormTab4->m_dTopCapSize/pView->m_pDlgSet->m_FormTab2->m_dCamScale)*0.5-6;
			m_ptCapCenter.y = miny2;
			m_dCapWidth = minx2 - (pView->m_pDlgSet->m_FormTab4->m_dTopCapSize/pView->m_pDlgSet->m_FormTab2->m_dCamScale);
			nret = 1;
		}
		else if(minx2==0) // Top Cap ���� ��踸 ã���� ���,
		{
			m_ptCapCenter.x = minx1 + (pView->m_pDlgSet->m_FormTab4->m_dTopCapSize/pView->m_pDlgSet->m_FormTab2->m_dCamScale)*0.5+6;
			m_ptCapCenter.y = miny1;
			nret = 1;
		}
		else // Top Cap ���� ��� ���� ã���� ���,
		{
			minx2=minx1=0;
			for(int i=rt.top;i<rt.bottom;i++)
			{
				if(m_ptDrawCapEdge[1][i].x==0 || m_ptDrawCapEdge[0][i].x==0)continue;
				if(m_ptDrawCapEdge[1][i].x-m_ptDrawCapEdge[0][i].x >= minx1){
					minx1 = m_ptDrawCapEdge[1][i].x-m_ptDrawCapEdge[0][i].x ;
					miny3 = i; // Y
				}
			}	
			if(miny1 <= miny2 && miny1 <=miny3)
				m_ptCapCenter.y = (miny2+miny3)/2;
			else if(miny2 <= miny1 && miny2 <=miny3)
				m_ptCapCenter.y = (miny1+miny3)/2;
			else if(miny3 <= miny1 && miny3 <=miny2 && miny3>0)
				m_ptCapCenter.y = (miny1+miny2)/2;
			else if(miny3==0 )
				m_ptCapCenter.y = miny2;
			else	
				m_ptCapCenter.y = (miny1+miny2+miny3)/3;

			if(abs(miny1 - miny2)>10 && abs(miny2-miny3)<10)
				m_ptCapCenter.y = (miny3+miny2)/2;
			else if(abs(miny1 - miny2)<10 && abs(miny2-miny3)>10)
				m_ptCapCenter.y = (miny1+miny2)/2;
			else if(abs(miny1 - miny3)<10 && abs(miny2-miny3)>10)
				m_ptCapCenter.y = (miny1+miny3)/2;
			else
			{
				if(miny1>miny2 && miny2>miny3)
					m_ptCapCenter.y = (miny2);
				else if(miny1>miny3 && miny3>miny2)
					m_ptCapCenter.y = (miny3);
				else 
					m_ptCapCenter.y = (miny1);
			}

			m_ptCapCenter.x = (m_ptDrawCapEdge[0][miny1].x+m_ptDrawCapEdge[1][miny2].x)*0.5;

			if(pView->m_nMachine!=3)
				m_ptCapCenter.y +=10;
			nret = 1;
		}
	}

	// hoho ���� �ִ��� ����� �ű�
	delete fm;

	d1[3] = pView->m_Project.GetPrecisionTime();

// 	CString str;
// 	str.Format("Cap Center : %d, %d", m_ptCapCenter.x, m_ptCapCenter.y);
// 	pView->AddtoInspLog(str);

	int nTolY = pView->m_pDlgSet->m_FormTab4->m_dTapTolY1/pView->m_pDlgSet->m_FormTab2->m_dCamScale; // Top
	int nTolY1 = pView->m_pDlgSet->m_FormTab4->m_dTapTolY2/pView->m_pDlgSet->m_FormTab2->m_dCamScale; // Bottom
	int ntL = pView->m_pDlgSet->m_FormTab4->m_dTabTol/pView->m_pDlgSet->m_FormTab2->m_dCamScale; // Left
	int ntR = pView->m_pDlgSet->m_FormTab4->m_dTabTol1/pView->m_pDlgSet->m_FormTab2->m_dCamScale; // Right

	// Tab OK ���� ����
	m_rtTabOkRange.SetRect(m_ptCapCenter.x-ntL,m_ptCapCenter.y-nTolY,m_ptCapCenter.x+ntR,m_ptCapCenter.y+nTolY1);
	m_rtTabOkRange = CheckROI(m_rtTabOkRange,CRect(50,50,CAMERA_WIDTH-50,CAMERA_HEIGHT-10));

	if(m_rtTabOkRange.left<0 ||m_rtTabOkRange.top<0 ||m_rtTabOkRange.right>=CAMERA_WIDTH-1 ||m_rtTabOkRange.bottom>CAMERA_HEIGHT || m_ptCapCenter.y > 750)
		nret = 0;

	if(pView->m_pDlgSet->m_FormTab4->m_bWeldingRange){
		nTolY = (pView->m_pDlgSet->m_FormTab4->m_dWeldingDistanceY - pView->m_pDlgSet->m_FormTab4->m_dWeldingDistanceRange[0]) / pView->m_pDlgSet->m_FormTab2->m_dCamScale;
		nTolY1 = (pView->m_pDlgSet->m_FormTab4->m_dWeldingDistanceY + pView->m_pDlgSet->m_FormTab4->m_dWeldingDistanceRange[1]) / pView->m_pDlgSet->m_FormTab2->m_dCamScale;
		m_rtWeldingOkRange.SetRect(m_ptCapCenter.x-10, m_ptCapCenter.y + nTolY, m_ptCapCenter.x+10, m_ptCapCenter.y + nTolY1);
		m_rtWeldingOkRange = CheckROI(m_rtWeldingOkRange,CRect(50,50,CAMERA_WIDTH-50,CAMERA_HEIGHT-10));

		if(m_rtWeldingOkRange.left<0 ||m_rtWeldingOkRange.top<0 ||m_rtWeldingOkRange.right>=CAMERA_WIDTH-1 ||m_rtWeldingOkRange.bottom>CAMERA_HEIGHT)
			nret = 0;
	}

	//tracker ��ġ �̵�
	if(pView->m_pDlgSet->m_FormTab4->m_bRegMode == TRUE){
		int nH1 = pView->m_Display[0].m_rtImgTracker[5].Height();
		pView->m_Display[0].m_rtImgTracker[5].top = m_ptCapCenter.y+pView->m_pDlgSet->m_FormTab4->m_nRegYpos;
		pView->m_Display[0].m_rtImgTracker[5].bottom = m_ptCapCenter.y+pView->m_pDlgSet->m_FormTab4->m_nRegYpos + nH1;
	}
	if(pView->m_pDlgSet->m_FormTab4->m_bRegMode == TRUE && pView->m_pDlgSet->m_FormTab4->m_bMidWeld){
		int nH1 = pView->m_Display[0].m_rtImgTracker[6].Height();
		pView->m_Display[0].m_rtImgTracker[6].top = m_ptCapCenter.y+pView->m_pDlgSet->m_FormTab4->m_nRegYpos1;
		pView->m_Display[0].m_rtImgTracker[6].bottom = m_ptCapCenter.y+pView->m_pDlgSet->m_FormTab4->m_nRegYpos1 + nH1;
	}
	d1[4] = pView->m_Project.GetPrecisionTime();

	return nret;
}

CRect CgProject::CircleFind2(BYTE * fmorigin)
{
	int nret = 0;
	CRect rt,rt1;
	double dCx,dCy;
	BYTE * fm = new BYTE[CAMERA_WIDTH*CAMERA_HEIGHT];
	memcpy(fm,fmorigin,CAMERA_HEIGHT*CAMERA_WIDTH);
	double d1[10]={0,};
	d1[0] = pView->m_Project.GetPrecisionTime();

	m_ptCapCenter1.SetPoint(0,0);
	m_ptCapCenter.SetPoint(0,0);
	rt = pView->m_Display[0].m_rtImgTracker[3]; // Top Cap Left Tracker
	rt = CheckROI(rt,CRect(10,10,CAMERA_WIDTH-10,CAMERA_HEIGHT-10));
	rt1 = pView->m_Display[0].m_rtImgTracker[4]; // Top Cap Right Tracker
	rt1 = CheckROI(rt1,CRect(10,10,CAMERA_WIDTH-10,CAMERA_HEIGHT-10));

	double dstd1,dstd2,dvar1,dvar2;
	CRect rttab;
	CRect rtSharpnessRoi;
	rtSharpnessRoi = pView->m_Display[0].m_rtImgTracker[7];
	rtSharpnessRoi = pView->m_Project.CheckROI(rtSharpnessRoi,CRect(10,10,CAMERA_WIDTH-10,CAMERA_HEIGHT-10));
	rttab.SetRect(0,0,0,0);
	int nmin1,nmax1,nmin2,nmax2;
	double dave1 = GetRoiInfo(fm,CAMERA_WIDTH,rt,1,1,&dstd1,&nmin1,&nmax1,&dvar1,0);//��⸦ �� ������?
	double dave2 = GetRoiInfo(fm,CAMERA_WIDTH,rt1,1,1,&dstd2,&nmin2,&nmax2,&dvar2,0);
	if(dave1<10 || dave2<10){
// 		memcpy(fm,pView->m_Project.m_MultiBuf[0],CAMERA_HEIGHT*CAMERA_WIDTH);
// 		dave1 = GetRoiInfo(fm,CAMERA_WIDTH,rt,1,1,&dstd1,&nmin1,&nmax1,&dvar1,0);//��⸦ �� ������?
// 		dave2 = GetRoiInfo(fm,CAMERA_WIDTH,rt1,1,1,&dstd2,&nmin2,&nmax2,&dvar2,0);

		if(dave1<10 || dave2<10){
			delete fm;
			return rttab;
		}
	}
	for(int i =0;i<CAMERA_WIDTH;i++)		m_ptDrawCapEdge[0][i].x = m_ptDrawCapEdge[1][i].x = m_ptDrawCapEdge[0][i].y = m_ptDrawCapEdge[1][i].y = 0;
	int minx1=CAMERA_WIDTH,minx2=0,miny1=0,miny2=0,miny3=0;

	int nCapEdgeValue = pView->m_pDlgSet->m_FormTab4->m_nTopCapEdgeValue; // Top Cap Edge Left
	int noffset = 12;
	double dEdgeX[CAMERA_WIDTH],dEdgeY[CAMERA_WIDTH];
	double dEdgeX1[CAMERA_WIDTH],dEdgeY1[CAMERA_WIDTH];
	for(int i=0;i<CAMERA_WIDTH;i++)dEdgeY[i]=dEdgeX[i] =dEdgeY1[i]=dEdgeX1[i] = 0;
	int nCnt = 0;
	int nEdgeTh = dave1*0.3;//value ���� Ȯ��.
	int nEdgeTh1 = dave2*0.3;//value ���� Ȯ��.
	if(nEdgeTh<10)nEdgeTh = 10;
	if(nEdgeTh1<10)nEdgeTh1 = 10;

	//Sharpness Data ���� woo 201203
	int nSharpnessGap=4;
	int nw = CAMERA_WIDTH;
	int nSharpnessHeight = rtSharpnessRoi.Height();
	double  dmax1,dsharp1;
	dmax1 = 0;
	dsharp1 = 0;
	m_dSharpness = 0;
	for (int y = rtSharpnessRoi.top ; y<rtSharpnessRoi.bottom-nSharpnessGap ; y++)
	{
		for (int x = rtSharpnessRoi.left ; x<rtSharpnessRoi.right-nSharpnessGap ; x++)
		{
			if(dmax1< abs(*(fm+y*nw+x) - *(fm+y*nw+x-nSharpnessGap)))
				dmax1 = abs(*(fm+y*nw+x) - *(fm+y*nw+x-nSharpnessGap));

		}
		dsharp1 += dmax1;
	}
	if (nSharpnessHeight <= 0)
		nSharpnessHeight = 1;
	m_dSharpness = dsharp1/nSharpnessHeight; //woo �����Ͻ���
	CString strSharpness;
	strSharpness.Format("SharpNess:%.2f",m_dSharpness);
	pView->AddtoInspLog1(strSharpness, 1);

	// Top Cap Edge Left �˻�
	int nCapEdgeAve = 0,nedgecnt1=0,nedgecnt2=0,nedgemargin=100,nFlag1=0,nedgeth11=10;
	for(int i=rt.top;i<rt.bottom-4;i++)//top cap reverse
	{
		nFlag1 = 0;
		for(int j=rt.left;j<rt.right-noffset;j++)
		{
			if(nCnt >= CAMERA_WIDTH)			break;
			if(*(fm+i*CAMERA_WIDTH+j) - *(fm+i*CAMERA_WIDTH+j+noffset) >=nEdgeTh &&*(fm+(i+2)*CAMERA_WIDTH+j) - *(fm+(i+2)*CAMERA_WIDTH+j+noffset) >=nEdgeTh
				&&*(fm+(i+3)*CAMERA_WIDTH+j) - *(fm+(i+3)*CAMERA_WIDTH+j+noffset) >=nEdgeTh &&*(fm+(i+4)*CAMERA_WIDTH+j) - *(fm+(i+4)*CAMERA_WIDTH+j+noffset) >=nEdgeTh) //���
			{
				if(minx1>=j)minx1 = j,miny1=i;
				m_ptDrawCapEdge[0][i].SetPoint(j+noffset,i);
				dEdgeX[nCnt] = j+noffset;		dEdgeY[nCnt]=i;			nCnt++;nFlag1=j+noffset;
				break;
			}
		}
		if(nFlag1>0 && pView->m_pDlgSet->m_FormTab4->m_nCapReverseV>0){
			for(int j=nFlag1+50;j<nFlag1+nedgemargin;j++)
			{
				nCapEdgeAve += *(fm+i*CAMERA_WIDTH+j);
				nedgecnt1++;
				if( *(fm+i*CAMERA_WIDTH+j) -  *(fm+i*CAMERA_WIDTH+j+4)>nedgeth11)
					nedgecnt2++;
			}
		}
		nFlag1 = 0;
		for(int j=rt1.right;j>rt1.left+noffset;j--)
		{
			if(nCnt >= CAMERA_WIDTH)			break;
			if(*(fm+i*CAMERA_WIDTH+j) - *(fm+i*CAMERA_WIDTH+j-noffset) >=nEdgeTh1 &&*(fm+(i+2)*CAMERA_WIDTH+j) - *(fm+(i+2)*CAMERA_WIDTH+j-noffset) >=nEdgeTh1
				&&*(fm+(i+3)*CAMERA_WIDTH+j) - *(fm+(i+3)*CAMERA_WIDTH+j-noffset) >=nEdgeTh1 &&*(fm+(i+4)*CAMERA_WIDTH+j) - *(fm+(i+4)*CAMERA_WIDTH+j-noffset) >=nEdgeTh1) //����
			{

				if(minx2<j)minx2= j,miny2=i;
				m_ptDrawCapEdge[1][i].SetPoint(j-noffset,i);
				dEdgeX[nCnt] = j-noffset;dEdgeY[nCnt]=i;nCnt++;nFlag1 = j-noffset;
				break;
			}
		}
		if(nFlag1>0 && pView->m_pDlgSet->m_FormTab4->m_nCapReverseV>0){
			for(int j=nFlag1-50;j>nFlag1-nedgemargin;j--)
			{
				nCapEdgeAve += *(fm+i*CAMERA_WIDTH+j);
				nedgecnt1++;
				if( *(fm+i*CAMERA_WIDTH+j) -  *(fm+i*CAMERA_WIDTH+j-4)>nedgeth11)
					nedgecnt2++;
			}
		}
	}
	if(nedgecnt1>0)
		nCapEdgeAve /= nedgecnt1;
	else nCapEdgeAve = 0;
	pView->m_nTopCapV = nCapEdgeAve;
	m_dCapWidth = minx2-minx1;
	d1[1] = pView->m_Project.GetPrecisionTime();

// 	// Top Cap �߾� ��ġ(Hard Coding Version)
// 	m_ptCapCenter.x = minx1+m_dCapWidth*0.5;
// 	m_ptCapCenter.y = miny1;

	//CCircleFitting circleFit;
	double dRetX,dRetY,dRetR;
	circleFit.RobustCircleFit(dEdgeX, dEdgeY, nCnt, &dRetX, &dRetY, &dRetR, int(nCnt*0.5), 1);

	// Top Cap �߾� ��ġ(Circle Fitting Version)
	m_ptCapCenter1.x = dRetX;
	m_ptCapCenter1.y = dRetY;
	m_dCabRadius = dRetR;

	// Circle Fitting Version ����,
	m_ptCapCenter.x = dRetX;
	m_ptCapCenter.y = dRetY;
	nret = 1;


	if(pView->m_pDlgSet->m_FormTab5->m_bCalibration == TRUE){
		int nY = (int)dRetY;
		for(int x=0; x<CAMERA_WIDTH*0.5; x++){
			if(*(fm+nY*CAMERA_WIDTH+x) < 200 && *(fm+nY*CAMERA_WIDTH+(x+2)) < 200 && *(fm+nY*CAMERA_WIDTH+(x+4)) < 200){
				m_nTopCapleft = x;
				break;
			}
		}
		for(int x=CAMERA_WIDTH; x>CAMERA_WIDTH*0.5; x--){
			if(*(fm+nY*CAMERA_WIDTH+x) < 200 && *(fm+nY*CAMERA_WIDTH+(x-2)) < 200 && *(fm+nY*CAMERA_WIDTH+(x-4)) < 200){
				m_nTopCapRight = x;
				break;
			}
		}
		m_nTopCapleft1 = dRetX-m_dCabRadius;
		m_nTopCapRight1 = dRetX+m_dCabRadius;
		m_dCapWidth = (m_nTopCapRight-m_nTopCapleft) * pView->m_pDlgSet->m_FormTab2->m_dCamScale;
		m_dCapWidth1 = (m_dCabRadius*2) * pView->m_pDlgSet->m_FormTab2->m_dCamScale;
	}

	int nMarginx = 200,nMarginy = 200,nMarginy1 = 150;
	//top cap �߽ɺ�
	//if(m_ptCapCenter.x>200 && m_ptCapCenter.x <CAMERA_WIDTH-200 &&m_ptCapCenter.y>200 && m_ptCapCenter.y <CAMERA_HEIGHT-200){
	if(m_ptCapCenter.x>200 && m_ptCapCenter.x <CAMERA_WIDTH-200 &&m_ptCapCenter.y>30 && m_ptCapCenter.y <CAMERA_HEIGHT-200){
		//		if(pView->m_pDlgSet->m_FormTab4->m_b2Tab)	
		if(pView->m_pDlgSet->m_FormTab4->m_bTwoTab)	
			rttab.SetRect(m_ptCapCenter.x-nMarginx,m_ptCapCenter.y-210,m_ptCapCenter.x+nMarginx,m_ptCapCenter.y-110);
		else
			rttab.SetRect(m_ptCapCenter.x-nMarginx,m_ptCapCenter.y-nMarginy1,m_ptCapCenter.x+nMarginx,m_ptCapCenter.y+nMarginy);
	}

	// hoho ���� �ִ��� ����� �ű�
	delete fm;

	if(nCnt<100)
		rttab.SetRect(0,0,0,0);
	d1[3] = pView->m_Project.GetPrecisionTime();

	// 	CString str;
	// 	str.Format("Cap Center : %d, %d", m_ptCapCenter.x, m_ptCapCenter.y);
	// 	pView->AddtoInspLog(str);

 	int nTolY = pView->m_pDlgSet->m_FormTab4->m_dTapTolY1/pView->m_pDlgSet->m_FormTab2->m_dCamScale; // Top
 	int nTolY1 = pView->m_pDlgSet->m_FormTab4->m_dTapTolY2/pView->m_pDlgSet->m_FormTab2->m_dCamScale; // Bottom
 	int ntL = pView->m_pDlgSet->m_FormTab4->m_dTabTol/pView->m_pDlgSet->m_FormTab2->m_dCamScale; // Left
 	int ntR = pView->m_pDlgSet->m_FormTab4->m_dTabTol1/pView->m_pDlgSet->m_FormTab2->m_dCamScale; // Right
 

 	// Tab OK ���� ����
// 	if(pView->m_pDlgSet->m_FormTab4->m_bTwoTab)//m_n2TabOffsetY   // woo
//  		m_rtTabOkRange.SetRect(m_ptCapCenter.x-ntL,m_ptCapCenter.y-nTolY-pView->m_pDlgSet->m_FormTab4->m_n2TabOffsetY,m_ptCapCenter.x+ntR,m_ptCapCenter.y+nTolY1-pView->m_pDlgSet->m_FormTab4->m_n2TabOffsetY);
// 	else
		m_rtTabOkRange.SetRect(m_ptCapCenter.x-ntL,m_ptCapCenter.y-nTolY,m_ptCapCenter.x+ntR,m_ptCapCenter.y+nTolY1);

 	m_rtTabOkRange = CheckROI(m_rtTabOkRange,CRect(50,50,CAMERA_WIDTH-50,CAMERA_HEIGHT-10));
 
 	if(m_rtTabOkRange.left<0 ||m_rtTabOkRange.top<0 ||m_rtTabOkRange.right>=CAMERA_WIDTH-1 ||m_rtTabOkRange.bottom>CAMERA_HEIGHT || m_ptCapCenter.y > 750)
 		rttab.SetRect(0,0,0,0);
// 
// 	if(pView->m_pDlgSet->m_FormTab4->m_bWeldingRange){
// 		nTolY = (pView->m_pDlgSet->m_FormTab4->m_dWeldingDistanceY - pView->m_pDlgSet->m_FormTab4->m_dWeldingDistanceRange[0]) / pView->m_pDlgSet->m_FormTab4->m_dCamScale;
// 		nTolY1 = (pView->m_pDlgSet->m_FormTab4->m_dWeldingDistanceY + pView->m_pDlgSet->m_FormTab4->m_dWeldingDistanceRange[1]) / pView->m_pDlgSet->m_FormTab4->m_dCamScale;
// 		m_rtWeldingOkRange.SetRect(m_ptCapCenter.x-10, m_ptCapCenter.y + nTolY, m_ptCapCenter.x+10, m_ptCapCenter.y + nTolY1);
// 		m_rtWeldingOkRange = CheckROI(m_rtWeldingOkRange,CRect(50,50,CAMERA_WIDTH-50,CAMERA_HEIGHT-10));
// 
// 		if(m_rtWeldingOkRange.left<0 ||m_rtWeldingOkRange.top<0 ||m_rtWeldingOkRange.right>=CAMERA_WIDTH-1 ||m_rtWeldingOkRange.bottom>CAMERA_HEIGHT)
// 			nret = 0;
// 	}
// 
// 	//tracker ��ġ �̵�
// 	if(pView->m_pDlgSet->m_FormTab4->m_bRegMode == TRUE){
// 		int nH1 = pView->m_Display[0].m_rtImgTracker[5].Height();
// 		pView->m_Display[0].m_rtImgTracker[5].top = m_ptCapCenter.y+pView->m_pDlgSet->m_FormTab4->m_nRegYpos;
// 		pView->m_Display[0].m_rtImgTracker[5].bottom = m_ptCapCenter.y+pView->m_pDlgSet->m_FormTab4->m_nRegYpos + nH1;
// 	}
	d1[4] = pView->m_Project.GetPrecisionTime();

	return rttab;
}

int CgProject::findCircleWidth(BYTE * fm,CRect rt,CRect rt1,int ncx,int ncy,int* nw1,int* nw2,int* nh1,int* nh2)
{
	int i,j,ntmp = 0,ntmp1 = 0,ntmp2 = 0,nl,nr,nt,nb;
	double dtmp,dc,dtmpold,dsum;
	dtmp=dc=dtmpold=dsum = 0;
	int margin = 5;
	int	npitch[CAMERA_WIDTH],npitch1[CAMERA_WIDTH];
	int	nHight[CAMERA_HEIGHT],nHight1[CAMERA_HEIGHT];
	for(i=0;i<CAMERA_WIDTH;i++) npitch[i]=npitch1[i] = 0;
	for(i=0;i<CAMERA_HEIGHT;i++) nHight[i]=nHight1[i] = 0;
	for(i=rt.left;i<rt.right;i++)
	{
		ntmp = 0;
		for(j=ncy-margin;j<ncy+margin;j++)
		{
			npitch[i] += *(fm+j*CAMERA_WIDTH+i);// fm[j*CAMERA_WIDTH+i];
		}
	}
	for(i=rt.top;i<rt.bottom;i++)
	{
		ntmp = 0;
		for(j=ncx-margin;j<ncx+margin;j++)
		{
			nHight[i] += *(fm+i*CAMERA_WIDTH+j);// fm[i*CAMERA_WIDTH+j];
		}
	}

	for(i=rt.left;i<rt.right-5;i++)
		npitch1[i] = abs(npitch[i]-npitch[i+4]);

	for(i=rt.top;i<rt.bottom-5;i++)
		nHight1[i] = abs(nHight[i]-nHight[i+4]);
	
	//left point
	ntmp = 0;
	for(i=rt.left;i<rt1.left;i++)
	{
		if(ntmp <= npitch1[i])
		{
			ntmp = npitch1[i];		nl = i+3;
		}
	}
	ntmp = 0;
	for(i=rt.right;i>rt1.right;i--)
	{
		if(ntmp < npitch1[i])
		{
			ntmp = npitch1[i];	nr = i;
		}
	}
	ntmp = 0;
	for(i=rt.top;i<rt1.top;i++)
	{
		if(ntmp <= nHight1[i])
		{
			ntmp = nHight1[i];	nt = i+3;
		}
	}
	ntmp = 0;
	for(i=rt.bottom;i>rt1.bottom;i--)
	{
		if(ntmp <= nHight1[i])
		{
			ntmp = nHight1[i];	nb = i;
		}
	}
	*nw1 = nl;
	*nw2 = nr;
	*nh1 = nt;
	*nh2 = nb;

	return 0;
}

double CgProject::GetHistoAve(BYTE * fm,int *nMin,int *nMax,double *dAve,int noffx,int noffy,int* nprox,int* nproy)
{
	int i,j,ntmp;
	double dc,dtmp,dave;
	int nHisto[256];
	for(i=0;i<256;i++)nHisto[i] = 0;
	dc=dtmp =dave= 0;
	noffx = 150;
	noffy = 50;
	CRect rt;
	rt.SetRect(290,130,CAMERA_WIDTH-290,CAMERA_HEIGHT-130);//700 x 700
	int nprojx[CAMERA_WIDTH],nprojy[CAMERA_WIDTH];
	for(i=0;i<CAMERA_WIDTH;i++)	nprojx[i]=nprojy[i]=0;


	for(j=rt.top;j<rt.bottom;j++)
		for(i=rt.left;i<rt.right;i++)
		{
			ntmp = fm[i+CAMERA_WIDTH*j];
			nHisto[ntmp]++;
			dtmp += ntmp;
			nprojy[j]+=ntmp;
			nprojx[i]+=ntmp;
			dc++;
		}
		dave = dtmp/dc;	

		int nhsitoGap = 100;
		int nold,nold1;
		int nmin = 0,nmax = 0;
		nold = nold1 = 0;
		int nping1,nping2;
		nping2 = nping1 =0;
		for(i=0;i<dave-5;i++)
		{
			if(nHisto[i]>nold && nHisto[i+2] < nHisto[i] && nping1 ==0 )
			{
				nold = i;
				nmin = nold;
				nping1 = 1;
			}
		}
		nold = 0;
		for(i=255;i>dave+5;i--)
		{
			if(nHisto[i]>nold && nping2 ==0)
			{
				nold = i; 
				nmax = nold;
				nping2 =1;
			}
		}
		*nMax = nmax;
		*nMin = nmin;
		*dAve = dave;

		int ntmpx,ntmpy;
		ntmpx=ntmpy=0;
		for(i=noffx;i<CAMERA_WIDTH-noffx;i++)
		{
			ntmpx+=nprojy[j]-nprojy[i-2];
			ntmpy+=nprojx[j]-nprojx[i-2];
		}
		* nprox =abs(ntmpx);
		* nproy = abs(ntmpy);
		if(* nprox>* nproy)
			m_nDirection = 0;//���ι��� 
		else
			m_nDirection = 1;//���ι���
		return 0;
}

CPoint CgProject::GetCog(BYTE * fm,CRect rt,int xwidth,int ywidth)
{
	CPoint pt;
	int i,j,ntmp;
	double dx,dy,dc=0;
	dx = dy = 0;
	for(i=rt.top;i<rt.bottom;i++)
	{
		for(j=rt.left;j<rt.right;j++)
		{
			ntmp = fm[i*CAMERA_WIDTH+j];
			if(ntmp<10)
			{
				dx += j;
				dy += i;
				dc++;
			}
		}
	}
	if(dc)
	{
		dx = dx/dc;
		dy = dy/dc;
	}
	else
		dx=dy=0;

	int nBlack = 0,nsx,nex,nsy,ney;
	dx=dy=dc=0;
	nBlack = nsx = nex = 0;
	if(m_nDirection==0)
		for(i=rt.top;i<rt.bottom;i++)
		{
			nBlack = nsx = nex = 0;
			for(j=rt.left;j<rt.right;j++)
			{
				ntmp = fm[i*xwidth+j];
				if(ntmp<10 && nBlack==0)
				{
					nsx =nex= j;
					nex = j+1;
					nBlack = 1;
				}
				else if(ntmp <10)// && nBlack==1)
					nex = j;
				else
				{
					if( nex-nsx > xwidth)
					{
						dx += nsx+(nex-nsx)*0.5;		dc++;
					}
					else //if(nsx>0 && nex>0)
					{
						for(int k=nsx-1;k<nex+1;k++)
							fm[i*CAMERA_WIDTH+k] = 255;
					}
					nsx = nex = 0;
					nBlack = 0;
				}
			}
			//		if(dc)		dx = dx/dc;
		}
	else
	{
		dc = 0;
		for(j=rt.left;j<rt.right;j++)
		{
			nBlack =nsy=ney= 0;
			for(i=rt.top;i<rt.bottom;i++)
			{
				ntmp = fm[i*xwidth+j];
				if(ntmp<10 && nBlack==0)
				{
					nsy = ney=i;
					ney = i+1;
					nBlack = 1;
				}
				else if(ntmp <10 && nBlack==1)
					ney = i;
				else
				{
					if(ney-nsy > ywidth)
					{
						dx += nsx+(nex-nsx)*0.5;			dc++;
					}
					else 
					{
						if(nsy<=0)nsy = 1;
						for(int k=nsy-1;k<ney+1;k++)
							fm[k*CAMERA_WIDTH+j] = 255;
					}
					ney=nsy = 0;					nBlack = 0;
				}
			}
		}
	}
	pt.SetPoint(dx,dy);

	return pt;
}

int CgProject::FindTabPosition(CRect rt)
{
	int nret = 0;
	CString str;
	CRect rtBottom;
	m_rtTabOkRange = CheckROI(m_rtTabOkRange,CRect(1,1,CAMERA_WIDTH-1,CAMERA_HEIGHT-1));
	rtBottom = m_rtTabOkRange;
	rtBottom.left = m_ptCapCenter.x-40;
	rtBottom.right = m_ptCapCenter.x+40;
	int nProj[CAMERA_HEIGHT];
	for(int i=0;i<CAMERA_HEIGHT;i++)nProj[i]=m_nTabProjPos[i]=0;
	for (int i=0;i<2;i++)		for(int j=0;j<CAMERA_HEIGHT;j++)			m_ptTab[i][j].SetPoint(0,0);

	//�ϴ� Ʈ��Ŀ1�� ��ġ���� ���� �˻� ũ���Ť��� ũ��� 1000�� ������ �ȵô�
	CRect rttmp1,rttmp2;
	rttmp1 = pView->m_Display[0].m_rtImgTracker[0];//Ʈ��Ŀ1�� ��ġ�� �޾ƿ´�
	rttmp2 = pView->m_Display[0].m_rtImgTracker[0];//Ʈ��Ŀ1�� ��ġ�� �޾ƿ´�
	rttmp2 = CheckROI(rttmp2,CRect(1,1,CAMERA_WIDTH-1,CAMERA_HEIGHT-1));
	int npisX[1400],npisX1[1400];
	for(int i=0;i<1400;i++)npisX[i]=npisX1[i]=0;
	m_nBottomEdge[0]=m_nBottomEdge[1]=0;
	if(rttmp2.Height()<1){
		pView->AddtoInspLog("Tracker1 Roi Err");
		return 1;
	}

	// Tab OK ���� ��� ���
	double dtmp1=0,dtmp2=0;
	for(int i=m_rtTabOkRange.top;i<m_rtTabOkRange.bottom;i++)
	{
		for(int j=m_rtTabOkRange.left;j<m_rtTabOkRange.right;j++)
		{
			dtmp1 += *(m_fm[0]+i*CAMERA_WIDTH+j);
			dtmp2++;
		}
	}
	if(dtmp2>0)
		dtmp1 /= dtmp2;

	// Tracker 1(Tab �ϴ�)���� Tab OK ���� ��� ��⺸�� 20 �̻� ���� �͵��� Tab OK ���� ��� ���� ġȯ
	int ntmp1 = 0;
	for(int i=rttmp2.left;i<rttmp2.right;i++)
	{
		for(int j=rttmp2.top;j<rttmp2.bottom;j++){
			ntmp1 = *(m_fm[0]+j*CAMERA_WIDTH+i);
			if(ntmp1>dtmp1+20)
				ntmp1 = dtmp1;
			npisX1[i] += ntmp1;
		}
		npisX1[i] /= rttmp2.Height(); // X�� ��������(?)
	}
	
	// X�� �������� �Ѱ� �ٽ� 3���� ���� ���
	for(int i=rttmp2.left;i<rttmp2.right-3;i++)
	{
		if(npisX1[i]>0)
			npisX[i] = (npisX1[i]+npisX1[i+1]+npisX1[i+2])*0.33;
		else
			npisX[i] = 0;
	}

	int nmax1=0,nmax2=0,npos1=0,npos2=0,nflag=0;
	int gap = 16,ntapmax = 0,ntappos=0;
	int noff = 0;
	// hoho 1ȣ�� ���ϴ� ������ 0����
	//if(pView->m_nMachine==3)
	//	noff = -50;
	int nright1 = rttmp2.left;
	int nnv=pView->m_pDlgSet->m_FormTab4->m_nTapEdge;//8
	int ns = m_ptCapCenter.x+noff;
	if(ns <rttmp2.left-gap)
		ns = rttmp2.left+gap+2;
	
	// Tab �ϴ� ���� ��� ã��. Top Cap �߽�X���� �������� ���鼭 ã��
	for(int i=m_ptCapCenter.x+noff;i>rttmp2.left+gap;i--)
	{
		if(npisX[i-gap]==0)
			continue;

		// [1] ������ ����� �ִ밪�� �� ��ġ
		if(pView->m_pDlgSet->m_FormTab4->m_bTabBottomAbsMode){
			if(nmax1<abs(npisX[i] - npisX[i-gap])){
				nmax1 = abs(npisX[i] - npisX[i-gap]);
				//npos1=i-gap;
				npos1=i-(int)(gap/2);
			}
		}
		else{ // hoho �� �˻� W2B
			if(nmax1< npisX[i] - npisX[i-gap] ){
				nmax1 = npisX[i] - npisX[i-gap];
				//npos1=i-gap;
				npos1=i-(int)(gap/2);
			}
		}

		// [2] ������ Tab Edge L�� �̻� ���̰� �ִ�, ���� �߽ɿ��� ����� ���� ��ġ
		if(abs(npisX[i] - npisX[i-gap])>=nnv && nflag ==0){
			//m_nBottomEdge[0] = i-gap;
			m_nBottomEdge[0] = i-(int)(gap/2);
			nflag = abs(npisX[i] - npisX[i-gap]);
		}

		if(npos1>=530 && npos1 <= 545)
			npos1 = npos1;
	}

	// ������ Tab Edge L������ ū ���̰� �ִ°� ������(nflag=0) [1]�� ������ // [1],[2]�� ���� ���̰� �������� [2]�� ������
	if(pView->m_pDlgSet->m_FormTab4->m_bTabBottomAbsMode){
		if(nflag<nmax1-5 && pView->m_nMachine != 1)
			m_nBottomEdge[0] = npos1;
	}
	else{
		m_nBottomEdge[0] = npos1;
	}
	
//	if(pView->m_strComname.Find("CH")>=0)
//		pView->WriteProfileLog(0,6,1000,npisX,"");
	
	ntapmax = nflag=ntappos=0;

	// ��ã�� ��� [1]�� ��ġ ǥ��
	if(m_nBottomEdge[0]==0 /*|| nmax1>18*/)
		m_nBottomEdge[0] = npos1; 
	int nnv1=pView->m_pDlgSet->m_FormTab4->m_nTapEdge1;//8


	// Tab �ϴ� ���� ��� ã��. ������ ã�� ���� ���+100 ���� ���������� �˻�
	for(int i=m_nBottomEdge[0]+100;i<rttmp2.right-gap-4;i++)
	{
		// [1] ������ ����� �ִ밪�� �� ��ġ
		if(nmax2<abs(npisX[i+gap] - npisX[i])){
			nmax2 = abs(npisX[i+gap] - npisX[i]);
			npos2=i+(int)(gap/2);
			//npos2=i+gap;
		}
		// [2] ������ Tab Edge R�� �̻� ���̰� �ִ�, ���� �߽ɿ��� ����� ���� ��ġ
		if(abs(npisX[i+gap] - npisX[i])>=nnv1 && nflag==0){
			nflag = abs(npisX[i+gap] - npisX[i]);
			m_nBottomEdge[1] = i+(int)(gap/2);
			//m_nBottomEdge[1] = i+gap;
		}
	}
	
	// [2]�� ���ų�, [1]���� 18 �̻��̸�, 
	if(m_nBottomEdge[1]==0 || nmax2>18)
		m_nBottomEdge[1] = npos2;


	// Tab Refind üũ �Ǹ�. 3,4ȣ�� �ش� ����
	//if(pView->m_pDlgSet->m_FormTab4->m_bLowBlackSkip)
	//{
	//	nmax1 = nmax2=0;npos1=npos2=0;
	//	for(int i=rttmp2.left;i<rttmp2.right-gap-2;i++)
	//	{
	//		if(abs(npisX[i+gap] - npisX[i])>nmax1)
	//		{
	//			nmax1 = abs(npisX[i+gap] - npisX[i]);
	//			npos1 = i;
	//			i+=2;
	//		}
	//		else if(abs(npisX[i+gap] - npisX[i])>nmax2 && abs(npisX[i+gap] - npisX[i])<nmax1)
	//		{
	//			nmax2 = abs(npisX[i+gap] - npisX[i]);
	//			npos2 = i;
	//		}

	//	}
	//	//recheck
	//	int nmax3 =0, nmax4=0,npos3=0,npos4=0,nmax5 =0, nmax6=0,npos5=0,npos6=0;
	//	for(int i=rttmp2.left;i<npos1-12;i++)
	//	{
	//		if(i>710)
	//			i=i;
	//		if(abs(npisX[i+gap] - npisX[i])>nmax3)
	//		{
	//			nmax3 = abs(npisX[i+gap] - npisX[i]);
	//			npos3 = i;
	//			i+=2;
	//		}
	//		else if(abs(npisX[i+gap] - npisX[i])>nmax4 && abs(npisX[i+gap] - npisX[i])<nmax3)
	//		{
	//			nmax4 = abs(npisX[i+gap] - npisX[i]);
	//			npos4 = i;
	//		}

	//	}
	//	for(int i=npos1+12;i<rttmp2.right-gap-2;i++)
	//	{
	//		if(abs(npisX[i+gap] - npisX[i])>nmax5)
	//		{
	//			nmax5 = abs(npisX[i+gap] - npisX[i]);
	//			npos5 = i;
	//			i+=2;
	//		}
	//		else if(abs(npisX[i+gap] - npisX[i])>nmax6 && abs(npisX[i+gap] - npisX[i])<nmax5)
	//		{
	//			nmax6 = abs(npisX[i+gap] - npisX[i]);
	//			npos6 = i;
	//			i+=2;
	//		}
	//	}
	//	int nn1 = pView->m_pDlgSet->m_FormTab4->m_dTapSize/pView->m_pDlgSet->m_FormTab4->m_dCamScale;
	//	if(npos1-m_ptCapCenter.x>50 && nmax1 > 18)
	//	{
	//		if(m_nBottomEdge[0] > npos1-nn1-20 && m_nBottomEdge[0] < npos1-nn1+20)
	//		{
	//		}
	//		else
	//		{
	//			m_nBottomEdge[1] = npos1;
	//			m_nBottomEdge[0] = npos1-nn1;
	//			pView->AddtoInspLog("Change Tab Pos1");
	//		}
	//	}
	//	else 
	//	{
	//		if(nmax1<=18)
	//			;
	//		else
	//		{
	//			if(m_nBottomEdge[1] > npos1+nn1-20 && m_nBottomEdge[1] < npos1+nn1+20 && nmax3<25)
	//			{
	//			}
	//			else
	//			{
	//				if(nmax3>20)
	//				{
	//					m_nBottomEdge[1] = npos1;
	//					m_nBottomEdge[0] = npos3;
	//					pView->AddtoInspLog("Change Tab Pos1");
	//				}
	//				else
	//				{
	//					m_nBottomEdge[0] = npos1;
	//					m_nBottomEdge[1] = npos1+nn1;
	//					pView->AddtoInspLog("Change Tab Pos2");
	//				}

	//			}
	//		}
	//	}
	//}

	// �߽��� ���� �¿츦 üũ�Ͽ� ������ ã�´� - center under because tap Length short
	// Tab ��� �¿��踦 ã�� ���� ���� ����, ���� ���� ����
	CRect rt1,rt2;

	int noffset = 60;
	rt1.SetRect(100,100,100,100);
	rt2.SetRect(100,100,100,100);
	rt1.SetRect(m_nBottomEdge[0]-noffset,m_ptCapCenter.y+20,m_nBottomEdge[0]+noffset,m_ptCapCenter.y+200);
	if(m_nBottomEdge[1]>0)
		rt2.SetRect(m_nBottomEdge[1]-noffset,m_ptCapCenter.y+20,m_nBottomEdge[1]+noffset,m_ptCapCenter.y+200);
	else // ??
		rt2.SetRect(m_nBottomEdge[0]+noffset,m_ptCapCenter.y+20,m_nBottomEdge[0]+noffset,m_ptCapCenter.y+200);
	
	if(pView->m_pDlgSet->m_FormTab4->m_b2Tab){
		rt1.SetRect(m_nBottomEdge[0]-noffset,m_ptCapCenter.y-200,m_nBottomEdge[0]+noffset,m_ptCapCenter.y-20);
		rt2.SetRect(m_nBottomEdge[1]-noffset,m_ptCapCenter.y-200,m_nBottomEdge[1]+noffset,m_ptCapCenter.y-20);
	}

	rt1 = CheckROI(rt1,CRect(50,50,CAMERA_WIDTH-50,CAMERA_HEIGHT-10));
	rt2 = CheckROI(rt2,CRect(50,50,CAMERA_WIDTH-50,CAMERA_HEIGHT-10));

	if(pView->m_nMachine==3 && pView->m_pDlgSet->m_FormTab4->m_bHoleSearchArea)
	{
		rt1 = CheckROI(rt1,CRect(pView->m_Display[0].m_rtImgTracker[2].left,50,pView->m_Display[0].m_rtImgTracker[2].right,CAMERA_HEIGHT-10));
		rt2 = CheckROI(rt2,CRect(pView->m_Display[0].m_rtImgTracker[2].left,50,pView->m_Display[0].m_rtImgTracker[2].right,CAMERA_HEIGHT-10));
	}

	int hisoy1[CAMERA_WIDTH],histoy2[CAMERA_WIDTH],flag1=0,flag2=0,nv1=0,nv2=0;
	for(int i=0;i<CAMERA_WIDTH;i++)hisoy1[i]=histoy2[i]=0;
	npos1=npos2=0;
	if(rt1.Height()<1 || rt2.Height()<1)
	{
		pView->AddtoInspLog("Tab Top LR Rect Err");
		return 1;

	}

	// Tab ��� ���� ��� ã��
	int	nv=pView->m_pDlgSet->m_FormTab4->m_nTapEdge;//8
	for(int i=rt1.left;i<rt1.right-1;i++)
	{
		for(int j=rt1.top;j<rt1.bottom;j++)
			hisoy1[i]+= (*(m_fm[0]+j*CAMERA_WIDTH+i)+*(m_fm[0]+(j)*CAMERA_WIDTH+(i+1)))*0.5;
		hisoy1[i] /= rt1.Height(); // X�� ��������

		if(i>rt1.left+5 && i<rt1.right-5)
		{
			// [1] ������ ����� �ִ밪�� �� ��ġ
			if(abs(hisoy1[i-4]-hisoy1[i]) >nv1){
				nv1 = abs(hisoy1[i-4]-hisoy1[i]);
				npos1=i;
			}
			// [2] ������ Tab Edge L�� �̻� ���̰� �ִ�, ������ ���� ������ ���� ��ġ
			if(flag1==0 && hisoy1[i-4]-hisoy1[i]>=nv){
				m_nTopEdge[0] = i;
				flag1 = hisoy1[i-4]-hisoy1[i];
			}
		}
	}

	// Tab ��� ���� ��� ã��
	nv=pView->m_pDlgSet->m_FormTab4->m_nTapEdge1;//8
	for(int i=rt2.right;i>rt2.left;i--)
	{
		for(int j=rt2.top;j<rt2.bottom;j++)
			histoy2[i]+= m_fm[0][j*CAMERA_WIDTH+i];
		histoy2[i] /= rt2.Height(); // X�� ��������

		if(i<rt2.right-5 && i>rt2.left+5 )
		{
			// [1] ������ ����� �ִ밪�� �� ��ġ
			if(abs(histoy2[i]-histoy2[i+4]) >nv2){
				nv2 = abs(histoy2[i]-histoy2[i+4]);
				npos2=i;
			}
			// [2] ������ Tab Edge R�� �̻� ���̰� �ִ�, ������ ���� �������� ���� ��ġ
			if(flag2==0&& histoy2[i]-histoy2[i+4]>=nv){
				m_nTopEdge[1] = i;
				flag2 = histoy2[i]-histoy2[i+4];
			}
		}
	}

	// [2]�� ������ �¿����� ���� [1]������ ����
	if(flag1==0)m_nTopEdge[0] = npos1;
	if(flag2==0)m_nTopEdge[1] = npos2;

	double nd1 = pView->m_pDlgSet->m_FormTab4->m_dTapSize/pView->m_pDlgSet->m_FormTab2->m_dCamScale;

	// Tab ��� �¿찣 �߽� ���ϱ�
	m_nTapCenter = (m_nTopEdge[1]+m_nTopEdge[0])*0.5;

	// Top Cap �߽ɰ� Tab ��� �¿� ��谣�� �Ÿ�
	m_nEdgeDist[0] = m_ptCapCenter.x- m_nTopEdge[0]; // ������ Tab ���� ��谡 Top Cap �߽ɺ��� ������
	m_nEdgeDist[1] = m_nTopEdge[1]-m_ptCapCenter.x; // ������ Tab ���� ��谡 Top Cap �߽ɺ��� ����


	//tap�� top position check
	//tap width��ŭ projection 
	//������ ���� ���Ѵ�.
	CRect rtcen,rtcen1;
	if(pView->m_nMachine==3){
		rtcen.SetRect(m_ptCapCenter.x-30,m_ptCapCenter.y-20,m_ptCapCenter.x+30,m_ptCapCenter.y+40);
		rtcen1.SetRect(m_ptCapCenter.x-30,m_ptCapCenter.y+40,m_ptCapCenter.x+30,m_ptCapCenter.y+80);
	}
	else // ���� ����. [1]rtcen�� Top Cap �߽� ����. [2]rtcen1�� Tab ��ܺ� �Ʒ��� ���׸İ�
		rtcen.SetRect(m_ptCapCenter.x-50,m_ptCapCenter.y-50,m_ptCapCenter.x+50,m_ptCapCenter.y+50);
	rtcen1.SetRect(m_nTopEdge[0]+20,m_ptCapCenter.y+50,m_nTopEdge[0]+80,m_ptCapCenter.y+50+40);

	if(pView->m_nMachine==3)
	{
		if(m_nEdgeDist[0]<50){
			rtcen.SetRect(m_nTopEdge[0]+20,m_ptCapCenter.y-40,m_nTopEdge[0]+80,m_ptCapCenter.y+50);
			rtcen1.SetRect(m_nTopEdge[0]+20,m_ptCapCenter.y+50,m_nTopEdge[0]+80,m_ptCapCenter.y+50+40);
		}
	}
	
	double dcenave = 0,dcencnt=0,dcenave1 = 0,dcencnt1=0;
	int nth = 15,nthcnt = 0,nthcnt1 = 0;

	// ����[1] ��չ��
	for(int i=rtcen.top;i<rtcen.bottom;i++)
	{
		for(int j=rtcen.left;j<rtcen.right;j++)
		{
			dcenave += *(m_fm[0]+i*CAMERA_WIDTH+j);
			dcencnt++;
		}
	}
	if(dcencnt!=0)	dcenave /= dcencnt;
	else	dcenave = 0;

	// ����[1]������ ����[1] ��չ��-nth(15)���� ��ο� �͵� ī��Ʈ
	for(int i=rtcen.top;i<rtcen.bottom;i++)
	{
		for(int j=rtcen.left;j<rtcen.right;j++)
		{
			if(*(m_fm[0]+i*CAMERA_WIDTH+j)<dcenave-nth)
				nthcnt++;
		}
	}

	// if�� else�� �Ȱ���. 
	if(rtcen1.bottom>960) // Top Cap �߽�Y�� �ٴڿ� �پ� ���� ��� -> Top Cap �߽��� ����� �� ã�Ұų� Top Cap�� �ٴڿ� �پ��ִ�?
	{
		rtcen1.bottom=959;
		// ����[2] ��� ���
		for(int i=rtcen1.top;i<rtcen1.bottom;i++)
		{
			for(int j=rtcen1.left;j<rtcen1.right;j++)
			{
				dcenave1 += *(m_fm[0]+i*CAMERA_WIDTH+j);
				dcencnt1++;
			}
		}
		if(dcencnt1!=0)		dcenave1 /= dcencnt1;
		else dcenave1 = 0;

		// ����[2]������ ����[2] ��չ��-nth(15)���� ��ο� �͵� ī��Ʈ
		for(int i=rtcen1.top;i<rtcen1.bottom;i++)
		{
			for(int j=rtcen1.left;j<rtcen1.right;j++)
			{
				if(*(m_fm[0]+i*CAMERA_WIDTH+j)<dcenave1-nth)
					nthcnt1++;
			}
		}
	}
	else
	{
		for(int i=rtcen1.top;i<rtcen1.bottom;i++)
		{
			for(int j=rtcen1.left;j<rtcen1.right;j++)
			{
				dcenave1 += *(m_fm[0]+i*CAMERA_WIDTH+j);
				dcencnt1++;
			}
		}
		if(dcencnt1!=0)		dcenave1 /= dcencnt1;
		else dcenave1 = 0;

		for(int i=rtcen1.top;i<rtcen1.bottom;i++)
		{
			for(int j=rtcen1.left;j<rtcen1.right;j++)
			{
				if(*(m_fm[0]+i*CAMERA_WIDTH+j)<dcenave1-nth)
					nthcnt1++;
			}
		}
	}

	// hoho : �򰥷��� ���� �ø�.
	//if(dcencnt!=0)	dcenave /= dcencnt;
	//else	dcenave = 0;

	//for(int i=rtcen.top;i<rtcen.bottom;i++)
	//{
	//	for(int j=rtcen.left;j<rtcen.right;j++)
	//	{
	//		if(m_fm[0][i*CAMERA_WIDTH+j]<dcenave-nth)
	//			nthcnt++;
	//	}
	//}

	int nHistTop[CAMERA_WIDTH],ntopmax=0,ntoppos=0;
	for(int i=0;i<CAMERA_WIDTH;i++)nHistTop[i] = 0;

	// Top Cap �߽�Y�� �ʹ� ���󰡸� ������ ����
	if(m_ptCapCenter.y > CAMERA_HEIGHT-200)
		m_ptCapCenter.y = CAMERA_HEIGHT-200;
	if(m_ptCapCenter.y-250 <=0)
		m_ptCapCenter.y = 250;

	CRect rtedge1,rtedge2;
	int nedge1=0,nedge2=0,ncount1=0;
	nedge1 = m_nBottomEdge[0];

	/////////���� �ٲ�

	// ���ܺο� ������� ����ġ�� �ݴ����� ã�´�.
	// ������ ���������� �� ���� ����
	CRgn rgIns1;
	int noffset2=pView->m_pDlgSet->m_FormTab2->m_nInnerCiircle;
	CRect rtcap1;
	rtcap1.SetRect(m_ptCapCenter.x-noffset2,m_ptCapCenter.y-noffset2,m_ptCapCenter.x+noffset2,m_ptCapCenter.y+noffset2);
	rgIns1.DeleteObject(); 
	rgIns1.CreateEllipticRgn(m_ptCapCenter.x-noffset2,m_ptCapCenter.y-noffset2,m_ptCapCenter.x+noffset2,m_ptCapCenter.y+noffset2);
	double dcnt=0,dave=0,dsum=0;
	int	nHisto[256];
	int	nwidth =pView->m_pDlgSet->m_FormTab4->m_dTapSize/ pView->m_pDlgSet->m_FormTab2->m_dCamScale;
	int nDirection = 0,nedge11=0;//0 left tab  1 right tab

	ntoppos = m_ptCapCenter.y - 10;
	// Tracker1(Tab �ϴ�) ������ Top���� TopCap �߽�Y���� ���� ����
	for(int i=rttmp2.top;i>ntoppos+20;i-=20)
	{
		// ó���� Tab �ϴ� ���� ��� ����. 
		rtedge1.SetRect(nedge1-20,i-20,nedge1+20,i);
		int nn = findReectEdge(m_fm[0],CAMERA_WIDTH,rtedge1,1); // ������ ����� �ִ� ��ġ

		// ã�� ��� ����
		for(int k=i-20;k<i;k++)
			m_ptTab[0][k].SetPoint(nn,k);

		// �� �� ����� �ִ� ��ġ�� ������ ������ ��.
		nedge1 = nn;

		// ��ġ���� �˴� ���Ѵ�
		nedge2 += nn;
		nedge11 += nn;
		ncount1++;
	}

	// Tab ���� ���. ��ġ������ ���.
	if(ncount1>0)
		nedge11 /= ncount1;

	// Circle �ܺο� �ִ��� üũ
	// hoho : m_nTopEdge[0] -> ntoppos+20
	if(rgIns1.PtInRegion(CPoint(nedge11, ntoppos+20)))
	{
	}
	else
	{
		nDirection = 1;
	}
	rgIns1.DeleteObject();

	// ������ ã�� ���� ��谡 Circle �ܺο� ���� ���, Tab �ϴ� ���� ��� �������� �ٽ� ã�´�
	if(nDirection==1)
	{
		nedge1 = m_nBottomEdge[1];
		ncount1 = nedge2 = 0;

		for(int i=rttmp2.top;i>ntoppos+20;i-=20)
		{
			// ó���� Tab �ϴ� ���� ��� ����. 
			rtedge1.SetRect(nedge1-20,i-20,nedge1+60,i);
			int nn = findReectEdge(m_fm[0],CAMERA_WIDTH,rtedge1,2); // ������ ����� �ִ� ��ġ
			for(int k=i-20;k<i;k++)
				m_ptTab[0][k].SetPoint(nn,k);
			nedge1 = nn;
			nedge2 += nn;
			ncount1++;
		}
	}

	// ã�� ����� ��ġ(�� �����ϼ���, �� �����ϼ���). ��ġ������ ���.
	if(ncount1>0)
		nedge2 /= ncount1;
	else
		nedge2 = 0;
	
	// 2���� �ƴϸ�,
	if(!pView->m_pDlgSet->m_FormTab4->m_b2Tab)
	{
		int nRectOffset = 10;
		// Top Cap �߽ɿ��� ���ʿ� ���� ���
		//for(int i=m_ptCapCenter.y+120;i>m_ptCapCenter.y-220;i--)
		for(int i=m_ptCapCenter.y+140;i>m_ptCapCenter.y-220;i--) // hoho
		{
			for(int j=m_nTopEdge[0];j<m_nTopEdge[1];j++){
				if(i<0 || i>CAMERA_HEIGHT)continue;
				if(j<0 || j>CAMERA_WIDTH)continue;

				if(pView->m_pDlgSet->m_FormTab4->m_bTabTopRefind && pView->m_pDlgSet->m_FormTab2->m_bLineWelding){
					int val1 = *(m_fm[0]+i*CAMERA_WIDTH+j);
					int val2 = *(m_fm[0]+(i+1)*CAMERA_WIDTH+j);

					if(j>rtcen.left-nRectOffset && j<rtcen.right+nRectOffset && i>rtcen.top-nRectOffset && i<rtcen.bottom+nRectOffset){
						if(val1 < dcenave * 0.7)
							val1 = dcenave;
						if(val2 < dcenave * 0.7)
							val2 = dcenave;
					}
					nHistTop[i]+= (val1+val2)/2;
				}
				else{
					nHistTop[i]+= (*(m_fm[0]+i*CAMERA_WIDTH+j)+*(m_fm[0]+(i+1)*CAMERA_WIDTH+j))/2;
				}
			}
		
			// Tab ��� �¿� ��� �ȹٷ� ã�Ҵٸ�, Y�������� ���ݰ�. ������?
			if(m_nTopEdge[1]-m_nTopEdge[0]>1)
				nHistTop[i] /= (m_nTopEdge[1]-m_nTopEdge[0])/2;
			else nHistTop[i] = 0;
		}
	}
	else
	{
		for(int i=pView->m_Display[0].m_rtImgTracker[2].bottom+120;i>pView->m_Display[0].m_rtImgTracker[2].top-50;i--)
		{
			for(int j=m_nTopEdge[0];j<m_nTopEdge[1];j++){
				if(i<0 || i>CAMERA_HEIGHT-10)continue;
				if(j<0 || j>CAMERA_WIDTH)continue;
				nHistTop[i]+= (*(m_fm[0]+i*CAMERA_WIDTH+j)+*(m_fm[0]+(i+1)*CAMERA_WIDTH+j))/2;
			}

			if(m_nTopEdge[1]-m_nTopEdge[0]>1)
				nHistTop[i] /= (m_nTopEdge[1]-m_nTopEdge[0])/2;
			else nHistTop[i] = 0;
		}

	}

	int offset = 120;
	if(pView->m_nMachine==1)
		offset = 60;
	int ntb[2];

	// 2���� �ƴϸ�,
	if(!pView->m_pDlgSet->m_FormTab4->m_b2Tab)
	{
		//str.Format("Center Ave %.0f(%.0f) - Black %d(%d)",dcenave,dcenave1,nthcnt,nthcnt1);
		//pView->AddtoInspLog(str);
		//pView->AddtoList(str,1);
		
		int ntopoff = 70,ntopoff1 = 70;
		
		// Tab Top�� �˻��� ���� ���� ���. ���� �ǹ̴� ��
		// ������ Bottom�� Tracker3���� Bottom�� ������ �ʴ´�.
		if(nthcnt<450 && dcenave>80){ // �̰� ��ų�. žĸ�߽��� ���� ���� �� ������
			ntopoff= 20; 
			ntopoff1 = 140;
		}
		else{
			ntopoff= 120; ntopoff1 = 50;
			if(nthcnt>1000 || dcenave<50)
				ntopoff1 = 40;
			if(pView->m_nMachine>=2 && nthcnt1<100)
			{
				ntopoff= 70; ntopoff1 = 140;
			}
			if(pView->m_nMachine==3 && nthcnt>800)
			{
				ntopoff= 100;
			}
		}		
		ntb[0] = pView->m_Display[0].m_rtImgTracker[2].top; // Tab Top Check Tracker
		ntb[1] = pView->m_Display[0].m_rtImgTracker[2].bottom;

		// Tab Top�� �˻��� ������ Top�� Tracker3���� Top���� ����.
		// Tracker3���� Top�� Top Cap �߽ɺ��� �׻� ���� �ְ� ����.
		if(pView->m_pDlgSet->m_FormTab4->m_bHoleSearchArea) 
		{
			ntopoff1 = m_ptCapCenter.y - pView->m_Display[0].m_rtImgTracker[2].top+10;
		}

		// hoho, TopCap ���ã�� �������� �缳��
		if(pView->m_strComname.Find("3A")>=0 ||pView->m_strComname.Find("3B")>=0 ||pView->m_strComname.Find("3C")>=0 
			|| pView->m_strComname.Find("4A")>=0 ||pView->m_strComname.Find("4B")>=0||pView->m_strComname.Find("4C")>=0
			|| pView->m_strComname.Find("5A")>=0 ||pView->m_strComname.Find("5B")>=0||pView->m_strComname.Find("5C")>=0
			|| pView->m_strComname.Find("6A")>=0 ||pView->m_strComname.Find("6B")>=0||pView->m_strComname.Find("6C")>=0
			|| pView->m_strComname.Find("7A")>=0 ||pView->m_strComname.Find("7B")>=0||pView->m_strComname.Find("7C")>=0
			|| pView->m_strComname.Find("8A")>=0 ||pView->m_strComname.Find("8B")>=0||pView->m_strComname.Find("8C")>=0){
			if(pView->m_pDlgSet->m_FormTab4->m_bRegMode == TRUE){
				ntopoff = pView->m_Display[0].m_rtImgTracker[5].top - m_ptCapCenter.y + 10;
				ntopoff += 10;
			}
		} 
	
		for(int i=m_ptCapCenter.y+ntopoff-15;i>m_ptCapCenter.y-ntopoff1;i--) //180209 Tap ��� ã�� ������ 15 �ø�
		{
			if(pView->m_pDlgSet->m_FormTab4->m_bHoleSearchArea){
				// hoho, TopCap ���ã�⿡�� �糢�� ����. 
				if(pView->m_nMachine==3 || pView->m_strComname.Find("3A")>=0 ||pView->m_strComname.Find("3B")>=0 ||pView->m_strComname.Find("3C")>=0 
					|| pView->m_strComname.Find("4A")>=0 ||pView->m_strComname.Find("4B")>=0||pView->m_strComname.Find("4C")>=0
					|| pView->m_strComname.Find("5A")>=0 ||pView->m_strComname.Find("5B")>=0||pView->m_strComname.Find("5C")>=0
					|| pView->m_strComname.Find("6A")>=0 ||pView->m_strComname.Find("6B")>=0||pView->m_strComname.Find("6C")>=0
					|| pView->m_strComname.Find("7A")>=0 ||pView->m_strComname.Find("7B")>=0||pView->m_strComname.Find("7C")>=0
					|| pView->m_strComname.Find("8A")>=0 ||pView->m_strComname.Find("8B")>=0||pView->m_strComname.Find("8C")>=0){
					if(pView->m_pDlgSet->m_FormTab4->m_bRegMode == TRUE){
						//if(ntb[0]>i || pView->m_Display[0].m_rtImgTracker[5].bottom<i)
						if(ntb[0]>i || pView->m_Display[0].m_rtImgTracker[5].top<i)
							continue;
					}
					else{
						if(ntb[0]>i || ntb[1]<i)
							continue;
					}
				}
				else{
					if(ntb[0]>i || ntb[1]<i)
						continue;
				}
				//if(ntb[0]>i || ntb[1]<i) 
				//	continue;
			}
			if(pView->m_nMachine==1 && dcenave<90 && i<=m_ptCapCenter.y)
				continue;
			if(i<5 || i>CAMERA_HEIGHT-5)continue;

//////////////////core model skip
//			if(pView->m_nMachine==2 && i> 560 )continue;
			

			//center ������ 8�̻� �Ǵ� ���� ������� üũ

			// ������ ����� �ִ밪�� �� ��ġ
			if(nHistTop[i-4]>0&&nHistTop[i]>0 && abs(nHistTop[i] - nHistTop[i-4])>ntopmax)
			{
				//if(i<m_ptCapCenter.y-80 &&pView->m_nMachine==1)// abs(nHistTop[i] - nHistTop[i-4])<=ntopmax+2)
				if(i<m_ptCapCenter.y-120 &&pView->m_nMachine==1)// abs(nHistTop[i] - nHistTop[i-4])<=ntopmax+2)
						continue;
				ntopmax = abs(nHistTop[i] - nHistTop[i-4]);
				ntoppos = i;

				if(pView->m_pDlgSet->m_FormTab4->m_bUseTabTopTh && ntopmax > pView->m_pDlgSet->m_FormTab4->m_nTabTopTh)
					break;
			}
		}
	}
	else
	{
		ntb[0] = pView->m_Display[0].m_rtImgTracker[2].top;
		ntb[1] = pView->m_Display[0].m_rtImgTracker[2].bottom;
		if(ntb[0]<=0)ntb[0] = 5;
		for(int i=ntb[0];i<=ntb[1];i++)
		{
			if(i<5 || i>CAMERA_HEIGHT-5)continue;
			//���� ū���� ã�´�.
			if(nHistTop[i-4]>0&&nHistTop[i]>0 &&abs(nHistTop[i] - nHistTop[i-4])>ntopmax)
			{
				//				if(i<m_ptCapCenter.y-80 &&pView->m_nMachine==1)// abs(nHistTop[i] - nHistTop[i-4])<=ntopmax+2)
				ntopmax = abs(nHistTop[i] - nHistTop[i-4]);
				ntoppos = i;
			}
		}
	}
	
	// �� ��ġ�� Tab Top����
	m_nTopPos[0]=ntoppos;

	str.Format("Tab LT(%d) RT(%d) T(%d)",m_nTopEdge[0],m_nTopEdge[1],m_nTopPos[0]);
	pView->AddtoInspLog(str);	

	int ngap = 13;
	if(pView->m_nMachine==1)ngap = 13;

	// Tab ��� ���� ������ Tab������ �۰ų� Ŭ��, ��� �¿� ��踦 Circle �̿����� �ʰ� Tab Top�� �ٽ� ã�´�!
	// Tab ��� �¿� ���� �Ʒ� ���ǿ� �� ���ֳ�? ������ ���ʿ��� �ٸ� �������� ������ Tab ����ŭ ����ִµ�?
	if(m_nTopEdge[1]-m_nTopEdge[0]<nwidth-ngap || m_nTopEdge[1]-m_nTopEdge[0]>nwidth+ngap)
	{
		pView->AddtoInspLog("Tab Top Refind!");

		int old1 = m_nTopEdge[1];
		int old0 = m_nTopEdge[0];
		
		if(pView->m_nMachine==3)
		{
			if(m_nBottomEdge[1]-m_nBottomEdge[0]<130 || m_nBottomEdge[1]-m_nBottomEdge[0]>250)
			{
				rt1.SetRect(m_ptCapCenter.x-200,m_nTopPos[0]+20,m_ptCapCenter.x+20,m_nTopPos[0]+80);
				rt2.SetRect(m_ptCapCenter.x,m_nTopPos[0]+20,m_ptCapCenter.x+200,m_nTopPos[0]+80);
			}
			else
			{
				rt1.SetRect(m_nBottomEdge[0]-noffset,m_nTopPos[0]+20,m_nBottomEdge[0]+noffset+20,m_nTopPos[0]+80);
				rt2.SetRect(m_nBottomEdge[1]-noffset-40,m_nTopPos[0]+20,m_nBottomEdge[1]+noffset+40,m_nTopPos[0]+80);
			}
		}
		else
		{
			// Tab �ϴ� ���� �̻��� ��, (3,4ȣ�� ����, ������ �뷫 3mm�� 175, 4mm�� 238)
			if(m_nBottomEdge[1]-m_nBottomEdge[0]<130 || m_nBottomEdge[1]-m_nBottomEdge[0]>250)
			{
				// Top Cap �߽ɰ� Tab Top�� �������� Tab ��ܺο� �¿� ���� ����
				rt1.SetRect(m_ptCapCenter.x-200,m_nTopPos[0]+20,m_ptCapCenter.x+20,m_nTopPos[0]+80);
				rt2.SetRect(m_ptCapCenter.x,m_nTopPos[0]+20,m_ptCapCenter.x+200,m_nTopPos[0]+80);
			}
			else // ������ ����,
			{
				// Tab �ϴ� �¿�� Tab Top�� �������� Tab ��ܺο� �¿� ���� ����
				rt1.SetRect(m_nBottomEdge[0]-noffset,m_nTopPos[0]+20,m_nBottomEdge[0]+noffset-20,m_nTopPos[0]+80);
				rt2.SetRect(m_nBottomEdge[1]-noffset-20,m_nTopPos[0]+20,m_nBottomEdge[1]+noffset+20,m_nTopPos[0]+80);
			}
		}

		rt1 = CheckROI(rt1,CRect(50,50,CAMERA_WIDTH-50,CAMERA_HEIGHT-10));
		rt2 = CheckROI(rt2,CRect(50,50,CAMERA_WIDTH-50,CAMERA_HEIGHT-10));

		if(pView->m_nMachine==3 && pView->m_pDlgSet->m_FormTab4->m_bHoleSearchArea)
		{
			rt1 = CheckROI(rt1,CRect(pView->m_Display[0].m_rtImgTracker[2].left,100,pView->m_Display[0].m_rtImgTracker[2].right,CAMERA_HEIGHT-10));
			rt2 = CheckROI(rt2,CRect(pView->m_Display[0].m_rtImgTracker[2].left,100,pView->m_Display[0].m_rtImgTracker[2].right,CAMERA_HEIGHT-10));

			if(rt1.Height() <= 0)
				rt1.top = rt1.top - 50;
			if(rt2.Height() <= 0)
				rt2.top = rt2.top - 50;
		}

		int hisoy1[CAMERA_WIDTH],histoy2[CAMERA_WIDTH],flag1=0,flag2=0,nv1=0,nv2=0;
		int	nv=8;
		for(int i=0;i<CAMERA_WIDTH;i++)hisoy1[i]=histoy2[i]=0;
		npos1=npos2=0;

		// ���ʺ���. �ٱ��ʿ��� �� ��������
		for(int i=rt1.left;i<rt1.right-1;i++)
		{
			for(int j=rt1.top;j<rt1.bottom;j++)
				hisoy1[i]+= (*(m_fm[0]+j*CAMERA_WIDTH+i)+*(m_fm[0]+(j)*CAMERA_WIDTH+(i+1)))*0.5; // X�� ��������
			hisoy1[i] /= rt1.Height();		
			if(i>rt1.left+5 && i<rt1.right-5)
			{
				if(i==450)
					i=i;
				// ������ ����� �ִ밪�� �� ��ġ
				if(abs(hisoy1[i-4]-hisoy1[i]) >=nv1){
					nv1 = abs(hisoy1[i-4]-hisoy1[i]);
					npos1=i;
				}
				// 8 �̻� ���̰� �ִ�, ���� �ٱ��� �ִ� ���� ��ġ
				if(flag1==0&& hisoy1[i-4]-hisoy1[i]>=nv)
				{
					m_nTopEdge[0] = i;
					flag1 = hisoy1[i-4]-hisoy1[i];
				}
			}
		}
		
		// ������. �ٱ��ʿ��� �� ��������
		for(int i=rt2.right;i>rt2.left;i--)
		{
			for(int j=rt2.top;j<rt2.bottom;j++)
				histoy2[i]+= *(m_fm[0]+j*CAMERA_WIDTH+i); // X�� ��������
			histoy2[i] /= rt2.Height();

			if(i<rt2.right-5 && i>rt2.left+5 )
			{
				// ������ ����� �ִ밪�� �� ��ġ
				if(abs(histoy2[i]-histoy2[i+4]) >nv2){
					nv2 = abs(histoy2[i]-histoy2[i+4]);
					npos2=i;
				}
				// 8 �̻� ���̰� �ִ�, ���� �ٱ��� �ִ� ���� ��ġ
				if(flag2==0&& (histoy2[i]-histoy2[i+4])>=nv)
				{
					m_nTopEdge[1] = i;
					flag2 = histoy2[i]-histoy2[i+4];
				}
			}
		}

		// �ִ������� ���� ���� 100 �̻��϶�,
		if(npos2-npos1>100)
		{
			if(flag2<nv2-2 && flag2<11)
				m_nTopEdge[1] = npos2;
			if(flag1<nv1-2 && flag1<11)
				m_nTopEdge[0] = npos1;
			if(flag1==0)
				m_nTopEdge[0] = npos1;
			if(flag2==0)
				m_nTopEdge[1] = npos2;
		}

		if((pView->m_nMachine==1||pView->m_nMachine==3) &&abs((m_nTopEdge[1]-m_nTopEdge[0])- (nwidth))>13 )
		{
			//�� ���� ���Ѱ��� �������� �Ѵ�.
			if(nv1>=nv2)
				m_nTopEdge[1] = m_nTopEdge[0]+ nwidth;
			else
				m_nTopEdge[0] = m_nTopEdge[1]- nwidth;
		}

		m_nTapCenter = (m_nTopEdge[1]+m_nTopEdge[0])*0.5;


		m_nEdgeDist[0] = m_ptCapCenter.x- m_nTopEdge[0];
		m_nEdgeDist[1] = m_nTopEdge[1]-m_ptCapCenter.x;

		str.Format("Tab LT %d, RT %d",m_nTopEdge[0],m_nTopEdge[1]);
		pView->AddtoInspLog(str);	
	}

	return nret;
}

int CgProject::FindWeldingPosition()
{
	int nret = 0;
	m_nFindWhite = 0;
	//1. ���� ����Ʈ���� ������ �������鼭 ����/�л꿡 ���� ��ġ ã��//m_nTopPos
	CRect rtstd,StdRt;
	// Tab Top �Ʒ��� ���ڴ�
	int ntmp1 = m_nTopPos[0];
	// �㳪, Tab Top�� Top Cap �߽ɺ��� ���� ���� ����, Top Cap �߽� �Ʒ��� ���ڴ�
	if(m_ptCapCenter.y>m_nTopPos[0] && pView->m_nMachine>=2)
		ntmp1 = m_ptCapCenter.y;
	
	rtstd.SetRect(m_ptCapCenter.x-50,ntmp1+60,m_ptCapCenter.x+50,ntmp1+180);
	if(pView->m_nMachine==3)
		rtstd.SetRect(m_nTopEdge[0]+50,ntmp1+60,m_nTopEdge[0]+120,ntmp1+200);

	if(pView->m_nMachine==2 && rtstd.top<560)
		rtstd.top = 560;
	if(pView->m_nMachine==2 && rtstd.bottom<700)
		rtstd.bottom=700;
	if(pView->m_nMachine==1 && rtstd.top<530){
		rtstd.top = 530;
		rtstd.bottom+=100;
	}
//	if(pView->m_nMachine==3 )rtstd.top = 550,rtstd.bottom = 710;
//	if(pView->m_nMachine==2)
//		rtstd.SetRect(m_ptCapCenter.x-50,ntmp1+60,m_ptCapCenter.x+50,ntmp1+140);
	if(pView->m_nMachine==1 && rtstd.bottom>700)
		rtstd.bottom = 700;
	
	if(pView->m_pDlgSet->m_FormTab4->m_b2Tab)
	{
		rtstd.top = m_nTopPos[0]+50;
		rtstd.bottom = rtstd.top + 250;
	}

	memcpy(m_fmTemp,m_fm[0],CAMERA_WIDTH*CAMERA_HEIGHT);

	if(pView->m_nMachine==3)
		rtstd = CheckROI(rtstd,CRect(m_nTopEdge[0],200,m_nTopEdge[1],CAMERA_HEIGHT-200));
	else
		rtstd = CheckROI(rtstd,CRect(100,200,CAMERA_WIDTH-200,CAMERA_HEIGHT-200));

	int nPeak1y,nPeak2y;
	int nHStdgap = 32;
	double	std1Ave,std2Ave,stdTempAve;
	double dStd1,dStd2,dStdOld,dstdTmp,dStdOld1,dStdMin,dStdMinOld;
	dStd1=dStd2=dStdOld=dstdTmp=dStdOld1=0.0;
	std1Ave=std2Ave=stdTempAve=0.;
	m_nStdCount = 0;
	dStdMin=dStdMinOld=1000.0;
	nPeak1y=nPeak2y =rtstd.top;
	m_nStartCircle =m_nEndCircle= 0;
	for(int i=0;i<STDCOUNT;i++)
		m_nStdLineInsp[i] =m_dVarValue[i]=m_nStdLine[i]=m_nStdLineInsp[i]=0;

	for(int j=rtstd.top;j<rtstd.bottom-nHStdgap*0.5;j+=nHStdgap/2)
	{
		//�¿� ���� ������ ����.
		StdRt.SetRect(rtstd.left,j,rtstd.right,j+(nHStdgap*0.5));
//		StdRt = checkRect(StdRt);

		// stdTempAve : ��չ��, dstdTmp : ǥ������, m_nMin : �ּҰ�, m_nMax : �ִ밪, m_dVar : �л�
		stdTempAve=GetRoiInfo(m_fm[0],CAMERA_WIDTH,StdRt,1,1,&dstdTmp,&m_nMin,&m_nMax,&m_dVar,-1);
		m_strStdValue[m_nStdCount].Format(_T("%2.2f"),dstdTmp);
		m_dVarValue[m_nStdCount] = m_dVar;
		m_strVarValue[m_nStdCount].Format(_T("%2.2f-%2.2f"),m_dVar,dstdTmp);
		m_nStdLine[m_nStdCount] = j;
		m_nStdLineInsp[m_nStdCount] = j;
		m_nStdCount++;

		// ���� ���� �л갪�� ���Ѵ�
		if(m_dVar<dStdMinOld)
		{
			dStdMinOld = m_dVar;
			dStdMin=dStdMinOld;
		}

		// ���� ū ǥ���������� �ι�°�� ū ǥ���������� ������ ������ ǥ������, ��չ��, Y���� ����
		if(dstdTmp>dStdOld)
		{
			if(dstdTmp>dStd1)
			{
				dStd2=dStd1; // �ι�°�� ū ǥ������
				m_dStd2Ave = m_dStd1Ave; // �ű��� ��չ��
				nPeak2y = nPeak1y; // �ű��� Y(������ Top)
				dStd1 = dstdTmp; // ���� ū ǥ������
				m_dStd1Ave = stdTempAve; // �ű��� ��չ��
				nPeak1y = j; // �ű��� Y(������ Top)
			}
			dStdOld = dstdTmp;
		}
		// �ι�°�� ū ǥ������. ���� ��Ʈ
		if(dstdTmp>dStd2 && dstdTmp <dStd1)
		{
			dStd2=dstdTmp;
			m_dStd2Ave = stdTempAve;
			nPeak2y = j;
		}
	}

	int nCircleTopPos =	fnCirPosFind(dStdMin); // ���� ���� �л갪�� �־, ������ ���� ���� ��ġ�� �̴´�.
	pView->AddtoInspLog("fnCirPosFind");
	if(nCircleTopPos==0){
		return 1;
	}
	m_nCadidateY = nCircleTopPos;
	//int ncircleCenter1;
	CRect temprt;
	double dVarValue1[100];
	int nPosValue[100],nposcount = 0;

	// �ǹ� ����
	if(m_nEndCircle-m_nStartCircle < 120 && m_nEndCircle-m_nStartCircle>10)
	{
		//ncircleCenter1 = (m_nEndCircle+m_nStartCircle)/2;
		temprt.SetRect(m_nTopEdge[0],m_nStartCircle,m_nTopEdge[1],m_nEndCircle);
	}
	else
	{
		//ncircleCenter1 = nPeak1y;
		// �������� �л갪�� ��պ��� ���ų� ���� ������ ������, ���� ū ǥ�������� ������ ������ ����
		if(m_nStartCircle ==0 || m_nEndCircle==0){
			m_nStartCircle = nPeak1y;
			m_nEndCircle = nPeak1y+20;
		}
		temprt.SetRect(m_nTopEdge[0],m_nStartCircle,m_nTopEdge[1],m_nEndCircle);
	}
	temprt = CheckROI(temprt,CRect(100,200,CAMERA_WIDTH-200,CAMERA_HEIGHT-100));

	m_nStdLine1 = m_nStartCircle;
	m_nStdLine2 = m_nEndCircle;

	// ������ ���� ���� ��ġ�� �� �̾Ƴ�����,
	if(nCircleTopPos==0){
		// m_nStartCircle�� m_nEndCircle �������� ����
		if(m_nStartCircle>0 && m_nEndCircle>0)
			nCircleTopPos = (m_nStartCircle+m_nEndCircle)*0.5;
		else{
			if(m_nStartCircle<m_nEndCircle)
				nCircleTopPos=m_nEndCircle;
			else	
				nCircleTopPos=m_nStartCircle;
		}
	}

	//if(pView->m_pDlgSet->m_FormTab4->m_bHoleSearchArea)
	//{
	//	if(pView->m_Display[0].m_rtImgTracker[2].top>nCircleTopPos-64)
	//		nCircleTopPos = pView->m_Display[0].m_rtImgTracker[2].top;
	//	if(pView->m_Display[0].m_rtImgTracker[2].bottom<nCircleTopPos+64)
	//		nCircleTopPos = pView->m_Display[0].m_rtImgTracker[2].bottom;
	//}

	 m_nBlackStart =m_nBlackEnd = 0;


//	hole rect set
	BYTE * fmHole = new BYTE[m_nHoleSearchingAreaW*m_nHoleSearchingAreaH];//origin
	BYTE * fmHole1 = new BYTE[m_nHoleSearchingAreaW*m_nHoleSearchingAreaH];//bin
	BYTE * fmHole2 = new BYTE[m_nHoleSearchingAreaW*m_nHoleSearchingAreaH];//morph
	BYTE * fmHole3 = new BYTE[m_nHoleSearchingAreaW*m_nHoleSearchingAreaH];//morph
	BYTE * fmHole4 = new BYTE[m_nHoleSearchingAreaW*m_nHoleSearchingAreaH];//morph
	BYTE * fmHole5 = new BYTE[m_nHoleSearchingAreaW*m_nHoleSearchingAreaH];//morph
	BYTE * fmHole6 = new BYTE[m_nHoleSearchingAreaW*m_nHoleSearchingAreaH];//morph
	memset(fmHole,0,m_nHoleSearchingAreaW*m_nHoleSearchingAreaH);	
	memset(fmHole1,0,m_nHoleSearchingAreaW*m_nHoleSearchingAreaH);	
	memset(fmHole2,0,m_nHoleSearchingAreaW*m_nHoleSearchingAreaH);
	memset(fmHole3,0,m_nHoleSearchingAreaW*m_nHoleSearchingAreaH);
//	CRect rtHole;
	int nmargin3,nmargin1,nmargin2;//80
	nmargin3 = int((pView->m_pDlgSet->m_FormTab4->m_dTapSize/pView->m_pDlgSet->m_FormTab2->m_dCamScale)*0.5)-6;
	nmargin1 = m_nTapCenter-nmargin3;
	nmargin2 = m_nTapCenter+nmargin3;

//	rtHole.SetRect(m_nTapCenter-100,(m_nStdLine1+m_nStdLine2)*0.5-64,m_nTapCenter+100,(m_nStdLine1+m_nStdLine2)*0.5+64);
	int noffset = 20;// 28;//40;//�̹��� ���� ��

	// ���� �ĺ��� �ʹ� �Ʒ��� ������ ������ �÷���
	if(!pView->m_pDlgSet->m_FormTab4->m_b2Tab && (m_ptCapCenter.y+240)<(nCircleTopPos+64+noffset))
		nCircleTopPos=m_ptCapCenter.y+240-64-noffset;

	if(pView->m_nMachine==1 && !pView->m_pDlgSet->m_FormTab4->m_b2Tab){
		if((m_ptCapCenter.y+50)>(nCircleTopPos-64+noffset))
			nCircleTopPos=m_ptCapCenter.y+50+64-noffset;
		else if((m_ptCapCenter.y+200)<(nCircleTopPos+64+noffset))
			nCircleTopPos=m_ptCapCenter.y+200-64-noffset;
	}

	// ���� ã�� ����(���� �ڽ�) ���ϱ�
	if(pView->m_pDlgSet->m_FormTab4->m_bRegMode){
		CRect rttmp14;
		rttmp14 = pView->m_Display[0].m_rtImgTracker[5]; // Welding Tracker
		int nregcenter = rttmp14.top+rttmp14.Height()*0.5;

		// Tracker6(����)�� Y�߽ɸ� ���پ�
		m_rtHoleRect.SetRect(m_nTapCenter-(m_nHoleSearchingAreaW/2),nregcenter-(m_nHoleSearchingAreaH/2),m_nTapCenter+(m_nHoleSearchingAreaW/2),nregcenter+(m_nHoleSearchingAreaH/2));
	}
	else // Tracker6 �Ⱦ���, ���� �ĺ� ��ġ ����
		m_rtHoleRect.SetRect(m_nTapCenter-(m_nHoleSearchingAreaW/2),nCircleTopPos-(m_nHoleSearchingAreaH/2)+noffset,m_nTapCenter+(m_nHoleSearchingAreaW/2),nCircleTopPos+(m_nHoleSearchingAreaH/2)+noffset);


	if(pView->m_nMachine==3 && m_rtHoleRect.bottom>760)
	{
		m_rtHoleRect.SetRect(m_nTapCenter-(m_nHoleSearchingAreaW/2), 572, m_nTapCenter+(m_nHoleSearchingAreaW/2),700);
		m_rtHoleRect.SetRect(m_nTapCenter-(m_nHoleSearchingAreaW/2), 560, m_nTapCenter+(m_nHoleSearchingAreaW/2), 560+m_nHoleSearchingAreaH);

		m_rtHoleRect = CheckROI(m_rtHoleRect,CRect(pView->m_Display[0].m_rtImgTracker[2].left,540,pView->m_Display[0].m_rtImgTracker[2].right,740));
	}

	if(pView->m_nMachine==3 && m_ptCapCenter.y>530)
	{//center down
		m_rtHoleRect.SetRect(m_nTapCenter-(m_nHoleSearchingAreaW/2), 550+(m_ptCapCenter.y-500), m_nTapCenter+(m_nHoleSearchingAreaW/2), 550+m_nHoleSearchingAreaH+(m_ptCapCenter.y-500));
		m_rtHoleRect = CheckROI(m_rtHoleRect,CRect(pView->m_Display[0].m_rtImgTracker[2].left,540,pView->m_Display[0].m_rtImgTracker[2].right,840));
	}
	else if(pView->m_nMachine==3)
	{
		m_rtHoleRect.OffsetRect(0,5);//.SetRect(m_nTapCenter-100,550+(m_ptCapCenter.y-500),m_nTapCenter+100,550+128+(m_ptCapCenter.y-500));
	//	m_rtHoleRect = CheckROI(m_rtHoleRect,CRect(pView->m_Display[0].m_rtImgTracker[2].left,540,pView->m_Display[0].m_rtImgTracker[2].right,840));
		m_rtHoleRect = CheckROIoffset(m_rtHoleRect,CRect(pView->m_Display[0].m_rtImgTracker[2].left,540,pView->m_Display[0].m_rtImgTracker[2].right,pView->m_Display[0].m_rtImgTracker[2].bottom));
	}

	if(pView->m_nMachine==2 && pView->m_pDlgSet->m_FormTab4->m_bHoleSearchArea)
	{
		// ��� Tracker�� �¿� �� Ȯ���� ��
		if(!pView->m_pDlgSet->m_FormTab4->m_bRegMode) // RegMode ���ϸ�, Tracker3(Tab Top) ���󰡰�,
			m_rtHoleRect = CheckROIoffset(m_rtHoleRect,CRect(pView->m_Display[0].m_rtImgTracker[2].left,540,pView->m_Display[0].m_rtImgTracker[2].right,pView->m_Display[0].m_rtImgTracker[2].bottom));
		else // RegMode �ϸ�, Tracker6(Welding) ���󰣴�.
			m_rtHoleRect = CheckROIoffset(m_rtHoleRect,CRect(pView->m_Display[0].m_rtImgTracker[5].left,200,pView->m_Display[0].m_rtImgTracker[5].right,900));
	}
	// hoho 1ȣ�� �����ڽ� �ٿ��޶���
	// hoho 1ȣ�� �����ڽ��� Bottom Tab ���� ���ʿ��� �ְ�
	if(pView->m_nMachine==3){
		if(m_rtHoleRect.left < m_nBottomEdge[0])
			m_rtHoleRect.OffsetRect( +(m_nBottomEdge[0] - m_rtHoleRect.left),0);
		if(m_rtHoleRect.right > m_nBottomEdge[1])
			m_rtHoleRect.OffsetRect( -(m_rtHoleRect.right - m_nBottomEdge[1]),0);
	}
	
	m_rtHoleRect = CheckROI(m_rtHoleRect,CRect(50,50,CAMERA_WIDTH-50,CAMERA_HEIGHT-50));
	m_Pro.ImageSelectCopy(m_fm[0],fmHole,CAMERA_WIDTH,CAMERA_HEIGHT,m_rtHoleRect,m_nHoleSearchingAreaW,m_nHoleSearchingAreaH,0,0,0);

	memcpy(fmHole5,fmHole,m_nHoleSearchingAreaW*m_nHoleSearchingAreaH);
	memcpy(fmHole6,fmHole,m_nHoleSearchingAreaW*m_nHoleSearchingAreaH);

	pView->AddtoInspLog("ModifyBlackLine");
	
	//����1 ��ó��
	int nSt = 0,nEnd=0;
	if(pView->m_pDlgSet->m_FormTab4->m_bImageModify && pView->m_nMachine==3)
	{
		//���� ����
		ModifyBlackLine(fmHole,fmHole,m_nHoleSearchingAreaW,m_nHoleSearchingAreaH,&nSt,&nEnd);
		//main Image
		m_nBlackStart=nSt+m_rtHoleRect.top-5;
		m_nBlackEnd=nEnd+m_rtHoleRect.top+5;
//		if(m_nBlackEnd>650)m_nBlackEnd+=100;
	}

	memcpy(fmHole1,fmHole,m_nHoleSearchingAreaW*m_nHoleSearchingAreaH);
	memcpy(fmHole2,fmHole,m_nHoleSearchingAreaW*m_nHoleSearchingAreaH);

	nmargin1 = nmargin1 - m_rtHoleRect.left; // margin1 = m_nTapCenter - nmargin3(Tab �� ����)
	nmargin2 = m_rtHoleRect.right-nmargin2; // margin2 = m_nTapCenter + nmargin3(Tab �� ����)

	//���� ����ȭ
	// L : nmargin1
	// T : 0
	// R : m_nHoleSearchingAreaW-nmargin2
	// B : m_nHoleSearchingAreaH
	// ���� �� Tab ������ ��չ�⸸ ����
	//int holeave = AreaAverage(0,fmHole,m_nHoleSearchingAreaW,nmargin1,0,m_nHoleSearchingAreaW-nmargin2,m_nHoleSearchingAreaH); 
	int holeave = AreaAverage(0,fmHole,m_nHoleSearchingAreaW,0,0,m_nHoleSearchingAreaW,m_nHoleSearchingAreaH); 
	pView->AddtoInspLog("AreaAverage");

	// hoho : ���� ã�� test //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//BYTE * fmHoleSmooth = new BYTE[200*128];
	//memset(fmHoleSmooth,0,200*128);

	////// ������ 1ȸ
	//for(int y=0; y<128; y++){
	//	if(y == 0 || y == 127)
	//		continue;

	//	for(int x=0; x<200; x++){
	//		if(x < nmargin1 || x > 200-nmargin2)
	//			continue;
	//		
	//		if(x == 199)
	//			continue;

	//		double dAve = 0;
	//		int nCnt = 0;

	//		for(int y1=y; y1<y+2; y1++){
	//			for(int x1=x; x1<x+2; x1++){
	//				dAve = dAve + fmHole[(128-y1-1)*200+x1];
	//				nCnt++;
	//			}
	//		}

	//		dAve = dAve / nCnt;
	//		
	//		fmHoleSmooth[(128-y-1)*200+x] = dAve;
	//	}
	//}
	//for(int y=0; y<128; y++){
	//	for(int x=0; x<200; x++)
	//	{
	//		m_fm[0][(y+130)*CAMERA_WIDTH+x] = fmHoleSmooth[(128-y-1)*200+x];
	//	}
	//}

	//BYTE * fmHoleSmooth1 = new BYTE[200*128];
	//memset(fmHoleSmooth1,0,200*128);

	//// ���� �� ��Ƣ�� �����
	//int nMaskSize = 3;
	//int nMask[3][3] = {{1,2,1},{2,4,2},{1,2,1}};

	//for(int y=0; y<128; y++){
	//	if(y == 0 || y == 127)
	//		continue;

	//	for(int x=0; x<200; x++){
	//		if(x == 0 || x == 199)
	//			continue;

	//		double dAve = 0;
	//		int a=0,b=0;
	//		int nDenominator = 0;
	//		
	//		for(int y1=y-1; y1<=y+1; y1++){
	//			b = 0;
	//			for(int x1=x-1; x1<=x+1; x1++){
	//				int nValue = fmHoleSmooth[(128-y1-1)*200+x1];
	//				nDenominator = nDenominator + nMask[a][b];

	//				if(nValue != 0)
	//					dAve = dAve + nValue * nMask[a][b];

	//				b++;
	//			}
	//			a++;
	//		} 

	//		//dAve = dAve / 16;
	//		dAve = dAve / nDenominator;

	//		fmHoleSmooth1[(128-y-1)*200+x] = dAve;
	//	}
	//}
	//
	//for(int y=0; y<128; y++){
	//	for(int x=0; x<200; x++){
	//		m_fm[0][(y+260)*CAMERA_WIDTH+x] = fmHoleSmooth1[(128-y-1)*200+x];
	//	}
	//}

	//int nSumOfDeviation[128] = {0,};
	//for(int y=0; y<128; y++){
	//	for(int x=0; x<200-1; x++){
	//		nSumOfDeviation[y] = nSumOfDeviation[y] + abs(fmHoleSmooth1[(128-y-1)*200+x] - fmHoleSmooth1[(128-y-1)*200+x+1]);
	//	}
	//}

	//delete fmHoleSmooth;
	//delete fmHoleSmooth1;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	// hoho : ���� ã�� test //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//CRect rtWeldingSearch;
	//rtWeldingSearch = m_rtHoleRect;

	//double dStdWeldingSearchArea = 0;
	//int nMinWeldingSearchArea = 0;
	//int nMaxWeldingSearchArea = 0;
	//double dVarWeldingSearchArea = 0;
	//double dAveWeldingSearchArea = 0;

	//int nSkipSide = 0;

	//if(m_rtHoleRect.left < m_nTapCenter - nmargin3 && m_rtHoleRect.right > m_nTapCenter + nmargin3){ // ������ġ�˻� ������ Tab �ٱ����� ��������. 3mm��
	//	nSkipSide = 12;
	//	rtWeldingSearch.left = rtWeldingSearch.left + nSkipSide;
	//	rtWeldingSearch.right = rtWeldingSearch.right - nSkipSide;
	//}
	//else{ // ������ġ�˻� ������ Tab �ٱ����� �Ȼ�������. 4mm��
	//	nSkipSide = 1;
	//}
	//
	//// stdTempAve : ��չ��, dStdWeldingSearchArea : ǥ������, nMinWeldingSearchArea : �ּҰ�, nMaxWeldingSearchArea : �ִ밪, dVarWeldingSearchArea : �л�
	//dAveWeldingSearchArea = GetRoiInfo(m_fm[0],CAMERA_WIDTH,rtWeldingSearch,2,2,&dStdWeldingSearchArea,&nMinWeldingSearchArea,&nMaxWeldingSearchArea,&dVarWeldingSearchArea,-1);
	//
	//BYTE * fmHoleSmooth = new BYTE[200*128];
	//memset(fmHoleSmooth,0,200*128);

	//// ������ 1ȸ
	//for(int y=0; y<128; y++){
	//	if(y == 0 || y == 127)
	//		continue;

	//	for(int x=0; x<200; x++){
	//		if(x < nmargin1 || x > 200-nmargin2)
	//			continue;
	//		
	//		if(x == 199)
	//			continue;

	//		double dAve = 0;
	//		int nCnt = 0;

	//		for(int y1=y; y1<y+2; y1++){
	//			for(int x1=x; x1<x+2; x1++){
	//				dAve = dAve + fmHole[(128-y1-1)*200+x1];
	//				nCnt++;
	//			}
	//		}

	//		dAve = dAve / nCnt;
	//		
	//		fmHoleSmooth[(128-y-1)*200+x] = dAve;
	//	}
	//}
	//for(int y=0; y<128; y++){
	//	for(int x=0; x<200; x++)
	//	{
	//		m_fm[0][(y+130)*CAMERA_WIDTH+x] = fmHoleSmooth[(128-y-1)*200+x];
	//	}
	//}

	//double dAveArea = 0;
	//int nCountArea = 0;
	//int nValue = 0;
	//double dVal1 = 0;
	//double dVal2 = 0;
	//for(int y=0; y<128; y++){
	//	for(int x=0; x<200; x++){
	//		nValue = fmHoleSmooth[(128-y-1)*200+x];

	//		if(nValue == 0)
	//			continue;

	//		dAveArea = dAveArea + nValue;
	//		nCountArea++;
	//		dVal1 += nValue;
	//		dVal2 += nValue*nValue;
	//	}
	//}

	//dAveArea = dAveArea / nCountArea;
	//double dVar = (nCountArea*dVal2-dVal1*dVal1)/(nCountArea*(nCountArea-1.)); // �л�
	//double dStd = sqrt((nCountArea*dVal2-dVal1*dVal1)/(nCountArea*(nCountArea-1.))); // ǥ������

	//// Ƣ�ºκ��� ���� ������
	//for(int y=0; y<128; y=y+3){
	//	for(int x=0; x<200; x=x+3)
	//	{
	//		;
	//	}
	//}	

	//for(int y=0; y<128; y++){
	//	for(int x=0; x<200; x++)
	//	{
	//		m_fm[0][(y+260)*CAMERA_WIDTH+x] = fmHoleSmooth[(128-y-1)*200+x];
	//	}
	//}

	//delete fmHoleSmooth;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// ��չ�Ⱑ ��ο�� �˻� ����
	if(holeave<40){
		delete fmHole;//������
		delete fmHole1;//����ȭ
		delete fmHole2;//������
		delete fmHole3;//3*3���
		delete fmHole4;
		delete fmHole5;
		delete fmHole6;//preprocessing
		return 1;
	}

	// ������ ��չ���� ��%�� TH�� �����ΰ�
	double dHoleTh = holeave * (pView->m_pDlgSet->m_FormTab4->m_nHoleTh*0.01);

	// 3*3���. �ε巴�� ����� �̰ǰ�?
	for(int i=0;i<m_nHoleSearchingAreaH;i++) // y
	{
		for(int j=1;j<m_nHoleSearchingAreaW-1;j++) // x
		{
			if(i<1) // �� �� ù��° ��. ���� ���
				*(fmHole3+i*m_nHoleSearchingAreaW+j) = (( *(fmHole+i*m_nHoleSearchingAreaW+(j))+ *(fmHole+i*m_nHoleSearchingAreaW+(j+1)))/2+ ( *(fmHole+(i)*m_nHoleSearchingAreaW+(j))+ *(fmHole+(i+1)*m_nHoleSearchingAreaW+(j)))/2)/2;
//				fmHole3[i*m_nHoleSearchingAreaW+j] = ((fmHole[i*m_nHoleSearchingAreaW+(j)]+fmHole[i*m_nHoleSearchingAreaW+(j+1)])/2+ (fmHole[(i)*m_nHoleSearchingAreaW+(j)]+fmHole[(i+1)*m_nHoleSearchingAreaW+(j)])/2)/2;
			else if( i<m_nHoleSearchingAreaH-1) // �����¿� ���
				*(fmHole3+i*m_nHoleSearchingAreaW+j) = (( *(fmHole+i*m_nHoleSearchingAreaW+(j-1))+*(fmHole+i*m_nHoleSearchingAreaW+(j))+*(fmHole+i*m_nHoleSearchingAreaW+(j+1)))/3+ (*(fmHole+(i-1)*m_nHoleSearchingAreaW+(j))+*(fmHole+(i)*m_nHoleSearchingAreaW+(j))+*(fmHole+(i+1)*m_nHoleSearchingAreaW+(j)))/3)/2;
			else // �� �Ʒ� ��������. �»� ���
				*(fmHole3+i*m_nHoleSearchingAreaW+j) = ((*(fmHole+i*m_nHoleSearchingAreaW+(j-1))+*(fmHole+i*m_nHoleSearchingAreaW+(j)))/2+ (*(fmHole+(i-1)*m_nHoleSearchingAreaW+(j))+*(fmHole+(i)*m_nHoleSearchingAreaW+(j)))/2)/2;

			if(i<1) // �� �� ù��° ��. ���� ���
				*(fmHole6+i*m_nHoleSearchingAreaW+j) = ((*(fmHole5+i*m_nHoleSearchingAreaW+(j))+*(fmHole5+i*m_nHoleSearchingAreaW+(j+1)))/2+ (*(fmHole5+(i)*m_nHoleSearchingAreaW+(j))+*(fmHole5+(i+1)*m_nHoleSearchingAreaW+(j)))/2)/2;
			else if( i<m_nHoleSearchingAreaH-1) // �����¿� ���
				*(fmHole6+i*m_nHoleSearchingAreaW+j) = ((*(fmHole5+i*m_nHoleSearchingAreaW+(j-1))+*(fmHole5+i*m_nHoleSearchingAreaW+(j))+*(fmHole5+i*m_nHoleSearchingAreaW+(j+1)))/3+ (*(fmHole5+(i-1)*m_nHoleSearchingAreaW+(j))+*(fmHole5+(i)*m_nHoleSearchingAreaW+(j))+*(fmHole5+(i+1)*m_nHoleSearchingAreaW+(j)))/3)/2;
			else // �� �Ʒ� ��������. �»� ���
				*(fmHole6+i*m_nHoleSearchingAreaW+j) = ((*(fmHole5+i*m_nHoleSearchingAreaW+(j-1))+*(fmHole5+i*m_nHoleSearchingAreaW+(j)))/2+ (*(fmHole5+(i-1)*m_nHoleSearchingAreaW+(j))+*(fmHole5+(i)*m_nHoleSearchingAreaW+(j)))/2)/2;
		}
	}

	int nBCnt=0,nwcnt = 0;

	// TH�������� ���̳ʸ�. fmHole1�� TH �̻��ΰ͵��� ������. fmHole4�� TH �̻��ΰ͵��� ������.
	for(int i=0;i<m_nHoleSearchingAreaH;i++) // y
	{
		for(int j=0;j<m_nHoleSearchingAreaW;j++) // x
		{
			//if(j<=nmargin1  || j>=m_nHoleSearchingAreaW-nmargin2)			
			//	*(fmHole1+i*m_nHoleSearchingAreaW+j) = 0;//�¿� ������ skip
			//else
			{
				if(pView->m_pDlgSet->m_FormTab4->m_bRegMode )
				{
					// Welding Tracker ����, �����ĺ�����(�����ڽ�)���� Tracker ���Ʒ��� �Ѿ�°͵鵵 skip
					if(pView->m_Display[0].m_rtImgTracker[5].top-m_rtHoleRect.top>i)
						*(fmHole1+i*m_nHoleSearchingAreaW+j) = 0;
					else if(m_nHoleSearchingAreaH-(m_rtHoleRect.bottom-pView->m_Display[0].m_rtImgTracker[5].bottom)<i)
						*(fmHole1+i*m_nHoleSearchingAreaW+j) = 0;
					else{
						// 3*3��ձ��Ѱſ��� TH �̻��ΰ͵��� 0. ���� ī��Ʈ++
						if( *(fmHole3+i*m_nHoleSearchingAreaW+j)>=dHoleTh)
						{
							*(fmHole1+i*m_nHoleSearchingAreaW+j) = 0;
							nBCnt++;
						}
						else{ // TH �̸��̸� ���� ī��Ʈ++	
							*(fmHole1+i*m_nHoleSearchingAreaW+j) = 255;
							nwcnt++;
						}
						// ���� �ݴ�. fmHole1�� TH �̻��ΰ͵��� ������. fmHole4�� TH �̻��ΰ͵��� ������.
						if( *(fmHole6+i*m_nHoleSearchingAreaW+j)>=dHoleTh)
							*(fmHole4+i*m_nHoleSearchingAreaW+j) = 255;
						else
							*(fmHole4+i*m_nHoleSearchingAreaW+j) = 0;
					}

				}
				else
				{
					if(*(fmHole3+i*m_nHoleSearchingAreaW+j)>=dHoleTh)
					{
						*(fmHole1+i*m_nHoleSearchingAreaW+j) = 0;
						nBCnt++;
					}
					else			
						*(fmHole1+i*m_nHoleSearchingAreaW+j) = 255;

					if(*(fmHole6+i*m_nHoleSearchingAreaW+j)>=dHoleTh)	
						*(fmHole4+i*m_nHoleSearchingAreaW+j) = 255;
					else				
						*(fmHole4+i*m_nHoleSearchingAreaW+j) = 0;
				}

			}
		}
	}

	pView->AddtoInspLog("Binary");

	if(nBCnt<10000 && pView->m_pDlgSet->m_FormTab4->m_bRegMode==FALSE)
	{
		unsigned long* integralImg = 0;
		int i, j;
		int nPitch=m_nHoleSearchingAreaW;
		int nHeight=m_nHoleSearchingAreaH;
		int S = nPitch/8;
		long sum=0;
		int count=0;
		int index;
		int x1, y1, x2, y2;
		int s2 = S/2;
		float T = 0.15f;

		// create the integral image
		integralImg = (unsigned long*)malloc(nPitch*nHeight*sizeof(unsigned long*));

		for (i=0; i<nPitch; i++)
		{
			// reset this column sum
			sum = 0;

			for (j=0; j<nHeight; j++)
			{
				index = j*nPitch+i;

				sum += fmHole[index];
				if (i==0)
					integralImg[index] = sum;
				else
					integralImg[index] = integralImg[index-1] + sum;
			}
		}

		// perform thresholding
		for (i=0; i<nPitch; i++)
		{
			for (j=0; j<nHeight; j++)
			{
				index = j*nPitch+i;

				// set the SxS region
				x1=i-s2; x2=i+s2;
				y1=j-s2; y2=j+s2;

				// check the border
				if (x1 < 0) x1 = 0;
				if (x2 >= nPitch) x2 = nPitch-1;
				if (y1 < 0) y1 = 0;
				if (y2 >= nHeight) y2 = nHeight-1;

				count = (x2-x1)*(y2-y1);

				// I(x,y)=s(x2,y2)-s(x1,y2)-s(x2,y1)+s(x1,x1)
				sum = integralImg[y2*nPitch+x2] -
					integralImg[y1*nPitch+x2] -
					integralImg[y2*nPitch+x1] +
					integralImg[y1*nPitch+x1];

				if ((long)(fmHole[index]*count) < (long)(sum*(1.0-T)))
					fmHole1[index] = 255;
				else
					fmHole1[index] = 0;
			}
		}

		free (integralImg);
	}

	// ���̳ʸ��Ѱ� ���̵� ���̵� ������ �����Ѵ�. ������� ���δ�?
	if(pView->m_nMachine>=2)// && !pView->m_pDlgSet->m_FormTab4->m_bRegMode)
	{
		for(int i=0;i<m_nHoleSearchingAreaH;i++) // y
		{
			for(int j=0;j<m_nHoleSearchingAreaW-2;j++) // x
			{
				// fmHole1�� TH �̻��ΰ͵��� ������ �� ��. ���� ������ ������
				if(*(fmHole1+i*m_nHoleSearchingAreaW+j)==255 &&*(fmHole1+i*m_nHoleSearchingAreaW+j+1)==255 &&*(fmHole1+i*m_nHoleSearchingAreaW+j+2)==0 )
					*(fmHole1+i*m_nHoleSearchingAreaW+j) = 0;
				else if(*(fmHole1+i*m_nHoleSearchingAreaW+j)==255 &&*(fmHole1+i*m_nHoleSearchingAreaW+j+1)==0 &&*(fmHole1+i*m_nHoleSearchingAreaW+j+2)==0 )
					*(fmHole1+i*m_nHoleSearchingAreaW+j) = 0;
				else if(*(fmHole1+i*m_nHoleSearchingAreaW+j)==0 &&*(fmHole1+i*m_nHoleSearchingAreaW+j+1)==255 &&*(fmHole1+i*m_nHoleSearchingAreaW+j+2)==0 )
					*(fmHole1+i*m_nHoleSearchingAreaW+j) = 0;
				else if(*(fmHole1+i*m_nHoleSearchingAreaW+j)==0 &&*(fmHole1+i*m_nHoleSearchingAreaW+j+1)==255 &&*(fmHole1+i*m_nHoleSearchingAreaW+j+2)==255 )
					*(fmHole1+i*m_nHoleSearchingAreaW+j) = 0;
	//			else fmHole6[i*m_nHoleSearchingAreaW+j] = fmHole1[i*m_nHoleSearchingAreaW+j];

				// fmHole4�� TH �̻��ΰ͵��� ������ �� ��. ���� ������ ������
				if(*(fmHole4+i*m_nHoleSearchingAreaW+j)==0 &&*(fmHole4+i*m_nHoleSearchingAreaW+j+1)==0 &&*(fmHole4+i*m_nHoleSearchingAreaW+j+2)==255 )
					*(fmHole4+i*m_nHoleSearchingAreaW+j) = 255;
				else if(*(fmHole4+i*m_nHoleSearchingAreaW+j)==0 &&*(fmHole4+i*m_nHoleSearchingAreaW+j+1)==255 &&*(fmHole4+i*m_nHoleSearchingAreaW+j+2)==255 )
					*(fmHole4+i*m_nHoleSearchingAreaW+j) = 255;
				else if(*(fmHole4+i*m_nHoleSearchingAreaW+j)==255 &&*(fmHole4+i*m_nHoleSearchingAreaW+j+1)==0 &&*(fmHole4+i*m_nHoleSearchingAreaW+j+2)==255 )
					*(fmHole4+i*m_nHoleSearchingAreaW+j) = 255;
				else if(*(fmHole4+i*m_nHoleSearchingAreaW+j)==255 &&*(fmHole4+i*m_nHoleSearchingAreaW+j+1)==0 &&*(fmHole4+i*m_nHoleSearchingAreaW+j+2)==0 )
					*(fmHole4+i*m_nHoleSearchingAreaW+j) = 255;

			}
		}
	//	memcpy(fmHole1,fmHole6,m_nHoleSearchingAreaW*m_nHoleSearchingAreaH);
	}

	for(int i=0;i<7;i++)m_dMaxChainData[i] = 0;
	//check hole position
	int nwhitecnt = 0;
	if(pView->m_pDlgSet->m_FormTab4->m_bWhiteCheck){
		// Input : fmHole6�� �������� 3*3����� ��, fmHole4�� TH �̻��ΰ͵��� ������ �� ��
		// �Լ� �����Ŀ��� fmHole6, fmHole4���� White ħ���Ѱɷ� �������
		nwhitecnt = FindHoleWhite(fmHole6,fmHole4,m_nHoleSearchingAreaW,m_nHoleSearchingAreaH); 

		if(pView->m_strComname.Find("3A")>=0||pView->m_strComname.Find("3B")>=0||pView->m_strComname.Find("3C")>=0 
			|| pView->m_strComname.Find("4A")>=0 || pView->m_strComname.Find("4B")>=0 || pView->m_strComname.Find("4C")>=0 
			|| pView->m_strComname.Find("5A")>=0 || pView->m_strComname.Find("5B")>=0 || pView->m_strComname.Find("5C")>=0
			|| pView->m_strComname.Find("6A")>=0 || pView->m_strComname.Find("6B")>=0 || pView->m_strComname.Find("6C")>=0
			|| pView->m_strComname.Find("7A")>=0 || pView->m_strComname.Find("7B")>=0 || pView->m_strComname.Find("7C")>=0
			|| pView->m_strComname.Find("8A")>=0 || pView->m_strComname.Find("8B")>=0 || pView->m_strComname.Find("8C")>=0)
			nwhitecnt = 0;
	}

	pView->AddtoInspLog("FindHoleWhite");

	if(nwhitecnt>0)
		; // ������ ã���� ������µ�,
	else
	{
		// ��ã������ �ٽ�
		// fmHole2�� ����. fmHole1�� TH �̻��ΰ͵��� ������ �� ��. holeave : ���� �� Tab ������ ��չ��
		// �Լ� ���� �Ŀ��� fmHole2���� ���������Ѱ�. 
		int pos = FindHolePosition(fmHole2,fmHole1,m_nHoleSearchingAreaW,m_nHoleSearchingAreaH,holeave); //morphology �� Y ��ġ �缳��
		if(pos==-1){
			delete fmHole;
			delete fmHole1;
			delete fmHole2;
			delete fmHole3;
			delete fmHole4;
			delete fmHole5;
			delete fmHole6;
			pView->AddtoInspLog("Find Hole Error");
			return 1;
		}
	//	if(pos>0)
	//	{//���� �ٽ� ����
	////pos <30 || >98 ���
	//		if(pos<30)			m_rtHoleRect.SetRect(m_nTapCenter-100,nCircleTopPos-64+noffset-(64-pos),m_nTapCenter+100,nCircleTopPos+64+noffset-(64-pos));
	//		else if(pos>98)		m_rtHoleRect.SetRect(m_nTapCenter-100,nCircleTopPos-64+noffset+(pos-64),m_nTapCenter+100,nCircleTopPos+64+noffset+(pos-64));
	//		m_rtHoleRect = CheckROI(m_rtHoleRect,CRect(100,100,CAMERA_HEIGHT-100,CAMERA_HEIGHT-100));

	//		m_Pro.ImageSelectCopy(m_fm[0],fmHole,CAMERA_WIDTH,CAMERA_HEIGHT,m_rtHoleRect,200,128,0,0,0);
	//		memcpy(fmHole1,fmHole,200*128);
	//		//���� ����ȭ
	//		int holeave = AreaAverage(0,fmHole,200,nmargin1,0,200-nmargin2,128);
	//		double dHoleTh = holeave * (pView->m_pDlgSet->m_FormTab4->m_nHoleTh*0.01);

	//		for(int i=0;i<128;i++)
	//		{
	//			for(int j=0;j<200;j++)
	//			{
	//				if(j<=nmargin1  || j>=200-nmargin2)			fmHole1[i*200+j] = 0;//�¿� ������ skip
	//				else
	//				{
	//					if(fmHole[i*200+j]>=dHoleTh)				fmHole1[i*200+j] = 0;
	//					else									fmHole1[i*200+j] = 255;
	//				}
	//			}
	//		}
	//		int pos = FindHolePosition1(fmHole2,fmHole1,200,128); //morphology �� Y ��ġ �缳��
	//	}
	}

	pView->AddtoInspLog("FindHolePosition");

	//ã�� point ���� Roi �׸���
	// fmHole : ����, fmHole1�� TH �̻��ΰ͵��� ������ �� ��
	FindRoi(fmHole,fmHole1,m_nHoleSearchingAreaW,nmargin1,nmargin2); // margin1 = m_nTapCenter - nmargin3(Tab �� ����)

	pView->AddtoInspLog("FindRoi");


	// hoho : 1ȣ��, 2ȣ��. �ʷϻ��ڽ��� �����ڽ� �� �����
	if(pView->m_nMachine==3 || (pView->m_nMachine==2 && pView->m_strComname.Find("CSW1")>=0 || pView->m_strComname.Find("CSW2")>=0) ){
		if(m_rtWeldRect[0].left < m_rtHoleRect.left){
			int offsetWedRect = m_rtHoleRect.left - m_rtWeldRect[0].left;
			m_rtWeldRect[0].OffsetRect( +offsetWedRect, 0);
			m_rtWeldRect[1].OffsetRect( +offsetWedRect, 0);
		}
		if(m_rtWeldRect[1].right > m_rtHoleRect.right){
			int offsetWedRect = m_rtWeldRect[1].right - m_rtHoleRect.right;
			m_rtWeldRect[0].OffsetRect( -offsetWedRect, 0);
			m_rtWeldRect[1].OffsetRect( -offsetWedRect, 0);
		}

		if(m_rtWeldRect[0].top < m_rtHoleRect.top){
			int offsetWedRect = m_rtHoleRect.top - m_rtWeldRect[0].top;
			m_rtWeldRect[0].OffsetRect(0, +offsetWedRect);
			m_rtWeldRect[1].OffsetRect(0, +offsetWedRect);
		}

		if(m_rtWeldRect[0].bottom > m_rtHoleRect.bottom){
			int offsetWedRect = m_rtWeldRect[0].bottom - m_rtHoleRect.bottom;
			m_rtWeldRect[0].OffsetRect(0, -offsetWedRect);
			m_rtWeldRect[1].OffsetRect(0, -offsetWedRect);
		}
	}
	if(pView->m_nMachine==2 && pView->m_pDlgSet->m_FormTab2->m_bLineWelding && pView->m_pDlgSet->m_FormTab4->m_bWeldingPosXFix){
		//welding�� top cap center�� �����ȴ�.
		int nWeldWidth = m_rtWeldRect[0].Width()*0.5;
		int ngap11 = 8;
		 m_rtWeldRect[0].left = m_ptCapCenter.x-nWeldWidth+ngap11;
		 m_rtWeldRect[0].right = m_ptCapCenter.x+nWeldWidth+ngap11;
		 m_rtWeldRect[1].left = m_ptCapCenter.x-nWeldWidth+ngap11;
		 m_rtWeldRect[1].right = m_ptCapCenter.x+nWeldWidth+ngap11;
	}
	
	for(int i=0;i<3;i++)memset(fmRectTh[i],0,m_nHoleSearchingAreaW*m_nHoleSearchingAreaH);
	//roi ���� ����
	FindHoleInfo(m_fm[0], fmHole, m_nHoleSearchingAreaW);

	pView->AddtoInspLog("FindHoleInfo");
	
	// whitecheck �ɼ��� Ű��, fmHole1���� fmHole4(TH �̻��ΰ͵��� ������ �� ���� White ħ��)���� �������
	// whitecheck �ɼ� ��Ű�� fmHole4�� �׳� TH �̻��ΰ͵��� ������ �� ����
	if(nwhitecnt>0)
		memcpy(fmHole1,fmHole4,m_nHoleSearchingAreaW*m_nHoleSearchingAreaH);

	if(pView->m_pDlgSet->m_FormTab4->m_bImageModify && pView->m_nMachine==3 && !pView->m_pDlgSet->m_FormTab4->m_bCheckView)
		memcpy(fmHole,fmHole5,m_nHoleSearchingAreaW*m_nHoleSearchingAreaH);

	// �̰� �Ⱦ��µ� �׳� ������
	for(int i=0;i<m_nHoleSearchingAreaH;i++)
		memcpy(m_fmHole+i*m_nHoleSearchingAreaW,fmHole+(m_nHoleSearchingAreaH-i-1)*m_nHoleSearchingAreaW,m_nHoleSearchingAreaW); 
//	if(pView->m_pDlgSet->m_FormTab4->m_bImageModify && pView->m_nMachine==2 && pView->m_pDlgSet->m_FormTab4->m_bCheckView)
//		memcpy(m_fm[0],m_fmTemp,CAMERA_WIDTH*CAMERA_HEIGHT);

//	memcpy(m_fmHole,fmHole,200*128);
	
	//main imgage�� ����
	if(pView->m_pDlgSet->m_FormTab2->m_bTapImageShow)
	{
		int originx,originy;
		for(int i=0;i<m_nHoleSearchingAreaH;i++)
		{
			// ����
			for(int j=0;j<m_nHoleSearchingAreaW;j++)
			{
				originx = j;//CAMERA_WIDTH-m_nHoleSearchingAreaW+j;
				m_fm[0][i*CAMERA_WIDTH+originx] = fmHole[(m_nHoleSearchingAreaH-i-1)*m_nHoleSearchingAreaW+j];
			}
			// FindHoleWhite()�� ������ ã������, TH �̻��ΰ͵��� ������ �� ���� White ħ���� ��
			// ����, FindHoleWhite()�� ������ ��ã�Ұų�, whitecheck �ɼ� ��Ű�� �׳� TH �̻��ΰ͵��� ������ �� ��
			for(int j=0;j<m_nHoleSearchingAreaW;j++)
			{
				originy = i;//+128;				
				originx = m_nHoleSearchingAreaW+j;//CAMERA_WIDTH-200+j;
				m_fm[0][originy*CAMERA_WIDTH+originx] = fmHole1[(m_nHoleSearchingAreaH-i-1)*m_nHoleSearchingAreaW+j];
			}
			// FindHoleWhite()�� ������ ã������, ����
			// ����, FindHoleWhite()�� ������ ��ã�Ұų�, whitecheck �ɼ� ��Ű�� ���������Ѱ�
			for(int j=0;j<m_nHoleSearchingAreaW;j++)
			{
				originy = i;//+(128*2)+1;				
				originx = (m_nHoleSearchingAreaW*2)+j;//CAMERA_WIDTH-200+j;
				m_fm[0][originy*CAMERA_WIDTH+originx] = fmHole2[(m_nHoleSearchingAreaH-i-1)*m_nHoleSearchingAreaW+j];
			}
			// whitecheck �ɼ��� Ű��, TH �̻��ΰ͵��� ������ �� ���� White ħ���� ��
			// whitecheck �ɼ� ��Ű��, fmHole4�� �׳� TH �̻��ΰ͵��� ������ �� ��
			for(int j=0;j<m_nHoleSearchingAreaW;j++)
			{
				originy = i;//+(128*2)+1;				
				originx = (m_nHoleSearchingAreaW*3)+j;//CAMERA_WIDTH-m_nHoleSearchingAreaW+j;
				m_fm[0][originy*CAMERA_WIDTH+originx] = fmHole4[(m_nHoleSearchingAreaH-i-1)*m_nHoleSearchingAreaW+j];
			}

/*		
			for(int j=0;j<200;j++)
			{
				originy = i;				originx = j;
				m_fm[0][originy*CAMERA_WIDTH+originx] = fmRectTh[0][(128-i-1)*200+j];
			}
			for(int j=0;j<200;j++)
			{
				originy = i+128;				originx = j;
				m_fm[0][originy*CAMERA_WIDTH+originx] = fmRectTh[1][(128-i-1)*200+j];
			}
			for(int j=0;j<200;j++)
			{
				originy = i+(128*2)+1;				originx = j;
				m_fm[0][originy*CAMERA_WIDTH+originx] = fmRectTh[2][(128-i-1)*200+j];
			}
			*/
		}
	}

	delete fmHole;
	delete fmHole1;
	delete fmHole2;
	delete fmHole3;
	delete fmHole4;
	delete fmHole5;
	delete fmHole6;
	return nret;
}

double CgProject::GetRoiInfo(BYTE* fm,int pitch,CRect rt, int nSkipX, int nSkipY,double *std,int *min,int *max,double *var,int nconversion)
{
	double dret = 0.;
	double dtemp1 = 0;
	double dtemp2 = 0;
	double dSum=0;
	int i,j,nSum=0, nCount=0;
	int nmin = 255,nmax = 0,ntmp = 0;
	CRect newrt;
	newrt = rt;
	for(j=newrt.top; j<newrt.bottom; j+=nSkipY){
		for(i=newrt.left; i<newrt.right; i+=nSkipX){
			ntmp = *(fm+j*pitch+i);
			if(ntmp < nmin)	
				nmin = ntmp;
			if(ntmp > nmax) 
				nmax = ntmp;
			nSum += ntmp;
			dtemp1 += ntmp;
			dtemp2 += ntmp*ntmp;
			nCount++;
		}
	}
	if(nCount != 0){
		dret = (double)nSum / nCount;
	}else{
		dret = 0;
	}
	//	Variance
	*var = (nCount*dtemp2-dtemp1*dtemp1)/(nCount*(nCount-1.)); // �л�
	*std = sqrt((nCount*dtemp2-dtemp1*dtemp1)/(nCount*(nCount-1.))); // ǥ������
	*min = nmin; // ������ �ּҰ�
	*max = nmax; // ������ �ִ밪
	nCount = 0;

	return dret; // ���� ��չ��
}


int CgProject::fnCirPosFind(double dmin)
{
	int i,nTmp,nCandiCount = 0;
	int nCandidatePosY = 0,nCandidatePosY1 = 0;
	int nThValue = 14,nStrongValue = 18;
	double dsum=0.,dave=0.;

	// Tab �߻�� ��� ������ ũ��(12�̻�),
	if(dmin > 12) 
	{
		nThValue = dmin+5;
		nStrongValue = nThValue+5;
	}
	double dmax = 0;

	// ���� ū �л갪�� ������ ������ Y(������ Top)�� ����
	for(i=0;i<m_nStdCount-1;i++)	{
		if(dmax<m_dVarValue[i])
		{
			dmax = m_dVarValue[i];
			nCandidatePosY = m_nStdLineInsp[i]; // �ᱹ, nPeak1y�̶� �Ȱ���
		}
		dsum += m_dVarValue[i];
	}
	// �� ������ ������ �л갪�� ���
	if(m_nStdCount<1)return 0;
	dave = dsum/m_nStdCount;

	// ��?
	if(nStrongValue<dave)
		nStrongValue = (dave+nStrongValue)*0.8+2;

	// �ش翵�� �л갪�� �������� �л갪�� ��պ��� ������, ���� ���� ����ְ�
	for(i=0;i<m_nStdCount-1;i++)
	{
		if(m_dVarValue[i] > dave+4)
		{
			m_nStartCircle = m_nStdLineInsp[i];		break;
		}
	}
	
	// �ش翵�� �л갪�� �������� �л갪�� ��պ��� ������, ���� �Ʒ��� ����ְ�
	for(i=m_nStdCount-1;i>0;i--)
	{
		if(m_dVarValue[i] > dave+4)
		{
			m_nEndCircle = m_nStdLineInsp[i];	break;
		}
	}

	for(i=1;i<m_nStdCount-1;i++)
	{
		nTmp = m_dVarValue[i];

		// �Ⱦ�
		if(nTmp>=nThValue)
		{
			nCandiCount++;
			if(nCandiCount==1)	
				nCandidatePosY1 = i;
		}
		else		
			nCandiCount = 0;
		//if(nCandiCount>=4)
		//{
		//	nCandidatePosY = m_nStdLineInsp[nCandidatePosY1];
		//	break;
		//}
		//else if(nCandiCount==0 && nTmp >=nStrongValue)

		// ������ ���� ���� ��ġ�� ����ش�
		if(nTmp>=nStrongValue)
		{
			nCandidatePosY = m_nStdLineInsp[i];	
			break;
		}
	}
	return nCandidatePosY;
}


int CgProject::FindHolePosition(BYTE *fmOrg,BYTE* fmTh,int pitch,int height,int ave)
{
	int nret =0;
	for(int i=0;i<20;i++){m_ptHole[i].SetPoint(0,0);m_ptHole1[i].SetPoint(0,0);m_ptHole2[i].SetPoint(0,0);}
	m_nHoleCandiCount = 0;

	BYTE * fmtmp = new BYTE[pitch*height];
	BYTE * fmtmp1 = new BYTE[pitch*height];
	BYTE * fmtmp2 = new BYTE[pitch*height];

	// fmtmp, fmtmp1, fmtmp2 ���� fmTh(TH �̻��ΰ͵��� ������ �� ��)�� ������
	memcpy(fmtmp,fmTh,pitch*height);
	memcpy(fmtmp1,fmTh,pitch*height);
	memcpy(fmtmp2,fmTh,pitch*height);


	CRect rt;
//	rt.SetRect(0,0,pitch-1,height-1);
	rt.SetRect(0,0,pitch,height);

	//morphology
	for(int i=0;i<pView->m_pDlgSet->m_FormTab4->m_nMorphCount;i++)
	{
		// �ѹ��ϸ� 0 �ι��ϸ� 0 1 �����ϸ� 0 1 0 
		if(i%2==0)	
			fnDilate(fmtmp, fmtmp1 , rt,0); // ħ��
		else
			fnDilate(fmtmp1, fmtmp , rt,1); // ��â
	}
//	memcpy(fmtmp3,fmtmp2,pitch*height);
	
	// fmtmp�� fmtmp1�� �Ȱ��� �����
	if(pView->m_pDlgSet->m_FormTab4->m_nMorphCount%2!=0)
		memcpy(fmtmp,fmtmp1,pitch*height);
	else	
		memcpy(fmtmp1,fmtmp,pitch*height);
	
	// fmOrg�� ���������Ѱ� ������
	memcpy(fmOrg,fmtmp,pitch*height);

	//cog�� ����� �����߽��� ���Ѵ�. 
	CPoint ptCog;
	ptCog = GetCog2(fmtmp,rt,m_nHoleSearchingAreaW,m_nHoleSearchingAreaH,1);//0 black 1white

	// �߽��� Ʋ���� ��� �̹��� �缳���� �Ѵ�.
	// �̰� ����. �ּ�ó����
	CString str;
	if(ptCog.y < 10 ||ptCog.y>115){
		str.Format("Position Recheck X:%d Y:%d",ptCog.x,ptCog.y);
		pView->AddtoInspLog(str);
		//projection���� top ��ġ�� ã�´�.

		if(pView->m_pDlgSet->m_FormTab2->m_bLineWelding)
		{
			// ù°��
			m_ptHole[m_nHoleCandiCount].SetPoint(100,70);
			m_ptHole1[m_nHoleCandiCount].x = 100 + m_rtHoleRect.left;
			m_ptHole1[m_nHoleCandiCount].y = 70 + m_rtHoleRect.top;
			m_ptHole2[m_nHoleCandiCount].x = 100 + CAMERA_WIDTH-m_nHoleSearchingAreaW;
			m_ptHole2[m_nHoleCandiCount].y = 100 + (m_nHoleSearchingAreaH*2);
			m_nHoleCandiCount++;

			// ��°��
			m_ptHole[m_nHoleCandiCount].SetPoint(100,100);
			m_ptHole1[m_nHoleCandiCount].x = 100 + m_rtHoleRect.left;
			m_ptHole1[m_nHoleCandiCount].y = 100 + m_rtHoleRect.top;
			m_ptHole2[m_nHoleCandiCount].x = 100 + CAMERA_WIDTH-m_nHoleSearchingAreaW;
			m_ptHole2[m_nHoleCandiCount].y = 100 + (m_nHoleSearchingAreaH*2);
		}
		delete fmtmp;
		delete fmtmp1;
		delete fmtmp2;
		return ptCog.y;
	}

	//blob	//
	double dTemp = 0,ratio=0,dCx,dCy,avg=0,dAngle,max,min;
	int maxx,minx,miny,maxy,nw,nh;
	int nAreaMin,nAreaMax;
	nAreaMin = 160;
	nAreaMax = 4000;
	if(pView->m_nMachine==2)
		nAreaMax = 2000;
	else if(pView->m_nMachine==3)
		nAreaMax = 2500;
	
	if(pView->m_nMachine==3)
		nAreaMin = 185;

	int	nBlobRecheck = 0;
	int ntop = 2;
	// fmtmp2�� ���������Ѱ� ������
	memcpy(fmtmp2,fmtmp,pitch*height); 
	// Fast Chain ����. object white, ���������Ѱ� ���(fmtmp), skipx : 2, skipy : 2, blob �ѷ��� 20�̻� 20000���ϸ� ���
	m_pChain->SetChainData(1,fmtmp,2,2,20,20000,pitch,height);
	// Fast Chain. Input : ����, Output : Blob ����
	int nBlobCount = m_pChain->FastChain(1, ntop, pitch-1,height-1);
	int count1=0;
	int dmaxpos=-1;
	int	nCx= 0,nCy = 0,ncc=0;
	int nbdata[3];//0 ���� 1minx 2maxx
	
	int nDown1 = 0,nDown1Pos = 1000;
	// Ȧ ����
	if(pView->m_pDlgSet->m_FormTab2->m_bLineWelding == FALSE && pView->m_pDlgSet->m_FormTab2->m_b2LineWelding == FALSE)
	{
		for(int i=0;i<nBlobCount;i++)
		{
			dTemp = m_pChain->Chain_Area(i);
			m_pChain->Chain_Center(i, &dCx, &dCy);
			dAngle = m_pChain->FindAngle(i);
			m_pChain->FineDistFromPoint(i, dCx,dCy, &min, &max, &avg);

			maxx = m_pChain->FindMaxX(i);
			minx = m_pChain->FindMinX(i);
			maxy = m_pChain->FindMaxY(i);
			miny = m_pChain->FindMinY(i);
			nw = maxx-minx;
			nh = maxy-miny;
			if(nh<1)continue;
			ratio = double(nw)/double(nh);

			if(pView->m_nMachine==3)
			{
				if(nDown1Pos>dCy)
				{
					nDown1Pos = dCy;
					nDown1 = i;

				}
			}
	//		if((maxy+miny)*0.5 <nTop || (maxy+miny)*0.5 > nBottom)		continue;
			if(m_dMaxChainData[0]<dTemp && (ratio<1.35 || ratio>0.7))
			{//����, cx,cy,w,h,ratio
				m_dMaxChainData[0] = dTemp;
				m_dMaxChainData[1]=dCx;
				m_dMaxChainData[2]=dCy;
				m_dMaxChainData[3] = nw;
				m_dMaxChainData[4]=nh;
				m_dMaxChainData[5]=ratio;

			}

			if(pView->m_nMachine==2){
				if(maxx >= pitch-2 || minx <= 1 || maxy >= height-10 || miny <= ntop+1)	
				{
					if(dTemp > 4000 && maxy-miny > 40 && maxx-minx>100)				nBlobRecheck = 1;continue;
				}
			}
			else
			{
				if(maxx >= pitch-2 || minx <= 1 || maxy >= height-2 || miny < ntop+1)	
				{
					if(dTemp>5800 && maxy-miny>100 && maxx-minx<110 )
						dmaxpos = i;
					if(dTemp > 3500 && maxy-miny > 40 && maxx-minx>100)				nBlobRecheck = 1;
					else if(dTemp>4000 && ratio < 1.25 && ratio >0.75)
					{
						m_ptHole[m_nHoleCandiCount].SetPoint(dCx,m_nHoleSearchingAreaH-dCy);
						m_ptHole1[m_nHoleCandiCount].x = dCx + m_rtHoleRect.left;
						m_ptHole1[m_nHoleCandiCount].y = m_nHoleSearchingAreaH-dCy + m_rtHoleRect.top;
						m_ptHole2[m_nHoleCandiCount].x = dCx + CAMERA_WIDTH-m_nHoleSearchingAreaW;
						m_ptHole2[m_nHoleCandiCount].y = m_nHoleSearchingAreaH-dCy + (m_nHoleSearchingAreaH*2);
						m_nHoleCandiCount++;
					}
					continue;
				}

			}
			if(dTemp>0 && dTemp<nAreaMax && dTemp>120 )
			{
				nCx += dCx;nCy += dCy;ncc++;
			}

			if(dTemp>nAreaMin && dTemp<nAreaMax && (ratio >=0.45 && ratio <= 2.8) && maxy-miny < 90 &&  miny>1 && maxy < m_nHoleSearchingAreaH-1)//min> 8 &&
			{
				nbdata[0] = dTemp;nbdata[1]=minx;nbdata[2]=maxx;
				if(pView->m_nMachine==3 && maxx-minx>90)
				{
					int ng1 = 60,nh1=20;
					m_ptHole[m_nHoleCandiCount].SetPoint((maxx-minx)*0.3+ng1,m_nHoleSearchingAreaH-dCy-nh1);
					m_ptHole1[m_nHoleCandiCount].x = (maxx-minx)*0.3+ng1 + m_rtHoleRect.left;
					m_ptHole1[m_nHoleCandiCount].y = m_nHoleSearchingAreaH-dCy + m_rtHoleRect.top-nh1;
					m_ptHole2[m_nHoleCandiCount].x = (maxx-minx)*0.3+ng1 + CAMERA_WIDTH-m_nHoleSearchingAreaW;
					m_ptHole2[m_nHoleCandiCount].y = m_nHoleSearchingAreaH-dCy + (m_nHoleSearchingAreaH*2)-nh1;
					m_nHoleCandiCount++;

					m_ptHole[m_nHoleCandiCount].SetPoint((maxx-minx)*0.66+ng1,m_nHoleSearchingAreaH-dCy-nh1);
					m_ptHole1[m_nHoleCandiCount].x = (maxx-minx)*0.66+ng1 + m_rtHoleRect.left;
					m_ptHole1[m_nHoleCandiCount].y = m_nHoleSearchingAreaH-dCy + m_rtHoleRect.top-nh1;
					m_ptHole2[m_nHoleCandiCount].x = (maxx-minx)*0.66+ng1 + CAMERA_WIDTH-m_nHoleSearchingAreaW;
					m_ptHole2[m_nHoleCandiCount].y = m_nHoleSearchingAreaH-dCy + (m_nHoleSearchingAreaH*2)-nh1;
					m_nHoleCandiCount++;
				}
				else
				{	
					if(pView->m_nMachine==3 && maxx==195)continue;
					int ntol = 0;
					if(pView->m_nMachine==3 && maxx-minx>=60 )
					{
						if(minx>85)						ntol = -(maxx-minx-pView->m_pDlgSet->m_FormTab4->m_nWeldingSize+10);
						else						ntol = (maxx-minx-pView->m_pDlgSet->m_FormTab4->m_nWeldingSize+10);
					}
					else if(pView->m_nMachine==3 && maxx-minx>=50 )
					{
						if(minx>85)						ntol = -(maxx-minx-pView->m_pDlgSet->m_FormTab4->m_nWeldingSize+5);
						else						ntol = (maxx-minx-pView->m_pDlgSet->m_FormTab4->m_nWeldingSize+5);
					}
					dTemp = dTemp;
					m_ptHole[m_nHoleCandiCount].SetPoint(dCx+ntol,m_nHoleSearchingAreaH-dCy);
					m_ptHole1[m_nHoleCandiCount].x = dCx+ntol + m_rtHoleRect.left;
					m_ptHole1[m_nHoleCandiCount].y = m_nHoleSearchingAreaH-dCy + m_rtHoleRect.top;
					m_ptHole2[m_nHoleCandiCount].x = dCx+ntol + CAMERA_WIDTH-m_nHoleSearchingAreaW;
					m_ptHole2[m_nHoleCandiCount].y = m_nHoleSearchingAreaW-dCy + (m_nHoleSearchingAreaW*2);

					m_nHoleCandiCount++;
				}
			}
			else if(pView->m_nMachine!=3&&dTemp > 4000 && maxy-miny > 40 && maxx-minx>100)
			{
				//�� 2��
				dTemp = dTemp;
				if(maxx-minx>125)
				{
					m_ptHole[m_nHoleCandiCount].SetPoint((maxx-minx)*0.3+30,m_nHoleSearchingAreaH-dCy);
					m_ptHole1[m_nHoleCandiCount].x = (maxx-minx)*0.3+30 + m_rtHoleRect.left;
					m_ptHole1[m_nHoleCandiCount].y = m_nHoleSearchingAreaH-dCy + m_rtHoleRect.top;
					m_ptHole2[m_nHoleCandiCount].x = (maxx-minx)*0.3+30 + CAMERA_WIDTH-m_nHoleSearchingAreaW;
					m_ptHole2[m_nHoleCandiCount].y = m_nHoleSearchingAreaH-dCy + (m_nHoleSearchingAreaH*2);
					m_nHoleCandiCount++;

					m_ptHole[m_nHoleCandiCount].SetPoint((maxx-minx)*0.66+30,m_nHoleSearchingAreaH-dCy);
					m_ptHole1[m_nHoleCandiCount].x = (maxx-minx)*0.66+30 + m_rtHoleRect.left;
					m_ptHole1[m_nHoleCandiCount].y = m_nHoleSearchingAreaH-dCy + m_rtHoleRect.top;
					m_ptHole2[m_nHoleCandiCount].x = (maxx-minx)*0.66+30 + CAMERA_WIDTH-m_nHoleSearchingAreaW;
					m_ptHole2[m_nHoleCandiCount].y = m_nHoleSearchingAreaH-dCy + (m_nHoleSearchingAreaH*2);
				}
				else
				{
					int ng1 = 60;
					m_ptHole[m_nHoleCandiCount].SetPoint((maxx-minx)*0.3+ng1,m_nHoleSearchingAreaH-dCy);
					m_ptHole1[m_nHoleCandiCount].x = (maxx-minx)*0.3+ng1 + m_rtHoleRect.left;
					m_ptHole1[m_nHoleCandiCount].y = m_nHoleSearchingAreaH-dCy + m_rtHoleRect.top;
					m_ptHole2[m_nHoleCandiCount].x = (maxx-minx)*0.3+ng1 + CAMERA_WIDTH-200;
					m_ptHole2[m_nHoleCandiCount].y = m_nHoleSearchingAreaH-dCy + (m_nHoleSearchingAreaH*2);
					m_nHoleCandiCount++;

					m_ptHole[m_nHoleCandiCount].SetPoint((maxx-minx)*0.66+ng1,m_nHoleSearchingAreaH-dCy);
					m_ptHole1[m_nHoleCandiCount].x = (maxx-minx)*0.66+ng1 + m_rtHoleRect.left;
					m_ptHole1[m_nHoleCandiCount].y = m_nHoleSearchingAreaH-dCy + m_rtHoleRect.top;
					m_ptHole2[m_nHoleCandiCount].x = (maxx-minx)*0.66+ng1 + CAMERA_WIDTH-m_nHoleSearchingAreaW;
					m_ptHole2[m_nHoleCandiCount].y = m_nHoleSearchingAreaH-dCy + (m_nHoleSearchingAreaH*2);
				}

				m_nHoleCandiCount++;

			}
			else if(pView->m_nMachine!=3&&dTemp>4000 && ratio < 1.3 && ratio >0.7 )
			{
				m_ptHole[m_nHoleCandiCount].SetPoint(dCx,m_nHoleSearchingAreaH-dCy);
				m_ptHole1[m_nHoleCandiCount].x = dCx + m_rtHoleRect.left;
				m_ptHole1[m_nHoleCandiCount].y = m_nHoleSearchingAreaH-dCy + m_rtHoleRect.top;
				m_ptHole2[m_nHoleCandiCount].x = dCx + CAMERA_WIDTH-m_nHoleSearchingAreaW;
				m_ptHole2[m_nHoleCandiCount].y = m_nHoleSearchingAreaH-dCy + (m_nHoleSearchingAreaH*2);

				m_nHoleCandiCount++;

			}
			else if(pView->m_nMachine==3&& dTemp>nAreaMax && (ratio >1.5 ) && maxx-minx>110 )
			{
				int nl =  (maxx+minx)/2-30;
				int nr =  (maxx+minx)/2+30;
				if( maxx-minx>130)
				{
					m_nHoleCandiCount= 0;
					m_ptHole[m_nHoleCandiCount].SetPoint(nl,m_nHoleSearchingAreaH-dCy);
					m_ptHole1[m_nHoleCandiCount].x = nl + m_rtHoleRect.left;
					m_ptHole1[m_nHoleCandiCount].y = m_nHoleSearchingAreaH-dCy + m_rtHoleRect.top;
					m_ptHole2[m_nHoleCandiCount].x = nl + CAMERA_WIDTH-m_nHoleSearchingAreaW;
					m_ptHole2[m_nHoleCandiCount].y = m_nHoleSearchingAreaH-dCy + (m_nHoleSearchingAreaH*2);
					m_nHoleCandiCount++;

					m_ptHole[m_nHoleCandiCount].SetPoint(nr,m_nHoleSearchingAreaH-dCy);
					m_ptHole1[m_nHoleCandiCount].x = nr + m_rtHoleRect.left;
					m_ptHole1[m_nHoleCandiCount].y = m_nHoleSearchingAreaH-dCy + m_rtHoleRect.top;
					m_ptHole2[m_nHoleCandiCount].x = nr + CAMERA_WIDTH-m_nHoleSearchingAreaW;
					m_ptHole2[m_nHoleCandiCount].y = m_nHoleSearchingAreaH-dCy + (m_nHoleSearchingAreaH*2);
					m_nHoleCandiCount++;
				}
				else
				{
					m_nHoleCandiCount= 0;
					m_ptHole[m_nHoleCandiCount].SetPoint((maxx-minx)*0.3+70,m_nHoleSearchingAreaH-dCy);
					m_ptHole1[m_nHoleCandiCount].x = (maxx-minx)*0.3+70 + m_rtHoleRect.left;
					m_ptHole1[m_nHoleCandiCount].y = m_nHoleSearchingAreaH-dCy + m_rtHoleRect.top;
					m_ptHole2[m_nHoleCandiCount].x = (maxx-minx)*0.3+70 + CAMERA_WIDTH-m_nHoleSearchingAreaW;
					m_ptHole2[m_nHoleCandiCount].y = m_nHoleSearchingAreaH-dCy + (m_nHoleSearchingAreaH*2);
					m_nHoleCandiCount++;

					m_ptHole[m_nHoleCandiCount].SetPoint((maxx-minx)*0.66+70,m_nHoleSearchingAreaH-dCy);
					m_ptHole1[m_nHoleCandiCount].x = (maxx-minx)*0.66+70 + m_rtHoleRect.left;
					m_ptHole1[m_nHoleCandiCount].y = m_nHoleSearchingAreaH-dCy + m_rtHoleRect.top;
					m_ptHole2[m_nHoleCandiCount].x = (maxx-minx)*0.66+70 + CAMERA_WIDTH-m_nHoleSearchingAreaW;
					m_ptHole2[m_nHoleCandiCount].y = m_nHoleSearchingAreaH-dCy + (m_nHoleSearchingAreaH*2);
					m_nHoleCandiCount++;
				}
				break;

			}
			else if(pView->m_nMachine==3&& dTemp>nAreaMax &&(ratio <1.2 ||ratio>0.9) &&  maxy-miny>60 )
			{
				m_nHoleCandiCount= 0;
				m_ptHole[m_nHoleCandiCount].SetPoint((maxx-minx)*0.2+minx,m_nHoleSearchingAreaH-dCy);
				m_ptHole1[m_nHoleCandiCount].x = (maxx-minx)*0.2+minx + m_rtHoleRect.left;
				m_ptHole1[m_nHoleCandiCount].y = m_nHoleSearchingAreaH-dCy + m_rtHoleRect.top;
				m_ptHole2[m_nHoleCandiCount].x = (maxx-minx)*0.2+minx + CAMERA_WIDTH-m_nHoleSearchingAreaW;
				m_ptHole2[m_nHoleCandiCount].y = m_nHoleSearchingAreaH-dCy + (m_nHoleSearchingAreaH*2);
				m_nHoleCandiCount++;

				m_ptHole[m_nHoleCandiCount].SetPoint((maxx-minx)*0.56+minx,m_nHoleSearchingAreaH-dCy);
				m_ptHole1[m_nHoleCandiCount].x = (maxx-minx)*0.56+minx + m_rtHoleRect.left;
				m_ptHole1[m_nHoleCandiCount].y = m_nHoleSearchingAreaH-dCy + m_rtHoleRect.top;
				m_ptHole2[m_nHoleCandiCount].x = (maxx-minx)*0.56+minx + CAMERA_WIDTH-m_nHoleSearchingAreaW;
				m_ptHole2[m_nHoleCandiCount].y = m_nHoleSearchingAreaH-dCy + (m_nHoleSearchingAreaH*2);
				m_nHoleCandiCount++;
				break;

			}
			else if(pView->m_nMachine==3&& dTemp>nAreaMax &&  maxx-minx>90 )
			{
				m_nHoleCandiCount= 0;
				m_ptHole[m_nHoleCandiCount].SetPoint((maxx-minx)*0.3+minx,m_nHoleSearchingAreaH-dCy);
				m_ptHole1[m_nHoleCandiCount].x = (maxx-minx)*0.3+minx + m_rtHoleRect.left;
				m_ptHole1[m_nHoleCandiCount].y = m_nHoleSearchingAreaH-dCy + m_rtHoleRect.top;
				m_ptHole2[m_nHoleCandiCount].x = (maxx-minx)*0.3+minx + CAMERA_WIDTH-m_nHoleSearchingAreaW;
				m_ptHole2[m_nHoleCandiCount].y = m_nHoleSearchingAreaH-dCy + (m_nHoleSearchingAreaH*2);
				m_nHoleCandiCount++;

				m_ptHole[m_nHoleCandiCount].SetPoint((maxx-minx)*0.66+minx,m_nHoleSearchingAreaH-dCy);
				m_ptHole1[m_nHoleCandiCount].x = (maxx-minx)*0.66+minx + m_rtHoleRect.left;
				m_ptHole1[m_nHoleCandiCount].y = m_nHoleSearchingAreaH-dCy + m_rtHoleRect.top;
				m_ptHole2[m_nHoleCandiCount].x = (maxx-minx)*0.66+minx + CAMERA_WIDTH-m_nHoleSearchingAreaW;
				m_ptHole2[m_nHoleCandiCount].y = m_nHoleSearchingAreaH-dCy + (m_nHoleSearchingAreaH*2);
				m_nHoleCandiCount++;
				break;

			}
		}
		//1ȣ�� �ٴ� ���ַ� ã��
		if(pView->m_nMachine==3 && m_nHoleCandiCount !=2)
		{
			dTemp = m_pChain->Chain_Area(nDown1);
			m_pChain->Chain_Center(nDown1, &dCx, &dCy);
			dAngle = m_pChain->FindAngle(nDown1);
			m_pChain->FineDistFromPoint(nDown1, dCx,dCy, &min, &max, &avg);

			maxx = m_pChain->FindMaxX(nDown1);
			minx = m_pChain->FindMinX(nDown1);
			maxy = m_pChain->FindMaxY(nDown1);
			miny = m_pChain->FindMinY(nDown1); 
			nw = maxx-minx;
			nh = maxy-miny;
			nbdata[0] = dTemp;nbdata[1]=minx;nbdata[2]=maxx;

//			if(nw>100)
			{
				m_nHoleCandiCount= 0;
				m_ptHole[m_nHoleCandiCount].SetPoint((maxx-minx)*0.2+minx,m_nHoleSearchingAreaH-dCy);
				m_ptHole1[m_nHoleCandiCount].x = (maxx-minx)*0.2+minx + m_rtHoleRect.left;
				m_ptHole1[m_nHoleCandiCount].y = m_nHoleSearchingAreaH-dCy + m_rtHoleRect.top;
				m_ptHole2[m_nHoleCandiCount].x = (maxx-minx)*0.2+minx + CAMERA_WIDTH-m_nHoleSearchingAreaW;
				m_ptHole2[m_nHoleCandiCount].y = m_nHoleSearchingAreaH-dCy + (m_nHoleSearchingAreaH*2);
				m_nHoleCandiCount++;

				m_ptHole[m_nHoleCandiCount].SetPoint((maxx-minx)*0.56+minx,m_nHoleSearchingAreaH-dCy);
				m_ptHole1[m_nHoleCandiCount].x = (maxx-minx)*0.56+minx + m_rtHoleRect.left;
				m_ptHole1[m_nHoleCandiCount].y = m_nHoleSearchingAreaH-dCy + m_rtHoleRect.top;
				m_ptHole2[m_nHoleCandiCount].x = (maxx-minx)*0.56+minx + CAMERA_WIDTH-m_nHoleSearchingAreaW;
				m_ptHole2[m_nHoleCandiCount].y = m_nHoleSearchingAreaH-dCy + (m_nHoleSearchingAreaH*2);
				m_nHoleCandiCount++;
			}


		}
		//1ȣ�� �ǿ����� ����
		if(pView->m_nMachine==3 && m_nHoleCandiCount==1 && m_ptCapCenter.x-m_nTopEdge[0]>-10 && nbdata[2]>0 && nbdata[1]>0 && nbdata[1]>0)
		{
			int nw1 = nbdata[2]-nbdata[1];
			if(nbdata[1]<22 && nw1 >50)
			{
				m_nHoleCandiCount= 0;
				m_ptHole[m_nHoleCandiCount].x = nbdata[1];
				m_ptHole1[m_nHoleCandiCount].x = nbdata[1] + m_rtHoleRect.left;
				m_ptHole2[m_nHoleCandiCount].x = nbdata[1] + CAMERA_WIDTH-m_nHoleSearchingAreaW;

				m_nHoleCandiCount++;
				m_ptHole[m_nHoleCandiCount] = m_ptHole[m_nHoleCandiCount-1];
				m_ptHole1[m_nHoleCandiCount] = m_ptHole1[m_nHoleCandiCount-1];
				m_ptHole2[m_nHoleCandiCount] = m_ptHole2[m_nHoleCandiCount-1];
				m_ptHole[m_nHoleCandiCount].x = nbdata[1]+60;
				m_ptHole1[m_nHoleCandiCount].x = nbdata[1]+60 + m_rtHoleRect.left;
				m_ptHole2[m_nHoleCandiCount].x = nbdata[1]+60 + CAMERA_WIDTH-m_nHoleSearchingAreaW;
				m_nHoleCandiCount++;
			}
		}
		//recheck black point
		if(m_nHoleCandiCount==0 && nBlobRecheck==1)
		{
			m_pChain->SetChainData(0,fmtmp1,2,2,10,20000,pitch,height);
			nBlobCount = m_pChain->FastChain(0, 0, pitch-1,height-1);
			for(int i=0;i<nBlobCount;i++)
			{			
				dTemp = m_pChain->Chain_Area(i);
				m_pChain->Chain_Center(i, &dCx, &dCy);
				dAngle = m_pChain->FindAngle(i);
				m_pChain->FineDistFromPoint(i, dCx,dCy, &min, &max, &avg);
				maxx = m_pChain->FindMaxX(i);
				minx = m_pChain->FindMinX(i);
				maxy = m_pChain->FindMaxY(i);
				miny = m_pChain->FindMinY(i); 
				nw = maxx-minx;
				nh = maxy-miny;
				ratio = double(nw)/double(nh);
				if(maxx >= pitch-2 || minx <= 1 || maxy >= height-2 || miny <= 1)	
					continue;
				if(dTemp>150 && dTemp<nAreaMax && (ratio >=0.5 && ratio <= 2.0) && maxy-miny < 90 &&  miny>1 && maxy < m_nHoleSearchingAreaH-1)//min> 8 &&
				{
					dTemp = dTemp;
					m_ptHole[m_nHoleCandiCount].SetPoint(dCx,m_nHoleSearchingAreaH-dCy);
					m_ptHole1[m_nHoleCandiCount].x = dCx + m_rtHoleRect.left;
					m_ptHole1[m_nHoleCandiCount].y = m_nHoleSearchingAreaH-dCy + m_rtHoleRect.top;
					m_ptHole2[m_nHoleCandiCount].x = dCx + CAMERA_WIDTH-m_nHoleSearchingAreaW;
					m_ptHole2[m_nHoleCandiCount].y = m_nHoleSearchingAreaH-dCy + (m_nHoleSearchingAreaH*2);

					m_nHoleCandiCount++;
				}

			}

		}
		if(m_nHoleCandiCount==0 && nBlobRecheck==0 && nCx !=0 && nCy!=0)
		{
			if(ncc==0)ncc = 1;
			nCx /= ncc;nCy /= ncc;
			m_ptHole[m_nHoleCandiCount].SetPoint(nCx,m_nHoleSearchingAreaH-nCy);
			m_ptHole1[m_nHoleCandiCount].x = nCx + m_rtHoleRect.left;
			m_ptHole1[m_nHoleCandiCount].y = m_nHoleSearchingAreaH-nCy + m_rtHoleRect.top;
			m_ptHole2[m_nHoleCandiCount].x = nCx + CAMERA_WIDTH-m_nHoleSearchingAreaW;
			m_ptHole2[m_nHoleCandiCount].y = m_nHoleSearchingAreaH-nCy + (m_nHoleSearchingAreaH*2);
			m_nHoleCandiCount++;
		}
		if(m_nHoleCandiCount==0 && nBlobRecheck==0 &&dmaxpos>-1)
		{
			maxx = m_pChain->FindMaxX(dmaxpos);
			minx = m_pChain->FindMinX(dmaxpos);
			maxy = m_pChain->FindMaxY(dmaxpos);
			miny = m_pChain->FindMinY(dmaxpos); 
			m_pChain->Chain_Center(dmaxpos, &dCx, &dCy);


			m_ptHole[m_nHoleCandiCount].SetPoint(dCx,m_nHoleSearchingAreaH-dCy);
			m_ptHole1[m_nHoleCandiCount].x = dCx + m_rtHoleRect.left;
			m_ptHole1[m_nHoleCandiCount].y = m_nHoleSearchingAreaH-dCy + m_rtHoleRect.top;
			m_ptHole2[m_nHoleCandiCount].x = dCx + CAMERA_WIDTH-m_nHoleSearchingAreaW;
			m_ptHole2[m_nHoleCandiCount].y = m_nHoleSearchingAreaH-dCy + (m_nHoleSearchingAreaH*2);
			m_nHoleCandiCount++;

		}
		if(m_nHoleCandiCount==2)
		{
			if(m_ptHole[0].x>m_ptHole[1].x)
			{
				m_ptHole[3] = m_ptHole[0];
				m_ptHole1[3] = m_ptHole1[0];
				m_ptHole2[3] = m_ptHole2[0];
				m_ptHole[0] = m_ptHole[1];
				m_ptHole1[0] = m_ptHole1[1];
				m_ptHole2[0] = m_ptHole2[1];
				m_ptHole[1] = m_ptHole[3];
				m_ptHole1[1] = m_ptHole1[3];
				m_ptHole2[1] = m_ptHole2[3];
			}
		}
	} // 1���� ����
	else if(pView->m_pDlgSet->m_FormTab2->m_bLineWelding == TRUE && pView->m_pDlgSet->m_FormTab2->m_b2LineWelding == FALSE)
	{
		//���� ū�� ã��
		double dmax1 = 0;int nmaxno = -1;
		for(int i=0;i<nBlobCount;i++)
		{
			dTemp = m_pChain->Chain_Area(i); // Blob ����
			m_pChain->Chain_Center(i, &dCx, &dCy); // Blob �߽�
			dAngle = m_pChain->FindAngle(i); // Blob ����
			m_pChain->FineDistFromPoint(i, dCx,dCy, &min, &max, &avg); // Blob �߽ɿ��� ���� ª���Ÿ�, ��Ÿ�, ��հŸ�
			maxx = m_pChain->FindMaxX(i); // Blob X �ִ밪
			minx = m_pChain->FindMinX(i); // Blob X �ּҰ�
			maxy = m_pChain->FindMaxY(i); // Blob Y �ִ밪
			miny = m_pChain->FindMinY(i); // Blob Y �ּҰ�
			nw = maxx-minx; // Blob ��
			nh = maxy-miny; // Blob ����
			if(nh<1)continue;
			ratio = double(nw)/double(nh); // ���� �� �� ����. �¿�� ��������� 1���� Ŀ��. ���Ʒ��� ��������� 1���� �۾���.

			// Blob�� �� ���� ���� �����Ŷ� �װ� ��ȣ
			if(dTemp>=dmax1){
				dmax1 = dTemp;
				nmaxno = i;
			}

			// Blob�� �� ���� ���� ������ ���� �ֱ�
			if(m_dMaxChainData[0]<dTemp )
			{//����, cx,cy,w,h,ratio
				m_dMaxChainData[0] = dTemp;
				m_dMaxChainData[1]=dCx;
				m_dMaxChainData[2]=dCy;
				m_dMaxChainData[3] = nw;
				m_dMaxChainData[4]=nh;
				m_dMaxChainData[5]=ratio;
			}
		}
		if(nmaxno==-1){
			delete fmtmp;
			delete fmtmp1;
			delete fmtmp2;
			return -1;
		}
		// Blob�� �� ���� ���� �������� X �ִ�/�ּҰ�, Y �ִ�/�ּҰ�
		maxx = m_pChain->FindMaxX(nmaxno);
		minx = m_pChain->FindMinX(nmaxno);
		maxy = m_pChain->FindMaxY(nmaxno);
		miny = m_pChain->FindMinY(nmaxno); 
		//X-cog
		m_pChain->Chain_Center(nmaxno, &dCx, &dCy); // �װ� �߽�
		CString strtmp;
		strtmp.Format("Cx1 %.0f, Cog %.d",dCx,ptCog.x);
		pView->AddtoInspLog(strtmp);

		// ��� �����߽�X�� ������ ���� Blob�߽ɿ� ���
		dCx = ptCog.x;

		m_ptHole[0].SetPoint(dCx,m_nHoleSearchingAreaH-dCy);
		m_ptHole1[0].x = dCx + m_rtHoleRect.left;
		m_ptHole1[0].y = m_nHoleSearchingAreaH-dCy + m_rtHoleRect.top;
		m_ptHole2[0].x = dCx + CAMERA_WIDTH-m_nHoleSearchingAreaW;
		m_ptHole2[0].y = m_nHoleSearchingAreaH-dCy + (m_nHoleSearchingAreaH*2);
		m_nHoleCandiCount++;

		//���� �β� ����
		if(pView->m_pDlgSet->m_FormTab2->m_bLineWelding)
		{
			////[0] min pos [1] max pos [2]min hight [3]max hight
			for(int i=0;i<200;i++)m_nLineHeight[i] = -1;
			m_nLinMinMax[0]= m_nLinMinMax[2] = 1000;
			m_nLinMinMax[1] = m_nLinMinMax[3] = 0;
			// Fast Chain ����. object white, ���������Ѱ� ���(fmtmp2), skipx : 2, skipy : 2, blob �ѷ��� 50�̻� 20000���ϸ� ���
			m_pChain->SetChainData(1,fmtmp2,2,2,50,20000,pitch,height);
			int nBlobCount = m_pChain->FastChain(1, 1, pitch-1,height-1);
			int	nCx= 0,nCy = 0,ncc=0,maxx,minx,maxy,miny,nmaxno=0;
			int nbdata[3];//0 ���� 1minx 2maxx
			double dTemp = 0,dCx,dCy,dAngle,avg,min,max,perimeter,moment,dtmparea=0;
			for(int i=0;i<nBlobCount;i++)
			{
				dTemp = m_pChain->Chain_Area(i);
				// ���� ū ������ �� ������ ������ Blob ��ȣ
				if(dTemp>dtmparea){
					dtmparea =dTemp;
					nmaxno=i;
				}
			}
			maxx = m_pChain->FindMaxX(nmaxno);
			minx = m_pChain->FindMinX(nmaxno);
			maxy = m_pChain->FindMaxY(nmaxno);
			miny = m_pChain->FindMinY(nmaxno); 
			int nsx,nex;//m_dMinHeight //m_dMaxHeight

			// ���� ū ������ ���� 80�̻��� ��,
			if(maxx-minx>80){
				nsx = minx+15; // ���� ������ġ
				nex = maxx-15; // ���� ������ġ
			}
			else
			{
				nsx = 60;
				nex = 160;
			}
			// ����. ���� �������� ���� �ľ�
//			for(int i=minx+15;i<maxx-15;i++)
			for(int i=nsx;i<nex;i++)
			{
				int nb1 = 0,nbmax=0;
				for(int j=0;j<height;j++)
				{//white
					if(nb1>nbmax)
						nbmax = nb1;
//					nbdata[3] = *(fmtmp2+j*pitch+i);
					if(*(fmtmp2+j*pitch+i) > 125){
						nb1++;
					}
					else 
						nb1 = 0;
				}
				m_nLineHeight[i] = nbmax;
				if(m_nLinMinMax[0]>nbmax){
					m_nLinMinMax[0] = nbmax; // ���� �ּҰ�
					m_nLinMinMax[2] = i; // �� ��ġ
				}
				else if(m_nLinMinMax[1]<nbmax){
					m_nLinMinMax[1] = nbmax; // ���� �ִ밪
					m_nLinMinMax[3] = i; // �� ��ġ
				}
			}
		}
	}
	else if(pView->m_pDlgSet->m_FormTab2->m_bLineWelding == TRUE && pView->m_pDlgSet->m_FormTab2->m_b2LineWelding == TRUE)
	{ // hoho
		//���� ū�� ã��
		double dmax1 = 0;int nmaxno = -1;
		for(int i=0;i<nBlobCount;i++)
		{
			dTemp = m_pChain->Chain_Area(i); // Blob ����
			m_pChain->Chain_Center(i, &dCx, &dCy); // Blob �߽�
			dAngle = m_pChain->FindAngle(i); // Blob ����
			m_pChain->FineDistFromPoint(i, dCx,dCy, &min, &max, &avg); // Blob �߽ɿ��� ���� ª���Ÿ�, ��Ÿ�, ��հŸ�
			maxx = m_pChain->FindMaxX(i); // Blob X �ִ밪
			minx = m_pChain->FindMinX(i); // Blob X �ּҰ�
			maxy = m_pChain->FindMaxY(i); // Blob Y �ִ밪
			miny = m_pChain->FindMinY(i); // Blob Y �ּҰ�
			nw = maxx-minx; // Blob ��
			nh = maxy-miny; // Blob ����
			if(nh<1)continue;
			ratio = double(nw)/double(nh); // ���� �� �� ����. �¿�� ��������� 1���� Ŀ��. ���Ʒ��� ��������� 1���� �۾���.

			// Blob�� �� ���� ���� �����Ŷ� �װ� ��ȣ
			if(dTemp>=dmax1){
				dmax1 = dTemp;
				nmaxno = i;
			}

			// Blob�� �� ���� ���� ������ ���� �ֱ�
			if(m_dMaxChainData[0]<dTemp )
			{//����, cx,cy,w,h,ratio
				m_dMaxChainData[0] = dTemp;
				m_dMaxChainData[1]=dCx;
				m_dMaxChainData[2]=dCy;
				m_dMaxChainData[3] = nw;
				m_dMaxChainData[4]=nh;
				m_dMaxChainData[5]=ratio;
			}
		}
		if(nmaxno==-1){
			delete fmtmp;
			delete fmtmp1;
			delete fmtmp2;
			return -1;
		}
		// Blob�� �� ���� ���� �������� X �ִ�/�ּҰ�, Y �ִ�/�ּҰ�
		maxx = m_pChain->FindMaxX(nmaxno);
		minx = m_pChain->FindMinX(nmaxno);
		maxy = m_pChain->FindMaxY(nmaxno);
		miny = m_pChain->FindMinY(nmaxno); 
		//X-cog
		m_pChain->Chain_Center(nmaxno, &dCx, &dCy); // �װ� �߽�
		CString strtmp;
		strtmp.Format("Cx1 %.0f, Cog %.d",dCx,ptCog.x);
		pView->AddtoInspLog(strtmp);

		// ��� �����߽�X�� ������ ���� Blob�߽ɿ� ���
		dCx = ptCog.x;

		if((m_nHoleSearchingAreaH-dCy) > m_nHoleSearchingAreaH*0.5 +10)//margin�ʿ�.
		{
			// ù°��
			m_ptHole[m_nHoleCandiCount].SetPoint(dCx,m_nHoleSearchingAreaH-dCy-35);
			m_ptHole1[m_nHoleCandiCount].x = dCx + m_rtHoleRect.left;
			m_ptHole1[m_nHoleCandiCount].y = m_nHoleSearchingAreaH-dCy-35 + m_rtHoleRect.top;
			m_ptHole2[m_nHoleCandiCount].x = dCx + CAMERA_WIDTH-m_nHoleSearchingAreaW;
			m_ptHole2[m_nHoleCandiCount].y = (maxy-miny)*0.5 + (m_nHoleSearchingAreaH*2);
			m_nHoleCandiCount++;

			// ��°��
			m_ptHole[m_nHoleCandiCount].SetPoint(dCx,m_nHoleSearchingAreaH-dCy);
			m_ptHole1[m_nHoleCandiCount].x = dCx + m_rtHoleRect.left;
			m_ptHole1[m_nHoleCandiCount].y = m_nHoleSearchingAreaH-dCy + m_rtHoleRect.top;
			m_ptHole2[m_nHoleCandiCount].x = dCx + CAMERA_WIDTH-m_nHoleSearchingAreaW;
			m_ptHole2[m_nHoleCandiCount].y = (maxy-miny)*0.5 + (m_nHoleSearchingAreaH*2);
		}
		else{
			// ù°��
			m_ptHole[m_nHoleCandiCount].SetPoint(dCx,m_nHoleSearchingAreaH-dCy);
			m_ptHole1[m_nHoleCandiCount].x = dCx + m_rtHoleRect.left;
			m_ptHole1[m_nHoleCandiCount].y = (m_nHoleSearchingAreaH-dCy) + m_rtHoleRect.top;
			m_ptHole2[m_nHoleCandiCount].x = dCx + CAMERA_WIDTH-m_nHoleSearchingAreaW;
			m_ptHole2[m_nHoleCandiCount].y = (maxy-miny)*0.5 + (m_nHoleSearchingAreaH*2);
			m_nHoleCandiCount++;

			// ��°��
			m_ptHole[m_nHoleCandiCount].SetPoint(dCx,m_nHoleSearchingAreaH-dCy+35);
			m_ptHole1[m_nHoleCandiCount].x = dCx + m_rtHoleRect.left;
			m_ptHole1[m_nHoleCandiCount].y = m_nHoleSearchingAreaH-dCy+35 + m_rtHoleRect.top;
			m_ptHole2[m_nHoleCandiCount].x = dCx + CAMERA_WIDTH-m_nHoleSearchingAreaW;
			m_ptHole2[m_nHoleCandiCount].y = (maxy-miny)*0.5 + (m_nHoleSearchingAreaH*2);
		}
	}

	delete fmtmp;
	delete fmtmp1;
	delete fmtmp2;
	return nret;
}


int CgProject::FindHolePosition1(BYTE *fmOrg,BYTE* fmTh,int pitch,int height)
{
	int nret =0;

	BYTE * fmtmp = new BYTE[pitch*height];
	BYTE * fmtmp1 = new BYTE[pitch*height];
	memcpy(fmtmp,fmTh,pitch*height);
	memcpy(fmtmp1,fmTh,pitch*height);
	CRect rt;
	rt.SetRect(0,0,pitch,height);
	//morphology
	for(int i=0;i<pView->m_pDlgSet->m_FormTab4->m_nMorphCount;i++)
	{
		if(i%2==0)	
			fnDilate(fmtmp, fmtmp1 , rt,1) ;
		else
			fnDilate(fmtmp1, fmtmp , rt,1) ;
	}

	if(pView->m_pDlgSet->m_FormTab4->m_nMorphCount%2!=0)
		memcpy(fmtmp,fmtmp1,pitch*height);	
	memcpy(fmOrg,fmtmp,pitch*height);

	//blob	//
	double dTemp = 0,ratio=0,dCx,dCy,avg=0,dAngle,max,min;
	int maxx,minx,miny,maxy,nw,nh;
	int nAreaMin,nAreaMax;
	nAreaMin = 200;
	nAreaMax = 4000;

	m_pChain->SetChainData(1,fmtmp,2,2,50,8000,pitch,height);
	int nBlobCount = m_pChain->FastChain(0, 0, pitch-1,height-1);

	for(int i=0;i<nBlobCount;i++)
	{
		dTemp = m_pChain->Chain_Area(i);
		m_pChain->Chain_Center(i, &dCx, &dCy);
		dAngle = m_pChain->FindAngle(i);
		m_pChain->FineDistFromPoint(i, dCx,dCy, &min, &max, &avg);

		maxx = m_pChain->FindMaxX(i);
		minx = m_pChain->FindMinX(i);
		maxy = m_pChain->FindMaxY(i);
		miny = m_pChain->FindMinY(i); 
		nw = maxx-minx;
		nh = maxy-miny;
		ratio = double(nw)/double(nh);
		//		if((maxy+miny)*0.5 <nTop || (maxy+miny)*0.5 > nBottom)		continue;
		if(maxx >= pitch-2 || minx <= 1 || maxy >= height-2 || miny <= 1)			continue;

//		if(dTemp>nAreaMin && dTemp<nAreaMax && (ratio >=0.45 && ratio <= 1.9) && maxy-miny < 100)//&&  min>1 && min < 80 && max<110)//min> 8 &&
		if(dTemp>nAreaMin && dTemp<nAreaMax && (ratio >=0.45 && ratio <= 2.4) && maxy-miny < 90 &&  miny>1 && maxy < 127)//min> 8 &&
		{
			dTemp = dTemp;
			m_ptHole[m_nHoleCandiCount].SetPoint(dCx,m_nHoleSearchingAreaH-dCy);// �������� �̹����� ����Ʈ
			m_ptHole1[m_nHoleCandiCount].x = dCx + m_rtHoleRect.left;//������ ������� ����Ʈ
			m_ptHole1[m_nHoleCandiCount].y = m_nHoleSearchingAreaH-dCy + m_rtHoleRect.top;//������ ������� ����Ʈ
			m_ptHole2[m_nHoleCandiCount].x = dCx + CAMERA_WIDTH-m_nHoleSearchingAreaW;//���� �������� ����Ʈ
			m_ptHole2[m_nHoleCandiCount].y = m_nHoleSearchingAreaH-dCy + (m_nHoleSearchingAreaH*2);//����������� ����Ʈ

			m_nHoleCandiCount++;
		}
	}
	delete fmtmp;
	delete fmtmp1;
	return nret;
}

void CgProject::fnDilate(LPBYTE fmSour, LPBYTE fmDest,CRect rt,int dir) // dir : 0�� ħ��. 1�� ��â
{
	int i,j,k,l;
	int nOrgX, nOrgY;
	int nKernelSizeY,nKernelSizeX;
	nKernelSizeY = nKernelSizeX = 3;
	int nHeight = rt.Height();
	int nWidth = rt.Width();
	int kernel[9]; // X(3) * Y(3)
	for(i=0;i<9;i++) kernel[i] = 1;

	nOrgX=(int)(nKernelSizeX/2.0-0.5);
	nOrgY=(int)(nKernelSizeY/2.0-0.5);


	for(i=0;i<nHeight-nKernelSizeY;i++) // y
		for(j=0;j<nWidth-nKernelSizeX;j++) // x
		{
			if(dir)
			{ // ������ �ϳ��� �ִٸ� �������� ���������ϱ�. �������忡���� ��â.
				for(k=0;k<nKernelSizeY;k++)
					for(l=0;l<nKernelSizeX;l++)
						//				   if( *(pKernel+nKernelSizeX*k+l) )    //				   if( *(kernel+nKernelSizeX*k+l) )                
						if( *(fmSour+(i+k)*nWidth+j+l) )
						{
							*(fmDest+(i+nOrgY)*nWidth+j+nOrgX)=255;
							goto LOOP;
						}
						*(fmDest+(i+nOrgY)*nWidth+j+nOrgX)=0;					   
			}
			else
			{
				// ������ �ϳ��� �ִٸ� �������� ����������. �������忡���� ħ��.
				for(k=0;k<nKernelSizeY;k++)
					for(l=0;l<nKernelSizeX;l++)
						//				   if( *(pKernel+nKernelSizeX*k+l) )     //				   if( *(kernel+nKernelSizeX*k+l) )                
						if( *(fmSour+(i+k)*nWidth+j+l) ==0)
						{
							*(fmDest+(i+nOrgY)*nWidth+j+nOrgX)=0;
							goto LOOP;
						}
						*(fmDest+(i+nOrgY)*nWidth+j+nOrgX)=255;					   
			}

LOOP:      continue;
		}

}


CPoint CgProject::GetCog2(BYTE * fm,CRect rt,int pitch,int height,int mode)
{//mode 0 = black          mode 1 = white
	CPoint pt;
	int i,j,ntmp=0;
	double dx,dy,dc=0;
	dx = dy = 0;
	for(i=rt.top;i<rt.bottom;i++) // y
	{
		for(j=rt.left;j<rt.right;j++) // x
		{
			// mode�� ���� ������������ Ȥ�� ������������
			if((mode==0 && *(fm+i*pitch+j)==0) ||(mode==1 && *(fm+i*pitch+j)==255))
			{
				// �� ������ X�� ���� �򸰰�(�����߽�?), Y�� ���� �򸰰�(�����߽�?)�� ����
				dx += j;
				dy += i;
				dc++;				
			}
		}
	}
	if(dc>0)
	{
		dx = dx/dc;	
		dy = dy/dc;
	}
	else
		dx=dy=0;

	pt.SetPoint(dx,height-dy);
	return pt;
}

int CgProject::FindRoi(BYTE * fmorigin,BYTE * fmBin,int pitch,int skipx1,int skipx2)
{
	int nret = 0;
//point ���� ������������ ���� ��ġ�� ã�´�.
	int ncen = 0;

	for(int i=0;i<4;i++)m_rtWeldRect[i].SetRect(0,0,0,0);
	// ������ fmmini�� ����
	BYTE * fmmini = new BYTE[pitch*m_nHoleSearchingAreaH];
	memcpy(fmmini,fmorigin,pitch*m_nHoleSearchingAreaH);

	// 200*128���� ���� �۰� ���� ����
	CRect rt,rt1;
	rt.SetRect(skipx1,10,pitch-skipx2,m_nHoleSearchingAreaH-10);

	int nLRpos,nLRpos1;
	int ncheckpoint = 0,ncheckcount=0;
	int nminx=200,nmaxx=0,nminNo=0,nMaxNo=0;
	int nrectmargin = pView->m_pDlgSet->m_FormTab4->m_nWeldingSize*0.5;
	
	if(m_nHoleCandiCount==0 && !pView->m_pDlgSet->m_FormTab2->m_bLineWelding)
	{
		//�߾ӿ� �׸���
		m_pChain->SetChainData(1,fmBin,2,2,20,8000,pitch,m_nHoleSearchingAreaH);
		int nBlobCount = m_pChain->FastChain(0, 0, pitch-1,m_nHoleSearchingAreaH-1);
		int count1=0;
		int nmaxarea=0,nmaxindex=0,minx,maxx,miny,maxy;
		double dTemp,dCx,dCy;
		for(int i=0;i<nBlobCount;i++)
		{
			dTemp = m_pChain->Chain_Area(i);
			if(dTemp >nmaxarea)
			{
				nmaxarea = dTemp;
				nmaxindex = i;
			}
		}
		if(nBlobCount==0)
		{
			int nx1 = 70,ny1=50;
			m_ptHole[m_nHoleCandiCount].SetPoint(nx1,m_nHoleSearchingAreaH-ny1);
			m_ptHole1[m_nHoleCandiCount].x = nx1 + m_rtHoleRect.left;
			m_ptHole1[m_nHoleCandiCount].y = m_nHoleSearchingAreaH-ny1 + m_rtHoleRect.top;
			m_ptHole2[m_nHoleCandiCount].x = nx1 + CAMERA_WIDTH-m_nHoleSearchingAreaW;
			m_ptHole2[m_nHoleCandiCount].y = m_nHoleSearchingAreaH-ny1 + (m_nHoleSearchingAreaH*2);
			m_nHoleCandiCount++;
			nx1 += pView->m_pDlgSet->m_FormTab4->m_nWeldingSize;
			m_ptHole[m_nHoleCandiCount].SetPoint(nx1,m_nHoleSearchingAreaH-ny1);
			m_ptHole1[m_nHoleCandiCount].x = nx1 + m_rtHoleRect.left;
			m_ptHole1[m_nHoleCandiCount].y = m_nHoleSearchingAreaH-ny1 + m_rtHoleRect.top;
			m_ptHole2[m_nHoleCandiCount].x = nx1 + CAMERA_WIDTH-m_nHoleSearchingAreaW;
			m_ptHole2[m_nHoleCandiCount].y = m_nHoleSearchingAreaH-ny1 + (m_nHoleSearchingAreaH*2);
			m_nHoleCandiCount++;

		}
		else
		{
			m_pChain->Chain_Center(nmaxindex, &dCx, &dCy);
			maxx = m_pChain->FindMaxX(nmaxindex);
			minx = m_pChain->FindMinX(nmaxindex);
			maxy = m_pChain->FindMaxY(nmaxindex);
			miny = m_pChain->FindMinY(nmaxindex); 
			if(maxx-minx > 70 )
			{//2ea
				int nx1 = (maxx+minx)/3;
				m_ptHole[m_nHoleCandiCount].SetPoint(nx1,m_nHoleSearchingAreaH-dCy);
				m_ptHole1[m_nHoleCandiCount].x = nx1 + m_rtHoleRect.left;
				m_ptHole1[m_nHoleCandiCount].y = m_nHoleSearchingAreaH-dCy + m_rtHoleRect.top;
				m_ptHole2[m_nHoleCandiCount].x = nx1 + CAMERA_WIDTH-m_nHoleSearchingAreaW;
				m_ptHole2[m_nHoleCandiCount].y = m_nHoleSearchingAreaH-dCy + (m_nHoleSearchingAreaH*2);
				m_nHoleCandiCount++;
				nx1 = (maxx+minx)/3;
				m_ptHole[m_nHoleCandiCount].SetPoint(nx1,m_nHoleSearchingAreaH-dCy);
				m_ptHole1[m_nHoleCandiCount].x = nx1 + m_rtHoleRect.left;
				m_ptHole1[m_nHoleCandiCount].y = m_nHoleSearchingAreaH-dCy + m_rtHoleRect.top;
				m_ptHole2[m_nHoleCandiCount].x = nx1 + CAMERA_WIDTH-m_nHoleSearchingAreaW;
				m_ptHole2[m_nHoleCandiCount].y = m_nHoleSearchingAreaH-dCy + (m_nHoleSearchingAreaH*2);
				m_nHoleCandiCount++;
			}
			else
			{//qea
				m_ptHole[m_nHoleCandiCount].SetPoint(dCx+nrectmargin*2,m_nHoleSearchingAreaH-dCy);
				m_ptHole1[m_nHoleCandiCount].x = dCx+nrectmargin*2 + m_rtHoleRect.left;
				m_ptHole1[m_nHoleCandiCount].y = m_nHoleSearchingAreaH-dCy + m_rtHoleRect.top;
				m_ptHole2[m_nHoleCandiCount].x = dCx+nrectmargin*2 + CAMERA_WIDTH-m_nHoleSearchingAreaW;
				m_ptHole2[m_nHoleCandiCount].y = m_nHoleSearchingAreaH-dCy + (m_nHoleSearchingAreaH*2);
				m_nHoleCandiCount++;
			}
		}
	}

	// ���ο����� ����!
	if(pView->m_pDlgSet->m_FormTab2->m_bLineWelding)//m_nHoleCandiCount==0)
	{
		// �ǹ̾���. �ؿ��� �ٲ�
		int nsumtop = (m_ptHole1[0].y+m_ptHole1[0].y)/2+10; // m_ptHole1[0].y + 10
		
		// �ǹ̾���. nminNo = 0. �ᱹ nsumtop�� �׻� m_ptHole1[0].y
		if(abs(m_ptHole1[nminNo].y-m_ptHole1[0].y)<=2)
			nsumtop = (m_ptHole1[nminNo].y+m_ptHole1[0].y)/2;

		int nsumtop1 = m_nHoleSearchingAreaH - (m_ptHole[nminNo].y+m_ptHole[0].y)/2+10;
		int ncen1 = m_ptCapCenter.x-m_rtHoleRect.left;//100

		// nminNo �� �׻� 0.
		if(nminNo<0)
		{
			m_rtWeldRect[0].SetRect(m_ptHole1[0].x-(nrectmargin*3),m_ptHole1[0].y-nrectmargin,m_ptHole1[0].x+nrectmargin,m_ptHole1[0].y+nrectmargin);
			m_rtWeldRect[1].SetRect(m_ptHole1[0].x-(nrectmargin*3),m_ptHole1[0].y-nrectmargin,m_ptHole1[0].x+nrectmargin,m_ptHole1[0].y+nrectmargin);
			m_rtWeldRect[2].SetRect(m_ptHole[0].x-nrectmargin,m_nHoleSearchingAreaH-m_ptHole[0].y-nrectmargin,m_ptHole[0].x+(nrectmargin*3),m_nHoleSearchingAreaH-m_ptHole[0].y+nrectmargin);
			m_rtWeldRect[3].SetRect(m_ptHole[0].x-nrectmargin,m_nHoleSearchingAreaH-m_ptHole[0].y-nrectmargin,m_ptHole[0].x+(nrectmargin*3),m_nHoleSearchingAreaH-m_ptHole[0].y+nrectmargin);
		}
		else // �׻� ����� ����. ���� ����(��� �ڽ�)
		{
			if(pView->m_pDlgSet->m_FormTab2->m_b2LineWelding == FALSE){
				// �� �ߺ�?
//				if(m_ptCapCenter.x>m_ptHole1[nminNo].x)
				{
					m_rtWeldRect[0].SetRect(m_ptHole1[nminNo].x-(nrectmargin),nsumtop-nrectmargin,m_ptHole1[nminNo].x+(nrectmargin*3),nsumtop+nrectmargin);
					m_rtWeldRect[1].SetRect(m_ptHole1[nminNo].x-(nrectmargin),nsumtop-nrectmargin,m_ptHole1[nminNo].x+(nrectmargin*3),nsumtop+nrectmargin);
					m_rtWeldRect[2].SetRect(m_ptHole[nminNo].x-(nrectmargin),nsumtop1-nrectmargin,m_ptHole[nminNo].x+(nrectmargin*3),nsumtop1+nrectmargin);
					m_rtWeldRect[3].SetRect(m_ptHole[nminNo].x-(nrectmargin),nsumtop1-nrectmargin,m_ptHole[nminNo].x+(nrectmargin*3),nsumtop1+nrectmargin);
				}
//				else
				{

				// nrectmargin = ���������� Param * 0.5
				// m_rtWeldRect[0], [1] : ���� 2�� ����. [2],[3] : ���� �̹����� ǥ���ҷ���? �ٵ� ����
				m_rtWeldRect[0].SetRect(m_ptHole1[nminNo].x-(nrectmargin*2),nsumtop-nrectmargin,m_ptHole1[nminNo].x+(nrectmargin*2),nsumtop+nrectmargin);
				m_rtWeldRect[1].SetRect(m_ptHole1[nminNo].x-(nrectmargin*2),nsumtop-nrectmargin,m_ptHole1[nminNo].x+(nrectmargin*2),nsumtop+nrectmargin);
				m_rtWeldRect[2].SetRect(m_ptHole[nminNo].x-(nrectmargin*2),nsumtop1-nrectmargin,m_ptHole[nminNo].x+(nrectmargin*2),nsumtop1+nrectmargin);
				m_rtWeldRect[3].SetRect(m_ptHole[nminNo].x-(nrectmargin*2),nsumtop1-nrectmargin,m_ptHole[nminNo].x+(nrectmargin*2),nsumtop1+nrectmargin);

				int nMargin = 0;
				// 3~4ȣ�� ����, 3mmTab : 1.5mm, 4mmTab : 2mm
				if(pView->m_pDlgSet->m_FormTab4->m_dTapSize == 4 && (pView->m_strComname.Find("5A")<0 && pView->m_strComname.Find("5B")<0 && pView->m_strComname.Find("5C")<0) )
					nMargin = 15;
				m_rtWeldRect[0].SetRect(m_ptHole1[0].x-(nrectmargin*4) - nMargin, m_ptHole1[0].y-(nrectmargin*2), m_ptHole1[0].x+(nrectmargin*4) + nMargin, m_ptHole1[0].y+(nrectmargin*2));
				}
			}
			else{ // hoho, 2�� ���� ����. [0] : ù°��, [1] : ��°��
				// WeldingSize ���� ���ϰ�
				int nMargin = 0;
				// 3~4ȣ�� ����, 3mmTab : 1.5mm, 4mmTab : 2mm
				if(pView->m_pDlgSet->m_FormTab4->m_dTapSize == 4 && (pView->m_strComname.Find("5A")<0 && pView->m_strComname.Find("5B")<0 && pView->m_strComname.Find("5C")<0) )
					nMargin = 15;

				m_rtWeldRect[0].SetRect(m_ptHole1[0].x-(nrectmargin*4) - nMargin, m_ptHole1[0].y-nrectmargin, m_ptHole1[0].x+(nrectmargin*4) + nMargin, m_ptHole1[0].y+nrectmargin);
				m_rtWeldRect[1].SetRect(m_ptHole1[1].x-(nrectmargin*4) - nMargin, m_ptHole1[1].y-nrectmargin, m_ptHole1[1].x+(nrectmargin*4) + nMargin, m_ptHole1[1].y+nrectmargin);
				m_rtWeldRect[2].SetRect(10,10,20,20);
				m_rtWeldRect[3].SetRect(20,10,30,20);
			}
		}
	}
	else
	{
		for(int i=0;i<m_nHoleCandiCount;i++)
			ncen += m_ptHole1[i].y;
		ncen /= m_nHoleCandiCount;
		if(pView->m_nMachine==1 && m_nHoleCandiCount==2 && abs(m_ptHole[0].x-m_ptHole[1].x)<10)
			m_nHoleCandiCount = 1;
		if(pView->m_nMachine==3 && m_nHoleCandiCount==2)
		{
			if(abs(m_ptHole[0].y-m_ptHole[1].y)>50 && abs(m_ptHole[0].x-m_ptHole[1].x<20))
			{//center near
				if(abs(64-m_ptHole[0].y)>abs(64-m_ptHole[1].y))
				{
					m_ptHole[0] = m_ptHole[1];
					m_ptHole1[0] = m_ptHole1[1];
					m_ptHole2[0] = m_ptHole1[1];
				}
				m_nHoleCandiCount = 1;	
			}
		}

		if(m_nHoleCandiCount==1)
		{//�߽ɿ��� ������ �Ÿ� ���ϱ�
			
			int ncen22 = m_ptCapCenter.x;

			if(pView->m_nMachine==2 && pView->m_strComname.Find("1")>=0)ncen22 +=10;
			else if(pView->m_nMachine==3)ncen22+=40;
//			if(100-m_ptHole[0].x>0)
//			if(ncen22-m_ptHole1[0].x>0)
//			if(pView->m_nMachine !=1)
			{
				int nav1=0,nav2=0,nb1=0,nb2=0;
//				if(pView->m_nMachine==3 && ncen22-m_ptHole1[0].x<=25)
				{//roi recheck
					int nleftgap = m_nTopEdge[0]-m_rtHoleRect.left;
					int nrightgap = m_rtHoleRect.right-m_nTopEdge[1];
					CRect rtch1,rtch2;
					rtch1.SetRect(m_ptHole[0].x-(nrectmargin*3),m_ptHole[0].y-nrectmargin,m_ptHole[0].x-nrectmargin,m_ptHole[0].y+nrectmargin);
					rtch2.SetRect(m_ptHole[0].x+(nrectmargin),m_ptHole[0].y-nrectmargin,m_ptHole[0].x+(nrectmargin*3),m_ptHole[0].y+nrectmargin);
					int gap = 0;
					if(pView->m_pDlgSet->m_FormTab4->m_dTapSize<3.5){
						gap = 5;
						rtch1 = CheckROI(rtch1,CRect(nleftgap,0,m_nHoleSearchingAreaW-nrightgap,m_nHoleSearchingAreaH));
						rtch2 = CheckROI(rtch2,CRect(nleftgap,0,m_nHoleSearchingAreaW-nrightgap,m_nHoleSearchingAreaH));
					}
					else
					{
						rtch1 = CheckROI(rtch1,CRect(gap,0,m_nHoleSearchingAreaW-gap,m_nHoleSearchingAreaH));
						rtch2 = CheckROI(rtch2,CRect(gap,0,m_nHoleSearchingAreaW-gap,m_nHoleSearchingAreaH));
					}
					
					nav1 = AreaAverage1(0,fmorigin,pitch,rtch1.left,rtch1.top,rtch1.right,rtch1.bottom,&nb1);
					nav2 = AreaAverage1(0,fmorigin,pitch,rtch2.left,rtch2.top,rtch2.right,rtch2.bottom,&nb2);
					
					if(pView->m_nMachine !=1){
						nav1 = AreaAverage1(0,fmorigin,pitch,rtch1.left,m_nHoleSearchingAreaH-rtch1.bottom,rtch1.right,m_nHoleSearchingAreaH-rtch1.top,&nb1);
						nav2 = AreaAverage1(0,fmorigin,pitch,rtch2.left,m_nHoleSearchingAreaH-rtch2.bottom,rtch2.right,m_nHoleSearchingAreaH-rtch2.top,&nb2);
					}
					if(nb1<nb2){
						if(m_ptHole1[0].x- m_nTopEdge[0]<105 )
						{
							nLRpos = 1;//left
							nminNo = 0;nminx=m_ptHole[0].x;
							nMaxNo = -1;nminx=m_ptHole[0].x;
						}
						else if(m_ptHole1[0].x- m_nTopEdge[0]<120 && nb1==0)
						{
							nLRpos = 1;//left
							nminNo = 0;nminx=m_ptHole[0].x;
							nMaxNo = -1;nminx=m_ptHole[0].x;
						}
						else if(pView->m_nMachine==3 && m_ptHole1[0].x- m_nTopEdge[0]>110 && m_ptCapCenter.x-m_ptHole1[0].x<8)
						{
							nLRpos = 1;//left
							nminNo = 0;nminx=m_ptHole[0].x;
							nMaxNo = -1;nminx=m_ptHole[0].x;
						}
						else
						{	
							pView->AddtoInspLog("Change ROI");
							nLRpos = 2;//right
							nminNo = -1;nminx=m_ptHole[0].x;
							nMaxNo = 0;nminx=m_ptHole[0].x;
						}
					}
					else if(nb1>nb2 && pView->m_nMachine==3)
					{
						nLRpos = 2;//right
						nminNo = -1;nminx=m_ptHole[0].x;
						nMaxNo = 0;nminx=m_ptHole[0].x;

					}
					else if(nb1==0 && nb2==0)
					{
						if(pView->m_nMachine==1)
						{
							if(ncen22-m_ptHole1[0].x>10)
							{
								nLRpos = 1;//left
								nminNo = 0;nminx=m_ptHole[0].x;
								nMaxNo = -1;nminx=m_ptHole[0].x;

							}
							else
							{

								//if(nav2 ==0)
								//{
								//	nLRpos = 1;//left
								//	nminNo = 0;nminx=m_ptHole[0].x;
								//	nMaxNo = -1;nminx=m_ptHole[0].x;

								//}
								//else
								{
									nLRpos = 2;//right
									nminNo = -1;nminx=m_ptHole[0].x;
									nMaxNo = 0;nminx=m_ptHole[0].x;
								}
							}
						}
						else if(nav1<nav2 && nav1>0 && nav2>0)
						{
							if(pView->m_nMachine==2 && (rtch1.Width()<40 ||nav1+4>nav2) )// &&  
							{
								nLRpos = 1;//right
								nminNo = 0;nminx=m_ptHole[0].x;
								nMaxNo = -1;nminx=m_ptHole[0].x;
							}
							else{
								nLRpos = 2;//right
								nminNo = -1;nminx=m_ptHole[0].x;
								nMaxNo = 0;nminx=m_ptHole[0].x;
							}

						}
						else if(nav1>nav2 && nav1>0 && nav2>0)
						{
							if(pView->m_nMachine==2 && (rtch2.Width()<40 ||nav1<nav2+4) )// &&  
							{
								nLRpos = 2;//right
								nminNo = -1;nminx=m_ptHole[0].x;
								nMaxNo = 0;nminx=m_ptHole[0].x;
							}
							else{
								nLRpos = 1;//left
								nminNo = 0;nminx=m_ptHole[0].x;
								nMaxNo = -1;nminx=m_ptHole[0].x;
							}
						}
						else
						{
							if(nav1==0 && nav2>0)
							{
								nLRpos = 1;//left
								nminNo = 0;nminx=m_ptHole[0].x;
								nMaxNo = -1;nminx=m_ptHole[0].x;
							}
							else if(nav2==0 && nav1>0)
							{	
								
								if(m_ptCapCenter.x -m_ptHole1[0].x >10 && pView->m_nMachine==3)
								{
									pView->AddtoInspLog("ReChange ROI");
									nLRpos = 1;//left
									nminNo = 0;nminx=m_ptHole[0].x;
									nMaxNo = -1;nminx=m_ptHole[0].x;
								}
								else
								{
									pView->AddtoInspLog("Change ROI");
									nLRpos = 2;//right
									nminNo = -1;nminx=m_ptHole[0].x;
									nMaxNo = 0;nminx=m_ptHole[0].x;
								}
							}
							else
							{
								if(rtch1.Width()<rtch2.Width())
								{
									nLRpos = 1;//left
									nminNo = 0;nminx=m_ptHole[0].x;
									nMaxNo = -1;nminx=m_ptHole[0].x;

								}
								else
								{
									pView->AddtoInspLog("Change ROI");
									nLRpos = 2;//right
									nminNo = -1;nminx=m_ptHole[0].x;
									nMaxNo = 0;nminx=m_ptHole[0].x;
								}
							}
						}
					}
					else
					{
						pView->AddtoInspLog("Change ROI");
						if(m_nTopEdge[0] - m_ptHole1[0].x < -45 &&  nb2<150)
						{
							nLRpos = 2;//right
							nminNo = -1;nminx=m_ptHole[0].x;
							nMaxNo = 0;nminx=m_ptHole[0].x;
						}
						else if(nb2==0)
						{
							nLRpos = 2;//right
							nminNo = -1;nminx=m_ptHole[0].x;
							nMaxNo = 0;nminx=m_ptHole[0].x;
						}
						else
						{
							nLRpos = 1;//left
							nminNo = 0;nminx=m_ptHole[0].x;
							nMaxNo = -1;nminx=m_ptHole[0].x;
						}
					}				
				}
				//else
				//{
				//	nLRpos = 1;//left
				//	nminNo = 0;nminx=m_ptHole[0].x;
				//	nMaxNo = -1;nminx=m_ptHole[0].x;
				//}

			}
			//else
			//{
			//	nLRpos = 2;//right
			//	nminNo = -1;nminx=m_ptHole[0].x;
			//	nMaxNo = 0;nminx=m_ptHole[0].x;
			//}

			ncheckcount=1;
		}
		else if(m_nHoleCandiCount==2)
		{
			if(pView->m_nMachine==1)
			{
				//������ �Ÿ� ���
				if(abs(m_ptHole[0].x-m_ptHole[1].x)<30 && m_ptHole[0].x>45 &&m_ptHole[1].x>45)
				{
					if(abs(m_ptHole[0].x-m_ptHole[1].x)<10 )
					{
						ncheckcount = 1;
						if(abs(m_ptHole1[0].y-m_ptCapCenter.y)>abs(m_ptHole1[1].y-m_ptCapCenter.y))
						{
							m_nHoleCandiCount=0;
							m_ptHole[m_nHoleCandiCount] = m_ptHole[1];//.SetPoint(m_ptHole[1],128-dCy);
							m_ptHole1[m_nHoleCandiCount] = m_ptHole1[1];//.x = dCx + m_rtHoleRect.left;
							m_ptHole2[m_nHoleCandiCount] = m_ptHole1[1];//.y = 128-dCy + m_rtHoleRect.top;
							m_nHoleCandiCount++;
						}
						else
						{
							m_nHoleCandiCount=0;
//							m_ptHole[m_nHoleCandiCount] = m_ptHole[1];//.SetPoint(m_ptHole[1],128-dCy);
//							m_ptHole1[m_nHoleCandiCount] = m_ptHole1[1];//.x = dCx + m_rtHoleRect.left;
//							m_ptHole2[m_nHoleCandiCount] = m_ptHole1[1];//.y = 128-dCy + m_rtHoleRect.top;
							m_nHoleCandiCount++;
						}
					}
					else
					{
						ncheckcount = 2;
						if(m_ptHole[0].x>m_ptHole[1].x){
							nminx=m_ptHole[1].x;nmaxx=m_ptHole[0].x;
							nminNo = 1;nMaxNo = 0;
						}
						else
						{
							nminx=m_ptHole[0].x;nmaxx=m_ptHole[1].x;
							nminNo = 0;nMaxNo = 1;
						}
					}
				}
				else //if(abs(m_ptHole[0].x-m_ptHole[1].x)>=45)
				{//���Ϳ��� �հŸ�
					if(m_ptHole[0].x>m_ptHole[1].x)
					{
						ncheckpoint = 0;
						nminx=m_ptHole[1].x;nmaxx=m_ptHole[0].x;
						nminNo = 1;nMaxNo = 0;
					}
					else{
						nminx=m_ptHole[0].x;nmaxx=m_ptHole[1].x;
						nminNo = 0;nMaxNo = 1;

						ncheckpoint = 1;
					}
					ncheckcount=2;

					if(m_ptHole[0].x > 100 && m_ptHole[1].x>100&& (abs(m_ptHole[0].y-m_ptHole[1].y)<20) &&(abs(m_ptHole[0].x -m_ptHole[1].x)<30 &&abs(m_ptHole[0].x -m_ptHole[1].x)>36))
					{
						int n1 = 0;
						if((m_ptHole[0].x>m_ptHole[1].x) )
						{
							m_ptHole[0].x =m_ptHole[1].x-nrectmargin*2 ;
							m_ptHole1[0].x = m_ptHole1[1].x-nrectmargin*2 ;
							m_ptHole2[0].x = m_ptHole2[1].x-nrectmargin*2 ;

						}
						else
						{
							m_ptHole[1] =m_ptHole[0];
							m_ptHole1[1] = m_ptHole1[0];
							m_ptHole2[1] = m_ptHole2[0];
							m_ptHole[0].x =m_ptHole[1].x-nrectmargin*2 ;
							m_ptHole1[0].x = m_ptHole1[1].x-nrectmargin*2 ;
							m_ptHole2[0].x = m_ptHole2[1].x-nrectmargin*2 ;


						}
					}

				}
			}
			else
			{	
				//������ �Ÿ� ���
				if(abs(m_ptHole[0].x-m_ptHole[1].x)<30)
				{
					ncheckcount = 2;
					if(abs(m_ptHole[0].x-m_ptHole[1].x)<10)
					{
						if( m_ptHole1[0].x-m_ptCapCenter.x >60)
						{
							nminx=m_ptHole[1].x;nmaxx=m_ptHole[0].x;
							nminNo = 0;nMaxNo = 1;
						}
						else
						{
							nminx=m_ptHole[1].x;nmaxx=m_ptHole[0].x;
							nminNo = 0;nMaxNo = 1;
						}
					}
					else
					{
						if(m_ptHole[0].x<m_ptHole[1].x){
							nminx=m_ptHole[1].x;nmaxx=m_ptHole[0].x;
							nminNo = 0;nMaxNo = 1;
						}
						else
						{
							nminx=m_ptHole[1].x;nmaxx=m_ptHole[0].x;
							nminNo = 1;nMaxNo = 0;
						}
					}					
				}
				else //if(abs(m_ptHole[0].x-m_ptHole[1].x)>=35)
				{//���Ϳ��� �հŸ�
//					if(abs(m_ptHole[0].y-m_ptHole[1].y)>)
					if(m_ptHole[0].x>m_ptHole[1].x)
					{
						ncheckpoint = 0;
						nminx=m_ptHole[1].x;nmaxx=m_ptHole[0].x;
						nminNo = 1;nMaxNo = 0;
					}
					else{

						if(pView->m_nMachine==3 && abs(m_ptHole[0].y-m_ptHole[1].y)>=35)
						{
							if(abs(m_ptHole1[0].y-600) <abs(m_ptHole1[1].y-600))
							{
								ncheckpoint = 0;
								nminx=m_ptHole[0].x;nmaxx=m_ptHole[0].x;
								nminNo = 0;nMaxNo = 0;
							}
							else
							{
								nminx=m_ptHole[0].x;nmaxx=m_ptHole[1].x;
								nminNo = 0;nMaxNo = 1;
								ncheckpoint = 1;
							}
						}
						else
						{
								nminx=m_ptHole[0].x;nmaxx=m_ptHole[1].x;
								nminNo = 0;nMaxNo = 1;
								ncheckpoint = 1;

						}
					}
					ncheckcount=2;
				}
			}
		}
		else
		{
			if(m_nHoleCandiCount==3 )
			{
					int ntmp1 = m_ptHole[0].y-m_ptHole[1].y;
					int ntmp2 = m_ptHole[0].y-m_ptHole[2].y;
					int ntmp3 = m_ptHole[1].y-m_ptHole[2].y;

						if(abs(ntmp1)<14)
					{
						if(m_ptHole[0].x<m_ptHole[1].x){
							nminx = m_ptHole[0].x,nminNo=0;
							nmaxx = m_ptHole[1].x,nMaxNo=1;
						}
						else{
							nminx = m_ptHole[1].x,nminNo=1;
							nmaxx = m_ptHole[0].x,nMaxNo=0;
						}
						m_ptHole[0].y = (m_ptHole[0].y+m_ptHole[1].y)*0.5;
						m_ptHole[1].y = (m_ptHole[0].y+m_ptHole[1].y)*0.5;
						m_ptHole1[0].y = (m_ptHole1[0].y+m_ptHole1[1].y)*0.5;
						m_ptHole1[1].y = (m_ptHole1[0].y+m_ptHole1[1].y)*0.5;

					}
					else if(abs(ntmp2)<14)
					{
						if(m_ptHole[0].x<m_ptHole[2].x){
							nminx = m_ptHole[0].x,nminNo=0;
							nmaxx = m_ptHole[2].x,nMaxNo=2;
						}
						else{
							nminx = m_ptHole[2].x,nminNo=2;
							nmaxx = m_ptHole[0].x,nMaxNo=0;
						}
					}
					else if(abs(ntmp3)<14)
					{
						if(m_ptHole[1].x<m_ptHole[2].x){
							nminx = m_ptHole[1].x,nminNo=1;
							nmaxx = m_ptHole[2].x,nMaxNo=2;
						}
						else{
							nminx = m_ptHole[2].x,nminNo=2;
							nmaxx = m_ptHole[1].x,nMaxNo=1;
						}
					}
					else
					{
						for(int i=0;i<m_nHoleCandiCount;i++)
						{
							if(nminx>m_ptHole[i].x)nminx = m_ptHole[i].x,nminNo=i;
							if(nmaxx<m_ptHole[i].x)nmaxx = m_ptHole[i].x,nMaxNo=i;
						}
					}

			}
			else if(m_nHoleCandiCount==4 )
			{
				int ntmp1 = m_ptHole[0].y-m_ptHole[1].y;
				int ntmp2 = m_ptHole[1].y-m_ptHole[2].y;
				int ntmp3 = m_ptHole[2].y-m_ptHole[3].y;
				int ntmp4 = m_ptHole[0].y-m_ptHole[3].y;
				int ntmpx1 = m_ptHole[0].x-m_ptHole[1].x;
				int ntmpx2 = m_ptHole[1].x-m_ptHole[2].x;
				int ntmpx3 = m_ptHole[2].x-m_ptHole[3].x;

				if(abs(ntmp1)<15 )
				{
					if(m_ptHole[0].x<m_ptHole[1].x){
						nminx = m_ptHole[0].x,nminNo=0;
						nmaxx = m_ptHole[1].x,nMaxNo=1;
					}
					else{
						nminx = m_ptHole[1].x,nminNo=1;
						nmaxx = m_ptHole[0].x,nMaxNo=0;
					}
				}
				else if(abs(ntmp2)<15)
				{
					if(m_ptHole[1].x<m_ptHole[2].x){
						nminx = m_ptHole[1].x,nminNo=1;
						nmaxx = m_ptHole[2].x,nMaxNo=2;
					}
					else{
						nminx = m_ptHole[2].x,nminNo=2;
						nmaxx = m_ptHole[1].x,nMaxNo=1;
					}
				}
				else if(abs(ntmp3)<15)
				{
					if(m_ptHole[2].x<m_ptHole[3].x){
						nminx = m_ptHole[2].x,nminNo=2;
						nmaxx = m_ptHole[3].x,nMaxNo=3;
					}
					else{
						nminx = m_ptHole[3].x,nminNo=3;
						nmaxx = m_ptHole[2].x,nMaxNo=2;
					}
				}
				else
				{
					for(int i=0;i<m_nHoleCandiCount;i++)
					{
						if(nminx>m_ptHole[i].x)nminx = m_ptHole[i].x,nminNo=i;
						if(nmaxx<m_ptHole[i].x)nmaxx = m_ptHole[i].x,nMaxNo=i;
					}
				}

			}
			else
			{
					for(int i=0;i<m_nHoleCandiCount;i++)
					{
						if(nminx>m_ptHole[i].x)nminx = m_ptHole[i].x,nminNo=i;
						if(nmaxx<m_ptHole[i].x)nmaxx = m_ptHole[i].x,nMaxNo=i;
					}
			}
			ncheckcount = 2;
		}
		if(ncheckcount==1)
		{//recheck // hole center
			//if(m_ptHole1[0].x)

		}

		//Rect �׸���
		if(ncheckcount==1)
		{
			if(nminNo==0){
				m_rtWeldRect[0].SetRect(m_ptHole1[0].x-nrectmargin,m_ptHole1[0].y-nrectmargin,m_ptHole1[0].x+nrectmargin,m_ptHole1[0].y+nrectmargin);
				m_rtWeldRect[1].SetRect(m_ptHole1[0].x+nrectmargin,m_ptHole1[0].y-nrectmargin,m_ptHole1[0].x+(nrectmargin*3),m_ptHole1[0].y+nrectmargin);
				m_rtWeldRect[2].SetRect(m_ptHole[0].x-nrectmargin,m_nHoleSearchingAreaH-m_ptHole[0].y-nrectmargin,m_ptHole[0].x+nrectmargin,m_nHoleSearchingAreaH-m_ptHole[0].y+nrectmargin);
				m_rtWeldRect[3].SetRect(m_ptHole[0].x+nrectmargin,m_nHoleSearchingAreaH-m_ptHole[0].y-nrectmargin,m_ptHole[0].x+(nrectmargin*3),m_nHoleSearchingAreaH-m_ptHole[0].y+nrectmargin);
			}
			else
			{
				m_rtWeldRect[0].SetRect(m_ptHole1[0].x-(nrectmargin*3),m_ptHole1[0].y-nrectmargin,m_ptHole1[0].x-nrectmargin,m_ptHole1[0].y+nrectmargin);
				m_rtWeldRect[1].SetRect(m_ptHole1[0].x-nrectmargin,m_ptHole1[0].y-nrectmargin,m_ptHole1[0].x+(nrectmargin),m_ptHole1[0].y+nrectmargin);
				m_rtWeldRect[2].SetRect(m_ptHole[0].x-nrectmargin,m_nHoleSearchingAreaH-m_ptHole[0].y-nrectmargin,m_ptHole[0].x+nrectmargin,m_nHoleSearchingAreaH-m_ptHole[0].y+nrectmargin);
				m_rtWeldRect[3].SetRect(m_ptHole[0].x+nrectmargin,m_nHoleSearchingAreaH-m_ptHole[0].y-nrectmargin,m_ptHole[0].x+(nrectmargin*3),m_nHoleSearchingAreaH-m_ptHole[0].y+nrectmargin);
			}
		}
		else
		{
			int nsumtop = (m_ptHole1[nminNo].y+m_ptHole1[nMaxNo].y)/2+10;
			if(abs(m_ptHole1[nminNo].y-m_ptHole1[nMaxNo].y)<=2)
				nsumtop = (m_ptHole1[nminNo].y+m_ptHole1[nMaxNo].y)/2;
			int nsumtop1 = m_nHoleSearchingAreaH - (m_ptHole[nminNo].y+m_ptHole[nMaxNo].y)/2+10;
			int ncen1 = m_ptCapCenter.x-m_rtHoleRect.left;//100
			//�߽ɿ� ������� �������� Ȯ��
			if(abs(ncen1-m_ptHole[nminNo].x) < abs(ncen1-m_ptHole[nMaxNo].x))
			{
				{
					if(pView->m_nMachine==3)
					{
						int nleftgap = m_nTopEdge[0]-m_rtHoleRect.left;
						int nrightgap = m_rtHoleRect.right-m_nTopEdge[1],nav1,nav2,nb1,nb2;
						CRect rtch1,rtch2;
						rtch1.SetRect(m_ptHole[nminNo].x-(nrectmargin*3),m_ptHole[nminNo].y-nrectmargin,m_ptHole[nminNo].x-nrectmargin,m_ptHole[nminNo].y+nrectmargin);
						rtch2.SetRect(m_ptHole[nminNo].x+(nrectmargin),m_ptHole[nminNo].y-nrectmargin,m_ptHole[nminNo].x+(nrectmargin*3),m_ptHole[nminNo].y+nrectmargin);
						int gap = 0;
						if(pView->m_pDlgSet->m_FormTab4->m_dTapSize<3.5){
							gap = 5;
							rtch1 = CheckROI(rtch1,CRect(nleftgap,0,m_nHoleSearchingAreaW-nrightgap,m_nHoleSearchingAreaH));
							rtch2 = CheckROI(rtch2,CRect(nleftgap,0,m_nHoleSearchingAreaW-nrightgap,m_nHoleSearchingAreaH));
						}
						else
						{
							rtch1 = CheckROI(rtch1,CRect(gap,0,m_nHoleSearchingAreaW-gap,m_nHoleSearchingAreaH));
							rtch2 = CheckROI(rtch2,CRect(gap,0,m_nHoleSearchingAreaW-gap,m_nHoleSearchingAreaH));
						}
						nav1 = AreaAverage1(0,fmorigin,pitch,rtch1.left,rtch1.top,rtch1.right,rtch1.bottom,&nb1);
						nav2 = AreaAverage1(0,fmorigin,pitch,rtch2.left,rtch2.top,rtch2.right,rtch2.bottom,&nb2);

						nav1 = AreaAverage1(0,fmorigin,pitch,rtch1.left,m_nHoleSearchingAreaH-rtch1.bottom,rtch1.right,m_nHoleSearchingAreaH-rtch1.top,&nb1);
						nav2 = AreaAverage1(0,fmorigin,pitch,rtch2.left,m_nHoleSearchingAreaH-rtch2.bottom,rtch2.right,m_nHoleSearchingAreaH-rtch2.top,&nb2);
						if(m_nHoleCandiCount==3 && m_rtHoleRect.left+rtch2.left>m_nTopEdge[1]-15 && nb1>90)
						{
							nb1 += 50;
							pView->AddtoInspLog("Rechange Roi");

						}
//						else
						{
							if(nb1>nb2){
								m_rtWeldRect[0].SetRect(m_ptHole1[nminNo].x-(nrectmargin*3),nsumtop-nrectmargin,m_ptHole1[nminNo].x-nrectmargin,nsumtop+nrectmargin);
								m_rtWeldRect[1].SetRect(m_ptHole1[nminNo].x-nrectmargin,nsumtop-nrectmargin,m_ptHole1[nminNo].x+(nrectmargin),nsumtop+nrectmargin);
								m_rtWeldRect[2].SetRect(m_ptHole[nminNo].x-(nrectmargin*3),nsumtop1-nrectmargin,m_ptHole[nminNo].x-nrectmargin,nsumtop1+nrectmargin);
								m_rtWeldRect[3].SetRect(m_ptHole[nminNo].x-nrectmargin,nsumtop1-nrectmargin,m_ptHole[nminNo].x+(nrectmargin),nsumtop1+nrectmargin);
							}
							else
							{
								m_rtWeldRect[0].SetRect(m_ptHole1[nminNo].x-(nrectmargin),nsumtop-nrectmargin,m_ptHole1[nminNo].x+nrectmargin,nsumtop+nrectmargin);
								m_rtWeldRect[1].SetRect(m_ptHole1[nminNo].x+nrectmargin,nsumtop-nrectmargin,m_ptHole1[nminNo].x+(nrectmargin*3),nsumtop+nrectmargin);
								m_rtWeldRect[2].SetRect(m_ptHole[nminNo].x-(nrectmargin),nsumtop1-nrectmargin,m_ptHole[nminNo].x+nrectmargin,nsumtop1+nrectmargin);
								m_rtWeldRect[3].SetRect(m_ptHole[nminNo].x+nrectmargin,nsumtop1-nrectmargin,m_ptHole[nminNo].x+(nrectmargin*3),nsumtop1+nrectmargin);

							}
						}
						//�¿� üũ
					}
					else
					{
						m_rtWeldRect[0].SetRect(m_ptHole1[nMaxNo].x-(nrectmargin*3),nsumtop-nrectmargin,m_ptHole1[nMaxNo].x-nrectmargin,nsumtop+nrectmargin);
						m_rtWeldRect[1].SetRect(m_ptHole1[nMaxNo].x-nrectmargin,nsumtop-nrectmargin,m_ptHole1[nMaxNo].x+(nrectmargin),nsumtop+nrectmargin);
						m_rtWeldRect[2].SetRect(m_ptHole[nMaxNo].x-(nrectmargin*3),nsumtop1-nrectmargin,m_ptHole[nMaxNo].x-nrectmargin,nsumtop1+nrectmargin);
						m_rtWeldRect[3].SetRect(m_ptHole[nMaxNo].x-nrectmargin,nsumtop1-nrectmargin,m_ptHole[nMaxNo].x+(nrectmargin),nsumtop1+nrectmargin);
					}
				}
			}
			else
			{
				if(pView->m_nMachine==3 && nminNo == nMaxNo)
				{
					m_rtWeldRect[0].SetRect(m_ptHole1[nMaxNo].x-(nrectmargin*3),nsumtop-nrectmargin,m_ptHole1[nMaxNo].x-nrectmargin,nsumtop+nrectmargin);
					m_rtWeldRect[1].SetRect(m_ptHole1[nMaxNo].x-nrectmargin,nsumtop-nrectmargin,m_ptHole1[nMaxNo].x+(nrectmargin),nsumtop+nrectmargin);
					m_rtWeldRect[2].SetRect(m_ptHole[nMaxNo].x-(nrectmargin*3),nsumtop1-nrectmargin,m_ptHole[nMaxNo].x-nrectmargin,nsumtop1+nrectmargin);
					m_rtWeldRect[3].SetRect(m_ptHole[nMaxNo].x-nrectmargin,nsumtop1-nrectmargin,m_ptHole[nMaxNo].x+(nrectmargin),nsumtop1+nrectmargin);
				}
				else
				{
					m_rtWeldRect[0].SetRect(m_ptHole1[nminNo].x-(nrectmargin),nsumtop-nrectmargin,m_ptHole1[nminNo].x+nrectmargin,nsumtop+nrectmargin);
					m_rtWeldRect[1].SetRect(m_ptHole1[nminNo].x+(nrectmargin),nsumtop-nrectmargin,m_ptHole1[nminNo].x+(nrectmargin*3),nsumtop+nrectmargin);
					m_rtWeldRect[2].SetRect(m_ptHole[nminNo].x-(nrectmargin),nsumtop1-nrectmargin,m_ptHole[nminNo].x+nrectmargin,nsumtop1+nrectmargin);
					m_rtWeldRect[3].SetRect(m_ptHole[nminNo].x+(nrectmargin),nsumtop1-nrectmargin,m_ptHole[nminNo].x+(nrectmargin*3),nsumtop1+nrectmargin);
				}
			}
			if(pView->m_nMachine==1)
			{
				if((m_nTopEdge[1]+m_nTopEdge[0])/2-5 <m_ptHole1[nminNo].x && abs(m_ptHole[0].x-m_ptHole[1].x)<38)
				{
					int nx1 = (m_ptHole1[nMaxNo].x + m_ptHole1[nminNo].x)/2;
					int nx2 =  (m_ptHole[nMaxNo].x + m_ptHole[nminNo].x)/2;
					m_rtWeldRect[0].SetRect(nx1-(nrectmargin*3),nsumtop-nrectmargin,nx1-nrectmargin,nsumtop+nrectmargin);
					m_rtWeldRect[1].SetRect(nx1-nrectmargin,nsumtop-nrectmargin,nx1+(nrectmargin),nsumtop+nrectmargin);
					m_rtWeldRect[2].SetRect(nx2-(nrectmargin*3),nsumtop1-nrectmargin,nx2-nrectmargin,nsumtop1+nrectmargin);
					m_rtWeldRect[3].SetRect(nx2-nrectmargin,nsumtop1-nrectmargin,nx2+(nrectmargin),nsumtop1+nrectmargin);

				}

			}
		}
		//line weld
		if(pView->m_pDlgSet->m_FormTab2->m_bLineWelding)
		{
			int nsumtop = (m_ptHole1[nminNo].y+m_ptHole1[nMaxNo].y)/2+10;
			if(abs(m_ptHole1[nminNo].y-m_ptHole1[nMaxNo].y)<=2)
				nsumtop = (m_ptHole1[nminNo].y+m_ptHole1[nMaxNo].y)/2;
			int nsumtop1 = m_nHoleSearchingAreaH- (m_ptHole[nminNo].y+m_ptHole[nMaxNo].y)/2+10;
			int ncen1 = m_ptCapCenter.x-m_rtHoleRect.left;//100
			if(nminNo<0)
			{
				m_rtWeldRect[0].SetRect(m_ptHole1[0].x-(nrectmargin*3),m_ptHole1[0].y-nrectmargin,m_ptHole1[0].x+nrectmargin,m_ptHole1[0].y+nrectmargin);
				m_rtWeldRect[1].SetRect(m_ptHole1[0].x-(nrectmargin*3),m_ptHole1[0].y-nrectmargin,m_ptHole1[0].x+nrectmargin,m_ptHole1[0].y+nrectmargin);
				m_rtWeldRect[2].SetRect(m_ptHole[0].x-nrectmargin,m_nHoleSearchingAreaH-m_ptHole[0].y-nrectmargin,m_ptHole[0].x+(nrectmargin*3),m_nHoleSearchingAreaH-m_ptHole[0].y+nrectmargin);
				m_rtWeldRect[3].SetRect(m_ptHole[0].x-nrectmargin,m_nHoleSearchingAreaH-m_ptHole[0].y-nrectmargin,m_ptHole[0].x+(nrectmargin*3),m_nHoleSearchingAreaH-m_ptHole[0].y+nrectmargin);
			}
			else
			{
				m_rtWeldRect[0].SetRect(m_ptHole1[nminNo].x-(nrectmargin),nsumtop-nrectmargin,m_ptHole1[nminNo].x+(nrectmargin*3),nsumtop+nrectmargin);
				m_rtWeldRect[1].SetRect(m_ptHole1[nminNo].x-(nrectmargin),nsumtop-nrectmargin,m_ptHole1[nminNo].x+(nrectmargin*3),nsumtop+nrectmargin);
				m_rtWeldRect[2].SetRect(m_ptHole[nminNo].x-(nrectmargin),nsumtop1-nrectmargin,m_ptHole[nminNo].x+(nrectmargin*3),nsumtop1+nrectmargin);
				m_rtWeldRect[3].SetRect(m_ptHole[nminNo].x-(nrectmargin),nsumtop1-nrectmargin,m_ptHole[nminNo].x+(nrectmargin*3),nsumtop1+nrectmargin);
			}
		}
	}

	for(int i=0;i<2;i++)
		m_rtWeldRect[i] = CheckROI(m_rtWeldRect[i],CRect(100,100,CAMERA_WIDTH-100,CAMERA_HEIGHT-100));
	for(int i=2;i<4;i++)
		m_rtWeldRect[i] = CheckROI(m_rtWeldRect[i],CRect(0,0,m_nHoleSearchingAreaW,m_nHoleSearchingAreaH));
	delete fmmini;
	return nret;
}

int CgProject::FindHoleInfo(BYTE* fmFull,BYTE * fmorigin,int pitch)
{
	int ret = 0;
	double davetmp=0,dc=0;
	int nc12 = 0;
	double davetmp1 = 0;
	int nhisto[256];

	for(int i=0;i<256;i++)
		nhisto[i] = 0;

	// fmHole �߾Ӻκ��� ave�� ���Ѵ�.
	for(int i=0;i<m_nHoleSearchingAreaH;i++){ // y
		for(int j=50;j<pitch-50;j++){ // x
			davetmp += *(fmorigin+i*pitch+j);
			nhisto[*(fmorigin+i*pitch+j)]++;
			nc12++;
		}
	}
	if(nc12<0)nc12 =1;
	davetmp /= nc12;

	// �Ⱦ�
	int nmax12 = 0;
	for(int i=250;i>0;i--)
	{
		if(nhisto[i]>400){
			nmax12 = i;
			break;
		}
	}
	
	int nmargin = 7,nmargin1=0;
	//if(pView->m_nMachine==1 && abs(m_nCapMaxBright - nmax12)>nmargin)
	//{
	//	nmargin1 = 3;
	//}
	//CString str12;
	//str12.Format("%d %d %d",m_nCapMaxBright-nmax12,m_nCapMaxBright,nmax12);
	//pView->AddtoInspLog(str12);
	
	// nj3,4 �ش����
	if(pView->m_nMachine==1 ||pView->m_nMachine==3)
	{//ave 1ȸ ��
		nc12 = 0;
		for(int i=0;i<m_nHoleSearchingAreaH;i++)
			for(int j=50;j<pitch-50;j++)
				if(*(fmorigin+i*pitch+j)>davetmp)
				{
					davetmp1 += *(fmorigin+i*pitch+j);
					nc12++;
				}
		if(nc12<1)nc12=1;
		davetmp1 /= nc12;

		davetmp = (davetmp+davetmp1)/2;
	}
	int nblack,nwhite,nb1,nw1;
	int npostmp1,npostmp2,npostmp3;
	int ngap = 3;
//	if(pView->m_nMachine==1 && m_nFindWhite>0)
//		nmargin1 = 3;
	if(pView->m_pDlgSet->m_FormTab4->m_b2Tab)
		nmargin1 = 6;
	else if(pView->m_nMachine==1)
		nmargin1 = 4;
	
	// n,j 3,4 ���� �ش����
	if(pView->m_nMachine==1){
		ngap = 12+nmargin1;
	}
	else if(pView->m_nMachine==2 && pView->m_strComname.Find("1")>=0 && davetmp>=118)
	{
		ngap = 6;
	}
	else if(pView->m_nMachine==3)
	{
		if( davetmp>=120) // 120
			ngap = 35;//6*3.5;
		else if(davetmp>=110) // 110
			ngap = 30;//6*3;
		else if(davetmp>100) // 100
			ngap = 25;//6*2.5;
		else
			ngap = 20;//6*2;
	}
	
	// ����ȭ�� ���ϴ� 200*100 ��İ� �����
	if(pView->m_pDlgSet->m_FormTab2->m_bTapImageShow)
	{
		for(int i=CAMERA_HEIGHT-100;i<CAMERA_HEIGHT;i++)
		{
			for(int j=0;j<m_nHoleSearchingAreaW;j++)
				*(fmFull+i*CAMERA_WIDTH+j) = 0;
		}
	}

	if(pView->m_pDlgSet->m_FormTab2->m_bLineWelding)
	{//���� + �β�
		if(pView->m_pDlgSet->m_FormTab2->m_b2LineWelding == FALSE){
			m_n1Hole = 0;
			m_nCogDist[0][0]=m_nCogDist[0][1]=m_nCogDist[1][0]=m_nCogDist[1][1]=0;
			// davetmp-ngap : fmHole �߾Ӻκ� ��� - 3
			m_dLMean = GetRoiMean1(fmFull,CAMERA_WIDTH,CAMERA_HEIGHT,m_rtWeldRect[0], 1,1,&nblack,&nwhite,&nb1,&nw1,davetmp-ngap,1,&npostmp1);//chcho//m_nUserTh
			m_nBlackCount[0] = nblack;
			m_nWhiteCount[0] = nwhite;
			m_nBCount[0] = nb1;
			m_nWCount[0] = nw1;
		}
		else{ // 2�� ���� ����
			m_n1Hole = 0;
			m_nCogDist[0][0]=m_nCogDist[0][1]=m_nCogDist[1][0]=m_nCogDist[1][1]=0;
			m_dLMean = GetRoiMean1(fmFull,CAMERA_WIDTH,CAMERA_HEIGHT,m_rtWeldRect[0], 1,1,&nblack,&nwhite,&nb1,&nw1,davetmp-ngap,1,&npostmp1);//chcho//m_nUserTh
			m_nBlackCount[0] = nblack;
			m_nWhiteCount[0] = nwhite;
			m_nBCount[0] = nb1;
			m_nWCount[0] = nw1;

			m_dRMean = GetRoiMean1(fmFull,CAMERA_WIDTH,CAMERA_HEIGHT,m_rtWeldRect[1], 1,1,&nblack,&nwhite,&nb1,&nw1,davetmp-ngap,0,&npostmp2);//chcho//m_nUserTh
			m_nBlackCount[1] = nblack;
			m_nWhiteCount[1] = nwhite;
			m_nBCount[1] = nb1;
			m_nWCount[1] = nw1;
		}
	}
	else
	{
		m_n1Hole = 0;
		m_nCogDist[0][0]=m_nCogDist[0][1]=m_nCogDist[1][0]=m_nCogDist[1][1]=0;
		m_dLMean = GetRoiMean1(fmFull,CAMERA_WIDTH,CAMERA_HEIGHT,m_rtWeldRect[0], 1,1,&nblack,&nwhite,&nb1,&nw1,davetmp-ngap,1,&npostmp1);//chcho//m_nUserTh
		m_nBlackCount[0] = nblack;
		m_nWhiteCount[0] = nwhite;
		m_nBCount[0] = nb1;
		m_nWCount[0] = nw1;

	//	m_dRMean = GetRoiMean1(fmorigin,pitch,128,rt2, 1,1,&nblack,&nwhite,&nb1,&nw1,davetmp-ngap,0,&npostmp2);//chcho//m_nUserTh
		m_dRMean = GetRoiMean1(fmFull,CAMERA_WIDTH,CAMERA_HEIGHT,m_rtWeldRect[1], 1,1,&nblack,&nwhite,&nb1,&nw1,davetmp-ngap,0,&npostmp2);//chcho//m_nUserTh
		m_nBlackCount[1] = nblack;
		m_nWhiteCount[1] = nwhite;
		m_nBCount[1] = nb1;
		m_nWCount[1] = nw1;
	}

	//CString str;
	//str.Format("B1:%d,W1:%d,B2:%d,W2:%d",m_nBlackCount[0],m_nWhiteCount[0],m_nBlackCount[1],m_nWhiteCount[1]);
	//pView->AddtoInspLog(str);
//	m_nPos[0]=npostmp1;//+rtc1.left;
//	m_nPos[1]=npostmp2;//+newrt.left;

	return ret;
}

double CgProject::GetRoiMean1(BYTE* fmorigin,int pitch,int height,CRect rt, int nSkipX, int nSkipY,int *black,int *white,int *nb,int *nw,int th,int no,int *pos)
{// fmFull �̹��� ���, rt�� Ȧ����(�ʷϹڽ�), th�� davetmp-ngap(fmHole �߾Ӻκ� ��� - 3), no�� 1: Left(or 1st line), 0: Right(or 2nd line)
	double dave=0.;
	int nSum,nCount,ntmp,i,j;
	nSum=nCount=ntmp = 0;
	*black = 0;
	*white = 0;
	*nb = 0;
	*nw = 0;
	*pos = 0;
	int nBlackCount=0,nWhiteCount=0;	
	int nBCount=0,nWCount=0;

	if(rt.Width()<=0 || rt.Height()<=0)
		return 0;
	
	int nw1 = rt.Width();
	int nh1 = rt.Height();
	BYTE * fmtmp = new BYTE[nw1*nh1];
	memset(fmtmp,0,nw1*nh1);

	for(j=0;j<6;j++)	m_nRangeCount[no][j] =m_nRangeFlow[no][j]= 0;

	int offsetx = 0;
	if(no==0) 
		offsetx = 100;
	int offsety = CAMERA_HEIGHT-100;

	if(pView->m_pDlgSet->m_FormTab2->m_bLineWelding == TRUE && pView->m_pDlgSet->m_FormTab2->m_b2LineWelding == TRUE){
		offsety = CAMERA_HEIGHT-100;

		if(no==0)
			offsety = CAMERA_HEIGHT-50;
	}

	// R ���ϱ�
	for(j=rt.top; j<rt.bottom; j+=nSkipY){
		offsetx = -1;
		
		if(no==0){ 
			offsetx = 100-1;

			if(pView->m_pDlgSet->m_FormTab2->m_bLineWelding == TRUE && pView->m_pDlgSet->m_FormTab2->m_b2LineWelding == TRUE)
				offsetx = -1;
		}
		
		for(i=rt.left; i<rt.right; i+=nSkipX){
			offsetx++;

			// th���� ��ο�� ���������
			if(*(fmorigin+j*pitch+i) < th)
				*(fmtmp+nw1*(j-rt.top)+(i-rt.left)) = 255;

			// ���� �ϴܿ� �׸���
			if(pView->m_pDlgSet->m_FormTab2->m_bTapImageShow)
			{
				if(*(fmorigin+j*pitch+i) < th){
					*(fmorigin+offsety*pitch+offsetx) = 255;
				}
			}
			if(pView->m_pDlgSet->m_FormTab4->m_bImageModify && pView->m_nMachine==3&& j>=m_nBlackStart && j<=m_nBlackEnd && m_nFindWhite==0)continue;
			if(pView->m_pDlgSet->m_FormTab4->m_bHoleEdgeSkip )//&& (m_nTopEdge[0]>i||m_nTopEdge[1]<i))// && m_nFindWhite==0)
			{
				if(no==0 && rt.right-m_nTopEdge[1]<50 && rt.right-m_nTopEdge[1] <= 20 && m_nTopEdge[1]<i)
					continue;
				else if(no==0 && rt.right-m_nTopEdge[1]<50  && rt.right-m_nTopEdge[1] > 20 && m_nTopEdge[1]+(rt.right-m_nTopEdge[1])*0.6<i)
					continue;
				else if(no==1 && m_nTopEdge[0]-rt.left <= 20 && m_nTopEdge[0]>i)continue;
				else if(no==1 && m_nTopEdge[0]-rt.left > 20 && m_nTopEdge[0]-10>i)continue;

			}

			// th���� ��ο�� ī��Ʈ. ���� �򰥸��� ����
			if(*(fmorigin+j*pitch+i) < th)
				nBlackCount++; // R
			else
				nWhiteCount++;	

			nSum += *(fmorigin+j*pitch+i);
			nCount++;
		}
		offsety++;
	}
	if(nCount != 0)	dave = (double)nSum / nCount;
	else			dave = 0.;

	// ������ �� ��迡 �ɷ�������
	if((no==1 && rt.left<m_nTopEdge[0]-5) || (no==0 && rt.right>m_nTopEdge[1]+5))
	{
		// Fast Chain ����. object white, ������ ������ ����� ���(fmtmp), skipx : 2, skipy : 2, blob �ѷ��� 10�̻� 50���ϸ� ���
		m_pChain->SetChainData(1,fmtmp,2,2,10,50,nw1,nh1);
		int nBlobCount = m_pChain->FastChain(0, 0, nw1,nh1);
		int	nCx= 0,nCy = 0,ncc=0,maxx,minx,maxy,miny;
		int nbdata[3];//0 ���� 1minx 2maxx
		double dTemp = 0,dCx,dCy,dAngle,avg,min,max,perimeter,moment;
		for(int i=0;i<nBlobCount;i++)
		{
			dTemp = m_pChain->Chain_Area(i);
			maxx = m_pChain->FindMaxX(i);
			minx = m_pChain->FindMinX(i);
			maxy = m_pChain->FindMaxY(i);
			miny = m_pChain->FindMinY(i); 
			miny = miny;

			m_pChain->Chain_Center(i, &dCx, &dCy);
			dAngle = m_pChain->FindAngle(i);
			m_pChain->FineDistFromPoint(i, dCx,dCy, &min, &max, &avg);

			if(min<1.4||max >12 || dTemp<10)
				continue;
			//pView->AddtoInspLog("Check!");
			m_n1Hole++;
		}
	}

	delete fmtmp;
	int nThreshold = 30;
	int nc = 0;
	int nroiw,nroih,nroidivW,nroidivH;
	nroiw = rt.right-rt.left; // ��������(�ʷϹڽ�) ��
	nroih = rt.bottom-rt.top; // ��������(�ʷϹڽ�) ����
	nroidivW = nroiw/6; // �� �� 6���
	nroidivH = nroih/6; // �� ���� 6���
	int nc1,nc2,nc3,nc4,nc5;
	nc1=nc2=nc3 =nc4=nc5= 0;
	int nflow = 0,nflow1 = 0;
	int possum = 0;
//	if(dave<th)	dave = (dave+th)/2;//*0.5;
	
	// �Ⱦ��ϱ� ������
	if(dave<90)
		dave = (dave+th)/2;//*0.5;

	int ntmp123[3];
	for(i=0;i<3;i++)ntmp123[i]=0;

	m_nRectSize = rt.Width()*rt.Height();

	int	ntmpcnt = 0,nLeft = 1000,nRight=0,ncontinue = 0;
	double dcx=0,dcy=0,dxc=0,dyc=0;

	// S ���ϱ�
	for(j=rt.top; j<rt.bottom; j+=nSkipY){
		for(i=rt.left; i<rt.right; i+=nSkipX){
			// ��������(�ʷϹڽ�) 6����Ѱſ��� ���� ����. -> 6����� �׸� �ȿ� �׸� �ȿ� �׸� �ȿ� �׸�..
			if(j>rt.top+(nroidivH*2) && j<rt.bottom-(nroidivH*2) && i>rt.left+(nroidivW*2) && i<rt.right-(nroidivW*2))
			{
				nThreshold = pView->m_pDlgSet->m_FormTab4->m_nV1;
				nc1++;
				nflow = 0;	
			} // �߰���
			else if(j>rt.top+(nroidivH) && j<rt.bottom-(nroidivH) && i>rt.left+(nroidivW) && i<rt.right-(nroidivW))
			{
				nThreshold = pView->m_pDlgSet->m_FormTab4->m_nV2;
				nc2++;
				nflow = 1;
			} // ���� �ٱ���
			else{	
				nThreshold = pView->m_pDlgSet->m_FormTab4->m_nV3;
				nc3++;
				nflow = 2;
			}

			if(pView->m_pDlgSet->m_FormTab4->m_bImageModify && pView->m_nMachine==3 &&j>=m_nBlackStart && j<=m_nBlackEnd)
					continue;
			if(pView->m_nMachine==3 && (m_nTopEdge[0]>i||m_nTopEdge[1]<i) )
			{
				if(no==0 && rt.right-m_nTopEdge[1]<50)
					continue;
			}

			ntmp = *(fmorigin+j*pitch+i);

			// fmHole �߾Ӻκ� �뷫 ��� - nThreshold
			if(ntmp < th-nThreshold)	//chcho
			{
//				if(no==1)					fmRectTh[nflow][(127-(j-rt.top+m_rtWeldRect[2].top))*200+(i-rt.left+m_rtWeldRect[2].left)] = 255;
//				else					fmRectTh[nflow][(127-(j-rt.top+m_rtWeldRect[3].top))*200+(i-rt.left+m_rtWeldRect[3].left)] = 255;
				
				// ��ġ ���� ���ְ�
				dcx += i;
				dcy+=j;
				// ī��Ʈ
				dxc++;
				
				// ������ ���� 2���̻� ���ӵǴ� ���� �����̶� ���� ������
				ncontinue++;
				if(ncontinue>3)
				{
					if(i<nLeft)nLeft=i;
					if(i>nRight)nRight = i;

					ncontinue = 0;
				}

				ntmpcnt++; // �Ⱦ�
				nBCount++; // S ī��Ʈ ����
				m_nRangeCount[no][nflow]++; // �Ⱦ�
				possum +=i;
			}
			else
			{
				ncontinue = 0;
				// �Ⱦ�
				if(ntmp > th+nThreshold)
					ntmpcnt++; 
				nWCount++; // �Ⱦ�
			}
		}
	}
	
	// ��ġ �����ش�
	if(dxc>0){
		dcx/=dxc;
		dcy/=dxc;
	}

	if(no==1){
		m_dBlackPer2[0] = ntmpcnt/(double)m_nRectSize*100;
		m_nCogDist[0][0] = nLeft;
		m_nCogDist[0][1]=nRight;
	}
	else if(no==0){
		m_dBlackPer2[1] = ntmpcnt/(double)m_nRectSize*100;
		m_nCogDist[1][0] = nLeft;
		m_nCogDist[1][1]=nRight;
	}
	
	// �Ⱦ�
	m_nRangeFlow[no][0] = nc1;m_nRangeFlow[no][1] = nc2;m_nRangeFlow[no][2] = nc3;
	
	// S ������ 
	if(nBCount == 0)
		*pos = 0;
	else // ������ X �߽�
		*pos = possum/nBCount;

	*black = nBlackCount; // R
	*white = nWhiteCount;
	*nb = nBCount; // S
	*nw = nWCount;
	
	return dave;
}

CRect	CgProject::CheckROI(CRect rt,CRect Range)
{
	if(rt.left<Range.left)
		rt.left = Range.left;
	if(rt.top<Range.top)
		rt.top = Range.top;
	if(rt.right>=Range.right)
		rt.right = Range.right-1;
	if(rt.bottom>=Range.bottom)
		rt.bottom = Range.bottom-1;
	
	if(rt.right<rt.left)
		rt.right = Range.right;
	if (rt.top > rt.bottom)
		rt.bottom = Range.bottom - 1;
	return rt;
}

CRect	CgProject::CheckROIoffset(CRect rt,CRect Range)
{
	//	if(rt.left<Range.left)rt.left = Range.left;
	//	if(rt.top<Range.top)rt.top = Range.top;
	//	if(rt.right>=Range.right)rt.right = Range.right-1;
	//	if(rt.bottom>=Range.bottom)rt.bottom = Range.bottom-1;

	if(rt.left<Range.left)rt.OffsetRect(Range.left-rt.left,0);//.left = Range.left;
	if(rt.top<Range.top)rt.OffsetRect(0,Range.top-rt.top);//.top = Range.top;
	if(rt.right>=Range.right)rt.OffsetRect(Range.right-rt.right,0);//.right = Range.right-1;
	if(rt.bottom>=Range.bottom)rt.OffsetRect(0,Range.bottom-rt.bottom);//.bottom = Range.bottom-1;

	return rt;
}


int CgProject::FindHoleWhite(BYTE *fmOrg,BYTE* fmTh,int pitch,int height)
{// fmOrg�� �������� 3*3����� ��, fmTh�� TH �̻��ΰ͵��� ������ �� ��
	int nret = 0;
	m_nHoleCandiCount = 0;

	BYTE * fmtmp2 = new BYTE[pitch*height];
	BYTE * fmtmp3 = new BYTE[pitch*height];
	memcpy(fmtmp2,fmOrg,pitch*height);
	memcpy(fmtmp3,fmOrg,pitch*height);

	double dave=0;
	if(pView->m_nMachine>=2)
	{ // fmtmp2, fmtmp3���� TH �̻��ΰ͵��� ������ �� ��
		memcpy(fmtmp2,fmTh,pitch*height);
		memcpy(fmtmp3,fmTh,pitch*height);
	}
	else{
		for(int i=0;i<height;i++)
		{
			for(int j=0;j<pitch;j++)
			{
				dave += *(fmOrg+i*pitch+j);
			}
		}
		dave /= (height*pitch);


		if(pView->m_pDlgSet->m_FormTab4->m_bTapWhiteSkip && pView->m_nMachine!=3)
		{
			for(int i=0;i<height;i++)
			{
				for(int j=0;j<pitch;j++)
				{
					if(*(fmOrg+i*pitch+j)>dave+15)
						*(fmOrg+i*pitch+j) = dave;

				}
			}
		}
		double dave1 = m_Pro.FindAutoThreshold(fmOrg,1,1,pitch-1,height-1,pitch);
		int nth = 0;
		if(pView->m_nMachine==3)
			nth = dave+10;// +5;//* (2-pView->m_pDlgSet->m_FormTab4->m_nHoleTh*0.01)+5;
		else
			nth = dave * (2-pView->m_pDlgSet->m_FormTab4->m_nHoleTh*0.01)+5;
		for(int i=0;i<height;i++)
		{
			for(int j=0;j<pitch;j++)
			{
				if(*(fmOrg+i*pitch+j)>nth)
					*(fmtmp2+i*pitch+j) = 255;
				else
					*(fmtmp2+i*pitch+j) = 0;
			}
		}
	}

	CRect rt;
	rt.SetRect(0,0,pitch,height); 
	fnDilate(fmtmp2, fmtmp3 , rt,0); // fmtmp3���� ħ��
	if(pView->m_nMachine==3){
		fnDilate(fmtmp3, fmtmp2 , rt,0);
		fnDilate(fmtmp2, fmtmp3 , rt,0);
	}
//	memcpy(fmtmp3,fmtmp2,height*pitch);
	//	rt.SetRect(0,0,pitch-1,height-1);

	// fmOrg�� ħ��(fmtmp3)�Ѱɷ� �����
	if(pView->m_pDlgSet->m_FormTab4->m_bWhiteCheck)
		memcpy(fmOrg,fmtmp3,pitch*height);

	//cog�� ����� �߽��� ���Ѵ�  �߽��� Ʋ���� ��� �̹��� �缳���� �Ѵ�.
	//blob	//
	double dTemp = 0,ratio=0,dCx,dCy,avg=0,dAngle,max,min;
	int maxx,minx,miny,maxy,nw,nh;
	int nAreaMin,nAreaMax;
	nAreaMin = 160;
	nAreaMax = 4000;
	if(pView->m_nMachine==2)
		nAreaMax = 2000;

	int	nBlobRecheck = 0;
	int ntop = 2;
	//white check
	int nwcx[20],nwcy[20],nwcheck = 0;
	for(int i=0;i<20;i++){
		nwcx[i]=nwcy[i] = m_nHoleCandiCount=0;
		m_ptHole[i].SetPoint(0,0);
		m_ptHole1[i].SetPoint(0,0);
		m_ptHole2[i].SetPoint(0,0);
	}
	if(pView->m_pDlgSet->m_FormTab4->m_bWhiteCheck)
	{
		// Fast Chain ����. object white, ħ���Ѱ� ���(fmtmp3), skipx : 2, skipy : 2, blob �ѷ��� 40�̻� 8000���ϸ� ���
		m_pChain->SetChainData(1,fmtmp3,2,2,40,8000,pitch,height);
		// Fast Chain. Input : ����, Output : Blob ����
		int nwcount = m_pChain->FastChain(18, ntop, pitch-18,height-1);
		int npluscount = 0;
		for(int i=0;i<nwcount;i++)
		{
			// �ش� Blob ���� ���ϱ�
			dTemp = m_pChain->Chain_Area(i);

			// ������ ������ ������?? ��¶�� ��� ������ ���� Blob ���� ���ϱ�
			if(dTemp>0)
				npluscount++;
		}
		for(int i=0;i<nwcount;i++)
		{
			dTemp = m_pChain->Chain_Area(i);
			m_pChain->Chain_Center(i, &dCx, &dCy); // Blob �߽ɱ��ϱ�
			dAngle = m_pChain->FindAngle(i); //  Blob �������ϱ�
			m_pChain->FineDistFromPoint(i, dCx,dCy, &min, &max, &avg); // Blob �߽ɿ��� ���� ª���Ÿ�, ��Ÿ�, ��հŸ�
			maxx = m_pChain->FindMaxX(i); // Blob X �ִ밪
			minx = m_pChain->FindMinX(i); // Blob X �ּҰ�
			maxy = m_pChain->FindMaxY(i); // Blob Y �ִ밪
			miny = m_pChain->FindMinY(i); // Blob Y �ּҰ�
			nw = maxx-minx; // Blob ��
			nh = maxy-miny; // Blob ����
			if(nh==0)continue;
			ratio = double(nw)/double(nh); // ���� �� �� ����. �¿�� ��������� 1���� Ŀ��. ���Ʒ��� ��������� 1���� �۾���.

			// �ش�Ǵ°� ������ ���� ����ϰ�, return ����
			if(pView->m_nMachine !=3 &&ratio<1.2 && ratio>0.78 && dTemp>190 && dTemp<2000 && min >=4.5 && max < 30 && maxy<m_nHoleSearchingAreaH-3 && miny>3)
			{
				nwcx[nret] = dCx;
				nwcy[nret] = dCy;
				nret++;
				m_ptHole[m_nHoleCandiCount].SetPoint(dCx,m_nHoleSearchingAreaH-dCy);
				m_ptHole1[m_nHoleCandiCount].x = dCx + m_rtHoleRect.left;
				m_ptHole1[m_nHoleCandiCount].y = m_nHoleSearchingAreaH-dCy + m_rtHoleRect.top;
				m_ptHole2[m_nHoleCandiCount].x = dCx + CAMERA_WIDTH-m_nHoleSearchingAreaW;
				m_ptHole2[m_nHoleCandiCount].y = m_nHoleSearchingAreaH-dCy + (m_nHoleSearchingAreaH*2);

				m_nHoleCandiCount++;
				m_nFindWhite++;
			}
			else if(pView->m_nMachine ==3 &&ratio<1.45 && ratio>0.7 && dTemp>190 && dTemp<2500 && min >=4.5 && max < 30 && maxy<m_nHoleSearchingAreaH-3 && miny>3)
			{
				nwcx[nret] = dCx;nwcy[nret] = dCy;nret++;
				m_ptHole[m_nHoleCandiCount].SetPoint(dCx,m_nHoleSearchingAreaH-dCy);
				m_ptHole1[m_nHoleCandiCount].x = dCx + m_rtHoleRect.left;
				m_ptHole1[m_nHoleCandiCount].y = m_nHoleSearchingAreaH-dCy + m_rtHoleRect.top;
				m_ptHole2[m_nHoleCandiCount].x = dCx + CAMERA_WIDTH-m_nHoleSearchingAreaW;
				m_ptHole2[m_nHoleCandiCount].y = m_nHoleSearchingAreaH-dCy + (m_nHoleSearchingAreaH*2);

				m_nHoleCandiCount++;
				m_nFindWhite++;

			}
			else if(pView->m_nMachine ==3 && npluscount==2&&ratio<2.2 && ratio>0.7 && dTemp>145 && dTemp<2500 && min >=1.6 && max < 30 && maxy<m_nHoleSearchingAreaH-3 && miny>3)
			{
				nwcx[nret] = dCx;nwcy[nret] = dCy;nret++;
				m_ptHole[m_nHoleCandiCount].SetPoint(dCx,m_nHoleSearchingAreaH-dCy);
				m_ptHole1[m_nHoleCandiCount].x = dCx + m_rtHoleRect.left;
				m_ptHole1[m_nHoleCandiCount].y = m_nHoleSearchingAreaH-dCy + m_rtHoleRect.top;
				m_ptHole2[m_nHoleCandiCount].x = dCx + CAMERA_WIDTH-m_nHoleSearchingAreaW;
				m_ptHole2[m_nHoleCandiCount].y = m_nHoleSearchingAreaH-dCy + (m_nHoleSearchingAreaH*2);

				m_nHoleCandiCount++;
				m_nFindWhite++;

			}

		}
	}
	// fmTh�� ħ��(fmtmp3)�Ѱɷ� �����
	memcpy(fmTh,fmtmp3,pitch*height);

	delete fmtmp2;
	delete fmtmp3;

	return nret;
}

int CgProject::ModifyBlackLine(BYTE *fmOrg,BYTE* fmTh,int pitch,int height,int *nstart,int* nend)
{
//������ �������� ����.
	BYTE * fmtmp1=new BYTE[pitch*height];
	BYTE * fmtmp2=new BYTE[pitch*height];
	memcpy(fmtmp1,fmOrg,pitch*height);
	memcpy(fmtmp2,fmOrg,pitch*height);

	//��չ�� ���ϱ�
	int navee=0,BlackCount = 0,nave1=0,nc=0,nb1=0,nmax=0;
	int nBlack[128],nBlack1[128];
	int nth = 55;
	for(int i=0;i<128;i++)nBlack[i]=nBlack1[i]=0;
	for(int i=0;i<height;i++)
	{
		nmax = 0;
		for(int j=0;j<pitch-1;j++)
		{
			navee += fmtmp1[i*pitch+j];
			if(fmtmp1[i*pitch+j]<nth)
			{
				nBlack[i]++;
				BlackCount++;
			}
			if(nb1>nmax)nmax = nb1;
			if(fmtmp1[i*pitch+j]<nth)
			{
				nb1++;
			}
			else nb1=0;
		}
		nBlack1[i] = nmax;
	}
	navee /= ((pitch-1)*height);

	//��չ�� ���� ���
	for(int i=0;i<height;i++)
	{
		for(int j=0;j<pitch;j++)
		{
			if(fmtmp1[i*pitch+j]>navee){
				nave1 += fmtmp1[i*pitch+j];
				nc++;
			}
		}
	}
	if(nc==0)nc = 1;
	nave1 /=nc;
	int nst=-1,ned=-1,peak = 55,peak1 = 40;;

	for(int i=0;i<128;i++)
	{
		if(nBlack[i]>peak && nst<0 && nBlack1[i]>peak1){
			nst = i;break;
		}
	}
	for(int i=127;i>0;i--)
	{
		if(nBlack[i]>peak && ned<0 && nBlack1[i]>peak1){
			ned = i;break;
		}
	}

	int ncen = (ned+nst)/2;
	//�߰� ���� �ִ��� üũ
	int newcnt1=0,newcnt2=0;
	if(ned-nst>20)
	{
		for(int i=ncen-(ned-nst)/2;i<ncen+(ned-nst)/2;i++)
		{

			if(nBlack1[i]>peak1)
				newcnt1++;
			else newcnt2++;
		}
	}
	if(newcnt2>(ned-nst)/2 ||newcnt2>=30)
	{//center hole
		ned = nst+15;
	}
	else if(ned >= 126 && (ned-nst)>40)
	{
		nst += 10;
	}
	CString str;
	str.Format("ST:%d  ED:%d",nst,ned);
	pView->AddtoInspLog(str);
	if(ned-nst>=3)
	{
		if(nst>5)		nst -= 4;if(ned<height-6)ned+=6;
		if(nst<45)nst = 0;
		for(int i=nst;i<ned;i++)
		{
			for(int j=0;j<pitch;j++)
				*(fmtmp2+i*pitch+j) = nave1;
		}
	}
	*nstart = 128-ned;//nst;
	*nend = 128-nst;//ned;
	memcpy(fmTh,fmtmp2,pitch*height);
	delete fmtmp1;
	delete fmtmp2;
	return 0;
}

int	CgProject::findReectEdge(BYTE* fm,int pitch,CRect rt,int nFlag) // nFlag : 1 - �������, 2 - �������
{
	int nret = 0;
	int nw1 = rt.Width();
	int *nw = new int[nw1];

	// hoho
	//if(nFlag == 1){ // ������踦 ã�� ��,
	//	for(int i=rt.left;i<rt.right;i++)
	//	{
	//		nw[i-rt.left] = 0;
	//		for(int j=rt.top;j<rt.bottom;j++){
	//			nw[i-rt.left]+= *(fm+j*pitch+i); // X�� ���������� �ƴ϶� �˴� ���Ѱ���. 
	//		}
	//	}
	//}
	//else{ // ������踦 ã�� ��,
	//	for(int i=rt.right;i>rt.left;i--)
	//	{
	//		nw[rt.right-i] = 0;
	//		for(int j=rt.top;j<rt.bottom;j++){
	//			nw[rt.right-i]+= *(fm+j*pitch+i); // X�� ���������� �ƴ϶� �˴� ���Ѱ���. 
	//		}
	//	}
	//}

	for(int i=rt.left;i<rt.right;i++)
	{
		nw[i-rt.left] = 0;
		for(int j=rt.top;j<rt.bottom;j++){
			nw[i-rt.left]+= *(fm+j*pitch+i); // X�� ���������� �ƴ϶� �˴� ���Ѱ���. 
		}
	}

	int nmax=0,npos=0,ncount=0;

	// ����� �ִ밪�̶� �� ��ġ
	for(int i=0;i<nw1-4;i++)
	{
		// Tab �ٱ����� ��������
		if(abs(nw[i]-nw[i+4])>nmax){
			nmax = abs(nw[i]-nw[i+4]);
			npos = i;
		}
	}

	// �� ��ġ�� ��ȯ
	nret = npos+rt.left;
	delete nw;
	return nret;
}

double CgProject::GetPrecisionTime(void)
{
	LARGE_INTEGER lpFrequency;
	LARGE_INTEGER lpPerformanceCount;
	QueryPerformanceFrequency(&lpFrequency);
	QueryPerformanceCounter(&lpPerformanceCount);
	return  (double)lpPerformanceCount.QuadPart /(double)lpFrequency.QuadPart;
}


int CgProject::FindCanPosition(BYTE * fm, CRect rt,int nTh,int * nLeft, int * nRight, int * nCenter)
{
	int nret = 1;
	CRect rtcan;
	rtcan =CheckROI(rt,CRect(10,10,CAMERA_WIDTH-10,CAMERA_HEIGHT-10));
	int ncanleft=0,ncanright=0,ncount1=0,ncount2=0,ncancenter=0;
	for(int y=rtcan.top;y<rtcan.bottom;y++){
		for(int x=rtcan.left;x<m_ptCapCenter.x;x++){
			if(*(fm+CAMERA_WIDTH*y+x) - *(fm+CAMERA_WIDTH*y+(x+4)) >nTh){
				ncanleft += x+4;
				ncount1++;
				break;
			}
		}
		for(int x=rtcan.right;x>m_ptCapCenter.x;x--){
			if(*(fm+CAMERA_WIDTH*y+x) - *(fm+CAMERA_WIDTH*y+(x-4)) >nTh){
				ncanright += x-4;
				ncount2++;
				break;
			}
		}
	}
	if(ncount1>0 && ncount2>0){
		ncanleft/= ncount1;ncanright/= ncount2;
		ncancenter = (ncanleft+ncanright)*0.5;
	}
	else{
		//ng
		*nLeft = 0;*nRight = 0;*nCenter = 0;

		return 0;
	}
	//top
	int nprocantop[CAMERA_WIDTH]={0,},nblackcnt=0;
	rtcan.SetRect(ncanleft+10,m_ptCapCenter.y,ncanleft+50,CAMERA_HEIGHT-10);
	for(int y=rtcan.bottom;y>rtcan.top;y--){
		nblackcnt=0;
		for(int x=rtcan.left;x<rtcan.right;x++){
			nprocantop[y]+=*(pView->m_Project.m_MultiBuf[0]+CAMERA_WIDTH*y+(x));
		}
		nprocantop[y] /= rtcan.Width();
		if(nprocantop[y]>200){
			nblackcnt = y+10;
			break;
		}
	}
	m_ptCanTop[0].SetPoint(ncanleft,nblackcnt);
	m_ptCanTop[1].SetPoint(ncanright,nblackcnt);

	*nLeft = ncanleft;*nRight = ncanright;*nCenter = ncancenter;
	return nret;
}

int CgProject::fnTabFind(BYTE * fmtab,CRect rttab,BOOL bShow)
{
	int nth = 150,nEdgeth=70,nmin1=255,nmax11=0;
	int nHisto[256]={0,};
	//�� ������ ���ϸ� �����Ѵ�.
	//Top Cap Center�� STD 
	CRect rttab1[3],rttab2;
	double dstd1=0,dstd2=0,dstd3=0,dmin1=0,dvar1=0,dstdave=0;
	CString str1;
	if(pView->m_pDlgSet->m_FormTab4->m_b2Tab){
		rttab1[1].SetRect(m_ptCapCenter.x-20,m_ptCapCenter.y-20,m_ptCapCenter.x+20,m_ptCapCenter.y+20);
		rttab1[2].SetRect(m_ptCapCenter.x-20,m_ptCapCenter.y-40,m_ptCapCenter.x+20,m_ptCapCenter.y-20);
		rttab1[0].SetRect(m_ptCapCenter.x-20,m_ptCapCenter.y+20,m_ptCapCenter.x+20,m_ptCapCenter.y+40);
	}
	else{
		rttab1[1].SetRect(m_ptCapCenter.x-20,m_ptCapCenter.y-20,m_ptCapCenter.x+20,m_ptCapCenter.y+20);
		rttab1[2].SetRect(m_ptCapCenter.x-20,m_ptCapCenter.y-40,m_ptCapCenter.x+20,m_ptCapCenter.y-20);
		rttab1[0].SetRect(m_ptCapCenter.x-20,m_ptCapCenter.y+20,m_ptCapCenter.x+20,m_ptCapCenter.y+40);
	}

	rttab1[0] =CheckROI(rttab1[0],CRect(10,10,CAMERA_WIDTH-10,CAMERA_HEIGHT-10));
	rttab1[1] =CheckROI(rttab1[1],CRect(10,10,CAMERA_WIDTH-10,CAMERA_HEIGHT-10));
	rttab1[2] =CheckROI(rttab1[2],CRect(10,10,CAMERA_WIDTH-10,CAMERA_HEIGHT-10));

	double dave1 = GetRoiInfo(fmtab,CAMERA_WIDTH,rttab1[0],1,1,&dstd1,&nmin1,&nmax11,&dvar1,0);
	str1.Format("Std %.2f ,Min %d,Max%d,Ave%.1f",dstd1,nmin1,nmax11,dave1);pView->AddtoInspLog(str1);
	double dave2 =GetRoiInfo(fmtab,CAMERA_WIDTH,rttab1[1],1,1,&dstd2,&nmin1,&nmax11,&dvar1,0);
	str1.Format("Std %.2f ,Min %d,Max%d,Ave%.1f",dstd2,nmin1,nmax11,dave2);pView->AddtoInspLog(str1);
	double dave3 = GetRoiInfo(fmtab,CAMERA_WIDTH,rttab1[2],1,1,&dstd3,&nmin1,&nmax11,&dvar1,0);
	str1.Format("Std %.2f ,Min %d,Max%d,Ave%.1f",dstd3,nmin1,nmax11,dave3);pView->AddtoInspLog(str1);
	//���� ������ ������ Y ��ġ�� ã�´�.
	//������ ������ ������ ������ ���� 
	dstdave = (dstd1+dstd2+dstd3)*0.33;
	double dstdth = dstdave*0.9;
	int nYoffset1=50,nYoffset2=150,nYoffset3=0;

	BYTE * fmtop = new BYTE[CAMERA_WIDTH*CAMERA_HEIGHT];
	memcpy(fmtop,m_AlphaBuf[0],CAMERA_HEIGHT*CAMERA_WIDTH);

	if(dstd1>dstdth && dstd2>dstdth && dstd3>dstdth){//�ٽ� ã�� ��� �߰��ؾ���.
		//��ĵ�� ������ �ؾ� �ϳ�?
		str1.Format("oth:stdAve %.2f ,th %.2f",dstdave,dstdth);pView->AddtoInspLog(str1);
	}
	else if(dstd2 > dstdth && dstd1<dstdth){//�߾Ӻ� ���� ����	//2�� ���Һ���
		nYoffset1=20,nYoffset2=80;
		nYoffset3 = 20;
		str1.Format("mid:stdAve %.2f ,th %.2f",dstdave,dstdth);pView->AddtoInspLog(str1);
	}
	else if(dstd2 < dstdth && dstd1 < dstdth&& dstd3 > dstdth){//���� ���� ���� //1�� ���Һ���
		nYoffset1=150,nYoffset2=50;
		str1.Format("top:stdAve %.2f ,th %.2f",dstdave,dstdth);pView->AddtoInspLog(str1);
	}
	else if(dstd1 > dstdth && dstd2 > dstdth&& dstd3 > dstdth){//��Ŭ���?�� ü �ٽ� ��ĵ?? NG�� Ȯ���� ����.
	}
	else{
		str1.Format("bot2:stdAve %.2f ,th %.2f",dstdave,dstdth);pView->AddtoInspLog(str1);
	}
	//top search�� ���� ������ �����Ѵ�.
	rttab2 = rttab;
	if(pView->m_pDlgSet->m_FormTab4->m_b2Tab){
		rttab2.top = m_ptCapCenter.y-220;
		rttab2.bottom = m_ptCapCenter.y-80;
		rttab.top = m_ptCapCenter.y-220;rttab.bottom = m_ptCapCenter.y-80;
	}
	else{
		rttab2.top = m_ptCapCenter.y-nYoffset1;
		rttab2.bottom = m_ptCapCenter.y+nYoffset2;
		rttab.top = m_ptCapCenter.y+nYoffset3;rttab.bottom = m_ptCapCenter.y+120;
	}


	CRect  rtRange;//���� ž ��ġ�� Botom�� ������Ų��.
	if(pView->m_pDlgSet->m_FormTab4->m_b2Tab){
		rtRange.SetRect(20,180,CAMERA_WIDTH-20,m_rtWeldPos.top);//
	}
	else{
		rtRange.SetRect(20,180,CAMERA_WIDTH-20,m_rtWeldPos.top);//
	}
	rttab2 = CheckROI(rttab2,rtRange);
	rttab =CheckROI(rttab,rtRange);


	pView->m_Display[0].AddArrayRect(rttab,0);
	int nProjx[CAMERA_WIDTH],nProjx1[CAMERA_WIDTH],nProjy[CAMERA_WIDTH],nProjy1[CAMERA_WIDTH],nProjy2[CAMERA_WIDTH],nProjy3[CAMERA_WIDTH],projSum[CAMERA_WIDTH],projSum1[CAMERA_WIDTH];
	for(int i=0;i<CAMERA_WIDTH;i++)nProjx[i]=nProjx1[i]=nProjy[i]=nProjy1[i]=nProjy2[i]=nProjy3[i]=projSum[i]=projSum1[i]=0;

	//���� Y pos�� ĵ ��ܰ� ĸ �߾��� �������� �����Ѵ�.

	double dave=0,dc1=0;
	// 	for(int y=0;y<CAMERA_HEIGHT;y++){
	// 		for(int x=0;x<CAMERA_WIDTH;x++){
	for(int y=rttab.top;y<rttab.bottom;y++){
		for(int x=rttab.left;x<rttab.right;x++){		
			nProjx1[x] += *(fmtab+y*CAMERA_WIDTH+x);
			nHisto[*(fmtab+y*CAMERA_WIDTH+x)]++;
			dave +=  *(fmtab+y*CAMERA_WIDTH+x);
		}
	}
	dave /= rttab.Width()*rttab.Height();
	if(pView->m_pDlgSet->m_FormTab4->m_b2Tab){
		nth = dave*1.2;
		nEdgeth = dave*0.8;
	}
	else{
		nth = dave*1.8;
		nEdgeth = dave;
	}

	CRect rttmp1;
	rttmp1 = rttab;
	if(rttab.top>rttab2.top)rttmp1.top = rttab2.top;
	if(rttab.bottom<rttab2.bottom)rttmp1.bottom = rttab2.bottom;
	if(rttab.left>rttab2.left)rttmp1.left = rttab2.left;
	if(rttab.right<rttab2.right)rttmp1.right = rttab2.right;
	for(int y=rttmp1.top;y<rttmp1.bottom;y++){
		for(int x=rttmp1.left;x<rttmp1.right;x++){


			// 			if(abs(*(fm+y*CAMERA_WIDTH+x)-*(fm+y*CAMERA_WIDTH+x+4)) >nEdgeth)
			// 				*(fm+y*CAMERA_WIDTH+x) = 255;
			// 			else
			// 				*(fm+y*CAMERA_WIDTH+x) = 0;

			if(*(fmtab+y*CAMERA_WIDTH+x) >nth)			*(fmtab+y*CAMERA_WIDTH+x) = 255;
			else			*(fmtab+y*CAMERA_WIDTH+x) = 0;
			if(*(fmtop+y*CAMERA_WIDTH+x) >nEdgeth)			*(fmtop+y*CAMERA_WIDTH+x) = 255;
			else			*(fmtop+y*CAMERA_WIDTH+x) = 0;


		}
	}

	// 	for(int y=rttab2.top;y<rttab2.bottom;y++){
	// 		for(int x=rttab2.left;x<rttab2.right;x++){
	// 			if(*(fmtab+y*CAMERA_WIDTH+x) >nth)			*(fmtab+y*CAMERA_WIDTH+x) = 255;
	// 			else			*(fmtab+y*CAMERA_WIDTH+x) = 0;
	// 		}
	// 	}


	//	d1[3] = pView->m_Project.GetPrecisionTime();

	int nmax = 0,npos=0,nmax1 = 0,npos1=0;
	for(int i=50;i<256;i++)
		if(nHisto[i]>nmax){nmax = nHisto[i];npos=i;}
		//��/�찡 ��������? ���� ��������?
		//master Edge ����
		int nMaster=0; //1-left  2-right	0-all
		//projection
		for(int y=rttab.top;y<rttab.bottom;y++){
			for(int x=rttab.left;x<rttab.right;x++){
				nProjx[x] += *(fmtab+y*CAMERA_WIDTH+x);
				if(*(fmtab+y*CAMERA_WIDTH+x)==255)
					nProjy[x]++;
			}
			//		nProjy[y] /= rttab.Width();
		}
		//	d1[4] = pView->m_Project.GetPrecisionTime();

		nmax = npos=0;
		for(int x=rttab.left;x<rttab.right;x++){
			nProjx[x] /= rttab.Height();
			nProjx1[x] /= rttab.Height();
			projSum[x] = (nProjx[x]+nProjx1[x]);//+nProjy[x]);
			if(x>rttab.left+1)
				projSum1[x] = abs(projSum[x]-projSum[x-2]);

			if(projSum1[x]>nmax){nmax = projSum1[x];npos = x;}
			//		Sleep(5);		TRACE("\n%d - %d - %d - %d - %d",x,nProjx1[x],nProjx[x],nProjy[x],projSum[x]);
		}
		//	d1[5] = pView->m_Project.GetPrecisionTime();

		//peak ���� 50 �ȼ��� �ǳʶٰ� 2��° max�� ã�´�.
		for(int x=rttab.left+4;x<rttab.right-4;x++){
			if(x> npos-20 && x<npos+20)continue;
			if(pView->m_nType==3){
				if(projSum1[x]>nmax1 && (abs(projSum[x]-projSum[x-4])>50 || abs(projSum[x]-projSum[x+4])>50)){
					nmax1 = projSum1[x];npos1 = x;}
			}
			else{
				if(projSum1[x]>nmax1){
					nmax1 = projSum1[x];npos1 = x;}
			}

		}
		CRect rttmp[4];//1���� 2������ ���� �߰� �˻� ����
		double dTabSize = pView->m_pDlgSet->m_FormTab4->m_dTapSize;
		double doffset1 = dTabSize/pView->m_pDlgSet->m_FormTab2->m_dCamScale;
		//max 1,2 �Ÿ��� ������ ������ �ǳʶڴ�.
		int nMaxtmp[4]={0,},npostmp[4]={0,};
		if(abs(npos1-npos)  >doffset1-20 && abs(npos1-npos)  <doffset1+20){

		}
		else
		{
			rttmp[0]=rttmp[1]=rttmp[2]=rttmp[3]=rttab;
			rttmp[0].left = npos-doffset1-20;rttmp[0].right = npos-doffset1+20;
			rttmp[1].left = npos+doffset1-20;rttmp[1].right = npos+doffset1+20;
			rttmp[2].left = npos1-doffset1-20;rttmp[2].right = npos1-doffset1+20;
			rttmp[3].left = npos1+doffset1-20;rttmp[3].right = npos1+doffset1+20;

			for(int k=0;k<4;k++){
				for(int x=rttmp[k].left;x<rttmp[k].right;x++){
					if(rttmp[k].left<rttab.left || rttmp[k].right>rttab.right)continue;

					if(projSum1[x]>nMaxtmp[k]){
						nMaxtmp[k] = projSum1[x];npostmp[k] = x;}

				}
				m_ptTabCandidate[k].SetPoint(npostmp[k],rttmp[k].top);
			}
			int nmax12 = 0,npeak = 0;
			for(int k=0;k<4;k++){
				if(nMaxtmp[k]>nmax12 ){
					nmax12 = nMaxtmp[k];
					//				npos1 = npostmp[k];
					npeak = k;
				}
			}
			if(npeak<2){
				npos1 =  npostmp[npeak];
			}
			else{
				npos = npostmp[npeak];
			}

//			pView->AddtoInspLog("Change Position");
		}

		//draw Line //m_Project.m_nBottomEdge[i]
		CPoint pt1,pt2;
		if(npos>npos1)pt1.x=npos1,pt2.x=npos;
		else	pt1.x=npos,pt2.x=npos1;
		CRect rttop;
		rttop = rttab2;
		rttop.left = pt1.x;rttop.right = pt2.x;
		pView->m_Display[0].AddArrayRect(rttop,0);

		int nWhiteCheck = 0,nBlackCheck=0,nLineWhite=0;
		for(int y=rttop.top;y<rttop.bottom;y++){
			for(int x=rttop.left;x<rttop.right;x++){
				nProjy1[y] += *(fmtop+y*CAMERA_WIDTH+x);
				if(pView->m_pDlgSet->m_FormTab4->m_b2Tab)
					nProjy3[y] += *(m_MultiBuf[10]+CAMERA_WIDTH*y+x);
			}
			if(rttop.Width()>0){
				nProjy1[y] /= rttop.Width();
				if(pView->m_pDlgSet->m_FormTab4->m_b2Tab){
					nProjy3[y]/= rttop.Width();
					if(nProjy3[y]>200)nLineWhite++;
				}
			}

			if(y>rttop.top){
				nProjy2[y] = (nProjy1[y]+nProjy1[y-1])*0.5;
			}
		}

		nmax11 = 0;npos1=0;
		int nFind = 0;
		for(int y=rttop.bottom-10;y>rttop.top+4;y--){
			//�ϴܿ��� �ö󰡸鼭 Ŀ���µ�
			if(abs(nProjy2[y]-nProjy2[y-4])>nmax11){
				nmax11 = abs(nProjy2[y]-nProjy2[y-4]);
				npos1 = y;
			}
			if(abs(nProjy2[y]-nProjy2[y-4])>45){
				pt1.y = y;
				pt2.y = y;
				m_nTopPos[0] = y;
				m_ptLeftTop=pt1;
				m_ptRightTop=pt2;
				nFind = 1;
				break;
			}
		}
		if(nFind==0){
			pt1.y = npos1;
			pt2.y = npos1;
			m_nTopPos[0] = npos1;
			m_ptLeftTop=pt1;
			m_ptRightTop=pt2;
		}
		if(npos1<10){
			delete fmtop;
			return 0;
		}
		if(pView->m_pDlgSet->m_FormTab4->m_b2Tab){
			//��� üũ �� ����. // 17�� ���������� üũ
			CRect rtCheck1;
			rtCheck1.SetRect(pt1.x,pt1.y-80,pt2.x,pt1.y);
			//200 �̻� ��� üũ
			for(int y=rtCheck1.top;y<rtCheck1.bottom;y++){
				for(int x=rtCheck1.left;x<rtCheck1.right;x++){
					if(*(m_MultiBuf[10]+CAMERA_WIDTH*y+x)>200 )
						nWhiteCheck++;
					if(*(m_MultiBuf[10]+CAMERA_WIDTH*y+x)<20 )
						nBlackCheck++;
				}
			}
			if(nWhiteCheck>3000 || nLineWhite>5)
			{
				rtCheck1.top = rtRange.top;
				for(int y=rtCheck1.top;y<rtCheck1.bottom;y++){
					nProjy2[y]=nProjy1[y] =0;
					for(int x=rtCheck1.left;x<rtCheck1.right;x++){
						nProjy1[y] += *(fmtop+y*CAMERA_WIDTH+x);
					}
					if(rttop.Width()>0)
						nProjy1[y] /= rtCheck1.Width();
					if(y>rtCheck1.top){
						nProjy2[y] = (nProjy1[y]+nProjy1[y-1])*0.5;
					}
				}
				for(int y=rtCheck1.top+2;y<rtCheck1.bottom-4;y++){
					if(abs(nProjy2[y] - nProjy2[y+4]) > 33){
						pt1.y = y+4;
						pt2.y = y+4;
						m_nTopPos[0] = y+4;
						m_ptLeftTop=pt1;
						m_ptRightTop=pt2;
						break;
					}
				}
			}
		}

		//	d1[7] = pView->m_Project.GetPrecisionTime();

		m_nTopEdge[0] = pt1.x;m_nTopEdge[1] = pt2.x;
		m_nEdgeDist[0] = m_ptCapCenter.x- m_nTopEdge[0]; // ������ Tab ���� ��谡 Top Cap �߽ɺ��� ������
		m_nEdgeDist[1] = m_nTopEdge[1]-m_ptCapCenter.x; // ������ Tab ���� ��谡 Top Cap �߽ɺ��� ����
		double ddist1 = m_nEdgeDist[0]*pView->m_pDlgSet->m_FormTab2->m_dCamScale;
		double ddist2 = m_nEdgeDist[1]*pView->m_pDlgSet->m_FormTab2->m_dCamScale;
		pView->m_dTabDist[0] = ddist1;pView->m_dTabDist[1]=ddist2;


		if(bShow)
			pView->DrawDisplay(0);
		//	d1[8] = pView->m_Project.GetPrecisionTime();

		int nnn = 0;
		delete fmtop;
		return 0;
}

CRect CgProject::fnBlobImage(BYTE * fm, int nw, int nh,double * dArea,double* dCx,double* dCy,double *dAngle)
{

	// Fast Chain ����. object white, ���������Ѱ� ���(fmtmp), skipx : 2, skipy : 2, blob �ѷ��� 20�̻� 20000���ϸ� ���
	m_pChain->SetChainData(1,fm,2,2,20,20000,nw,nh);
	// Fast Chain. Input : ����, Output : Blob ����
	int nBlobCount = m_pChain->FastChain(1, 1, nw-1,nh-1);
	int count1=0,maxx=0,minx=0,maxy=0,miny=0;
	int dmaxpos=-1;
	int	nCx= 0,nCy = 0,ncc=0,npos=0;
	int nbdata[3];//0 ���� 1minx 2maxx
	double dTemp = 0,dCx1=0,dCy1=0,avg=0,ratio=0,dmax = 0,dmin=0,min=0,max=0,dAngle1=0;
	CRect rt;

	int nDown1 = 0,nDown1Pos = 1000;
	// Ȧ ����
//	if(pView->m_pDlgSet->m_FormTab2->m_bLineWelding == FALSE && pView->m_pDlgSet->m_FormTab2->m_b2LineWelding == FALSE)
	{
		for(int i=0;i<nBlobCount;i++)
		{
			dTemp = m_pChain->Chain_Area(i);
			if(dTemp>dmax){
				dmax = dTemp;
				npos = i;
			}

		}
		m_pChain->Chain_Center(npos, &dCx1, &dCy1);
		dAngle1 = m_pChain->FindAngle(npos);
		m_pChain->FineDistFromPoint(npos, dCx1,dCy1, &min, &max, &avg);
		

		maxx = m_pChain->FindMaxX(npos);
		minx = m_pChain->FindMinX(npos);
		maxy = m_pChain->FindMaxY(npos);
		miny = m_pChain->FindMinY(npos); 
		int nw1 = maxx-minx;
		int nh1 = maxy-miny;
		ratio = double(nw1)/double(nh1);
		*dCx = dCx1;
		*dCy = dCy1;
		rt.SetRect(minx,miny,maxx,maxy);
	}

	return rt;
}


int CgProject::CheckRect(BYTE * fm, CRect rt,int ave)
{
	int nret = 0;
	double dave=0,dc=0;
	double dedge = 0;
	CRect rt1;
	rt1 = rt;
	rt1.InflateRect(-5,-5);
	pView->m_Display[0].AddArrayRect(rt1);
	int ntmp = 0;
	for(int i=rt1.top;i<rt1.bottom;i++){
		for(int j=rt1.left;j<rt1.right;j++){
			dave += *(fm+i*CAMERA_WIDTH+j);
			ntmp = abs(*(fm+i*CAMERA_WIDTH+j) - *(fm+i*CAMERA_WIDTH+(j+4)));
			if(ntmp<15)
				ntmp = 0;
			dedge += ntmp;
			dc++;
		}
	}
	if(dc>0)dave /= dc;
	else return -1;
	nret = (dedge/dc)*10;

// 	if(rt1.Width()<50)
// 		nret = -1;
	return nret;
}