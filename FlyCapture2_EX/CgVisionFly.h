// Fly 1.1

#pragma once

#include "FlyCapture2.h"
#include "FlyCapture2GUI.h"
using namespace FlyCapture2;

class CgVision
{
public:
	CgVision();
public:
	~CgVision(void);
protected:
	BOOL m_bGrab;
	LPVOID m_pMain;
	Camera m_Cam;
	Image m_Image;
	Image m_ImageColor;
	Error m_error;
	static UINT threadGrab(LPVOID pParam);

	void CamSetProperty(PropertyType propertyType, bool onOff, bool autoManualMode, bool absControl, float value);
	void CamSetFrameRate(FrameRate value);

public:
	double m_dTest;
	HANDLE m_heventThreadDone;
	void CamSetGain(float dGain);
	void CamSetShutter(float dShutter);
	void CamSetTrig(bool bTrig);

	bool InitCam(LPVOID pParam,int xOffset,int yOffset,int width,int height,bool bTrig,float dShtter,float dGain);
	void CamSetRoi(int nOffsetX, int nOffsetY, int nWidth, int nHei);
	void StopCapture();
	void StartCapture();
	void GetGrabData();
	bool SWTrigger();
	void Destroy(void);

	LPBITMAPINFO m_bitmapInfo;

};
