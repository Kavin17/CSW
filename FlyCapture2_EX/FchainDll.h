//Blob�� �ϱ����� �ݵ�� �ѹ� Call �ؾ� �Ѵ�.
//nMaxChainNum : �ִ� Blob�� ���� �����ϴ� �Ķ�����̴� ���� 100 ���� �д�.
//nTotalLength : ��� Blob�� Perimeter�� ���Ѱ� ���� ũ�� ������ �ȴ� ���� 10000
//return : 0 SUCCESS
//       : 1 FAILURE
extern "C" int PASCAL EXPORT  FChain_Init(int nMaxChainNum, int nTotalLength);

//Blob ������ �ݵ�� Call�ؾ� �Ѵ�. �� �Լ��� Call���� ������ �޸𸮰� Free �� �ȵǹǷ� Memory Leak�߻�
extern "C" void PASCAL EXPORT  FChain_Close();

//FChain_FaseChain�ϱ����� Call������Ѵ�. 
//object : Object Color 0(Black) 1(White)
//fm     : �޸� ���۹���
//skip   : ������ ���� ū �� ���� 10�̻�
//minboundary : blob�� Perimeter�� �̰����� ������ �����Ѵ�.
//maxboundary : blob�� Perimeter�� �̰����� ũ�� �����Ѵ�.
//pitch   : Image ��ġ 
//height  : Image ��ü ����
extern "C" void PASCAL EXPORT m_pChain->SetChainData(int object, LPBYTE fm, int skipx, int skipy, int minboundary,
								   long maxboundary, int pitch, int height);

//������ ��⸦ �����ϸ� FastChain�Ҷ� ��踦 �ش��ϴ� Color�� ä���.
extern "C" int PASCAL EXPORT  FChain_SetBoundaryColor(int color);

//Blob�� ã�� �������� Blob�� ã�´�.
//return : ã�� Blob�Ǽ� 
extern "C" int PASCAL EXPORT  m_pChain->FastChain(int left, int top, int right, int bottom);

//N��° Blob�� �߽��� ���Ѵ�.
extern "C" void PASCAL EXPORT m_pChain->Chain_Center(int N, double *cx, double *cy);

//N��° Blob�� ��踸 ������ �߽��� ���Ѵ�.
extern "C" void PASCAL EXPORT m_pChain->Chain_CenterWithBoundary(int N, double *cx, double *cy);

//N��° Blob�� ������ ���Ѵ�.
extern "C" void PASCAL EXPORT FChain_Area(int N, double *area);

//N��° Blob�� �ֺ����̸� ���Ѵ�. 
extern "C" void PASCAL EXPORT FChain_Perimeter(int N, double *perimeter);

//N��° Blob�� X�ִ밪 
extern "C" int PASCAL EXPORT  m_pChain->FindMaxX(int N);

//N��° Blob�� X�ּҰ� 
extern "C" int PASCAL EXPORT  m_pChain->FindMinX(int N);

//N��° Blob�� Y�ִ밪 
extern "C" int PASCAL EXPORT  m_pChain->FindMaxY(int N);

//N��° Blob�� Y�ּҰ� 
extern "C" int PASCAL EXPORT  m_pChain->FindMinY(int N);

//N��° Blob�� �߽ɿ� ���� Moment
extern "C" void PASCAL EXPORT FChain_FindMoment(int N, double *moment);

//N��° Blob�� (Cx, Cy)�� ���� moment
extern "C" void PASCAL EXPORT FChain_FindMomentWithCenter(int N, double Cx, double Cy, double *moment);

////N��° Blob�� Compactness
extern "C" void PASCAL EXPORT FChain_FindCompactness(int N, double *compactness);

//N��° Blob�� ������ ���� 
extern "C" void PASCAL EXPORT FChain_FindAngle(int N, double *angle);

//N��° Blob�� ��(Cx, Cy)�κ��� ���� ª���Ÿ�, ��Ÿ�, ��հŸ��� ���Ѵ�.
extern "C" void PASCAL EXPORT m_pChain->FineDistFromPoint(int N, double Cx,double Cy, double *min, double *max, double *avg);

//N��° Blob�� YValue���� ���� X �� �߿��� ���� ������ X��ǥ�� ���� �������� X��ǥ�� ���Ѵ�.
extern "C" void PASCAL EXPORT FChain_LeftRightBoundary(int N, int YValue, int *left, int *right);

//N��° Blob�� ������ �������� �ּ� �簢���� ���Ѵ�.
// (Cx, Cy) Blob �߽�
// angle : ���ఢ�� 
//
//		*[0]
//	   *   *		
//	  * [1]  *[3]
//      *   *
//		   *[2]
//
//
extern "C" void  PASCAL EXPORT FChain_FindMinRect(int N, double Cx, double Cy, double angle,  double pos[][2]);

//N��° Blob�� Chain������ ��´�.
//��뿹 
// int nLength, *Pixel;
// N=3;
// FChain_GetChain(N, &nLength, Pixel);
// nLength: N��° Blob�� Chain ����
// ChainData: N��° Blob�� Chain �ȼ�����Ÿ
// i=0~(nLength-1) ���� 
// Pixel[2*i] : X��ǥ
// Pixel[2*i+1]: Y��ǥ 
extern "C" void  PASCAL EXPORT FChain_GetChain(int N, int *ChainLength, int **ChainData);

// �ڵ����� Threshold ���ϴ� �Ϲ��Լ� 
// fm : ������ �޸� ���۹���
// x1,y1,x2,y2 : ��������
// pitch : �����Ӹ޸��� x���� ���� ��)MVB03�� 1024, ������ Bitmap File�� �� ������ Width
//
// Return   -1   Error : x2, y2 �� x1, y1���� Ŀ����.
//           -2   Error : ������ �Ѱ��� ������ ������ �ִ�.
//			�׿� ���� Threshold��                             
//
//
extern "C" int PASCAL EXPORT FChain_AutoThreshold(unsigned char* fm, int x1, int y1, int x2, int y2, int pitch);
