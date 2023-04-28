#include "stdafx.h"
#include "FindCircle.h"
#include "math.h"



//////////////////////////////////////////////////////////////////////////////////////////
//
//      Local Functions( FindEdge.h에 있는 함수들 내부에서 사용하는 Subroutine들)
//
//////////////////////////////////////////////////////////////////////////////////////////
//
// (x1,y1)-(x2,y2)를 1Pixel간격으로 Sampling하는 함수 
//int GetLineData(LPBYTE fm, int x1, int y1, int x2, int y2, int nPitch,
//				double *pX, double *pY, int *pGrayValue);
// 두개의 직선으로부터 Cross하는 점 구하기
int FindCrossPoint(double t1, double a1, double b1,
	  			   double t2, double a2, double b2, double *cx, double *cy);
//////////////////////////////////////////////////////////////////////////////////////////

#define MAXEDGENUMBER   100    //한 Line에서 구할수 있는 최대 Edge의 수 (임의로 증가가능)
// double PixelX[1280], PixelY[1280];
// double EdgeM[MAXEDGENUMBER];
// int   PixelV[1280];

//////////////////////////////////////////////////////////////////////////////////////////


int LN_FindEdge1_B2W(int n, int *data, double *edge, int *slope,int subpitch)
{
	int i;
	int maxslope, maxpos;
	double Temp_A, Temp_B;
	
	if(n<2)	return -1;

	maxslope=maxpos=0;
	for(i=subpitch;i<n-subpitch;i++)
		if( (data[i+subpitch]-data[i-subpitch])> maxslope)
		{
			maxpos=i;
			maxslope=(data[i+subpitch]-data[i-subpitch]);
		}
		
	if(maxslope==0) return -2;

	*slope=abs(maxslope);
	if(maxpos<=subpitch+1 || maxpos>=(n-subpitch-2))
	{
		*edge=(double)maxpos;
	}
	else
	{
		if(fabs((double)data[maxpos+subpitch+1]-data[maxpos]) < fabs((double)data[maxpos]-data[maxpos-subpitch-1]))
		{
			maxpos-=1;
		}

		Temp_A=(double)fabs((double)data[maxpos+2]+data[maxpos-2]-2*data[maxpos]);
		Temp_B=(double)fabs((double)data[maxpos+3]+data[maxpos-1]-2*data[maxpos+1]);

		if((Temp_A+Temp_B)!=0)
			*edge=(double)maxpos+Temp_A/(Temp_A+Temp_B);	
		else
			*edge=(double)maxpos;
	}

	return 1;
}

int LN_FindEdge1_W2B(int n, int *data, double *edge, int *slope,int subpitch)
{
	int i;
	int maxslope, maxpos;
	double Temp_A, Temp_B;
	
	if(n<2)	return -1;

	maxslope=maxpos=0;
	for(i=subpitch;i<n-subpitch;i++)
		if( (data[i+subpitch]-data[i-subpitch])< maxslope)
		{
			maxpos=i;
			maxslope=(data[i+subpitch]-data[i-subpitch]);
		}
		
	if(maxslope==0) return -2;

	*slope=abs(maxslope);
	if(maxpos<=subpitch+1 || maxpos>=(n-subpitch-2))
	{
		*edge=(double)maxpos;
	}
	else
	{
		if(fabs((double)data[maxpos+subpitch+1]-data[maxpos]) < fabs((double)data[maxpos]-data[maxpos-subpitch-1]))
		{
			maxpos-=1;
		}

		Temp_A=(double)fabs((double)data[maxpos+2]+data[maxpos-2]-2*data[maxpos]);
		Temp_B=(double)fabs((double)data[maxpos+3]+data[maxpos-1]-2*data[maxpos+1]);


		if((Temp_A+Temp_B)!=0)
			*edge=(double)maxpos+Temp_A/(Temp_A+Temp_B);	
		else
			*edge=(double)maxpos;
	}

	return 1;
}

int LN_FindEdge1_ABS(int n, int *data, double *edge, int *slope, int *dir,int subpitch)
{
	int i;
	int maxslope, maxpos;
	double Temp_A, Temp_B;
	
	if(n<2)	return -1;

	maxslope=maxpos=0;
	for(i=1;i<n-1;i++)
		if( abs(data[i+1]-data[i-1])> maxslope)
		{
			maxpos=i;
			maxslope=abs(data[i+1]-data[i-1]);
		}
		
	if(maxslope==0) return -2;

	*slope=abs(maxslope);
	
	if(data[maxpos+1]>data[maxpos]) *dir=1; //Black->White
	else                            *dir=0;	//White->Black

	if(maxpos<=2 || maxpos>=(n-3))
	{
		*edge=(double)maxpos;
	}
	else
	{
		if(fabs((double)data[maxpos+2]-data[maxpos]) < fabs((double)data[maxpos]-data[maxpos-2]))
		{
			maxpos-=1;
		}

		Temp_A=(double)fabs((double)data[maxpos+2]+data[maxpos-2]-2*data[maxpos]);
		Temp_B=(double)fabs((double)data[maxpos+3]+data[maxpos-1]-2*data[maxpos+1]);

		if((Temp_A+Temp_B)!=0)
			*edge=(double)maxpos+Temp_A/(Temp_A+Temp_B);	
		else
			*edge=(double)maxpos;
	}

	return 1;
}

int LN_FindEdgePos1_B2W(LPBYTE fm, int x1, int y1, int x2, int y2, int nPitch,int nHeight,
						double *edgeX, double *edgeY, int *slope,int subpitch)
{
	double PixelX[1280], PixelY[1280];
	double EdgeM[MAXEDGENUMBER];
	int   PixelV[1280];
	int nData;
	double edge, fLineLength;

    fLineLength=(double)sqrt(double((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2)));


	nData=GetLineData(fm, x1, y1, x2, y2, nPitch,nHeight, PixelX, PixelY, PixelV);
	if(nData<0) return -1;

	if(LN_FindEdge1_B2W(nData, PixelV, &edge, slope,subpitch)<0) return -2;

	*edgeX=(x1*(fLineLength-edge)+x2*edge)/fLineLength;
	*edgeY=(y1*(fLineLength-edge)+y2*edge)/fLineLength;

	return 1;
}

