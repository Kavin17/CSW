// FormTab3.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "FlyCapture2_EX.h"
#include "FlyCapture2_EXDlg.h"
#include "FormTab3.h"
#include "DlgSet.h"
#include "ini.h"


// CFormTab3 대화 상자입니다.

IMPLEMENT_DYNAMIC(CFormTab3, CDialog)

CFormTab3::CFormTab3(CWnd* pParent /*=NULL*/)
	: CDialog(CFormTab3::IDD, pParent)
	, m_bALLOK(FALSE)
	, m_bALLNG(FALSE)
	, m_nRemainHddSpace(10)
	, m_bMesUSe(FALSE)
	, m_nWinCpu(100)
	, m_nPGMCpu(100)
	, m_bAllNgHole(FALSE)
	, m_bManualRoi(FALSE)
	, m_bAllNGDual(FALSE)
	, m_nLightValueCh1(255)
	, m_nLightValueCh2(17)
	, m_dCamGain(400)
	, m_nWeldingNum(0)
	, m_nWeldingColor(0)
	, m_bWeldingLabel(FALSE)
	, m_nAngleLightValCh1(255)
	, m_nAngleLightValCh2(255)
	, m_bLiquidLensConnect(FALSE)
	, m_dLiquidLensFocusVal(50)
	, m_bCheckType(TRUE)
	, m_bFullAlphaImage(FALSE)
	, m_bFolderNGSave(FALSE)
	, m_bCkAuto(FALSE)
{
	m_hParent = pParent;
	m_bMesConnect = FALSE;
	for(int i=0; i<6; i++)
		for (int j=0; j<4; j++)
			m_nRoi[i][j]=0;
}

CFormTab3::~CFormTab3()
{
}

void CFormTab3::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK_ALL_OK, m_bALLOK);
	DDX_Check(pDX, IDC_CHECK_ALL_NG, m_bALLNG);
	DDX_Text(pDX, IDC_EDIT_REAMINHDD, m_nRemainHddSpace);
	DDX_Check(pDX, IDC_CHECK_MES, m_bMesUSe);
	DDX_Text(pDX, IDC_EDIT_WINCPU, m_nWinCpu);
	DDX_Text(pDX, IDC_EDIT_PGMCPU, m_nPGMCpu);
	DDX_Check(pDX, IDC_CHECK_USE_ROI_SET, m_bManualRoi);
	DDX_Check(pDX, IDC_CHECK_ALL_NG2, m_bAllNgHole);
	DDX_Check(pDX, IDC_CHECK_ALL_NG3, m_bAllNGDual);
	DDX_Text(pDX, IDC_EDIT_LIGHT_VALUE_CH1, m_nLightValueCh1);
	DDX_Text(pDX, IDC_EDIT_LIGHT_VALUE_CH2, m_nLightValueCh2);
	DDX_Text(pDX, IDC_EDIT_CAM_GAIN, m_dCamGain);
	DDV_MinMaxDouble(pDX, m_dCamGain, 300, 800);
	DDX_Control(pDX, IDC_COMBO_WELDING_NUM, m_comboWeldingNum);
	DDX_Control(pDX, IDC_COMBO_WELDING_NUM2, m_comboWeldingColor);
	DDX_Check(pDX, IDC_CHECK_USE_LABEL_DISPLAY, m_bWeldingLabel);
	DDX_Text(pDX, IDC_EDIT_ANGLE_LIGHT_VALUE_CH1, m_nAngleLightValCh1);
	DDX_Text(pDX, IDC_EDIT_ANGLE_LIGHT_VALUE_CH2, m_nAngleLightValCh2);
	DDX_Text(pDX, IDC_EDIT_LIQUIDLENS_FOCUS_VALUE, m_dLiquidLensFocusVal);
	DDX_Control(pDX, IDC_SPIN_FOCUS_BIG_DIGIT, m_ctSpinFocusBigDigit);
	DDX_Control(pDX, IDC_SPIN_FOCUS_SMALL_DIGIT, m_ctSpinFocusSmallDigit);
	DDX_Check(pDX, IDC_CHECK_TYPE, m_bCheckType);
	DDX_Check(pDX, IDC_CHECK5, m_bFullAlphaImage);
	DDX_Check(pDX, IDC_CHECK_NGSAVE, m_bFolderNGSave);
	DDX_Check(pDX, IDC_CHECK_AUTO, m_bCkAuto);

	DDX_Text(pDX, IDC_EDIT_ROI_LEFT_1, m_nRoi[0][0]);
	DDX_Text(pDX, IDC_EDIT_ROI_RIGHT_1, m_nRoi[0][1]);
	DDX_Text(pDX, IDC_EDIT_ROI_TOP_1, m_nRoi[0][2]);
	DDX_Text(pDX, IDC_EDIT_ROI_BOTTOM_1, m_nRoi[0][3]);

	DDX_Text(pDX, IDC_EDIT_ROI_LEFT_2, m_nRoi[1][0]);
	DDX_Text(pDX, IDC_EDIT_ROI_RIGHT_2, m_nRoi[1][1]);
	DDX_Text(pDX, IDC_EDIT_ROI_TOP_2, m_nRoi[1][2]);
	DDX_Text(pDX, IDC_EDIT_ROI_BOTTOM_2, m_nRoi[1][3]);

	DDX_Text(pDX, IDC_EDIT_ROI_LEFT_3, m_nRoi[2][0]);
	DDX_Text(pDX, IDC_EDIT_ROI_RIGHT_3, m_nRoi[2][1]);
	DDX_Text(pDX, IDC_EDIT_ROI_TOP_3, m_nRoi[2][2]);
	DDX_Text(pDX, IDC_EDIT_ROI_BOTTOM_3, m_nRoi[2][3]);

	DDX_Text(pDX, IDC_EDIT_ROI_LEFT_4, m_nRoi[3][0]);
	DDX_Text(pDX, IDC_EDIT_ROI_RIGHT_4, m_nRoi[3][1]);
	DDX_Text(pDX, IDC_EDIT_ROI_TOP_4, m_nRoi[3][2]);
	DDX_Text(pDX, IDC_EDIT_ROI_BOTTOM_4, m_nRoi[3][3]);

	DDX_Text(pDX, IDC_EDIT_ROI_LEFT_5, m_nRoi[4][0]);
	DDX_Text(pDX, IDC_EDIT_ROI_RIGHT_5, m_nRoi[4][1]);
	DDX_Text(pDX, IDC_EDIT_ROI_TOP_5, m_nRoi[4][2]);
	DDX_Text(pDX, IDC_EDIT_ROI_BOTTOM_5, m_nRoi[4][3]);

	DDX_Text(pDX, IDC_EDIT_ROI_LEFT_6, m_nRoi[5][0]);
	DDX_Text(pDX, IDC_EDIT_ROI_RIGHT_6, m_nRoi[5][1]);
	DDX_Text(pDX, IDC_EDIT_ROI_TOP_6, m_nRoi[5][2]);
	DDX_Text(pDX, IDC_EDIT_ROI_BOTTOM_6, m_nRoi[5][3]);

}


BEGIN_MESSAGE_MAP(CFormTab3, CDialog)
	ON_BN_CLICKED(IDC_BUTTONDUMMY, &CFormTab3::OnBnClickedButtondummy)
	ON_BN_CLICKED(IDC_CHECK_ALL_OK, &CFormTab3::OnBnClickedCheckAllOk)
	ON_BN_CLICKED(IDC_CHECK_ALL_NG, &CFormTab3::OnBnClickedCheckAllNg)
	ON_BN_CLICKED(IDC_CHECK_MES, &CFormTab3::OnBnClickedCheckMes)
	ON_BN_CLICKED(IDC_CHECK_ALL_NG2, &CFormTab3::OnBnClickedCheckAllNg2)
	ON_BN_CLICKED(IDC_CHECK_ALL_NG3, &CFormTab3::OnBnClickedCheckAllNg3)
	ON_BN_CLICKED(IDC_BUTTON_LIGHT_TURN_ON_CH1, &CFormTab3::OnBnClickedButtonLightTurnOnCh1)
	ON_BN_CLICKED(IDC_BUTTON_LIGHT_TURN_OFF_CH1, &CFormTab3::OnBnClickedButtonLightTurnOffCh1)
	ON_BN_CLICKED(IDC_BUTTON_LIGHT_TURN_ON_CH2, &CFormTab3::OnBnClickedButtonLightTurnOnCh2)
	ON_BN_CLICKED(IDC_BUTTON_LIGHT_TURN_OFF_CH2, &CFormTab3::OnBnClickedButtonLightTurnOffCh2)
	ON_BN_CLICKED(IDC_EDIT_SET_CAM_GAIN, &CFormTab3::OnBnClickedButtonSetGain)
	ON_BN_CLICKED(IDC_CHECK_ANGLE_LIGHT_CH1_NO1, &CFormTab3::OnBnClickedCheckAngleLightCh1No1)
	ON_BN_CLICKED(IDC_CHECK_ANGLE_LIGHT_CH1_NO2, &CFormTab3::OnBnClickedCheckAngleLightCh1No2)
	ON_BN_CLICKED(IDC_CHECK_ANGLE_LIGHT_CH1_NO3, &CFormTab3::OnBnClickedCheckAngleLightCh1No3)
	ON_BN_CLICKED(IDC_CHECK_ANGLE_LIGHT_CH1_NO4, &CFormTab3::OnBnClickedCheckAngleLightCh1No4)
	ON_BN_CLICKED(IDC_CHECK_ANGLE_LIGHT_CH1_NO5, &CFormTab3::OnBnClickedCheckAngleLightCh1No5)
	ON_BN_CLICKED(IDC_CHECK_ANGLE_LIGHT_CH1_NO6, &CFormTab3::OnBnClickedCheckAngleLightCh1No6)
	ON_BN_CLICKED(IDC_CHECK_ANGLE_LIGHT_CH1_NO7, &CFormTab3::OnBnClickedCheckAngleLightCh1No7)
	ON_BN_CLICKED(IDC_CHECK_ANGLE_LIGHT_CH1_NO8, &CFormTab3::OnBnClickedCheckAngleLightCh1No8)
	ON_BN_CLICKED(IDC_CHECK_ANGLE_LIGHT_CH1_NO9, &CFormTab3::OnBnClickedCheckAngleLightCh1No9)
	ON_BN_CLICKED(IDC_CHECK_ANGLE_LIGHT_CH1_NO10, &CFormTab3::OnBnClickedCheckAngleLightCh1No10)
	ON_BN_CLICKED(IDC_CHECK_ANGLE_LIGHT_CH2_NO1, &CFormTab3::OnBnClickedCheckAngleLightCh2No1)
	ON_BN_CLICKED(IDC_CHECK_ANGLE_LIGHT_CH2_NO2, &CFormTab3::OnBnClickedCheckAngleLightCh2No2)
	ON_BN_CLICKED(IDC_CHECK_ANGLE_LIGHT_CH2_NO3, &CFormTab3::OnBnClickedCheckAngleLightCh2No3)
	ON_BN_CLICKED(IDC_CHECK_ANGLE_LIGHT_CH2_NO4, &CFormTab3::OnBnClickedCheckAngleLightCh2No4)
	ON_BN_CLICKED(IDC_CHECK_ANGLE_LIGHT_CH2_NO5, &CFormTab3::OnBnClickedCheckAngleLightCh2No5)
	ON_BN_CLICKED(IDC_CHECK_ANGLE_LIGHT_CH2_NO6, &CFormTab3::OnBnClickedCheckAngleLightCh2No6)
	ON_BN_CLICKED(IDC_CHECK_ANGLE_LIGHT_CH2_NO7, &CFormTab3::OnBnClickedCheckAngleLightCh2No7)
	ON_BN_CLICKED(IDC_CHECK_ANGLE_LIGHT_CH2_NO8, &CFormTab3::OnBnClickedCheckAngleLightCh2No8)
	ON_BN_CLICKED(IDC_BUTTON_APPLY_LIQUIDLENS_FOCUS, &CFormTab3::OnBnClickedButtonApplyLiquidlensFocus)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_FOCUS_BIG_DIGIT, &CFormTab3::OnDeltaposSpinFocusBigDigit)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_FOCUS_SMALL_DIGIT, &CFormTab3::OnDeltaposSpinFocusSmallDigit)
	ON_BN_CLICKED(IDC_EDIT_SET_CAM_GAIN2, &CFormTab3::OnBnClickedEditSetCamGain2)
	ON_BN_CLICKED(IDC_BUTTON1, &CFormTab3::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON10, &CFormTab3::OnBnClickedButton10)
	ON_BN_CLICKED(IDC_BUTTON11, &CFormTab3::OnBnClickedButton11)
	ON_BN_CLICKED(IDC_BUTTON12, &CFormTab3::OnBnClickedButton12)
	ON_BN_CLICKED(IDC_BUTTON13, &CFormTab3::OnBnClickedButton13)
	ON_BN_CLICKED(IDC_BUTTON14, &CFormTab3::OnBnClickedButton14)
	ON_BN_CLICKED(IDC_BUTTON15, &CFormTab3::OnBnClickedButton15)
	ON_BN_CLICKED(IDC_BUTTON16, &CFormTab3::OnBnClickedButton16)
	ON_BN_CLICKED(IDC_BUTTON17, &CFormTab3::OnBnClickedButton17)
	ON_BN_CLICKED(IDC_BUTTON6, &CFormTab3::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON_LOAD, &CFormTab3::OnBnClickedButtonLoad)
	//ON_BN_CLICKED(IDC_BUTTON_TEST, &CFormTab3::OnBnClickedButtonTest)
	ON_BN_CLICKED(IDC_BUTTONTOPBOTTOM, &CFormTab3::OnBnClickedButtontopbottom)
	ON_BN_CLICKED(IDC_EDIT_SET_FULSAVE, &CFormTab3::OnBnClickedEditSetFulsave)
	ON_BN_CLICKED(IDC_BUTTON_LOAD3, &CFormTab3::OnBnClickedButtonLoad3)
	ON_BN_CLICKED(IDC_BUTTON_LEFT, &CFormTab3::OnBnClickedButtonLeft)
	ON_BN_CLICKED(IDC_BUTTON_RIGHT, &CFormTab3::OnBnClickedButtonRight)
	ON_BN_CLICKED(IDC_BUTTON18, &CFormTab3::OnBnClickedButton18)
	ON_BN_CLICKED(IDC_CHECK_ANGLE_LIGHT_CH1_NO11, &CFormTab3::OnBnClickedCheckAngleLightCh1No11)
	ON_BN_CLICKED(IDC_CHECK_ANGLE_LIGHT_CH1_NO12, &CFormTab3::OnBnClickedCheckAngleLightCh1No12)
	ON_BN_CLICKED(IDC_CHECK_ANGLE_LIGHT_CH2_NO9, &CFormTab3::OnBnClickedCheckAngleLightCh2No9)
	ON_BN_CLICKED(IDC_CHECK_ANGLE_LIGHT_CH2_NO10, &CFormTab3::OnBnClickedCheckAngleLightCh2No10)
	ON_BN_CLICKED(IDC_CHECK_ANGLE_LIGHT_CH2_NO11, &CFormTab3::OnBnClickedCheckAngleLightCh2No11)
	ON_BN_CLICKED(IDC_CHECK_ANGLE_LIGHT_CH2_NO12, &CFormTab3::OnBnClickedCheckAngleLightCh2No12)
	ON_BN_CLICKED(IDC_CHECK_TYPE, &CFormTab3::OnBnClickedCheckType)
	ON_BN_CLICKED(IDC_BUTTON_LOAD4, &CFormTab3::OnBnClickedButtonLoad4)
	ON_BN_CLICKED(IDC_CHECK5, &CFormTab3::OnBnClickedCheck5)
	ON_BN_CLICKED(IDC_CHECK_NGSAVE, &CFormTab3::OnBnClickedCheckNgsave)
	ON_BN_CLICKED(IDC_BUTTON19, &CFormTab3::OnBnClickedButton19)
	ON_WM_RBUTTONDOWN()
	ON_BN_CLICKED(IDC_CHECK_AUTO, &CFormTab3::OnBnClickedCheckAuto)
	ON_BN_CLICKED(IDC_BUTTON20, &CFormTab3::OnBnClickedButton20)
	ON_BN_CLICKED(IDC_BUTTON21, &CFormTab3::OnBnClickedButton21)
	ON_CBN_SELCHANGE(IDC_COMBO_WELDING_NUM, &CFormTab3::OnCbnSelchangeComboWeldingNum)
	ON_BN_CLICKED(IDC_BUTTON_LOAD5, &CFormTab3::OnBnClickedButtonLoad5)
	ON_BN_CLICKED(IDC_BUTTON23, &CFormTab3::OnBnClickedButton23)
//	ON_BN_CLICKED(IDC_BUTTON26, &CFormTab3::OnBnClickedButton26)
ON_BN_CLICKED(IDC_BUTTON25, &CFormTab3::OnBnClickedButton25)
END_MESSAGE_MAP()


// CFormTab3 메시지 처리기입니다.

//true 읽기 flase 쓰기
void CFormTab3::UpdateIni(BOOL bLoad)
{
	CFlyCapture2_EXDlg* pView = (CFlyCapture2_EXDlg *)m_hParent;
	if(bLoad==FALSE){
		UpdateData(TRUE);
// 		m_nWeldingNum = m_comboWeldingNum.GetCurSel();
// 		m_nWeldingColor = m_comboWeldingColor.GetCurSel();
	}

	CString strPath;
	strPath.Format("%ssetup.ini",GLIM_PATH);
	strPath.Format("c:\\Glim\\Model\\%s.ini", pView->m_pDlgSet->m_strSelectProduct);

	CIni ini1(strPath, "기타설정" , 1 );
	ini1.SER_GETD(bLoad, m_nRemainHddSpace, 10);
	ini1.SER_GETD(bLoad, m_nWinCpu, 100);
	ini1.SER_GETD(bLoad, m_nPGMCpu, 100);
	ini1.SER_GETD(bLoad, m_nLightValueCh1, 255); 
	ini1.SER_GETD(bLoad, m_nLightValueCh2, 17);
	ini1.SER_GETD(bLoad, m_dCamGain, 450);
	ini1.SER_GETD(bLoad, m_nWeldingNum, 0);
	ini1.SER_GETD(bLoad, m_nWeldingColor, 0);
	ini1.SER_GETD(bLoad, m_bWeldingLabel, 0);
	ini1.SER_GETD(bLoad, m_nAngleLightValCh1, 255);
	ini1.SER_GETD(bLoad, m_nAngleLightValCh2, 255);
	ini1.SER_GETD(bLoad, m_bCheckType, 1);
	ini1.SER_GETD(bLoad, m_bManualRoi, 0);
	ini1.SER_GETD(bLoad, m_bFullAlphaImage, 0);


// 	if(bLoad){
// 		m_comboWeldingNum.SetCurSel(m_nWeldingNum);
// 		m_comboWeldingColor.SetCurSel(m_nWeldingColor);
// 	}

	int n = CheckMes(1);
	if(n>0) 
		m_bMesUSe = TRUE;
	else 
		m_bMesUSe = FALSE;

	if(bLoad)
		UpdateData(FALSE);
}

void CFormTab3::OnBnClickedButtondummy()
{
	UpdateData();
}

void CFormTab3::OnBnClickedCheckMes()
{
	UpdateData();
}

int CFormTab3::CheckMes(int check)
{
	CString str;
	CStdioFile   file;
	CString filename;
	str = "";
	int n=0;
	filename.Format(_T("c:\\Glim\\Mes.glm"));

	if(check==1)//Read
	{
		if(file.Open(filename, CFile::modeReadWrite|CFile::typeText)) 
		{	
			file.ReadString(str);
			m_bMesUSe = atoi(str);
			file.Close();
		}
	}
	else
	{
		if(m_bMesUSe) str = "1";
		else str = "0";
		
		if(file.Open(filename, CFile::modeCreate|CFile::modeReadWrite|CFile::typeText))
		{	
			file.WriteString(str);
			file.Close();
		}
	}

	return m_bMesUSe;
}

BOOL CFormTab3::OnInitDialog()
{
	CDialog::OnInitDialog();

	CFlyCapture2_EXDlg* pView = (CFlyCapture2_EXDlg *)m_hParent;

	m_nModeChange = 1;
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CFormTab3::OnBnClickedCheckAllOk()
{//m_bALLOK
	UpdateData();
	if(m_bALLOK)
		m_bAllNgHole = m_bAllNGDual = m_bALLNG = FALSE;
	UpdateData(FALSE);
}

void CFormTab3::OnBnClickedCheckAllNg()
{//m_ballNG
	UpdateData();
	if(m_bALLNG)
		m_bAllNgHole = m_bAllNGDual = m_bALLOK = FALSE;
	UpdateData(FALSE);
}

void CFormTab3::OnBnClickedCheckAllNg2()
{//m_bAllNgHole
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData();
	if(m_bAllNgHole)
		m_bALLOK = m_bAllNGDual = m_bALLNG = FALSE;
	UpdateData(FALSE);

}

void CFormTab3::OnBnClickedCheckAllNg3()
{//m_bAllNGDual
	UpdateData();
	if(m_bAllNGDual)
		m_bAllNgHole = m_bALLOK = m_bALLNG = FALSE;
	UpdateData(FALSE);

}

void CFormTab3::OnBnClickedButtonLightTurnOnCh1()
{
	CFlyCapture2_EXDlg* pView = (CFlyCapture2_EXDlg *)m_hParent;
	int nChannel = 1;
	UpdateData();
	
	if(pView->m_nMachine==2 && (pView->m_strComname.Find("5A")>=0 || pView->m_strComname.Find("6A")>=0 || pView->m_strComname.Find("8A")>=0) )
		m_LightCtrl.LightOnUVision(m_nLightValueCh1, nChannel-1);
	else
		m_LightCtrl.LightOnVisioncowork(m_nLightValueCh1, nChannel);
}

void CFormTab3::OnBnClickedButtonLightTurnOffCh1()
{
	CFlyCapture2_EXDlg* pView = (CFlyCapture2_EXDlg *)m_hParent;
	int nChannel = 1;
	
	if(pView->m_nMachine==2 && (pView->m_strComname.Find("5A")>=0 || pView->m_strComname.Find("6A")>=0 || pView->m_strComname.Find("8A")>=0) )
		m_LightCtrl.LightOffUVision(0, nChannel-1);
	else if(pView->m_strComname.Find("7A")>=0)
		m_LightCtrl.LightOffVisioncowork(0, nChannel);
}

void CFormTab3::OnBnClickedButtonLightTurnOnCh2()
{
	CFlyCapture2_EXDlg* pView = (CFlyCapture2_EXDlg *)m_hParent;
	int nChannel = 2;
	UpdateData();

	if(pView->m_nMachine==2 && (pView->m_strComname.Find("5A")>=0 || pView->m_strComname.Find("6A")>=0 || pView->m_strComname.Find("7A")>=0 || pView->m_strComname.Find("8A")>=0) )
		m_LightCtrl.LightOnUVision(m_nLightValueCh2, nChannel-1);
	else
		m_LightCtrl.LightOnVisioncowork(m_nLightValueCh2, nChannel);
}

void CFormTab3::OnBnClickedButtonLightTurnOffCh2()
{
	CFlyCapture2_EXDlg* pView = (CFlyCapture2_EXDlg *)m_hParent;
	int nChannel = 2;

	if(pView->m_nMachine==2 && (pView->m_strComname.Find("5A")>=0 || pView->m_strComname.Find("6A")>=0 || pView->m_strComname.Find("7A")>=0 || pView->m_strComname.Find("8A")>=0) )
		m_LightCtrl.LightOffUVision(0, nChannel-1);
	else
		m_LightCtrl.LightOffVisioncowork(0, nChannel);
}

void CFormTab3::OnBnClickedButtonSetGain()
{
	UpdateData();
	CDlgSet *pDlg = (CDlgSet*)m_hParent;	
	pDlg->setGain(m_dCamGain);
}

void CFormTab3::OnBnClickedCheckAngleLightCh1No1()
{
	UpdateData();

	CString strComNum = _T("COM1");
	int nChannel = 1;

	// Checked
	if( IsDlgButtonChecked(IDC_CHECK_ANGLE_LIGHT_CH1_NO1) ){
		m_LightCtrl.LightOnVisioncoworkAngleLight(strComNum, m_nAngleLightValCh1, nChannel);
	}
	else{ // Unchecked
		m_LightCtrl.LightOffVisioncoworkAngleLight(strComNum, 0, nChannel);
	}
}

void CFormTab3::OnBnClickedCheckAngleLightCh1No2()
{
	UpdateData();

	CString strComNum = _T("COM1");
	int nChannel = 2;

	// Checked
	if( IsDlgButtonChecked(IDC_CHECK_ANGLE_LIGHT_CH1_NO2) ){
		m_LightCtrl.LightOnVisioncoworkAngleLight(strComNum, m_nAngleLightValCh1, nChannel);
	}
	else{ // Unchecked
		m_LightCtrl.LightOffVisioncoworkAngleLight(strComNum, 0, nChannel);
	}
}

void CFormTab3::OnBnClickedCheckAngleLightCh1No3()
{
	UpdateData();

	CString strComNum = _T("COM1");
	int nChannel = 3;

	// Checked
	if( IsDlgButtonChecked(IDC_CHECK_ANGLE_LIGHT_CH1_NO3) ){
		m_LightCtrl.LightOnVisioncoworkAngleLight(strComNum, m_nAngleLightValCh1, nChannel);
	}
	else{ // Unchecked
		m_LightCtrl.LightOffVisioncoworkAngleLight(strComNum, 0, nChannel);
	}
}

void CFormTab3::OnBnClickedCheckAngleLightCh1No4()
{
	UpdateData();

	CString strComNum = _T("COM1");
	int nChannel = 4;

	// Checked
	if( IsDlgButtonChecked(IDC_CHECK_ANGLE_LIGHT_CH1_NO4) ){
		m_LightCtrl.LightOnVisioncoworkAngleLight(strComNum, m_nAngleLightValCh1, nChannel);
	}
	else{ // Unchecked
		m_LightCtrl.LightOffVisioncoworkAngleLight(strComNum, 0, nChannel);
	}
}

void CFormTab3::OnBnClickedCheckAngleLightCh1No5()
{
	UpdateData();

	CString strComNum = _T("COM1");
	int nChannel = 5;

	// Checked
	if( IsDlgButtonChecked(IDC_CHECK_ANGLE_LIGHT_CH1_NO5) ){
		m_LightCtrl.LightOnVisioncoworkAngleLight(strComNum, m_nAngleLightValCh1, nChannel);
	}
	else{ // Unchecked
		m_LightCtrl.LightOffVisioncoworkAngleLight(strComNum, 0, nChannel);
	}
}

void CFormTab3::OnBnClickedCheckAngleLightCh1No6()
{
	UpdateData();

	CString strComNum = _T("COM1");
	int nChannel = 6;

	// Checked
	if( IsDlgButtonChecked(IDC_CHECK_ANGLE_LIGHT_CH1_NO6) ){
		m_LightCtrl.LightOnVisioncoworkAngleLight(strComNum, m_nAngleLightValCh1, nChannel);
	}
	else{ // Unchecked
		m_LightCtrl.LightOffVisioncoworkAngleLight(strComNum, 0, nChannel);
	}
}

void CFormTab3::OnBnClickedCheckAngleLightCh1No7()
{
	UpdateData();

	CString strComNum = _T("COM1");
	int nChannel = 7;

	// Checked
	if( IsDlgButtonChecked(IDC_CHECK_ANGLE_LIGHT_CH1_NO7) ){
		m_LightCtrl.LightOnVisioncoworkAngleLight(strComNum, m_nAngleLightValCh1, nChannel);
	}
	else{ // Unchecked
		m_LightCtrl.LightOffVisioncoworkAngleLight(strComNum, 0, nChannel);
	}
}

void CFormTab3::OnBnClickedCheckAngleLightCh1No8()
{
	UpdateData();

	CString strComNum = _T("COM1");
	int nChannel = 8;

	// Checked
	if( IsDlgButtonChecked(IDC_CHECK_ANGLE_LIGHT_CH1_NO8) ){
		m_LightCtrl.LightOnVisioncoworkAngleLight(strComNum, m_nAngleLightValCh1, nChannel);
	}
	else{ // Unchecked
		m_LightCtrl.LightOffVisioncoworkAngleLight(strComNum, 0, nChannel);
	}
}

void CFormTab3::OnBnClickedCheckAngleLightCh1No9()
{
	UpdateData();

	CString strComNum = _T("COM1");
	int nChannel = 9;

	// Checked
	if( IsDlgButtonChecked(IDC_CHECK_ANGLE_LIGHT_CH1_NO9) ){
		m_LightCtrl.LightOnVisioncoworkAngleLight(strComNum, m_nAngleLightValCh1, nChannel);
	}
	else{ // Unchecked
		m_LightCtrl.LightOffVisioncoworkAngleLight(strComNum, 0, nChannel);
	}
}

void CFormTab3::OnBnClickedCheckAngleLightCh1No10()
{
	UpdateData();

	CString strComNum = _T("COM1");
	int nChannel = 10;

	// Checked
	if( IsDlgButtonChecked(IDC_CHECK_ANGLE_LIGHT_CH1_NO10) ){
		m_LightCtrl.LightOnVisioncoworkAngleLight(strComNum, m_nAngleLightValCh1, nChannel);
	}
	else{ // Unchecked
		m_LightCtrl.LightOffVisioncoworkAngleLight(strComNum, 0, nChannel);
	}
}

void CFormTab3::OnBnClickedCheckAngleLightCh2No1()
{
	UpdateData();

	CString strComNum = _T("COM2");
	int nChannel = 1;

	// Checked
	if( IsDlgButtonChecked(IDC_CHECK_ANGLE_LIGHT_CH2_NO1) ){
		m_LightCtrl.LightOnVisioncoworkAngleLight(strComNum, m_nAngleLightValCh2, nChannel);
	}
	else{ // Unchecked
		m_LightCtrl.LightOffVisioncoworkAngleLight(strComNum, 0, nChannel);
	}
}

void CFormTab3::OnBnClickedCheckAngleLightCh2No2()
{
	UpdateData();

	CString strComNum = _T("COM2");
	int nChannel = 2;

	// Checked
	if( IsDlgButtonChecked(IDC_CHECK_ANGLE_LIGHT_CH2_NO2) ){
		m_LightCtrl.LightOnVisioncoworkAngleLight(strComNum, m_nAngleLightValCh2, nChannel);
	}
	else{ // Unchecked
		m_LightCtrl.LightOffVisioncoworkAngleLight(strComNum, 0, nChannel);
	}
}

void CFormTab3::OnBnClickedCheckAngleLightCh2No3()
{
	UpdateData();

	CString strComNum = _T("COM2");
	int nChannel = 3;

	// Checked
	if( IsDlgButtonChecked(IDC_CHECK_ANGLE_LIGHT_CH2_NO3) ){
		m_LightCtrl.LightOnVisioncoworkAngleLight(strComNum, m_nAngleLightValCh2, nChannel);
	}
	else{ // Unchecked
		m_LightCtrl.LightOffVisioncoworkAngleLight(strComNum, 0, nChannel);
	}
}	

void CFormTab3::OnBnClickedCheckAngleLightCh2No4()
{
	UpdateData();

	CString strComNum = _T("COM2");
	int nChannel = 4;

	// Checked
	if( IsDlgButtonChecked(IDC_CHECK_ANGLE_LIGHT_CH2_NO4) ){
		m_LightCtrl.LightOnVisioncoworkAngleLight(strComNum, m_nAngleLightValCh2, nChannel);
	}
	else{ // Unchecked
		m_LightCtrl.LightOffVisioncoworkAngleLight(strComNum, 0, nChannel);
	}
}

void CFormTab3::OnBnClickedCheckAngleLightCh2No5()
{
	UpdateData();

	CString strComNum = _T("COM2");
	int nChannel = 5;

	// Checked
	if( IsDlgButtonChecked(IDC_CHECK_ANGLE_LIGHT_CH2_NO5) ){
		m_LightCtrl.LightOnVisioncoworkAngleLight(strComNum, m_nAngleLightValCh2, nChannel);
	}
	else{ // Unchecked
		m_LightCtrl.LightOffVisioncoworkAngleLight(strComNum, 0, nChannel);
	}
}

void CFormTab3::OnBnClickedCheckAngleLightCh2No6()
{
	UpdateData();

	CString strComNum = _T("COM2");
	int nChannel = 6;

	// Checked
	if( IsDlgButtonChecked(IDC_CHECK_ANGLE_LIGHT_CH2_NO6) ){
		m_LightCtrl.LightOnVisioncoworkAngleLight(strComNum, m_nAngleLightValCh2, nChannel);
	}
	else{ // Unchecked
		m_LightCtrl.LightOffVisioncoworkAngleLight(strComNum, 0, nChannel);
	}
}

void CFormTab3::OnBnClickedCheckAngleLightCh2No7()
{
	UpdateData();

	CString strComNum = _T("COM2");
	int nChannel = 7;

	// Checked
	if( IsDlgButtonChecked(IDC_CHECK_ANGLE_LIGHT_CH2_NO7) ){
		m_LightCtrl.LightOnVisioncoworkAngleLight(strComNum, m_nAngleLightValCh2, nChannel);
	}
	else{ // Unchecked
		m_LightCtrl.LightOffVisioncoworkAngleLight(strComNum, 0, nChannel);
	}
}

void CFormTab3::OnBnClickedCheckAngleLightCh2No8()
{
	UpdateData();

	CString strComNum = _T("COM2");
	int nChannel = 8;

	// Checked
	if( IsDlgButtonChecked(IDC_CHECK_ANGLE_LIGHT_CH2_NO8) ){
		m_LightCtrl.LightOnVisioncoworkAngleLight(strComNum, m_nAngleLightValCh2, nChannel);
	}
	else{ // Unchecked
		m_LightCtrl.LightOffVisioncoworkAngleLight(strComNum, 0, nChannel);
	}
}

void CFormTab3::OnBnClickedButtonApplyLiquidlensFocus()
{
	double dTemp = m_dLiquidLensFocusVal;

	UpdateData();

	if(m_dLiquidLensFocusVal < 24 || m_dLiquidLensFocusVal > 70){
		m_dLiquidLensFocusVal = dTemp;
		UpdateData(FALSE);
		return;
	}

	if(LiquidLensOpen(_T("COM4"))){
		int nValue = (m_dLiquidLensFocusVal - 24) / 0.001;
		CString strValue;
		strValue.Format(_T("%04X"), nValue);

		int nFocusLSB = _tcstol(strValue.Right(2), NULL, 16);
		int nFocusMSB = _tcstol(strValue.Left(2), NULL, 16);

		int nSumLSB = 0x02 + 0x37 + 0x00 + 0x01 + nFocusLSB;
		int nSumMSB = 0x02 + 0x37 + 0x01 + 0x01 + nFocusMSB;

		unsigned char strLSB[6] = {0x02, 0x37, 0x00, 0x01, 0x00, 0x3A}; // Init. 00
		strLSB[4] = nFocusLSB;
		strLSB[5] = nSumLSB;

		unsigned char strMSB[6] = {0x02, 0x37, 0x01, 0x01, 0x00, 0x3B}; // Init. 00
		strMSB[4] = nFocusMSB;
		strMSB[5] = nSumMSB;

		unsigned char strSave[6] = {0x02, 0x37, 0x02, 0x01, 0xFF, 0x3B}; // Init. Save

		DWORD dwWriteBytes = NULL;
		if(m_bLiquidLensConnect){
			// Set Focus Value
			WriteFile(m_LiquidLensComHandle, strLSB, 6, &dwWriteBytes, NULL);

			Sleep(10);

			WriteFile(m_LiquidLensComHandle, strMSB, 6, &dwWriteBytes, NULL);

			Sleep(10);

			// Save it to EEPROM
			WriteFile(m_LiquidLensComHandle, strSave, 6, &dwWriteBytes, NULL);
		}
	}
	else{
		dTemp = dTemp;
	}
}

BOOL CFormTab3::LiquidLensOpen(CString strPort)
{
	if(m_bLiquidLensConnect){
		LiquidLensClose();
	}

	m_LiquidLensComHandle = CreateFile(strPort, GENERIC_READ|GENERIC_WRITE, 
		NULL, NULL, OPEN_EXISTING,	FILE_ATTRIBUTE_NORMAL, NULL);
	if(m_LiquidLensComHandle == INVALID_HANDLE_VALUE) 
		return FALSE;

	DCB	dcb;
	GetCommState(m_LiquidLensComHandle, &dcb);

	dcb.BaudRate = 57600;
	dcb.ByteSize = 8;
	dcb.Parity = NOPARITY;
	dcb.StopBits = ONESTOPBIT;
	dcb.fNull = FALSE;
	dcb.fBinary = TRUE; // Binary Mode

	SetCommState(m_LiquidLensComHandle, &dcb);

	DWORD evtMask = EV_RXCHAR|EV_TXEMPTY;
	SetCommMask(m_LiquidLensComHandle, evtMask);

	m_bLiquidLensConnect = TRUE;

	return TRUE;
}
void CFormTab3::OnDeltaposSpinFocusBigDigit(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	if(m_dLiquidLensFocusVal < 24 || m_dLiquidLensFocusVal > 70)
		return;

	double dTemp = m_dLiquidLensFocusVal;

	if(pNMUpDown->iDelta > 0) // Up
		dTemp++;
	else // Down
		dTemp--;

	if(dTemp < 24 || dTemp > 70)
		return;

	m_dLiquidLensFocusVal = dTemp;

	UpdateData(FALSE);

	*pResult = 0;
}

void CFormTab3::OnDeltaposSpinFocusSmallDigit(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	if(m_dLiquidLensFocusVal < 24 || m_dLiquidLensFocusVal > 70)
		return;

	double dTemp = m_dLiquidLensFocusVal;

	if(pNMUpDown->iDelta > 0) // Up
		dTemp = dTemp + 0.01;
	else // Down
		dTemp = dTemp - 0.01;

	if(dTemp < 24 || dTemp > 70)
		return;

	m_dLiquidLensFocusVal = dTemp;

	UpdateData(FALSE);

	*pResult = 0;
	
}

void CFormTab3::LiquidLensClose()
{
	m_bLiquidLensConnect = FALSE; 
	CloseHandle(m_LiquidLensComHandle);
}
void CFormTab3::OnBnClickedEditSetCamGain2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CFlyCapture2_EXDlg* pView = (CFlyCapture2_EXDlg *)m_hParent;

	CString str;
	CTime time;
	CTime ttime = CTime::GetCurrentTime();
	CString kk = ttime.Format("%Y%m%d");
	CString kk2 = ttime.Format("%Y%m");
	CString kk3 = ttime.Format("%d");
	CString kk1 = ttime.Format("[%Y%m%d_%H%M%S]");

	str.Format("%s.jpg", kk1);

	pView->m_Display[0].SaveTestImg(str, pView->ComputerUserName());
}

