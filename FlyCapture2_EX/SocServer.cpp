// SocServer.cpp : implementation file
//

#include "stdafx.h"
#include <afxsock.h>		// MFC socket extensions
#include "SocServer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSocServer

CSocServer::CSocServer()
{
	m_pSocCom = NULL;
}

CSocServer::~CSocServer()
{
	if(m_pSocCom != NULL){
		delete m_pSocCom;
	}
								
}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CSocServer, CSocket)
	//{{AFX_MSG_MAP(CSocServer)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CSocServer member functions

void CSocServer::Init(HWND hWnd, int nIndex)
{
	// ���� ������ ������ �ޱ�
	m_hWnd = hWnd;
	m_pSocCom = new CSocCom();
	m_pSocCom->Init(hWnd, nIndex);
	m_serverAccept = FALSE;
	m_serverAccpetAlramOn = FALSE;
	m_nIndex = nIndex;
}

void CSocServer::ReconnectSocket()
{
	// ���� ������ ������ �ޱ�
	delete m_pSocCom;
	m_pSocCom = new CSocCom();
	m_pSocCom->Init(m_hWnd, m_nIndex);
	m_serverAccept = FALSE;
	m_serverAccpetAlramOn = FALSE;
}

void CSocServer::CloseSocket()
{
	if(m_pSocCom != NULL){
		delete m_pSocCom;
		m_pSocCom = NULL;
		m_serverAccept = FALSE;
	}
}


void CSocServer::OnAccept(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	//m_pSocCom->
	if(m_serverAccept == TRUE){
		//������ ����� ������ ���� �ٽ� ����
		ReconnectSocket();
		//return;
	}
	m_pSocCom = GetAcceptSocCom();
	Accept(*m_pSocCom);
	//m_pSocCom->Send("connect-ok", 10);
	m_serverAccept = TRUE;
	m_serverAccpetAlramOn = TRUE;
	SendMessage(m_hWnd, UM_ACCEPT, m_nIndex, 0);

	CSocket::OnAccept(nErrorCode);
}


CSocCom* CSocServer::GetAcceptSocCom()
{
	// ��ż����� �����Ѵ�.
	// ��ȯ�Ǵ� ��� ������ Ŭ���̾�Ʈ�� ����ȴ�.
	return m_pSocCom;
}
