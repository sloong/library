// SLUniversal.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include <iostream>
#include <map>
#include "log.h"
#include "univ.h"
#include "Version.h"
#include <assert.h>
#ifndef _WINDOWS
#include <libgen.h>
#endif // !_WINDOWS

using namespace std;
using namespace Sloong::Universal;

typedef map<int, string> MSGMAP;
MSGMAP g_MessageMap;

wstring CUniversal::Version()
{
	return VERSION_LEGALCOPYRIGHT;
}

bool g_bIsInst = false;

void CUniversal::CopyStringToPoint(LPSTR& lpTarget, LPCSTR lpFrom)
{
	SAFE_DELETE_ARR(lpTarget);
	int nLength = (int)strlen(lpFrom);
	lpTarget = new char[nLength + 1];
	assert(lpTarget);
	strncpy(lpTarget, lpFrom, nLength);
}

void CUniversal::CopyStringToPoint(LPWSTR& lpTarget, LPCWSTR lpFrom)
{
	SAFE_DELETE_ARR(lpTarget);
	int nLength = (int)wcslen(lpFrom);
	lpTarget = new wchar_t[nLength + 1];
	assert(lpTarget);
	wcsncpy(lpTarget, lpFrom, nLength);
}



string CUniversal::trim(const string& str)
{
    string::size_type pos = str.find_first_not_of(' ');
    if (pos == string::npos)
    {
        return str;
    }
    string::size_type pos2 = str.find_last_not_of(' ');
    if (pos2 != string::npos)
    {
        return str.substr(pos, pos2 - pos + 1);
    }
    return str.substr(pos);
}


int CUniversal::splitString(const string& str, vector<string>& ret_, string sep /* = "," */)
{
    if (str.empty())
    {
        return 0;
    }

    string tmp;
    string::size_type pos_begin = str.find_first_not_of(sep);
    string::size_type comma_pos = 0;

    while (pos_begin != string::npos)
    {
        comma_pos = str.find(sep, pos_begin);
        if (comma_pos != string::npos)
        {
            tmp = str.substr(pos_begin, comma_pos - pos_begin);
            pos_begin = comma_pos + sep.length();
        }
        else
        {
            tmp = str.substr(pos_begin);
            pos_begin = comma_pos;
        }

        if (!tmp.empty())
        {
            ret_.push_back(tmp);
            tmp.clear();
        }
    }
    return 0;
}

string CUniversal::replace(const string& str, const string& src, const string& dest)
{
    string ret;

    string::size_type pos_begin = 0;
    string::size_type pos       = str.find(src);
    while (pos != string::npos)
    {
        cout <<"replacexxx:" << pos_begin <<" " << pos <<"\n";
        ret.append(str.data() + pos_begin, pos - pos_begin);
        ret += dest;
        pos_begin = pos + 1;
        pos       = str.find(src, pos_begin);
    }
    if (pos_begin < str.length())
    {
        ret.append(str.begin() + pos_begin, str.end());
    }
    return ret;
}

string Sloong::Universal::CUniversal::toansi(const wstring& str)
{
	string strResult;
	int nLen = (int)str.size();
	LPSTR szMulti = new char[nLen + 1];
	memset(szMulti, 0, nLen + 1);
	// use the c standard library function to convert
	wcstombs(szMulti, str.c_str(), nLen);
	strResult = szMulti;
	delete[] szMulti;
	return strResult;


}

wstring Sloong::Universal::CUniversal::toutf(const string& str)
{
	wstring strResult;
	int nLen = (int)str.size();
	LPWSTR strWide = new WCHAR[nLen + 1];
	memset(strWide, 0, sizeof(TCHAR)*(nLen + 1));
	mbstowcs(strWide,str.c_str(),nLen);
	strResult = strWide;
	delete[] strWide;
	return strResult;
}

#ifndef _WINDOWS


