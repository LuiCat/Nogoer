#include "clockwidget.h"

#include <QPainter>
#include <QDebug>

ClockWidget::ClockWidget(QWidget *parent) : QWidget(parent)
{
    setPlayerName("Player");
    allPauseTime = pauseTime = 0;
    thisStart = isStop = isClear = false;

    timer.setInterval(500);
    connect(&timer, SIGNAL(timeout()), this, SLOT(nextSecond()));

    loadEngineButton = new QPushButton("LoadEngine", this);
    connect(loadEngineButton, SIGNAL(clicked()), this, SLOT(engineLoading()));

    showLogButton = new QPushButton("ShowLog", this);
    connect(showLogButton, SIGNAL(clicked()), this, SIGNAL(showLog()));
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
    update();
}

void ClockWidget::timeClear()
{
    allPauseTime = 0;
    pauseTime = 0;
    thisStart = false;
    isClear = true;
    update();
}

bool ClockWidget::isLoadEngine()
{
    return engineLoaded;
}

void ClockWidget::setEngineState(bool isEngine, QString name)
{
    engineLoaded = isEngine;
    if (isEngine)
    {
        playerName = name;
        loadEngineButton->setText("UnloadEngine");
    }
    else
    {
        loadEngineButton->setText("LoadEngine");
        playerName = QString("Player");
    }
    update();
}

void ClockWidget::setPlayerName(QString tString)
{
    playerName = tString;
}

QString ClockWidget::strTime()
{
    QString tmpTim;
    int tmpTime;
    int showM, showS, showMs;
    tmpTime = getNowTime();

    showM = tmpTime / 1000 / 60;
    showS = tmpTime / 1000 - showM * 60;
    showMs = tmpTime - showM * 60 * 1000 - showS * 1000;

    tmpTim.sprintf("%d:%02d:%03d", showM, showS, showMs);

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
    outString = strTime();
    loadEngineButton->setGeometry((width() - 110) / 2, (height() - 20) / 2 + 20, 110, 20);
    showLogButton->setGeometry((width() - 80) / 2, (height() - 20) / 2 + 45, 80, 20);
    painter.drawText(QPoint((width() - playerName.length() * 7) / 2, (height()) / 2), playerName);
    painter.drawText(QPoint((width() - outString.length() * 7) / 2, (height() - 50) / 2), outString);
    painter.setPen("brown");
    painter.drawRect(10, 10, width() - 20, height() - 20);
    painter.drawRect(15, 15, width() - 30, height() - 30);
}

void ClockWidget::resizeEvent(QResizeEvent *)
{
    update();
}

void ClockWidget::nextSecond()
{
    update();
}

void ClockWidget::engineLoading()
{
    if (engineLoaded)
    {
        emit unloadEngine();
    }
    else
    {
        emit loadEngine();
    }
}
