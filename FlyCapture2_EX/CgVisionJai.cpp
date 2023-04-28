#include "StdAfx.h"
#include "CgVisionJai.h"
#include "resource.h"
#include "FlyCapture2_EXDlg.h"			//maindlg헤더파일을 include해야함.
#pragma comment(lib,"Jai_Factory.lib")


DWORD ProcessCaller(CgVision* pThread)
{
	pThread->StreamProcess();

	return 0;
}


CgVision::CgVision()
{


	//jai
	m_bAuto = FALSE;
	m_CameraCount = 0;
	m_hFactory = NULL;

	for (int m=0; m< MAX_CAMERAS; m++)
	{
		m_hCam[m] = NULL;
		m_hView[m] = NULL;
		m_hThread[m] = NULL;
	}	
	m_CameraCount = 0;	

	//jai thread
	m_hStreamThread = NULL;
	m_hStreamEvent = NULL;
	m_hDS = NULL;
	m_iStreamChannel = 0;
	m_hCondition = NULL;
	m_bStreamStarted = false;
	
	


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

	pDlg->LoopGrab(m_Image);


}



bool CgVision::SWTrigger()
{
// 	m_dTest = GetTickCount();
// 	const unsigned int k_softwareTrigger = 0x62C;
// 	const unsigned int k_fireVal = 0x80000000;
// 	Error error;    
// 
// 	error = m_hCam.WriteRegister( k_softwareTrigger, k_fireVal );
// 	if (error != PGRERROR_OK)
// 	{
// 		return false;
// 	}
	//GetGrabData();
	//m_error = pCam->RetrieveBuffer( &m_Image );

	return true;
}

bool CgVision::InitCam(LPVOID pParam,int xOffset,int yOffset,int width,int height,bool bTrig,float dShtter,float dGain)
{
	m_pMain = pParam;
	m_heventThreadDone = CreateEvent(NULL, FALSE, FALSE, NULL); // 핸들 이벤트 생성	
	m_Image = new BYTE[CAMERA_WIDTH*CAMERA_HEIGHT];
	memset(m_Image,0,CAMERA_WIDTH*CAMERA_HEIGHT);

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


	J_STATUS_TYPE   retval;
	uint32_t        iSize;
	uint32_t        iNumDev;
	bool8_t         bHasChange;

	// Open factory
	retval = J_Factory_Open("" , &m_hFactory);
	if (retval != J_ST_SUCCESS)
	{
		AfxMessageBox(CString("Could not open factory!"));
		return FALSE;
	}
	//    TRACE("Opening factory succeeded\n");

	// Update camera list
	retval = J_Factory_UpdateCameraList(m_hFactory, &bHasChange);
	if (retval != J_ST_SUCCESS)
	{
		AfxMessageBox(CString("Could not update camera list!"), MB_OK | MB_ICONEXCLAMATION);
		return FALSE;
	}
	//TRACE("Updating camera list succeeded\n");

	// Get the number of Cameras
	retval = J_Factory_GetNumOfCameras(m_hFactory, &iNumDev);
	if (retval != J_ST_SUCCESS)
	{
		AfxMessageBox(CString("Could not get the number of cameras!"), MB_OK | MB_ICONEXCLAMATION);
		return FALSE;
	}
	if (iNumDev == 0)
	{
		//        AfxMessageBox(CString("There is no camera!"), MB_OK | MB_ICONEXCLAMATION);
		return FALSE;
	}
	//TRACE("%d cameras were found\n", iNumDev);

	// We only want to get MAX_CAMERAS cameras connected at a time
	// and we assume that iNumDev is the actual camera count*2 because we assume
	// that we have 2 drivers active (SockerDriver+FilerDriver)

	bool bFdUse = false;
	int	iValidCamera = 0;

	for (int i=0; i< (int)iNumDev; i++)
	{
		// Get camera IDs
		iSize = J_CAMERA_ID_SIZE;
		m_sCameraId[iValidCamera][0] = 0;
		retval = J_Factory_GetCameraIDByIndex(m_hFactory, i, m_sCameraId[iValidCamera], &iSize);
		if (retval != J_ST_SUCCESS)
		{
			AfxMessageBox(CString("Could not get the camera ID!"), MB_OK | MB_ICONEXCLAMATION);
			return FALSE;
		}
		//TRACE("Camera ID[%d]: %s\n", i, m_sCameraId[iValidCamera]);

		if(0 == strncmp("TL=>GevTL , INT=>FD", m_sCameraId[iValidCamera], 19))
		{ // FD
			bFdUse = true;
			// Open camera
			retval = J_Camera_Open(m_hFactory, m_sCameraId[iValidCamera], &m_hCam[iValidCamera]);
			if (retval != J_ST_SUCCESS)
			{
				AfxMessageBox(CString("Could not open the camera!"), MB_OK | MB_ICONEXCLAMATION);
				return FALSE;
			}
			iValidCamera++;
			//TRACE("Opening camera %d succeeded\n", iValidCamera);
		}
		else
		{ // SD
			if(bFdUse == false)
			{
				// Open camera
				retval = J_Camera_Open(m_hFactory, m_sCameraId[iValidCamera], &m_hCam[iValidCamera]);
				if (retval != J_ST_SUCCESS)
				{
					AfxMessageBox(CString("Could not open the camera!"), MB_OK | MB_ICONEXCLAMATION);
					return FALSE;
				}
				iValidCamera++;
				//TRACE("Opening camera %d succeeded\n", iValidCamera);
			}
		}
		if(iValidCamera >= MAX_CAMERAS)
			break;
	}
	m_CameraCount = min(iValidCamera, MAX_CAMERAS);

	Sleep(500);
	if (!SetupActions(bTrig))
	{
		AfxMessageBox(CString("Unable to set-up the actions for one or more of the connected cameras!\nPlease check that the connected cameras are GigE Vision 1.1 devices and that they implement Actions!"), MB_OK | MB_ICONEXCLAMATION);
	}

	Sleep(500);	
	

	return TRUE;
}

