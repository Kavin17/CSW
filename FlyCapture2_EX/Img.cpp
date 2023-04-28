// Img.cpp : implementation file
//

#include "stdafx.h"
#include "Img.h"
#include "turbojpeg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE 
static char THIS_FILE[] = __FILE__;
#endif

#define DIB_HEADER_MARKER   ((WORD) ('M' << 8) | 'B')

/******************************************************
				������, �ʱ�ȭ �Լ�
******************************************************/

CImg::CImg()
{
	m_Size		 = CSize(1,1);
	m_hImage	 = NULL;
	m_pPal		 = NULL;
	m_nBands = 1;
}

CImg::CImg( CImg &Src )
{
	m_Size		 = CSize(1,1);
	m_hImage	 = NULL;
	m_pPal		 = NULL;
	m_nBands = 1;
	*this = Src;
}

CImg::~CImg()
{ 
	Free(); 
}

CString CImg::GetFileName()
{
	return m_filename;
}

BOOL CImg::InitDIB(BOOL bCreatePalette)
{
	// �̹����� ����, ���� ũ�� ����
	LPSTR pDIB = (LPSTR) m_hImage;
	m_Size = CSize((int) ::DIBWidth(pDIB), (int) ::DIBHeight(pDIB));

	if(m_Size.cx < 1000 || m_Size.cy <1000)
		return FALSE;
	if(bCreatePalette)
	{
		if(m_pPal != NULL) delete m_pPal;
		// �ȷ�Ʈ ����
		m_pPal = new CPalette;
		if (CreateDIBPalette() == NULL)
		{
			// �ȷ�Ʈ�� ������ �ʴ� ���
			delete m_pPal;
			m_pPal = NULL;
			return FALSE;
		}
	}
	return TRUE;
}

void CImg::SetHandle(HANDLE hHandle)
{
	m_hImage = (LPVOID)hHandle;
} 


LPBYTE CImg::GetImagePtr()
{
	LPBITMAPINFOHEADER lpBI;
	DWORD nSize;

	// �޸� ����
	lpBI = (LPBITMAPINFOHEADER)m_hImage;
	nSize= lpBI->biSize + PaletteSize((LPSTR)lpBI);

	return (LPBYTE)((LPBYTE)m_hImage+nSize);
}

void	SetImage(unsigned char* fm)
{
//	memmove(fm, m_fm, size...);
}


BOOL CImg::Create(int width, int height, int depth)
{
    LPBITMAPINFOHEADER lpbi ;
	BYTE		*lpPal;
    DWORD       dwSizeImage, nSize;
    int         i;

	m_Size.cx=width;
	m_Size.cy=height;

    dwSizeImage = height*(DWORD)((width*depth/8+3)&~3);

	if(depth == 24)
	{
		nSize=sizeof(BITMAPINFOHEADER)+dwSizeImage;
		m_nBands = 3;
	}
    else
	{
		nSize=sizeof(BITMAPINFOHEADER)+dwSizeImage + 1024;
		m_nBands = 1;
	}
	m_hImage= (LPVOID)new BYTE[nSize];

    if (m_hImage == NULL)	
        return FALSE;

	lpbi = (LPBITMAPINFOHEADER)m_hImage;
	lpbi->biSize            = sizeof(BITMAPINFOHEADER) ;
    lpbi->biWidth           = width;
    lpbi->biHeight          = height;
    lpbi->biPlanes          = 1;
    lpbi->biBitCount        = depth;
    lpbi->biCompression     = BI_RGB ;
    lpbi->biSizeImage       = dwSizeImage;
    lpbi->biXPelsPerMeter   = 0 ;
    lpbi->biYPelsPerMeter   = 0 ;
    lpbi->biClrUsed         = 0 ;
    lpbi->biClrImportant    = 0 ;

	lpPal = (BYTE *) lpbi;
	if (depth == 8)
	{
		lpbi->biClrUsed = 256;

		DWORD offDest = sizeof(BITMAPINFOHEADER);
		for(i = 0; i < 256; i++)
		{
			lpPal[offDest++] = (BYTE)i;
			lpPal[offDest++] = (BYTE)i;
			lpPal[offDest++] = (BYTE)i;
			lpPal[offDest++] = 0x00;
		}                  
	}
//bitmap depth4
	else if(depth == 4)
	{
		lpbi->biClrUsed = 32;

		DWORD offDest = sizeof(BITMAPINFOHEADER);
		for(i = 0; i < 32; i++)
		{
			lpPal[offDest++] = (BYTE)i;
			lpPal[offDest++] = (BYTE)i;
			lpPal[offDest++] = (BYTE)i;
			lpPal[offDest++] = 0x00;
		}                  
	}

	InitDIB(FALSE);
	return TRUE;
}

