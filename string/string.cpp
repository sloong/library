#include "stdafx.h"
#include "string.h"
using Sloong::Universal::CString;

#define MAX_BUFFER	2048

CString::CString()
{
	m_strString = new wstring();
	m_strTemp = NULL;
}


Sloong::Universal::CString::CString(LPCSTR lpStr, ...)
{
	new(this) CString();
	va_list args;
	va_start(args, lpStr);
	FormatA(lpStr, args);
	va_end(args);
}

Sloong::Universal::CString::CString(LPCWSTR lpStr, ...)
{
	new(this) CString();
	va_list args;
	va_start(args, lpStr);
	FormatW(lpStr, args);
	va_end(args);
}

Sloong::Universal::CString::CString(string lpStr)
{
	new(this) CString();
	(*m_strString) = ANSIToUnicode(lpStr.c_str());
}

Sloong::Universal::CString::CString(wstring lpStr)
{
	new(this) CString();
	(*m_strString) = lpStr;
}

Sloong::Universal::CString::CString(const CString& str)
{
	new(this) CString();
	(*m_strString) = (*str.m_strString);
}


CString::~CString()
{
	if (m_strString)
	{
		delete m_strString;
		m_strString = NULL;
	}

	if (m_strTemp)
	{
		delete m_strTemp;
		m_strTemp = NULL;
	}
}


string CString::UnicodeToANSI(LPCWSTR strWide)
{
	string strResult;
	int nLen = (int)wcslen(strWide);
	LPSTR szMulti = new char[nLen + 1];
	memset(szMulti, 0, nLen + 1);
	// use the c standard library function to convert
	wcstombs(szMulti,strWide,nLen);
	//WideCharToMultiByte(CP_ACP, 0, strWide, (int)wcslen(strWide), szMulti, nLen, NULL, FALSE);
	strResult = szMulti;
	delete[] szMulti;
	return strResult;
}

wstring CString::ANSIToUnicode(LPCSTR strMulti)
{
	wstring strResult;
	int nLen = (int)strlen(strMulti);
	LPWSTR strWide = new WCHAR[nLen + 1];
	memset(strWide, 0, sizeof(TCHAR)*(nLen + 1));
	mbstowcs(strWide,strMulti,nLen);
	//MultiByteToWideChar(CP_ACP, 0, strMulti, (int)strlen(strMulti), strWide, nLen);
	strResult = strWide;
	delete[] strWide;
	return strResult;
}


string CString::GetStringA() const
{
	return UnicodeToANSI(m_strString->c_str());
}

wstring CString::GetStringW() const
{
	return (*m_strString);
}

void Sloong::Universal::CString::FormatA(LPCSTR lpStr, ...)
{
	va_list args;
	va_start(args, lpStr);
	char szBuffer[MAX_BUFFER];
	vsnprintf(szBuffer, MAX_BUFFER, lpStr, args);
	(*m_strString) = ANSIToUnicode(szBuffer);
	va_end(args);
}

void Sloong::Universal::CString::FormatW(LPCWSTR lpStr, ...)
{
	va_list args;
	va_start(args, lpStr);
	WCHAR szBuffer[MAX_BUFFER] = {0};
	vswprintf(szBuffer, MAX_BUFFER, lpStr, args);
	va_end(args);
	(*m_strString) = szBuffer;
}

CString& Sloong::Universal::CString::operator=(LPCSTR lpStr)
{
	(*m_strString) = ANSIToUnicode(lpStr);
	return *this;
}

CString& Sloong::Universal::CString::operator=(LPCWSTR lpStr)
{
	(*m_strString) = lpStr;
	return *this;
}

CString& Sloong::Universal::CString::operator=(string lpStr)
{
	(*m_strString) = ANSIToUnicode(lpStr.c_str());
	return *this;
}

CString& Sloong::Universal::CString::operator=(wstring lpStr)
{
	(*m_strString) = lpStr;
	return *this;
}

CString& Sloong::Universal::CString::operator=(const CString& str)
{
	(*m_strString) = (*str.m_strString);
	return *this;
}

LPCWSTR Sloong::Universal::CString::w_str() const
{
	return m_strString->c_str();
}

LPCSTR Sloong::Universal::CString::a_str() const
{
	if ( m_strTemp == NULL )
	{
		m_strTemp = new string();
	}

	(*m_strTemp) = UnicodeToANSI(m_strString->c_str());
	return m_strTemp->c_str();
}
