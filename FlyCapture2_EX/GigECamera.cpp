//#include "FlyCapture2_EXDlg.h"				//maindlg헤더파일을 include해야함.


#include "StdAfx.h"
#include "GigECamera.h"

#include "FlyCapture2_EX.h"
#include "FlyCapture2_EXDlg.h"

#include "ConfigurationEventPrinter.h"

#define GRAB_MODE_WEB	0


using namespace GenApi;
using namespace Basler_GigECameraParams;

CFlyCapture2_EXDlg* pMain;


bool CGigECamera::PersistentIpAddr(CString sSerialNumber, CString sIpAddr, CString sSubnetMask, CString sGateway)
{

	IGigETransportLayer* const gigeTL = dynamic_cast<IGigETransportLayer*>(CTlFactory::GetInstance().CreateTl(BaslerGigEDeviceClass));

	if ( gigeTL )
	{
		CString sMacAddr = "";
		DeviceInfoList_t dil;
		int nCamera = gigeTL->EnumerateAllDevices(dil);
		CTlFactory& TlFactory = CTlFactory::GetInstance();

		for(int i = 0; i < nCamera; i++)
		{
			CString sDevSerialNumber = dil[i].GetSerialNumber();
			if (sDevSerialNumber == sSerialNumber)
			{
				CBaslerGigEDeviceInfo &bdi = static_cast<CBaslerGigEDeviceInfo&>(dil[i]);
				sMacAddr = bdi.GetMacAddress();
			}
		}

		if (sMacAddr != "")
		{
			gigeTL->ForceIp(sMacAddr.GetBuffer(), sIpAddr.GetBuffer(), sSubnetMask.GetBuffer(), "0.0.0.0");

			CBaslerGigEDeviceInfo devInfo;
			devInfo.SetIpAddress(sIpAddr.GetBuffer());
			devInfo.SetSubnetMask(sSubnetMask.GetBuffer());

			Camera_t camera;
			camera.Attach(TlFactory.CreateDevice(devInfo));
			camera.Open();

			camera.ChangeIpConfiguration(true, true);
			camera.SetPersistentIpAddress(sIpAddr.GetBuffer(), sSubnetMask.GetBuffer(), sGateway.GetBuffer());
			camera.Close();
		}
	}

	return true;
}

CGigECamera::CGigECamera(CString sIpAddr, int nContext ,BOOL bMono)
	: m_nLogIndex(0)
	, m_pBuffer(NULL)
	, m_pBufferC(NULL)
{

	m_pBaslerCamera = NULL;
	m_nMode = MODE_SOFTTRIGGER;

	m_sIpAddr = sIpAddr;
	m_nContext = nContext;

	m_hView = NULL;
	m_pDC = NULL;


	m_nGrabMode=GRAB_MODE_WEB;
	m_bMono = bMono;
	// Build bitmap info Y8
	if(m_bMono==TRUE)
	{
		m_pBitmapInfo = (BITMAPINFO *) new BYTE[sizeof(BITMAPINFO) + 255*sizeof(RGBQUAD)];
		m_pBitmapInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		m_pBitmapInfo->bmiHeader.biPlanes = 1;
		m_pBitmapInfo->bmiHeader.biBitCount = 8;
		m_pBitmapInfo->bmiHeader.biCompression = BI_RGB;
		m_pBitmapInfo->bmiHeader.biSizeImage = 0;
		m_pBitmapInfo->bmiHeader.biXPelsPerMeter = 0;
		m_pBitmapInfo->bmiHeader.biYPelsPerMeter = 0;
		m_pBitmapInfo->bmiHeader.biClrUsed = 0;
		m_pBitmapInfo->bmiHeader.biClrImportant = 0;
		for (int i = 0 ; i < 256 ; i++)
		{
			m_pBitmapInfo->bmiColors[i].rgbBlue = (BYTE)i;
			m_pBitmapInfo->bmiColors[i].rgbGreen = (BYTE)i;
			m_pBitmapInfo->bmiColors[i].rgbRed = (BYTE)i;
			m_pBitmapInfo->bmiColors[i].rgbReserved = 0;
		}
		m_pBitmapInfo->bmiHeader.biWidth = 0;
		m_pBitmapInfo->bmiHeader.biHeight = 0;
	}
	else
	{
		m_pBitmapInfo = (BITMAPINFO *) new BYTE[sizeof(BITMAPINFO)];
		m_pBitmapInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		m_pBitmapInfo->bmiHeader.biPlanes = 1;
		m_pBitmapInfo->bmiHeader.biBitCount = 24;
		m_pBitmapInfo->bmiHeader.biCompression = BI_RGB;
		m_pBitmapInfo->bmiHeader.biSizeImage = 0;
		m_pBitmapInfo->bmiHeader.biXPelsPerMeter = 0;
		m_pBitmapInfo->bmiHeader.biYPelsPerMeter = 0;
		m_pBitmapInfo->bmiHeader.biClrUsed = 0;
		m_pBitmapInfo->bmiHeader.biClrImportant = 0;
		m_pBitmapInfo->bmiHeader.biWidth = 0;//_n_Width / ( _p_BmpInfo->bmiHeader.biBitCount / 8) * 3;
		m_pBitmapInfo->bmiHeader.biHeight = 0;//-_n_Height;
	}
}

