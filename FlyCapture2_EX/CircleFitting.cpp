// CircleFitting.cpp: implementation of the CCircleFitting class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CircleFitting.h"
#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCircleFitting::CCircleFitting()
{

}

CCircleFitting::~CCircleFitting()
{

}

int CCircleFitting::RobustCircleFit(double *x, double *y, int num, double *aa, double *bb, double *rr
									, int UseFulNum,double DeviationLimit)
{
	if(UseFulNum==0)	return 0;
	double a,b,r;
	double Dev;
	int CurNum;
//	double srtTime;
	
	CurNum = num;
	Dev = 1000.;
//	srtTime = TIMER_Get();
	do{
		FindCircleFit(x,y,num,&a,&b,&r);
		if(r<0 && r>10000)
			break;
		RemovePoints(x,y,num,a,b,r,&Dev,&CurNum);
		num = CurNum;
//		if((TIMER_Get()-srtTime)>1000.){
			//windowMessageBox("Time Over !",MB_OK);
//			guiStatusOutput("Time Over !");
//			break;
//		}
	}while(CurNum >= UseFulNum || Dev >= DeviationLimit);

	*aa = a;
	*bb = b;
	*rr = r;
	return CurNum;
}

int CCircleFitting::FindCircleFit(double *x, double *y, int num, double *aa, double *bb, double *rr)
{
	double f[3];
	double g[9],gd[9];
	double d[3];
	double a,b,r;
	double *xi,*yi;
	int i;
	
	f[0] = f[1] = f[2] = 0.0;
	for(i=0;i<9;i++) g[i] = 0.0;
	xi = x;
	yi = y;
	for(i=0;i<num;i++){
//		if(*xi > 0 && *xi < 1280 && *yi > 0 && *yi < 960){
//		if(*xi > 0 && *xi < 760 && *yi > 0 && *yi < 560){
			g[0] += *xi**xi;	g[1] += *xi**yi;	g[2] += *xi;
			g[3] += *xi**yi;	g[4] += *yi**yi;	g[5] += *yi;
			g[6] += *xi;		g[7] += *yi;		g[8] += 1.0;
			f[0] += (-1.0)*(*xi**xi**xi + *xi**yi**yi);
			f[1] += (-1.0)*(*xi**xi**yi + *yi**yi**yi);
			f[2] += (-1.0)*(*xi**xi + *yi**yi);
//		}
		xi++;
		yi++;
	}
	matrix_inverse(g,3,gd);
	matrix_multiply(gd,f,d,3,3,1);
	*aa = a = (-0.5)*d[0];
	*bb = b = (-0.5)*d[1];
	*rr = r = sqrt(0.25*(d[0]*d[0] + d[1]*d[1])-d[2]);
	return 0;
}

void CCircleFitting::RemovePoints(double *x, double *y, int num, double a, double b, double r, double *dev, int *curnum)
{
	int i;
	double *dis;
	double distance;
	double ellinum = num*0.03;

	dis = (double*)malloc(sizeof(double)*num);
	for(i=0;i<num;i++){
		distance = sqrt((a-*(x+i))*(a-*(x+i))+(b-*(y+i))*(b-*(y+i)));
		*(dis+i) = fabs(distance-r);
	}

	SortDistance(dis,x,y,num);
	*dev = GetDeviation(dis,(int)(num-ellinum));
	*curnum = num - ellinum;

	free(dis);
	return;
}

int CCircleFitting::matrix_inverse(double *a, int n, double *b)
{
int i,j,k,l,l1,sw_ok;
double c,d;
	char *str = (char*)malloc(64);

    sw_ok = OFF;
    for (i=0;i<n;i++) 
    {
        for (j=0; j<n;j++) *(b+i*n+j) = 0.00000000000000;
        *(b+i*n+i) = 1.0;
    }
    for (l=0;l<n;l++) 
    {
        d = fabs((*(a+l*n+l)));
        if (d < SMALLEST)
        {
            l1 = l;
            d = fabs((*(a+l1*n+l)));
            while (d < SMALLEST && (++l1<n)) 
                d = fabs((*(a+l1*n+l)));
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
	free(str);
    return sw_ok;
}

void CCircleFitting::matrix_multiply(double *a, double *b, double *c, int l, int m, int n)
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

void CCircleFitting::SortDistance(double *dis, double *x, double *y, int num)
{
	int i,j,mini=0;
	double mindis;
	double imsidis,imsix,imsiy;

	for(i=0;i<num;i++){
		mindis = 10000.;
		for(j=i;j<num;j++){
			if(*(dis+j) < mindis){
				mindis = dis[j];
				mini = j;
			}
		}

		imsidis = *(dis+i);
		imsix = *(x+i);
		imsiy = *(y+i);

		*(dis+i) = mindis;
		*(x+i) = *(x+mini);
		*(y+i) = *(y+mini);

		*(dis+mini) = imsidis;
		*(x+mini) = imsix;
		*(y+mini) = imsiy;
	}
}

int CCircleFitting::GetDeviation(double *dis, int num)
{
	int i;
	double max=0.;

	for(i=0;i<num;i++)
		if(*(dis+i)>max)
			max = *(dis+i);

	return (int)max;
}
