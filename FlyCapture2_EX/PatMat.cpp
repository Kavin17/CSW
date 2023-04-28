// PatMat.cpp : implementation file
//

#include "stdafx.h"
//#include "DlgBase.h"
#include "PatMat.h"
#include <math.h>
#define PI 3.14159

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPatMat
static PBYTE	(*nematAlloc8)(int)=0;
static void	(*nematFree8)(PBYTE)=0;
static void	(*nematShowErrorFunc)(const char*)=0;

CPatMat::CPatMat()
{
//	(*nematAlloc8)(int)=0;
//	(*nematFree8)(PBYTE)=0;
//	(*nematShowErrorFunc)(const char*)=0;
}

CPatMat::~CPatMat()
{
}
/*
BEGIN_MESSAGE_MAP(CPatMat, CWnd)
	//{{AFX_MSG_MAP(CPatMat)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
*/


/////////////////////////////////////////////////////////////////////////////
// CPatMat message handlers

void CPatMat::nematInit(PBYTE (*alloc_8)(int),void (*free_8)(PBYTE)) {
	nematAlloc8 = (alloc_8)? alloc_8 : (PBYTE (*)(int))malloc;
	nematFree8 = (free_8)? free_8 : (void (*)(PBYTE))free;
	nematShowErrorFunc = 0;
}

void CPatMat::nematSetErrorDisplay(void (*showError)(const char *)) {
	nematShowErrorFunc = showError;
}

NEPAT CPatMat::nematBuildPattern(PBYTE src,int memw,
		int left,int top,int right,int bottom,
		float hotOffX,float hotOffY,int bUseSrc) {
int	ok=0;
NEPAT	pat=0;
int	width,height;
int	i,x,y;
unsigned int	val,sum,sumsum;
float	sumsumF;
	// alloc pattern
	if (!(pat=(NEPAT)malloc(sizeof(struct tagNEPAT)))) {
		nematShowError("Memory shortage");
		goto done;
	}
	// assign
	pat->hotOffX = hotOffX;
	pat->hotOffY = hotOffY;
	pat->bUseSrc = bUseSrc;
	pat->nLevel = 0;
	for (i=0; i<MAX_LEVEL; i++)
		pat->image[i] = 0;
	// set level 0
	if (!(pat->image[0]=(NEIMG)malloc(sizeof(struct tagNEIMG)))) {
		nematShowError("Memory shortage");
		goto done;
	}
	width = right-left+1;
	height = bottom-top+1;
	if (width<MIN_PATSIDE || height<MIN_PATSIDE)
		goto done;
	if (bUseSrc) {
		pat->image[0]->bits = src+top*memw+left;
		pat->image[0]->memw = memw;
		sum = 0;
		sumsumF = 0.0;
		for (y=top; y<=bottom; y++) {
			sumsum = 0;
			for (x=left; x<=right; x++) {
				val = src[y*memw+x];
				sum += val;
				sumsum += val*val;
			}
			sumsumF += (float)sumsum;
		}
	}
	else {
		if (!(pat->image[0]->bits=(*nematAlloc8)(width*height)))
			goto done;
		pat->image[0]->memw = width;
		sum = 0;
		sumsumF = 0.0;
		for (y=top; y<=bottom; y++) {
			sumsum = 0;
			for (x=left; x<=right; x++) {
				val = src[y*memw+x];
				pat->image[0]->bits[(y-top)*width+x-left] = val;
				sum += val;
				sumsum += val*val;
			}
			sumsumF += (float)sumsum;
		}
	}
	pat->image[0]->width = width;
	pat->image[0]->height = height;
	pat->image[0]->s = (float)sum;
	pat->image[0]->ss = sumsumF;
	pat->image[0]->dAngle = 180.0*atan(2.0/ sqrt((float)((width-1)*(width-1)+(height-1)*(height-1))))/PI;
	pat->nLevel = 1;
	// build levels
	for (i=1; i<MAX_LEVEL; i++) {
		width = pat->image[i-1]->width>>1;
		height = pat->image[i-1]->height>>1;
		if (width<MIN_PATSIDE || height<MIN_PATSIDE)
			break;
		if (!(pat->image[i]=(NEIMG)malloc(sizeof(struct tagNEIMG)))) {
			nematShowError("Memory shortage");
			goto done;
		}
		if (!(pat->image[i]->bits=(*nematAlloc8)(width*height))) {
			nematShowError("Image buffer shortage");
			goto done;
		}
		pat->image[i]->memw = width;
		pat->image[i]->width = width;
		pat->image[i]->height = height;
		nematShrink(pat->image[i-1],pat->image[i],1);
		pat->image[i]->dAngle = 180.0*atan(2.0/
		 sqrt((float)((width-1)*(width-1)+(height-1)*(height-1))))/PI;
		if (nematOrgMatch(pat->image[0],pat->image[i],i)<=ORG_SCORE*ORG_SCORE) {
			(*nematFree8)(pat->image[i]->bits);
			free(pat->image[i]);
			pat->image[i] = 0;
			break;
		}
		pat->nLevel++;
	}
	// done
	ok = 1;
done:
	if (!ok) {
		nematFreePattern(&pat);
		nematShowError("FAIL: nematBuildPattern");
	}
	return pat;
}

