#pragma once

#include "IUniversal.h"
using namespace SoaringLoong;

class CSloongConnection : public ISloongConnection
{
private:
	_ConnectionPtr  m_pConn;
	LPCTSTR         m_sErrorMessage;  //used to save error message


	////////////////////////////////////////////////////////////////////////////////////////////////////
	//Construction/Destruction                                                                        //
	////////////////////////////////////////////////////////////////////////////////////////////////////
public:
	CSloongConnection();
	~CSloongConnection();

	virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, _COM_Outptr_ void **ppvObject);
	virtual ULONG STDMETHODCALLTYPE AddRef();
	virtual ULONG STDMETHODCALLTYPE Release();
	////////////////////////////////////////////////////////////////////////////////////////////////////
	// Property Get/Set Method                                                                        //
	////////////////////////////////////////////////////////////////////////////////////////////////////
public:
	//Property: ConnectionString
	LPCTSTR STDMETHODCALLTYPE GetConnectionString() const;
	void STDMETHODCALLTYPE SetConnectionString(LPCTSTR charConn);

	//Property: ConnectionTimeout
	long STDMETHODCALLTYPE GetConnectionTimeout() const;
	void STDMETHODCALLTYPE SetConnectionTimeout(long time);

	//Property: CursorLocation
	CursorLocationEnum STDMETHODCALLTYPE GetCursorLocation() const;
	void STDMETHODCALLTYPE SetCursorLocation(CursorLocationEnum CursorLocation);

	//Property: CommandTimeout
	long STDMETHODCALLTYPE GetCommandTimeout() const;
	void STDMETHODCALLTYPE SetCommandTimeout(long time);


	//Property: State
	ObjectStateEnum STDMETHODCALLTYPE GetState() const;

	//Property: m_sErrorMessage
	LPCTSTR STDMETHODCALLTYPE GetErrorMessage() const;

	//Property: m_pConn
	_ConnectionPtr STDMETHODCALLTYPE GetConnection() const;


	////////////////////////////////////////////////////////////////////////////////////////////////////
	// Core Method                                                                                    //
	////////////////////////////////////////////////////////////////////////////////////////////////////    
public:
	bool STDMETHODCALLTYPE Open(LPCTSTR ConnectionString, LPCTSTR UserID = _T(""), LPCTSTR Password = _T(""), long ConnectOption = adConnectUnspecified);//ID和Password为空，因为要再连接字符串中去指明ID和密码
	bool STDMETHODCALLTYPE Close();
	bool STDMETHODCALLTYPE Cancel();
	bool STDMETHODCALLTYPE RollbackTrans();
	bool STDMETHODCALLTYPE CommitTrans();
	long STDMETHODCALLTYPE BeginTrans();
};
