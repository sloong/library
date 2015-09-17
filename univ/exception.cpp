#include "stdafx.h"
#include "exception.h"

using namespace Sloong::Universal;


Sloong::Universal::normal_except::normal_except()
{

}

Sloong::Universal::normal_except::normal_except(CString lpstr)
{
	m_strMessage = lpstr;
}

Sloong::Universal::normal_except::normal_except(const normal_except&)
{

}

Sloong::Universal::normal_except::normal_except(CString lpStr, long hRes)
{
	m_strMessage = lpStr;
	m_hResult = hRes;
}

normal_except& Sloong::Universal::normal_except::operator=(const normal_except&)
{

}

Sloong::Universal::normal_except::~normal_except()
{

}

const char* Sloong::Universal::normal_except::what() const noexcept
{
	return m_strMessage.a_str();
}