CGigECamera::~CGigECamera(void)
{
	Close();
	if (m_pBitmapInfo != NULL)
	{
		delete m_pBitmapInfo;
		m_pBitmapInfo = NULL;
	}
}


void CGigECamera::InitGigE(LPVOID lParam)
{
	pMain = (CFlyCapture2_EXDlg*)lParam;

}

bool CGigECamera::Open(void)
{
	bool bRet = false;


	/********************************************************************************/
	/* Set Device Info                                                              */
	CTlFactory& tlFactory = CTlFactory::GetInstance(); 
	CBaslerGigEDeviceInfo di;
	di.SetIpAddress(m_sIpAddr.GetBuffer());
	/* Set Device Info                                                              */
	/********************************************************************************/

	try
	{
		m_pBaslerCamera = new Camera_t(tlFactory.CreateDevice(di));
		m_pBaslerCamera->SetCameraContext(m_nContext);
		m_pBaslerCamera->RegisterImageEventHandler( this, RegistrationMode_Append, Ownership_ExternalOwnership);
		m_pBaslerCamera->RegisterConfiguration(this, RegistrationMode_ReplaceAll, Ownership_ExternalOwnership);

		// For demonstration purposes only, register another configuration event handler that handles device removal.
//		m_pBaslerCamera->RegisterConfiguration( new CSampleConfigurationEventHandler, RegistrationMode_Append, Cleanup_Delete);

		// For demonstration purposes only, add a sample configuration event handler to print out information
		// about camera use. 
		m_pBaslerCamera->RegisterConfiguration( new CConfigurationEventPrinter, RegistrationMode_Append, Cleanup_Delete);



		m_pBaslerCamera->Open();

		//m_pBaslerCamera->GrabCameraEvents = true;
		//m_pBaslerCamera->RegisterCameraEventHandler(this, "ExposureEndEventData", eExposureEndEvent, RegistrationMode_ReplaceAll, Ownership_ExternalOwnership);
		//m_pBaslerCamera->RegisterCameraEventHandler(this, "EventOverrunEventData", eEventOverrunEvent, RegistrationMode_Append, Ownership_ExternalOwnership);

		//// Select the event to receive.
		//m_pBaslerCamera->EventSelector.SetValue(EventSelector_ExposureEnd);
		//// Enable it.
		//m_pBaslerCamera->EventNotification.SetValue(EventNotification_GenICamEvent);


		bRet = true;
	}
	catch (GenICam::GenericException &e)
    {
        // Error handling
        //cerr << "An exception occurred." << endl
        String_t errName = e.GetDescription();
		
		//20140513 ljh 추가한부분*************
		Close();
		bRet = false;
		return bRet;
		//**********************
        //exitCode = 1;
    }
	catch(...)
	{
		Close();
		bRet = false;
	}
	//Camera Roi 설정
	//SetAOI(0,0,CAM_WIDTH,CAM_PATIAL_LINE);
	return bRet;
}



