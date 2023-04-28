// ModelCount.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "FlyCapture2_EX.h"
#include "FlyCapture2_EXDlg.h"

#include "ModelCount.h"


// CModelCount ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CModelCount, CDialog)

CModelCount::CModelCount(CWnd* pParent /*=NULL*/)
	: CDialog(CModelCount::IDD, pParent)
	, m_nPackCount(200)
{
	m_pParent = pParent;
	m_nModelSelect = 1;
	m_nModelShow = 0;
	m_strSelectModel = "";
	m_nOkNGCount[0]=m_nOkNGCount[1]=0;
	m_nMesResult = 0;
}

CModelCount::~CModelCount()
{
}

void CModelCount::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_LABEL_SELECT_MODEL, m_LabelModelName);
	DDX_Text(pDX, IDC_EDIT_PACL_COUNT, m_nPackCount);
	DDX_Control(pDX, IDC_STATIC_LABEL_SELECT_MODEL2, m_ctClickCheck);
}


BEGIN_MESSAGE_MAP(CModelCount, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_MODEL1, &CModelCount::OnBnClickedButtonModel1)
	ON_BN_CLICKED(IDC_BUTTON_MODEL2, &CModelCount::OnBnClickedButtonModel2)
	ON_BN_CLICKED(IDC_BUTTON_MODEL3, &CModelCount::OnBnClickedButtonModel3)
	ON_BN_CLICKED(IDC_BUTTON_MODEL4, &CModelCount::OnBnClickedButtonModel4)
	ON_BN_CLICKED(IDC_BUTTON_MODEL5, &CModelCount::OnBnClickedButtonModel5)
	ON_BN_CLICKED(IDC_BUTTON_MODEL6, &CModelCount::OnBnClickedButtonModel6)
	ON_BN_CLICKED(IDC_BUTTON_MODEL7, &CModelCount::OnBnClickedButtonModel7)
	ON_BN_CLICKED(IDC_BUTTON_MODEL8, &CModelCount::OnBnClickedButtonModel8)
	ON_BN_CLICKED(IDC_BUTTON_NEXT, &CModelCount::OnBnClickedButtonNext)
	ON_BN_CLICKED(IDC_BUTTON_MODEL9, &CModelCount::OnBnClickedButtonModel9)
	ON_BN_CLICKED(IDC_BUTTON_MODEL10, &CModelCount::OnBnClickedButtonModel10)
	ON_BN_CLICKED(IDC_BUTTON_MODEL11, &CModelCount::OnBnClickedButtonModel11)
	ON_BN_CLICKED(IDC_BUTTON_MODEL12, &CModelCount::OnBnClickedButtonModel12)
	ON_BN_CLICKED(IDC_BUTTON_OK, &CModelCount::OnBnClickedButtonOk)
	ON_BN_CLICKED(IDC_BUTTON_NG, &CModelCount::OnBnClickedButtonNg)
END_MESSAGE_MAP()


// CModelCount �޽��� ó�����Դϴ�.

void CModelCount::OnBnClickedButtonModel1()
{
	m_nModelSelect = 1;
	CFlyCapture2_EXDlg *pDlg = (CFlyCapture2_EXDlg*)m_pParent;
	pDlg->m_strModelType = "I";
	CheckModel(m_nModelSelect);
}

void CModelCount::OnBnClickedButtonModel2()
{
	m_nModelSelect = 2;
	CFlyCapture2_EXDlg *pDlg = (CFlyCapture2_EXDlg*)m_pParent;
	pDlg->m_strModelType = "E";
	CheckModel(m_nModelSelect);
}

void CModelCount::OnBnClickedButtonModel3()
{
	m_nModelSelect = 3;
	CFlyCapture2_EXDlg *pDlg = (CFlyCapture2_EXDlg*)m_pParent;
	pDlg->m_strModelType = "D";
	CheckModel(m_nModelSelect);
}

void CModelCount::OnBnClickedButtonModel4()
{
	m_nModelSelect = 4;
	CFlyCapture2_EXDlg *pDlg = (CFlyCapture2_EXDlg*)m_pParent;
	pDlg->m_strModelType = "B";
	CheckModel(m_nModelSelect);
}

