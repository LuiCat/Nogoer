#include "mainwidget.h"

#include <QKeyEvent>
#include <QPainter>

int MainWidget::sideMinimumWidth = 160;

MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent)
{    
    setMinimumSize(800, 480);

    widgetChessBoard = new ChessBoardWidget(0, this);

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
