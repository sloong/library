
// testDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "test.h"
#include "testDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "Base64.h"
#include "MD5.h"
#include "BlowFish.h"
#pragma comment(lib,"univ.lib")
using namespace Sloong::Universal;
typedef string(*Func)(string orgTxt, string key, BOOL bBase64);
typedef struct FuncListItem
{
	LPCTSTR FuncName;
	Func pFunc;
}FuncListItem;

FuncListItem FuncList[] =
{
	{ "Base64 Encoding", Base64Encoding },
	{ "Base64 Decoding", Base64Decoding },
	{ "BlowFish Encoding", BlowFishEncoding },
	{ "������MD5+Base64", BinaryMD5Base64Encoding },
	{ "MD5 Encoding", MD5Encoding},
	{ "BlowFish Ayers Encoding", BlowFishAyersEncoding },
	{ "BlowFish Ayers Decoding", BlowFishAyersDecoding },
	{ "BlowFish->Ayers Encoding", BlowFish2AyersEncoding },
	{ "Base64Ayers->BlowFish", Base64Ayers2Blowfish },
};


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CtestDlg �Ի���



CtestDlg::CtestDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_TEST_DIALOG, pParent)
	, m_strOrg(_T(""))
	, m_strRes(_T(""))
	, m_strKey(_T(""))
	, m_bBase64Res(FALSE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CtestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT2, m_strOrg);
	DDX_Text(pDX, IDC_EDIT1, m_strRes);
	DDX_Text(pDX, IDC_EDIT3, m_strKey);
	DDX_Control(pDX, IDC_COMBO1, m_oConvertType);
	DDX_Check(pDX, IDC_CHECK1, m_bBase64Res);
}

BEGIN_MESSAGE_MAP(CtestDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_CHECK1, &CtestDlg::OnBnClickedCheck1)
	ON_BN_CLICKED(IDOK, &CtestDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CtestDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CtestDlg ��Ϣ�������

BOOL CtestDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	int len = sizeof(FuncList) / sizeof(FuncListItem);
	for (int i = 0; i < len; i++)
	{
		m_oConvertType.AddString(FuncList[i].FuncName);
	}
	m_strKey = "NAN74TN3";
	UpdateData(FALSE);
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CtestDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CtestDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		::CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CtestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CtestDlg::OnBnClickedCheck1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}


void CtestDlg::OnBnClickedOk()
{
	UpdateData();
	CString curTxt;
	m_oConvertType.GetLBText(m_oConvertType.GetCurSel(), curTxt);
	int len = sizeof(FuncList) / sizeof(FuncListItem);
	for (int i = 0; i < len; i++)
	{
		if (curTxt == FuncList[i].FuncName)
		{
			auto pFunc = FuncList[i].pFunc;
			auto strRes = pFunc(m_strOrg.GetBuffer(m_strOrg.GetLength()), m_strKey.GetBuffer(m_strKey.GetLength()), m_bBase64Res);
			m_strRes = CString(strRes.c_str());
			break;
		}
	}
	UpdateData(FALSE);
}


void CtestDlg::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnCancel();
}




string Base64Encoding(string orgTxt, string key, BOOL bBase64)
{
	return CBase64::Encoding((const unsigned char*)orgTxt.c_str(), orgTxt.length());
}


std::string MD5Encoding(string orgTxt, string key, BOOL bBase64)
{
	auto res = CMD5::Encoding(orgTxt,FALSE);
	if ( bBase64 )
	{
		return CBase64::Encoding((unsigned char*)res.c_str(), res.length());
	}
	return res;
}

string Base64Decoding(string orgTxt, string key, BOOL bBase64)
{
	return CBase64::Decoding(orgTxt);
}

string BinaryMD5Base64Encoding(string orgTxt, string key, BOOL bBase64)
{
	unsigned char md[16] = { 0 };
	CMD5::Binary_Encoding(orgTxt, md);
	return CBase64::Encoding(md, 16);
}


