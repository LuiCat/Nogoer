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
    QPushButton* loadEngineButton;
    QPushButton* showLogButton;
    QString playerName;
    QString outString;
    int allPauseTime, pauseTime, nowTime;
    bool engineLoaded;
    bool isStart, isPause;

    QString strTime();
    int getNowTime();
public:
    explicit ClockWidget(QWidget *parent = 0);
    void timeStart();
    void timeClear();
    void timeStop();
    bool isLoadEngine();
    void setEngineState(bool isEngine, QString name=QString());
public slots:
    void setPlayerName(QString tString);
signals:
    void loadEngine();
    void unloadEngine();
    void showLog();
public slots:
    void nextSecond();
    void engineLoading();
protected:
    void paintEvent(QPaintEvent *);
    void resizeEvent(QResizeEvent *);
};

#endif // CLOCKWIDGET_H
