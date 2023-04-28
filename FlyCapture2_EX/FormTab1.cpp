// FormTab1.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "FlyCapture2_EX.h"
#include "FlyCapture2_EXDlg.h"

#include "FormTab1.h"
#include "ini.h"
#include <direct.h>
#include <process.h>
#include <io.h>
#include "DlgSet.h"

static _TCHAR *_gszL1Label[NUM_COLUMNS] = {_T("ModelName"), _T("")};//, _T("상하"), _T("표시"), _T("인치"), _T("출하처")};//약호	상하판	표시	인치

static int _gnLColumnWidth[NUM_COLUMNS] = {	250, 250};
// CFormTab1 대화 상자입니다.

IMPLEMENT_DYNAMIC(CFormTab1, CDialog)

CFormTab1::CFormTab1(CWnd* pParent /*=NULL*/)
	: CDialog(CFormTab1::IDD, pParent)
	, m_strSelectProduct(_T(""))
	, m_strProductName(_T(""))
{
	m_hParent = pParent;
	for(int i=0;i<NUM_COLUMNS;i++)
		m_strCodeInfo[i] = "";
	m_bShowHide = FALSE;
	m_nRecipe = 0;
	m_strPgmVersion = "11";
	m_strServerIP = "192.168.1.222";
	m_bUseDeepLearning = FALSE;
	m_bUseAging = FALSE;
	m_nAgingDelay = 800;
}

CFormTab1::~CFormTab1()
{
}

void CFormTab1::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_PRODUCT01, m_strSelectProduct);
	DDX_Text(pDX, IDC_EDIT_PRODUCT_NAME, m_strProductName);
	DDX_Control(pDX, IDC_LIST_PRODUCT_TAB_1, m_pList);
}


BEGIN_MESSAGE_MAP(CFormTab1, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_PRODCUT_TAB_1, &CFormTab1::OnBnClickedButtonProdcutTab1)	
	ON_BN_CLICKED(IDC_BUTTON_SELECT_TAB_1, &CFormTab1::OnBnClickedButtonSelectTab1)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_PRODUCT_TAB_1, &CFormTab1::OnNMDblclkListProductTab1)
	ON_BN_CLICKED(IDC_BUTTON_DELETE_TAB_1, &CFormTab1::OnBnClickedButtonDeleteTab1)
	ON_BN_CLICKED(IDC_BUTTON_REFRESH, &CFormTab1::OnBnClickedButtonRefresh)
END_MESSAGE_MAP()


// CFormTab1 메시지 처리기입니다.


BOOL CFormTab1::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	m_pList.SetExtendedStyle(  LVS_EX_FLATSB | LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);

	for(int i = 0 ; i <NUM_COLUMNS; i++)
		m_pList.InsertColumn(i,_gszL1Label[i], LVCFMT_LEFT, _gnLColumnWidth[i]);

	CString str;
	str=GLIM_PATH;
	str+="Model";
	CreateDirectory(str,NULL);
	DirFileCount((LPSTR)(LPCSTR)str);

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CFormTab1::OnBnClickedButtonProdcutTab1()
{
	UpdateData(TRUE);

	{
		if(m_strProductName == "")
		{
			AfxMessageBox("Do you Type Model Name!");
			return;
		}


		for(int i=0; i < m_pList.GetItemCount(); i++)
		{
			if(m_strProductName == m_pList.GetItemText(i,0))
			{
				AfxMessageBox("Registration Model have a Same Model Name");
				return;
			}
		}

		CListCtrl* pCtrl;
		pCtrl = (CListCtrl*)GetDlgItem(IDC_LIST_PRODUCT_TAB_1);
		CString strSelectProduct;
		CString strFilePath;
		strFilePath = GLIM_PATH;
		strFilePath+="Model\\";

		POSITION pos = pCtrl->GetFirstSelectedItemPosition();

		if (pos != NULL)
		{
			while (pos)
			{
				int nItem = pCtrl->GetNextSelectedItem(pos);
				pCtrl->SetCheck(nItem , TRUE);
				// you could do your own processing on nItem here
				strSelectProduct = m_pList.GetItemText(nItem,0);
				strFilePath+=strSelectProduct+".ini";
				//if(strSelectProduct == m_strSelectProduct)
				//{
				//	AfxMessageBox("This Model Don't Delete! Because this Model Running!");
				//	return;
				//}
	//			m_pList.DeleteItem(nItem);
				break;
			}

		}
		CString str;
		str.Format("Copy Model? (%s -> %s)",strSelectProduct,m_strProductName);
		if(MessageBox(str,	"GLIM",MB_OKCANCEL) == IDOK)
		{
			CString strnewpath;
			strnewpath.Format("c:\\Glim\\Model\\%s.ini",m_strProductName);
			CopyFile(strFilePath,strnewpath,TRUE);

				//m_strProductName
			int nNum = m_pList.GetItemCount();
			CString strTemp;	
			m_pList.InsertItem(nNum,NULL);

			m_pList.SetItemText(nNum,0,m_strProductName);
			ProductData(m_strProductName+".ini",FALSE);	
			m_strProductName="";
			UpdateData(FALSE);
		}
	}
}

