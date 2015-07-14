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

    void doLoadEngine(bool isFirst, QString filename);
    void doStartChess(QString paramFirst, QString paramSecond);

    void run();

signals:

    void loadEngine(bool isFirst, QString filename);
    void startChess(QString paramFirst, QString paramSecond);
    void error(QString errorString);

public:

    ChessScript(const QByteArray& filename, QObject* parent=0);

    void increaseWinCount(bool isWhite);

public slots:

    void resume();

};

#endif // CHESSSCRIPT_H