BOOL CImg::CreateDIBPalette()
{
	LPLOGPALETTE lpPal;      // pointer to a logical palette
	HANDLE hLogPal;          // handle to a logical palette
	HPALETTE hPal = NULL;    // handle to a palette
	int i;                   // loop index
	WORD wNumColors;         // number of colors in color table
	LPSTR lpbi;              // pointer to packed-DIB
	LPBITMAPINFO lpbmi;      // pointer to BITMAPINFO structure (Win3.0)
	LPBITMAPCOREINFO lpbmc;  // pointer to BITMAPCOREINFO structure (old)
	BOOL bWinStyleDIB;       // flag which signifies whether this is a Win3.0 DIB
	BOOL bResult = FALSE;

	/* if handle to DIB is invalid, return FALSE */

	if (m_hImage == NULL)
	  return FALSE;

   lpbi = (LPSTR)m_hImage;

   /* get pointer to BITMAPINFO (Win 3.0) */
   lpbmi = (LPBITMAPINFO)lpbi;

   /* get pointer to BITMAPCOREINFO (old 1.x) */
   lpbmc = (LPBITMAPCOREINFO)lpbi;

   /* get the number of colors in the DIB */
   wNumColors = ::DIBNumColors(lpbi);

   if (wNumColors != 0)
   {
		/* allocate memory block for logical palette */
		hLogPal = new BYTE[sizeof(LOGPALETTE)
									+ sizeof(PALETTEENTRY)
									* wNumColors];

		/* if not enough memory, clean up and return NULL */
		if (hLogPal == 0)
		{
			return FALSE;
		}

		lpPal = (LPLOGPALETTE) hLogPal;

		/* set version and number of palette entries */
		lpPal->palVersion = PALVERSION;
		lpPal->palNumEntries = (WORD)wNumColors;

		/* is this a Win 3.0 DIB? */
		bWinStyleDIB = IS_WIN30_DIB(lpbi);
		for (i = 0; i < (int)wNumColors; i++)
		{
			if (bWinStyleDIB)
			{
				lpPal->palPalEntry[i].peRed = lpbmi->bmiColors[i].rgbRed;
				lpPal->palPalEntry[i].peGreen = lpbmi->bmiColors[i].rgbGreen;
				lpPal->palPalEntry[i].peBlue = lpbmi->bmiColors[i].rgbBlue;
				lpPal->palPalEntry[i].peFlags = 0;
			}
			else
			{
				lpPal->palPalEntry[i].peRed = lpbmc->bmciColors[i].rgbtRed;
				lpPal->palPalEntry[i].peGreen = lpbmc->bmciColors[i].rgbtGreen;
				lpPal->palPalEntry[i].peBlue = lpbmc->bmciColors[i].rgbtBlue;
				lpPal->palPalEntry[i].peFlags = 0;
			}
		}

		/* create the palette and get handle to it */
		bResult = m_pPal->CreatePalette(lpPal);
		
		delete hLogPal;
	}

	return bResult;
}

/******************************************************
				�Ҹ���, ���� �Լ�
******************************************************/

void CImg::Free()
{
	if(m_hImage)
	{
		delete m_hImage ;
		m_hImage = NULL;
	}

	if(m_pPal)
	{
		delete m_pPal;
		m_pPal = NULL;
	}
}

/******************************************************
				�̹��� ������ ��� �Լ�
******************************************************/

int CImg::GetBitCount()
{
	if (m_hImage == NULL) return -1;
	LPBITMAPINFOHEADER lpbi;
	lpbi = (LPBITMAPINFOHEADER)m_hImage;
	return lpbi->biBitCount;
}



/******************************************************
						�׸���
******************************************************/