string BlowFishAyersEncoding(string orgTxt, string key, BOOL bBase64)
{
	CBlowFish blowfish((unsigned char*)key.c_str(), key.length());
	CString strEncodeData;
	blowfish.Encrypt((unsigned char*)orgTxt.c_str(), (unsigned char*)strEncodeData.GetBuffer(3 * orgTxt.length()), orgTxt.length());
	strEncodeData.ReleaseBuffer();
	CString strAyersData("");
	int nAyersData = CBlowFish::CompressBlowfish(strEncodeData, strAyersData.GetBuffer(strEncodeData.GetLength()));
	strAyersData.ReleaseBuffer();
	return CBase64::Encoding((const unsigned char*)strAyersData.GetBuffer(nAyersData + 4), nAyersData + 4);
}


string BlowFishAyersDecoding(string orgTxt, string key, BOOL bBase64)
{
	CString strBase64;
	unsigned char* pBuf = (unsigned char*)strBase64.GetBuffer(2046);
	int nLen = CBase64::Binary_Decoding(orgTxt.c_str(), pBuf);
	int nLenMsg = 0;
	memcpy(&nLenMsg, (LPCTSTR)pBuf, 4);
	CString strChgBuffer("");
	int nEncodeDataLength = CBlowFish::UncompressBlowfish((char*)pBuf + 4, strChgBuffer.GetBuffer(nLenMsg), nLenMsg);
	strChgBuffer.ReleaseBuffer();
	CBlowFish blowfish((unsigned char*)key.c_str(), key.length());
	CString strDecDataBuffer;
	blowfish.Decrypt((unsigned char *)(LPTSTR)(LPCTSTR)strChgBuffer, (unsigned char*)strDecDataBuffer.GetBuffer(nEncodeDataLength), nEncodeDataLength);
	strDecDataBuffer.ReleaseBuffer();
	return (LPCTSTR)strDecDataBuffer;
}

std::string BlowFish2AyersEncoding(string orgTxt, string key, BOOL bBase64)
{
	CString strAyersData("");
	int nAyersData = CBlowFish::CompressBlowfish(orgTxt.c_str(), strAyersData.GetBuffer(orgTxt.length()));
	strAyersData.ReleaseBuffer();
	int nLenMsg = 0;
	memcpy(&nLenMsg, (LPCTSTR)strAyersData, 4);
	char* pBlowfish = new char[3 * nLenMsg + 1]();
	memset(pBlowfish, 0x00, 3 * nLenMsg + 1);

	char* pBuf = (char*)strAyersData.GetBuffer();
	int nLen = CBlowFish::UncompressBlowfish(pBuf + 4, pBlowfish, nLenMsg);
	if (CString(orgTxt.c_str()) != CString(pBlowfish))
	{
		MessageBox(NULL, "Error", "Error", MB_OK);
	}
	SAFE_DELETE_ARR(pBlowfish);
	return CBase64::Encoding((const unsigned char*)strAyersData.GetBuffer(nAyersData + 4), nAyersData + 4);
}

string BlowFishEncoding(string orgTxt, string key, BOOL bBase64)
{
	CBlowFish blowfish((unsigned char*)key.c_str(), key.length());
	CString strEncodeData;
	blowfish.Encrypt((unsigned char*)orgTxt.c_str(), (unsigned char*)strEncodeData.GetBuffer(3 * orgTxt.length()), orgTxt.length());
	strEncodeData.ReleaseBuffer();
	if (bBase64 == TRUE)
	{
		return CBase64::Encoding((const unsigned char*)strEncodeData.GetBuffer(strEncodeData.GetLength()), strEncodeData.GetLength());
	}
	return strEncodeData.GetBuffer(strEncodeData.GetLength());
}


std::string Base64Ayers2Blowfish(string orgTxt, string key, BOOL bBase64)
{
	string str = CBase64::Decoding(orgTxt);
	CString strChgBuffer("");
	int nLenMsg = 0;
	memcpy(&nLenMsg, (LPCTSTR)str.c_str(), 4);
	int nEncodeDataLength = CBlowFish::UncompressBlowfish((char*)str.c_str() + 4, strChgBuffer.GetBuffer(nLenMsg), nLenMsg);
	strChgBuffer.ReleaseBuffer();
	return strChgBuffer;
}
