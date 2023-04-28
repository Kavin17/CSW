#include "StdAfx.h"
#include "FlyCapture2_EX.h"
#include "FlyCapture2_EXDlg.h"
#include "Connect.h"



CConnect::CConnect(CWnd* pParent)
{
	m_pParent = pParent;
	m_socService = NULL;
	m_strServerIP = _T("");
	Init();
}

CConnect::~CConnect(void)
{
}

BOOL CConnect::LoadList()
{
	int deli1, deli2, deli3;
	char skip[300];
	char filepath[100] = "c:\\GLIM\\Connect_list.txt";
	FILE* fp;
	fp = fopen(filepath, "r");

	if(!fp){
		fp = fopen(filepath, "r");
		if(!fp){
			AfxMessageBox(_T("Server List not ready!!"));
			return FALSE;
		}
		return FALSE;
	}

	m_nConnectionList = 0;

	while(!feof(fp) && (m_nConnectionList < MAX_CONNECT) ){
		deli1 = fgetc(fp);
		deli2 = fgetc(fp);
		deli3 = fgetc(fp);
		fseek(fp, -3, SEEK_CUR);

		if( (char)deli1 == '/' || (char)deli2 == '/' || (char)deli3 == '/' ){
			fgets(skip, 300, fp);
			continue;
		}

		memset(&m_pConnection[m_nConnectionList], 0, sizeof(CONNECTION));
		fscanf(fp, "%s", m_pConnection[m_nConnectionList].name);
		fscanf(fp, "%s", m_pConnection[m_nConnectionList].ip);
		fscanf(fp, "%s", m_pConnection[m_nConnectionList].port);
		char ttstr[10];
		char ttstr2[10];
		fscanf(fp, "%s", ttstr);
		//fscanf(fp, "%s", ttstr2);
		m_pConnection[m_nConnectionList].con_try = atoi(ttstr);
		//m_pConnection[m_nConnectionList].id = atoi(ttstr2);
		m_nConnectionList++;
		
	}

	int i=0;
	for(i=0; i<m_nConnectionList; i++){
		if(strlen(m_pConnection[i].name) == 0)
			break;
	}

	if(i!=m_nConnectionList)
		m_nConnectionList = i;

	fclose(fp);
	return true;
}

void CConnect::CreateSocketModeServer(CWnd* pParent, CString strServerIP)
{
	CFlyCapture2_EXDlg* pView = (CFlyCapture2_EXDlg*)m_pParent;

	Init();
	//LoadList();

	int opMode = 0;
	m_socService = new CSocService(pParent);

	for(int i=0; i<MAX_CONNECT; i++){
		if(m_pConnection[i].con_try == 1 && m_pConnection[i].id == 0){
			//CString strIP = m_socService->GetIpAddress();
			CString strIP = strServerIP;
			CString strPort;
			strPort.Format(_T("%s"),m_pConnection[i].port);
			m_socService->InitSocService(opMode, strIP, strPort, i);
			CString strTemp;
			strTemp.Format(_T("%s:%s,%s-소켓생성"), m_pConnection[i].name, strIP, m_pConnection[i].port);
//			pView->AddtoList(strTemp);

		}	
	}
}

bool CConnect::CreateSocketModeClient(CWnd* pParent, CString strServerIP, int nIndex)
{
	CFlyCapture2_EXDlg* pView = (CFlyCapture2_EXDlg*)m_pParent;
	bool bRes = true;

	Init();
	//LoadList();

	BOOL isGetPort = FALSE;
	int opMode = 1;
	m_socService = new CSocService(pParent);

	CString strPort = _T("");
	CString strServerIp = strServerIP;
	CString strPcName = _T("");

	//PC이름 가져오기
/*	strPcName = pView->ComputerName();*/


	for(int i=0; i<m_nConnectionList; i++){
		if(m_pConnection[i].con_try == 1){// && m_pConnection[i].id == 0){
			CString strConnectPcName;
			strConnectPcName.Format(_T("%s"),m_pConnection[i].name);
			if(strConnectPcName.Find(strPcName) >= 0){
				//포트가져오기
				strPort = m_pConnection[i].port;
				isGetPort = TRUE;
				break;
			}
		}
	}

	CString strTemp;
	isGetPort = TRUE;
	strPort.Format(_T("3302"));
	if(isGetPort == TRUE){
		m_socService->InitSocService(opMode, strServerIp, strPort, nIndex);
		
		if(m_socService->m_clientAccept == FALSE){
			strTemp.Format(_T("%s:%s-소켓생성-연결실패"), strServerIp, strPort);
			bRes = false;
		}
		else{
			strTemp.Format(_T("%s:%s-소켓생성-연결완료"), strServerIp, strPort);
			bRes = true;
		}
		
	}
	else{
		strTemp.Format(_T("%s:%s-소켓생성-포트번호 찾을 수 없음"), strServerIp, strPort);
		bRes = false;
	}
	
//	pView->AddtoList(strTemp);

	return bRes;
}

BOOL CConnect::Connect()
{
	CString strTemp;

	for(int i=0; i<m_nConnectionList; i++){
		
		if(m_pConnection[i].made == 0 || m_pConnection[i].made == 1){
			strTemp ="";
			strTemp += CString(m_pConnection[i].name);
			strTemp += "    - ";
			strTemp += ConnectionToStr(m_pConnection[i].made, m_pConnection[i].con_try);
		}
	}

	return TRUE;
}

BOOL CConnect::UseSocket(int i)
{
	BOOL bRet = FALSE;

	if(m_pConnection[i].con_try == 1 && m_pConnection[i].id == 0 && strlen(m_pConnection[i].name) != 0){
		bRet = TRUE;
	}
	return bRet;
}

CString CConnect::ConnectionToStr(BOOL mode, BOOL tryy)
{
	CString temp_str;

	if(tryy)
	{
		if(mode)
			return _T("연결 완료");
		else
			return _T("미연결");
	}

	return _T("a");

}

void CConnect::SetConnectState(int nIndex, BOOL b_state)
{
	m_pConnection[nIndex].made = b_state;
}

BOOL CConnect::GetConnectState(int nIndex)
{
	return m_pConnection[nIndex].made;
}

BOOL CConnect::CheckAllConnectState()
{
	BOOL bRet = FALSE;
	int nUseSocket = 0;
	int nConnectSocket = 0;

	for(int i=0; i<m_nConnectionList; i++){
		if(UseSocket(i) == TRUE){
			nUseSocket++;
			if(GetConnectState(i) == TRUE){
				nConnectSocket++;
			}
		}
	}

	if(nUseSocket == nConnectSocket && nConnectSocket != 0){
		bRet = TRUE;
	}
	return bRet;
}

void CConnect::CloseConnect()
{
	m_socService->CloseSocket();
}

void CConnect::Init()
{
	if(m_socService != NULL){
		delete m_socService;
	}
	m_socService = NULL;

	for(int i=0; i<MAX_CONNECT; i++){
		memset(&m_pConnection[i], 0, sizeof(CONNECTION));
	}
}

void CConnect::SendMessageAll(CString str)
{
	
	for(int i=0; i<MAX_CONNECT; i++){
		//memset(&m_pConnection[i], 0, sizeof(CONNECTION));
		if(UseSocket(i) == TRUE && GetConnectState(i) == TRUE){
			m_socService->DataSend(str, i);
		}
	}
	
}

