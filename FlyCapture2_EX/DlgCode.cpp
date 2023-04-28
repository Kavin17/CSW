// DlgCode.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "FlyCapture2_EX.h"
#include "FlyCapture2_EXDlg.h"

#include "DlgCode.h"
#include "ini.h"


// CDlgCode ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CDlgCode, CDialog)

CDlgCode::CDlgCode(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgCode::IDD, pParent)
	, m_strBarCode(_T(""))
	, m_strMidCode(_T(""))
	, m_strPartNo(_T(""))
	, m_strUpLow(_T(""))
	, m_strOCVCode(_T(""))
	, m_strInch(_T(""))
	, m_strLongLen(_T(""))
	, m_strShortLen(_T(""))
	, m_nMakeCount(200)
	, m_strPlcLot(_T(""))
	, m_nPlcMovemm(380)
	, m_nPlcMoveSpeed(1200)
	, m_dPlcBlowerTime(1)
	, m_dPlcNg2Time(1)
	, m_dPlcOkAlignTime(1)
	, m_dOcvPosX(100)
	, m_dOcvPosY(100)
	, m_bShowItem(FALSE)
	, m_strPLCLongLen(_T(""))
	, m_strPLCShortLen(_T(""))
	, m_strReadBarcode(_T(""))
	, m_strLGSdc(_T(""))
	, m_nOkDelayCount(0)
	, m_nLiftCount(0)
	, m_bAutoHide(TRUE)
	, m_nBlowerCount1(0)
	, m_dDelayTime1(0)
	, m_strSelectModel(_T(""))
	, m_nMEsNgCount(0)
{
	m_pParent = pParent;
	 m_nPlcBlowerCount=9;
	 m_dUpDelay= 12;
	 m_nVacuumCount= 12;
	 m_nReturnCount= 3;

}

CDlgCode::~CDlgCode()
{
}

void CDlgCode::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_BARCODE, m_strBarCode);
	DDX_Text(pDX, IDC_EDIT_MARK, m_strMidCode);
	DDX_Text(pDX, IDC_EDIT_PARTNO, m_strPartNo);
	DDX_Text(pDX, IDC_EDIT_UPLOW, m_strUpLow);
	DDX_Text(pDX, IDC_EDIT_CODE, m_strOCVCode);
	DDX_Text(pDX, IDC_EDIT_INCH, m_strInch);
	DDX_Text(pDX, IDC_EDIT_YLENGTH, m_strLongLen);
	DDX_Text(pDX, IDC_EDIT_SHORTLENGTH, m_strShortLen);
	DDX_Text(pDX, IDC_EDIT_MAKECOUNT, m_nMakeCount);
	DDX_Text(pDX, IDC_EDIT_PLCLOT, m_strPlcLot);
	DDX_Text(pDX, IDC_EDIT_PLCMOVEMM, m_nPlcMovemm);
	DDX_Text(pDX, IDC_EDIT_PLCMOVESPEED, m_nPlcMoveSpeed);
	DDX_Text(pDX, IDC_EDIT_PLCBLOWER, m_dPlcBlowerTime);
	DDX_Text(pDX, IDC_EDIT_PLCNG2UPTIME, m_dPlcNg2Time);
	DDX_Text(pDX, IDC_EDIT_PLCOKALIGNTIME, m_dPlcOkAlignTime);
	DDX_Text(pDX, IDC_EDIT_OCRLIMIT_X, m_dOcvPosX);
	DDX_Text(pDX, IDC_EDIT_OCRLIMIT_Y, m_dOcvPosY);
	DDX_Check(pDX, IDC_CHECK_SHOW, m_bShowItem);
	DDX_Text(pDX, IDC_EDIT_YLENGTH2, m_strPLCLongLen);
	DDX_Text(pDX, IDC_EDIT_SHORTLENGTH2, m_strPLCShortLen);
	DDX_Text(pDX, IDC_EDIT_READ_BARCODE, m_strReadBarcode);
	DDX_Text(pDX, IDC_STATIC_LGSDC, m_strLGSdc);
	DDX_Text(pDX, IDC_EDIT_PLCOKDELAYCOUNT, m_nOkDelayCount);
	DDX_Text(pDX, IDC_EDIT_PLCLIFTHEIGHTCOUNT, m_nLiftCount);
	DDX_Check(pDX, IDC_CHECK_AUTOHIDE, m_bAutoHide);
	DDX_Text(pDX, IDC_EDIT_PLCBLOWERCOUNT, m_nBlowerCount1);
	DDX_Text(pDX, IDC_EDIT_PLCUPDELAY, m_dDelayTime1);
	DDX_Text(pDX, IDC_STATIC_SELECTMODEL, m_strSelectModel);
	DDX_Text(pDX, IDC_EDIT_MES_NGCOUNT, m_nMEsNgCount);
}


