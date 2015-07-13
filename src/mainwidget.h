#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QDialog>

#include "chessboardwidget.h"
#include "clockwidget.h"
#include "historywidget.h"
#include "controlwidget.h"

class MainWidget : public QWidget
{
    Q_OBJECT

public:

    explicit MainWidget(QWidget *parent = 0);
    ~MainWidget();

protected:

    void resizeEvent(QResizeEvent*);

protected slots:

    void doPlayerMove(int x, int y);

    void switchSide();

private:

    static int sideMinimumWidth;
    static int controlPanelHeight;

    int moveCount;
    bool playerBlack;

    ChessBoard* chessboard;
    ChessBoardWidget* widgetChessBoard;

    ClockWidget* widgetClockBlack;
    ClockWidget* widgetClockWhite;

    HistoryWidget* widgetHistory;

    ControlWidget* widgetControl;

    inline ClockWidget* getClockWidget(bool isBlack)
    {
        return isBlack?widgetClockBlack:widgetClockWhite;
    }

    bool doMove(int x, int y);

};


#endif // MAINWIDGET_H
