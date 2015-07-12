#ifndef HISTORYWIDGET_H
#define HISTORYWIDGET_H

#include "historylist.h"
#include "historyitem.h"

#include <QWidget>
#include <QListView>

class HistoryWidget : public QWidget
{
    Q_OBJECT

public:

    explicit HistoryWidget(QWidget* parent=0);

protected:

    void resizeEvent(QResizeEvent*);

private:

    QListView* view;
    HistoryList* list;
    HistoryItem* item;

};

#endif // HISTORYWIDGET_H
