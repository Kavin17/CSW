// WeakWeldDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "FlyCapture2_EX.h"
#include "WeakWeldDlg.h"


// WeakWeldDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(WeakWeldDlg, CDialog)

WeakWeldDlg::WeakWeldDlg(CWnd* pParent /*=NULL*/)
	: CDialog(WeakWeldDlg::IDD, pParent)
{

}

WeakWeldDlg::~WeakWeldDlg()
{
}

void WeakWeldDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(WeakWeldDlg, CDialog)
END_MESSAGE_MAP()


// WeakWeldDlg 메시지 처리기입니다.
