#pragma once


// CPassWord ��ȭ �����Դϴ�.

class CPassWord : public CDialog
{
	DECLARE_DYNAMIC(CPassWord)

public:
	CPassWord(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.

	CWnd *m_hParent;
	void ReadPassword();
	void WritePassword();
	CString	m_strReadPass;

	virtual ~CPassWord();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_PASSWORD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	BOOL m_bModifyPass;
	afx_msg void OnBnClickedButtonCheck();
	afx_msg void OnBnClickedCheckModifypass();
	afx_msg void OnBnClickedButtonCheck2();
	CString m_strPassword1;
	CString m_strPassword2;
	CString m_strPassword3;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonCheck3();
};
