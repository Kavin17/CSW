#include "StdAfx.h"
#include "CgProcess.h"
#include <math.h>

CgProcess::CgProcess(void)
{
	m_White=255;    // Default Value	//object
	m_Black=0;
}

CgProcess::~CgProcess(void)
{

}


// AutoThreshold 구하는 일반함수 
// Return   -1   Error : x2, y2 가 x1, y1보다 커야함.
//          -2   Error : 영상이 한가지 색만을 가지고 있다.
//			그외 구한 Threshold값                             
int CgProcess::FindAutoThreshold(LPBYTE fm, int x1, int y1, int x2, int y2, int pitch)
{
	int i,j;
	const int GRAYLEVEL=256;
	DWORD  m_nHistoData[256];
	DWORD  m_nArea,nSum=0;
	double m_dU;    
	int nCount=0, Pos[3];
	int nThreshold;
	double pt, qt, U1, U2, qtPrev;
	double SigmaB2, MaxSigmaB2=0;

	if(x1 > x2 || y1 > y2 ) return -1;


	// 초기화 
	for(i=0;i<GRAYLEVEL;i++) m_nHistoData[i]=0;

	// Histogram Data 구하기 
	for(i=y1;i<=y2;i++)
		for(j=x1;j<=x2;j++)
			m_nHistoData[*(fm+i*pitch+j)]++;

	for(i=0;i<GRAYLEVEL;i++)
	{
		nSum+=i*m_nHistoData[i];
	}

	// 면적과 평균을 구한다. 
	m_nArea=(x2-x1+1)*(y2-y1+1);
	m_dU   =(double)nSum/(double)m_nArea;//ave

	// Histogram의 데이타가 NONZERO인 곳이 1,2있을때>>>>>>>>
	for(i=0;i<GRAYLEVEL;i++)
		if(m_nHistoData[i])
		{
			Pos[nCount]=i;
			nCount++;
			if(nCount>2) break;
		}
		if(nCount==1) return -2;
		else if(nCount==2) return (Pos[0]+Pos[1])/2;
		//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


		qt=(double)m_nHistoData[0]/(double)m_nArea;
		if(qt==1) return 0;

		U1   = 0;
		U2   = (m_dU-qt*U1)/(1-qt);

		for(i=0;i<GRAYLEVEL-1;i++)
		{
			SigmaB2=qt*(1-qt)*(U2-U1)*(U2-U1);
			if(SigmaB2 > MaxSigmaB2 )
		 {
			 nThreshold=i;
			 MaxSigmaB2=SigmaB2;
			}
			qtPrev=qt;
			pt=(double)m_nHistoData[i+1]/(double)m_nArea;
			qt=qtPrev+pt;
			if(qt==0)       continue;
			else if(qt==1)  break;    //qt가 1 이면 더이상은 의미가 없으므로 Break함
			U1=(qtPrev*U1+(i+1)*pt)/qt;
			U2=(m_dU-qt*U1)/(1-qt);		 
		}

		return nThreshold;
}
void CgProcess::ImageFlip(unsigned char* fmSrc,unsigned char* fmDest,int nWidth,int nHeight,BOOL bIsX)
{
	for(int j=0;j<nHeight;j++)
		for (int i=0;i<nWidth;i++)
		{
			if(bIsX)
				fmDest[(nWidth-i)+nWidth*j] = fmSrc[i+nWidth*j];
			else
				fmDest[(i)+nWidth*(nHeight-j)] = fmSrc[i+nWidth*j];
		}

}

void CgProcess::ImageResizeNearest(unsigned char* fmSrc,unsigned char* fmDest,int nSrcW,int nSrcH,int nDestW,int nDestH)
{
//	double dScaleX = (double)nDestW/(double)nSrcW;;//(double)nSrcW/(double)nDestW;
//	double dScaleY =(double)nDestH/ (double)nSrcH;//(double)nSrcH/(double)nDestH;
	double dScaleX = (double)nSrcW/(double)nDestW;
	double dScaleY =(double)nSrcH/(double)nDestH;

//	dScaleX = dScaleY = 1;
	//dScaleX = dScaleY = 1;
	int nc1,nc2;
	nc1 = nc2=0;
	if(nc1==0){
		for (int i=0;i<nDestW;i++)
			for(int j=0;j<nDestH;j++)
			{
				int x = (i*dScaleX);
				int y = (dScaleY*j);
				if(x<nSrcW && y<nSrcH)
				{
					fmDest[i+nDestW*j] = fmSrc[x+nSrcW*y];
					nc1++;
				}
				else
				{
					fmDest[i+nDestW*j] = 0;
					nc2++;
				}

			}
	}
	else
	{
		for(int j=0;j<nSrcH;j++)
			for (int i=0;i<nSrcW;i++)
			{
				int x = (i/dScaleX);
				int y = (j/dScaleY);
				if(x<nDestW && y<nDestH)
				{
					fmDest[x+nDestW*y] = fmSrc[i+nSrcW*j];
					nc1++;
				}
				else
				{
					fmDest[x+nDestW*y] = 0;
					nc2++;
				}

			}

	}
	int nc = nc1+nc2;
 	int nn = nDestH*nDestW;

}
void CgProcess::CropImageSelectCopy(unsigned char* fmSrc,unsigned char* fmDest,int nSrcW,int nSrcH,CRect rtSrc,int nDestW,int nDestH)	//큰영상 -> 작은영상
{
	int x,y;
	for(int i=rtSrc.left;i<rtSrc.right;i++)
	{
		for(int j=rtSrc.top;j<rtSrc.bottom;j++)
		{
			x = i-rtSrc.left;
			y = j-rtSrc.top;
			if(0<=x && x<nDestW && 0<=y && y<nDestH&& 0<=i && i<nSrcW  && 0<=j && j<nSrcW)
			fmDest[(x)+nDestW*(y)] = fmSrc[i+nSrcW*j];
		}
	}
}

void CgProcess::ImageSelectCopy(unsigned char* fmSrc,unsigned char* fmDest,int nSrcW,int nSrcH,CRect rtSrc,int nDestW,int nDestH)	//큰영상 -> 작은영상
{
	int x,y;
	for(int i=rtSrc.left;i<rtSrc.right;i++)
		for(int j=rtSrc.top;j<rtSrc.bottom;j++)
		{
			x = i-rtSrc.left;
			y = j-rtSrc.top;
			if(0<=x && x<nDestW && 0<=y && y<nDestH&& 0<=i && i<nSrcW  && 0<=j && j<nSrcW)
			fmDest[(i-x)+nDestW*(j-y)] = fmSrc[i+nSrcW*j];
			//fmDest[(x)+nDestW*(y)] = fmSrc[i+nSrcW*j];
		}

}

