// FormTab1.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "FlyCapture2_EX.h"
#include "FormTab1.h"



#define NUM_COLUMNS	4
static _TCHAR *_gszL1Label[NUM_COLUMNS] = {_T("��ǰ��"), _T("���������̰Ÿ�"), _T("����"), _T("���Ⱚ")};
static int _gnLColumnWidth[NUM_COLUMNS] = {	120, 80, 70,70};
// CFormTab1 ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CFormTab1, CDialog)

CFormTab1::CFormTab1(CWnd* pParent /*=NULL*/)
	: CDialog(CFormTab1::IDD, pParent)
	, m_strSelectProduct(_T(""))
	, m_strProductName(_T(""))
	, m_dEditPara1(0)
	, m_dEditPara2(0)
	, m_dEditPara3(0)
{
	m_hParent = pParent;
}

CFormTab1::~CFormTab1()
{
}

void CFormTab1::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_PRODUCT01, m_strSelectProduct);
	DDX_Text(pDX, IDC_EDIT_PRODUCT_NAME, m_strProductName);
	DDX_Text(pDX, IDC_EDIT_PARA01, m_dEditPara1);
	DDX_Text(pDX, IDC_EDIT_PARA02, m_dEditPara2);
	DDX_Text(pDX, IDC_EDIT_PARA03, m_dEditPara3);
	DDX_Control(pDX, IDC_LIST_PRODUCT_TAB_1, m_pList);
}


BEGIN_MESSAGE_MAP(CFormTab1, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_PRODCUT_TAB_1, &CFormTab1::OnBnClickedButtonProdcutTab1)
	
	ON_BN_CLICKED(IDC_BUTTON_SELECT_TAB_1, &CFormTab1::OnBnClickedButtonSelectTab1)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_PRODUCT_TAB_1, &CFormTab1::OnNMDblclkListProductTab1)
	ON_BN_CLICKED(IDC_BUTTON_DELETE_TAB_1, &CFormTab1::OnBnClickedButtonDeleteTab1)
END_MESSAGE_MAP()


// CFormTab1 �޽��� ó�����Դϴ�.


BOOL CFormTab1::OnInitDialog()
{
	CDialog::OnInitDialog();
	

	m_pList.SetExtendedStyle(  LVS_EX_FLATSB | LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);

	for(int i = 0 ; i <NUM_COLUMNS; i++)
		m_pList.InsertColumn(i,_gszL1Label[i], LVCFMT_LEFT, _gnLColumnWidth[i]);

	GetProductData();//��ǰ ����Ʈ ������ ��


	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
}



void CFormTab1::OnBnClickedButtonProdcutTab1()
{
	UpdateData(TRUE);

	if(m_strProductName == "")
	{
		AfxMessageBox("��ǰ���� �Է��Ͻʽÿ�.");
		return;
	}
	if(m_dEditPara1 == 0)
	{
		AfxMessageBox("���������� �Ÿ��� �Է��Ͻʽÿ�.");
		return;
	}
	if(m_dEditPara2 == 0)
	{
		AfxMessageBox("������ �Է��Ͻʽÿ�.");
		return;
	}
	if(m_dEditPara3 == 0)
	{
		AfxMessageBox("���Ⱚ�� �Է��Ͻʽÿ�.");
		return;
	}


	for(int i=0; i < m_pList.GetItemCount(); i++)
	{
		if(m_strProductName == m_pList.GetItemText(i,0))
		{
			AfxMessageBox("���� ��ǰ���� �����մϴ�.");
			return;
		}
	}

	int nNum = m_pList.GetItemCount();
	CString strTemp;	
	m_pList.InsertItem(nNum,NULL);

	m_pList.SetItemText(nNum,0,m_strProductName);
	strTemp.Format("%.2lf",m_dEditPara1);
	m_pList.SetItemText(nNum,1,strTemp+" mm");
	strTemp.Format("%.2lf",m_dEditPara2);
	m_pList.SetItemText(nNum,2,strTemp+" mm");
	strTemp.Format("%.2lf",m_dEditPara3);
	m_pList.SetItemText(nNum,3,strTemp+" v");

	CString str;
	str = CFlyCapture2_EXApp::GetEnvValue(PRODUCT_PATH,"PRODUCT", "SET",NULL);
	//str += "|";
	str += m_strProductName + ",";
	strTemp.Format("%.2lf",m_dEditPara1);
	str += strTemp + ",";
	strTemp.Format("%.2lf",m_dEditPara2);
	str += strTemp + ",";
	strTemp.Format("%.2lf",m_dEditPara3);
	str += strTemp + ",";
	str += "0|";

	CFlyCapture2_EXApp::SetEnvValue(PRODUCT_PATH,"PRODUCT", "SET",str);

	m_strProductName="";
	m_dEditPara1=0;
	m_dEditPara2=0;
	m_dEditPara3=0;
	UpdateData(FALSE);

}

