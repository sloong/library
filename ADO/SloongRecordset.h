#pragma once
#include "IUniversal.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
// class difine                                                                                   //
////////////////////////////////////////////////////////////////////////////////////////////////////

using namespace Sloong;

class CSloongRecordset : public ISloongRecordset
{
////////////////////////////////////////////////////////////////////////////////////////////////////
// Member                                                                                         //
////////////////////////////////////////////////////////////////////////////////////////////////////
private:

	LPCTSTR          m_sErrorMessage;
	
public:
		_RecordsetPtr     m_pRst;
////////////////////////////////////////////////////////////////////////////////////////////////////
// Construction/Destruction                                                                       //
////////////////////////////////////////////////////////////////////////////////////////////////////
public:
	CSloongRecordset();
	virtual ~CSloongRecordset();
	
	virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, _COM_Outptr_ void **ppvObject);
	virtual ULONG STDMETHODCALLTYPE AddRef();
	virtual ULONG STDMETHODCALLTYPE Release();
////////////////////////////////////////////////////////////////////////////////////////////////////
// Property Get/Set Method                                                                        //
////////////////////////////////////////////////////////////////////////////////////////////////////
public:
	//Property: RecordCount
	long STDMETHODCALLTYPE GetRecordCount() const;
	
	//Property: PageCount
	long STDMETHODCALLTYPE GetPageCount() const;
	
	//Property: PageSize
	long STDMETHODCALLTYPE GetPageSize() const;
	void STDMETHODCALLTYPE SetPageSize(long pageSize);
	
	//Property: AbsolutePage
	long STDMETHODCALLTYPE GetAbsolutePage() const;
	void STDMETHODCALLTYPE SetAbsolutePage(long page);
	
	//Property: AbsolutePosition
	long STDMETHODCALLTYPE GetAbsolutePosition() const;
	void STDMETHODCALLTYPE SetAbsolutePosition(long pos);
	
	//Property: State
	ObjectStateEnum STDMETHODCALLTYPE GetState() const;
	
	//Property: CursorLocation
	void STDMETHODCALLTYPE SetCursorLocation(CursorLocationEnum CursorLocation);
	
	
////////////////////////////////////////////////////////////////////////////////////////////////////
// Core  Method                                                                                   //
//////////////////////////////////////////////////////////////////////////////////////////////////// 
	
public:
	void STDMETHODCALLTYPE Open(LPCTSTR Source, _ConnectionPtr pConn, CursorTypeEnum CursorType = adOpenStatic, LockTypeEnum LockType = adLockOptimistic, long Options = adCmdText);
	void STDMETHODCALLTYPE Open(LPCTSTR Source, ISloongConnection & ActiveConn, CursorTypeEnum CursorType = adOpenStatic, LockTypeEnum LockType = adLockOptimistic, long Options = adCmdText);
	bool STDMETHODCALLTYPE Close();
	
	
////////////////////////////////////////////////////////////////////////////////////////////////////
// Position                                                                                       //
//////////////////////////////////////////////////////////////////////////////////////////////////// 
	void STDMETHODCALLTYPE MoveNext();
	void STDMETHODCALLTYPE MovePrevious();
	void STDMETHODCALLTYPE MoveLast();
	void STDMETHODCALLTYPE MoveFirst();
	void STDMETHODCALLTYPE Move(long pos);
	bool STDMETHODCALLTYPE rstEOF();
	bool STDMETHODCALLTYPE rstBOF();
	
	
////////////////////////////////////////////////////////////////////////////////////////////////////
// Recordset update db                                                                            //
//////////////////////////////////////////////////////////////////////////////////////////////////// 
	void STDMETHODCALLTYPE AddNew();
	void STDMETHODCALLTYPE Delete(AffectEnum Option);
	void STDMETHODCALLTYPE Cancel();
	void STDMETHODCALLTYPE Update();
    
////////////////////////////////////////////////////////////////////////////////////////////////////
// GetValues form recordset                                                                       //
////////////////////////////////////////////////////////////////////////////////////////////////////
	int STDMETHODCALLTYPE GetInt(LPCTSTR columnName);
	LPCTSTR STDMETHODCALLTYPE GetString(LPCTSTR columnName);
	
	
////////////////////////////////////////////////////////////////////////////////////////////////////
// SetValues form recordset                                                                       //
//////////////////////////////////////////////////////////////////////////////////////////////////// 
	void STDMETHODCALLTYPE SetInt(LPCTSTR columnName, int value);
	void STDMETHODCALLTYPE SetString(LPCTSTR columnName, LPCTSTR value);
	
};