void CgProcess::ImageSelectCopy(unsigned char* fmSrc,unsigned char* fmDest,int nSrcW,int nSrcH,CRect rtSrc,int nDestW,int nDestH,int nDestSttX,int nDestSttY,int nn)
{
	int x,y;
	if(nn==0)
	{
		//예외처리
	//	if(rtSrc.Width())
		for(int j=rtSrc.top;j<rtSrc.bottom;j++)
			for(int i=rtSrc.left;i<rtSrc.right;i++)
			{
				x = (nDestSttX+i-rtSrc.left);
				y = (nDestSttY+j-rtSrc.top);
				if(0<=x && x<nDestW && 0<=y && y<nDestH
				&& 0<=i && i<nSrcW  && 0<=j && j<nSrcH)
				{
					fmDest[x+nDestW*(nDestH-y-1)] = fmSrc[i+nSrcW*j];
				}
			}
	}
	else if(nn==3)
	{
		//예외처리

		for(int j=rtSrc.top;j<rtSrc.bottom;j++)
			for(int i=rtSrc.left;i<rtSrc.right;i++)
			{
				x = (nDestSttX+i-rtSrc.left);
				y = (nDestSttY+j-rtSrc.top);
				if(0<=x && x<nDestW && 0<=y && y<nDestH
				&& 0<=i && i<nSrcW  && 0<=j && j<nSrcH)
					fmDest[x+nDestW*y] = fmSrc[i+nSrcW*j];
			}
	}
	else
	{

		for(int j=rtSrc.top;j<rtSrc.bottom;j++)
		{
			memcpy(fmDest+j*nDestW+nDestSttX,fmSrc+j*nSrcW,nSrcW);
		}

	}
}

void CgProcess::ImageRotate(unsigned char* fmSrc,unsigned char* fmDest,int nSrcW,int nSrcH,int nAngle)
{
	for(int j=0;j<nSrcH;j++)
		for(int i=0;i<nSrcW;i++)
		{
			if(nAngle == 0)
				fmDest[i+nSrcH*(j)] = fmSrc[i+nSrcW*j];
		 	else if(nAngle == 90)
				fmDest[j+nSrcH*(nSrcH-i-1)] = fmSrc[i+nSrcW*j];
			else if(nAngle == 180)
				fmDest[(nSrcW-i-1)+nSrcH*(nSrcH-j-1)] = fmSrc[i+nSrcW*j];

				//fmDest[i+nSrcW*(j)] = fmSrc[i+nSrcW*j];
		}

}
void CgProcess::ImageReduce(unsigned char* fmSrc,unsigned char* fmDest,int nSrcW,int nSrcH,int nScale)
{
	int nDestW = int(nSrcW/nScale);
	int nDestH = int(nSrcH/nScale);

	int nskip = 4;//1;
	for(int j=0;j<nDestH;j++)
		for(int i=0;i<nDestW;i++)
		{
			int nCnt = 0,nSum=0;
			*(fmDest+i+nDestW*j)=255;
			for (int y=0;y<nScale;y+=2)
				for (int x=0;x<nScale;x+=nskip)
				{
					//nSum += fmSrc[(i*nScale+x)+CAMERA_WIDTH*(j*nScale+y)];
					if (*(fmSrc+(i*nScale+x)+nSrcW*(j*nScale+y))<*(fmDest+i+nDestW*j))
						*(fmDest+i+nDestW*j) = *(fmSrc+(i*nScale+x)+nSrcW*(j*nScale+y));
				}
				//fmDest[i+nDestW*j] = int(nSum/(nScale*nScale));
		}
}
void CgProcess::ImageReduce2(unsigned char* fmSrc,unsigned char* fmDest,int nSrcW,int nSrcH,int nScale)
{
	int nDestW = int(nSrcW/nScale);
	int nDestH = int(nSrcH/nScale);

	int nskip = 2;
	for(int j=0;j<nDestH;j++)
		for(int i=0;i<nDestW;i++)
		{
			int nCnt = 0,nSum=0;
			*(fmDest+i+nDestW*j)=255;
			for (int y=0;y<nScale;y+=nskip)
				for (int x=0;x<nScale;x+=nskip)
				{
					//nSum += fmSrc[(i*nScale+x)+CAMERA_WIDTH*(j*nScale+y)];
					if (*(fmSrc+(i*nScale+x)+nSrcW*(j*nScale+y))<*(fmDest+i+nDestW*j))
						*(fmDest+i+nDestW*j) = *(fmSrc+(i*nScale+x)+nSrcW*(j*nScale+y));
				}
				//fmDest[i+nDestW*j] = int(nSum/(nScale*nScale));
		}
}

void CgProcess::ImageReduce1(unsigned char* fmSrc,unsigned char* fmDest,int nSrcW,int nSrcH,int nScale)
{
	int nDestW = int(nSrcW/nScale);
	int nDestH = int(nSrcH/nScale);

	for(int j=0;j<nSrcH;j+=nScale)
		for(int i=0;i<nSrcW;i+=nScale)
		{
			*(fmDest+(j/nScale)*nDestW+(i/nScale)) = *(fmSrc+j*nSrcW+i);
			//for (int y=0;y<nScale;y++)
			//	for (int x=0;x<nScale;x++)
			//	{
			//		//nSum += fmSrc[(i*nScale+x)+CAMERA_WIDTH*(j*nScale+y)];
			//		if (fmSrc[(i*nScale+x)+nSrcW*(j*nScale+y)]<fmDest[i+nDestW*j])
			//			fmDest[i+nDestW*j] = fmSrc[(i*nScale+x)+nSrcW*(j*nScale+y)];
			//	}
			//	//fmDest[i+nDestW*j] = int(nSum/(nScale*nScale));
		}
}

void CgProcess::ImageBin(unsigned char* fmSrc,unsigned char* fmDest,int nWidth,int nHeight,double nTh)
{
	//memset(fmDest,0,nWidth*nHeight)

	int nCnt = 0,nCnt1=0;
	for(int j=0;j<nHeight;j++)
		for(int i=0;i<nWidth;i++)
		{
			if (fmSrc[i+nWidth*j]>nTh)
				fmDest[i+nWidth*j] = m_Black,nCnt++;
			else
				fmDest[i+nWidth*j] = m_White,nCnt1++;
		}
	return;
}


double CgProcess::ImageProjection(unsigned char* fm,int nWidth,int nHeight,CRect rt,BOOL bDirIsX,BOOL bIsCount,double* pHisto)
{
	double dPeak = 0.;
	int nPeakPos = - 1;

	if(bDirIsX)	
	{
		memset(pHisto,0,sizeof(double)*nWidth);
		for(int i=rt.left;i<rt.right;i++)
		{
			int ncnt = 0;
			for(int j=rt.top;j<rt.bottom;j++,ncnt++)
			{
				if(i<0 || i>=nWidth || j<0 || j>=nHeight)
					continue;
				if(bIsCount)
				{
					if(fm[i+nWidth*j] == m_White)
						pHisto[i] += 1.;
				}
				else
					pHisto[i] += fm[i+nWidth*j];
			}			

			if(!bIsCount)
				pHisto[i] /= ncnt;
			if(pHisto[i]>dPeak)
				dPeak = pHisto[i],nPeakPos = i;
		}
	}
	else
	{
		memset(pHisto,0,sizeof(double)*nHeight);
		for(int j=rt.top;j<rt.bottom;j++)
		{
			int ncnt = 0;
			for(int i=rt.left;i<rt.right;i++,ncnt++)
			{
				if(i<0 || i>=nWidth || j<0 || j>=nHeight)
					continue;
				if(bIsCount)
				{
					if(fm[i+nWidth*j] == m_White)
						pHisto[j] += 1.;
				}
				else
					pHisto[j] += fm[i+nWidth*j];
			}			

			if(!bIsCount)
				pHisto[j] /= ncnt;
			if(pHisto[j]>dPeak)
				dPeak = pHisto[j],nPeakPos = j;
		}
	}

	return (double)nPeakPos;
}

