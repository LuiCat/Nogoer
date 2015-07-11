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

    ChessBoard* board;

    QImage imgBG;

    QPen penLine;
    QPen penDot;

};

#endif // CHESSBOARDWIDGET_H