void CModelCount::OnBnClickedButtonModel5()
{
	m_nModelSelect = 5;
	CFlyCapture2_EXDlg *pDlg = (CFlyCapture2_EXDlg*)m_pParent;
	pDlg->m_strModelType = "C";
	CheckModel(m_nModelSelect);
}

void CModelCount::OnBnClickedButtonModel6()
{
	m_nModelSelect = 6;
	CFlyCapture2_EXDlg *pDlg = (CFlyCapture2_EXDlg*)m_pParent;
	pDlg->m_strModelType = "L";
	CheckModel(m_nModelSelect);
}

void CModelCount::OnBnClickedButtonModel7()
{
	m_nModelSelect = 7;
	CFlyCapture2_EXDlg *pDlg = (CFlyCapture2_EXDlg*)m_pParent;
	pDlg->m_strModelType = "G";
	CheckModel(m_nModelSelect);
}

void CModelCount::OnBnClickedButtonModel8()
{
	m_nModelSelect = 8;
	CFlyCapture2_EXDlg *pDlg = (CFlyCapture2_EXDlg*)m_pParent;
	pDlg->m_strModelType = "R";

	CheckModel(m_nModelSelect);
}

void CModelCount::CheckModel(int nModel)
{
	m_nPackCount = 100*m_nModelSelect;
	m_LabelModelName.SetTextColor(COLOR_WHITE);
	m_LabelModelName.SetBkColor(RGB(80,100,115));
	m_LabelModelName.SetFontBold(TRUE);
	m_LabelModelName.SetFontName(_T("����"));
	m_LabelModelName.SetFontSize(16);

	CFlyCapture2_EXDlg *pDlg = (CFlyCapture2_EXDlg*)m_pParent;

	if(nModel==0)
	{
		m_strSelectModel.Format("�Ｚ����");
		m_LabelModelName.SetText(" ���� �� : �Ｚ����");

	}
	else if(nModel==1)
	{
		m_strSelectModel.Format("�Ｚ����");
		m_LabelModelName.SetText(" ���� �� : �Ｚ����");

	}
	else if(nModel==2)
	{
		m_strSelectModel.Format("�Ｚ����");
		m_LabelModelName.SetText(" ���� �� : �Ｚ����");

	}
	else if(nModel==3)
	{
		m_strSelectModel.Format("����������");
		m_LabelModelName.SetText(" ���� �� : ����������");

	}
	else if(nModel==4)
	{
		m_strSelectModel.Format("õ���������");
		m_LabelModelName.SetText(" ���� �� : õ���������");

	}
	else if(nModel==5)
	{
		m_strSelectModel.Format("õ��������");
		m_LabelModelName.SetText(" ���� �� : õ��������");

	}
	else if(nModel==6)
	{
		m_strSelectModel.Format("LGD");
		m_LabelModelName.SetText(" ���� �� : LGD");

	}
	else if(nModel==7)
	{
		m_strSelectModel.Format("LGD �������");
		m_LabelModelName.SetText(" ���� �� : LGD�������");
	}
	else if(nModel==8)
	{
		m_strSelectModel.Format("������ R/W");
		m_LabelModelName.SetText(" ���� �� : ������R/W");
	}
	else if(nModel==9){
		m_strSelectModel.Format("Ÿ��1");
		m_LabelModelName.SetText(" ���� �� : Ÿ��1");
	}else if(nModel==10){
		m_strSelectModel.Format("Ÿ��2");
		m_LabelModelName.SetText(" ���� �� : Ÿ��2");
	}else if(nModel==11){
		m_strSelectModel.Format("Ÿ��3");
		m_LabelModelName.SetText(" ���� �� : Ÿ��3");
	}else if(nModel==12){
		m_strSelectModel.Format("Ÿ��4");
		m_LabelModelName.SetText(" ���� �� : Ÿ��4");
	}else if(nModel==13){
		m_strSelectModel.Format("Ÿ��5");
		m_LabelModelName.SetText(" ���� �� : Ÿ��5");
	}else{
		m_strSelectModel.Format("�Ｚ����");
		m_LabelModelName.SetText(" ���� �� : �Ｚ����");
	}
	pDlg->AddtoList(m_strSelectModel,1);

	if(nModel>8)
		AfxMessageBox("���� ��ϵ��� �ʾҽ��ϴ�. �ٽ� �����ϼ���.");
	else
	{
		OnBnClickedButtonNext();
	}
		
	UpdateData(FALSE);
}

