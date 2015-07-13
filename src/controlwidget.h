#ifndef CONTROLWIDGET_H
#define CONTROLWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QCheckBox>

class ControlWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ControlWidget(QWidget *parent = 0);

signals:
    void startGame();
    void stopGame();
    void loadScript();
    void setHint(bool);
public slots:
private:
    QPushButton *gameButton, *scriptButton;
    QCheckBox *showCBInfoBox;

    // QWidget interface
protected:
    void paintEvent(QPaintEvent *);
};

#endif // CONTROLWIDGET_H
