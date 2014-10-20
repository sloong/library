// SLUniversal.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include <iostream>
#include <windows.h>
#include <winerror.h>
#include <map>
#include "SLoongError.h"
#include "factory.h"
#include "LogSystem.h"
#include "Universal.h"


using namespace std;
using namespace SoaringLoong;

typedef map<int, _tstring> MSGMAP;
MSGMAP g_MessageMap;

TCHAR g_szFormat[NUM_SWAP][MAX_STRING] = { 0 };

ULONG CUniversal::m_objNum = 0;
CRITICAL_SECTION CUniversal::m_cs;

LPCTSTR g_SoaringLoong = TEXT("Copyright (C) 2014 Soaring Loong Corp");


UNIVERSAL_API HRESULT _stdcall CreateUniversal(LPVOID* pUniversal)
{
	IUnknown* pUnknown = NULL;
	GUID CLSID_Universal;
	LRESULT hResult = CLSIDFromProgID(g_CLSID, &CLSID_Universal);
	if (S_OK != hResult)
	{
		_tprintf(TEXT("Can't find CLSID!\n"));
		return S_FALSE;
	}
	else
	{
		LPOLESTR szCLSID;
		StringFromCLSID(CLSID_Universal, &szCLSID);
		CoTaskMemFree(szCLSID);
	}

	hResult = CoCreateInstance(CLSID_Universal, NULL, CLSCTX_INPROC_SERVER, IID_IUnknown, (void **)&pUnknown);

	if (S_OK != hResult || NULL == pUnknown)
	{
		printf("Create Object Failed!\n");
		return S_FALSE;
	}

	hResult = pUnknown->QueryInterface(IID_IUniversal, pUniversal);

	return S_OK;
}

UNIVERSAL_API HRESULT _stdcall CreateLogSystem(LPVOID* pLog)
{
	IUnknown* pUnknown = NULL;
	GUID CLSID_Universal;
	LRESULT hResult = CLSIDFromProgID(g_CLSID, &CLSID_Universal);
	if (S_OK != hResult)
	{
		_tprintf(TEXT("Can't find CLSID!\n"));
		return S_FALSE;
	}
	else
	{
		LPOLESTR szCLSID;
		StringFromCLSID(CLSID_Universal, &szCLSID);
		CoTaskMemFree(szCLSID);
	}

	hResult = CoCreateInstance(CLSID_Universal, NULL, CLSCTX_INPROC_SERVER, IID_ILogSystem, (void **)&pUnknown);

	if (S_OK != hResult || NULL == pUnknown)
	{
		printf("Create Object Failed!\n");
		return S_FALSE;
	}

	hResult = pUnknown->QueryInterface(IID_ILogSystem, pLog);

	return S_OK;
}



CUniversal::CUniversal()
{
	m_Ref = 0;
	EnterCriticalSection(&m_cs);
	m_objNum++;
	LeaveCriticalSection(&m_cs);
}

CUniversal::~CUniversal()
{
	EnterCriticalSection(&m_cs);
	m_objNum--;
	LeaveCriticalSection(&m_cs);
}

HRESULT _stdcall CUniversal::QueryInterface(const IID &riid, void **ppvObject)
{
	if (IID_IUnknown == riid){
		*ppvObject = (IUnknown*)this;
		((IUnknown*)(*ppvObject))->AddRef();
	}
	else if (IID_IUniversal == riid){
		*ppvObject = (IUniversal*)this;
		((IUniversal*)(*ppvObject))->AddRef();
	}
// 	else if (IID_ILogSystem == riid)
// 	{
// 		*ppvObject = new CLogSystem();
// 	}
	else{
		*ppvObject = NULL;
		return E_NOINTERFACE;
	}
	return S_OK;
}

ULONG _stdcall CUniversal::AddRef()
{
	m_Ref++;
	return m_Ref;
}

ULONG _stdcall CUniversal::Release()
{
	m_Ref--;
	if (0 == m_Ref)
	{
		DeleteCriticalSection(&m_cs);
		delete this;
		return 0;
	}
	return m_Ref;
}

LPCTSTR _stdcall CUniversal::HelloWorld()
{
	return g_SoaringLoong;
}

int CUniversal::Init()
{
	m_objNum = 0;
	InitializeCriticalSection(&m_cs);
	return 0;
}