void CFormTab1::GetProductData()
{
	
	CString strTemp,str;

	str = CFlyCapture2_EXApp::GetEnvValue(PRODUCT_PATH, "PRODUCT", "SET",NULL);


	int nCnt = 0;
	while(AfxExtractSubString(strTemp,str,nCnt,'|'))
	{

		if(strTemp == "")
			break;

		m_pList.InsertItem(nCnt, NULL);	
		m_pList.SetItemText(nCnt,0,strTemp.Mid(0,strTemp.FindOneOf(",")));
		strTemp = strTemp.Mid(strTemp.FindOneOf(",")+1,strTemp.GetLength());
		m_pList.SetItemText(nCnt,1,strTemp.Mid(0,strTemp.FindOneOf(","))+" mm");
		strTemp = strTemp.Mid(strTemp.FindOneOf(",")+1,strTemp.GetLength());
		m_pList.SetItemText(nCnt,2,strTemp.Mid(0,strTemp.FindOneOf(","))+" mm");
		strTemp = strTemp.Mid(strTemp.FindOneOf(",")+1,strTemp.GetLength());
		m_pList.SetItemText(nCnt,3,strTemp.Mid(0,strTemp.FindOneOf(","))+" v");

		strTemp = strTemp.Mid(strTemp.FindOneOf(",")+1,strTemp.GetLength());

		if(strTemp == "1")
		{
			m_strSelectProduct = m_pList.GetItemText(nCnt,0);
			
			//1�Ÿ�  2����  3���Ⱚ
			m_dPara1 = atof(m_pList.GetItemText(nCnt,1));
			m_dPara2 = atof(m_pList.GetItemText(nCnt,2));
			m_dPara3 = atof(m_pList.GetItemText(nCnt,3));

		}
		nCnt++;
	}

	UpdateData(FALSE);
	
}

void CFormTab1::OnBnClickedButtonSelectTab1()
{

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


			if(MessageBox(m_pList.GetItemText(nItem,0)+" ��ǰ���� ���� �Ͻðڽ��ϱ�?",
				"GLIM",MB_OKCANCEL) != IDOK)
			{
				return;
			}

			m_strSelectProduct = m_pList.GetItemText(nItem,0);
			m_dPara1 = atof(m_pList.GetItemText(nItem,1));
			m_dPara2 = atof(m_pList.GetItemText(nItem,2));
			m_dPara3 = atof(m_pList.GetItemText(nItem,3));			
			break;
		}

	}	

	CString str,strTemp,strSave="";
	str = CFlyCapture2_EXApp::GetEnvValue(PRODUCT_PATH,"PRODUCT", "SET",NULL);

	int nCnt =0;
	while(AfxExtractSubString(strTemp,str,nCnt,'|'))
	{
		if(strTemp=="")
			break;
		nCnt++;

		str1 = strTemp.Mid(0,strTemp.FindOneOf(","));
		if(m_strSelectProduct == str1)
		{
			strSave += strTemp.Mid(0,strTemp.GetLength()-1);
			strSave +="1|";				
			continue;
		}

		strSave += strTemp.Mid(0,strTemp.GetLength()-1);
		strSave +="0|";

	}

	CFlyCapture2_EXApp::SetEnvValue(PRODUCT_PATH,"PRODUCT", "SET",strSave);

	UpdateData(FALSE);
}

void CFormTab1::OnNMDblclkListProductTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	//LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	OnBnClickedButtonSelectTab1();
	*pResult = 0;
}

void CFormTab1::OnBnClickedButtonDeleteTab1()
{
	CListCtrl* pCtrl;
	pCtrl = (CListCtrl*)GetDlgItem(IDC_LIST_PRODUCT_TAB_1);
	CString strSelectProduct;

	POSITION pos = pCtrl->GetFirstSelectedItemPosition();

	if (pos != NULL)
	{
		while (pos)
		{
			int nItem = pCtrl->GetNextSelectedItem(pos);
			pCtrl->SetCheck(nItem , TRUE);
			// you could do your own processing on nItem here
			strSelectProduct = m_pList.GetItemText(nItem,0);
			if(strSelectProduct == m_strSelectProduct)
			{
				AfxMessageBox("�˻����� ��ǰ�� ������ �� �����ϴ�.");
				return;
			}
			m_pList.DeleteItem(nItem);
			break;
		}

	}
	CString str,strTemp;
	str = CFlyCapture2_EXApp::GetEnvValue(PRODUCT_PATH,"PRODUCT", "SET",NULL);

	int nCnt =0;
	while(AfxExtractSubString(strTemp,str,nCnt,'|'))
	{
		CString str1;

		str1 = strTemp.Mid(0,strTemp.FindOneOf(","));
		if(strSelectProduct == str1)
		{

			strTemp +="|";			
			str.Replace(strTemp,"");
			CFlyCapture2_EXApp::SetEnvValue(PRODUCT_PATH,"PRODUCT", "SET",str);
			break;
		}
		nCnt++;

	}

}
