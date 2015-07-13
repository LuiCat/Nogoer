#include "clockwidget.h"

#include <QPainter>
#include <QDebug>

ClockWidget::ClockWidget(QWidget *parent) : QWidget(parent)
{
    setPlayerName("Player");
    engineLoaded = false;
    allPauseTime = pauseTime = 0;
    isStart = false;
    isPause = true;

    timer.setInterval(73);
    connect(&timer, SIGNAL(timeout()), this, SLOT(nextSecond()));

    loadEngineButton = new QPushButton("LoadEngine", this);
    connect(loadEngineButton, SIGNAL(clicked()), this, SLOT(engineLoading()));

    showLogButton = new QPushButton("ShowLog", this);
    connect(showLogButton, SIGNAL(clicked()), this, SIGNAL(showLog()));
}

void ClockWidget::timeStart()
{
    if (!isStart)
    {
        time.start();
        isStart = true;
    }
    if (!isPause)
    {
        return;
    }
    isPause = false;
    allPauseTime += time.elapsed() - pauseTime;
    timer.start();
    update();
}

void ClockWidget::timeStop()
{
    if (!isStart || isPause)
    {
        return;
    }
    lastTime = getNowTime();
    isPause = true;
    pauseTime = time.elapsed();
    timer.stop();
    update();
}

void ClockWidget::timeClear()
{
    isStart = false;
    isPause = true;
    allPauseTime = pauseTime = 0;
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
    update();
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
    if (!isStart)
    {
        return 0;
    }
    if (isPause)
    {
        return lastTime;
    }
    return time.elapsed() - allPauseTime;
}

void ClockWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    outString = strTime();
    loadEngineButton->setGeometry((width() - 110) / 2, (height() - 20) / 2 + 30, 110, 25);
    showLogButton->setGeometry((width() - 80) / 2, (height() - 20) / 2 + 60, 80, 25);
    painter.drawText(QRectF(0, height() * 0.5, width(), 12), Qt::AlignCenter, playerName);
    painter.setFont(QFont("arial", 21));
    painter.drawText(QRectF(0, height() * 0.5 - 60, width(), 50), Qt::AlignHCenter|Qt::AlignBottom, outString);
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
