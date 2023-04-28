#if !defined(AFX_PATMAT_H__010C6023_65E3_11D5_8210_00032D00198F__INCLUDED_)
#define AFX_PATMAT_H__010C6023_65E3_11D5_8210_00032D00198F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PatMat.h : header file
//

#ifndef	BYTE
typedef	unsigned char	BYTE;
#endif
#ifndef	PBYTE
typedef BYTE*	PBYTE;
#endif

//#define	PI	3.14159265359

#define	MAX_LEVEL		10
	// maximum possible pyramid level when building pattern
#define	MIN_PATSIDE		8
	// minumum pattern image side length
#define	ORG_SCORE		0.5
	// compression-related parameter when building pattern.
	// compression(leveling) stops when compressed pattern's
	// matching score with original goes below this value
	// valid range is (0.0~1.0)
	// increase this to perform exact match
	// decrease this to speed up

typedef struct tagNEIMG*	NEIMG;

struct tagNEIMG {
	PBYTE	bits;
	int	memw,width,height;
	float	s,ss;
	float	dAngle;	// degree
};

typedef struct tagNEPAT*	NEPAT;

struct tagNEPAT {
	float	hotOffX,hotOffY;
	int	bUseSrc;
	int	nLevel;
	NEIMG	image[MAX_LEVEL];
};

typedef struct tagSUBINFO*	SUBINFO;

struct tagSUBINFO {
	int	level;
	int	x,y;
	float	sl,st,sr,sb,sm;
	float	degree,da;	// for rotation
	float	su,sd;	// for rotation
};

/////////////////////////////////////////////////////////////////////////////
// CPatMat window

class CPatMat //: public CWnd
{

// Construction
public:
	CPatMat();

// Attributes
public:

// Operations
void	nematInit(PBYTE (*alloc_8)(int),void (*free_8)(PBYTE));
	// initializes nemat library
	// args
	//	alloc_8 :	pointer to function, which allocates
	//			8bit buffer. if 0, malloc() is used.
	//	free_8 :	pointer to function, which frees buffer
	//			allocated by alloc_8 function.
	//			if 0, free() is used.

void	nematSetErrorDisplay(void (*showError)(const char *));
	// assigns error reporting function
	// args
	//	showError :	pointer to function, which shows error
	//			message delivered as (const char*).

NEPAT	nematBuildPattern(PBYTE src,int memw,
		int left,int top,int right,int bottom,
		float hotOffX,float hotOffY,int bUseSrc);
	// creates and initializes template pattern from source image
	// return
	//	pointer to pattern structure created (of type NEPAT)
	// args
	//	src :		address of (0,0) pixel of source image
	//	memw :		memory width of source image
	//	left,top,right,bottom :	region of image to register
	//	hotOffX,hotOffY :	reference (x,y) of pattern
	//				w.r.t. pattern image.
	//	bUseSrc :	nonzero is possible if source image remains
	//			unchanged while using this pattern. it is to
	//			save memory space used to copy original.
	//			if source changes, set this to be 0.

void	nematFreePattern(NEPAT *ppat);
	// frees pattern structure, which is created by nematBuildPattern
	// args
	//	ppat :		pointer  to NEPAT object, which was
	//			created by nematBuildPattern function. *ppat becomes
	//			0 when this function ends.

int	nematSearch(NEPAT pat,PBYTE dst,int memw,
		int left,int top,int right,int bottom,float minScore,int pixRes,
		float *locX,float *locY,float *score);
	// executes pattern matching without rotation option
	// return
	//	nonzero if found, zero otherwise.
	// args
	//	pat :		pointer to NEPAT structure created by nematBuildPattern.
	//	dst :		address of (0,0) pixel of destination image
	//	memw :		memory width of destination image
	//	left,top,right,bottom :	search region in destination image
	//	minScore :	minimum acceptance level used for algorithm.
	//			valid range is (0.0~1.0). set 0.5 for normal case.
	//			increase this to speed up searching.
	//			decrease this to find noisy object.
	//	pixRes	:	pixel resolution.
	//				setting value of 1 (minimum) means full searching.
	//				increasing this value speeds up searching, but
	//				makes less reliable.
	//	locX :		pointer to save result X of 'pat's hot point.
	//			set 0 to skip saving
	//	locY :		pointer to save result Y of 'pat's hot point.
	//			set 0 to skip saving
	//	score :		pointer to save result score (0.0~1.0).
	//			set 0 to skip saving

int	nematSearchWithAngle(NEPAT pat,PBYTE dst,int memw,
		int left,int top,int right,int bottom,float loDegree,float hiDegree,
		float minScore,int pixRes,
		float *locX,float *locY,float *degree,float *score);
	// executes pattern matching with rotation option
	// return
	//	nonzero if found, zero otherwise.
	// args
	//	pat :		pointer to NEPAT structure created by nematBuildPattern.
	//	dst :		address of (0,0) pixel of destination image
	//	memw :		memory width of destination image
	//	left,top,right,bottom :	search region in destination image
	//	loDegree,hiDegree :	low/high degree value of search angle.
	//			the angle increases clockwise. loDegree<hiDegree must.
	//	minScore :	minimum acceptance level used for algorithm.
	//			valid range is (0.0~1.0). set 0.5 for normal case.
	//			increase this to speed up searching.
	//			decrease this to find noisy object.
	//	pixRes	:	pixel resolution.
	//				setting value of 1 (minimum) means full searching.
	//				increasing this value speeds up searching, but
	//				makes less reliable.
	//	locX :		pointer to save result X of 'pat's hot point.
	//			set 0 to skip saving
	//	locY :		pointer to save result Y of 'pat's hot point.
	//			set 0 to skip saving
	//	degree :	pointer to save result Theta of 'pat'.
	//			set 0 to skip saving
	//	score :		pointer to save result score (0.0~1.0).
	//			set 0 to skip saving

public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPatMat)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPatMat();

	// Generated message map functions
