#include "mainwidget.h"

#include <QKeyEvent>
#include <QPainter>
#include <QtWidgets/QMessageBox>

int MainWidget::sideMinimumWidth = 160;
int MainWidget::controlPanelHeight = 100;

MainWidget::MainWidget(QWidget *parent)
    :QWidget(parent)
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

    chessboard = new ChessBoard(this);
    ++gameCount;
    historyBoard.insert(gameCount, chessboard);
    widgetChessBoard->setChessBoard(chessboard);

}

MainWidget::~MainWidget()
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
    ++moveCount;
    playerBlack=!playerBlack;
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
        QMessageBox::information(this, "Game Over", QString("Winner is %0 !").arg(playerBlack?"BLACK":"WHITE"));
    }
    return true;
}
