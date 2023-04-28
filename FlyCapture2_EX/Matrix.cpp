#include "stdafx.h"
#include "matrix.h"

/* DSP용 >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// Matrix Transpose
// n x m ==> m x n														 */
int MatrixTranspose(int n, int m, float A[][9], float Ai[][9])
{
	int i,j;
	for(i=0;i<n;i++)
	   for(j=0;j<m;j++)
          Ai[j][i]=A[i][j];	      

	return 0;
}
/* Matrix Mulpiplicant
// Size [n1 x m1] [n2 x m2] => [n1 x m2]    m1==n2
// [A][B]=[C]                           */
int MatrixMultiplicant(int n1, int m1, float A[][9], int n2, int m2, float B[][9], float C[][9])
{
	int i,j,k;

	if(	n1 > 9 || m1 >9 || n2 >9 || m2 >9 || m1!=n2) return 1;
	
	for(i=0 ; i<n1; i++)
		for(j=0; j<m2; j++)
		   C[i][j]=0;


	for(i=0 ; i<n1; i++)
		for(j=0; j<m2; j++)
		    for(k=0; k<m1; k++)
			   C[i][j]+=A[i][k]*B[k][j];

	return 0;
}

/* Matrix Mulpiplicant
// Size [n x m] [m x 1] => [n x 1]
// [A][B]=[C]					    */
int MatrixMultiplicant2(int n, int m, float A[][9], float B[], float C[])
{
	int i,j;

	if(	n > 9 || m >9 ) return 1;
	
	for(i=0 ; i<n; i++)
		   C[i]=0;


	for(i=0 ; i<n; i++)
		for(j=0; j<m; j++)
			   C[i]+=A[i][j]*B[j];

	return 0;
}

/* MAtrix Inverse
// Size n x n
// Inverse [A] => [Ai]
// return 0: 정상 NONZERO 불량   */
int MatrixInverse(int n, float A[][9], float Ai[][9])
{
   float big, pivot_inverse, temp, abs_element;
   int pivot_flag[9], swap_col[9], swap_row[9];
   int i,j, row,col,swap,irow,icol;

   if(	n > 9) return 1;
   
   for(i=0;i<n;i++)   
	   for(j=0;j<n;j++)
   			Ai[i][j]=A[i][j];

   for(i=0;i<9;i++)
     pivot_flag[i]=swap_row[i]=swap_col[i]=0;
   

   for(i=0;i<n;i++)  /* iterations of pivots */
   {
	   big=0.0;
       for(row=0; row <n ;row++)
	   {
		   if(!pivot_flag[row]) /* only unused pivotd */
		   {
			   for(col=0; col<n ;col++)
	           {
                  if(!pivot_flag[col])
			      {
				     abs_element=fabs(Ai[row][col]);
				     if(abs_element >= big)
				     {
					    big=abs_element;
					    irow=row;
					    icol=col;
				     }
			      }
		       }
	        }
       }
       pivot_flag[icol]++;
   
   /* swap rows to make this diagonal the bggest absolute pivot */
       if(irow!=icol)
       {
	      for(col=0;col <n;col++)
	      {
		     temp=Ai[irow][col];
		     Ai[irow][col]=Ai[icol][col];
		     Ai[icol][col]=temp;
	      }
       }

   /*  store what we swaped */
       swap_row[i]=irow;
       swap_col[i]=icol;

   /* Bad News if the pivot is zero	*/
       if(Ai[icol][icol]==0.0)  return 1;	   
		   

   /*  divide the row by the pivot*/
       pivot_inverse = 1.0/Ai[icol][icol];
       Ai[icol][icol]=1.0;
       for(col=0;col<n;col++)
	      Ai[icol][col]=Ai[icol][col]*pivot_inverse;

   /* Fix the other rows by substraction */
       for(row=0; row <n; row++)
	      if(row!=icol)
          {
	         temp=Ai[row][icol];
		     Ai[row][icol]=0.0;
		     for(col=0; col<n ; col++)
			    Ai[row][col] = Ai[row][col]-Ai[icol][col]*temp;
          }
   }
   for(swap=n-1;swap>=0;swap--)
   {
	   if(swap_row[swap] !=swap_col[swap])
	   {
		   for(row=0;row<n;row++)
		   {
			   temp=Ai[row][swap_row[swap]];
			   Ai[row][swap_row[swap]]=Ai[row][swap_col[swap]];
			   Ai[row][swap_col[swap]]=temp;
		   }
	   }
   }

   
    return 0;
}


