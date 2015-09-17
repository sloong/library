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
	// 现在的栈：-1 => nil; index => table
	if ( index >= lua_gettop(L))
	{
		throw CException("The index is too big.");
	}

	while (lua_next(L, index))
	{
		// 现在的栈：-1 => value; -2 => key; index => table
		// 拷贝一份 key 到栈顶，然后对它做 lua_tostring 就不会改变原始的 key 值了
		lua_pushvalue(L, -2);
		// 现在的栈：-1 => key; -2 => value; -3 => key; index => table

		CString k = lua_tostring(L, -1);
		CString v = lua_tostring(L, -2);
		wstring key = k.GetStringW();
		wstring value = v.GetStringW();

		data[key] = value;
		// 弹出 value 和拷贝的 key，留下原始的 key 作为下一次 lua_next 的参数
		lua_pop(L, 2);
		// 现在的栈：-1 => key; index => table
	}
	// 现在的栈：index => table （最后 lua_next 返回 0 的时候它已经把上一次留下的 key 给弹出了）
	// 所以栈已经恢复到进入这个函数时的状态
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

