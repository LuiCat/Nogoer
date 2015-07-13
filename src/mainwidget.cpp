#include "mainwidget.h"

#include <QKeyEvent>
#include <QPainter>
#include <QMessageBox>
#include <QFileDialog>

int MainWidget::sideMinimumWidth = 160;
int MainWidget::controlPanelHeight = 100;

MainWidget::MainWidget(QWidget *parent)
    :QWidget(parent)
    ,gameStarted(false)
    ,gameCount(0)
    ,moveCount(0)
    ,playerBlack(true)
    ,chessboard(0)
    ,engineBlack(0)
    ,engineWhite(0)
{
    setMinimumSize(800, 480);

    widgetChessBoard = new ChessBoardWidget(0, this);

    widgetClockBlack = new ClockWidget(this);
    widgetClockWhite = new ClockWidget(this);

    widgetHistory = new HistoryWidget(this);

    widgetControl = new ControlWidget(this);

    connect(widgetChessBoard, SIGNAL(clickGrid(int, int)), this, SLOT(doPlayerMove(int, int)));
    connect(widgetHistory, SIGNAL(showHistory(int, int)), this, SLOT(doShowHistory(int, int)));

    connect(widgetClockBlack, SIGNAL(loadEngine()), this, SLOT(loadEngineBlack()));
    connect(widgetClockBlack, SIGNAL(unloadEngine()), this, SLOT(unloadEngineBlack()));
    connect(widgetClockBlack, SIGNAL(showLog()), this, SLOT(showLogBlack()));
    connect(widgetClockWhite, SIGNAL(loadEngine()), this, SLOT(loadEngineWhite()));
    connect(widgetClockWhite, SIGNAL(unloadEngine()), this, SLOT(unloadEngineWhite()));
    connect(widgetClockWhite, SIGNAL(showLog()), this, SLOT(showLogWhite()));

    connect(widgetControl, SIGNAL(startGame()), this, SLOT(restartGame()));
    connect(widgetControl, SIGNAL(stopGame()), this, SLOT(stopGame()));
    connect(widgetControl, SIGNAL(loadScript()), this, SLOT(loadScript()));
    connect(widgetControl, SIGNAL(setGuide(bool)), this, SLOT(setGuide(bool)));


}

MainWidget::~MainWidget()
{

}

void MainWidget::restartGame()
{
    chessboard = new ChessBoard(this);
    ++gameCount;
    moveCount=0;
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
    const QString& path=QFileDialog::getOpenFileName(this, "Open Executable...", ".", "Chess Engine (*.exe)");
    if(!path.isNull())
        loadEngineBlack(path);
}

void MainWidget::loadEngineWhite()
{
    const QString& path=QFileDialog::getOpenFileName(this, "Open Executable...", ".", "Chess Engine (*.exe)");
    if(!path.isNull())
        loadEngineWhite(path);
}

void MainWidget::loadEngineBlack(QString path)
{
    if(engineBlack)
        unloadEngineBlack();
    engineBlack = new ChessEngine(this);
    connect(engineBlack, SIGNAL(engineExited(bool)), this, SIGNAL(onEngineExit(bool)));
    connect(engineBlack, SIGNAL(nameChanged(QString)), widgetClockBlack, SLOT(setPlayerName(QString)));
    connect(engineBlack, SIGNAL(moveChess(int,int)), this, SLOT(onEngineBlackMove(int,int)));
    if(!engineBlack->loadEngine(path))
    {
        delete engineBlack;
        engineBlack=0;
        return;
    }
    widgetClockBlack->setEngineState(true, engineBlack->getName());
}

void MainWidget::loadEngineWhite(QString path)
{
    if(engineWhite)
        unloadEngineWhite();
    engineWhite = new ChessEngine(this);
    connect(engineWhite, SIGNAL(engineExited(bool)), this, SIGNAL(onEngineExit(bool)));
    connect(engineWhite, SIGNAL(nameChanged(QString)), widgetClockWhite, SLOT(setPlayerName(QString)));
    connect(engineWhite, SIGNAL(moveChess(int,int)), this, SLOT(onEngineWhiteMove(int,int)));
    if(!engineWhite->loadEngine(path))
    {
        delete engineWhite;
        engineWhite=0;
        return;
    }
    widgetClockWhite->setEngineState(true, engineWhite->getName());
}

void MainWidget::unloadEngineBlack()
{
    if(!engineBlack)
        return;
    disconnect(engineBlack, SIGNAL(nameChanged(QString)), 0, 0);
    engineBlack->unloadEngine();
    delete engineBlack;
    widgetClockBlack->setEngineState(false);
}

void MainWidget::unloadEngineWhite()
{
    if(!engineWhite)
        return;
    disconnect(engineWhite, SIGNAL(nameChanged(QString)), 0, 0);
    engineWhite->unloadEngine();
    delete engineWhite;
    widgetClockWhite->setEngineState(false);
}

void MainWidget::showLogBlack()
{

}

void MainWidget::showLogWhite()
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

void MainWidget::onEngineExit(bool isCrash)
{
    if(isCrash)
    {
        widgetHistory->pushHistory("Engine Crashed", 1);
        QMessageBox::critical(this, "", QString("Engine crashed !"));
    }
}

void MainWidget::onEngineBlackMove(int x, int y)
{
    if(gameStarted&&playerBlack)
    {
        if(doMove(x, y))
            switchSide();
        else
        {
            widgetHistory->pushHistory(QString().sprintf("%c%d(INVALID)", 'A'+y, x), moveCount, gameCount);
            stopGame();
        }
    }
}

void MainWidget::onEngineWhiteMove(int x, int y)
{
    if(gameStarted&&!playerBlack)
    {
        if(doMove(x, y))
            switchSide();
        else
        {
            widgetHistory->pushHistory(QString().sprintf("%c%d(INVALID)", 'A'+y, x), moveCount, gameCount);
            stopGame();
        }
    }
}

void MainWidget::doPlayerMove(int x, int y)
{
    if(gameStarted&&doMove(x, y))
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
