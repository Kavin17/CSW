#ifndef __NEPROTOCOL_H__
#define __NEPROTOCOL_H__

#ifdef NESERIALDLL_EXPORTS
#define __NESERIAL_EXPORT __declspec(dllexport)
#else
#define __NESERIAL_EXPORT
#endif

#include "winserial.h"

#define STX 0x02
#define ETX 0x03
#define ACK 0x06
#define NAK 0x25

class __NESERIAL_EXPORT CNeProtocol : public CComPort
{
public:
	CNeProtocol();
	virtual ~CNeProtocol();

	BOOL SendString(LPCTSTR str);
	int GetString(char *pString, int nStrSize, int nTimeout);
	//if nTimeout is zero, wait data to infinity
	BOOL WaitCommand(unsigned char nCommand, int nTimeout);
	BOOL WaitCommand(unsigned char *pCommand, int nTimeout);
	
	int TestChkSum(unsigned char *p, int bytes, unsigned char chksum);
	unsigned char GenChkSum(unsigned char *p, int bytes);

	BOOL NeModemSend(LPCTSTR sFileName, unsigned char *buffer, unsigned int nSize, int nTimeout);

	void SetHookFunc(BOOL (*func)(LPVOID,int), LPVOID param);

	int GetClusterSize();

private:
	BOOL (*HookFunc)(LPVOID param, int progress);
	LPVOID m_pParam;
};

#endif