int CGigECamera::CamStatus()
{
	int n = m_pBaslerCamera->IsCameraDeviceRemoved();//GetCameraContext();
	//error//0 -> ok 1->fail
//	int bn ;//= m_pBaslerCamera->IsCameraDeviceRemoved();//>AcquisitionStatus();
	try
	{
//		GenApi::CIntegerPtr width(m_pBaslerCamera->GetNodeMap().GetNode("Width"));
//		width->SetValue( width->GetMax() - (width->GetInc() * (80 % 2)));
	}
	catch (GenICam::GenericException &e)
	{
		if ( m_pBaslerCamera->IsCameraDeviceRemoved())	
		{
			//n = -1;
			////Reconnect
			//CDeviceInfo info;
			//// Get the transport layer factory.
			//CTlFactory& tlFactory = CTlFactory::GetInstance();

			//// Remember the camera properties that allow detecting the same camera again.
			//info.SetDeviceClass( m_pBaslerCamera->GetDeviceInfo().GetDeviceClass());
			//info.SetSerialNumber( m_pBaslerCamera->GetDeviceInfo().GetSerialNumber());
			//// Destroy the Pylon Device representing the detached camera device.
			//// It cannot be used anymore.
			//m_pBaslerCamera->DestroyDevice();
			//// Create a filter containing the CDeviceInfo object info which describes the properties of the device we are looking for.
			//DeviceInfoList_t filter;
			//filter.push_back( info);

			//DeviceInfoList_t devices;
			//if ( tlFactory.EnumerateDevices(devices, filter) > 0 )
			//{
			//	// The camera has been found. Create and attach it to the Instant Camera object.
			//	m_pBaslerCamera->Attach( tlFactory.CreateDevice( devices[0]));
			//}
			//Sleep(50);
			//if ( m_pBaslerCamera->IsPylonDeviceAttached())
			//{
			//	// Open the camera.
			//	m_pBaslerCamera->Open();
			//	bn = m_pBaslerCamera->IsCameraDeviceRemoved();//>AcquisitionStatus();
			//	if(bn)
			//		n+=10;
			//}
			//else // Timeout
			//{
			//	n+=10;
			//}
			n+=10;
		}//n+=4;
		else		throw;
	}
	//if(bn)
	//{
	//	n = -1;
	//	//Reconnect
	//	CDeviceInfo info;
	//	// Get the transport layer factory.
	//	CTlFactory& tlFactory = CTlFactory::GetInstance();

	//	// Remember the camera properties that allow detecting the same camera again.
	//	info.SetDeviceClass( m_pBaslerCamera->GetDeviceInfo().GetDeviceClass());
	//	info.SetSerialNumber( m_pBaslerCamera->GetDeviceInfo().GetSerialNumber());
	//	// Destroy the Pylon Device representing the detached camera device.
	//	// It cannot be used anymore.
	//	m_pBaslerCamera->DestroyDevice();
	//	// Create a filter containing the CDeviceInfo object info which describes the properties of the device we are looking for.
	//	DeviceInfoList_t filter;
	//	filter.push_back( info);

	//	DeviceInfoList_t devices;
	//	if ( tlFactory.EnumerateDevices(devices, filter) > 0 )
	//	{
	//		// The camera has been found. Create and attach it to the Instant Camera object.
	//		m_pBaslerCamera->Attach( tlFactory.CreateDevice( devices[0]));
	//	}
	//	Sleep(50);
	//	if ( m_pBaslerCamera->IsPylonDeviceAttached())
	//	{
	//		// Open the camera.
	//		m_pBaslerCamera->Open();
	//		bn = m_pBaslerCamera->IsCameraDeviceRemoved();//>AcquisitionStatus();
	//		if(bn)
	//			n+=10;
	//	}
	//	else // Timeout
	//	{
	//		n+=10;
	//	}

	//}
	return n;
}

