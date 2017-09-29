﻿#ifdef _WINDOWS
	#ifdef SLOONGUNIVERSAL_EXPORTS
		#define UNIVERSAL_API __declspec(dllexport)
	#else
		#define UNIVERSAL_API __declspec(dllimport)
	#endif
#else
	#define UNIVERSAL_API
#endif


#ifndef UNIV_H
#define UNIV_H

#include <map>
#include <vector>
#include <list>
#include <string>
#include <fstream>
#include <sstream>
using namespace std;
#include "defines.h"
#include <boost/format.hpp>
#define	ACC_R	4		/* Test for read permission.  */
#define	ACC_W	2		/* Test for write permission.  */
#define	ACC_RUN	1		/* Test for execute permission.  */
#define	ACC_E	0		/* Test for existence.  */
#define ACC_RW  6


#ifdef _WINDOWS
#include <WinSock2.h>
#pragma comment(lib,"ws2_32.lib")
#else
#include <arpa/inet.h>
#include <string.h>
#include <inttypes.h>
inline uint64_t htonll(uint64_t val) {
	return  (((uint64_t)htonl(val)) << 32) + htonl(val >> 32);
}

inline uint64_t ntohll(uint64_t val) {
	return  (((uint64_t)ntohl(val)) << 32) + ntohl(val >> 32);
}
#define SOCKET int

#endif

namespace Sloong
{
	namespace Universal
	{
		class UNIVERSAL_API CUniversal
		{
		public:
			static wstring Version();
			static void CopyStringToPoint(LPSTR& lpTarget, LPCSTR lpFrom);
			static void CopyStringToPoint(LPWSTR& lpTarget, LPCWSTR lpFrom);
			static wstring FormatWindowsErrorMessage(DWORD dwErrCode);
			static string CheckFileDirectory(string filepath);
			static int MoveFile(string lpExistingFileName, string lpNewFileName);
			static int splitString(const string& str, vector<string>& res, string sep = ",");
			static string trim(const string& str);
			static wstring trim(const wstring& str);
			static string replace(const string& str, const string& src, const string& dest);
			static wstring replace(const wstring& str, const wstring& src, const wstring& dest);
			static string toansi(const wstring& str);
			static wstring toutf(const string& str);	
			static bool RunSystemCmd(string cmd);

			static inline void LongToBytes(long long l, char* pBuf)
			{
				auto ul_MessageLen = htonll(l);
				memcpy(pBuf, (void*)&ul_MessageLen, 8);
			}

			static inline long long BytesToLong(char* point)
			{
				long long netLen = 0;
				memcpy(&netLen, point, 8);
				return ntohll(netLen);
			}

			/************************************************************************/
			/*		SendEx function.
			Params:
				sock	-> the socket handle
				buf		-> the data buffer
				nSize	-> the send size
				nStart	-> the offset for the start index.
				eagain	-> continue when the EINTR,EAGAIN error if value is true.
							else return direct. in default is false.  *Only LinuxOS
			Return:
				如果请求成功，返回大于0的接收数据长度。
				如果接收超时，返回0。
				如果发生EINTR,EAGAIN错误且eagain为true，返回0。 *Only LinuxOS
				如果发生其他错误，返回-1。			*/
			/************************************************************************/
			static int SendEx(SOCKET sock, const char* buf, int nSize, int nStart=0, bool eagain=false);
			/************************************************************************/
			/*		ReceEx function.
			Params:
				sock	-> the socket handle
				buf		-> the data buffer
				nSize	-> the receive size
				nTimeout-> timeout time, default is 0. no need timeout
				eagain	-> continue when the EINTR,EAGAIN error if value is true.
							else return direct. in default is false.    *Only LinuxOS
			Return:
				如果请求成功，返回大于0的接收数据长度。
				如果接收超时，返回0。
				如果发生EINTR,EAGAIN错误且eagain为true，返回0。 *Only LinuxOS
				如果发生其他错误，返回-1。			*/
			/************************************************************************/
			static int RecvEx(int sock, char* buf, int nSize, int nTimeout, bool bAgain = false );

