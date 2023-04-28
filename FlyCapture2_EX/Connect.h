#pragma once

#include "SocService.h"

typedef struct tagConnection
{
 	char	name[20];
 	char	ip[20];
 	char	port[5];
	int		con_try;	//0:사용안함 1:선택적 송신에서 선택X 2:선택적 송신에서 선택O
	BOOL	made;
	int		status;
	int		id;
}CONNECTION;


class CConnect
{
public:
	CConnect(CWnd* pParent = NULL);
	~CConnect(void);
	CWnd*	m_pParent;	
	CONNECTION		m_pConnection[200];
	int				m_nConnectionList;
	CString			m_strServerIP;

	CSocService* m_socService;


	BOOL CConnect::LoadList();
	BOOL CConnect::Connect();
	CString CConnect::ConnectionToStr(BOOL mode, BOOL tryy);
	void CConnect::CreateSocketModeServer(CWnd* pParent, CString strServerIP);
	bool CConnect::CreateSocketModeClient(CWnd* pParent, CString strServerIP, int nIndex);
	void CConnect::SetConnectState(int nIndex, BOOL b_state);
	BOOL CConnect::UseSocket(int i);
	BOOL CConnect::GetConnectState(int nIndex);
	BOOL CConnect::CheckAllConnectState();
	void CConnect::CloseConnect();
	void CConnect::Init();
	void CConnect::SendMessageAll(CString str);
};
