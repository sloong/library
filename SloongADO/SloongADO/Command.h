//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Command.h:                                                                                      +
// Author: —î Ð¡±ø @China                                                                          +
//                                                                                                 +
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#ifndef YXB_CCOMMAND_H_H
#define YXB_CCOMMAND_H_H

////////////////////////////////////////////////////////////////////////////////////////////////////
// include flies                                                                                  //      
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <afx.h>
#include "SloongADO.h"
#include "Recordset.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
using namespace SloongADO;

class SLOONGADO_API CSloongCommand
{
private:
	_CommandPtr m_pCmd;
    CString     m_sErrorMessage;

public:
	CSloongCommand();
	virtual ~CSloongCommand();


////////////////////////////////////////////////////////////////////////////////////////////////////
// Property Get/Set Method                                                                        //
////////////////////////////////////////////////////////////////////////////////////////////////////
public:
	//Property: ActiveConnection
	void SetActiveConnection(CSloongConnection &ActiveConn);
	
    //Property: CommandText
	void SetCommandText(CString strCmd);
	
	//Property: CommandTimeout
	void SetCommandTimeout(long time);
	
	//Property: CommandType
	//void SetCommandType(CommandTypeEnum CommandType);

	//Property: State
    ObjectStateEnum GetState() const;


////////////////////////////////////////////////////////////////////////////////////////////////////
// Other Method                                                                                   //
////////////////////////////////////////////////////////////////////////////////////////////////////
public:
	bool ExecuteQuery(CSloongRecordset &Rst,CommandTypeEnum CommandType=CommandType::adCmdText);
	bool ExecuteUpdate(long &AffectedRows,CSloongRecordset &Rst,CommandTypeEnum CommandType=CommandType::adCmdText);
	void Release();
	bool Cancel();
};

#endif //#ifndef YXB_COMMAND_H_H

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//