int CgProcess::Dilate_Binary(LPBYTE fmSour, LPBYTE fmDest, int nWidth, int nHeight, int nPitch,
						  int* pKernel, int nKernelSizeX, int nKernelSizeY)
{
	// 변수 체크 
	if(nWidth>nPitch) return 0;

	int i,j,o,p;
	int nOrgX, nOrgY;

	nOrgX=(int)(nKernelSizeX/2.0-0.5);
	nOrgY=(int)(nKernelSizeY/2.0-0.5);

	//BEGIN-0: 테두리는 원래 영상과 동일하게 만든다. >>>>>>>>>>>>>>>>>>
	for(i=0;i<=nOrgY;i++) 
		for(j=0;j<nWidth;j++)
		{
			*(fmDest+nPitch*i+j)=*(fmSour+nPitch*i+j);
			*(fmDest+nPitch*(nHeight-1-i)+j)=*(fmSour+nPitch*(nHeight-1-i)+j);
		}

		for(j=0;j<=nOrgX;j++)
			for(i=0;i<nHeight;i++) 
			{
				*(fmDest+nPitch*i+j)=*(fmSour+nPitch*i+j);
				*(fmDest+nPitch*i+(nWidth-1-j))=*(fmSour+nPitch*i+(nWidth-1-j));
			}
			//END-0: <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


			for(i=0;i<nHeight-nKernelSizeY;i++) 
			{//전체영역이 10x10이고 kernel 이 4x4이면 X,Y범위는 (0,0)~(6,6) 이다.
				for(j=0;j<nWidth-nKernelSizeX;j++)      //                     
				{
					for(o=0;o<nKernelSizeY;o++)
					{	
						for(p=0;p<nKernelSizeX;p++)
						{
							if( *(pKernel+nKernelSizeX*o+p) )                
							{
								if( *(fmSour+(i+o)*nPitch+j+p) )
							   {
								   *(fmDest+(i+nOrgY)*nPitch+j+nOrgX)=m_White;
								   goto LOOP;
							   }
							}
						}
					}
					//if(*(fmSour+(i+o)*nPitch+j+p)>130 )
					//	*(fmDest+(i+nOrgY)*nPitch+j+nOrgX)=m_White;					   
					//else
						*(fmDest+(i+nOrgY)*nPitch+j+nOrgX)=m_Black;					   
LOOP:      continue;
				}
			}
				return 1;
}


int CgProcess::Dilate_BinaryMini(LPBYTE fmSour, LPBYTE fmDest, int nWidth, int nHeight, int nPitch)
{

	int i,j,k,l;
	int nOrgX, nOrgY;
	int nKernelSizeY,nKernelSizeX;
	nKernelSizeY = nKernelSizeX = 3;
	int kernel[9];
	for(i=0;i<9;i++) kernel[i] = 1;

	nOrgX=(int)(nKernelSizeX/2.0-0.5);
	nOrgY=(int)(nKernelSizeY/2.0-0.5);


	for(i=0;i<nHeight-nKernelSizeY;i++)         //
		for(j=0;j<nWidth-nKernelSizeX;j++)      //                     
		{
			
			{
				for(k=0;k<nKernelSizeY;k++)
					for(l=0;l<nKernelSizeX;l++)
						//				   if( *(pKernel+nKernelSizeX*k+l) )                
						//				   if( *(kernel+nKernelSizeX*k+l) )                
						if( *(fmSour+(i+k)*nWidth+j+l) )
						{
							*(fmDest+(i+nOrgY)*nWidth+j+nOrgX)=255;
							goto LOOP;
						}
						*(fmDest+(i+nOrgY)*nWidth+j+nOrgX)=0;					   
			}


LOOP:      continue;
		}
		return 1;

}


int CgProcess::Erode_Binary(LPBYTE fmSour, LPBYTE fmDest, int nWidth, int nHeight, int nPitch,
						 int* pKernel, int nKernelSizeX, int nKernelSizeY)
{
	// 변수 체크 
	if(nWidth>nPitch) return 0;

	int i,j,k,l;
	int nOrgX, nOrgY;

	nOrgX=(int)(nKernelSizeX/2.0-0.5);
	nOrgY=(int)(nKernelSizeY/2.0-0.5);

	//BEGIN-0: 테두리는 원래 영상과 동일하게 만든다. >>>>>>>>>>>>>>>>>>
	for(i=0;i<=nOrgY;i++) 
		for(j=0;j<nWidth;j++)
		{
			*(fmDest+nPitch*i+j)=*(fmSour+nPitch*i+j);
			*(fmDest+nPitch*(nHeight-1-i)+j)=*(fmSour+nPitch*(nHeight-1-i)+j);
		}

		for(j=0;j<=nOrgX;j++)
			for(i=0;i<nHeight;i++) 
			{
				*(fmDest+nPitch*i+j)=*(fmSour+nPitch*i+j);
				*(fmDest+nPitch*i+(nWidth-1-j))=*(fmSour+nPitch*i+(nWidth-1-j));
			}
			//END-0: <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


			for(i=0;i<nHeight-nKernelSizeY;i++)         //전체영역이 10x10이고 kernel 이 4x4이면 X,Y범위는 (0,0)~(6,6) 이다.
				for(j=0;j<nWidth-nKernelSizeX;j++)      //                     
				{
					for(k=0;k<nKernelSizeY;k++)
						for(l=0;l<nKernelSizeX;l++)
							if( *(pKernel+nKernelSizeX*k+l) )                
								if( !*(fmSour+(i+k)*nPitch+j+l) )
					   {
						   *(fmDest+(i+nOrgY)*nPitch+j+nOrgX)=m_Black;
						   goto LOOP;
					   }

								*(fmDest+(i+nOrgY)*nPitch+j+nOrgX)=m_White;					   
LOOP:      continue;
				}
				return 1;
}

