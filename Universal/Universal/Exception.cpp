#include "stdafx.h"
#include "IUniversal.h"
#include "Exception.h"


CException::CException()
{
	m_strMessage = NULL;
}

CException::~CException()
{
	SAFE_DELETE(m_strMessage);
}

void CException::SetExceptionText(LPCTSTR strMessage)
{
	//CopyStringToPoint(m_strMessage, strMessage);
}

LPCTSTR CException::GetExceptionText()
{
	return m_strMessage;
}

void CException::SetExceptionCode(HRESULT lCode)
{
	m_lResult = lCode;
}

HRESULT CException::GetExceptionCode()
{
	return m_lResult;
}
