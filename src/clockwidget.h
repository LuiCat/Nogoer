#ifndef CLOCKWIDGET_H
#define CLOCKWIDGET_H

#include <QTime>
#include <QTimer>
#include <QWidget>

class ClockWidget : public QWidget
{
    Q_OBJECT
private:
    QTime time;
    QTimer timer;
    int allPauseTime, pauseTime, nowTime;
    bool isStop, isClear, thisStart;

    QString strTime();
    int getNowTime();
public:
    explicit ClockWidget(QWidget *parent = 0);
    void timeStart();
    void timeClear();
    void timeStop();
signals:

public slots:
    void nextSecond();
    // QWidget interface
protected:
    void paintEvent(QPaintEvent *);
    void resizeEvent(QResizeEvent *);
    // QWidget interface
};

#endif // CLOCKWIDGET_H
