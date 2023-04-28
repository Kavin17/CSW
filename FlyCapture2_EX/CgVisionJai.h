

#pragma once

#include "Jai_Factory.h"
//using namespace Jai_Factory;
//#pragma comment(lib,"Jai_Factory.lib")


#define MAX_CAMERAS         1
#define	NUM_OF_BUFFER	70	//5
#define	DEVICE_KEY  0x00000001
#define GROUP_KEY1  0x00000001
#define GROUP_KEY2  0x00000002
#define	GROUP_MASK  0xffffffff
#define GROUP_MASK1 0x00000001
#define GROUP_MASK2 0x00000002


//jai
#define NODE_NAME_WIDTH         "Width"
#define NODE_NAME_HEIGHT        "Height"
#define NODE_NAME_PIXELFORMAT   "PixelFormat"
#define NODE_NAME_GAIN          "GainRaw"
#define NODE_NAME_ACQSTART      "AcquisitionStart"
#define NODE_NAME_ACQSTOP       "AcquisitionStop"


class CgVision
{
public:
	CgVision();
public:
	~CgVision(void);
protected:
	BOOL m_bGrab;
	LPVOID m_pMain;
	CAM_HANDLE      m_hCam[MAX_CAMERAS];    // Camera Handles	
	static UINT threadGrab(LPVOID pParam);
	int				m_CameraCount;

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
	// Create structure to be used for image display
	
	unsigned char *m_Image;



	//jai °øÅë
	FACTORY_HANDLE  m_hFactory;             // Factory Handle
	THRD_HANDLE     m_hThread[MAX_CAMERAS]; // Stream handles
	VIEW_HANDLE     m_hView[MAX_CAMERAS];   // View Window handles
	int8_t          m_sCameraId[MAX_CAMERAS][J_CAMERA_ID_SIZE]; // Camera IDs
	BOOL			OpenFactoryAndCamera();
	void			CloseFactoryAndCamera();
	bool			SetupActions(BOOL bTrig);
	BOOL m_bAuto;

	//jaji thread
	int32_t         m_iStreamChannel;   // Stream channel number (default 0)
	STREAM_HANDLE   m_hDS;              // Handle to the data stream
	HANDLE          m_hStreamThread;    // Handle to the image acquisition thread
	bool            m_bStreamStarted;   // Flag indicating that the J_Stream_StartAcquisition() has been called
	HANDLE          m_hStreamEvent;     // Thread used to signal when image thread stops
	uint32_t        m_iValidBuffers;    // Number of buffers allocated to image acquisition
	uint8_t*        m_pAquBuffer[NUM_OF_BUFFER];    // Buffers allocated to hold image data
	BUF_HANDLE      m_pAquBufferID[NUM_OF_BUFFER];  // Handles for all the image buffers
	HANDLE	        m_hCondition;       // Condition used for getting the New Image Events
	BOOL CreateStreamThread(CAM_HANDLE hCam, uint32_t iChannel, uint32_t iBufferSize);
	BOOL TerminateStreamThread(void);   // Terminate the image acquisition thread
	void StreamProcess(void);           // The actual image acquisition thread
	void TerminateThread(void);         
	void WaitForThreadToTerminate(void);
	void CloseThreadHandle(void);
	uint32_t PrepareBuffer(int iBufferSize);
	BOOL UnPrepareBuffer(void);
	//  void ShowErrorMsg(CString message, J_STATUS_TYPE error);


};
