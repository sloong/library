#include "stdafx.h"
#include "luapacket.h"
#include <sstream>
#include "exception.h"

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

CLuaPacket::CLuaPacket()
{
}

CLuaPacket::~CLuaPacket()
{
}

void CLuaPacket::empty(lua_State *L)
{
    m_oDataMap.clear();
}

void CLuaPacket::setdata(lua_State *L)
{
    int nType = lua_type(L,2);
    if( nType == LUA_TUSERDATA || nType == LUA_TTABLE )
    {
        lua_pushboolean(L,0);
        return;
    }
    if(lua_isnumber(L,1))
    {
        lua_pushboolean(L,SetFieldValue(PAI(L,1),PAS(L,2),PASL(L,2)) );
        return;
    }

    lua_pushboolean(L,SetFieldValue(PAS(L,1),PAS(L,2),PASL(L,2)));
    return;
}

void CLuaPacket::getdata(lua_State *L)
{
    if(lua_isnumber(L,1))
    {
        getdata_n(L);
        return;
    }

    string key(PAS(L,1));
    if( true == Exist(key) )
    {
        string value = m_oDataMap[key];
        lua_pushlstring(L,value.c_str(),value.length());
    }
    else
    {
        lua_pushnil(L);
    }
}


void CLuaPacket::getdata_n(lua_State *L)
{
    string key(ntos(PAI(L,1)));
    if( true == Exist(key) )
    {
        string value = m_oDataMap[key];
        lua_pushlstring(L,value.c_str(),value.length());
    }
    else
    {
        lua_pushnil(L);
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

bool CLuaPacket::SetFieldValue(ULONG ulID, string strFieldValue, int nLen )
{
    if( ulID == 0)
    {
        return false;
    }

    return SetFieldValue(ntos(ulID),strFieldValue, nLen);
}


bool CLuaPacket::SetFieldValue(string key, string value, int nLen )
{
    if( key.empty() )
    {
        return false;
    }

    if( value.length() != (size_t)nLen )
    {
        throw normal_except("The lua string length is diff.");
    }
    LOCK_GUARD(m_oMutex);
    m_oDataMap[key] = value;
    return true;
}
