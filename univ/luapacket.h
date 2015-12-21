#ifndef LUAPACKET_H
#define LUAPACKET_H


#include "univ.h"
#include <mutex>
#include<map>
using std::mutex;
using std::map;
#include"Lunar.h"
struct lua_State;
namespace Sloong
{
    namespace Universal
    {
        class UNIVERSAL_API CLuaPacket
        {
        public:
            static const char className[];
            static Lunar<CLuaPacket>::RegType methods[];
        public:
            CLuaPacket();
            CLuaPacket(lua_State* L);
            virtual ~CLuaPacket();

            int empty(lua_State* L);
            int setdata(lua_State* L);
            int getdata(lua_State* L);

            void SetData(string key, string value);
            string GetData(string key, bool except = false);

        protected:
            bool Exist(string key);

        private:
            mutex	m_oMutex;
            map<string,string> m_oDataMap;
        };

    }
}

#endif // LUAPACKET_H