BEGIN_MESSAGE_MAP(CDlgCode, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_MODIFY, &CDlgCode::OnBnClickedButtonModify)
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, &CDlgCode::OnBnClickedButtonClose)
	ON_BN_CLICKED(IDC_BUTTON_CLOSE2, &CDlgCode::OnBnClickedButtonClose2)
	ON_BN_CLICKED(IDC_BUTTON4, &CDlgCode::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_CHECK_SHOW, &CDlgCode::OnBnClickedCheckShow)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_ADJUST, &CDlgCode::OnBnClickedButtonAdjust)
	ON_BN_CLICKED(IDC_CHECK_AUTOHIDE, &CDlgCode::OnBnClickedCheckAutohide)
	ON_BN_CLICKED(IDC_BUTTON6, &CDlgCode::OnBnClickedButton6)
	ON_WM_RBUTTONDOWN()
END_MESSAGE_MAP()


// CDlgCode �޽��� ó�����Դϴ�.

void CDlgCode::OnBnClickedButtonModify()
{
	UpdateData(TRUE);
	CFlyCapture2_EXDlg *pDlg = (CFlyCapture2_EXDlg*)m_pParent;
	if(m_strInch != "")
	{
	}

//	UpdateIni2(FALSE);//
	//if(MessageBox("����� ���� �����Ͻðڽ��ϱ�?",
	//	"GLIM",MB_OKCANCEL) == IDOK)
	//{
	//	WriteModelData();
	//}
	//DAta write

	//PLC Write

}

void CDlgCode::OnBnClickedButtonClose()
{
	CFlyCapture2_EXDlg *pDlg = (CFlyCapture2_EXDlg*)m_pParent;
//	pDlg->Invalidate();
//	KillTimer(125);
//	KillTimer(105);
////m_nMEsNgCount
//	pDlg->m_nShowSet = 0;
////	pDlg->OnBnClickedStart();
//	pDlg->StartInsp();

	UpdateData();
	//0109
	//if(m_nMEsNgCount==-1)
	//{
	//	AfxMessageBox("MES NG ������ �Է��� �ٽ� �ڵ��˻� ��ư�� ��������!");
	//	return;
	//}
	pDlg->StartInsp();
	KillTimer(125);//bcr check
	KillTimer(105);//focus
	EditColtol1(FALSE);

	ShowWindow(SW_HIDE);

}

void CDlgCode::WriteModelData()
{
	CStdioFile   file,file1;
	CString filename,filename1;
	CString strpath("C:\\GLIM\\");
	CString strheader;
	
	filename.Format(_T("c:\\Glim\\NewModel.txt"));
	filename1.Format(_T("c:\\Glim\\NewModel1.txt"));
	CString strtmp1,strtmp2,str1;
	strtmp1 = m_strMidCode;
	int nmax = 1000;

	if(file.Open(filename, CFile::modeReadWrite|CFile::typeText)) 
	{	
		if(file1.Open(filename1, CFile::modeCreate|CFile::modeReadWrite|CFile::typeText))
		{
			for(int i=0;i<nmax;i++)
			{
				file.ReadString(str1);	
				if(str1 == "") break;
				if(str1.Find(strtmp1)>=0)
				{//���� ����
//					strtmp2.Format("%s\t%s\t%s\t%s\t%s\t%s\t%s",m_strPartNo,m_strMidCode,m_strUpLow,m_strOCVCode,m_strInch,m_strLongLen,m_strShortLen);
					strtmp2.Format("%s\t%s%s\t%s%s%s\t%s",m_strPartNo,m_strMidCode,m_strUpLow,m_strOCVCode,m_strInch,m_strLongLen,m_strShortLen);
					file1.WriteString(strtmp2);
					file1.WriteString("\n");
				}
				else
				{
					file1.WriteString(str1);
					file1.WriteString("\n");
				}

			}
			file1.Close();
		}
		file.Close();
	}
	//deletefile
//	DeleteFile(filename);
//	rename(filename1,filename);
	//6308L-1774A	SXB	����	1774T (����ȣ)-(���ȣ)	15.6 	352.50 	199.40
}

