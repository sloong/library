
// UnitTestDlg.h : header file
//

#pragma once

#include "..\\SloongADO\\SloongADO.h"
#include "..\\SloongADO\\Command.h"
#include "..\\SloongADO\\Recordset.h"
#pragma comment(lib,"..\\Debug\\SloongADO.lib")

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
	CSloongConnection m_Conn;
	CSloongRecordset  m_Rst;
	CSloongCommand    m_Cmd;
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
};
