#include "controlwidget.h"

#include <QPainter>

ControlWidget::ControlWidget(QWidget *parent) : QWidget(parent)
{
    gameStart = false;
    gameButton = new QPushButton("Start", this);
    scriptButton = new QPushButton("Script", this);
    showCBInfoBox = new QCheckBox("Show Guide", this);

    connect(gameButton, SIGNAL(clicked()), this, SLOT(gameControl()));
    connect(scriptButton, SIGNAL(clicked()), this, SIGNAL(loadScript()));
    connect(showCBInfoBox, SIGNAL(toggled(bool)), this, SIGNAL(setGuide(bool)));
}

bool ControlWidget::isGameStart()
{
    return gameStart;
}

void ControlWidget::setGameState(bool gameStart)
{
    this->gameStart = gameStart;
    if (gameStart)
        gameButton->setText("Stop");
    else
        gameButton->setText("Start");
}

void ControlWidget::gameControl()
{
    if (!gameStart)
    {
        emit startGame();
    }
    else
    {
        emit stopGame();
    }
}

void ControlWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    showCBInfoBox->setGeometry((width() - 100) / 2, (height() - 25) / 2 + 30, 100, 25);
    gameButton->setGeometry((width() - 100) / 2, (height() - 25) / 2 - 30, 100, 25);
    scriptButton->setGeometry((width() - 100) / 2, (height() - 25) / 2, 100, 25);/*
    painter.setPen("brown");
    painter.drawRect(10, 10, width() - 20, height() - 20);
    painter.drawRect(15, 15, width() - 30, height() - 30);*/
}

void ControlWidget::resizeEvent(QResizeEvent *)
{
    update();
}