bool CGigECamera::Grab(void)
{
	bool res = false;

	if (m_pBaslerCamera != NULL)
	{
		try
		{
			if(IsGrabbing()){}
			else
				m_pBaslerCamera->StartGrabbing( GrabStrategy_OneByOne, GrabLoop_ProvidedByInstantCamera);
			res = true;
		}
		catch (GenICam::GenericException ex)
		{
			AddLog("Error", "CGigECamera::Grab", ex.GetDescription());
			//pMain->AddtoList("Cam Error Grab");
		}
		catch (...)//catch (CException* e)
		{
			AddLog("Error", "CGigECamera::Grab", "failed : StartGrabbing");
			//pMain->AddtoList("Cam Error Start Grabbing");
		}
	}

	return res;
}

void CGigECamera::Stop(void)
{
	if (m_pBaslerCamera != NULL)
	{
		try
		{
			if (IsGrabbing())
			{
				m_pBaslerCamera->StopGrabbing();
			}
		}
		catch (...)
		{
			AddLog("Error", "CGigECamera::Stop", "failed : StopGrabbing");
			//pMain->AddtoList("Cam Error Stop Grabbing");
		}
	}
}

void CGigECamera::Close(void)
{
	if (m_pBaslerCamera != NULL)
	{
		Stop();

		m_pBaslerCamera->Close();

		m_pBaslerCamera->DeregisterImageEventHandler(this);
		m_pBaslerCamera->DeregisterConfiguration(this);

		m_pBaslerCamera->DestroyDevice();

		delete m_pBaslerCamera;
		m_pBaslerCamera = NULL;
	}
}

bool CGigECamera::IsGrabbing(void)
{
	bool res = false;

	if (m_pBaslerCamera != NULL)
	{
		try
		{
			res = m_pBaslerCamera->IsGrabbing();
		}
		catch (...)
		{
		}
	}

	return res;
}

bool CGigECamera::SetMode(eMODE nMode)
{
	bool bRet = false;

	if (m_pBaslerCamera != NULL && !m_pBaslerCamera->IsGrabbing())
	{
		INodeMap &Control = m_pBaslerCamera->GetNodeMap();
		CEnumerationPtr triggerSelector(Control.GetNode(_T("TriggerSelector")));
		CEnumerationPtr triggerMode(Control.GetNode(_T("TriggerMode")));
		CEnumerationPtr triggerSource(Control.GetNode(_T("TriggerSource")));

		switch (nMode)
		{
		case MODE_SOFTTRIGGER:
			triggerMode->FromString(_T("On"));
			triggerSource->FromString(_T("Software"));
			break;

		case MODE_HARDTRIGGER:
			triggerMode->FromString(_T("On"));
			triggerSource->FromString(_T("Line1"));
			break;

		case MODE_CONTINUE:
			triggerSelector->FromString(_T("FrameStart"));
			triggerMode->FromString(_T("Off"));
			break;
		}

		m_nMode = nMode;
	}

	return bRet;
}

void CGigECamera::OnImageGrabbed(CInstantCamera& camera, const CGrabResultPtr& ptrGrabResult)
{
	//if (m_pDC != NULL && ptrGrabResult->GrabSucceeded() == true)
	//static int cnt = 1;
	if (ptrGrabResult->GrabSucceeded() == true)
	{
		int nWidth = ptrGrabResult->GetWidth();
		int nHeight = ptrGrabResult->GetHeight();
		int nBufferPitch = nWidth;
		if(m_bMono==TRUE)
			m_pBitmapInfo->bmiHeader.biWidth = nBufferPitch / ( m_pBitmapInfo->bmiHeader.biBitCount / 8);  // Width = Pitch(bytes) divided by the number of bytes per pixel
		else
			m_pBitmapInfo->bmiHeader.biWidth = nBufferPitch;// / ( m_pBitmapInfo->bmiHeader.biBitCount / 8) * 3;  // Width = Pitch(bytes) divided by the number of bytes per pixel
			
		m_pBitmapInfo->bmiHeader.biHeight = -nHeight ;

		int nContext = (int)camera.GetCameraContext();
		unsigned char* buffer = (unsigned char*)ptrGrabResult->GetBuffer();

		//if(m_bMono)
		//{
		//	//lotate-----------------------------------------------
		//	BYTE* buf =	new BYTE[nWidth*nHeight];
		//	for (int i=0;i<nWidth;i++)
		//		for (int j=0;j<nHeight;j++)
		//		{
		//			buf[nHeight*(nWidth-i-1)+j] = buffer[nWidth*j+i];
		//		}
		//	
		//	//-----------------------------------------------------
		//	pMain->LoopGrab(nContext,buf);
		//	delete[] buf;
		//}

		pMain->LoopGrab(nContext,buffer);
	}
	else
	{
		//pMain->AddtoList("NonGrab Complete");
	}
	//cnt++;
}