void CFormTab3::OnBnClickedButton1()
{
	CFlyCapture2_EXDlg* pView = (CFlyCapture2_EXDlg *)m_hParent;
//3장 Grab
	//조명 채널 변경
	int nport= 0;
	int nSleep = 50;
	if(pView->m_bCamStats[0])
	{
		pView->m_Vision[0]->Grab();//startgrabbing
	}

			fnSetLight(nport,1, TRUE);
		pView->Wait(nSleep);
		pView->m_Vision[0]->SoftTriggerSend();
		pView->Wait(nSleep);
		fnSetLight(nport,1, FALSE);
		pView->Wait(nSleep);
		fnSetLight(nport,2, TRUE);
		pView->Wait(nSleep);
		pView->m_Vision[0]->SoftTriggerSend();
		pView->Wait(nSleep);
		fnSetLight(nport,2, FALSE);
		pView->Wait(nSleep);
		fnSetLight(nport,3, TRUE);
		pView->Wait(nSleep);
		pView->m_Vision[0]->SoftTriggerSend();
		pView->Wait(nSleep);
		fnSetLight(nport,3, FALSE);
		pView->Wait(nSleep);
		for(int i=1;i<4;i++)
			fnSetLight(nport,i, TRUE);
		pView->Wait(nSleep);
		pView->m_Vision[0]->SoftTriggerSend();
		pView->Wait(nSleep);
		for(int i=1;i<4;i++)
			fnSetLight(nport,i, FALSE);


//	pView->m_Vision[0]->SoftTriggerSend();
	pView->AddtoInspLog("Top1");

	if(pView->m_bCamStats[0])
	{
		pView->m_Vision[0]->Stop();//startgrabbing
	}
	
}


void CFormTab3::fnSetLight(int nPort,int nCh, BOOL nOnOff)
{
	UpdateData();


	CString strComNum = _T("COM1");
	if(nPort==1)
		strComNum = _T("COM2");
	int nChannel = nCh;
//

	// Checked
	if( nOnOff){
		m_LightCtrl.LightOnVisioncoworkAngleLight(strComNum, m_nAngleLightValCh2, nChannel);
	}
	else{ // Unchecked
		m_LightCtrl.LightOffVisioncoworkAngleLight(strComNum, 0, nChannel);
	}
}
void CFormTab3::OnBnClickedButton10()
{
	CFlyCapture2_EXDlg* pView = (CFlyCapture2_EXDlg *)m_hParent;
	int nport= 1;
	int nSleep = 50;
	if(pView->m_bCamStats[0])
	{
		pView->m_Vision[0]->Grab();//startgrabbing
	}

	fnSetLight(nport,1, TRUE);
	pView->Wait(nSleep);
		pView->m_Vision[0]->SoftTriggerSend();
		pView->Wait(nSleep);
	fnSetLight(nport,1, FALSE);

	pView->Wait(nSleep);
	fnSetLight(nport,2, TRUE);
	pView->Wait(nSleep);
		pView->m_Vision[0]->SoftTriggerSend();
		pView->Wait(nSleep);
	fnSetLight(nport,2, FALSE);

	pView->Wait(nSleep);
	fnSetLight(nport,3, TRUE);
	pView->Wait(nSleep);
		pView->m_Vision[0]->SoftTriggerSend();
		pView->Wait(nSleep);
	fnSetLight(nport,3, FALSE);

	pView->Wait(nSleep);
	for(int i=1;i<4;i++)
		fnSetLight(nport,i, TRUE);
	pView->Wait(nSleep);
		pView->m_Vision[0]->SoftTriggerSend();
		pView->Wait(nSleep);
	for(int i=1;i<4;i++)
		fnSetLight(nport,i, FALSE);
	if(pView->m_bCamStats[0])
	{
		pView->m_Vision[0]->Stop();//startgrabbing
	}

}

void CFormTab3::OnBnClickedButton11()
{
	CFlyCapture2_EXDlg* pView = (CFlyCapture2_EXDlg *)m_hParent;
	int nport= 1;
	int nSleep = 50;
	if(pView->m_bCamStats[0])
	{
		pView->m_Vision[0]->Grab();//startgrabbing
	}
	if(m_nModeChange==0){
		fnSetLight(nport,4, TRUE);
		pView->Wait(nSleep);
		pView->m_Vision[0]->SoftTriggerSend();
		pView->Wait(nSleep);
		fnSetLight(nport,4, FALSE);

		pView->Wait(nSleep);
		fnSetLight(nport,5, TRUE);
		pView->Wait(nSleep);
		pView->m_Vision[0]->SoftTriggerSend();
		pView->Wait(nSleep);
		fnSetLight(nport,5, FALSE);

		pView->Wait(nSleep);
		fnSetLight(nport,6, TRUE);
		pView->Wait(nSleep);
		pView->m_Vision[0]->SoftTriggerSend();
		pView->Wait(nSleep);
		fnSetLight(nport,6, FALSE);

		pView->Wait(nSleep);
		fnSetLight(nport,7, TRUE);
		pView->Wait(nSleep);
		pView->m_Vision[0]->SoftTriggerSend();
		pView->Wait(nSleep);
		fnSetLight(nport,7, FALSE);

		pView->Wait(nSleep);
		fnSetLight(nport,8, TRUE);
		pView->Wait(nSleep);
		pView->m_Vision[0]->SoftTriggerSend();
		pView->Wait(nSleep);
		fnSetLight(nport,8, FALSE);
		for(int i=4;i<9;i++)
			fnSetLight(nport,i, TRUE);
		pView->Wait(nSleep);
		pView->m_Vision[0]->SoftTriggerSend();
		pView->Wait(nSleep);
		for(int i=4;i<9;i++)
			fnSetLight(nport,i, FALSE);
	}
	else if(m_nModeChange==1){
		fnSetLight(nport,4, TRUE);
		fnSetLight(nport,5, TRUE);
		pView->Wait(nSleep);
		pView->m_Vision[0]->SoftTriggerSend();
		pView->Wait(nSleep);
		fnSetLight(nport,4, FALSE);
		fnSetLight(nport,5, FALSE);

		pView->Wait(nSleep);

		fnSetLight(nport,6, TRUE);
		fnSetLight(nport,7, TRUE);
		fnSetLight(nport,8, TRUE);
		pView->Wait(nSleep);
		pView->m_Vision[0]->SoftTriggerSend();
		pView->Wait(nSleep);

		for(int i=4;i<9;i++)
			fnSetLight(nport,i, TRUE);
		pView->Wait(nSleep);
		pView->m_Vision[0]->SoftTriggerSend();
		pView->Wait(nSleep);
		for(int i=4;i<9;i++)
			fnSetLight(nport,i, FALSE);
	}

	if(pView->m_bCamStats[0])
	{
		pView->m_Vision[0]->Stop();//startgrabbing
	}

}

void CFormTab3::OnBnClickedButton12()
{
	CFlyCapture2_EXDlg* pView = (CFlyCapture2_EXDlg *)m_hParent;
	pView->m_bSaveImg1 = FALSE;
	CString str1;
	str1.Format("%d_NG_%s",pView->m_nOKCount+pView->m_nNgCount,pView->m_strBadInfo1[1]);

	if(1.0 < pView->m_dRemainHDDSpace2)
		pView->m_Display[0].SaveImgNG1(str1,0,pView->m_pDlgSet->m_FormTab2->m_bCsvSave, pView->m_pDlgSet->m_FormTab2->m_bImgReverse);

	return;
	int nport= 0;
	int nSleep = 50;
	if(pView->m_bCamStats[0])
	{
		pView->m_Vision[0]->Grab();//startgrabbing
	}
	if(m_nModeChange==0){
		fnSetLight(nport,4, TRUE);
		pView->Wait(nSleep);
		pView->m_Vision[0]->SoftTriggerSend();
		pView->Wait(nSleep);
		fnSetLight(nport,4, FALSE);

		pView->Wait(nSleep);
		fnSetLight(nport,5, TRUE);
		pView->Wait(nSleep);
		pView->m_Vision[0]->SoftTriggerSend();
		pView->Wait(nSleep);
		fnSetLight(nport,5, FALSE);

		pView->Wait(nSleep);
		fnSetLight(nport,6, TRUE);
		pView->Wait(nSleep);
		pView->m_Vision[0]->SoftTriggerSend();
		pView->Wait(nSleep);
		fnSetLight(nport,6, FALSE);

		pView->Wait(nSleep);
		fnSetLight(nport,7, TRUE);
		pView->Wait(nSleep);
		pView->m_Vision[0]->SoftTriggerSend();
		pView->Wait(nSleep);
		fnSetLight(nport,7, FALSE);

		pView->Wait(nSleep);
		fnSetLight(nport,8, TRUE);
		pView->Wait(nSleep);
		pView->m_Vision[0]->SoftTriggerSend();
		pView->Wait(nSleep);
		fnSetLight(nport,8, FALSE);
		for(int i=4;i<9;i++)
			fnSetLight(nport,i, TRUE);
		pView->Wait(nSleep);
		pView->m_Vision[0]->SoftTriggerSend();
		pView->Wait(nSleep);
		for(int i=4;i<9;i++)
			fnSetLight(nport,i, FALSE);
	}
	else if(m_nModeChange==1){
		fnSetLight(nport,4, TRUE);
		fnSetLight(nport,5, TRUE);
		pView->Wait(nSleep);
		pView->m_Vision[0]->SoftTriggerSend();
		pView->Wait(nSleep);
		fnSetLight(nport,4, FALSE);
		fnSetLight(nport,5, FALSE);

		pView->Wait(nSleep);
		fnSetLight(nport,6, TRUE);
		fnSetLight(nport,7, TRUE);
		fnSetLight(nport,8, TRUE);
		pView->Wait(nSleep);
		pView->m_Vision[0]->SoftTriggerSend();
		pView->Wait(nSleep);
		for(int i=4;i<9;i++)
			fnSetLight(nport,i, TRUE);
		pView->Wait(nSleep);
		pView->m_Vision[0]->SoftTriggerSend();
		pView->Wait(nSleep);
		for(int i=4;i<9;i++)
			fnSetLight(nport,i, FALSE);
	}

	if(pView->m_bCamStats[0])
	{
		pView->m_Vision[0]->Stop();//startgrabbing
	}

}


void CFormTab3::OnBnClickedButton13()
{
	//top
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CFlyCapture2_EXDlg* pView = (CFlyCapture2_EXDlg *)m_hParent;
	//상하에 대한 에지를 찾는다.


}

void CFormTab3::OnBnClickedButton14()
{
	//left
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CFlyCapture2_EXDlg* pView = (CFlyCapture2_EXDlg *)m_hParent;
	//상하에 대한 에지를 찾는다.

	//탑캡 중심을 찾고 일정 영역을 설정하낟.

	//	int ntap =pView->m_Project.CircleFind1();//Tracker search
	BYTE * fmtmp = new BYTE[CAMERA_WIDTH*CAMERA_HEIGHT];
	memset(fmtmp,0,CAMERA_HEIGHT*CAMERA_WIDTH);

	//0-2번영상 
// 	for(int k=3;k<5;k++){
// 		for(int y=0;y<CAMERA_HEIGHT-4;y++){
// 			for(int x=0;x<CAMERA_WIDTH-4;x++){
// 				*(fmtmp+y*CAMERA_WIDTH+x) += abs(*(pView->m_Project.m_MultiBuf[k]+y*CAMERA_WIDTH+x) - *(pView->m_Project.m_MultiBuf[k]+(y)*CAMERA_WIDTH+x+4)) ;
// 				if(*(fmtmp+y*CAMERA_WIDTH+x)>250)*(fmtmp+y*CAMERA_WIDTH+x) = 250;
// 			}
// 		}
// 	}
	for(int k=3;k<5;k++){
		for(int y=0;y<CAMERA_HEIGHT-4;y++){
			for(int x=0; x< CAMERA_WIDTH-4;x++){
				if(abs(*(pView->m_Project.m_MultiBuf[k]+y*CAMERA_WIDTH+x) - *(pView->m_Project.m_MultiBuf[k]+(y)*CAMERA_WIDTH+x+4)) <0)
					*(fmtmp+y*CAMERA_WIDTH+x) += 0;
				else
					*(fmtmp+y*CAMERA_WIDTH+x) += abs(*(pView->m_Project.m_MultiBuf[k]+y*CAMERA_WIDTH+x) - *(pView->m_Project.m_MultiBuf[k]+(y)*CAMERA_WIDTH+x+4))*2 ;
				if(*(fmtmp+y*CAMERA_WIDTH+x)>250)*(fmtmp+y*CAMERA_WIDTH+x) = 250;
			}
		}
	}
	memcpy(pView->m_Display[0].m_pImg->GetImagePtr(),fmtmp,CAMERA_WIDTH*CAMERA_HEIGHT);
	memcpy(pView->m_Project.m_AlphaBuf[1],fmtmp,CAMERA_WIDTH*CAMERA_HEIGHT);
	
	//	memcpy(m_Display[TargetCam].m_pImg->GetImagePtr(),m_Project.m_fm[TargetCam],CAMERA_WIDTH*CAMERA_HEIGHT);
	memcpy(pView->m_DisplaySave[0].m_pImg->GetImagePtr(),pView->m_Display[0].m_pImg->GetImagePtr(),CAMERA_WIDTH*CAMERA_HEIGHT);

	CRect rttab1;
	int nmargin = 40;

	
	rttab1.SetRect(pView->m_Project.m_ptTopEdge[0].x-nmargin,pView->m_Project.m_ptTopEdge[0].y+20,pView->m_Project.m_ptTopEdge[0].x+nmargin,pView->m_Project.m_ptTopEdge[0].y+90);
	rttab1 = pView->m_Project.CheckROI(rttab1,CRect(10,10,CAMERA_WIDTH-10,CAMERA_HEIGHT-10));
	pView->m_Display[0].AddArrayRect(rttab1,0);
	FindLeft(pView->m_Project.m_AlphaBuf[1] , rttab1);
	pView->DrawDisplay(0);

	delete fmtmp;
}

void CFormTab3::OnBnClickedButton15()
{
	//right
	CFlyCapture2_EXDlg* pView = (CFlyCapture2_EXDlg *)m_hParent;
	//상하에 대한 에지를 찾는다.
	//탑캡 중심을 찾고 일정 영역을 설정하낟.

	//	int ntap =pView->m_Project.CircleFind1();//Tracker search
	BYTE * fmtmp = new BYTE[CAMERA_WIDTH*CAMERA_HEIGHT];
	memset(fmtmp,0,CAMERA_HEIGHT*CAMERA_WIDTH);

	//0-2번영상 
// 	for(int k=0;k<6;k++){
// 		for(int y=0;y<CAMERA_HEIGHT-4;y++){
// 			for(int x=0;x<CAMERA_WIDTH-4;x++){
// 				*(fmtmp+y*CAMERA_WIDTH+x) += abs(*(pView->m_Project.m_MultiBuf[k]+y*CAMERA_WIDTH+x) - *(pView->m_Project.m_MultiBuf[k]+(y)*CAMERA_WIDTH+x+4)) ;
// 				if(*(fmtmp+y*CAMERA_WIDTH+x)>250)*(fmtmp+y*CAMERA_WIDTH+x) = 250;
// 			}
// 		}
// 	}
	for(int k=6;k<8;k++){
		for(int y=0;y<CAMERA_HEIGHT-4;y++){
			for(int x=0; x< CAMERA_WIDTH-4;x++){
				if(abs(*(pView->m_Project.m_MultiBuf[k]+y*CAMERA_WIDTH+x) - *(pView->m_Project.m_MultiBuf[k]+(y)*CAMERA_WIDTH+x+4)) <0)
					*(fmtmp+y*CAMERA_WIDTH+x) += 0;
				else
					*(fmtmp+y*CAMERA_WIDTH+x) += abs(*(pView->m_Project.m_MultiBuf[k]+y*CAMERA_WIDTH+x) - *(pView->m_Project.m_MultiBuf[k]+(y)*CAMERA_WIDTH+x+4))*1 ;
				if(*(fmtmp+y*CAMERA_WIDTH+x)>250)*(fmtmp+y*CAMERA_WIDTH+x) = 250;
			}
		}
	}
	memcpy(pView->m_Display[0].m_pImg->GetImagePtr(),fmtmp,CAMERA_WIDTH*CAMERA_HEIGHT);
	//	memcpy(m_Display[TargetCam].m_pImg->GetImagePtr(),m_Project.m_fm[TargetCam],CAMERA_WIDTH*CAMERA_HEIGHT);
	memcpy(pView->m_DisplaySave[0].m_pImg->GetImagePtr(),pView->m_Display[0].m_pImg->GetImagePtr(),CAMERA_WIDTH*CAMERA_HEIGHT);
	memcpy(pView->m_Project.m_AlphaBuf[2],fmtmp,CAMERA_WIDTH*CAMERA_HEIGHT);

	CRect rttab1;
	int nmargin = 30;

	rttab1.SetRect(pView->m_Project.m_ptTopEdge[1].x-nmargin,pView->m_Project.m_ptTopEdge[1].y,pView->m_Project.m_ptTopEdge[1].x+nmargin,pView->m_Project.m_ptTopEdge[1].y+70);
	rttab1 = pView->m_Project.CheckROI(rttab1,CRect(10,10,CAMERA_WIDTH-10,CAMERA_HEIGHT-10));

	FindRight(pView->m_Project.m_AlphaBuf[2] , rttab1);
	pView->m_Display[0].AddArrayRect(rttab1,0);
	pView->DrawDisplay(0);


	delete fmtmp;
}

void CFormTab3::OnBnClickedButton16()
{
	//Image summary
	CFlyCapture2_EXDlg* pView = (CFlyCapture2_EXDlg *)m_hParent;
	BYTE * fmtmp = new BYTE[CAMERA_WIDTH*CAMERA_HEIGHT];
	memset(fmtmp,0,CAMERA_HEIGHT*CAMERA_WIDTH);

	int nmode = 1;
	if(nmode==0){
		for(int k=0;k<3;k++){
			for(int y=0;y<CAMERA_HEIGHT-4;y++){
				for(int x=0;x<CAMERA_WIDTH-4;x++){
					*(fmtmp+y*CAMERA_WIDTH+x) += *(pView->m_Project.m_AlphaBuf[k]+y*CAMERA_WIDTH+x) ;
									if(*(fmtmp+y*CAMERA_WIDTH+x)>250)*(fmtmp+y*CAMERA_WIDTH+x) = 250;
							}
					}
			}
	}
	else{
	 	for(int y=0;y<CAMERA_HEIGHT-4;y++){
	 		for(int x=0;x<CAMERA_WIDTH-4;x++){
				if(*(pView->m_Project.m_AlphaBuf[0]+y*CAMERA_WIDTH+x) > *(pView->m_Project.m_AlphaBuf[1]+y*CAMERA_WIDTH+x)){
					if(*(pView->m_Project.m_AlphaBuf[0]+y*CAMERA_WIDTH+x) > *(pView->m_Project.m_AlphaBuf[2]+y*CAMERA_WIDTH+x)){
						*(fmtmp+y*CAMERA_WIDTH+x) = *(pView->m_Project.m_AlphaBuf[0]+y*CAMERA_WIDTH+x);
					}
					else
						*(fmtmp+y*CAMERA_WIDTH+x) = *(pView->m_Project.m_AlphaBuf[2]+y*CAMERA_WIDTH+x);
				}
				else{
					if(*(pView->m_Project.m_AlphaBuf[1]+y*CAMERA_WIDTH+x) > *(pView->m_Project.m_AlphaBuf[2]+y*CAMERA_WIDTH+x)){
						*(fmtmp+y*CAMERA_WIDTH+x) = *(pView->m_Project.m_AlphaBuf[1]+y*CAMERA_WIDTH+x);
					}
					else
						*(fmtmp+y*CAMERA_WIDTH+x) = *(pView->m_Project.m_AlphaBuf[2]+y*CAMERA_WIDTH+x);
				}
	 		}
	 	}
	}


	memcpy(pView->m_Display[0].m_pImg->GetImagePtr(),fmtmp,CAMERA_WIDTH*CAMERA_HEIGHT);
	//	memcpy(m_Display[TargetCam].m_pImg->GetImagePtr(),m_Project.m_fm[TargetCam],CAMERA_WIDTH*CAMERA_HEIGHT);
	memcpy(pView->m_DisplaySave[0].m_pImg->GetImagePtr(),pView->m_Display[0].m_pImg->GetImagePtr(),CAMERA_WIDTH*CAMERA_HEIGHT);
//	pView->DrawDisplay(0);

	pView->AddtoInspLog("Sum");
	delete fmtmp;

}

void CFormTab3::OnBnClickedButton17()
{
//Full Grab
	CFlyCapture2_EXDlg* pView = (CFlyCapture2_EXDlg *)m_hParent;


	pView->ResetData();
	return;

	pView->m_Display[0].ResetArray();
	pView->m_Project.m_nMultiFrame = 0;
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	OnBnClickedButton1();//top Grab
	pView->AddtoInspLog("Top");
	pView->Wait(50);

	OnBnClickedButton11();//6
	pView->AddtoInspLog("Left");
	pView->Wait(50);
	OnBnClickedButton12();
	pView->AddtoInspLog("Right");
	pView->Wait(50);
	OnBnClickedButtontopbottom();//View용 Top Bottom
	pView->Wait(50);
// 	fnYSum(0,4,0);//top
// 	fnXSum(4,10,1);//left
// 	fnXSum(10,16,2);//left
// 	OnBnClickedButton16();
	OnBnClickedButtonTest();

	OnBnClickedEditSetFulsave();

}

void CFormTab3::fnYSum(int nstart,int nend,int nFrame)
{//0
	CFlyCapture2_EXDlg* pView = (CFlyCapture2_EXDlg *)m_hParent;
	//상하에 대한 에지를 찾는다.

	//탑캡 중심을 찾고 일정 영역을 설정하낟.

	//	int ntap =pView->m_Project.CircleFind1();//Tracker search
	BYTE * fmtmp = new BYTE[CAMERA_WIDTH*CAMERA_HEIGHT];
	memset(fmtmp,0,CAMERA_HEIGHT*CAMERA_WIDTH);

	//0-2번영상 
	for(int k=nstart;k<nend;k++){
		for(int y=0;y<CAMERA_HEIGHT-4;y++){
			for(int x=0;x<CAMERA_WIDTH-4;x++){
				*(fmtmp+y*CAMERA_WIDTH+x) += abs(*(pView->m_Project.m_MultiBuf[k]+y*CAMERA_WIDTH+x) - *(pView->m_Project.m_MultiBuf[k]+(y+4)*CAMERA_WIDTH+x)) ;
				if(*(fmtmp+y*CAMERA_WIDTH+x)>250)*(fmtmp+y*CAMERA_WIDTH+x) = 250;
			}
		}
	}
	memcpy(pView->m_Display[0].m_pImg->GetImagePtr(),fmtmp,CAMERA_WIDTH*CAMERA_HEIGHT);
	//	memcpy(m_Display[TargetCam].m_pImg->GetImagePtr(),m_Project.m_fm[TargetCam],CAMERA_WIDTH*CAMERA_HEIGHT);
	memcpy(pView->m_DisplaySave[0].m_pImg->GetImagePtr(),pView->m_Display[0].m_pImg->GetImagePtr(),CAMERA_WIDTH*CAMERA_HEIGHT);
	memcpy(pView->m_Project.m_AlphaBuf[nFrame],fmtmp,CAMERA_WIDTH*CAMERA_HEIGHT);

//	pView->DrawDisplay(0);

	delete fmtmp;
}

