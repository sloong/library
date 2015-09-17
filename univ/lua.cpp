#include "stdafx.h"
#include "lua.h"
#include "exception.h"
using namespace Sloong::Universal;

typedef int(*LuaFunc)(lua_State* pLuaState);

#define LOCK_GUARD(m) {lock_guard<mutex> lck(m);}

CLua::CLua()
{
	m_pErrorHandler = NULL;

	m_pScriptContext = luaL_newstate();
	luaL_openlibs(m_pScriptContext);
}

CLua::~CLua()
{
	if (m_pScriptContext)
		lua_close(m_pScriptContext);

}

static CString findScript(CString strFullName)
{
	string strFileName = strFullName.GetStringA();
	
	FILE* fFind;

	char szDrive[MAX_PATH];
	char szDir[MAX_PATH];
	char szFileName[MAX_PATH];
	char szExtension[MAX_PATH];

	_splitpath(strFileName.c_str(), szDrive, szDir, szFileName, szExtension);

	string strTestFile = (string)szDrive + szDir + ("Scripts\\") + szFileName + (".LUB");
	fFind = fopen(strTestFile.c_str(), "r");
	if (!fFind)
	{
		strTestFile = (string)szDrive + szDir + ("Scripts\\") + szFileName + (".LUA");
		fFind = fopen(strTestFile.c_str(), "r");
	}

	if (!fFind)
	{
		strTestFile = (string)szDrive + szDir + szFileName + (".LUB");
		fFind = fopen(strTestFile.c_str(), "r");
	}

	if (!fFind)
	{
		strTestFile = (string)szDrive + szDir + szFileName + (".LUA");
		fFind = fopen(strTestFile.c_str(), "r");
	}

	if (fFind)
	{
		fclose(fFind);
	}

	return strTestFile;
}

bool CLua::RunScript(CString strFileName)
{
	LOCK_GUARD(m_oMutex);
	CString strFullName = findScript(strFileName);

	if ( 0 != luaL_loadfile(m_pScriptContext, strFullName.a_str()))
	{
		HandlerError("Load Script", strFullName);
		return false;
	}

	if ( 0 != lua_pcall(m_pScriptContext,0,LUA_MULTRET,0))
	{
		HandlerError("Run Script", strFullName);
		return false;
	}
	return true;
}


bool CLua::RunBuffer( LPCSTR pBuffer,size_t sz)
{
	if (0 != luaL_loadbuffer(m_pScriptContext, (LPCSTR)pBuffer, sz, NULL))
	{
		CString str(pBuffer);
		HandlerError("Load Buffer", str);
		return false;
	}

	if (0 != lua_pcall(m_pScriptContext, 0, LUA_MULTRET, 0))
	{
		CString str(pBuffer);
		HandlerError("Run Buffer", str);
		return false;
	}
	return true;
}

bool CLua::RunString(CString strCommand)
{
	LOCK_GUARD(m_oMutex);
	if (0 != luaL_loadstring(m_pScriptContext, strCommand.a_str()))
	{
		HandlerError("String Load", strCommand);
		return false;
	}

	if (0 != lua_pcall(m_pScriptContext, 0, LUA_MULTRET, 0))
	{
		HandlerError("Run String", strCommand);
		return false;
	}

	return true;
}

CString CLua::GetErrorString()
{
	CString strError(luaL_checkstring(m_pScriptContext, -1));

	return strError;
}


bool CLua::AddFunction( CString pFunctionName, LuaFunctionType pFunction)
{
	LuaFunc pFunc = (LuaFunc)pFunction;
	lua_register(m_pScriptContext, pFunctionName.a_str(), pFunc);
	return true;
}


CString CLua::GetStringArgument(int num, CString pDefault)
{
	auto str = luaL_optstring(m_pScriptContext, num, pDefault.a_str());

	return str;
}

double CLua::GetNumberArgument(int num, double dDefault)
{
	return luaL_optnumber(m_pScriptContext, num, dDefault);
}

void CLua::PushString(CString pString)
{
	lua_pushstring(m_pScriptContext, pString.a_str());
}

void CLua::PushNumber(double value)
{
	lua_pushnumber(m_pScriptContext, value);
}

bool CLua::RunFunction(CString strFunctionName, CString args)
{
	CString cmd;
	cmd.FormatW(L"%s(%s)", strFunctionName.w_str(), args.w_str());
	return RunString(cmd);
}

void CLua::HandlerError(CString strErrorType, CString strCmd)
{
	if (m_pErrorHandler)
	{
		CString msg;
		msg.FormatW(L"\n Error - %s:\n %s\n Error Message:%s", strErrorType.w_str(), strCmd.w_str(), GetErrorString().w_str());
		m_pErrorHandler(msg);
	}
}

map<wstring, wstring> CLua::GetTableParam(int index)
{
	auto L = m_pScriptContext;
	map<wstring, wstring> data;
	lua_pushnil(L);
	// ���ڵ�ջ��-1 => nil; index => table
	if ( index >= lua_gettop(L))
	{
		throw CException("The index is too big.");
	}

	while (lua_next(L, index))
	{
		// ���ڵ�ջ��-1 => value; -2 => key; index => table
		// ����һ�� key ��ջ����Ȼ������� lua_tostring �Ͳ���ı�ԭʼ�� key ֵ��
		lua_pushvalue(L, -2);
		// ���ڵ�ջ��-1 => key; -2 => value; -3 => key; index => table

		CString k = lua_tostring(L, -1);
		CString v = lua_tostring(L, -2);
		wstring key = k.GetStringW();
		wstring value = v.GetStringW();

		data[key] = value;
		// ���� value �Ϳ����� key������ԭʼ�� key ��Ϊ��һ�� lua_next �Ĳ���
		lua_pop(L, 2);
		// ���ڵ�ջ��-1 => key; index => table
	}
	// ���ڵ�ջ��index => table ����� lua_next ���� 0 ��ʱ�����Ѿ�����һ�����µ� key �������ˣ�
	// ����ջ�Ѿ��ָ��������������ʱ��״̬
	return data;
}

LuaType CLua::CheckType(int index)
{
	int nType = lua_type(m_pScriptContext, index);
	return (LuaType)nType;
}

double CLua::StringToNumber(CString string)
{
	lua_stringtonumber(m_pScriptContext, string.a_str());
	
	return lua_tonumber(m_pScriptContext, -1);
}

inline lua_State* CLua::GetScriptContext()
{
	return m_pScriptContext;
}

inline void CLua::SetErrorHandle(void(*pErrHandler)(CString strError))
{
	m_pErrorHandler = pErrHandler;
}