void CgVision::StartCapture()
{
	m_bGrab = TRUE;
	CWinThread *pThread = ::AfxBeginThread(threadGrab, this); // thread 생성
	//m_error = m_Cam.StartCapture();

	m_bAuto = TRUE;

	J_STATUS_TYPE   retval;
	int64_t int64Val;
	int64_t pixelFormat;

	SIZE	ViewSize;
	//POINT	TopLeft;

	// Get Width from the camera based on GenICam name
	retval = J_Camera_GetValueInt64(m_hCam[0], NODE_NAME_WIDTH, &int64Val);
	ViewSize.cx = (LONG)int64Val;     // Set window size cx

	// Get Height from the camera
	retval = J_Camera_GetValueInt64(m_hCam[0], NODE_NAME_HEIGHT, &int64Val);
	ViewSize.cy = (LONG)int64Val;     // Set window size cy

	// Get pixelformat from the camera
	retval = J_Camera_GetValueInt64(m_hCam[0], NODE_NAME_PIXELFORMAT, &int64Val);
	pixelFormat = int64Val;

	// Calculate number of bits (not bytes) per pixel using macro
	int bpp = J_BitsPerPixel(pixelFormat);

	// Set window position
	//TopLeft.x = 100;
	//TopLeft.y = 50;

	// Open view window
	//retval = J_Image_OpenViewWindow(_T("Image View Window"), &TopLeft, &ViewSize, &m_hView);
	//if (retval != J_ST_SUCCESS) {
	//    ShowErrorMsg(CString("Could not open view window!"), retval);
	//    return;
	//}
	//TRACE("Opening view window succeeded\n");

	// Create image acquisition thread (this allocates buffers)
	CreateStreamThread(m_hCam[0], 0, (ViewSize.cx * ViewSize.cy * bpp)/8);

	// Wait for the stream thread to start the acquisition
	while(!m_bStreamStarted)
		Sleep(10);
	//TRACE("Opening stream succeeded\n");

	// Start Acquision
	retval = J_Camera_ExecuteCommand(m_hCam[0], NODE_NAME_ACQSTART);
	if (retval != J_ST_SUCCESS)
	{
		AfxMessageBox(CString("Could not Start Acquisition!"), retval);
		return;
	}



}

