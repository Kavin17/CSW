#include "StdAfx.h"
#include "CellData.h"
#include "resource.h"			// view�� �����Ϸ��� �������
#include "FlyCapture2_EXDlg.h"	// view�� �����Ϸ��� �������

CFlyCapture2_EXDlg* pDlgView;

const int		nNameLength = 30;
const int		nTotalSpecParamCount[1] = {10};


CCellData::CCellData()
{

}


CCellData::~CCellData()
{
}

void CCellData::Init(LPVOID lParam)
{
	pDlgView = (CFlyCapture2_EXDlg*)lParam;
}

bool CCellData::GetSpecParam(int nVisionType)
{
	
	GetSpecParamCSW();

	return true;
}

bool CCellData::GetSpecParamCSW()
{

	int nFrameWidth = CAMERA_WIDTH;
	int nFrameHeight = CAMERA_HEIGHT;
	CRect rtInsRoi;
	//spec[0]:���� , spec[1]:����
	// [1] Can ����
	m_structSpecParamData[0].nInspType = static_cast<int>(JsonInspType::_DEFECT);
	strncpy_s(m_structSpecParamData[0].cDetectionName, _T("CANMSS"), nNameLength);
	rtInsRoi = pDlgView->m_pDlgSet->m_FormTab4->m_rtTacker[0];
	m_structSpecParamData[0].rtROI.SetRect(rtInsRoi.left, rtInsRoi.top, rtInsRoi.right, rtInsRoi.bottom);
	m_structSpecParamData[0].dSpec[0] = pDlgView->m_pDlgSet->m_FormTab4->m_nCheckCanValue;
	m_structSpecParamData[0].dSpec[1] = 0;

	// [2] T/C ����
	m_structSpecParamData[1].nInspType = static_cast<int>(JsonInspType::_DEFECT);
	strncpy_s(m_structSpecParamData[1].cDetectionName, _T("TCPMSS"), nNameLength);
	rtInsRoi = pDlgView->m_pDlgSet->m_FormTab4->m_rtTacker[1];
	m_structSpecParamData[1].rtROI.SetRect(rtInsRoi.left, rtInsRoi.top, rtInsRoi.right, rtInsRoi.bottom);
	m_structSpecParamData[1].dSpec[0] = 0;
	m_structSpecParamData[1].dSpec[1] =	pDlgView->m_pDlgSet->m_FormTab4->m_nCheckCanValue;
										
	//[3] Tab ��ġ
	m_structSpecParamData[2].nInspType = static_cast<int>(JsonInspType::_DIMENSION);
	strncpy_s(m_structSpecParamData[2].cDetectionName, _T("TABPOS_U1"), nNameLength);
	rtInsRoi = pDlgView->m_pDlgSet->m_FormTab4->m_rtTacker[2];
	m_structSpecParamData[2].rtROI.SetRect(rtInsRoi.left, rtInsRoi.top, rtInsRoi.right, rtInsRoi.bottom);
	m_structSpecParamData[2].dSpec[0] = pDlgView->m_pDlgSet->m_FormTab4->m_dTapTolY2 * -1;
	m_structSpecParamData[2].dSpec[1] = pDlgView->m_pDlgSet->m_FormTab4->m_dTapTolY1;

	//[4] ������
	m_structSpecParamData[3].nInspType = static_cast<int>(JsonInspType::_DEFECT);
	strncpy_s(m_structSpecParamData[3].cDetectionName, _T("TABOWD"), nNameLength);
	rtInsRoi = pDlgView->m_pDlgSet->m_FormTab4->m_rtTacker[7];
	m_structSpecParamData[3].rtROI.SetRect(rtInsRoi.left, rtInsRoi.top, rtInsRoi.right, rtInsRoi.bottom);
	m_structSpecParamData[3].dSpec[0] = 0;
	m_structSpecParamData[3].dSpec[1] = pDlgView->m_pDlgSet->m_FormTab4->m_dLeftArea1;

	//[5] Can-Cap �߽� �Ÿ� X
	m_structSpecParamData[4].nInspType = static_cast<int>(JsonInspType::_DIMENSION);
	strncpy_s(m_structSpecParamData[4].cDetectionName, _T("BCCDST_C1"), nNameLength);
	m_structSpecParamData[4].rtROI.SetRect(0, 0, nFrameWidth, nFrameHeight);
	m_structSpecParamData[4].dSpec[0] = 0;
	m_structSpecParamData[4].dSpec[1] = pDlgView->m_pDlgSet->m_FormTab4->m_dCapCandistX;
	
	//[5-2] Can-Cap �߽� �Ÿ� Y
	m_structSpecParamData[5].nInspType = static_cast<int>(JsonInspType::_DIMENSION);
	strncpy_s(m_structSpecParamData[5].cDetectionName, _T("BCCDST_C2"), nNameLength);
	m_structSpecParamData[5].rtROI.SetRect(0, 0, nFrameWidth, nFrameHeight);
	m_structSpecParamData[5].dSpec[0] = pDlgView->m_pDlgSet->m_FormTab4->m_dCenterOffset2;
	m_structSpecParamData[5].dSpec[1] = pDlgView->m_pDlgSet->m_FormTab4->m_dCenterOffset;

	//[6] �����
	m_structSpecParamData[6].nInspType = static_cast<int>(JsonInspType::_DEFECT);
	strncpy_s(m_structSpecParamData[6].cDetectionName, _T("TABWWD"), nNameLength);
	rtInsRoi = pDlgView->m_pDlgSet->m_FormTab4->m_rtTacker[7];
	m_structSpecParamData[6].rtROI.SetRect(rtInsRoi.left, rtInsRoi.top, rtInsRoi.right, rtInsRoi.bottom);
	m_structSpecParamData[6].dSpec[0] = pDlgView->m_pDlgSet->m_FormTab4->m_nWeakWeld;
	m_structSpecParamData[6].dSpec[1] = pDlgView->m_pDlgSet->m_FormTab4->m_nWeakWeld2;

	//[7] ���� ������
	m_structSpecParamData[7].nInspType = static_cast<int>(JsonInspType::_DEFECT);
	strncpy_s(m_structSpecParamData[7].cDetectionName, _T("TABBBD"), nNameLength);
	rtInsRoi = pDlgView->m_pDlgSet->m_FormTab4->m_rtTacker[7];
	m_structSpecParamData[7].rtROI.SetRect(rtInsRoi.left, rtInsRoi.top, rtInsRoi.right, rtInsRoi.bottom);
	m_structSpecParamData[7].dSpec[0] = 0;
	m_structSpecParamData[7].dSpec[1] = pDlgView->m_pDlgSet->m_FormTab4->m_dLeftArea;

	return true;
}

bool CCellData::GetSystemParam()
{
	return true;
}