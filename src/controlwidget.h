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
    bool isGameStart();
    void setGameState(bool);
signals:
    void startGame();
    void stopGame();
    void loadScript();
    void setGuide(bool);

public slots:
    void gameControl();

private:
    QPushButton *gameButton, *scriptButton;
    QCheckBox *showCBInfoBox;
    bool gameStart;

protected:
    void paintEvent(QPaintEvent *);
    void resizeEvent(QResizeEvent *);

};

#endif // CONTROLWIDGET_H