int CgProcess::Open_Binary(LPBYTE fmSour, LPBYTE fmDest, int nWidth, int nHeight, int nPitch,
						int* pKernel, int nKernelSizeX, int nKernelSizeY)
{
	LPBYTE fmTmp = (LPBYTE) new char[nPitch*nHeight];

	Erode_Binary(fmSour, fmTmp, nWidth, nHeight, nPitch,pKernel,nKernelSizeX,nKernelSizeY);

	Dilate_Binary(fmTmp, fmDest, nWidth, nHeight, nPitch,pKernel,nKernelSizeX,nKernelSizeY);

	delete fmTmp;
	return 1;
}

int CgProcess::Close_Binary(LPBYTE fmSour, LPBYTE fmDest, int nWidth, int nHeight, int nPitch,
						 int* pKernel, int nKernelSizeX, int nKernelSizeY)
{
	LPBYTE fmTmp = (LPBYTE) new char[nPitch*nHeight];

	Dilate_Binary(fmSour, fmTmp, nWidth, nHeight, nPitch,pKernel,nKernelSizeX,nKernelSizeY);

	Erode_Binary(fmTmp, fmDest, nWidth, nHeight, nPitch,pKernel,nKernelSizeX,nKernelSizeY);

	delete fmTmp;
	return 1;
}

int CgProcess::Dilate_Gray(LPBYTE fmSour, LPBYTE fmDest, int nWidth, int nHeight, int nPitch,
						int* pKernel, int nKernelSizeX, int nKernelSizeY)
{
	// 변수 체크 
	if(nWidth>nPitch) return 0;

	int i,j,k,l;
	int nOrgX, nOrgY;
	int Kernel_ALLZero=0;
	int MaxValue;

	nOrgX=(int)(nKernelSizeX/2.0-0.5);
	nOrgY=(int)(nKernelSizeY/2.0-0.5);

	//BEGIN-0: 테두리는 원래 영상과 동일하게 만든다. >>>>>>>>>>>>>>>>>>
	for(i=0;i<=nOrgY;i++){
		memcpy(fmDest+i,fmSour+i,nWidth);
		for(j=0;j<nWidth;j++)
		{
//			*(fmDest+nPitch*i+j)=*(fmSour+nPitch*i+j);
			*(fmDest+nPitch*(nHeight-1-i)+j)=*(fmSour+nPitch*(nHeight-1-i)+j);
		}
	}

	for(i=0;i<nHeight;i++){
		memcpy(fmDest+i,fmSour+i,nOrgX);
		for(j=0;j<=nOrgX;j++)
		{
			//*(fmDest+nPitch*i+j)=*(fmSour+nPitch*i+j);
			*(fmDest+nPitch*i+(nWidth-1-j))=*(fmSour+nPitch*i+(nWidth-1-j));
		}
	}
			//END-0: <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

	for(k=0;k<nKernelSizeY;k++)
		for(l=0;l<nKernelSizeX;l++)
			if( *(pKernel+nKernelSizeX*k+l) ) Kernel_ALLZero=1;

	if(Kernel_ALLZero==0) // 모든 kernel의 값이 ZERO이면 
	{
		for(i=0;i<nHeight-nKernelSizeY;i++)         
			for(j=0;j<nWidth-nKernelSizeX;j++)                           
			{
				MaxValue=0;
				for(k=0;k<nKernelSizeY;k++)
					for(l=0;l<nKernelSizeX;l++)
						if( *(fmSour+(i+k)*nPitch+j+l) > MaxValue)
						{
							MaxValue=*(fmSour+(i+k)*nPitch+j+l);
						}

						*(fmDest+(i+nOrgY)*nPitch+j+nOrgX)=MaxValue;
			}
	}
	else  //kernel이 어떤 값을 가지고 있을때
	{
		for(i=0;i<nHeight-nKernelSizeY;i++)         
			for(j=0;j<nWidth-nKernelSizeX;j++)                           
			{
				MaxValue=0;
				for(k=0;k<nKernelSizeY;k++)
					for(l=0;l<nKernelSizeX;l++)
						if( *(fmSour+(i+k)*nPitch+j+l)+*(pKernel+nKernelSizeX*k+l) > MaxValue)
						{
							MaxValue=*(fmSour+(i+k)*nPitch+j+l)+*(pKernel+nKernelSizeX*k+l);
						}
						if(	MaxValue>255) MaxValue=255;	   
						*(fmDest+(i+nOrgY)*nPitch+j+nOrgX)=MaxValue;
			}

	}

	return 1;
}

int CgProcess::Erode_Gray(LPBYTE fmSour, LPBYTE fmDest, int nWidth, int nHeight, int nPitch,
					   int* pKernel, int nKernelSizeX, int nKernelSizeY)
{
	// 변수 체크 
	if(nWidth>nPitch) return 0;

	int i,j,k,l;
	int nOrgX, nOrgY;
	int Kernel_ALLZero=0;
	int MinValue;

	nOrgX=(int)(nKernelSizeX/2.0-0.5);
	nOrgY=(int)(nKernelSizeY/2.0-0.5);

	//BEGIN-0: 테두리는 원래 영상과 동일하게 만든다. >>>>>>>>>>>>>>>>>>
	for(i=0;i<=nOrgY;i++) 
	{
		memcpy(fmDest+i,fmSour+i,nPitch);//speedup
		for(j=0;j<nWidth;j++)
		{
//			*(fmDest+nPitch*i+j)=*(fmSour+nPitch*i+j);
			*(fmDest+nPitch*(nHeight-1-i)+j)=*(fmSour+nPitch*(nHeight-1-i)+j);
		}
	}

	for(int h=0;h<nHeight;h++) 
	{
		memcpy(fmDest+h,fmSour+h,nOrgX);//speedup
		for(j=0;j<=nOrgX;j++)
		{
//			*(fmDest+nPitch*h+j)=*(fmSour+nPitch*h+j);
			*(fmDest+nPitch*h+(nWidth-1-j))=*(fmSour+nPitch*h+(nWidth-1-j));
		}
	}
			//END-0: <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

	for(k=0;k<nKernelSizeY;k++)
		for(l=0;l<nKernelSizeX;l++)
			if( *(pKernel+nKernelSizeX*k+l) ) Kernel_ALLZero=1;

	if(Kernel_ALLZero==0) // 모든 kernel의 값이 ZERO이면 
	{
		for(i=0;i<nHeight-nKernelSizeY;i++)         
			for(j=0;j<nWidth-nKernelSizeX;j++)                           
			{
				MinValue=255;
				for(k=0;k<nKernelSizeY;k++)
					for(l=0;l<nKernelSizeX;l++)
						if( *(fmSour+(i+k)*nPitch+j+l) < MinValue)
						{
							MinValue=*(fmSour+(i+k)*nPitch+j+l);
						}

				*(fmDest+(i+nOrgY)*nPitch+j+nOrgX)=MinValue;
			}
	}
	else  //kernel이 어떤 값을 가지고 있을때
	{
		for(i=0;i<nHeight-nKernelSizeY;i++)         
			for(j=0;j<nWidth-nKernelSizeX;j++)                           
			{
				MinValue=255;
				for(k=0;k<nKernelSizeY;k++)
					for(l=0;l<nKernelSizeX;l++)
						if( *(fmSour+(i+k)*nPitch+j+l)-*(pKernel+nKernelSizeX*k+l) < MinValue)
						{
							MinValue=*(fmSour+(i+k)*nPitch+j+l)-*(pKernel+nKernelSizeX*k+l);
						}
				if(	MinValue<0) MinValue=0;	   
				*(fmDest+(i+nOrgY)*nPitch+j+nOrgX)=MinValue;
			}

	}

	return 1;
}

