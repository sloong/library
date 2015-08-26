// TestLogDlg.cpp : implementation file
//

#include "stdafx.h"
#include "UnitTest.h"
#include "TestLogDlg.h"
#include "afxdialogex.h"
#include "SloongUniversal.h"
#include "SloongLog.h"
using namespace Sloong::Universal;

// CTestLogDlg dialog

IMPLEMENT_DYNAMIC(CTestLogDlg, CDialogEx)

CTestLogDlg::CTestLogDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTestLogDlg::IDD, pParent)
{

}

CTestLogDlg::CTestLogDlg(IUniversal* pUniv)
{
	pUniversal = pUniv;
}

CTestLogDlg::~CTestLogDlg()
{
}

void CTestLogDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTestLogDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CTestLogDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CTestLogDlg message handlers


void CTestLogDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	// Create ILogSystem from IUniversal::CreateLogSystem interface.
	CLog pLog;
	pLog.WriteLine(TEXT("TestLog"));
	pLog.WriteLine(CUniversal::Version());
}