void CgVision::StopCapture()
{
	m_bGrab = FALSE;
	J_STATUS_TYPE retval;

	// Start Acquision
	if (m_hCam[0]) {
		retval = J_Camera_ExecuteCommand(m_hCam[0], NODE_NAME_ACQSTOP);
		if (retval != J_ST_SUCCESS)
		{
			AfxMessageBox(CString("Could not Stop Acquisition!"), retval);
		}
	}

	// Close stream (this frees all allocated buffers)
	TerminateStreamThread();
	//TRACE("Closed stream\n");

	// View window opened?
	if(m_hView[0])
	{
		// Close view window
		//        retval = J_Image_CloseViewWindow(m_hView);
		//        if (retval != J_ST_SUCCESS)          AfxMessageBox(CString("Could not close View Window!"), retval);

		m_hView[0] = NULL;
		//TRACE("Closed view window\n");
	}
}

void CgVision::Destroy(void)
{
	StopCapture();
	//	Sleep(1000);
	DWORD dwRet = WaitForSingleObject(m_heventThreadDone, 3000);
	CloseHandle(m_heventThreadDone); // 핸들 종료
	delete[] m_bitmapInfo;
	delete[] m_Image;
	//	StopCapture();
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
	//트리거 여부
	if (!SetupActions(bTrig))
	{
		AfxMessageBox(CString("Unable to set-up the actions for one or more of the connected cameras!\nPlease check that the connected cameras are GigE Vision 1.1 devices and that they implement Actions!"), MB_OK | MB_ICONEXCLAMATION);
	}

}