void Sloong::Universal::_splitpath(const char *path, char *drive, char *dir, char *fname, char *ext)
{

	char *tmp_path;
	int conut;
	char tmp[MAX_PATH];
	char tmp_name[MAX_PATH];
	char tmp_ext[MAX_PATH];
	int  check = 0;
	int check_1 = 0;
	int i, j = 0, k = 0;
	int conut_1, conut_2, conut_3;

	tmp_path = NULL;
	if ((realpath(path, tmp_path) == NULL))
	{
		tmp_path = (char*)path;
	}

	conut = 0;
	do
	{
		if (tmp_path[conut] == ':')
		{
			check = 1;
			conut_1 = conut;
		}
		if (tmp_path[conut] == '/')
		{
			conut_2 = conut;
		}
		if (tmp_path[conut] == '.')
		{
			check_1 = 1;
			conut_3 = conut;
		}
		conut++;
	} while (tmp_path[conut] != '\0');

	if (check == 1)
	{
		fname = basename(tmp_path);
		dir = dirname(tmp_path);
		for (i = 0; i < conut; i++)
		{
			if (dir[i] != ':')
			{
				if (i >= 2)
				{
					tmp[i - 2] = dir[i];
				}
			}
			else
			{
				strncpy(drive, &dir[i - 1], 1);
				//drive=dir[i-1];

			}
		}

		memset(dir, 0, sizeof(dir));
		strcpy(dir, tmp);

	}
	else
	{
		fname = basename(tmp_path);
		dir = dirname(tmp_path);

	}

	if (check_1 == 1)
	{
		while (fname[j] != '\0')
		{

			if (fname[j] == '.')
			{
				k = j + 1;
			}
			else
			{
				tmp_name[j] = fname[j];
				if (j >= k && k != 0)
				{

					tmp_ext[j - k] = fname[j];
				}
			}

			if (fname[j] == '.')
			{
				k = j;
			}

			if (j >= k && k != 0)
			{
				tmp_ext[j - k] = fname[j];
			}
			else
			{
				tmp_name[j] = fname[j];

			}
			j++;
		}
		memset(ext, 0, sizeof(ext));
		memset(fname, 0, sizeof(fname));
		strcpy(fname, tmp_name);
		strcpy(ext, tmp_ext);

	}
	printf("drive=====%c\n", drive);
	printf("dir=======%s\n", dir);
	printf("fname========%s\n", fname);
	printf("ext======%s\n", ext);

}

#else

// Remarks:
//		Format the windows error message
wstring CUniversal::FormatWindowsErrorMessage( DWORD dwErrCode)
{
	wstring strError;
	TCHAR szErr[1024] = { 0 };
	DWORD systemLocale = MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US);
	DWORD dwLength = 0;
	FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, dwErrCode, systemLocale, szErr, 1024, NULL);
	strError = szErr;
	return strError;
}




// CSize
inline CSize::CSize() throw()
{
	cx = 0;
	cy = 0;
}

inline CSize::CSize(
	_In_ int initCX,
	_In_ int initCY) throw()
{
	cx = initCX;
	cy = initCY;
}

inline CSize::CSize(_In_ SIZE initSize) throw()
{
	*(SIZE*)this = initSize;
}

inline CSize::CSize(_In_ POINT initPt) throw()
{
	*(POINT*)this = initPt;
}

inline CSize::CSize(_In_ DWORD dwSize) throw()
{
	cx = (short)LOWORD(dwSize);
	cy = (short)HIWORD(dwSize);
}

inline BOOL CSize::operator==(_In_ SIZE size) const throw()
{
	return (cx == size.cx && cy == size.cy);
}

inline BOOL CSize::operator!=(_In_ SIZE size) const throw()
{
	return (cx != size.cx || cy != size.cy);
}

inline void CSize::operator+=(_In_ SIZE size) throw()
{
	cx += size.cx;
	cy += size.cy;
}

inline void CSize::operator-=(_In_ SIZE size) throw()
{
	cx -= size.cx;
	cy -= size.cy;
}

inline void CSize::SetSize(
	_In_ int CX,
	_In_ int CY) throw()
{
	cx = CX;
	cy = CY;
}

inline CSize CSize::operator+(_In_ SIZE size) const throw()
{
	return CSize(cx + size.cx, cy + size.cy);
}

inline CSize CSize::operator-(_In_ SIZE size) const throw()
{
	return CSize(cx - size.cx, cy - size.cy);
}

inline CSize CSize::operator-() const throw()
{
	return CSize(-cx, -cy);
}

inline CPoint CSize::operator+(_In_ POINT point) const throw()
{
	return CPoint(cx + point.x, cy + point.y);
}

inline CPoint CSize::operator-(_In_ POINT point) const throw()
{
	return CPoint(cx - point.x, cy - point.y);
}


// CPoint
inline CPoint::CPoint() throw()
{
	x = 0;
	y = 0;
}

