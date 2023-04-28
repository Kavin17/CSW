#pragma once

#define MAX_DATA	80 
#define MAX_STDLINE	10
#define MAX_OCR		12	//7
#define MAX_CHAR	40	//37
#define NEW_CHAR	3	//ctw
#define MAX_PAT		6	//2

#define NEW_MAX_PAT		6	//10


//#define MAX_BUFFER	100
#include "Img.h"
#include "CgProcess.h"
#include "PatMat.h"

#include "CircleFitting.h "

#include "fchain.h"

#define	STDCOUNT	50


typedef unsigned char PIXEL;
typedef struct IMAGE
{
	PIXEL *addr; 
	int width,nx,ny;
};
typedef IMAGE* PIMAGE;

#define  MAX_COPY_BUF	2
class CgProject
{
public:
	CgProject(void);
	~CgProject(void);
///NEw CSW

	double	m_dTabTopLen;
	CRect CgProject::fnBlobImage(BYTE * fm, int nw, int nh,double * dArea,double* dCx,double* dCy,double* dAngle);

	BYTE * m_MultiBuf[20];
	BYTE * m_AlphaBuf[20];
	BYTE * m_WeldBuf[5];
	CPoint	m_ptWeldPos[10];
	int		m_nMultiFrame;
	CRect	m_rtTabPos;
	int m_nWeldMinMax[2];//0min 1max
	CRect CgProject::CircleFind2(BYTE * fmorigin);//탑캡 중심 찾고 검사영역 리턴
	CPoint	m_ptCanTop[2];
	int CgProject::FindCanPosition(BYTE * fm, CRect rt,int nTh,int * nLeft, int * nRight, int * nCenter);
	int CgProject::fnTabFind(BYTE * fmtab,CRect rttab,BOOL bShow);

	CPoint m_ptTopEdge[2];//left right

//////////////////////////////////////////
	CCircleFitting circleFit;
	double CgProject::GetPrecisionTime(void);

	double	m_dCabRadius;
	CPoint m_ptLeftTop;
	CPoint m_ptRightTop;
	CPoint m_ptRegWeldPos[2];//left top  , right bottom
	int		m_nRegWeldPos[2];//x , y
	int		m_nRegWeldPos1[2];//x , y
	CPoint m_ptTabCandidate[4];
	int		m_nWhiteValue;
	int		m_nBlackValue;

	CRect m_rtRegRect;
	int	m_nRegYPos;
	int	m_nRegYPos1;

	int	m_nLineHeight[200];
	int	m_nLinMinMax[4];//[0] min pos [1] max pos [2]min hight [3]max hight
	//default----------------------------
	int CgProject::AreaAverage2(int nIndex,BYTE *fm,int pitch,int nl,int nt,int nr,int nb,int th,int* cnt,double* dstd);

	static UINT threadInspMark(LPVOID pParam);
	int CgProject::FindHoleWhite(BYTE *fmOrg,BYTE* fmTh,int pitch,int height);
	int	ModifyBlackLine(BYTE *fmOrg,BYTE* fmTh,int pitch,int height,int *nstart,int* nend);
	double	m_dInspectTime[10]; // 각 프로세스 완료 시각 / 0 Trigger, 1 Grab, 2 Buf Copy, 3 DrawDisplay, 4 Inspect Start, 5 Tab, 6 Hole-IO-Label-Csv, 7 DrawDisplay-NG Label, 8 Grid
	int	 m_nBlackStart;
	int	m_nBlackEnd;
	CRect	CgProject::CheckROIoffset(CRect rt,CRect Range);

	int	m_ncapblackcount;
	int m_nFindWhite;
	int	m_nCapMaxBright;
	int	m_n1Hole;
	CPoint m_ptTab[2][CAMERA_HEIGHT];
	int	CgProject::findReectEdge(BYTE* fm,int pitch,CRect rt,int nFlag);

