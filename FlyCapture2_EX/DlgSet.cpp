// DlgSet.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "FlyCapture2_EX.h"
#include "FlyCapture2_EXDlg.h"
#include "DlgSet.h"

// CDlgSet 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgSet, CDialog)

CDlgSet::CDlgSet(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSet::IDD, pParent)
{
	m_pParent = pParent;
	m_pwndShow = NULL;
	m_nGrabMode = 0;
}

CDlgSet::~CDlgSet()
{
	//if(m_FormTab1)delete m_FormTab1;
	//if(m_FormTab2)delete m_FormTab2;
	//if(m_FormTab3)delete m_FormTab3;
	//if(m_FormTab5)delete m_FormTab5;
}

void CDlgSet::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_Tab);
}


BEGIN_MESSAGE_MAP(CDlgSet, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_ADJUST, &CDlgSet::OnBnClickedButtonAdjust)
	ON_BN_CLICKED(IDC_BUTTON_CACEL, &CDlgSet::OnBnClickedButtonCacel)
	ON_WM_CREATE()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CDlgSet::OnTcnSelchangeTab1)
	ON_MESSAGE(UM_SELECT_PRODUCT_MESSAGE, &CDlgSet::OnProductSelect)
	ON_WM_DESTROY()
	ON_WM_RBUTTONDOWN()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CDlgSet 메시지 처리기입니다.


