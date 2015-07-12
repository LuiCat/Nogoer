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

    ChessBoard* getChessBoard() const;

    /// Show a chessboard to screen not making modifies to origin chessboard.
    /// Restore with empty parameter or set to null.
    void showChessBoard(ChessBoard* chessboard=0);

public slots:

    void doChess(int x, int y, bool isBlack);

signals:

    void clickGrid(int x, int y);

protected:

    void paintEvent(QPaintEvent*);
    void resizeEvent(QResizeEvent*);

    void mousePressEvent(QMouseEvent* e);

private:

    static double gridAreaSize;
    static double gridBorderSpace;

    static int gridDots[5][2];

    ChessBoard* board;
    ChessBoard* showBoard;

    int currentX;
    int currentY;

    QImage imgBoard;
    QImage imgChessBlack[2];
    QImage imgChessWhite[2];

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