            template<typename T>
            static string ntos(T n)
            {
                stringstream ss;
                ss << n;
                return ss.str();
            }

            // packaging the boost::format;
            template<class TFirst>
            static void Format(boost::format& fmt, TFirst&& first) {  fmt % first; }

            template<class TFirst>
            static string Format(const char* format, TFirst&& first)
            {
                boost::format fmt(format);
                Format(fmt, first);
                return fmt.str();
            }

            template<class TFirst, class... TOther>
            static void Format(boost::format& fmt, TFirst&& first, TOther&&... other)
            {
                fmt % first;
                Format(fmt, other...);
            }

            template<class TFirst, class... TOther>
            static string Format(const char* format, TFirst&& first, TOther&&... other)
            {
                boost::format fmt(format);
                Format(fmt, first, other...);
                return fmt.str();
            }

			// packaging the boost::wformat;
			template<class TFirst>
			static void Format(boost::wformat& fmt, TFirst&& first) { fmt % first; }

			template<class TFirst>
			static wstring Format(const wchar_t* format, TFirst&& first)
			{
				boost::wformat fmt(format);
				Format(fmt, first);
				return fmt.str();
			}

			template<class TFirst, class... TOther>
			static void Format(boost::wformat& fmt, TFirst&& first, TOther&&... other)
			{
				fmt % first;
				Format(fmt, other...);
			}

			template<class TFirst, class... TOther>
			static wstring Format(const wchar_t* format, TFirst&& first, TOther&&... other)
			{
				boost::wformat fmt(format);
				Format(fmt, first, other...);
				return fmt.str();
			}

            static void tolower(string& str)
            {
                transform(str.begin(), str.end(), str.begin(), ::tolower);
            }

            static void touper(string& str)
            {
                transform(str.begin(), str.end(), str.begin(), ::toupper);
            }

			static string Replace(const string& str, const string& src, const string& dest);

		};

#ifdef _WINDOWS
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
				 int initCX,
				 int initCY) throw();
			// create from another size
			CSize( SIZE initSize) throw();
			// create from a point
			CSize( POINT initPt) throw();
			// create from a DWORD: cx = LOWORD(dw) cy = HIWORD(dw)
			CSize( DWORD dwSize) throw();

			// Operations
			BOOL operator==( SIZE size) const throw();
			BOOL operator!=( SIZE size) const throw();
			void operator+=( SIZE size) throw();
			void operator-=( SIZE size) throw();
			void SetSize( int CX,  int CY) throw();

			// Operators returning CSize values
			CSize operator+( SIZE size) const throw();
			CSize operator-( SIZE size) const throw();
			CSize operator-() const throw();

			// Operators returning CPoint values
			CPoint operator+( POINT point) const throw();
			CPoint operator-( POINT point) const throw();

