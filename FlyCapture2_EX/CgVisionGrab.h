#pragma once

#include "multicam.h"

#define EURESYS_SURFACE_COUNT 3
//---------------------------------------------------------------------------
// Callback function declaration
void WINAPI GlobalCallback (PMCSIGNALINFO SigInfo);


class CgVision
{
public:
	CgVision();
public:
	~CgVision(void);
protected:
 	LPVOID m_pMain;
    MCHANDLE m_Channel;
    PVOID m_pCurrent;
    int m_SizeX;
    int m_SizeY;
    int m_BufferPitch;
    volatile BOOL m_bScreenRefreshCompleted;
	PUINT8 m_ImageBuffer[EURESYS_SURFACE_COUNT];
	

//	void CamSetProperty(PropertyType propertyType, bool onOff, bool autoManualMode, bool absControl, float value);
//	void CamSetFrameRate(FrameRate value);

public:

    void Callback (PMCSIGNALINFO SigInfo);	
	bool InitCam(LPVOID pParam,int xOffset,int yOffset,int width,int height,bool bTrig,float dShtter,float dGain);
	void CamSetRoi(int nOffsetX, int nOffsetY, int nWidth, int nHei);
	void StopCapture();
	void StartCapture();
	void AbortCapture();
	void GetGrabData(unsigned char* fm);
	void Destroy(void);

};
