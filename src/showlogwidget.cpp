#include "showlogwidget.h"

ShowLogWidget::ShowLogWidget(const QString& logTitle, QWidget *parent) : QDialog(parent)
{
    resize(250, 300);
    setMinimumSize(250, 300);
    setMaximumWidth(250);

    titleText = new QLabel(logTitle, this);
    titleText->setGeometry(10, 3, 250, 20);
    mainText = new QPlainTextEdit("editddeidiede\nesavioasnvao", this);
    mainText->setGeometry(10, 25, 230, height() - 35);
    mainText->setReadOnly(true);
}

void ShowLogWidget::pushLine(const QString& newLogInfo)
{
    QString history = mainText->toPlainText() + newLogInfo + "\n";
    mainText->setPlainText(history);
}

void ShowLogWidget::pushText(const QString & newLogInfo)
{
    QString history = mainText->toPlainText() + newLogInfo;
    mainText->setPlainText(history);
}

void ShowLogWidget::paintEvent(QPaintEvent *)
{
    mainText->setGeometry(10, 25, 230, height() - 35);
}
