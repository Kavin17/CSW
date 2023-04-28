//============================================================-
// CDIO.H
// Headder file for CONTEC Digital I/O device
// Supported Devices:
//       DI-16(USB), DO-16(USB), DIO-8/8(USB), CPU-CA10(USB)
// Ver1.01 Added support device 'CPU-CA10(USB)'
//         Added function 'DioGetDigitalFilter()'
//         Added function 'DioSetIoDirectionEx()'
//         Added function 'DioGetIoDirectionEx()'
//         Added error code 'STATUS_SYS_USB_ACCESS_DENIED'
//         Added error code 'STATUS_SYS_USB_INVALID_HANDLE'
//         Added error code 'DIO_ERR_SYS_IODIRECTION'
//                                              CONTEC.Co.,Ltd.
//============================================================-

//-------------------------------------------------
// Prototype definition
//-------------------------------------------------
#ifdef __cplusplus
extern "C"{
#endif
// Common function
long WINAPI DioInit(char *DeviceName, short *Id);
long WINAPI DioExit(short Id);
long WINAPI DioResetDevice(short Id);
long WINAPI DioGetErrorString(long ErrorCode , char *ErrorString);

// Digital filter function
long WINAPI DioSetDigitalFilter(short Id, short FilterValue);
long WINAPI DioGetDigitalFilter(short Id, short *FilterValue);

// I/O Direction function
long WINAPI DioSetIoDirection(short Id, DWORD dwDir);
long WINAPI DioGetIoDirection(short Id, DWORD *dwDir);
long WINAPI DioSetIoDirectionEx(short Id, DWORD dwDir);
long WINAPI DioGetIoDirectionEx(short Id, DWORD *dwDir);

// Simple I/O function
long WINAPI DioInpByte(short Id, short PortNo, BYTE *Data);
long WINAPI DioInpBit(short Id, short BitNo, BYTE *Data);
long WINAPI DioOutByte(short Id, short PortNo, BYTE Data);
long WINAPI DioOutBit(short Id, short BitNo, BYTE Data);
long WINAPI DioEchoBackByte(short Id, short PortNo, BYTE *Data);
long WINAPI DioEchoBackBit(short Id, short BitNo, BYTE *Data);

// Multiple I/O function
long WINAPI DioInpMultiByte(short Id, short *PortNo, short PortNum, BYTE *Data);
long WINAPI DioInpMultiBit(short Id, short *BitNo, short BitNum, BYTE *Data);
long WINAPI DioOutMultiByte(short Id, short *PortNo, short PortNum, BYTE *Data);
long WINAPI DioOutMultiBit(short Id, short *BitNo, short BitNum, BYTE *Data);
long WINAPI DioEchoBackMultiByte(short Id, short *PortNo, short PortNum, BYTE *Data);
long WINAPI DioEchoBackMultiBit(short Id, short *BitNo, short BitNum, BYTE *Data);

// Interrupt function
long WINAPI DioNotifyInt(short Id, short IntBit, short Logic, HANDLE hWnd);
long WINAPI DioStopNotifyInt(short Id, short IntBit);

// Trigger function
long WINAPI DioNotifyTrg(short Id, short TrgBit, short TrgKind, long Tim, HANDLE hWnd);
long WINAPI DioStopNotifyTrg(short Id, short TrgBit);

// Information function
long WINAPI DioGetDeviceInfo(char *Device, short InfoType, void *Param1, void *Param2, void *Param3);
long WINAPI DioQueryDeviceName(short Index, char *DeviceName, char *Device);
long WINAPI DioGetDeviceType(char *Device, short *DeviceType);
long WINAPI DioGetMaxPorts(short Id, short *InPortNum, short *OutPortNum);

#ifdef __cplusplus
};
#endif
//-------------------------------------------------
// Type definition
//-------------------------------------------------

#define	DEVICE_TYPE_ISA					0	//ISA or C bus
#define	DEVICE_TYPE_PCI					1	//PCI bus
#define	DEVICE_TYPE_PCMCIA				2	//PCMCIA
#define	DEVICE_TYPE_USB					3	//USB
#define	DEVICE_TYPE_FIT					4	//FIT

//-------------------------------------------------
// Parameters
//-------------------------------------------------
// I/O(for Sample)
#define	DIO_MAX_ACCS_PORTS		256
// DioNotifyInt:Logic
#define	DIO_INT_RISE	1
#define	DIO_INT_FALL	2
// DioNotifyTrg:TrgKind
#define	DIO_TRG_RISE	1
#define	DIO_TRG_FALL	2
// Message
#define	DIOM_INTERRUPT	0x1300
#define	DIOM_TRIGGER	0x1340
// Device Information
#define	IDIO_DEVICE_TYPE			0	// device type.						Param1:short
#define	IDIO_NUMBER_OF_8255			1	// Number of 8255 chip.				Param1:int
#define	IDIO_IS_8255_BOARD			2	// Is 8255 board?					Param1:BOOL(True/False)
#define	IDIO_NUMBER_OF_DI_BIT		3	// Number of digital input bit.		Param1:int
#define	IDIO_NUMBER_OF_DO_BIT		4	// Number of digital outout bit.	Param1:int
#define	IDIO_NUMBER_OF_DI_PORT		5	// Number of digital input port.	Param1:int
#define	IDIO_NUMBER_OF_DO_PORT		6	// Number of digital output port.	Param1:int
#define	IDIO_IS_POSITIVE_LOGIC		7	// Is positive logic?				Param1:BOOL(True/False)
#define	IDIO_IS_ECHO_BACK			8	// Can echo back output port?		Param1:BOOL(True/False)

