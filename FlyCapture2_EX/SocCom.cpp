// SocCom.cpp : implementation file
//
#include "afxwin.h"
#include "stdafx.h"
#include "SocCom.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSocCom

CSocCom::CSocCom()
{
}

CSocCom::~CSocCom()
{
}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CSocCom, CSocket)
	//{{AFX_MSG_MAP(CSocCom)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CSocCom member functions

void CSocCom::Init(HWND hWnd, int nIndex)
{
	m_hWnd = hWnd;
	m_nIndex = nIndex;
	//recvTrg = FALSE;
	//recvEnd = TRUE;
}

void CSocCom::OnReceive(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(m_nIndex == 0)
		SendMessage(m_hWnd, UM_RECEIVE1, m_nIndex, 0);
	else if(m_nIndex == 1)
		SendMessage(m_hWnd, UM_RECEIVE2, m_nIndex, 0);
	else if(m_nIndex == 2)
		SendMessage(m_hWnd, UM_RECEIVE3, m_nIndex, 0);
	else
		;

	CSocket::OnReceive(nErrorCode);
}


void CSocCom::OnClose(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	SendMessage(m_hWnd, UM_CLOSE, m_nIndex, 0);
	CSocket::OnClose(nErrorCode);
}

