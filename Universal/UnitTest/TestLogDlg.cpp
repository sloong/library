// TestLogDlg.cpp : implementation file
//

#include "stdafx.h"
#include "UnitTest.h"
#include "TestLogDlg.h"
#include "afxdialogex.h"
#include "IUniversal.h"
using namespace Sloong;

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
	ILogSystem* pLog = NULL;
	pUniversal->CreateLogSystem(pUniversal, &pLog);
	pLog->WriteLine(TEXT("TestLog"));
	pLog->WriteLine(pUniversal->HelloWorld());
	pLog->Release();
	// Create ILogSystem by CoCreateInstance interface.
	ILogSystem* pLog2 = NULL;
	CoCreateInstance(CLSID_SLOONGUniversal, NULL, CLSCTX_INPROC_SERVER, IID_ILogSystem, (LPVOID*)&pLog2);
	pLog2->Initialize(pUniversal, TEXT("Log2.Log"));
	pLog2->WriteLine(TEXT("TEST2"));
	pLog2->Release();
}
