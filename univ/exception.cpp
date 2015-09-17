#include "stdafx.h"
#include "exception.h"

using namespace Sloong::Universal;



CException::CException(CString strMessage)
{
	m_strMessage = strMessage;
}

CException::CException(CString lpStr, HRESULT hRes)
{
	m_strMessage = lpStr;
	m_hResult = hRes;
}


CException::~CException()
{
}


CString CException::GetException() const
{
	return m_strMessage;
}

HRESULT CException::GetResult() const
{
	return m_hResult;
}