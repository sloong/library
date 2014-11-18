
// UnitTestDlg.h : header file
//

#pragma once
#import "C:\Program Files\Common Files\System\ado\msado15.dll" no_namespace rename("EOF","rstEOF") rename("BOF","rstBOF")
#include "IUniversal.h"
#pragma comment(lib,"Universal.lib")
using namespace SoaringLoong;
// CUnitTestDlg dialog
class CUnitTestDlg : public CDialogEx
{
// Construction
public:
	CUnitTestDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_UNITTEST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
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
