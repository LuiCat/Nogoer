#ifndef CHESSSCRIPT_H
#define CHESSSCRIPT_H

#include <QThread>
#include <QMutex>
#include <QWaitCondition>

#include "luascript.h"

class ChessScript : public QThread
{
    Q_OBJECT
private:

    LuaScript script;

    QByteArray path;

    QWaitCondition yield;
    QMutex mtx;

    int whiteWins;
    int blackWins;

protected:

    static ChessScript* inst;

    static int lua_loadEngine(lua_State* L);
    static int lua_startChess(lua_State* L);

    void doLoadEngine(bool isWhite, QString filename);
    void doStartChess();

    void run();

signals:

    void loadEngine(bool isWhite, QString filename);
    void startChess();

public:

    ChessScript(const QByteArray& filename);

    void increaseWinCount(bool isWhite);

public slots:

    void resume();

};

#endif // CHESSSCRIPT_H