int LN_FindEdgePos1_W2B(LPBYTE fm, int x1, int y1, int x2, int y2, int nPitch,int nHeight,
						double *edgeX, double *edgeY, int *slope,int subpitch)
{
	double PixelX[1280], PixelY[1280];
	double EdgeM[MAXEDGENUMBER];
	int   PixelV[1280];	int nData;
	double edge, fLineLength;

    fLineLength=(double)sqrt((double)((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2)));

	nData=GetLineData(fm, x1, y1, x2, y2, nPitch,nHeight, PixelX, PixelY, PixelV);
	if(nData<0) return -1;

	if(LN_FindEdge1_W2B(nData, PixelV, &edge, slope,subpitch)<0) return -2;

	*edgeX=(x1*(fLineLength-edge)+x2*edge)/fLineLength;
	*edgeY=(y1*(fLineLength-edge)+y2*edge)/fLineLength;

	return 1;
}

int LN_FindEdgePos1_ABS(LPBYTE fm, int x1, int y1, int x2, int y2, int nPitch,int nHeight,
						double *edgeX, double *edgeY, int *slope, int *dir,int subpitch)
{
	double PixelX[1280], PixelY[1280];
	double EdgeM[MAXEDGENUMBER];
	int   PixelV[1280];
	int nData;
	double edge, fLineLength;

    fLineLength=(double)sqrt((double)((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2)));

	nData=GetLineData(fm, x1, y1, x2, y2, nPitch,nHeight, PixelX, PixelY, PixelV);
	if(nData<0) return -1;

	if(LN_FindEdge1_ABS(nData, PixelV, &edge, slope, dir,subpitch)<0) return -2;

	*edgeX=(x1*(fLineLength-edge)+x2*edge)/fLineLength;
	*edgeY=(y1*(fLineLength-edge)+y2*edge)/fLineLength;

	return 1;
}

int LN_FindEdge1st_B2W(int n, int *data, int slope, double *edge,int subpitch,int nLimmit)
{
	int sumdata1,sumdata2;
	int i,j;
	int maxslope, maxpos;
	double Temp_A, Temp_B;

	if(n<2)	return -1;

	maxpos=-1;
	for(i=subpitch;i<n-subpitch;i++)
	{
		sumdata1=0,sumdata2=0;
		for(j=1;j<subpitch+1;j++)
		{
			sumdata1 += (data[i+j]*(subpitch-j+1));
			sumdata2 += (data[i-j]*(subpitch-j+1));
		}
		if (data[i+subpitch*2] > nLimmit)
		{
			maxpos = -1;
			break;
		}

		if( (sumdata1-sumdata2) >= slope )
		{
			maxslope=(sumdata1-sumdata2);
			//maxslope=(data[i+1]-data[i-1]);
			maxpos=i;
			break;
		}
	}	
	if(maxpos<0) return -2;

	for(i=maxpos+subpitch;i<n-subpitch;i++)
	{
		sumdata1=0,sumdata2=0;
		for(j=1;j<subpitch+1;j++)
		{
			sumdata1 += (data[i+j]*(subpitch-j+1));
			sumdata2 += (data[i-j]*(subpitch-j+1));
		}
		if( (sumdata1-sumdata2) >= maxslope)
			//if( (data[i+1]-data[i]) >= maxslope)
		{
			maxslope=(sumdata1-sumdata2);
			//maxslope=(data[i+1]-data[i]);
			maxpos=i;
		}
		else 
			break;
	}		

	if(maxpos<=subpitch+1 || maxpos>=(n-subpitch-2))
	{
		*edge=(double)maxpos;
	}
	else
	{
		if(fabs((double)data[maxpos+2]-data[maxpos]) < fabs((double)data[maxpos]-data[maxpos-2]))
		{
			maxpos-=1;
		}

		Temp_A=(double)fabs((double)data[maxpos+2]+data[maxpos-2]-2*data[maxpos]);
		Temp_B=(double)fabs((double)data[maxpos+3]+data[maxpos-1]-2*data[maxpos+1]);

		if((Temp_A+Temp_B)!=0)
			*edge=(double)maxpos+Temp_A/(Temp_A+Temp_B);	
		else
			*edge=(double)maxpos;
	}

	return 1;
}

int LN_FindEdge1st_W2B(int n, int *data, int slope, double *edge,int subpitch)
{
	int sumdata1,sumdata2;
	int i,j;
	int maxpos,maxslope;
	double Temp_A, Temp_B;
	
	if(n<2)	return -1;

	if(slope>0) slope*=-1;

	maxpos=-1;
	for(i=subpitch;i<n-subpitch;i++)
	{
		sumdata1=0,sumdata2=0;
		for(j=1;j<subpitch+1;j++)
		{
			sumdata1 += (data[i+j]*(subpitch-j+1));
			sumdata2 += (data[i-j]*(subpitch-j+1));
		}
		//if( (data[i+subpitch]-data[i-subpitch]) <= slope)
		if( (sumdata1-sumdata2) <= slope)
		{
			//maxslope=(data[i+subpitch]-data[i-subpitch]);
			maxslope=(sumdata1-sumdata2);
			maxpos=i;
			break;
		}
	}

	if(maxpos<0) return -2;

	for(i=maxpos+subpitch;i<n-subpitch;i++)
	{
		sumdata1=0,sumdata2=0;
		for(j=1;j<subpitch+1;j++)
		{
			sumdata1 += (data[i+j]*(subpitch-j+1));
			sumdata2 += (data[i-j]*(subpitch-j+1));
		}
		//if( (data[i+subpitch]-data[i-subpitch]) <= maxslope)
		if( (sumdata1-sumdata2) <= maxslope)
		{
		//	maxslope=(data[i+subpitch]-data[i-subpitch]);
			maxslope=(sumdata1-sumdata2);
			maxpos=i;
		}
		else 
			break;
	}
		
	if(maxpos<=subpitch+1 || maxpos>=(n-subpitch-2))
	{
		*edge=(double)maxpos;
	}
	else
	{
		if(fabs((double)data[maxpos+2]-data[maxpos]) < fabs((double)data[maxpos]-data[maxpos-2]))
		{
			maxpos-=1;
		}

		Temp_A=(double)fabs((double)data[maxpos+2]+data[maxpos-2]-2*data[maxpos]);
		Temp_B=(double)fabs((double)data[maxpos+3]+data[maxpos-1]-2*data[maxpos+1]);

		if((Temp_A+Temp_B)!=0)
			*edge=(double)maxpos+Temp_A/(Temp_A+Temp_B);	
		else
			*edge=(double)maxpos;
	}

	return 1;
}