//-------------------------------------------------
// Error codes
//-------------------------------------------------
// Initialize Error
// Common
#define	DIO_ERR_SUCCESS					0		//	�Fnormal completed
#define	DIO_ERR_INI_RESOURCE			1		//	�Finvalid resource reference specified
#define	DIO_ERR_INI_INTERRUPT			2		//	�Finvalid interrupt routine registered
#define	DIO_ERR_INI_MEMORY				3		//	�Finvalid memory allocationed
#define	DIO_ERR_INI_REGISTRY			4		//	�Finvalid registry accesse
// DIO

// DLL Error
// Common
#define	DIO_ERR_DLL_DEVICE_NAME			10000	//	�Finvalid device name specified.
#define	DIO_ERR_DLL_INVALID_ID			10001	//	�Finvalid ID specified.
#define	DIO_ERR_DLL_CALL_DRIVER			10002	//	�Fnot call the driver.(Invalid device I/O controller)
#define	DIO_ERR_DLL_CREATE_FILE			10003	//	�Fnot create the file.(Invalid CreateFile)
#define	DIO_ERR_DLL_CLOSE_FILE			10004	//	�Fnot close the file.(Invalid CloseFile)
#define	DIO_ERR_DLL_CREATE_THREAD		10005	//	�Fnot create the thread.(Invalid CreateThread)
#define	DIO_ERR_INFO_INVALID_DEVICE		10050	//	�Finvalid device infomation specified .Please check the spell.
#define	DIO_ERR_INFO_NOT_FIND_DEVICE	10051	//	�Fnot find the available device
#define	DIO_ERR_INFO_INVALID_INFOTYPE	10052	//	�Fspecified device infomation type beyond the limit

// DIO
#define DIO_ERR_DLL_BUFF_ADDRESS		10100	//	�Finvalid data buffer address
#define	DIO_ERR_DLL_HWND				10200	//	�Fwindow handle beyond the limit
#define	DIO_ERR_DLL_TRG_KIND			10300	//	�Ftrigger kind beyond the limit

// SYS Error
// Common
#define	DIO_ERR_SYS_MEMORY				20000	//	�Fnot secure memory
#define	DIO_ERR_SYS_NOT_SUPPORTED		20001	//	�Fthis board couldn't use this function
#define	DIO_ERR_SYS_BOARD_EXECUTING		20002	//	�Fboard is behaving, not execute
#define	DIO_ERR_SYS_USING_OTHER_PROCESS	20003	//	�Fother process is using the device, not execute

#ifndef STATUS_SYS_USB_CRC
#define	STATUS_SYS_USB_CRC						20020	// the last data packet received from end point exist CRC error
#define	STATUS_SYS_USB_BTSTUFF					20021	// the last data packet received from end point exist bit stuffing offense error
#define	STATUS_SYS_USB_DATA_TOGGLE_MISMATCH		20022	// the last data packet received from end point exist toggle packet mismatch error
#define	STATUS_SYS_USB_STALL_PID				20023	// end point return STALL packet identifier
#define	STATUS_SYS_USB_DEV_NOT_RESPONDING		20024	// device don't respond to token(IN) ,don't support handshake
#define	STATUS_SYS_USB_PID_CHECK_FAILURE		20025	
#define	STATUS_SYS_USB_UNEXPECTED_PID			20026	// invalid packet identifier received
#define	STATUS_SYS_USB_DATA_OVERRUN				20027	// end point return data quantity overrun
#define	STATUS_SYS_USB_DATA_UNDERRUN			20028	// end point return data quantity underrun
#define	STATUS_SYS_USB_BUFFER_OVERRUN			20029	// IN transmit specified buffer overrun
#define	STATUS_SYS_USB_BUFFER_UNDERRUN			20030	// OUT transmit specified buffer underrun
#define	STATUS_SYS_USB_ENDPOINT_HALTED			20031	// end point status is STALL, not transmit
#define	STATUS_SYS_USB_NOT_FOUND_DEVINFO		20032	// not found device infomation
#define	STATUS_SYS_USB_ACCESS_DENIED			20033	// Access denied
#define	STATUS_SYS_USB_INVALID_HANDLE			20034	// Invalid handle
#endif
// DIO
#define DIO_ERR_SYS_PORT_NO				20100	//	�Fboard No. beyond the limit
#define DIO_ERR_SYS_PORT_NUM			20101	//	�Fboard number beyond the limit
#define DIO_ERR_SYS_BIT_NO				20102	//	�Fbit No. beyond the limit
#define DIO_ERR_SYS_BIT_NUM				20103	//	�Fbit number beyond the limit
#define DIO_ERR_SYS_BIT_DATA			20104	//	�Fbit data beyond the limit of 0 to 1
#define	DIO_ERR_SYS_INT_BIT				20200	//	�Finterrupt bit beyond the limit
#define	DIO_ERR_SYS_INT_LOGIC			20201	//	�Finterrupt logic beyond the limit
#define	DIO_ERR_SYS_TIM					20300	//	�Ftimer value beyond the limit
#define	DIO_ERR_SYS_FILTER				20400	//	�Ffilter number beyond the limit
#define DIO_ERR_SYS_IODIRECTION			20500	//	�FDirection value is out of range

//	end of file