bool CgVision::SetupActions(BOOL bTrig)
{
	J_STATUS_TYPE	status = J_ST_SUCCESS;

	for (int i=0; i<MAX_CAMERAS;i++)
	{
		if (m_hCam[i])
		{
			// ActionDeviceKey = DEVICE_KEY
			if(J_ST_SUCCESS	!= J_Camera_SetValueInt64(m_hCam[i], "ActionDeviceKey", DEVICE_KEY))
			{
				AfxMessageBox(_T("Failed to set the value to ActionDeviceKey!"), MB_OK | MB_ICONINFORMATION);
				return false;
			}

			///////////////////////////////////////////////////////////////////////////////
			// ActionSelector = 1
			if(J_ST_SUCCESS != J_Camera_SetValueInt64(m_hCam[i], "ActionSelector", 1))
			{
				AfxMessageBox(_T("Failed to set the value to ActionSelector!"), MB_OK | MB_ICONINFORMATION);
				return false;
			}

			// ActionGroupKey = GROUP_KEY1
			if(J_ST_SUCCESS != J_Camera_SetValueInt64(m_hCam[i], "ActionGroupKey", GROUP_KEY1))
			{
				AfxMessageBox(_T("Failed to set the value to ActionGroupKey!"), MB_OK | MB_ICONINFORMATION);
				return false;
			}

			// ActionGroupMask = GROUP_MASK
			if(J_ST_SUCCESS != J_Camera_SetValueInt64(m_hCam[i], "ActionGroupMask", GROUP_MASK1))
			{
				AfxMessageBox(_T("Failed to set the value to ActionGroupMask!"), MB_OK | MB_ICONINFORMATION);
				return false;
			}

			///////////////////////////////////////////////////////////////////////////////
			// Settings of Trigger Action

			// We have two possible ways of setting up triggers: JAI or GenICam SFNC
			// The JAI trigger setup uses a node called "ExposureMode" and the SFNC uses a node
			// called "TriggerSelector". Therefor we have to determine which way to use here.
			// First we see if a node called "TriggerSelector" exists.
			NODE_HANDLE hNode = NULL;
			status = J_Camera_GetNodeByName(m_hCam[i], "TriggerSelector", &hNode);

			// Does the "TriggerSelector" node exist?
			if ((status == J_ST_SUCCESS) && (hNode != NULL))
			{
				// Here we assume that this is GenICam SFNC trigger so we do the following:
				// TriggerSelector=FrameStart
				// TriggerMode=On
				// TriggerSource=Action1
				if(J_ST_SUCCESS != J_Camera_SetValueString(m_hCam[i], "TriggerSelector", "FrameStart"))
				{
					AfxMessageBox(_T("Failed to set the value to TriggerSelector!"), MB_OK | MB_ICONINFORMATION);
					return false;
				}
				//트리거 여부
				if(bTrig == TRUE)
				{
					if(J_ST_SUCCESS != J_Camera_SetValueString(m_hCam[i], "TriggerMode","On"))//On
					{
						AfxMessageBox(_T("Failed to set the value to TriggerMode!"), MB_OK | MB_ICONINFORMATION);
						//                  return false;
					}
				}					
				else
				{
					if(J_ST_SUCCESS != J_Camera_SetValueString(m_hCam[i], "TriggerMode","Off"))//On
					{
						AfxMessageBox(_T("Failed to set the value to TriggerMode!"), MB_OK | MB_ICONINFORMATION);
						//                  return false;
					}
				}
				
				//                if(J_ST_SUCCESS != J_Camera_SetValueString(m_hCam[i], "TriggerSource", "Line7"))//Action1
				//                {
				//                    AfxMessageBox(_T("Failed to set the value to TriggerSource!"), MB_OK | MB_ICONINFORMATION);
				//                  return false;
				//                }
			}
			else
			{
				// Here we assume that this is JAI trigger so we do the following:
				// ExposureMode=EdgePreSelect
				// LineSelector=CameraTrigger0
				// LineSource=Action1
				// LineInverter=ActiveHigh
				if(J_ST_SUCCESS != J_Camera_SetValueString(m_hCam[i], "ExposureMode", "EdgePreSelect"))
				{
					AfxMessageBox(_T("Failed to set the value to ExposureMode!"), MB_OK | MB_ICONINFORMATION);
					return false;
				}

				if(J_ST_SUCCESS != J_Camera_SetValueString(m_hCam[i], "LineSelector", "CameraTrigger0"))
				{
					AfxMessageBox(_T("Failed to set the value to LineSelector!"), MB_OK | MB_ICONINFORMATION);
					return false;
				}

				if(J_ST_SUCCESS != J_Camera_SetValueString(m_hCam[i], "LineSource", "Action1"))
				{
					AfxMessageBox(_T("Failed to set the value to LineSource!"), MB_OK | MB_ICONINFORMATION);
					return false;
				}
			}
		}
	}

	return true;
}
BOOL CgVision::CreateStreamThread(CAM_HANDLE hCam, uint32_t iChannel, uint32_t iBufferSize)
{
	J_STATUS_TYPE   retval;

	// Open the stream channel(GVSP)
	if(m_hDS == NULL)
	{
		retval = J_Camera_CreateDataStream(hCam, iChannel, &m_hDS);

		if(   (retval != J_ST_SUCCESS)
			|| (m_hDS == NULL))
		{
			if (retval != J_ST_SUCCESS)
			{
				//TRACE(CString("Could not create DataStream!"), retval);
			}

			return FALSE;
		}
	}

	// Prepare the frame buffers (this announces the buffers to the acquisition engine)
	if(0 == PrepareBuffer(iBufferSize))
	{
		AfxMessageBox(CString("PrepareBuffer failed!"), retval);

		retval = J_DataStream_Close(m_hDS);
		if (retval != J_ST_SUCCESS)
		{
			AfxMessageBox(CString("Could not close DataStream!"), retval);
		}
		return FALSE;
	}

	// Stream thread event created?
	if(m_hStreamEvent == NULL)
		m_hStreamEvent = CreateEvent(NULL, true, false, NULL);
	else
		ResetEvent(m_hStreamEvent);

	// Set the thread execution flag

	// Create a Stream Thread.

	if(NULL == (m_hStreamThread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ProcessCaller, this, NULL, NULL)))
	{
		AfxMessageBox(CString("CreateThread failed!"), retval);
		retval = J_DataStream_Close(m_hDS);
		if (retval != J_ST_SUCCESS)
		{
			AfxMessageBox(CString("Could not close DataStream!"), retval);
		}
		return FALSE;
	}

	return TRUE;
}