ULONG CUniversal::ObjNum()
{
	return m_objNum;
}



typedef struct stMessageMap
{
	int nErrorCode;
	LPCTSTR strErrorText;
} MESSAGEMAP;


MESSAGEMAP g_Map[] =
{
	S_FALSE, TXT_ERR_FALSE,
	S_ERROR_REGISTRY_CANNOT_READ, TXT_ERR_REGISTRY_CANNOT_WRITE,
	S_ERROR_REGISTRY_CANNOT_WRITE, TXT_ERR_REGISTRY_CANNOT_READ,
	S_ERROR_REGISTRY_READ_FAILED, TXT_ERR_REGISTRY_READ_FAILED,
	S_ERROR_CREATE_WINDOW_FAILED, TXT_ERR_CREATE_WINDOW_FAILED,
	S_ERROR_CREATE_ENGINE_FAILED, TXT_ERR_CREATE_ENGINE_FAILED,
	S_ERROR_ENGINE_INIT_FAILED, TXT_ERR_ENGINE_INIT_FAILED,
	S_ERROR_GAME_INIT_FAILED, TXT_ERR_GAME_INIT_FAILED,
	S_ERROR_MUTEX_EXIST, TXT_ERR_MUTEX_EXIST,
	S_ERROR_LOAD_SCRIPT_FILE, TXT_ERR_LOAD_SCRIPT_FILE,
	S_ERROR_PARAM_VALUE, TXT_ERR_PARAM_VALUE,
	S_ERROR_CREATE_DEVICE, TXT_ERR_CREATE_DEVICE,
	S_ERROR_GET_SWAP_CHAIN_BUFFER, TXT_ERR_GET_SWAP_CHAIN_BUFFER,
	S_ERROR_CREATE_RENDER_TARGET_VIEW, TXT_ERR_CREATE_RENDER_TARGET_VIEW,
	S_ERROR_OPEN_LOG_FILE, TXT_ERR_OPEN_LOG_FILE,
	S_ERROR_NO_OPEN_FILE, TXT_ERR_NO_OPEN_FILE,
	S_ERROR_CREATE_OBJECT, TXT_ERR_CREATE_OBJECT,
	S_ERROR_FIND_OBJECT, TXT_ERR_FIND_OBJECT,
	S_ERROR_CREATE_TEXUTE, TXT_ERR_CREATE_TEXUTE,
	S_WARNING_FUNCTION_DISABLE, TEXT_WARN_FUNCTION_DISABLE,
};

bool g_bIsInst = false;

//--- 0000010 --- 2013/9/30 --- WCB --- Add
// Remarks:
//		Format the error message with the error code
HRESULT FormatErrorMessage(LPTSTR szErrText, DWORD dwSize, HRESULT nErrorCode)
{
	// if error code map is not initialize, do it.
	if (false == g_bIsInst)
	{
		for (int i = 0; i < ARRAYSIZE(g_Map); i++)
		{
			g_MessageMap.insert(map<int, _tstring>::value_type(g_Map[i].nErrorCode, g_Map[i].strErrorText));
		}
		g_bIsInst = true;
	}

	// find the message with error code.
	MSGMAP::iterator Key = g_MessageMap.find((int)nErrorCode);
	if (Key == g_MessageMap.end())
	{
		_ASSERT(NULL);
		_stprintf_s(szErrText, dwSize, TEXT("Unknow error, code = %d"), nErrorCode);
		return S_OK;
	}
	else
	{
		_tcscpy_s(szErrText, dwSize / 2, Key->second.c_str());
		return S_OK;
	}
}

LPCTSTR SoaringLoong::Format(LPCTSTR strString, ...)
{
	va_list args;
	va_start(args, strString);

	LPTSTR strTarget = NULL;
	LPTSTR strNext = NULL;

	// Find a empty buffer, and set the next buffer.
	for (int i = 0; i < NUM_SWAP; i++)
	{
		if (0 == _tcsicmp(TEXT(""), g_szFormat[i]))
		{
			strTarget = g_szFormat[i];
			if ((i + 1) >= NUM_SWAP)
			{
				strNext = g_szFormat[0];
			}
			else
			{
				strNext = g_szFormat[i + 1];
			}
			break;
		}
	}

	try
	{
		// Format the target buffer and clear next buffer.
		_tvsprintf_s(strTarget, MAX_STRING, strString, args);
		_tcscpy_s(strNext, MAX_STRING, TEXT(""));
	}
	catch (...)
	{
		return NULL;
	}

	return strTarget;
}

