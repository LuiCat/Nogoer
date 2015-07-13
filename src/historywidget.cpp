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
    view->setSelectionMode(QAbstractItemView::SingleSelection);
    view->verticalScrollBar()->setStyleSheet(
            "QScrollBar:vertical {margin:0px 0px 0px 0px; background-color:rgb(0, 0, 0, 10);\
                                  float:right; width:6px; border-radius:3px;}\
             QScrollBar::handle:vertical {background-color:rgb(0, 0, 0, 50); width:6px; border-radius:3px;}\
             QScrollBar::sub-line:vertical {subcontrol-origin:margin; height:0px;}\
             QScrollBar::add-line:vertical {subcontrol-origin:margin; height:0px;}");

    list->append("Infomation");
    list->append("Warning!!!", 1);
    for(int i=1;i<222;++i)
        list->append(QString("Shen %0 Hao").arg(i), i, 1);

    connect(view, SIGNAL(clicked(QModelIndex)), this, SLOT(onMousePress));

}

void HistoryWidget::resizeEvent(QResizeEvent*)
{
    view->setGeometry(rect().marginsRemoved(QMargins(10, 10, 10, 10)));
}

void HistoryWidget::onMousePress(QModelIndex index)
{
    item->setCurrentIndex(index);
}

