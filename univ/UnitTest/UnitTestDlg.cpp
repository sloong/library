
// UnitTestDlg.cpp : implementation file
//

#include "stdafx.h"
#include "UnitTest.h"
#include "UnitTestDlg.h"
#include "afxdialogex.h"
#include "resource.h"
#include "univ\\univ.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace Sloong::Universal;

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
	
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CUnitTestDlg dialog

CUnitTestDlg::CUnitTestDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CUnitTestDlg::IDD, pParent)
{
	m_pUniversal = NULL;
	CoInitialize(NULL);
	m_page2 = new CTestLogDlg(m_pUniversal);
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CUnitTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_tab);
}

BEGIN_MESSAGE_MAP(CUnitTestDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CUnitTestDlg::OnTcnSelchangeTab1)
END_MESSAGE_MAP()


// CUnitTestDlg message handlers

BOOL CUnitTestDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	// Test IUniversal interface.
	
	
	TCITEM tcList[] =
	{
		{ TCIF_TEXT, 0, 0, _T("ADO测试") },
		{ TCIF_TEXT, 0, 0, _T("Log模块测试") },
	};

	for (int i = 0; i < ARRAYSIZE(tcList); i++ )
	{
		m_tab.InsertItem(i, &tcList[i]);
	}

	Sloong::Universal::CRect rec;
	m_tab.GetClientRect(&rec);//获得TAB控件的坐标

	//定位选项卡页的位置，这里可以根据情况自己调节偏移量
	rec.bottom -= 2;
	rec.left += 1;
	rec.top += 27;
	rec.right -= 3;

	//创建子页面
	m_page2->Create(IDD_DIALOG2, GetDlgItem(IDC_TAB1));
	//将子页面移动到指定的位置
	m_page2->MoveWindow(&rec);
	//显示子页面
	m_page2->ShowWindow(SW_HIDE);
	auto str = CUniversal::Version();
	MessageBox(str.c_str());
//	EnableControl(false);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CUnitTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CUnitTestDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		Sloong::Universal::CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CUnitTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CUnitTestDlg::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	switch (m_tab.GetCurSel())
	{
	case 0:
		m_page2->ShowWindow(SW_HIDE);
		break;
	case 1:
		m_page2->ShowWindow(SW_SHOW);
		break;
	default:
		break;
	}
}


BOOL CUnitTestDlg::DestroyWindow()
{
	delete m_page2;
	CoUninitialize();
	return CDialogEx::DestroyWindow();
}
