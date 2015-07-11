#include "luascript.h"

//===========================================================================

void LuaScript::lua_excall(int narg, int nres) const
{
    int res=lua_pcall(luaState, narg, nres, 0);
    if(res!=0)throw LuaError(lua_tostring(luaState, -1), res);
}

void LuaScript::lua_exload(const char* cstr) const
{
    int res=luaL_loadfile(luaState, cstr);
    if(res!=0)throw LuaError(lua_tostring(luaState, -1), res);
}

lua_State* LuaScript::createthread(int idx) const
{
    if(!lua_isfunction(luaState, idx))return 0;
    lua_pushvalue(luaState, idx);
    lua_State* T=lua_newthread(luaState);
    lua_pop(luaState, 1);
    lua_xmove(luaState, T, 1);
    return T;
}

void LuaScript::loadScriptFile(const char* filename) const
{
    lua_exload(filename);
    lua_excall(0, 0);
}

void LuaScript::registerFunc(const char* name, LuaFunc func)
{
    lua_register(luaState, name, func);
}

//===========================================================================

LuaScript::LuaScript()
{
    luaState=luaL_newstate();
    luaL_openlibs(luaState);
}

LuaScript::~LuaScript()
{
    if(luaState)
        lua_close(luaState);
    luaState=0;
}