void CFormTab3::fnYSum1(int nstart,int nend,int nFrame,CRect rt,BOOL bShow,BOOL bSave,double dAmplify,int nInsp)
{//0
	CFlyCapture2_EXDlg* pView = (CFlyCapture2_EXDlg *)m_hParent;
	//상하에 대한 에지를 찾는다.
	BYTE * fmtmp = new BYTE[CAMERA_WIDTH*CAMERA_HEIGHT];
	memset(fmtmp,0,CAMERA_HEIGHT*CAMERA_WIDTH);
	//0-2번영상 
	int ntmp1 = 0;
	if(dAmplify<2){
		for(int k=nstart;k<=nend;k++){
			for(int y=rt.top;y<rt.bottom;y++){
				for(int x=rt.left;x<rt.right;x++){
					*(fmtmp+y*CAMERA_WIDTH+x) += abs(*(pView->m_Project.m_MultiBuf[k]+y*CAMERA_WIDTH+x) - *(pView->m_Project.m_MultiBuf[k]+(y+4)*CAMERA_WIDTH+x)) ;
					if(*(fmtmp+y*CAMERA_WIDTH+x)>250)*(fmtmp+y*CAMERA_WIDTH+x) = 250;
				}
			}
		}
	}
	else{
		for(int k=nstart;k<=nend;k++){
			for(int y=rt.top; y<rt.bottom;y++){
				for(int x=rt.left; x<rt.right;x++){
					if(abs(*(pView->m_Project.m_MultiBuf[k]+y*CAMERA_WIDTH+x) - *(pView->m_Project.m_MultiBuf[k]+(y+4)*CAMERA_WIDTH+x)) < 0 )
						*(fmtmp+y*CAMERA_WIDTH+x) += 0;
					else
						*(fmtmp+y*CAMERA_WIDTH+x) += abs(*(pView->m_Project.m_MultiBuf[k]+y*CAMERA_WIDTH+x) - *(pView->m_Project.m_MultiBuf[k]+(y+4)*CAMERA_WIDTH+x))*dAmplify ;
					ntmp1 = *(fmtmp+y*CAMERA_WIDTH+x);
					if(*(fmtmp+y*CAMERA_WIDTH+x)>250)
						*(fmtmp+y*CAMERA_WIDTH+x) = 250;
				}
			}
		}
	}
	if(bShow)memcpy(pView->m_Display[0].m_pImg->GetImagePtr(),fmtmp,CAMERA_WIDTH*CAMERA_HEIGHT);
//	memcpy(pView->m_DisplaySave[0].m_pImg->GetImagePtr(),pView->m_Display[0].m_pImg->GetImagePtr(),CAMERA_WIDTH*CAMERA_HEIGHT);
	memcpy(pView->m_Project.m_AlphaBuf[0],fmtmp,CAMERA_WIDTH*CAMERA_HEIGHT);

	int nSelectcam = pView->m_nSelectFrame;
	int nh1 = pView->m_Project.m_rtWeldPos.Height();
	int nw1 = pView->m_Project.m_rtWeldPos.Width();
	int x2=0,y2=pView->m_Project.m_rtWeldPos.Height()+10;
	int x3 = pView->m_Project.m_rtWeldPos.left,y3=pView->m_Project.m_rtWeldPos.top;
	//여러장을 도니 다시 복사한다.
	/*if(nSelectcam<=10){
		for(int i=y2;i<y2+nh1;i++){
			x3 = pView->m_Project.m_rtWeldPos.left;
			if(y3>CAMERA_HEIGHT-10|| y3<0)break;
			for(int j=x2;j<x2+nw1;j++){
				if(x3>CAMERA_WIDTH-10 || x3<0)break;
				*(pView->m_Project.m_MultiBuf[nSelectcam]+i*CAMERA_WIDTH+j) = *(fmtmp+(y3)*CAMERA_WIDTH+x3);
				x3++;
			}
			y3++;
		}
	}*/
	
	//fmnew
	WeldPoint(fmtmp,CAMERA_WIDTH,CAMERA_HEIGHT,pView->m_Project.m_rtWeldPos,1);



	CRect rttab;
	int noffset1 = pView->m_pDlgSet->m_FormTab2->m_nTabTopOffset;
	
//	rttab.SetRect(pView->m_Project.m_ptCapCenter.x-20,pView->m_Project.m_ptCapCenter.y-noffset1,pView->m_Project.m_ptCapCenter.x+20,pView->m_Project.m_ptCapCenter.y+noffset1);
	//topcap 센터에 원의 밝기를 구해서 영역 재설정
	CRect rtcapcenter;
	int ngap=20,nbcnt = 0;double dtmp11 = 0;
	int TabSizeHalf;
	TabSizeHalf=pView->m_pDlgSet->m_FormTab4->m_dTapSize/pView->m_pDlgSet->m_FormTab2->m_dCamScale/3;
	if(pView->m_Project.m_ptCapCenter.x <= ngap ) pView->m_Project.m_ptCapCenter.x = ngap;
	if(pView->m_Project.m_ptCapCenter.y <= ngap ) pView->m_Project.m_ptCapCenter.y = ngap;
	rtcapcenter.SetRect(pView->m_Project.m_ptCapCenter.x-ngap,pView->m_Project.m_ptCapCenter.y-ngap,pView->m_Project.m_ptCapCenter.x+ngap,pView->m_Project.m_ptCapCenter.y+ngap);
	
	pView->m_nTopCapCenterBrightness = pView->m_Project.AreaAverage2(2,fmtmp,CAMERA_WIDTH,rtcapcenter.left,rtcapcenter.top,rtcapcenter.right,rtcapcenter.bottom,15,&nbcnt,&dtmp11);
	if(pView->m_nMachine==2)
		rttab.SetRect(pView->m_Project.m_ptCapCenter.x-TabSizeHalf,pView->m_Display[0].m_rtImgTracker[2].top,pView->m_Project.m_ptCapCenter.x+TabSizeHalf,pView->m_Display[0].m_rtImgTracker[2].bottom);
//		rttab.SetRect(pView->m_Project.m_ptCapCenter.x-40,pView->m_Display[0].m_rtImgTracker[2].top,pView->m_Project.m_ptCapCenter.x+40,pView->m_Display[0].m_rtImgTracker[2].bottom);
	else if(pView->m_nMachine==8){
		rttab.SetRect(pView->m_Project.m_ptCapCenter.x-40,pView->m_Display[0].m_rtImgTracker[2].top,pView->m_Project.m_ptCapCenter.x+40,pView->m_Project.m_rtWeldPos.top-20);
	}
// 밝기가 안나오고 좌/우로 치우친 경우??

	if(pView->m_pDlgSet->m_FormTab4->m_bTwoTab){
		rttab.top = pView->m_Display[0].m_rtImgTracker[2].top; 
		rttab.bottom = pView->m_Display[0].m_rtImgTracker[2].bottom; 

	}
	else{
		if(pView->m_nTopCapCenterBrightness > pView->m_pDlgSet->m_FormTab2->m_nCapCenterBright){
			rttab.top = pView->m_Project.m_ptCapCenter.y-10; 
		}

		if(pView->m_rtWeldBlob.Width()<70){
			rttab = pView->m_Project.CheckROI(rttab,CRect(10,10,CAMERA_WIDTH-10,pView->m_Display[0].m_rtImgTracker[2].bottom-10));
		}
		else
			rttab = pView->m_Project.CheckROI(rttab,CRect(10,10,CAMERA_WIDTH-10,pView->m_rtWeldBlob.top-20));
	}
	pView->m_Display[0].AddArrayRect(rttab);
	int x11,x12,y11,y12;
	FindTopTab(pView->m_Project.m_AlphaBuf[0] , rttab,&x11, &y11,&x12,&y12);

	pView->m_Project.m_nTopPos[0] = y11;
	if(bShow)
		pView->DrawDisplay2(0);

	delete fmtmp;
}

void CFormTab3::fnXSum(int nstart,int nend,int nFrame)
{//0
	CFlyCapture2_EXDlg* pView = (CFlyCapture2_EXDlg *)m_hParent;
	//상하에 대한 에지를 찾는다.

	//탑캡 중심을 찾고 일정 영역을 설정하낟.

	//	int ntap =pView->m_Project.CircleFind1();//Tracker search
	BYTE * fmtmp = new BYTE[CAMERA_WIDTH*CAMERA_HEIGHT];
	memset(fmtmp,0,CAMERA_HEIGHT*CAMERA_WIDTH);
	int nLimit = 0;

	//0-2번영상 
	for(int k=nstart;k<nend;k++){
		for(int y=0;y<CAMERA_HEIGHT-4;y++){
			for(int x=0;x<CAMERA_WIDTH-4;x++){
				if(abs(*(pView->m_Project.m_MultiBuf[k]+y*CAMERA_WIDTH+x) - *(pView->m_Project.m_MultiBuf[k]+(y)*CAMERA_WIDTH+x+4)) < nLimit )
					*(fmtmp+y*CAMERA_WIDTH+x) += 0;
				else
					*(fmtmp+y*CAMERA_WIDTH+x) += abs(*(pView->m_Project.m_MultiBuf[k]+y*CAMERA_WIDTH+x) - *(pView->m_Project.m_MultiBuf[k]+(y)*CAMERA_WIDTH+x+4)) ;
//			*(fmtmp+y*CAMERA_WIDTH+x) += abs(*(pView->m_Project.m_MultiBuf[k]+y*CAMERA_WIDTH+x) - *(pView->m_Project.m_MultiBuf[k]+(y)*CAMERA_WIDTH+x+4)) ;
				if(*(fmtmp+y*CAMERA_WIDTH+x)>250)*(fmtmp+y*CAMERA_WIDTH+x) = 250;
			}
		}
	}
	memcpy(pView->m_Display[0].m_pImg->GetImagePtr(),fmtmp,CAMERA_WIDTH*CAMERA_HEIGHT);
	//	memcpy(m_Display[TargetCam].m_pImg->GetImagePtr(),m_Project.m_fm[TargetCam],CAMERA_WIDTH*CAMERA_HEIGHT);
	memcpy(pView->m_DisplaySave[0].m_pImg->GetImagePtr(),pView->m_Display[0].m_pImg->GetImagePtr(),CAMERA_WIDTH*CAMERA_HEIGHT);
	memcpy(pView->m_Project.m_AlphaBuf[nFrame],fmtmp,CAMERA_WIDTH*CAMERA_HEIGHT);

//	pView->DrawDisplay(0);

	delete fmtmp;
}

void CFormTab3::fnXSum1(int nstart,int nend,int nFrame,CRect rt,BOOL bShow,BOOL bSave,double dAmplify)
{//0
	CFlyCapture2_EXDlg* pView = (CFlyCapture2_EXDlg *)m_hParent;
	BYTE * fmtmp = new BYTE[CAMERA_WIDTH*CAMERA_HEIGHT];
	memset(fmtmp,0,CAMERA_HEIGHT*CAMERA_WIDTH);
	for(int k=nstart;k<=nend;k++){
		for(int y=rt.top;y<rt.bottom;y++){
			for(int x=rt.left; x< rt.right;x++){
				if(abs(*(pView->m_Project.m_MultiBuf[k]+y*CAMERA_WIDTH+x) - *(pView->m_Project.m_MultiBuf[k]+(y)*CAMERA_WIDTH+x+4)) <0)
					*(fmtmp+y*CAMERA_WIDTH+x) += 0;
				else
					*(fmtmp+y*CAMERA_WIDTH+x) += abs(*(pView->m_Project.m_MultiBuf[k]+y*CAMERA_WIDTH+x) - *(pView->m_Project.m_MultiBuf[k]+(y)*CAMERA_WIDTH+x+4))*dAmplify ;
				if(*(fmtmp+y*CAMERA_WIDTH+x)>250)*(fmtmp+y*CAMERA_WIDTH+x) = 250;
			}
		}
	}
	if(bShow)	memcpy(pView->m_Display[0].m_pImg->GetImagePtr(),fmtmp,CAMERA_WIDTH*CAMERA_HEIGHT);
	memcpy(pView->m_Project.m_AlphaBuf[1],fmtmp,CAMERA_WIDTH*CAMERA_HEIGHT);
	//	memcpy(pView->m_DisplaySave[0].m_pImg->GetImagePtr(),pView->m_Display[0].m_pImg->GetImagePtr(),CAMERA_WIDTH*CAMERA_HEIGHT);
	CRect rttab1;
	int nmargin = 50,nmargin1 = 0;
	//	rttab1.SetRect(pView->m_Project.m_ptTopEdge[0].x-nmargin,pView->m_Project.m_ptTopEdge[0].y,pView->m_Project.m_ptTopEdge[0].x+nmargin,pView->m_Project.m_ptTopEdge[0].y+100);
	rttab1.SetRect(pView->m_Project.m_ptCapCenter.x-((pView->m_Display[0].m_rtImgTracker[2].Width())/2),pView->m_Project.m_ptTopEdge[0].y,pView->m_Project.m_ptCapCenter.x-10,pView->m_Project.m_ptTopEdge[0].y+50);//woo
	if(pView->m_Project.m_dTabTopLen<pView->m_pDlgSet->m_FormTab4->m_dTapSize-0.5 || pView->m_Project.m_dTabTopLen>pView->m_pDlgSet->m_FormTab4->m_dTapSize+0.5){
		if(pView->m_pDlgSet->m_FormTab4->m_dTapSize>3.1){
			nmargin = 60;
			nmargin1 = 0;
		}
		else if(pView->m_pDlgSet->m_FormTab4->m_dTapSize<3.1){
			nmargin = 70;
			nmargin1 = 0;
		}
		else{
			nmargin = 40;
			nmargin1 = 0;
		}
		//		rttab1.SetRect(pView->m_Project.m_ptTopEdge[0].x-nmargin,pView->m_Project.m_ptTopEdge[0].y,pView->m_Project.m_ptCapCenter.x+nmargin1,pView->m_Project.m_ptTopEdge[0].y+100);
		rttab1.SetRect(pView->m_Project.m_ptCapCenter.x-((pView->m_Display[0].m_rtImgTracker[2].Width())/2),pView->m_Project.m_ptTopEdge[0].y,pView->m_Project.m_ptCapCenter.x-10,pView->m_Project.m_ptTopEdge[0].y+50);
	}
	rttab1.SetRect(pView->m_Project.m_ptCapCenter.x - 140, pView->m_Project.m_ptTopEdge[0].y + 20, pView->m_Project.m_ptCapCenter.x - 10, pView->m_Project.m_ptTopEdge[0].y + 200);
	//rttab1.SetRect(pView->m_Project.m_ptCapCenter.x - 140, pView->m_Project.m_ptTopEdge[0].y + 20, pView->m_Project.m_ptCapCenter.x + 30, pView->m_Project.m_ptTopEdge[0].y + 200);
	rttab1 = pView->m_Project.CheckROI(rttab1,CRect(10,10,CAMERA_WIDTH-10,CAMERA_HEIGHT-10));
	pView->m_Display[0].AddArrayRect(rttab1,0);
	FindLeft(pView->m_Project.m_AlphaBuf[1] , rttab1);

	int nSelectcam = pView->m_nSelectFrame;
	int nh1 = pView->m_Project.m_rtWeldPos.Height();
	int nw1 = pView->m_Project.m_rtWeldPos.Width();
	int x2=0,y2=(pView->m_Project.m_rtWeldPos.Height()+10)*2;
	int x3 = pView->m_Project.m_rtWeldPos.left,y3=pView->m_Project.m_rtWeldPos.top;
	//여러장을 도니 다시 복사한다.

	//if(nSelectcam<=10){
	//	for(int i=y2;i<y2+nh1;i++){
	//		x3 = pView->m_Project.m_rtWeldPos.left;
	//		if(y3>CAMERA_HEIGHT-10|| y3<0)break;
	//		for(int j=x2;j<x2+nw1;j++){
	//			if(x3>CAMERA_WIDTH-10 || x3<0)break;
	//			*(pView->m_Project.m_MultiBuf[nSelectcam]+i*CAMERA_WIDTH+j) = *(fmtmp+(y3)*CAMERA_WIDTH+x3);
	//			x3++;
	//		}
	//		y3++;
	//	}
	//}

	WeldPoint(fmtmp,CAMERA_WIDTH,CAMERA_HEIGHT,pView->m_Project.m_rtWeldPos,2);

	if(bShow)
		pView->DrawDisplay2(0);

	delete fmtmp;
}

void CFormTab3::fnXSum2(int nstart,int nend,int nFrame,CRect rt,BOOL bShow,BOOL bSave,double dAmplify)
{
	//right
	CFlyCapture2_EXDlg* pView = (CFlyCapture2_EXDlg *)m_hParent;
	//상하에 대한 에지를 찾는다.
	//탑캡 중심을 찾고 일정 영역을 설정하낟.

	//	int ntap =pView->m_Project.CircleFind1();//Tracker search
	BYTE * fmtmp = new BYTE[CAMERA_WIDTH*CAMERA_HEIGHT];
	memset(fmtmp,0,CAMERA_HEIGHT*CAMERA_WIDTH);
	for(int k=nstart;k<=nend;k++){
		for(int y=rt.top;y<rt.bottom;y++){
			for(int x=rt.left; x< rt.right;x++){
				if(abs(*(pView->m_Project.m_MultiBuf[k]+y*CAMERA_WIDTH+x) - *(pView->m_Project.m_MultiBuf[k]+(y)*CAMERA_WIDTH+x+4)) <0)
					*(fmtmp+y*CAMERA_WIDTH+x) += 0;
				else
					*(fmtmp+y*CAMERA_WIDTH+x) += abs(*(pView->m_Project.m_MultiBuf[k]+y*CAMERA_WIDTH+x) - *(pView->m_Project.m_MultiBuf[k]+(y)*CAMERA_WIDTH+x+4))*dAmplify ;
				if(*(fmtmp+y*CAMERA_WIDTH+x)>250)*(fmtmp+y*CAMERA_WIDTH+x) = 250;
			}
		}
	}
	if(bShow)memcpy(pView->m_Display[0].m_pImg->GetImagePtr(),fmtmp,CAMERA_WIDTH*CAMERA_HEIGHT);
	//memcpy(pView->m_DisplaySave[0].m_pImg->GetImagePtr(),pView->m_Display[0].m_pImg->GetImagePtr(),CAMERA_WIDTH*CAMERA_HEIGHT);
	memcpy(pView->m_Project.m_AlphaBuf[2],fmtmp,CAMERA_WIDTH*CAMERA_HEIGHT);

	CRect rttab1;
	int nmargin = 40,nmargin1=0;
	//	rttab1.SetRect(pView->m_Project.m_ptTopEdge[1].x-nmargin,pView->m_Project.m_ptTopEdge[1].y,pView->m_Project.m_ptTopEdge[1].x+nmargin,pView->m_Project.m_ptTopEdge[1].y+100);
	rttab1.SetRect(pView->m_Project.m_ptCapCenter.x+20,pView->m_Project.m_ptTopEdge[1].y,pView->m_Project.m_ptCapCenter.x+((pView->m_Display[0].m_rtImgTracker[2].Width())/2),pView->m_Project.m_ptTopEdge[1].y+50);
	if(pView->m_Project.m_dTabTopLen<pView->m_pDlgSet->m_FormTab4->m_dTapSize-0.5 || pView->m_Project.m_dTabTopLen>pView->m_pDlgSet->m_FormTab4->m_dTapSize+0.5){
		//	if(pView->m_Project.m_dTabTopLen<pView->m_pDlgSet->m_FormTab4->m_dTapSize || pView->m_Project.m_dTabTopLen>pView->m_pDlgSet->m_FormTab4->m_dTapSize){ //woo 무조건 영역 크게

		if(pView->m_pDlgSet->m_FormTab4->m_dTapSize>3.1){
			nmargin = 60;
			nmargin1 = 0;
		}
		else if(pView->m_pDlgSet->m_FormTab4->m_dTapSize<3.1){
			nmargin = 70;	
			nmargin1 = 0;
		}
		else	
			nmargin = 40;
		// 		rttab1.SetRect(pView->m_Project.m_ptCapCenter.x+nmargin1,pView->m_Project.m_ptTopEdge[1].y,pView->m_Project.m_ptTopEdge[1].x+nmargin,pView->m_Project.m_ptTopEdge[1].y+100);
		rttab1.SetRect(pView->m_Project.m_ptCapCenter.x+20,pView->m_Project.m_ptTopEdge[1].y,pView->m_Project.m_ptCapCenter.x+((pView->m_Display[0].m_rtImgTracker[2].Width())/2),pView->m_Project.m_ptTopEdge[1].y+50);
	}
	rttab1.SetRect(pView->m_Project.m_ptCapCenter.x + 10, pView->m_Project.m_ptTopEdge[1].y + 20, pView->m_Project.m_ptCapCenter.x + 140, pView->m_Project.m_ptTopEdge[1].y + 200);
	//rttab1.SetRect(pView->m_Project.m_ptCapCenter.x - 30, pView->m_Project.m_ptTopEdge[1].y + 20, pView->m_Project.m_ptCapCenter.x + 140, pView->m_Project.m_ptTopEdge[1].y + 200);
	rttab1 = pView->m_Project.CheckROI(rttab1,CRect(10,10,CAMERA_WIDTH-10,CAMERA_HEIGHT-10));
	FindRight(pView->m_Project.m_AlphaBuf[2] , rttab1);
	//FindRight(pView->m_Project.m_AlphaBuf[1] , rttab1);
	pView->m_Display[0].AddArrayRect(rttab1,0);
	int nSelectcam = pView->m_nSelectFrame;
	int nh1 = pView->m_Project.m_rtWeldPos.Height();
	int nw1 = pView->m_Project.m_rtWeldPos.Width();
	int x2=0,y2=(pView->m_Project.m_rtWeldPos.Height()+10)*3;
	int x3 = pView->m_Project.m_rtWeldPos.left,y3=pView->m_Project.m_rtWeldPos.top;

	//여러장을 도니 다시 복사한다.
	/*if(nSelectcam<=10){
		for(int i=y2;i<y2+nh1;i++){
			x3 = pView->m_Project.m_rtWeldPos.left;
			if(y3>CAMERA_HEIGHT-10|| y3<0)break;
			for(int j=x2;j<x2+nw1;j++){
				if(x3>CAMERA_WIDTH-10 || x3<0)break;
				*(pView->m_Project.m_MultiBuf[nSelectcam]+i*CAMERA_WIDTH+j) = *(fmtmp+(y3)*CAMERA_WIDTH+x3);
				x3++;
			}
			y3++;
		}
	}*/
	WeldPoint(fmtmp,CAMERA_WIDTH,CAMERA_HEIGHT,pView->m_Project.m_rtWeldPos,3);

	delete fmtmp;

}
void CFormTab3::fnImageSummary(BOOL bShow, BOOL bSave)
{
	//Image summary
	CFlyCapture2_EXDlg* pView = (CFlyCapture2_EXDlg *)m_hParent;
	BYTE * fmtmp = new BYTE[CAMERA_WIDTH*CAMERA_HEIGHT];
	memset(fmtmp,0,CAMERA_HEIGHT*CAMERA_WIDTH);

	int nmode = 1;
	if(nmode==0){
		for(int k=0;k<3;k++){
			for(int y=0;y<CAMERA_HEIGHT-4;y++){
				for(int x=0;x<CAMERA_WIDTH-4;x++){
					*(fmtmp+y*CAMERA_WIDTH+x) += *(pView->m_Project.m_AlphaBuf[k]+y*CAMERA_WIDTH+x) ;
					if(*(fmtmp+y*CAMERA_WIDTH+x)>250)*(fmtmp+y*CAMERA_WIDTH+x) = 250;
				}
			}
		}
	}
	else{
		for(int y=0;y<CAMERA_HEIGHT-4;y++){
			for(int x=0;x<CAMERA_WIDTH-4;x++){
				if(*(pView->m_Project.m_AlphaBuf[0]+y*CAMERA_WIDTH+x) > *(pView->m_Project.m_AlphaBuf[1]+y*CAMERA_WIDTH+x)){
					if(*(pView->m_Project.m_AlphaBuf[0]+y*CAMERA_WIDTH+x) > *(pView->m_Project.m_AlphaBuf[2]+y*CAMERA_WIDTH+x)){
						*(fmtmp+y*CAMERA_WIDTH+x) = *(pView->m_Project.m_AlphaBuf[0]+y*CAMERA_WIDTH+x);
					}
					else
						*(fmtmp+y*CAMERA_WIDTH+x) = *(pView->m_Project.m_AlphaBuf[2]+y*CAMERA_WIDTH+x);
				}
				else{
					if(*(pView->m_Project.m_AlphaBuf[1]+y*CAMERA_WIDTH+x) > *(pView->m_Project.m_AlphaBuf[2]+y*CAMERA_WIDTH+x)){
						*(fmtmp+y*CAMERA_WIDTH+x) = *(pView->m_Project.m_AlphaBuf[1]+y*CAMERA_WIDTH+x);
					}
					else
						*(fmtmp+y*CAMERA_WIDTH+x) = *(pView->m_Project.m_AlphaBuf[2]+y*CAMERA_WIDTH+x);
				}
			}
		}
	}


	memcpy(pView->m_Display[0].m_pImg->GetImagePtr(),fmtmp,CAMERA_WIDTH*CAMERA_HEIGHT);
	memcpy(pView->m_Project.m_AlphaBuf[3],fmtmp,CAMERA_WIDTH*CAMERA_HEIGHT);
	//	memcpy(m_Display[TargetCam].m_pImg->GetImagePtr(),m_Project.m_fm[TargetCam],CAMERA_WIDTH*CAMERA_HEIGHT);
	memcpy(pView->m_DisplaySave[0].m_pImg->GetImagePtr(),pView->m_Display[0].m_pImg->GetImagePtr(),CAMERA_WIDTH*CAMERA_HEIGHT);

//	pView->AddtoInspLog("Sum");
	delete fmtmp;

}
void CFormTab3::fnImageSummary1(BOOL bShow, BOOL bSave,CRect rt)
{
	//Image summary
	CFlyCapture2_EXDlg* pView = (CFlyCapture2_EXDlg *)m_hParent;
	BYTE * fmtmp = new BYTE[CAMERA_WIDTH*CAMERA_HEIGHT];
	memset(fmtmp,0,CAMERA_HEIGHT*CAMERA_WIDTH);

	int nmode = 1;
	if(nmode==0){
		for(int k=0;k<3;k++){
			for(int y=rt.top;y<rt.bottom;y++){
				for(int x=rt.left;x<rt.right;x++){
					*(fmtmp+y*CAMERA_WIDTH+x) += *(pView->m_Project.m_AlphaBuf[k]+y*CAMERA_WIDTH+x) ;
					if(*(fmtmp+y*CAMERA_WIDTH+x)>250)*(fmtmp+y*CAMERA_WIDTH+x) = 250;
				}
			}
		}
	}
	else{
		for(int y=rt.top;y<rt.bottom;y++){
			for(int x=rt.left;x<rt.right;x++){
				if(*(pView->m_Project.m_AlphaBuf[0]+y*CAMERA_WIDTH+x) > *(pView->m_Project.m_AlphaBuf[1]+y*CAMERA_WIDTH+x)){
					if(*(pView->m_Project.m_AlphaBuf[0]+y*CAMERA_WIDTH+x) > *(pView->m_Project.m_AlphaBuf[2]+y*CAMERA_WIDTH+x)){
						*(fmtmp+y*CAMERA_WIDTH+x) = *(pView->m_Project.m_AlphaBuf[0]+y*CAMERA_WIDTH+x);
					}
					else
						*(fmtmp+y*CAMERA_WIDTH+x) = *(pView->m_Project.m_AlphaBuf[2]+y*CAMERA_WIDTH+x);
				}
				else{
					if(*(pView->m_Project.m_AlphaBuf[1]+y*CAMERA_WIDTH+x) > *(pView->m_Project.m_AlphaBuf[2]+y*CAMERA_WIDTH+x)){
						*(fmtmp+y*CAMERA_WIDTH+x) = *(pView->m_Project.m_AlphaBuf[1]+y*CAMERA_WIDTH+x);
					}
					else
						*(fmtmp+y*CAMERA_WIDTH+x) = *(pView->m_Project.m_AlphaBuf[2]+y*CAMERA_WIDTH+x);
				}
			}
		}
	}


	if(bShow)memcpy(pView->m_Display[0].m_pImg->GetImagePtr(),fmtmp,CAMERA_WIDTH*CAMERA_HEIGHT);
	memcpy(pView->m_Project.m_AlphaBuf[3],fmtmp,CAMERA_WIDTH*CAMERA_HEIGHT);
	//	memcpy(m_Display[TargetCam].m_pImg->GetImagePtr(),m_Project.m_fm[TargetCam],CAMERA_WIDTH*CAMERA_HEIGHT);
	//memcpy(pView->m_DisplaySave[0].m_pImg->GetImagePtr(),pView->m_Display[0].m_pImg->GetImagePtr(),CAMERA_WIDTH*CAMERA_HEIGHT);

	//	pView->AddtoInspLog("Sum");
	delete fmtmp;

}


void CFormTab3::OnBnClickedButton6()
{//

	CFlyCapture2_EXDlg* pView = (CFlyCapture2_EXDlg *)m_hParent;

	pView->m_Project.m_ptLeftTop.SetPoint(0,0);
	pView->m_Project.m_ptRightTop=pView->m_Project.m_ptLeftTop;

	pView->m_Project.m_fm[0] =pView-> m_Display[0].m_pImg->GetImagePtr();
	BYTE * fm = pView-> m_Display[0].m_pImg->GetImagePtr();
	int nth = 150,nEdgeth=70;
	int nHisto[256]={0,};

//top center find
	double d1[10]={0,};
	d1[0] = pView->m_Project.GetPrecisionTime();
	int ntap = pView->m_Project.CircleFind1();//Tracker search
	d1[1] = pView->m_Project.GetPrecisionTime();
//m_ptCapCenter
	int nMarginx = 230,nMarginy = 100,nMarginy1 = 10;
	CRect rttab;
	rttab.SetRect(pView->m_Project.m_ptCapCenter.x-nMarginx,pView->m_Project.m_ptCapCenter.y-nMarginy1,pView->m_Project.m_ptCapCenter.x+nMarginx,pView->m_Project.m_ptCapCenter.y+nMarginy);
//check rect
	rttab = pView->m_Project.CheckROI(rttab,CRect(20,20,CAMERA_WIDTH-20,CAMERA_HEIGHT-20));
	int nProjx[CAMERA_WIDTH],nProjx1[CAMERA_WIDTH],nProjy[CAMERA_WIDTH],nProjy1[CAMERA_WIDTH],projSum[CAMERA_WIDTH],projSum1[CAMERA_WIDTH];
	for(int i=0;i<CAMERA_WIDTH;i++)nProjx[i]=nProjx1[i]=nProjy[i]=nProjy1[i]=projSum[i]=projSum1[i]=0;

	//용접 Y pos를 캔 상단과 캡 중앙을 기준으로 보정한다.

	double dave=0,dc1=0;
// 	for(int y=0;y<CAMERA_HEIGHT;y++){
// 		for(int x=0;x<CAMERA_WIDTH;x++){
	for(int y=rttab.top;y<rttab.bottom;y++){
		for(int x=rttab.left;x<rttab.right;x++){		
			nProjx1[x] += *(fm+y*CAMERA_WIDTH+x);
			nHisto[*(fm+y*CAMERA_WIDTH+x)]++;
			dave +=  *(fm+y*CAMERA_WIDTH+x);
		}
	}
	dave /= rttab.Width()*rttab.Height();
	nth = dave*1.8;

	d1[2] = pView->m_Project.GetPrecisionTime();

	for(int y=rttab.top;y<rttab.bottom;y++){
		for(int x=rttab.left;x<rttab.right;x++){

			
// 			if(abs(*(fm+y*CAMERA_WIDTH+x)-*(fm+y*CAMERA_WIDTH+x+4)) >nEdgeth)
// 				*(fm+y*CAMERA_WIDTH+x) = 255;
// 			else
// 				*(fm+y*CAMERA_WIDTH+x) = 0;

 			if(*(fm+y*CAMERA_WIDTH+x) >nth)			*(fm+y*CAMERA_WIDTH+x) = 255;
 			else			*(fm+y*CAMERA_WIDTH+x) = 0;
		}
	}
	d1[3] = pView->m_Project.GetPrecisionTime();

	int nmax = 0,npos=0,nmax1 = 0,npos1=0;
	for(int i=50;i<256;i++)
		if(nHisto[i]>nmax){nmax = nHisto[i];npos=i;}
//좌/우가 먼저일지? 상이 먼저일지?
//master Edge 선정
	int nMaster=0; //1-left  2-right	0-all
//projection
	for(int y=rttab.top;y<rttab.bottom;y++){
		for(int x=rttab.left;x<rttab.right;x++){
			nProjx[x] += *(fm+y*CAMERA_WIDTH+x);
			if(*(fm+y*CAMERA_WIDTH+x)==255)
				nProjy[x]++;
		}
//		nProjy[y] /= rttab.Width();
	}
	d1[4] = pView->m_Project.GetPrecisionTime();

	nmax = npos=0;
	for(int x=rttab.left;x<rttab.right;x++){
		nProjx[x] /= rttab.Height();
		nProjx1[x] /= rttab.Height();
		projSum[x] = (nProjx[x]+nProjx1[x]+nProjy[x]);
		if(x>rttab.left+1)
			projSum1[x] = abs(projSum[x]-projSum[x-2]);

		if(projSum1[x]>nmax){nmax = projSum1[x];npos = x;}
//		Sleep(5);		TRACE("\n%d - %d - %d - %d - %d",x,nProjx1[x],nProjx[x],nProjy[x],projSum[x]);
	}
	d1[5] = pView->m_Project.GetPrecisionTime();

	//peak 기준 50 픽셀을 건너뛰고 2번째 max를 찾는다.
	for(int x=rttab.left;x<rttab.right;x++){
		if(x> npos-50 && x<npos+50)continue;

		if(projSum1[x]>nmax1){
			nmax1 = projSum1[x];npos1 = x;}

	}
	//draw Line //m_Project.m_nBottomEdge[i]
	CPoint pt1,pt2;

	if(npos>npos1)pt1.x=npos1,pt2.x=npos;
	else	pt1.x=npos,pt2.x=npos1;
	CRect rttop;
	rttop = rttab;
	rttop.left = pt1.x;rttop.right = pt2.x;
	
	for(int y=rttop.top;y<rttop.bottom;y++){
		for(int x=rttop.left;x<rttop.right;x++){
			nProjy1[y] += *(fm+y*CAMERA_WIDTH+x);
		}
		nProjy1[y] /= rttop.Width();
	}
	d1[6] = pView->m_Project.GetPrecisionTime();

	for(int y=rttop.bottom-10;y>rttop.top;y--){
		//하단에서 올라가면서 커지는데
		if(abs(nProjy1[y]-nProjy1[y-4])>45){
			pt1.y = y;
			pt2.y = y;
			pView->m_Project.m_ptLeftTop=pt1;
			pView->m_Project.m_ptRightTop=pt2;

			break;
		}
	}
	d1[7] = pView->m_Project.GetPrecisionTime();

//	pView->DrawDisplay(0);
	d1[8] = pView->m_Project.GetPrecisionTime();

	int nnn = 0;
}

void CFormTab3::OnBnClickedButtonLoad()
{
	CFlyCapture2_EXDlg* pView = (CFlyCapture2_EXDlg *)m_hParent;

	//	CFileDialog dlg(TRUE, _T("bmp"), NULL, OFN_SHAREAWARE, _T("Bitmap Files (*.bmp)|*.bmp||"));
	CFileDialog dlg(TRUE, NULL, NULL, OFN_SHAREAWARE, "All Files(*.*)|*.*||");//szFilter);

	if(dlg.DoModal() == IDOK)
	{
		pView->Invalidate();
		CString strpath,str1,str2;
		strpath = dlg.GetPathName();
		int n = strpath.Find("Cam");
		int l = strpath.GetLength();
		str1 = strpath.Left(n+3);
		str2.Format("%s2%s",str1,strpath.Right(l-n-4));
		pView->m_Display[0].LoadImg(dlg.GetPathName());
	
		pView->m_Project.m_fm[0] =pView-> m_Display[0].m_pImg->GetImagePtr();
		pView->m_Display[0].SetZoom(0,CPoint(0,0));
		//pDlg->Invalidate();
	}
}

