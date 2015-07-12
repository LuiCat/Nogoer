#ifndef CLOCKWIDGET_H
#define CLOCKWIDGET_H

#include <QPushButton>
#include <QTime>
#include <QTimer>
#include <QWidget>

class ClockWidget : public QWidget
{
    Q_OBJECT
private:
    QTime time;
    QTimer timer;
    QPushButton *aboutEngine;
    QString playerName;
    int allPauseTime, pauseTime, nowTime;
    bool isStop, isClear, thisStart;
    bool engineLoaded;

    QString strTime();
    int getNowTime();
public:
    explicit ClockWidget(QWidget *parent = 0);
    void timeStart();
    void timeClear();
    void timeStop();
    bool isLoadEngine();
    void setEngineState(bool);
    void setPlayerName(QString tString = "Robot");
    // ClockWidget interface
signals:
    void loadEngine();
    void unloadEngine();
public slots:
    void nextSecond();
    void engineLoading();
protected:
    void paintEvent(QPaintEvent *);
    void resizeEvent(QResizeEvent *);
};

#endif // CLOCKWIDGET_H
