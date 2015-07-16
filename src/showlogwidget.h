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
    bool isOpen();
signals:

public slots:
    void clearText();
private:
    QLabel *titleText;
    QPlainTextEdit *mainText;
    QPushButton *clearButton;
    bool widgetOpen;

    // QWidget interface
protected:
    void paintEvent(QPaintEvent *);
    void showEvent(QShowEvent *);
    void closeEvent(QCloseEvent *);
    // QWidget interface

};

#endif // SHOWLOGWIDGET_H