//True 읽기 , False 쓰기
void CFormTab1::ProductData(CString strModel,BOOL bLoad)
{
	CDlgSet *pDlg = (CDlgSet*)m_hParent;	
	BOOL bProductCheck;
	CString str;
	CString strPath = GLIM_PATH;
	strPath+="Model\\"+strModel;
	str = strModel.Mid(0,strModel.GetLength()-4);

	if(bLoad == FALSE)// 쓰기 일경우
	{
		bProductCheck = 0;
	}

	CIni ini1( strPath, "제품설정" , 1 );
	ini1.SER_GETD(bLoad, bProductCheck, 1);	
	ini1.SER_GETD(bLoad, m_strCodeInfo[0], "");//약호
	ini1.SER_GETD(bLoad, m_strCodeInfo[1], "");//상하

	if(bLoad == TRUE){
		int nNum = m_pList.GetItemCount();
		CString strTemp;	
		m_pList.InsertItem(nNum,NULL);
		m_pList.SetItemText(nNum,0,str);
		m_pList.SetItemText(nNum,1,m_strCodeInfo[0]);
		m_pList.SetItemText(nNum,2,m_strCodeInfo[1]);
	}

	if(bProductCheck){
		m_strSelectProduct = str;
		UpdateData(FALSE);
	}	
}

//true 읽기 flase 쓰기
void CFormTab1::UpdateSetupIni(BOOL bLoad)
{
	CDlgSet *pDlg = (CDlgSet*)m_hParent;	

	if(bLoad==FALSE)	//save
		UpdateData(TRUE);

	CString strPath;
	strPath.Format("%ssetup.ini",GLIM_PATH);
	CIni ini1(strPath, "검사설정" , 0 );
	ini1.SER_GETD(bLoad, m_strPgmVersion , _T("Ver.1"));
	ini1.SER_GETD(bLoad, m_strServerIP, _T("192.168.1.2"));
	ini1.SER_GETD(bLoad, m_bUseAging, FALSE);
	ini1.SER_GETD(bLoad, m_nAgingDelay, 800);
	ini1.SER_GETD(bLoad, m_bUseDeepLearning, FALSE);

	if(bLoad)
		UpdateData(FALSE);
}


void CFormTab1::OnBnClickedButtonSelectTab1()
{
	CDlgSet *pDlg = (CDlgSet*)m_hParent;	
	CString strPath = GLIM_PATH;
	strPath+="Model\\"+m_strSelectProduct+".ini";
	BOOL bProductCheck = FALSE;

	CIni ini1( strPath, "제품설정" , 1 );	
	ini1.SER_GETD(FALSE, bProductCheck, 1);	
	
	CListCtrl* pCtrl;
	pCtrl = (CListCtrl*)GetDlgItem(IDC_LIST_PRODUCT_TAB_1);
	POSITION pos = pCtrl->GetFirstSelectedItemPosition();
	CString str1;
	if (pos != NULL)
	{
		while (pos)
		{
			int nItem = pCtrl->GetNextSelectedItem(pos);
			pCtrl->SetCheck(nItem , TRUE);
			// you could do your own processing on nItem here

			if(MessageBox(m_pList.GetItemText(nItem,0)+" Do You Wanna Change Model?",
				"GLIM",MB_OKCANCEL) != IDOK)
			{
				return;
			}

			m_strSelectProduct = m_pList.GetItemText(nItem,0);

			strPath = GLIM_PATH;
			strPath+="Model\\"+m_strSelectProduct+".ini";
			bProductCheck = TRUE;
			pDlg->m_strSelectProduct = m_strSelectProduct;

			CIni ini1( strPath, "제품설정" , 1 );	
			ini1.SER_GETD(FALSE, bProductCheck, 1);
			ini1.SER_GETD(1, m_strCodeInfo[0], "");//약호
			ini1.SER_GETD(1, m_strCodeInfo[1], "");//상하
			pDlg->m_strCodeInfo[0] = m_strCodeInfo[0];
			pDlg->m_strCodeInfo[1] = m_strCodeInfo[1];
			
			pDlg->SendMessage(UM_SELECT_PRODUCT_MESSAGE,0,0);

			break;
		}
	}

	UpdateData(FALSE);
}

void CFormTab1::OnNMDblclkListProductTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	//LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	OnBnClickedButtonSelectTab1();
	*pResult = 0;
}

