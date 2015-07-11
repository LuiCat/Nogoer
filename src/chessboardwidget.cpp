#include "chessboardwidget.h"

#include <QPainter>
#include <QImage>

ChessBoardWidget::ChessBoardWidget(ChessBoard* chessboard, QWidget *parent)
    :QWidget(parent)
    ,board(chessboard)
{
    if(!board)
        board=new ChessBoard(this);

    imgBG.load("data/chessboard.png");

    setMinimumSize(200, 200);

}

void ChessBoardWidget::paintEvent(QPaintEvent*)
{
    QPainter painter;

    painter.drawImage(size(), imgBG);

}