BOOL CgVision::TerminateStreamThread(void)
{
	J_STATUS_TYPE   retval;

	// Is the data stream opened?
	if(m_hDS == NULL)
		return FALSE;

	// Reset the thread execution flag.
	m_bAuto = false;

	// Signal the image thread to stop faster
	if (m_hCondition)
	{
		retval = J_Event_ExitCondition(m_hCondition);
		if (retval != J_ST_SUCCESS)
		{
			AfxMessageBox(CString("Could not exit Condition!"), retval);
		}
	}

	// Stop the image acquisition engine
	retval = J_DataStream_StopAcquisition(m_hDS, ACQ_STOP_FLAG_KILL);
	if (retval != J_ST_SUCCESS)
	{
		AfxMessageBox(CString("Could not stop DataStream acquisition!"), retval);
	}

	// Mark stream acquisition as stopped
	m_bStreamStarted = false;

	// Wait for the thread to end
	WaitForThreadToTerminate();

	// UnPrepare Buffers (this removed the buffers from the acquisition engine and frees buffers)
	UnPrepareBuffer();

	// Close Stream
	if(m_hDS)
	{
		retval = J_DataStream_Close(m_hDS);
		if (retval != J_ST_SUCCESS)
		{
			AfxMessageBox(CString("Could not clsoe DataStream!"), retval);
		}
		m_hDS = NULL;
	}

	return TRUE;
}
void CgVision::StreamProcess(void)
{
	J_STATUS_TYPE	iResult;
	uint32_t        iSize;
	BUF_HANDLE	    iBufferID;
	uint64_t	    iQueued = 0;

	// Create structure to be used for image display
	J_tIMAGE_INFO	tAqImageInfo = {0, 0, 0, 0, NULL, 0, 0, 0, 0, 0, 0};

	J_COND_WAIT_RESULT	WaitResult;

	// Create the condition used for signalling the new image event
	iResult = J_Event_CreateCondition(&m_hCondition);

	EVT_HANDLE	hEvent; // Buffer event handle

	// Register the event with the acquisition engine
	iResult = J_DataStream_RegisterEvent(m_hDS, EVENT_NEW_BUFFER, m_hCondition, &hEvent); 

	// Start image acquisition
	iResult = J_DataStream_StartAcquisition(m_hDS, ACQ_START_NEXT_IMAGE, 0 );

	// Mark stream acquisition as started
	m_bStreamStarted = true;

	// Loop of Stream Processing
	OutputDebugString(_T(">>> Start Stream Process Loop.\n"));

	while(m_bAuto)
	{
		// Wait for Buffer event (or kill event)
		iResult = J_Event_WaitForCondition(m_hCondition, 1000, &WaitResult);

		// Did we get a new buffer event?
		if(J_COND_WAIT_SIGNAL == WaitResult)
		{
			// Get the Buffer Handle from the event
			iSize = (uint32_t)sizeof(void *);
			iResult = J_Event_GetData(hEvent, &iBufferID,  &iSize);

			// Did we receive the event data?
			if (iResult == J_ST_SUCCESS)
			{
				// Fill in structure for image display
				// Get the pointer to the frame buffer.
				iSize = (uint32_t)sizeof (void *);
				iResult = J_DataStream_GetBufferInfo(m_hDS, iBufferID, BUFFER_INFO_BASE	, &(tAqImageInfo.pImageBuffer), &iSize);
				// Get the effective data size.
				iSize = (uint32_t)sizeof (uint32_t);
				iResult = J_DataStream_GetBufferInfo(m_hDS, iBufferID, BUFFER_INFO_SIZE	, &(tAqImageInfo.iImageSize), &iSize);
				// Get Pixel Format Type.
				iSize = (uint32_t)sizeof (uint32_t);
				iResult = J_DataStream_GetBufferInfo(m_hDS, iBufferID, BUFFER_INFO_PIXELTYPE, &(tAqImageInfo.iPixelType), &iSize);
				// Get Frame Width.
				iSize = (uint32_t)sizeof (uint32_t);
				iResult = J_DataStream_GetBufferInfo(m_hDS, iBufferID, BUFFER_INFO_WIDTH	, &(tAqImageInfo.iSizeX), &iSize);
				// Get Frame Height.
				iSize = (uint32_t)sizeof (uint32_t);
				iResult = J_DataStream_GetBufferInfo(m_hDS, iBufferID, BUFFER_INFO_HEIGHT	, &(tAqImageInfo.iSizeY), &iSize);
				// Get Timestamp.
				iSize = (uint32_t)sizeof (uint64_t);
				iResult = J_DataStream_GetBufferInfo(m_hDS, iBufferID, BUFFER_INFO_TIMESTAMP, &(tAqImageInfo.iTimeStamp), &iSize);
				// 				//TRACE(_T("%d\n"),tAqImageInfo.iTimeStamp);
				// Get # of missing packets in frame.
				iSize = (uint32_t)sizeof (uint32_t);
				iResult = J_DataStream_GetBufferInfo(m_hDS, iBufferID, BUFFER_INFO_NUM_PACKETS_MISSING, &(tAqImageInfo.iMissingPackets), &iSize);
				if(tAqImageInfo.iMissingPackets)
					//					AfxMessageBox(_T("MissingPackets"));
					//					//TRACE(_T("MissingPackets-%d\n"),tAqImageInfo.iMissingPackets);      

					// Initialize number of valid buffers announced
					tAqImageInfo.iAnnouncedBuffers = m_iValidBuffers;

				// Get # of buffers queued
				iSize = sizeof(uint64_t);
				iResult = J_DataStream_GetStreamInfo(m_hDS, STREAM_INFO_CMD_NUMBER_OF_FRAMES_QUEUED, &iQueued, &iSize);
				tAqImageInfo.iQueuedBuffers = static_cast<uint32_t>(iQueued & 0x0ffffffffL);
				//				tAqImageInfo.pImageBuffer



				//여기서 이미지 넘겨주는방법
				//StreamCBFunc1(&tAqImageInfo);				
				memcpy(m_Image,tAqImageInfo.pImageBuffer,CAMERA_WIDTH*CAMERA_HEIGHT);

				///데이터 처리
				//				//TRACE("kk\n");
				// 				TRACE(_T("%d\n"),tAqImageInfo.iTimeStamp);

				// Queue This Buffer Again for reuse in acquisition engine
				iResult = J_DataStream_QueueBuffer(m_hDS, iBufferID);
			}

			// Get # of frames awaiting delivery
			iSize = sizeof(uint64_t);
			iResult = J_DataStream_GetStreamInfo(m_hDS, STREAM_INFO_CMD_NUMBER_OF_FRAMES_AWAIT_DELIVERY, &iQueued, &iSize);
			// We re-trigger the event if there are any pending events available
			if((iResult == J_ST_SUCCESS) && (iQueued > 0))
				iResult = J_Event_SignalCondition(m_hCondition);
		}
		else
		{
			switch(WaitResult)
			{
				// Kill event
			case	J_COND_WAIT_EXIT:
				iResult = 1;
				break;
				// Timeout
			case	J_COND_WAIT_TIMEOUT:
				iResult = 2;
				break;
				// Error event
			case	J_COND_WAIT_ERROR:
				iResult = 3;
				break;
				// Unknown?
			default:
				iResult = 4;
				break;
			}
		}
	}
	OutputDebugString(_T(">>> Terminated Stream Process Loop.\n"));

	// Unregister new buffer event with acquisition engine
	//   iResult = J_DataStream_UnRegisterEvent(m_hDS, EVENT_NEW_BUFFER); 

	// Free the event object
	if (hEvent != NULL)
	{
		iResult = J_Event_Close(hEvent);
		hEvent = NULL;
	}

	// Terminate the thread.
	TerminateThread();

	// Free the Condition
	if (m_hCondition != NULL)
	{
		iResult = J_Event_CloseCondition(m_hCondition);
		m_hCondition = NULL;
	}
}
//==============================================================////
// Terminate image acquisition thread
//==============================================================////
void CgVision::TerminateThread(void)
{
	SetEvent(m_hStreamEvent);
}

