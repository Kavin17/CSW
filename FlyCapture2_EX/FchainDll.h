//Blob을 하기전에 반드시 한번 Call 해야 한다.
//nMaxChainNum : 최대 Blob의 수를 지정하는 파라미터이다 보통 100 정도 둔다.
//nTotalLength : 모든 Blob의 Perimeter를 합한값 보다 크게 잡으면 된다 보통 10000
//return : 0 SUCCESS
//       : 1 FAILURE
extern "C" int PASCAL EXPORT  FChain_Init(int nMaxChainNum, int nTotalLength);

//Blob 끝나면 반드시 Call해야 한다. 이 함수를 Call하지 않으면 메모리가 Free 가 안되므로 Memory Leak발생
extern "C" void PASCAL EXPORT  FChain_Close();

//FChain_FaseChain하기전에 Call해줘야한다. 
//object : Object Color 0(Black) 1(White)
//fm     : 메모리 시작번지
//skip   : 노이즈 보다 큰 값 보통 10이상
//minboundary : blob의 Perimeter가 이값보다 작으면 무시한다.
//maxboundary : blob의 Perimeter가 이값보다 크면 무시한다.
//pitch   : Image 피치 
//height  : Image 전체 높이
extern "C" void PASCAL EXPORT m_pChain->SetChainData(int object, LPBYTE fm, int skipx, int skipy, int minboundary,
								   long maxboundary, int pitch, int height);

//경계색의 밝기를 지정하면 FastChain할때 경계를 해당하는 Color로 채운다.
extern "C" int PASCAL EXPORT  FChain_SetBoundaryColor(int color);

//Blob을 찾을 영역에서 Blob을 찾는다.
//return : 찾은 Blob의수 
extern "C" int PASCAL EXPORT  m_pChain->FastChain(int left, int top, int right, int bottom);

//N번째 Blob의 중심을 구한다.
extern "C" void PASCAL EXPORT m_pChain->Chain_Center(int N, double *cx, double *cy);

//N번째 Blob의 경계만 가지고 중심을 구한다.
extern "C" void PASCAL EXPORT m_pChain->Chain_CenterWithBoundary(int N, double *cx, double *cy);

//N번째 Blob의 면적을 구한다.
extern "C" void PASCAL EXPORT FChain_Area(int N, double *area);

//N번째 Blob의 주변길이를 구한다. 
extern "C" void PASCAL EXPORT FChain_Perimeter(int N, double *perimeter);

//N번째 Blob의 X최대값 
extern "C" int PASCAL EXPORT  m_pChain->FindMaxX(int N);

//N번째 Blob의 X최소값 
extern "C" int PASCAL EXPORT  m_pChain->FindMinX(int N);

//N번째 Blob의 Y최대값 
extern "C" int PASCAL EXPORT  m_pChain->FindMaxY(int N);

//N번째 Blob의 Y최소값 
extern "C" int PASCAL EXPORT  m_pChain->FindMinY(int N);

//N번째 Blob의 중심에 대한 Moment
extern "C" void PASCAL EXPORT FChain_FindMoment(int N, double *moment);

//N번째 Blob의 (Cx, Cy)에 대한 moment
extern "C" void PASCAL EXPORT FChain_FindMomentWithCenter(int N, double Cx, double Cy, double *moment);

////N번째 Blob의 Compactness
extern "C" void PASCAL EXPORT FChain_FindCompactness(int N, double *compactness);

//N번째 Blob의 주죽의 각도 
extern "C" void PASCAL EXPORT FChain_FindAngle(int N, double *angle);

//N번째 Blob의 점(Cx, Cy)로부터 가장 짧은거리, 긴거리, 평균거리를 구한다.
extern "C" void PASCAL EXPORT m_pChain->FineDistFromPoint(int N, double Cx,double Cy, double *min, double *max, double *avg);

//N번째 Blob의 YValue값을 갖는 X 점 중에서 가장 왼쪽의 X좌표와 가장 오른쪽의 X좌표를 구한다.
extern "C" void PASCAL EXPORT FChain_LeftRightBoundary(int N, int YValue, int *left, int *right);

//N번째 Blob의 주축을 기준으로 최소 사각형울 구한다.
// (Cx, Cy) Blob 중심
// angle : 주축각도 
//
//		*[0]
//	   *   *		
//	  * [1]  *[3]
//      *   *
//		   *[2]
//
//
extern "C" void  PASCAL EXPORT FChain_FindMinRect(int N, double Cx, double Cy, double angle,  double pos[][2]);

//N번째 Blob의 Chain정보를 얻는다.
//사용예 
// int nLength, *Pixel;
// N=3;
// FChain_GetChain(N, &nLength, Pixel);
// nLength: N번째 Blob의 Chain 길이
// ChainData: N번째 Blob의 Chain 픽셀데이타
// i=0~(nLength-1) 까지 
// Pixel[2*i] : X좌표
// Pixel[2*i+1]: Y좌표 
extern "C" void  PASCAL EXPORT FChain_GetChain(int N, int *ChainLength, int **ChainData);

// 자동으로 Threshold 구하는 일반함수 
// fm : 프레임 메모리 시작번지
// x1,y1,x2,y2 : 영역정보
// pitch : 프레임메모리의 x방향 길이 예)MVB03은 1024, 보통의 Bitmap File은 그 파일의 Width
//
// Return   -1   Error : x2, y2 가 x1, y1보다 커야함.
//           -2   Error : 영상이 한가지 색만을 가지고 있다.
//			그외 구한 Threshold값                             
//
//
extern "C" int PASCAL EXPORT FChain_AutoThreshold(unsigned char* fm, int x1, int y1, int x2, int y2, int pitch);
