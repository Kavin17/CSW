#pragma once


// WeakWeldDlg ��ȭ �����Դϴ�.

class WeakWeldDlg : public CDialog
{
	DECLARE_DYNAMIC(WeakWeldDlg)

public:
	WeakWeldDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~WeakWeldDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG4 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};
