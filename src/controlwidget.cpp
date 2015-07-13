#include "controlwidget.h"

ControlWidget::ControlWidget(QWidget *parent) : QWidget(parent)
{
    gameButton = new QPushButton("Start", this);
    showCBInfoBox = new QCheckBox("ShowChessBoardInfo", this);
    showCBInfoBox->setGeometry(10, 10, 100, 30);
}

void ControlWidget::paintEvent(QPaintEvent *)
{

}