BOOL CImg::Draw(HDC hDC, LPRECT lpDIBRect, LPRECT lpDCRect)
{
	LPSTR	lpDIBHdr;	// BITMAPINFOHEADER�� ����ų ������
	LPSTR	lpDIBBits;	// DIB ��Ʈ�� ����ų ������
	BOOL		bSuccess=FALSE;	 // Success/fail �÷���
	HPALETTE 	hPal=NULL;		 // DIB �ȷ�Ʈ
	HPALETTE 	hOldPal=NULL;	 // ���� �ȷ�Ʈ

//Image Flip----------------------------------------------
//	int nTmp = lpDCRect->top;
//	lpDCRect->top = lpDCRect->bottom;
//	lpDCRect->bottom = nTmp;
//--------------------------------------------------------
	
	lpDIBHdr  = (LPSTR)m_hImage;
	// DIB ��Ʈ�� ����Ǿ� �ִ� ���� �ּҸ� ����
	lpDIBBits = ::FindDIBBits(lpDIBHdr);

	// �ȷ�Ʈ�� ��� DC�� ����
	if(m_pPal != NULL)
	{
		hPal = (HPALETTE) m_pPal->m_hObject;
		hOldPal = ::SelectPalette(hDC, hPal, TRUE);
	}

	::SetStretchBltMode(hDC, COLORONCOLOR);

/* --------------------------------------------------------------------------------------
	if ((RECTWIDTH(lpDCRect)  == RECTWIDTH(lpDIBRect)) &&
	   (RECTHEIGHT(lpDCRect) == RECTHEIGHT(lpDIBRect)))
		// ���� ũ��� �״�� ����ϴ� ���
		bSuccess = ::SetDIBitsToDevice(hDC, // hDC
			lpDCRect->left,		 			// DestX
			lpDCRect->top,		 			// DestY
			RECTWIDTH(lpDCRect),	 		// nDestWidth
			RECTHEIGHT(lpDCRect),			// nDestHeight
			lpDIBRect->left,		 		// SrcX
			(int)DIBHeight(lpDIBHdr) - lpDIBRect->top -	RECTHEIGHT(lpDIBRect),   		// SrcY
			0,                          	// nStartScan
			(WORD)DIBHeight(lpDIBHdr),  	// nNumScans
			lpDIBBits,                  	// lpBits
			(LPBITMAPINFO)lpDIBHdr,			// lpBitsInfo
			DIB_RGB_COLORS);				// wUsage
	else	// Ȯ�� �Ǵ� ����Ͽ� ����ϴ� ���
		bSuccess = ::StretchDIBits(hDC, 	// hDC
			lpDCRect->left,					// DestX
			lpDCRect->top,					// DestY
			RECTWIDTH(lpDCRect),			// nDestWidth
			RECTHEIGHT(lpDCRect),			// nDestHeight
			lpDIBRect->left,				// SrcX
			lpDIBRect->top,					// SrcY
			RECTWIDTH(lpDIBRect),			// wSrcWidth
			RECTHEIGHT(lpDIBRect),			// wSrcHeight
			lpDIBBits,						// lpBits
			(LPBITMAPINFO)lpDIBHdr,			// lpBitsInfo
			DIB_RGB_COLORS,					// wUsage
			SRCCOPY);						// dwROP
--------------------------------------------------------------------------------------------*/
	bSuccess = ::StretchDIBits(hDC, 	// hDC
		lpDCRect->left,					// DestX
		lpDCRect->top,					// DestY
		RECTWIDTH(lpDCRect),			// nDestWidth
		RECTHEIGHT(lpDCRect),			// nDestHeight
		lpDIBRect->left,				// SrcX
		lpDIBRect->top,					// SrcY
		RECTWIDTH(lpDIBRect),			// wSrcWidth
		RECTHEIGHT(lpDIBRect),			// wSrcHeight
		lpDIBBits,						// lpBits
		(LPBITMAPINFO)lpDIBHdr,			// lpBitsInfo
		DIB_RGB_COLORS,					// wUsage
		SRCCOPY);						// dwROP


			
	// DC ����
	if (hOldPal != NULL) ::SelectPalette(hDC, hOldPal, TRUE);
	return bSuccess;
}

/******************************************************
				���� �о����, �����ϱ�
******************************************************/

