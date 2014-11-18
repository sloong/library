
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

using namespace SoaringLoong;

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
	CString str(_T("Provider=SQLNCLI11;Server=(localdb)\\Projects;Database=VEISDBDev_01;Trusted_Connection=yes"));
	IUniversal* pUniversal;
	CreateUniversal(&pUniversal);
	pUniversal->CreateADO(&m_pConn, &m_pRst, &m_pCmd);
	m_pConn->SetConnectionString(str);
	m_pConn->Open(str, _T(""), _T(""), adModeUnknown);

	m_pCmd->SetActiveConnection(*m_pConn);
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
	ON_BN_CLICKED(IDC_BUTTON_ADD, &CUnitTestDlg::OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, &CUnitTestDlg::OnBnClickedButtonDelete)
	ON_BN_CLICKED(IDC_BUTTON_FIRST, &CUnitTestDlg::OnBnClickedButtonFirst)
	ON_BN_CLICKED(IDC_BUTTON_PREV, &CUnitTestDlg::OnBnClickedButtonPrev)
	ON_BN_CLICKED(IDC_BUTTON_NEXT, &CUnitTestDlg::OnBnClickedButtonNext)
	ON_BN_CLICKED(IDC_BUTTON_LAST, &CUnitTestDlg::OnBnClickedButtonLast)
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
		SoaringLoong::CRect rect;
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
	try
	{
		m_pCmd->SetCommandText(_T("select * from [Table]"));
		m_pCmd->ExecuteQuery(*m_pRst, SoaringLoong::adCmdText);

		ShowData();

	}
	catch (_com_error e)
	{
		MessageBox(e.Description());
	}
}

void CUnitTestDlg::ShowData()
{
	if (m_pRst->GetRecordCount() > 0)
	{
		m_id = m_pRst->GetInt(_T("id"));
		m_name = m_pRst->GetString(_T("name"));
		m_phone = m_pRst->GetString(_T("phone"));
		m_email = m_pRst->GetString(_T("email"));

		m_cur = m_pRst->GetAbsolutePosition();
		m_total = m_pRst->GetRecordCount();

		UpdateData(false);

		DisableBtn();
	}
	else
	{
		MessageBox(_T("No data!"));

		m_pRst->Release();
		m_pCmd->Release();
		m_pConn->Release();
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

void CUnitTestDlg::OnBnClickedButtonAdd()
{
	// TODO: Add your control notification handler code here
	UpdateData(true);

	CString sql = _T("");
	sql.Format(_T("insert into [Table](id,name,phone,email) values(%d,'%s','%s','%s')"),m_id,m_name,m_phone,m_email);

	m_pCmd->SetCommandText(sql);
	long rows = 0;
	m_pCmd->ExecuteUpdate(rows, *m_pRst, SoaringLoong::adCmdText);

	CString msgStr;
	msgStr.Format(_T("成功添加了%d行。"), rows);
	MessageBox(msgStr);
}


void CUnitTestDlg::OnBnClickedButtonDelete()
{
	// TODO: Add your control notification handler code here
	CString sql;
	sql.Format(_T("delete from [Table] where id='%d'"),m_id);

	m_pCmd->SetCommandText(sql);
	long rows = 0;
	m_pCmd->ExecuteUpdate(rows, *m_pRst, SoaringLoong::adCmdText);

	CString msgStr;
	msgStr.Format(_T("删除了%d行。"), rows);
	MessageBox(msgStr);
}


void CUnitTestDlg::OnBnClickedButtonFirst()
{
	// TODO: Add your control notification handler code here
	try
	{
		m_pRst->MoveFirst();
		ShowData();
	}
	catch (_com_error e)
	{
		MessageBox(e.Description());
	}
}


void CUnitTestDlg::OnBnClickedButtonPrev()
{
	// TODO: Add your control notification handler code here
	try
	{
		m_pRst->MovePrevious();
		ShowData();
	}
	catch (_com_error e)
	{
		MessageBox(e.Description());
	}
}


void CUnitTestDlg::OnBnClickedButtonNext()
{
	// TODO: Add your control notification handler code here
	try
	{
		m_pRst->MoveNext();
		ShowData();
	}
	catch (_com_error e)
	{
		MessageBox(e.Description());
	}
}


void CUnitTestDlg::OnBnClickedButtonLast()
{
	// TODO: Add your control notification handler code here
	try
	{
		m_pRst->MoveLast();
		ShowData();
	}
	catch (_com_error e)
	{
		MessageBox(e.Description());
	}
}
