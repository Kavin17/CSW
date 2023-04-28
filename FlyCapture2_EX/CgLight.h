#pragma once
#include "LightCtrlComm.h"


class CgLight
{
public:
	CgLight(void);
	~CgLight(void);
	void LightONONI(int nValue);
	void LightOFFONI(int nValue);
	void LightONWAVE();
	void LightOFFWAVE();
	void LightOnVisioncowork(int nValue, int nChannel);
	void LightOffVisioncowork(int nValue, int nChannel);
	void LightOnUVision(int nValue, int nChannel);
	void LightOffUVision(int nValue, int nChannel);
	void LightOnVisioncoworkAngleLight(CString strComNum, int nValue, int nChannel);
	void LightOffVisioncoworkAngleLight(CString strComNum, int nValue, int nChannel);
	void LightSetMode(int nMode); //종우선언

	CLightCtrlComm m_LightCtrl; 

	int m_nValue;
};
