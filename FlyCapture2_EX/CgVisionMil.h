// Mil 1.1

#pragma once

#include <mil.h>
#define NB_GRAB_MAX	1
#define DCF_FILE	"c:\\GLIM\GLIM.DCF"


//mil 8.0¿ë-------------------------------/*
#define MIL_INT		long
#define MIL_DOUBLE	double
//*///----------------------------------------


/* Hook data structure. */
typedef struct UserDataStruct
{
	MIL_ID        *MilImage; 
	MIL_ID        MilDigitizer;  
	MIL_ID        GrabEndEvent;
	long          NbGrabStart;
	long          NbGrabEnd;
	long          NbFrames;
	MIL_DOUBLE    Time;
} ;

class CgVision
{
public:
	CgVision();
public:
	~CgVision(void);
protected:
	BOOL m_bGrab;
	LPVOID m_pMain;
	long m_nBand;
	long m_nSizeX;
	long m_nSizeY;
	static UINT threadGrab(LPVOID pParam);
	static UINT threadOneGrab(LPVOID pParam);
	MIL_ID   MilApplication;
	MIL_ID   MilSystem     ;
	MIL_ID   MilDigitizer  ;
	MIL_ID   MilDisplay    ;
	MIL_ID   MilImage[NB_GRAB_MAX];
	MIL_ID   MilImageDisp  ;
	MIL_TEXT_CHAR FrameIndex[10];
	UserDataStruct UserStruct;

//	void CamSetProperty(PropertyType propertyType, bool onOff, bool autoManualMode, bool absControl, float value);
//	void CamSetFrameRate(FrameRate value);

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
	void StartCapture(BOOL bOneShot);
	void GetOneGrabData();
	void GetGrabData();
	bool SWTrigger();
	void Destroy(void);

//	LPBITMAPINFO m_bitmapInfo;

};
