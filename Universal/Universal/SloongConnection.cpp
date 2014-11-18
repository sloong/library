#include "stdafx.h"
#include "SloongConnection.h"

// The one and only application object
using namespace SoaringLoong;
////////////////////////////////////////////////////////////////////////////////////////////////////
// Construction/Destruction                                                                       //
////////////////////////////////////////////////////////////////////////////////////////////////////

CSloongConnection::CSloongConnection()
{
	CoInitialize(NULL);
	m_pConn.CreateInstance("ADODB.Connection");
	//m_pConn.CreateInstance(__uuidof(Connection));

	//default set
	m_pConn->PutCursorLocation(adUseClient);

}

CSloongConnection::~CSloongConnection()
{
	Release();
	CoUninitialize();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// Property Get/Set Method                                                                        //
////////////////////////////////////////////////////////////////////////////////////////////////////

//Property: ConnectionString  
LPCTSTR CSloongConnection::GetConnectionString() const
{
	return (TCHAR *)(m_pConn->GetConnectionString());
}


void CSloongConnection::SetConnectionString(LPCTSTR charConn)
{
	m_pConn->PutConnectionString(_bstr_t(charConn));
}


//Property: ConnectionTimeout  
long CSloongConnection::GetConnectionTimeout() const
{
	return m_pConn->GetConnectionTimeout();
}

void CSloongConnection::SetConnectionTimeout(long time)
{
	m_pConn->PutConnectionTimeout(time);
}

//Property: CursorLocation  
CursorLocationEnum CSloongConnection::GetCursorLocation() const
{
	return m_pConn->GetCursorLocation();
}

void CSloongConnection::SetCursorLocation(CursorLocationEnum CursorLocation)
{
	m_pConn->PutCursorLocation(CursorLocation);
}

//Property: CommandTimeout  
long CSloongConnection::GetCommandTimeout() const
{
	return m_pConn->GetCommandTimeout();
}

void CSloongConnection::SetCommandTimeout(long time)
{
	m_pConn->PutCommandTimeout(time);
}

//Property: State  
ObjectStateEnum CSloongConnection::GetState() const
{
	return (ObjectStateEnum)(m_pConn->GetState());
}


//Property: m_sErrorMessage
LPCTSTR CSloongConnection::GetErrorMessage() const
{
	return m_sErrorMessage;
}

//Property: m_pConn
_ConnectionPtr CSloongConnection::GetConnection() const
{
	return m_pConn;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// Core Method                                                                                    //
////////////////////////////////////////////////////////////////////////////////////////////////////

//Open 
bool CSloongConnection::Open(LPCTSTR ConnectionString, LPCTSTR UserID, LPCTSTR Password, long ConnectOption)
{
	try
	{
		if (m_pConn == NULL)
		{
			m_pConn.CreateInstance("ADODB.Connection");
		}

		HRESULT hRsut = m_pConn->Open((_bstr_t)ConnectionString, (_bstr_t)UserID, (_bstr_t)Password, (long)ConnectOption);

		if (SUCCEEDED(hRsut))
		{
			return true;
		}
		else
		{
			m_sErrorMessage = _T("There is an error when open the connection!");
			return false;
		}
	}
	catch (_com_error e)
	{
		m_sErrorMessage = e.Description();
		return false;
	}
}


//Close
bool CSloongConnection::Close()
{
	if (m_pConn == NULL)
	{
		m_sErrorMessage = _T("Connectin is not available!");
		return true;
	}

	try
	{
		ObjectStateEnum state = (ObjectStateEnum)(m_pConn->GetState());

		if (state == adStateOpen)
		{
			HRESULT hRsut = m_pConn->Close();
			if (SUCCEEDED(hRsut))
			{
				return true;
			}
			else
			{
				//m_sErrorMessage
				m_sErrorMessage = _T("Failed to close the connection!");
				return false;
			}

		}

		return true;
	}
	catch (_com_error e)
	{
		//m_sErrorMessage
		m_sErrorMessage = _T("There is an error when called then function: Close()!");
		return false;
	}
}


//Cancel
bool CSloongConnection::Cancel()
{
	if (m_pConn == NULL)
	{
		//m_sErrorMessage
		m_sErrorMessage = _T("Connectin is not available!");
		return false;
	}

	try
	{
		HRESULT hRsut = m_pConn->Cancel();
		if (SUCCEEDED(hRsut))
		{
			return true;
		}
		else
		{
			//m_sErrorMessage
			m_sErrorMessage = _T("Failed to cancel!");
			return false;
		}
	}
	catch (_com_error e)
	{
		//m_sErrorMessage
		m_sErrorMessage = _T("There is an error when called then function: Cancel()!");
		return false;
	}
}

// 
// //Release
// void CSloongConnection::Release()
// {
// 	if (m_pConn != NULL)
// 	{
// 		try
// 		{
// 			Close();
// 			m_pConn.Release();
// 			m_pConn = NULL;
// 		}
// 		catch (_com_error e)
// 		{
// 			throw e;
// 		}
// 	}
// }


//BeginTrans
long CSloongConnection::BeginTrans()
{
	return m_pConn->BeginTrans();
}


//CommitTrans
bool  CSloongConnection::CommitTrans()
{
	if (m_pConn == NULL)
	{
		//m_sErrorMessage
		m_sErrorMessage = _T("Connectin is not available!");
		return false;
	}

	try
	{
		HRESULT hRsut = m_pConn->CommitTrans();
		if (SUCCEEDED(hRsut))
		{
			return true;
		}
		else
		{
			//m_sErrorMessage
			m_sErrorMessage = _T("Failed to commit the tranction!");
			return false;
		}
	}
	catch (_com_error e)
	{
		m_sErrorMessage = e.Description();
		return false;
	}
}


//RollbackTrans
bool CSloongConnection::RollbackTrans()
{
	if (m_pConn == NULL)
	{
		//m_sErrorMessage
		m_sErrorMessage = _T("Connectin is not available!");
		return false;
	}

	try
	{
		HRESULT hRsut = m_pConn->RollbackTrans();
		if (SUCCEEDED(hRsut))
		{
			return true;
		}
		else
		{
			//m_sErrorMessage
			m_sErrorMessage = _T("Failed to rollback the tranction!");
			return false;
		}
	}
	catch (_com_error e)
	{
		//m_sErrorMessage
		m_sErrorMessage = e.Description();
		return false;
	}

}

HRESULT STDMETHODCALLTYPE CSloongConnection::QueryInterface(REFIID riid, _COM_Outptr_ void **ppvObject)
{
	return S_OK;
}

ULONG STDMETHODCALLTYPE CSloongConnection::AddRef()
{
	return 0;
}

ULONG STDMETHODCALLTYPE CSloongConnection::Release()
{
	return 0;
}
