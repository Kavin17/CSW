// eg-io 1.1


#pragma once
#include "EGIOT.h"
#define	 MAX_INPUT	8

class CgIO
{
public:
	CgIO();
public:
	~CgIO(void);
protected:
	BOOL	m_bRead;
	short	m_nDIODevice;
	LPVOID	m_pMain;
	bool	m_bInputData[MAX_INPUT];
public:
	static UINT threadIO(LPVOID pParam);

	BOOL InitIO(LPVOID pParam,int nDeviceid);
	void GetIOData();
	void Destroy(void);
	void WriteOutputPort(int port, bool bValue);
	void WriteOutputPortPulse(int port,int ms);
	int ReadInputPort(int port);
	bool* ReadInput();
};