void CDlgCode::OnBnClickedButtonClose2()
{
	CFlyCapture2_EXDlg *pDlg = (CFlyCapture2_EXDlg*)m_pParent;
	KillTimer(125);
	KillTimer(105);

	ShowWindow(SW_HIDE);
		pDlg->m_nShowSet = 0;
		//m_strReadBarcode
}

void CDlgCode::OnBnClickedButton4()
{
	CFlyCapture2_EXDlg *pDlg = (CFlyCapture2_EXDlg*)m_pParent;
	UpdateData();
	//plc send
	LONG vlist;// = atol(m_strPlcValue);


	UpdateIni(FALSE);
}


void CDlgCode::WritePlc()
{
	CFlyCapture2_EXDlg *pDlg = (CFlyCapture2_EXDlg*)m_pParent;
	UpdateData();


	UpdateIni(FALSE);

}
void CDlgCode::OnBnClickedCheckShow()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData();
	if(m_bShowItem)
	{
		KillTimer(125);
		KillTimer(105);
	}
	else
	{
		SetTimer(125,500,NULL);
		SetTimer(105,500,NULL);
		
	}
	EditControl(m_bShowItem);
}

BOOL CDlgCode::OnInitDialog()
{
	CDialog::OnInitDialog();

	//EditControl(FALSE);

	//EditColtol1(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

void CDlgCode::EditControl(BOOL bis)
{
	GetDlgItem(IDC_EDIT_BARCODE)->EnableWindow(bis);
	GetDlgItem(IDC_EDIT_MARK)->EnableWindow(bis);
	GetDlgItem(IDC_EDIT_PARTNO)->EnableWindow(bis);
	GetDlgItem(IDC_EDIT_UPLOW)->EnableWindow(bis);
	GetDlgItem(IDC_EDIT_CODE)->EnableWindow(bis);
	GetDlgItem(IDC_EDIT_INCH)->EnableWindow(bis);
	GetDlgItem(IDC_EDIT_YLENGTH)->EnableWindow(bis);
	GetDlgItem(IDC_EDIT_SHORTLENGTH)->EnableWindow(bis);
//	GetDlgItem(IDC_EDIT_MAKECOUNT)->EnableWindow(bis);
	GetDlgItem(IDC_EDIT_OCRLIMIT_X)->EnableWindow(bis);
	GetDlgItem(IDC_EDIT_OCRLIMIT_Y)->EnableWindow(bis);

}

void CDlgCode::UpdateIni(BOOL bLoad)
{
	if(bLoad==FALSE)
		UpdateData(TRUE);
	CFlyCapture2_EXDlg *pDlg = (CFlyCapture2_EXDlg*)m_pParent;

	CString strPath;


	strPath.Format("c:\\Glim\\Model\\%s.ini",pDlg->m_pDlgSet->m_strSelectProduct);

	CIni ini1(strPath, "PLC����" , 1 );
	ini1.SER_GETD(bLoad, m_nPlcMovemm	, 380);//�̼۰Ÿ�
	ini1.SER_GETD(bLoad, m_nPlcMoveSpeed	, 1200);//�̼ۼӵ�
	ini1.SER_GETD(bLoad, m_nMakeCount, 200);//����ż�
	ini1.SER_GETD(bLoad, m_dPlcBlowerTime, 1);
	ini1.SER_GETD(bLoad, m_dPlcNg2Time, 1);
	ini1.SER_GETD(bLoad, m_dPlcOkAlignTime, 1);
	ini1.SER_GETD(bLoad, m_strPLCLongLen, "350");
	ini1.SER_GETD(bLoad, m_strPLCShortLen, "250");


	ini1.SER_GETD(bLoad, m_strPartNo, "250");
	ini1.SER_GETD(bLoad, m_strPlcLot, "250");
	ini1.SER_GETD(bLoad, m_strUpLow, "����");
	ini1.SER_GETD(bLoad, m_strOCVCode, "");
	ini1.SER_GETD(bLoad, m_strInch, "");
	ini1.SER_GETD(bLoad, m_strLongLen, "250");
	ini1.SER_GETD(bLoad, m_strShortLen, "250");

	ini1.SER_GETD(bLoad, m_nPlcBlowerCount, 9);//��ο�
	ini1.SER_GETD(bLoad, m_dUpDelay, 12);
	ini1.SER_GETD(bLoad, m_nVacuumCount, 12);//����
	ini1.SER_GETD(bLoad, m_nReturnCount, 3);

/*
	strPath.Format("%sModel\\PLC\\%s_setup.ini",GLIM_PATH,m_strInch);
	CIni ini12(strPath, "PLC����" , 1 );
	ini12.SER_GETD(bLoad, m_nPlcMovemm	, 380);//�̼۰Ÿ�
	ini12.SER_GETD(bLoad, m_nPlcMoveSpeed	, 1200);//�̼ۼӵ�
	ini12.SER_GETD(bLoad, m_nMakeCount, 200);//����ż�
	ini12.SER_GETD(bLoad, m_dPlcBlowerTime, 1);
	ini12.SER_GETD(bLoad, m_dPlcNg2Time, 1);
	ini12.SER_GETD(bLoad, m_dPlcOkAlignTime, 1);
	ini12.SER_GETD(bLoad, m_strPLCLongLen, "350");
	ini12.SER_GETD(bLoad, m_strPLCShortLen, "250");
*/
	if(bLoad)
		UpdateData(FALSE);
}

void CDlgCode::UpdateIni1(BOOL bLoad)
{
	if(bLoad==FALSE)
		UpdateData(TRUE);
	CFlyCapture2_EXDlg *pDlg = (CFlyCapture2_EXDlg*)m_pParent;

	CString strPath;
//	strPath.Format("%sModel\\PLC\\%s_setup.ini",GLIM_PATH,m_strMidCode);
//	CreateDirectory("c:\\Glim\\Model\\PLC\\",NULL);

	strPath.Format("c:\\Glim\\Model\\%s.ini",pDlg->m_pDlgSet->m_strSelectProduct);

	CFileFind finder;
	BOOL isfind;
	isfind = finder.FindFile(strPath);

	m_bAutoHide = TRUE;//FALSE;
	if(isfind)
	{
		CIni ini1(strPath, "PLC����" , 1 );
		ini1.SER_GETD(bLoad, m_nPlcMovemm	, 560);//�̼۰Ÿ�
		ini1.SER_GETD(bLoad, m_nPlcMoveSpeed	, 1000);//�̼ۼӵ�
		ini1.SER_GETD(bLoad, m_nMakeCount, 200);//����ż�
		ini1.SER_GETD(bLoad, m_dPlcBlowerTime, 1);//��ο��ð�
		ini1.SER_GETD(bLoad, m_nBlowerCount1, 1);//��ο�����
		ini1.SER_GETD(bLoad, m_dDelayTime1, 1);	//�з���½ð�
		ini1.SER_GETD(bLoad, m_dPlcNg2Time, 1);	//�ε���������
		ini1.SER_GETD(bLoad, m_dPlcOkAlignTime, 1);//�̼���������
		ini1.SER_GETD(bLoad, m_nOkDelayCount, 1);	//��ǰ����
		ini1.SER_GETD(bLoad, m_nLiftCount, 1);//����Ʈ����
		ini1.SER_GETD(bLoad, m_strPLCLongLen, "350");//
		ini1.SER_GETD(bLoad, m_strPLCShortLen, "250");//
	}
	else
	{
		m_bAutoHide = TRUE;// FALSE;
		CIni ini1(strPath, "PLC����" , 1 );
		ini1.SER_GETD(bLoad, m_nPlcMovemm	, 560);//�̼۰Ÿ�
		ini1.SER_GETD(bLoad, m_nPlcMoveSpeed	, 1000);//�̼ۼӵ�
//		ini1.SER_GETD(bLoad, m_nMakeCount, 200);//����ż�
		ini1.SER_GETD(bLoad, m_dPlcBlowerTime, 1);//��ο��ð�
		ini1.SER_GETD(bLoad, m_nBlowerCount1, 6);//��ο�����
		ini1.SER_GETD(bLoad, m_dDelayTime1, 1);	//�з���½ð�
		ini1.SER_GETD(bLoad, m_dPlcNg2Time, 8);	//�ε���������
		ini1.SER_GETD(bLoad, m_dPlcOkAlignTime, 8);//�̼���������
		ini1.SER_GETD(bLoad, m_nOkDelayCount, 5);	//��ǰ����
		ini1.SER_GETD(bLoad, m_nLiftCount, 1);//����Ʈ����
		ini1.SER_GETD(bLoad, m_strPLCLongLen, "350");//
		ini1.SER_GETD(bLoad, m_strPLCShortLen, "250");//
		UpdateData(FALSE);
		AfxMessageBox("�ű� ���� �ʿ�!!");
	}

	if(bLoad)
		UpdateData(FALSE);
	finder.Close();
}


void CDlgCode::UpdateIni2(BOOL bLoad)
{
	if(bLoad==FALSE)
		UpdateData(TRUE);
	CFlyCapture2_EXDlg *pDlg = (CFlyCapture2_EXDlg*)m_pParent;

	CString strPath;
//	strPath.Format("%sModel\\PLC\\%s_setup.ini",GLIM_PATH,m_strMidCode);
//	CreateDirectory("c:\\Glim\\Model\\PLC\\",NULL);

//	strPath+="Model\\"+pDlg->m_pDlgSet->m_FormTab1->m_strSelectProduct+".ini";

	strPath.Format("c:\\Glim\\Model\\PLC.ini");

	CFileFind finder;
	BOOL isfind;
	isfind = finder.FindFile(strPath);
	CString strInch;
	strInch.Format("%s - ��ġ", m_strInch);

	m_bAutoHide =TRUE;// FALSE;
	if(isfind)
	{
		CIni ini3(strPath, strInch , 0 );
		ini3.SER_GETD(bLoad, m_nPlcMovemm	, 560);//�̼۰Ÿ�
		ini3.SER_GETD(bLoad, m_nPlcMoveSpeed	, 1000);//�̼ۼӵ�
//		ini1.SER_GETD(bLoad, m_nMakeCount, 200);//����ż�
		ini3.SER_GETD(bLoad, m_dPlcBlowerTime, 1);//��ο��ð�
		ini3.SER_GETD(bLoad, m_nBlowerCount1, 1);//��ο�����
		ini3.SER_GETD(bLoad, m_dDelayTime1, 1);	//�з���½ð�
		ini3.SER_GETD(bLoad, m_dPlcNg2Time, 1);	//�ε���������
		ini3.SER_GETD(bLoad, m_dPlcOkAlignTime, 1);//�̼���������
		ini3.SER_GETD(bLoad, m_nOkDelayCount, 1);	//��ǰ����
		ini3.SER_GETD(bLoad, m_nLiftCount, 1);//����Ʈ����
		ini3.SER_GETD(bLoad, m_strPLCLongLen, "350");//
		ini3.SER_GETD(bLoad, m_strPLCShortLen, "250");//
	}
	else
	{
		m_bAutoHide = TRUE;//FALSE;
		CIni ini3(strPath, strInch , 0 );
		ini3.SER_GETD(bLoad, m_nPlcMovemm	, 560);//�̼۰Ÿ�
		ini3.SER_GETD(bLoad, m_nPlcMoveSpeed	, 1000);//�̼ۼӵ�
//		ini1.SER_GETD(bLoad, m_nMakeCount, 200);//����ż�
		ini3.SER_GETD(bLoad, m_dPlcBlowerTime, 1);//��ο��ð�
		ini3.SER_GETD(bLoad, m_nBlowerCount1, 6);//��ο�����
		ini3.SER_GETD(bLoad, m_dDelayTime1, 1);	//�з���½ð�
		ini3.SER_GETD(bLoad, m_dPlcNg2Time, 8);	//�ε���������
		ini3.SER_GETD(bLoad, m_dPlcOkAlignTime, 8);//�̼���������
		ini3.SER_GETD(bLoad, m_nOkDelayCount, 5);	//��ǰ����
		ini3.SER_GETD(bLoad, m_nLiftCount, 1);//����Ʈ����
		ini3.SER_GETD(bLoad, m_strPLCLongLen, "350");//
		ini3.SER_GETD(bLoad, m_strPLCShortLen, "250");//
		UpdateData(FALSE);
//		AfxMessageBox("�ű� ���� �ʿ�!!");
	}

	if(bLoad)
		UpdateData(FALSE);
	finder.Close();
}

void CDlgCode::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	if(nIDEvent == 105)
	{
		CEdit* pFocus = (CEdit*)GetDlgItem(IDC_EDIT_READ_BARCODE);
		pFocus->SetFocus();

	}
	if(nIDEvent == 125)//bcr check
	{
	}
	if(nIDEvent == 141)
	{
		
	}

	CDialog::OnTimer(nIDEvent);
}

