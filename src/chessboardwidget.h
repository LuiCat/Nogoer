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

    void setChessBoard(ChessBoard* chessboard);

    /// Show a chessboard to screen not making modifies to origin chessboard.
    /// Restore with empty parameter or set to null.
    void showChessBoard(ChessBoard* chessboard=0);

    /// Show history moves until given steps, set to 0 to clear history.
    void showHistory(int step=0);

public slots:

    void doChess(int x, int y, bool isBlack);
    void setHint(bool enable);
    void setGuide(bool enable);

signals:

    void clickGrid(int x, int y);

protected:

    void paintEvent(QPaintEvent* e);
    void resizeEvent(QResizeEvent* e);

    void mousePressEvent(QMouseEvent* e);
    void mouseMoveEvent(QMouseEvent* e);
    void focusOutEvent(QFocusEvent* e);

private:

    static double gridAreaSize;
    static double gridBorderSpace;

    static int gridDots[5][2];

    ChessBoard* board;
    ChessBoard* showBoard;

    int currentX;
    int currentY;

    int mouseX;
    int mouseY;

    int historyStep;
    bool isNextBlack;

    bool showHint;
    bool showGuide;

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