void	CPatMat::nematFreePattern(NEPAT *ppat) {
NEPAT	pat;
int	i;
	if (!ppat || !(pat=*ppat))
		return;
	if (!pat->bUseSrc)
		(*nematFree8)(pat->image[0]->bits);
	free(pat->image[0]);
	for (i=1; i<pat->nLevel; i++) {
		(*nematFree8)(pat->image[i]->bits);
		free(pat->image[i]);
	}
	free(pat);
	*ppat = 0;
}

#define	MAP(x,y)	map[((y)+1)*(sw+2)+(x)+1]

int	CPatMat::nematSearch(NEPAT pat,PBYTE dst,int memw,
		int left,int top,int right,int bottom,float minScore,int pixRes,
		float *locX,float *locY,float *score) {
int	ok=0;
NEIMG	dimg[MAX_LEVEL];
int	i,j;
int	width,height;
float	*map=0;
int	sw,sh;
int	level,pixLevel;
int	x,y;
float	maxScore;
float	tmp;
SUBINFO	maxSubInfo=0,subInfo=0;
float	LM,RM,MT,MB;
float	rx,ry;
	// init
	for (i=0; i<MAX_LEVEL; i++)
		dimg[i] = 0;
	level = pat->nLevel;
	// check score,width,height
	if (minScore<0.0)
		minScore = 0.0;
	minScore *= minScore;
	if (pixRes<1)
		pixRes = 1;
	for (i=0,j=1; i<level; i++,j<<=1)
		if (j<=pixRes)
			pixLevel = i;
	width = right-left+1;
	height = bottom-top+1;
	if (width<pat->image[0]->width || height<pat->image[0]->height)		//Search Area가 Pattern Size보다 커야 한다. 
		goto done;
	// build dimg
	if (!(dimg[0]=(NEIMG)malloc(sizeof(struct tagNEIMG)))) {
		nematShowError("Memory shortage");
		goto done;
	}
	dimg[0]->bits = dst+top*memw+left;
	dimg[0]->memw = memw;
	dimg[0]->width = width;
	dimg[0]->height = height;
	for (i=1; i<level; i++) {
		width = dimg[i-1]->width>>1;
		height = dimg[i-1]->height>>1;
		if (!(dimg[i]=(NEIMG)malloc(sizeof(struct tagNEIMG)))) {
			nematShowError("Memory shortage");
			goto done;
		}
		if (!(dimg[i]->bits=(*nematAlloc8)(width*height))) {
			nematShowError("Image buffer shortage");
			goto done;
		}
		dimg[i]->memw = width;
		dimg[i]->width = width;
		dimg[i]->height = height;
		nematShrink(dimg[i-1],dimg[i],0);
	}
	// search at level-1 to map
	sw = dimg[level-1]->width-pat->image[level-1]->width+1;
	sh = dimg[level-1]->height-pat->image[level-1]->height+1;
	if (!(map=(float*)malloc((sw+2)*(sh+2)*sizeof(float)))) {
		nematShowError("Memory shortage");
		goto done;
	}
	for (x=-1; x<=sw; x++)
		MAP(x,-1) = MAP(x,sh) = 0.0;
	for (y=0; y<=sh-1; y++)
		MAP(-1,y) = MAP(sw,y) = 0.0;
	for (y=0; y<sh; y++)
		for (x=0; x<sw; x++)
			MAP(x,y) = nematCorr(pat->image[level-1]->bits,pat->image[level-1]->memw,
			 pat->image[level-1]->s,pat->image[level-1]->ss,
			 dimg[level-1]->bits+y*dimg[level-1]->memw+x,dimg[level-1]->memw,
			 pat->image[level-1]->width,pat->image[level-1]->height);
	// find location
	if (!(maxSubInfo=(SUBINFO)malloc(sizeof(struct tagSUBINFO)))) {
		nematShowError("Memory shortage");
		goto done;
	}
	maxScore = -2.0;
	if (level>1) {
		if (!(subInfo=(SUBINFO)malloc(sizeof(struct tagSUBINFO)))) {
			nematShowError("Memory shortage");
			goto done;
		}
		for (y=0; y<sh; y++)
		for (x=0; x<sw; x++)
			if ((tmp=MAP(x,y))>minScore) {
				if (tmp>=MAP(x-1,y-1) && tmp>=MAP(x+1,y+1) &&
				 tmp>=MAP(x-1,y+1) && tmp>=MAP(x+1,y-1) &&
				 tmp>=MAP(x-1,y) && tmp>=MAP(x+1,y) &&
				 tmp>=MAP(x,y-1) && tmp>=MAP(x,y+1)) {
					tmp = (level-1==pixLevel)?
					 nematBottomSearch(dimg[pixLevel],pat->image[pixLevel],pixLevel,
					 x,y,subInfo) :
					 nematDownSearch(dimg,pat->image,level-2,pixLevel,minScore,
					 x<<1,y<<1,subInfo);
					if (tmp>maxScore) {
						maxScore = tmp;
						memcpy(maxSubInfo,subInfo,
						 sizeof(struct tagSUBINFO));
					}
				}
			}
	}
	else {
		maxSubInfo->level = 0;
		for (y=0; y<sh; y++)
		for (x=0; x<sw; x++)
			if ((tmp=MAP(x,y))>minScore) {
				if (tmp>=MAP(x-1,y-1) && tmp>=MAP(x+1,y+1) &&
				 tmp>=MAP(x-1,y+1) && tmp>=MAP(x+1,y-1) &&
				 tmp>=(LM=MAP(x-1,y)) && tmp>=(RM=MAP(x+1,y)) &&
				 tmp>=(MT=MAP(x,y-1)) && tmp>=(MB=MAP(x,y+1))) {
					if (tmp>maxScore) {
						maxScore = tmp;
						maxSubInfo->x = x;
						maxSubInfo->y = y;
						maxSubInfo->sl = LM;
						maxSubInfo->st = MT;
						maxSubInfo->sr = RM;
						maxSubInfo->sb = MB;
						maxSubInfo->sm = tmp;
					}
				}
			}
	}
	if (maxScore<=0.0)
		goto done;
	nematSubPixel(dimg[maxSubInfo->level],pat->image[maxSubInfo->level],maxSubInfo,
		&rx,&ry);
	if (locX)
		*locX = (float)left+rx+pat->hotOffX;
	if (locY)
		*locY = (float)top+ry+pat->hotOffY;
	if (score)
		*score = (float)sqrt(maxScore);
	// done
	ok = 1;
done:
	if (dimg[0])
		free(dimg[0]);
	for (i=1; i<MAX_LEVEL; i++)
		if (dimg[i]) {
			(*nematFree8)(dimg[i]->bits);
			free(dimg[i]);
		}
	if (map)
		free(map);
	if (maxSubInfo)
		free(maxSubInfo);
	if (subInfo)
		free(subInfo);
	return ok;
}