int CFormTab3::fnTabFind(BYTE * fmtab,CRect rttab,BOOL bShow)
{
	CFlyCapture2_EXDlg* pView = (CFlyCapture2_EXDlg *)m_hParent;
	int nth = 150,nEdgeth=70,nmin1=255,nmax11=0;
	int nHisto[256]={0,};
//탭 영역의 상하를 조정한다.
	//Top Cap Center에 STD 
	CRect rttab1[3],rttab2;
	double dstd1=0,dstd2=0,dstd3=0,dmin1=0,dvar1=0,dstdave=0;
	CString str1;
	if(pView->m_pDlgSet->m_FormTab4->m_b2Tab){
		rttab1[1].SetRect(pView->m_Project.m_ptCapCenter.x-20,pView->m_Project.m_ptCapCenter.y-20,pView->m_Project.m_ptCapCenter.x+20,pView->m_Project.m_ptCapCenter.y+20);
		rttab1[2].SetRect(pView->m_Project.m_ptCapCenter.x-20,pView->m_Project.m_ptCapCenter.y-40,pView->m_Project.m_ptCapCenter.x+20,pView->m_Project.m_ptCapCenter.y-20);
		rttab1[0].SetRect(pView->m_Project.m_ptCapCenter.x-20,pView->m_Project.m_ptCapCenter.y+20,pView->m_Project.m_ptCapCenter.x+20,pView->m_Project.m_ptCapCenter.y+40);
	}
	else{
		rttab1[1].SetRect(pView->m_Project.m_ptCapCenter.x-20,pView->m_Project.m_ptCapCenter.y-20,pView->m_Project.m_ptCapCenter.x+20,pView->m_Project.m_ptCapCenter.y+20);
		rttab1[2].SetRect(pView->m_Project.m_ptCapCenter.x-20,pView->m_Project.m_ptCapCenter.y-40,pView->m_Project.m_ptCapCenter.x+20,pView->m_Project.m_ptCapCenter.y-20);
		rttab1[0].SetRect(pView->m_Project.m_ptCapCenter.x-20,pView->m_Project.m_ptCapCenter.y+20,pView->m_Project.m_ptCapCenter.x+20,pView->m_Project.m_ptCapCenter.y+40);
	}
//	pView->m_Display[0].AddArrayRect(rttab1[1],0);pView->m_Display[0].AddArrayRect(rttab1[2],0);pView->m_Display[0].AddArrayRect(rttab1[0],0);

	rttab1[0] = pView->m_Project.CheckROI(rttab1[0],CRect(10,10,CAMERA_WIDTH-10,CAMERA_HEIGHT-10));
	rttab1[1] = pView->m_Project.CheckROI(rttab1[1],CRect(10,10,CAMERA_WIDTH-10,CAMERA_HEIGHT-10));
	rttab1[2] = pView->m_Project.CheckROI(rttab1[2],CRect(10,10,CAMERA_WIDTH-10,CAMERA_HEIGHT-10));

	double dave1 = pView->m_Project.GetRoiInfo(fmtab,CAMERA_WIDTH,rttab1[0],1,1,&dstd1,&nmin1,&nmax11,&dvar1,0);
//	str1.Format("Std %.2f ,Min %d,Max%d,Ave%.1f",dstd1,nmin1,nmax11,dave1);pView->AddtoInspLog(str1);
	double dave2 = pView->m_Project.GetRoiInfo(fmtab,CAMERA_WIDTH,rttab1[1],1,1,&dstd2,&nmin1,&nmax11,&dvar1,0);
//	str1.Format("Std %.2f ,Min %d,Max%d,Ave%.1f",dstd2,nmin1,nmax11,dave2);pView->AddtoInspLog(str1);
	double dave3 = pView->m_Project.GetRoiInfo(fmtab,CAMERA_WIDTH,rttab1[2],1,1,&dstd3,&nmin1,&nmax11,&dvar1,0);
//	str1.Format("Std %.2f ,Min %d,Max%d,Ave%.1f",dstd3,nmin1,nmax11,dave3);pView->AddtoInspLog(str1);
//편차 값으로 개략적 Y 위치를 찾는다.
	//편차가 높으면 밑으로 낮으면 위로 
	dstdave = (dstd1+dstd2+dstd3)*0.33;
	double dstdth = dstdave*0.9;
	int nYoffset1=50,nYoffset2=150,nYoffset3=0;

	BYTE * fmtop = new BYTE[CAMERA_WIDTH*CAMERA_HEIGHT];
	memcpy(fmtop,pView->m_Project.m_AlphaBuf[0],CAMERA_HEIGHT*CAMERA_WIDTH);

 	if(dstd1>dstdth && dstd2>dstdth && dstd3>dstdth){//다시 찾는 방법 추가해야함.
 		//스캔도 밑으로 해야 하나?
 		str1.Format("oth:stdAve %.2f ,th %.2f",dstdave,dstdth);pView->AddtoInspLog(str1);
 	}
 	else if(dstd2 > dstdth && dstd1<dstdth){//중앙부 에지 있음	//2번 바텀부터
 		nYoffset1=20,nYoffset2=80;
		nYoffset3 = 20;
 		str1.Format("mid:stdAve %.2f ,th %.2f",dstdave,dstdth);pView->AddtoInspLog(str1);
 	}
 	else if(dstd2 < dstdth && dstd1 < dstdth&& dstd3 > dstdth){//위쪽 에지 있음 //1번 바텀부터
 		nYoffset1=150,nYoffset2=50;
 		str1.Format("top:stdAve %.2f ,th %.2f",dstdave,dstdth);pView->AddtoInspLog(str1);
 	}
 	else if(dstd1 > dstdth && dstd2 > dstdth&& dstd3 > dstdth){//다클경우?전 체 다시 스캔?? NG일 확률이 높음.
 	}
 	else{
 		str1.Format("bot2:stdAve %.2f ,th %.2f",dstdave,dstdth);pView->AddtoInspLog(str1);
 	}
//top search에 대한 영역만 설정한다.
	rttab2 = rttab;
	if(pView->m_pDlgSet->m_FormTab4->m_b2Tab){
		rttab2.top = pView->m_Project.m_ptCapCenter.y-220;
		rttab2.bottom = pView->m_Project.m_ptCapCenter.y-80;
		rttab.top = pView->m_Project.m_ptCapCenter.y-220;rttab.bottom = pView->m_Project.m_ptCapCenter.y-80;
	}
	else{
		rttab2.top = pView->m_Project.m_ptCapCenter.y-nYoffset1;
		rttab2.bottom = pView->m_Project.m_ptCapCenter.y+nYoffset2;
		rttab.top = pView->m_Project.m_ptCapCenter.y+nYoffset3;rttab.bottom = pView->m_Project.m_ptCapCenter.y+120;
	}


	CRect  rtRange;//용접 탑 위치에 Botom을 고정시킨다.
	if(pView->m_pDlgSet->m_FormTab4->m_b2Tab){
		rtRange.SetRect(20,180,CAMERA_WIDTH-20,pView->m_Project.m_rtWeldPos.top);//
	}
	else{
		rtRange.SetRect(20,180,CAMERA_WIDTH-20,pView->m_Project.m_rtWeldPos.top);//
	}
	rttab2 = pView->m_Project.CheckROI(rttab2,rtRange);
	rttab = pView->m_Project.CheckROI(rttab,rtRange);
	

///	str1.Format("rttab : %d %d %d %d",rttab.left,rttab.top,rttab.right,rttab.bottom);pView->AddtoInspLog(str1);

	pView->m_Display[0].AddArrayRect(rttab,0);
	int nProjx[CAMERA_WIDTH],nProjx1[CAMERA_WIDTH],nProjy[CAMERA_WIDTH],nProjy1[CAMERA_WIDTH],nProjy2[CAMERA_WIDTH],nProjy3[CAMERA_WIDTH],projSum[CAMERA_WIDTH],projSum1[CAMERA_WIDTH];
	for(int i=0;i<CAMERA_WIDTH;i++)nProjx[i]=nProjx1[i]=nProjy[i]=nProjy1[i]=nProjy2[i]=nProjy3[i]=projSum[i]=projSum1[i]=0;

	//용접 Y pos를 캔 상단과 캡 중앙을 기준으로 보정한다.

	double dave=0,dc1=0;
// 	for(int y=0;y<CAMERA_HEIGHT;y++){
// 		for(int x=0;x<CAMERA_WIDTH;x++){
	for(int y=rttab.top;y<rttab.bottom;y++){
		for(int x=rttab.left;x<rttab.right;x++){		
			nProjx1[x] += *(fmtab+y*CAMERA_WIDTH+x);
			nHisto[*(fmtab+y*CAMERA_WIDTH+x)]++;
			dave +=  *(fmtab+y*CAMERA_WIDTH+x);
		}
	}
	dave /= rttab.Width()*rttab.Height();
	if(pView->m_pDlgSet->m_FormTab4->m_b2Tab){
		nth = dave*1.2;
		nEdgeth = dave*0.8;
	}
	else{
		nth = dave*1.8;
		nEdgeth = dave;
	}

//	d1[2] = pView->m_Project.GetPrecisionTime();
	CRect rttmp1;
	rttmp1 = rttab;
	if(rttab.top>rttab2.top)rttmp1.top = rttab2.top;
	if(rttab.bottom<rttab2.bottom)rttmp1.bottom = rttab2.bottom;
	if(rttab.left>rttab2.left)rttmp1.left = rttab2.left;
	if(rttab.right<rttab2.right)rttmp1.right = rttab2.right;
	for(int y=rttmp1.top;y<rttmp1.bottom;y++){
		for(int x=rttmp1.left;x<rttmp1.right;x++){

			
// 			if(abs(*(fm+y*CAMERA_WIDTH+x)-*(fm+y*CAMERA_WIDTH+x+4)) >nEdgeth)
// 				*(fm+y*CAMERA_WIDTH+x) = 255;
// 			else
// 				*(fm+y*CAMERA_WIDTH+x) = 0;

			if(*(fmtab+y*CAMERA_WIDTH+x) >nth)			*(fmtab+y*CAMERA_WIDTH+x) = 255;
			else			*(fmtab+y*CAMERA_WIDTH+x) = 0;
			if(*(fmtop+y*CAMERA_WIDTH+x) >nEdgeth)			*(fmtop+y*CAMERA_WIDTH+x) = 255;
			else			*(fmtop+y*CAMERA_WIDTH+x) = 0;

			
		}
	}

// 	for(int y=rttab2.top;y<rttab2.bottom;y++){
// 		for(int x=rttab2.left;x<rttab2.right;x++){
// 			if(*(fmtab+y*CAMERA_WIDTH+x) >nth)			*(fmtab+y*CAMERA_WIDTH+x) = 255;
// 			else			*(fmtab+y*CAMERA_WIDTH+x) = 0;
// 		}
// 	}


//	d1[3] = pView->m_Project.GetPrecisionTime();

	int nmax = 0,npos=0,nmax1 = 0,npos1=0;
	for(int i=50;i<256;i++)
		if(nHisto[i]>nmax){nmax = nHisto[i];npos=i;}
//좌/우가 먼저일지? 상이 먼저일지?
//master Edge 선정
	int nMaster=0; //1-left  2-right	0-all
//projection
	for(int y=rttab.top;y<rttab.bottom;y++){
		for(int x=rttab.left;x<rttab.right;x++){
			nProjx[x] += *(fmtab+y*CAMERA_WIDTH+x);
			if(*(fmtab+y*CAMERA_WIDTH+x)==255)
				nProjy[x]++;
		}
//		nProjy[y] /= rttab.Width();
	}
//	d1[4] = pView->m_Project.GetPrecisionTime();

	nmax = npos=0;
	for(int x=rttab.left;x<rttab.right;x++){
		nProjx[x] /= rttab.Height();//이진화 영상
		nProjx1[x] /= rttab.Height();
		projSum[x] = (nProjx[x]+nProjx1[x]);//+nProjy[x]);
		if(x>rttab.left+1)
			projSum1[x] = abs(projSum[x]-projSum[x-2]);

		if(projSum1[x]>nmax){nmax = projSum1[x];npos = x;}
//		Sleep(5);		TRACE("\n%d - %d - %d - %d - %d",x,nProjx1[x],nProjx[x],nProjy[x],projSum[x]);
	}
//	d1[5] = pView->m_Project.GetPrecisionTime();

	//peak 기준 50 픽셀을 건너뛰고 2번째 max를 찾는다.
	for(int x=rttab.left+4;x<rttab.right-4;x++){
		if(x> npos-20 && x<npos+20)continue;
		if(pView->m_nType==3){
			if(projSum1[x]>nmax1 && (abs(projSum[x]-projSum[x-4])>50 || abs(projSum[x]-projSum[x+4])>50)){
				nmax1 = projSum1[x];npos1 = x;}
		}
		else{
			if(projSum1[x]>nmax1){
				nmax1 = projSum1[x];npos1 = x;}
		}

	}
	CRect rttmp[4];//1순위 2순위에 대한 추가 검사 영역
	double dTabSize = pView->m_pDlgSet->m_FormTab4->m_dTapSize;
	double doffset1 = dTabSize/pView->m_pDlgSet->m_FormTab2->m_dCamScale;
	//max 1,2 거리가 탭폭과 같으면 건너뛴다.
	int nMaxtmp[4]={0,},npostmp[4]={0,};
	if(abs(npos1-npos)  >doffset1-20 && abs(npos1-npos)  <doffset1+20){

	}
	else
	{
		rttmp[0]=rttmp[1]=rttmp[2]=rttmp[3]=rttab;
		rttmp[0].left = npos-doffset1-20;rttmp[0].right = npos-doffset1+20;
		rttmp[1].left = npos+doffset1-20;rttmp[1].right = npos+doffset1+20;
		rttmp[2].left = npos1-doffset1-20;rttmp[2].right = npos1-doffset1+20;
		rttmp[3].left = npos1+doffset1-20;rttmp[3].right = npos1+doffset1+20;

		for(int k=0;k<4;k++){
			for(int x=rttmp[k].left;x<rttmp[k].right;x++){
				if(rttmp[k].left<rttab.left || rttmp[k].right>rttab.right)continue;

				if(projSum1[x]>nMaxtmp[k]){
					nMaxtmp[k] = projSum1[x];npostmp[k] = x;}

			}
			pView->m_Project.m_ptTabCandidate[k].SetPoint(npostmp[k],rttmp[k].top);
		}
		int nmax12 = 0,npeak = 0;
		for(int k=0;k<4;k++){
			if(nMaxtmp[k]>nmax12 ){
				nmax12 = nMaxtmp[k];
//				npos1 = npostmp[k];
				npeak = k;
			}
		}
		if(npeak<2){
			npos1 =  npostmp[npeak];
		}
		else{
			npos = npostmp[npeak];
		}

		pView->AddtoInspLog("Change Position");
	}

	//draw Line //m_Project.m_nBottomEdge[i]
	CPoint pt1,pt2;
	if(npos>npos1)pt1.x=npos1,pt2.x=npos;
	else	pt1.x=npos,pt2.x=npos1;
	CRect rttop;
	rttop = rttab2;
	rttop.left = pt1.x;rttop.right = pt2.x;
	pView->m_Display[0].AddArrayRect(rttop,0);
	
	int nWhiteCheck = 0,nBlackCheck=0,nLineWhite=0;
	for(int y=rttop.top;y<rttop.bottom;y++){
		for(int x=rttop.left;x<rttop.right;x++){
			nProjy1[y] += *(fmtop+y*CAMERA_WIDTH+x);
			if(pView->m_pDlgSet->m_FormTab4->m_b2Tab)
				nProjy3[y] += *(pView->m_Project.m_MultiBuf[10]+CAMERA_WIDTH*y+x);
		}
		if(rttop.Width()>0){
			nProjy1[y] /= rttop.Width();
			if(pView->m_pDlgSet->m_FormTab4->m_b2Tab){
				nProjy3[y]/= rttop.Width();
				if(nProjy3[y]>200)nLineWhite++;
			}
		}
		
		if(y>rttop.top){
			nProjy2[y] = (nProjy1[y]+nProjy1[y-1])*0.5;
		}
	}
//	d1[6] = pView->m_Project.GetPrecisionTime();

	nmax11 = 0;npos1=0;
	int nFind = 0;
	for(int y=rttop.bottom-10;y>rttop.top+4;y--){
		//하단에서 올라가면서 커지는데
		if(abs(nProjy2[y]-nProjy2[y-4])>nmax11){
			nmax11 = abs(nProjy2[y]-nProjy2[y-4]);
			npos1 = y;
		}
		if(abs(nProjy2[y]-nProjy2[y-4])>45){
			pt1.y = y;
			pt2.y = y;
			pView->m_Project.m_nTopPos[0] = y;
			pView->m_Project.m_ptLeftTop=pt1;
			pView->m_Project.m_ptRightTop=pt2;
			nFind = 1;
			break;
		}
	}
	if(nFind==0){
		pt1.y = npos1;
		pt2.y = npos1;
		pView->m_Project.m_nTopPos[0] = npos1;
		pView->m_Project.m_ptLeftTop=pt1;
		pView->m_Project.m_ptRightTop=pt2;
	}
	if(npos1<10){
		delete fmtop;
		return 0;
	}
	if(pView->m_pDlgSet->m_FormTab4->m_b2Tab){
		//백색 체크 탭 꺽임. // 17번 영ㅅㅇ에서 체크
		CRect rtCheck1;
		rtCheck1.SetRect(pt1.x,pt1.y-80,pt2.x,pt1.y);
		//200 이상 밝기 체크
		for(int y=rtCheck1.top;y<rtCheck1.bottom;y++){
			for(int x=rtCheck1.left;x<rtCheck1.right;x++){
				if(*(pView->m_Project.m_MultiBuf[10]+CAMERA_WIDTH*y+x)>200 )
					nWhiteCheck++;
				if(*(pView->m_Project.m_MultiBuf[10]+CAMERA_WIDTH*y+x)<20 )
					nBlackCheck++;
			}
		}
		if(nWhiteCheck>3000 || nLineWhite>5)
		{
			rtCheck1.top = rtRange.top;
			for(int y=rtCheck1.top;y<rtCheck1.bottom;y++){
				nProjy2[y]=nProjy1[y] =0;
				for(int x=rtCheck1.left;x<rtCheck1.right;x++){
					nProjy1[y] += *(fmtop+y*CAMERA_WIDTH+x);
				}
				if(rttop.Width()>0)
					nProjy1[y] /= rtCheck1.Width();
				if(y>rtCheck1.top){
					nProjy2[y] = (nProjy1[y]+nProjy1[y-1])*0.5;
				}
			}
			for(int y=rtCheck1.top+2;y<rtCheck1.bottom-4;y++){
				if(abs(nProjy2[y] - nProjy2[y+4]) > 33){
					pt1.y = y+4;
					pt2.y = y+4;
					pView->m_Project.m_nTopPos[0] = y+4;
					pView->m_Project.m_ptLeftTop=pt1;
					pView->m_Project.m_ptRightTop=pt2;
					break;
				}
			}
		}
	}

//	d1[7] = pView->m_Project.GetPrecisionTime();

	pView->m_Project.m_nTopEdge[0] = pt1.x;pView->m_Project.m_nTopEdge[1] = pt2.x;
	pView->m_Project.m_nEdgeDist[0] = pView->m_Project.m_ptCapCenter.x- pView->m_Project.m_nTopEdge[0]; // 음수면 Tab 좌측 경계가 Top Cap 중심보다 오른쪽
	pView->m_Project.m_nEdgeDist[1] = pView->m_Project.m_nTopEdge[1]-pView->m_Project.m_ptCapCenter.x; // 음수면 Tab 우측 경계가 Top Cap 중심보다 왼쪽
	double ddist1 = pView->m_Project.m_nEdgeDist[0]*pView->m_pDlgSet->m_FormTab2->m_dCamScale;
	double ddist2 = pView->m_Project.m_nEdgeDist[1]*pView->m_pDlgSet->m_FormTab2->m_dCamScale;
	pView->m_dTabDist[0] = ddist1;pView->m_dTabDist[1]=ddist2;


//	d1[8] = pView->m_Project.GetPrecisionTime();

	int nnn = 0;
	delete fmtop;
	return 0;
}

#pragma comment (lib, "Iphlpapi.lib")

