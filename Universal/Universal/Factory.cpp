#include "stdafx.h"
#include "factory.h"
#include "Universal.h"
#include "LogSystem.h"

CFactory::CFactory()
{
	m_Ref = 0;
}

CFactory::~CFactory()
{

}

HRESULT _stdcall CFactory::QueryInterface(const IID &riid, void **ppvObject)
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

ULONG _stdcall CFactory::AddRef()
{
	m_Ref++;
	return m_Ref;
}

ULONG _stdcall CFactory::Release()
{
	m_Ref--;
	if (0 == m_Ref)
	{
		delete this;
		return 0;
	}
	return m_Ref;
}

HRESULT _stdcall CFactory::CreateInstance(IUnknown *pUnkOuter, const IID &riid, void **ppvObject)//最重要的函数，这个函数创建CompTestClass对象，并返回所需接口
{
	if (NULL != pUnkOuter)
	{
		return CLASS_E_NOAGGREGATION;
	}

	HRESULT hr = E_OUTOFMEMORY;

	if (riid == IID_ILogSystem)
	{
		CLogSystem::Init();
		CLogSystem* pObj = new CLogSystem();
		if (NULL == pObj)
		{
			return hr;
		}
		hr = pObj->QueryInterface(riid, ppvObject);
		if (S_OK != hr)
		{
			delete pObj;
		}
	}
	if (riid == IID_IUniversal || riid == IID_IUnknown)
	{
		CUniversal::Init();
		CUniversal* pObj = new CUniversal();
		if (NULL == pObj)
		{
			return hr;
		}
		hr = pObj->QueryInterface(riid, ppvObject);
		if (S_OK != hr)
		{
			delete pObj;
		}
		
	}
	
	return hr;
}

HRESULT _stdcall CFactory::LockServer(BOOL fLock)
{
	return NOERROR;
}