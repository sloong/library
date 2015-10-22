#pragma once

#import "C:\Program Files\Common Files\System\ado\msado15.dll" no_namespace rename("EOF","rstEOF") rename("BOF","rstBOF")
#include "IUniversal.h"
using namespace Sloong::Universal;
// CTestADODlg dialog

class CTestADODlg : public CDialogEx
{
	DECLARE_DYNAMIC(CTestADODlg)

public:
	CTestADODlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTestADODlg();

// Dialog Data
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	ISloongConnection* m_pConn;
	ISloongRecordset*  m_pRst;
	ISloongCommand*    m_pCmd;
	int m_id;
	CString m_name;
	CString m_phone;
	CString m_email;
	int m_cur;
	long m_total;
	void ShowData();
	void EnableControl(bool flag);
	void DisableBtn();
	afx_msg void OnBnClickedButtonSearch();
	afx_msg void OnBnClickedButtonAdd();
	afx_msg void OnBnClickedButtonDelete();
	afx_msg void OnBnClickedButtonFirst();
	afx_msg void OnBnClickedButtonPrev();
	afx_msg void OnBnClickedButtonNext();
	afx_msg void OnBnClickedButtonLast();
};