int CgProcess::Open_Gray(LPBYTE fmSour, LPBYTE fmDest, int nWidth, int nHeight, int nPitch,
					  int* pKernel, int nKernelSizeX, int nKernelSizeY)
{
	DWORD dw1 = GetTickCount();

	LPBYTE fmTmp = (LPBYTE) new char[nPitch*nHeight];

	Erode_Gray(fmSour, fmTmp, nWidth, nHeight, nPitch,pKernel,nKernelSizeX,nKernelSizeY);
	DWORD dw2 = GetTickCount();

	Dilate_Gray(fmTmp, fmDest, nWidth, nHeight, nPitch,pKernel,nKernelSizeX,nKernelSizeY);
	DWORD dw3 = GetTickCount();


	delete fmTmp;
	return 1;
}

int CgProcess::Close_Gray(LPBYTE fmSour, LPBYTE fmDest, int nWidth, int nHeight, int nPitch,
					   int* pKernel, int nKernelSizeX, int nKernelSizeY)
{
	LPBYTE fmTmp = (LPBYTE) new char[nPitch*nHeight];

	Dilate_Gray(fmSour, fmTmp, nWidth, nHeight, nPitch,pKernel,nKernelSizeX,nKernelSizeY);

	Erode_Gray(fmTmp, fmDest, nWidth, nHeight, nPitch,pKernel,nKernelSizeX,nKernelSizeY);

	delete fmTmp;
	return 1;
}


//////////////////////////////////////////////////////////////////////////////////////////


// fm : Frame Memory 시작번지
// (left, top)-(right,bottom) : ROI
// nPitch : Frame 폭 
// nSlope : 최소 Edge Slope 
// (*Cx, *Cy) : Circle Center
// return 1: Success
//        <0 : Failure

int CgProcess::FindCircleCenter(LPBYTE fm, int left, int top, int right, int bottom, int nPitch, int nSlope, int nSkip,
					 double *Cx, double *Cy)
{
	int nResult=-1;
	int nRet1, nRet2;
	int i, nslope1, nslope2,nCount;
	double edgex1, edgey1, edgex2, edgey2;
	if(nSkip<=0) nSkip=1;


	nCount=0;
	for(i=top;i<bottom;i+=nSkip)
	{
		nRet1=LN_FindEdgePos1_W2B(fm, left, i, right, i, nPitch, &edgex1, &edgey1,&nslope1);
		nRet2=LN_FindEdgePos1_W2B(fm, right, i, left, i, nPitch, &edgex2, &edgey2,&nslope2);
		if(nRet1>0 && nRet2>0 && nslope1>nSlope && nslope2>nSlope)
		{
			Edge[nCount++]=(edgex1+edgex2)*0.5;
		}
	}					
	nCount=ChooseGoodData(nCount, Edge, 5);
	if(nCount<MINDATA_COUNT) goto done;
	*Cx=FN_GetCenter(nCount, Edge);
			
	nCount=0;
	for(i=left;i<right;i+=nSkip)
	{
		nRet1=LN_FindEdgePos1_W2B(fm, i, top, i, bottom, nPitch, &edgex1, &edgey1,&nslope1);
		nRet2=LN_FindEdgePos1_W2B(fm, i, bottom, i, top, nPitch, &edgex2, &edgey2,&nslope2);
		if(nRet1>0 && nRet2>0 && nslope1>nSlope && nslope2>nSlope)
		{
			Edge[nCount++]=(edgey1+edgey2)*0.5;
		}
	}					
	nCount=ChooseGoodData(nCount, Edge, 5);
	if(nCount<MINDATA_COUNT) goto done;
	*Cy=FN_GetCenter(nCount, Edge);

	nResult=1;
done:
	return nResult;
}

int CgProcess::FindCircleCenter1(LPBYTE fm, int left, int top, int right, int bottom, int nPitch, int nSlope, int nSkip,
								double *Cx, double *Cy)
{
	int nResult=-1;
	int nRet1, nRet2;
	int i, nslope1, nslope2,nCount;
	double edgex1, edgey1, edgex2, edgey2;
	if(nSkip<=0) nSkip=1;


	nCount=0;
	for(i=top;i<bottom;i+=nSkip)
	{
		nRet1=LN_FindEdgePos1_B2W(fm, left, i, right, i, nPitch, &edgex1, &edgey1,&nslope1);
		nRet2=LN_FindEdgePos1_B2W(fm, right, i, left, i, nPitch, &edgex2, &edgey2,&nslope2);
		if(nRet1>0 && nRet2>0 && nslope1>nSlope && nslope2>nSlope)
		{
			Edge[nCount++]=(edgex1+edgex2)*0.5;
		}
	}					
	nCount=ChooseGoodData(nCount, Edge, 5);
	if(nCount<MINDATA_COUNT) goto done;
	*Cx=FN_GetCenter(nCount, Edge);

	nCount=0;
	for(i=left;i<right;i+=nSkip)
	{
		nRet1=LN_FindEdgePos1_B2W(fm, i, top, i, bottom, nPitch, &edgex1, &edgey1,&nslope1);
		nRet2=LN_FindEdgePos1_B2W(fm, i, bottom, i, top, nPitch, &edgex2, &edgey2,&nslope2);
		if(nRet1>0 && nRet2>0 && nslope1>nSlope && nslope2>nSlope)
		{
			Edge[nCount++]=(edgey1+edgey2)*0.5;
		}
	}					
	nCount=ChooseGoodData(nCount, Edge, 5);
	if(nCount<MINDATA_COUNT) goto done;
	*Cy=FN_GetCenter(nCount, Edge);

	nResult=1;
done:
	return nResult;
}


