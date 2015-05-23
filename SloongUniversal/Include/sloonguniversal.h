#pragma once

#ifdef UNIVERSAL_EXPORTS
#define UNIVERSAL_API __declspec(dllexport)
#define EXPIMP_TEMPLATE
#else
#define UNIVERSAL_API __declspec(dllimport)
#define EXPIMP_TEMPLATE extern
#endif


#pragma region Sloong Universal Defines

#ifndef _ARGB
#define _ARGB(a,r,g,b) ((ULONG)((((a)&0xff)<<24)|(((r)&0xff)<<16)|(((g)&0xff)<<8)|((b)&0xff)))
#endif	// _ARGB

#ifndef SAFE_DELETE
#define SAFE_DELETE(p)		{if(NULL != (p)){delete (p);(p)=NULL;}}
#endif	// SAFE_DELETE

#ifndef SAFE_DELETE_ARR
#define SAFE_DELETE_ARR(p)	{if(NULL != (p)){delete[] (p);(p)=NULL;}}
#endif	// SAFE_DELETE_ARR

#ifndef SAFE_SHUTDOWN
#define SAFE_SHUTDOWN(p)	{if(NULL != (p)){(p)->Shutdown();(p)=NULL;}}
#endif	// SAFE_SHUTDOWN

#ifndef SAFE_DESTROY
#define SAFE_DESTROY(p)		{if(NULL != (p)){(p)->Destroy();(p)=NULL;}}
#endif	// SAFE_DESTROY

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)		{if(NULL != (p)){(p)->Release();p = NULL;}}
#endif // !SAFE_RELEASE

#ifndef SAFE_DEL_ALL_DATA_FROM_LIST
#define SAFE_DEL_ALL_DATA_FROM_LIST(p,type,del) {if(NULL!=p){for(int i=0;i<p->m_nNum;i++){CLinkList* pNode=p->Locate(i);type pData=(type)pNode->GetData();del(pData);pNode->SetData(NULL,false);}}}
#endif // !SAFE_DEL_ALL_DATA_FROM_LIST

#ifdef _UNICODE
#define _tofstream		wofstream
#define _tifstream		wifstream
#define _tmemcpy		wmemcpy
#define _tmemcpy_s		wmemcpy_s
#define _tcscpy			wcscpy
#define _ftprintf		fwprintf
#define _tfopen			_wfopen
#define _tfopen_s		_wfopen_s
#define _stscanf		swscanf
#define _stscanf_s		swscanf_s
#define _sntprintf_s	_snwprintf_s
#define _sntprintf		_snwprintf
#define _tmemmove		wmemmove
#define _ttof			_wtof
#define _tcschr			wcschr
#define _fgetts			fgetws
#define _fputts			fputws
#define _tprintf		wprintf
#define _tsprintf		wsprintf
#define _tvsprintf		wvsprintf
#define _tvsprintf_s	vswprintf_s
#define _tcscat_s		wcscat_s
#define _ttoi			_wtoi
#define _tstring		wstring
#define UNSIGNED
#define __STR2WSTR(str) L##str
#define _STR2WSTR(str) __STR2WSTR(str)
#define __FUNCTIONW__ _STR2WSTR(__FUNCTION__)
#define __TFILE__		__FILEW__
#define __TFUNCTION__	__FUNCTIONW__
typedef wstring tstring;
typedef const wstring ctstring;
#define stscanf swscanf
#define stscanf_s swscanf_s
#define fgetts fgetws
#else
#define _tofstream		ofstream
#define _tifstream		ifstream
#define _tmemcpy		memcpy
#define _tmemcpy_s		memcpy_s
#define _tcscpy			strcpy
#define _ftprintf		fprintf
#define _tfopen			fopen
#define _tfopen_s		fopen_s
#define _stscanf		sscanf
#define _stscanf_s		sscanf_s
#define _sntprintf		_snprintf
#define _sntprintf_s	_snprintf_s
#define _tmemmove		memmove
#define _ttof			atof
#define _tcschr			strchr
#define _fgetts			fgets
#define _fputts			fputs
#define _tprintf		printf
#define _tsprintf		wsprintf
#define _tvsprintf		vsprintf
#define _tvsprintf_s	vsprintf_s
#define _tcscat_s		strcat_s 
#define _ttoi			atoi
#ifndef	UNSIGNED
#define UNSIGNED		unsigned
#endif // !UNSIGNED
#define _tstring		string
#define __TFILE__		__FILE__
#define __TFUNCTION__	__FUNCTION__
typedef string tstring;
typedef const string ctstring;
#define stscanf sscanf
#define stscanf_s swscanf_s
#define fgetts fgets
#endif //_UNICODE
#pragma endregion


namespace Sloong
{