			// Operators returning CRect values
			CRect operator+( const RECT* lpRect) const throw();
			CRect operator-( const RECT* lpRect) const throw();
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
				 int initX,
				 int initY) throw();
			// create from another point
			CPoint( POINT initPt) throw();
			// create from a size
			CPoint( SIZE initSize) throw();
			// create from an LPARAM: x = LOWORD(dw) y = HIWORD(dw)
			CPoint( LPARAM dwPoint) throw();


			// Operations

			// translate the point
			void Offset(
				 int xOffset,
				 int yOffset) throw();
			void Offset( POINT point) throw();
			void Offset( SIZE size) throw();
			void SetPoint(
				 int X,
				 int Y) throw();

			BOOL operator==( POINT point) const throw();
			BOOL operator!=( POINT point) const throw();
			void operator+=( SIZE size) throw();
			void operator-=( SIZE size) throw();
			void operator+=( POINT point) throw();
			void operator-=( POINT point) throw();

			// Operators returning CPoint values
			CPoint operator+( SIZE size) const throw();
			CPoint operator-( SIZE size) const throw();
			CPoint operator-() const throw();
			CPoint operator+( POINT point) const throw();

			// Operators returning CSize values
			CSize operator-( POINT point) const throw();

			// Operators returning CRect values
			CRect operator+( const RECT* lpRect) const throw();
			CRect operator-( const RECT* lpRect) const throw();
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
				int l,
				int t,
				int r,
				int b) throw();
			// copy constructor
			CRect(const RECT& srcRect) throw();

			// from a pointer to another rect
			CRect(LPCRECT lpSrcRect) throw();
			// from a point and size
			CRect(
				POINT point,
				SIZE size) throw();
			// from two points
			CRect(
				POINT topLeft,
				POINT bottomRight) throw();

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
			BOOL PtInRect(POINT point) const throw();

			// Operations

			// set rectangle from left, top, right, and bottom
			void SetRect(
				int x1,
				int y1,
				int x2,
				int y2) throw();
			void SetRect(
				POINT topLeft,
				POINT bottomRight) throw();
			// empty the rectangle
			void SetRectEmpty() throw();
			// copy from another rectangle
			void CopyRect(LPCRECT lpSrcRect) throw();
			// TRUE if exactly the same as another rectangle
			BOOL EqualRect(LPCRECT lpRect) const throw();

			// Inflate rectangle's width and height by
			// x units to the left and right ends of the rectangle
			// and y units to the top and bottom.
			void InflateRect(
				int x,
				int y) throw();
			// Inflate rectangle's width and height by
			// size.cx units to the left and right ends of the rectangle
			// and size.cy units to the top and bottom.
			void InflateRect(SIZE size) throw();
			// Inflate rectangle's width and height by moving individual sides.
			// Left side is moved to the left, right side is moved to the right,
			// top is moved up and bottom is moved down.
			void InflateRect(LPCRECT lpRect) throw();
			void InflateRect(
				int l,
				int t,
				int r,
				int b) throw();

			// deflate the rectangle's width and height without
			// moving its top or left
			void DeflateRect(
				int x,
				int y) throw();
			void DeflateRect(SIZE size) throw();
			void DeflateRect(LPCRECT lpRect) throw();
			void DeflateRect(
				int l,
				int t,
				int r,
				int b) throw();

			// translate the rectangle by moving its top and left
			void OffsetRect(
				int x,
				int y) throw();
			void OffsetRect(SIZE size) throw();
			void OffsetRect(POINT point) throw();
			void NormalizeRect() throw();

			// absolute position of rectangle
			void MoveToY(int y) throw();
			void MoveToX(int x) throw();
			void MoveToXY(
				int x,
				int y) throw();
			void MoveToXY(POINT point) throw();

			// set this rectangle to intersection of two others
			BOOL IntersectRect(
				LPCRECT lpRect1,
				LPCRECT lpRect2) throw();

			// set this rectangle to bounding union of two others
			BOOL UnionRect(
				LPCRECT lpRect1,
				LPCRECT lpRect2) throw();

			// set this rectangle to minimum of two others
			BOOL SubtractRect(
				LPCRECT lpRectSrc1,
				LPCRECT lpRectSrc2) throw();

			// Additional Operations
			void operator=(const RECT& srcRect) throw();
			BOOL operator==(const RECT& rect) const throw();
			BOOL operator!=(const RECT& rect) const throw();
			void operator+=(POINT point) throw();
			void operator+=(SIZE size) throw();
			void operator+=(LPCRECT lpRect) throw();
			void operator-=(POINT point) throw();
			void operator-=(SIZE size) throw();
			void operator-=(LPCRECT lpRect) throw();
			void operator&=(const RECT& rect) throw();
			void operator|=(const RECT& rect) throw();

			// Operators returning CRect values
			CRect operator+(POINT point) const throw();
			CRect operator-(POINT point) const throw();
			CRect operator+(LPCRECT lpRect) const throw();
			CRect operator+(SIZE size) const throw();
			CRect operator-(SIZE size) const throw();
			CRect operator-(LPCRECT lpRect) const throw();
			CRect operator&(const RECT& rect2) const throw();
			CRect operator|(const RECT& rect2) const throw();
			CRect MulDiv(
				int nMultiplier,
				int nDivisor) const throw();
		};
#endif // !_WINDOWS
	}
}

#endif //UNIV_H
