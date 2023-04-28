#pragma once

#include <Afxmt.h>
#include "Img.h"
#define MAX_RECT	300
#define MAX_POINT	CAM_WIDTH*3	//1280	//CAMERA_HEIGHT*3
#define DEFAULT_COLOR	RGB(255,0,0)

class CgDisplay
{
public:
	CgDisplay(void);
public:
	~CgDisplay(void);

	void CgDisplay::SaveImgNG(CString strInfo,BOOL bPath=FALSE);
	void CgDisplay::FullSave(CString strInfo, CString userName);
	void CgDisplay::SaveImgNG1(CString strInfo,BOOL bPath,BOOL bNas, BOOL bImgReverse);
	
	void FullSave1(CString strInfo, CString userName,int nType);
	CString	m_strImagePath[5];//옥찬이 요청사항
	CString	m_strLot;
	int m_nCamera_Width;
	int	m_nCamera_Height;
	CString m_strTimePath;
	CRect m_rtMaster;
	CString m_strMesLot;
	BOOL	m_bHoleSearchArea;
	BOOL	m_bTrackshow;
	int		m_nMachine;	
	BOOL	m_bRegMode;
	int		m_nUseTracker;

	unsigned char * m_fm;
	CRect m_rtDummyFilm;
	BOOL  m_bDummyDraw;
	BOOL m_bDisplayRun;
	//이미지관련
	CImg*	m_pImg;
	CImg*	m_pImgbuf;	//이미지 저장로드 할때만 씀
	void SaveServerImg(CString strVisionType, CString strEqpID, int nLane, int nCam, int nFrameNum, BOOL bNG, CString strServerPath, SYSTEMTIME sysTime, CString strUploadPath, CString strCellID, CString strLotID, BOOL bPath=FALSE);
	void SaveImg(CString strInfo,BOOL bPath=FALSE);
	void SaveDLImg(BOOL bRet, int nGrabCount, BOOL bPath = FALSE);
	CString SaveCropImg(CString strVisionType, SYSTEMTIME sysTime, CString strEqpID, CString strLotID, CString strCellID, BOOL bNG, int nLane, int nNgCode, int nPosX, int nPosY, BOOL bPath=FALSE);

	void SaveTestImg(CString strInfo, CString userName);
	CString SaveImg2(CString strLot,CString strInfo);
	void SaveImgEvent(CString strInfo);
	void LoadImg(CString strPath);
	void LoadImg(CString strPath,int nMode);
	int* m_pProfileX;
	BOOL m_brtDetect;
	void SaveJpgImg(CString strInfo,CString strdate,int ncnt);
	void CgDisplay::SaveJpgImgBadImage(CString strInfo);
	CString m_strAIName;

	CEvent	m_eSave;
	CString m_strSaveInfo;
	BOOL	m_NotSaveImage;
	static UINT threadSave(LPVOID pParam);

	//확대,축소 픽셀정보
	int		m_nZoom;
	CRect	m_rtZoom;
	CRect   m_rtins[100]; // 주옥찬
	DOUBLE  m_vp[CAMERA_HEIGHT];
	DOUBLE  m_ep[CAMERA_HEIGHT];
	DOUBLE  m_tp[CAMERA_HEIGHT];

	CPoint	m_ptMouseCur;
	CPoint	m_ptMouseStt;
	void	SetMousePt(CPoint ptMouseStt);
	void	SetZoom(short zDelta, CPoint pt);
	CPoint	ScaleTrans(CPoint point,int direction);
	CPoint	SetMove(CPoint ptCur);
	CPoint	GetPos(CPoint ptCur,int* nValue);
	CPoint	CheckPt(CPoint ptCheck);
	void	CheckScreen();


	//화면초기화
	CRect	m_rtDisplay;
	CRect	m_rtRoiImg;
	CRect	m_rtFullImg;
	void InitDisplay(LPVOID pParam,BOOL bMono,CRect rtDisplay,CRect rtFullImg,CRect rtRoiImg);

