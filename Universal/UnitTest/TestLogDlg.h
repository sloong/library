#pragma once


// CTestLogDlg dialog
namespace Sloong
{
	namespace Universal
	{
		class IUniversal;
	}
}
using namespace Sloong::Universal;
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
