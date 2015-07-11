#include "clockwidget.h"

#include <QPainter>

ClockWidget::ClockWidget(QWidget *parent) : QWidget(parent)
{
    timer->setInterval(16);
    connect(timer, SIGNAL(timeout()), this, SLOT(nextUpdate()));
}

void ClockWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setPen(QPen("red"));
    painter.drawRect(5, 5, 100, 100);
}

void ClockWidget::nextUpdate()
{
    update();
}