int CFormTab3::OnBnClickedButtonTest()
{
	CFlyCapture2_EXDlg* pView = (CFlyCapture2_EXDlg *)m_hParent;
	FormTab4* pFormTab4=pView->m_pDlgSet->m_FormTab4;
	DEFECTDATA dataDefect;
	double dUsl = 0.0, dLsl = 0.0;
	double dScale = pView->m_pDlgSet->m_FormTab2->m_dCamScale;
	CPoint ptStart, ptEnd;

	int nCapCenterX = 0;
	int nCapCenterY = 0;
	int nResult=0;
	CRect rtCrop;
	CString strMil;
	pView->m_bHoleBad[0][0]=pView->m_bHoleBad[0][1]=pView->m_bHoleBad[1][0]=pView->m_bHoleBad[1][1] = FALSE;

	for(int i=0; i<20; i++){
		pView->m_bResult[i]=FALSE;
	}
	
	int nminus = pView->m_nMinus;
	pView->m_nMaxThFrame[0]=pView->m_nMaxThFrame[1]=0;

	pView->m_nType = 3;
	pView->m_Display[0].ResetArray();
	pView->m_Project.m_ptCanTop[0].SetPoint(0,0);
	pView->m_Project.m_ptCanTop[1].SetPoint(0,0);
	pView->m_Project.m_ptLeftTop.SetPoint(0,0);
	pView->m_Project.m_ptRightTop.SetPoint(0,0);

	int nCropHalf = NG_CROP_SIZE_X / 2;
	int nth = 150,nEdgeth=70;
	CRect rttab,rttmp;
	int nHisto[256]={0,};
	rttmp.SetRect(10,10,CAMERA_WIDTH-10,CAMERA_HEIGHT-10);

	//top center find //m_Project.m_MultiBuf[m_Project.m_nMultiFrame]
	double d1[10]={0,};
	pView->m_dJudgeTime = d1[0] = pView->m_Project.GetPrecisionTime();
	
	//1번 영상이 계속 문제가 있을 경우 0번영상으로 대체
	CRect rtchek;
	rtchek = pView->m_Display[0].m_rtImgTracker[1];
	rtchek = pView->m_Project.CheckROI(rtchek,CRect(10,10,CAMERA_WIDTH-10,CAMERA_HEIGHT-10));

	//Light
	double dLight = 0;
	pView->m_nLight = 0;
	rtchek = pView->m_Display[0].m_rtImgTracker[6];
	rtchek = pView->m_Project.CheckROI(rtchek, CRect(10, 10, CAMERA_WIDTH - 10, CAMERA_HEIGHT - 10));
	pView->m_nLight = pView->m_Project.AreaAverage2(2, pView->m_Project.m_MultiBuf[pView->m_pDlgSet->m_FormTab2->m_nTopCapCenterFrame], CAMERA_WIDTH, rtchek.left, rtchek.top, rtchek.right, rtchek.bottom, 15, &pView->m_nCanBlackCount, &dLight);

	double dtmp11 = 0;
	int tmp = 1;
	strMil.Format("TotalGrab~Insp%d_%d",tmp,(pView->m_nOKCount+pView->m_nNgCount));

	int ntopcapCheck = pView->m_Project.AreaAverage2(2,pView->m_Project.m_MultiBuf[pView->m_pDlgSet->m_FormTab2->m_nTopCapCenterFrame],CAMERA_WIDTH,rtchek.left,rtchek.top,rtchek.right,rtchek.bottom,15,&pView->m_nCanBlackCount,&dtmp11);
	pView->m_nTopCapCheck = ntopcapCheck;
	rtchek = pView->m_Display[0].m_rtImgTracker[0];
	rtchek = pView->m_Project.CheckROI(rtchek,CRect(10,10,CAMERA_WIDTH-10,CAMERA_HEIGHT-10));
	pView->m_nCanCheckAve = pView->m_Project.AreaAverage2(2,pView->m_Project.m_MultiBuf[pView->m_pDlgSet->m_FormTab2->m_nTopCapCenterFrame],CAMERA_WIDTH,rtchek.left,rtchek.top,rtchek.right,rtchek.bottom,15,&pView->m_nCanBlackCount,&dtmp11);


	rttab = pView->m_Project.CircleFind2(pView->m_Project.m_MultiBuf[pView->m_pDlgSet->m_FormTab2->m_nTopCapCenterFrame]);//-1자동으로 해줌 화면상 1이면 0번 프레임
	d1[1] = pView->m_Project.GetPrecisionTime();

	nCapCenterX = pView->m_Project.m_ptCapCenter.x;
	nCapCenterY = pView->m_Project.m_ptCapCenter.y;
	
	//top can에 대한 판단
	//일정 영역을 벗어나면 ng tracker로 판단....
	
	//트랙커 2번(1번) 밝기 체크 
	//top cap 접힘 ng 처리
	if(pView->m_Project.m_ptCapCenter.y>750 || rttab.Width()==0 || rttab.Height()==0 || pView->m_nTopCapCheck > pView->m_pDlgSet->m_FormTab4->m_nTopCapValue){
		//not cap	
		//pView->LabelResult(0);

		memcpy(pView->m_Display[0].m_pImg->GetImagePtr(),pView->m_Project.m_MultiBuf[8],CAMERA_HEIGHT*CAMERA_WIDTH);
		if(pView->m_nSelectFrame>10)
			memcpy(pView->m_Display[0].m_pImg->GetImagePtr(),pView->m_Project.m_AlphaBuf[pView->m_nSelectFrame-11],CAMERA_HEIGHT*CAMERA_WIDTH);

		if(pView->m_pDlgSet->m_FormTab2->m_bUseSaveCrop){
			rtCrop.SetRect(pView->m_Project.m_ptCapCenter.x-nCropHalf,pView->m_Project.m_ptCapCenter.y-nCropHalf,pView->m_Project.m_ptCapCenter.x+nCropHalf,pView->m_Project.m_ptCapCenter.y+nCropHalf);
			rtCrop = pView->m_Project.CheckROI(rtCrop,CRect(0,0,CAMERA_WIDTH,CAMERA_HEIGHT));
			pView->m_Project.m_Pro.CropImageSelectCopy(pView->m_Project.m_MultiBuf[pView->m_nSelectFrame], pView->m_DisplayNgCrop.m_pImg->GetImagePtr(), CAMERA_WIDTH, CAMERA_HEIGHT, rtCrop, NG_CROP_SIZE_X, NG_CROP_SIZE_Y);
			pView->SetDefectCropInfo(pView->m_Project.m_MultiBuf[7], pView->m_DisplayNgCrop.m_pImg->GetImagePtr(), "NOTCAN", CAMERA_WIDTH, CAMERA_HEIGHT, &dataDefect, CPoint(rtCrop.CenterPoint().x, rtCrop.CenterPoint().y), rtCrop, false);
				//SetDefectCropInfo(fm, fm, fmCropTemp, nScale, nWidth, nHeight, &dataDefect, CPoint(dBlobCenterX, dBlobCenterY), rtIns, false);
		}
		GetLocalTime(&pView->m_timeInspEnd);
		pView->AddtoList("Not Cap",0);
		pView->m_nBadCount[0]++;
		pView->m_nTimeCount[pView->m_nHour][2]++;//not cap
		return NG_NOT_CAP;
	}

	
//////can 위치 찾기
	CRect rtcan,rtcan1;
	rtcan = pView->m_Display[0].m_rtImgTracker[0];
	rtcan = pView->m_Project.CheckROI(rtcan,CRect(10,10,CAMERA_WIDTH-10,CAMERA_HEIGHT-10));
	int ncanleft=0,ncanright=0,ncount1=0,ncount2=0,ncancenter=0,ncancenterY=0;
	int ncanth = 15;
	int nFrame = pView->m_pDlgSet->m_FormTab2->m_nCanPosFrame;//1
	for(int y=rtcan.top;y<rtcan.bottom;y++){
		for(int x=rtcan.left;x<pView->m_Project.m_ptCapCenter.x;x++){
			if(*(pView->m_Project.m_MultiBuf[nFrame]+CAMERA_WIDTH*y+x) - *(pView->m_Project.m_MultiBuf[nFrame]+CAMERA_WIDTH*y+(x+4)) >ncanth){
				ncanleft += x+4;
				ncount1++;
				break;
			}
		}
		for(int x=rtcan.right;x>pView->m_Project.m_ptCapCenter.x;x--){
			if(*(pView->m_Project.m_MultiBuf[nFrame]+CAMERA_WIDTH*y+x) - *(pView->m_Project.m_MultiBuf[nFrame]+CAMERA_WIDTH*y+(x-4)) >ncanth){
				ncanright += x-4;
				ncount2++;
				break;
			}
		}
	}
	if(ncount1>0 && ncount2>0){
		ncanleft/= ncount1;
		ncanright/= ncount2;
		ncancenter = (ncanleft+ncanright)*0.5;
		int nprocantop[CAMERA_WIDTH]={0,},nblackcnt=0;
		int nprocantop1[CAMERA_WIDTH]={0,},nblackcnt1=0;
		rtcan.SetRect(ncanleft+10,pView->m_Project.m_ptCapCenter.y,ncanleft+80,CAMERA_HEIGHT-10);
		rtcan1.SetRect(ncanright-80,pView->m_Project.m_ptCapCenter.y,ncanright-10,CAMERA_HEIGHT-10);

		rtcan = pView->m_Project.CheckROI(rtcan,CRect(10,10,CAMERA_WIDTH-10,CAMERA_HEIGHT-10));
		rtcan1 = pView->m_Project.CheckROI(rtcan1,CRect(10,10,CAMERA_WIDTH-10,CAMERA_HEIGHT-10));

//		pView->m_Display[0].AddArrayRect(rtcan);
		int nMaxy1 = 0,nmaxv1=0,nMaxy2 = 0,nmaxv2=0;
		for(int y=rtcan.bottom;y>rtcan.top;y--){
			nblackcnt=0;
			for(int x=rtcan.left;x<rtcan.right;x++){
				nprocantop[y]+=*(pView->m_Project.m_MultiBuf[nFrame]+CAMERA_WIDTH*y+(x));
			}
			for(int x=rtcan1.left;x<rtcan1.right;x++){
				nprocantop1[y]+=*(pView->m_Project.m_MultiBuf[nFrame]+CAMERA_WIDTH*y+(x));
			}
			nprocantop[y] /= rtcan.Width();
			nprocantop1[y] /= rtcan1.Width();
			if(y<rtcan.bottom-10 && nmaxv1<nprocantop[y]-nprocantop[y+8] && y>rtcan.top+10){
				nmaxv1 = nprocantop[y]-nprocantop[y+8];
				nMaxy1 = y+8;

			}
			if(y<rtcan1.bottom-10 && nmaxv2<nprocantop1[y]-nprocantop1[y+8] && y>rtcan1.top+10){
				nmaxv2 = nprocantop1[y]-nprocantop1[y+8];
				nMaxy2 = y+8;

			}
		}
		pView->m_Project.m_ptCanTop[0].SetPoint(ncanleft,nMaxy1);
		pView->m_Project.m_ptCanTop[1].SetPoint(ncanright,nMaxy2);
		ncancenterY = (nMaxy1+nMaxy2)*0.5;
	}
	else{
		/*if(pView->m_pDlgSet->m_FormTab2->m_bUseSaveCrop){
			rtCrop.SetRect(pView->m_Project.m_ptCapCenter.x-nCropHalf,pView->m_Project.m_ptCapCenter.y-nCropHalf,pView->m_Project.m_ptCapCenter.x+nCropHalf,pView->m_Project.m_ptCapCenter.y+nCropHalf);
			rtCrop = pView->m_Project.CheckROI(rtCrop,CRect(0,0,CAMERA_WIDTH,CAMERA_HEIGHT));
			pView->m_Project.m_Pro.CropImageSelectCopy(pView->m_Project.m_MultiBuf[pView->m_nSelectFrame], pView->m_DisplayNgCrop.m_pImg->GetImagePtr(), CAMERA_WIDTH, CAMERA_HEIGHT, rtCrop, NG_CROP_SIZE_X, NG_CROP_SIZE_Y);
			pView->SetDefectCropInfo(pView->m_Project.m_MultiBuf[7], pView->m_DisplayNgCrop.m_pImg->GetImagePtr(), "NOTCAP", CAMERA_WIDTH, CAMERA_HEIGHT, &dataDefect, CPoint(rtCrop.CenterPoint().x, rtCrop.CenterPoint().y), rtCrop, false);
		}*/
		return NG_NOT_CAN;
	}

	//can cap 거리
	double dValue = 0.0;
	ptEnd.SetPoint( (ncanleft+ncanright) *0.5 , ncancenterY);
	pView->m_dCapCanDistX = dValue = (pView->m_Project.m_ptCapCenter.x - ncancenter) * dScale;
	if (dValue > pFormTab4->m_dCapCandistX){
		pView->m_Project.m_nTapOkNg = 1;
		pView->m_nBadCount[4]++;
		pView->DisplayNgCrop(0, 0, _T("Can-Cap X Dist"), 4, CPoint(pView->m_Project.m_ptCapCenter.x, pView->m_Project.m_ptCapCenter.y));
		pView->SetDimensionData(1, FALSE, dValue, CPoint(nCapCenterX, nCapCenterY), CPoint(ptEnd.x, ptEnd.y), TRUE);
		return NG_CAN_CAP_DIST_X;
	}
	pView->SetDimensionData(1, FALSE, dValue, CPoint(nCapCenterX, nCapCenterY), CPoint(ptEnd.x, ptEnd.y), FALSE);

	pView->m_dCapCanDistY = dValue = abs((pView->m_Project.m_ptCapCenter.y - ncancenterY) * dScale);
	if (dValue > pFormTab4->m_dCenterOffset || dValue < pFormTab4->m_dCenterOffset2){
		pView->m_Project.m_nTapOkNg = 1;
		pView->m_nBadCount[4]++;
		pView->DisplayNgCrop(0, 0, _T("Can-Cap Y Dist"), 4, CPoint(nCapCenterX, nCapCenterY));
		pView->SetDimensionData(2, FALSE, dValue, CPoint(nCapCenterX, nCapCenterY), CPoint(ptEnd.x, ptEnd.y), TRUE);
		return NG_CAN_CAP_DIST_Y;
	}
	pView->SetDimensionData(2, FALSE, dValue, CPoint(nCapCenterX, nCapCenterY), CPoint(ptEnd.x, ptEnd.y), FALSE);

//	pView->m_Project.m_nTopPos[0] = nblackcnt;
//용접 위치 따라가게
	CRect rttmp11,rttmp1,rttmp2,rttmp12;
	rttmp11 = pView->m_Display[0].m_rtImgTracker[5];
	int nw1 = rttmp11.Width();//8의 배수로 만들어야 함.
	int nh1 = rttmp11.Height();//8의 배수로 만들어야 함.
	nw1 -= nw1%8;nh1 -= nh1%8;
	rttmp1.SetRect(pView->m_Project.m_ptCapCenter.x-pView->m_Project.m_nRegWeldPos[0],pView->m_Project.m_ptCapCenter.y+pView->m_Project.m_nRegWeldPos[1],pView->m_Project.m_ptCapCenter.x-pView->m_Project.m_nRegWeldPos[0]+nw1,pView->m_Project.m_ptCapCenter.y+pView->m_Project.m_nRegWeldPos[1]+nh1);	
	rttmp1 = pView->m_Project.CheckROI(rttmp1,CRect(10,10,CAMERA_WIDTH-10,CAMERA_HEIGHT-10));

	if(pView->m_pDlgSet->m_FormTab4->m_bMidWeld){
		rttmp12 = pView->m_Display[0].m_rtImgTracker[6];
		int nw1 = rttmp12.Width();//8의 배수로 만들어야 함.
		int nh1 = rttmp12.Height();//8의 배수로 만들어야 함.
		rttmp2.SetRect(pView->m_Project.m_ptCapCenter.x-pView->m_Project.m_nRegWeldPos1[0],pView->m_Project.m_ptCapCenter.y+pView->m_Project.m_nRegWeldPos1[1],pView->m_Project.m_ptCapCenter.x-pView->m_Project.m_nRegWeldPos1[0]+nw1,pView->m_Project.m_ptCapCenter.y+pView->m_Project.m_nRegWeldPos1[1]+nh1);	
		rttmp2 = pView->m_Project.CheckROI(rttmp2,CRect(10,10,CAMERA_WIDTH-10,CAMERA_HEIGHT-10));
	}
	else
		rttmp2.SetRect(0,0,0,0);
	pView->m_Project.m_rtMidWeldPos = rttmp2;
	//	pView->m_Display[0].AddArrayRect(rttmp1);
	pView->m_Project.m_rtWeldPos = rttmp1;

	d1[2] = pView->m_Project.GetPrecisionTime();
	//check rect
	rttab = pView->m_Project.CheckROI(rttab, CRect(20, 20, CAMERA_WIDTH - 20, CAMERA_HEIGHT - 20));

	rttmp = rttab;
	if (m_bFullAlphaImage)
		rttmp.SetRect(10, 10, CAMERA_WIDTH - 10, CAMERA_HEIGHT - 10);
	else if (pView->m_pDlgSet->m_FormTab4->m_bTwoTab == 0) {
		rttmp.SetRect(pView->m_Project.m_ptCapCenter.x - 200, pView->m_Project.m_ptCapCenter.y - 200, pView->m_Project.m_ptCapCenter.x + 200, pView->m_Project.m_ptCapCenter.y + 200);
	}
	else
		rttmp.SetRect(pView->m_Project.m_ptCapCenter.x - 200, pView->m_Project.m_ptCapCenter.y - 300, pView->m_Project.m_ptCapCenter.x + 200, pView->m_Project.m_ptCapCenter.y + 200);
	rttmp = pView->m_Project.CheckROI(rttmp, CRect(20, 20, CAMERA_WIDTH - 20, CAMERA_HEIGHT - 20));

	fnYSum1(pView->m_pDlgSet->m_FormTab2->m_nTabTopSelectFrame[0], pView->m_pDlgSet->m_FormTab2->m_nTabTopSelectFrame[1], 0, rttmp, 0, 0, 2, 1);//top

	if (pView->m_pDlgSet->m_FormTab2->m_bUseTabTopFrame) {
		fnXSum1(pView->m_pDlgSet->m_FormTab2->m_nStartFrameL-1, pView->m_pDlgSet->m_FormTab2->m_nEndFrameL-1, 1, rttmp, 0, 0, 2);//left
		fnXSum2(pView->m_pDlgSet->m_FormTab2->m_nStartFrameR-1, pView->m_pDlgSet->m_FormTab2->m_nEndFrameR-1, 2, rttmp, 0, 0, 1);//right
	}
	else {
		if (!m_bCheckType) {
			fnXSum1(3 - nminus, 5 - nminus, 1, rttmp, 0, 0, 2);//left
			fnXSum2(6 - nminus, 8 - nminus, 2, rttmp, 0, 0, 2);//right
		}
		else {
			if (pView->m_nPcName == 2)//mid
			{	//fnXSum1 프레임 범위 넓힘
				fnXSum1(3 - nminus, 5 - nminus, 1, rttmp, 0, 0, 2);//left
				fnXSum2(6 - nminus, 8 - nminus, 2, rttmp, 0, 0, 2);//right
			}
			else if (pView->m_nPcName == 3) {
				fnXSum1(1 - nminus, 5 - nminus, 1, rttmp, 0, 0, 1);//left
				fnXSum2(6 - nminus, 9 - nminus, 2, rttmp, 0, 0, 1);//right
			}
			else {//left
				fnXSum1(3 - nminus, 5 - nminus, 1, rttmp, 0, 0, 1);//left
				fnXSum2(6 - nminus, 8 - nminus, 2, rttmp, 0, 0, 2);//right
			}
		}
	}
	//TabTop 다시
	if (pView->m_pDlgSet->m_FormTab2->m_bTabReFind) {
		int nX11 = 0, nX12 = 0;
		TabTopRefind(pView->m_Project.m_AlphaBuf[0], pView->m_Project.m_ptLeftTop, pView->m_Project.m_ptRightTop, 40, 40, &nX11, &nX12);
		pView->m_Project.m_ptLeftTop.y = nX11;
		pView->m_Project.m_ptRightTop.y = nX12;
		if (abs(pView->m_Project.m_ptCapCenter.y - nX11) > abs(pView->m_Project.m_ptCapCenter.y - nX12))
			pView->m_Project.m_nTopPos[0] = nX12;
		else
			pView->m_Project.m_nTopPos[0] = nX11;
	}
	d1[3] = pView->m_Project.GetPrecisionTime();

	//CAP Center-TAB Top
	dValue = (nCapCenterY - pView->m_Project.m_nTopPos[0]) * dScale;
	dUsl = pFormTab4->m_dTapTolY1;
	dLsl = pFormTab4->m_dTapTolY2 * -1;
	if (dValue > dUsl || dLsl > dValue) {
		pView->m_Project.m_nTapOkNg = 1;
		pView->m_nBadCount[2]++;
		pView->DisplayNgCrop(0, 0, _T("TAB TOP"), 1, CPoint(nCapCenterX, nCapCenterY));
		pView->SetDimensionData(0, FALSE, dValue, CPoint(pView->m_Project.m_ptLeftTop.x, pView->m_Project.m_ptRightTop.y), CPoint(pView->m_Project.m_ptLeftTop.x, pView->m_Project.m_ptRightTop.y), TRUE);
		return NG_TAP_POS;
	}


	//Tab Width
	pView->m_Project.m_dTabTopLen = dValue = (pView->m_Project.m_ptTopEdge[1].x - pView->m_Project.m_ptTopEdge[0].x) * dScale;//dlen;
	/*dUsl = pFormTab4->m_dTapSize;
	if(dValue > dUsl){
		pView->SetDimensionData(2, FALSE, dValue, CPoint(pView->m_Project.m_ptLeftTop.x, pView->m_Project.m_ptRightTop.y), CPoint(pView->m_Project.m_ptLeftTop.x, pView->m_Project.m_ptRightTop.y), TRUE);
		return NG_TAP_WIDTH;
	}
	pView->SetDimensionData(2, FALSE, dValue, CPoint(pView->m_Project.m_ptLeftTop.x, pView->m_Project.m_ptRightTop.y), CPoint(pView->m_Project.m_ptLeftTop.x, pView->m_Project.m_ptRightTop.y), FALSE);
*/

	double dlen = (pView->m_Project.m_ptRightTop.x - pView->m_Project.m_ptLeftTop.x)* dScale;
	double dlen1 = fabs(pView->m_pDlgSet->m_FormTab4->m_dTapSize - fabs((pView->m_Project.m_ptRightTop.x - pView->m_Project.m_ptLeftTop.x)* dScale));
	double dlen2 = fabs(pView->m_pDlgSet->m_FormTab4->m_dTapSize - fabs((pView->m_Project.m_ptTopEdge[1].x - pView->m_Project.m_ptTopEdge[0].x)* dScale));
	CPoint ptOldX = pView->m_Project.m_ptLeftTop;

	if (dlen1 > dlen2) {
		pView->m_Project.m_ptLeftTop = pView->m_Project.m_ptTopEdge[0];
		pView->m_Project.m_ptRightTop = pView->m_Project.m_ptTopEdge[1];
	}

	//cap~각 tab edge 거리.
	pView->m_Project.m_nEdgeDist[0] = pView->m_Project.m_ptCapCenter.x - pView->m_Project.m_ptLeftTop.x;
	pView->m_Project.m_nEdgeDist[1] = abs(pView->m_Project.m_ptCapCenter.x - pView->m_Project.m_ptRightTop.x);
	double ddist1 = pView->m_Project.m_nEdgeDist[0] * dScale;
	double ddist2 = pView->m_Project.m_nEdgeDist[1] * dScale;

	int nLeftRight = 2;//1 left 2 right 0 
	if (pView->m_pDlgSet->m_FormTab2->m_bUseLeft)nLeftRight = 1;
	else if (pView->m_pDlgSet->m_FormTab2->m_bUseRight)nLeftRight = 2;
	else nLeftRight = 0;
	if (ddist1 + ddist2 > pView->m_pDlgSet->m_FormTab4->m_dTapSize + 0.3 || ddist1 + ddist2 < pView->m_pDlgSet->m_FormTab4->m_dTapSize + 0.3) {//KDM 0102674-5107 김병규 과장
		//우선 레프트만 민다.
		if (pView->m_Project.m_ptLeftTop.x < ptOldX.x) {
			pView->m_Project.m_ptLeftTop = pView->m_Project.m_ptTopEdge[0] = ptOldX;
			pView->m_Project.m_nEdgeDist[0] = pView->m_Project.m_ptCapCenter.x - pView->m_Project.m_ptLeftTop.x;
			ddist1 = pView->m_Project.m_nEdgeDist[0] * dScale;
		}
		if (nLeftRight == 2 && (ddist1 + ddist2 > pView->m_pDlgSet->m_FormTab4->m_dTapSize + 0.3 || ddist1 + ddist2 < pView->m_pDlgSet->m_FormTab4->m_dTapSize + 0.3)) {//right 우선
			double dleft1 = pView->m_Project.m_ptRightTop.x - (pView->m_pDlgSet->m_FormTab4->m_dTapSize / pView->m_pDlgSet->m_FormTab2->m_dCamScale);
			pView->m_Project.m_ptLeftTop.x = pView->m_Project.m_ptTopEdge[0].x = dleft1;
			pView->m_Project.m_nEdgeDist[0] = pView->m_Project.m_ptCapCenter.x - pView->m_Project.m_ptLeftTop.x;
			ddist1 = pView->m_Project.m_nEdgeDist[0] * dScale;
		}
		if (nLeftRight == 1 && (ddist1 + ddist2 > pView->m_pDlgSet->m_FormTab4->m_dTapSize + 0.3 || ddist1 + ddist2 < pView->m_pDlgSet->m_FormTab4->m_dTapSize + 0.3)) {//left 우선
			double dright1 = pView->m_Project.m_ptLeftTop.x + (pView->m_pDlgSet->m_FormTab4->m_dTapSize / pView->m_pDlgSet->m_FormTab2->m_dCamScale);
			pView->m_Project.m_ptRightTop.x = pView->m_Project.m_ptTopEdge[1].x = dright1;
			pView->m_Project.m_nEdgeDist[1] = pView->m_Project.m_ptRightTop.x - pView->m_Project.m_ptCapCenter.x;
			ddist2 = pView->m_Project.m_nEdgeDist[1] * dScale;
		}
	}

	//CAP Center-TAB Edge L
	pView->m_dTabDist[0] = dValue = ddist1;
	dUsl = pFormTab4->m_dTabTol;
	if (dValue > dUsl) {
		pView->m_Project.m_nTapOkNg = 1;
		pView->m_nBadCount[4]++;
		pView->DisplayNgCrop(0, 0, _T("Tab-Cap L"), 1, CPoint(nCapCenterX, nCapCenterY));
		//pView->SetDimensionData(2, FALSE, dValue, CPoint(pView->m_Project.m_ptLeftTop.x, pView->m_Project.m_ptRightTop.y), CPoint(pView->m_Project.m_ptLeftTop.x, pView->m_Project.m_ptRightTop.y), TRUE);
		return NG_CAP_TAB_DIST_L;
	}
	//pView->SetDimensionData(2, FALSE, dValue, CPoint(pView->m_Project.m_ptLeftTop.x, pView->m_Project.m_ptRightTop.y), CPoint(pView->m_Project.m_ptLeftTop.x, pView->m_Project.m_ptRightTop.y), FALSE);

	//CAP Center-TAB Edge R
	pView->m_dTabDist[1] = dValue = ddist2;
	dUsl = pFormTab4->m_dTabTol1;
	if (dValue > dUsl) {
		pView->m_Project.m_nTapOkNg = 1;
		pView->m_nBadCount[4]++;
		pView->DisplayNgCrop(0, 0, _T("Tab-Cap R"), 1, CPoint(nCapCenterX, nCapCenterY));
		//pView->SetDimensionData(2, FALSE, dValue, CPoint(pView->m_Project.m_ptLeftTop.x, pView->m_Project.m_ptRightTop.y), CPoint(pView->m_Project.m_ptLeftTop.x, pView->m_Project.m_ptRightTop.y), TRUE);
		return NG_CAP_TAB_DIST_R;
	}
	//pView->SetDimensionData(2, FALSE, dValue, CPoint(pView->m_Project.m_ptLeftTop.x, pView->m_Project.m_ptRightTop.y), CPoint(pView->m_Project.m_ptLeftTop.x, pView->m_Project.m_ptRightTop.y), FALSE);

	//CAP Pos Y 
	dValue = CAMERA_HEIGHT - 200;
	if (pView->m_Project.m_ptCapCenter.y > dValue) {
		pView->m_Project.m_nTapOkNg = 1;
		pView->m_nBadCount[1]++;
		pView->DisplayNgCrop(0, 0, _T("CAP POS Y"), 1, CPoint(nCapCenterX, nCapCenterY));
		return NG_CAP_POS;
	}

	//CAP Reverse
	dValue = pView->m_nTopCapV;
	dUsl = pFormTab4->m_nCapReverseV;
	if (pFormTab4->m_nCapReverseV && dValue > dUsl) {
		pView->m_Project.m_nTapOkNg = 1;
		pView->m_nBadCount[1]++;
		pView->DisplayNgCrop(0, 0, _T("CAP Reverse"), 1, CPoint(nCapCenterX, nCapCenterY));
		return NG_CAP_REVERSE;
	}

	//TAB Edge Y Gap(m_bTabCornerDist)
	dValue = abs(pView->m_Project.m_ptLeftTop.y - pView->m_Project.m_ptRightTop.y);
	dUsl = pFormTab4->m_nTabCornerDist;
	if (pFormTab4->m_bTabCornerDist && dValue > dUsl) {
		pView->m_Project.m_nTapOkNg = 1;
		pView->m_nBadCount[2]++;
		pView->DisplayNgCrop(0, 0, _T("TAB Edge Gap"), 1, CPoint(nCapCenterX, nCapCenterY));
		return NG_CAP_REVERSE;
	}

	//Alpha 0,1,2번으로 3번 만듬.
	if (pView->m_Project.m_ptLeftTop.x)
		fnImageSummary1(0, 0, rttmp);

	d1[4] = pView->m_Project.GetPrecisionTime();

	//여기까지 TAB, CAP 검사-jb
	pView->m_Project.m_nTapOkNg = RET_OK;

	////탭에 대한 판단
	int ntapresult = 1;// pView->judgeTap();

	//용접검사
	pView->m_Project.m_nTopEdge[0] = pView->m_Project.m_ptLeftTop.x;
	pView->m_Project.m_nTopEdge[1] = pView->m_Project.m_ptRightTop.x;
	pView->m_dData[18] = pView->m_Project.m_ptRightTop.y - pView->m_Project.m_ptLeftTop.y;//220520 csv 파일에 저장해야할 데이터 추가 m_nTab_Gap_Y
	
	int nWeldPOsFind = 0;	

	//WeldPosRect(1-nminus,2-nminus,3,pView->m_Project.m_rtWeldPos,0);// 용접디스플레이 주석
	//Welding Sum Image //m_nWeldInTabpos
	if(pView->m_pDlgSet->m_FormTab2->m_bWeldInTab){
		//2동 요청 추가 용접 탭내 용접 위치 찾기 2동은 11번 영상이 가장 밝음
		CRect rtweldintab;//용접 후보영역 바텀에서 탭 끝까지 영역 설정
		rtweldintab.SetRect(pView->m_Project.m_ptLeftTop.x,pView->m_Project.m_ptLeftTop.y+20,pView->m_Project.m_ptRightTop.x,pView->m_Project.m_rtWeldPos.bottom-10);
		rtweldintab = pView->m_Project.CheckROI(rtweldintab,CRect(100,100,CAMERA_WIDTH-100,CAMERA_HEIGHT-100));
		pView->m_Display[0].AddArrayRect(rtweldintab);
//  		if(pView->m_nMachine==2 )//중국만 이거 씀 WOO 200922 용접 다시 찾는기
//  			pView->m_ptWeldCenterPos= FindWeldInTab(pView->m_Project.m_MultiBuf[pView->m_pDlgSet->m_FormTab2->m_nWeldInTabpos],rtweldintab,pView->m_Project.m_rtWeldPos);
	}
	int nweldsumx=0,nweldsumy=0,nweldnct = 0;

	//용접 센터 x,y
	for(int i=0;i<4;i++){
		if(pView->m_Project.m_nWeldMinMax[0] == pView->m_Project.m_ptWeldPos[i].x)		continue;
		if(pView->m_Project.m_nWeldMinMax[1] == pView->m_Project.m_ptWeldPos[i].x)		continue;
		nweldsumx += pView->m_Project.m_ptWeldPos[i].x;
		nweldsumy += pView->m_Project.m_ptWeldPos[i].y;
		nweldnct++;
	}
	int ngap11 = 8;
	int nweldsize = pView->m_pDlgSet->m_FormTab4->m_nWeldingSize;
	int nweldcx =0;
	int nweldcy =0;

	if(nweldnct>=1){
		nweldsumx /= nweldnct;
		nweldsumy /= nweldnct;
		pView->m_Display[0].AddArrayPoint(CPoint(nweldsumx,nweldsumy),1);
		int ngap11 = pView->m_pDlgSet->m_FormTab2->m_nWeldLeftOffset;
		nweldsize = pView->m_pDlgSet->m_FormTab4->m_nWeldingSize;
		nweldcx = nweldsumx+ pView->m_Project.m_rtWeldPos.left;
		nweldcy = nweldsumy+ pView->m_Project.m_rtWeldPos.top;
		if(pView->m_pDlgSet->m_FormTab4->m_bLineWelding ||pView->m_pDlgSet->m_FormTab4->m_b2LineWelding){
			pView->m_Project.m_rtWeldRect[0].SetRect(nweldcx-nweldsize+ngap11,nweldcy-nweldsize*0.5,nweldcx+nweldsize+ngap11,nweldcy+nweldsize*0.5);
			pView->m_Project.m_rtWeldRect[1]=pView->m_Project.m_rtWeldRect[0];
		}
		else{
			pView->m_Project.m_rtWeldRect[0].SetRect(nweldcx-nweldsize+ngap11,nweldcy-nweldsize*0.5,nweldcx+ngap11,nweldcy+nweldsize*0.5);
			pView->m_Project.m_rtWeldRect[1].SetRect(nweldcx+ngap11,nweldcy-nweldsize*0.5,nweldcx+nweldsize+ngap11,nweldcy+nweldsize*0.5);
		}
	}
	if(pView->m_pDlgSet->m_FormTab2->m_bWeldCenterPos==FALSE){
		nweldcx = pView->m_Project.m_ptWeldPos[0].x+ pView->m_Project.m_rtWeldPos.left;
		nweldcy = pView->m_Project.m_ptWeldPos[0].y+ pView->m_Project.m_rtWeldPos.top;
		pView->m_Project.m_rtWeldRect[0].SetRect(nweldcx-nweldsize+ngap11,nweldcy-nweldsize*0.5,nweldcx+nweldsize+ngap11,nweldcy+nweldsize*0.5);
		pView->m_Project.m_rtWeldRect[1]=pView->m_Project.m_rtWeldRect[0];
	}
	pView->m_Project.m_rtWeldRect[0] = pView->m_Project.CheckROI(pView->m_Project.m_rtWeldRect[0],CRect(100,100,CAMERA_WIDTH-100,CAMERA_HEIGHT-100));
	pView->m_Project.m_rtWeldRect[10] = pView->m_Project.CheckROI(pView->m_Project.m_rtWeldRect[1],CRect(100,100,CAMERA_WIDTH-100,CAMERA_HEIGHT-100));
	pView->m_nSelectFrame = pView->m_pDlgSet->m_FormTab2->m_nTopCapCenterFrame;

	if(pFormTab4->m_bTabExistChk){
		CRect rtTabFold;
		int nBright=0,nBright2=0,nBright3=0;
		int nTabCount = 0;
		int nEdgeTH2=pView->m_pDlgSet->m_FormTab4->m_nTabExistTH;
		rtTabFold.SetRect(pView->m_Project.m_ptCapCenter.x-pView->m_pDlgSet->m_FormTab4->m_nTabExistX1,pView->m_Project.m_ptCapCenter.y-pView->m_pDlgSet->m_FormTab4->m_nTabExistY1,pView->m_Project.m_ptCapCenter.x+pView->m_pDlgSet->m_FormTab4->m_nTabExistX2,pView->m_Project.m_ptCapCenter.y+pView->m_pDlgSet->m_FormTab4->m_nTabExistY2);
		rtTabFold = pView->m_Project.CheckROI(rtTabFold,CRect(0,0,CAMERA_WIDTH,CAMERA_HEIGHT));
		int nRectWidth=rtTabFold.Width();
		int nRectHeight=rtTabFold.Height();
		int nYCnt[100]={0,};

		for (int x=rtTabFold.left+10; x<rtTabFold.right-10; x++)
		{
			for (int y=rtTabFold.top+2; y<rtTabFold.bottom-2; y++)
			{
				if(*(pView->m_Project.m_AlphaBuf[0]+y*CAMERA_WIDTH+x)>nEdgeTH2)
					nTabCount++;
			}
		}
		dValue = pView->m_nTabEdgeCnt = nTabCount;
		dUsl = pFormTab4->m_nTabExist;
		if(dValue >= dUsl){
			pView->m_Project.m_nTapOkNg = 1;
			pView->m_nBadCount[2]++;
			pView->DisplayNgCrop(0, 0, _T("NOT TAB"), 1, CPoint(nCapCenterX, nCapCenterY));
			return NG_NOT_TAB;
		}
	}

	//용접 상태
	//2 3번 이미지 체크 //m_dLeftArea //m_dRightArea
	// 	int nWth = pView->m_pDlgSet->m_FormTab4->m_dWhiteCut;
	// 	//이미지 체크 1 / 2 번 이ㅣㅁ지가 밝은지를 본다
	// 	double dstd1=0,dvar1=0;int nmin1=0,nmax1=0;
	// 	double dave11 = pView->m_Project.GetRoiInfo(pView->m_Project.m_MultiBuf[1],CAMERA_WIDTH,pView->m_Project.m_rtWeldRect[0], 1, 1,&dstd1,&nmin1,&nmax1,&dvar1,0);
// 	double dave12 = pView->m_Project.GetRoiInfo(pView->m_Project.m_MultiBuf[2],CAMERA_WIDTH,pView->m_Project.m_rtWeldRect[0], 1, 1,&dstd1,&nmin1,&nmax1,&dvar1,0);
// 
// 	int nstart1 = 1,nend1=2;
// 	if(dave11>pView->m_pDlgSet->m_FormTab4->m_nTopCapWhiteCut)nstart1=2;
// 	if(dave12>pView->m_pDlgSet->m_FormTab4->m_nTopCapWhiteCut)nend1 = 1;
// 	int nholeresult = WeldCheck(nstart1,nend1,CAMERA_WIDTH,CAMERA_HEIGHT,pView->m_Project.m_rtWeldRect[0],pView->m_Project.m_rtWeldRect[1],nWth,pView->m_pDlgSet->m_FormTab4->m_dLeftArea,pView->m_pDlgSet->m_FormTab4->m_dRightArea);
	//0 ok 1leftng 2 right ng 3 allng
	int nholeresult =  0;
	BOOL bWeldPos = FALSE;
	int nwth1 = pView->m_pDlgSet->m_FormTab4->m_dWhiteCut;
	int bth = pView->m_pDlgSet->m_FormTab4->m_nHoleTh;
	double dwhitecutl = pView->m_pDlgSet->m_FormTab4->m_dLeftArea1;//white
	double dwhitecutr = pView->m_pDlgSet->m_FormTab4->m_dRightArea1;//white
	double dblackcutl = pView->m_pDlgSet->m_FormTab4->m_dLeftArea;//black
	double dblackcutr = pView->m_pDlgSet->m_FormTab4->m_dRightArea;//black
	//	int nholeresult = WeldCheckNew(1-nminus,2-nminus,CAMERA_WIDTH,CAMERA_HEIGHT,pView->m_Project.m_rtWeldRect[0],pView->m_Project.m_rtWeldRect[1],nwth1,dwhitecutl,dwhitecutr,bth,dblackcutl,dblackcutr);
	nholeresult = WeldCheckNew(rttab,-nminus,2-nminus,CAMERA_WIDTH,CAMERA_HEIGHT,pView->m_Project.m_rtWeldRect[0],pView->m_Project.m_rtWeldRect[1],nwth1,dwhitecutl,dwhitecutr,bth,dblackcutl,dblackcutr);
	//
	//weld 구분

	//

	if(nholeresult != RET_OK)
	{//ng
		pView->m_nBadCount[3]++;
		pView->DisplayNgCrop(0, 0, _T(pView->m_strBadInfo1[1]), 1, CPoint(nCapCenterX, nCapCenterY));
		return nholeresult;//NG_WEAK_WELD;
	}

	//if(!nholeresult == RET_OK)
	//{//ng
	//	CTime ttime = CTime::GetCurrentTime();
	//	CString kk2 = ttime.Format("%H:%M:%S");
	//	pView->m_Project.m_nHoleOkNg = 1;
	//	pView->m_IO.m_nBadResult1 = 2; // hole ng signal
	//	pView->m_Project.m_rtHoleRect.SetRect(pView->m_Project.m_ptCapCenter.x-150,pView->m_Project.m_ptCapCenter.y-96,pView->m_Project.m_ptCapCenter.x+150,pView->m_Project.m_ptCapCenter.y+96);
	//	//pView->m_strBadInfo1[1] = "Hole";
	//	pView->m_strBadInfo1[0].Format("%s (%d)",kk2,pView->m_nOKCount+pView->m_nNgCount);
	//	pView->ImageShiftBad();

	//	pView->DisplayNgCrop(0, 0, _T(pView->m_strBadInfo1[1]), 1, CPoint(nCapCenterX, nCapCenterY));

	//	return NG_WEAK_WELD;
	//}


	double dMidWeldV = -1;
	int nMidCount = 0;
	double dmidstd = 0;
	if(pView->m_pDlgSet->m_FormTab4->m_bMidWeld){
		//용접 부위만 검사  두가지 방법 사용
		//1 평균 밝기 2. 어두운 카운트 3. 에지 개수
		CRect rt3 = pView->m_Project.m_rtMidWeldPos;// = rttmp2; //m_nTabTopSelectFrame[0]
		int nfno = pView->m_pDlgSet->m_FormTab2->m_nTabTopSelectFrame[0];
		int nmidV = 20;pView->m_pDlgSet->m_FormTab4->m_dMidWeld;
		// 에지값만 구한다.
		int mEdgeSum=0,nEdgecnt=0;
		for(int y=rt3.top;y<rt3.bottom;y++){
			for(int x=rt3.left;x<rt3.right;x++){
				mEdgeSum += abs(*(pView->m_Project.m_MultiBuf[nfno]+y*CAMERA_WIDTH+x) - *(pView->m_Project.m_MultiBuf[nfno]+y*CAMERA_WIDTH+x+4))*4;
				nEdgecnt++;
			}
		}
		if(nEdgecnt>0)
			mEdgeSum /= nEdgecnt;
		else  mEdgeSum = 0;
		dMidWeldV= mEdgeSum; 
		if(pView->m_pDlgSet->m_FormTab4->m_bMidWeld && dMidWeldV>0){
			if(dMidWeldV<pView->m_pDlgSet->m_FormTab4->m_dMidWeld )nholeresult = 10;
		}
	}
	pView->m_dMidWeldV = dMidWeldV;

	CRect rtcheck1;
	int nt1 = pView->m_Project.m_ptLeftTop.y+10;
	if(nt1 < pView->m_Project.m_ptRightTop.y+10)
		nt1 = pView->m_Project.m_ptRightTop.y+10;
	//			rtcheck1.SetRect(pView->m_Project.m_ptLeftTop.x+10,nt1,pView->m_Project.m_ptRightTop.x-10,nt1+50);//여기 수정해야함..

	// 			if (abs(pView->m_pDlgSet->m_FormTab4->m_nTabEdgeY1)>abs(pView->m_pDlgSet->m_FormTab4->m_nTabEdgeY2))
	// 			{
	// 				rtcheck1.SetRect(pView->m_Project.m_ptCapCenter.x-pView->m_pDlgSet->m_FormTab4->m_nTabEdgeX2,pView->m_Project.m_ptCapCenter.y-pView->m_pDlgSet->m_FormTab4->m_nTabEdgeY2,pView->m_Project.m_ptCapCenter.x+pView->m_pDlgSet->m_FormTab4->m_nTabEdgeX1,pView->m_Project.m_ptCapCenter.y+pView->m_pDlgSet->m_FormTab4->m_nTabEdgeY1);
	// 
	// 			}
	// 			else if (abs(pView->m_pDlgSet->m_FormTab4->m_nTabEdgeY1)<abs(pView->m_pDlgSet->m_FormTab4->m_nTabEdgeY2))
	// 			{
	// 				rtcheck1.SetRect(pView->m_Project.m_ptCapCenter.x-pView->m_pDlgSet->m_FormTab4->m_nTabEdgeX2,pView->m_Project.m_ptCapCenter.y-pView->m_pDlgSet->m_FormTab4->m_nTabEdgeY1,pView->m_Project.m_ptCapCenter.x+pView->m_pDlgSet->m_FormTab4->m_nTabEdgeX1,pView->m_Project.m_ptCapCenter.y+pView->m_pDlgSet->m_FormTab4->m_nTabEdgeY2);
	// 
	// 			}//Edge Y의 +-를 반대로 했을 경우 대비
	rtcheck1.SetRect(pView->m_Project.m_ptCapCenter.x-pView->m_pDlgSet->m_FormTab4->m_nTabEdgeX1,pView->m_Project.m_ptCapCenter.y-pView->m_pDlgSet->m_FormTab4->m_nTabEdgeY1,pView->m_Project.m_ptCapCenter.x+pView->m_pDlgSet->m_FormTab4->m_nTabEdgeX2,pView->m_Project.m_ptCapCenter.y+pView->m_pDlgSet->m_FormTab4->m_nTabEdgeY2);
	//Tab Bottom 검사 WOO
	int nCanLX=0,nCanRX=0,nCanCX=0,nCanCY=0;
	nCanCX = (pView->m_Project.m_ptCanTop[1].x + pView->m_Project.m_ptCanTop[0].x)*0.5;
	nCanCY = (pView->m_Project.m_ptCanTop[1].y + pView->m_Project.m_ptCanTop[0].y)*0.5;
	nCanLX = nCanCX - pView->m_pDlgSet->m_FormTab4->m_nBottomTabLeft;//200;
	nCanRX = nCanCX + pView->m_pDlgSet->m_FormTab4->m_nBottomTabRight;//200;

	nFrame = 2; 
	int nTabLeft=0, nTabRight=0,nCount3=0,nCount4=0;
	CRect rtIns;
	rtIns.SetRect(nCanLX, nCanCY - pView->m_pDlgSet->m_FormTab4->m_nBottomTabTop, nCanRX, nCanCY - pView->m_pDlgSet->m_FormTab4->m_nBottomTabBot);
	rtIns = pView->m_Project.CheckROI(rtIns, CRect(0, 0, CAMERA_WIDTH, CAMERA_HEIGHT));

	int nBottomTabFrame = pView->m_pDlgSet->m_FormTab4->m_nBottomTabFrame;
	if (pFormTab4->m_bBottomTab){
		for (int y = nCanCY - pView->m_pDlgSet->m_FormTab4->m_nBottomTabTop; y < nCanCY - pView->m_pDlgSet->m_FormTab4->m_nBottomTabBot; y++) {  //좌측 찾기
			for (int x = nCanCX; x > nCanLX; x--) {
				if (*(pView->m_Project.m_MultiBuf[nBottomTabFrame] + CAMERA_WIDTH*y + x) - *(pView->m_Project.m_MultiBuf[nBottomTabFrame] + CAMERA_WIDTH*y + (x - 4)) > pView->m_pDlgSet->m_FormTab4->m_nBottomLeftTh &&
					*(pView->m_Project.m_MultiBuf[nBottomTabFrame] + CAMERA_WIDTH*y + x) - *(pView->m_Project.m_MultiBuf[nBottomTabFrame] + CAMERA_WIDTH*y + (x - 4)) < 100) {
					nTabLeft += x - 4;
					nCount3++;
				}
			}
		}
		for (int y = nCanCY - pView->m_pDlgSet->m_FormTab4->m_nBottomTabTop; y < nCanCY - pView->m_pDlgSet->m_FormTab4->m_nBottomTabBot; y++) {  //우측 찾기 
			for (int x = nCanCX; x < nCanRX; x++) {
				if (*(pView->m_Project.m_MultiBuf[nBottomTabFrame] + CAMERA_WIDTH*y + x) - *(pView->m_Project.m_MultiBuf[nBottomTabFrame] + CAMERA_WIDTH*y + (x + 4)) > pView->m_pDlgSet->m_FormTab4->m_nBottomRightTh
					&& *(pView->m_Project.m_MultiBuf[nBottomTabFrame] + CAMERA_WIDTH*y + x) - *(pView->m_Project.m_MultiBuf[nBottomTabFrame] + CAMERA_WIDTH*y + (x + 4)) < 100) {
					nTabRight += x + 4;
					nCount4++;
				}
			}
		}

		if (nCount3 > 0 && nCount4 > 0) {
			nTabLeft /= nCount3;
			nTabRight /= nCount4;
		}
		else if (nCount3 > 0 && nCount4 == 0) {
			nTabLeft /= nCount3;
			nTabRight = nCanCX;
		}
		else if (nCount3 == 0 && nCount4 > 0) {
			nTabLeft = nCanCX;
			nTabRight /= nCount4;
		}
	}

	pView->m_nTabBottomLeft = nTabLeft;
	pView->m_nTabBottomRight = nTabRight;//tab 하단 좌/우 좌표
	pView->m_nTabBottomTop = nCanCY;

	//Can 높이
	dValue = (CAMERA_HEIGHT-nCanCY) * dScale;
	dUsl = pView->m_pDlgSet->m_FormTab4->m_dCanDistMax;
	dLsl = pView->m_pDlgSet->m_FormTab4->m_dCanDistMin;
	pView->m_dCanDistY=dValue;
	if(pView->m_pDlgSet->m_FormTab4->m_bCanDist &&  (dValue > dUsl || dValue < dLsl)){
		pView->m_Project.m_nHoleOkNg = 1;
		pView->m_nBadCount[4]++;
		pView->DisplayNgCrop(0, 0, _T("CAN Dist"), 1, CPoint(nCanCX, nCanCY));
		//pView->SetDimensionData(2, FALSE, dValue, CPoint(nCanCX, nCanCY), CPoint(nCanCX, CAMERA_HEIGHT), TRUE);
		return NG_CAN_DIST;
		//pView->m_IO.WriteOutputPort(8,TRUE); // Can높이 불량시 8번 I/O 살리고 죽이기.
		//Sleep(pView->m_pDlgSet->m_FormTab2->m_nIoDelay);
		//pView->m_IO.WriteOutputPort(8,FALSE);
	}
	//	pView->SetDimensionData(2, FALSE, dValue, CPoint(nCanCX, nCanCY), CPoint(nCanCX, CAMERA_HEIGHT), FALSE);

	//용접 캔 거리
	dValue =  abs((pView->m_Project.m_ptCanTop[0].y+pView->m_Project.m_ptCanTop[1].y)*0.5- pView->m_ptWeldCenterPos.y) * pView->m_pDlgSet->m_FormTab2->m_dCamScale;
	dUsl = pView->m_pDlgSet->m_FormTab4->m_dDistWeldMax;
	dLsl = pView->m_pDlgSet->m_FormTab4->m_dDistWeldMin;

	if(pView->m_pDlgSet->m_FormTab4->m_bWeldDist && ( dValue > dUsl || dValue< dLsl)) {
		pView->m_Project.m_nHoleOkNg = 1;
		pView->m_nBadCount[4]++;
		pView->DisplayNgCrop(0, 0, _T("L4 Dist"), 1, CPoint(nCapCenterX, nCapCenterY));
		//pView->SetDimensionData(2, FALSE, dValue, CPoint(nCanLX, nCanCY), CPoint(nCanRX, nCanCY), TRUE); 확인필요
		return NG_WELD_CAN_DIST;

			//pView->m_nBadCount[1]++;
			//pView->m_Project.m_nHoleOkNg = 1;
			//pView->m_IO.m_nBadResult1 = 2; // hole ng signal
			//pView->m_Project.m_rtHoleRect.SetRect(pView->m_Project.m_ptCapCenter.x-150,pView->m_Project.m_ptCapCenter.y-96,pView->m_Project.m_ptCapCenter.x+150,pView->m_Project.m_ptCapCenter.y+96);
			//pView->m_strBadInfo1[1] = "L4 Dist";
			//pView->ImageShiftBad();
			//ntapresult = 0;//ng
	}
	//	pView->SetDimensionData(2, FALSE, dValue, CPoint(nCanLX, nCanCY), CPoint(nCanRX, nCanCY), FALSE); 확인필요

	//용접에서 탑캡
	dValue =  abs(pView->m_Project.m_ptCapCenter.y- pView->m_ptWeldCenterPos.y)*pView->m_pDlgSet->m_FormTab2->m_dCamScale;
	dUsl = pView->m_pDlgSet->m_FormTab4->m_dWeldDistCapMax;
	dLsl = pView->m_pDlgSet->m_FormTab4->m_dWeldDistCapMin;
	if(pView->m_pDlgSet->m_FormTab4->m_bWeldDistCap && ( dValue > dUsl || dValue < dLsl )){
		pView->m_Project.m_nHoleOkNg = 1;
		pView->m_nBadCount[4]++;
		pView->DisplayNgCrop(0, 0, _T("L2 Dist"), 1, CPoint(nCapCenterX, nCapCenterY));
		//pView->SetDimensionData(2, FALSE, dValue, CPoint(nCanLX, nCanCY), CPoint(nCanRX, nCanCY), TRUE); 수정필요
		return NG_WELD_CAP_DIST;

			//CTime ttime = CTime::GetCurrentTime();
			//CString kk2 = ttime.Format("%H:%M:%S");
			//pView->m_nBadCount[1]++;
			//pView->m_Project.m_nHoleOkNg = 1;
			//pView->m_IO.m_nBadResult1 = 2; // hole ng signal
			//pView->m_Project.m_rtHoleRect.SetRect(pView->m_Project.m_ptCapCenter.x-150,pView->m_Project.m_ptCapCenter.y-96,pView->m_Project.m_ptCapCenter.x+150,pView->m_Project.m_ptCapCenter.y+96);
			//pView->m_strBadInfo1[1] = "L2 Dist";
			//pView->m_strBadInfo1[0].Format("%s (%d)",kk2,pView->m_nOKCount+pView->m_nNgCount);		
			//pView->ImageShiftBad();
			//ntapresult = 0;//ng
	}
	//	pView->SetDimensionData(2, FALSE, dValue, CPoint(nCanLX, nCanCY), CPoint(nCanRX, nCanCY), FALSE); 수정필요

	//woo bottom
	pView->m_dTabBottomSize[0] = (pView->m_Project.m_ptCapCenter.x - nTabLeft) * dScale;
	pView->m_dTabBottomSize[1] = (nTabRight - pView->m_Project.m_ptCapCenter.x) * dScale;
	/*pView->m_dTabBottomSize[0] = (nCanCX - nTabLeft) * dScale;
	pView->m_dTabBottomSize[1] = (nTabRight - nCanCX) * dScale;*/
	int nedge = pView->m_Project.CheckRect(pView->m_Project.m_MultiBuf[pView->m_pDlgSet->m_FormTab2->m_nTabEdgeVFrame] ,rtcheck1,0);

	pView->m_nTabTopEdge = nedge;

	//Bottom tab
	//Cap Center X ~ Tab Edge(L)
	dValue =  pView->m_dTabBottomSize[0];
	dUsl = pFormTab4->m_dCanToCapLMin;
	dLsl = pFormTab4->m_dCanToCapL;
	//dUsl = pView->m_pDlgSet->m_FormTab4->m_dTabTol;
	if ( (dUsl < dValue || dLsl > dValue) && pFormTab4->m_bTwoTab==FALSE && pFormTab4->m_bBottomTab){
		pView->m_Project.m_nHoleOkNg = 1;
		pView->m_nBadCount[4]++;
		pView->DisplayNgCrop(0, 0, _T("Bottom Tab L"), 1, CPoint(nCapCenterX, nCapCenterY));
		return NG_BOTTOM_TAB;
	}

	//Cap Center X ~ Tab Edge(R)
	dValue =  pView->m_dTabBottomSize[1];
	dUsl = pFormTab4->m_dCanToCapRMin;
	dLsl = pFormTab4->m_dCanToCapR;
	if ( (dUsl < dValue || dLsl > dValue) && pFormTab4->m_bTwoTab==FALSE && pFormTab4->m_bBottomTab){
		pView->m_Project.m_nHoleOkNg = 1;
		pView->m_nBadCount[4]++;
		pView->DisplayNgCrop(0, 0, _T("Bottom Tab R"), 1, CPoint(nCapCenterX, nCapCenterY));
		return NG_BOTTOM_TAB;
	}
	
	//TAB Edge V
	dValue =  nedge;
	dUsl = pFormTab4->m_nCenterEdgeValue;
	if(nedge> dUsl){//&& pView->m_pDlgSet->m_FormTab4->m_bTwoTab==FALSE){ woo
		pView->m_Project.m_nHoleOkNg = 1;
		pView->m_nBadCount[2]++;
		pView->DisplayNgCrop(0, 0, _T("Tab Edge V"), 1, CPoint(nCapCenterX, nCapCenterY));
		return NG_TAP_FOLD;
	}

	double dcheckmm = pView->m_pDlgSet->m_FormTab4->m_dHoleEdgemm / dScale;
	double dcheckmm1 = pView->m_pDlgSet->m_FormTab4->m_dHoleEdgeINmm/ dScale;
	double dl1,dr1;
	dl1 = (pView->m_Project.m_nTopEdge[0] - pView->m_rtWeldBlob.left)* dScale;
	dr1 = (pView->m_rtWeldBlob.right-(pView->m_Project.m_nTopEdge[1]))* dScale;
	if(pView->m_pDlgSet->m_FormTab4->m_bHoleEdgeCheck )
	{
		// hoho 판정에 맞게 수정
		if(pView->m_rtWeldBlob.left< pView->m_Project.m_nTopEdge[0]-dcheckmm || 
			pView->m_rtWeldBlob.left> pView->m_Project.m_nTopEdge[0]+dcheckmm1 ||
			pView->m_rtWeldBlob.right> pView->m_Project.m_nTopEdge[1]+dcheckmm || 
			pView->m_rtWeldBlob.right< pView->m_Project.m_nTopEdge[1]-dcheckmm1){
	
				pView->m_Project.m_nHoleOkNg = 1;
				pView->m_nBadCount[3]++;
				pView->DisplayNgCrop(0, 0, _T("H-Edge"), 1, CPoint(nCapCenterX, nCapCenterY));
				return NG_WELD_TAB_DIST;
		}
	}	

	//용접 위치 검사


	//16 18번 이미지
	if(pView->m_nSelectFrame>10)
		memcpy(pView->m_Display[0].m_pImg->GetImagePtr(),pView->m_Project.m_AlphaBuf[pView->m_nSelectFrame-11],CAMERA_HEIGHT*CAMERA_WIDTH);
	else
		memcpy(pView->m_Display[0].m_pImg->GetImagePtr(),pView->m_Project.m_MultiBuf[pView->m_nSelectFrame],CAMERA_HEIGHT*CAMERA_WIDTH);

	pView->m_bResultIP = TRUE;
	return RET_OK;
}