inline CPoint::CPoint(
	_In_ int initX,
	_In_ int initY) throw()
{
	x = initX;
	y = initY;
}

inline CPoint::CPoint(_In_ POINT initPt) throw()
{
	*(POINT*)this = initPt;
}

inline CPoint::CPoint(_In_ SIZE initSize) throw()
{
	*(SIZE*)this = initSize;
}

inline CPoint::CPoint(_In_ LPARAM dwPoint) throw()
{
	x = (short)LOWORD(dwPoint);
	y = (short)HIWORD(dwPoint);
}

inline void CPoint::Offset(
	_In_ int xOffset,
	_In_ int yOffset) throw()
{
	x += xOffset;
	y += yOffset;
}

inline void CPoint::Offset(_In_ POINT point) throw()
{
	x += point.x;
	y += point.y;
}

inline void CPoint::Offset(_In_ SIZE size) throw()
{
	x += size.cx;
	y += size.cy;
}

inline void CPoint::SetPoint(
	_In_ int X,
	_In_ int Y) throw()
{
	x = X;
	y = Y;
}

inline BOOL CPoint::operator==(_In_ POINT point) const throw()
{
	return (x == point.x && y == point.y);
}

inline BOOL CPoint::operator!=(_In_ POINT point) const throw()
{
	return (x != point.x || y != point.y);
}

inline void CPoint::operator+=(_In_ SIZE size) throw()
{
	x += size.cx;
	y += size.cy;
}

inline void CPoint::operator-=(_In_ SIZE size) throw()
{
	x -= size.cx;
	y -= size.cy;
}

inline void CPoint::operator+=(_In_ POINT point) throw()
{
	x += point.x;
	y += point.y;
}

inline void CPoint::operator-=(_In_ POINT point) throw()
{
	x -= point.x;
	y -= point.y;
}

inline CPoint CPoint::operator+(_In_ SIZE size) const throw()
{
	return CPoint(x + size.cx, y + size.cy);
}

inline CPoint CPoint::operator-(_In_ SIZE size) const throw()
{
	return CPoint(x - size.cx, y - size.cy);
}

inline CPoint CPoint::operator-() const throw()
{
	return CPoint(-x, -y);
}

inline CPoint CPoint::operator+(_In_ POINT point) const throw()
{
	return CPoint(x + point.x, y + point.y);
}

inline CSize CPoint::operator-(_In_ POINT point) const throw()
{
	return CSize(x - point.x, y - point.y);
}




inline CRect CSize::operator+(_In_ const RECT* lpRect) const throw()
{
	return CRect(lpRect) + *this;
}

inline CRect CSize::operator-(_In_ const RECT* lpRect) const throw()
{
	return CRect(lpRect) - *this;
}



inline CRect CPoint::operator+(_In_ const RECT* lpRect) const throw()
{
	return CRect(lpRect) + *this;
}

inline CRect CPoint::operator-(_In_ const RECT* lpRect) const throw()
{
	return CRect(lpRect) - *this;
}


// CRect
inline CRect::CRect() throw()
{
	left = 0;
	top = 0;
	right = 0;
	bottom = 0;
}

inline CRect::CRect(
	_In_ int l,
	_In_ int t,
	_In_ int r,
	_In_ int b) throw()
{
	left = l;
	top = t;
	right = r;
	bottom = b;
}

inline CRect::CRect(_In_ const RECT& srcRect) throw()
{
	::CopyRect(this, &srcRect);
}

inline CRect::CRect(_In_ LPCRECT lpSrcRect) throw()
{
	::CopyRect(this, lpSrcRect);
}

inline CRect::CRect(
	_In_ POINT point,
	_In_ SIZE size) throw()
{
	right = (left = point.x) + size.cx;
	bottom = (top = point.y) + size.cy;
}

inline CRect::CRect(
	_In_ POINT topLeft,
	_In_ POINT bottomRight) throw()
{
	left = topLeft.x;
	top = topLeft.y;
	right = bottomRight.x;
	bottom = bottomRight.y;
}

inline int CRect::Width() const throw()
{
	return right - left;
}

inline int CRect::Height() const throw()
{
	return bottom - top;
}

inline CSize CRect::Size() const throw()
{
	return CSize(right - left, bottom - top);
}

inline CPoint& CRect::TopLeft() throw()
{
	return *((CPoint*)this);
}

