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
    ,script(0)
{
    setMinimumSize(800, 480);

    widgetChessBoard = new ChessBoardWidget(0, this);

    widgetClockBlack = new ClockWidget(this);
    widgetClockWhite = new ClockWidget(this);

    widgetHistory = new HistoryWidget(this);

    widgetControl = new ControlWidget(this);

    widgetLogBlack = new ShowLogWidget(QString("Black Logs"));
    widgetLogWhite = new ShowLogWidget(QString("White Logs"));

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
    connect(widgetControl, SIGNAL(loadScript()), this, SLOT(doLoadScript()));
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

void MainWidget::restartGame(const QString& paramBlack, const QString& paramWhite)
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
    if(engineBlack)
    {
        engineBlack->writeLine(paramBlack.toLocal8Bit());
        engineBlack->writeLine("newblack");
    }
    if(engineWhite)
    {
        engineWhite->writeLine(paramWhite.toLocal8Bit());
        engineWhite->writeLine("newwhite");
    }
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
    const QString& path=QFileDialog::getOpenFileName(this, "Open Script File...", "",
                                                     "Lua Script (*.lua);;All Files (*)");
    if(!path.isNull())
        loadScript(path);
}

void MainWidget::loadScript(QString path)
{
    unloadScript();
    script = new ChessScript(path.toLocal8Bit(), this);
    widgetHistory->pushHistory("Script Loaded");
    connect(script, SIGNAL(finished()), this, SLOT(onScriptFinish()));
    connect(script, SIGNAL(startChess(QString, QString)), this, SLOT(onScriptStartChess(QString, QString)));
    connect(script, SIGNAL(loadEngine(bool, QString)), this, SLOT(onScriptLoadEngine(bool, QString)));
    connect(script, SIGNAL(error(QString)), this, SLOT(onScriptError(QString)));
    script->start();
}

void MainWidget::unloadScript()
{
    if(script)
    {
        widgetHistory->pushHistory("Script Terminated");
        disconnect(script, 0, 0, 0);
        script->terminate();
        script->wait();
        delete script;
        script=0;
    }
}

void MainWidget::setGuide(bool enable)
{
    widgetChessBoard->setGuide(enable);
}

void MainWidget::loadEngineBlack()
{
    const QString& path=QFileDialog::getOpenFileName(this, "Open Executable...", "",
                                                     "Chess Engine (*.exe);;All Files (*)");
    if(!path.isNull())
        loadEngineBlack(path);
}

void MainWidget::loadEngineWhite()
{
    const QString& path=QFileDialog::getOpenFileName(this, "Open Executable...", "",
                                                     "Chess Engine (*.exe);;All Files (*)");
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
    connect(engineBlack, SIGNAL(engineLine(QByteArray)), this, SLOT(onEngineBlackLog(QByteArray)));
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
    connect(engineWhite, SIGNAL(engineLine(QByteArray)), this, SLOT(onEngineWhiteLog(QByteArray)));
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

void MainWidget::showLogWhite()
{
    if (widgetLogWhite->isOpen())
        widgetLogWhite->close();
    else
        widgetLogWhite->show();
}

void MainWidget::showLogBlack()
{
    if (widgetLogBlack->isOpen())
        widgetLogBlack->close();
    else
        widgetLogBlack->show();
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
    stopGame();
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

void MainWidget::onEngineBlackLog(QByteArray line)
{
    widgetLogBlack->pushLine(QString::fromLocal8Bit(line));
}

void MainWidget::onEngineWhiteLog(QByteArray line)
{
    widgetLogWhite->pushLine(QString::fromLocal8Bit(line));
}

void MainWidget::onScriptLoadEngine(bool isFirst, QString filename)
{
    if(isFirst)loadEngineBlack(filename);
    else loadEngineWhite(filename);
    if(script)
        script->resume();
}

void MainWidget::onScriptStartChess(QString paramFirst, QString paramSecond)
{
    restartGame(paramFirst, paramSecond);
}

void MainWidget::onScriptFinish()
{
    widgetHistory->pushHistory("Script Finish");
    unloadScript();
}

void MainWidget::onScriptError(QString errorLine)
{
    widgetHistory->pushHistory(errorLine, 1);
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

void MainWidget::doLoadScript()
{
    if(script)
    {
        unloadScript();
    }
    else
    {
        loadScript();
    }
}

bool MainWidget::doMove(int x, int y)
{
    if(!chessboard->checkMove(x, y, playerBlack))
        return false;
    widgetChessBoard->doChess(x, y, playerBlack);
    if(!script)
        widgetHistory->pushHistory(QString().sprintf("%c%d", 'A'+y, x), moveCount+1, gameCount);
    if(chessboard->checkFinished(!playerBlack))
    {
        stopGame();
        widgetHistory->pushHistory(QString("%0 Win").arg(playerBlack?"BLACK":"WHITE"));
        if(script)
        {
            script->increaseWinCount(playerBlack, chessboard->getMoveNum(playerBlack)*2+1);
            script->resume();
        }
        else
        {
            QMessageBox::information(this, "Game Over",
                                     QString("Winner is %0 !").arg(playerBlack?"BLACK":"WHITE"));
        }
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
