#pragma once

#define MINDATA_COUNT    5
#define MAXEDGENUMBER    100

class CgProcess
{
public:
	CgProcess(void);
	~CgProcess(void);
	void ImageReduce2(unsigned char* fmSrc,unsigned char* fmDest,int nSrcW,int nSrcH,int nScale);
	void ImageReduce(unsigned char* fmSrc,unsigned char* fmDest,int nSrcW,int nSrcH,int nScale);
	void ImageBin(unsigned char* fmSrc,unsigned char* fmDest,int nSrcW,int nSrcH,double nTh);
	double ImageProjection(unsigned char* fm,int nWidth,int nHeight,CRect rt,BOOL bDirIsX,BOOL bIsCount,double* pHisto);
	void ImageRotate(unsigned char* fmSrc,unsigned char* fmDest,int nSrcW,int nSrcH,int nAngle);
	void ImageSelectCopy(unsigned char* fmSrc,unsigned char* fmDest,int nSrcW,int nSrcH,CRect rtSrc,int nDestW,int nDestH);
	void ImageSelectCopy(unsigned char* fmSrc,unsigned char* fmDest,int nSrcW,int nSrcH,CRect rtSrc,int nDestW,int nDestH,int nDestSttX,int nDestSttY,int nn);
	void CropImageSelectCopy(unsigned char* fmSrc,unsigned char* fmDest,int nSrcW,int nSrcH,CRect rtSrc,int nDestW,int nDestH);
	void ImageResizeNearest(unsigned char* fmSrc,unsigned char* fmDest,int nSrcW,int nSrcH,int nDestW,int nDestH);
	void ImageFlip(unsigned char* fmSrc,unsigned char* fmDest,int nWidth,int nHeight,BOOL bIsX);
	void ImageReduce1(unsigned char* fmSrc,unsigned char* fmDest,int nSrcW,int nSrcH,int nScale);
	int Dilate_BinaryMini(LPBYTE fmSour, LPBYTE fmDest, int nWidth, int nHeight, int nPitch);

	// AutoThreshold 구하는 일반함수 
	int FindAutoThreshold(LPBYTE fm, int x1, int y1, int x2, int y2, int pitch);

	// Binary 함수들 
	// White 증가한다.
	int Dilate_Binary(LPBYTE fmSour, LPBYTE fmDest, int nWidth, int nHeight, int nPitch,
		int* pKernel, int nKernelSizeX, int nKernelSizeY);
	// White 감소한다.
	int Erode_Binary(LPBYTE fmSour, LPBYTE fmDest, int nWidth, int nHeight, int nPitch,
		int* pKernel, int nKernelSizeX, int nKernelSizeY);
	// White 영역내에 있는 검은 점들이 없어진다.( Dilate + Erode)
	int Close_Binary(LPBYTE fmSour, LPBYTE fmDest, int nWidth, int nHeight, int nPitch,
		int* pKernel, int nKernelSizeX, int nKernelSizeY);
	// White 바깥에 붙은 White들이 없어진다. (Erode + Dilate)
	int Open_Binary(LPBYTE fmSour, LPBYTE fmDest, int nWidth, int nHeight, int nPitch,
		int* pKernel, int nKernelSizeX, int nKernelSizeY);

	// Gray 함수들 
	// 화면이 대체로 밝아 진다.
	int Dilate_Gray(LPBYTE fmSour, LPBYTE fmDest, int nWidth, int nHeight, int nPitch,
		int* pKernel, int nKernelSizeX, int nKernelSizeY);
	// 화면이 대체로 어두워진다.
	int Erode_Gray(LPBYTE fmSour, LPBYTE fmDest, int nWidth, int nHeight, int nPitch,
		int* pKernel, int nKernelSizeX, int nKernelSizeY);
	// 영상의 밝은 부분을 잇는 평면으로 Smoothing되는 효과가 있다
	// 즉 Black 노이즈가 없어진다.
	int Close_Gray(LPBYTE fmSour, LPBYTE fmDest, int nWidth, int nHeight, int nPitch,
		int* pKernel, int nKernelSizeX, int nKernelSizeY);
	// 영상의 어두은 점들을 잇는 평면으로 Smoothing되는 효과가 있다.
	// 즉 White 노이즈가 없어진다.
	int Open_Gray(LPBYTE fmSour, LPBYTE fmDest, int nWidth, int nHeight, int nPitch,
		int* pKernel, int nKernelSizeX, int nKernelSizeY);

	int FindCircleCenter(LPBYTE fm, int left, int top, int right, int bottom, int nPitch, int nSlope, int nSkip,
		double *Cx, double *Cy);
	int FindCircleCenter1(LPBYTE fm, int left, int top, int right, int bottom, int nPitch, int nSlope, int nSkip,
		double *Cx, double *Cy);

	int CircleFittingCenter(LPBYTE fm, int left, int top, int right, int bottom, int nPitch, int nObjectColor, int nSlope,
		double *Cx, double *Cy, double *Diameter);
	int FindCircleEdge(LPBYTE fm, int nPitch, int Cx, int Cy, int nR, int nObjectColor, int nSlope, double *pEdgeX, double *pEdgeY);



	int HoughCircle(LPBYTE fm, int left, int top, int right, int bottom, int nPitch,
		int nObjectColor, int nSlope, int nR1, int nR2,
		double *Cx, double *Cy);




