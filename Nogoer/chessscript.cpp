#include "chessscript.h"

#include <QDebug>

ChessScript* ChessScript::inst = 0;

int ChessScript::lua_loadEngine(lua_State* L)
{
    if(lua_isstring(L, 1))
        inst->doLoadEngine(true, QString::fromLocal8Bit(lua_tostring(L, 1)));
    if(lua_isstring(L, 2))
        inst->doLoadEngine(false, QString::fromLocal8Bit(lua_tostring(L, 2)));
    return 0;
}

int ChessScript::lua_startChess(lua_State* L)
{
    int n=1;
    if(lua_isnumber(L, 1))
        n=lua_tointeger(L, 1);
    inst->whiteWins=0;
    inst->blackWins=0;
    while(n--)
        inst->doStartChess();
    lua_pushnumber(L, inst->whiteWins);
    lua_pushnumber(L, inst->blackWins);
    return 2;
}

void ChessScript::doLoadEngine(bool isWhite, QString filename)
{
    emit loadEngine(isWhite, filename);
    yield.wait(&mtx);
}

void ChessScript::doStartChess()
{
    emit startChess();
    yield.wait(&mtx);
}

void ChessScript::run()
{
    try
    {
        script.loadScriptFile(path.data());
    }
    catch(const LuaError& e)
    {
        qWarning()<<e.what()<<"(code: "<<e.code()<<")";
    }
}

ChessScript::ChessScript(const QByteArray& filename)
    :path(filename)
{
    inst=this;
    script.registerFunc("loadEngine", lua_loadEngine);
    script.registerFunc("start", lua_startChess);
    mtx.lock();
}

void ChessScript::increaseWinCount(bool isWhite)
{
    ++(isWhite?whiteWins:blackWins);
}

void ChessScript::resume()
{
    yield.wakeOne();
}
