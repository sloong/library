
// UnitTest.h : main header file for the UnitTest application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CUnitTestApp:
// See UnitTest.cpp for the implementation of this class
//

class CUnitTestApp : public CWinApp
{
public:
	CUnitTestApp();


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CUnitTestApp theApp;
