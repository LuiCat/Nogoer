#include "mainwidget.h"

#include <QKeyEvent>

MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent)
{    
    setMinimumSize(800, 480);

    widgetChessBoard = new ChessBoardWidget(0, this);
    widgetChessBoard->setGeometry();

}

MainWidget::~MainWidget()
{

}

void MainWidget::resizeEvent(QResizeEvent*)
{

}