BOOL CImg::Save(LPCTSTR lpszFileName, int nJpegQual, BOOL bupper)
{
	CString filetype;
	filetype = lpszFileName;
	if(bupper)
		filetype.MakeUpper();

	if(filetype.Find(_T(".BMP")) > -1) return SaveBMP(lpszFileName);
//	else if(filetype.Find(".TIF") > -1) return SaveTIF(lpszFileName);
//	else if(filetype.Find(".GIF") > -1) return SaveGIF(lpszFileName);
	else if(filetype.Find(".JPG") > -1) return SaveJPEG(lpszFileName, nJpegQual);
	else return FALSE;
}

BOOL CImg::Load(LPCTSTR lpszFileName,BOOL bupper)
{
	m_filename = lpszFileName;
	if(bupper)
		m_filename.MakeUpper();
	if(bupper){
		if(m_filename.Find(_T(".BMP")) > -1) return LoadBMP(lpszFileName);
	//	else if(filetype.Find(".TIF") > -1) return LoadTIF(lpszFileName);
	//	else if(filetype.Find(".GIF") > -1) return LoadGIF(lpszFileName);
		else if(m_filename.Find(".JPG") > -1) return LoadJPEG(lpszFileName);
	}
	else
		if(m_filename.Find(_T(".bmp")) > -1) return LoadBMP(lpszFileName);

	else return FALSE;
}

/******************************************************
  ���� �׷��� ������ �о���� ��ƾ�� ���� ���Ͽ� ����
 
	ImageFileBmp.cpp : BMP ���� (LoadBMP, SaveBMP)
	ImageFileGif.cpp : GIF ���� (LoadGIF, SaveGIF)
	ImageFileTif.cpp : TIFF ���� (LoadTIF, SaveTIF)

******************************************************/


/******************************************************
				DIB�� ���õ� ���� �Լ�
******************************************************/

LPSTR WINAPI FindDIBBits(LPSTR lpbi)
{
	return (lpbi + *(LPDWORD)lpbi + ::PaletteSize(lpbi));
}


DWORD WINAPI DIBWidth(LPSTR lpDIB)
{
	LPBITMAPINFOHEADER lpbmi;  // pointer to a Win 3.0-style DIB
	LPBITMAPCOREHEADER lpbmc;  // pointer to an other-style DIB

	/* point to the header (whether Win 3.0 and old) */

	lpbmi = (LPBITMAPINFOHEADER)lpDIB;
	lpbmc = (LPBITMAPCOREHEADER)lpDIB;

	/* return the DIB width if it is a Win 3.0 DIB */
	if (IS_WIN30_DIB(lpDIB))
		return lpbmi->biWidth;
	else  /* it is an other-style DIB, so return its width */
		return (DWORD)lpbmc->bcWidth;
}


DWORD WINAPI DIBHeight(LPSTR lpDIB)
{
	LPBITMAPINFOHEADER lpbmi;  // pointer to a Win 3.0-style DIB
	LPBITMAPCOREHEADER lpbmc;  // pointer to an other-style DIB

	/* point to the header (whether old or Win 3.0 */

	lpbmi = (LPBITMAPINFOHEADER)lpDIB;
	lpbmc = (LPBITMAPCOREHEADER)lpDIB;

	/* return the DIB height if it is a Win 3.0 DIB */
	if (IS_WIN30_DIB(lpDIB))
		return lpbmi->biHeight;
	else  /* it is an other-style DIB, so return its height */
		return (DWORD)lpbmc->bcHeight;
}



WORD WINAPI PaletteSize(LPSTR lpbi)
{
	/* calculate the size required by the palette */
	if (IS_WIN30_DIB (lpbi))
		return (WORD)(::DIBNumColors(lpbi) * sizeof(RGBQUAD));
	else
		return (WORD)(::DIBNumColors(lpbi) * sizeof(RGBTRIPLE));
}



