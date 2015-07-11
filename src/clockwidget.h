#ifndef CLOCKWIDGET_H
#define CLOCKWIDGET_H

#include <QTime>
#include <QTimer>
#include <QWidget>

class ClockWidget : public QWidget
{
    QTime *time;
    QTimer *timer;
    Q_OBJECT
public:
    explicit ClockWidget(QWidget *parent = 0);

signals:

public slots:
    void nextUpdate();
    // QWidget interface
protected:
    void paintEvent(QPaintEvent *);
};

#endif // CLOCKWIDGET_H
