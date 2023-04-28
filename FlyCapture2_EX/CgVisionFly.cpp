#include "StdAfx.h"
#include "CgVisionFly.h"
#include "resource.h"
#include "FlyCapture2_EXDlg.h"				//maindlg헤더파일을 include해야함.
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
	SetEvent(pVis->m_heventThreadDone); // 이벤트를 Set 함

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

	// 현재 카메라에서 설정가능한 Format7 정보, 최대 크기, 설정가능한 Pixelformat, 설정가능한 좌표등에 대한 정보 획득
	bool supported; // 현재 카메라가 Format7 지원하는지
	FlyCapture2::Format7Info F7Info; // 설정가능한 Format7 정보
	cam.GetFormat7Info(&F7Info, &supported); // 획득

	// ROI 설정
	FlyCapture2::Format7ImageSettings F7Settings;
	F7Settings.mode=FlyCapture2::MODE_0; // ROI 모드
	F7Settings.offsetX=nOffsetX;	// 센서 상에서 ROI 시작 좌표 (x,y)  
							// 주의) x는 F7Info.offsetHStepSize의 배수로 설정 가능, y 는 F7Info.offsetVStepSize 의 배수로 설정 가능 
							// 예)  F7Info.offsetHStepSize 값이 4일 경우 x는 4의 배수로 설정, (4, 8, 16, 20, ... 32, ...  72 등)
	F7Settings.offsetY=nOffsetY;
	F7Settings.width=nWidth;	// ROI 크기 (width, height) 
							// 주의) width는 F7Info.imageHStepSize의 배수로 설정 가능, height는 F7Info.imageVStepSize 의 배수로 설정 가능
	F7Settings.height=nHei;
	F7Settings.pixelFormat=FlyCapture2::PIXEL_FORMAT_MONO8; // 출력 데이터 포맷, 흑백카메라 일 경우 PIXEL_FORMAT_MONO8, 컬러카메라일 경우 PIXEL_FORMAT_RAW8
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
	m_heventThreadDone = CreateEvent(NULL, FALSE, FALSE, NULL); // 핸들 이벤트 생성

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
	CamSetProperty(AUTO_EXPOSURE, true, false, true, 0.);	//수치적용은안됨 OnOff,Auto 플래그는 변경됨
	CamSetProperty(SHARPNESS, true, false, true, 0.);		//적용안됨
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
	CWinThread *pThread = ::AfxBeginThread(threadGrab, this); // thread 생성
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
	CloseHandle(m_heventThreadDone); // 핸들 종료
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