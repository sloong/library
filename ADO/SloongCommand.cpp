#include "stdafx.h"
#include "SloongCommand.h"
using namespace Sloong;


////////////////////////////////////////////////////////////////////////////////////////////////////
// Construction/Destruction                                                                       //
////////////////////////////////////////////////////////////////////////////////////////////////////

CSloongCommand::CSloongCommand()
{
   CoInitialize(NULL);  
   m_pCmd.CreateInstance("ADODB.Command");
   //m_pCmd.CreateInstance(__uuidof(Command));
}

CSloongCommand::~CSloongCommand()
{
	Release();
}


////////////////////////////////////////////////////////////////////////////////////////////////////
// Property Get/Set Method                                                                        //
////////////////////////////////////////////////////////////////////////////////////////////////////

//Property: ActiveConnection
void CSloongCommand::SetActiveConnection(ISloongConnection &ActiveConn)
{
	m_pCmd->PutActiveConnection(ActiveConn.GetConnection().GetInterfacePtr());
}

//Property: CommandText
void CSloongCommand::SetCommandText(LPCTSTR strCmd)
{
	m_pCmd->PutCommandText(_bstr_t(strCmd));
}

//Property: CommandTimeout
void CSloongCommand::SetCommandTimeout(long time)
{
	m_pCmd->PutCommandTimeout(time);
}

//Property: State
ObjectStateEnum CSloongCommand::GetState() const
{
	return (ObjectStateEnum)(m_pCmd->GetState());
}


////////////////////////////////////////////////////////////////////////////////////////////////////
// Core Method                                                                                    //
////////////////////////////////////////////////////////////////////////////////////////////////////

//ExecuteQuery
bool CSloongCommand::ExecuteQuery(ISloongRecordset &Rst,CommandTypeEnum CommandType)
{
	_RecordsetPtr res=m_pCmd->Execute(NULL,NULL,CommandType);
	Rst.SetRecordsetPtr(res);
	return true;	
}

//ExecuteUpdate
bool CSloongCommand::ExecuteUpdate(long &AffectedRows,ISloongRecordset &Rst,CommandTypeEnum CommandType)
{	
	try
	{
		VARIANT rows;
		rows.vt = VT_I4;
		_RecordsetPtr res = m_pCmd->Execute(&rows, NULL, CommandType);
		Rst.SetRecordsetPtr(res);
		AffectedRows = rows.lVal;

	}
	catch (_com_error e)
	{
		m_sErrorMessage = e.Description();
	}
	return true;	
}

//Release
// void CSloongCommand::Release()
// {
// 	if(m_pCmd!=NULL)
// 	{
// 		try
// 		{
// 			m_pCmd.Release();
// 			m_pCmd=NULL;
// 		}
// 		catch(_com_error e)
// 		{
//             throw e;
// 		}
// 	}
// }

//Cancel
bool CSloongCommand::Cancel()
{
	if(m_pCmd==NULL)
	{
		//m_sErrorMessage
		m_sErrorMessage=_T("Command is not available!");
		return false;
	}
	
	try
	{
		HRESULT hRsut=m_pCmd->Cancel();
		if(SUCCEEDED(hRsut))
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
	catch(_com_error e)
	{
		//m_sErrorMessage
		m_sErrorMessage = e.Description();
		return false;	
	}
}

HRESULT STDMETHODCALLTYPE CSloongCommand::QueryInterface(REFIID riid, _COM_Outptr_ void **ppvObject)
{
	return S_OK;
}

ULONG STDMETHODCALLTYPE CSloongCommand::AddRef()
{
	return 0;
}

ULONG STDMETHODCALLTYPE CSloongCommand::Release()
{
	return 0;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//