#pragma once

#include <pylon/PylonIncludes.h>
#include <pylon/gige/PylonGigEIncludes.h>
#include <pylon/GrabResultPtr.h>
#include <pylon/ImageEventHandler.h>
#include <pylon/SoftwareTriggerConfiguration.h>
#include <pylon/gige/BaslerGigEInstantCamera.h>
#include <pylon/gige/GigETransportLayer.h>
#include <pylon/ConfigurationEventHandler.h>



typedef Pylon::CBaslerGigEInstantCamera Camera_t;

enum eMODE
{
	MODE_SOFTTRIGGER,				// Soft-Trigger mode
	MODE_HARDTRIGGER,				// Hard-Trigger mode
	MODE_CONTINUE					// Free Run mode
};

enum eEvents
{
	eExposureEndEvent  = 100,
	eEventOverrunEvent = 200
};


using namespace Pylon;

/********************************************************************************/
/* CGigECamera class                                                            */
/* CGigECamera class                                                            */
/********************************************************************************/
class CGigECamera
	: public CImageEventHandler, public CBaslerGigECameraEventHandler, public CConfigurationEventHandler
{
	/********************************************************************************/
	/* etc                                                                          */
public:
	static bool					PersistentIpAddr(CString sSerialNumber, CString sIpAddr, CString sSubnetMask, CString sGateway);
	/* etc                                                                          */
	/********************************************************************************/

	/********************************************************************************/
	/* Constructor & Destructor                                                     */
public:

	CGigECamera(CString sIpAddr, int nContext = 0,BOOL bMono = TRUE);
	~CGigECamera(void);
	/* Constructor & Destructor                                                     */
	/********************************************************************************/

	/********************************************************************************/
	/* Basic functions                                                              */
public:
	void InitGigE(LPVOID lParam);
	bool						Open(void);
	bool						Grab(void);
	void						Stop(void);
	void						Close(void);
	bool						SetMode(eMODE nMode);
	void						SetView(CDC* pDC);
	void						SoftTriggerSend();

	bool						IsGrabbing(void);
	int							CamStatus();
	void CGigECamera::SetGain(double dvalue);

public:
	void						SetAOI(int64_t nX, int64_t nY, int64_t nWidth, int64_t nHeight);
	/* Basic functions                                                              */
	/********************************************************************************/

	/********************************************************************************/
	/* Expert functions                                                             */
public:
	void						SetExpose(int64_t nValue);
	void						SetPacketSize(int64_t nValue);
	/* Expert functions                                                             */
	/********************************************************************************/

	/********************************************************************************/
	/* for Events                                                                   */
public:
	virtual void				OnImageGrabbed(CInstantCamera& camera, const CGrabResultPtr& ptrGrabResult);

	virtual void				OnCameraEvent(Camera_t& camera, intptr_t userProvidedId, GenApi::INode* pNode);

	virtual void				OnOpened( CInstantCamera& camera);
	virtual void				OnCameraDeviceRemoved(CInstantCamera& camera);
	/* for Events                                                                   */
	/********************************************************************************/

	/********************************************************************************/
	/* Attribute : for Image                                                        */
private:
	BITMAPINFO*					m_pBitmapInfo;
	byte*						m_pBuffer;
	byte*						m_pBufferC;
	/* Attribute : for Image                                                        */
	/********************************************************************************/

	/********************************************************************************/
	/* Attribute                                                                    */
private:
	CString						m_sIpAddr;

	Camera_t*					m_pBaslerCamera;
	int							m_nContext;
	int							m_nMode;
	HWND						m_hView;
	CDC*						m_pDC;
	/* Attribute                                                                    */
	/********************************************************************************/


	/********************************************************************************/
	/* for Debug                                                                    */
private:
	long						m_nLogIndex;
public:
	void						AddLog(CString arg_log);
	void						AddLog(CString arg_category, CString arg_function, CString arg_msg);
	/* for Debug                                                                    */
	/********************************************************************************/


	int m_nGrabMode;
	BOOL m_bMono;
};

