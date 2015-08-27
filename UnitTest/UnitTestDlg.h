
// UnitTestDlg.h : header file
//

#pragma once
#import "C:\Program Files\Common Files\System\ado\msado15.dll" no_namespace rename("EOF","rstEOF") rename("BOF","rstBOF")
#include "univ\\univ.h"
#include "TestLogDlg.h"
#include "afxcmn.h"
#pragma comment(lib,"univ.lib")
using namespace Sloong::Universal;
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
	IUniversal*	m_pUniversal;

public:
	CTestLogDlg* m_page2;
	
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
	CTabCtrl m_tab;
	virtual BOOL DestroyWindow();
};
