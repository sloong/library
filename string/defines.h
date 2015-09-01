// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
// ��ı�׼�������� DLL �е������ļ��������������϶���� STRING_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
// �κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ
// STRING_API ������Ϊ�Ǵ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�

#ifndef DEFINES_H
#define DEFINES_H

// close warning
#pragma warning(disable:4996)


#ifndef _WINDOWS


#ifndef 	LPCWSTR
#define		LPCWSTR		const wchar_t*
#endif
#ifndef 	LPWSTR
#define		LPWSTR		wchar_t*
#endif
#ifndef 	LPCSTR
#define		LPCSTR		const char*
#endif
#ifndef 	LPSTR
#define		LPSTR		char*
#endif
#ifndef 	TRUE
#define		TRUE		1
#endif
#ifndef 	FALSE
#define		FALSE		0
#endif
#ifndef		WCHAR
#define 	WCHAR		wchar_t
#endif
#ifndef		CHAR
#define 	CHAR		char
#endif

#ifndef		DWORD
#define		DWORD		unsigned long
#endif

#ifndef LPVOID
#define LPVOID	void*
#endif // !LPVOID

#ifndef HRESULT
typedef long HRESULT
#endif // !HRESULT

#ifndef LONG
typedef long                LONG;
#endif // !LONG

#ifndef SHORT
typedef short               SHORT;
#endif // !SHORT


#ifndef tagRECT
typedef struct tagRECT
{
	LONG    left;
	LONG    top;
	LONG    right;
	LONG    bottom;
} RECT, *PRECT;
#endif // !tagRECT

#ifndef tagPOINT
typedef struct tagPOINT
{
	LONG  x;
	LONG  y;
} POINT, *PPOINT;
#endif // !tagPOINT

#ifndef tagSIZE
typedef struct tagSIZE
{
	LONG        cx;
	LONG        cy;
} SIZE, *PSIZE, *LPSIZE;
#endif // !tagSIZE

#ifndef tagPOINTS
typedef struct tagPOINTS
{
	SHORT   x;
	SHORT   y;
} POINTS, *PPOINTS, *LPPOINTS;
#endif // !tagPOINTS

#ifndef S_OK
#define S_OK                            ((HRESULT)0L)
#endif // !S_OK

#ifndef S_FALSE
#define S_FALSE                                ((HRESULT)1L)
#endif // !S_FALSE


#ifndef S_FAILED
#define S_FAILED						((HRESULT)-1L)
#endif // !S_FAILED

#define SUCCEEDED(hr)   (((HRESULT)(hr)) >= 0)
#define FAILED(hr)      (((HRESULT)(hr)) < 0)

#endif // !_WINDOWS


#ifdef UNICODE
	#define		tstring		wstring
	#ifndef 	LPCTSTR
		#define		LPCTSTR		LPCWSTR
	#endif
	#ifndef 	LPTSTR
		#define		LPTSTR		LPWSTR
	#endif
	#ifndef		TCHAR
		#define		TCHAR		WCHAR
	#endif
#else
	#define		tstring		string
	#ifndef 	LPCTSTR
		#define		LPCTSTR		LPCSTR
	#endif
	#ifndef 	LPTSTR
		#define		LPTSTR		LPSTR
	#endif
	#ifndef		TCHAR
		#define		TCHAR		CHAR
	#endif
#endif // !UNICODE



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
#define _tcslen			wcslen
#define UNSIGNED
#define __STR2WSTR(str) L##str
#define _STR2WSTR(str) __STR2WSTR(str)
#ifndef __FUNCTIONW__
#define __FUNCTIONW__ _STR2WSTR(__FUNCTION__)
#endif
#define __TFILE__		__FILEW__
#define __TFUNCTION__	__FUNCTIONW__
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
#define stscanf sscanf
#define stscanf_s swscanf_s
#define fgetts fgets
#endif //_UNICODE
#pragma endregion



#endif //DEFINES_H
