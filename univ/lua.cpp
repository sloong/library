#include "stdafx.h"
#include "lua.h"
#include "exception.h"
#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>
#include <boost/foreach.hpp>
#ifdef _WINDOWS
#include <io.h>
#endif // _WINDOWS
using namespace Sloong::Universal;

typedef int(*LuaFunc)(lua_State* pLuaState);

#define LOCK_GUARD(m) {lock_guard<mutex> lck(m);}

#include "Lunar.h"

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

Lunar<CLuaPacket>::RegType g_methods[] =
{
    METHOD(CLuaPacket,empty),
    METHOD(CLuaPacket,setdata),
    METHOD(CLuaPacket,getdata),
    {0,0}
};


string g_SearchList[] =
{
    "%pathdir%scripts/%filename%.lua",
    "%pathdir%scripts/%filename%",
    "%pathdir%scripts/%filename%.lub",
    "%pathdir%%filename%.lua",
    "%pathdir%%filename%",
    "%pathdir%%filename%.lub",
};


CLua::CLua()
{
	m_pErrorHandler = NULL;

	m_pScriptContext = luaL_newstate();
	luaL_openlibs(m_pScriptContext);
    Lunar<CLuaPacket>::Register(m_pScriptContext,g_methods);
	m_strScriptFolder = "./";
}

CLua::~CLua()
{
	if (m_pScriptContext)
		lua_close(m_pScriptContext);
}

