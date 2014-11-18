#include            "stdafx.h"
#include            "Recordset.h"
using namespace     SloongADO;


////////////////////////////////////////////////////////////////////////////////////////////////////
// Construction/Destruction                                                                       //
//////////////////////////////////////////////////////////////////////////////////////////////////// 

CSloongRecordset::CSloongRecordset()
{
	CoInitialize(NULL);  
	m_pRst.CreateInstance("ADODB.Recordset");
	//m_pRst.CreateInstance(__uuidof(Recordset));

	//default set

}

CSloongRecordset::~CSloongRecordset()
{
	Release();
}



////////////////////////////////////////////////////////////////////////////////////////////////////
// Property Get/Set Method                                                                        //
//////////////////////////////////////////////////////////////////////////////////////////////////// 

//Property: RecordCount
long CSloongRecordset::GetRecordCount() const
{
	return m_pRst->GetRecordCount();
}

//Property: PageCount
long CSloongRecordset::GetPageCount() const
{
	return m_pRst->GetPageCount();
}

//Property: PageSize
long CSloongRecordset::GetPageSize() const
{
	return m_pRst->GetPageSize();
}

void CSloongRecordset::SetPageSize(long pageSize)
{
	m_pRst->PutPageSize(pageSize);
}

//Property: AbsolutePage
long CSloongRecordset::GetAbsolutePage() const
{
	return m_pRst->GetAbsolutePage();
}

void CSloongRecordset::SetAbsolutePage(long page)
{
	m_pRst->PutAbsolutePage((PositionEnum)page);	
}

//Property: AbsolutePosition
long CSloongRecordset::GetAbsolutePosition() const
{
	return m_pRst->GetAbsolutePosition();
}

void CSloongRecordset::SetAbsolutePosition(long pos)
{
	m_pRst->PutAbsolutePosition(PositionEnum(pos));
}

//Property: State
ObjectStateEnum CSloongRecordset::GetState() const
{
	return (ObjectStateEnum)m_pRst->GetState();
}

//Property: CursorLocation
void CSloongRecordset::SetCursorLocation(CursorLocationEnum CursorLocation)
{
	m_pRst->PutCursorLocation(CursorLocation);
}



////////////////////////////////////////////////////////////////////////////////////////////////////
// Core Method                                                                                    //
//////////////////////////////////////////////////////////////////////////////////////////////////// 

//Open
void CSloongRecordset::Open(CString Source,CSloongConnection & ActiveConn,CursorTypeEnum CursorType,LockTypeEnum LockType,long Options)
{
    m_pRst->Open(_variant_t(Source),ActiveConn.GetConnection().GetInterfacePtr(),CursorType,LockType,Options);
}


void CSloongRecordset::Open(CString Source,_ConnectionPtr pConn,CursorTypeEnum CursorType, LockTypeEnum LockType,long Options)
{
	m_pRst->Open(_variant_t(Source),pConn.GetInterfacePtr(),CursorType,LockType,Options);	
}


//Close
bool CSloongRecordset::Close() 
{
	if(m_pRst==NULL)
	{
        m_sErrorMessage="Recordset is not available!";
		return true;
	}
	
	try
	{
		ObjectStateEnum state=(ObjectStateEnum)(m_pRst->GetState());
		
		if(state==ObjectState::adStateOpen)
		{
			HRESULT hRsut=m_pRst->Close();
			if(SUCCEEDED(hRsut))
			{
				return true;
			}
			else
			{
				m_sErrorMessage="Failed to close the Recordset!";
				return false;
			}
			
		}
		
		return true;
	}
	catch(_com_error e)
	{
		m_sErrorMessage="There is an error when called then function: Close()!";
		return false;	
	}
}

//Release
void CSloongRecordset::Release()
{
	if(m_pRst!=NULL)
	{
		try
		{
			Close();
			m_pRst.Release();
			m_pRst=NULL;
		}
		catch(_com_error e)
		{
            throw e;
		}
	}
}



//rstBOF
bool CSloongRecordset::rstBOF()
{
	return (m_pRst->GetrstBOF()==VARIANT_TRUE?true:false);
}

//rstEOF
bool CSloongRecordset::rstEOF()
{
	return (m_pRst->GetrstEOF()==VARIANT_TRUE?true:false);
}

//MoveFirst: Move the cursor to the first record.
void CSloongRecordset::MoveFirst()  
{
	m_pRst->MoveFirst();
}

//MoveLast: Move the cursor to the last record.
void CSloongRecordset::MoveLast()
{
	m_pRst->MoveLast();
}

//MovePrevious
void CSloongRecordset::MovePrevious()
{
	if(m_pRst->GetrstBOF()!=VARIANT_TRUE)
	{
		m_pRst->MovePrevious();
	}
}

//MoveNext
void CSloongRecordset::MoveNext()
{
	if(m_pRst->GetrstEOF()!=VARIANT_TRUE)
	{
		m_pRst->MoveNext();
	}
}

//MoveNext
void CSloongRecordset::Move(long pos)
{
	if(m_pRst->GetrstEOF()!=VARIANT_TRUE)
	{
		m_pRst->Move(pos);
	}
}


////////////////////////////////////////////////////////////////////////////////////////////////////
// Recordset update db                                                                            //
////////////////////////////////////////////////////////////////////////////////////////////////////

//AddNew
void CSloongRecordset::AddNew()
{
	m_pRst->AddNew();
}

//Delete
void CSloongRecordset::Delete(AffectEnum Option)
{
	m_pRst->Delete(Option);
}

//Update
void CSloongRecordset::Update()
{
	m_pRst->Update();
}

//Cancel
void CSloongRecordset::Cancel()
{
	m_pRst->Cancel();
}


////////////////////////////////////////////////////////////////////////////////////////////////////
// Method GetValues                                                                               //
//////////////////////////////////////////////////////////////////////////////////////////////////// 

//GetString
CString CSloongRecordset::GetString(CString columnName)
{
	try
	{
		return (TCHAR*)(_bstr_t)m_pRst->GetCollect(_variant_t(columnName));
	}
	catch(_com_error e)
	{
		throw e;	
	}
}

//GetInt
int CSloongRecordset::GetInt(CString columnName)
{
	try
	{
		return (short)m_pRst->GetCollect(_variant_t(columnName));
	}
	catch(_com_error e)
	{
		throw e;	
	}
}