#include "StdAfx.h"
#include "CgVisionFly.h"
#include "resource.h"
#include "FlyCapture2_EXDlg.h"				//maindlg��������� include�ؾ���.
#pragma comment(lib,"flycapture2.lib")

CgVision::CgVision()
{

}

CgVision::~CgVision(void)
{
}

UINT CgVision::threadGrab(LPVOID pParam)
{
	CgVision* pVis = (CgVision*)pParam;
	int Retval=0;

	while(pVis->m_bGrab)
		pVis->GetGrabData();
	SetEvent(pVis->m_heventThreadDone); // �̺�Ʈ�� Set ��

	return Retval;
}

void CgVision::GetGrabData()
{
	CFlyCapture2_EXDlg* pDlg = (CFlyCapture2_EXDlg*)m_pMain;

	CString str;
	TimeStamp timestamp;
	m_error = m_Cam.RetrieveBuffer(&m_Image);
	str = m_error.GetDescription();
	m_error = m_Image.Convert(PIXEL_FORMAT_MONO8, &m_ImageColor);
	//memcpy(fm,m_Image.GetData(),*m_Image.GetRows());
	timestamp = m_Image.GetTimeStamp();
	
	if(m_Image.GetCols()*m_Image.GetRows()>0)
	{
		TRACE("%.3lf \n",GetTickCount()-m_dTest);
		pDlg->LoopGrab(m_Image.GetData());
		TRACE("%.3lf \n",GetTickCount()-m_dTest);
	}
}
void CgVision::CamSetFrameRate(FrameRate value)
{
	Camera cam;
	cam.Connect();

    VideoMode videomode;
    FrameRate framerate;
	cam.GetVideoModeAndFrameRate(&videomode,&framerate);

	framerate = value;
	cam.SetVideoModeAndFrameRate(videomode,framerate);

	cam.Disconnect();
}

void CgVision::CamSetProperty(PropertyType propertyType, bool onOff, bool autoManualMode, bool absControl, float value)
{
	Camera cam;
	
	cam.Connect();
	if(propertyType==BRIGHTNESS || propertyType==AUTO_EXPOSURE || propertyType==GAMMA || propertyType==PAN 
		|| propertyType==SHUTTER || propertyType==GAIN)
	{
		// Camera property control
		Property prop;
		prop.type=propertyType;
		cam.GetProperty(&prop);
		prop.absControl=absControl;					//abs
		prop.autoManualMode=autoManualMode;			//auto
		prop.onOff=onOff;	
		if(prop.absControl)	
			prop.absValue=value;
		else				
			prop.valueA=(int)value;

		cam.SetProperty(&prop);
	}

	if(propertyType==TRIGGER_MODE)
	{
		// trigger control
		TriggerMode trgMode;
		cam.GetTriggerMode(&trgMode);
		trgMode.onOff=onOff;
		if(trgMode.onOff)
		{
			trgMode.polarity=0; // or 1
			trgMode.source=0; // IO 0 ~ 3
			trgMode.mode=0; // trigger mode 0
		}
		cam.SetTriggerMode(&trgMode);
	}
	cam.Disconnect();
}

void CgVision::CamSetRoi(int nOffsetX, int nOffsetY, int nWidth, int nHei)
{
	Camera cam;
	cam.Connect();

	// ���� ī�޶󿡼� ���������� Format7 ����, �ִ� ũ��, ���������� Pixelformat, ���������� ��ǥ� ���� ���� ȹ��
	bool supported; // ���� ī�޶� Format7 �����ϴ���
	FlyCapture2::Format7Info F7Info; // ���������� Format7 ����
	cam.GetFormat7Info(&F7Info, &supported); // ȹ��

	// ROI ����
	FlyCapture2::Format7ImageSettings F7Settings;
	F7Settings.mode=FlyCapture2::MODE_0; // ROI ���
	F7Settings.offsetX=nOffsetX;	// ���� �󿡼� ROI ���� ��ǥ (x,y)  
							// ����) x�� F7Info.offsetHStepSize�� ����� ���� ����, y �� F7Info.offsetVStepSize �� ����� ���� ���� 
							// ��)  F7Info.offsetHStepSize ���� 4�� ��� x�� 4�� ����� ����, (4, 8, 16, 20, ... 32, ...  72 ��)
	F7Settings.offsetY=nOffsetY;
	F7Settings.width=nWidth;	// ROI ũ�� (width, height) 
							// ����) width�� F7Info.imageHStepSize�� ����� ���� ����, height�� F7Info.imageVStepSize �� ����� ���� ����
	F7Settings.height=nHei;
	F7Settings.pixelFormat=FlyCapture2::PIXEL_FORMAT_MONO8; // ��� ������ ����, ���ī�޶� �� ��� PIXEL_FORMAT_MONO8, �÷�ī�޶��� ��� PIXEL_FORMAT_RAW8
	cam.SetFormat7Configuration(&F7Settings,900.f);
//	cam.SetFormat7Configuration(&F7Settings,6.5f);
	///////////////////////////////
	cam.Disconnect();
}

