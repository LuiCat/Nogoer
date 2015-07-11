#ifndef CHESSBOARDWIDGET_H
#define CHESSBOARDWIDGET_H

#include "chessboard.h"

#include <QWidget>

class ChessBoardWidget : public QWidget
{
    Q_OBJECT

public:

    explicit ChessBoardWidget(ChessBoard* chessboard=0, QWidget *parent=0);

protected:

    void paintEvent(QPaintEvent*);

private:

    ChessBoard* board;

    QImage imgBG;

};

#endif // CHESSBOARDWIDGET_H