BOOL CDlgSet::OnInitDialog()
{
	CDialog::OnInitDialog();
	CRect rect;
	m_Tab.GetClientRect(&rect);
	
	//rect.InflateRect(0,0,100,100);
	CFlyCapture2_EXDlg *pDlg = (CFlyCapture2_EXDlg*)m_pParent;

	m_strComname = pDlg->m_strComname;




	m_FormTab1 = new CFormTab1(this);
	m_FormTab1->Create(IDD_Form_Tab_1,&m_Tab);
	m_FormTab1->SetWindowPos(NULL,5,25,rect.Width()-10,rect.Height()-30,SWP_SHOWWINDOW|SWP_NOZORDER);
	GetDlgItem(IDC_BUTTON_ADJUST)->ShowWindow(SW_HIDE);
//	m_FormTab1->ShowWindow(0);

	m_strSelectProduct =  m_FormTab1->m_strSelectProduct;

	m_FormTab4 = new FormTab4(m_pParent);
	m_FormTab4->Create(IDD_Form_Tab_4,&m_Tab);
	m_FormTab4->SetWindowPos(NULL,5,25,rect.Width()-10,rect.Height()-30,SWP_NOZORDER);
	m_FormTab4->ShowWindow(0);

	m_FormTab3 = new CFormTab3(m_pParent);
	m_FormTab3->Create(IDD_Form_Tab_3,&m_Tab);
	m_FormTab3->SetWindowPos(NULL,5,25,rect.Width()-10,rect.Height()-30,SWP_NOZORDER);
	m_FormTab3->ShowWindow(0);

	m_FormTab5 = new CFormTab5(m_pParent);
	m_FormTab5->Create(IDD_Form_Tab_5,&m_Tab);
	m_FormTab5->SetWindowPos(NULL,5,25,rect.Width()-10,rect.Height()-30,SWP_NOZORDER);
	m_FormTab5->ShowWindow(0);

	m_FormTab2 = new CFormTab2(m_pParent);
	m_FormTab2->Create(IDD_Form_Tab_2,&m_Tab);
	m_FormTab2->SetWindowPos(NULL,5,25,rect.Width()-10,rect.Height()-30,SWP_NOZORDER);
	m_FormTab2->ShowWindow(0);
	
	m_pwndShow = m_FormTab1;

	Sleep(500);
	m_Tab.InsertItem(0,"Model");
	m_Tab.InsertItem(1,"Parameter");
	m_Tab.InsertItem(2,"ETC Para.");
	m_Tab.InsertItem(3,"  D/L");
	m_Tab.InsertItem(4,"Cam etc");

	GetiniData(1);//1 l/load 0save 	

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CDlgSet::LoadImage(int nCam,BOOL bSMode)
{
	CFlyCapture2_EXDlg *pDlg = (CFlyCapture2_EXDlg*)m_pParent;

//	CFileDialog dlg(TRUE, _T("bmp"), NULL, OFN_SHAREAWARE, _T("Bitmap Files (*.bmp)|*.bmp||"));
	CFileDialog dlg(TRUE, NULL, NULL, OFN_SHAREAWARE, "All Files(*.*)|*.*||");//szFilter);

	if(dlg.DoModal() == IDOK)
	{
		pDlg->m_Project.m_bSLoadMode = bSMode;
		{
			pDlg->Invalidate();
			CString strpath,str1,str2;
			strpath = dlg.GetPathName();
			int n = strpath.Find("Cam");
			int l = strpath.GetLength();
			str1 = strpath.Left(n+3);
			str2.Format("%s2%s",str1,strpath.Right(l-n-4));
			pDlg->m_Display[nCam].LoadImg(dlg.GetPathName());

			pDlg->m_Project.m_fm[nCam] =pDlg-> m_Display[nCam].m_pImg->GetImagePtr();
//			pDlg->m_Project.m_fmInsp[nCam] = pDlg->m_Display[nCam].m_pImg->GetImagePtr();
			//nCam=1;
			//pDlg->m_Display[nCam].LoadImg(str2);

			//pDlg->m_Project.m_fm[nCam] =pDlg-> m_Display[nCam].m_pImg->GetImagePtr();
			//pDlg->m_Project.m_fmInsp[nCam] = pDlg->m_Display[nCam].m_pImg->GetImagePtr();

			//	m_nNewCount++;

			pDlg->m_Display[0].SetZoom(0,CPoint(0,0));
//			pDlg->Invalidate();

		}
	}
}

void CDlgSet::Test()
{
	CFlyCapture2_EXDlg *pDlg = (CFlyCapture2_EXDlg*)m_pParent;
	pDlg->m_nMasterMode=1;
	pDlg->LabelResult(-1, AI_READY);
	pDlg->Test(0);

	pDlg->m_nMasterMode=0;
}

void CDlgSet::OnBnClickedButtonAdjust()	
{
	CFlyCapture2_EXDlg *pDlg = (CFlyCapture2_EXDlg*)m_pParent;

//	if(dlg.DoModal() == IDOK)
	{
		CString str;
		str.Format(_T("%s"), m_strSelectProduct);
		pDlg->m_btModel.SetWindowTextA(str);
		//model backup

		CTime ttime = CTime::GetCurrentTime();
		CString kk = ttime.Format("%Y%m%d");
		CString kk2 = ttime.Format("%Y%m");
		CString kk3 = ttime.Format("%d");
		CString kk1 = ttime.Format("[%Y%m%d_%H%M%S]");

		CString str1;
		str1.Format("d:\\Backup\\");
		CreateDirectory(str1,NULL);
		str1.Format("d:\\Backup\\Model\\");
		CreateDirectory(str1,NULL);
		str1.Format("d:\\Backup\\Model\\%s\\",kk2);
		CreateDirectory(str1,NULL);

		CString strOrigin,strCopy,strcom;
		strOrigin.Format("c:\\Glim\\Model\\%s.ini",m_strSelectProduct);
		strCopy.Format("d:\\Backup\\Model\\%s\\%s%s.ini",kk2,kk1,m_strSelectProduct);
		BOOL b1 = CopyFile(strOrigin,strCopy,0);
//		MoveFile(strOrigin,strCopy);
//		strcom.Format("copy %s %s",strOrigin,strCopy);
//		::system(strcom);
	}	
	pDlg->AddtoList("Parameter Backup!",1);

	GetiniData(0);

	pDlg->m_bUseAI = pDlg->m_pDlgSet->m_FormTab4->m_bUseAiIns;
	pDlg->m_bUseAIResult = pDlg->m_pDlgSet->m_FormTab4->m_bWeakWeldOutputUse;

	pDlg->SetEqipID(pDlg->m_pDlgSet->m_FormTab2->m_nLineNumber +1, pDlg->m_pDlgSet->m_FormTab2->m_nLaneNumber +1);

	if (pDlg->m_pDlgSet->m_FormTab2->m_bUseServer) 
		if (pDlg->PingCheck(pDlg->m_pDlgSet->m_FormTab1->m_strServerIP))
			pDlg->WriteJsonSpecPara(0, 1, pDlg->m_pDlgSet->m_FormTab2->m_nLaneNumber +1);

	pDlg->AddtoList("Parameter Save!",1);
}

void CDlgSet::OnBnClickedButtonCacel()
{
	CFlyCapture2_EXDlg *pDlg = (CFlyCapture2_EXDlg*)m_pParent;
	UpdateData(FALSE);
	ShowWindow(SW_HIDE);
	pDlg->m_nShowSet = 0;

	m_FormTab4->m_bRoiSet = FALSE;
	pDlg->m_Display[0].m_bTrackerUse = FALSE;
	m_FormTab4->UpdateData(FALSE);

	m_FormTab5->UpdateData(FALSE);
	KillTimer(1545);
	pDlg->m_pDlgPassword->ShowWindow(SW_HIDE);
}

int CDlgSet::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}

