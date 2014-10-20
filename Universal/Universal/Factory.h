#pragma once

#include <unknwn.h>

class CFactory : public IClassFactory
{
public:
	CFactory();
	~CFactory();

	//Ҫʵ��IUnknown�ӿ�
	virtual HRESULT _stdcall QueryInterface(const IID& riid, void** ppvObject);
	virtual ULONG _stdcall AddRef();
	virtual ULONG _stdcall Release();

	//Ҫʵ��IClassFactory�ӿ�
	virtual HRESULT _stdcall CreateInstance(IUnknown *pUnkOuter, const IID& riid, void **ppvObject);
	virtual HRESULT _stdcall LockServer(BOOL fLock);

protected:
	ULONG m_Ref;
};