// fm : Frame Memory 시작번지
// (left, top)-(right,bottom) : ROI
// nPitch : Frame 폭 
// nSlope : 최소 Edge Slope 
// (*Cx, *Cy) : Circle Center
// return 1: Success
//        <0 : Failure
int CgProcess::CircleFittingCenter(LPBYTE fm, int left, int top, int right, int bottom, int nPitch, int nObjectColor, int nSlope,
					   double *Cx, double *Cy, double *Diameter)
{
	long nResult=-1;

	int i,j, nCount;
	int    maxpos;
	double dist,maxdist;

	double theta, ex, ey;
	double nX, nY, SX, SY, SArea, SXY, SXX, SYY, SXXX, SYYY, SYYX, SXXY;
	double Mat_A[9], Mat_AInv[9], Mat_B[3];
	double Parm1, Parm2, Parm3;
	double dRadius2, dRadius;

	int nR1=(bottom-top);
	int nR2=(right-left);
	int nR;

	if(nR1<nR2) nR=nR1*0.5;
	else        nR=nR2*0.5;


	for(i=0;i<1;i++)
	{
		if(i==0)
			nCount=FindCircleEdge(fm, nPitch, (left+right)*0.5, (top+bottom)*0.5, nR, nObjectColor, nSlope, pX, pY);
		else
			nCount=FindCircleEdge(fm, nPitch, *Cx, *Cy, *Diameter*0.6, nObjectColor, nSlope, pX, pY);

		SX=SY=SArea=SXY=SXX=SYY=SXXX=SYYY=SYYX=SXXY=0;
		for(i=0;i<nCount;i++)
		{
			nX=pX[i];
			nY=pY[i];

			SArea++;
			SX+=nX;
			SY+=nY;
			SXY+=nY*nX;
			SXX+=nX*nX;
			SYY+=nY*nY;
			SXXX+=nX*nX*nX;
			SYYY+=nY*nY*nY;
			SYYX+=nY*nY*nX;
			SXXY+=nX*nX*nY;
		}

		Mat_A[0]=(double)SXX;
		Mat_A[1]=(double)SXY;
		Mat_A[2]=(double)SX;
		Mat_A[3]=(double)SXY;
		Mat_A[4]=(double)SYY;
		Mat_A[5]=(double)SY;
		Mat_A[6]=(double)SX;
		Mat_A[7]=(double)SY;
		Mat_A[8]=(double)SArea;

		Mat_B[0]=(double)(-SXXX-SYYX);
		Mat_B[1]=(double)(-SYYY-SXXY);
		Mat_B[2]=(double)(-SXX-SYY);

		matrixInverse(Mat_A,3,Mat_AInv);

		Parm1=Mat_AInv[0]*Mat_B[0]+Mat_AInv[1]*Mat_B[1]+Mat_AInv[2]*Mat_B[2];
		Parm2=Mat_AInv[3]*Mat_B[0]+Mat_AInv[4]*Mat_B[1]+Mat_AInv[5]*Mat_B[2];
		Parm3=Mat_AInv[6]*Mat_B[0]+Mat_AInv[7]*Mat_B[1]+Mat_AInv[8]*Mat_B[2];
		dRadius2=(Parm1*Parm1+Parm2*Parm2-4.0*Parm3)*0.25;
		*Cx=-Parm1*0.5;	
		*Cy=-Parm2*0.5;	
		*Diameter=sqrt(dRadius2)*2;
	}

	
	for(i=0;i<MAXEDGENUMBER*0.25;i++)
	{
		maxdist=0;
		dRadius=*Diameter*0.5;

		for(j=0;j<nCount;j++)
		{
			dist=sqrt((*Cx-pX[j])*(*Cx-pX[j])+(*Cy-pY[j])*(*Cy-pY[j]))-dRadius;
			if(dist<0) dist*=-1.0;

			if(dist>maxdist)
			{
				maxdist=dist;
				maxpos=j;
			}
		}

		if(maxdist<0.25) break;

		nX=pX[maxpos];
		nY=pY[maxpos];

		SArea--;
		SX-=nX;
		SY-=nY;
		SXY-=nY*nX;
		SXX-=nX*nX;
		SYY-=nY*nY;
		SXXX-=nX*nX*nX;
		SYYY-=nY*nY*nY;
		SYYX-=nY*nY*nX;
		SXXY-=nX*nX*nY;

		Mat_A[0]=(double)SXX;
		Mat_A[1]=(double)SXY;
		Mat_A[2]=(double)SX;
		Mat_A[3]=(double)SXY;
		Mat_A[4]=(double)SYY;
		Mat_A[5]=(double)SY;
		Mat_A[6]=(double)SX;
		Mat_A[7]=(double)SY;
		Mat_A[8]=(double)SArea;

		Mat_B[0]=(double)(-SXXX-SYYX);
		Mat_B[1]=(double)(-SYYY-SXXY);
		Mat_B[2]=(double)(-SXX-SYY);

		matrixInverse(Mat_A,3,Mat_AInv);

		Parm1=Mat_AInv[0]*Mat_B[0]+Mat_AInv[1]*Mat_B[1]+Mat_AInv[2]*Mat_B[2];
		Parm2=Mat_AInv[3]*Mat_B[0]+Mat_AInv[4]*Mat_B[1]+Mat_AInv[5]*Mat_B[2];
		Parm3=Mat_AInv[6]*Mat_B[0]+Mat_AInv[7]*Mat_B[1]+Mat_AInv[8]*Mat_B[2];
		dRadius2=(Parm1*Parm1+Parm2*Parm2-4.0*Parm3)*0.25;
		*Cx=-Parm1*0.5;	
		*Cy=-Parm2*0.5;	
		*Diameter=sqrt(dRadius2)*2;

		nCount--;
		for(j=maxpos;j<nCount;j++)
		{
			pX[j]=pX[j+1];
			pY[j]=pY[j+1];
		}
	}

	nResult=1;  //OK
done:
	return nResult;
}