void CDlgSet::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	if(m_pwndShow != NULL)
	{
		m_pwndShow->ShowWindow(SW_HIDE);
		m_pwndShow=NULL;
	}


	int nIndex = m_Tab.GetCurSel();

// 	switch (nIndex)
// 	{
// 	case 0:
// 		m_FormTab1->ShowWindow(SW_SHOW);
// 		m_pwndShow = m_FormTab1;
// 		GetDlgItem(IDC_BUTTON_ADJUST)->ShowWindow(SW_HIDE);
// 		break;
// 	case 1:
// 		m_FormTab4->ShowWindow(SW_SHOW);
// 		m_pwndShow = m_FormTab4;
// 		GetDlgItem(IDC_BUTTON_ADJUST)->ShowWindow(SW_SHOW);
// 		break;
// 	case 2:
// 		m_FormTab3->ShowWindow(SW_SHOW);
// 		m_pwndShow = m_FormTab3;
// 		GetDlgItem(IDC_BUTTON_ADJUST)->ShowWindow(SW_SHOW);
// 		break;
// 	case 3:
// 		m_FormTab5->ShowWindow(SW_SHOW);
// 		m_pwndShow = m_FormTab5;
// 		GetDlgItem(IDC_BUTTON_ADJUST)->ShowWindow(SW_SHOW);
// 		break;
// 	case 4:
// 		m_FormTab2->ShowWindow(SW_SHOW);
// 		m_pwndShow = m_FormTab2;
// 		GetDlgItem(IDC_BUTTON_ADJUST)->ShowWindow(SW_SHOW);
// 		break;
// 	}
	switch (nIndex)
	{
	case 0:
		m_FormTab1->ShowWindow(SW_SHOW);
		m_pwndShow = m_FormTab1;
		GetDlgItem(IDC_BUTTON_ADJUST)->ShowWindow(SW_HIDE);
		break;
	case 1:
		m_FormTab4->ShowWindow(SW_SHOW);
		m_pwndShow = m_FormTab4;
		GetDlgItem(IDC_BUTTON_ADJUST)->ShowWindow(SW_SHOW);
		break;
	case 2:
		m_FormTab3->ShowWindow(SW_SHOW);
		m_pwndShow = m_FormTab3;
		GetDlgItem(IDC_BUTTON_ADJUST)->ShowWindow(SW_SHOW);
		break;
	case 3:
		m_FormTab5->ShowWindow(SW_SHOW);
		m_pwndShow = m_FormTab5;
		GetDlgItem(IDC_BUTTON_ADJUST)->ShowWindow(SW_SHOW);
		break;
	case 4:
		m_FormTab2->ShowWindow(SW_SHOW);
		m_pwndShow = m_FormTab2;
		GetDlgItem(IDC_BUTTON_ADJUST)->ShowWindow(SW_SHOW);
		break;
	}	
	*pResult = 0;
}

