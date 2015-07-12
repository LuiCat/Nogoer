#include "historyitem.h"

#include <QPainter>

HistoryItem::HistoryItem(QObject* parent)
    :QAbstractItemDelegate(parent)
    ,cellHeight(30)
{

}

void HistoryItem::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    QMap<QString, QVariant> map(index.data().toMap());
    painter->drawRect(option.rect.marginsRemoved(QMargins(2, 2, 2, 2)));
    painter->drawText(option.rect.marginsRemoved(QMargins(2, 2, 2, 2)),
                       Qt::AlignCenter, map.value("text").toString());
}

QSize HistoryItem::sizeHint(const QStyleOptionViewItem& option, const QModelIndex&) const
{
    return QSize(option.rect.width(), cellHeight);
}
