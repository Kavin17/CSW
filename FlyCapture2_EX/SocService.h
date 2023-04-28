#pragma once
#include "SocCom.h"
#include "SocServer.h"
#include "winsock2.h"
#include <IPHlpApi.h>
#include <IcmpAPI.h>


//#pragma comment(lib, "WS2_32.lib")


#define DESIRED_WINSOCK_VERSION        0x0101
#define MINIMUM_WINSOCK_VERSION        0x0001
#define SERVER_RECEIVE_BUFFER_SIZE	   1000
#define SHERE_SEND_BUFFER_SIZE		   1000
#define OPERATE_MODE_SERVER			   0
#define OPERATE_MODE_CLIENT			   1


class CSocService
{
public:
	CSocServer*		m_socServer[200];
	CSocCom*		m_socClient;
	CWnd*			m_pParent;
	int				m_opMode;
	CString			m_strIP;
	int				m_port;
	
	int				m_nSendDataLength;
	BOOL			m_sendDataON;
	BOOL			m_sendDataEnd;
	BOOL			m_clientAccept;

public:
	CSocService();
	CSocService(CWnd* pParent);
	~CSocService(void);
	//BOOL	CheckPing(CString strIp);
	void	InitSocService(int opMode, CString ip, CString port, int nIndex);
	void	CloseSocket();
	void	CloseServerSocket();
	void	CloseClientSocket();
	void	Serv_Connect(int nIndex);
	void	Client_Connect(int nIndex);
	CString GetInIpAddress();
	CString GetIpAddress();
	//전송
	BOOL	DataSend(CString strSend);
	BOOL	DataSend(CString strSend, int index);
	
	//수신
	int		DataReceive(CSocCom* pSocket);
	BOOL	GetStatusSocket(int opMode);

private:
	BOOL	DataSendToSocket(CSocCom* pSocket, CString str);
};