void SoaringLoong::CopyStringToPoint(LPTSTR& lpTarget, LPCTSTR lpFrom)
{
	SAFE_DELETE_ARR(lpTarget);
	UINT nLength = (UINT)_tcslen(lpFrom);
	lpTarget = new TCHAR[nLength + 1];
	assert(lpTarget);
	_tcscpy_s(lpTarget, nLength + 1, lpFrom);
}


HRESULT CreateScriptParsing(LPVOID& pParsing, PARSETYPE emType)
{
	if (NULL == pParsing)
	{
		switch (emType)
		{
		case PARSE_CMD:
			pParsing = new CCMDParser;
			break;
		case PARSE_TOKEN:
			pParsing = new CTokenParser;
			break;
		case PARSE_XML:
			//pParsing = new CXMLParser;
			break;
		default:
			pParsing = NULL;
			break;
		}
		return S_OK;
	}
	return S_FALSE;
}


CException::CException()
{
	m_strMessage = NULL;
}

CException::~CException()
{
	SAFE_DELETE(m_strMessage);
}

void CException::SetExceptionText(LPCTSTR strMessage)
{
	SoaringLoong::CopyStringToPoint(m_strMessage, strMessage);
}

LPCTSTR CException::GetExceptionText()
{
	return m_strMessage;
}

void CException::SetExceptionCode(HRESULT lCode)
{
	m_lResult = lCode;
}