bool CgVision::SWTrigger()
{
	m_dTest = GetTickCount();
    const unsigned int k_softwareTrigger = 0x62C;
    const unsigned int k_fireVal = 0x80000000;
    Error error;    

    error = m_Cam.WriteRegister( k_softwareTrigger, k_fireVal );
    if (error != PGRERROR_OK)
    {
        return false;
    }
	//GetGrabData();
	//m_error = pCam->RetrieveBuffer( &m_Image );

    return true;
}

bool CgVision::InitCam(LPVOID pParam,int xOffset,int yOffset,int width,int height,bool bTrig,float dShtter,float dGain)
{
	m_pMain = pParam;
	m_heventThreadDone = CreateEvent(NULL, FALSE, FALSE, NULL); // �ڵ� �̺�Ʈ ����

	m_bitmapInfo = NULL;
	m_bitmapInfo = (LPBITMAPINFO)( new BYTE [sizeof(BITMAPINFOHEADER) + 256 * sizeof(RGBQUAD)]);
	m_bitmapInfo->bmiHeader.biSize=sizeof(BITMAPINFOHEADER);
	m_bitmapInfo->bmiHeader.biPlanes=1;
	m_bitmapInfo->bmiHeader.biCompression=BI_RGB;
	m_bitmapInfo->bmiHeader.biXPelsPerMeter=100;
	m_bitmapInfo->bmiHeader.biYPelsPerMeter=100;
	m_bitmapInfo->bmiHeader.biClrUsed=256;
	m_bitmapInfo->bmiHeader.biClrImportant=0;
	m_bitmapInfo->bmiHeader.biBitCount=8;
	m_bitmapInfo->bmiHeader.biSizeImage = 0;
	m_bitmapInfo->bmiHeader.biWidth  = width;
	m_bitmapInfo->bmiHeader.biHeight = height;

	for(int i=0; i<256; i++)
	{
		m_bitmapInfo->bmiColors[i].rgbBlue = (BYTE)i;
		m_bitmapInfo->bmiColors[i].rgbGreen = (BYTE)i;
		m_bitmapInfo->bmiColors[i].rgbRed = (BYTE)i;
		m_bitmapInfo->bmiColors[i].rgbReserved = (BYTE)0;
	}	

	BusManager xBusManager;
	PGRGuid xGuid;
	FC2Config xConfig;	
	CameraBase* pxCamerabase;  

	m_error = xBusManager.GetCameraFromIndex(0,&xGuid);
	m_error = m_Cam.Connect(&xGuid);

	m_error = m_Cam.GetConfiguration(&xConfig);
	xConfig.grabMode = BUFFER_FRAMES;
	xConfig.numBuffers = 20;
	m_error = m_Cam.SetConfiguration(&xConfig);

    if (m_error != PGRERROR_OK)
        return false;

	pxCamerabase = new Camera;		
	pxCamerabase->Connect(0);

	if(pxCamerabase) delete pxCamerabase;
  
	CamSetProperty(BRIGHTNESS, true, false, true, 0.);		//OK
	CamSetProperty(AUTO_EXPOSURE, true, false, true, 0.);	//��ġ�������ȵ� OnOff,Auto �÷��״� �����
	CamSetProperty(SHARPNESS, true, false, true, 0.);		//����ȵ�
	CamSetProperty(GAMMA, true, false, false, 0);			//OK
	CamSetProperty(PAN, true, false, true, 12);				//OK
	CamSetProperty(TILT, true, false, true, 12);			//OK
	CamSetProperty(SHUTTER, true, false, true, dShtter);	//OK
	CamSetProperty(GAIN, true, false, true, dGain);			//OK
	CamSetFrameRate(FRAMERATE_15);						//
	CamSetProperty(TRIGGER_MODE, bTrig, NULL, NULL, NULL);	//OK
	CamSetRoi(xOffset, yOffset, width, height);	

	return true;
}

void CgVision::StartCapture()
{
	m_bGrab = TRUE;
	CWinThread *pThread = ::AfxBeginThread(threadGrab, this); // thread ����
	m_error = m_Cam.StartCapture();
}

void CgVision::StopCapture()
{
	m_bGrab = FALSE;
	m_error = m_Cam.StopCapture();
}

void CgVision::Destroy(void)
{
	StopCapture();
//	Sleep(1000);
	DWORD dwRet = WaitForSingleObject(m_heventThreadDone, 3000);
	CloseHandle(m_heventThreadDone); // �ڵ� ����
	delete[] m_bitmapInfo;
//	StopCapture();
}

void CgVision::CamSetGain(float dGain)
{
	CamSetProperty(GAIN, true, false, true, dGain);		
}

void CgVision::CamSetShutter(float dShutter)
{
	CamSetProperty(SHUTTER, true, false, true, dShutter);		
}

void CgVision::CamSetTrig(bool bTrig)
{
	CamSetProperty(TRIGGER_MODE, bTrig, NULL, NULL, NULL);	//OK		
}