// FlyCapture2_EX.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.
//#include "CgProject.h"

// CFlyCapture2_EXApp:
// �� Ŭ������ ������ ���ؼ��� FlyCapture2_EX.cpp�� �����Ͻʽÿ�.
//



class CFlyCapture2_EXApp : public CWinApp
{
public:
	CFlyCapture2_EXApp();
	
// �������Դϴ�.
	ULONG_PTR m_gdiplusToken;

	public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();
};

extern CFlyCapture2_EXApp theApp;