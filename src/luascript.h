class LuaScript;

#ifndef LUASCRIPT_H
#define LUASCRIPT_H

#include "lua/lua.hpp"
#include <string>
#include <exception>
using namespace std;

#define lua_tableregister(L,n,f) (lua_pushstring(L,n),lua_pushcfunction(L,f),lua_settable(L,-3))

typedef int (*LuaFunc)(lua_State*);

class LuaError
{
private:
    string errorString;
    int errorCode;
public:
    LuaError(const char* str, int code)
        :errorString(str)
        ,errorCode(code)
    {
    }
    inline int code() const
    {
        return errorCode;
    }
    inline const char* what() const
    {
        return errorString.c_str();
    }
};

class LuaScript
{
protected:

    lua_State* luaState;

    void lua_excall(int narg, int nres) const;
    void lua_exload(const char* cstr) const;

public:

    explicit LuaScript();
    ~LuaScript();

    void reset();

    void loadScriptFile(const char* filename) const;

    lua_State* createthread(int idx) const;

    void registerFunc(const char* name, LuaFunc func);

    inline lua_State* L() const
    {
        return luaState;
    }

    inline operator lua_State* () const
    {
        return luaState;
    }

};

#endif
