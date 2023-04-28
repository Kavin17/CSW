#include "StdAfx.h"
#include "CgVisionGrab.h"

#include "resource.h"
#include "FlyCapture2_EXDlg.h"				//maindlg헤더파일을 include해야함.
#pragma comment(lib,"multicam.lib")

CgVision::CgVision()
{
    m_Channel = 0;
    m_pCurrent = NULL;
    m_SizeX = 0;
    m_SizeY = 0;
    m_BufferPitch = m_SizeX;
    m_bScreenRefreshCompleted = TRUE;
}

CgVision::~CgVision(void)
{
    // Set the channel to IDLE before deleting it
    McSetParamInt(m_Channel, MC_ChannelState, MC_ChannelState_IDLE);

    // Delete the channel
    McDelete(m_Channel);
}

/////////////////////////////////////////////////////////////////////////////
// GlobalCallback

void WINAPI GlobalCallback(PMCSIGNALINFO SigInfo)
{
    if (SigInfo && SigInfo->Context)
    {
        CgVision* pDoc = (CgVision*) SigInfo->Context;
        pDoc->Callback (SigInfo);
    }
}

/////////////////////////////////////////////////////////////////////////////
// Callback

void CgVision::Callback(PMCSIGNALINFO SigInfo)
{
    // + GrablinkWeb Sample Program
	CFlyCapture2_EXDlg* pDlg = (CFlyCapture2_EXDlg*)m_pMain;
	PVOID pTemp;
	static int ncnt=0;
	ncnt++;

    switch(SigInfo->Signal)
    {
        case MC_SIG_SURFACE_PROCESSING:
            // Update "current" surface address pointer
			McGetParamInt (SigInfo->SignalInfo, MC_SurfaceAddr, (PINT32) &m_pCurrent); 


			//-------Insert the code here.-------------
			pDlg->LoopGrab2((unsigned char *)m_pCurrent);

			
			for(int i=0;i<EURESYS_SURFACE_COUNT;i++)
			{
				McGetParamInst (m_Channel, MC_Cluster+i, &SigInfo->SignalInfo); 
				McGetParamInt (SigInfo->SignalInfo, MC_SurfaceAddr, (PINT32)&pTemp); 
				if(pTemp==m_pCurrent)
				{
					m_ImageBuffer[i]=PUINT8(pTemp);	
					memset(m_ImageBuffer[(i)%3],0,CAMERA_WIDTH*CAMERA_HEIGHT);	
				}
			}

             //----------------------------------------

			break;
        case MC_SIG_ACQUISITION_FAILURE:
            AfxMessageBox(_T("Acquisition Failure !"));
            break;
        default:
            break;

    }
    // - GrablinkWeb Sample Program
}
/*
void CgVision::CamSetFrameRate(FrameRate value)
{

}

void CgVision::CamSetProperty(PropertyType propertyType, bool onOff, bool autoManualMode, bool absControl, float value)
{

}

void CgVision::CamSetRoi(int nOffsetX, int nOffsetY, int nWidth, int nHei)
{

}

*/