void CFormTab1::OnBnClickedButtonDeleteTab1()
{
	CListCtrl* pCtrl;
	pCtrl = (CListCtrl*)GetDlgItem(IDC_LIST_PRODUCT_TAB_1);
	CString strSelectProduct;
	CString strFilePath;
	strFilePath = GLIM_PATH;
	strFilePath+="Model\\";

	POSITION pos = pCtrl->GetFirstSelectedItemPosition();

	if (pos != NULL)
	{
		while (pos)
		{
			int nItem = pCtrl->GetNextSelectedItem(pos);
			pCtrl->SetCheck(nItem , TRUE);
			// you could do your own processing on nItem here
			strSelectProduct = m_pList.GetItemText(nItem,0);
			strFilePath+=strSelectProduct+".ini";
			if(strSelectProduct == m_strSelectProduct)
			{
				AfxMessageBox("This Mo del Don't Delete! Because this Model Running!");
				return;
			}

			CString str;
			str.Format("Delete Model? %s",strSelectProduct);
			if(MessageBox(str,	"GLIM",MB_OKCANCEL) == IDOK)
			{
			}
			else
				return;

			m_pList.DeleteItem(nItem);
			break;
		}

	}

	DeleteFile(strFilePath);

}

// 전체 파일갯수 취득
int CFormTab1::DirFileCount(char *pDirName)
{
	struct _finddata_t c_file;
	char FileSpec[_MAX_PATH], TmpFileName[_MAX_PATH];
	long hFile;
	int SubDirCount=0;

	sprintf( (char *)FileSpec, "%s\\*.*", pDirName);

	if( (hFile = _findfirst( (const char *)FileSpec, &c_file )) == -1L ) 
	{
		return 0;
	}else 
	{
		SubDirCount=0;
		if( c_file.attrib & _A_SUBDIR) 
		{
			if( c_file.name[0] != '.') 
				SubDirCount++;
		}else 
		{
			sprintf( (char *)TmpFileName, "%s\\%s", pDirName, c_file.name);
		}

		while( _findnext( hFile, &c_file ) == 0 ) 
		{ // possible uninitialized memory read from Purify
			if( c_file.attrib & _A_SUBDIR) 
			{
				if( c_file.name[0] != '.') 
					SubDirCount++;
			} else 
			{
				CString str,strTemp;
				sprintf( (char *)TmpFileName, "%s\\%s", pDirName, c_file.name);
				str = TmpFileName;
				strTemp = str.Right(4);
				if(strTemp == ".ini")
				{
					ProductData(c_file.name,TRUE);
				}
				

			}
		}
		_findclose( hFile );
	}

	if ( SubDirCount < 1 ) 
	{
		return 0;
	}

	char **SubDirNames;
	int i;

	SubDirNames = (char **)malloc( SubDirCount * sizeof(char *));
	if( SubDirNames == NULL ) 
		return 0;

	for( i=0;i<SubDirCount;i++) 
	{
		SubDirNames[i] = (char *)malloc(_MAX_PATH);
		if ( SubDirNames[i] == NULL ) 
			return 0;
	}

	i=0;
	if( (hFile = _findfirst( (const char *)FileSpec, &c_file )) == -1L ) 
		return 0;
	else 
	{
		if( (c_file.attrib & _A_SUBDIR) && (c_file.name[0] != '.')) 
			sprintf( (char *)SubDirNames[i++], "%s\\%s", pDirName, c_file.name);

		while( _findnext( hFile, &c_file ) == 0 ) 
		{
			if( (c_file.attrib & _A_SUBDIR) && (c_file.name[0] != '.')) 
				sprintf( (char *)SubDirNames[i++], "%s\\%s", pDirName, c_file.name);
		}

		_findclose( hFile );
	}

	if( i != SubDirCount ) 
		return 0;

	for(i=0;i<SubDirCount;i++) 
	{
		DirFileCount( SubDirNames[i] );
		free( SubDirNames[i] );
	}

	free( SubDirNames );

	return 0;
}

void CFormTab1::OnBnClickedButtonRefresh()
{
	CDlgSet *pDlg = (CDlgSet*)m_hParent;

	m_pList.DeleteAllItems();

	CFileFind finder;
	CString strpath,strfile,strfile1;
	strpath.Format("c:\\Glim\\Model\\*.ini");
	BOOL bfind = finder.FindFile(strpath);
	int n = 0;
	BOOL bwork = FALSE;

	while(bfind)
	{
		bfind = finder.FindNextFile();
		strfile = finder.GetFileName();
		if(finder.IsDirectory() || finder.IsDots())
			continue;
		else
		{
			strfile = finder.GetFileName();
			strfile.TrimRight(".ini");
			
			m_strSelectProduct = strfile;
			//loadini
			CString strPath;
			strPath = GLIM_PATH;
			strPath+="Model\\"+m_strSelectProduct+".ini";
			//	m_strSelectProduct = m_strSelectProduct;

			BOOL bLoad = TRUE;
			CIni ini1( strPath, "제품설정" , 1 );	
			ini1.SER_GETD(bLoad, m_strCodeInfo[0], "");//약호
			ini1.SER_GETD(bLoad, m_strCodeInfo[1], "");//상하

			int nNum = m_pList.GetItemCount();
			m_pList.InsertItem(nNum,NULL);

//			m_pList.InsertItem(1,strfile);
			m_pList.SetItemText(nNum,0,m_strSelectProduct);

			m_pList.SetItemText(nNum,1,m_strCodeInfo[0]);
			m_pList.SetItemText(nNum,2,m_strCodeInfo[1]);
		}
	}
}