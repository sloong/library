// 下列 ifdef 块是创建使从 DLL 导出更简单的
// 宏的标准方法。此 DLL 中的所有文件都是用命令行上定义的 STRING_EXPORTS
// 符号编译的。在使用此 DLL 的
// 任何其他项目上不应定义此符号。这样，源文件中包含此文件的任何其他项目都会将
// STRING_API 函数视为是从 DLL 导入的，而此 DLL 则将用此宏定义的
// 符号视为是被导出的。
#ifdef STRING_EXPORTS
#define STRING_API __declspec(dllexport)
#else
#define STRING_API __declspec(dllimport)
#endif
#pragma once

#ifdef UNICODE
#define		tstring		wstring
#else
#defint		tstring		string
#endif // !UNICODE

#pragma warning( disable:4251 )

#include <string>
using std::string;
using std::wstring;
namespace Sloong
{
	namespace Universal
	{
		class STRING_API CString
		{
		public:
			CString();
			CString(LPCSTR lpStr, ...);
			CString(LPCWSTR lpStr, ...);
			CString(string lpStr);
			CString(wstring lpStr);
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

#ifdef UNICODE
#define		GetString	GetStringW
#define		Format		FormatW
#else
#define		GetString	GetStringA
#define		Format		FormatA
#endif // !UNICODE

			bool empty() const{ return GetString().empty(); }
			LPCTSTR c_str() const { return GetString().c_str(); }
			size_t size() const { return GetString().size(); }
			bool operator!= (LPCTSTR str) const { return GetString() != str; }
			bool operator== (LPCTSTR str) const { return GetString() == str; }
			bool operator< (const CString& str) const { return GetString() < str.GetString(); }
			void clear() { m_strString->clear(); };

		public:
			static string UnicodeToANSI(LPCWSTR strWide);
			static wstring ANSIToUnicode(LPCSTR strMulti);
		protected:
			wstring* m_strString;
		};
	}

}
