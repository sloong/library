// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
// ��ı�׼�������� DLL �е������ļ��������������϶���� STRING_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
// �κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ
// STRING_API ������Ϊ�Ǵ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�
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
			CString(const CString& str);
			~CString();

			CString& operator=(LPCSTR lpStr);
			CString& operator=(LPCWSTR lpStr);
			CString& operator=(string lpStr);
			CString& operator=(wstring lpstr);
			CString& operator=(const CString& str);

			void FormatA(LPCSTR lpStr, ...);
			void FormatW(LPCWSTR lpStr, ...);

			string GetStringA() const;
			wstring GetStringW() const;

			LPCWSTR	w_str() const;
			LPCSTR	a_str() const;

#ifdef UNICODE
#define		GetString	GetStringW
#define		Format		FormatW
#define		t_str		w_str
#else
#define		GetString	GetStringA
#define		Format		FormatA
#define		t_str		a_str
#endif // !UNICODE

			bool empty() const{ return GetString().empty(); }
			
			size_t size() const { return GetString().size(); }
			bool operator!= (LPCTSTR str) const { return GetString() != str; }
			bool operator== (LPCTSTR str) const { return GetString() == str; }
			bool operator< (const CString& str) const { return GetString() < str.GetString(); }
			void clear() { m_strString->clear(); };

		public:
			static string UnicodeToANSI(LPCWSTR strWide);
			static wstring ANSIToUnicode(LPCSTR strMulti);
		protected:
			string*	m_strTemp;	// this object just used to return the ANSI string. no used to compute. 
			wstring* m_strString;
		};
	}

}
