#include "mainwidget.h"

#include <QKeyEvent>
#include <QPainter>
#include <QtWidgets/QMessageBox>

int MainWidget::sideMinimumWidth = 160;
int MainWidget::controlPanelHeight = 100;

MainWidget::MainWidget(QWidget *parent)
    :QWidget(parent)
    ,gameStarted(false)
    ,gameCount(0)
    ,moveCount(0)
    ,playerBlack(true)
{
    setMinimumSize(800, 480);

    widgetChessBoard = new ChessBoardWidget(0, this);

    widgetClockBlack = new ClockWidget(this);
    widgetClockWhite = new ClockWidget(this);

    widgetHistory = new HistoryWidget(this);

    widgetControl = new ControlWidget(this);

    connect(widgetChessBoard, SIGNAL(clickGrid(int, int)), this, SLOT(doPlayerMove(int, int)));
    connect(widgetHistory, SIGNAL(showHistory(int, int)), this, SLOT(doShowHistory(int, int)));

    connect(widgetControl, SIGNAL(startGame()), this, SLOT(restartGame()));
    connect(widgetControl, SIGNAL(stopGame()), this, SLOT(stopGame()));
    connect(widgetControl, SIGNAL(loadScript()), this, SLOT(loadScript()));
    connect(widgetControl, SIGNAL(setGuide(bool)), this, SLOT(setGuide(bool)));/*
    t1 = new QPushButton("test1",this);
    t2 = new QPushButton("test2",this);
    t3 = new QPushButton("test3",this);

    t1->setGeometry(0, 20, 50, 20);
    t2->setGeometry(0, 40, 50, 20);
    t3->setGeometry(0, 60, 50, 20);

    connect(t1, SIGNAL(released()), this, SLOT(slot1()));
    connect(t2, SIGNAL(released()), this, SLOT(slot2()));
    connect(t3, SIGNAL(released()), this, SLOT(slot3()));*/
}

MainWidget::~MainWidget()
{

}

void MainWidget::restartGame()
{
    chessboard = new ChessBoard(this);
    ++gameCount;
    playerBlack=true;
    historyBoard.insert(gameCount, chessboard);
    widgetChessBoard->setChessBoard(chessboard);
    widgetHistory->pushHistory("Game Started");
    widgetControl->setGameState(true);
    gameStarted=true;
    widgetClockBlack->timeClear();
    widgetClockWhite->timeClear();
    widgetClockBlack->timeStart();
}

void MainWidget::stopGame()
{
    if(!gameStarted)
        return;
    widgetClockBlack->timeStop();
    widgetClockWhite->timeStop();
    gameStarted=false;
    widgetControl->setGameState(false);
    widgetHistory->pushHistory("Game Stopped");
    chessboard->setFinished();
}

void MainWidget::loadScript()
{

}

void MainWidget::loadScript(QString path)
{

}

void MainWidget::setGuide(bool enable)
{
    widgetChessBoard->setGuide(enable);
}

void MainWidget::loadEngineBlack()
{

}

void MainWidget::loadEngineWhite()
{

}

void MainWidget::loadEngineBlack(QString path)
{

}

void MainWidget::loadEngineWhite(QString path)
{

}

void MainWidget::resizeEvent(QResizeEvent*)
{
    int widthCB, heightCB;
    int yCB;
    int widthLeft, widthRight;

    widthCB=width()-sideMinimumWidth*2;
    heightCB=(height()>widthCB?widthCB:(widthCB=height()));
    widthLeft=(width()-widthCB)/2;
    widthRight=width()-widthCB-widthLeft;
    yCB=(height()-heightCB)/2;

    widgetChessBoard->setGeometry(widthLeft, yCB, widthCB, heightCB);

    widgetClockBlack->setGeometry(widthLeft+widthCB-10, 0, widthRight+10, (height()+10)/2);
    widgetClockWhite->setGeometry(widthLeft+widthCB-10, (height()-10)/2, widthRight+10, (height()+10)/2);

    widgetHistory->setGeometry(0, 0, widthLeft+10, height()-controlPanelHeight);

    widgetControl->setGeometry(0, height()-controlPanelHeight-10, widthLeft+10, controlPanelHeight+10);

}

void MainWidget::doPlayerMove(int x, int y)
{
    if(doMove(x, y))
        switchSide();
}

void MainWidget::switchSide()
{
    (playerBlack?widgetClockBlack:widgetClockWhite)->timeStop();
    ++moveCount;
    playerBlack=!playerBlack;
    (playerBlack?widgetClockBlack:widgetClockWhite)->timeStart();
}

void MainWidget::doShowHistory(int gameNum, int stepNum)
{
    if(historyBoard.count(gameNum))
    {
        widgetChessBoard->showChessBoard(historyBoard[gameNum]);
        widgetChessBoard->showHistory(stepNum);
    }
}

bool MainWidget::doMove(int x, int y)
{
    if(!chessboard->checkMove(x, y, playerBlack))
        return false;
    widgetChessBoard->doChess(x, y, playerBlack);
    widgetHistory->pushHistory(QString().sprintf("%c%d", 'A'+y, x), moveCount+1, gameCount);
    if(chessboard->checkFinished(!playerBlack))
    {
        stopGame();
        QMessageBox::information(this, "Game Over", QString("Winner is %0 !").arg(playerBlack?"BLACK":"WHITE"));
    }
    return true;
}
