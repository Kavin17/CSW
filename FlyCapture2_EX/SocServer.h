#if !defined(AFX_SOCSERVER_H__EBA31B2F_5B3F_11D5_95D8_0050DA8BB346__INCLUDED_)
#define AFX_SOCSERVER_H__EBA31B2F_5B3F_11D5_95D8_0050DA8BB346__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SocServer.h : header file
//

#include "SocCom.h"


/////////////////////////////////////////////////////////////////////////////
// CSocServer command target

class CSocServer : public CSocket
{
// Attributes
public:

// Operations
public:
	CSocServer();
	virtual ~CSocServer();

	CSocCom* m_pSocCom; // 접속 요청이 왔을때 연결한 소켓
	HWND m_hWnd;
	
	void		Init(HWND hWnd, int nIndex);
	CSocCom*	GetAcceptSocCom();
	void		ReconnectSocket();
	void		CloseSocket();

	int m_nIndex;
	BOOL m_serverAccept;
	BOOL m_serverAccpetAlramOn;

// Overrides
public:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSocServer)
	public:
	virtual void OnAccept(int nErrorCode);
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CSocServer)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation
protected:
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SOCSERVER_H__EBA31B2F_5B3F_11D5_95D8_0050DA8BB346__INCLUDED_)
