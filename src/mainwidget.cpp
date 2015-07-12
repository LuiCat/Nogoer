#include "mainwidget.h"

#include <QKeyEvent>
#include <QPainter>
#include <QtWidgets/QMessageBox>

int MainWidget::sideMinimumWidth = 160;

MainWidget::MainWidget(QWidget *parent)
    :QWidget(parent)
    ,moveCount(0)
    ,playerBlack(true)
{    
    setMinimumSize(800, 480);

    chessboard=new ChessBoard(this);
    widgetChessBoard = new ChessBoardWidget(chessboard, this);

    connect(widgetChessBoard, SIGNAL(clickGrid(int, int)), this, SLOT(doPlayerMove(int, int)));

}

MainWidget::~MainWidget()
{

}

void MainWidget::resizeEvent(QResizeEvent*)
{
    int widthCB, heightCB;
    int yCB;
    int widthSide;

    widthCB=width()-sideMinimumWidth*2;
    heightCB=(height()>widthCB?widthCB:(widthCB=height()));
    widthSide=(width()-widthCB)/2;
    yCB=(height()-heightCB)/2;

    widgetChessBoard->setGeometry(widthSide, yCB, widthCB, heightCB);

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

bool MainWidget::doMove(int x, int y)
{
    if(!chessboard->checkMove(x, y, playerBlack))
        return false;
    widgetChessBoard->doChess(x, y, playerBlack);
    if(chessboard->checkFinished(!playerBlack))
    {
        QMessageBox::information(this, "Game Over", QString("Winner is %0 !").arg(playerBlack?"BLACK":"WHITE"));
    }
    return true;
}
