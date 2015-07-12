#include "clockwidget.h"

#include <QPainter>
#include <QDebug>

ClockWidget::ClockWidget(QWidget *parent) : QWidget(parent)
{
    allPauseTime = pauseTime = 0;
    thisStart = isStop = isClear = false;

    timer.setInterval(16);
    connect(&timer, SIGNAL(timeout()), this, SLOT(nextSecond()));

}

void ClockWidget::timeStart()
{
    isStop = isClear = false;
    if (!thisStart)
    {
        time.start();
        thisStart = true;
    }
    timer.start();
    allPauseTime += time.elapsed() - pauseTime;
}

void ClockWidget::timeStop()
{
    pauseTime = time.elapsed();
    timer.stop();
    nowTime = getNowTime();
    isStop = true;
}

void ClockWidget::timeClear()
{
    allPauseTime = 0;
    pauseTime = 0;
    thisStart = false;
    isClear = true;
    update();
}

QString ClockWidget::strTime()
{
    QString tmpTim;
    int tmpTime;
    int showH, showM, showS, showMs;
    tmpTime = getNowTime();

    showH = tmpTime / 1000 / 60 / 60;
    showM = tmpTime / 1000 / 60 - showH * 60;
    showS = tmpTime / 1000 - showH * 60 * 60 - showM * 60;
    showMs = tmpTime - showH * 60 * 60 * 1000 - showM * 60 * 1000 - showS * 1000;

    tmpTim.sprintf("%02d:%02d:%02d:%03d", showH, showM, showS, showMs);

    return tmpTim;
}

int ClockWidget::getNowTime()
{
    if (isClear)
        return 0;
    if (isStop)
        return nowTime;
    return time.elapsed() - allPauseTime;
}

void ClockWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.drawText(QPoint(15, 20), strTime());
    painter.drawRect(0, 0, 100, 100);
}

void ClockWidget::resizeEvent(QResizeEvent *)
{
    update();
}

void ClockWidget::nextSecond()
{
    update();
}
