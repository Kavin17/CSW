#ifndef __WINSERIAL_H__
#define __WINSERIAL_H__

#ifdef NESERIALDLL_EXPORTS
#define __NESERIAL_EXPORT __declspec(dllexport)
#else
#define __NESERIAL_EXPORT
#endif

#define		MAX_STREAM		1024

class __NESERIAL_EXPORT CComPort
{
public :
	CComPort();
	virtual ~CComPort();

	BOOL Open(int nPort);
	void Close();
	BOOL SetPortSetting(int nBaud, int nDataBit, int nStopBit, int nParity);
	void GetPortSetting(int *nBaud, int *nDataBit, int *nStopBit, int *nParity);
	unsigned long Read(char *pBuffer, int nLength);
	unsigned long Write(const char *pBuffer, int nLength);
	unsigned long Read(unsigned char *pBuffer, int nLength);
	unsigned long Write(const unsigned char *pBuffer, int nLength);

	void ClearRXBuffer();

	BOOL IsOpened();

protected:
	BOOL m_bIsOpened;
private:
	HANDLE m_hCOM;
	DCB m_stDCB;

};

#endif