// fm : Frame Memory 시작번지
// (left, top)-(right,bottom) : ROI
// nPitch : Frame 폭 
// nSlope : 최소 Edge Slope 
// (*Cx, *Cy) : Circle Center
// return 1: Success
//        <0 : Failure
int CgProcess::HoughCircle(LPBYTE fm, int left, int top, int right, int bottom, int nPitch,
				int nObjectColor, int nSlope, int nR1, int nR2,
				double *Cx, double *Cy)
{
	int i, j, k, ok=1;
	int nEdgeSense=(nSlope*3);  // 10.8=sqrt(2*765*765)/100;
	double gx,gy, gl;
	int xc, yc;
	int local_i, local_j, local_nPitch=(right-left+1), local_height=(bottom-top+1);
	int *pHough, nHough;
	int nHoughMax=0, Maxi, Maxj,  X_Start, X_End, Y_Start, Y_End;
	int nOffset;
	double sumx,sumy;
	double sumhough;
	int nThreshold;
	int nDiameter=(int)((nR1+nR2)*0.5);

	nOffset=nDiameter*0.2;

	if(nDiameter<50){
		nThreshold=20;
	}
	else if(nDiameter<100){
		nThreshold=25;
	}
	else{
		nThreshold=35;
	}


	pHough= (int*)malloc((right-left+1)*(bottom-top+1)*sizeof(int));

	for(i=0;i<local_height;i++)
		for(j=0;j<local_nPitch;j++)
			*(pHough+local_nPitch*i+j)=0;

	local_i=0;
	for(i=top+1;i<bottom;i++)
	{	
		local_i++;
		local_j=0;
		for(j=left+1;j<right;j++)
		{
			local_j++;
			gx= *(fm+nPitch*(i-1)+(j+1))-*(fm+nPitch*(i-1)+(j-1))+
				*(fm+nPitch*i+(j+1))-*(fm+nPitch*i+(j-1))+
				*(fm+nPitch*(i+1)+(j+1))-*(fm+nPitch*(i+1)+(j-1));
			gy= *(fm+nPitch*(i+1)+(j-1))-*(fm+nPitch*(i-1)+(j-1))+
				*(fm+nPitch*(i+1)+(j))-*(fm+nPitch*(i-1)+(j))+
				*(fm+nPitch*(i+1)+(j+1))-*(fm+nPitch*(i-1)+(j+1));
			if(nObjectColor==0)
			{
				gx*=(-1.0);
				gy*=(-1.0);
			}
			gl= sqrt(gx*gx+gy*gy);
			if(gl>=nEdgeSense)
			{
				for(k=nR1;k<=nR2;k++)
				{
					xc=(int)(local_j+k*(gx/gl)+0.5);
					yc=(int)(local_i+k*(gy/gl)+0.5);
					if(xc>0 && xc<local_nPitch && yc>0 && yc<local_height)
						*(pHough+local_nPitch*yc+xc)+=1;				
				}
			}	
		}
	}	

	//------------------------------중심 구하기 ---------------------------
	for(i=0;i<local_height;i++)
		for(j=0;j<local_nPitch;j++)
			if(*(pHough+local_nPitch*i+j)>nHoughMax)
			{
				nHoughMax=*(pHough+local_nPitch*i+j);
				Maxi=i;
				Maxj=j;
			}
	X_Start=Maxj-nOffset;
	if(X_Start<0) X_Start=0;
	X_End=Maxj+nOffset;
	if(X_End>=local_nPitch) X_End=local_nPitch-1;
	Y_Start=Maxi-nOffset;
	if(Y_Start<0) Y_Start=0;
	Y_End=Maxi+nOffset;
	if(Y_End>=local_height) Y_End=local_height-1;


	sumx=sumy=sumhough=0;
	for(i=Y_Start;i<=Y_End;i++)
		for(j=X_Start;j<=X_End;j++)
		{
			nHough=*(pHough+local_nPitch*i+j);
			if(nHough>nThreshold)
			{
				sumx+=(nHough*j);
				sumy+=(nHough*i);
				sumhough+=nHough;
			}
		}

	if(sumhough>0)
	{
		*Cx=sumx/sumhough+left;
		*Cy=sumy/sumhough+top;
	}
	else
	{
		ok=-1;
	}
	//--------------------------------------------------------------

	free(pHough);
	return ok;
}


//Transform Data : orgx, orgy, theta, dx, dy
//(x,y)			 : Old 좌표계의 Data
//(x_, y_)		 : Transform 된 후 좌표 

int CgProcess::PosTransform(double orgx, double orgy, double theta, double dx, double dy, 
				 double x, double y, double *x_, double *y_)
{
	double sinTheta, cosTheta;

	sinTheta=sin(theta);
	cosTheta=cos(theta);

	*x_=(x-orgx)*cosTheta-(y-orgy)*sinTheta+dx+orgx;
	*y_=(x-orgx)*sinTheta+(y-orgy)*cosTheta+dy+orgy;

	return 1;
}