	namespace Universal
	{
		EXPIMP_TEMPLATE template class UNIVERSAL_API std::basic_string<wchar_t, char_traits<wchar_t>, allocator<wchar_t> >;
		class UNIVERSAL_API CUniversal
		{
		public:
			static wstring Version();
			static wstring Format(wstring lpstr, ...);
			static string Format(string lpstr, ...);
			static wstring ToWString(string str);
			static string ToString(wstring str);
			static void CopyStringToPoint(LPTSTR& lpTarget, LPCTSTR lpFrom);
			static wstring FormatWindowsErrorMessage(DWORD dwErrCode);
		};

#pragma region Windows Define
		class CSize;
		class CPoint;
		class CRect;
		/////////////////////////////////////////////////////////////////////////////
		// CSize - An extent, similar to Windows SIZE structure.
		class UNIVERSAL_API CSize : public tagSIZE
		{
		public:

			// Constructors
			// construct an uninitialized size
			CSize() throw();
			// create from two integers
			CSize(
				_In_ int initCX,
				_In_ int initCY) throw();
			// create from another size
			CSize(_In_ SIZE initSize) throw();
			// create from a point
			CSize(_In_ POINT initPt) throw();
			// create from a DWORD: cx = LOWORD(dw) cy = HIWORD(dw)
			CSize(_In_ DWORD dwSize) throw();

			// Operations
			BOOL operator==(_In_ SIZE size) const throw();
			BOOL operator!=(_In_ SIZE size) const throw();
			void operator+=(_In_ SIZE size) throw();
			void operator-=(_In_ SIZE size) throw();
			void SetSize(_In_ int CX, _In_ int CY) throw();

			// Operators returning CSize values
			CSize operator+(_In_ SIZE size) const throw();
			CSize operator-(_In_ SIZE size) const throw();
			CSize operator-() const throw();

			// Operators returning CPoint values
			CPoint operator+(_In_ POINT point) const throw();
			CPoint operator-(_In_ POINT point) const throw();

			// Operators returning CRect values
			CRect operator+(_In_ const RECT* lpRect) const throw();
			CRect operator-(_In_ const RECT* lpRect) const throw();
		};

		/////////////////////////////////////////////////////////////////////////////
		// CPoint - A 2-D point, similar to Windows POINT structure.
		class UNIVERSAL_API CPoint : public tagPOINT
		{
		public:
			// Constructors

			// create an uninitialized point
			CPoint() throw();
			// create from two integers
			CPoint(
				_In_ int initX,
				_In_ int initY) throw();
			// create from another point
			CPoint(_In_ POINT initPt) throw();
			// create from a size
			CPoint(_In_ SIZE initSize) throw();
			// create from an LPARAM: x = LOWORD(dw) y = HIWORD(dw)
			CPoint(_In_ LPARAM dwPoint) throw();


			// Operations

			// translate the point
			void Offset(
				_In_ int xOffset,
				_In_ int yOffset) throw();
			void Offset(_In_ POINT point) throw();
			void Offset(_In_ SIZE size) throw();
			void SetPoint(
				_In_ int X,
				_In_ int Y) throw();

			BOOL operator==(_In_ POINT point) const throw();
			BOOL operator!=(_In_ POINT point) const throw();
			void operator+=(_In_ SIZE size) throw();
			void operator-=(_In_ SIZE size) throw();
			void operator+=(_In_ POINT point) throw();
			void operator-=(_In_ POINT point) throw();

			// Operators returning CPoint values
			CPoint operator+(_In_ SIZE size) const throw();
			CPoint operator-(_In_ SIZE size) const throw();
			CPoint operator-() const throw();
			CPoint operator+(_In_ POINT point) const throw();

			// Operators returning CSize values
			CSize operator-(_In_ POINT point) const throw();

			// Operators returning CRect values
			CRect operator+(_In_ const RECT* lpRect) const throw();
			CRect operator-(_In_ const RECT* lpRect) const throw();
		};

		/////////////////////////////////////////////////////////////////////////////
		// CRect - A 2-D rectangle, similar to Windows RECT structure.
		class UNIVERSAL_API CRect : public tagRECT
		{
			// Constructors
		public:
			// uninitialized rectangle
			CRect() throw();
			// from left, top, right, and bottom
			CRect(
				_In_ int l,
				_In_ int t,
				_In_ int r,
				_In_ int b) throw();
			// copy constructor
			CRect(_In_ const RECT& srcRect) throw();

			// from a pointer to another rect
			CRect(_In_ LPCRECT lpSrcRect) throw();
			// from a point and size
			CRect(
				_In_ POINT point,
				_In_ SIZE size) throw();
			// from two points
			CRect(
				_In_ POINT topLeft,
				_In_ POINT bottomRight) throw();

			// Attributes (in addition to RECT members)

