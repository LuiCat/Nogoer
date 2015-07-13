#ifndef SHOWLOGWIDGET_H
#define SHOWLOGWIDGET_H

#include <QDialog>
#include <QWidget>
#include <QLabel>

class ShowLogWidget : public QDialog
{
    Q_OBJECT
public:
    explicit ShowLogWidget(QString logTitle, QWidget *parent = 0);

signals:

public slots:

private:
    QLabel *mainText;
};

#endif // SHOWLOGWIDGET_H