//==============================================================////
// Wait for thread to terminate
//==============================================================////
void CgVision::WaitForThreadToTerminate(void)
{
	WaitForSingleObject(m_hStreamEvent, INFINITE);

	// Close the thread handle and stream event handle
	CloseThreadHandle();
}

//==============================================================////
// Close handles and stream
//==============================================================////
void CgVision::CloseThreadHandle(void)
{
	if(m_hStreamThread)
	{
		CloseHandle(m_hStreamThread);
		m_hStreamThread = NULL;
	}

	if(m_hStreamEvent)
	{
		CloseHandle(m_hStreamEvent);
		m_hStreamEvent = NULL;
	}
}
uint32_t CgVision::PrepareBuffer(int iBufferSize)
{
	J_STATUS_TYPE	iResult = J_ST_SUCCESS;
	int			i;

	m_iValidBuffers = 0;

	for(i = 0 ; i < NUM_OF_BUFFER ; i++)
	{
		// Make the buffer for one frame. 
		m_pAquBuffer[i] = new uint8_t[iBufferSize];

		// Announce the buffer pointer to the Acquisition engine.
		if(J_ST_SUCCESS != J_DataStream_AnnounceBuffer(m_hDS, m_pAquBuffer[i] ,iBufferSize , NULL, &(m_pAquBufferID[i])))
		{
			delete m_pAquBuffer[i];
			break;
		}

		// Queueing it.
		if(J_ST_SUCCESS != J_DataStream_QueueBuffer(m_hDS, m_pAquBufferID[i]))
		{
			delete m_pAquBuffer[i];
			break;
		}

		m_iValidBuffers++;
	}

	return m_iValidBuffers;
}

BOOL CgVision::UnPrepareBuffer(void)
{
	void		*pPrivate;
	void		*pBuffer;
	uint32_t	i;

	// Flush Queues
	J_DataStream_FlushQueue(m_hDS, ACQ_QUEUE_INPUT_TO_OUTPUT);
	J_DataStream_FlushQueue(m_hDS, ACQ_QUEUE_OUTPUT_DISCARD);

	for(i = 0 ; i < m_iValidBuffers ; i++)
	{
		// Remove the frame buffer from the Acquisition engine.
		J_DataStream_RevokeBuffer(m_hDS, m_pAquBufferID[i], &pBuffer , &pPrivate);

		delete m_pAquBuffer[i];
		m_pAquBuffer[i] = NULL;
		m_pAquBufferID[i] = 0;
	}

	m_iValidBuffers = 0;

	return TRUE;
}