#undef	MAP
#define	MAP(x,y,a)	map[((a)+1)*((srw+2)*(srh+2))+((y)+1)*(srw+2)+(x)+1]

int	CPatMat::nematSearchWithAngle(NEPAT pat,PBYTE dst,int memw,
		int left,int top,int right,int bottom,float loDegree,float hiDegree,
		float minScore,int pixRes,
		float *locX,float *locY,float *degree,float *score) {
int	ok=0;
NEIMG	dimg[MAX_LEVEL];
int	i,j;
int	width,height;
float	*map=0;
int	srw,srh,sa,sw,sh;
float	a0,da,angle;
int	level,pixLevel;
int	x,y,a;
float	maxScore;
float	tmp;
SUBINFO	maxSubInfo=0,subInfo=0;
float	LM,RM,MT,MB,UP,DN;
float	rx,ry;
float	cost,sint;
	// init
	for (i=0; i<MAX_LEVEL; i++)
		dimg[i] = 0;
	level = pat->nLevel;
	// check score,width,height
	if (minScore<0.0)
		minScore = 0.0;
	minScore *= minScore;
	if (pixRes<1)
		pixRes = 1;
	for (i=0,j=1; i<level; i++,j<<=1)
		if (j<=pixRes)
			pixLevel = i;
	width = right-left+1;
	height = bottom-top+1;
	if (loDegree>=hiDegree)
		goto done;
	// build dimg
	if (!(dimg[0]=(NEIMG)malloc(sizeof(struct tagNEIMG)))) {
		nematShowError("Memory shortage");
		goto done;
	}
	dimg[0]->bits = dst+top*memw+left;
	dimg[0]->memw = memw;
	dimg[0]->width = width;
	dimg[0]->height = height;
	for (i=1; i<level; i++) {
		width = dimg[i-1]->width>>1;
		height = dimg[i-1]->height>>1;
		if (!(dimg[i]=(NEIMG)malloc(sizeof(struct tagNEIMG)))) {
			nematShowError("Memory shortage");
			goto done;
		}
		if (!(dimg[i]->bits=(*nematAlloc8)(width*height))) {
			nematShowError("Image buffer shortage");
			goto done;
		}
		dimg[i]->memw = width;
		dimg[i]->width = width;
		dimg[i]->height = height;
		nematShrink(dimg[i-1],dimg[i],0);
	}
	// search at level-1 to map
	sw = pat->image[level-1]->width;
	sh = pat->image[level-1]->height;
	da = pat->image[level-1]->dAngle;
	srw = dimg[level-1]->width;
	srh = dimg[level-1]->height;
	for (angle=0.5*(loDegree+hiDegree); angle>loDegree; angle-=da);
	a0 = angle;
	sa = (int)(2.0*(0.5*(loDegree+hiDegree)-a0)/da+1.5);
	if (!(map=(float*)malloc((srw+2)*(srh+2)*(sa+2)*sizeof(float)))) {
		nematShowError("Memory shortage");
		goto done;
	}
	for (x=0; x<srw; x++)
		for (y=0; y<srh; y++)
			MAP(x,y,-1) = MAP(x,y,sa) = 0.0;
	for (a=-1; a<=sa; a++) {
		for (x=-1; x<=srw; x++)
			MAP(x,-1,a) = MAP(x,srh,a) = 0.0;
		for (y=0; y<=srh-1; y++)
			MAP(-1,y,a) = MAP(srw,y,a) = 0.0;
	}
	for (a=0; a<sa; a++)
	for (y=0; y<srh; y++)
	for (x=0; x<srw; x++)
		MAP(x,y,a) = nematCorr_Angle(
		 pat->image[level-1]->bits,pat->image[level-1]->memw,
		 sw,sh,
		 pat->image[level-1]->s,pat->image[level-1]->ss,
		 dimg[level-1]->bits,dimg[level-1]->memw,
		 dimg[level-1]->width,dimg[level-1]->height,
		 x,y,a0+a*da);
	// find location
	if (!(maxSubInfo=(SUBINFO)malloc(sizeof(struct tagSUBINFO)))) {
		nematShowError("Memory shortage");
		goto done;
	}
	maxScore = -2.0;
	if (level>1) {
		if (!(subInfo=(SUBINFO)malloc(sizeof(struct tagSUBINFO)))) {
			nematShowError("Memory shortage");
			goto done;
		}
		for (a=0; a<sa; a++)
		for (y=0; y<srh; y++)
		for (x=0; x<srw; x++)
			if ((tmp=MAP(x,y,a))>minScore) {
				if (tmp>=MAP(x-1,y-1,a) && tmp>=MAP(x+1,y+1,a) &&
				 tmp>=MAP(x-1,y+1,a) && tmp>=MAP(x+1,y-1,a) &&
				 tmp>=MAP(x-1,y,a) && tmp>=MAP(x+1,y,a) &&
				 tmp>=MAP(x,y-1,a) && tmp>=MAP(x,y+1,a) &&
				 tmp>=MAP(x,y,a-1) && tmp>=MAP(x,y,a+1) &&
				 tmp>=MAP(x-1,y-1,a-1) && tmp>=MAP(x+1,y+1,a+1) &&
				 tmp>=MAP(x+1,y+1,a-1) && tmp>=MAP(x-1,y-1,a+1) &&
				 tmp>=MAP(x-1,y+1,a-1) && tmp>=MAP(x+1,y-1,a+1) &&
				 tmp>=MAP(x+1,y-1,a-1) && tmp>=MAP(x-1,y+1,a+1) &&
				 tmp>=MAP(x-1,y,a-1) && tmp>=MAP(x+1,y,a+1) &&
				 tmp>=MAP(x+1,y,a-1) && tmp>=MAP(x-1,y,a+1) &&
				 tmp>=MAP(x,y-1,a-1) && tmp>=MAP(x,y+1,a+1) &&
				 tmp>=MAP(x,y+1,a-1) && tmp>=MAP(x,y-1,a+1)) {
					tmp = (level-1==pixLevel)?
					 nematBottomSearch_Angle(dimg[pixLevel],pat->image[pixLevel],pixLevel,
					 x,y,a0+a*da,subInfo) :
					 nematDownSearch_Angle(dimg,pat->image,level-2,pixLevel,minScore,
					 x<<1,y<<1,a0+a*da,subInfo);
					if (tmp>maxScore) {
						maxScore = tmp;
						memcpy(maxSubInfo,subInfo,
						 sizeof(struct tagSUBINFO));
					}
				}
			}
	}
	else {
		maxSubInfo->level = 0;
		for (a=0; a<sa; a++)
		for (y=0; y<srh; y++)
		for (x=0; x<srw; x++)
			if ((tmp=MAP(x,y,a))>minScore) {
				if (tmp>=MAP(x-1,y-1,a) && tmp>=MAP(x+1,y+1,a) &&
				 tmp>=MAP(x-1,y+1,a) && tmp>=MAP(x+1,y-1,a) &&
				 tmp>=(LM=MAP(x-1,y,a)) && tmp>=(RM=MAP(x+1,y,a)) &&
				 tmp>=(MT=MAP(x,y-1,a)) && tmp>=(MB=MAP(x,y+1,a)) &&
				 tmp>=(UP=MAP(x,y,a-1)) && tmp>=(DN=MAP(x,y,a+1)) &&
				 tmp>=MAP(x-1,y-1,a-1) && tmp>=MAP(x+1,y+1,a+1) &&
				 tmp>=MAP(x+1,y+1,a-1) && tmp>=MAP(x-1,y-1,a+1) &&
				 tmp>=MAP(x-1,y+1,a-1) && tmp>=MAP(x+1,y-1,a+1) &&
				 tmp>=MAP(x+1,y-1,a-1) && tmp>=MAP(x-1,y+1,a+1) &&
				 tmp>=MAP(x-1,y,a-1) && tmp>=MAP(x+1,y,a+1) &&
				 tmp>=MAP(x+1,y,a-1) && tmp>=MAP(x-1,y,a+1) &&
				 tmp>=MAP(x,y-1,a-1) && tmp>=MAP(x,y+1,a+1) &&
				 tmp>=MAP(x,y+1,a-1) && tmp>=MAP(x,y-1,a+1)) {
					if (tmp>maxScore) {
						maxScore = tmp;
						maxSubInfo->x = x;
						maxSubInfo->y = y;
						maxSubInfo->degree = a0+a*da;
						maxSubInfo->da = da;
						maxSubInfo->sl = LM;
						maxSubInfo->st = MT;
						maxSubInfo->sr = RM;
						maxSubInfo->sb = MB;
						maxSubInfo->sm = tmp;
						maxSubInfo->su = UP;
						maxSubInfo->sd = DN;
					}
				}
			}
	}
	if (maxScore<=0.0)
		goto done;
	nematSubPixel_Angle(dimg[maxSubInfo->level],pat->image[maxSubInfo->level],maxSubInfo,
	 &rx,&ry,&angle);
	cost = nematCos(PI*angle/180.0);
	sint = nematSin(PI*angle/180.0);
	if (locX)
		*locX = (float)left+rx+cost*pat->hotOffX-sint*pat->hotOffY;
	if (locY)
		*locY = (float)top+ry+sint*pat->hotOffX+cost*pat->hotOffY;
	if (degree)
		*degree = angle;
	if (score)
		*score = sqrt(maxScore);
	// done
	ok = 1;
done:
	if (dimg[0])
		free(dimg[0]);
	for (i=1; i<MAX_LEVEL; i++)
		if (dimg[i]) {
			(*nematFree8)(dimg[i]->bits);
			free(dimg[i]);
		}
	if (map)
		free(map);
	if (maxSubInfo)
		free(maxSubInfo);
	if (subInfo)
		free(subInfo);
	return ok;
}
#undef	MAP

