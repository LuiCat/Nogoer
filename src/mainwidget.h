#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QDialog>

#include "chessboardwidget.h"
#include "clockwidget.h"

class MainWidget : public QWidget
{
    Q_OBJECT

public:

    explicit MainWidget(QWidget *parent = 0);
    ~MainWidget();

protected:

    void resizeEvent(QResizeEvent*);

private:

    static int sideMinimumWidth;

    ChessBoardWidget* widgetChessBoard;

};


#endif // MAINWIDGET_H
