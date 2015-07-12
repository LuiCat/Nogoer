#include "chessboardwidget.h"

#include "commondef.h"

#include <QPainter>
#include <QImage>

double ChessBoardWidget::gridAreaSize = 0.8;
double ChessBoardWidget::gridBorderSpace = 5.0;
int ChessBoardWidget::gridDots[5][2] = {{2, 2}, {2, 6}, {4, 4}, {6, 2}, {6, 6}};

ChessBoardWidget::ChessBoardWidget(ChessBoard* chessboard, QWidget *parent)
    :QWidget(parent)
    ,board(chessboard)
    ,currentX(-1)
    ,currentY(-1)
{
    if(!board)
        board=new ChessBoard(this);

    imgBoard.load("data/chessboard.png");
    imgChessBlack[0].load("data/chessblack.png");
    imgChessWhite[0].load("data/chesswhite.png");
    imgChessBlack[1].load("data/chessblacks.png");
    imgChessWhite[1].load("data/chesswhites.png");

    penLine.setWidth(2);
    penDot.setWidth(9);
    penDot.setCapStyle(Qt::RoundCap);

    setMinimumSize(200, 200);

}

void ChessBoardWidget::paintEvent(QPaintEvent*)
{
    QPainter painter(this);

    painter.setRenderHint(QPainter::Antialiasing, true);

    painter.drawImage(rect(), imgBoard);

    painter.save();
    drawGrid(painter);
    painter.restore();

    painter.save();
    drawChess(painter);
    painter.restore();

}

void ChessBoardWidget::resizeEvent(QResizeEvent*)
{
    gridTop=height()*(1-gridAreaSize)*0.5;
    gridBottom=height()*(1+gridAreaSize)*0.5;
    gridLeft=width()*(1-gridAreaSize)*0.5;
    gridRight=width()*(1+gridAreaSize)*0.5;
    gridHeight=height()*gridAreaSize;
    gridWidth=width()*gridAreaSize;
    cellHeight=gridHeight/(B_HEIGHT-1);
    cellWidth=gridWidth/(B_WIDTH-1);

    update();
}

void ChessBoardWidget::drawGrid(QPainter& painter)
{
    painter.translate(gridLeft, gridTop);

    painter.setPen(penLine);

    for(int i=0;i<B_HEIGHT;++i)
        painter.drawLine(QPointF(0.0, i*cellHeight), QPointF(gridWidth, i*cellHeight));
    for(int i=0;i<B_WIDTH;++i)
        painter.drawLine(QPointF(i*cellWidth, 0.0), QPointF(i*cellWidth, gridHeight));

    painter.drawRect(QRectF(QPointF(-gridBorderSpace, -gridBorderSpace),
                            QPointF(gridWidth+gridBorderSpace, gridHeight+gridBorderSpace)));

    painter.setPen(penDot);

    for(int i=0;i<5;++i)
        painter.drawPoint(QPointF(gridDots[i][0]*cellWidth, gridDots[i][1]*cellHeight));

}

void ChessBoardWidget::drawChess(QPainter& painter)
{
    if(!board)
        return;

    int i,j;
    bool flag;

    painter.translate(gridLeft-cellWidth*0.5, gridTop-cellHeight*0.5);

    for(i=0;i<B_HEIGHT;++i)
    {
        for(j=0;j<B_WIDTH;++j)
        {
            if(!board->isChess(i, j))
                continue;
            flag=(i==currentY&&j==currentX);
            painter.save();
            painter.translate(cellWidth*j, cellHeight*i);
            painter.drawImage(QRectF(0, 0, cellWidth, cellHeight),
                              board->getGrid(i, j)==ChessBoard::black?imgChessBlack[flag]:imgChessWhite[flag]);
            painter.restore();
        }
    }

}

