#pragma once

#include "univ\\univ.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
using namespace Sloong;
class CSloongCommand : public ISloongCommand
{
private:
	_CommandPtr m_pCmd;
	LPCTSTR     m_sErrorMessage;

public:
	CSloongCommand();
	virtual ~CSloongCommand();

	virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, _COM_Outptr_ void **ppvObject);
	virtual ULONG STDMETHODCALLTYPE AddRef();
	virtual ULONG STDMETHODCALLTYPE Release();
////////////////////////////////////////////////////////////////////////////////////////////////////
// Property Get/Set Method                                                                        //
////////////////////////////////////////////////////////////////////////////////////////////////////
public:
	//Property: ActiveConnection
	void STDMETHODCALLTYPE SetActiveConnection(ISloongConnection &ActiveConn);
	
	//Property: CommandText
	void STDMETHODCALLTYPE SetCommandText(LPCTSTR strCmd);
	
	//Property: CommandTimeout
	void STDMETHODCALLTYPE SetCommandTimeout(long time);
	
	//Property: CommandType
	//void SetCommandType(CommandTypeEnum CommandType);

	//Property: State
	ObjectStateEnum STDMETHODCALLTYPE GetState() const;


////////////////////////////////////////////////////////////////////////////////////////////////////
// Other Method                                                                                   //
////////////////////////////////////////////////////////////////////////////////////////////////////
public:
	bool STDMETHODCALLTYPE ExecuteQuery(ISloongRecordset &Rst, CommandTypeEnum CommandType = adCmdText);
	bool STDMETHODCALLTYPE ExecuteUpdate(long &AffectedRows, ISloongRecordset &Rst, CommandTypeEnum CommandType = adCmdText);
	bool STDMETHODCALLTYPE Cancel();

protected:
	ULONG m_Ref;
};