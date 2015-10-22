#include "stdafx.h"
#include "exception.h"

using namespace Sloong::Universal;


Sloong::Universal::normal_except::normal_except()
{

}

Sloong::Universal::normal_except::normal_except(std::string lpstr)
{
	m_strMessage = lpstr;
}

Sloong::Universal::normal_except::normal_except(const normal_except&)
{

}

Sloong::Universal::normal_except::normal_except(std::string lpStr, long hRes)
{
	m_strMessage = lpStr;
	m_hResult = hRes;
}

normal_except& Sloong::Universal::normal_except::operator=(const normal_except&)
{
	return (*this);
}

Sloong::Universal::normal_except::~normal_except()
{

}

const char* Sloong::Universal::normal_except::what() const
{
	return m_strMessage.c_str();
}