void CGigECamera::SetView(CDC* pDC)
{
	m_pDC = pDC;
}

void CGigECamera::SoftTriggerSend()
{
	if (m_pBaslerCamera != NULL)
	{
		m_pBaslerCamera->ExecuteSoftwareTrigger();
	}
}

void CGigECamera::OnCameraEvent( CBaslerGigEInstantCamera& camera, intptr_t userProvidedId, GenApi::INode* pNode)
{
	CString str;
	str.Format(_T("%d"), camera.GetCameraContext());
	switch (userProvidedId)
	{
	case eExposureEndEvent:
		break;

	default:
		break;
	}
}

void CGigECamera::OnCameraDeviceRemoved(CInstantCamera& camera)
{
	AddLog("Error", "CGigECamera::OnCameraDeviceRemoved", "Camera Disconnected.");
}

void CGigECamera::SetExpose(int64_t nValue)
{
	if (m_pBaslerCamera != NULL){
		// hoho : Minimum Exposure 
		if(nValue < 35)
			nValue = 35;

		nValue -= (nValue%35);
		m_pBaslerCamera->ExposureTimeRaw.SetValue(nValue);
		Sleep(1);
	}
}

void CGigECamera::SetGain(double dvalue)
{
	if (m_pBaslerCamera != NULL){
		int nn = dvalue;
		if(nn<300)
			nn = 300;
		if(nn>800)nn = 800;
		m_pBaslerCamera->GainRaw.SetValue(nn);
	}
}

void CGigECamera::SetPacketSize(int64_t nValue)
{
	if (m_pBaslerCamera != NULL)
	{
		INodeMap &Control = m_pBaslerCamera->GetNodeMap();
		CIntegerPtr packetSize(Control.GetNode(_T("GevSCPSPacketSize")));

		packetSize->SetValue(nValue);
	}
}

void CGigECamera::SetAOI(int64_t nX, int64_t nY, int64_t nWidth, int64_t nHeight)
{
	if (m_pBaslerCamera != NULL)
	{
		if (m_pBaslerCamera->IsOpen() == true)
		{
			bool bGrabbing = m_pBaslerCamera->IsGrabbing();

			if (bGrabbing == true)
			{
				m_pBaslerCamera->StopGrabbing();
			}

			//m_pBaslerCamera->OffsetX.SetValue(nX);
			//m_pBaslerCamera->OffsetY.SetValue(0);
			//m_pBaslerCamera->Width.SetValue(nWidth);	
			//m_pBaslerCamera->Height.SetValue(80);

			m_pBaslerCamera->Width.SetValue(nWidth);	
			m_pBaslerCamera->Height.SetValue(nHeight);
			m_pBaslerCamera->OffsetX.SetValue(nX);
			m_pBaslerCamera->OffsetY.SetValue(nY);


			CString str;
//			str.Format("%d , %d , %d , %d",int(nX),int(nY),int(nWidth),int(nHeight));
//			pMain->AddtoList(str);
			if (bGrabbing == true)
			{
				m_pBaslerCamera->StartGrabbing( GrabStrategy_OneByOne, GrabLoop_ProvidedByInstantCamera);
			}
		}
	}
}

void CGigECamera::AddLog(CString arg_log)
{
	TRACE(arg_log);
	TRACE("\n");
}

void CGigECamera::AddLog(CString arg_category, CString arg_function, CString arg_msg)
{
	CString sTmp;
	sTmp.Format("[%06d] [%-20s]: [%s] > %s", m_nLogIndex++, arg_category, arg_function, arg_msg);

	AddLog(sTmp);
}

