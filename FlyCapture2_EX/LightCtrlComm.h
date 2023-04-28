#if !defined(AFX_LIGHTCTRLCOMM_H__6F0D13F4_1490_4B16_AFCC_0BE19AB2AA9A__INCLUDED_)
#define AFX_LIGHTCTRLCOMM_H__6F0D13F4_1490_4B16_AFCC_0BE19AB2AA9A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


/////////////////////////////////////////////////////////////////////////////
// 
#define WM_RECEIVEDATA	WM_USER+13

class CFormTab3; 

class CLightCtrlComm : public CWnd
{
public:
	HWND m_hwnd;

	bool Open(CString ComPort);
	void Close();

	void TxData(LPSTR TxChar, int len);
	void WriteString(CString strString); 
	
	CLightCtrlComm();
	virtual ~CLightCtrlComm();
	bool GetConnected() { return m_bConnected; };
protected:
	HANDLE m_ComHandle;
	CWinThread* m_pComThread;
	static UINT ComThread(LPVOID pParam);

	CString m_strPort;
	bool m_bConnected;
};

/////////////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_LIGHTCTRLCOMM_H__6F0D13F4_1490_4B16_AFCC_0BE19AB2AA9A__INCLUDED_)