int LN_FindEdge1st_ABS(int n, int *data, int slope, double *edge, int *dir,int subpitch)
{
	int i;
	int maxpos, maxslope;
	double Temp_A, Temp_B;
	
	if(n<2)	return -1;

	maxpos=-1;
	for(i=1;i<n-1;i++)
		if( abs(data[i+1]-data[i-1]) >= slope)
		{
			maxslope=abs(data[i+1]-data[i-1]);
			maxpos=i;
			break;
		}

	if(maxpos<0) return -2;

	for(i=maxpos+1;i<n-1;i++)
	{
		if( abs(data[i+1]-data[i-1]) >= maxslope)
		{
			maxslope=abs(data[i+1]-data[i-1]);
			maxpos=i;
		}
		else 
			break;
	}
		
	if(data[maxpos+1]>data[maxpos]) *dir=1; //Black->White
	else                            *dir=0;	//White->Black

	if(maxpos<=2 || maxpos>=(n-3))
	{
		*edge=(double)maxpos;
	}
	else
	{
		if(fabs((double)data[maxpos+2]-data[maxpos]) < fabs((double)data[maxpos]-data[maxpos-2]))
		{
			maxpos-=1;
		}

		Temp_A=(double)fabs((double)data[maxpos+2]+data[maxpos-2]-2*data[maxpos]);
		Temp_B=(double)fabs((double)data[maxpos+3]+data[maxpos-1]-2*data[maxpos+1]);

		if((Temp_A+Temp_B)!=0)
			*edge=(double)maxpos+Temp_A/(Temp_A+Temp_B);	
		else
			*edge=(double)maxpos;
	}

	return 1;
}

int LN_FindEdgePos1st_B2W(LPBYTE fm, int x1, int y1, int x2, int y2, int nPitch,int nHeight, int slope,
						  double *edgeX, double *edgeY,int subpitch,int nLimmit)
{
	double PixelX[2000], PixelY[2000];
	double EdgeM[MAXEDGENUMBER];
	int   PixelV[2000];
	int nData;
	double edge, fLineLength;

	fLineLength=(double)sqrt((double)((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2)));

	nData=GetLineData(fm, x1, y1, x2, y2, nPitch,nHeight, PixelX, PixelY, PixelV);
	if(nData<0) return -1;

	if(LN_FindEdge1st_B2W(nData, PixelV, slope, &edge,subpitch,nLimmit)<0) return -2;

	*edgeX=(x1*(fLineLength-edge)+x2*edge)/fLineLength;
	*edgeY=(y1*(fLineLength-edge)+y2*edge)/fLineLength;

	return 1;
}

int LN_FindEdgePos1st_W2B(LPBYTE fm, int x1, int y1, int x2, int y2, int nPitch,int nHeight, int slope,
						  double *edgeX, double *edgeY,int subpitch)
{
	double PixelX[1280], PixelY[1280];
	double EdgeM[MAXEDGENUMBER];
	int   PixelV[1280];
	int nData;
	double edge, fLineLength;

    fLineLength=(double)sqrt((double)((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2)));

	nData=GetLineData(fm, x1, y1, x2, y2, nPitch,nHeight, PixelX, PixelY, PixelV);
	if(nData<0) return -1;

	if(LN_FindEdge1st_W2B(nData, PixelV, slope, &edge,subpitch)<0) return -2;

	*edgeX=(x1*(fLineLength-edge)+x2*edge)/fLineLength;
	*edgeY=(y1*(fLineLength-edge)+y2*edge)/fLineLength;

	return 1;
}

int LN_FindEdgePos1st_ABS(LPBYTE fm, int x1, int y1, int x2, int y2, int nPitch,int nHeight, int slope,
						  double *edgeX, double *edgeY, int *dir,int subpitch)
{
	double PixelX[1280], PixelY[1280];
	double EdgeM[MAXEDGENUMBER];
	int   PixelV[1280];
	int nData;
	double edge, fLineLength;

    fLineLength=(double)sqrt((double)((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2)));

	nData=GetLineData(fm, x1, y1, x2, y2, nPitch,nHeight, PixelX, PixelY, PixelV);
	if(nData<0) return -1;

	if(LN_FindEdge1st_ABS(nData, PixelV, slope, &edge, dir,subpitch)<0) return -2;

	*edgeX=(x1*(fLineLength-edge)+x2*edge)/fLineLength;
	*edgeY=(y1*(fLineLength-edge)+y2*edge)/fLineLength;

	return 1;
}


int LN_FindEdgeM_B2W(int n, int *data, int slope, double *edge)
{
	int i, nCount=0;
	int offset=0, flag;
	double  edge1;

	while(1)
	{
		if(LN_FindEdge1st_B2W(n-offset, &data[offset], slope, &edge1)<0) break;
		edge[nCount++]=(double)offset+edge1;
		flag=0;
		for(i=offset+(int)edge1+1;i<n-1;i++)
			if((data[i+1]-data[i-1]) <=0)
			{
				offset=i;
				flag=1;
				break;
			}
		if(!flag) break;
		if(nCount==MAXEDGENUMBER) break;
	}	
	
	return nCount;
}	
	
int LN_FindEdgeM_W2B(int n, int *data, int slope, double *edge)
{
	int i, nCount=0;
	int offset=0, flag;
	double  edge1;

	while(1)
	{
		if(LN_FindEdge1st_W2B(n-offset, &data[offset], slope, &edge1)<0) break;
		edge[nCount++]=(double)offset+edge1;
		flag=0;
		for(i=offset+(int)edge1+1;i<n-1;i++)
			if((data[i+1]-data[i-1]) >=0)
			{
				offset=i;
				flag=1;
				break;
			}
		if(!flag) break;
		if(nCount==MAXEDGENUMBER) break;
	}	
	
	return nCount;
}	

