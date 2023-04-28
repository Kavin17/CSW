// FormTab5.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "FlyCapture2_EX.h"
#include "FlyCapture2_EXDlg.h"
#include "FormTab5.h"
//#include "FormTab3.h"
#include "DlgSet.h"
#include "ini.h"

// CFormTab5 대화 상자입니다.


IMPLEMENT_DYNAMIC(CFormTab5, CDialog)

/*
D500	1	30	Recipe Number	설비 Recipe 번호
D502	0	Max	Feeding Stroke	1축 Robot 이송거리
D504	0%	100%	Feeding Speed	1축 Robot 이송속도
D506	0	1000	포장매수	포장매수 (아래 Comment 확인)
D508	0.0 	5.0 	Air Blower time	투입부 Air 동작 시간(1 Cycle)
D510	0	1000	OK 매수	현재까지의 OK 수량
D512	0	1000	NG 매수	현재까지의 NG 수량
D514	0	1000	보류 매수	현재까지의 보류 수량

////////////////////////////


D550	0	30	이상발생 번호	설비 이상 발생 번호
D552			양품수량 감소	사용자가 뺀필름 총수량
D554			보류수량 감소	"
D556			불량수량 감소	"



*/

CFormTab5::CFormTab5(CWnd* pParent /*=NULL*/)
	: CDialog(CFormTab5::IDD, pParent)
{
	m_hParent = pParent;
	m_nDLScale = 0;
	m_nDLDelayCount = 10;
	for (int i = 0; i < 2; i++) {
		m_dDLSpecTcCanX[i] = 0.0;
		m_dDLSpecTcCanY[i] = 0.0;
		m_dDLSpecTcWeldY[i] = 0.0;
		m_dDLSpecTcTabY[i] = 0.0;
		m_dDLSpecTcTabLeft[i] = 0.0;
		m_dDLSpecTcTabRight[i] = 0.0;
		m_dDLSpecWeldCanY[i] = 0.0;
		m_dDLSpecTabEdgeGapY[i] = 0.0;
		m_dDLSpecStrongWeldArea[i] = 0.0;
		m_dDLSpecStrongWeldLength[i] = 0.0;
		m_dDLSpecSootWeldArea[i] = 0.0;
		m_dDLSpecSootWeldLength[i] = 0.0;
		m_dDLSpecWeldArea[i] = 0.0;
		m_dDLSpecWeldLength[i] = 0.0;
	}
	m_bCalibration = FALSE;

}

CFormTab5::~CFormTab5()
{
}

