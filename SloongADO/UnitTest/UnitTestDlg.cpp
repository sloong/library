
// UnitTestDlg.cpp : implementation file
//

#include "stdafx.h"
#include "UnitTest.h"
#include "UnitTestDlg.h"
#include "afxdialogex.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace SloongADO;

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
	, m_id(0)
	, m_name(_T(""))
	, m_phone(_T(""))
	, m_email(_T(""))
	, m_cur(0)
	, m_total(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_Conn.SetConnectionString("Provider=Microsoft.Jet.OLEDB.4.0; Data Source= TestDb.mdb;Jet OLEDB:DataBase password=1314;");
	m_Conn.Open(_T(""), _T(""), _T(""), ConnectOption::adConnectUnspecified);

	m_Cmd.SetActiveConnection(m_Conn);
}

void CUnitTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_ID, m_id);
	DDX_Text(pDX, IDC_EDIT_NAME, m_name);
	DDX_Text(pDX, IDC_EDIT_PHONE, m_phone);
	DDX_Text(pDX, IDC_EDIT_EMAIL, m_email);
	DDX_Text(pDX, IDC_EDIT_CURRENTPOS, m_cur);
	DDX_Text(pDX, IDC_EDIT_TOTAL, m_total);
}

BEGIN_MESSAGE_MAP(CUnitTestDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_SEARCH, &CUnitTestDlg::OnBnClickedButtonSearch)
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

	// TODO: Add extra initialization here
	EnableControl(false);
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
		CRect rect;
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



void CUnitTestDlg::OnBnClickedButtonSearch()
{
	// TODO: Add your control notification handler code here
	try
	{
		//方法一：通过CConnection对象的Open方法来查询
		//m_Rst.Open("select * from Phone",m_Conn,CursorType::adOpenDynamic,LockType::adLockOptimistic,CommandType::adCmdText);


		//方法二：通过CCommand对象的ExecuteQuery方法来查询

		m_Cmd.SetCommandText(_T("select * from Phone"));
		m_Cmd.ExecuteQuery(m_Rst, CommandType::adCmdText);

		ShowData();

	}
	catch (_com_error e)
	{
		MessageBox(e.Description());
	}
}

void CUnitTestDlg::ShowData()
{
	if (m_Rst.GetRecordCount() > 0)
	{
		m_id = m_Rst.GetInt(_T("id"));
		m_name = m_Rst.GetString(_T("name"));
		m_phone = m_Rst.GetString(_T("phone"));
		m_email = m_Rst.GetString(_T("email"));

		m_cur = m_Rst.GetAbsolutePosition();
		m_total = m_Rst.GetRecordCount();

		UpdateData(false);

		DisableBtn();
	}
	else
	{
		MessageBox(_T("No data!"));

		m_Rst.Release();
		m_Cmd.Release();
		m_Conn.Release();
	}
}


void CUnitTestDlg::DisableBtn()
{

	EnableControl(true);

	/////////////////////////////////////////////////////////////

	if (m_cur >= m_total)
	{
		GetDlgItem(IDC_BUTTON_NEXT)->EnableWindow(false);
		GetDlgItem(IDC_BUTTON_LAST)->EnableWindow(false);
	}
	else if (m_cur <= 1)
	{
		GetDlgItem(IDC_BUTTON_FIRST)->EnableWindow(false);
		GetDlgItem(IDC_BUTTON_PREV)->EnableWindow(false);
	}
	else
	{
	}

}

void CUnitTestDlg::EnableControl(bool flag)
{
	//////////////////////////////////////////////////////////////
	GetDlgItem(IDC_EDIT_ID)->EnableWindow(flag);
	GetDlgItem(IDC_EDIT_NAME)->EnableWindow(flag);
	GetDlgItem(IDC_EDIT_PHONE)->EnableWindow(flag);
	GetDlgItem(IDC_EDIT_EMAIL)->EnableWindow(flag);

	//////////////////////////////////////////////////////////////
	GetDlgItem(IDC_EDIT_CURRENTPOS)->EnableWindow(flag);
	GetDlgItem(IDC_EDIT_TOTAL)->EnableWindow(flag);

	//////////////////////////////////////////////////////////////
	GetDlgItem(IDC_BUTTON_FIRST)->EnableWindow(flag);
	GetDlgItem(IDC_BUTTON_PREV)->EnableWindow(flag);
	GetDlgItem(IDC_BUTTON_NEXT)->EnableWindow(flag);
	GetDlgItem(IDC_BUTTON_LAST)->EnableWindow(flag);
	GetDlgItem(IDC_BUTTON_ADD)->EnableWindow(flag);
	GetDlgItem(IDC_BUTTON_DELETE)->EnableWindow(flag);
}