BOOL CDlgSet::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message==WM_KEYDOWN)
	{
		if ((pMsg->wParam==VK_RETURN) || (pMsg->wParam==VK_ESCAPE))
		{
			return TRUE;
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CDlgSet::OnDestroy()
{
	CDialog::OnDestroy();

	if(m_FormTab1)		delete m_FormTab1;
	if(m_FormTab4)		delete m_FormTab4;
	if(m_FormTab3)		delete m_FormTab3;
//	if(m_FormTab4)		delete m_FormTab4;
//	if(m_FormTab5)		delete m_FormTab5;
}

LRESULT CDlgSet::OnProductSelect(WPARAM wParam,LPARAM lParam)
{
	CFlyCapture2_EXDlg *pDlg = (CFlyCapture2_EXDlg*)m_pParent;
	m_FormTab3->UpdateIni(TRUE);
	pDlg->m_DlgBarcode->UpdateIni(TRUE);

	GetiniData(1);
	pDlg->m_strIsCode = "None";
	pDlg->m_strModelName = m_strSelectProduct;

	return 0;
}

void CDlgSet::GetiniData(int nsave)
{
	CFlyCapture2_EXDlg *pDlg = (CFlyCapture2_EXDlg*)m_pParent;

	//선택된 제품
	m_strSelectProduct =  m_FormTab1->m_strSelectProduct;

	CString str;
	str.Format(_T("%s"),m_strSelectProduct);
	pDlg->m_btModel.SetWindowTextA(str);
	m_FormTab3->UpdateIni(nsave);
	m_FormTab4->UpdateIni(nsave);
	m_FormTab2->UpdateIni(nsave);
	m_FormTab2->HideParam(m_FormTab2->m_b2LineWelding);
	m_FormTab1->UpdateSetupIni(nsave);

	if(m_FormTab1->m_bUseDeepLearning)
		m_FormTab5->UpdateIni(nsave);

	
	BOOL bshow = FALSE;
	if(m_FormTab4->m_bMidWeld){
		pDlg->m_Display[0].m_nUseTracker = 8;
		bshow = TRUE;
	}
	else
		pDlg->m_Display[0].m_nUseTracker = 8;
	//pDlg->Invalidate();
}

void CDlgSet::InitDisplay()
{
	CFlyCapture2_EXDlg *pDlg = (CFlyCapture2_EXDlg*)m_pParent;

	pDlg->InitDisplay();
	pDlg->m_Project.InitBuf();
}

void CDlgSet::ShowPass()
{
	CFlyCapture2_EXDlg *pDlg = (CFlyCapture2_EXDlg*)m_pParent;
	pDlg->AddtoList("Pass Word Click",1);
	pDlg->SetTimer(1115,100,NULL);
}

void	CDlgSet::WriteLog(CString str, int n)
{
	CFlyCapture2_EXDlg *pDlg = (CFlyCapture2_EXDlg*)m_pParent;

	if(n==0)
		pDlg->AddtoList(str,n);
	else if(n==2)
		pDlg->AddtoInspLog(str,n);
	else
		pDlg->AddtoList(str,n);
}

int  CDlgSet::FolderInsp(int nCam)
{
	CFlyCapture2_EXDlg *pDlg = (CFlyCapture2_EXDlg*)m_pParent;

	CFile file;
	CString str,strng,strok;
	TCHAR	spath[100];
	BROWSEINFO bi;
	memset(&bi,0,sizeof(bi));
	bi.hwndOwner = GetSafeHwnd();

	bi.ulFlags=BIF_RETURNONLYFSDIRS;

	LPITEMIDLIST lpifq=SHBrowseForFolder(&bi);

	if(lpifq)
	{
		SHGetPathFromIDList(lpifq,spath);
		LPMALLOC lpMalloc=NULL;
		HRESULT hr = SHGetMalloc(&lpMalloc);
		if(SUCCEEDED(hr))
		{
			lpMalloc->Free(lpifq);
			lpifq = NULL;
			lpMalloc->Release();
			lpMalloc = NULL;
		}
	}
	else
	{
	}

	CString src, tar,strDevice;
	HANDLE hFind;				// FindFirstFile 핸들
	WIN32_FIND_DATA	fd;			// 검색한 화일의 정보
	BOOL bRet = TRUE;
	CString strfile;
	str = LPCTSTR(spath);
	str += "\\*.jpg";
	int ntmp =0;
	CString strMonth;
	pDlg->m_nMasterMode=1;

	bRet = TRUE;
	hFind = FindFirstFile( str, &fd);
	strDevice = strfile.Left(3);
	m_nTestMode = 1;
	while(hFind != INVALID_HANDLE_VALUE && bRet)
	{			
		strfile.Format("%s\\%s", spath, fd.cFileName);
		if( !(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) )		// Not directory, which means file.
		{
			Sleep(10);
			if(nCam==0){
				if(strfile.Find("Cam1")<0)
				{bRet= FindNextFile(hFind, &fd);
					continue;
				}
			}
			else
			{
				if(strfile.Find("Cam2")<0) 
				{
					bRet= FindNextFile(hFind, &fd);
					continue;
				}
			}

			pDlg->m_Display[nCam].LoadImg(strfile);

			pDlg->m_Project.m_fm[nCam] =pDlg-> m_Display[nCam].m_pImg->GetImagePtr();
//			pDlg->m_Project.m_fmInsp[nCam] = pDlg->m_Display[nCam].m_pImg->GetImagePtr();

			int nn = 0;
			if(nCam==0)
				pDlg->Test(0);

			pDlg->WriteFolderData(nCam, fd.cFileName);
			Sleep(10);
			//			Sleep(100);
		}
		else if( strfile == "." || strfile == "..")
		{
			// do nothing
		}
		else	// it is directory
		{
			// do nothing	choi-sh@koreno.co.kr
		}
		bRet= FindNextFile(hFind, &fd);
	}
	::FindClose(hFind);
	AfxMessageBox("Directory Inspect Complete!!!");	
	pDlg->m_nMasterMode=0;
	m_nTestMode = 0;

	return 0;
}

void CDlgSet::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	//CFlyCapture2_EXDlg *pDlg = (CFlyCapture2_EXDlg*)m_pParent;
	//if(m_nTestMode==1)
	//{
	//	m_ev_rbutton_down.SetEvent();
	//}
	CDialog::OnRButtonDown(nFlags, point);
}

void CDlgSet::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	if(nIDEvent==1545)
	{

	}
	CDialog::OnTimer(nIDEvent);
}


void CDlgSet::Rotate(BYTE* des,BYTE* src,double angle)
{
	CFlyCapture2_EXDlg *pDlg = (CFlyCapture2_EXDlg*)m_pParent;
	pDlg->m_Project.Rotate(des,src,CAMERA_WIDTH*0.5,CAMERA_HEIGHT*0.5,CAMERA_WIDTH,CAMERA_HEIGHT,angle);

}

void CDlgSet::setGain(double dGainValue)
{
	CFlyCapture2_EXDlg *pDlg = (CFlyCapture2_EXDlg*)m_pParent;

	pDlg->m_Vision[0]->SetGain(dGainValue);
}