// PC용(C++) >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

// Matrix Transpose
// n x m ==> m x n
int MatrixTranspose(int n, int m, double A[][9], double Ai[][9])
{
	int i,j;
	for(i=0;i<n;i++)
	   for(j=0;j<m;j++)
          Ai[j][i]=A[i][j];	      

	return 0;
}
// Matrix Mulpiplicant
// Size [n1 x m1] [n2 x m2] => [n1 x m2]   m1==n2
// [A][B]=[C]
int MatrixMultiplicant(int n1, int m1, double A[][9], int n2, int m2, double B[][9], double C[][9])
{
	int i,j,k;

	if(	n1 > 9 || m1 >9 || n2 >9 || m2 >9 || m1!=n2) return 1;
	
	for(i=0 ; i<n1; i++)
		for(j=0; j<m2; j++)
		   C[i][j]=0;


	for(i=0 ; i<n1; i++)
		for(j=0; j<m2; j++)
		    for(k=0; k<m1; k++)
			   C[i][j]+=A[i][k]*B[k][j];

	return 0;
}

// Matrix Mulpiplicant
// Size [n x m] [m x 1] => [n x 1]
// [A][B]=[C]
int MatrixMultiplicant2(int n, int m, double A[][9], double B[], double C[])
{
	int i,j;

	if(	n > 9 || m >9 ) return 1;
	
	for(i=0 ; i<n; i++)
		   C[i]=0;


	for(i=0 ; i<n; i++)
		for(j=0; j<m; j++)
			   C[i]+=A[i][j]*B[j];

	return 0;
}

// MAtrix Inverse
// Size n x n
// Inverse [A] => [Ai]
// return 0: 정상 NONZERO 불량 
int MatrixInverse(int n, double A[][9], double Ai[][9])
{
   double big, pivot_inverse, temp, abs_element;
   int pivot_flag[9], swap_col[9], swap_row[9];
   int i,j, row,col,swap,irow,icol;

   if(	n > 9) return 1;
   
   for(i=0;i<n;i++)   
	   for(j=0;j<n;j++)
   			Ai[i][j]=A[i][j];

   for(i=0;i<9;i++)
     pivot_flag[i]=swap_row[i]=swap_col[i]=0;


   for(i=0;i<n;i++)  //iterations of pivots
   {
	   big=0.0;
       for(row=0; row <n ;row++)
	   {
		   if(!pivot_flag[row]) // only unused pivotd
		   {
			   for(col=0; col<n ;col++)
	           {
                  if(!pivot_flag[col])
			      {
				     abs_element=fabs(Ai[row][col]);
				     if(abs_element >= big)
				     {
					    big=abs_element;
					    irow=row;
					    icol=col;
				     }
			      }
		       }
	        }
       }
       pivot_flag[icol]++;
   
   // swap rows to make this diagonal the bggest absolute pivot 
       if(irow!=icol)
       {
	      for(col=0;col <n;col++)
	      {
		     temp=Ai[irow][col];
		     Ai[irow][col]=Ai[icol][col];
		     Ai[icol][col]=temp;
	      }
       }

   //  store what we swaped
       swap_row[i]=irow;
       swap_col[i]=icol;

   // Bad News if the pivot is zero
       if(Ai[icol][icol]==0.0)  return 1;
	   

   // divide the row by the pivot
       pivot_inverse = 1.0/Ai[icol][icol];
       Ai[icol][icol]=1.0;
       for(col=0;col<n;col++)
	      Ai[icol][col]=Ai[icol][col]*pivot_inverse;

   //Fix the other rows by substraction
       for(row=0; row <n; row++)
	      if(row!=icol)
          {
	         temp=Ai[row][icol];
		     Ai[row][icol]=0.0;
		     for(col=0; col<n ; col++)
			    Ai[row][col] = Ai[row][col]-Ai[icol][col]*temp;
          }
   }
   for(swap=n-1;swap>=0;swap--)
   {
	   if(swap_row[swap] !=swap_col[swap])
	   {
		   for(row=0;row<n;row++)
		   {
			   temp=Ai[row][swap_row[swap]];
			   Ai[row][swap_row[swap]]=Ai[row][swap_col[swap]];
			   Ai[row][swap_col[swap]]=temp;
		   }
	   }
   }
 
   return 0;

}

