// Image.h : header file


#ifndef __IMAGE_H__
#define __IMAGE_H__

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

class CImage
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
	CImage();
    CImage( CImage & );
	BOOL InitDIB(BOOL bCreatePalette = TRUE);
	void SetHandle(HANDLE hHandle);
	BOOL Create(int width, int height, int depth);
	BOOL CreateDIBPalette();

	// �Ҹ���, ���� �Լ�
	virtual ~CImage();
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
	LPBYTE GetImagePtr1();
	void	SetImage(unsigned char* fm);	


	// �׸���
	BOOL Draw(HDC hDC, LPRECT sourceRect, LPRECT destRect);
	
	// ���� �о����, �����ϱ�
	BOOL Save(LPCTSTR lpszFileName);
	BOOL Load(LPCTSTR lpszFileName);

protected:
	// ���� ���� �о����
	BOOL LoadBMP(LPCTSTR lpszFileName);
//	BOOL LoadJPG(LPCTSTR lpszFileName);
//	BOOL LoadGIF(LPCTSTR lpszFileName);
//	BOOL LoadTIF(LPCTSTR lpszFileName);

	// ���� ���� �����ϱ�
	BOOL SaveBMP(LPCTSTR lpszFileName);
//	BOOL SaveJPG(LPCTSTR lpszFileName);
//	BOOL SaveGIF(LPCTSTR lpszFileName);
//	BOOL SaveTIF(LPCTSTR lpszFileName);
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
#endif