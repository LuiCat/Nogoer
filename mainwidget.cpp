#include "mainwidget.h"

#include <QKeyEvent>

MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent)
{
    setMinimumSize(800, 480);
}

MainWidget::~MainWidget()
{

}

void MainWidget::keyPressEvent(QKeyEvent* e)
{
    if(e->key()==Qt::Key_Escape)
    {
        close();
    }
    else
    {
        QWidget::keyPressEvent(e);
    }
}
