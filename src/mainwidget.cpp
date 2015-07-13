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

    /*
    t1 = new QPushButton("test1",this);
    t2 = new QPushButton("test2",this);
    t3 = new QPushButton("test3",this);

    t1->setGeometry(0, 20, 50, 20);
    t2->setGeometry(0, 40, 50, 20);
    t3->setGeometry(0, 60, 50, 20);

    connect(t1, SIGNAL(released()), this, SLOT(slot1()));
    connect(t2, SIGNAL(released()), this, SLOT(slot2()));
    connect(t3, SIGNAL(released()), this, SLOT(slot3()));
    */

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
    if(engineBlack)engineBlack->reloadEngine();
    if(engineWhite)engineWhite->reloadEngine();
    historyBoard.insert(gameCount, chessboard);
    widgetChessBoard->setChessBoard(chessboard);
    widgetHistory->pushHistory("Game Started");
    widgetControl->setGameState(true);
    gameStarted=true;
    widgetClockBlack->timeClear();
    widgetClockWhite->timeClear();
    playerBegin(playerBlack);
    if(engineBlack)engineBlack->writeLine("newblack");
    if(engineWhite)engineWhite->writeLine("newwhite");
}

void MainWidget::stopGame()
{
    if(!gameStarted)
        return;
    if(engineBlack)engineBlack->writeLine("stop");
    if(engineWhite)engineWhite->writeLine("stop");
    playerEnd(!playerBlack);
    playerEnd(playerBlack);
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
    const QString& path=QFileDialog::getOpenFileName(this, "Open Executable...", "", "Chess Engine (*.exe);;All Files (*)");
    if(!path.isNull())
        loadEngineBlack(path);
}

void MainWidget::loadEngineWhite()
{
    const QString& path=QFileDialog::getOpenFileName(this, "Open Executable...", "", "Chess Engine (*.exe);;All Files (*)");
    if(!path.isNull())
        loadEngineWhite(path);
}

void MainWidget::loadEngineBlack(QString path)
{
    if(engineBlack)
        unloadEngineBlack();
    engineBlack = new ChessEngine(this);
    connect(engineBlack, SIGNAL(engineExited(bool)), this, SLOT(onEngineExit(bool)));
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
    connect(engineWhite, SIGNAL(engineExited(bool)), this, SLOT(onEngineExit(bool)));
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
    disconnect(engineBlack, 0, 0, 0);
    engineBlack->unloadEngine();
    delete engineBlack;
    engineBlack=0;
    widgetClockBlack->setEngineState(false);
}

void MainWidget::unloadEngineWhite()
{
    if(!engineWhite)
        return;
    disconnect(engineWhite, 0, 0, 0);
    engineWhite->unloadEngine();
    delete engineWhite;
    engineWhite=0;
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
    if(!gameStarted)
        return;
    if(playerBlack)
    {
        if(doMove(x, y))
        {
            switchSide();
        }
        else
        {
            widgetHistory->pushHistory(QString().sprintf("%c%d(INVALID)", 'A'+y, x), moveCount, gameCount);
            stopGame();
        }
    }
}

void MainWidget::onEngineWhiteMove(int x, int y)
{
    if(!gameStarted)
        return;
    if(!playerBlack)
    {
        if(doMove(x, y))
        {
            switchSide();
        }
        else
        {
            widgetHistory->pushHistory(QString().sprintf("%c%d(INVALID)", 'A'+y, x), moveCount, gameCount);
            stopGame();
        }
    }
}

void MainWidget::doPlayerMove(int x, int y)
{
    if(!gameStarted)
        return;
    if((playerBlack?engineBlack:engineWhite)==0&&doMove(x, y))
        switchSide();
}

void MainWidget::switchSide()
{
    if(!gameStarted)
        return;
    playerEnd(playerBlack);
    ++moveCount;
    playerBlack=!playerBlack;
    playerBegin(playerBlack);
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
    else
    {
        ChessEngine* engine=(playerBlack?engineWhite:engineBlack);
        if(engine)engine->writeMove(x, y);
    }
    return true;
}

void MainWidget::playerBegin(bool isBlack)
{
    (isBlack?widgetClockBlack:widgetClockWhite)->timeStart();
    widgetChessBoard->setHint((isBlack?engineBlack:engineWhite)==0);
}

void MainWidget::playerEnd(bool isBlack)
{
    (isBlack?widgetClockBlack:widgetClockWhite)->timeStop();
}
