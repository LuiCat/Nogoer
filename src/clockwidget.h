#ifndef CLOCKWIDGET_H
#define CLOCKWIDGET_H

#include <QWidget>

class ClockWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ClockWidget(QWidget *parent = 0);

signals:

public slots:

    // QWidget interface
protected:
    void paintEvent(QPaintEvent *);
};

#endif // CLOCKWIDGET_H