	//도형그리기
	int		m_nRtCnt[MAX_CAMERAS];
	int		m_nBadCnt[MAX_CAMERAS];
	int		m_nBadVal[MAX_RECT];
	int		m_nBadFlag[MAX_RECT];
	double	m_dBadSize[MAX_RECT];
	double	m_dBadHeight[MAX_RECT];
	CRect	m_rt[MAX_CAMERAS][MAX_RECT];
	CRect	m_rtBad[MAX_CAMERAS][MAX_RECT];
	CString m_strRtInfo[MAX_CAMERAS][MAX_RECT];
	void DrawRect(CDC *pDC, CRect rt, COLORREF lColor, int nLineWidth,CString strInfo,int nFlag = 1);
	void DrawArrayRect(CDC *pDC);
	void AddArrayRect(CRect rt,int nIdx=0,CString str="");
	void DrawBadArrayRect(CDC *pDC);

	int m_nPtCnt[2];
	CPoint*	m_ptDefault[2];
	void DrawPoint(CDC *pDC, CPoint p1, COLORREF lColor, int nLineWidth);
	void DrawArrayPoint(CDC *pDC);
	void AddArrayPoint(CPoint pt,int nIdx=0);
	void DrawArrayPoint(CDC *pDC,int nNo,double* dx,double* dy, COLORREF lColor, int nLineWidth);

	int m_nXmarkCnt[2];
	void DrawXmark(CDC *pDC, CRect rt, COLORREF lColor, int nLineWidth,CString strInfo);
	void DrawArrayXmark(CDC *pDC);
	void AddArrayXmark(CPoint pt,int nIdx=0);
	CWnd* m_hParent;

	void ResetArray();
	void DrawBMP(LPCTSTR lpszFileName, CDC *pDC, int nX, int nY, DWORD dwRop);
	void DrawCircle(CDC *pDC, CPoint ptC,int nR, COLORREF lColor, int nLineWidth=1,CString strInfo="");
	void DrawLine(CDC *pDC, CPoint p1, CPoint p2,CString strInfo, COLORREF lColor, int nLineWidth);
	void DrawArrayLine(CDC *pDC,int nNo,double* dx,double* dy,CString* strInfo,COLORREF lColor, int nLineWidth ,int nHei);
	void DrawCross(CDC *pDC, CRect rt, COLORREF lColor, int nLineWidth,CString strInfo);
	void DrawDotMark(CDC *pDC, CPoint p1, COLORREF lColor, int nLineWidth);
	void DrawImage(CDC *pDC,int bShowPixel = TRUE);

	//-------------------------
	void DrawRoiRect(CDC *pDC, CPoint p1, CPoint p2, COLORREF lColor, int nLineWidth);
	void DrawRoiRect(CDC *pDC, CRect rect, COLORREF lColor, int nLineWidth, CRect ViewRect);
	void DrawText(CDC *pDC, CPoint pt, CString str,int bkmode=1,COLORREF lColor=COLOR_GREEN,COLORREF lbkColor=COLOR_BLACK);
	CPoint Scale(CPoint point, CRect ViewRect);
	CRect Scale(CRect rect, CRect ViewRect);
	CPoint		m_ptPatVeiw;

	//트래커
	BOOL			m_bTrackerUse;
	CRectTracker	m_Tracker[MAX_TRACKER];
	CRect			newRect;
	CRect			OldRect;
	CRect			OldTrueRect;
	CRect			m_rtImgTracker[MAX_TRACKER]; // 0 : BottomTab Tracker, 1 : Cap Check Tracker, 2: Tab Top Check Tracker, 3 : Top Cap Left Tracker, 4 : Top Cap Right Tracker, 5 : Welding Tracker
	CString			m_strTrackerName[MAX_TRACKER];
	void			TrackerCtrl(CWnd* pWnd,CPoint point);
	BOOL			TrackerSetCursor(CWnd* pWnd, UINT nHitTest);
	BOOL			m_bAutoSetOn;

	//ROI
	CRect	m_rtFilmTopIns[2];
	CRect	m_rtFilmBotIns[2];

	double	m_dFilmHeight;
	double	m_dFilmWidth;
	double	m_dFilmWidth2;
	double	m_dMetalHeight;
	double	m_dFilmPitch;
	double	m_dFilmH1;
	double	m_dFilmH2;
	BOOL	m_bJoint;

	double	m_dAveFilmTopVal;
	double	m_dAveFilmMidVal;
	double	m_dAveFilmBotVal;
	double	m_dAveMetalVal;
	CString m_strImgName;
};