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
    QString lineFirst, lineSecond;
    if(lua_isstring(L, 2))
        lineFirst=QString::fromLocal8Bit(lua_tostring(L, 2));
    if(lua_isstring(L, 3))
        lineSecond=QString::fromLocal8Bit(lua_tostring(L, 3));
    while(n--)
        inst->doStartChess(lineFirst, lineSecond);
    lua_pushnumber(L, inst->whiteWins);
    lua_pushnumber(L, inst->blackWins);
    return 2;
}

void ChessScript::doLoadEngine(bool isFirst, QString filename)
{
    emit loadEngine(isFirst, filename);
    yield.wait(&mtx);
}

void ChessScript::doStartChess(QString paramFirst, QString paramSecond)
{
    emit startChess(paramFirst, paramSecond);
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
        emit error(QString("%0 (code: %1)").arg(e.what()).arg(e.code()));
    }
}

ChessScript::ChessScript(const QByteArray& filename, QObject* parent)
    :QThread(parent)
    ,path(filename)
{
    inst=this;
    script.registerFunc("loadEngine", lua_loadEngine);
    script.registerFunc("start", lua_startChess);
    mtx.lock();
}

void ChessScript::increaseWinCount(bool isWhite, int num)
{
    (isWhite?whiteWins:blackWins)+=num;
}

void ChessScript::resume()
{
    yield.wakeOne();
}
