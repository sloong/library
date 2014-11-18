#pragma once

#include "IUniversal.h"

using namespace SoaringLoong;

class CLogSystem;
class CUniversal : public IUniversal
{
public:

	CUniversal();
	~CUniversal();

	virtual HRESULT _stdcall QueryInterface(const IID& riid, void** ppvObject);
	virtual ULONG _stdcall AddRef();
	virtual ULONG _stdcall Release();
	static int Init();
	ULONG ObjNum();


	virtual LPCTSTR _stdcall HelloWorld();
	virtual LPCTSTR _stdcall Format(LPCTSTR strString, ...);
	virtual void _stdcall CopyStringToPoint(LPTSTR& lpTarget, LPCTSTR lpFrom);
	virtual HRESULT _stdcall CreateLogSystem(IUniversal* pUniversal, ILogSystem** pLog);
	virtual HRESULT _stdcall CreateLinkList(ILinkList** pLinkList);
	virtual HRESULT _stdcall CreateScriptParser(PARSETYPE emType, IScriptParser** pParser);
	virtual HRESULT STDMETHODCALLTYPE CreateADO(ISloongConnection** pConnection, ISloongRecordset** pRecordset, ISloongCommand** pCommand);
protected:
	static ULONG m_objNum;
	static CRITICAL_SECTION m_cs;
	ULONG m_Ref;
};