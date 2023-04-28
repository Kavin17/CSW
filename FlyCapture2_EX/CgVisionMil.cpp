#include "StdAfx.h"
#include "CgVisionMil.h"
#include "resource.h"
#include "FlyCapture2_EXDlg.h"				//maindlg헤더파일을 include해야함.



MIL_INT MFTYPE GrabStart(MIL_INT, MIL_ID, void MPTYPE *);
MIL_INT MFTYPE GrabEnd(MIL_INT, MIL_ID, void MPTYPE *);

CgVision::CgVision()
{
	m_nSizeX = CAMERA_WIDTH;
	m_nSizeY = CAMERA_HEIGHT;
	m_nBand = 8;
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

UINT CgVision::threadOneGrab(LPVOID pParam)
{
	CgVision* pVis = (CgVision*)pParam;
	int Retval=0;


	pVis->GetOneGrabData();;
	/* Wait for end of last grab. */
	//MdigGrabWait(MilDigitizer, M_GRAB_END);

	return Retval;
}

void CgVision::GetOneGrabData()
{
	CFlyCapture2_EXDlg* pDlg = (CFlyCapture2_EXDlg*)m_pMain;

	MdigGrab(MilDigitizer, MilImage[0]);
    MdigGrabWait(MilDigitizer, M_GRAB_END);

	unsigned char * pImage;
	pImage = (unsigned char*)MbufInquire(MilImage[0], M_HOST_ADDRESS, M_NULL);
	pDlg->LoopGrab(pImage);
	//TRACE("\nGet[]");

}
void CgVision::GetGrabData()
{
	CFlyCapture2_EXDlg* pDlg = (CFlyCapture2_EXDlg*)m_pMain;

	CString str;
   /* At this point the CPU is free to do other tasks and the sequence will be
    * grabbed during this time. Here, the CPU will print and draw the   
    * index of the buffer grabbed before copying it to display.
    */
      long GrabEndIndex;
     
     /* Wait end of grab event */
     MthrWait(UserStruct.GrabEndEvent, M_EVENT_WAIT, M_NULL);

     /* Print the current grab index. */
//     GrabEndIndex = (UserStruct.NbGrabEnd-1+UserStruct.NbFrames)%UserStruct.NbFrames;
     GrabEndIndex = (UserStruct.NbGrabEnd-1+UserStruct.NbFrames)%UserStruct.NbFrames;
	 TRACE("\nframe=%ld[%d]",GrabEndIndex,GrabEndIndex%UserStruct.NbFrames);
	 if (GrabEndIndex >= 0)
     {
        //MosPrintf(MIL_TEXT("\rBuffer #%ld grabbed.   "),GrabEndIndex);
         //MosSprintf(FrameIndex, 10, MIL_TEXT("%ld"), GrabEndIndex);
         //MgraText(M_DEFAULT, MilImage[GrabEndIndex], 32, 32, FrameIndex);
         //MbufCopy(MilImage[GrabEndIndex],MilImageDisp);
		 unsigned char * pImage;
		 pImage = (unsigned char*)MbufInquire(MilImage[GrabEndIndex], M_HOST_ADDRESS, M_NULL);
		 pDlg->LoopGrab(pImage);
		 //TRACE("\nGet[%d]",GrabEndIndex);
      }
}
/*
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
*/
void CgVision::CamSetRoi(int nOffsetX, int nOffsetY, int nWidth, int nHei)
{
	/*
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
	*/
}

bool CgVision::SWTrigger()
{
/*	m_dTest = GetTickCount();
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
*/
    return true;
}

bool CgVision::InitCam(LPVOID pParam,int xOffset,int yOffset,int width,int height,bool bTrig,float dShtter,float dGain)
{
	m_pMain = pParam;
	m_heventThreadDone = CreateEvent(NULL, FALSE, FALSE, NULL); // 핸들 이벤트 생성

   MIL_DOUBLE TimeWait = 0.0;
   long NbFrames = 0, n=0;
         
   //--
	MappAlloc(M_DEFAULT, &MilApplication);
	MsysAlloc(M_SYSTEM_SOLIOS,  M_DEFAULT, M_DEFAULT, &MilSystem);	
	if(M_NULL == MdigAlloc(MilSystem, M_DEV0, "c:\\GLIM\\glim.dcf", M_DEFAULT, &MilDigitizer)){
		return false;
	}
	MdigControl(MilDigitizer, M_GRAB_TIMEOUT, M_INFINITE);
	// CC1 Output를 정의해 주지 않으면, CC출력이 되지 않음. 반드시 추가해 주어야 함.
	MdigControl(MilDigitizer, M_CAMERALINK_CC1_SOURCE, M_GRAB_EXPOSURE+M_TIMER1);
	MdigControl(MilDigitizer, M_GRAB_MODE, M_ASYNCHRONOUS);
	//--

   /* Allocate and clear sequence and display images. */
   MappControl(M_ERROR, M_PRINT_DISABLE);
   for (NbFrames=0; NbFrames<NB_GRAB_MAX; NbFrames++)
      {
	  MbufAlloc2d(MilSystem, m_nSizeX, m_nSizeY, 8+M_UNSIGNED, M_IMAGE+M_GRAB+M_DISP, &MilImage[NbFrames]); //!!
	  MbufClear(MilImage[NbFrames], 0xff);

      }
   MbufAlloc2d(MilSystem, m_nSizeX, m_nSizeY, 8+M_UNSIGNED, M_IMAGE+M_GRAB+M_DISP, &MilDisplay);
   MbufAlloc2d(MilSystem, m_nSizeX, m_nSizeY, 8+M_UNSIGNED, M_IMAGE+M_GRAB+M_DISP, &MilImageDisp);
   MbufClear(MilImageDisp, 0xff);

   MappControl(M_ERROR, M_PRINT_ENABLE);

   /* Free buffers to leave space for possible temporary buffers. */
   /*for (n=0; n<2 && NbFrames; n++)
      {
      NbFrames--;
      MbufFree(MilImage[NbFrames]);
      }
	*/
   /* MIL event allocation for grab end hook. */
   MthrAlloc(MilSystem, M_EVENT, M_DEFAULT, M_NULL, M_NULL, &UserStruct.GrabEndEvent);
   
   /* Initialize hook structure. */
   UserStruct.MilDigitizer = MilDigitizer;
   UserStruct.MilImage     = MilImage;
   UserStruct.NbGrabStart  = 0;
   UserStruct.NbGrabEnd    = 0;
   UserStruct.NbFrames     = NbFrames;
   UserStruct.Time         =  0;

   /* Grab on display. */
   MdigGrabContinuous(MilDigitizer, MilImageDisp);
  
   /* Print a message, wait for a key press and stop the grab. */
   MdigHalt(MilDigitizer);
  
   /* Hook functions to grab the sequence. */
   //MdigHookFunction(MilDigitizer, M_GRAB_START, GrabStart, (void *)(&UserStruct));
   //MdigHookFunction(MilDigitizer, M_GRAB_END,   GrabEnd,   (void *)(&UserStruct));

   /* Put digitizer in asynchronous mode. */
   MdigControl(MilDigitizer, M_GRAB_MODE, M_ASYNCHRONOUS);

   /* Start sequence with a grab in the first buffer. */
   //MdigGrab(MilDigitizer, MilImage[0]);
   
   return 0;
}

void CgVision::StartCapture()
{
	m_bGrab = TRUE;
	CWinThread *pThread = ::AfxBeginThread(threadGrab, this); // thread 생성

   UserStruct.NbGrabStart  = 0;
   UserStruct.NbGrabEnd    = 0;
 
	MdigHookFunction(MilDigitizer, M_GRAB_START, GrabStart, (void *)(&UserStruct));
	MdigHookFunction(MilDigitizer, M_GRAB_END,   GrabEnd,   (void *)(&UserStruct));

	MdigGrab(MilDigitizer, MilImage[0]);

	return;
}
void CgVision::StartCapture(BOOL bOneShot)
{
	//m_bGrab = TRUE;
	CWinThread *pThread = ::AfxBeginThread(threadOneGrab, this); // thread 생성

	return;
}


/* Grab Start hook function: 
 *      - This function is used to queue a grab at the beginning of
 *        the each current grab.
 *      - It is also used to measure the frame rate. This is done 
 *        by starting a timer at the beginning of a dummy grab and 
 *        by stopping the timer at the beginning of the last grab.  
 */
MIL_INT MFTYPE GrabStart(MIL_INT HookType, MIL_ID EventId, void MPTYPE *UserStructPtr)
{
  UserDataStruct *UserPtr=(UserDataStruct*)UserStructPtr;

  /* Increment grab start count. */
  UserPtr->NbGrabStart++;
  UserPtr->NbGrabStart = UserPtr->NbGrabStart%UserPtr->NbFrames;

 
  /* Start the timer when needed. */
  if(UserPtr->NbGrabStart == 0)
  	 MappTimer(M_TIMER_RESET+M_GLOBAL, (MIL_DOUBLE *)&UserPtr->Time);
     
  /* Queue a new grab or stop the timer at the end. */
  if (UserPtr->NbGrabStart < UserPtr->NbFrames)
  {
	  MdigGrab(UserPtr->MilDigitizer, 
              UserPtr->MilImage[(UserPtr->NbGrabStart < 0) ? 0 : UserPtr->NbGrabStart]);
	  //TRACE("\nGrab[%d]",(UserPtr->NbGrabStart < 0) ? 0 : UserPtr->NbGrabStart);
  }
  else   
     MappTimer(M_TIMER_READ+M_GLOBAL, (MIL_DOUBLE *)&UserPtr->Time);
  
  return(0);
}
void CgVision::StopCapture()
{
	m_bGrab = FALSE;
	MdigHookFunction(MilDigitizer, M_GRAB_START+M_UNHOOK, GrabStart, (void *)(&UserStruct));
	MdigHookFunction(MilDigitizer, M_GRAB_END+M_UNHOOK, GrabEnd, (void *)(&UserStruct));

}

/* Grab end hook function: 
 *      - This function is used to signal to a waiting thread (here the main())
 *        that a grab is completed and that the data can be processed.
 *
 *  Note: Time spend in the hook function should be minimal. External 
 *	      thread waiting on an event should be used to do processing.
 */
MIL_INT MFTYPE GrabEnd(MIL_INT HookType, MIL_ID EventId, void MPTYPE *UserStructPtr)
{
  UserDataStruct *UserPtr=(UserDataStruct*)UserStructPtr;
  
  /* Increment grab count. */
  UserPtr->NbGrabEnd++;
  UserPtr->NbGrabEnd = UserPtr->NbGrabEnd%UserPtr->NbFrames;
        
  /* Signal the end of grab event to the main thread waiting. */
  MthrControl(UserPtr->GrabEndEvent, M_EVENT_SET, M_SIGNALED);
  
  return(0);
}

void CgVision::Destroy(void)
{
	/* Unhook functions. */
	MdigHookFunction(MilDigitizer, M_GRAB_START+M_UNHOOK, GrabStart, (void *)(&UserStruct));
	MdigHookFunction(MilDigitizer, M_GRAB_END+M_UNHOOK, GrabEnd, (void *)(&UserStruct));

	m_bGrab = false;
	Sleep(500);
	/* Wait for end of last grab. */
	MdigGrabWait(MilDigitizer, M_GRAB_END);


	/* Free event. */
	MthrFree(UserStruct.GrabEndEvent);

	/* Free allocations. */
	for (int n=0; n<NB_GRAB_MAX; n++)
	{
		MbufFree(MilImage[n]);
	}
	//MappFreeDefault(MilApplication, MilSystem, MilDisplay, MilDigitizer, MilImageDisp);
	MbufFree(MilImageDisp);
	MbufFree(MilDisplay);
	MdigFree(MilDigitizer);
	
	MsysFree(MilSystem);
	MappFree(MilApplication);


}

void CgVision::CamSetGain(float dGain)
{
	//CamSetProperty(GAIN, true, false, true, dGain);		
}

void CgVision::CamSetShutter(float dShutter)
{
	//CamSetProperty(SHUTTER, true, false, true, dShutter);		
}

void CgVision::CamSetTrig(bool bTrig)
{
	//CamSetProperty(TRIGGER_MODE, bTrig, NULL, NULL, NULL);	//OK		
}