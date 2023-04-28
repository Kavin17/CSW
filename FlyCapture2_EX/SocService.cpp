#include "StdAfx.h"
#include "SocService.h"



CSocService::CSocService()
{
	m_socClient = NULL;
	for(int i=0; i<MAX_CONNECT; i++){
		m_socServer[i] = new CSocServer();
	}
}


CSocService::CSocService(CWnd* pParent)
{
	m_pParent = pParent;
	m_socClient = NULL;
	for(int i=0; i<MAX_CONNECT; i++){
		m_socServer[i] = new CSocServer();
	}
	
}

CSocService::~CSocService(void)
{
	for(int i=0; i<MAX_CONNECT; i++){
		delete m_socServer[i];
	}
}

/*
BOOL CSocService::CheckPing(CString strIp)
{
	BOOL bChkInternet;
	int nMaxTime = 2;//m_nMaxTCPTime;
	HRESULT hr;
	char *SendData = "Check TCP/IP";
	LPVOID ReplyBuffer;
	DWORD dwRetVal;
	HANDLE hlcmpFile;
	hlcmpFile = IcmpCreateFile();
	ReplyBuffer = (VOID*)malloc(sizeof(ICMP_ECHO_REPLY)+sizeof(SendData));
	char* ch =  (char*)(LPCTSTR)strIp;

	while(TRUE){
		bChkInternet = IcmpSendEcho(hlcmpFile,
			inet_addr(ch),
			SendData,
			strlen(SendData),
			NULL,
			ReplyBuffer,
			sizeof(ReplyBuffer)+sizeof(ICMP_ECHO_REPLY),1000);

		if(bChkInternet) break;

		Sleep(1000);

		nMaxTime--;

		if(0==nMaxTime){
			//AfxMessageBox(_T("H/W Check"),MB_OK);
			return FALSE;
		}
	}

	return TRUE;
}
*/
//opMode 0 : 서버, 1 : 클라이언트
void CSocService::InitSocService(int opMode, CString ip, CString port, int nIndex)
{
	char* ch =  (char*)(LPCTSTR)port;

	char tmp[10];
	sprintf(tmp,"%s",port);		//unicode

	m_port = atoi(tmp);
	m_strIP = ip;
	m_clientAccept = FALSE;

	if(opMode == OPERATE_MODE_SERVER){
		m_opMode = OPERATE_MODE_SERVER;
		Serv_Connect(nIndex);
	}
	else if(opMode == OPERATE_MODE_CLIENT){
		m_opMode = OPERATE_MODE_CLIENT;
		Client_Connect(nIndex);
	}
}

void CSocService::CloseSocket()
{
	if(m_opMode == OPERATE_MODE_SERVER){
		CloseServerSocket();
	}
	else if(m_opMode == OPERATE_MODE_CLIENT){
		CloseClientSocket();
	}
}

void CSocService::CloseServerSocket()
{
	for(int i=0; i<MAX_CONNECT; i++){
		m_socServer[i]->CloseSocket();
		m_socServer[i]->Close();
	}
}


void CSocService::CloseClientSocket()
{
	if(m_socClient != NULL){
		m_socClient->Close();
		delete m_socClient;
		m_socClient = NULL;
		m_clientAccept = FALSE;
	}
}


void CSocService::Serv_Connect(int nIndex)
{
	AfxSocketInit();
	
	if(m_socServer[nIndex]->Create(m_port)){

	}

	m_socServer[nIndex]->Listen();
	m_socServer[nIndex]->Init(m_pParent->m_hWnd, nIndex);
	
}

void CSocService::Client_Connect(int nIndex)
{
	BOOL bPing = TRUE;//CheckPing(m_strIP);	//180405_안됌 테스트 해볼것

	if(bPing == TRUE){
		AfxSocketInit();
		if(m_socClient != NULL){
			delete m_socClient;
			m_socClient = NULL;
		}
		if(m_socClient == NULL){
			m_socClient = new CSocCom;
		}

		m_socClient->Create();
		// IP 주소와 포트 번호를 지정
		m_clientAccept = m_socClient->Connect(m_strIP, m_port);
		if(m_clientAccept == TRUE){ 
			m_socClient->Init(m_pParent->m_hWnd, nIndex);
		}
	}
}

CString CSocService::GetInIpAddress()
{
	WSADATA wsadata;
	CString strIP; // 이 변수에 IP주소가 저장된다.
	strIP = "";

	if( !WSAStartup( DESIRED_WINSOCK_VERSION, &wsadata ) )
	{
		if( wsadata.wVersion >= MINIMUM_WINSOCK_VERSION )
		{
			HOSTENT *p_host_info;
			IN_ADDR in;
			char host_name[128]={0, };

			gethostname(host_name, 128);
			p_host_info = gethostbyname( host_name );

			if( p_host_info != NULL )
			{
				for( int i = 0; p_host_info->h_addr_list[i]; i++ )
				{
					memcpy( &in, p_host_info->h_addr_list[i], 4 );
					strIP = inet_ntoa( in );
				}
			}
		}
		WSACleanup();
	}
	return strIP;
}

