#pragma once

#include "IUniversal.h"

using namespace SoaringLoong;

class UniversalClass :
	public IUniversal
{
public:

	UniversalClass();
	~UniversalClass();

	virtual HRESULT _stdcall QueryInterface(const IID& riid, void** ppvObject);
	virtual ULONG _stdcall AddRef();
	virtual ULONG _stdcall Release();
	static int Init();
	ULONG ObjNum();


	virtual LPCTSTR _stdcall HelloWorld();
protected:
	static ULONG m_objNum;
	static CRITICAL_SECTION m_cs;
	ULONG m_Ref;
};