// PassWord.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "FlyCapture2_EX.h"
#include "FlyCapture2_EXDlg.h"
#include "PassWord.h"


// CPassWord ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CPassWord, CDialog)

CPassWord::CPassWord(CWnd* pParent /*=NULL*/)
	: CDialog(CPassWord::IDD, pParent)
	, m_bModifyPass(FALSE)
	, m_strPassword1(_T(""))
	, m_strPassword2(_T(""))
	, m_strPassword3(_T(""))
{
	m_hParent = pParent;
	m_strReadPass = "";

}

CPassWord::~CPassWord()
{
}

void CPassWord::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK_MODIFYPASS, m_bModifyPass);
	DDX_Text(pDX, IDC_EDIT_PASS1, m_strPassword1);
	DDX_Text(pDX, IDC_EDIT_PASS2, m_strPassword2);
	DDX_Text(pDX, IDC_EDIT_PASS3, m_strPassword3);
}


BEGIN_MESSAGE_MAP(CPassWord, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_CHECK, &CPassWord::OnBnClickedButtonCheck)
	ON_BN_CLICKED(IDC_CHECK_MODIFYPASS, &CPassWord::OnBnClickedCheckModifypass)
	ON_BN_CLICKED(IDC_BUTTON_CHECK2, &CPassWord::OnBnClickedButtonCheck2)
	ON_BN_CLICKED(IDC_BUTTON_CHECK3, &CPassWord::OnBnClickedButtonCheck3)
END_MESSAGE_MAP()


// CPassWord �޽��� ó�����Դϴ�.

void CPassWord::OnBnClickedButtonCheck()
{
	m_strPassword1 = "";
	UpdateData(FALSE);
	ShowWindow(SW_HIDE);
}

void CPassWord::OnBnClickedCheckModifypass()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	//m_bModifyPass;
	UpdateData();
	if(m_bModifyPass)
	{
		GetDlgItem(IDC_EDIT_PASS2)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_PASS3)->EnableWindow(TRUE);

	}
	else
	{
		GetDlgItem(IDC_EDIT_PASS2)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_PASS3)->EnableWindow(FALSE);
	}
}

void CPassWord::OnBnClickedButtonCheck2()
{
	CFlyCapture2_EXDlg* pView = (CFlyCapture2_EXDlg *)m_hParent;

	pView->AddtoList("��ȣ ���� Ŭ��!",1);
	UpdateData();
	
	if(m_strPassword1 == m_strPassword2)
		WritePassword();
	else
		AfxMessageBox("Password�� �ٸ��ϴ�.");

	m_bModifyPass = FALSE;
	m_strPassword1 = m_strPassword2 = m_strPassword3 = "";
	UpdateData(FALSE);
	OnBnClickedCheckModifypass();
}

void CPassWord::ReadPassword()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData();

	CString str;
	CStdioFile   file;
	CString filename;
	str = "";
	int n=0;
	int check = 1;
	filename.Format(_T("C:\\MELSEC\\Pass.glm"));

	if(check==1)//Read
	{
		if(file.Open(filename, CFile::modeReadWrite|CFile::typeText)) 
		{	
			file.ReadString(str);
			m_strReadPass = str;
			file.Close();
		}
		else
			m_strReadPass = "";

	}
	else
	{
		str = m_strPassword2;

		if(file.Open(filename, CFile::modeCreate|CFile::modeReadWrite|CFile::typeText))
		{	
			file.WriteString(str);
			file.Close();
		}
	}
}
void CPassWord::WritePassword()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData();

	CString str;
	CStdioFile   file;
	CString filename;
	str = "";
	int n=0;
	int check = 0;
	filename.Format(_T("C:\\MELSEC\\Pass.glm"));

	if(check==1)//Read
	{
		if(file.Open(filename, CFile::modeReadWrite|CFile::typeText)) 
		{	
			file.ReadString(str);
			m_strReadPass = str;
			file.Close();
		}
	}
	else
	{
		str = m_strPassword3;

		if(file.Open(filename, CFile::modeCreate|CFile::modeReadWrite|CFile::typeText))
		{	
			file.WriteString(str);
			file.Close();
		}
		m_strPassword2 = m_strPassword3;
		m_bModifyPass = TRUE;
		
		OnBnClickedCheckModifypass();
		


	}
}


BOOL CPassWord::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	GetDlgItem(IDC_EDIT_PASS2)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_PASS3)->EnableWindow(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

void CPassWord::OnBnClickedButtonCheck3()
{
	CFlyCapture2_EXDlg* pView = (CFlyCapture2_EXDlg *)m_hParent;

	UpdateData();
	ReadPassword();
	if(m_strReadPass == m_strPassword1)
	{
		pView->AddtoList("Password �Է� OK",1);
		pView->m_pDlgSet->m_FormTab1->m_bShowHide = TRUE;
		pView->m_pDlgSet->ShowWindow(SW_SHOW);	
		ShowWindow(SW_HIDE);

		
	}
	else
	{
		pView->AddtoList("Password ����",1);
		pView->m_pDlgSet->m_FormTab1->m_bShowHide = FALSE;
		
	}
	m_strPassword1 = "";
	UpdateData(FALSE);

}