	int FindTransformData(double OldXY[][2], double NewXY[][2], 
		double *orgx, double *orgy, double *angle, double *dx, double *dy);
	int PosTransform(double orgx, double orgy, double theta, double dx, double dy, 
		double x, double y, double *x_, double *y_);

	int matrixInverse(double *a,int n,double *b);


	//////////////////////////////////////////////////////////////////////////////
	// 
	//  Edge 구하는 함수                            2000. 2. 29  고영우 
	//
	/////////////////////////////////////////////////////////////////////////////
	//
	// [ 용어정리 ]
	//
	//  INPUT
	//		n : 픽셀데이타 수 
	//		*data: 픽셀데이타 
	//  OUTPUT
	//		*edge: 구한 Edge
	//		*slope: 이웃하는 픽셀간의 차 즉, ABS(data[i+1]-data[i]). slope는 절대값을 사용한다.
	//		*dir=0: White->Black
	//			=1: Black->White
	//  INPUT
	//		LPBYTE: unsigned char* 또는 int *
	//		fm    : 메모리 시작번지 
	//		(x1,y1) : Line의 시작좌표 
	//		(x2,y2) : Line의 끝좌표        x1>x2 또는 x2>=x1, y1>y2 또는 y2>=y1
	//		nPitch  : 메모리 폭 ( M8, MVB03인경우 1024, 보통 BMP처리시는 640)
	//      (left, top) : 영역의 좌상귀좌표 
	//      (right, bottom): 영역의 우하귀 좌표  반드시, right>left, bottom>top
	//  OUTPUT
	//		(*edgeX, *edgeY) : 구한 Edge의 좌표 
	//
	//
	//  함수 이름에 대해서 
	//
	//  LN_  : 한 Line에 대해 Edge구함 
	//  AR_  : 한영역에서 여러 Line에 대해 Edge를 구함 
	//  _B2W : Black에서 White로 변하는 Edge구함 
	//  _W2B : White에서 Black으로 변하는 Edge구함 
	//  _ABS : Black->White, 또는 White->Black 구별없이 조건만 맞으로 Edge구함 
	//  _T2B : Top에서 Bottom으로 Pixel Data를 얻어서 Edge를 구함
	//  _L2R : Left에서 Right로 Pixel Data를 얻어서 Edge를 구함 
	//  _B2T : bottom 에서 Top 방향으로 Pixel Data를 얻어서 Edge를 구함 
	//  _R2L : Right에서 Left로 Pixel Data를 얻어 Edge를 구함 
	//
	// (예) LN_FindEdge1_B2W : 한 Line Data에 대해서 Black->White로 변하는 기울기가 가장큰 1개의 Edge구함 
	//                         이때 구한 결과는  *edge 이다.
	//      LN_FindEdgePos1_W2B: 한 Line의 시작점에서 출발하여 끝점까지 White->Black으로 변하는 Edge중
	//                         기울기가 가장큰 1개의 Edge구함 
	//                         구한 결과는 (*edgeX, *edgeY)이다.
	//   
	//  
	// return >0 SUCCESS (Edge Number)
	//        <0 FAILURE 


	// Edge를 1개만 구하는 함수-----------------------------------------------------------------

	// n 개의 Pixel data로부터 기울기가 가장 큰 Edge를 구하는 함수 
	// B2W : Black->White로 변하는 Edge중에서 구함 
	// W2B : White->Black로 변하는 Edge중에서 구함
	// ASB : 기울기가 가장큰 Edge구함 
	int LN_FindEdge1_B2W(int n, int *data, double *edge, int *slope);
	int LN_FindEdge1_W2B(int n, int *data, double *edge, int *slope);

	// 시작점(x1,y1)-끝점(x2,y2) 에서  기울기가 가장 큰 Edge를 구하는 함수 
	int LN_FindEdgePos1_B2W(LPBYTE fm, int x1, int y1, int x2, int y2, int nPitch,
		double *edgeX, double *edgeY, int *slope);
	int LN_FindEdgePos1_W2B(LPBYTE fm, int x1, int y1, int x2, int y2, int nPitch,
		double *edgeX, double *edgeY, int *slope);


	int LineFitting(int dataN, double *x, double *y, int removeN, double ErrorLimit,
		double *t, double *a, double *b);

	int LineFitting2(int dataN, double *x, double *y, int removeN, double ErrorLimit,
		double *t, double *a, double *b);


	//////////////////////////////////////////////////////////////////////////////////////////
	//
	//      Local Functions( FindEdge.h에 있는 함수들 내부에서 사용하는 Subroutine들)
	//
	//////////////////////////////////////////////////////////////////////////////////////////
	//
	// (x1,y1)-(x2,y2)를 1Pixel간격으로 Sampling하는 함수 
	int GetLineData(LPBYTE fm, int x1, int y1, int x2, int y2, int nPitch,
		double *pX, double *pY, int *pGrayValue);
	int ChooseGoodData(int N, double *x, int HISTO_SPACE);
	double FN_GetCenter(int N, double *x);

	//////////////////////////////////////////////////////////////////////////////////////////

	double PixelX[800];
	double PixelY[800];
	int   PixelV[800];
	double Edge[640];
	double pX[MAXEDGENUMBER];
	double pY[MAXEDGENUMBER];
	int histo[200];



private:
	int m_Black;
	int m_White;
};