void CDlgCode::OnBnClickedButtonAdjust()
{
	UpdateData(FALSE);
	//Find model
	CFlyCapture2_EXDlg *pDlg = (CFlyCapture2_EXDlg*)m_pParent;
//	if(m_nMEsNgCount>-1)//0109
		pDlg->StopInsp();
	//else{//0109
	//	AfxMessageBox("������ Ȯ���� MES ������ ��������.");
		return;
//	}
//	pDlg->m_strBarCode = m_strReadBarcode;
//	SetTimer(125,500,NULL);
	//EditColtol1(FALSE);

	ShowWindow(SW_HIDE);
}

void CDlgCode::OnBnClickedCheckAutohide()
{//m_bAutoHide
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData();
}


void CDlgCode::UpdatePlc()
{
	CString str,str1,strtmp;

	CStdioFile   file;
	CString filename;
	int nmax = 1000;
	str = "";
	int n,n1,nlen;
	int cnt = -1;
	int nc = atoi(m_strInch);
	strtmp.Format("%d ��ġ PLC ������ �����ϴ�.",nc);

	filename.Format(_T("c:\\Glim\\InchInfo\\%d.txt"),nc);

	if(file.Open(filename, CFile::modeReadWrite|CFile::typeText)) 
	{	
		file.ReadString(str1);	//�̼۰Ÿ�
		m_nPlcMovemm = atoi(str1);
		file.ReadString(str1);	//�̼ۼӵ�
		m_nPlcMoveSpeed = atoi(str1);
		file.ReadString(str1);	//��ο��ð�
		m_dPlcBlowerTime = atof(str1);
		file.ReadString(str1);	//��ο�����
		m_nBlowerCount1 = atoi(str1);
		file.ReadString(str1);	//�з���½ð�
		m_dDelayTime1 = atof(str1);
		file.ReadString(str1);	//�ε���������
		m_dPlcNg2Time = atof(str1);
		file.ReadString(str1);	//�̼���������
		m_dPlcOkAlignTime = atof(str1);
		file.ReadString(str1);	//��ǰ����
		m_nOkDelayCount = atoi(str1);
		file.ReadString(str1);	//����Ʈ����
		m_nLiftCount = atoi(str1);
		//file.ReadString(str1);	
		//m_nOkDelayCount = atoi(str1);
		//file.ReadString(str1);	
		//m_nOkDelayCount = atoi(str1);
		//file.ReadString(str1);	
		//m_nOkDelayCount = atoi(str1);
		file.Close();
	}
	else
	{
		m_nPlcMovemm = 560;//atoi(str1);//�̼۰Ÿ�
		m_nPlcMoveSpeed = 1000;//atoi(str1);//�̼ۼӵ�
		m_dPlcBlowerTime = 10;//atof(str1);//��ο��ð�
		m_nBlowerCount1 = 10;//atoi(str1);//��ο�����
		m_dDelayTime1 = 8;//atof(str1);///�з���½ð�
		m_dPlcNg2Time = 1.5;//atof(str1);//�ε���������
		m_dPlcOkAlignTime = 8;//atof(str1);//�̼���������
		m_nOkDelayCount = 8;//atoi(str1);//��ǰ����
		m_nLiftCount = 10;//atoi(str1);//����Ʈ����
//		AfxMessageBox(strtmp);
	}
}