int LN_FindEdgeM_ABS(int n, int *data, int slope, double *edge, int *dir)
{
	int i, nCount=0;
	int offset=0, flag, dir1;
	double  edge1;

	while(1)
	{
		if(LN_FindEdge1st_ABS(n-offset, &data[offset], slope, &edge1, &dir1)<0) break;
		dir[nCount]=dir1;
		edge[nCount++]=(double)offset+edge1;
		flag=0;
		for(i=offset+(int)edge1+1;i<n-1;i++)
			if((data[i+1]-data[i])*(data[i]-data[i-1]) <= 0)
			{
				offset=i;
				flag=1;
				break;
			}
		if(!flag) break;
		if(nCount==MAXEDGENUMBER) break;
	}	
	
	return nCount;
}	

int LN_FindEdgePosM_B2W(LPBYTE fm, int x1, int y1, int x2, int y2, int nPitch,int nHeight, int slope,
						double *edgeX, double *edgeY)
{
	double PixelX[1280], PixelY[1280];
	double EdgeM[MAXEDGENUMBER];
	int   PixelV[1280];
	int i, nData;
	double fLineLength;
	int nEdge;

    fLineLength=(double)sqrt((double)((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2)));

	nData=GetLineData(fm, x1, y1, x2, y2, nPitch,nHeight, PixelX, PixelY, PixelV);
	if(nData<0) return -1;

	nEdge=LN_FindEdgeM_B2W(nData, PixelV, slope, EdgeM);

	for(i=0;i<nEdge;i++)
	{
		edgeX[i]=(x1*(fLineLength-EdgeM[i])+x2*EdgeM[i])/fLineLength;
		edgeY[i]=(y1*(fLineLength-EdgeM[i])+y2*EdgeM[i])/fLineLength;
	}
	return nEdge;
}

int LN_FindEdgePosM_W2B(LPBYTE fm, int x1, int y1, int x2, int y2, int nPitch,int nHeight, int slope,
						double *edgeX, double *edgeY)
{
	double PixelX[1280], PixelY[1280];
	double EdgeM[MAXEDGENUMBER];
	int   PixelV[1280];
	int i, nData;
	double fLineLength;
	int nEdge;

    fLineLength=(double)sqrt((double)((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2)));

	nData=GetLineData(fm, x1, y1, x2, y2, nPitch,nHeight, PixelX, PixelY, PixelV);
	if(nData<0) return -1;

	nEdge=LN_FindEdgeM_W2B(nData, PixelV, slope, EdgeM);

	for(i=0;i<nEdge;i++)
	{
		edgeX[i]=(x1*(fLineLength-EdgeM[i])+x2*EdgeM[i])/fLineLength;
		edgeY[i]=(y1*(fLineLength-EdgeM[i])+y2*EdgeM[i])/fLineLength;
	}
	return nEdge;
}

int LN_FindEdgePosM_ABS(LPBYTE fm, int x1, int y1, int x2, int y2, int nPitch,int nHeight, int slope,
						double *edgeX, double *edgeY, int *dir)
{
	double PixelX[1280], PixelY[1280];
	double EdgeM[MAXEDGENUMBER];
	int   PixelV[1280];
	int i, nData;
	double fLineLength;
	int nEdge;

    fLineLength=(double)sqrt((double)((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2)));

	nData=GetLineData(fm, x1, y1, x2, y2, nPitch,nHeight, PixelX, PixelY, PixelV);
	if(nData<0) return -1;

	nEdge=LN_FindEdgeM_ABS(nData, PixelV, slope, EdgeM, dir);

	for(i=0;i<nEdge;i++)
	{
		edgeX[i]=(x1*(fLineLength-EdgeM[i])+x2*EdgeM[i])/fLineLength;
		edgeY[i]=(y1*(fLineLength-EdgeM[i])+y2*EdgeM[i])/fLineLength;
	}
	return nEdge;
}

//return -1: (Left,top)-(right,bottom) Error
//		 -2: Memory Allocation Error
//       -3: Edge못찾음 
//		 
//        1: SUCCESS
int AR_FindEdge_T2B_B2W(LPBYTE fm, int left, int top, int right, int bottom, int nPitch,int nHeight,
						double *edgeX, double *edgeY)
{
	int i, nCount=0;
	int nData=right-left+1;
	int slope;
	double *edgeX1, *edgeY1;
	double l_t, l_a, l_b;  //Line함수의 Parameter (t*y=a*x+b)
	
	if(nData<2) return -1;
	edgeX1 = (double*)malloc(nData*sizeof(double));
	edgeY1 = (double*)malloc(nData*sizeof(double));
	if(edgeX1==0 || edgeY1==0) return -2;

	for(i=left;i<=right;i++)
	{
		if(LN_FindEdgePos1_B2W(fm, i, top, i, bottom, nPitch,nHeight,&edgeX1[nCount], &edgeY1[nCount], &slope)>0)
			nCount++;
	}

	if(nCount>=2)
	{
		if(LineFitting(nCount, edgeX1, edgeY1, (int)(nCount*0.5), 0.1, &l_t, &l_a, &l_b)<0)
			return -2;

		if(FindCrossPoint(l_t, l_a, l_b, 0, -1, (double)(left+right)/2.0,edgeX,edgeY)<0) return -3;
		if(*edgeX<left || *edgeX>right || *edgeY<top && *edgeY>bottom) return -3;

	}
	else return -3;

	if(edgeX1) free(edgeX1);
	if(edgeY1) free(edgeY1);
	return 1;
}

//return -1: (Left,top)-(right,bottom) Error
//		 -2: Memory Allocation Error
//       -3: Edge못찾음 
//		 
//        1: SUCCESS
int AR_FindEdge_T2B_W2B(LPBYTE fm, int left, int top, int right, int bottom, int nPitch,int nHeight,
						double *edgeX, double *edgeY)
{
	int i, nCount=0;
	int nData=right-left+1;
	int slope;
	double *edgeX1, *edgeY1;
	double l_t, l_a, l_b;  //Line함수의 Parameter (t*y=a*x+b)
	
	if(nData<2) return -1;
	edgeX1 = (double*)malloc(nData*sizeof(double));
	edgeY1 = (double*)malloc(nData*sizeof(double));
	if(edgeX1==0 || edgeY1==0) return -2;

	for(i=left;i<=right;i++)
	{
		if(LN_FindEdgePos1_W2B(fm, i, top, i, bottom, nPitch,nHeight,&edgeX1[nCount], &edgeY1[nCount], &slope)>0)
			nCount++;
	}

	if(nCount>=2)
	{
		if(LineFitting(nCount, edgeX1, edgeY1, (int)(nCount*0.5), 0.1, &l_t, &l_a, &l_b)<0)
			return -2;

		if(FindCrossPoint(l_t, l_a, l_b, 0, -1, (double)(left+right)/2.0,edgeX,edgeY)<0) return -3;
		if(*edgeX<left || *edgeX>right || *edgeY<top && *edgeY>bottom) return -3;
	}
	else return -3;

	if(edgeX1) free(edgeX1);
	if(edgeY1) free(edgeY1);
	return 1;
}