BOOL CModelCount::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_LabelModelName.SetTextColor(COLOR_WHITE);
	m_LabelModelName.SetBkColor(RGB(80,100,115));
	m_LabelModelName.SetFontBold(TRUE);
	m_LabelModelName.SetFontName(_T("����"));
	m_LabelModelName.SetFontSize(16);
	m_LabelModelName.SetText(" ���� �� : ��1");
//	m_strSelectModel.Format("�Ｚ����");
	m_ctClickCheck.SetTextColor(COLOR_WHITE);
	m_ctClickCheck.SetBkColor(RGB(80,100,115));
	m_ctClickCheck.SetFontBold(TRUE);
	m_ctClickCheck.SetFontName(_T("����"));
	m_ctClickCheck.SetFontSize(16);
	m_ctClickCheck.SetText("");


	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

void CModelCount::OnBnClickedButtonNext()
{
	ShowWindow(SW_HIDE);
	m_nModelShow = 0;
	CFlyCapture2_EXDlg *pDlg = (CFlyCapture2_EXDlg*)m_pParent;

	pDlg->m_DlgBarcode->m_nMEsNgCount = 0;
	pDlg->m_DlgBarcode->m_strReadBarcode = "";
	pDlg->m_DlgBarcode->m_strSelectModel.Format("����� : %s",m_strSelectModel);
	pDlg->m_DlgBarcode->ShowWindow(SW_SHOW);	
	pDlg->m_DlgBarcode->UpdateData(FALSE);
	pDlg->m_DlgBarcode->m_bAutoHide = FALSE;
	pDlg->m_DlgBarcode->SetTimer(105,500,NULL);
	pDlg->m_DlgBarcode->SetTimer(125,500,NULL);
}

void CModelCount::OnBnClickedButtonModel9()
{
	m_nModelSelect = 9;
	CheckModel(m_nModelSelect);
}

void CModelCount::OnBnClickedButtonModel10()
{
	m_nModelSelect = 10;
	CheckModel(m_nModelSelect);
}

void CModelCount::OnBnClickedButtonModel11()
{
	m_nModelSelect = 11;
	CheckModel(m_nModelSelect);
}

void CModelCount::OnBnClickedButtonModel12()
{
	m_nModelSelect = 12;
	CheckModel(m_nModelSelect);
}


void CModelCount::ShowLabel(CString str)
{
	m_LabelModelName.SetText(str);
}

void CModelCount::OnBnClickedButtonOk()
{
	CFlyCapture2_EXDlg *pDlg = (CFlyCapture2_EXDlg*)m_pParent;
	CString str;
	m_nOkNGCount[0]++;
	str.Format("��ǰ ����  %dȸ ����",m_nOkNGCount[0]);
	pDlg->AddtoList(str,1);
	pDlg->WriteDetailLog(str);
	m_ctClickCheck.SetText(str);
	if(m_nOkNGCount[0]>=2)
	{
		m_nMesResult = 102;
		m_nOkNGCount[0] = 0;
		m_nOkNGCount[1] = 0;
		ShowWindow(SW_HIDE);
		pDlg->m_nWaitResult=1;
	}
}

void CModelCount::OnBnClickedButtonNg()
{
	CFlyCapture2_EXDlg *pDlg = (CFlyCapture2_EXDlg*)m_pParent;
	CString str;
	m_nOkNGCount[1]++;
	str.Format("�ҷ� ����  %dȸ ����",m_nOkNGCount[1]);
	pDlg->AddtoList(str,1);
	pDlg->WriteDetailLog(str);
	m_ctClickCheck.SetText(str);
	if(m_nOkNGCount[1]>=2)
	{
		m_nMesResult = 202;
		m_nOkNGCount[0] = 0;
		m_nOkNGCount[1] = 0;
		ShowWindow(SW_HIDE);
		pDlg->m_nWaitResult=2;
	}
}