void CFormTab5::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	
	
	DDX_Text(pDX, IDC_EDIT_DL_SCALE, m_nDLScale);
	DDX_Text(pDX, IDC_EDIT_DL_SPEC_CAP_CAN_X, m_dDLSpecTcCanX[1]);
	DDX_Text(pDX, IDC_EDIT_DL_SPEC_CAP_CAN_Y_MIN, m_dDLSpecTcCanY[0]);
	DDX_Text(pDX, IDC_EDIT_DL_SPEC_CAP_CAN_Y_MAX, m_dDLSpecTcCanY[1]);
	DDX_Text(pDX, IDC_EDIT_DL_SPEC_CAP_WELD_Y_MIN, m_dDLSpecTcWeldY[0]);
	DDX_Text(pDX, IDC_EDIT_DL_SPEC_CAP_WELD_Y_MAX, m_dDLSpecTcWeldY[1]);
	DDX_Text(pDX, IDC_EDIT_DL_SPEC_CAP_TAP_TOP_Y, m_dDLSpecTcTabY[1]);
	DDX_Text(pDX, IDC_EDIT_DL_SPEC_CAP_TAP_LEFT_X, m_dDLSpecTcTabLeft[1]);
	DDX_Text(pDX, IDC_EDIT_DL_SPEC_CAP_TAP_RIGHT_X, m_dDLSpecTcTabRight[1]);
	DDX_Text(pDX, IDC_EDIT_DL_SPEC_CAN_WELD_Y_MIN, m_dDLSpecWeldCanY[0]);
	DDX_Text(pDX, IDC_EDIT_DL_SPEC_CAN_WELD_Y_MAX, m_dDLSpecWeldCanY[1]);
	DDX_Text(pDX, IDC_EDIT_DL_SPEC_TAB_L_R_GAP, m_dDLSpecTabEdgeGapY[1]);
	DDX_Text(pDX, IDC_EDIT_DL_SPEC_STRONG_WELD_AREA, m_dDLSpecStrongWeldArea[1]);
	DDX_Text(pDX, IDC_EDIT_DL_SPEC_STRONG_WELD_LENGTH, m_dDLSpecStrongWeldLength[1]);
	DDX_Text(pDX, IDC_EDIT_DL_SPEC_SOOT_WELD_AREA, m_dDLSpecSootWeldArea[1]);
	DDX_Text(pDX, IDC_EDIT_DL_SPEC_SOOT_WELD_LENGTH, m_dDLSpecSootWeldLength[1]);
	DDX_Text(pDX, IDC_EDIT_DL_SPEC_WELD_WIDTH, m_dDLSpecWeldArea[1]);
	DDX_Text(pDX, IDC_EDIT_DL_SPEC_WELD_LENGTH, m_dDLSpecWeldLength[1]);
	DDX_Text(pDX, IDC_EDIT_DL_DELAY_COUNT, m_nDLDelayCount);
	DDX_Check(pDX, IDC_CHECK_DL_SAVE_ALL, m_bDLSaveAll);
	DDX_Check(pDX, IDC_CHECK_DL_SAVE_NG, m_bDLSaveNg);

}
void CFormTab5::UpdateIni(BOOL bLoad)
{
	CFlyCapture2_EXDlg *pDlg = (CFlyCapture2_EXDlg*)m_hParent;

	if (bLoad == FALSE) {
		UpdateData(TRUE);
		pDlg->SetAiSpec();

	}
	CString strPath;
	strPath.Format("c:\\Glim\\Model\\%s.ini", pDlg->m_pDlgSet->m_strSelectProduct);

	CIni ini1(strPath, "DL_Parameter", 0);
	ini1.SER_GETD(bLoad, m_nDLScale, 0.0);
	ini1.SER_GETD(bLoad, m_dDLSpecTcCanX[1], 0.0);
	ini1.SER_GETD(bLoad, m_dDLSpecTcCanY[0], 0.0);
	ini1.SER_GETD(bLoad, m_dDLSpecTcCanY[1], 0.0);
	ini1.SER_GETD(bLoad, m_dDLSpecTcWeldY[0], 0.0);
	ini1.SER_GETD(bLoad, m_dDLSpecTcWeldY[1], 0.0);
	ini1.SER_GETD(bLoad, m_dDLSpecTcTabY[1], 0.0);
	ini1.SER_GETD(bLoad, m_dDLSpecTcTabLeft[1], 0.0);
	ini1.SER_GETD(bLoad, m_dDLSpecTcTabRight[1], 0.0);
	ini1.SER_GETD(bLoad, m_dDLSpecWeldCanY[0], 0.0);
	ini1.SER_GETD(bLoad, m_dDLSpecWeldCanY[1], 0.0);
	ini1.SER_GETD(bLoad, m_dDLSpecTabEdgeGapY[1], 0.0);
	ini1.SER_GETD(bLoad, m_dDLSpecStrongWeldArea[1], 0.0);
	ini1.SER_GETD(bLoad, m_dDLSpecStrongWeldLength[1], 0.0);
	ini1.SER_GETD(bLoad, m_dDLSpecSootWeldArea[1], 0.0);
	ini1.SER_GETD(bLoad, m_dDLSpecSootWeldLength[1], 0.0);
	ini1.SER_GETD(bLoad, m_dDLSpecWeldArea[1], 0.0);
	ini1.SER_GETD(bLoad, m_dDLSpecWeldLength[1], 0.0);
	ini1.SER_GETD(bLoad, m_nDLDelayCount, 10);
	ini1.SER_GETD(bLoad, m_bDLSaveAll, FALSE);
	ini1.SER_GETD(bLoad, m_bDLSaveNg, TRUE);
	
	if (bLoad) {	
		UpdateData(FALSE);
	}

}

BEGIN_MESSAGE_MAP(CFormTab5, CDialog)
	ON_WM_TIMER()

	ON_BN_CLICKED(IDC_CHECK_DL_SAVE_ALL, &CFormTab5::OnBnClickedCheckDlSaveAll)
	ON_BN_CLICKED(IDC_CHECK_DL_SAVE_NG, &CFormTab5::OnBnClickedCheckDlSaveNg)
END_MESSAGE_MAP()


// CFormTab5 메시지 처리기입니다.

BOOL CFormTab5::OnInitDialog()
{
	CDialog::OnInitDialog();	

	CFlyCapture2_EXDlg *pView = (CFlyCapture2_EXDlg*)m_hParent;

//	SetTimer(2001,100,NULL);

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CFormTab5::OnTimer(UINT_PTR nIDEvent)
{
	CFlyCapture2_EXDlg *pView = (CFlyCapture2_EXDlg*)m_hParent;

	

	CDialog::OnTimer(nIDEvent);
}



void CFormTab5::OnBnClickedCheckDlSaveAll()
{
	CFlyCapture2_EXDlg *pView = (CFlyCapture2_EXDlg*)m_hParent;
	UpdateData();
	if (m_bDLSaveAll) {
		m_bDLSaveNg = FALSE;
		UpdateData(FALSE);

	}
}


void CFormTab5::OnBnClickedCheckDlSaveNg()
{
	CFlyCapture2_EXDlg *pView = (CFlyCapture2_EXDlg*)m_hParent;
	UpdateData();
	if (m_bDLSaveNg) {
		m_bDLSaveAll = FALSE;
		UpdateData(FALSE);
	}
}