//return -1: (Left,top)-(right,bottom) Error
//		 -2: Memory Allocation Error
//       -3: Edge못찾음 
//		 
//        1: SUCCESS
int AR_FindEdge_L2R_B2W(LPBYTE fm, int left, int top, int right, int bottom, int nPitch,int nHeight,
						double *edgeX, double *edgeY)
{
	int i, nCount=0;
	int nData=bottom-top+1;
	int slope;
	double *edgeX1, *edgeY1;
	double l_t, l_a, l_b;  //Line함수의 Parameter (t*y=a*x+b)
	
	if(nData<2) return -1;
	edgeX1 = (double*)malloc(nData*sizeof(double));
	edgeY1 = (double*)malloc(nData*sizeof(double));
	if(edgeX1==0 || edgeY1==0) return -2;

	for(i=top;i<=bottom;i++)
	{
		if(LN_FindEdgePos1_B2W(fm, left, i, right, i, nPitch,nHeight,&edgeX1[nCount], &edgeY1[nCount], &slope)>0)
			nCount++;
	}

	if(nCount>=2)
	{
		if(LineFitting1(nCount, edgeX1, edgeY1, (int)(nCount*0.5), 0.1, &l_t, &l_a, &l_b)<0)
			return -2;

		if(FindCrossPoint(l_t, l_a, l_b, 1, 0, (double)(top+bottom)/2.0,edgeX,edgeY)<0) return -3;
		if(*edgeX<left || *edgeX>right || *edgeY<top && *edgeY>bottom) return -3;
	}
	else return -3;

	if(edgeX1) free(edgeX1);
	if(edgeY1) free(edgeY1);
	return 1;
}

//return -1: (Left,top)-(right,bottom) Error
//		 -2: Memory Allocation Error
//       -3: Edge못찾음 
//		 
//        1: SUCCESS
int AR_FindEdge_L2R_W2B(LPBYTE fm, int left, int top, int right, int bottom, int nPitch,int nHeight,
						double *edgeX, double *edgeY)
{
	int i, nCount=0;
	int nData=bottom-top+1;
	int slope;
	double *edgeX1, *edgeY1;
	double l_t, l_a, l_b;  //Line함수의 Parameter (t*y=a*x+b)
	
	if(nData<2) return -1;
	edgeX1 = (double*)malloc(nData*sizeof(double));
	edgeY1 = (double*)malloc(nData*sizeof(double));
	if(edgeX1==0 || edgeY1==0) return -2;

	for(i=top;i<=bottom;i++)
	{
		if(LN_FindEdgePos1_W2B(fm, left, i, right, i, nPitch,nHeight,&edgeX1[nCount], &edgeY1[nCount], &slope)>0)
			nCount++;
	}

	if(nCount>=2)
	{
		if(LineFitting1(nCount, edgeX1, edgeY1, (int)(nCount*0.5), 0.1, &l_t, &l_a, &l_b)<0)
			return -2;

		if(FindCrossPoint(l_t, l_a, l_b, 1, 0, (double)(top+bottom)/2.0,edgeX,edgeY)<0) return -3;
		if(*edgeX<left || *edgeX>right || *edgeY<top && *edgeY>bottom) return -3;
	}
	else return -3;

	if(edgeX1) free(edgeX1);
	if(edgeY1) free(edgeY1);
	return 1;
}
int AR_FindEdge_B2T_B2W(LPBYTE fm, int left, int top, int right, int bottom, int nPitch,int nHeight,
						double *edgeX, double *edgeY)
{
	return AR_FindEdge_T2B_W2B(fm, left, top, right, bottom, nPitch,nHeight,edgeX, edgeY);
}
int AR_FindEdge_B2T_W2B(LPBYTE fm, int left, int top, int right, int bottom, int nPitch,int nHeight,
						double *edgeX, double *edgeY)
{
	return AR_FindEdge_T2B_B2W(fm, left, top, right, bottom, nPitch,nHeight,edgeX, edgeY);
}
int AR_FindEdge_R2L_B2W(LPBYTE fm, int left, int top, int right, int bottom, int nPitch,int nHeight,
						double *edgeX, double *edgeY)
{
	return AR_FindEdge_L2R_W2B(fm, left, top, right, bottom, nPitch,nHeight,edgeX, edgeY);
}
int AR_FindEdge_R2L_W2B(LPBYTE fm, int left, int top, int right, int bottom, int nPitch,int nHeight,
						double *edgeX, double *edgeY)
{
	return AR_FindEdge_L2R_B2W(fm, left, top, right, bottom, nPitch,nHeight,edgeX, edgeY);
}

/////////////////////////////////////////////////////////////////////////////////////
//
//	각종 LOCAL 함수들 
//
/////////////////////////////////////////////////////////////////////////////////////