void CPatMat::nematShowError(const char *msg) {
	if (nematShowErrorFunc)
		(*nematShowErrorFunc)(msg);
}

void	CPatMat::nematShrink(NEIMG dn,NEIMG up,int bCalcSum) {
PBYTE	db,ub;
int	dm,dw,dh,um,uw,uh;
int	dx,dy,ux,uy;
int	offset;
unsigned int	val,sum,sumsum;
float	sumsumF;
	db = dn->bits;
	dm = dn->memw;
	dw = dn->width;
	dh = dn->height;
	ub = up->bits;
	um = up->memw;
	uw = up->width;
	uh = up->height;
	if (bCalcSum) {
		sum = 0;
		sumsumF = 0.0;
		for (uy=0,dy=0; uy<uh; uy++,dy+=2) {
			sumsum = 0;
			for (ux=0,dx=0; ux<uw; ux++,dx+=2) {
				offset = dy*dm+dx;
				val = ub[uy*um+ux] = ((unsigned int)db[offset]+
				 (unsigned int)db[offset+1]+
				 (unsigned int)db[offset+dm]+
				 (unsigned int)db[offset+dm+1])>>2;
				sum += val;
				sumsum += val*val;
			}
			sumsumF += (float)sumsum;
		}
		up->s = (float)sum;
		up->ss = sumsumF;
	}
	else {
		for (uy=0,dy=0; uy<uh; uy++,dy+=2)
			for (ux=0,dx=0; ux<uw; ux++,dx+=2) {
				offset = dy*dm+dx;
				ub[uy*um+ux] = ((int)db[offset]+(int)db[offset+1]+
				 (int)db[offset+dm]+(int)db[offset+dm+1])>>2;
			}
	}
}