//CircleFittingCenter의 SubRoutine
int CgProcess::FindCircleEdge(LPBYTE fm, int nPitch, int Cx, int Cy, int nR, int nObjectColor, int nSlope, double *pEdgeX, double *pEdgeY)
{
	int i, Xe, Ye, slope, nCount=0;
	double theta, ex, ey;
		
	for(i=0;i<MAXEDGENUMBER;i++)
	{
		theta=2.0*3.141592/(double)MAXEDGENUMBER;
		Xe=Cx+nR*cos(i*theta);
		Ye=Cy+nR*sin(i*theta);

		if(Xe<0.0)				  Xe=0.0;
		else if(Xe>639)			  Xe=639.0;
		if(Ye<0.0)				  Ye=0.0;
		else if(Ye>479)			  Ye=479.0;
	
		if(nObjectColor==0)
		{
			if(LN_FindEdgePos1_B2W(fm, Cx, Cy,Xe, Ye, nPitch,&ex,&ey, &slope)<0)  //In->Out, B->W
				continue;
		}
		else
		{
			if(LN_FindEdgePos1_W2B(fm, Cx, Cy,Xe, Ye, nPitch,&ex,&ey, &slope)<0)  //In->Out, W->B
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

int CgProcess::LN_FindEdge1_B2W(int n, int *data, double *edge, int *slope)
{
	int i;
	int maxslope, maxpos;
	double Temp_A, Temp_B;
	
	if(n<2)	return -1;

	maxslope=maxpos=0;
	for(i=1;i<n-1;i++)
		if( (data[i+1]-data[i-1])> maxslope)
		{
			maxpos=i;
			maxslope=(data[i+1]-data[i-1]);
		}
		
	if(maxslope==0) return -2;

	*slope=abs(maxslope);
	if(maxpos<=2 || maxpos>=(n-3))
	{
		*edge=(double)maxpos;
	}
	else
	{
		if(fabs(double(data[maxpos+2]-data[maxpos])) < fabs(double(data[maxpos]-data[maxpos-2])))
		{
			maxpos-=1;
		}

		Temp_A=(double)fabs(double(data[maxpos+2]+data[maxpos-2]-2*data[maxpos]));
		Temp_B=(double)fabs(double(data[maxpos+3]+data[maxpos-1]-2*data[maxpos+1]));

		if((Temp_A+Temp_B)!=0)
			*edge=(double)maxpos+Temp_A/(Temp_A+Temp_B);	
		else
			*edge=(double)maxpos;
	}

	return 1;
}

int CgProcess::LN_FindEdge1_W2B(int n, int *data, double *edge, int *slope)
{
	int i;
	int maxslope, maxpos;
	double Temp_A, Temp_B;
	
	if(n<2)	return -1;

	maxslope=maxpos=0;
	for(i=1;i<n-1;i++)
		if( (data[i+1]-data[i-1])< maxslope)
		{
			maxpos=i;
			maxslope=(data[i+1]-data[i-1]);
		}
		
	if(maxslope==0) return -2;

	*slope=abs(maxslope);
	if(maxpos<=2 || maxpos>=(n-3))
	{
		*edge=(double)maxpos;
	}
	else
	{
		if(fabs(double(data[maxpos+2]-data[maxpos])) < fabs(double(data[maxpos]-data[maxpos-2])))
		{
			maxpos-=1;
		}

		Temp_A=(double)fabs(double(data[maxpos+2]+data[maxpos-2]-2*data[maxpos]));
		Temp_B=(double)fabs(double(data[maxpos+3]+data[maxpos-1]-2*data[maxpos+1]));

		if((Temp_A+Temp_B)!=0)
			*edge=(double)maxpos+Temp_A/(Temp_A+Temp_B);	
		else
			*edge=(double)maxpos;
	}

	return 1;
}


int CgProcess::LN_FindEdgePos1_B2W(LPBYTE fm, int x1, int y1, int x2, int y2, int nPitch,
						double *edgeX, double *edgeY, int *slope)
{
	int nData;
	double edge, fLineLength;

    fLineLength=(double)sqrt(double((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2)));

	nData=GetLineData(fm, x1, y1, x2, y2, nPitch, PixelX, PixelY, PixelV);
	if(nData<0) return -1;

	if(LN_FindEdge1_B2W(nData, PixelV, &edge, slope)<0) return -2;

	*edgeX=(x1*(fLineLength-edge)+x2*edge)/fLineLength;
	*edgeY=(y1*(fLineLength-edge)+y2*edge)/fLineLength;

	return 1;
}

int CgProcess::LN_FindEdgePos1_W2B(LPBYTE fm, int x1, int y1, int x2, int y2, int nPitch,
						double *edgeX, double *edgeY, int *slope)
{
	int nData;
	double edge, fLineLength;

    fLineLength=(double)sqrt(double((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2)));

	nData=GetLineData(fm, x1, y1, x2, y2, nPitch, PixelX, PixelY, PixelV);
	if(nData<0) return -1;

	if(LN_FindEdge1_W2B(nData, PixelV, &edge, slope)<0) return -2;

	*edgeX=(x1*(fLineLength-edge)+x2*edge)/fLineLength;
	*edgeY=(y1*(fLineLength-edge)+y2*edge)/fLineLength;

	return 1;
}

// 함수설명 : 임의의 라인을 1Pixel 간격으로 ReSampling해주는 함수 
// return Value : 구한 좌표의 개수       
//               <0 FAILURE
int CgProcess::GetLineData(LPBYTE fm, int x1, int y1, int x2, int y2, int nPitch,
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
    fLineLength=(double)sqrt(double((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2)));
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
			if(N>638 || M<=0 || M>478) break;
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
			pGrayValue[i]=(int)*(fm+nPitch*(int)pY[i]+x1);
			nCount++;
        }
	}

    return (nCount);
}
int CgProcess::ChooseGoodData(int N, double *x, int HISTO_SPACE)
{
	int i;
	int nMax=0, iMax;		//nMax : Max histo Data, iMax: Max histo 발생지점
	int nTemp;				
	int nCount=0;
	int MAX_HISTO;
	
	if(N<=0) return N;
	if(HISTO_SPACE<5) HISTO_SPACE=5;
	MAX_HISTO=((640/HISTO_SPACE)+HISTO_SPACE);

	for(i=0;i<MAX_HISTO;i++)
		histo[i]=0;

	for(i=0;i<N;i++)
	{
		nTemp=(int)(x[i]/HISTO_SPACE);
		if(nTemp<MAX_HISTO)
			histo[nTemp]++;
	}

	for(i=0;i<MAX_HISTO;i++)
		if(histo[i]>nMax)
		{
			nMax=histo[i];
			iMax=i;
		}

	for(i=0;i<N;i++)
	{
		if((int)(x[i]/HISTO_SPACE)>=iMax-1 && (int)(x[i]/HISTO_SPACE)<=iMax+1)
		{
			x[nCount]=x[i];
		//	y[nCount]=y[i];
			nCount++;
		}
	}	
	return nCount;
}
 
double CgProcess::FN_GetCenter(int N, double *x)
{
	int i;
	double sum=0;
	if(N<=0) return -1;

	for(i=0;i<N;i++)
	{
		sum+=x[i];
	}
 
	return (sum/(double)N);
}
int CgProcess::matrixInverse(double *a,int n,double *b)
{
	 int i,j,k,l,l1;
	 double c,d;
//	 double SMALLEST=0.00000001;
	 for (i=0;i<n;i++) {
		 for (j=0; j<n;j++)
			 *(b+i*n+j) = 0.;
		 *(b+i*n+i) = 1.0;
	 }
	 for (l=0;l<n;l++){
		 d = fabs((*(a+l*n+l)));
		 if( d<SMALLEST ){
			 l1 = l;
			 d = fabs((*(a+l1*n+l)));
			 while (d < SMALLEST && (++l1<n))
				 d = fabs((*(a+l1*n+l)));
			 if (l1>=n)
				 return -1;
			 for (j=0;j<n;j++){
				 *(a+l*n+j) += (*(a+l1*n+j));
				 *(b+l*n+j) += (*(b+l1*n+j));
			 }
		 }
		 c = 1.0/(*(a+l*n+l));
		 for (j=l;j<n;j++) *(a+l*n+j) *= c;
		 for (j=0;j<n;j++) *(b+l*n+j) *= c;
		 k = l+1;
		 for (i=k;i<n;i++){
			 c = *(a+i*n+l);
			 for (j=l;j<n;j++) *(a+i*n+j) -= (*(a+l*n+j) * c);
			 for (j=0;j<n;j++) *(b+i*n+j) -= (*(b+l*n+j) * c);
		 }
	 }
	 for (l=n-1; l>=0;l--){
		 for (k=1;k<=l;k++){
			 i = l-k;
			 c = *(a+i*n+l);
			 for (j=l;j<n;j++) *(a+i*n+j) -= (*(a+l*n+j) * c);
			 for (j=0;j<n;j++) *(b+i*n+j) -= (*(b+l*n+j) * c);
		 }
	 }
	 return 0;
}
//Line Fitting
//ty=ax+b
//dataN : 주어진 (x,y)수
//*x, *y; (x,y)좌표 
//removeN : LineFitting후 Line에서 가장 멀리있는 점을 없애고 다시 fitting울 반복한다.
//          이 반복횟수를 나타낸다.
//ErrorLimit: LineFitting후 Line에서 가장 멀리있는 점이 이값 이하이면 반복을 중지한다.
//결과값 return: 0: 정상수행
//				 <0 : FAILURE
//              *t, *a, *b (ty=ax+b)

int CgProcess::LineFitting(int dataN, double *x, double *y, int removeN, double ErrorLimit,
				 double *t, double *a, double *b)
{
	int k,i,j, nResult=0;
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
		if(fabs(dd)<ZeroData)
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
			dist=fabs(-*(y+j)+*a**(x+j)+*b);
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
	return nResult;
}

int CgProcess::LineFitting2(int dataN, double *x, double *y, int removeN, double ErrorLimit,
				 double *t, double *a, double *b)
{
	int k, nResult=0;
	double dAvgX=0.,dAvgY=0.;

	for(k=0;k<dataN;k++)
	{
		dAvgX += *(x+k);
		dAvgY += *(y+k);
	}
	dAvgX /= dataN;
	dAvgY /= dataN;

	double a_low=0.,a_high=0.;
	for(k=0;k<dataN;k++)
	{
		a_high += (*(y+k)-dAvgY)*(*(x+k)-dAvgX);
		a_low += (*(x+k)-dAvgX)*(*(x+k)-dAvgX);
	}

	*a = a_high/a_low;
	*b = dAvgY - (*a)*dAvgX;


	return nResult;
}