std::string CLua::findScript(std::string strFullName)
{
	string testFile(""),res("");
    for (int i = 0; i< ARRAY_SIZE(g_SearchList); i++)
	{
        testFile = CUniversal::Replace(CUniversal::Replace(g_SearchList[i], "%pathdir%", m_strScriptFolder), "%filename%", strFullName);;
		if (0 == access(testFile.c_str(), ACC_R))
		{
			res = testFile;
			break;
		}
	}
	return res;
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
#ifndef lua_pushliteral
#define lua_pushliteral(L,s) lua_pushlstring(L,"" s,(sizeof(s)/sizeof(char))-1)
#endif
#define LEVELS1 12  // size of the first part of the stack
#define LEVELS2 10 // size of the second part

std::string CLua::GetErrorString()
{
    int level = 0;
    int firstpart = 1;

    lua_Debug ar;
    if(!lua_isstring(m_pScriptContext,1))
        return "";

    lua_settop(m_pScriptContext,1);
    lua_pushliteral(m_pScriptContext,"\r\n");
    lua_pushliteral(m_pScriptContext,"Call Stack:\r\n");
    while (lua_getstack(m_pScriptContext,level++,&ar))
    {
        char buff[10];
        if(level > LEVELS1 && firstpart )
        {
            if(!lua_getstack(m_pScriptContext,level+LEVELS2,&ar))
            {
                level--;
            }
            else
            {
                lua_pushliteral(m_pScriptContext,"                    ....\r\n");
                while (lua_getstack(m_pScriptContext,level+LEVELS2,&ar))
                {
                    level++;
                }
            }
            firstpart = 0;
            continue;
        }

        sprintf(buff,"%4d-   ",level-1);
        lua_pushstring(m_pScriptContext,buff);
        lua_getinfo(m_pScriptContext,"Snl",&ar);
        lua_pushfstring(m_pScriptContext,"%s:",ar.short_src);
        if(ar.currentline >0)
            lua_pushfstring(m_pScriptContext,"%d:",ar.currentline);

        switch (*ar.namewhat)
        {
            case 'g': // global
            case 'l': // local
            case 'f': // field
            case 'm': // method
                lua_pushfstring(m_pScriptContext," In function '%s'",ar.name);
                break;
            default:
            {
                if(*ar.what == 'm')
                    lua_pushfstring(m_pScriptContext,"in main chunk");
                else if(*ar.what == 'C') // c function
                    lua_pushfstring(m_pScriptContext,"%s", ar.short_src);
                else
                    lua_pushfstring(m_pScriptContext," in function <%s:%d>",ar.short_src, ar.linedefined);
            }
        }
        lua_pushliteral(m_pScriptContext,"\r\n");
        lua_concat(m_pScriptContext,lua_gettop(m_pScriptContext));
    }

    lua_concat(m_pScriptContext,lua_gettop((m_pScriptContext)));

    return lua_tostring(m_pScriptContext,-1);
}


bool CLua::AddFunction(std::string pFunctionName, LuaFunctionType pFunction)
{
	LuaFunc pFunc = (LuaFunc)pFunction;
	lua_register(m_pScriptContext, pFunctionName.c_str(), pFunc);
	return true;
}

void CLua::AddFunctions(vector<LuaFunctionRegistr> *pFuncList)
{
    BOOST_FOREACH( LuaFunctionRegistr item, *pFuncList)
    {
        AddFunction(item.strFunctionName,item.pFunction);
    }
}


string CLua::GetStringArgument(lua_State* l, int nNum, std::string pDefault /* = "" */)
{
	auto str = luaL_optstring(l, nNum, pDefault.c_str());
	return str;
}

std::string CLua::GetStringArgument(int num, std::string pDefault)
{
	return GetStringArgument(m_pScriptContext, num, pDefault);
}

double CLua::GetNumberArgument(lua_State* l, int nNum, double dDefault /* = -1.0f */)
{
	return luaL_optnumber(l, nNum, dDefault);
}

double CLua::GetNumberArgument(int num, double dDefault)
{
	return GetNumberArgument(m_pScriptContext, num, dDefault);
}

void CLua::PushString(lua_State* l, std::string strString)
{
	lua_pushstring(l, strString.c_str());
}

void CLua::PushString(std::string pString)
{
	CLua::PushString(m_pScriptContext, pString);
}

void CLua::PushNumber(lua_State* l, double dValue)
{
	lua_pushnumber(l, dValue);
}

void CLua::PushNumber(double value)
{
	CLua::PushNumber(m_pScriptContext,value);
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
	return GetTableParam(m_pScriptContext, index);
}

LuaType CLua::CheckType(int index)
{
	int nType = lua_type(m_pScriptContext, index);
	return (LuaType)nType;
}


lua_State* CLua::GetScriptContext()
{
	return m_pScriptContext;
}

void CLua::SetErrorHandle(ErrorHandleType pErr)
{
    m_pErrorHandler = pErr;
}

void CLua::PushPacket( CLuaPacket* pData )
{
    if( pData )
    {
        Lunar<CLuaPacket>::push(m_pScriptContext,pData,false);
    }
    else
    {
        lua_pushnil(m_pScriptContext);
    }
}

bool CLua::GetLuaFuncRef( int& nFunc, const string& strFuncName )
{
    if( strFuncName.length() == 0)
        return false;

    lua_getglobal(m_pScriptContext, strFuncName.c_str());
    if(!lua_isfunction(m_pScriptContext,-1))
        return false;

    nFunc = luaL_ref(m_pScriptContext,LUA_REGISTRYINDEX);
    return true;
}

bool CLua::PushFunction(const string &strFuncName)
{
    int nFunc = 0;
    if( !GetLuaFuncRef(nFunc,strFuncName))
        return false;

    return PushFunction(nFunc);
}

bool CLua::PushFunction(int nFuncRef)
{
    lua_rawgeti(m_pScriptContext,LUA_REGISTRYINDEX,nFuncRef);
    if(!lua_isfunction(m_pScriptContext,-1))
    {
        return false;
    }
    return true;
}

bool CLua::RunFunction(string strFunctionName,CLuaPacket* pUserInfo, CLuaPacket* pRequest, CLuaPacket* pResponse )
{
    int nTop = lua_gettop(m_pScriptContext);
    int nErr = 0;

    PushFunction("OnError");
    nErr = lua_gettop(m_pScriptContext);

    PushFunction(strFunctionName);

    PushPacket(pUserInfo);
    PushPacket(pRequest);
    PushPacket(pResponse);



    if( 0 != lua_pcall(m_pScriptContext,3,LUA_MULTRET,nErr))
    {
        GetErrorString();
        return false;
    }
    lua_settop(m_pScriptContext,nTop);
    return true;
}

void Sloong::Universal::CLua::SetScriptFolder(string folder)
{
	m_strScriptFolder = folder;
    char tag = m_strScriptFolder[m_strScriptFolder.length() - 1];
    if (tag != '/' && tag != '\\')
	{
		m_strScriptFolder += '/';
	}
}

int Sloong::Universal::CLua::RunFunction(string strFunctionName, CLuaPacket* pUserInfo, string& strRequest, string& strResponse)
{
	int nTop = lua_gettop(m_pScriptContext);
	int nErr = 0;

	PushFunction("OnError");
	nErr = lua_gettop(m_pScriptContext);

	PushFunction(strFunctionName);

	PushPacket(pUserInfo);
    PushString(strRequest);
	
    if (0 != lua_pcall(m_pScriptContext, 2, LUA_MULTRET, nErr))
	{
		strResponse = GetErrorString();
		return -2;
	}
	strResponse = lua_tostring(m_pScriptContext, -2);
	int nRes = lua_tonumber(m_pScriptContext, -1);

	lua_settop(m_pScriptContext, nTop);
	return nRes;
}

void Sloong::Universal::CLua::RunFunction(string strFunctionName, CLuaPacket* pUserInfo)
{
	int nTop = lua_gettop(m_pScriptContext);
	int nErr = 0;

	PushFunction("OnError");
	nErr = lua_gettop(m_pScriptContext);

	PushFunction(strFunctionName);

	PushPacket(pUserInfo);

	if (0 != lua_pcall(m_pScriptContext, 1, LUA_MULTRET, nErr))
	{
		throw normal_except( GetErrorString());
	}
	lua_settop(m_pScriptContext, nTop);
}

int Sloong::Universal::CLua::GetIntegerArgument(lua_State*l, int nNum, int nDev /*= -1*/)
{
	return luaL_optinteger(l, nNum, nDev);
}

void Sloong::Universal::CLua::PushInteger(lua_State*l, int nValue)
{
	lua_pushinteger(l, nValue);
}

map<std::string, std::string> Sloong::Universal::CLua::GetTableParam(lua_State*l, int index)
{
	map<string, string> data;
	lua_pushnil(l);
	// 现在的栈：-1 => nil; index => table
	if (index >= lua_gettop(l))
	{
		throw normal_except("The index is too big.");
	}

	while (lua_next(l, index))
	{
		// 现在的栈：-1 => value; -2 => key; index => table
		// 拷贝一份 key 到栈顶，然后对它做 lua_tostring 就不会改变原始的 key 值了
		lua_pushvalue(l, -2);
		// 现在的栈：-1 => key; -2 => value; -3 => key; index => table

		string key = std::string(lua_tostring(l, -1));
		string value = std::string(lua_tostring(l, -2));

		data[key] = value;
		// 弹出 value 和拷贝的 key，留下原始的 key 作为下一次 lua_next 的参数
		lua_pop(l, 2);
		// 现在的栈：-1 => key; index => table
	}
	// 现在的栈：index => table （最后 lua_next 返回 0 的时候它已经把上一次留下的 key 给弹出了）
	// 所以栈已经恢复到进入这个函数时的状态
	return data;
}

void Sloong::Universal::CLua::PushInteger(int nValue)
{
	PushInteger(m_pScriptContext, nValue);
}