// 함수설명 : 임의의 라인을 1Pixel 간격으로 ReSampling해주는 함수 
// return Value : 구한 좌표의 개수       
//               <0 FAILURE
int GetLineData(LPBYTE fm, int x1, int y1, int x2, int y2, int nPitch,int nHeight,
				double *pX, double *pY, int *pGrayValue)
{
	int i, nCount=0;
    double fLineLength;
	int	  nAbsX1_X2, nAbsY1_Y2;
	double t=1.0;
    double slope;
	int  N, M ; // Interpolation Function에서 사용하는 변수들 
	double a,b;  // f(a,b)=P(N,M)(1-a)(1-b)+P(N+1,M)(a)(1-b)+p(N,M+1)(1-a)(b)+P(N+1,M+1)(a)(b)

    nAbsX1_X2 = abs(x1-x2);
	nAbsY1_Y2 = abs(y1-y2);

	// 라인의 길이 
    fLineLength=(double)sqrt((double)((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2)));
	if(fLineLength<2) return -1;

	if(nAbsX1_X2>0 && nAbsY1_Y2>0)
    {
		if(nAbsX1_X2 >= nAbsY1_Y2)  // 선이 x축으로 누운경우 
        {
            t=(double)nAbsX1_X2/fLineLength;
			if(x1>x2) t*=-1;
			slope = (double)(y1-y2)/(double)(x1-x2);
            for(i=0;i<=(int)fLineLength;i++)
            {
				pX[i]=(double)(x1+i*t);
				pY[i]=slope*(pX[i]-x1)+y1;  //직선의 방정식으로 부터 각 좌표를 구한다. 
            }
		}	
		else                     // 선이 y축으로 누운경우 
        {
            t=(double)nAbsY1_Y2/fLineLength;
			if(y1>y2) t*=-1;
			slope = (double)(x1-x2)/(double)(y1-y2);
            for(i=0;i<=(int)fLineLength;i++)
            {
				pY[i]=(double)(y1+i*t);
				pX[i]=slope*(pY[i]-y1)+x1; //직선의 방정식으로 부터 각 좌표를 구한다.
            }           
        }

        // 각 좌표에서의 픽셀값을 구한다.
		for(i=0;i<=(int)fLineLength;i++)
        {
			N=(int)pX[i];
			M=(int)pY[i];
			if(N>nPitch-2|| M<=0 || M>nHeight-2) break;
			//if(N>638 || M<=0 || M>478) break;
			a=pX[i]-N;
			b=pY[i]-M;
			pGrayValue[i]=(int)(*(fm+M*nPitch+N)*(1.0-a)*(1.0-b)+*(fm+M*nPitch+N+1)*(a)*(1.0-b)+
								*(fm+(M+1)*nPitch+N)*(1.0-a)*(b)+*(fm+(M+1)*nPitch+N+1)*(a)*(b));
			nCount++;
        }

    }
	else if(nAbsY1_Y2==0)
	{
		if(x1>x2) t=-1.0;
        for(i=0;i<=(int)fLineLength;i++)
        {
			pX[i]=(double)(x1+i*t);
			pY[i]=(double)y1;  
			pGrayValue[i]=(int)*(fm+nPitch*y1+(int)pX[i]);
			nCount++;
        }
	}
	else if(nAbsX1_X2==0)
	{
		if(y1>y2) t=-1.0;
        for(i=0;i<=(int)fLineLength;i++)
        {

			pY[i]=(double)(y1+i*t);
			pX[i]=(double) x1; 
			if(i>pX[i]-1 || pY[i]<=0 || pY[i]>nHeight-1) continue;
			pGrayValue[i]=(int)*(fm+nPitch*(int)pY[i]+x1);
			nCount++;
        }
	}

// 	static BOOL init = FALSE;
// 	if(init == FALSE)
// 	{
// 		init = TRUE;
// 		TRACE("\n - %d",(int)fLineLength);
// 		for (int i=0;i<nCount;i++)
// 		{
// 			TRACE("%d\t",pGrayValue[i]);
// 		}
// 	}
    return (nCount);
}

//Line Fitting
//ty=ax+b
//dataN : 주어진 (x,y)수
//*x, *y; (x,y)좌표 
//removeN : LineFitting후 Line에서 가장 멀리있는 점을 없애고 다시 fitting울 반복한다.
//          이 반복횟수를 나타낸다.
//ErrorLimit: LineFitting후 Line에서 가장 멀리있는 점이 이값 이하이면 반복을 중지한다.
//결과값 return: 1: 정상수행
//				 <0 : FAILURE
//              *t, *a, *b (ty=ax+b)

int LineFitting(int dataN, double *x, double *y, int removeN, double ErrorLimit,
				 double *t, double *a, double *b)
{
	int k,i,j;
	int nCount, MaxPos,*nValid;  
	double xvalue,yvalue;
	double Sxx, Sx, Sy,Sxy, SxSy, dd, MaxDist=0,dist;
	double ZeroData=0.00001;
	if(dataN<2) return -1;

	if(removeN>(dataN-2)) removeN=(dataN-2);

	nValid=(int*)malloc(dataN*sizeof(int));
	if(nValid==0) return -1;

	for(i=0;i<dataN;i++) nValid[i]=0;

	for(k=0;k<removeN+1;k++)
	{
		if(k==0)
		{
			Sx=Sy=Sxx=Sxy=SxSy=0.0;
			for(i=0;i<dataN;i++)
			{
				xvalue=*(x+i);
				yvalue=*(y+i);
				Sx+=xvalue;
				Sy+=yvalue;
				Sxx+=xvalue*xvalue;
				Sxy+=xvalue*yvalue;
			}
		}
		else
		{
			xvalue=*(x+MaxPos);
			yvalue=*(y+MaxPos);
			Sx-=xvalue;
			Sy-=yvalue;
			Sxx-=xvalue*xvalue;
			Sxy-=xvalue*yvalue;		
		}

		nCount=dataN-k;
		SxSy=Sx*Sy;

		dd=nCount*Sxx-Sx*Sx;
		if(fabs((double)dd)<ZeroData)
		{
			*t=0;
			*a=1;
			*b=-Sx/nCount;
			goto done;
		}
		else
		{
			*t=1;
			*a=(nCount*Sxy-Sx*Sy)/dd;
			*b=(-Sxy*Sx+Sy*Sxx)/dd;
		}

		dd=sqrt(*a**a+1);
		MaxDist=0;
		for(j=0;j<dataN;j++)
		{
			if(nValid[j]) continue;
			dist=fabs((double)-*(y+j)+*a**(x+j)+*b);
			if(dist>MaxDist)
			{
				MaxDist=dist;
				MaxPos=j;
			}
		}
		if(MaxDist/dd<ErrorLimit) goto done;
		else nValid[MaxPos]=1;
	}
done:
	if(nValid) free(nValid);
	return 1;
}