			// retrieves the width
			int Width() const throw();
			// returns the height
			int Height() const throw();
			// returns the size
			CSize Size() const throw();
			// reference to the top-left point
			CPoint& TopLeft() throw();
			// reference to the bottom-right point
			CPoint& BottomRight() throw();
			// const reference to the top-left point
			const CPoint& TopLeft() const throw();
			// const reference to the bottom-right point
			const CPoint& BottomRight() const throw();
			// the geometric center point of the rectangle
			CPoint CenterPoint() const throw();
			// swap the left and right
			void SwapLeftRight() throw();
			static void WINAPI SwapLeftRight(_Inout_ LPRECT lpRect) throw();

			// convert between CRect and LPRECT/LPCRECT (no need for &)
			operator LPRECT() throw();
			operator LPCRECT() const throw();

			// returns TRUE if rectangle has no area
			BOOL IsRectEmpty() const throw();
			// returns TRUE if rectangle is at (0,0) and has no area
			BOOL IsRectNull() const throw();
			// returns TRUE if point is within rectangle
			BOOL PtInRect(_In_ POINT point) const throw();

			// Operations

			// set rectangle from left, top, right, and bottom
			void SetRect(
				_In_ int x1,
				_In_ int y1,
				_In_ int x2,
				_In_ int y2) throw();
			void SetRect(
				_In_ POINT topLeft,
				_In_ POINT bottomRight) throw();
			// empty the rectangle
			void SetRectEmpty() throw();
			// copy from another rectangle
			void CopyRect(_In_ LPCRECT lpSrcRect) throw();
			// TRUE if exactly the same as another rectangle
			BOOL EqualRect(_In_ LPCRECT lpRect) const throw();

			// Inflate rectangle's width and height by
			// x units to the left and right ends of the rectangle
			// and y units to the top and bottom.
			void InflateRect(
				_In_ int x,
				_In_ int y) throw();
			// Inflate rectangle's width and height by
			// size.cx units to the left and right ends of the rectangle
			// and size.cy units to the top and bottom.
			void InflateRect(_In_ SIZE size) throw();
			// Inflate rectangle's width and height by moving individual sides.
			// Left side is moved to the left, right side is moved to the right,
			// top is moved up and bottom is moved down.
			void InflateRect(_In_ LPCRECT lpRect) throw();
			void InflateRect(
				_In_ int l,
				_In_ int t,
				_In_ int r,
				_In_ int b) throw();

			// deflate the rectangle's width and height without
			// moving its top or left
			void DeflateRect(
				_In_ int x,
				_In_ int y) throw();
			void DeflateRect(_In_ SIZE size) throw();
			void DeflateRect(_In_ LPCRECT lpRect) throw();
			void DeflateRect(
				_In_ int l,
				_In_ int t,
				_In_ int r,
				_In_ int b) throw();

			// translate the rectangle by moving its top and left
			void OffsetRect(
				_In_ int x,
				_In_ int y) throw();
			void OffsetRect(_In_ SIZE size) throw();
			void OffsetRect(_In_ POINT point) throw();
			void NormalizeRect() throw();

			// absolute position of rectangle
			void MoveToY(_In_ int y) throw();
			void MoveToX(_In_ int x) throw();
			void MoveToXY(
				_In_ int x,
				_In_ int y) throw();
			void MoveToXY(_In_ POINT point) throw();

			// set this rectangle to intersection of two others
			BOOL IntersectRect(
				_In_ LPCRECT lpRect1,
				_In_ LPCRECT lpRect2) throw();

			// set this rectangle to bounding union of two others
			BOOL UnionRect(
				_In_ LPCRECT lpRect1,
				_In_ LPCRECT lpRect2) throw();

			// set this rectangle to minimum of two others
			BOOL SubtractRect(
				_In_ LPCRECT lpRectSrc1,
				_In_ LPCRECT lpRectSrc2) throw();

			// Additional Operations
			void operator=(_In_ const RECT& srcRect) throw();
			BOOL operator==(_In_ const RECT& rect) const throw();
			BOOL operator!=(_In_ const RECT& rect) const throw();
			void operator+=(_In_ POINT point) throw();
			void operator+=(_In_ SIZE size) throw();
			void operator+=(_In_ LPCRECT lpRect) throw();
			void operator-=(_In_ POINT point) throw();
			void operator-=(_In_ SIZE size) throw();
			void operator-=(_In_ LPCRECT lpRect) throw();
			void operator&=(_In_ const RECT& rect) throw();
			void operator|=(_In_ const RECT& rect) throw();

			// Operators returning CRect values
			CRect operator+(_In_ POINT point) const throw();
			CRect operator-(_In_ POINT point) const throw();
			CRect operator+(_In_ LPCRECT lpRect) const throw();
			CRect operator+(_In_ SIZE size) const throw();
			CRect operator-(_In_ SIZE size) const throw();
			CRect operator-(_In_ LPCRECT lpRect) const throw();
			CRect operator&(_In_ const RECT& rect2) const throw();
			CRect operator|(_In_ const RECT& rect2) const throw();
			CRect MulDiv(
				_In_ int nMultiplier,
				_In_ int nDivisor) const throw();
		};

#pragma endregion

	}
}