protected:
	//{{AFX_MSG(CPatMat)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
//	DECLARE_MESSAGE_MAP()

private:
	void	nematShowError(const char *msg);
	void	nematShrink(NEIMG dn,NEIMG up,int bCalcSum);
	float	nematOrgMatch(NEIMG img0,NEIMG img,int level);
	float	nematCorr(PBYTE src,int smemw,float s_s,float s_ss,
			PBYTE dst,int dmemw,int w,int h);
	float	nematDownSearch(NEIMG *dimg,NEIMG *patimg,int level,int pixLevel,
			float minScore,int x,int y,SUBINFO subInfo);
	float	nematBottomSearch(NEIMG dimg,NEIMG patimg,int pixLevel,
			int x,int y,SUBINFO subInfo);
	void	nematSubPixel(NEIMG img,NEIMG patimg,SUBINFO subInfo,float *rx,float *ry);
	float	nematCorr_Angle(PBYTE src,int smemw,int sw,int sh,float s_s,float s_ss,
			PBYTE dst,int dmemw,int dw,int dh,int x,int y,float degree);
	float	nematDownSearch_Angle(NEIMG *dimg,NEIMG *patimg,int level,int pixLevel,
			float minScore,int x,int y,float degree,SUBINFO subInfo);
	void	nematSubPixel_Angle(NEIMG img,NEIMG patimg,SUBINFO subInfo,
			float *rx,float *ry,float *degree);
	float	nematBottomSearch_Angle(NEIMG dimg,NEIMG patimg,int pixLevel,
			int x,int y,float degree,SUBINFO subInfo);
	float	nematCos(float theta);
	float	nematSin(float theta);

//variable
//	static PBYTE	*nematAlloc8;
//	static void		*nematFree8;
//	static void		*nematShowErrorFunc;

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PATMAT_H__010C6023_65E3_11D5_8210_00032D00198F__INCLUDED_)
