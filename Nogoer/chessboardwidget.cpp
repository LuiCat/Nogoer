#include "chessboardwidget.h"

ChessBoardWidget::ChessBoardWidget(ChessBoard* chessboard, QWidget *parent)
    :QWidget(parent)
    ,board(chessboard)
{
    if(!board)
        board=new ChessBoard(this);

    setMinimumSize(200, 200);

}

void ChessBoardWidget::paintEvent(QPaintEvent*)
{

}

