#include "showlogwidget.h"

ShowLogWidget::ShowLogWidget(QString logTitle, QWidget *parent) : QDialog(parent)
{
    resize(250, 300);
    setMinimumSize(250, 300);
    setMaximumWidth(250);

    mainText = new QLabel(logTitle, this);
    mainText->setGeometry((width() - logTitle.length() * 7) / 2, 0, 250, 20);
}
