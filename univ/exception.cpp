#include "stdafx.h"
#include "exception.h"

using namespace Sloong::Universal;



CException::CException(wstring strMessage)
{
	m_strMessage = strMessage;
}

CException::CException(wstring lpStr, HRESULT hRes)
{
	m_strMessage = lpStr;
	m_hResult = hRes;
}


CException::~CException()
{
}


wstring CException::GetException() const
{
	return m_strMessage;
}

HRESULT CException::GetResult() const
{
	return m_hResult;
}