inline CPoint& CRect::BottomRight() throw()
{
	return *((CPoint*)this + 1);
}

inline const CPoint& CRect::TopLeft() const throw()
{
	return *((CPoint*)this);
}

inline const CPoint& CRect::BottomRight() const throw()
{
	return *((CPoint*)this + 1);
}

inline CPoint CRect::CenterPoint() const throw()
{
	return CPoint((left + right) / 2, (top + bottom) / 2);
}

inline void CRect::SwapLeftRight() throw()
{
	SwapLeftRight(LPRECT(this));
}

inline void WINAPI CRect::SwapLeftRight(_Inout_ LPRECT lpRect) throw()
{
	LONG temp = lpRect->left;
	lpRect->left = lpRect->right;
	lpRect->right = temp;
}

inline CRect::operator LPRECT() throw()
{
	return this;
}

inline CRect::operator LPCRECT() const throw()
{
	return this;
}

inline BOOL CRect::IsRectEmpty() const throw()
{
	return ::IsRectEmpty(this);
}

inline BOOL CRect::IsRectNull() const throw()
{
	return (left == 0 && right == 0 && top == 0 && bottom == 0);
}

inline BOOL CRect::PtInRect(_In_ POINT point) const throw()
{
	return ::PtInRect(this, point);
}

inline void CRect::SetRect(
	_In_ int x1,
	_In_ int y1,
	_In_ int x2,
	_In_ int y2) throw()
{
	::SetRect(this, x1, y1, x2, y2);
}

inline void CRect::SetRect(
	_In_ POINT topLeft,
	_In_ POINT bottomRight) throw()
{
	::SetRect(this, topLeft.x, topLeft.y, bottomRight.x, bottomRight.y);
}

inline void CRect::SetRectEmpty() throw()
{
	::SetRectEmpty(this);
}

inline void CRect::CopyRect(_In_ LPCRECT lpSrcRect) throw()
{
	::CopyRect(this, lpSrcRect);
}

inline BOOL CRect::EqualRect(_In_ LPCRECT lpRect) const throw()
{
	return ::EqualRect(this, lpRect);
}

inline void CRect::InflateRect(
	_In_ int x,
	_In_ int y) throw()
{
	::InflateRect(this, x, y);
}

inline void CRect::InflateRect(_In_ SIZE size) throw()
{
	::InflateRect(this, size.cx, size.cy);
}

inline void CRect::DeflateRect(
	_In_ int x,
	_In_ int y) throw()
{
	::InflateRect(this, -x, -y);
}

inline void CRect::DeflateRect(_In_ SIZE size) throw()
{
	::InflateRect(this, -size.cx, -size.cy);
}

inline void CRect::OffsetRect(
	_In_ int x,
	_In_ int y) throw()
{
	::OffsetRect(this, x, y);
}

inline void CRect::OffsetRect(_In_ POINT point) throw()
{
	::OffsetRect(this, point.x, point.y);
}

inline void CRect::OffsetRect(_In_ SIZE size) throw()
{
	::OffsetRect(this, size.cx, size.cy);
}

inline void CRect::MoveToY(_In_ int y) throw()
{
	bottom = Height() + y;
	top = y;
}

inline void CRect::MoveToX(_In_ int x) throw()
{
	right = Width() + x;
	left = x;
}

inline void CRect::MoveToXY(
	_In_ int x,
	_In_ int y) throw()
{
	MoveToX(x);
	MoveToY(y);
}

inline void CRect::MoveToXY(_In_ POINT pt) throw()
{
	MoveToX(pt.x);
	MoveToY(pt.y);
}

inline BOOL CRect::IntersectRect(
	_In_ LPCRECT lpRect1,
	_In_ LPCRECT lpRect2) throw()
{
	return ::IntersectRect(this, lpRect1, lpRect2);
}

inline BOOL CRect::UnionRect(
	_In_ LPCRECT lpRect1,
	_In_ LPCRECT lpRect2) throw()
{
	return ::UnionRect(this, lpRect1, lpRect2);
}

inline void CRect::operator=(_In_ const RECT& srcRect) throw()
{
	::CopyRect(this, &srcRect);
}

inline BOOL CRect::operator==(_In_ const RECT& rect) const throw()
{
	return ::EqualRect(this, &rect);
}

