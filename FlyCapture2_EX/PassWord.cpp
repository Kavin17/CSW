// PassWord.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "FlyCapture2_EX.h"
#include "FlyCapture2_EXDlg.h"
#include "PassWord.h"


// CPassWord 대화 상자입니다.

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


// CPassWord 메시지 처리기입니다.

void CPassWord::OnBnClickedButtonCheck()
{
	m_strPassword1 = "";
	UpdateData(FALSE);
	ShowWindow(SW_HIDE);
}

void CPassWord::OnBnClickedCheckModifypass()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
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

	pView->AddtoList("암호 변경 클릭!",1);
	UpdateData();
	
	if(m_strPassword1 == m_strPassword2)
		WritePassword();
	else
		AfxMessageBox("Password가 다릅니다.");

	m_bModifyPass = FALSE;
	m_strPassword1 = m_strPassword2 = m_strPassword3 = "";
	UpdateData(FALSE);
	OnBnClickedCheckModifypass();
}

void CPassWord::ReadPassword()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
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
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
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

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	GetDlgItem(IDC_EDIT_PASS2)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_PASS3)->EnableWindow(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CPassWord::OnBnClickedButtonCheck3()
{
	CFlyCapture2_EXDlg* pView = (CFlyCapture2_EXDlg *)m_hParent;

	UpdateData();
	ReadPassword();
	if(m_strReadPass == m_strPassword1)
	{
		pView->AddtoList("Password 입력 OK",1);
		pView->m_pDlgSet->m_FormTab1->m_bShowHide = TRUE;
		pView->m_pDlgSet->ShowWindow(SW_SHOW);	
		ShowWindow(SW_HIDE);

		
	}
	else
	{
		pView->AddtoList("Password 오류",1);
		pView->m_pDlgSet->m_FormTab1->m_bShowHide = FALSE;
		
	}
	m_strPassword1 = "";
	UpdateData(FALSE);

}
