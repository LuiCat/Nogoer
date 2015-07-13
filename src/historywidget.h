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

    void pushHistory(const QString& text, int stepNum=0, int gameNum=0);

protected:

    void resizeEvent(QResizeEvent*);

protected slots:

    void onSelectItem(QModelIndex index);

signals:

    void showHistory(int gameNum, int stepNum);

private:

    QListView* view;
    HistoryList* list;
    HistoryItem* item;

};

#endif // HISTORYWIDGET_H
