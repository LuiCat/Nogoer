#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QDialog>
#include <QMap>

#include "chessboardwidget.h"
#include "clockwidget.h"
#include "historywidget.h"
#include "controlwidget.h"
#include "showlogwidget.h"

#include "chessboard.h"
#include "chessengine.h"

class MainWidget : public QWidget
{
    Q_OBJECT

public:

    explicit MainWidget(QWidget *parent = 0);
    ~MainWidget();

public slots:

    void restartGame();
    void stopGame();

    void loadScript();
    void loadScript(QString path);

    void setGuide(bool enable);

    void loadEngineBlack();
    void loadEngineWhite();
    void loadEngineBlack(QString path);
    void loadEngineWhite(QString path);
    void unloadEngineBlack();
    void unloadEngineWhite();

    void showLogBlack();
    void showLogWhite();

    void doPlayerMove(int x, int y);
    void switchSide();

    void doShowHistory(int gameNum, int stepNum);

//    void slot1()
//    {
//        widgetClockBlack->timeStart();
//    }
//    void slot2()
//    {
//        widgetClockBlack->timeStop();
//    }
//    void slot3()
//    {
//        widgetClockBlack->timeClear();
//    }

protected:

    void resizeEvent(QResizeEvent*);

protected slots:

    void onEngineExit(bool isCrash);
    void onEngineBlackMove(int x, int y);
    void onEngineWhiteMove(int x, int y);

private:

    static int sideMinimumWidth;
    static int controlPanelHeight;

    bool gameStarted;

    int gameCount;
    int moveCount;
    bool playerBlack;

    ChessBoard* chessboard;

    QMap<int, ChessBoard*> historyBoard;

    ChessEngine* engineBlack;
    ChessEngine* engineWhite;

    ChessBoardWidget* widgetChessBoard;

    ClockWidget* widgetClockBlack;
    ClockWidget* widgetClockWhite;

    HistoryWidget* widgetHistory;

    ControlWidget* widgetControl;

    inline ClockWidget* getClockWidget(bool isBlack)
    {
        return isBlack?widgetClockBlack:widgetClockWhite;
    }

    bool doMove(int x, int y);

    void playerBegin(bool isBlack);
    void playerEnd(bool isBlack);

    //QPushButton *t1, *t2, *t3;

};


#endif // MAINWIDGET_H