bool CgVision::InitCam(LPVOID pParam,int xOffset,int yOffset,int width,int height,bool bTrig,float dShtter,float dGain)
{

	m_pMain = pParam;
    // + GrablinkWeb Sample Program

    // Initialize driver and error handling
    McOpenDriver(NULL);

    // Activate message box error handling and generate an error log file
    McSetParamInt (MC_CONFIGURATION, MC_ErrorHandling, MC_ErrorHandling_MSGBOX);
    McSetParamStr (MC_CONFIGURATION, MC_ErrorLog, "error.log");

 	// In order to support a 10-tap camera on Grablink Full
	// BoardTopology need to be set to MC_BoardTopology_MONO_DECA
	// In order to support 2 cameras on Grablink Expert 2
	// BoardTopology need to be set to MC_BoardTopology_DUO
	// In all other case the default value will work properly 
	// and the parameter don't need to be set

    // Set the board topology to support 2 cameras (only with a Grablink Expert 2)
    // McSetParamInt(MC_BOARD + 0, MC_BoardTopology, MC_BoardTopology_DUO);

	// Set the board topology to support 10 taps mode (only with a Grablink Full)
    // McSetParamInt(MC_BOARD + 0, MC_BoardTopology, MC_BoardTopology_MONO_DECA);

    // Create a channel and associate it with the first connector on the first board
    McCreate(MC_CHANNEL, &m_Channel);
    McSetParamInt(m_Channel, MC_DriverIndex, 0);

	// In order to use single camera on connector A
	// MC_Connector need to be set to A for Grablink Expert 2 and Grablink DualBase
	// For all the other Grablink boards the parameter has to be set to M  
	
	// For all GrabLink boards but Grablink Expert 2 and Dualbase
	McSetParamStr(m_Channel, MC_Connector, "M");
	// For Grablink Expert 2 and Dualbase
	// McSetParamStr(m_Channel, MC_Connector, "A");

    // Choose the video standard
    McSetParamStr(m_Channel, MC_CamFile, "AVIIVA-UM8-CL-12K_L12288RG");
    // Choose the camera expose duration
    McSetParamInt(m_Channel, MC_Expose_us, 10000);
    // Choose the pixel color format
    McSetParamInt(m_Channel, MC_ColorFormat, MC_ColorFormat_Y8);

    // Set the acquisition mode
    McSetParamInt(m_Channel, MC_AcquisitionMode, MC_AcquisitionMode_WEB);

    // Choose the number of lines per page
    McSetParamInt(m_Channel, MC_PageLength_Ln, height);
    // Choose the way the first acquisition is triggered
    McSetParamInt(m_Channel, MC_TrigMode, MC_TrigMode_IMMEDIATE);
    // Choose the triggering mode for subsequent acquisitions
    McSetParamInt(m_Channel, MC_NextTrigMode, MC_NextTrigMode_REPEAT);
    // Choose the number of images to acquire
    McSetParamInt(m_Channel, MC_SeqLength_Fr, MC_INDETERMINATE);

    // Retrieve image dimensions
    McGetParamInt(m_Channel, MC_ImageSizeX, &m_SizeX);
    McGetParamInt(m_Channel, MC_ImageSizeY, &m_SizeY);
    McGetParamInt(m_Channel, MC_BufferPitch, &m_BufferPitch);

    // The memory allocation for the images is automatically done by Multicam when activating the channel.
    // We only set the number of surfaces to be created by MultiCam.
    //McSetParamInt(m_Channel, MC_SurfaceCount, EURESYS_SURFACE_COUNT);
	McSetParamInt(m_Channel, MC_SurfaceCount, EURESYS_SURFACE_COUNT);

    // Enable MultiCam signals
    McSetParamInt(m_Channel, MC_SignalEnable + MC_SIG_SURFACE_PROCESSING, MC_SignalEnable_ON);
    McSetParamInt(m_Channel, MC_SignalEnable + MC_SIG_ACQUISITION_FAILURE, MC_SignalEnable_ON);

    // Register the callback function
    McRegisterCallback(m_Channel, GlobalCallback, this);

    // - GrablinkWeb Sample Program

	return true;
}

void CgVision::GetGrabData(unsigned char* fm)
{

}

void CgVision::StartCapture()
{
    // Start an acquisition sequence by activating the channel
    McSetParamInt(m_Channel, MC_ChannelState, MC_ChannelState_ACTIVE);
    //McSetParamInt(m_Channel, MC_ChannelState, MC_BreakEffect_ABORT);
}

void CgVision::StopCapture()
{
   // Stop an acquisition sequence by deactivating the channel
    McSetParamInt(m_Channel, MC_ChannelState, MC_ChannelState_IDLE);

}

void CgVision::Destroy(void)
{
   // Stop an acquisition sequence by deactivating the channel
    McSetParamInt(m_Channel, MC_ChannelState, MC_ChannelState_IDLE);

}
void CgVision::AbortCapture()
{
    McSetParamInt(m_Channel, MC_ChannelState, MC_BreakEffect_ABORT);
 
}