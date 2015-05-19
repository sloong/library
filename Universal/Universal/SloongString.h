#pragma once

#include "IUniversal.h"

namespace Sloong
{
	namespace Universal
	{
		class CString //: wstring
		{
		public:
			CString();
			CString(LPCSTR lpStr, ...);
			CString(LPCWSTR lpStr, ...);
			~CString();

			CString& operator=(LPCSTR lpStr);
			CString& operator=(LPCWSTR lpStr);
			CString& operator=(string lpStr);
			CString& operator=(wstring lpstr);

			void FormatA(LPCSTR lpStr, ...);
			void FormatW(LPCWSTR lpStr, ...);
			void Format(LPCTSTR lpStr, ...);

			string GetStringA() const;
			wstring GetStringW() const;

			tstring GetString() const;

		public:
			static string UnicodeToANSI(LPCWSTR strWide);
			static wstring ANSIToUnicode(LPCSTR strMulti);
		protected:
			wstring* m_strString;
		};
#ifdef UNICODE
#define  GetString  GetStringW
#define  Format		FormatW
#else
#define GetString  GetStringA
#define  Format		FormatA
#endif // !UNICODE
	}
}
