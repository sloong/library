#pragma once

#include <unknwn.h>

class UniversalFactory :
	public IClassFactory
{
public:
	UniversalFactory();
	~UniversalFactory();

	//要实现IUnknown接口
	virtual HRESULT _stdcall QueryInterface(const IID& riid, void** ppvObject);
	virtual ULONG _stdcall AddRef();
	virtual ULONG _stdcall Release();

	//要实现IClassFactory接口
	virtual HRESULT _stdcall CreateInstance(IUnknown *pUnkOuter, const IID& riid, void **ppvObject);
	virtual HRESULT _stdcall LockServer(BOOL fLock);

protected:
	ULONG m_Ref;
};