WORD WINAPI DIBNumColors(LPSTR lpbi)
{
	WORD wBitCount;  // DIB bit count

	/*  If this is a Windows-style DIB, the number of colors in the
	 *  color table can be less than the number of bits per pixel
	 *  allows for (i.e. lpbi->biClrUsed can be set to some value).
	 *  If this is the case, return the appropriate value.
	 */

	if (IS_WIN30_DIB(lpbi))
	{
		DWORD dwClrUsed;

		dwClrUsed = ((LPBITMAPINFOHEADER)lpbi)->biClrUsed;
		if (dwClrUsed != 0)
			return (WORD)dwClrUsed;
	}

	/*  Calculate the number of colors in the color table based on
	 *  the number of bits per pixel for the DIB.
	 */
	if (IS_WIN30_DIB(lpbi))
		wBitCount = ((LPBITMAPINFOHEADER)lpbi)->biBitCount;
	else
		wBitCount = ((LPBITMAPCOREHEADER)lpbi)->bcBitCount;

	/* return number of colors based on bits per pixel */
	switch (wBitCount)
	{
		case 1:
			return 2;

		case 4:
			return 16;

		case 8:
			return 256;

		default:
			return 0;
	}
}



BOOL CImg::SaveBMP(LPCTSTR lpszFileName)
{
	CFile file;
	CFileException fe;
	BITMAPFILEHEADER bmfHdr;
	LPBITMAPINFOHEADER lpBI;
	DWORD dwDIBSize;
	

	// ���� ���� ���� ����
	if (!file.Open(lpszFileName, CFile::modeCreate | CFile::modeWrite | CFile::shareDenyWrite, &fe)) return FALSE;
	
	// �޸� �ڵ��� ��ȿ���� Ȯ��
	if (m_hImage == NULL) return FALSE;

	lpBI = (LPBITMAPINFOHEADER)m_hImage;
	if (lpBI == NULL) return FALSE;

	// ��Ʈ�� ���� ��� ������ ����
	bmfHdr.bfType = DIB_HEADER_MARKER;  // "BM"
	dwDIBSize = *(LPDWORD)lpBI + ::PaletteSize((LPSTR)lpBI);
	if((lpBI->biCompression==BI_RLE8) || (lpBI->biCompression==BI_RLE4))
		dwDIBSize += lpBI->biSizeImage;
	else 
	{
		DWORD dwBmBitsSize;  // Size of Bitmap Bits only
		dwBmBitsSize = WIDTHBYTES((lpBI->biWidth)*((DWORD)lpBI->biBitCount)) * lpBI->biHeight;
		dwDIBSize += dwBmBitsSize;
		lpBI->biSizeImage = dwBmBitsSize;
	}

	bmfHdr.bfSize = dwDIBSize + sizeof(BITMAPFILEHEADER);
	bmfHdr.bfReserved1 = 0;
	bmfHdr.bfReserved2 = 0;
	bmfHdr.bfOffBits=(DWORD)sizeof(BITMAPFILEHEADER)+lpBI->biSize + PaletteSize((LPSTR)lpBI);
	TRY
	{
		// ��Ʈ�� ���� ����� ���Ͽ� ����
		file.Write((LPSTR)&bmfHdr, sizeof(BITMAPFILEHEADER));
		// ������ �����͸� ���Ͽ� ����
//		file.Write((LPSTR)&lpBI, bmfHdr.bfOffBits - sizeof (BITMAPFILEHEADER));
//		file.Write((LPVOID)m_lpImage, dwDIBSize-bmfHdr.bfOffBits+sizeof (BITMAPFILEHEADER));
		file.Write(lpBI, dwDIBSize);
	}
	CATCH (CFileException, e)
	{
		THROW_LAST();
	}
	END_CATCH
	
	return TRUE;
}


BOOL CImg::LoadBMP(LPCTSTR lpszFileName)
{
	CFile file;
	CFileException fe;
	LPSTR pDIB;
	DWORD dwBitsSize;
	BITMAPFILEHEADER bmfHeader;

	// �б� ���� ���� ����
	if(!file.Open(lpszFileName, CFile::modeRead|CFile::shareDenyWrite, &fe))
		return FALSE;

	// ������ ���̸� ����
	dwBitsSize = (DWORD)file.GetLength();

	// ���� ��� �б�
	if(file.Read((LPSTR)&bmfHeader, sizeof(bmfHeader))!=sizeof(bmfHeader))
		return FALSE;

	// BMP �������� ��Ÿ���� "BM" ��Ŀ�� �ִ��� Ȯ��
	if (bmfHeader.bfType != DIB_HEADER_MARKER)
		return FALSE;

	// �޸� �Ҵ�
	if(m_hImage)
		delete [] m_hImage;
	m_hImage = (LPVOID)new BYTE[dwBitsSize];
	if(m_hImage== NULL) return FALSE;

	
	pDIB = (LPSTR) m_hImage;

	// ���� �б�
	if (file.Read(pDIB, dwBitsSize - sizeof(BITMAPFILEHEADER)) != dwBitsSize - sizeof(BITMAPFILEHEADER) ) 
	{	
		delete  m_hImage;
		return FALSE;
	}

	// DIB �ʱ�ȭ
	InitDIB();

	return TRUE;
}

