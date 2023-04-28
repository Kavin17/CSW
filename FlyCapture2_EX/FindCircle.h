
int FindCircleEdge(LPBYTE fm, int nPitch,int nHeight, int Cx, int Cy, int nR, int nObjectColor, int nSlope, double *pEdgeX, double *pEdgeY);

int FindCircleEdge2(LPBYTE fm, int nPitch,int nHeight, int Cx, int Cy, int nSttR,int nEndR, int nEdgeNum,int nObjectColor, int nSlope, double *pEdgeX, double *pEdgeY,bool bInsType,int subpitch,int nCam,int nLimmit);



/*
int FindCircleCenter(LPBYTE fm, int left, int top, int right, int bottom, int nPitch, int nSlope, int nSkip,
					 double *Cx, double *Cy);


int CircleFittingCenter(LPBYTE fm, int left, int top, int right, int bottom, int nPitch, int nObjectColor, int nSlope,
					   double *Cx, double *Cy, double *Diameter);


int HoughCircle(LPBYTE fm, int left, int top, int right, int bottom, int nPitch,
				int nObjectColor, int nSlope, int nR1, int nR2,
				double *Cx, double *Cy);




int FindTransformData(double OldXY[][2], double NewXY[][2], 
					  double *orgx, double *orgy, double *angle, double *dx, double *dy);
int PosTransform(double orgx, double orgy, double theta, double dx, double dy, 
				 double x, double y, double *x_, double *y_);

int matrixInverse(double *a,int n,double *b);
*/

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


// n ���� Pixel data�κ��� ���Ⱑ ���� ū Edge�� ���ϴ� �Լ� 
// B2W : Black->White�� ���ϴ� Edge�߿��� ���� 
// W2B : White->Black�� ���ϴ� Edge�߿��� ����
// ASB : ���Ⱑ ����ū Edge���� 
int LN_FindEdge1_B2W(int n, int *data, double *edge, int *slope,int subpitch=1);
int LN_FindEdge1_W2B(int n, int *data, double *edge, int *slope,int subpitch=1);
int LN_FindEdge1_ABS(int n, int *data, double *edge, int *slope, int *dir,int subpitch=1);

// ������(x1,y1)-����(x2,y2) ����  ���Ⱑ ���� ū Edge�� ���ϴ� �Լ� 
int LN_FindEdgePos1_B2W(LPBYTE fm, int x1, int y1, int x2, int y2, int nPitch,int nHeight,
						double *edgeX, double *edgeY, int *slope,int subpitch=1);
int LN_FindEdgePos1_W2B(LPBYTE fm, int x1, int y1, int x2, int y2, int nPitch,int nHeight,
						double *edgeX, double *edgeY, int *slope,int subpitch=1);
int LN_FindEdgePos1_ABS(LPBYTE fm, int x1, int y1, int x2, int y2, int nPitch,int nHeight,
						double *edgeX, double *edgeY, int *slope, int *dir,int subpitch=1);

//���Ⱑ slope���� ū ù��° Edge�� 1�� ���ϴ� �Լ�
int LN_FindEdge1st_B2W(int n, int *data, int slope, double *edge,int subpitch=1,int nLimmit=256);
int LN_FindEdge1st_W2B(int n, int *data, int slope, double *edge,int subpitch=1);
int LN_FindEdge1st_ABS(int n, int *data, int slope, double *edge, int *dir,int subpitch=1);

// ������(x1,y1)-����(x2,y2) ���� ���Ⱑ  slope���� ū ù��° Edge�� ���ϴ� �Լ� 
int LN_FindEdgePos1st_B2W(LPBYTE fm, int x1, int y1, int x2, int y2, int nPitch,int nHeight, int slope,
						  double *edgeX, double *edgeY,int subpitch=1,int nLimmit=256);
int LN_FindEdgePos1st_W2B(LPBYTE fm, int x1, int y1, int x2, int y2, int nPitch,int nHeight, int slope,
						  double *edgeX, double *edgeY,int subpitch=1);
int LN_FindEdgePos1st_ABS(LPBYTE fm, int x1, int y1, int x2, int y2, int nPitch,int nHeight, int slope,
						  double *edgeX, double *edgeY, int *dir,int subpitch=1);
