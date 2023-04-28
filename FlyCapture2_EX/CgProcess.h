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

	// AutoThreshold ���ϴ� �Ϲ��Լ� 
	int FindAutoThreshold(LPBYTE fm, int x1, int y1, int x2, int y2, int pitch);

	// Binary �Լ��� 
	// White �����Ѵ�.
	int Dilate_Binary(LPBYTE fmSour, LPBYTE fmDest, int nWidth, int nHeight, int nPitch,
		int* pKernel, int nKernelSizeX, int nKernelSizeY);
	// White �����Ѵ�.
	int Erode_Binary(LPBYTE fmSour, LPBYTE fmDest, int nWidth, int nHeight, int nPitch,
		int* pKernel, int nKernelSizeX, int nKernelSizeY);
	// White �������� �ִ� ���� ������ ��������.( Dilate + Erode)
	int Close_Binary(LPBYTE fmSour, LPBYTE fmDest, int nWidth, int nHeight, int nPitch,
		int* pKernel, int nKernelSizeX, int nKernelSizeY);
	// White �ٱ��� ���� White���� ��������. (Erode + Dilate)
	int Open_Binary(LPBYTE fmSour, LPBYTE fmDest, int nWidth, int nHeight, int nPitch,
		int* pKernel, int nKernelSizeX, int nKernelSizeY);

	// Gray �Լ��� 
	// ȭ���� ��ü�� ��� ����.
	int Dilate_Gray(LPBYTE fmSour, LPBYTE fmDest, int nWidth, int nHeight, int nPitch,
		int* pKernel, int nKernelSizeX, int nKernelSizeY);
	// ȭ���� ��ü�� ��ο�����.
	int Erode_Gray(LPBYTE fmSour, LPBYTE fmDest, int nWidth, int nHeight, int nPitch,
		int* pKernel, int nKernelSizeX, int nKernelSizeY);
	// ������ ���� �κ��� �մ� ������� Smoothing�Ǵ� ȿ���� �ִ�
	// �� Black ����� ��������.
	int Close_Gray(LPBYTE fmSour, LPBYTE fmDest, int nWidth, int nHeight, int nPitch,
		int* pKernel, int nKernelSizeX, int nKernelSizeY);
	// ������ ����� ������ �մ� ������� Smoothing�Ǵ� ȿ���� �ִ�.
	// �� White ����� ��������.
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
	//  Edge ���ϴ� �Լ�                            2000. 2. 29  ���� 
	//
	/////////////////////////////////////////////////////////////////////////////
	//
	// [ ������� ]
	//
	//  INPUT
	//		n : �ȼ�����Ÿ �� 
	//		*data: �ȼ�����Ÿ 
	//  OUTPUT
	//		*edge: ���� Edge
	//		*slope: �̿��ϴ� �ȼ����� �� ��, ABS(data[i+1]-data[i]). slope�� ���밪�� ����Ѵ�.
	//		*dir=0: White->Black
	//			=1: Black->White
	//  INPUT
	//		LPBYTE: unsigned char* �Ǵ� int *
	//		fm    : �޸� ���۹��� 
	//		(x1,y1) : Line�� ������ǥ 
	//		(x2,y2) : Line�� ����ǥ        x1>x2 �Ǵ� x2>=x1, y1>y2 �Ǵ� y2>=y1
	//		nPitch  : �޸� �� ( M8, MVB03�ΰ�� 1024, ���� BMPó���ô� 640)
	//      (left, top) : ������ �»����ǥ 
	//      (right, bottom): ������ ���ϱ� ��ǥ  �ݵ��, right>left, bottom>top
	//  OUTPUT
	//		(*edgeX, *edgeY) : ���� Edge�� ��ǥ 
	//
	//
	//  �Լ� �̸��� ���ؼ� 
	//
	//  LN_  : �� Line�� ���� Edge���� 
	//  AR_  : �ѿ������� ���� Line�� ���� Edge�� ���� 
	//  _B2W : Black���� White�� ���ϴ� Edge���� 
	//  _W2B : White���� Black���� ���ϴ� Edge���� 
	//  _ABS : Black->White, �Ǵ� White->Black �������� ���Ǹ� ������ Edge���� 
	//  _T2B : Top���� Bottom���� Pixel Data�� �� Edge�� ����
	//  _L2R : Left���� Right�� Pixel Data�� �� Edge�� ���� 
	//  _B2T : bottom ���� Top �������� Pixel Data�� �� Edge�� ���� 
	//  _R2L : Right���� Left�� Pixel Data�� ��� Edge�� ���� 
	//
	// (��) LN_FindEdge1_B2W : �� Line Data�� ���ؼ� Black->White�� ���ϴ� ���Ⱑ ����ū 1���� Edge���� 
	//                         �̶� ���� �����  *edge �̴�.
	//      LN_FindEdgePos1_W2B: �� Line�� ���������� ����Ͽ� �������� White->Black���� ���ϴ� Edge��
	//                         ���Ⱑ ����ū 1���� Edge���� 
	//                         ���� ����� (*edgeX, *edgeY)�̴�.
	//   
	//  
	// return >0 SUCCESS (Edge Number)
	//        <0 FAILURE 


	// Edge�� 1���� ���ϴ� �Լ�-----------------------------------------------------------------

	// n ���� Pixel data�κ��� ���Ⱑ ���� ū Edge�� ���ϴ� �Լ� 
	// B2W : Black->White�� ���ϴ� Edge�߿��� ���� 
	// W2B : White->Black�� ���ϴ� Edge�߿��� ����
	// ASB : ���Ⱑ ����ū Edge���� 
	int LN_FindEdge1_B2W(int n, int *data, double *edge, int *slope);
	int LN_FindEdge1_W2B(int n, int *data, double *edge, int *slope);

	// ������(x1,y1)-����(x2,y2) ����  ���Ⱑ ���� ū Edge�� ���ϴ� �Լ� 
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
	//      Local Functions( FindEdge.h�� �ִ� �Լ��� ���ο��� ����ϴ� Subroutine��)
	//
	//////////////////////////////////////////////////////////////////////////////////////////
	//
	// (x1,y1)-(x2,y2)�� 1Pixel�������� Sampling�ϴ� �Լ� 
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
