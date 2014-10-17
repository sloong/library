#include "stdafx.h"
#include "factory.h"
#include "UniversalClass.h"

UniversalFactory::UniversalFactory()
{
	m_Ref = 0;
}

UniversalFactory::~UniversalFactory()
{

}

HRESULT _stdcall UniversalFactory::QueryInterface(const IID &riid, void **ppvObject)
{
	if (IID_IUnknown == riid){
		*ppvObject = (IUnknown*)this;
		((IUnknown*)(*ppvObject))->AddRef();
	}
	else if (IID_IClassFactory == riid){
		*ppvObject = (IClassFactory*)this;
		((IClassFactory*)(*ppvObject))->AddRef();
	}
	else{
		*ppvObject = NULL;
		return E_NOINTERFACE;
	}
	return S_OK;
}

ULONG _stdcall UniversalFactory::AddRef()
{
	m_Ref++;
	return m_Ref;
}

ULONG _stdcall UniversalFactory::Release()
{
	m_Ref--;
	if (0 == m_Ref)
	{
		delete this;
		return 0;
	}
	return m_Ref;
}

HRESULT _stdcall UniversalFactory::CreateInstance(IUnknown *pUnkOuter, const IID &riid, void **ppvObject)//最重要的函数，这个函数创建CompTestClass对象，并返回所需接口
{
	if (NULL != pUnkOuter){
		return CLASS_E_NOAGGREGATION;
	}
	HRESULT hr = E_OUTOFMEMORY;
	UniversalClass::Init();
	UniversalClass* pObj = new UniversalClass();
	if (NULL == pObj){
		return hr;
	}

	hr = pObj->QueryInterface(riid, ppvObject);
	if (S_OK != hr){
		delete pObj;
	}
	return hr;
}

HRESULT _stdcall UniversalFactory::LockServer(BOOL fLock)
{
	return NOERROR;
}