	////////////////////////////////////csw hole
	double		m_dMaxChainData[7];//면적, cx,cy,w,h,ratio
	BYTE * m_fmTemp;
	int		m_nCogDist[2][2];//left[left/right]right[left/right]
	BYTE * fmRectTh[3];
	CRect	m_rtCanCheckROI;
	int m_nTopPos[3];//0 전체 1중심에서 왼쪽 2중심에서 오른쪽
	int	m_nEdgeDist[2];
	BYTE*	m_fmHole;
	int m_nHoleSearchingAreaW;
	int m_nHoleSearchingAreaH;
	int	m_nTabProjPos[CAMERA_HEIGHT];
	int	m_nBottomEdge[2];
	int	m_nTopEdge[2];
	CRect m_rtTabOkRange;
	CRect m_rtWeldingOkRange;
	CPoint	m_ptCapCenter;//cap center
	CPoint	m_ptCapCenter1;//cap center
	int		m_nTopCapleft;
	int		m_nTopCapRight;
	int		m_nTopCapleft1;
	int		m_nTopCapRight1;
	double	m_dCapWidth;
	double	m_dCapWidth1;
	CPoint	m_ptDrawCapEdge[2][CAMERA_WIDTH];//좌측 우측
	double	m_dSharpness;
	int	m_nMin;//std min
	int	m_nMax;//std max
	double	m_dVar;//variance
	int		m_nStdLine1;
	int		m_nStdLine2;
	CString m_strStdValue[STDCOUNT];
	CString m_strVarValue[STDCOUNT];
	int		m_nStdCount;
	double	m_dVarValue[STDCOUNT];
	int		m_nStdLine[STDCOUNT];//화면 View용.
	int		m_nStdLineInsp[STDCOUNT];//검사용 Position Y
	double	m_dLMean;
	double	m_dRMean;
	double	m_dStd1Ave;
	double	m_dStd2Ave;
	int m_nStartCircle;
	int m_nEndCircle;
	int	m_nTapCenter;
	int	m_nCadidateY;//가강 강한점
	CPoint m_ptHole[20];
	CPoint m_ptHole1[20];
	CPoint m_ptHole2[20];
	int		m_nHoleCandiCount;
	CRect	m_rtHoleRect;
	CRect	m_rtWeldRect[4];
	CRect	m_rtWeldPos;
	CRect	m_rtMidWeldPos;
	int		m_nTapOkNg;//0ng 1ok
	int		m_nHoleOkNg;//0ng 1ok
	double	m_dBlackPer[2];
	double	m_dBlackPer1[2];
	double	m_dBlackPer2[2];//미용접용
	int		m_nRectSize;
	int		m_nRangeCount[3][6];
	int		m_nRangeFlow[3][6];
	double	m_dRangeValue[2][3];
	int		m_nBlackCount[2];
	int		m_nWhiteCount[2];
	int		m_nBCount[2];
	int		m_nWCount[2];
	CString m_strTimeHMS;


	CRect	CheckROI(CRect rt,CRect Range);
	double CgProject::GetRoiMean1(BYTE* fmorigin,int pitch,int height,CRect rt, int nSkipX, int nSkipY,int *black,int *white,int *nb,int *nw,int th,int no,int *pos);
	int CgProject::FindHoleInfo(BYTE* fmFull,BYTE * fmorigin,int pitch);
	int CgProject::FindRoi(BYTE * fmorigin,BYTE * fmBin,int pitch,int skipx1,int skipx2);
	CPoint CgProject::GetCog2(BYTE * fm,CRect rt,int pitch,int height,int mode);
	void CgProject::fnDilate(LPBYTE fmSour, LPBYTE fmDest,CRect rt,int dir) ;
	int CgProject::FindHolePosition(BYTE *fmOrg,BYTE* fmTh,int pitch,int height,int ave);
	int CgProject::FindHolePosition1(BYTE *fmOrg,BYTE* fmTh,int pitch,int height);
	int CgProject::fnCirPosFind(double dmin);
	int CgProject::FindWeldingPosition();
//	double CgProject::GetRoiInfo(CRect rt, int nSkipX, int nSkipY,double *std,int *min,int *max,double *var,int nconversion);
	double CgProject::GetRoiInfo(BYTE* fm,int pitch,CRect rt, int nSkipX, int nSkipY,double *std,int *min,int *max,double *var,int nconversion);
	int CgProject::FindTabPosition(CRect rt);
	int CgProject::CircleFind();
	int CgProject::CircleFind1();
	int CgProject::findCircleWidth(BYTE * fm,CRect rt,CRect rt1,int ncx,int ncy,int* nw1,int* nw2,int* nh1,int* nh2);
	double CgProject::GetHistoAve(BYTE * fm,int *nMin,int *nMax,double *dAve,int noffx,int noffy,int* nprox,int* nproy);
	CPoint CgProject::GetCog(BYTE * fm,CRect rt,int xwidth,int ywidth);
	int	m_nDirection;




