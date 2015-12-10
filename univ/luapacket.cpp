#include "stdafx.h"
#include"Lunar.h"
#include "luapacket.h"
#include <sstream>
#include "exception.h"
#include "log.h"

#define LOCK_GUARD(m) {lock_guard<mutex> lck(m);}

extern "C" {
#include "../lua/src/lua.h"
#include "../lua/src/lualib.h"
#include "../lua/src/lauxlib.h"
}

using namespace Sloong::Universal;

#define PAI(L,n) (long)(lua_gettop(L) >= abs(n) && lua_isnumber(L,n) ? luaL_checknumber((L),(n)) : 0) //get number param

#define PAS(L,n) (lua_gettop(L) >= abs(n) && lua_isstring(L,n) ? luaL_checkstring((L),(n)) : "") // get string param

#define PASL(L,n) (lua_gettop(L) >= abs(n) && lua_isstring(L,n) ? luaL_len(L,n) : 0 ) // get string length

#define PAD(L,n) (double)(lua_gettop(L) >= abs(n) && lua_isnumber(L,n) ? luaL_checknumber((L),(n)) : 0.0f) // get float param


template<typename T>
string ntos(T n)
{
    stringstream ss;
    ss << n;
    return ss.str();
}

const char CLuaPacket::className[] =  "LuaPacket";
Lunar<CLuaPacket>::RegType CLuaPacket::methods[] =
{
    METHOD(CLuaPacket,empty),
    METHOD(CLuaPacket,setdata),
    METHOD(CLuaPacket,getdata),
    {0,0}
};

CLuaPacket::CLuaPacket()
{
}

CLuaPacket::CLuaPacket(lua_State* L)
{
}

CLuaPacket::~CLuaPacket()
{
}

int CLuaPacket::empty(lua_State *L)
{
    m_oDataMap.clear();
    return 1;
}

int CLuaPacket::setdata(lua_State *L)
{
    int nType = lua_type(L,2);
    if( nType == LUA_TUSERDATA || nType == LUA_TTABLE )
    {
        lua_pushboolean(L,0);
        return 0;
    }
    string key, value;
    if(lua_isnumber(L,1))
    {
        key = ntos(PAI(L,1));
    }
    else
    {
        key = PAS(L,1);
    }

    value = PAS(L,2);
    if(value.length() != (size_t)PASL(L,2))
        throw normal_except("The lua string length is diff.");

    SetData(key,value);
    lua_pushboolean(L,true);
    return 1;
}

void CLuaPacket::SetData(string key, string value)
{
    if( key.empty() )
    {
        return;
    }

    LOCK_GUARD(m_oMutex);
    m_oDataMap[key] = value;
}

int CLuaPacket::getdata(lua_State *L)
{
    string key;
    if(lua_isnumber(L,1))
    {
        key = ntos(PAI(L,1));
    }
    else
        key = string(PAS(L,1));

    try
    {
        /*string key(PAS(L,1));
            if( true == Exist(key) )
            {
                string value = m_oDataMap[key];
                lua_pushlstring(L,value.c_str(),value.length());
            }
            else
            {
                lua_pushnil(L);
            }*/
        string value = GetData(key);
        lua_pushlstring(L,value.c_str(),value.length());
    }
    catch(CExceptKeyNoFound ex)
    {
        CLog::showLog(CUniversal::Format("get data fiald.%s",ex.what()));
        lua_pushnil(L);
    }

    return 1;
}


string CLuaPacket::GetData(string key, bool except /* = false */ )
{
    if( true == Exist(key) )
    {
        return m_oDataMap[key];
    }
    else
    {
        if( except )
        {
            // TODO: message should be have the key name.
            //throw CExceptKeyNoFound((boost::format("key is not find in maps, key name is :%s")%key.c_str()).str());
            throw CExceptKeyNoFound(CUniversal::Format("key is not find in maps, key name is :%s",key));
        }
        else
        {
            return "";
        }
    }
}


bool CLuaPacket::Exist(string key)
{
    if( m_oDataMap.find(key) != m_oDataMap.end() )
    {
        return true;
    }
    else
    {
        return false;
    }
}