float CPatMat::nematOrgMatch(NEIMG img0,NEIMG img,int level) {
int	val1,val2;
unsigned int	sum1,sum2,sum11,sum12,sum22;
float	sum11F,sum12F,sum22F;
int	u,w_1,h;
int	smemw,dmemw,dmemwz;
PBYTE	soff,doff,soffe,dhead;
float	s1,s2,s11,s12,s22;
float	len,upper,below;
int	x,y;
int	level2;
int	zoom;
	sum1 = sum2 = sum11 = sum12 = sum22 = 0;
	sum11F = sum12F = sum22F = 0.0;
	w_1 = img->width-1;
	h = img->height;
	level2 = level<<1;
	zoom = 0x1<<level;
	smemw = img->memw;
	dmemw = img0->memw;
	dmemwz = dmemw<<level;
	for (soff=img->bits,soffe=img->bits+h*smemw,doff=img0->bits; soff<soffe;
	 soff+=smemw,doff+=dmemwz) {
		sum11 = sum12 = sum22 = 0;
		for (u=0,dhead=doff; u<=w_1; u++,dhead+=zoom) {
			val1 = soff[u];
			sum1 += val1<<level2;
			sum11 += (val1*val1)<<level2;
			for (y=0; y<zoom; y++)
				for (x=0; x<zoom; x++) {
					val2 = dhead[y*dmemw+x];
					sum2 += val2;
					sum22 += val2*val2;
					sum12 += val1*val2;
				}
		}
		sum11F += (float)sum11;
		sum12F += (float)sum12;
		sum22F += (float)sum22;
	}
	len = (float)(((w_1+1)*h)<<level2);
	s1 = (float)sum1;
	s2 = (float)sum2;
	s11 = sum11F;
	s12 = sum12F;
	s22 = sum22F;
	upper = len*s12-s1*s2;
	below = (len*s11-s1*s1)*(len*s22-s2*s2);
	return (upper<=0.0 || below<0.01)? -1.0 : upper*upper/below;
}

float	CPatMat::nematCorr(PBYTE src,int smemw,float s_s,float s_ss,PBYTE dst,int dmemw,int w,int h) {
unsigned int	val2;
unsigned int	sum2,sum12,sum22;
int	u,w_1;
PBYTE	soff,doff,soffe;
float	s2,s12,s22;
float	len,upper,below;
	sum2 = 0;
	s12 = s22 = 0.0;
	w_1 = w-1;
	for (soff=src,soffe=src+h*smemw,doff=dst; soff<soffe; soff+=smemw,doff+=dmemw) {
		sum12 = sum22 = 0;
		for (u=0; u<=w_1; u++) {
			val2 = doff[u];
			sum2 += val2;
			sum22 += val2*val2;
			sum12 += soff[u]*val2;
		}
		s12 += (float)sum12;
		s22 += (float)sum22;
	}
	len = (float)(w*h);
	s2 = (float)sum2;
	upper = len*s12-s_s*s2;
	below = (len*s_ss-s_s*s_s)*(len*s22-s2*s2);
	return (upper<=0.0 || below<0.01)? 0.0 : upper*upper/below;
}