void CGigECamera::OnOpened(CInstantCamera& camera)
{
	using namespace GenApi;

	try
	{
		// Get the camera control object.
		INodeMap &Control = camera.GetNodeMap();

		// Get the trigger selector node.
		CEnumerationPtr triggerSelector(Control.GetNode("TriggerSelector"));
		// Get the trigger mode node.
		CEnumerationPtr triggerMode(Control.GetNode("TriggerMode"));

		// Check the available camera trigger mode(s) to select the appropriate one: acquisition start trigger mode 
		// (used by previous cameras, i.e. for cameras supporting only the legacy image acquisition control mode;
		// do not confuse with acquisition start command) or frame start trigger mode
		// (used by newer cameras, i.e.for cameras using the standard image acquisition control mode;
		// equivalent to the acquisition start trigger mode in the leagcy image acquisition control mode).
		bool frameStartAvailable = false;
		bool acquisitionStartAvailable = false;
		{
			// Frame start trigger mode available?
			GenApi::IEnumEntry* frameStart = triggerSelector->GetEntryByName("FrameStart");
			frameStartAvailable = frameStart && IsAvailable(frameStart);

			// Acquisition start trigger mode available?
			GenApi::IEnumEntry* acquisitionStart = triggerSelector->GetEntryByName("AcquisitionStart");
			acquisitionStartAvailable = acquisitionStart && IsAvailable(acquisitionStart);
		}

		// Preselect the trigger mode for image acquisition.
		const char* triggerSelectorValue = "FrameStart";

		// Check to see if the camera implements the acquisition start trigger mode only.
		if ( acquisitionStartAvailable && !frameStartAvailable)
		{
			// Camera uses the acquisition start trigger as the only trigger mode.
			triggerSelector->FromString("AcquisitionStart");
			triggerMode->FromString("On");
			triggerSelectorValue = "AcquisitionStart";
		}
		else  
		{   
			// Camera may have the acquisition start trigger mode and the frame start trigger mode implemented.
			// In this case, the acquisition trigger mode must be switched off.
			if ( acquisitionStartAvailable )
			{
				triggerSelector->FromString("AcquisitionStart");
				triggerMode->FromString("Off");
			}
			// To trigger each single frame by software or external hardware trigger: Enable the frame start trigger mode.
			assert( frameStartAvailable); //Frame start trigger mode must be available here.
			triggerSelector->FromString("FrameStart");
			//triggerMode->FromString("On");
			triggerMode->FromString("Off");
		}

		// Note: The trigger selector must be set to the appropriate trigger mode 
		// before setting the trigger source or issuing software triggers.
		// Frame start trigger mode for newer cameras (i.e. for cameras supporting the standard image acquisition control mode).
		triggerSelector->FromString( triggerSelectorValue);

		// The trigger source must be set to 'Software'.
		CEnumerationPtr(Control.GetNode("TriggerSource"))->FromString("Software");

		//// Alternative hardware trigger configuration:
		//// This configuration can be copied and modified to create a hardware trigger configuration.
		//// Remove setting the 'TriggerSource' to 'Software' (see above) and 
		//// use the commented lines as a starting point.
		//// The camera user's manual contains more information about available configurations.
		//// The pylon Viewer tool can be used to test the selected settings first.

		//// The trigger source must be set to the trigger input, e.g. 'Line1'.
		//CEnumerationPtr(Control.GetNode("TriggerSource"))->FromString("Line1");

		////The trigger activation must be set to e.g. 'RisingEdge'.
		//CEnumerationPtr(Control.GetNode("TriggerActivation"))->FromString("RisingEdge");

		// Enable the continuous acquisition mode.
		CEnumerationPtr(Control.GetNode("AcquisitionMode"))->FromString("Continuous");
	}
	catch (GenICam::GenericException& e)
	{
		throw RUNTIME_EXCEPTION( "Could not apply configuration. GenICam::GenericException caught in OnOpened method msg=%hs", e.what());
	}
	catch (std::exception& e)
	{
		throw RUNTIME_EXCEPTION( "Could not apply configuration. std::exception caught in OnOpened method msg=%hs", e.what());
	}
	catch (...)
	{
		throw RUNTIME_EXCEPTION( "Could not apply configuration. Unknown exception caught in OnOpened method.");
	}
}
