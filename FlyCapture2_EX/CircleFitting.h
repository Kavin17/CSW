// CircleFitting.h: interface for the CCircleFitting class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CIRCLEFITTING_H__3578DE13_D3DF_11D5_8D93_0050DA28BCA9__INCLUDED_)
#define AFX_CIRCLEFITTING_H__3578DE13_D3DF_11D5_8D93_0050DA28BCA9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#define  ON                        1
//#define  OFF                       0
//#define  SMALLEST          (1.00e-20)

class CCircleFitting  
{
public:
	void matrix_multiply(double *a, double *b, double *c, int l, int m, int n);
	int RobustCircleFit(double* x,double* y,int num,double* aa,double* bb,double* rr, int UseFulNum,double DeviationLimit);
	CCircleFitting();
	virtual ~CCircleFitting();

private:
	int GetDeviation(double *dis,int num);
	void SortDistance(double* dis,double* x,double* y,int num);
	int matrix_inverse(double *a, int n, double *b);
	void RemovePoints(double *x,double* y,int num,double a,double b,double r, double* dev,int* curnum);
	int FindCircleFit(double *x, double *y, int num, double *aa, double *bb, double *rr);
};

#endif // !defined(AFX_CIRCLEFITTING_H__3578DE13_D3DF_11D5_8D93_0050DA28BCA9__INCLUDED_)