//------------------------------------------------------------------------------------------


// Edge�� ������ ���ϴ� �Լ� ---------------------------------------------------------------
// n ���� Pixel data�κ��� ���Ⱑ slope���� ū ��� Edge�� ���ϴ� �Լ� 
int LN_FindEdgeM_B2W(int n, int *data, int slope, double *edge);
int LN_FindEdgeM_W2B(int n, int *data, int slope, double *edge);
int LN_FindEdgeM_ABS(int n, int *data, int slope, double *edge, int *dir);

// ������(x1,y1)-����(x2,y2) ���� ���Ⱑ  slope���� ū ��� Edge�� ���ϴ� �Լ� 
int LN_FindEdgePosM_B2W(LPBYTE fm, int x1, int y1, int x2, int y2, int nPitch,int nHeight, int slope,
						double *edgeX, double *edgeY);
int LN_FindEdgePosM_W2B(LPBYTE fm, int x1, int y1, int x2, int y2, int nPitch,int nHeight, int slope,
						double *edgeX, double *edgeY);
int LN_FindEdgePosM_ABS(LPBYTE fm, int x1, int y1, int x2, int y2, int nPitch,int nHeight, int slope,
						double *edgeX, double *edgeY, int *dir);
//------------------------------------------------------------------------------------------



// AR_FindEdge_T2B_B2W�� ���ε�� �����ϸ�... 
// �Ʒ� �׸����� �� ���������� Edge�� �ϳ��� ���Ͽ� �� Edge�� Line Fitting�Ѵ�.
// Fitting�� Line�� ������ �߽��� ������ �������� ������ ���� �� ������ Edge Point�� �����Ѵ�.
//
//    _____
//   |  |  |     .... : �������� ���� ���� Edge��
//   |  |  |       *  : �� Edge�� �մ� Line�� ������ �߽� ���������� ������ �� 
//   |..*..|    
//   |  |  |
//   |__|__|
//  

int AR_FindEdge_T2B_B2W(LPBYTE fm, int left, int top, int right, int bottom, int nPitch,int nHeight,
						double *edgeX, double *edgeY);
int AR_FindEdge_T2B_W2B(LPBYTE fm, int left, int top, int right, int bottom, int nPitch,int nHeight,
						double *edgeX, double *edgeY);
int AR_FindEdge_L2R_B2W(LPBYTE fm, int left, int top, int right, int bottom, int nPitch,int nHeight,
						double *edgeX, double *edgeY);
int AR_FindEdge_L2R_W2B(LPBYTE fm, int left, int top, int right, int bottom, int nPitch,int nHeight,
						double *edgeX, double *edgeY);
int AR_FindEdge_B2T_B2W(LPBYTE fm, int left, int top, int right, int bottom, int nPitch,int nHeight,
						double *edgeX, double *edgeY);
int AR_FindEdge_B2T_W2B(LPBYTE fm, int left, int top, int right, int bottom, int nPitch,int nHeight,
						double *edgeX, double *edgeY);
int AR_FindEdge_R2L_B2W(LPBYTE fm, int left, int top, int right, int bottom, int nPitch,int nHeight,
						double *edgeX, double *edgeY);
int AR_FindEdge_R2L_W2B(LPBYTE fm, int left, int top, int right, int bottom, int nPitch,int nHeight,
						double *edgeX, double *edgeY);


// �Լ����� : ������ ������ 1Pixel �������� ReSampling���ִ� �Լ� 
// return Value : ���� ��ǥ�� ����       
//               <0 FAILURE
int GetLineData(LPBYTE fm, int x1, int y1, int x2, int y2, int nPitch,int nHeight,
				double *pX, double *pY, int *pGrayValue);


//Line�� ���� ������ �ƴҶ� ����ϴ� Line Fitting�Լ� 
int LineFitting(int dataN, double *x, double *y, int removeN, double ErrorLimit,
				double *t, double *a, double *b);
//Line�� ���� �����϶� ����ϴ� Line Fitting�Լ�
int LineFitting1(int dataN, double *x, double *y, int removeN, double ErrorLimit,
				 double *t, double *a, double *b);


int SurfaceFitting(int nData, double *x, double *y, double *z, int nRemove, double dErrorLmt,
				   double *a, double *b, double *c);