CString CSocService::GetIpAddress()
{
	WORD wVersionRequested;
	WSADATA wsaData;
	char name[255];
	PHOSTENT hostinfo;
	CString strIpAddress = _T("");
	wVersionRequested = MAKEWORD(2, 0);

	if(WSAStartup(wVersionRequested, &wsaData) == 0)
	{
		if(gethostname(name, sizeof(name)) == 0)
		{
			if((hostinfo = gethostbyname(name)) != NULL)
				strIpAddress = inet_ntoa (*(struct in_addr *)*hostinfo->h_addr_list);
		} 
		WSACleanup();
	}
	return strIpAddress;
}

BOOL CSocService::DataSend(CString strSend)
{
	/*int n = 0;
	n = strSend.GetLength();
	m_nSendDataLength = n;

	BOOL bRet = GetStatusSocket(m_opMode);

	if(bRet == TRUE){
		if(m_opMode == OPERATE_MODE_SERVER){
			DataSendToSocket(m_socServer.m_pSocCom, strSend);
		}
		else if(m_opMode == OPERATE_MODE_CLIENT){
			DataSendToSocket(m_socClient, strSend);
		}
	}*/

	//return bRet;
	return TRUE;
}

BOOL CSocService::DataSend(CString strSend, int index)
{
	int n = 0;
	n = strSend.GetLength();
	m_nSendDataLength = n;

	BOOL bRet = GetStatusSocket(m_opMode);

	if(bRet == TRUE){
		if(m_opMode == OPERATE_MODE_SERVER){
			DataSendToSocket(m_socServer[index]->m_pSocCom, strSend);
		}
		else if(m_opMode == OPERATE_MODE_CLIENT){
			DataSendToSocket(m_socClient, strSend);
		}
	}

	//return bRet;
	return TRUE;
}

BOOL CSocService::GetStatusSocket(int opMode)
{
	//BOOL bRet = FALSE;

	//if(m_opMode == OPERATE_MODE_SERVER){
	//	if(m_socServer.m_serverAccept == TRUE){
	//		bRet = TRUE;
	//	}
	//	else{
	//		bRet = FALSE;
	//	}
	//}
	//else if(m_opMode == OPERATE_MODE_CLIENT){
	//	if(m_clientAccept == TRUE){
	//		bRet = TRUE;
	//	}
	//	else{
	//		bRet = FALSE;
	//	}
	//}
	//return bRet;
	return TRUE;
}

BOOL CSocService::DataSendToSocket(CSocCom* pSocket, CString str)
{
	BOOL bRet = FALSE;
	int nn;
	if(pSocket == NULL){
		bRet = FALSE;
	}
	char tmp[SEND_PACKET_LENGTH];
	BYTE byteTmp[SEND_PACKET_LENGTH]={0x02,0x01,0x00,0x03};
	memset(tmp, 0, SEND_PACKET_LENGTH);
	memset(byteTmp, 0, SEND_PACKET_LENGTH);
	//sprintf(tmp, "%s", str);		//unicode

	byteTmp[0]=0x02;
	byteTmp[1]=0x01;
	byteTmp[2]=0x00;
	byteTmp[3]=0x03;
//	sprintf(tmp, "%s", "X0051+000101B3");		//unicode
//	tmp[0] = 0x01;
	//int socketReponse = pSocket->Send(tmp, 14);
// 	tmp[0]=02;
// 	tmp[1]=01;
// 	tmp[2]=00;
// 	tmp[3]=03;
	int socketReponse = pSocket->Send(byteTmp, m_nSendDataLength);

	if(socketReponse == SOCKET_ERROR){
		bRet = FALSE;	
	}
	else{
		bRet = TRUE;
	}
	return bRet;
}
/*
BOOL CSocService::DataSendToSocket(CSocCom* pSocket, CString str)
{
	BOOL bRet = FALSE;

	if (pSocket == NULL) {
		bRet = FALSE;
	}
	char tmp[SEND_PACKET_LENGTH];
	sprintf(tmp, "%S", str);		//unicode
	int socketReponse = pSocket->Send(tmp, m_nSendDataLength);
	//	int socketReponse = pSocket->Send(str, m_nSendDataLength);

	if (socketReponse == SOCKET_ERROR) {
		bRet = FALSE;
	}
	else {
		bRet = TRUE;
	}
	return bRet;
}
*/