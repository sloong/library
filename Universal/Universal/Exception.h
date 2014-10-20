#pragma once
#include "IUniversal.h"
using SoaringLoong::IException;

class CException : public IException
{
public:
	CException();
	~CException();

	void SetExceptionText(LPCTSTR strMessage);
	LPCTSTR GetExceptionText();

	void SetExceptionCode(HRESULT lCode);
	HRESULT GetExceptionCode();

private:
	LPTSTR m_strMessage;
	HRESULT m_lResult;
};