#include "StdAfx.h"
#include "CgLight.h"
#include "LightCtrlComm.h"


CgLight::CgLight(void)
: m_nValue(0)
{
}

CgLight::~CgLight(void)
{
}

void CgLight::LightOnVisioncowork(int nValue, int nChannel)
{
	if(m_LightCtrl.Open(_T("COM1")))//종우 COM2로 변경해야한다.
	{
		CString strCmd(_T(""));
		//	int nChannel = m_ctrlChannel.GetCurSel() + 1; 
		int nLevel = nValue;//GetDlgItemInt(IDC_WRITE_LEVEL); 

		for(int i=1; i<=4;i++)
		{
			if(nChannel != i)
				continue;

			strCmd.Format(_T("%cN%d%03d%c%c%c"), 0x02, i, nLevel,0x0D,0x0A, 0x03); 
			m_LightCtrl.WriteString(strCmd); 
		}
	}
}

void CgLight::LightOffVisioncowork(int nValue, int nChannel)
{
	if(m_LightCtrl.Open(_T("COM2")))
	{
		CString strCmd(_T(""));
		//	int nChannel = m_ctrlChannel.GetCurSel() + 1; 
		int nLevel = nValue;//GetDlgItemInt(IDC_WRITE_LEVEL); 

		for(int i=1; i<=4;i++)
		{
			if(nChannel != i)
				continue;

			strCmd.Format(_T("%cE%d%c%c%c"), 0x02, i,0x0D,0x0A, 0x03); 
			m_LightCtrl.WriteString(strCmd); 
		}
	}
}

void CgLight::LightOnUVision(int nValue, int nChannel)
{
	if(m_LightCtrl.Open(_T("COM9")))
	{
		CString strCmd(_T(""));
		int nLevel = nValue;

		// WDccbbvvv\n
		strCmd.Format(_T("WD%02d00%03d%c"), nChannel, nLevel, 0x0A); 
		m_LightCtrl.WriteString(strCmd); 
	}
}

void CgLight::LightOffUVision(int nValue, int nChannel)
{
	if(m_LightCtrl.Open(_T("COM9")))
	{
		CString strCmd(_T(""));
		int nLevel = nValue;

		// WDccbbvvv\n
		strCmd.Format(_T("WD%02d00%03d%c"), nChannel, nLevel, 0x0A); 
		m_LightCtrl.WriteString(strCmd); 
	}
}

void CgLight::LightONONI(int nValue)
{
	if(m_LightCtrl.Open(_T("COM5")))
	{
		CString strCmd(_T(""));
		//	int nChannel = m_ctrlChannel.GetCurSel() + 1; 
		int nLevel = nValue;//GetDlgItemInt(IDC_WRITE_LEVEL); 

		int nOnOff = 1; // 1=true, 0=false
		for(int i=1; i<=2;i++)
		{
			strCmd.Format(_T("%c%02dRD%c"), 0x02, i, 0x03); 
			m_LightCtrl.WriteString(strCmd); 

			strCmd.Format(_T("%c%02dWR%03d%c"), 0x02, i, nLevel, 0x03); 
			m_LightCtrl.WriteString(strCmd); 

			strCmd.Format(_T("%c%02dON%d%c"), 0x02, i, nOnOff, 0x03); 
			m_LightCtrl.WriteString(strCmd); 
		}
	}
}
void CgLight::LightOFFONI(int nValue)
{
	if(m_LightCtrl.Open(_T("COM5")))
	{
		CString strCmd(_T(""));
		//	int nChannel = m_ctrlChannel.GetCurSel() + 1; 
		int nLevel = nValue;//GetDlgItemInt(IDC_WRITE_LEVEL); 

		int nOnOff = 0; // 1=true, 0=false
		for(int i=1; i<=2;i++)
		{
			strCmd.Format(_T("%c%02dRD%c"), 0x02, i, 0x03); 
			m_LightCtrl.WriteString(strCmd); 

			strCmd.Format(_T("%c%02dWR%03d%c"), 0x02, i, nLevel, 0x03); 
			m_LightCtrl.WriteString(strCmd); 

			strCmd.Format(_T("%c%02dON%d%c"), 0x02, i, nOnOff, 0x03); 
			m_LightCtrl.WriteString(strCmd); 
		}
	}
}

void CgLight::LightONWAVE() 
{
	CString strCmd(_T(""));

	int nOnOff = 1; // 1=true, 0=false

	//조명 on off
	strCmd.Format(_T("%cSA%d%c"), 0x02, nOnOff, 0x03); 
	m_LightCtrl.WriteString(strCmd); 
	//조명 밝기 설정 0~100%
	strCmd.Format(_T("%cWA100%c"), 0x02,0x03); 
	m_LightCtrl.WriteString(strCmd); 

}

void CgLight::LightOFFWAVE()
{
	CString strCmd(_T(""));

	int nOnOff = 0; // 1=true, 0=false
	
	//조명 밝기 설정 0~100%
	strCmd.Format(_T("%cWA0%c"), 0x02,0x03); 
	m_LightCtrl.WriteString(strCmd);
	//조명 on off
	strCmd.Format(_T("%cSA%d%c"), 0x02, nOnOff, 0x03); 
	m_LightCtrl.WriteString(strCmd); 	 
}
////////////////////////종우추가
void CgLight::LightSetMode(int nMode)
{
	CString strCmd(_T(""));
	strCmd.Format(_T("%cD%d%d%d%c%c%c"), 0x02, 0, 1, 5, 0x0D, 0x0A, 0x03); 
	m_LightCtrl.WriteString(strCmd); 				

}
void CgLight::LightOnVisioncoworkAngleLight(CString strComNum, int nValue, int nChannel)
{
	if(m_LightCtrl.Open(strComNum))
	{
		CString strCmd(_T(""));
		int nLevel = nValue;

		//strCmd.Format(_T("%cN%d%03d%c%c%c"), 0x02, nChannel, nLevel,0x0D,0x0A, 0x03); 
		strCmd.Format(_T("%cN%02d%03d%c%c%c"), 0x02, nChannel, nLevel,0x0D,0x0A, 0x03); 
		m_LightCtrl.WriteString(strCmd); 
	}
}

void CgLight::LightOffVisioncoworkAngleLight(CString strComNum, int nValue, int nChannel)
{
	if(m_LightCtrl.Open(strComNum))
	{
		CString strCmd(_T(""));
		int nLevel = 0;

		//strCmd.Format(_T("%cE%d%c%c%c"), 0x02, nChannel, 0x0D,0x0A, 0x03); 
		strCmd.Format(_T("%cN%02d%03d%c%c%c"), 0x02, nChannel, nLevel,0x0D,0x0A, 0x03); 
		m_LightCtrl.WriteString(strCmd); 
	}
}