#pragma once

#ifdef SLOONGADO_EXPORTS
#define SLOONGADO_API __declspec(dllexport)
#else
#define SLOONGADO_API __declspec(dllimport)
#endif

#import "C:\Program Files\Common Files\System\ado\msado15.dll" no_namespace rename("EOF","rstEOF") rename("BOF","rstBOF")

namespace SloongADO
{
	namespace CursorLocation
	{
		CursorLocationEnum const adUseNone = CursorLocationEnum(1);             //
		CursorLocationEnum const adUseServer = CursorLocationEnum(2);           //
		CursorLocationEnum const adUseClient = CursorLocationEnum(3);           //
		CursorLocationEnum const adUseClientBatch = CursorLocationEnum(3);      //
	}

	namespace CursorType
	{
		CursorTypeEnum const adOpenUnspecified = CursorTypeEnum(-1);            //
		CursorTypeEnum const adOpenForwardOnly = CursorTypeEnum(0);             //
		CursorTypeEnum const adOpenKeyset = CursorTypeEnum(1);                  //
		CursorTypeEnum const adOpenDynamic = CursorTypeEnum(2);                 //
		CursorTypeEnum const adOpenStatic = CursorTypeEnum(3);                  //
	}

	namespace LockType
	{
		LockTypeEnum const adLockUnspecified = LockTypeEnum(-1);                //
		LockTypeEnum const adLockReadOnly = LockTypeEnum(1);                    //
		LockTypeEnum const adLockPessimistic = LockTypeEnum(2);                 //
		LockTypeEnum const adLockOptimistic = LockTypeEnum(3);                  //
		LockTypeEnum const adLockBatchOptimistic = LockTypeEnum(4);             //
	}

	namespace ObjectState
	{
		ObjectStateEnum const adStateClosed = ObjectStateEnum(0);               //
		ObjectStateEnum const adStateOpen = ObjectStateEnum(1);                 //
		ObjectStateEnum const adStateConnecting = ObjectStateEnum(2);           //
		ObjectStateEnum const adStateExecuting = ObjectStateEnum(4);            //
		ObjectStateEnum const adStateFetching = ObjectStateEnum(8);             //
	}


	namespace ConnectOption
	{
		ConnectOptionEnum const adConnectUnspecified = ConnectOptionEnum(-1);   //
		ConnectOptionEnum const adAsyncConnect = ConnectOptionEnum(16);         //
	}

	namespace CommandType
	{
		CommandTypeEnum const adCmdUnspecified = CommandTypeEnum(-1);           //
		CommandTypeEnum const adCmdText = CommandTypeEnum(1);                   //
		CommandTypeEnum const adCmdTable = CommandTypeEnum(2);                  //
		CommandTypeEnum const adCmdStoredProc = CommandTypeEnum(4);             //
		CommandTypeEnum const adCmdUnknown = CommandTypeEnum(8);                //
		CommandTypeEnum const adCmdFile = CommandTypeEnum(256);                 //
		CommandTypeEnum const adCmdTableDirect = CommandTypeEnum(512);          //
	}

	namespace ExecuteOption
	{
		ExecuteOptionEnum const adOptionUnspecified = ExecuteOptionEnum(-1);       //
		ExecuteOptionEnum const adAsyncExecute = ExecuteOptionEnum(16);            //
		ExecuteOptionEnum const adAsyncFetch = ExecuteOptionEnum(32);              //
		ExecuteOptionEnum const adAsyncFetchNonBlocking = ExecuteOptionEnum(64);   //
		ExecuteOptionEnum const adExecuteNoRecords = ExecuteOptionEnum(128);       //
		ExecuteOptionEnum const adExecuteStream = ExecuteOptionEnum(1024);         //
		ExecuteOptionEnum const adExecuteRecord = ExecuteOptionEnum(2048);         //
	}



	namespace Affect
	{
		AffectEnum const adAffectCurrent = AffectEnum(1);
		AffectEnum const adAffectGroup = AffectEnum(2);
		AffectEnum const adAffectAll = AffectEnum(3);
		AffectEnum const adAffectAllChapters = AffectEnum(4);
	}



	class SLOONGADO_API CSloongConnection
	{
	private:
		_ConnectionPtr  m_pConn;
		CString         m_sErrorMessage;  //used to save error message


		////////////////////////////////////////////////////////////////////////////////////////////////////
		//Construction/Destruction                                                                        //
		////////////////////////////////////////////////////////////////////////////////////////////////////
	public:
		CSloongConnection();
		~CSloongConnection();

		////////////////////////////////////////////////////////////////////////////////////////////////////
		// Property Get/Set Method                                                                        //
		////////////////////////////////////////////////////////////////////////////////////////////////////
	public:
		//Property: ConnectionString
		CString GetConnectionString() const;
		void SetConnectionString(char * charConn);
		void SetConnectionString(CString strConn);

		//Property: ConnectionTimeout
		long GetConnectionTimeout() const;
		void SetConnectionTimeout(long time);

		//Property: CursorLocation
		CursorLocationEnum GetCursorLocation() const;
		void SetCursorLocation(CursorLocationEnum CursorLocation);

		//Property: CommandTimeout
		long GetCommandTimeout() const;
		void SetCommandTimeout(long time);


		//Property: State
		ObjectStateEnum GetState() const;

		//Property: m_sErrorMessage
		CString GetErrorMessage() const;

		//Property: m_pConn
		_ConnectionPtr GetConnection() const;


		////////////////////////////////////////////////////////////////////////////////////////////////////
		// Core Method                                                                                    //
		////////////////////////////////////////////////////////////////////////////////////////////////////    
	public:
		bool Open(CString ConnectionString, CString UserID = _T(""), CString Password = _T(""), ConnectOptionEnum ConnectOption = ConnectOption::adConnectUnspecified);//ID和Password为空，因为要再连接字符串中去指明ID和密码
		bool Close();
		bool Cancel();
		void Release();
		bool RollbackTrans();
		bool CommitTrans();
		long BeginTrans();
	};
}
