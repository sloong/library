#include "stdafx.h"
#include "lua.h"
#include "exception.h"
#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>
using namespace Sloong::Universal;

typedef int(*LuaFunc)(lua_State* pLuaState);

#define LOCK_GUARD(m) {lock_guard<mutex> lck(m);}
extern "C" {
#include "../lua/src/lua.h"
#include "../lua/src/lualib.h"
#include "../lua/src/lauxlib.h"

                }
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

static std::string findScript(std::string strFullName)
{
	string strFileName = strFullName;
	
	FILE* fFind;

	char szDir[MAX_PATH] = {0};

	getcwd(szDir,MAX_PATH);
	string strDir(szDir);
	strDir += "/";

    string strTestFile = strDir + ("script/") + strFileName;
	fFind = fopen(strTestFile.c_str(), "r");
	if (!fFind)
	{
        strTestFile = strDir + ("script\\") + strFileName + (".lua");
		fFind = fopen(strTestFile.c_str(), "r");
	}

    if (!fFind)
    {
        strTestFile = strDir + ("script\\") + strFileName + (".lub");
        fFind = fopen(strTestFile.c_str(), "r");
    }

	if (!fFind)
	{
		strTestFile = strDir + strFileName + (".lub");
		fFind = fopen(strTestFile.c_str(), "r");
	}

	if (!fFind)
	{
		strTestFile = strDir + strFileName + (".lua");
		fFind = fopen(strTestFile.c_str(), "r");
	}

	if (fFind)
	{
		fclose(fFind);
	}

	return strTestFile;
}

bool CLua::RunScript(std::string strFileName)
{
	LOCK_GUARD(m_oMutex);
	std::string strFullName = findScript(strFileName);

	if ( 0 != luaL_loadfile(m_pScriptContext, strFullName.c_str()))
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
		std::string str(pBuffer);
		HandlerError("Load Buffer", str);
		return false;
	}

	if (0 != lua_pcall(m_pScriptContext, 0, LUA_MULTRET, 0))
	{
		std::string str(pBuffer);
		HandlerError("Run Buffer", str);
		return false;
	}
	return true;
}

bool CLua::RunString(std::string strCommand)
{
	LOCK_GUARD(m_oMutex);
	if (0 != luaL_loadstring(m_pScriptContext, strCommand.c_str()))
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

std::string CLua::GetErrorString()
{
	std::string strError(luaL_checkstring(m_pScriptContext, -1));

	return strError;
}


bool CLua::AddFunction( std::string pFunctionName, LuaFunctionType pFunction)
{
	LuaFunc pFunc = (LuaFunc)pFunction;
	lua_register(m_pScriptContext, pFunctionName.c_str(), pFunc);
	return true;
}


std::string CLua::GetStringArgument(int num, std::string pDefault)
{
	auto str = luaL_optstring(m_pScriptContext, num, pDefault.c_str());

	return str;
}

double CLua::GetNumberArgument(int num, double dDefault)
{
	return luaL_optnumber(m_pScriptContext, num, dDefault);
}

void CLua::PushString(std::string pString)
{
	lua_pushstring(m_pScriptContext, pString.c_str());
}

void CLua::PushNumber(double value)
{
	lua_pushnumber(m_pScriptContext, value);
}

bool CLua::RunFunction(std::string strFunctionName, std::string args)
{
	std::string cmd = (boost::format("%1%(%2%)") % strFunctionName % args).str();
	return RunString(cmd);
}

void CLua::HandlerError(std::string strErrorType, std::string strCmd)
{
	if (m_pErrorHandler)
	{
		std::string msg = (boost::format("\n Error - %1%:\n %2%\n Error Message:%3%") % strErrorType % strCmd % GetErrorString()).str();
		m_pErrorHandler(msg);
	}
}

map<string, string> CLua::GetTableParam(int index)
{
	auto L = m_pScriptContext;
	map<string, string> data;
	lua_pushnil(L);
	// ���ڵ�ջ��-1 => nil; index => table
	if ( index >= lua_gettop(L))
	{
		throw normal_except("The index is too big.");
	}

	while (lua_next(L, index))
	{
		// ���ڵ�ջ��-1 => value; -2 => key; index => table
		// ����һ�� key ��ջ����Ȼ������� lua_tostring �Ͳ���ı�ԭʼ�� key ֵ��
		lua_pushvalue(L, -2);
		// ���ڵ�ջ��-1 => key; -2 => value; -3 => key; index => table
		
		string key = std::string(lua_tostring(L, -1));
		string value = std::string(lua_tostring(L, -2));

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

double CLua::StringToNumber(std::string string)
{
	lua_stringtonumber(m_pScriptContext, string.c_str());
	
	return lua_tonumber(m_pScriptContext, -1);
}

inline lua_State* CLua::GetScriptContext()
{
	return m_pScriptContext;
}

inline void CLua::SetErrorHandle(void(*pErrHandler)(std::string strError))
{
	m_pErrorHandler = pErrHandler;
}

