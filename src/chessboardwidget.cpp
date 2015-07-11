#include "chessboardwidget.h"

#include "commondef.h"

#include <QPainter>
#include <QImage>

double ChessBoardWidget::gridAreaSize = 0.8;
double ChessBoardWidget::gridBorderSpace = 5.0;

ChessBoardWidget::ChessBoardWidget(ChessBoard* chessboard, QWidget *parent)
    :QWidget(parent)
    ,board(chessboard)
{
    if(!board)
        board=new ChessBoard(this);

    imgBG.load("data/chessboard.png");

    penLine.setWidth(3);
    penDot.setWidth(12);

    setMinimumSize(200, 200);

}

void ChessBoardWidget::paintEvent(QPaintEvent*)
{
    QPainter painter(this);

    painter.setRenderHint(QPainter::Antialiasing, true);

    painter.drawImage(rect(), imgBG);

    painter.setPen(penLine);

    double gridTop=height()*(1-gridAreaSize)*0.5;
    double gridBottom=height()*(1+gridAreaSize)*0.5;
    double gridLeft=width()*(1-gridAreaSize)*0.5;
    double gridRight=width()*(1+gridAreaSize)*0.5;
    double cellHeight=height()*gridAreaSize/(B_HEIGHT-1);
    double cellWidth=width()*gridAreaSize/(B_WIDTH-1);

    for(int i=0;i<=B_HEIGHT-1;++i)
        painter.drawLine(QPointF(gridLeft, gridTop+i*cellHeight), QPointF(gridRight, gridTop+i*cellHeight));
    for(int i=0;i<=B_WIDTH-1;++i)
        painter.drawLine(QPointF(gridLeft+i*cellWidth, gridTop), QPointF(gridLeft+i*cellWidth, gridBottom));

    painter.drawRect(QRectF(QPointF(gridLeft-gridBorderSpace, gridTop-gridBorderSpace),
                            QPointF(gridRight+gridBorderSpace, gridBottom+gridBorderSpace)));

}

void ChessBoardWidget::resizeEvent(QResizeEvent*)
{
    update();
}

