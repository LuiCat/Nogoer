#include "historywidget.h"

#include <QApplication>
#include <QListView>
#include <QScrollBar>

HistoryWidget::HistoryWidget(QWidget* parent)
    :QWidget(parent)
{
    item=new HistoryItem(this);
    list=new HistoryList(this);
    view=new QListView(this);

    view->setItemDelegate(item);
    view->setModel(list);
    view->setSpacing(2);
    view->verticalScrollBar()->setStyleSheet(
                "QScrollBar:vertical {margin:0px 0px 0px 0px; background-color:rgb(255, 255, 255, 0); width:8px;}\
                 QScrollBar::handle:vertical {background-color: rgb(0, 0, 0, 50); width:8px; border-radius: 4px;}\
                 QScrollBar::sub-line:vertical {subcontrol-origin:margin; height:0px;}\
                 QScrollBar::add-line:vertical {subcontrol-origin:margin; height:0px;}");

    for(int i=0;i<123;++i)
        list->append(QString("Fuck %1 times").arg(i));


}

void HistoryWidget::resizeEvent(QResizeEvent*)
{
    view->setGeometry(rect().marginsRemoved(QMargins(10, 10, 10, 10)));
}