inline BOOL CRect::operator!=(_In_ const RECT& rect) const throw()
{
	return !::EqualRect(this, &rect);
}

inline void CRect::operator+=(_In_ POINT point) throw()
{
	::OffsetRect(this, point.x, point.y);
}

inline void CRect::operator+=(_In_ SIZE size) throw()
{
	::OffsetRect(this, size.cx, size.cy);
}

inline void CRect::operator+=(_In_ LPCRECT lpRect) throw()
{
	InflateRect(lpRect);
}

inline void CRect::operator-=(_In_ POINT point) throw()
{
	::OffsetRect(this, -point.x, -point.y);
}

inline void CRect::operator-=(_In_ SIZE size) throw()
{
	::OffsetRect(this, -size.cx, -size.cy);
}

inline void CRect::operator-=(_In_ LPCRECT lpRect) throw()
{
	DeflateRect(lpRect);
}

inline void CRect::operator&=(_In_ const RECT& rect) throw()
{
	::IntersectRect(this, this, &rect);
}

inline void CRect::operator|=(_In_ const RECT& rect) throw()
{
	::UnionRect(this, this, &rect);
}

inline CRect CRect::operator+(_In_ POINT pt) const throw()
{
	CRect rect(*this);
	::OffsetRect(&rect, pt.x, pt.y);
	return rect;
}

inline CRect CRect::operator-(_In_ POINT pt) const throw()
{
	CRect rect(*this);
	::OffsetRect(&rect, -pt.x, -pt.y);
	return rect;
}

inline CRect CRect::operator+(_In_ SIZE size) const throw()
{
	CRect rect(*this);
	::OffsetRect(&rect, size.cx, size.cy);
	return rect;
}

inline CRect CRect::operator-(_In_ SIZE size) const throw()
{
	CRect rect(*this);
	::OffsetRect(&rect, -size.cx, -size.cy);
	return rect;
}

inline CRect CRect::operator+(_In_ LPCRECT lpRect) const throw()
{
	CRect rect(this);
	rect.InflateRect(lpRect);
	return rect;
}

inline CRect CRect::operator-(_In_ LPCRECT lpRect) const throw()
{
	CRect rect(this);
	rect.DeflateRect(lpRect);
	return rect;
}

inline CRect CRect::operator&(_In_ const RECT& rect2) const throw()
{
	CRect rect;
	::IntersectRect(&rect, this, &rect2);
	return rect;
}

inline CRect CRect::operator|(_In_ const RECT& rect2) const throw()
{
	CRect rect;
	::UnionRect(&rect, this, &rect2);
	return rect;
}

inline BOOL CRect::SubtractRect(
	_In_ LPCRECT lpRectSrc1,
	_In_ LPCRECT lpRectSrc2) throw()
{
	return ::SubtractRect(this, lpRectSrc1, lpRectSrc2);
}

inline void CRect::NormalizeRect() throw()
{
	int nTemp;
	if (left > right)
	{
		nTemp = left;
		left = right;
		right = nTemp;
	}
	if (top > bottom)
	{
		nTemp = top;
		top = bottom;
		bottom = nTemp;
	}
}

inline void CRect::InflateRect(_In_ LPCRECT lpRect) throw()
{
	left -= lpRect->left;
	top -= lpRect->top;
	right += lpRect->right;
	bottom += lpRect->bottom;
}

inline void CRect::InflateRect(
	_In_ int l,
	_In_ int t,
	_In_ int r,
	_In_ int b) throw()
{
	left -= l;
	top -= t;
	right += r;
	bottom += b;
}

inline void CRect::DeflateRect(_In_ LPCRECT lpRect) throw()
{
	left += lpRect->left;
	top += lpRect->top;
	right -= lpRect->right;
	bottom -= lpRect->bottom;
}

inline void CRect::DeflateRect(
	_In_ int l,
	_In_ int t,
	_In_ int r,
	_In_ int b) throw()
{
	left += l;
	top += t;
	right -= r;
	bottom -= b;
}

inline CRect CRect::MulDiv(
	_In_ int nMultiplier,
	_In_ int nDivisor) const throw()
{
	return CRect(
		::MulDiv(left, nMultiplier, nDivisor),
		::MulDiv(top, nMultiplier, nDivisor),
		::MulDiv(right, nMultiplier, nDivisor),
		::MulDiv(bottom, nMultiplier, nDivisor));
}

#endif // _WINDOWS