float	CPatMat::nematDownSearch(NEIMG *dimg,NEIMG *patimg,int level,int pixLevel,
		float minScore,int x,int y,SUBINFO subInfo) {

	CString str;
	str.Format("\n%3d %3d %3d", level, x, y);
//	TRACE(str);
PBYTE	dst,src;
int	dmemw,dw,dh,smemw,sw,sh,minU,maxU,minV,maxV;
float	maxScore,score;
int	u,v,uMax,vMax;
float	s_s,s_ss;
	// check
	if (level==pixLevel)
		return nematBottomSearch(dimg[pixLevel],patimg[pixLevel],pixLevel,x,y,subInfo);
	// set
	dst = dimg[level]->bits;
	dmemw = dimg[level]->memw;
	dw = dimg[level]->width;
	dh = dimg[level]->height;
	src = patimg[level]->bits;
	smemw = patimg[level]->memw;
	sw = patimg[level]->width;
	sh = patimg[level]->height;
	s_s = patimg[level]->s;
	s_ss = patimg[level]->ss;
	if ((minU=x-1)<0)
		minU = 0;
	if ((maxU=x+2)+sw>dw)
		maxU = dw-sw;
	if ((minV=y-1)<0)
		minV = 0;
	if ((maxV=y+2)+sh>dh)
		maxV = dh-sh;
	// search max
	maxScore = -2.0;
	uMax = x;
	vMax = y;
	for (v=minV; v<=maxV; v++)
		for (u=minU; u<=maxU; u++) {
			score = nematCorr(src,smemw,s_s,s_ss,
			 dst+v*dmemw+u,dmemw,sw,sh);
			if (score>maxScore) {
				maxScore = score;
				uMax = u;
				vMax = v;
			}
		}
	// decide recursion
	if (maxScore>=minScore)
		return nematDownSearch(dimg,patimg,level-1,pixLevel,minScore,
		 uMax<<1,vMax<<1,subInfo);
	// otherwise
	subInfo->level = level;
	subInfo->x = uMax;
	subInfo->y = vMax;
	return maxScore;
}

float	CPatMat::nematBottomSearch(NEIMG dimg,NEIMG patimg,int pixLevel,
			int x,int y,SUBINFO subInfo) {
PBYTE	dst,src;
int	dmemw,dw,dh,smemw,sw,sh;
float	s_s,s_ss;
float	map[5][5];
int	i,j,u,v;
float	maxScore,score;
int	maxI,maxJ;
	// set
	dst = dimg->bits;
	dmemw = dimg->memw;
	dw = dimg->width;
	dh = dimg->height;
	src = patimg->bits;
	smemw = patimg->memw;
	sw = patimg->width;
	sh = patimg->height;
	s_s = patimg->s;
	s_ss = patimg->ss;
	// search max
	maxScore = -2.0;
	for (i=0; i<=4; i++)
		map[i][0] = map[i][4] = -1.0;
	for (j=1; j<=3; j++)
		map[0][j] = map[4][j] = -1.0;
	for (j=1; j<=3; j++) {
		v = y+j-2;
		if (v<0 || v+sh>dh) {
			for (i=1; i<=3; i++)
				map[i][j] = 0.0;
			continue;
		}
		for (i=1; i<=3; i++) {
			u = x+i-2;
			if (u<0 || u+sw>dw) {
				map[i][j] = 0.0;
				continue;
			}
			score = map[i][j] = nematCorr(src,smemw,s_s,s_ss,
			 dst+v*dmemw+u,dmemw,sw,sh);
			if (score>maxScore) {
				maxScore = score;
				maxI = i;
				maxJ = j;
			}
		}
	}
	subInfo->level = pixLevel;
	subInfo->x = x+maxI-2;
	subInfo->y = y+maxJ-2;
	subInfo->sl = map[maxI-1][maxJ];
	subInfo->st = map[maxI][maxJ-1];
	subInfo->sr = map[maxI+1][maxJ];
	subInfo->sb = map[maxI][maxJ+1];
	subInfo->sm = maxScore;
	return maxScore;
}

