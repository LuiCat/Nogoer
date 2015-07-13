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
    view->setSpacing(1);
    view->setSelectionMode(QAbstractItemView::SingleSelection);
    view->verticalScrollBar()->setStyleSheet(
            "QScrollBar:vertical {margin:0px 0px 0px 0px; background-color:rgb(0, 0, 0, 10);\
                                  float:right; width:6px; border-radius:3px;}\
             QScrollBar::handle:vertical {background-color:rgb(0, 0, 0, 50); width:6px; border-radius:3px;}\
             QScrollBar::sub-line:vertical {subcontrol-origin:margin; height:0px;}\
             QScrollBar::add-line:vertical {subcontrol-origin:margin; height:0px;}");

    connect(view, SIGNAL(clicked(QModelIndex)), this, SLOT(onSelectItem(QModelIndex)));

}

void HistoryWidget::pushHistory(const QString& text, int stepNum, int gameNum)
{
    list->append(text, stepNum, gameNum);
    view->scrollToBottom();
}

void HistoryWidget::resizeEvent(QResizeEvent*)
{
    view->setGeometry(rect().marginsRemoved(QMargins(10, 10, 10, 10)));
}

void HistoryWidget::onSelectItem(QModelIndex index)
{
    const HistoryList::ListData& data=list->getHistoryData(index.row());
    if(data.game>0&&data.step>0)
    {
        emit showHistory(data.game, data.step);
    }
}

