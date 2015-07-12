#include "historywidget.h"

#include <QListView>

HistoryWidget::HistoryWidget(QWidget* parent)
    :QWidget(parent)
{
    item=new HistoryItem(this);
    list=new HistoryList(this);
    view=new QListView(this);

    view->setItemDelegate(item);
    view->setModel(list);
    view->setSpacing(2);

    for(int i=0;i<123;++i)
        list->append(QString("Fuck %1 times").arg(i));

}

void HistoryWidget::resizeEvent(QResizeEvent*)
{
    view->setGeometry(rect().marginsRemoved(QMargins(10, 10, 10, 10)));
}

