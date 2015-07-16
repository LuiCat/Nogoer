#include "showlogwidget.h"

#include <QPushButton>
#include <QScrollBar>

ShowLogWidget::ShowLogWidget(const QString& logTitle, QWidget *parent) : QDialog(parent)
{
    resize(250, 300);
    setMinimumSize(250, 300);
    setMaximumWidth(250);

    widgetOpen = false;
    titleText = new QLabel(logTitle, this);
    titleText->setGeometry(10, 3, 250, 20);
    mainText = new QPlainTextEdit("", this);
    mainText->setGeometry(10, 25, 230, height() - 35);
    mainText->setReadOnly(true);
    clearButton = new QPushButton("Clear", this);
    clearButton->setGeometry(180, 3, 60, 20);

    connect(clearButton, SIGNAL(clicked()), this, SLOT(clearText()));
}

void ShowLogWidget::pushLine(const QString& newLogInfo)
{
    QString history = mainText->toPlainText() + newLogInfo + "\n";
    mainText->setPlainText(history.right(20000));
    mainText->verticalScrollBar()->triggerAction(QAbstractSlider::SliderToMaximum);
}

void ShowLogWidget::pushText(const QString & newLogInfo)
{
    QString history = mainText->toPlainText() + newLogInfo;
    mainText->setPlainText(history.right(20000));
    mainText->verticalScrollBar()->triggerAction(QAbstractSlider::SliderToMaximum);
}

bool ShowLogWidget::isOpen()
{
    return widgetOpen;
}

void ShowLogWidget::clearText()
{
    mainText->clear();
}

void ShowLogWidget::paintEvent(QPaintEvent *)
{
    mainText->setGeometry(10, 25, 230, height() - 35);
}

void ShowLogWidget::showEvent(QShowEvent *)
{
    widgetOpen = true;
}

void ShowLogWidget::closeEvent(QCloseEvent *)
{
    widgetOpen = false;
}