void CFormTab3::OnBnClickedButtontopbottom()
{
	CFlyCapture2_EXDlg* pView = (CFlyCapture2_EXDlg *)m_hParent;
	BYTE * fmtmp = new BYTE[CAMERA_WIDTH*CAMERA_HEIGHT];
	memset(fmtmp,0,CAMERA_HEIGHT*CAMERA_WIDTH);

	int nStart = 1,nEnd=2;
	if(m_bCheckType==1)nEnd = 2;

	double dAmplify = 3;

	//0-2번영상 
	if(dAmplify<2){
		for(int k=nStart;k<nEnd;k++){
			for(int y=0;y<CAMERA_HEIGHT-4;y++){
				for(int x=0;x<CAMERA_WIDTH-4;x++){
					*(fmtmp+y*CAMERA_WIDTH+x) += abs(*(pView->m_Project.m_MultiBuf[k]+y*CAMERA_WIDTH+x) - *(pView->m_Project.m_MultiBuf[k]+(y+4)*CAMERA_WIDTH+x)) ;
					if(*(fmtmp+y*CAMERA_WIDTH+x)>250)*(fmtmp+y*CAMERA_WIDTH+x) = 250;
				}
			}
		}
	}
	else{
		for(int k=nStart;k<nEnd;k++){
			for(int y=0; y<CAMERA_HEIGHT-4;y++){
				for(int x=0; x<CAMERA_WIDTH-4;x++){
					if(abs(*(pView->m_Project.m_MultiBuf[k]+y*CAMERA_WIDTH+x) - *(pView->m_Project.m_MultiBuf[k]+(y+4)*CAMERA_WIDTH+x)) < 0 )
						*(fmtmp+y*CAMERA_WIDTH+x) += 0;
					else
						*(fmtmp+y*CAMERA_WIDTH+x) += abs(*(pView->m_Project.m_MultiBuf[k]+y*CAMERA_WIDTH+x) - *(pView->m_Project.m_MultiBuf[k]+(y+4)*CAMERA_WIDTH+x))*dAmplify ;
					if(*(fmtmp+y*CAMERA_WIDTH+x)>250)*(fmtmp+y*CAMERA_WIDTH+x) = 250;
				}
			}
		}
	}

	memcpy(pView->m_Display[0].m_pImg->GetImagePtr(),fmtmp,CAMERA_WIDTH*CAMERA_HEIGHT);
	//	memcpy(m_Display[TargetCam].m_pImg->GetImagePtr(),m_Project.m_fm[TargetCam],CAMERA_WIDTH*CAMERA_HEIGHT);
	memcpy(pView->m_DisplaySave[0].m_pImg->GetImagePtr(),pView->m_Display[0].m_pImg->GetImagePtr(),CAMERA_WIDTH*CAMERA_HEIGHT);
	memcpy(pView->m_Project.m_AlphaBuf[4],fmtmp,CAMERA_WIDTH*CAMERA_HEIGHT);


	CRect rttab;
	rttab.SetRect(pView->m_Project.m_ptCapCenter.x-20,pView->m_Project.m_ptCapCenter.y-70,pView->m_Project.m_ptCapCenter.x+20,pView->m_Project.m_ptCapCenter.y+70);
	rttab = pView->m_Project.CheckROI(rttab,CRect(10,10,CAMERA_WIDTH-10,CAMERA_HEIGHT-10));


	pView->DrawDisplay(0);

	delete fmtmp;
}

void CFormTab3::OnBnClickedEditSetFulsave()
{
	CFlyCapture2_EXDlg* pView = (CFlyCapture2_EXDlg *)m_hParent;
	double d1 = pView->m_Project.GetPrecisionTime();

	CString str;
	CTime time;
	CTime ttime = CTime::GetCurrentTime();
	CString kk = ttime.Format("%Y%m%d");
	CString kk2 = ttime.Format("%Y%m%d_%H%M%S");
	CString kk3 = ttime.Format("%d");
	CString kk1 = ttime.Format("%Y%m%d_%H%M%S");


// 	for(int i=0;i<19;i++){
// 		memcpy(pView->m_Display[0].m_pImg->GetImagePtr(),pView->m_Project.m_MultiBuf[i],CAMERA_WIDTH*CAMERA_HEIGHT);
// 		str.Format("%d.jpg", i+1);
// 		pView->m_Display[0].FullSave(kk1, str);
// 	}
	for(int i=0;i<4;i++){
		memcpy(pView->m_Display[0].m_pImg->GetImagePtr(),pView->m_Project.m_AlphaBuf[i],CAMERA_WIDTH*CAMERA_HEIGHT);
		str.Format("Alpha%d.jpg", i+1);
		pView->m_Display[0].FullSave(kk1, str);
	}
	double d2 = pView->m_Project.GetPrecisionTime();
	CString str1;
	str1.Format("Save Complete. %.1f",(d2-d1)*1000);
//	pView->AddtoInspLog(str1);
}

