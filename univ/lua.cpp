#include "stdafx.h"
#include "lua.h"
#include "exception.h"
#include "string\\string.h"
using namespace Sloong::Universal;

typedef int(*LuaFunc)(lua_State* pLuaState);

CLua::CLua()
{
	m_pErrorHandler = NULL;

	m_pScriptContext = luaL_newstate();
	luaL_openlibs(m_pScriptContext);
	m_pMutex = CreateMutex(NULL, FALSE, _T("SloongLuaMutex"));
}

CLua::~CLua()
{
	if (m_pScriptContext)
		lua_close(m_pScriptContext);

	CloseHandle(m_pMutex);
}

static wstring findScript(CString strFullName)
{
	string strFileName = strFullName.GetStringA();
	
	FILE* fFind;

	char szDrive[MAX_PATH];
	char szDir[MAX_PATH];
	char szFileName[MAX_PATH];
	char szExtension[MAX_PATH];

	_splitpath_s(strFileName.c_str(), szDrive, MAX_PATH, szDir, MAX_PATH, szFileName, MAX_PATH, szExtension, MAX_PATH);

	string strTestFile = (string)szDrive + szDir + ("Scripts\\") + szFileName + (".LUB");
	fopen_s(&fFind,strTestFile.c_str(), "r");
	if (!fFind)
	{
		strTestFile = (string)szDrive + szDir + ("Scripts\\") + szFileName + (".LUA");
		fopen_s(&fFind, strTestFile.c_str(), "r");
	}

	if (!fFind)
	{
		strTestFile = (string)szDrive + szDir + szFileName + (".LUB");
		fopen_s(&fFind, strTestFile.c_str(), "r");
	}

	if (!fFind)
	{
		strTestFile = (string)szDrive + szDir + szFileName + (".LUA");
		fopen_s(&fFind, strTestFile.c_str(), "r");
	}

	if (fFind)
	{
		fclose(fFind);
	}

	return CUniversal::ToWString(strTestFile);
}

bool CLua::RunScript(CString strFileName)
{
	WaitForSingleObject(m_pMutex, INFINITE);

	wstring strFullName = findScript(strFileName);

	if ( 0 != luaL_loadfile(m_pScriptContext, CUniversal::ToString(strFullName).c_str()))
	{
		HandlerError(_T("Load Script"), strFullName.c_str());
		ReleaseMutex(m_pMutex);
		return false;
	}

	if ( 0 != lua_pcall(m_pScriptContext,0,LUA_MULTRET,0))
	{
		HandlerError(_T("Run Script"), strFullName.c_str());
		ReleaseMutex(m_pMutex);
		return false;
	}
	ReleaseMutex(m_pMutex);
	return true;
}


bool CLua::RunBuffer( LPCSTR pBuffer,size_t sz)
{
	if (0 != luaL_loadbuffer(m_pScriptContext, (LPCSTR)pBuffer, sz, NULL))
	{
		string str(pBuffer);
		HandlerError(_T("Load Buffer"), CUniversal::ToWString(str).c_str());
		return false;
	}

	if (0 != lua_pcall(m_pScriptContext, 0, LUA_MULTRET, 0))
	{
		string str(pBuffer);
		HandlerError(_T("Run Buffer"), CUniversal::ToWString(str).c_str());
		return false;
	}
	return true;
}

bool CLua::RunString(CString strCommand)
{
	WaitForSingleObject(m_pMutex, INFINITE);
	if (0 != luaL_loadstring(m_pScriptContext, strCommand.GetStringA().c_str()))
	{
		HandlerError(_T("String Load"), strCommand);
		ReleaseMutex(m_pMutex);
		return false;
	}

	if (0 != lua_pcall(m_pScriptContext, 0, LUA_MULTRET, 0))
	{
		HandlerError(_T("Run String"), strCommand);
		ReleaseMutex(m_pMutex);
		return false;
	}

	ReleaseMutex(m_pMutex);
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
	lua_register(m_pScriptContext, pFunctionName.GetStringA().c_str(), pFunc);
	return true;
}


CString CLua::GetStringArgument(int num, CString pDefault)
{
	auto str = luaL_optstring(m_pScriptContext, num, pDefault.GetStringA().c_str());

	return str;
}

double CLua::GetNumberArgument(int num, double dDefault)
{
	return luaL_optnumber(m_pScriptContext, num, dDefault);
}

void CLua::PushString(CString pString)
{
	lua_pushstring(m_pScriptContext, pString.GetStringA().c_str());
}

void CLua::PushNumber(double value)
{
	lua_pushnumber(m_pScriptContext, value);
}

bool CLua::RunFunction(CString strFunctionName, CString args)
{
	CString cmd;
	cmd.FormatA("%s(%s)", strFunctionName.GetStringA().c_str(), args.GetStringA().c_str());
	return RunString(cmd);
}

void CLua::HandlerError(CString strErrorType, CString strCmd)
{
	if (m_pErrorHandler)
	{
		CString msg;
		msg.FormatW(L"\n Error - %s:\n %s\n Error Message:%s", strErrorType.GetStringW().c_str(), strCmd.GetStringW().c_str(), GetErrorString().GetStringW().c_str());
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
		throw CException(_T("The index is too big."));
	}

	while (lua_next(L, index))
	{
		// 现在的栈：-1 => value; -2 => key; index => table
		// 拷贝一份 key 到栈顶，然后对它做 lua_tostring 就不会改变原始的 key 值了
		lua_pushvalue(L, -2);
		// 现在的栈：-1 => key; -2 => value; -3 => key; index => table

		wstring key = CUniversal::ToWString(lua_tostring(L, -1));
		wstring value = CUniversal::ToWString(lua_tostring(L, -2));

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
	lua_stringtonumber(m_pScriptContext, string.GetStringA().c_str());
	
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

void CLua::InitializeWindow(HWND hWnd)
{

}

void Sloong::Universal::CLua::ShowLuaWindow()
{

}

void Sloong::Universal::CLua::RenderLuaWindow()
{

}
