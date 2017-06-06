
// testDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"


// CtestDlg �Ի���
class CtestDlg : public CDialogEx
{
// ����
public:
	CtestDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TEST_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CString m_strOrg;
	CString m_strRes;
	CString m_strKey;
	CComboBox m_oConvertType;
	BOOL m_bBase64Res;
	afx_msg void OnBnClickedCheck1();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};

string Base64Encoding(string orgTxt, string key, BOOL bBase64);
string MD5Encoding(string orgTxt, string key, BOOL bBase64);
string Base64Decoding(string orgTxt, string key, BOOL bBase64);
string BinaryMD5Base64Encoding(string orgTxt, string key, BOOL bBase64);
string BlowFishAyersEncoding(string orgTxt, string key, BOOL bBase64);
string BlowFishAyersDecoding(string orgTxt, string key, BOOL bBase64);
string BlowFish2AyersEncoding(string orgTxt, string key, BOOL bBase64);
string BlowFishEncoding(string orgTxt, string key, BOOL bBase64);
string Base64Ayers2Blowfish(string orgTxt, string key, BOOL bBase64);