	////////////////////////////////////////////

	int CgProject::AreaAverage1(int nIndex,BYTE *fm,int pitch,int nl,int nt,int nr,int nb,int* cnt);
	int	m_nWhiteDetact;//0 not 1 ng
	int	m_njigPos;
	int	m_nsheetCount[400];
	int	m_nInspComplete;
	CPoint m_ptMaster[4];
	CPoint m_ptDefect[MAX_DEFECT][4];
	CPoint m_ptDefect2[MAX_DEFECT][4];
	double	m_dSheetHeight;
	int CgProject::AreaAverage(int nIndex,BYTE *fm,int pitch,int nl,int nt,int nr,int nb);
	int	m_nBadFlag[MAX_DEFECT];//0sp sheet , 1,top , 2white  3,black  4,bottom
	int CgProject::CheckPSsheet(CRect rt);
	int m_nSheetPoint[MAX_DEFECT];
	int	m_nEdgePoint[100];
	int CgProject::FindInsArea(int nIndex,CRect rt,BYTE *fm);
	void CgProject::Ins_Smoothing(int Sm_count);
	void CgProject::FindLineEdge_ABS(CRect rtInsArea, int k, int Dist, int TrackerNum);//rtInsArea:검사할영역 TargetDiff:명암차기준 Dist:픽셀거리
	int CgProject::Sheet_RevIns(int k);
	int		m_nMarkPos[MAX_CAMERAS];//마크 검출 평션(1 = 문자위치 검사시 2=문자외 위치검사시)
	CRect	FindEdge1(int nCam,BYTE* fm,int width,int height,int* nl,int* nr,int* nt,int* nb,int npos);
	int		GetCog1(BYTE* fm,CRect rt,int pitch,int th,int rowcol);
	int		fnCheckPoint(BYTE* fm,int nwidth,int x, int y, int xoff,int yoff);
	int CgProject::CheckRect(BYTE * fm, CRect rt,int ave);

	int		m_nIndex;
	CChain *m_pChain;
	CChain *m_pChain1;
	int		m_nBlobCnt[MAX_CAMERAS];
	int		m_nBlobCnt1[MAX_CAMERAS];
	int		m_nCam2SkipCount;
	CRect	m_rtCam2Skip[5];

