#include "chessboardwidget.h"

#include "commondef.h"

#include <QPainter>
#include <QImage>
#include <QKeyEvent>
#include <QDebug>

double ChessBoardWidget::gridAreaSize = 0.75;
double ChessBoardWidget::gridBorderSpace = 5.0;
int ChessBoardWidget::gridDots[5][2] = {{2, 2}, {2, 6}, {4, 4}, {6, 2}, {6, 6}};

ChessBoardWidget::ChessBoardWidget(ChessBoard* chessboard, QWidget *parent)
    :QWidget(parent)
    ,board(chessboard)
    ,showBoard(0)
    ,currentX(-1)
    ,currentY(-1)
    ,mouseX(-1)
    ,mouseY(-1)
    ,historyStep(0)
    ,isNextBlack(true)
    ,showHint(true)
    ,showGuide(false)
{

    showBoard=board;

    imgBoard.load("data/chessboard.png");
    imgChessBlack[0].load("data/chessblack.png");
    imgChessWhite[0].load("data/chesswhite.png");
    imgChessBlack[1].load("data/chessblacks.png");
    imgChessWhite[1].load("data/chesswhites.png");

    penLine.setWidth(2);
    penDot.setWidth(9);
    penDot.setCapStyle(Qt::RoundCap);

    setMinimumSize(200, 200);

    setMouseTracking(true);

}

ChessBoard* ChessBoardWidget::getChessBoard() const
{
    return board;
}

void ChessBoardWidget::setChessBoard(ChessBoard* chessboard)
{
    board=chessboard;
    if(!showBoard)
        showBoard=board;
    update();
}

void ChessBoardWidget::showChessBoard(ChessBoard* chessboard)
{
    showBoard=(chessboard?chessboard:board);
    update();
}

void ChessBoardWidget::showHistory(int step)
{
    historyStep=step;
    update();
}

void ChessBoardWidget::doChess(int x, int y, bool isBlack)
{
    board->doChess(x, y, isBlack);
    currentX=x;
    currentY=y;
    showChessBoard();
    isNextBlack=!isBlack;
}

void ChessBoardWidget::setHint(bool enable)
{
    showHint=enable;
    update();
}

void ChessBoardWidget::setGuide(bool enable)
{
    showGuide=enable;
    update();
}

void ChessBoardWidget::paintEvent(QPaintEvent*)
{
    QPainter painter(this);

    painter.setRenderHint(QPainter::Antialiasing, true);

    painter.drawImage(rect(), imgBoard);

    painter.save();
    drawGrid(painter);
    painter.restore();

    if(board)
    {
        painter.save();
        drawChess(painter);
        painter.restore();
    }

}

void ChessBoardWidget::resizeEvent(QResizeEvent* e)
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

    QWidget::resizeEvent(e);
}

void ChessBoardWidget::mousePressEvent(QMouseEvent* e)
{
    if(board&&!board->isFinished()&&(showBoard!=board||historyStep!=0))
    {
        showHistory();
        showChessBoard();
    }
    else if(e->button()==Qt::LeftButton)
    {
        int cx,cy;
        QPointF p=e->localPos();

        cx=qRound((p.x()-gridLeft)/cellWidth);
        cy=qRound((p.y()-gridTop)/cellHeight);

        emit clickGrid(cx, cy);
    }
    QWidget::mousePressEvent(e);
}

void ChessBoardWidget::mouseMoveEvent(QMouseEvent* e)
{
    QPointF p=e->localPos();
    int mx=qRound((p.x()-gridLeft)/cellWidth);
    int my=qRound((p.y()-gridTop)/cellHeight);
    if(mx!=mouseX||my!=mouseY)
    {
        mouseX=mx;
        mouseY=my;
        update();
    }
    QWidget::mouseMoveEvent(e);
}

void ChessBoardWidget::focusOutEvent(QFocusEvent* e)
{
    mouseX=-1;
    mouseY=-1;
    QWidget::focusOutEvent(e);
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

    painter.setFont(QFont("arial", 14));

    for(int i=0;i<B_WIDTH;++i)
        painter.drawText(QRectF(i*cellWidth-10, -cellHeight*0.5-15, 20, 20),
                         Qt::AlignCenter, QString("%0").arg(i));

    for(int i=0;i<B_HEIGHT;++i)
        painter.drawText(QRectF(-cellWidth*0.5-15, i*cellHeight-10, 20, 20),
                         Qt::AlignCenter, QString().sprintf("%c",'A'+i));

    for(int i=0;i<B_WIDTH;++i)
        painter.drawText(QRectF(i*cellWidth-10, gridHeight+cellHeight*0.5-5, 20, 20),
                         Qt::AlignCenter, QString("%0").arg(i));

    for(int i=0;i<B_HEIGHT;++i)
        painter.drawText(QRectF(gridWidth+cellWidth*0.5-5, i*cellHeight-10, 20, 20),
                         Qt::AlignCenter, QString().sprintf("%c",'A'+i));

}

void ChessBoardWidget::drawChess(QPainter& painter)
{
    if(!board)
        return;

    int i,j,step;
    bool flag;
    ChessBoard::ChessType type;

    painter.setFont(QFont("arial", cellHeight*0.35));
    painter.translate(gridLeft-cellWidth*0.5, gridTop-cellHeight*0.5);

    for(i=0;i<B_HEIGHT;++i)
    {
        for(j=0;j<B_WIDTH;++j)
        {
            type=board->getGrid(i, j);
            if(type==ChessBoard::invalid)
                continue;
            painter.save();
            painter.translate(cellWidth*i, cellHeight*j);
            if(type==ChessBoard::empty)
            {
                if(historyStep==0&&!board->isFinished()&&i==mouseX&&j==mouseY
                        &&showHint&&board->checkMove(i, j, isNextBlack))
                {
                    painter.setOpacity(0.3);
                    painter.drawImage(QRectF(0, 0, cellWidth, cellHeight),
                                      isNextBlack?imgChessBlack[0]:imgChessWhite[0]);
                }
            }
            else
            {
                step=board->getStep(i, j);
                flag=(i==currentX&&j==currentY);
                if(historyStep>0&&historyStep<step)
                    painter.setOpacity(0.2);
                painter.drawImage(QRectF(0, 0, cellWidth, cellHeight),
                                  type==ChessBoard::black?imgChessBlack[flag]:imgChessWhite[flag]);
                if(showGuide||historyStep>=step)
                {
                    painter.setPen(type==ChessBoard::black?"white":"black");
                    painter.drawText(QRectF(0, 0, cellWidth, cellHeight), Qt::AlignCenter,
                                     QString("%0").arg(step));
                }
            }
            painter.restore();
        }
    }

}
