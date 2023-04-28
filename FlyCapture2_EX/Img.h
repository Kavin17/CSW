// Img.h : header file


#ifndef __IMG_H__
#define __IMG_H__

#define HDIB HGLOBAL

// DIB constants
#define PALVERSION   0x300

// DIB Macros
#define IS_WIN30_DIB(lpbi) ((*(LPDWORD)(lpbi)) == sizeof(BITMAPINFOHEADER))
#define RECTWIDTH(lpRect)  ((lpRect)->right - (lpRect)->left)
#define RECTHEIGHT(lpRect) ((lpRect)->bottom - (lpRect)->top)
#define WIDTHBYTES(bits)   (((bits) + 31) / 32 * 4)

//BMP 파일 
//BITMAPFILEHEADER  ---Image Save할때 만둘어서 저장한다.
//BITMAPINFOHEADER  ---|
//BITMAP DATA		---|>	m_hImage
// 
//#pragma comment(lib, "Jpeg.lib")
class CImg
{
/******************************************************
						멤버 변수
******************************************************/
protected:
	LPVOID m_hImage;	//image handle 
	CSize m_Size;		//image size
	CPalette * m_pPal;	//image palette
	unsigned char* m_fm;
	
	CString m_filename;

/******************************************************
						멤버 함수
******************************************************/
public:

	CString GetFileName();
	// 생성자, 초기화 함수
	CImg();
    CImg( CImg & );
	BOOL InitDIB(BOOL bCreatePalette = TRUE);
	void SetHandle(HANDLE hHandle);
	BOOL Create(int width, int height, int depth);
	BOOL CreateDIBPalette();

	// 소멸자, 정리 함수
	virtual ~CImg();
	void Free();


	// 이미지 정보를 얻는 함수
	int GetBitCount();	  //Image Depth 
	LPVOID GetHandle()		{return m_hImage;}
	BOOL IsDataNull()		{return (m_hImage == NULL);}
	CSize GetSize()			{return m_Size;}	
	int GetHeight()			{return m_Size.cy;}
	int GetWidth()			{return m_Size.cx;}
	int GetRealWidth()		{return WIDTHBYTES((GetWidth()*GetBitCount()));}
	CPalette *GetPalette()	{return m_pPal;}
	LPBYTE GetImagePtr();
	void	SetImage(unsigned char* fm);	


	// 그리기
	BOOL Draw(HDC hDC, LPRECT sourceRect, LPRECT destRect);
	
	// 파일 읽어오기, 저장하기
	BOOL Save(LPCTSTR lpszFileName, int nJpegQual=70, BOOL bupper=TRUE);
	BOOL Load(LPCTSTR lpszFileName,BOOL bupper=TRUE);

protected:
	int			m_nBands;
	// 실제 파일 읽어오기
	BOOL LoadBMP(LPCTSTR lpszFileName);
//	BOOL LoadGIF(LPCTSTR lpszFileName);
//	BOOL LoadTIF(LPCTSTR lpszFileName);
	BOOL LoadJPEG(LPCTSTR lpszFileName);

	// 실제 파일 저장하기
	BOOL SaveBMP(LPCTSTR lpszFileName);
//	BOOL SaveGIF(LPCTSTR lpszFileName);
//	BOOL SaveTIF(LPCTSTR lpszFileName);
	BOOL SaveJPEG(LPCTSTR lpszFileName, int nJpegQual);
};

/******************************************************
						전역 함수
******************************************************/
LPSTR WINAPI FindDIBBits(LPSTR lpbi);
DWORD WINAPI DIBWidth(LPSTR lpDIB);
DWORD WINAPI DIBHeight(LPSTR lpDIB);
WORD WINAPI PaletteSize(LPSTR lpbi);
WORD WINAPI DIBNumColors(LPSTR lpbi);
/////////////////////////////////////////////////////////////////////////////
#endif#pragma once


// CIMG