	CRect	m_rtMaster;
	int		m_nEndPos;
	double	m_dAnglepol;
	double	fnFindAngle(int nCam,BYTE * fm,CRect rt,int pitch);
	double	m_dInspTime;
	BOOL	m_bUseChain;
	int		FindSkipArea(int nCam,BYTE * fm, CRect * rtinsp, CRect * rtSkip);
	double	m_dBrightness1;
	CString m_strRect[MAX_CAMERAS];
	int		m_nSkipCount;
	int		m_nBlobCnttest[MAX_CAMERAS];
	CPoint	RotateRoi(CPoint spt,float angle);
	CRect	rtSkipMax[10];//상판 2 하판3
	CRect	rtSkipMax1[10];//상판 2 하판3
	int		m_nFrameCount;
	int		m_nNgCount;
	double	m_dInsTime;
	double m_dTestEdge[2];
	CgProcess m_Pro;
	void	Rotate(BYTE *dest, BYTE *src,float x0,float y0,int imagewidth,int imageheight,float radian);
	int		m_nTop;
	int		m_nBottom;
	int		m_nLeft;
	int		m_nRight;
	int		m_nInspScale;
	//	BYTE * m_fmInsp[MAX_CAMERAS];
	//	BYTE* m_fmNew[MAX_CAMERAS][MAX_COPY_BUF];
	BOOL	m_bStart;
	//	BOOL	m_bInspComplete[MAX_CAMERAS];
	int		m_nOcvPosition;//0-init 1lt 2rt	3lb	4rb
	int		CheckPos(CRect rtFull, CRect rtTarget);
	double	m_dBrightness;
	int		m_nCharCount;
	CPoint	m_ptMark[MAX_CAMERAS][MAX_MARK_COUNT];
	CRect	m_rtMark[MAX_CAMERAS][MAX_MARK_COUNT];
	int		m_nFindMarkCount[MAX_CAMERAS];
	double	m_dInspAve;
	CRect	m_rtBigChar;
	CRect	m_rtSmallChar;
	CRect	FindEdge(int nCam,BYTE* fm,int width,int height,int* nl,int* nr,int* nt,int* nb,int npos);
	//CPoint	m_ptBlob[500];
	int		m_nBlobLenth;
	double	m_dThreshould1;
	int		m_nCharPos[4];//0 - Big start 1-BigEnd  2-small start 3-small
	int		m_nSmallPos[7];
	int		m_nSmallCharCnt;
	int		GetCog(BYTE* fm,int pitch,int height, int ncount);
	CRect	m_rtSChar[MAX_SAMLL_CHAR];
	void	PatLoadSmall();
	CRect	m_rtMarkInsp;
	CRect	m_rtMarkSkip;
	int		m_nMarkResult;
	int		m_nOverLap;
	int		m_nBigCharCount;
	int		m_nModifyHeight;

	////////bad infomation
	int		m_nMinV[MAX_MARK_COUNT];
	int		m_nMaxV[MAX_MARK_COUNT];
	double	m_dSizeX[MAX_MARK_COUNT];		
	double	m_dSizeY[MAX_MARK_COUNT];		
	CPoint	m_ptBadPos[MAX_MARK_COUNT];
	double	m_dBadAve[MAX_MARK_COUNT];
	double	m_dRatio[MAX_MARK_COUNT];
	int		m_nMinV1[MAX_MARK_COUNT];
	int		m_nMaxV1[MAX_MARK_COUNT];
	double	m_dSizeX1[MAX_MARK_COUNT];		
	double	m_dSizeY1[MAX_MARK_COUNT];		
	CPoint	m_ptBadPos1[MAX_MARK_COUNT];
	double	m_dBadAve1[MAX_MARK_COUNT];
	double	m_dRatio1[MAX_MARK_COUNT];

	void InitProject(LPVOID pView);
	void Destroy();
	int TestInspect();
	void ResetFrame();
	void ReadyData();
	//-------------------------------------

	int Segment(int nCam,int minbound,int maxbound,int blobarea,int blobmin);
	int Segment2(int nCam);
	int SplitChar1(int nCam);
	int SplitChar(int nCam);
	void EdgeCorrection(int nCam,unsigned char* fmTmp,int nLeft,int nTop);
	char GetIndex2Char(int idx);
	void PatLoad();
	void InitBuf();
	BOOL	m_bStartInsp;

	unsigned char * m_fm[MAX_CAMERAS];
	CImg* m_pImgInsp;
	//	CImg*	m_pBadImage[MAX_DEFECT];
	//	CImg*	m_pTmpImage[MAX_DEFECT];

	int m_nCharCnt;

	//	NEPAT			m_pNepatReg;
	//	CPatMat			m_PatReg;

	BOOL m_bSLoadMode;
	CPoint m_ptTemp[5];
	double m_dAvg;
	double m_dAngle;
	int m_nReduceScale;
	double m_dAutoTh;
	CRect m_rtInsArea;
	double	m_dSheetHei;
	CRect m_rtAlignArea;
	CRect m_rtFindArea;
	//double m_dVPointX[MAX_DATA];
	//double m_dVPointY[MAX_DATA];
	int	m_nVPointCnt;
	CString m_strInfo[MAX_DATA];
	//double m_dHPointX[MAX_DATA];
	//double m_dHPointY[MAX_DATA];
	int	m_nHPointCnt;
	double m_dSttPointX;
	double m_dSttPointY;
	int m_nStdPointX[MAX_STDLINE];
	int m_nStdPointY[MAX_STDLINE];

};