BOOL CImg::SaveJPEG(LPCTSTR lpszFileName, int nJpegQual)
{
	unsigned char *srcBuf; //passed in as a param containing pixel data in RGB pixel interleaved format
	tjhandle handle = tjInitCompress();
	if(handle == NULL)
	{
		return -1;
	}
	int jpegQual = nJpegQual;
	int width = GetWidth();//m_nWidth;//CAM_LINE_WIDTH_16384;
	int height = GetHeight();//m_nHeight;// CAM_LINE_HEIGHT_4096;
	int nbands = m_nBands;
	int flags = 0;
	unsigned char* jpegBuf = NULL;
	int pitch = width * nbands;
	int pixelFormat = TJPF_GRAY;
	int jpegSubsamp = TJSAMP_GRAY;
	if(nbands == 3)
	{
		pixelFormat = TJPF_BGR; //TJPF_RGB;
		jpegSubsamp = TJSAMP_411;
	}

	unsigned long jpegSize = 0;
	srcBuf = GetImagePtr();//new unsigned char[width * height * nbands];
	int tj_stat = tjCompress2( handle, srcBuf, width, pitch, height,
		pixelFormat, &(jpegBuf), &jpegSize, jpegSubsamp, jpegQual, flags);
	if(tj_stat != 0)
	{
		tjDestroy(handle);
		handle = NULL;
		tjFree(jpegBuf);
		return -1;
	}

	FILE *fp;
	fp= fopen(lpszFileName, "wb");
	if (!fp) {
		tjFree(jpegBuf);
		handle = NULL;
		return -1;
	}
	if (fwrite(jpegBuf, jpegSize, 1, fp) < 1) {
		tjFree(jpegBuf);
		handle = NULL;
		return -1;
	}
	fclose(fp);
	int tjstat = tjDestroy(handle); //should deallocate data buffer
	handle = 0;
	tjFree(jpegBuf);
	return TRUE;
}

BOOL CImg::LoadJPEG(LPCTSTR lpszFileName)
{
	long unsigned int _jpegSize; //!< _jpegSize from above
	unsigned char* _compressedImage; //!< _compressedImage from above

	int jpegSubsampt;
	int ntest = GetBitCount();
	int width = GetWidth();
	int height = GetHeight();
	int nbands = m_nBands;
	int nSize = width*height*nbands;
	_jpegSize = nSize;
	int pixelFormat = TJPF_GRAY;
	if(nbands == 3)
		pixelFormat = TJPF_BGR; //TJPF_RGB;

	unsigned char *buffer = new unsigned char[width*height*nbands];//=GetImagePtr();// new unsigned char[width*height*nbands]; //!< will contain the decompressed image

	_compressedImage = new unsigned char[width*height*nbands];
	FILE * infile;        /* source file */

	if ((infile = fopen(lpszFileName, "rb")) == NULL) {
		delete _compressedImage;
		delete buffer;
		return 0;
	}

	fread(_compressedImage,1,nSize,infile);
	fclose(infile);

	tjhandle _jpegDecompressor = tjInitDecompress();
	if(_jpegDecompressor == NULL)return 0;

	tjDecompressHeader2(_jpegDecompressor, _compressedImage, _jpegSize, &width, &height, &jpegSubsampt);

	jpegSubsampt = TJSAMP_GRAY;
	tjDecompress2(_jpegDecompressor, _compressedImage, _jpegSize, buffer, width, 0, height, pixelFormat, TJFLAG_FASTDCT);//Buffer�� �����ͷ� ����ϳ�?

	tjDestroy(_jpegDecompressor);
	memcpy(GetImagePtr(),buffer,nSize);

	delete _compressedImage;
	delete buffer;

	return TRUE;
}