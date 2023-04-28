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

//BMP ���� 
//BITMAPFILEHEADER  ---Image Save�Ҷ� ���Ѿ �����Ѵ�.
//BITMAPINFOHEADER  ---|
//BITMAP DATA		---|>	m_hImage
// 
//#pragma comment(lib, "Jpeg.lib")
class CImg
{
/******************************************************
						��� ����
******************************************************/
protected:
	LPVOID m_hImage;	//image handle 
	CSize m_Size;		//image size
	CPalette * m_pPal;	//image palette
	unsigned char* m_fm;
	
	CString m_filename;

/******************************************************
						��� �Լ�
******************************************************/
public:

	CString GetFileName();
	// ������, �ʱ�ȭ �Լ�
	CImg();
    CImg( CImg & );
	BOOL InitDIB(BOOL bCreatePalette = TRUE);
	void SetHandle(HANDLE hHandle);
	BOOL Create(int width, int height, int depth);
	BOOL CreateDIBPalette();

	// �Ҹ���, ���� �Լ�
	virtual ~CImg();
	void Free();


	// �̹��� ������ ��� �Լ�
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


	// �׸���
	BOOL Draw(HDC hDC, LPRECT sourceRect, LPRECT destRect);
	
	// ���� �о����, �����ϱ�
	BOOL Save(LPCTSTR lpszFileName, int nJpegQual=70, BOOL bupper=TRUE);
	BOOL Load(LPCTSTR lpszFileName,BOOL bupper=TRUE);

protected:
	int			m_nBands;
	// ���� ���� �о����
	BOOL LoadBMP(LPCTSTR lpszFileName);
//	BOOL LoadGIF(LPCTSTR lpszFileName);
//	BOOL LoadTIF(LPCTSTR lpszFileName);
	BOOL LoadJPEG(LPCTSTR lpszFileName);

	// ���� ���� �����ϱ�
	BOOL SaveBMP(LPCTSTR lpszFileName);
//	BOOL SaveGIF(LPCTSTR lpszFileName);
//	BOOL SaveTIF(LPCTSTR lpszFileName);
	BOOL SaveJPEG(LPCTSTR lpszFileName, int nJpegQual);
};

/******************************************************
						���� �Լ�
******************************************************/
LPSTR WINAPI FindDIBBits(LPSTR lpbi);
DWORD WINAPI DIBWidth(LPSTR lpDIB);
DWORD WINAPI DIBHeight(LPSTR lpDIB);
WORD WINAPI PaletteSize(LPSTR lpbi);
WORD WINAPI DIBNumColors(LPSTR lpbi);
/////////////////////////////////////////////////////////////////////////////
#endif#pragma once


// CIMG