void CDlgCode::UpdateWritePlc()
{
	CStdioFile   file;
	CString filename,str;
	filename.Format(_T("c:\\Glim\\InchInfo\\%s.txt"),m_strInch);
	
	if(file.Open(filename, CFile::modeCreate|CFile::modeReadWrite|CFile::typeText))
	{
		str.Format("%d",m_nPlcMovemm);//�̼۰Ÿ�
		file.WriteString(str);	
		file.WriteString(_T("\n"));
		file.WriteString(str);	
		str.Format("%d",m_nPlcMoveSpeed);//�̼ۼӵ�
		file.WriteString(str);	
		file.WriteString(_T("\n"));
		file.WriteString(str);	
		str.Format("%.1f",m_dPlcBlowerTime);//��ο��ð�
		file.WriteString(str);	
		file.WriteString(_T("\n"));
		file.WriteString(str);	
		str.Format("%d",m_nBlowerCount1);//��ο�����
		file.WriteString(str);	
		file.WriteString(_T("\n"));
		file.WriteString(str);	
		str.Format("%.1f",m_dDelayTime1);//�з���½ð�
		file.WriteString(str);	
		file.WriteString(_T("\n"));
		file.WriteString(str);	
		str.Format("%.1f",m_dPlcNg2Time);//�ε���������
		file.WriteString(str);	
		file.WriteString(_T("\n"));
		file.WriteString(str);	
		str.Format("%.1f",m_dPlcOkAlignTime);//�̼���������
		file.WriteString(str);	
		file.WriteString(_T("\n"));
		file.WriteString(str);	
		str.Format("%d",m_nOkDelayCount);//��ǰ����
		file.WriteString(str);	
		file.WriteString(_T("\n"));
		file.WriteString(str);	
		str.Format("%d",m_nLiftCount);//����Ʈ����
		file.WriteString(str);	
		file.WriteString(_T("\n"));
		file.WriteString(str);	
		file.Close();

	}
	else
	{
		//str.Format("%d",m_nPlcMovemm);//�̼۰Ÿ�
		//file.WriteString(str);	
		//file.WriteString(_T("\n"));
		//file.WriteString(str);	
		//str.Format("%d",m_nPlcMoveSpeed);//�̼ۼӵ�
		//file.WriteString(str);	
		//file.WriteString(_T("\n"));
		//file.WriteString(str);	
		//str.Format("%.1f",m_dPlcBlowerTime);//��ο��ð�
		//file.WriteString(str);	
		//file.WriteString(_T("\n"));
		//file.WriteString(str);	
		//str.Format("%d",m_nBlowerCount1);//��ο�����
		//file.WriteString(str);	
		//file.WriteString(_T("\n"));
		//file.WriteString(str);	
		//str.Format("%.1f",m_dDelayTime1);//�з���½ð�
		//file.WriteString(str);	
		//file.WriteString(_T("\n"));
		//file.WriteString(str);	
		//str.Format("%.1f",m_dPlcNg2Time);//�ε���������
		//file.WriteString(str);	
		//file.WriteString(_T("\n"));
		//file.WriteString(str);	
		//str.Format("%.1f",m_dPlcOkAlignTime);//�̼���������
		//file.WriteString(str);	
		//file.WriteString(_T("\n"));
		//file.WriteString(str);	
		//str.Format("%d",m_nOkDelayCount);//��ǰ����
		//file.WriteString(str);	
		//file.WriteString(_T("\n"));
		//file.WriteString(str);	
		//str.Format("%d",m_nLiftCount);//����Ʈ����
		//file.WriteString(str);	
		//file.WriteString(_T("\n"));
		//file.WriteString(str);	
		//file.Close();

	}
}
void CDlgCode::OnBnClickedButton6()
{
	m_strInch = "55";
	UpdatePlc();
	UpdateData(FALSE);
}


void CDlgCode::EditColtol(BOOL bshow)
{
//�Ʒ� ��ư��
	GetDlgItem(IDC_BUTTON_MODIFY)->EnableWindow(bshow);
	GetDlgItem(IDC_BUTTON4)->EnableWindow(bshow);
	GetDlgItem(IDC_BUTTON_CLOSE2)->EnableWindow(bshow);
	GetDlgItem(IDC_BUTTON_CLOSE)->EnableWindow(bshow);
}

void CDlgCode::EditColtol1(BOOL bshow)
{
	//�Ʒ� ��ư��
	GetDlgItem(IDC_BUTTON_ADJUST)->EnableWindow(bshow);
}