HRESULT CException::GetExceptionCode()
{
	return m_lResult;
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

inline CRect CSize::operator+(_In_ const RECT* lpRect) const throw()
{
	return CRect(lpRect) + *this;
}

inline CRect CSize::operator-(_In_ const RECT* lpRect) const throw()
{
	return CRect(lpRect) - *this;
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


XMLProc::XMLProc()

{

	//CoInitialize(NULL);

	createFile = NULL;

	readFile = NULL;

	createRoot = NULL;

	readRoot = NULL;

	pPI = NULL;

}

IXMLDOMDocumentPtr XMLProc::Create(LPCTSTR rootTag)

{

	if (rootTag == NULL)

		return 0;

	hr = createFile.CreateInstance(__uuidof(DOMDocument));

	assert(hr);

	hr = createFile->createProcessingInstruction(_bstr_t(_T("xml")), _bstr_t(_T("version=\"1.0\" encoding=\"GBK\"")), &pPI);

	//hr = createFile->createProcessingInstruction( _bstr_t(_T("xml")), _bstr_t(_T("version=\"1.0\"")), &pPI );

	assert(hr);

	hr = createFile->appendChild(pPI, NULL);

	assert(hr);

	hr = createFile->createElement(_bstr_t(rootTag), &createRoot);

	assert(hr);

	hr = createFile->appendChild(createRoot, NULL);

	assert(hr);

	return createFile;

}

IXMLDOMDocumentPtr XMLProc::Read(LPCTSTR sfile)

{

	VARIANT_BOOL dl;

	hr = readFile.CreateInstance(__uuidof(DOMDocument));

	assert(hr);

	hr = readFile->load(_variant_t(sfile), &dl);

	assert(hr);

	hr = readFile->get_documentElement(&readRoot);

	assert(hr);

	return readFile;

}

BOOL XMLProc::NewElement(LPCTSTR tag, LPCTSTR text, IXMLDOMElement** newNode)

{

	if (createFile == NULL || tag == NULL || text == NULL){

		return FALSE;

	}

	hr = createFile->createElement(_bstr_t(tag), newNode);

	if (hr != S_OK)

		return FALSE;

	(*newNode)->put_text(_bstr_t(text));



	return TRUE;

}



BOOL XMLProc::NewElement(LPCTSTR tag, IXMLDOMElement** newNode)

{

	if (createFile == NULL || tag == NULL){

		return FALSE;

	}

	hr = createFile->createElement(_bstr_t(tag), newNode);

	if (hr != S_OK)

		return FALSE;



	return TRUE;

}



BOOL XMLProc::SelectElement(LPCTSTR tag, IXMLDOMElement** node)

{

	TCHAR usetag[MAX_STRING + 2] = TEXT("//");

	if (readFile == NULL || tag == NULL || _tcslen(tag) >= MAX_STRING)

		return FALSE;

	_tcscat_s(usetag, MAX_STRING, tag);

	hr = readFile->selectSingleNode(_bstr_t(usetag), (IXMLDOMNode**)node);

	if (hr != S_OK)

		return FALSE;

	return TRUE;

}



BOOL XMLProc::GetText(IXMLDOMElement *elem, LPTSTR text, int nBufferSize, int* tlen)

{

	BSTR bst;

	_bstr_t bt;

	if (elem == NULL || text == NULL)

		return FALSE;

	hr = elem->get_text(&bst);

	if (hr != S_OK)

		return FALSE;

	else {

		bt = bst;

		*tlen = (int)_tcslen((LPCTSTR)bt) > *tlen ? *tlen : (int)_tcslen((LPCTSTR)bt);

		_tcsncpy_s(text, nBufferSize, (LPCTSTR)bt, *tlen);

		text[*tlen] = 0;

		return TRUE;

	}

}



BOOL XMLProc::ChangeText(IXMLDOMElement *elem, LPCTSTR text)

{

	if (elem == NULL)

		return FALSE;

	hr = elem->put_text(_bstr_t(text));

	if (hr != S_OK)

		return FALSE;

	else {

		return TRUE;

	}

}



BOOL XMLProc::AddChild(IXMLDOMElement* childElem, IXMLDOMElement* parentElem)

{

	if (parentElem == NULL || childElem == NULL)

		return FALSE;

	hr = parentElem->appendChild((IXMLDOMNode*)childElem, NULL);

	if (hr != S_OK)

		return FALSE;

	else

		return TRUE;

}



BOOL XMLProc::GetParent(IXMLDOMElement* childElem, IXMLDOMElement** parentElem)

{

	if (childElem == NULL)

		return FALSE;

	hr = childElem->get_parentNode((IXMLDOMNode**)parentElem);

	if (hr != S_OK)

		return FALSE;

	else

		return TRUE;

}



BOOL XMLProc::GetChild(IXMLDOMElement** childElem, IXMLDOMElement* parentElem)

{

	if (parentElem == NULL)

		return FALSE;

	hr = parentElem->get_firstChild((IXMLDOMNode**)childElem);

	if (hr != S_OK)

		return FALSE;

	else

		return TRUE;

}



BOOL XMLProc::SetAttribute(IXMLDOMElement* node, LPCTSTR attr, LPCTSTR value)

{

	if (node == NULL){

		return FALSE;

	}

	hr = node->setAttribute(_bstr_t(attr), _variant_t(value));

	if (hr != S_OK)

		return FALSE;

	else

		return TRUE;

}



BOOL XMLProc::RemoveChild(IXMLDOMElement* childElem, IXMLDOMElement* parentElem)

{

	if (parentElem == NULL || childElem == NULL)

		return FALSE;

	hr = parentElem->removeChild((IXMLDOMNode*)childElem, NULL);

	if (hr != S_OK)

		return FALSE;

	else

		return TRUE;

}



BOOL XMLProc::SaveCreateFile(LPCTSTR dfile)

{

	if (createFile == NULL || dfile == NULL)

		return FALSE;

	hr = createFile->save(_variant_t(dfile));

	if (hr != S_OK){

		return FALSE;

	}

	else

		return TRUE;

}



BOOL XMLProc::SaveReadFile(LPCTSTR dfile)

{

	if (readFile == NULL || dfile == NULL)

		return FALSE;

	hr = readFile->save(_variant_t(dfile));

	if (hr != S_OK)

		return FALSE;

	else

		return TRUE;

}



IXMLDOMDocumentPtr XMLProc::GetCreateFile()

{

	return createFile;

}



IXMLDOMDocumentPtr XMLProc::GetReadFile()

{

	return readFile;

}



IXMLDOMElement* XMLProc::GetCreateRoot()

{

	return createRoot;

}



IXMLDOMElement* XMLProc::GetReadRoot()

{

	return readRoot;

}



void XMLProc::Fail()

{

	if (readRoot)

		readRoot.Release();

	if (readFile)

		readFile.Release();

	if (createRoot)

		createRoot.Release();

	if (pPI)

		pPI.Release();

	if (createFile)

		createFile.Release();



}



void XMLProc::Exit()

{

	Fail();

	//CoUninitialize();

}