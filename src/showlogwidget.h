#ifndef SHOWLOGWIDGET_H
#define SHOWLOGWIDGET_H

#include <QDialog>
#include <QWidget>
#include <QLabel>
#include <QPlainTextEdit>

class ShowLogWidget : public QDialog
{
    Q_OBJECT
public:
    explicit ShowLogWidget(const QString&, QWidget *parent = 0);
    void pushLine(const QString&);
    void pushText(const QString&);
signals:

public slots:

private:
    QLabel *titleText;
    QPlainTextEdit *mainText;

    // QWidget interface
protected:
    void paintEvent(QPaintEvent *);
};

#endif // SHOWLOGWIDGET_H
