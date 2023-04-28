#pragma once


// CPassWord 대화 상자입니다.

class CPassWord : public CDialog
{
	DECLARE_DYNAMIC(CPassWord)

public:
	CPassWord(CWnd* pParent = NULL);   // 표준 생성자입니다.

	CWnd *m_hParent;
	void ReadPassword();
	void WritePassword();
	CString	m_strReadPass;

	virtual ~CPassWord();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_PASSWORD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

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