void	CPatMat::nematSubPixel(NEIMG img,NEIMG patimg,SUBINFO subInfo,float *rx,float *ry) {
PBYTE	src,dst;
int	smemw,sw,sh,dmemw,dx,dy,dw,dh;
float	s,ss;
float	d1,d2,under;
float	resX,resY,resMult;
	src = patimg->bits;
	smemw = patimg->memw;
	sw = patimg->width;
	sh = patimg->height;
	s = patimg->s;
	ss = patimg->ss;
	dst = img->bits;
	dmemw = img->memw;
	dx = subInfo->x;
	dy = subInfo->y;
	dw = img->width;
	dh = img->height;
	if (subInfo->sl<-0.5)
		subInfo->sl = (dx>0)? nematCorr(src,smemw,s,ss,
		 dst+dy*dmemw+dx-1,dmemw,sw,sh) : 0.0;
	if (subInfo->st<-0.5)
		subInfo->st = (dy>0)? nematCorr(src,smemw,s,ss,
		 dst+(dy-1)*dmemw+dx,dmemw,sw,sh) : 0.0;
	if (subInfo->sr<-0.5)
		subInfo->sr = (dx+sw<=dw)? nematCorr(src,smemw,s,ss,
		 dst+dy*dmemw+dx+1,dmemw,sw,sh) : 0.0;
	if (subInfo->sb<-0.5)
		subInfo->sb = (dy+sh<=dh)? nematCorr(src,smemw,s,ss,
		 dst+(dy+1)*dmemw+dx,dmemw,sw,sh) : 0.0;

//2002/5/23 비디오라벨검사 하면서 X위치 튀는 현상 제거 
	under = 2.0*(subInfo->sl+subInfo->sr-2.0*subInfo->sm);
	resX = (under>-0.0000001)? (float)dx : (float)dx+(subInfo->sl-subInfo->sr)/under;
	under = 2.0*(subInfo->st+subInfo->sb-2.0*subInfo->sm);
	resY = (under>-0.0000001)? (float)dy : (float)dy+(subInfo->st-subInfo->sb)/under;
	resMult = pow(2.0,subInfo->level);
	*rx = resMult*resX;
	*ry = resMult*resY;
/*	
	d1 = subInfo->sm-subInfo->sl;
	d2 = subInfo->sr-subInfo->sm;
	if ((under=d1-d2)<0.0000001)
		resX = (float)dx;
	else
		resX = (float)dx-0.5-d2/under;
	d1 = subInfo->sm-subInfo->st;
	d2 = subInfo->sb-subInfo->sm;
	if ((under=d1-d2)<0.0000001)
		resY = (float)dy;
	else
		resY = (float)dy-0.5-d2/under;
	resMult = pow(2.0,subInfo->level);
	*rx = resMult*resX;
	*ry = resMult*resY;
*/
}
float	CPatMat::nematCorr_Angle(PBYTE src,int smemw,int sw,int sh,float s_s,float s_ss,
		PBYTE dst,int dmemw,int dw,int dh,int x,int y,float degree) {
unsigned int	val2;
unsigned int	sum2,sum12,sum22;
float	cost,sint;
int	dx,u;
float	drx,dry,dcx,dcy;
PBYTE	soff,soffe;
float	s2,s12,s22;
float	len,upper,below;
	// check
	if (x<0 || x>=dw || y<0 || y>=dh)
		return 0.0;
	cost = nematCos(PI*degree/180.0);
	sint = nematSin(PI*degree/180.0);
	// check
	dx = (int)(x+cost*(sw-1)+0.5);
	u = (int)(y+sint*(sw-1)+0.5);
	if (dx<0 || dx>=dw || u<0 || u>=dh)
		return 0.0;
	dx = (int)(x-sint*(sh-1)+0.5);
	u = (int)(y+cost*(sh-1)+0.5);
	if (dx<0 || dx>=dw || u<0 || u>=dh)
		return 0.0;
	dx = (int)(x+cost*(sw-1)-sint*(sh-1)+0.5);
	u = (int)(y+sint*(sw-1)+cost*(sh-1)+0.5);
	if (dx<0 || dx>=dw || u<0 || u>=dh)
		return 0.0;
	// begin
	sum2 = 0;
	s12 = s22 = 0.0;
	dx = sw-1;
	for (soff=src,soffe=src+sh*smemw,drx=(float)x+0.5,dry=(float)y+0.5; soff<soffe;
	 soff+=smemw,drx-=sint,dry+=cost) {
		sum12 = sum22 = 0;
		for (u=0,dcx=drx,dcy=dry; u<=dx; u++,dcx+=cost,dcy+=sint) {
			val2 = dst[((int)dcy)*dmemw+(int)dcx];
			sum2 += val2;
			sum22 += val2*val2;
			sum12 += soff[u]*val2;
		}
		s12 += (float)sum12;
		s22 += (float)sum22;
	}
	len = (float)(sw*sh);
	s2 = (float)sum2;
	upper = len*s12-s_s*s2;
	below = (len*s_ss-s_s*s_s)*(len*s22-s2*s2);
	return (upper<=0.0 || below<0.01)? 0.0 : upper*upper/below;
}

float	CPatMat::nematDownSearch_Angle(NEIMG *dimg,NEIMG *patimg,int level,int pixLevel,
			float minScore,int x,int y,float degree,SUBINFO subInfo) {
PBYTE	dst,src;
int	dmemw,dw,dh,smemw,sw,sh;
float	maxScore,score;
int	uMax,vMax;
float	aMax;
int	u,v,a;
float	s_s,s_ss;
float	da;
	// check
	if (level==pixLevel)
		return nematBottomSearch_Angle(dimg[pixLevel],patimg[pixLevel],pixLevel,x,y,degree,subInfo);
	// set
	dst = dimg[level]->bits;
	dmemw = dimg[level]->memw;
	dw = dimg[level]->width;
	dh = dimg[level]->height;
	src = patimg[level]->bits;
	smemw = patimg[level]->memw;
	sw = patimg[level]->width;
	sh = patimg[level]->height;
	da = patimg[level]->dAngle;
	s_s = patimg[level]->s;
	s_ss = patimg[level]->ss;
	// search max
	maxScore = -2.0;
	uMax = x;
	vMax = y;
	aMax = degree;
	for (a=-1; a<=1; a++)
	for (v=y-1; v<=y+2; v++)
	for (u=x-1; u<=x+2; u++) {
		score = nematCorr_Angle(src,smemw,sw,sh,s_s,s_ss,dst,dmemw,dw,dh,
		 u,v,degree+a*da);
		if (score>maxScore) {
			maxScore = score;
			uMax = u;
			vMax = v;
			aMax = degree+a*da;
		}
	}
	// decide recursion
	if (maxScore>=minScore)
		return nematDownSearch_Angle(dimg,patimg,level-1,pixLevel,minScore,
		 uMax<<1,vMax<<1,aMax,subInfo);
	// otherwise
	subInfo->level = level;
	subInfo->x = uMax;
	subInfo->y = vMax;
	subInfo->degree = aMax;
	return maxScore;
}

