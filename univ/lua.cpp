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

#include "Lunar.h"
CLua::CLua()
{
	m_pErrorHandler = NULL;

	m_pScriptContext = luaL_newstate();
	luaL_openlibs(m_pScriptContext);
    Lunar<CLuaPacket>::Register(m_pScriptContext);
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

#define lua_pushliteral(L,s) lua_pushlstring(L,"" s,(sizeof(s)/sizeof(char))-1)
#define LEVELS1 12  // size of the first part of the stack
#define LEVELS2 10 // size of the second part

std::string CLua::GetErrorString()
{
	std::string strError(luaL_checkstring(m_pScriptContext, -1));

    int level = 0;
    int firstpart = 1;

    lua_Debug ar;
    /*if(!lua_isstring(m_pScriptContext,1))
        return lua_gettop(m_pScriptContext);*/

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

    time_t st;
    time(&st);
    struct tm* lt = localtime(&st);
    std::string strTime = (boost::format("[%d/%d/%d - %.2d:%.2d:%.2d] : ") %(lt->tm_year + 1900) % lt->tm_mon % lt->tm_mday %
        lt->tm_hour% lt->tm_min% lt->tm_sec).str();

    FILE* pf = fopen("scripterr.log","a+b");
    if(pf)
    {
        fwrite(strTime.c_str(),1,strTime.length(),pf);
        fwrite(lua_tostring(m_pScriptContext,-1),1,luaL_len(m_pScriptContext,-1),pf);
        fclose(pf);
    }

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
	// 现在的栈：-1 => nil; index => table
	if ( index >= lua_gettop(L))
	{
		throw normal_except("The index is too big.");
	}

	while (lua_next(L, index))
	{
		// 现在的栈：-1 => value; -2 => key; index => table
		// 拷贝一份 key 到栈顶，然后对它做 lua_tostring 就不会改变原始的 key 值了
		lua_pushvalue(L, -2);
		// 现在的栈：-1 => key; -2 => value; -3 => key; index => table
		
		string key = std::string(lua_tostring(L, -1));
		string value = std::string(lua_tostring(L, -2));

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

bool CLua::RunFunction(string strFunctionName,CLuaPacket* pUserInfo, CLuaPacket* pRequest, CLuaPacket* pResponse )
{
    int nFunc = -1;
    lua_getglobal(m_pScriptContext,strFunctionName.c_str());
    if(lua_isfunction(m_pScriptContext,-1))
        nFunc = luaL_ref(m_pScriptContext,LUA_REGISTRYINDEX);

    lua_rawgeti(m_pScriptContext,LUA_REGISTRYINDEX,nFunc);

    PushPacket(pUserInfo);
    PushPacket(pRequest);
    PushPacket(pResponse);

    if( 0 != lua_pcall(m_pScriptContext,1,LUA_MULTRET,0))
    {
        GetErrorString();
    }
}
