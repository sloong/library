#pragma once

#include "IUniversal.h"

using namespace SoaringLoong;

class CLogSystem;
class CUniversal : public IUniversal
{
public:

	CUniversal();
	~CUniversal();

	virtual HRESULT STDMETHODCALLTYPE QueryInterface(const IID& riid, void** ppvObject);
	virtual ULONG STDMETHODCALLTYPE AddRef();
	virtual ULONG STDMETHODCALLTYPE Release();
	static int Init();
	ULONG ObjNum();


	virtual LPCTSTR STDMETHODCALLTYPE HelloWorld();
	virtual LPCTSTR STDMETHODCALLTYPE Format(LPCTSTR strString, ...);
	virtual void STDMETHODCALLTYPE CopyStringToPoint(LPTSTR& lpTarget, LPCTSTR lpFrom);
	virtual DWORD STDMETHODCALLTYPE FormatWindowsErrorMessage(LPTSTR szErrText, DWORD dwSize, DWORD dwErrCode);
	virtual HRESULT STDMETHODCALLTYPE CreateLogSystem(IUniversal* pUniversal, ILogSystem** pLog);
	virtual HRESULT STDMETHODCALLTYPE CreateLinkList(ILinkList** pLinkList);
	virtual HRESULT STDMETHODCALLTYPE CreateScriptParser(PARSETYPE emType, IScriptParser** pParser);
protected:
	static ULONG m_objNum;
	static CRITICAL_SECTION m_cs;
	ULONG m_Ref;
};