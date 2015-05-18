#pragma once


// CTestLogDlg dialog
namespace SoaringLoong
{
	class IUniversal;
}
using namespace SoaringLoong;
class CTestLogDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CTestLogDlg)

public:
	CTestLogDlg(CWnd* pParent = NULL);   // standard constructor
	CTestLogDlg(IUniversal* pUniversal);
	virtual ~CTestLogDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();

public:
	IUniversal*	pUniversal;
};