//1st Line Fitting(Line이 거의 수직일때)
// return 1: SUCCESS
//        -1: FAILURE
int LineFitting1(int dataN, double *x, double *y, int removeN, double ErrorLimit,
				 double *t, double *a, double *b)
{
	int returnValue;
	double la,lb,lt;

	returnValue=LineFitting(dataN,y, x, removeN, ErrorLimit, &lt,&la,&lb);

	if(returnValue==1)
	{
		if(fabs((double)la)<0.0001)  //기울기가 10000이상은 y축에 평행한것으로 간주한다.
		{
			*t=0;
			*a=lt;
			*b=-lb;
		}
		else
		{
			*t=1;
			*a=lt/la;
			*b=-lb/la;
		}	
		return 1;
	}
	else return returnValue;
}


// 두개의 직선으로부터 Cross하는 점 구하기
//t1y=a1x+b1
//t2y=a2x+b2
//return 1: SUCCESS
//       -1: FAILURE
int FindCrossPoint(double t1, double a1, double b1,
	  			   double t2, double a2, double b2, double *cx, double *cy)
{
	double local_LIMIT=1e-10;;
	

	if(fabs((double)t1-1)<local_LIMIT && fabs((double)t2-1) <local_LIMIT)
	{
		if(fabs((double)a1-a2) < local_LIMIT)
			return -1;
		*cx=(b2-b1)/(a1-a2);
		*cy=a1*(*cx)+b1;
	}
	else if(fabs((double)t1)<local_LIMIT && fabs((double)t2-1) <local_LIMIT)
	{
		if(fabs((double)a1) < local_LIMIT)
			return -1;
		*cx=-b1/a1;
		*cy=a2*(*cx)+b2;
	}
	else if(fabs((double)t1-1)<local_LIMIT && fabs((double)t2)<local_LIMIT)
	{
		if(fabs((double)a2) < local_LIMIT)
			return -1;
		*cx=-b2/a2;
		*cy=a1*(*cx)+b1;
	}
	else
		return -1;

	return 1;
}


//면보정 관련 함수 
void matrix_multiply(double *a, double *b, double *c, int l, int m, int n);
int matrix_inverse(double *a, int n, double *b);

/*면방정식 기본형 ax+by+cz=1
  A =	|x1  y1  z1|		X = | a |			B = | 1 |
		|x2  y2  z2|			| b |				| 1 |
		|x3  y3  z3|			| c |				| 1 |
		|x4  y4  y4|
			.
			.
			.
	AX = B를 만족하는 a,b,c를 구하는 문제
	(AT)AX = (AT)B
	(((AT)A)-¹)(AT)AX = (((AT)A)-¹)(AT)B
	X = ((AT)A)-¹(AT)B

*/
#define MAX_SURFACE_DATA_NUM	10
int SurfaceFitting(int nData, double *x, double *y, double *z, int nRemove, double dErrorLmt,
					double *a, double *b, double *c)
{
	if(nData<=0){
		*a = *b = *c = 0;
		return FALSE;
	}
	int i,j;
	double *A = (double*)malloc(sizeof(double)*3*nData);
	double *AT = (double*)malloc(sizeof(double)*3*nData);
	double *ATA = (double*)malloc(sizeof(double)*3*3);
	double *ATAI = (double*)malloc(sizeof(double)*3*3);
	double *ATAI_AT = (double*)malloc(sizeof(double)*3*nData);
	double *B = (double*)malloc(sizeof(double)*nData*1);
	double *RET = (double*)malloc(sizeof(double)*3*1);

//	Initialize Matrix Array Data
	memset(A, 0, sizeof(double)*3*nData);
	memset(AT, 0, sizeof(double)*3*nData);
	memset(ATA, 0, sizeof(double)*3*3);
	memset(ATAI, 0, sizeof(double)*3*3);
	memset(ATAI_AT, 0, sizeof(double)*3*nData);
	for(i=0; i<nData; i++)	B[i] = 1;
	memset(RET, 0, sizeof(double)*3*1);

	for(j=0; j<nData; j++){
		A[j*3+0] = AT[nData*0+j] = x[j];
		A[j*3+1] = AT[nData*1+j] = y[j];
		A[j*3+2] = AT[nData*2+j] = z[j];
	}
	matrix_multiply(AT, A, ATA, 3, nData, 3);
	matrix_inverse(ATA, 3, ATAI);
	matrix_multiply(ATAI, AT, ATAI_AT, 3, 3, nData);
	matrix_multiply(ATAI_AT, B, RET, 3, nData, 1);

	*a = RET[0];
	*b = RET[1];
	*c = RET[2];

	free(A);
	free(AT);
	free(ATA);
	free(ATAI);
	free(ATAI_AT);
	free(B);
	free(RET);

	return 1;
}

void matrix_multiply(double *a, double *b, double *c, int l, int m, int n)
{
int i,j,k;
double t;

    for (i=0; i<l; i++)
    {
        for (j=0; j<n; j++)
        {
            t = 0.0;
            for (k=0; k<m; k++)
                 t += (*(a+i*m+k) * (*(b+k*n+j)));
            *(c+i*n+j) = t;
        }
    }

}

int matrix_inverse(double *a, int n, double *b)
{
int i,j,k,l,l1,sw_ok;
double c,d;
//	char str[64];

    sw_ok = OFF;
    for (i=0;i<n;i++) 
    {
        for (j=0; j<n;j++) *(b+i*n+j) = 0.00000000000000;
        *(b+i*n+i) = 1.0;
    }
    for (l=0;l<n;l++) 
    {
        d = fabs((double)(*(a+l*n+l)));
        if (d < SMALLEST)
        {
            l1 = l;
            d = fabs((double)(*(a+l1*n+l)));
            while (d < SMALLEST && (++l1<n)) 
                d = fabs((double)(*(a+l1*n+l)));
            if (l1>=n) 
            {
                goto ERR; 
            }
            for (j=0;j<n;j++) 
            {
                *(a+l*n+j) += (*(a+l1*n+j));
                *(b+l*n+j) += (*(b+l1*n+j));
            }
        }
        c = 1.0/(*(a+l*n+l));
        for (j=l;j<n;j++) *(a+l*n+j) *= c;
        for (j=0;j<n;j++) *(b+l*n+j) *= c;
        k = l+1;
        for (i=k;i<n;i++) 
        {
            c = *(a+i*n+l);
            for (j=l;j<n;j++) *(a+i*n+j) -= (*(a+l*n+j) * c);
            for (j=0;j<n;j++) *(b+i*n+j) -= (*(b+l*n+j) * c);
        }
    }
    for (l=n-1; l>=0;l--) 
    {
        for (k=1;k<=l;k++) 
        {
               i = l-k;
               c = *(a+i*n+l);
               for (j=l;j<n;j++) *(a+i*n+j) -= (*(a+l*n+j) * c);
               for (j=0;j<n;j++) *(b+i*n+j) -= (*(b+l*n+j) * c);
        }
    }
    sw_ok = ON;
ERR:
    if (sw_ok==OFF) 
    {
//    ti_sprintf(str," Matrix Inversion : NO SOLUTION : l=%d", l);
//	  windowMessageBox(str,MB_OK);
    }
    return sw_ok;

}