void	CPatMat::nematSubPixel_Angle(NEIMG img,NEIMG patimg,SUBINFO subInfo,
		float *rx,float *ry,float *degree) {
PBYTE	src,dst;
int	smemw,sw,sh,dmemw,dx,dy,dw,dh;
float	dangle,da;
float	s,ss;
float	d1,d2,under;
float	resX,resY,resMult;
	src = patimg->bits;
	smemw = patimg->memw;
	sw = patimg->width;
	sh = patimg->height;
	s = patimg->s;
	ss = patimg->ss;
	dst = img->bits;
	dmemw = img->memw;
	dx = subInfo->x;
	dy = subInfo->y;
	dangle = subInfo->degree;
	da = subInfo->da;
	dw = img->width;
	dh = img->height;
	if (subInfo->sl<-0.5)
		subInfo->sl = nematCorr_Angle(src,smemw,sw,sh,s,ss,
		 dst,dmemw,dw,dh,dx-1,dy,dangle);
	if (subInfo->st<-0.5)
		subInfo->st = nematCorr_Angle(src,smemw,sw,sh,s,ss,
		 dst,dmemw,dw,dh,dx,dy-1,dangle);
	if (subInfo->sr<-0.5)
		subInfo->sr = nematCorr_Angle(src,smemw,sw,sh,s,ss,
		 dst,dmemw,dw,dh,dx+1,dy,dangle);
	if (subInfo->sb<-0.5)
		subInfo->sb = nematCorr_Angle(src,smemw,sw,sh,s,ss,
		 dst,dmemw,dw,dh,dx,dy+1,dangle);
	if (subInfo->su<-0.5)
		subInfo->su = nematCorr_Angle(src,smemw,sw,sh,s,ss,
		 dst,dmemw,dw,dh,dx,dy,dangle-da);
	if (subInfo->sd<-0.5)
		subInfo->sd = nematCorr_Angle(src,smemw,sw,sh,s,ss,
		 dst,dmemw,dw,dh,dx,dy,dangle+da);
	d1 = subInfo->sm-subInfo->sl;
	d2 = subInfo->sr-subInfo->sm;
	if ((under=d1-d2)<0.0000001)
		resX = (float)dx;
	else
		resX = (float)dx-0.5-d2/under;
	d1 = subInfo->sm-subInfo->st;
	d2 = subInfo->sb-subInfo->sm;
	if ((under=d1-d2)<0.0000001)
		resY = (float)dy;
	else
		resY = (float)dy-0.5-d2/under;
	resMult = pow(2.0,subInfo->level);
	*rx = resMult*resX;
	*ry = resMult*resY;
	d1 = subInfo->sm-subInfo->su;
	d2 = subInfo->sd-subInfo->sm;
	if ((under=d1-d2)<0.0000001)
		*degree = dangle;
	else
		*degree = dangle-0.5-d2/under;
}

float	CPatMat::nematBottomSearch_Angle(NEIMG dimg,NEIMG patimg,int pixLevel,
		int x,int y,float degree,SUBINFO subInfo) {
PBYTE	dst,src;
int	dmemw,dw,dh,smemw,sw,sh;
float	s_s,s_ss;
float	map[5][5][5];
int	i,j,k,u,v;
float	a;
float	maxScore,score;
int	maxI,maxJ,maxK;
float	da;
	// set
	dst = dimg->bits;
	dmemw = dimg->memw;
	dw = dimg->width;
	dh = dimg->height;
	src = patimg->bits;
	smemw = patimg->memw;
	sw = patimg->width;
	sh = patimg->height;
	s_s = patimg->s;
	s_ss = patimg->ss;
	da = patimg->dAngle;
	// search max
	maxScore = -2.0;
	for (k=0; k<=4; k++) {
		for (i=0; i<=4; i++)
			map[i][0][k] = map[i][4][k] = -1.0;
		for (j=1; j<=3; j++)
			map[0][j][k] = map[4][j][k] = -1.0;
	}
	for (i=1; i<=3; i++)
		for (j=1; j<=3; j++)
			map[i][j][0] = map[i][j][4] = -1.0;
	for (k=1; k<=3; k++) {
		a = degree+(k-2)*da;
		for (j=1; j<=3; j++) {
			v = y+j-2;
			for (i=1; i<=3; i++) {
				u = x+i-2;
				score = map[i][j][k] = nematCorr_Angle(src,smemw,sw,sh,
				 s_s,s_ss,dst,dmemw,dw,dh,u,v,a);
				if (score>maxScore) {
					maxScore = score;
					maxI = i;
					maxJ = j;
					maxK = k;
				}
			}
		}
	}
	subInfo->level = pixLevel;
	subInfo->x = x+maxI-2;
	subInfo->y = y+maxJ-2;
	subInfo->degree = degree+(maxK-2)*da;
	subInfo->da = da;
	subInfo->sl = map[maxI-1][maxJ][maxK];
	subInfo->st = map[maxI][maxJ-1][maxK];
	subInfo->sr = map[maxI+1][maxJ][maxK];
	subInfo->sb = map[maxI][maxJ+1][maxK];
	subInfo->su = map[maxI][maxJ][maxK-1];
	subInfo->sd = map[maxI][maxJ][maxK+1];
	subInfo->sm = maxScore;
	return maxScore;
}

float	CPatMat::nematCos(float theta) {
static float	t0=0.0;
static float	val=1.0;
	if (t0==theta)
		return val;
	return (val=cos(t0=theta));
}

float	CPatMat::nematSin(float theta) {
static float	t0=0.0;
static float	val=0.0;
	if (t0==theta)
		return val;
	return (val=sin(t0=theta));
}


