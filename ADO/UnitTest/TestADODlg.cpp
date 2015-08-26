// TestADODlg.cpp : implementation file
//

#include "stdafx.h"
#include "UnitTest.h"
#include "TestADODlg.h"
#include "afxdialogex.h"


// CTestADODlg dialog

IMPLEMENT_DYNAMIC(CTestADODlg, CDialogEx)

CTestADODlg::CTestADODlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTestADODlg::IDD, pParent)
	, m_id(0)
	, m_name(_T(""))
	, m_phone(_T(""))
	, m_email(_T(""))
	, m_cur(0)
	, m_total(0)
{
	//CString str(_T("Provider=SQLNCLI11;Server=(localdb)\\ProjectsV12;Database=SLOONG.COM_BaseDB;Trusted_Connection=yes"));
	//IUniversal* pUniversal;
	// 	CreateUniversal(&pUniversal);
	// 	pUniversal->CreateADO(&m_pConn, &m_pRst, &m_pCmd);
	// 	m_pConn->SetConnectionString(str);
	// 	m_pConn->Open(str, _T(""), _T(""), adModeUnknown);
	// 
	// 	m_pCmd->SetActiveConnection(*m_pConn);
}

CTestADODlg::~CTestADODlg()
{
}

void CTestADODlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_ID, m_id);
	DDX_Text(pDX, IDC_EDIT_NAME, m_name);
	DDX_Text(pDX, IDC_EDIT_PHONE, m_phone);
	DDX_Text(pDX, IDC_EDIT_EMAIL, m_email);
	DDX_Text(pDX, IDC_EDIT_CURRENTPOS, m_cur);
	DDX_Text(pDX, IDC_EDIT_TOTAL, m_total);
}


BEGIN_MESSAGE_MAP(CTestADODlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_SEARCH, &CTestADODlg::OnBnClickedButtonSearch)
	ON_BN_CLICKED(IDC_BUTTON_ADD, &CTestADODlg::OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, &CTestADODlg::OnBnClickedButtonDelete)
	ON_BN_CLICKED(IDC_BUTTON_FIRST, &CTestADODlg::OnBnClickedButtonFirst)
	ON_BN_CLICKED(IDC_BUTTON_PREV, &CTestADODlg::OnBnClickedButtonPrev)
	ON_BN_CLICKED(IDC_BUTTON_NEXT, &CTestADODlg::OnBnClickedButtonNext)
	ON_BN_CLICKED(IDC_BUTTON_LAST, &CTestADODlg::OnBnClickedButtonLast)
END_MESSAGE_MAP()


// CTestADODlg message handlers


void CTestADODlg::OnBnClickedButtonSearch()
{
	try
	{
		m_pCmd->SetCommandText(_T("select * from [Table]"));
		//m_pCmd->ExecuteQuery(*m_pRst, SoaringLoong::CommandTypeEnum::adCmdText);

		ShowData();

	}
	catch (_com_error e)
	{
		MessageBox(e.Description());
	}
}

void CTestADODlg::ShowData()
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


void CTestADODlg::DisableBtn()
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

void CTestADODlg::EnableControl(bool flag)
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

void CTestADODlg::OnBnClickedButtonAdd()
{
	// TODO: Add your control notification handler code here
	UpdateData(true);

	CString sql = _T("");
	sql.Format(_T("insert into [Table](id,name,phone,email) values(%d,'%s','%s','%s')"), m_id, m_name, m_phone, m_email);

	m_pCmd->SetCommandText(sql);
	long rows = 0;
//	m_pCmd->ExecuteUpdate(rows, *m_pRst, SoaringLoong::CommandTypeEnum::adCmdText);

	CString msgStr;
	msgStr.Format(_T("成功添加了%d行。"), rows);
	MessageBox(msgStr);
}


void CTestADODlg::OnBnClickedButtonDelete()
{
	// TODO: Add your control notification handler code here
	CString sql;
	sql.Format(_T("delete from [Table] where id='%d'"), m_id);

	m_pCmd->SetCommandText(sql);
	long rows = 0;
//	m_pCmd->ExecuteUpdate(rows, *m_pRst, SoaringLoong::CommandTypeEnum::adCmdText);

	CString msgStr;
	msgStr.Format(_T("删除了%d行。"), rows);
	MessageBox(msgStr);
}


void CTestADODlg::OnBnClickedButtonFirst()
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


void CTestADODlg::OnBnClickedButtonPrev()
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


void CTestADODlg::OnBnClickedButtonNext()
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


void CTestADODlg::OnBnClickedButtonLast()
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