//CircleFittingCenter의 SubRoutine
int FindCircleEdge(LPBYTE fm, int nPitch,int nHeight, int Cx, int Cy, int nR, int nObjectColor, int nSlope, double *pEdgeX, double *pEdgeY)
{
	int i, Xe, Ye, slope, nCount=0;
	double theta, ex, ey;

	for(i=0;i<MAXEDGENUMBER;i++)
	{
		theta=2.0*3.141592/(double)MAXEDGENUMBER;
		Xe=Cx+nR*cos(i*theta);
		Ye=Cy+nR*sin(i*theta);

		if(Xe<0.0)				
			Xe=0.0;
		else if(Xe>nPitch-1)	
			Xe=nPitch-1;
		if(Ye<0.0)				
			Ye=0.0;
		else if(Ye>CAMERA_HEIGHT-1)	
			Ye=CAMERA_HEIGHT-1;

		if(nObjectColor==0)
		{
			if(LN_FindEdgePos1_B2W(fm, Cx, Cy,Xe, Ye, nPitch,nHeight,&ex,&ey, &slope)<0)  //In->Out, B->W
				continue;
		}
		else
		{
			if(LN_FindEdgePos1_W2B(fm, Cx, Cy,Xe, Ye, nPitch,nHeight,&ex,&ey, &slope)<0)  //In->Out, W->B
				continue;
		}

		if(slope>nSlope)
		{
			pEdgeX[nCount]=ex;
			pEdgeY[nCount++]=ey;
		}
	}
	return nCount;
}

//CircleFittingCenter의 SubRoutine
int FindCircleEdge2(LPBYTE fm, int nPitch,int nHeight, int Cx, int Cy, int nSttR,int nEndR, int nEdgeNum,int nObjectColor, int nSlope, double *pEdgeX, double *pEdgeY,bool bInsType,int subpitch,int nCam,int nLimmit)
{
	int i, Xe=0, Ye=0,Xs=0,Ys=0, slope, nCount=0, tempXe=0, tempYe=0,tempXs=0,tempYs=0;
	double theta, ex, ey,angle;


	for(i=0;i<nEdgeNum;i++)
	{
		theta=2.0*3.141592/(double)nEdgeNum;
		angle=RADIAN2ANGLE(i*theta);

		if(i==7)
			i=i;

		if(nCam==0 && bInsType==1)//일부각도 경우
		{
			if(70>angle || angle>250)
			{
				tempXe=Xe;
				tempXs=Xs;
				tempYe=Ye;
				tempYs=Ys;
				Xs=Cx+nSttR*cos(i*theta);
				Ys=Cy+nSttR*sin(i*theta);
				Xe=Cx+nEndR*cos(i*theta);
				Ye=Cy+nEndR*sin(i*theta);

			}
			else
				continue;
		}
		else{
			tempXe=Xe;
			tempXs=Xs;
			tempYe=Ye;
			tempYs=Ys;
			Xs=Cx+nSttR*cos(i*theta);
			Ys=Cy+nSttR*sin(i*theta);
			Xe=Cx+nEndR*cos(i*theta);
			Ye=Cy+nEndR*sin(i*theta);
		}
		if(tempXe==Xe && tempXs==Xs && tempYe==Ye && tempYs==Ys)
			;
		else{
			if(Xs<0.0)					
				Xs=0.0;
			else if(Xs>nPitch-1)	
				Xs=nPitch-1;
			if(Ys<0.0)				
				Ys=0.0;
			else if(Ys>CAMERA_HEIGHT-1)
				Ys=CAMERA_HEIGHT-1;

			if(Xe<0.0)					
				Xe=0.0;
			else if(Xe>nPitch-1)	
				Xe=nPitch-1;
			if(Ye<0.0)				
				Ye=0.0;
			else if(Ye>CAMERA_HEIGHT-1)
				Ye=CAMERA_HEIGHT-1;

			if(nObjectColor==EDGE_B2W)
			{
				if(LN_FindEdgePos1_B2W(fm, Xs, Ys,Xe, Ye, nPitch,nHeight,&ex,&ey, &slope,subpitch)<0)  //In->Out, B->W
					continue;
			}
			else if(nObjectColor==EDGE_W2B)
			{
				if(LN_FindEdgePos1_W2B(fm, Xs, Ys,Xe, Ye, nPitch,nHeight,&ex,&ey, &slope,subpitch)<0)  //In->Out, W->B	
					continue;
			}
			else if(nObjectColor==EDGE_ABS)
			{
				int dir;	//b2w 인지 w2b인지 리턴해준다.
				if(LN_FindEdgePos1_ABS(fm, Xs, Ys,Xe, Ye, nPitch,nHeight,&ex,&ey, &slope,&dir,subpitch)<0)  //In->Out, ABS
					continue;
			}
			else if(nObjectColor==EDGE_1ST_B2W)
			{
				if(LN_FindEdgePos1st_B2W(fm, Xs, Ys,Xe, Ye, nPitch,nHeight,nSlope,&ex,&ey,subpitch,nLimmit)<0)  
					continue;
				slope = nSlope+99;
			}
			else if(nObjectColor==EDGE_1ST_W2B)
			{
				if(LN_FindEdgePos1st_W2B(fm, Xs, Ys,Xe, Ye, nPitch,nHeight,nSlope,&ex,&ey,subpitch)<0)  //완료
					continue;
				slope = nSlope+99;
			}
			else
				return -1;


			//ex = Xe;
			//ey = Ye;
			//slope = 100;
			if(slope>nSlope)
			{
				pEdgeX[nCount]=ex;
				pEdgeY[nCount++]=ey;
			}
		}
	}
	return nCount;
}