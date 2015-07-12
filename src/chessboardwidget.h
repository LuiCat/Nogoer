#ifndef CHESSBOARDWIDGET_H
#define CHESSBOARDWIDGET_H

#include "chessboard.h"

#include <QWidget>
#include <QPen>

class ChessBoardWidget : public QWidget
{
    Q_OBJECT

public:

    explicit ChessBoardWidget(ChessBoard* chessboard=0, QWidget *parent=0);

protected:

    void paintEvent(QPaintEvent*);
    void resizeEvent(QResizeEvent*);

private:

    static double gridAreaSize;
    static double gridBorderSpace;

    static int gridDots[5][2];

    ChessBoard* board;

    QImage imgBoard;
    QImage imgChessBlack;
    QImage imgChessWhite;

    QPen penLine;
    QPen penDot;

    double gridTop;
    double gridBottom;
    double gridLeft;
    double gridRight;
    double gridHeight;
    double gridWidth;
    double cellHeight;
    double cellWidth;

    void drawGrid(QPainter& painter);
    void drawChess(QPainter& painter);

};

#endif // CHESSBOARDWIDGET_H
