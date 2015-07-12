#ifndef HISTORYITEM_H
#define HISTORYITEM_H

#include <QAbstractItemDelegate>

class HistoryItem : public QAbstractItemDelegate
{
    Q_OBJECT

public:

    explicit HistoryItem(QObject* parent=0);

    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;
    QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const;

private:

    int cellHeight;

};

#endif // HISTORYITEM_H