int CALLBACK BrowseCallbackProc1(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
{
	switch (uMsg)
	{
		// 폴더선택 다이얼로그의 초기화가 끝난 경우     
	case BFFM_INITIALIZED:       
		SendMessage(hwnd, BFFM_SETSELECTION, TRUE, lpData);       
		break;    
	}    
	return 0;
} 
void CFormTab3::OnBnClickedButtonLoad3()
{
	CFlyCapture2_EXDlg* pView = (CFlyCapture2_EXDlg *)m_hParent;
	pView->m_Display[0].ResetArray();
	pView->m_Project.m_nMultiFrame = 0;
	pView->m_bTesting = TRUE;

	CFile file;
	CString str,strng,strok;
	TCHAR	spath[100];
	BROWSEINFO bi;

	memset(&bi,0,sizeof(bi));
	bi.hwndOwner = GetSafeHwnd();
	bi.ulFlags=BIF_RETURNONLYFSDIRS;
	bi.lpfn = BrowseCallbackProc1; 

	CTime ttime = CTime::GetCurrentTime();
	CString kk3 = ttime.Format("%y");
	CString kk4 = ttime.Format("%m");
	CString kk5 = ttime.Format("%d");
	CString strInitPath;
	if(pView->m_strComname.Find("CH")>=0)
		strInitPath.Format(_T("C:\\Glim\\TesImg\\")); 
	else strInitPath.Format(_T("D:\\Image\\")); 
	
	bi.lParam = (LPARAM)strInitPath.GetBuffer(strInitPath.GetLength()); 
	LPITEMIDLIST lpifq=SHBrowseForFolder(&bi);
	if(lpifq)
	{
		SHGetPathFromIDList(lpifq,spath);
		LPMALLOC lpMalloc=NULL;
		HRESULT hr = SHGetMalloc(&lpMalloc);
		if(SUCCEEDED(hr))
		{
			GetLocalTime(&pView->m_timeVisionInput);

			lpMalloc->Free(lpifq);
			lpifq = NULL;
			lpMalloc->Release();
			lpMalloc = NULL;
		}
	}
	else
	{
		int nn=0;
		return;
	}
	HANDLE hFind;
	WIN32_FIND_DATA	fd;			// 검색한 화일의 정보
	str = LPCTSTR(spath);
	CString strImageFormat;
	strImageFormat.Format(_T("jpg"));
	//str += "\\*.jpg";
	hFind = FindFirstFile(str + "\\*.jpg", &fd);
	if (hFind == INVALID_HANDLE_VALUE) {
		hFind = FindFirstFile(str + "\\*.bmp", &fd);
		strImageFormat.Format(_T("bmp"));
	}


	CString src, tar,strDevice;
	BOOL bRet = TRUE;
	//str = LPCTSTR(spath);
	//str += "\\*.jpg";
	int ntmp =0;
	CString strMonth;
	CString strtime;
	int Ret = 0;
	bool bBmp;
	bRet = TRUE;
	//hFind = FindFirstFile( str, &fd);
	DWORD dwST,dwED;
	CFileFind finder;
	BOOL isfind;
	CString strfilename,str1;
	double d1 = pView->m_Project.GetPrecisionTime();

	CString str2,strSrc;
	for(int i=0;i<11;i++){
		strfilename.Format("%s\\%d.%s",spath,i+1, strImageFormat);
		pView->m_strFileName.Format("%s",spath);
		if(i==9)
			strSrc = strfilename;
		isfind = finder.FindFile(strfilename);
		if(isfind){
//			if(pView->m_Display[0].m_pImg->IsDataNull())	
			//pView->m_Display[0].m_pImg->Free();

			pView->m_Display[0].LoadImg(strfilename);
			memcpy(pView->m_Project.m_MultiBuf[i],pView->m_Display[0].m_pImg->GetImagePtr(),CAMERA_WIDTH*CAMERA_HEIGHT);
			if(i==6)
				memcpy(pView->m_DisplayServer[0].m_pImg->GetImagePtr(),pView->m_Display[0].m_pImg->GetImagePtr(),CAMERA_WIDTH*CAMERA_HEIGHT);
		}

	}
	pView->m_test = TRUE;
	pView->m_nDLResult = RET_INIT;
	//pView->m_test = FALSE;
	GetLocalTime(&pView->m_timeInspStart);

	double d2 = pView->m_Project.GetPrecisionTime();
	str1.Format("Load Complete. %.1f",(d2-d1)*1000);
	pView->AddtoInspLog(str1);
	pView->DrawDisplay2(0);
	pView->Test(0);
	//pView->BadNgCount(1, pView->m_Project.m_nTapOkNg, pView->m_Project.m_nHoleOkNg);

}

void CFormTab3::OnBnClickedButtonLeft()
{
	CFlyCapture2_EXDlg* pView = (CFlyCapture2_EXDlg *)m_hParent;

	pView->m_nSelectFrame--;
	if(pView->m_nSelectFrame<=0)pView->m_nSelectFrame = 0;
	if(pView->m_nSelectFrame<=10)
		memcpy(pView->m_Display[0].m_pImg->GetImagePtr(),pView->m_Project.m_MultiBuf[pView->m_nSelectFrame],CAMERA_HEIGHT*CAMERA_WIDTH);
	else
		memcpy(pView->m_Display[0].m_pImg->GetImagePtr(),pView->m_Project.m_AlphaBuf[pView->m_nSelectFrame-11],CAMERA_HEIGHT*CAMERA_WIDTH);
	pView->DrawDisplay(0);
}

void CFormTab3::OnBnClickedButtonRight()
{
	CFlyCapture2_EXDlg* pView = (CFlyCapture2_EXDlg *)m_hParent;

	pView->m_nSelectFrame++;
	if(pView->m_nSelectFrame>=10+5)pView->m_nSelectFrame = 10+5;//+4
	if(pView->m_nSelectFrame<=10)
		memcpy(pView->m_Display[0].m_pImg->GetImagePtr(),pView->m_Project.m_MultiBuf[pView->m_nSelectFrame],CAMERA_HEIGHT*CAMERA_WIDTH);
	else{
		memcpy(pView->m_Display[0].m_pImg->GetImagePtr(),pView->m_Project.m_AlphaBuf[pView->m_nSelectFrame-11],CAMERA_HEIGHT*CAMERA_WIDTH);
	}
	pView->DrawDisplay(0);
}

void CFormTab3::OnBnClickedButton18()
{
	CFlyCapture2_EXDlg* pView = (CFlyCapture2_EXDlg *)m_hParent;
//	pView->ReadCamTimeRate(1);
	//Weld Inspect
	//1. top cap 중심에서 거리 
	return;
	CRect rttmp,rttmp1;
	rttmp = pView->m_Display[0].m_rtImgTracker[5];
	int nw1 = rttmp.Width();//8의 배수로 만들어야 함.
	int nh1 = rttmp.Height();//8의 배수로 만들어야 함.
	nw1 -= nw1%8;nh1 -= nh1%8;
	rttmp1.SetRect(pView->m_Project.m_ptCapCenter.x-pView->m_Project.m_nRegWeldPos[0],pView->m_Project.m_ptCapCenter.y+pView->m_Project.m_nRegWeldPos[1],pView->m_Project.m_ptCapCenter.x-pView->m_Project.m_nRegWeldPos[0]+nw1,pView->m_Project.m_ptCapCenter.y+pView->m_Project.m_nRegWeldPos[1]+nh1);
//	pView->m_Display[0].AddArrayRect(rttmp1);
	pView->m_Project.m_rtWeldPos = rttmp1;

	CRect rt1[10];
	int x1=0,y1=0;
	int nmaster1 = 16;//18
	for(int i=0;i<6;i++){
		rt1[i].SetRect(10,10+(i*nh1),nw1+10,nh1+10+(i*nh1));

		if(i<4){
			y1 = rt1[i].top;
			for(int y=rttmp1.top;y<rttmp1.bottom;y++,y1++){
				x1=rt1[i].left;
				for(int x=rttmp1.left;x<rttmp1.right;x++,x1++){
					*(pView->m_Display[0].m_pImg->GetImagePtr()+CAMERA_WIDTH*y1+x1) = *(pView->m_Project.m_AlphaBuf[i]+CAMERA_WIDTH*y+x);
				}
			}
		}
		else
		{
			if(i==4)nmaster1=16;
			else nmaster1 = 18;
			y1 = rt1[i].top;
			for(int y=rttmp1.top;y<rttmp1.bottom;y++,y1++){
				x1=rt1[i].left;
				for(int x=rttmp1.left;x<rttmp1.right;x++,x1++){
					*(pView->m_Display[0].m_pImg->GetImagePtr()+CAMERA_WIDTH*y1+x1) = *(pView->m_Project.m_MultiBuf[nmaster1]+CAMERA_WIDTH*y+x);
				}
			}
		}
	}
//	pView->DrawDisplay(0);

	
}

void CFormTab3::OnBnClickedCheckAngleLightCh1No11()
{
	UpdateData();

	CString strComNum = _T("COM1");
	int nChannel = 11;

	// Checked
	if( IsDlgButtonChecked(IDC_CHECK_ANGLE_LIGHT_CH1_NO11) ){
		m_LightCtrl.LightOnVisioncoworkAngleLight(strComNum, m_nAngleLightValCh1, nChannel);
	}
	else{ // Unchecked
		m_LightCtrl.LightOffVisioncoworkAngleLight(strComNum, 0, nChannel);
	}
}

void CFormTab3::OnBnClickedCheckAngleLightCh1No12()
{
	UpdateData();

	CString strComNum = _T("COM1");
	int nChannel = 12;

	// Checked
	if( IsDlgButtonChecked(IDC_CHECK_ANGLE_LIGHT_CH1_NO12) ){
		m_LightCtrl.LightOnVisioncoworkAngleLight(strComNum, m_nAngleLightValCh1, nChannel);
	}
	else{ // Unchecked
		m_LightCtrl.LightOffVisioncoworkAngleLight(strComNum, 0, nChannel);
	}
}

void CFormTab3::OnBnClickedCheckAngleLightCh2No9()
{
	UpdateData();

	CString strComNum = _T("COM2");
	int nChannel = 9;

	// Checked
	if( IsDlgButtonChecked(IDC_CHECK_ANGLE_LIGHT_CH2_NO9) ){
		m_LightCtrl.LightOnVisioncoworkAngleLight(strComNum, m_nAngleLightValCh1, nChannel);
	}
	else{ // Unchecked
		m_LightCtrl.LightOffVisioncoworkAngleLight(strComNum, 0, nChannel);
	}
}

void CFormTab3::OnBnClickedCheckAngleLightCh2No10()
{
	UpdateData();

	CString strComNum = _T("COM2");
	int nChannel = 10;

	// Checked
	if( IsDlgButtonChecked(IDC_CHECK_ANGLE_LIGHT_CH2_NO10) ){
		m_LightCtrl.LightOnVisioncoworkAngleLight(strComNum, m_nAngleLightValCh1, nChannel);
	}
	else{ // Unchecked
		m_LightCtrl.LightOffVisioncoworkAngleLight(strComNum, 0, nChannel);
	}
}

void CFormTab3::OnBnClickedCheckAngleLightCh2No11()
{
	UpdateData();

	CString strComNum = _T("COM2");
	int nChannel = 11;

	// Checked
	if( IsDlgButtonChecked(IDC_CHECK_ANGLE_LIGHT_CH2_NO11) ){
		m_LightCtrl.LightOnVisioncoworkAngleLight(strComNum, m_nAngleLightValCh1, nChannel);
	}
	else{ // Unchecked
		m_LightCtrl.LightOffVisioncoworkAngleLight(strComNum, 0, nChannel);
	}
}

void CFormTab3::OnBnClickedCheckAngleLightCh2No12()
{
	UpdateData();

	CString strComNum = _T("COM2");
	int nChannel = 12;

	// Checked
	if( IsDlgButtonChecked(IDC_CHECK_ANGLE_LIGHT_CH2_NO12) ){
		m_LightCtrl.LightOnVisioncoworkAngleLight(strComNum, m_nAngleLightValCh1, nChannel);
	}
	else{ // Unchecked
		m_LightCtrl.LightOffVisioncoworkAngleLight(strComNum, 0, nChannel);
	}
}

void CFormTab3::MultiFrameFullSave()
{
	CFlyCapture2_EXDlg* pView = (CFlyCapture2_EXDlg *)m_hParent;
	double d1 = pView->m_Project.GetPrecisionTime();
	CString str;
	CTime ttime = CTime::GetCurrentTime();
	CString kk1;// = ttime.Format("%Y%m%d_%H%M%S");
	kk1 = pView->m_strFolderName;
	int nMax = pView->m_nMaxFrame;
	for(int i=0;i<nMax;i++){
		memcpy(pView->m_Display[0].m_pImgbuf->GetImagePtr(),pView->m_Project.m_MultiBuf[i],CAMERA_WIDTH*CAMERA_HEIGHT);
		//str.Format("%d.jpg", i+1);
		str.Format("%d", i+1);
		pView->m_Display[0].FullSave1(kk1, str, pView->m_nRBResult);
	}
	//pView->m_Display[0].FullSave1(kk1, str,1);

	double d2 = pView->m_Project.GetPrecisionTime();
	CString str1;
	str1.Format("Save Complete. %.1f",(d2-d1)*1000);
	pView->AddtoInspLog(str1);
}

void CFormTab3::MultiFrameSingleSave(int nfame)	//약용점용 저장
{
	CFlyCapture2_EXDlg* pView = (CFlyCapture2_EXDlg *)m_hParent;
	double d1 = pView->m_Project.GetPrecisionTime();
	CString str;
	CTime ttime = CTime::GetCurrentTime();
	CString kk1;// = ttime.Format("%Y%m%d_%H%M%S");
	kk1 = pView->m_strFolderName;
	int nMax = 10;
	if(m_bCheckType)nMax=11;
	memcpy(pView->m_Display[0].m_pImgbuf->GetImagePtr(),pView->m_Project.m_MultiBuf[nfame],CAMERA_WIDTH*CAMERA_HEIGHT);
	str.Format("%d_%d", pView->m_nOKCount+pView->m_nNgCount,nfame+1);	
	pView->m_Display[0].FullSave1(kk1, str, AI_MODE);

	double d2 = pView->m_Project.GetPrecisionTime();
	CString str1;
	str1.Format("Save Complete. %.1f",(d2-d1)*1000);
}

void CFormTab3::AlphaFrameFullSave(int nstart,int nEnd)
{
	CFlyCapture2_EXDlg* pView = (CFlyCapture2_EXDlg *)m_hParent;
	double d1 = pView->m_Project.GetPrecisionTime();
	CString str;
	CTime ttime = CTime::GetCurrentTime();
	CString kk1;// = ttime.Format("%Y%m%d_%H%M%S");
	kk1 = pView->m_strFolderName;
	for(int i=nstart;i<nEnd;i++){
		memcpy(pView->m_Display[0].m_pImg->GetImagePtr(),pView->m_Project.m_AlphaBuf[i],CAMERA_WIDTH*CAMERA_HEIGHT);
		str.Format("Alpha%d.jpg", i+1);
		pView->m_Display[0].FullSave(kk1, str);
	}
	double d2 = pView->m_Project.GetPrecisionTime();
	CString str1;
	str1.Format("Alpha Save Complete. %.1f",(d2-d1)*1000);
	pView->AddtoInspLog(str1);
}
void CFormTab3::OnBnClickedCheckType()
{//m_bCheckType
	UpdateData();
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CFormTab3::FindTopTab(BYTE * fm , CRect rt,int *x1, int *y1,int *x2,int *y2)
{
	*x1=0;*x2=0;*y1=0;*y2=0;
	CFlyCapture2_EXDlg* pView = (CFlyCapture2_EXDlg *)m_hParent;

	int nProjy[CAMERA_WIDTH] = {0,};
	int nmax1=0,nmax2=0,npos1=0,npos2=0;
	int nProjy1[CAMERA_WIDTH] = {0,};
	int nmax11=0,nmax12=0,npos11=0,npos12=0;
	double dave1=0,dc1=0;
	double dave11=0,dc11=0;
	CRect rt1;
	rt1 = rt;
	rt1.InflateRect(40,0);
	for(int y=rt.bottom;y>rt.top;y--){
		for(int x=rt.left;x<rt.right;x++){
			nProjy[y] += *(fm+y*CAMERA_WIDTH+x);
			dave1 +=*(fm+y*CAMERA_WIDTH+x);
			dc1++;
		}
	}
	for(int y=rt1.bottom;y>rt1.top;y--){
		for(int x=rt1.left;x<rt1.right;x++){
			nProjy1[y] += *(fm+y*CAMERA_WIDTH+x);
			dave11 +=*(fm+y*CAMERA_WIDTH+x);
			dc11++;
		}
	}
	//가장 큰값 과 차이가 가장 큰 값찾기
	for(int y=rt.bottom-4;y>rt.top+4;y--){
		if(nProjy[y] > nmax1){
			nmax1 = nProjy[y];npos1 = y;
		}
		//		if(y>rt.top-4 && nProjy[y] - nProjy[y-4] > nmax2){
		if(y>rt.top-4 && nProjy[y] - nProjy[y+4] > nmax2){
			nmax2 =nProjy[y] - nProjy[y+4];npos2 = y-4;
		}
	}
	for(int y=rt1.bottom-4;y>rt1.top+4;y--){
		if(nProjy1[y] > nmax11){
			nmax11 = nProjy1[y];npos11 = y;
		}
		if(y>rt1.top-4 && nProjy1[y] - nProjy1[y+4] > nmax12){
			nmax12 =nProjy1[y] - nProjy1[y+4];npos12 = y-4;
		}
	}
	if(dc1>0)
		dave1 /= dc1;
//찾은 Y 기준 좌/우 끝을 찾는다.
	npos1 = npos11;npos2 = npos12;
	*y1 = npos11;
	*y2 = npos12;
 	CRect rttab1;
 	rttab1.SetRect(rt.left-200,npos1-10,rt.right+200,npos1+30);
//	rttab1 = pView->m_Project.CheckROI(rttab1,CRect(50,20,CAMERA_WIDTH-50,CAMERA_HEIGHT-20));
	rttab1 = pView->m_Project.CheckROI(rttab1,CRect(50,20,CAMERA_WIDTH-50,CAMERA_HEIGHT-50));
 	int nCen = rt.left+rt.Width()*0.5;
	if(nCen>CAMERA_WIDTH-10)nCen = CAMERA_WIDTH-20;
	int nOldpos1=0,noldpos2=0;
	BOOL bflag = FALSE;
	CPoint pt1,pt2;
	int ntopth = pView->m_pDlgSet->m_FormTab4->m_nTabTopTh;//20
	int noffset = 15;
	int nYposSum = 0,nycnt=0;
 	for(int x=nCen;x>rttab1.left;x--){
 		for(int y=rttab1.bottom;y>rttab1.top;y--){
 			//pView->m_Display[0].AddArrayPoint()
			if(*(fm+y*CAMERA_WIDTH+x)-*(fm+(y-4)*CAMERA_WIDTH+x)>ntopth){
				if(nOldpos1!=0 && x<nOldpos1-noffset)
					bflag = TRUE;
				if(bflag)break;
				nOldpos1 = x;
				pt1.SetPoint(x,y-4);
				nYposSum += y;nycnt++;
				if(pView->m_pDlgSet->m_FormTab2->m_bShowEdge)
					pView->m_Display[0].AddArrayPoint(pt1);

				*x1 = nOldpos1;
				break;
			}
			if(bflag)break;
 		}
		if(bflag)break;
 	}
	bflag = FALSE;
	for(int x=nCen;x<rttab1.right;x++){
		for(int y=rttab1.bottom;y>rttab1.top;y--){
			//pView->m_Display[0].AddArrayPoint()
			if(*(fm+y*CAMERA_WIDTH+x)-*(fm+(y-4)*CAMERA_WIDTH+x)>ntopth){
				if(noldpos2 !=0 && x>noldpos2+noffset)
					bflag = TRUE;
				if(bflag)break;
				noldpos2 = x;
				*x2 = noldpos2;
				pt2.SetPoint(x,y-4);
				nYposSum += y;nycnt++;
				if(pView->m_pDlgSet->m_FormTab2->m_bShowEdge)
					pView->m_Display[0].AddArrayPoint(pt2);

				break;
			}
			if(bflag)break;
		}
		if(bflag)break;
	}

	double dlen = (noldpos2-nOldpos1)*pView->m_pDlgSet->m_FormTab2->m_dCamScale;
	CPoint ptold1,ptold2;
	double dOldLen=0;
	ptold1.SetPoint(0,0);
	ptold2.SetPoint(0,0);
	if(dlen>pView->m_pDlgSet->m_FormTab4->m_dTapSize+0.5 || dlen<pView->m_pDlgSet->m_FormTab4->m_dTapSize-0.5){
		//rescan
		//클때 작을때 구분
		int ncheck1 = 0;//클때
		if( dlen<pView->m_pDlgSet->m_FormTab4->m_dTapSize-0.5)ncheck1 = 1;//작을때
		for(int k1=0;k1<10;k1++){
			if(ncheck1==0)
				ntopth = pView->m_pDlgSet->m_FormTab4->m_nTabTopTh+((k1+1)*5);////30;
			else
				ntopth = pView->m_pDlgSet->m_FormTab4->m_nTabTopTh-((k1+1)*5);////30;
			nOldpos1=0,noldpos2=0,nYposSum=0,nycnt=0;
			bflag = FALSE;
			for(int x=nCen;x>rttab1.left;x--){
				for(int y=rttab1.bottom;y>rttab1.top;y--){
					//pView->m_Display[0].AddArrayPoint()
					if(*(fm+y*CAMERA_WIDTH+x)-*(fm+(y-4)*CAMERA_WIDTH+x)>ntopth){
						if(nOldpos1!=0 && x<nOldpos1-10)
							bflag = TRUE;
						if(bflag)break;
						nOldpos1 = x;
						pt1.SetPoint(x,y-4);
						nYposSum += y;nycnt++;
						if(pView->m_pDlgSet->m_FormTab2->m_bShowEdge)
							pView->m_Display[0].AddArrayPoint(pt1,1);

						*x1 = nOldpos1;
						break;
					}
					if(bflag)break;
				}
				if(bflag)break;
			}
			bflag = FALSE;
			for(int x=nCen;x<rttab1.right;x++){
				for(int y=rttab1.bottom;y>rttab1.top;y--){
					//pView->m_Display[0].AddArrayPoint()
					if(*(fm+y*CAMERA_WIDTH+x)-*(fm+(y-4)*CAMERA_WIDTH+x)>ntopth){
						if(noldpos2 !=0 && x>noldpos2+10)
							bflag = TRUE;
						if(bflag)break;
						noldpos2 = x;
						*x2 = noldpos2;
						pt2.SetPoint(x,y-4);
						nYposSum += y;nycnt++;
						if(pView->m_pDlgSet->m_FormTab2->m_bShowEdge)
							pView->m_Display[0].AddArrayPoint(pt2,1);

						break;
					}
					if(bflag)break;
				}
				if(bflag)break;
			}
			dlen = (noldpos2-nOldpos1)*pView->m_pDlgSet->m_FormTab2->m_dCamScale;
			if(nycnt<5){

			}
			else{
				nYposSum /=nycnt;
				pt1.y = nYposSum;
				pt2.y = nYposSum;
				npos1 = nYposSum;

			}		
			pView->m_Project.m_dTabTopLen = dlen;
			if(ncheck1==1 && dlen > pView->m_pDlgSet->m_FormTab4->m_dTapSize && dlen>0){

				if(fabs(dlen-pView->m_pDlgSet->m_FormTab4->m_dTapSize) > fabs(dOldLen-pView->m_pDlgSet->m_FormTab4->m_dTapSize)){
					pt1 = ptold1;
					pt2 = ptold2;
					nOldpos1 = pt1.x;noldpos2 = pt2.x;
				}
				else{

				}
				break;
			}
			if(ncheck1==0 && dlen < pView->m_pDlgSet->m_FormTab4->m_dTapSize && dlen>0){
				if(fabs(dlen-pView->m_pDlgSet->m_FormTab4->m_dTapSize) > fabs(dOldLen-pView->m_pDlgSet->m_FormTab4->m_dTapSize)){
					pt1 = ptold1;
					pt2 = ptold2;
					nOldpos1 = pt1.x;noldpos2 = pt2.x;
				}
				else{

				}
				break;
			}
			if(dlen<=0){

			}
			dOldLen = dlen;
			ptold1=pt1,ptold2=pt2;
		}
	
	}
	//if(dlen>pView->m_pDlgSet->m_FormTab4->m_dTapSize-0.3 && dlen<pView->m_pDlgSet->m_FormTab4->m_dTapSize+0.3)
	{


		if((pt1.x<=0 || nOldpos1<=0) && (pt2.x>0 && noldpos2>0) ){
			int ntabwidth = pView->m_pDlgSet->m_FormTab4->m_dTapSize/pView->m_pDlgSet->m_FormTab2->m_dCamScale;
			pt1.x = nOldpos1 = 	noldpos2-ntabwidth;
			pView->AddtoInspLog("Tab Recheck");
		}
		else if((pt2.x<=0 || noldpos2<=0) && (pt1.x>0 && nOldpos1>0)){
			int ntabwidth = pView->m_pDlgSet->m_FormTab4->m_dTapSize/pView->m_pDlgSet->m_FormTab2->m_dCamScale;
			pt2.x = noldpos2 = 	nOldpos1+ntabwidth;
			pView->AddtoInspLog("Tab Recheck");
		}
		else if((pt1.x<=0 || nOldpos1<=0) && (pt2.x<=0 || noldpos2<=0)){
			//둘다 못찾음 //recheck
			int ntabwidth = pView->m_pDlgSet->m_FormTab4->m_dTapSize/pView->m_pDlgSet->m_FormTab2->m_dCamScale;
			pView->AddtoInspLog("Tab Recheck");

		}
		pView->m_Display[0].AddArrayPoint(pt1);
		pView->m_Display[0].AddArrayPoint(pt2);
		pView->m_Project.m_ptTopEdge[0].SetPoint(nOldpos1,npos1);
		pView->m_Project.m_ptTopEdge[1].SetPoint(noldpos2,npos1);

		//pView->m_Project.m_ptLeftTop.SetPoint(nOldpos1, npos1);
		//pView->m_Project.m_ptRightTop.SetPoint(noldpos2, npos1);
	}
	pView->m_Project.m_dTabTopLen = dlen;
}

void CFormTab3::FindLeft(BYTE * fm , CRect rt)
{
	CFlyCapture2_EXDlg* pView = (CFlyCapture2_EXDlg *)m_hParent;

	int nProjy[CAMERA_WIDTH] = {0,};
	int nmax1=0,nmax2=0,npos1=0,npos2=0;
	for(int x=rt.left;x<rt.right;x++)for(int y=rt.bottom;y>rt.top;y--){
		nProjy[x] += *(fm+y*CAMERA_WIDTH+x);
	}
	//가장 큰값 과 차이가 가장 큰 값찾기
	for(int x=rt.left+4;x<rt.right;x++){
		if(nProjy[x] > nmax1){
			nmax1 = nProjy[x];npos1 = x;
		}
		if(x<rt.right-4 && (nProjy[x] - nProjy[x+4] ) > nmax2){
//		if(x<rt.right-4 && ((nProjy[x] - nProjy[x+4]) +(nProjy[x] - nProjy[x-4])) > nmax2){
//			nmax2 =nProjy[x] - nProjy[x+4] +nProjy[x] - nProjy[x-4];npos2 = x;
			nmax2 =nProjy[x] - nProjy[x+4] ;npos2 = x;
		}
		if(npos2>660&& npos2<680)
			npos2 = npos2;
	}
	//찾은 Y 기준 좌/우 끝을 찾는다.
	if(pView->m_pDlgSet->m_FormTab2->m_bTabSummation){
		/*if(abs(npos1-npos2)>=10)*/npos2 = npos1;//(npos1+npos2)*0.5;
	}
	pView->m_Project.m_ptTopEdge[0].x = npos2;
	pView->m_Project.m_ptLeftTop.SetPoint(npos2,rt.top);
	pView->m_Display[0].AddArrayXmark(CPoint(npos2,rt.top));

}

void CFormTab3::FindRight(BYTE * fm , CRect rt)
{
	CFlyCapture2_EXDlg* pView = (CFlyCapture2_EXDlg *)m_hParent;

	int nProjy[CAMERA_WIDTH] = {0,};
	int nmax1=0,nmax2=0,npos1=0,npos2=0;
	for(int x=rt.left;x<rt.right;x++)for(int y=rt.bottom;y>rt.top;y--){
		nProjy[x] += *(fm+y*CAMERA_WIDTH+x);
	}
	//가장 큰값 과 차이가 가장 큰 값찾기
	int ngap = 6;
	for(int x=rt.right-ngap;x>rt.left ;x--){
		if(nProjy[x] > nmax1){
			nmax1 = nProjy[x];npos1 = x;
		}
		if(x>rt.left+ngap && nProjy[x-ngap]>0&& ( (nProjy[x-ngap] - nProjy[x])) > nmax2){
			nmax2 =nProjy[x-ngap] - nProjy[x];npos2 = x-ngap;
		}
	}
//	for(int x=rt.left+ngap;x<rt.right-10 ;x++){ //기존
//		if(nProjy[x] > nmax1){
//			nmax1 = nProjy[x];npos1 = x;
//		}
////		if(x<rt.right-4 && nProjy[x+4]>0&& ((nProjy[x] - nProjy[x+4])  + (nProjy[x] - nProjy[x-4])) > nmax2){
//		if(x<rt.right-ngap && nProjy[x-ngap]>0&& ( (nProjy[x] - nProjy[x-ngap])) > nmax2){
//			nmax2 =nProjy[x] - nProjy[x-ngap];npos2 = x+ngap;
//		}
//	}
	//찾은 Y 기준 좌/우 끝을 찾는다.
	
	if(pView->m_pDlgSet->m_FormTab2->m_bTabSummation){
		npos2 = (npos1+npos2)*0.5;
	}
	pView->m_Project.m_ptTopEdge[1].x = npos2;

	//탭폭이 많이 틀릴경우 다시 찾기
	if((pView->m_Project.m_ptTopEdge[1].x-pView->m_Project.m_ptTopEdge[0].x)*pView->m_pDlgSet->m_FormTab2->m_dCamScale < pView->m_pDlgSet->m_FormTab4->m_dTapSize-0.5 )
	{
		int nmincheck = 0;
		if(pView->m_Project.m_ptTopEdge[1].x < npos1)
			pView->m_Project.m_ptTopEdge[1].x = npos2 =npos1;

		nmax1 = 0;
		int npos3=0;
		for(int x=rt.right-ngap;x>rt.left+10 ;x--){
			if(x<rt.right-ngap && nProjy[x-ngap]>0&& ( (nProjy[x] - nProjy[x-ngap])) > nmax2*0.8){
				nmax2 =nProjy[x] - nProjy[x-ngap];npos3= x+ngap;
				break;
			}
		}
		if(npos3==0){
			for(int x=rt.right-ngap;x>rt.left+10 ;x--){
				if(x<rt.right-ngap && nProjy[x-ngap]>0&& ( (nProjy[x] - nProjy[x-ngap])) > nmax2*0.6){
					nmax2 =nProjy[x] - nProjy[x-ngap];npos3= x+ngap;
					break;
				}
			}
		}
		if(npos3>0)
			pView->m_Project.m_ptTopEdge[1].x = npos2 =npos3;

	}
	else if((pView->m_Project.m_ptTopEdge[1].x-pView->m_Project.m_ptTopEdge[0].x)*pView->m_pDlgSet->m_FormTab2->m_dCamScale > pView->m_pDlgSet->m_FormTab4->m_dTapSize+0.5 )
	{
		int nmaxcheck = 0;
		if(pView->m_Project.m_ptTopEdge[1].x > npos1)
			pView->m_Project.m_ptTopEdge[1].x =npos2 = npos1;

	}

	pView->m_Display[0].AddArrayXmark(CPoint(npos2,rt.top));
	pView->m_Project.m_ptRightTop.SetPoint(npos2,rt.top);

}
void CFormTab3::OnBnClickedButtonLoad4()
{
	CFlyCapture2_EXDlg* pView = (CFlyCapture2_EXDlg *)m_hParent;

	pView->m_nSelectFrame = 9;
	CFile file;
	CString str,strng,strok;
	TCHAR	spath[100];
	BROWSEINFO bi;
	memset(&bi,0,sizeof(bi));
	bi.hwndOwner = GetSafeHwnd();
	bi.ulFlags=BIF_RETURNONLYFSDIRS;
	bi.lpfn = BrowseCallbackProc1; 
	bi.ulFlags=BIF_RETURNONLYFSDIRS;

	CString strInitPath;
	if(pView->m_strComname.Find("CH")>=0)
		strInitPath.Format(_T("C:\\Glim\\TesImg\\")); //C:\glim\TesImg
	else strInitPath.Format(_T("D:\\Image\\")); //C:\glim\TesImg
	bi.lParam = (LPARAM)strInitPath.GetBuffer(strInitPath.GetLength()); 
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
		int nn=0;
		return;
	}

	CString src, tar,strDevice;
	HANDLE hFind;				// FindFirstFile 핸들
	WIN32_FIND_DATA	fd;			// 검색한 화일의 정보
	BOOL bRet = TRUE;
	CString strfile;
	str = LPCTSTR(spath);
	str += "\\*.*";
	int ntmp =0;
	CString strMonth;

	bRet = TRUE;
	hFind = FindFirstFile( str, &fd);
	strDevice = strfile.Left(3);
	while(hFind != INVALID_HANDLE_VALUE && bRet)
	{			
		strfile.Format("%s\\%s", spath, fd.cFileName);
		strMonth.Format("%s",fd.cFileName);
		if(strfile.Find(".")>=0){}
		else{
			if(strMonth.Find("_")>=5){
				CFileFind finder;
				BOOL isfind;
				CString strfilename,str1;
				double d1 = pView->m_Project.GetPrecisionTime();

				pView->m_strFileName.Format("%s",strfile);
				for(int i=0;i<11;i++){
					strfilename.Format("%s\\%d.jpg",strfile,i+1);
					isfind = finder.FindFile(strfilename);
					if(isfind){
						//			if(pView->m_Display[0].m_pImg->IsDataNull())	
						//pView->m_Display[0].m_pImg->Free();

						pView->m_Display[0].LoadImg(strfilename);
						memcpy(pView->m_Project.m_MultiBuf[i],pView->m_Display[0].m_pImg->GetImagePtr(),CAMERA_WIDTH*CAMERA_HEIGHT);
					}
				}

				double d2 = pView->m_Project.GetPrecisionTime();
				str1.Format("Load Complete. %.1f",(d2-d1)*1000);
				pView->AddtoInspLog(str1);
				pView->DrawDisplay2(0);
				OnBnClickedButtonTest();
				pView->capture();
				if(m_bFolderNGSave && (!pView->m_Project.m_nTapOkNg==RET_OK || !pView->m_Project.m_nHoleOkNg==RET_OK ))
				{
					MultiFrameFullNGSave("");
				}
				pView->m_Project.m_nFrameCount++;
				if(!m_bCkAuto)	
				{
					while( WaitForSingleObject((HANDLE)m_ev_rbutton_down, 50) != WAIT_OBJECT_0 )
					{
						pView->Wait(10);

					}
				}
				else
					pView->Wait(10);
			}

		}
		Sleep(10);
		
		bRet= FindNextFile(hFind, &fd);
	}
	::FindClose(hFind);
	AfxMessageBox("Directory Inspect Complete!!!");	

	return;
}

void CFormTab3::WeldPosRect(int nStart,int nEnd,double dAmplify,CRect rtweld,BOOL bShow)
{
	CFlyCapture2_EXDlg* pView = (CFlyCapture2_EXDlg *)m_hParent;
//8dml 배수가 필요한가?
	int nw1 = rtweld.Width();//8의 배수로 만들어야 함.
	int nh1 = rtweld.Height();//8의 배수로 만들어야 함.
// 	nw1 -= nw1%8;nh1 -= nh1%8;
// 	rtweld.right = rtweld.left+nw1;
// 	rtweld.bottom = rtweld.top+nh1;


	BYTE * fmnew = new BYTE[rtweld.Width()*rtweld.Height()];
	memset( fmnew,0,rtweld.Width()*rtweld.Height());
	int x1=0,y1=0,x2=0,y2=0;
	int nSelectcam = pView->m_nSelectFrame;
	//1-2번영상 
//좌측 제일 위에 
	for(int k=nStart;k<nEnd;k++){
		x1=y1=0;
		for(int y=rtweld.top; y<rtweld.bottom;y++){
			x1 = 0;
			for(int x=rtweld.left; x<rtweld.right;x++){
				if(abs(*(pView->m_Project.m_MultiBuf[k]+y*CAMERA_WIDTH+x) - *(pView->m_Project.m_MultiBuf[k]+(y+4)*CAMERA_WIDTH+x)) < 0 ){
					//*(fmtmp+y*CAMERA_WIDTH+x) += 0;
					*(fmnew+y1*nw1+x1) += 0;
				
				}
				else{
				//	*(fmtmp+y*CAMERA_WIDTH+x) += abs(*(pView->m_Project.m_MultiBuf[k]+y*CAMERA_WIDTH+x) - *(pView->m_Project.m_MultiBuf[k]+(y+4)*CAMERA_WIDTH+x))*dAmplify ;
					*(fmnew+y1*nw1+x1) += abs(*(pView->m_Project.m_MultiBuf[k]+y*CAMERA_WIDTH+x) - *(pView->m_Project.m_MultiBuf[k]+(y+4)*CAMERA_WIDTH+x))*dAmplify ;
				}
				//if(*(fmtmp+y*CAMERA_WIDTH+x)>250)*(fmtmp+y*CAMERA_WIDTH+x) = 255;
				if(*(fmnew+y1*nw1+x1)>250) *(fmnew+y1*nw1+x1) = 250;
				x1++;
			}
			y1++;
		}
	}
	//여러장을 도니 다시 복사한다.
	/*if(nSelectcam<=10){
		for(int i=y2;i<y2+nh1;i++){
			for(int j=x2;j<x2+nw1;j++){
				*(pView->m_Project.m_MultiBuf[nSelectcam]+i*CAMERA_WIDTH+j) = *(fmnew+(i-y2)*nw1+j);
			}
		}
	}*/

	CRect rt1;
	rt1.SetRect(0,0,nw1,nh1);
	WeldPoint(fmnew,nw1,nh1,rt1,0);

	CRect rttab;
	rttab.SetRect(pView->m_Project.m_ptCapCenter.x-20,pView->m_Project.m_ptCapCenter.y-70,pView->m_Project.m_ptCapCenter.x+20,pView->m_Project.m_ptCapCenter.y+70);
	rttab = pView->m_Project.CheckROI(rttab,CRect(10,10,CAMERA_WIDTH-10,CAMERA_HEIGHT-10));


//	pView->DrawDisplay(0);

	delete fmnew;
}
void CFormTab3::OnBnClickedCheck5()
{//m_bFullAlphaImage
	UpdateData();
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CFormTab3::WeldPoint(BYTE * fm,int nwidth,int nheight,CRect rt,int nmiltiful)
{
	CFlyCapture2_EXDlg* pView = (CFlyCapture2_EXDlg *)m_hParent;
	int nw1 = rt.Width();//8의 배수로 만들어야 함.
	int nh1 = rt.Height();//8의 배수로 만들어야 함.
//	nw1 -= nw1%8;nh1 -= nh1%8;
//	평균 밝기로 이진화
	double dave=0,dc=0;
	int nhisto[256] = {0,};
	BYTE * fmtmp = new BYTE[nw1*nh1];//영역 만큼만 포인터를 잡고
	memset(fmtmp,0,nw1*nh1);
	int nProjY[CAMERA_WIDTH] = {0,};
	for(int y=rt.top;y<rt.bottom;y++){
		for(int x=rt.left;x<rt.right;x++){
			dave += *(fm+y*nwidth+x);
			nhisto[*(fm+y*nwidth+x)]++;
		}
	}
	dave /= (rt.Width()*rt.Height());
	double dth1 = 0;
	int nmax1 = 0,npos1=0,ncnt=0,ncut=500,npos2=0,ncut1=600,npos3=0;
	for(int i=dave+10;i<255;i++){
		if(nhisto[i]>nmax1){
			nmax1 = nhisto[i];
			npos1 = i;
		}
	}
	if(nmiltiful==1){
		ncnt = 0;
		int nCutPercenter = (pView->m_pDlgSet->m_FormTab4->m_nWeldingSize*2)*pView->m_pDlgSet->m_FormTab4->m_nWeldingSize*(pView->m_pDlgSet->m_FormTab2->m_dWeldPercent*0.01);
		ncut = nCutPercenter;
	}
	if(ncut>250)ncut = 250;
	if(ncut<0)ncut = 0;
	for(int i=255;i>dave+10;i--){
		ncnt += nhisto[i];
		if(ncnt>ncut){//히스토그램의 255부터 0까지 누적 개수가 500을 넘는 밝기가 th값 
			npos2 = i;
			break;
		}
	}
	if(nmiltiful==1){
		ncnt = 0;
		int nCutPercenter = (pView->m_pDlgSet->m_FormTab4->m_nWeldingSize*2)*pView->m_pDlgSet->m_FormTab4->m_nWeldingSize*(pView->m_pDlgSet->m_FormTab2->m_dWeldPercent*0.01);
		for(int i=255;i>dave+10;i--){
			ncnt += nhisto[i];
			if(ncnt>nCutPercenter){//히스토그램의 255부터 0까지 누적 개수가 500을 넘는 밝기가 th값
				npos3 = i;
				break;
			}
		}
	}

	if(npos3==0)npos3 = ncut;
//	dth1 = npos1;
//	if(dave<10)
		dave = npos2;
	int y11 = 0;
	int ncx=0,ncy=0,ncc=0;
	//탭 에지 라인을 먼저 찾는다.
	for(int y=rt.top;y<rt.bottom;y++){
		int x11 = 0;
		for(int x=rt.left;x<rt.right;x++){
			if(*(fm+y*nwidth+x)> dave){
				*(fmtmp+y11*nw1+x11) = 255;
				*(pView->m_Project.m_WeldBuf[nmiltiful]+y11*400+x11) = 255;

//				ncx+= x11;ncy+=y11;ncc++;
				nProjY[x]++;
			}
			else{
				*(fmtmp+y11*nw1+x11) = 0;
				*(pView->m_Project.m_WeldBuf[nmiltiful]+y11*400+x11) = 0;
			}
			x11++;
		}
		y11++;
	}
// 라인이 있는지를 체크한다.
	//rect height 2/3 이상일 경우 라인으로 인식
	int nLineth = rt.Height()*0.66;
	for(int x=rt.left;x<rt.right;x++){
		if(nProjY[x]>nLineth){
			if(x>0)nProjY[x-1] = -1;
			nProjY[x] = -1;
		}
	}

	int ntmpsum1=0,ntmpsum2=0;
	for(int x=rt.left;x<rt.right;x++){
		ntmpsum1 += nProjY[x];
		if(ntmpsum1>=10){
			m_nLeftStart = x;
			break;
		}

	}
	y11 = 0;
	for(int y=rt.top;y<rt.bottom;y++){
		int x11 = 0;
		for(int x=rt.left;x<rt.right;x++){
			if(nProjY[x]==-1)continue;
			if(*(fm+y*nwidth+x)> dave){
				ncx+= x11;ncy+=y11;ncc++;
			}
			x11++;
		}
		y11++;
	}
	int nSelectcam = pView->m_nSelectFrame;
	int nh11 = pView->m_Project.m_rtWeldPos.Height();
	int nw11 = pView->m_Project.m_rtWeldPos.Width();
	int x2=nw1+10,y2=0;

	y2=(pView->m_Project.m_rtWeldPos.Height()+10)*nmiltiful;

	if(ncc>0){
		ncx /= ncc;ncy /= ncc;
		if(ncx !=0 && ncx<pView->m_Project.m_nWeldMinMax[0])//min
			pView->m_Project.m_nWeldMinMax[0] = ncx;
		if(ncx !=0 && ncx>pView->m_Project.m_nWeldMinMax[1])//max
			pView->m_Project.m_nWeldMinMax[1] = ncx;

		pView->m_Project.m_ptWeldPos[nmiltiful].SetPoint(ncx,ncy);
		pView->m_Display[0].AddArrayPoint(CPoint(ncx,ncy+y2));
	}
 	else{
 //		delete fmtmp;
 		return;
 	}

//dialte
	if(nmiltiful==1){//상부탭 찾는 이미지에서만 한다.

		if(nw1<=1 || nh1<=1){
			delete fmtmp;
			return;
		}

		BYTE * fmtmp1 = new BYTE[nw1*nh1];//영역 만큼만 포인터를 잡고
		memset(fmtmp1,0,nw1*nh1);
		pView->m_Project.fnDilate(fmtmp,fmtmp1,CRect(0,0,nw1,nh1),1);
//Blob
		double  dArea=0, dCx=0, dCy=0, dAngle=0;
		int nwcut  = 70,nhcut = 20;
		if(pView->m_nMachine==2)nhcut = pView->m_pDlgSet->m_FormTab2->m_nWeldHeight;//35
		CRect weldarea = pView->m_Project.fnBlobImage(fmtmp1, nw1, nh1,&dArea,&dCx,&dCy,&dAngle);
		if(weldarea.Width()<70){
			//recheck
			BYTE * fmtmp2 = new BYTE[nw1*nh1];//영역 만큼만 포인터를 잡고
			memset(fmtmp2,0,nw1*nh1);
			BYTE * fmtmp3 = new BYTE[nw1*nh1];//영역 만큼만 포인터를 잡고
			memset(fmtmp3,0,nw1*nh1);
			for(int mm=0;mm<30;mm++){
				pView->m_Project.fnDilate(fmtmp1,fmtmp2,CRect(0,0,nw1,nh1),1);
				memcpy(fmtmp1,fmtmp2,nw1*nh1);
				CRect weldarea1 = pView->m_Project.fnBlobImage(fmtmp1, nw1, nh1,&dArea,&dCx,&dCy,&dAngle);
				weldarea = weldarea1;
				pView->m_nMorphCount[0] = mm;
				if(mm>1 &&weldarea.Width()>nwcut &&weldarea.Height()>nhcut )
					break;
			}
			delete fmtmp2;
			delete fmtmp3;
		}
	
		//ho,woo 추가 200908
		if(pView->m_pDlgSet->m_FormTab2->m_bWeldCenterPos2){
			int nLeft = nw1;
			int nRight = 0;
			int nTop = nh1;
			int nBottom = 0;
			for(int nY=0; nY<nh1; nY++){
				for(int nX=0; nX<nw1; nX++){
					if(*(fmtmp1+nY*nw1+nX) > 200){
						if(nLeft > nX)
							nLeft = nX;          
						if(nRight < nX)
							nRight = nX;
						if(nTop > nY)
							nTop = nY;
						if(nBottom < nY)
							nBottom = nY;
					}
				}
			}
			weldarea.SetRect(nLeft, nTop, nRight, nBottom);
		}
		//200908
		pView->m_Display[0].AddArrayRect(weldarea,0);
		if(weldarea.Height()<20)weldarea.InflateRect(0,10);
		pView->m_rtWeldBlob = weldarea;
		pView->m_rtWeldBlob.OffsetRect(pView->m_Project.m_rtWeldPos.left,pView->m_Project.m_rtWeldPos.top);
		pView->m_rtWeldBlob = pView->m_Project.CheckROI(pView->m_rtWeldBlob,CRect(10,10,CAMERA_WIDTH-10,CAMERA_HEIGHT-10));

		if(pView->m_nMachine==8 || pView->m_nMachine==2){
			pView->m_ptWeldCenterPos = pView->m_rtWeldBlob.CenterPoint();
		}
		if(pView->m_pDlgSet->m_FormTab2->m_bMorphShow){
			//마지막거에 붙인다.
			//3qjs
		}

		memcpy(fmtmp,fmtmp1,nw1*nh1);
		delete fmtmp1;
	}

	int x3 = pView->m_Project.m_rtWeldPos.left,y3=pView->m_Project.m_rtWeldPos.top;
	//여러장을 도니 다시 복사한다.
	//if(nSelectcam<=10){
	//	for(int i=y2;i<y2+nh11;i++){
	//		for(int j=x2;j<x2+nw11;j++){
	//			if(j<0 || i<0 || j>=CAMERA_WIDTH||i>=CAMERA_HEIGHT)continue;
	//			*(pView->m_Project.m_MultiBuf[nSelectcam]+i*CAMERA_WIDTH+j) = *(fmtmp+(i-y2)*nw1+(j-x2));
	//		}
	//	}
	//}

	delete fmtmp;
}

int CFormTab3::WeldCheck(int nstart,int nend,int nwidth,int nheight,CRect rtL,CRect rtR,int nth,double dcutL,double dcutR)
{
	CFlyCapture2_EXDlg* pView = (CFlyCapture2_EXDlg *)m_hParent;
	int nw1 = rtL.Width();//8의 배수로 만들어야 함.
	int nh1 = rtL.Height();//8의 배수로 만들어야 함.
	int nw2 = rtR.Width();//8의 배수로 만들어야 함.
	int nh2 = rtR.Height();//8의 배수로 만들어야 함.
	int ret = 0;
	//	nw1 -= nw1%8;nh1 -= nh1%8;
	double dave=0,dc=0;

	int nWhite1 = 0,nWhite2=0;
	for(int k=nstart;k<=nend;k++){
		if(nw1>0){
			for(int y=rtL.top;y<rtL.bottom;y++){
				for(int x=rtL.left;x<rtL.right;x++){
					if(pView->m_Project.m_MultiBuf[k][y*CAMERA_WIDTH+x]>nth)
						nWhite1++;
				}
			}
		}
		if(nw2>0){
			for(int y=rtR.top;y<rtR.bottom;y++){
				for(int x=rtR.left;x<rtR.right;x++){
					if(pView->m_Project.m_MultiBuf[k][y*CAMERA_WIDTH+x]>nth)
						nWhite2++;
				}
			}
		}
	}
	double dl=0,dr=0;
	if(nw1>0)
		dl= double(nWhite1)/double(nw1*nh1)*1000;
	if(nw2>0)
		dr= double(nWhite2)/double(nw2*nh2)*1000;
	if(dcutL < dl){ret = 1;pView->m_bHoleBad[0][0] = TRUE;}
	if(dcutR <dr){ret = 2;pView->m_bHoleBad[0][1] = TRUE;}
	if(dcutL < dl &&dcutR <dr)ret = 3;
//m_bHoleBad[0][0]=m_bHoleBad[0][1]=m_bHoleBad[1][0]=m_bHoleBad[1][1] = TRUE;
	pView->m_Project.m_dBlackPer[0] = dl;
	pView->m_Project.m_dBlackPer[1] = dr;
	if(pView->m_pDlgSet->m_FormTab2->m_bLineWelding == TRUE || pView->m_pDlgSet->m_FormTab2->m_b2LineWelding == TRUE){
		pView->m_strResultValue[0].Format("R(Line1) : %.1f(%.0f)",pView->m_Project.m_dBlackPer[0],pView->m_pDlgSet->m_FormTab4->m_dLeftArea);
		pView->m_strResultValue[1].Format("S(Line1) : %.1f(%.0f)",pView->m_Project.m_dBlackPer1[0],pView->m_pDlgSet->m_FormTab4->m_dLeftArea1);
		pView->m_strResultValue[2].Format("R(Line2) : %.1f(%.0f)",pView->m_Project.m_dBlackPer[1],pView->m_pDlgSet->m_FormTab4->m_dRightArea);
		pView->m_strResultValue[3].Format("S(Line2) : %.1f(%.0f)",pView->m_Project.m_dBlackPer1[1],pView->m_pDlgSet->m_FormTab4->m_dRightArea1);
	}
	else{
		pView->m_strResultValue[0].Format("R(Left) : %.1f(%.0f)",pView->m_Project.m_dBlackPer[0],pView->m_pDlgSet->m_FormTab4->m_dLeftArea);
		pView->m_strResultValue[1].Format("S(Left) : %.1f(%.0f)",pView->m_Project.m_dBlackPer1[0],pView->m_pDlgSet->m_FormTab4->m_dLeftArea1);
		pView->m_strResultValue[2].Format("R(Right) : %.1f(%.0f)",pView->m_Project.m_dBlackPer[1],pView->m_pDlgSet->m_FormTab4->m_dRightArea);
		pView->m_strResultValue[3].Format("S(Right) : %.1f(%.0f)",pView->m_Project.m_dBlackPer1[1],pView->m_pDlgSet->m_FormTab4->m_dRightArea1);
	}

	return ret;//0 ok 1 left 2 right 3 all
}
void CFormTab3::OnBnClickedCheckNgsave()
{//m_bFolderNGSave
	UpdateData();
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CFormTab3::MultiFrameFullNGSave(CString filename1)
{
	CFlyCapture2_EXDlg* pView = (CFlyCapture2_EXDlg *)m_hParent;
	double d1 = pView->m_Project.GetPrecisionTime();
	CString str,str1;
	CTime ttime = CTime::GetCurrentTime();
	CString kk = ttime.Format("%Y%m%d");
	CString kk2 = ttime.Format("%Y%m");
	CString kk3 = ttime.Format("%d");
	CString kk1 = ttime.Format("[%Y%m%d_%H%M%S]");
	CString kk4 = ttime.Format("%H%M%S");
	CString kk5 = ttime.Format("%H");

	//if(m_strTimePath == "")
	//	m_strTimePath.Format("C-%s",kk4);
	//년월	일	m_strModelName	m_strInspStartTime
	str.Format("c:\\Data\\");
	CreateDirectory(str,NULL);
	str.Format("c:\\Data\\Image\\");
	CreateDirectory(str,NULL);
	str.Format("c:\\Data\\Image\\%s\\",kk2);//년월
	CreateDirectory(str,NULL);
	str.Format("c:\\Data\\Image\\%s\\%s\\",kk2,kk3);//일
	CreateDirectory(str,NULL);
	str.Format("c:\\Data\\Image\\%s\\%s\\%s\\",kk2,kk3,kk5);//time
	CreateDirectory(str,NULL);
	str.Format("c:\\Data\\Image\\%s\\%s\\%s\\Overlay\\",kk2,kk3,kk5);//time
	CreateDirectory(str,NULL);
	str.Format("c:\\Data\\Image\\%s\\%s\\%s\\Overlay\\NG\\",kk2,kk3,kk5);//time
	CreateDirectory(str,NULL);
	str.Format("c:\\Data\\Image\\%s\\%s\\%s\\Overlay\\NG\\Image\\",kk2,kk3,kk5);//time
	CreateDirectory(str,NULL);
	str.Format("c:\\Data\\Image\\%s\\%s\\%s\\Overlay\\NG\\Image\\%s_%d\\",kk2,kk3,kk5,kk1,pView->m_nOKCount+pView->m_nNgCount);//time
	CreateDirectory(str,NULL);

	int nMax = 10;
	if(m_bCheckType)nMax=11;
	for(int i=0;i<nMax;i++){
		memcpy(pView->m_Display[0].m_pImg->GetImagePtr(),pView->m_Project.m_MultiBuf[i],CAMERA_WIDTH*CAMERA_HEIGHT);
		str1.Format("%s%d.jpg", str,i+1);
//		pView->m_Display[0].sFullSave1(str, str1);
		int nh = pView->m_Display[0].m_pImg->GetHeight();

		for (int i=0;i<nh;i++)
			memcpy(pView->m_Display[0].m_pImgbuf->GetImagePtr()+CAMERA_WIDTH*i, pView->m_Display[0].m_pImg->GetImagePtr()+CAMERA_WIDTH*(nh-i-1), CAMERA_WIDTH);

		pView->m_Display[0].m_pImgbuf->Save(str1);

	}

	double d2 = pView->m_Project.GetPrecisionTime();
	str1.Format("Save Complete. %.1f",(d2-d1)*1000);
	pView->AddtoInspLog(str1);
}

int CFormTab3::WeldCheckNew(CRect rtLimit,int nstart,int nend,int nwidth,int nheight,CRect rtL,CRect rtR,int wth,double dcutL,double dcutR,int bth,double dbcutL,double dbcutR)
{
	CFlyCapture2_EXDlg* pView = (CFlyCapture2_EXDlg *)m_hParent;
	int nw1 = rtL.Width();//8의 배수로 만들어야 함.
	int nh1 = rtL.Height();//8의 배수로 만들어야 함.
	int nw2 = rtR.Width();//8의 배수로 만들어야 함.
	int nh2 = rtR.Height();//8의 배수로 만들어야 함.
	int ret = RET_OK;
	CRect rtIns;
	BOOL bUseIns = TRUE;
	if(rtL.left == rtR.left){
		nw2 = 0;nh2 = 0;
	}

	//Falg Check
	BOOL bUse[20] = {0,};
	for(int k=0;k<4;k++){
		bUse[pView->m_pDlgSet->m_FormTab2->m_nWeldCheckFrame[k]] = TRUE;
	}
	double dave=0,dc=0;
	double dstd1=0,dvar1=0,dstd2=0,dvar2=0;int nmin1=0,nmax1=0,nmin2=0,nmax2=0;
// 	double dave11 = pView->m_Project.GetRoiInfo(pView->m_Project.m_MultiBuf[nstart],CAMERA_WIDTH,pView->m_Project.m_rtWeldRect[0], 1, 1,&dstd1,&nmin1,&nmax1,&dvar1,0);
// 	double dave12 = pView->m_Project.GetRoiInfo(pView->m_Project.m_MultiBuf[nend],CAMERA_WIDTH,pView->m_Project.m_rtWeldRect[0], 1, 1,&dstd2,&nmin2,&nmax2,&dvar2,0);


	int nWhite1 = 0,nWhite2=0,nblack1=0,nblack2=0,nWhitesum=0,nBlackSum=0;
	double dwth=0;
	double dbth=0;
// 	dwth[nstart] = dave11 * pView->m_pDlgSet->m_FormTab4->m_dWhiteCut*0.01;
// 	dwth[nend] = dave12 * pView->m_pDlgSet->m_FormTab4->m_dWhiteCut*0.01;
// 	dbth[nstart] = dave11 * pView->m_pDlgSet->m_FormTab4->m_nHoleTh*0.01;
// 	dbth[nend] = dave12 * pView->m_pDlgSet->m_FormTab4->m_nHoleTh*0.01;
// 	if(dave11>pView->m_pDlgSet->m_FormTab4->m_nTopCapWhiteCut)nstart=2;
// 	if(dave12>pView->m_pDlgSet->m_FormTab4->m_nTopCapWhiteCut)nend = 1;
	int nmaxframe[2] = {0,};
	for(int k=0;k<=pView->m_nMaxFrame;k++){
		if(bUse[k]==FALSE)continue;
		 nWhite1 = 0,nWhite2=0,nblack1=0,nblack2=0;
		 double dave11 = pView->m_Project.GetRoiInfo(pView->m_Project.m_MultiBuf[k],CAMERA_WIDTH,pView->m_Project.m_rtWeldRect[0], 1, 1,&dstd1,&nmin1,&nmax1,&dvar1,0);
		 dwth = dave11 * pView->m_pDlgSet->m_FormTab4->m_dWhiteCut*0.01;
		 dbth = dave11 * pView->m_pDlgSet->m_FormTab4->m_nHoleTh*0.01;

		if(nw1>0){
			for(int y=rtL.top;y<rtL.bottom;y++){
				for(int x=rtL.left;x<rtL.right;x++){
					if(x<rtLimit.left+10 || x>rtLimit.right-10)continue;
					if(pView->m_Project.m_MultiBuf[k][y*CAMERA_WIDTH+x]>dwth )//&& pView->m_Project.m_MultiBuf[k][y*CAMERA_WIDTH+x]>200)
						nWhite1++;
					if(pView->m_Project.m_MultiBuf[k][y*CAMERA_WIDTH+x]<dbth)
						nblack1++;
				}
			}
		}
		if(nw2>0){
			for(int y=rtR.top;y<rtR.bottom;y++){
				for(int x=rtR.left;x<rtR.right;x++){
					if(pView->m_Project.m_MultiBuf[k][y*CAMERA_WIDTH+x]>dwth)// && pView->m_Project.m_MultiBuf[k][y*CAMERA_WIDTH+x]>200)
						nWhite2++;
					if(pView->m_Project.m_MultiBuf[k][y*CAMERA_WIDTH+x]<dbth)
						nblack2++;
				}
			}
		}
		if(nWhitesum<nWhite1)
		{
			nWhitesum = nWhite1;
			nmaxframe[1] = k;
		}
		if(nBlackSum<nblack1)
		{
			nBlackSum = nblack1;
			nmaxframe[0] = k;
		}
	}

	pView->m_nMaxThFrame[0] = nmaxframe[0];
	pView->m_nMaxThFrame[1] = nmaxframe[1];
	nWhite1 = nWhitesum;
	nblack1 = nBlackSum;

	double dl=0,dr=0,dbl=0,dbr=0;
	if(nw1>0){
		dl= double(nWhite1)/double(nw1*nh1)*100;
		dbl = double(nblack1)/double(nw1*nh1)*100;
	}
	if(nw2>0){
		dr= double(nWhite2)/double(nw2*nh2)*100;
		dbr = double(nblack2)/double(nw1*nh1)*100;
	}
	//m_bHoleBad[0][0]=m_bHoleBad[0][1]=m_bHoleBad[1][0]=m_bHoleBad[1][1] = TRUE;
	pView->m_Project.m_dBlackPer[0] = dbl;
	pView->m_Project.m_dBlackPer[1] = dbr;
	pView->m_Project.m_dBlackPer1[0] = dl;
	pView->m_Project.m_dBlackPer1[1] = dr;
	pView->m_dData[13]=pView->m_Project.m_dBlackPer[0];
	pView->m_dData[14]=pView->m_Project.m_dBlackPer1[0];
	if(pView->m_pDlgSet->m_FormTab2->m_bLineWelding == TRUE || pView->m_pDlgSet->m_FormTab2->m_b2LineWelding == TRUE){
		pView->m_strResultValue[0].Format("R(Line1) : %.1f(%.0f)",pView->m_Project.m_dBlackPer[0],pView->m_pDlgSet->m_FormTab4->m_dLeftArea);
		pView->m_strResultValue[1].Format("S(Line1) : %.1f(%.0f)",pView->m_Project.m_dBlackPer1[0],pView->m_pDlgSet->m_FormTab4->m_dLeftArea1);
		pView->m_strResultValue[2].Format("R(Line2) : %.1f(%.0f)",pView->m_Project.m_dBlackPer[1],pView->m_pDlgSet->m_FormTab4->m_dRightArea);
		pView->m_strResultValue[3].Format("S(Line2) : %.1f(%.0f)",pView->m_Project.m_dBlackPer1[1],pView->m_pDlgSet->m_FormTab4->m_dRightArea1);
	}
	else{
		pView->m_strResultValue[0].Format("R(Left) : %.1f(%.0f)",pView->m_Project.m_dBlackPer[0],pView->m_pDlgSet->m_FormTab4->m_dLeftArea);
		pView->m_strResultValue[1].Format("S(Left) : %.1f(%.0f)",pView->m_Project.m_dBlackPer1[0],pView->m_pDlgSet->m_FormTab4->m_dLeftArea1);
		pView->m_strResultValue[2].Format("R(Right) : %.1f(%.0f)",pView->m_Project.m_dBlackPer[1],pView->m_pDlgSet->m_FormTab4->m_dRightArea);
		pView->m_strResultValue[3].Format("S(Right) : %.1f(%.0f)",pView->m_Project.m_dBlackPer1[1],pView->m_pDlgSet->m_FormTab4->m_dRightArea1);
	}


	//left/ right , blur / hole 
	//불량카운트만.
	if( dbcutL<dbl){
		pView->m_bHoleBad[0][0] = TRUE;
		pView->m_strBadInfo1[1]="Blur";
		pView->m_nBadCount[3]++;
		return NG_BLUR_WELD;
	}
	if(dcutL < dl ){
		pView->m_bHoleBad[1][0] = TRUE;
		pView->m_strBadInfo1[1]="Strong";
		pView->m_nBadCount[2]++;
		return NG_STRONG_WELD;
	}
	if( dbcutR<dbr){
		pView->m_bHoleBad[0][1] = TRUE;
		pView->m_strBadInfo1[1]="Blur";
		pView->m_nBadCount[3]++;
		return NG_BLUR_WELD;
	}
	if(dcutR <dr ){
		pView->m_bHoleBad[1][1] = TRUE;
		pView->m_strBadInfo1[1]="Strong";
		pView->m_nBadCount[2]++;
		return NG_STRONG_WELD;
	}
	/*if((dcutL < dl &&dcutR <dr) ||(dbcutL<dbl && dbcutR<dbr))
	ret = 3;*/
	bUseIns = pView->m_pDlgSet->m_FormTab4->m_bCheckDamage;
	if(bUseIns){
		rtIns = pView->m_Project.m_rtWeldPos;
		int nAvg = 0;
		int nCount = 0;
		int nValue = 0;
		int nTapTopEdge = (pView->m_Project.m_ptTopEdge[0].y + pView->m_Project.m_ptTopEdge[1].y) / 2;
		int nHisto[256] = {0,};
		for(int x=rtIns.left; x<rtIns.right; x++){
			for(int y=nTapTopEdge-20; y<pView->m_rtWeldBlob.bottom; y++){
				nValue = pView->m_Project.m_MultiBuf[9][y*CAMERA_WIDTH+x];
				nHisto[nValue]++;
				nAvg += nValue;
				nCount++;
			}
		}
		if(nCount <= 1)
			nCount =1;

		nAvg /= nCount;

		int nBlackCount = 0;
		int nBlackValue = -1;
		int nWhiteCount = 0;
		int nWhiteValue = -1;
		int nCountTh = pView->m_pDlgSet->m_FormTab4->m_nCheckDamageTh;//= 300;

		for(int i=0; i<256; i++){
			nBlackCount += nHisto[i];

			if(nBlackCount > nCountTh){
				nBlackValue = i;
				break;
			}
		}
		pView->m_Project.m_nBlackValue = nBlackValue;

		nCountTh = 600;
		for(int i=255; i>=0; i--){
			nWhiteCount += nHisto[i];

			if(nWhiteCount > nCountTh){
				nWhiteValue = i;
				break;
			}
		}
		pView->m_Project.m_nWhiteValue = nWhiteValue;

		if( nBlackValue < pView->m_pDlgSet->m_FormTab4->m_nCheckDamage){
			pView->m_strBadInfo1[1]="Tab Damage";
			pView->m_nBadCount[2]++;//left/ right , blur / hole 
			return NG_TAP_DAMAGE;
		}
	}
	//ret = 1;
	//pView->m_strBadInfo1[1]="TabDamage";
	//pView->m_nBadCount[2]++;

	//약용접 체크
	//1 등록 영역의 평균 밝기 구하기
	//2 특정 영상에서 픅색의 양 체크
	//m_Project.m_rtWeldPos
	if(pView->m_pDlgSet->m_FormTab4->m_bWeakWeld)
	{
		pView->m_nWeakWeld = -1;
		CRect rt1;
		rt1 = pView->m_Project.m_rtWeldPos;
		int nWeldAve1 = 0,nWeldCnt1=0,nWeldAve2 = 0,nWeldCnt2=0,nWeldAve3 = 0;
		int nselectFrame = pView->m_pDlgSet->m_FormTab2->m_nWeakWeldFrame;//9;m_nWeldCheckFrame[2]
		for(int i=rt1.top;i<rt1.bottom;i++){
			for(int j=rt1.left;j<rt1.right;j++){
				nWeldAve1 += pView->m_Project.m_MultiBuf[nselectFrame][i*CAMERA_WIDTH+j];
				nWeldCnt1++;
			}
		}
		for(int i=rtL.top;i<rtL.bottom;i++){
			for(int j=rtL.left;j<rtL.right;j++){
				nWeldAve2 += pView->m_Project.m_MultiBuf[nselectFrame][i*CAMERA_WIDTH+j];
				nWeldCnt2++;
			}
		}
		if(nWeldCnt1>0){
			nWeldAve1 /= nWeldCnt1;
			if(nWeldCnt2>0)		nWeldAve2 /= nWeldCnt2;
			nWeldAve3 = (nWeldAve1+nWeldAve2)*0.5;
			int nWeldTh = nWeldAve3*0.8;
			int nWeldBlackCnt = 0;
 			CRect rt2;
			rt2 = pView->m_rtWeldBlob;
			if(rt2.Height()<20)rt2.InflateRect(0,10);//탭이 꺽여서 흰분분이 보일경우 대비
			for(int i=rt2.top;i<rt2.bottom;i++){
				for(int j=rt2.left;j<rt2.right;j++){
					if(pView->m_Project.m_MultiBuf[nselectFrame][i*CAMERA_WIDTH+j]<nWeldTh)
					{
						nWeldBlackCnt++;
						if(pView->m_pDlgSet->m_FormTab2->m_bShowEdge)pView->m_Display[0].AddArrayPoint(CPoint(j,i));
					}
				}
			}
			pView->m_nWeakWeld = nWeldBlackCnt;
			int WhiteWeldCheck = 120;
			int nWeldWhiteCnt = 0;
			nselectFrame = pView->m_pDlgSet->m_FormTab2->m_nWeldCheckFrame[2];//8;
			for(int i=rt2.top;i<rt2.bottom;i++){
				for(int j=rt2.left;j<rt2.right;j++){
					if (WhiteWeldCheck<pView->m_Project.m_MultiBuf[nselectFrame][i*CAMERA_WIDTH+j])
					{
						nWeldWhiteCnt++;
					}
				}
			}
			pView->m_nWhiteWeldCheck = nWeldWhiteCnt;
		}
		//어두운 것만 체크한다.
		//약용접
		bUseIns = pView->m_pDlgSet->m_FormTab4->m_bWeakWeld;
		if(bUseIns){
			if(pView->m_nWeakWeld < pView->m_pDlgSet->m_FormTab4->m_nWeakWeld ||
				pView->m_nWeakWeld > pView->m_pDlgSet->m_FormTab4->m_nWeakWeld2){
					pView->m_strBadInfo1[1]="Weak Weld";
					pView->m_nBadCount[2]++;//left/ right , blur / hole 
					return NG_WEAK_WELD;

			}
			//용접 빨간색 영역 안에서 밝은것만 체크한다.
			if(pView->m_pDlgSet->m_FormTab4->m_nWeldCheck > pView->m_nWhiteWeldCheck){
				pView->m_strBadInfo1[1]="WeldCheck";
				pView->m_nBadCount[2]++;//left/ right , blur / hole 
				return NG_WELD;
			}

		}
	}

	//if(ret>0){
	//	pView->m_Project.m_rtHoleRect = pView->m_Project.m_rtWeldPos;
	//	pView->m_nBadHoleCount++;
	//	//pView->ImageShiftBad();
	//}
	return ret;//0 ok 1 left 2 right 3 all
}
/////종우추가
void CFormTab3::OnBnClickedButton19()
{
//	m_LightCtrl.LightSetMode(1);
}

void CFormTab3::OnRButtonDown(UINT nFlags, CPoint point)
{
	m_ev_rbutton_down.SetEvent();

	CDialog::OnRButtonDown(nFlags, point);
}

void CFormTab3::OnBnClickedCheckAuto()
{//m_bCkAuto
	UpdateData();
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CFormTab3::OnBnClickedButton20()//스트로브
{
	m_LightCtrl.LightSetMode(1);
}

void CFormTab3::OnBnClickedButton21()//PWM
{
	m_LightCtrl.LightSetMode(0);
}

void CFormTab3::OnCbnSelchangeComboWeldingNum()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

CPoint CFormTab3::FindWeldInTab(BYTE* fm,CRect rt1,CRect rtweld)
{
	CFlyCapture2_EXDlg* pView = (CFlyCapture2_EXDlg *)m_hParent;
	CPoint pt;
	pt.SetPoint(0,0);
	double dave=0,dc=0,dc1=0;
	for(int y=rt1.top;y<rt1.bottom;y++){
		for(int x=rt1.left;x<rt1.right;x++){
			dave += *(fm+y*CAMERA_WIDTH+x);dc++;
		}
	}
	if(dc>0)dave /= dc;
	else	return pt;

	int nMode=0;//0 white check 1 balck 2 black+white check
	int nth = dave* pView->m_pDlgSet->m_FormTab2->m_dWeldInTabWhiteTh;//1.4; //백색에 대해 th를 구한다.
	int nth1 = dave * 0.6;//흑색에 대한 th

	int nxsum=0,nysum=0, nxsum1=0,nysum1=0;dc=dc1=0;
	for(int y=rt1.top;y<rt1.bottom;y++){
		for(int x=rt1.left;x<rt1.right;x++){
			if(*(fm+y*CAMERA_WIDTH+x)>nth){
				nxsum += x;nysum+=y;dc++;
// 				if(pView->m_pDlgSet->m_FormTab2->m_bShowEdge)
// 				pView->m_Display[0].AddArrayPoint(CPoint(x,y));
		
			}
// 			if(*(fm+y*CAMERA_WIDTH+x)<nth1){
// 				nxsum1 += x;nysum1+=y;dc1++;
// 				if(pView->m_pDlgSet->m_FormTab2->m_bShowEdge)
// 				pView->m_Display[0].AddArrayPoint(CPoint(x,y),1);
// 			}
		}
	}
	//white 중심을 먼저 구하고 영역을 재설정한다.
	if(dc<1)return pt;//백색에 미 존재
	double dx1 = nxsum/dc;double dy1=nysum/dc;
	CRect rt2;
	rt2.SetRect(dx1-60,dy1-60,dx1+60,dy1+60);
	rt2 = pView->m_Project.CheckROI(rt2,CRect(100,100,CAMERA_WIDTH-100,CAMERA_HEIGHT-100));
	for(int y=rt2.top;y<rt2.bottom;y++){
		for(int x=rt2.left;x<rt2.right;x++){
 			if(*(fm+y*CAMERA_WIDTH+x)<nth1){
 				nxsum1 += x;nysum1+=y;dc1++;
//  				if(pView->m_pDlgSet->m_FormTab2->m_bShowEdge)
//  				pView->m_Display[0].AddArrayPoint(CPoint(x,y),1);
 			}
		}
	}

	if(nMode==1){
		if(dc1<1)return pt;//백색에 미 존재
		nxsum1/=dc1;nysum1/=dc1;
		pt.SetPoint(nxsum1,nysum1);
		return pt;
	}
	else if(nMode==2){
		nxsum += nxsum1;nysum += nysum1;dc += dc1;
	}
	if(dc<1)dc = 1;
	nxsum /= dc;nysum/=dc;
	
	pt.SetPoint(nxsum,nysum);
//용접위치 blob dialate
	int y11 = 0;
	int ncx=0,ncy=0,ncc=0;
	//탭 에지 라인을 먼저 찾는다.
	dave = 0,dc=0;
	int nmaxbright = 0;
	for(int y=rtweld.top;y<rtweld.bottom;y++){
		for(int x=rtweld.left;x<rtweld.right;x++){
			dave += *(fm+y*CAMERA_WIDTH+x);dc++;
			if(nmaxbright< *(fm+y*CAMERA_WIDTH+x))
				nmaxbright = *(fm+y*CAMERA_WIDTH+x);
		}
	}
	double dTh = (dave/dc)*pView->m_pDlgSet->m_FormTab2->m_dWeldInTabWhiteTh;
	if(dTh>=nmaxbright)dTh = nmaxbright-10;
	int nx1=0,ny1=0,nc1=0;
	for(int y=rtweld.top;y<rtweld.bottom;y++){
		int x11 = 0;
		for(int x=rtweld.left;x<rtweld.right;x++){
			if(x<pView->m_Project.m_ptLeftTop.x)continue;
			if(x>pView->m_Project.m_ptRightTop.x)continue;
			if(*(fm+y*CAMERA_WIDTH+x)> dTh){
				*(pView->m_Project.m_WeldBuf[4]+y11*400+x11) = 255;
				nx1+=x;ny1+=y;nc1++;
				if(pView->m_pDlgSet->m_FormTab2->m_bShowEdge)
					pView->m_Display[0].AddArrayPoint(CPoint(x,y));

			}
			else{
				*(pView->m_Project.m_WeldBuf[4]+y11*400+x11) = 0;
			}
			x11++;
		}
		y11++;
	}
	
	if(nc1<1)
		nc1=1;
	pt.SetPoint(nx1/nc1,ny1/nc1);

	return pt;
}

int CFormTab3::TabTopRefind(BYTE* fm,CPoint ptL,CPoint ptR,int nXoffset,int nYoffset,int * nx1, int * nx2)
{
	CFlyCapture2_EXDlg* pView = (CFlyCapture2_EXDlg *)m_hParent;
	*nx1 = 0;*nx2 = 0;
	CRect rt[2];
	rt[0].SetRect(0,0,0,0);rt[1].SetRect(0,0,0,0);
	if(ptL.x>0)		rt[0].SetRect(ptL.x,ptL.y-nYoffset,ptL.x+nXoffset,ptL.y+nYoffset);
	if(ptR.x>0)		rt[1].SetRect(ptR.x-nXoffset,ptR.y-nYoffset,ptR.x,ptR.y+nYoffset);
	rt[0] = pView->m_Project.CheckROI(rt[0],CRect(50,50,CAMERA_WIDTH-50,CAMERA_HEIGHT-10));
	rt[1] = pView->m_Project.CheckROI(rt[1],CRect(50,50,CAMERA_WIDTH-50,CAMERA_HEIGHT-10));
	int nproj[2][CAMERA_WIDTH],nmax[2]={0,},nmaxpos[2]={0,};
	for(int i=0;i<CAMERA_WIDTH;i++){nproj[0][i]=nproj[1][i]=0;}
	for(int k=0;k<2;k++){
		if(rt[k].Width()==0)continue;

		for(int y=rt[k].top;y<rt[k].bottom;y++){
			for(int x=rt[k].left;x<rt[k].right;x++){
				nproj[k][y]+= *(fm+y*CAMERA_WIDTH+x);
			}
			nproj[k][y] /= rt[k].Width();
			if(nmax[k]< nproj[k][y]){
				nmax[k] = nproj[k][y];nmaxpos[k]=y;
			}
		}
	}
	*nx1 = nmaxpos[0];
	*nx2 = nmaxpos[1];

	return 0;
}

void CFormTab3::OnBnClickedButtonLoad5()
{
	CFlyCapture2_EXDlg* pView = (CFlyCapture2_EXDlg *)m_hParent;

	//pView->Invalidate();
	pView->m_pDlgAiView->ImageView("C:\\TEST\\5.jpg");

}

void CFormTab3::OnBnClickedButton23()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_LightCtrl.LightSetMode(10);

}

//void CFormTab3::OnBnClickedButton25()
//{
//	CFlyCapture2_EXDlg* pView = (CFlyCapture2_EXDlg *)m_hParent;
//
//	pView->CellServerInit();
//}

//void CFormTab3::OnBnClickedButton26()
//{
//	CFlyCapture2_EXDlg* pView = (CFlyCapture2_EXDlg *)m_hParent;
//	pView->CellServerView();
//
//}

void CFormTab3::OnBnClickedButton25()
{
	CFlyCapture2_EXDlg* pView = (CFlyCapture2_EXDlg *)m_hParent;
	pView->CellServerView();
}
